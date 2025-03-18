#ifndef __BIGINT_H
#define __BIGINT_H

class bigint;
class bigint_value;

class bigint_unit // Provides storage allocation and index checking
{
friend class bigint_value;
friend class bigint;

public:
	bigint_unit();
	~bigint_unit();

public:
	void     clear(); // set n to zero
	unsigned get( unsigned i ) const;     // get ith unsigned
	void     set( unsigned i, unsigned x );   // set ith unsigned
	void     reserve( unsigned x );           // storage hint
	void     fast_mul( bigint_unit & x, bigint_unit & y, unsigned n ); // Time critical routine

private:
	unsigned n; // used units (read-only)
	unsigned * a; // array of units
	unsigned z; // units allocated
};

class bigint_value : public bigint_unit 
{
friend class bigint;

public:
	bigint_value();

public:
	int      is_zero() const;
	unsigned bit( unsigned i ) const;
	void     setbit( unsigned i );
	void     clearbit( unsigned i );
	unsigned bits() const;
	int      cf( bigint_value& x ) const;
	int		 product( bigint_value &x ) const;
	void	 shl();
	int  shr(); // result is carry
	void	 shr( unsigned n );
	void	 add( bigint_value& x );
    void	 _xor( bigint_value& x );
	void	 _and( bigint_value& x );
	void	 subtract( bigint_value& x );
	void	 init( unsigned x );
	void	 copy( bigint_value& x );
	unsigned to_unsigned(); // Unsafe conversion to unsigned
	void	 mul( bigint_value& x, bigint_value& y );
	void	 divide( bigint_value& x, bigint_value& y, bigint_value& rem );

private:
	unsigned share; // share count, used by bigint to delay physical copying
};

class bigint // very long integer - can be used like long
{
public:
	// Construction and 
	bigint ( unsigned x = 0 );
	bigint ( const bigint & x );
	~bigint();

public:
	// Standard arithmetic operators
	friend bigint operator +( const bigint& x, const bigint& y );
	friend bigint operator -( const bigint& x, const bigint& y );
	friend bigint operator *( const bigint& x, const bigint& y );
	friend bigint operator /( const bigint& x, const bigint& y );
	friend bigint operator %( const bigint& x, const bigint& y );
	friend bigint operator ^( const bigint& x, const bigint& y );
	friend bigint pow2( unsigned n );
	friend bigint operator &( const bigint& x, const bigint& y );

	friend bigint operator <<( const bigint& x, unsigned n );

	bigint & operator +=( const bigint& x );
	bigint & operator -=( const bigint& x );
	bigint & operator >>=( unsigned n );

	// Standard comparison operators
	friend int operator !=( const bigint& x, const bigint& y );
	friend int operator ==( const bigint& x, const bigint& y );
	friend int operator >=( const bigint& x, const bigint& y );
	friend int operator <=( const bigint& x, const bigint& y );
	friend int operator > ( const bigint& x, const bigint& y );
	friend int operator < ( const bigint& x, const bigint& y );

	// Absolute value
	friend bigint abs( const bigint & x );

	// conversion operations
	friend unsigned to_unsigned( const bigint &x );
	bigint & operator =(const bigint& x);

	// Bit operations
	unsigned bits() const;
	unsigned bit(unsigned i) const;
	void setbit(unsigned i);
	void clearbit(unsigned i);
	bigint & operator ^=( const bigint& x );
	bigint & operator &=( const bigint& x );
	bigint & ror( unsigned n ); // single  bit rotate
	bigint & rol( unsigned n ); // single bit rotate
	friend int product( const bigint & x, const bigint & y ); // parity of x&y

	void load( const unsigned * a, unsigned n ); // load value, a[0] is lsw
	void store( unsigned * a, unsigned n ) const; // low level save, a[0] is lsw

private:
	bigint_value * value;
	int negative;
	int cf( const bigint & x ) const;
	void docopy();
	friend class monty;
};

bigint modexp( const bigint & x, const bigint & e, const bigint & m ); // m must be odd
bigint gcd( const bigint & X, const bigint & Y ); // greatest common denominator
bigint modinv( const bigint & a, const bigint & m ); // modular inverse
bigint lucas ( const bigint & P, const bigint & Z, const bigint & k, const bigint & p ); // P^2 - 4Z != 0
bigint sqrt( const bigint & g, const bigint & p ); // square root mod p
bigint pow2( unsigned n );

#endif
