using System;
using System.Text;

namespace SoftActivate.Licensing
{
    class Base32
    {
        Base32()
        {

        }

        public static string Encode(byte[] bytes, int loc, int len)
        {
            int total = loc + len;
            int index = 0, digit = 0;
            int currByte, nextByte;
            StringBuilder base32 = new StringBuilder((len + 7) * 8 / 5);
            while (loc < total)
            {
                currByte = bytes[loc];
                if (index > 3)
                {
                    if ((loc + 1) < total)
                        nextByte = bytes[loc + 1];
                    else
                        nextByte = 0;
                    digit = currByte & (0xFF >> index);
                    index = (index + 5) % 8;
                    digit <<= index;
                    digit |= nextByte >> (8 - index);
                    loc++;
                }
                else
                {
                    digit = (currByte >> (8 - (index + 5))) & 0x1F;
                    index = (index + 5) % 8;
                    if (index == 0)
                        loc++;
                }
                base32.Append(base32Chars[digit]);
            }
            return base32.ToString();
        }

        public static byte[] Decode(string base32, int loc, int len)
        {
            if (base32 == null)
                throw new Exception();

            int total = loc + len;
            int index, lookup, offset, digit;
            byte[] bytes = new byte[(len * 5 + 7) / 8];
            for (index = 0, offset = 0; loc < total; loc++)
            {
                lookup = base32[loc] - '0';
                if (lookup < 0 || lookup >= base32Lookup.Length)
                    continue;
                digit = base32Lookup[lookup];
                if (digit == 0xFF)
                    continue;
                if (index <= 3)
                {
                    index = (index + 5) % 8;
                    if (index == 0)
                    {
                        bytes[offset] |= (byte)digit;
                        offset++;
                        if (offset >= bytes.Length)
                            break;
                    }
                    else
                        bytes[offset] |= (byte)(digit << (8 - index));
                }
                else
                {
                    index = (index + 5) % 8;
                    bytes[offset] |= (byte)((uint)digit >> index);
                    offset++;
                    if (offset >= bytes.Length)
                        break;
                    bytes[offset] |= (byte)(digit << (8 - index));
                }
            }
            return bytes;
        }

        static string base32Chars = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789";
        private static int[] base32Lookup =
		    {
			    0xFF,0xFF,0x18,0x19,0x1A,0x1B,0x1C,0x1D, // '0', '1', '2', '3', '4', '5', '6', '7'
			    0x1E,0x1F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF, // '8', '9', ':', ';', '<', '=', '>', '?'
			    0xFF,0x00,0x01,0x02,0x03,0x04,0x05,0x06, // '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G'
			    0x07,0xFF,0x08,0x09,0x0A,0x0B,0x0C,0xFF, // 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O'
			    0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13,0x14, // 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W'
			    0x15,0x16,0x17,0xFF,0xFF,0xFF,0xFF,0xFF, // 'X', 'Y', 'Z', '[', '\', ']', '^', '_'
			    0xFF,0x00,0x01,0x02,0x03,0x04,0x05,0x06, // '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g'
			    0x07,0xFF,0x08,0x09,0x0A,0x0B,0x0C,0xFF, // 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o'
			    0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13,0x14, // 'p', 'q', 'r', 's', 't', 'u', 'v', 'w'
			    0x15,0x15,0x17,0xFF,0xFF,0xFF,0xFF,0xFF  // 'x', 'y', 'z', '{', '|', '}', '~', 'DEL'
		    };
    }
}
