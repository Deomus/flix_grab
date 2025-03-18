#ifndef __BASE64_H
#define __BASE64_H

class BASE64 {
public:
	BASE64();
	~BASE64();

	char * encode( unsigned char * input, int len, bool padding = false );
	unsigned char * decode( const char * input, int len, int * outlen );

	int encode_pad_length(int len, int *pad);

private:
	void encode_exactly(const unsigned char *buf, int len, char *encbuf, int enclen);

	int decode_into(const char *buf, int len, unsigned char *decbuf, int declen);
	int decode_alphabet(const char valmap[], const char *buf, int len, unsigned char *decbuf, int declen, int padding);

	static const char values[];
	static const char alphabet[];
};

#endif
