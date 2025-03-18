#ifndef __BITSTREAM_H
#define __BITSTREAM_H

class BitStream
{
public:
	BitStream();
	BitStream(void * buf, int bitSize);
	BitStream(int bitSize);
	~BitStream();

   bool Create(int sizeInBits);
   void Attach(void * buf, int sizeInBits);
   int Read(void *bitPtr, int countInBits);
   int Write(const void * bitPtr, int countInBits);
   int Seek(int offset, bool relative = false);
   int GetSize();
   void ZeroPadToNextByte();
   void * GetBuffer(int * sizeInBits = NULL);
   void ReleaseBuffer(int bitCount);
   void Clear();

protected:
	bool m_ownMemory;
	unsigned char * m_buf;
	int m_bufBits;
	int m_currentBitIndex;

	void Init();
};

#endif
