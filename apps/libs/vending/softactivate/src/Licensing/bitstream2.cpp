#include "precomp.h"

#include <stdlib.h>
#include <string.h>
#include "bitstream2.h"

const unsigned char BitStream2::hiMask[] = { 0, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF };

BitStream2::BitStream2() :
ownMemory(false),
currentBit(0),
currentByte(0),
buffer(NULL)
{

}

BitStream2::BitStream2(int bitCount)
{
	currentBit = 0;
	currentByte = 0;
	Create(bitCount);
}

BitStream2::~BitStream2()
{
	if (ownMemory)
		delete buffer;
}

void BitStream2::Create(int bitCount)
{
	bufSize = (bitCount + 7) >> 3;
	buffer = new unsigned char[(bitCount + 7) >> 3];
	ownMemory = true;
	memset(buffer, 0, bufSize);
}

void BitStream2::Attach(void * buf)
{
	buffer = (unsigned char *)buf;
}

int BitStream2::Write(const unsigned char * buf, int bitCount)
{
    unsigned char destByte;

    int leftBits = currentBit & 7;
    int rightBits = 8 - leftBits;

    int i = 0;
    int count = bitCount;
    while (count > 0)
    {
        destByte = (unsigned char)((buffer[currentByte] & hiMask[leftBits]) | (buf[i] >> leftBits));
        buffer[currentByte] = destByte;

        count = (count > rightBits) ? count - rightBits : 0;

        if (count > 0)
        {
            currentByte++;
            buffer[currentByte] = (unsigned char)(buf[i] << rightBits);
            count = (count > leftBits) ? count - leftBits : 0;
        }

        i++;
    }

    currentBit += bitCount;

    return bitCount;
}

int BitStream2::WriteUInt16(unsigned short val)
{
    unsigned char split[2]; split[0] = (unsigned char)(val >> 8); split[1] = (unsigned char)(val & 0xFF);
    return Write(split, 16);
}

int BitStream2::Read(unsigned char * buf, int bitCount)
{
    unsigned char destByte;
    int rightBits = 8 - (currentBit & 7);
    int leftBits = 8 - rightBits;

    int count = bitCount;
    int i = 0;
    while (count > 0)
    {
        buf[i] = (unsigned char)(buffer[currentByte] << leftBits);
        count = (count > rightBits) ? count - rightBits : 0;
        if (count > 0)
        {
            currentByte++;
            destByte = buf[i];
            destByte = (unsigned char)(destByte | (buffer[currentByte] >> rightBits));
            buf[i] = destByte;

            count = (count > leftBits) ? count - leftBits : 0;
        }

        i++;
    }

    currentBit += bitCount;

    return bitCount;
}

int BitStream2::ReadUInt16(unsigned short * val)
{
    unsigned char split[2];

    int result = Read(split, 16);

    *val = (unsigned short)(((unsigned short)split[0] << 8) | (unsigned short)split[1]);

    return result;
}

unsigned char * BitStream2::GetBuffer()
{
	return buffer;
}
