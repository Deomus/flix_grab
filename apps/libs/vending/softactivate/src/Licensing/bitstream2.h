#pragma once

class BitStream2
{
public:
	BitStream2();
	BitStream2(int bitCount);
	~BitStream2();

	void Create(int bitCount);
	void Attach(void * buf);
	int Write(const unsigned char * buf, int bitCount);
	int WriteUInt16(unsigned short val);
	int Read(unsigned char * buf, int bitCount);
	int ReadUInt16(unsigned short * val);
	unsigned char * GetBuffer();

protected:
	bool ownMemory;
	unsigned char * buffer;
	int bufSize;
	int currentBit, currentByte;
	static const unsigned char hiMask[];
};
