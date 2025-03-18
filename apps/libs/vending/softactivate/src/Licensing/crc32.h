#pragma once

class Crc32
{
public:
	static unsigned int Compute(const unsigned char * buffer, int count);

private:
	static const unsigned int crcTable[256];
};
