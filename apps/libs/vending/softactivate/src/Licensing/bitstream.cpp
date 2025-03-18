//
// Copyright (c) 2014 FreeGrabApp. All rights reserved.
//

#include "precomp.h"

#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <tchar.h>
#endif

#include "bitstream.h"

BitStream::BitStream()
{
    Init();
}

BitStream::BitStream(void * buf, int bufBits)
{
    Init();

    Attach(buf, bufBits);
}

BitStream::BitStream(int bitSize)
{
    Init();

    Attach(new unsigned char[(bitSize + 7) >> 3], bitSize);

    m_ownMemory = true;
}

BitStream::~BitStream()
{
    if (m_buf && m_ownMemory)
        delete [] m_buf;
}

void BitStream::Init()
{
    m_ownMemory = true;
    m_buf = NULL;
    m_bufBits = 0;
    m_currentBitIndex = 0;
}

bool BitStream::Create(int bitCount)
{
    m_ownMemory = true;
    m_bufBits = bitCount;
    m_currentBitIndex = 0;

    if (!bitCount)
    {
        m_ownMemory = false;
        return true;
    }

    int byteCount = (bitCount + 7) >> 3;

    if ((m_buf = new unsigned char[byteCount]) == NULL)
        return false;

    memset(m_buf, 0, byteCount);

    return true;
}

void BitStream::Attach(void * buf, int bufBits)
{
    if (m_buf && m_ownMemory)
        delete [] m_buf;

    m_buf = (unsigned char *)buf;
    m_bufBits = bufBits;
    m_currentBitIndex = 0;
    m_ownMemory = false;
}

int BitStream::Read(void *bitPtr, int bitCount)
{
    if(bitCount + m_currentBitIndex > m_bufBits)
    {
        bitCount = m_bufBits - m_currentBitIndex;
    }

    if(!bitCount)
    {
        return 0;
    }

    int readBitCount = bitCount;
    unsigned char lastByteMask = (unsigned char)((1 << (readBitCount & 7)) - 1);
    unsigned char * sourcePtr = m_buf + (m_currentBitIndex >> 3);
    int byteCount = (bitCount + 7) >> 3;
    unsigned char * destPtr = (unsigned char *)bitPtr;

    int downShift = m_currentBitIndex & 0x7;
    int upShift = 8 - downShift;

    if (!downShift)
    {
        while(byteCount--)
            *destPtr++ = *sourcePtr++;

        m_currentBitIndex += bitCount;

        if (lastByteMask > 0)
            ((unsigned char *)bitPtr)[readBitCount >> 3] &= lastByteMask;

        return readBitCount;
    }

    unsigned char sourceByte = *sourcePtr >> downShift;
    m_currentBitIndex += bitCount;

    for (; bitCount > 8; bitCount -= 8)
    {
        unsigned char nextByte = *++sourcePtr;
        *destPtr++ = sourceByte | (nextByte << upShift);
        sourceByte = nextByte >> downShift;
    }

    if (bitCount)
    {
        if (bitCount <= upShift)
        {
            *destPtr = sourceByte;
            return readBitCount;
        }

        *destPtr = sourceByte | ( (*++sourcePtr) << upShift);
    }

    if (lastByteMask > 0)
        ((unsigned char *)bitPtr)[readBitCount >> 3] &= lastByteMask;

    return readBitCount;
}

int BitStream::Write(const void * bitPtr, int bitCount)
{
   int maxBits = ((m_bufBits + 7) >> 3) << 3;

   if (bitCount + m_currentBitIndex > maxBits)
   {
        bitCount = maxBits - m_currentBitIndex;
   }

   if(!bitCount)
      return 0;

   int writeBitCount = bitCount;
   int upShift   = m_currentBitIndex & 0x7;
   int downShift = 8 - upShift;

   const unsigned char * sourcePtr = (unsigned char *)bitPtr;
   unsigned char * destPtr = m_buf + (m_currentBitIndex >> 3);

   // if this write is for <= 1 byte, and it will all fit in the
   // first dest byte, then do some special masking.
   if (downShift >= bitCount)
   {
      unsigned char mask = (unsigned char)(((1 << bitCount) - 1) << upShift);
      *destPtr = (*destPtr & ~mask) | ((*sourcePtr << upShift) & mask);
      m_currentBitIndex += bitCount;
      return writeBitCount;
   }

   // check for byte aligned writes -- this will be
   // much faster than the shifting writes.

   if(!upShift) 
   {
      m_currentBitIndex += bitCount;

      for(; bitCount >= 8; bitCount -= 8)
         *destPtr++ = *sourcePtr++;

      if(bitCount)
      {
         unsigned char mask = (unsigned char)((1 << bitCount) - 1);
         *destPtr = (*sourcePtr & mask) | (*destPtr & ~mask);
      }

      return writeBitCount;
   }

   // the write destination is not byte aligned.
   unsigned char sourceByte;
   unsigned char destByte = *destPtr & (0xFF >> downShift);
   unsigned char lastMask  = (unsigned char)(0xFF >> (7 - ((m_currentBitIndex + bitCount - 1) & 0x7)));

   m_currentBitIndex += bitCount;

   for(;bitCount >= 8; bitCount -= 8)
   {
      sourceByte = *sourcePtr++;
      *destPtr++ = destByte | (sourceByte << upShift);
      destByte = sourceByte >> downShift;
   }

   if(bitCount == 0)
   {
      *destPtr = (*destPtr & ~lastMask) | (destByte & lastMask);
      return writeBitCount;
   }

   if(bitCount <= downShift)
   {
      *destPtr = (*destPtr & ~lastMask) | ((destByte | (*sourcePtr << upShift)) & lastMask);
      return writeBitCount;
   }

   sourceByte = *sourcePtr;

   *destPtr++ = destByte | (sourceByte << upShift);
   *destPtr = (*destPtr & ~lastMask) | ((sourceByte >> downShift) & lastMask);

   return writeBitCount;
}

void BitStream::ZeroPadToNextByte()
{
    unsigned char zero = 0;

    if(m_currentBitIndex & 0x7)
        Write(&zero, 8 - (m_currentBitIndex & 0x7));
}

int BitStream::Seek(int offset, bool relative)
{
    unsigned oldIndex = m_currentBitIndex;
    m_currentBitIndex = (relative) ? m_currentBitIndex + offset : offset;
    return oldIndex;
}

int BitStream::GetSize()
{
    return m_bufBits;
}

void * BitStream::GetBuffer(int * sizeInBits)
{
    if (sizeInBits != NULL)
        *sizeInBits = m_bufBits;
    return m_buf;
}

void BitStream::ReleaseBuffer(int bitCount)
{
    m_currentBitIndex = bitCount;
}

void BitStream::Clear()
{
    memset(m_buf, 0, (m_bufBits + 7) >> 3);
}

