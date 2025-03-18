#ifndef __BITSTRUCT_H
#define __BITSTRUCT_H

#include "bitstream.h"
#include "uniconv.h"
#include "except.h"

#include <map>

#ifndef _WIN32
#include <alloca.h>
#endif

class BitStruct {

public:
	enum FIELD_TYPE
	{
		RAW,
		INTEGER,
		STRING,
		DATE14,
		DATE13,
		DATE16,
	};

private:
	class Field {
	public:
		Field()
		{

		}

		Field(FIELD_TYPE fieldType, int offset, int size)
		{
			m_fieldType = fieldType;
			m_offset = offset;
			m_size = size;
		}

	public:
		FIELD_TYPE m_fieldType;
		int m_offset;
		int m_size;
	};

	typedef std::map<string_t, Field> FieldMap;

public:

	BitStruct(void * buffer, int bitSize)
	{
		m_bits = new BitStream(buffer, bitSize);
	}

	BitStruct(int bitSize)
	{
		m_bits = new BitStream(bitSize);
	}

	BitStruct()
	{
		m_bits = NULL;
	}

	~BitStruct()
	{
		if (m_bits)
			delete m_bits;
	}

	void Attach(void * buffer, int bitSize)
	{
		if (!m_bits)
			m_bits = new BitStream(buffer, bitSize);
		else
			m_bits->Attach(buffer, bitSize);
	}

	void AddField(const char_t * fieldName, FIELD_TYPE fieldType, int fieldBitSize, int offset = -1 )
	{
		if (offset == -1)
			offset = currentFieldOffset;

		m_fieldMap.insert(FieldMap::value_type(fieldName, Field(fieldType, offset, fieldBitSize)));

		currentFieldOffset = offset + fieldBitSize;
	}

	bool GetField(const char_t * fieldName, FIELD_TYPE * fieldType, int * fieldSize, int * fieldOffset)
	{
		FieldMap::iterator iter = m_fieldMap.find(fieldName);
		
		if (iter == m_fieldMap.end())
			return false;

		*fieldType = iter->second.m_fieldType;
		*fieldSize = iter->second.m_size;
		*fieldOffset = iter->second.m_offset;

		return true;
	}

	bool EnumFields(void **enumHandle, const char_t **fieldName, FIELD_TYPE * fieldType, int * fieldSize, int * offset)
	{
		FieldMap::iterator * pIter;
		static string_t name;

		if (*enumHandle == NULL)
		{
			pIter = new FieldMap::iterator(m_fieldMap.begin());
			if (!pIter)
				return false;
		}
		else
			pIter = (FieldMap::iterator *)(*enumHandle);

		if (*pIter == m_fieldMap.end())
		{
			delete pIter;
			return false;
		}

		name = (*pIter)->first;
		*fieldName = name.c_str();

		*fieldType = (*pIter)->second.m_fieldType;
		*fieldSize = (*pIter)->second.m_size;
		*offset = (*pIter)->second.m_offset;

		pIter->operator ++();  // *Iter++ does not work. Think why :)

		*enumHandle = pIter;

		return true;
	}

	void Set(const char_t * fieldName, const void * buf, unsigned len)
	{
		FieldMap::iterator iter;
		iter = m_fieldMap.find(fieldName);
		if (iter == m_fieldMap.end() )
			throw new KeyExceptionImpl(STATUS_FIELD_NOT_FOUND);

		m_bits->Seek(iter->second.m_offset);
		//m_rawKey.Write(buf, iter->second.m_size);
		int bitlen = len << 3;

		if (bitlen >= iter->second.m_size)
			m_bits->Write(buf, iter->second.m_size);
		else
		{
			int bits = iter->second.m_size - bitlen;
			unsigned char * zeroes = (unsigned char *)_alloca((bits + 7) >> 3);
			memset(zeroes, 0, (bits + 7) >> 3);
			m_bits->Write(buf, bitlen);
			m_bits->Write(zeroes, bits);
		}
	}

	void Set(const char_t * fieldName, const char_t * data)
	{
		USES_CONVERSION;
		const char * utf8Data = T2UTF8(data);

		Set(fieldName, utf8Data, (unsigned)strlen(utf8Data));
	}

	void Set(const char_t * fieldName, int data)
	{
		Set(fieldName, &data, sizeof(data));
	}

	void Set(const char_t * fieldName, int year, int month, int day)
	{
		FieldMap::iterator iter;
		iter = m_fieldMap.find(fieldName);
		if (iter == m_fieldMap.end() )
			throw new KeyExceptionImpl(STATUS_FIELD_NOT_FOUND);

		int date;

		switch (iter->second.m_fieldType)
		{
		case FIELD_TYPE_DATE13:
			date = PackDate13(year, month, day);
			break;

		case FIELD_TYPE_DATE14:
			date = PackDate14(year, month, day);
			break;

		case FIELD_TYPE_DATE16:
			date = PackDate16(year, month, day);
			break;

		default:
			throw new KeyExceptionImpl(STATUS_INVALID_PARAM, _T("A data value is being set to a non-date field"));
		}

		Set(fieldName, date);
	}

	void Get(const char_t * dataField, void * buf, int * len)
	{
		if (dataField)
		{
			FieldMap::const_iterator iter;

			iter = m_fieldMap.find(dataField);
			if ( iter == m_fieldMap.end() )
				throw new KeyExceptionImpl(STATUS_FIELD_NOT_FOUND);

			if ( *len < ( iter->second.m_size + 7 ) >> 3 )
				throw new KeyExceptionImpl(STATUS_BUFFER_TOO_SMALL);

			m_bits->Seek(iter->second.m_offset);
			m_bits->Read(buf, iter->second.m_size);

			*len = (iter->second.m_size + 7) >> 3;
		} else
		{
			int validationDataLen = (m_bits->GetSize() + 7) >> 3;
			
			if (*len < validationDataLen)
			{
				*len = validationDataLen;
				throw new KeyExceptionImpl(STATUS_BUFFER_TOO_SMALL);
			}

			memcpy(buf, m_bits->GetBuffer(), validationDataLen);
			*len = validationDataLen;
		}
	}

	int GetInt(const char_t * dataField)
	{
		FieldMap::const_iterator iter;

		iter = m_fieldMap.find(dataField);
		if ( iter == m_fieldMap.end() )
			throw new KeyExceptionImpl(STATUS_FIELD_NOT_FOUND);

		unsigned char buf[sizeof(int)] = {0, 0, 0, 0};

		if ( sizeof(buf) < ( iter->second.m_size + 7 ) >> 3 )
			throw new KeyExceptionImpl(STATUS_BUFFER_TOO_SMALL);

		m_bits->Seek(iter->second.m_offset);
		m_bits->Read(buf, iter->second.m_size);

		return *(int *)buf;
	}

	void GetDate(const char_t * dataField, int * year, int * month, int * day)
	{
		FieldMap::const_iterator iter;

		iter = m_fieldMap.find(dataField);
		if ( iter == m_fieldMap.end() )
			throw new KeyExceptionImpl(STATUS_FIELD_NOT_FOUND);

		unsigned short packedDate = (unsigned short)GetInt(dataField);

		switch (iter->second.m_fieldType)
		{
		case FIELD_TYPE_DATE13:
			UnpackDate13(packedDate, year, month, day);
			break;

		case FIELD_TYPE_DATE14:
			UnpackDate14(packedDate, year, month, day);
			break;

		case FIELD_TYPE_DATE16:
			UnpackDate16(packedDate, year, month, day);
			break;

		default:
			throw new KeyExceptionImpl(STATUS_INVALID_PARAM, _T("An attempt was made to query a date from a non-date field"));
		}
	}

	void * GetBuffer(int * sizeInBits = NULL)
	{
		return m_bits->GetBuffer(sizeInBits);
	}

	int GetCurrentFieldOffset()
	{
		return currentFieldOffset;
	}

	void BeginFieldOffset()
	{
		currentFieldOffset = 0;
	}

	int GetSize()
	{
		return m_bits->GetSize();
	}

private:
    unsigned short PackDate13(int year, int month, int day)
    {
        if (year < 2012 || year > 2027)
			throw new KeyExceptionImpl(STATUS_INVALID_PARAM, _T("Invalid year for a 13-bit date. Allowed values are 2012-2027"));

       return (unsigned short)(((month - 1) << 9) | ((day - 1) << 4) | (year - 2012));
    }

    void UnpackDate13(unsigned short packedDate, int * year, int * month, int * day)
    {
        *year = 2012 + (packedDate & 0x0F);
		*month = 1 + (packedDate >> 9);
		*day = 1 + ((packedDate >> 4) & 0x1F);
    }

    unsigned short PackDate14(int year, int month, int day)
    {
        if (year < 2010 || year > 2041)
			throw new KeyExceptionImpl(STATUS_INVALID_PARAM, _T("Invalid year for a 14-bit date. Allowed values are 2010-2041"));

       return (unsigned short)(((month - 1) << 10) | ((day - 1) << 5) | (year - 2010));
    }

    void UnpackDate14(unsigned short packedDate, int * year, int * month, int * day)
    {
        *year = 2010 + (packedDate & 0x1F);
		*month = 1 + (packedDate >> 10);
		*day = 1 + ((packedDate >> 5) & 0x1F);
    }

    unsigned short PackDate16(int year, int month, int day)
    {
        if (year < 2000 || year > 2127)
			throw new KeyExceptionImpl(STATUS_INVALID_PARAM, _T("Invalid year for a 16-bit date. Allowed values are 2010-2127"));

        return (unsigned short)(((year - 2000) << 9) | (month << 5) | day);
    }

    void UnpackDate16(unsigned short packedDate, int * year, int * month, int * day)
    {
        *year = 2000 + (packedDate >> 9);
		*month = (packedDate & 0x01FF) >> 5;
		*day = packedDate & 0x001F;
    }

	FieldMap m_fieldMap;
	BitStream * m_bits;
	int currentFieldOffset;
};

#endif
