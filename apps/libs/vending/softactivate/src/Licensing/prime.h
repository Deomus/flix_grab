#ifndef __PRIME_H
#define __PRIME_H

class prime_factory
{
// construction
public:
	prime_factory( unsigned MP = 2000 ); // sieve size
	~prime_factory();

// methods
public:
	bigint find_prime( bigint start );
	int    make_prime( bigint & r, bigint &k, const bigint & rmin );
	bigint find_special( bigint start, bigint base=0 );

// properties
public:
	unsigned np;
	unsigned *pl;
};

int is_probable_prime( const bigint &p );

#endif
