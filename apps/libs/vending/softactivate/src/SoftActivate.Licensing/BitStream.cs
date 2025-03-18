//
// Copyright (c) 2014 SoftActivate. All rights reserved.
//

using System;

namespace SoftActivate.Licensing
{
    class BitStream
    {
        public BitStream()
        {

        }

        public BitStream(int sizeInBits, bool randomize = false)
        {
            Create(sizeInBits, randomize);
        }

        public BitStream(byte[] buf, int bufBits)
        {
            Attach(buf, bufBits);
        }

        ~BitStream()
        {

        }

        public void Create(int bitCount, bool randomize = false)
        {
            buf = new byte[(bitCount + 7) >> 3];

            if (randomize)
            {
                Random rand = new Random();

                for (int i = 0; i < buf.Length; i++)
                    buf[i] = (byte)rand.Next(0, 255);
            }

            bufBits = bitCount;
            currentBitIndex = 0;
        }

        public void Attach(byte[] buf, int bufBits)
        {
            this.buf = buf;
            this.bufBits = bufBits;
            currentBitIndex = 0;
        }

        public int Read(byte[] readBuf, int bitCount)
        {
            if (bitCount + currentBitIndex > bufBits)
            {
                bitCount = bufBits - currentBitIndex;
            }

            if (bitCount == 0)
            {
                return 0;
            }

            int readBitCount = bitCount;
            byte lastByteMask = (byte)((1 << (readBitCount & 7)) - 1);
            int sourcePtr = (currentBitIndex >> 3);
            int byteCount = (bitCount + 7) >> 3;
            int destPtr = 0;

            int downShift = currentBitIndex & 0x7;
            int upShift = 8 - downShift;

            if (downShift == 0)
            {
                while (byteCount-- > 0)
                {
                    readBuf[destPtr++] = buf[sourcePtr++];
                }

                currentBitIndex += bitCount;

                if (lastByteMask > 0)
                    readBuf[readBitCount >> 3] &= lastByteMask;
                
                return readBitCount;
            }

            byte sourceByte = (byte)(buf[sourcePtr] >> downShift);
            currentBitIndex += bitCount;

            for (; bitCount > 8; bitCount -= 8)
            {
                byte nextByte = buf[++sourcePtr];
                readBuf[destPtr++] = (byte)(sourceByte | (nextByte << upShift));
                sourceByte = (byte)(nextByte >> downShift);
            }

            if (bitCount > 0)
            {
                if (bitCount <= upShift)
                {
                    readBuf[destPtr] = sourceByte;

                    if (lastByteMask > 0)
                        readBuf[readBitCount >> 3] &= lastByteMask;
                    
                    return readBitCount;
                }

                readBuf[destPtr] = (byte)(sourceByte | (buf[sourcePtr + 1] << upShift));
            }

            if (lastByteMask > 0)
                readBuf[readBitCount >> 3] &= lastByteMask;

            return readBitCount;
        }

        public int Write(byte[] writeBuf, int bitCount)
        {
            int maxBits = ((bufBits + 7) >> 3) << 3;

            if (bitCount + currentBitIndex > maxBits)
            {
                bitCount = maxBits - currentBitIndex;
            }

            if (bitCount <= 0)
                return 0;

            int writeBitCount = bitCount;
            int upShift = currentBitIndex & 0x7;
            int downShift = 8 - upShift;

            int sourcePtr = 0;
            int destPtr = currentBitIndex >> 3;

            // if this write is for <= 1 byte, and it will all fit in the
            // first dest byte, then do some special masking.
            if (downShift >= bitCount)
            {
                byte mask = (byte)(((1 << bitCount) - 1) << upShift);
                buf[destPtr] = (byte)((buf[destPtr] & ~mask) | ((writeBuf[sourcePtr] << upShift) & mask));
                currentBitIndex += bitCount;
                return writeBitCount;
            }

            // check for byte aligned writes -- this will be
            // much faster than the shifting writes.
            if (upShift == 0)
            {
                currentBitIndex += bitCount;

                for (; bitCount >= 8; bitCount -= 8)
                    buf[destPtr++] = writeBuf[sourcePtr++];

                if (bitCount > 0)
                {
                    byte mask = (byte)((1 << bitCount) - 1);
                    buf[destPtr] = (byte)((writeBuf[sourcePtr] & mask) | (buf[destPtr] & ~mask));
                }

                return writeBitCount;
            }

            // the write destination is not byte aligned.
            byte sourceByte;
            byte destByte = (byte)(buf[destPtr] & (0xFF >> downShift));
            byte lastMask = (byte)(0xFF >> (7 - ((currentBitIndex + bitCount - 1) & 0x7)));

            currentBitIndex += bitCount;

            for (; bitCount >= 8; bitCount -= 8)
            {
                sourceByte = writeBuf[sourcePtr++];
                buf[destPtr++] = (byte)(destByte | (sourceByte << upShift));
                destByte = (byte)(sourceByte >> downShift);
            }

            if (bitCount == 0)
            {
                if (destPtr < buf.Length) buf[destPtr] = (byte)((buf[destPtr] & ~lastMask) | (destByte & lastMask));
                return writeBitCount;
            }

            if (bitCount <= downShift)
            {
                if (destPtr < buf.Length) buf[destPtr] = (byte)((buf[destPtr] & ~lastMask) | ((destByte | (writeBuf[sourcePtr] << upShift)) & lastMask));
                return writeBitCount;
            }

            sourceByte = writeBuf[sourcePtr];

            buf[destPtr++] = (byte)(destByte | (sourceByte << upShift));
            buf[destPtr] = (byte)((buf[destPtr] & ~lastMask) | ((sourceByte >> downShift) & lastMask));

            return writeBitCount;
        }

        public int Seek(int offset)
        {
            return Seek(offset, false);
        }

        public int Seek(int offset, bool relative)
        {
            int oldIndex = currentBitIndex;
            currentBitIndex = (relative) ? currentBitIndex + offset : offset;
            return oldIndex;
        }

        public int Size
        {
            get
            {
                return bufBits;
            }
        }

        public void ZeroPadToNextByte()
        {
            byte[] zero = new byte[1] { 0 };

            if ((currentBitIndex & 0x7) > 0)
                Write(zero, 8 - (currentBitIndex & 0x7));
        }

        public byte[] Buffer
        {
            get
            {
                return buf;
            }
        }

        public void Clear()
        {
            for (int i = 0; i < (bufBits + 7) >> 3; i++)
            {
                buf[i] = 0;
            }
        }

        public void Randomize()
        {
            Random rand = new Random();
            rand.NextBytes(buf);
        }

        public int Length
        {
            get
            {
                return bufBits;
            }
        }

        protected byte[] buf;
        protected int bufBits;
        protected int currentBitIndex;
    }
}
