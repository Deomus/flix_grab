#ifndef ___SHA1_H___
#define ___SHA1_H___

#include <stdio.h>  // Needed for file access
#include <memory.h> // Needed for memset and memcpy
#include <string.h> // Needed for strcat and strcpy

// If you're compiling big endian, just comment out the following line
#define SHA1_LITTLE_ENDIAN

typedef union
{
	unsigned char c[64];
	unsigned int l[16];
} SHA1_WORKSPACE_BLOCK;

class SHA1
{
public:
	// Two different formats for ReportHash(...)
	enum
	{
		REPORT_HEX = 0,
		REPORT_DIGIT = 1
	};

	// Constructor and Destructor
	SHA1();
	virtual ~SHA1();

	unsigned int m_state[5];
	unsigned int m_count[2];
	unsigned char m_buffer[64];
	unsigned char m_digest[20];

	void Reset();

	// Update the hash value
	void Update(unsigned char *data, unsigned int len);

	// Finalize hash and report
	void Final();
	void GetHash(unsigned char *uDest);

private:
	// Private SHA-1 transformation
	void Transform(unsigned int state[5], unsigned char buffer[64]);

	// Member variables
	unsigned char m_workspace[64];
	SHA1_WORKSPACE_BLOCK *m_block; // SHA1 pointer to the byte array above
};

#endif
