#include "precomp.h"

#include "bigint.h"
#include "rand.h"

#ifdef _WIN32
#include <windows.h>
#include <wincrypt.h>
#endif

#include <stdlib.h>

random::random()
{
#ifdef _WIN32
   	if ( !CryptAcquireContext( (HCRYPTPROV *)&m_hCryptProv, NULL, MS_DEF_PROV, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT ) )
		m_hCryptProv = NULL;
#else
	::srand(time(NULL));
#endif
}

random::~random()
{
#ifdef  _WIN32
	if ( m_hCryptProv )
		CryptReleaseContext( (HCRYPTPROV)m_hCryptProv, 0L );
#endif
}

unsigned random::rand( unsigned max )
{
	unsigned rnd;

#ifdef _WIN32
	if ( m_hCryptProv )
		CryptGenRandom( (HCRYPTPROV)m_hCryptProv, sizeof( unsigned ), (BYTE *)&rnd );
#else
	rnd = ::rand();
#endif

	return rnd % max;
}

bigint random::rand( bigint & max )
{
	unsigned n = ( max.bits() + ( ( sizeof( unsigned ) << 3 ) - 1 ) ) / ( sizeof( unsigned ) << 3 );
	unsigned * buf = new unsigned[ n ];
	if ( !buf ) return max - 1;

	bigint rnd;

#ifdef _WIN32
	if ( m_hCryptProv )
		CryptGenRandom( (HCRYPTPROV)m_hCryptProv, n * sizeof( unsigned ), (BYTE *)buf );
#else
	for (unsigned i = 0; i < n; i++)
		*((unsigned *)buf + i) = ::rand();
#endif

	rnd.load( buf, n );

	return rnd % max;
}
