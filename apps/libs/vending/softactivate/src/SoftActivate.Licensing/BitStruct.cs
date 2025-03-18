using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;

namespace SoftActivate.Licensing
{
    internal class BitStruct
    {
        public BitStruct()
        {
            fieldMap = new Dictionary<string, Field>();
        }

        public BitStruct(int size) : this()
        {
            bitStream = new BitStream(size);
        }

        public BitStruct(byte[] buf, int size) : this()
        {
            bitStream = new BitStream(buf, size);
        }

        public void Attach(byte[] buf, int bitSize)
        {
            bitStream = new BitStream(buf, bitSize);
        }

        public void Attach(BitStream bitStream)
        {
            this.bitStream = bitStream;
        }

        public void Detach()
        {
            bitStream = null;
        }

        public void AddField(string name, FieldType type, int size, int offset = -1)
        {
            if (offset == -1)
                offset = currentFieldOffset;

            if (type == FieldType.Date14 && size != 14)
                throw new Exception("Field '" + name + "' must have a size of 14 bits since it's a Date14 type");

            if (type == FieldType.Date16 && size != 16)
                throw new Exception("Field '" + name + "' must have a size of 16 bits since it's a Date16 type");

            if (type == FieldType.Date13 && size != 13)
                throw new Exception("Field '" + name + "' must have a size of 13 bits since it's a Date13 type");

            foreach (KeyValuePair<string, Field> entry in fieldMap)
            {
                if (!((offset + size - 1 < ((Field)entry.Value).Offset) || (((Field)entry.Value).Offset + ((Field)entry.Value).Size - 1 < offset)))
                    throw new Exception("Field '" + name + "' overlaps with field '" + entry.Key + "'");
            }

            fieldMap.Add(name, new Field(type, offset, size));

            currentFieldOffset += size;
        }

        public void DeleteField(string name)
        {
            if (fieldMap.ContainsKey(name))
            {
                fieldMap.Remove(name);
            }
        }

        public int MappedSize
        {
            get
            {
                int mappedSize = 0;
                int mapped;

                foreach (KeyValuePair<string, Field> entry in fieldMap)
                {
                    mapped = ((Field)entry.Value).Offset + ((Field)entry.Value).Size;
                    if (mappedSize < mapped)
                        mappedSize = mapped;
                }

                return mappedSize;
            }
        }

        public bool EnumFields(ref object enumHandle, out string fieldName, out FieldType fieldType, out int fieldSize, out int fieldOffset)
        {
            fieldName = "";
            fieldType = FieldType.Raw;
            fieldSize = 0;
            fieldOffset = 0;

            if (enumHandle == null)
                enumHandle = fieldMap.GetEnumerator();

            IDictionaryEnumerator enumerator = (IDictionaryEnumerator)enumHandle;

            if (!enumerator.MoveNext())
                return false;

            fieldName = (string)enumerator.Key;
            fieldType = ((Field)(enumerator.Value)).Type;
            fieldSize = ((Field)(enumerator.Value)).Size;
            fieldOffset = ((Field)(enumerator.Value)).Offset;

            return true;
        }

        public void BeginFieldOffset(int offset = 0)
        {
            currentFieldOffset = offset;
        }

        public bool GetFieldInfo(string fieldName, out FieldType fieldType, out int fieldSize, out int startPos)
        {
            Field field = fieldMap[fieldName] as Field;

            if (field == null)
            {
                fieldType = FieldType.Raw;
                fieldSize = 0;
                startPos = 0;

                return false;
            }

            fieldType = field.Type;
            fieldSize = field.Size;
            startPos = field.Offset;

            return true;
        }

        public void Set(string fieldName, byte[] buf)
        {
            int offset, size;

            if (!String.IsNullOrEmpty(fieldName))
            {
                Field field = (Field)fieldMap[fieldName];
                offset = field.Offset;
                size = field.Size;
            }
            else
            {
                offset = 0;
                size = bitStream.Size;
            }

            bitStream.Seek(offset);
            int bitlen = buf.Length << 3;

            if (bitlen >= size)
            {
                bitStream.Write(buf, size);
            }
            else
            {
                int bits = size - bitlen;

                byte[] zeroes = new byte[(bits + 7) >> 3];
                for (int i = 0; i < zeroes.Length; i++)
                    zeroes[i] = 0;

                bitStream.Write(buf, bitlen);
                bitStream.Write(zeroes, bits);
            }
        }

        public void Set(string fieldName, string data)
        {
            byte[] byteData = Encoding.UTF8.GetBytes(data);
            Set(fieldName, byteData);
        }

        public void Set(string fieldName, int data)
        {
            byte[] byteData = new byte[sizeof(int)];

            byteData[3] = (byte)(data >> 24);
            byteData[2] = (byte)((data >> 16) & 0xFF);
            byteData[1] = (byte)((data >> 8) & 0xFF);
            byteData[0] = (byte)(data & 0xFF);

            Set(fieldName, byteData);
        }

        public void Set(string fieldName, DateTime date)
        {
            FieldType type;
            int size, offset;

            GetFieldInfo(fieldName, out type, out size, out offset);

            byte[] rawDate;

            switch (type)
            {
                case FieldType.Date13:
                    rawDate = PackDate13(date);
                    break;

                case FieldType.Date14:
                    rawDate = PackDate14(date);
                    break;

                case FieldType.Date16:
                    rawDate = PackDate16(date);
                    break;

                default:
                    throw new Exception("It is not allowed to set a DateTime value on a non-date field");
            }

            Set(fieldName, rawDate);
        }

        public byte[] Get(string fieldName)
        {
            if (fieldName == null)
                return Buffer;

            Field field = (Field)fieldMap[fieldName];

            byte[] data = new byte[(field.Size + 7) >> 3];

            bitStream.Seek(field.Offset);
            bitStream.Read(data, field.Size);

            return data;
        }

        public int GetInt(string fieldName)
        {
            byte[] raw = Get(fieldName);

            int i = raw[0];
            if (raw.Length > 1) i |= ((int)raw[1] << 8);
            if (raw.Length > 2) i |= ((int)raw[2] << 16);
            if (raw.Length > 3) i |= ((int)raw[3] << 24);

            return i;
        }

        public DateTime GetDate(string fieldName)
        {
            FieldType type;
            int size, offset;

            GetFieldInfo(fieldName, out type, out size, out offset);

            switch (type)
            {
                case FieldType.Date13:
                    if (size != 13)
                        throw new Exception("Invalid field size for a Date13 field type. Size should be 13.");

                    return UnpackDate13(Get(fieldName));

                case FieldType.Date14:
                    if (size != 14)
                        throw new Exception("Invalid field size for a Date14 field type. Size should be 14.");

                    return UnpackDate14(Get(fieldName));

                case FieldType.Date16:
                    if (size != 16)
                        throw new Exception("Invalid field size for a Date16 field type. Size should be 16.");

                    return UnpackDate16(Get(fieldName));

                default:
                    throw new Exception("It is not allowed to query dates from a non-date field");
            }
        }

        public byte[] Buffer
        {
            get
            {
                return bitStream.Buffer;
            }
        }

        public int CurrentFieldOffset
        {
            get
            {
                return currentFieldOffset;
            }
        }

        private byte[] PackDate13(DateTime date)
        {
            if (date.Year < 2010 || date.Year > 2041)
                throw new Exception("Invalid year for a 13-bit date. Allowed values are 2012-2027");

            ushort udate = (ushort)(((date.Month - 1) << 9) | ((date.Day - 1) << 4) | (date.Year - 2012));

            return new byte[2] { (byte)(udate & 0xFF), (byte)(udate >> 8) };
        }

        private byte[] PackDate14(DateTime date)
        {
            if (date.Year < 2010 || date.Year > 2041)
                throw new Exception("Invalid year for a 14-bit date. Allowed values are 2010-2041");

            ushort udate = (ushort)(((date.Month - 1) << 10) | ((date.Day - 1) << 5) | (date.Year - 2010));

            return new byte[2] { (byte)(udate & 0xFF), (byte)(udate >> 8) };
        }

        private byte[] PackDate16(DateTime date)
        {
            if (date.Year < 2000 || date.Year > 2127)
                throw new Exception("Invalid year for a 16-bit date. Allowed values are 2010-2127");

            ushort expDateBits = (ushort)(((date.Year - 2000) << 9) | (date.Month << 5) | date.Day);

            return new byte[2] { (byte)(expDateBits & 0xFF), (byte)(expDateBits >> 8) };
        }

        private DateTime UnpackDate13(byte[] packedDate)
        {
            ushort keyData = (ushort)(((ushort)(packedDate[1] & 0x1F)) << 8 | packedDate[0]);
            return new DateTime(2012 + (keyData & 0x0F), 1 + (keyData >> 9), 1 + ((keyData >> 4) & 0x1F));
        }

        private DateTime UnpackDate14(byte[] packedDate)
        {
            ushort keyData = (ushort)(((ushort)(packedDate[1] & 0x3F)) << 8 | packedDate[0]);
            return new DateTime(2010 + (keyData & 0x1F), 1 + (keyData >> 10), 1 + ((keyData >> 5) & 0x1F));
        }

        private DateTime UnpackDate16(byte[] packedDate)
        {
            ushort keyData = (ushort)(((ushort)packedDate[1]) << 8 | packedDate[0]);
            return new DateTime(2000 + (keyData >> 9), (keyData & 0x01FF) >> 5, keyData & 0x001F);
        }

        BitStream bitStream;
        Dictionary<string, Field> fieldMap;
        private int currentFieldOffset;

        public enum FieldType
        {
            Raw, 
            Integer,
            String,
            Date14,
            Date16,
            Date13
        }

        class Field
        {
            public Field()
            {

            }

            public Field(FieldType type, int offset, int size)
            {
                this.type = type;
                this.offset = offset;
                this.size = size;
            }

            public FieldType Type
            {
                get
                {
                    return type;
                }
            }

            public int Offset
            {
                get
                {
                    return offset;
                }
            }

            public int Size
            {
                get
                {
                    return size;
                }
            }

            public object Value
            {
                get
                {
                    return value;
                }
                set
                {
                    this.value = value;
                }
            }

            private FieldType type;
            private int offset;
            private int size;
            private object value;
        }
    }
}
