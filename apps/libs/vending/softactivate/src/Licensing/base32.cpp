#include "precomp.h" 
#include "base32.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

const char BASE32::alphabet[] = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789";
const char BASE32::values[] = {
/*  0  1  2  3  4  5  6  7  8  9  */	/* 0123456789              */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,		/*            -  00 -&gt;  09 */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,		/*            -  10 -&gt;  19 */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,		/*            -  20 -&gt;  29 */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,		/*            -  30 -&gt;  39 */
    -1,-1,-1,-1,-1,-1,-1,-1,			/*            -  40 -&gt;  47 */

    -1,-1,24,25,26,27,28,29,30,31,		/* 0123456789 -  48 -&gt;  57 */
    -1,-1,-1,-1,-1,-1,-1, 0, 1, 2,		/* :;&lt;=&gt;?@ABC -  58 -&gt;  67 */
     3, 4, 5, 6, 7,-1, 8, 9,10,11,		/* DEFGHIJKLM -  68 -&gt;  77 */
    12,-1,13,14,15,16,17,18,19,20,		/* NOPQRSTUVW -  78 -&gt;  87 */
    21,22,23,							/* XYZ        -  88 -&gt;  90 */

    -1,-1,-1,-1,-1,-1, 0, 1, 2, 3,		/*       abcd -  91 -&gt; 100 */
     4, 5, 6, 7,-1, 8, 9,10,11,12,		/* efghijklmn - 101 -&gt; 110 */
    -1,13,14,15,16,17,18,19,20,21,		/* opqrstuvwx - 111 -&gt; 120 */
    22,23,-1,-1,-1,-1,-1,-1,-1,-1,		/* yz         - 121 -&gt; 130 */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,		/*            - 131 -&gt; 140 */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,		/*            - 141 -&gt; 150 */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,		/*            - 151 -&gt; 160 */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,		/*            - 161 -&gt; 170 */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,		/*            - 171 -&gt; 180 */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,		/*            - 181 -&gt; 190 */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,		/*            - 191 -&gt; 200 */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,		/*            - 201 -&gt; 210 */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,		/*            - 211 -&gt; 220 */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,		/*            - 221 -&gt; 230 */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,		/*            - 231 -&gt; 240 */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,		/*            - 241 -&gt; 250 */
    -1,-1,-1,-1,-1,						/*            - 251 -&gt; 255 */
};

BASE32::BASE32()
{

}

BASE32::~BASE32()
{

}

char * BASE32::encode( unsigned char * buf, int len, bool padding )
{
	int pad;
	int enclen = encode_pad_length( len, &pad );
	char * encbuf;

	if (!padding)
		pad = 0;

	encbuf = (char *)malloc( enclen + pad + 1 );	/* Allow for trailing NUL */

	encode_exactly(buf, len, encbuf, enclen);
	if (pad)
		memset(encbuf + enclen, '=', pad);
	encbuf[enclen + pad] = '\0';

	//if (retpad)
	//	*retpad = pad;

	return encbuf;
}

/*
 * encode_exactly
 *
 * Encode `len' bytes from `buf' into `enclen' bytes starting from `encbuf'.
 * Caller must have ensured that there was EXACTLY the needed room in encbuf.
 */
void BASE32::encode_exactly(const unsigned char *buf, int len, char *encbuf, int enclen)
{
	int i = 0;					/* Input accumulator, 0 for trailing pad */
	unsigned char const *ip = buf + len;	/* Input pointer, one byte off end */
	char *op = encbuf + enclen;	/* Output pointer, one byte off end */

	assert(buf);
	assert(encbuf);
	assert(len > 0);
	assert(enclen >= len * 8 / 5);

	/*
	 * In the following picture, we represent how the 5 bytes of input
	 * are split into groups of 5 bits, each group being encoded as a
	 * single base32 digit.
	 *
	 * input byte       0        1        2        3        4
	 *              +--------+--------+--------+--------+--------+
	 *              |01234012|34012340|12340123|40123401|23401234|
	 *              +--------+--------+--------+--------+--------+
	 *               <---><----><---><----><----><---><----><--->
	 * output digit    0     1    2     3     4    5     6    7
	 *
	 *
	 * Because of possible padding, which must be done as if the input
	 * was 0, and because the fractional part is at the end, we'll
	 * start encoding from the end.  The encoding loop is unrolled for
	 * greater performance (using the infamous Duff's device to directly
	 * switch at the proper stage within the do {} while loop).
	 */

	switch (len % 5) {
	case 0:
		do {
			assert(op - encbuf >= 8);
			i = (unsigned char) *--ip;		    /* Input #4 */
			*--op = alphabet[i & 0x1f];		    /* Ouput #7 */
			i >>= 5;							/* upper <234>, input #4 */
			/* FALLTHROUGH */
	case 4:
			i |= ((unsigned char) *--ip) << 3;	/* had 3 bits in `i' */
			*--op = alphabet[i & 0x1f];			/* Output #6 */
			i >>= 5;							/* upper <401234>, input #3 */
			*--op = alphabet[i & 0x1f];			/* Output #5 */
			i >>= 5;							/* upper <4>, input #3 */
			/* FALLTHROUGH */
	case 3:
			i |= ((unsigned char) *--ip) << 1;	/* had 1 bits in `i' */
			*--op = alphabet[i & 0x1f];			/* Output #4 */
			i >>= 5;							/* upper <1234>, input #2 */
			/* FALLTHROUGH */
	case 2:
			i |= ((unsigned char) *--ip) << 4;	/* had 4 bits in `i' */
			*--op = alphabet[i & 0x1f];			/* Output #3 */
			i >>= 5;							/* upper <3401234>, input #1 */
			*--op = alphabet[i & 0x1f];			/* Output #2 */
			i >>= 5;							/* upper <34>, input #1 */
			/* FALLTHROUGH */
	case 1:
			i |= ((unsigned char) *--ip) << 2;	/* had 2 bits in `i' */
			*--op = alphabet[i & 0x1f];			/* Output #1 */
			i >>= 5;							/* upper <01234>, input #0 */
			*--op = alphabet[i & 0x1f];			/* Output #0 */
			i >>= 5;							/* Holds nothing, MBZ */
			assert(i == 0);
			assert(op >= encbuf);
		} while (op > encbuf);
	}
}

/*
 * encode_pad_length
 *
 * Compute the number of base32 digits and amount of padding necessary
 * to encode `len' bytes.
 *
 * Returns the number of base32 digits necessary.
 * Furthermore, if `pad' is a non-NULL pointer, it is filled with the amount
 * of padding chars that would be necessary.
 */
int BASE32::encode_pad_length(int len, int *pad)
{
	int ndigits;	    /* Base32 digits necessary */
	int npad = 0;		/* Final padding chars necessary */
	int qcount;			/* Amount of full quintets */
	int remainder;		/* Amount of input bytes in final quintet */

	assert(len > 0);

	qcount = len / 5;
	remainder = len - (qcount * 5);

	assert(remainder >= 0);

	switch (remainder) {
		case 0: npad = 0; break;
		case 1: npad = 6; break;
		case 2: npad = 4; break;
		case 3: npad = 3; break;
		case 4: npad = 1; break;
		default: assert(0);		/* Not possible */
	}

	ndigits = qcount * 8;		/* Each full quintet encoded on 8 bytes */
	if (npad != 0)
		ndigits += (8 - npad);

	if (pad)
		*pad = npad;

	return ndigits;
}

unsigned char * BASE32::decode( const char * buf, int len, int * outlen )
{
	int declen;
	unsigned char *decbuf;
	int decoded;

	if (len == 0 || (len & 0x7))		/* Empty, or padding bytes missing */
		return NULL;

	declen = (len >> 3) * 5;
	decbuf = (unsigned char *)malloc(declen);

	decoded = decode_into(buf, len, decbuf, declen);

	if (decoded == 0) {
        if (decbuf) free (decbuf);
		return NULL;
	}

	if (outlen != NULL)
		*outlen = decoded;

	return decbuf;
}

/*
 * decode_into
 *
 * Decode `len' bytes from `buf' into `declen' bytes starting from `decbuf',
 * faking the necessary amount of padding if necessary.
 * Caller must have ensured that there was sufficient room in decbuf.
 *
 * Returns the amount of bytes decoded (without trailing padding) if successful,
 * 0 if the input was not valid base32.
 */
int BASE32::decode_into(const char *buf, int len, unsigned char *decbuf, int declen)
{
	int padding = 0;

	if (len & 0x7)
		padding = 8 - (len & 0x7);

	return decode_alphabet(values, buf, len + padding, decbuf, declen, padding);
}

/*
 * decode_alphabet
 *
 * Decode `len' bytes from `buf' into `declen' bytes starting from `decbuf'.
 * Caller must have ensured that there was sufficient room in decbuf.
 * Uses the specified decoding alphabet.
 *
 * `padding', when non-zero, is the amount of padding that is missing from
 * the input buffer and which we must assume.
 *
 * Return decoded bytes if successful, 0 if the input was not valid base32.
 */
int BASE32::decode_alphabet(const char valmap[],
	const char *buf, int len, unsigned char *decbuf, int declen, int padding)
{
	int i = 0;					/* Input accumulator, 0 for trailing pad */
	char const *ip = buf + len;		/* Input pointer, one byte off end */
	int dlen = (len >> 3) * 5;		/* Exact decoded length */
	unsigned char *op;				/* Output pointer, one byte off end */
	int bytes;						/* bytes decoded without padding */
	char v;

	assert(padding >= 0);
	assert(buf);
	assert(decbuf);
	assert(len > 0);
	assert((len & 0x7) == 0);			/* `len' is a multiple of 8 bytes */
	//assert(declen >= dlen);

	/*
	 * If the last byte of input is '=', there is padding and we need to
	 * zero the tail of the decoding buffer.
	 *
	 * Likewise, when `padding' is non-zero, act as if the '=' were there.
	 */

	if (buf[len-1] == '=' || padding > 0) {
		int pad = 0;
		int n = 0;							/* Amount of bytes to zero */
		int s = 0;							/* Amount of bytes to zero */

		/*
		 * Remove and count trailing input padding bytes.
		 */

		if (padding == 0) {
			while (*--ip == '=')
				pad++;
			ip++;			/* Points one byte after real non-padding input */
		} else {
			pad = padding;
			ip -= padding;
		}

		switch (pad) {
			case 1: n = 1; s = 0; break;
			case 3: n = 2; s = 1; break;
			case 4: n = 3; s = 2; break;
			case 6: n = 4; s = 3; break;
			default:
				return 0;				/* Cannot be valid base32 */
		}

		memset(decbuf + (dlen - n), 0, n);
		op = decbuf + (dlen - s);
		bytes = dlen - n;
	} else {
		op = decbuf + dlen;
		bytes = dlen;
	}

	/*
	 * In the following picture, we represent how the 8 bytes of input,
	 * each consisting of only 5 bits of information forming a base32 digit,
	 * are concatenated back into 5 bytes of binary information.
	 *
	 * input digit     0     1    2     3     4    5     6    7
	 *               <---><----><---><----><----><---><----><--->
	 *              +--------+--------+--------+--------+--------+
	 *              |01234012|34012340|12340123|40123401|23401234|
	 *              +--------+--------+--------+--------+--------+
	 * output byte      0        1        2        3        4
	 *
	 *
	 * Because of possible padding, which must be done as if the input
	 * was 0, and because the fractional part is at the end, we'll
	 * start decoding from the end.  The decoding loop is unrolled for
	 * greater performance (using the infamous Duff's device to directly
	 * switch at the proper stage within the do {} while loop).
	 */

	switch ((ip - buf) % 8) {
	case 0:
		do {
			i = valmap[(unsigned char) *--ip];		/* Input #7 */
			if (i < 0) 
				return 0;
			/* FALLTHROUGH */
	case 7:
			v = valmap[(unsigned char) *--ip];		/* Input #6 */
			if (v < 0) 
				return 0;
			i |= v << 5;					/* had 5 bits */
			*--op = i & 0xff;				/* Output #4 */
			i >>= 8;						/* lower <01> of output #3 */
			/* FALLTHROUGH */
	case 6:
			v = valmap[(unsigned char) *--ip];		/* Input #5 */
			if (v < 0) 
				return 0;
			i |= v << 2;					/* had 2 bits */
			/* FALLTHROUGH */
	case 5:
			v = valmap[(unsigned char) *--ip];		/* Input #4 */
			if (v < 0) 
				return 0;
			i |= v << 7;					/* had 7 bits */
			*--op = i & 0xff;				/* Output #3 */
			i >>= 8;						/* lower <0123> of output #2 */
			/* FALLTHROUGH */
	case 4:
			v = valmap[(unsigned char) *--ip];		/* Input #3 */
			if (v < 0) 
				return 0;
			i |= v << 4;					/* had 4 bits */
			*--op = i & 0xff;				/* Output #2 */
			i >>= 8;						/* lower <0> of output #1 */
			/* FALLTHROUGH */
	case 3:
			v = valmap[(unsigned char) *--ip];		/* Input #2 */
			if (v < 0) 
				return 0;
			i |= v << 1;					/* had 1 bit */
			/* FALLTHROUGH */
	case 2:
			v = valmap[(unsigned char) *--ip];		/* Input #1 */
			if (v < 0) 
				return 0;
			i |= v << 6;					/* had 6 bits */
			*--op = i & 0xff;				/* Output #1 */
			i >>= 8;						/* lower <012> of output #0 */
			/* FALLTHROUGH */
	case 1:
			v = valmap[(unsigned char) *--ip];		/* Input #0 */
			if (v < 0) 
				return 0;
			i |= v << 3;					/* had 3 bits */
			*--op = i & 0xff;				/* Output #0 */
			i >>= 8;						/* Holds nothing, MBZ */
			assert(i == 0);
			assert(op >= decbuf);
		} while (op > decbuf);
	}

	return bytes;
}
