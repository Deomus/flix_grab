// elliptic curves over GF(2^(L*K))

#ifndef __EC2M_H
#define __EC2M_H

#define MAXL 16
#define MAXK 193

class field;
class field_element;
class curve;
class full_curve_parameter;

class small_field
{
	friend class field;
#if MAXL > 16 // assumes that unsigned short is at least 16 bits
	typedef unsigned lunit;
#else
#if MAXL > 8
	typedef unsigned short lunit;
#else
	typedef unsigned char lunit;
#endif
#endif

// construction
public:
	small_field( unsigned L, unsigned root );
	~small_field();

// methods
public:
	unsigned * curve_order();

// properties
public:
	lunit * const alog; // index range is [0..(BASE-2)]
	lunit * const log;  // index range is [1..(BASE-1)], but log[0] is set to (BASE-1)
	const unsigned L, BASE, BASE_M1;
};

// elements are polynomials with coefficients in small_field
class field : public small_field
{
	friend class curve;
	friend class field_element;
	friend class full_curve_parameter;

// construction
public:
	field( class full_curve_parameter & a );
	virtual unsigned rand( unsigned base );

// methods
private:
	typedef unsigned poly[ 2 * MAXK ];

	static void add( const poly a, const poly b, poly c );
	static void copy ( const poly a, poly b );
	static int  equal( const poly a, const poly b );
	void div( poly a, unsigned b );
	void set_random( poly a );
	int  set_K( unsigned K, unsigned T );
	void reduce( poly a );
	void mul( const poly a, const poly b, poly c );
	void square( const poly a, poly b );
	void inverse( const poly a, poly b );
	int  trace( const poly a );
	int  slow_trace( const poly a );
	void quad_solve( const poly a, poly b );
	void sqrt( const poly a, poly b );
	void addmul( poly a, unsigned alpha, unsigned j, const poly b );

	friend field_element sqrt( const field_element x );
	void   unpack( const bigint & x, poly a );
	bigint pack( const poly a );

// properties
private:
	const unsigned M, K, T;
	unsigned prng;
	poly nzt; // element with non-zero trace
	poly tm;  // trace mask ( trace(x) is number of bits in x & tm )
};

class field_element
{
	friend class field;
	friend class curve;
	friend class point;
	friend field_element sqrt( const field_element x );

// construction
public:
	field_element( const field_element & x );
	field_element( field * F );
	field_element();

// methods
public:
	int operator == ( const field_element & x ) const;
	int operator == ( unsigned x ) const;
	field_element operator + ( const field_element & x ) const;
	field_element operator * ( const field_element & x ) const;
	field_element operator / ( const field_element & x ) const;
	field_element & operator = ( const field_element & x );

// properties
private:
	field * f;
	field::poly v;
};

class point
{
	friend class curve;
	friend class ec_crypt;
	friend point operator * ( const bigint & k, const point & P );

// construction
public:
	point();
	point( const point & P );
	point( curve * C );

// methods
public:
	point & operator = ( const point & P );
	point operator + ( const point & P ) const;
	point operator - ( const point & P ) const;

// properties
private:
	curve * c;
	field_element x, y;
};

struct curve_parameter
{
	unsigned L;
	unsigned K;
	unsigned T;
	unsigned root;
	unsigned b;
	unsigned nso;
	unsigned ntf;
};

class full_curve_parameter : public curve_parameter
{
// construction
public:
	full_curve_parameter( const curve_parameter & bp );

// properties
public:
	bigint tm, p0, P0;
};

class curve : public field
{
	friend class curve_factory;
	friend class point;
	friend point operator * ( const bigint & k, const point & P );

// construction
public:
	curve( full_curve_parameter & a );

// methods
public:
	point  random_point();
	static bigint pack( const point & P );
	point  unpack( const bigint & X );
	static bigint to_vlong( const point & P );

// methods
private:
	void add( const point & P, const point & Q, point & R );
	void sub( const point & P, const point & Q, point & R );
	void mul( const point & P, const bigint & x, point & Q );
	int  calc_y( point & R, unsigned ybit=0 );
	static int MOV( unsigned B, const bigint & q, const bigint & r );
	static bigint small_lucas( bigint P, bigint Z, unsigned ik );
	static unsigned ybit( const field_element & x );
	static field_element sq( const field_element & x );

// properties
public:
	point PP; // point with prime order
	bigint p; // prime order of P

// properties
private:
	field_element b;
};

class curve_factory // Used for calculating curve_parameter but in practice
					// use pre-calculated table ncdata.hpp or equivalent
{
// construction
public:
	curve_factory( unsigned L ); // can take a long time
	~curve_factory();

// methods
public:
	int find( unsigned K, curve_parameter & result );

// properties
private:
	unsigned L;
	unsigned root;
	unsigned so_min;
	unsigned so_max;
	unsigned *so_set;
	bigint comp;
};

struct pair 
{
	bigint r;
	bigint s;
};

class ECC_BASE : private curve
{
// construction
public:
	ECC_BASE( full_curve_parameter & a );
	~ECC_BASE();

// methods
public:
	virtual unsigned rand( unsigned base );

	bigint create_private_key();
	bigint create_public_key( const bigint & private_key );

	bigint encrypt( const bigint & public_key, bigint & message );
	bigint decrypt( const bigint & private_key, const bigint & message );

	pair schnorr_sign( const bigint & msg, const bigint & private_key, bigint (*hash)(const bigint &, const bigint &) = 0, unsigned hashbits = 1 );
	bool schnorr_verify( const bigint & msg, const pair & sig, const bigint & public_key, bigint (*hash)(const bigint &, const bigint &) = 0, unsigned hashbits = 1 );

	pair dsa_sign( const bigint & msg, const bigint & private_key, bigint (*hash)(const bigint &, const bigint &));
	bool dsa_verify( const bigint & msg, const pair & sig, const bigint & public_key, bigint (*hash)(const bigint &, const bigint &));

// properties
public:
	const unsigned bits; // number of bits in prime order
#ifdef WIN32
	HCRYPTPROV m_cryptProv;
#endif
};

#endif
