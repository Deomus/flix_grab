using System;

namespace SoftActivate.Licensing
{
    class BitStream2
    {
        public BitStream2()
        {
            currentByte = 0;
            currentBit = 0;
        }

        public BitStream2(int bitCount)
            : this()
        {
            buffer = new byte[(bitCount + 7) >> 3];
        }

        public BitStream2(byte[] buf)
            : this()
        {
            Attach(buf, buf.Length << 3);
        }

        public void Attach(byte[] buf, int bitCount)
        {
            buffer = buf;
        }

        public int Write(byte val, int bitCount)
        {
            return Write(new byte[1] { val }, bitCount);
        }

        public int Write(byte[] buf, int bitCount)
        {
            byte destByte;

            int leftBits = currentBit & 7;
            int rightBits = 8 - leftBits;

            int i = 0;
            int count = bitCount;
            while (count > 0)
            {
                destByte = (byte)((buffer[currentByte] & hiMask[leftBits]) | (buf[i] >> leftBits));
                buffer[currentByte] = destByte;

                count = (count > rightBits) ? count - rightBits : 0;

                if (count > 0)
                {
                    currentByte++;
                    buffer[currentByte] = (byte)(buf[i] << rightBits);
                    count = (count > leftBits) ? count - leftBits : 0;
                }

                i++;
            }

            currentBit += bitCount;

            return bitCount;
        }

        public int WriteUInt32(UInt32 val)
        {
            byte[] split = new byte[4] { (byte)(val >> 24), (byte)((val >> 16) & 0xFF), (byte)((val >> 8) & 0xFF), (byte)(val & 0xFF) };
            return Write(split, 32);
        }

        public int WriteUInt16(UInt16 val)
        {
            byte[] split = new byte[2] { (byte)(val >> 8), (byte)(val & 0xFF) };
            return Write(split, 16);
        }

        public int Read(byte[] readBuf, int bitCount)
        {
            byte destByte;
            int rightBits = 8 - (currentBit & 7);
            int leftBits = 8 - rightBits;

            int count = bitCount;
            int i = 0;
            while (count > 0)
            {
                readBuf[i] = (byte)(buffer[currentByte] << leftBits);
                count = (count > rightBits) ? count - rightBits : 0;
                if (count > 0)
                {
                    currentByte++;
                    destByte = readBuf[i];
                    destByte = (byte)(destByte | (buffer[currentByte] >> rightBits));
                    readBuf[i] = destByte;

                    count = (count > leftBits) ? count - leftBits : 0;
                }

                i++;
            }

            currentBit += bitCount;

            return bitCount;
        }

        public int ReadUInt16(out ushort val)
        {
            return ReadUInt16(out val, 16);
        }

        public int ReadUInt16(out ushort val, int bitCount)
        {
            byte[] split = new byte[2];
            int result = Read(split, bitCount);

            val = (ushort)(((ushort)split[0] << 8) | (ushort)split[1]);

            return result;
        }

        public int ReadUInt32(out uint val)
        {
            return ReadUInt32(out val, 32);
        }

        public int ReadUInt32(out uint val, int bitCount)
        {
            byte[] split = new byte[4];
            int result = Read(split, bitCount);

            val = (uint)(((uint)split[0] << 24) | ((uint)split[1] << 16) | ((uint)split[2] << 8) | (uint)split[3]);

            return result;
        }

        public byte[] Buffer
        {
            get { return buffer; }
        }

        byte[] buffer;
        int currentByte;
        int currentBit;
        byte[] hiMask = new byte[9] { 0, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF };
        byte[] loMask = new byte[9] { 0, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF };
    }
}