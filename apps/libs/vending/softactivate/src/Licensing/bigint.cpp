//
// Copyright (c) 2014 FreeGrabApp. All rights reserved.
//

#include "precomp.h"
#include "bigint.h"

unsigned char bittab[256] =
{ 0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8 };

unsigned bigint_unit::get( unsigned i ) const
{
    if ( i >= n )
        return 0;
    
    return a[ i ];
}

void bigint_unit::clear()
{
    n = 0;
}

bigint_unit::bigint_unit()
{
    z = 0;
    a = 0;
    n = 0;
}

bigint_unit::~bigint_unit()
{
    unsigned i = z;

    while ( i )
    {
        i -= 1;
        a[i] = 0;
    }

    delete [] a;
}

void bigint_unit::reserve( unsigned x )
{
    if ( x > z )
    {
        unsigned * na = new unsigned[ x ];

        for ( unsigned i = 0; i < n; i += 1 ) 
            na[ i ] = a[ i ];

        delete [] a;
        a = na;
        z = x;
    }
}

void bigint_unit::set( unsigned i, unsigned x )
{
    if ( i < n )
    {
        a[ i ] = x;

        if ( x == 0 )
        {
            while ( n && a[ n - 1 ] == 0 ) 
                n -= 1; // normalise
        }
    }
    else if ( x )
    {
        reserve( i + 1 );

        for ( unsigned j = n; j < i; j += 1 )
            a[j] = 0;

        a[i] = x;
        n = i + 1;
    }
}

// Macros for doing double precision multiply
#define BPU ( 8 * sizeof( unsigned ) )       // Number of bits in an unsigned
#define lo(x) ( (x) & ( ( 1 << ( BPU / 2 ) ) -1 ) ) // lower half of unsigned
#define hi(x) ( (x) >> ( BPU / 2 ) )         // upper half
#define lh(x) ( (x) << ( BPU / 2 ) )         // make upper half

unsigned do_inner( unsigned n, unsigned m, unsigned * a, unsigned * ya )
{
    if (a == NULL || ya == NULL)
        return 0;

    unsigned c = 0;

#if defined(_MSC_VER) && defined(_M_IX86)
    __asm mov edi, a
    __asm mov esi, ya
loop1:  
    __asm mov eax, [esi]
    __asm mul m
    __asm add esi, 4
    __asm add eax, c
    __asm adc edx, 0
    __asm add [edi], eax
    __asm adc edx, 0
    __asm add edi, 4
    __asm mov c, edx
    __asm dec n
    __asm jnz loop1
#else
    while ( n-- )
    {
        unsigned v = *a, 
                 p = *ya++;
        v += c; 
        c = ( v < c );
        unsigned w;
        w = lo( p ) * lo( m ); 
        v += w; 
        c += ( v < w );
        w = lo( p ) * hi( m ); 
        c += hi( w ); 
        w = lh( w ); 
        v += w; 
        c += ( v < w );
        w = hi( p ) * lo( m );
        c += hi( w ); 
        w = lh( w ); 
        v += w; 
        c += ( v < w );
        c += hi( p ) * hi( m );
        *a++ = v;
    }
#endif

    return c;
}

void bigint_unit::fast_mul( bigint_unit &x, bigint_unit &y, unsigned keep )
{
    unsigned i,
             limit = ( keep + BPU - 1 ) / BPU; // size of result in words

    reserve( limit );

    for ( i = 0; i < limit; i += 1) 
        a[i] = 0;

    unsigned min = x.n;

    if ( min > limit ) 
        min = limit;

    for ( i = 0; i < min; i += 1 )
    {
        unsigned m = x.a[ i ];
        unsigned min = i + y.n; 
        
        if ( min > limit ) 
            min = limit;

        unsigned c = do_inner( min - i, m, a + i, y.a );
        
        unsigned j = min;    

        while ( c && j < limit )
        {
            a[ j ] += c;
            c = a[ j ] < c;
            j += 1;
        }
    }

    // eliminate unwanted bits
    keep %= BPU;
    if ( keep )
        a[ limit-1 ] &= ( 1 << keep ) - 1;

    // calculate n
    while ( limit && a[ limit-1 ] == 0 )
        limit -= 1;

    n = limit;
};

unsigned bigint_value::to_unsigned()
{
    return get( 0 );
}

int bigint_value::is_zero() const
{
    return n == 0;
}

unsigned bigint_value::bit( unsigned i ) const
{ 
    return ( get( i / BPU ) & ( 1 << ( i % BPU ) ) ) != 0;
}

void bigint_value::setbit( unsigned i )
{
    set( i / BPU, get( i / BPU ) | ( 1 << i % BPU ) );
}

void bigint_value::clearbit( unsigned i )
{
    set( i / BPU, get( i / BPU ) & ~( 1 << i % BPU ) );
}

unsigned bigint_value::bits() const
{
    unsigned x = n;

    if ( x )
    {
        unsigned w = BPU;
        unsigned msw = get( x-1 );

        x = ( x-1 ) * BPU;

        do
        {
            w >>= 1;
            if ( msw >= ( 1u << w ) )
            {
                x += w;
                msw >>= w;
            }
        } while ( w > 8 );

        x += bittab[ msw ];
    }

    return x;
}

int bigint_value::cf( bigint_value & x ) const
{
    if ( n > x.n ) 
        return +1;

    if ( n < x.n ) 
        return -1;

    unsigned i = n;

    while ( i )
    {
        i -= 1;

        if ( get( i ) > x.get( i ) ) 
            return +1;

        if ( get( i ) < x.get( i ) ) 
            return -1;
    }

    return 0;
}

void bigint_value::shl()
{
    unsigned carry = 0;
    unsigned N = n; // necessary, since n can change

    for ( unsigned i = 0; i <= N; i += 1 )
    {
        unsigned u = get( i );
        set( i, ( u << 1 ) + carry );
        carry = u >> ( BPU - 1 );
    }
}

int bigint_value::shr()
{
    unsigned carry = 0;
    unsigned i = n;

    while ( i )
    {
        i -= 1;
        unsigned u = get( i );
        set( i,( u >> 1 ) + carry );
        carry = u << ( BPU - 1 );
    }

    return carry != 0;
}

void bigint_value::shr( unsigned x )
{
    unsigned delta = x / BPU; 

    x %= BPU;

    for ( unsigned i = 0; i < n; i += 1 )
    {
        unsigned u = get( i + delta );

        if ( x )
        {
            u >>= x;
            u += get( i + delta + 1 ) << ( BPU-x );
        }

        set( i, u );
    }
}

void bigint_value::add( bigint_value & x )
{
    unsigned carry = 0;
    unsigned max = n; 

    if ( max < x.n ) 
        max = x.n;

    reserve( max );

    for ( unsigned i = 0; i < max + 1; i += 1 )
    {
        unsigned u = get( i );
        u = u + carry; 
        carry = ( u < carry );
        unsigned ux = x.get( i );
        u = u + ux; carry += ( u < ux );
        set( i,u );
    }
}

void bigint_value::_xor( bigint_value & x )
{
    unsigned max = n; 
    
    if (max<x.n) 
        max = x.n;
    
    reserve(max);

    for ( unsigned i = 0; i < max; i += 1 )
    {
        set( i, get( i ) ^ x.get( i ) );
    }
}

void bigint_value::_and( bigint_value & x )
{
    unsigned max = n; 

    if ( max < x.n ) 
        max = x.n;

    reserve( max );

    for ( unsigned i = 0; i < max; i += 1 )
    {
        set( i, get( i ) & x.get( i ) );
    }
}

int bigint_value::product( bigint_value & x ) const
{
    unsigned max = n;
    unsigned tmp = 0;
    unsigned count = 0;

    if ( max < x.n ) max = x.n;

    for ( unsigned i = 0; i < max; i += 1 )
    {
        tmp ^= get( i ) & x.get( i );
    }

    while ( tmp )
    {
        if ( tmp & 1 ) count += 1;
        tmp >>= 1;
    }

    return count & 1;
}

void bigint_value::subtract( bigint_value & x )
{
    unsigned carry = 0;
    unsigned N = n;

    for ( unsigned i = 0; i < N; i += 1 )
    {
        unsigned ux = x.get( i );
        ux += carry;
        if ( ux >= carry )
        {
            unsigned u = get( i );
            unsigned nu = u - ux;
            carry = nu > u;
            set( i, nu );
        }
    }
}

void bigint_value::init( unsigned x )
{
    clear();
    set( 0, x );
}

void bigint_value::copy( bigint_value & x )
{
    unsigned i = x.n;

    clear();

    while ( i ) 
    { 
        i -= 1; 
        set( i, x.get( i ) ); 
    }
}

bigint_value::bigint_value()
{
    share = 0;
}

void bigint_value::mul( bigint_value & x, bigint_value & y )
{
    fast_mul( x, y, x.bits() + y.bits() );
}

void bigint_value::divide( bigint_value & x, bigint_value & y, bigint_value & rem )
{
    bigint_value m, s;

    init( 0 );
    rem.copy( x );
    m.copy( y );
    s.init( 1 );

    int i = 0;

    while ( rem.cf( m ) > 0 )
    {
        m.shl();
        s.shl();

        i++;
    }

    while ( rem.cf( y ) >= 0 )
    {
        while ( rem.cf( m ) < 0 )
        {
            m.shr();
            s.shr();
        }

        rem.subtract( m );

        add( s );
    }
}

// Implementation of bigint

int operator !=( const bigint& x, const bigint& y )
{ 
    return x.cf( y ) != 0; 
}

int operator ==( const bigint& x, const bigint& y )
{ 
    return x.cf( y ) == 0; 
}

int operator >=( const bigint& x, const bigint& y )
{ 
    return x.cf( y ) >= 0; 
}

int operator <=( const bigint& x, const bigint& y )
{ 
    return x.cf( y ) <= 0; 
}

int operator > ( const bigint& x, const bigint& y )
{ 
    return x.cf( y ) > 0; 
}

int operator < ( const bigint& x, const bigint& y )
{ 
    return x.cf( y ) < 0; 
}

void bigint::load( const unsigned * a, unsigned n )
{
    docopy();
    value->clear();
    for ( unsigned i = 0; i < n; i += 1 )
        value->set( i, a[ i ] );
}

void bigint::store( unsigned * a, unsigned n ) const
{
    for ( unsigned i = 0; i < n; i += 1 )
        a[ i ] = value->get( i );
}

void bigint::docopy()
{
    if ( value->share )
    {
        value->share -= 1;
        bigint_value * nv = new bigint_value;
        nv->copy( *value );
        value = nv;
    }
}

unsigned bigint::bits() const
{
    return value->bits();
}

unsigned bigint::bit( unsigned i ) const
{
    return value->bit( i );
}

void bigint::setbit( unsigned i )
{
    docopy();
    value->setbit( i );
}

void bigint::clearbit( unsigned i )
{
    docopy();
    value->clearbit( i );
}

int bigint::cf( const bigint & x ) const
{
    int neg = negative && !value->is_zero();

    if ( neg == ( x.negative && !x.value->is_zero() ) )
        return value->cf( *x.value );
    else 
    if ( neg ) 
        return -1;
     else 
        return +1;
}

bigint::bigint( unsigned x )
{
    value = new bigint_value;
    negative = 0;
    value->init( x );
}

bigint::bigint( const bigint& x ) // copy constructor
{
    negative = x.negative;
    value = x.value;
    value->share += 1;
}

bigint & bigint::operator =( const bigint & x )
{
    if ( value->share ) 
        value->share -=1;
    else 
        delete value;

    value = x.value;
    value->share += 1;
    negative = x.negative;

    return *this;
}

bigint::~bigint()
{
    if ( value->share ) 
        value->share -= 1; 
    else 
        delete value;
}

unsigned to_unsigned ( const bigint & x ) // conversion to unsigned
{
    return x.value->to_unsigned();
}

bigint & bigint::operator ^=( const bigint& x )
{
    docopy();
    value->_xor( *x.value );

    return *this;
}

bigint & bigint::operator &=( const bigint & x )
{
    docopy();
    value->_and( *x.value );

    return *this;
}

bigint & bigint::ror( unsigned n )
{
    docopy();
    int carry = value->shr();
    if ( carry )
        value->setbit( n - 1 );

    return *this;
}

bigint & bigint::rol( unsigned n )
{
    docopy();
    int carry = value->bit( n - 1 );
    if ( carry ) value->clearbit( n - 1 );
    value->shl();
    if ( carry ) value->setbit( 0 );
    
    return *this;
}

bigint & bigint::operator >>=( unsigned n ) // divide by 2**n
{
    docopy();
    value->shr(n);

    return *this;
}

bigint & bigint::operator +=( const bigint & x )
{
    if ( negative == x.negative )
    {
        docopy();
        value->add( *x.value );
    }
    else if ( value->cf( *x.value ) >= 0 )
    {
        docopy();
        value->subtract( *x.value );
    }
    else
    {
        bigint tmp = *this;
        *this = x;
        *this += tmp;
    }

    return *this;
}

bigint & bigint::operator -=( const bigint & x )
{
    if ( negative != x.negative )
    {
        docopy();
        value->add( *x.value );
    }
    else if ( value->cf( *x.value ) >= 0 )
    {
        docopy();
        value->subtract( *x.value );
    }
    else
    {
        bigint tmp = *this;
        *this = x;
        *this -= tmp;
        negative = 1 - negative;
    }

    return *this;
}

bigint operator +( const bigint & x, const bigint & y )
{
    bigint result = x;

    result += y;

    return result;
}

bigint operator -( const bigint & x, const bigint & y )
{
    bigint result = x;
    
    result -= y;
    
    return result;
}

bigint operator *( const bigint & x, const bigint & y )
{
    bigint result;

    result.value->mul( *x.value, *y.value );
    result.negative = x.negative ^ y.negative;

    return result;
}

bigint operator /( const bigint & x, const bigint & y )
{
    bigint result;
    bigint_value rem;

    result.value->divide( *x.value, *y.value, rem );
    result.negative = x.negative ^ y.negative;

    return result;
}

bigint operator %( const bigint& x, const bigint& y )
{
    bigint result;
    bigint_value divide;
    
    divide.divide( *x.value, *y.value, *result.value );
    result.negative = x.negative; // not sure about this?

    return result;
}

bigint operator ^( const bigint& x, const bigint& y )
{
    bigint result = x;

    result ^= y;

    return result;
}

bigint operator &( const bigint & x, const bigint & y )
{
    bigint result = x;

    result &= y;

    return result;
}

bigint operator <<( const bigint & x, unsigned n ) // multiply by 2**n
{
    bigint result = x;

    while ( n )
    {
        n -= 1;
        result += result;
    }

    return result;
}

bigint abs( const bigint & x )
{
    bigint result = x;

    result.negative = 0;

    return result;
}

int product ( const bigint & X, const bigint & Y )
{
    return X.value->product( *Y.value );
}

bigint pow2( unsigned n )
{
    bigint result;

    result.setbit( n );

    return result;
}

bigint gcd( const bigint &X, const bigint &Y )
{
    bigint x = X, 
           y = Y;

    while ( 1 )
    {
        if ( y == 0 ) return x;
        x = x % y;
        if ( x == 0 ) return y;
        y = y % x;
    }
}

bigint modinv( const bigint & a, const bigint & m ) // modular inverse
// returns i in range 1..m-1 such that i*a = 1 mod m
// a must be in range 1..m-1
{
    bigint j = 1, 
           i = 0, 
           b = m, 
           c = a, 
           x, 
           y;

    while ( c != 0 )
    {
        x = b / c;
        y = b - x * c;
        b = c;
        c = y;
        y = j;
        j = i - j * x;
        i = y;
    }

    if ( i < 0 )
        i += m;

    return i;
}

class monty // class for montgomery modular exponentiation
{
    bigint m, n1;
    bigint T, k;   // work registers
    unsigned N;  // bits for R
    void mul( bigint & x, const bigint & y );
public:
    bigint R, R1;
    bigint exp( const bigint & x, const bigint & e );
    bigint monty_exp( const bigint & x, const bigint & e );
    monty( const bigint & M );
};

monty::monty( const bigint & M )
{
    m = M;
    N = 0; 
    R = 1;

    while ( R < M ) 
    { 
        R += R; 
        N += 1; 
    }

    R1 = modinv( R - m, m );
    n1 = R - modinv( m, R );
}

void monty::mul( bigint & x, const bigint & y )
{
    // T = x*y;
    T.value->fast_mul( *x.value, *y.value, N * 2 );

    // k = ( T * n1 ) % R;
    k.value->fast_mul( *T.value, *n1.value, N );

    // x = ( T + k*m ) / R;
    x.value->fast_mul( *k.value, *m.value, N * 2 );
    x += T;
    x.value->shr( N );

    if ( x >= m ) x -= m;
}

bigint monty::monty_exp( const bigint & x, const bigint & e )
{
    bigint result = R - m, 
           t = x; // don't convert input
    unsigned bits = e.value->bits();
    unsigned i = 0;

    t.docopy(); // careful not to modify input

    while ( 1 )
    {
        if ( e.value->bit( i ) )
            mul( result, t );
        i += 1;
        if ( i == bits ) break;
        mul( t, t );
    }

    return result; // don't convert output
}

bigint monty::exp( const bigint & x, const bigint & e )
{
    return ( monty_exp( ( x * R ) % m, e ) * R1 ) % m;
}

bigint modexp( const bigint & x, const bigint & e, const bigint & m )
{
    monty me( m );
    return me.exp( x, e );
}

static bigint half( const bigint & a, const bigint & p )
{
    if ( a.bit( 0 ) )
        return ( a + p ) / 2;

    return a / 2;
}

bigint lucas ( const bigint & P, const bigint & Z, const bigint & k, const bigint & p ) // P^2 - 4Z != 0
{
    bigint D = P * P - 4 * Z, 
           U = 1, 
           V = P, 
           U1, 
           V1;
    unsigned i = k.bits() - 1;

    while ( i )
    {
        i -= 1;
        
        U1 = U * V; 
        V1 = V * V + D * U * U; 
        U = U1 % p;
        V = half( V1 % p, p );

        if ( k.bit( i ) )
        {
            U1 = P * U + V; 
            V1 = P * V + D * U; 
            U = half( U1 % p, p ); 
            V = half( V1 % p, p );
        }
    }

    return V;
}

bigint sqrt( const bigint & g, const bigint & p )
{
    bigint result;

    if ( p % 4 == 3 )
        result = modexp( g, p / 4 + 1, p );
    else if ( p % 8 == 5 )
    {
        bigint gamma = modexp( 2 * g, p / 8, p);
        bigint i = 2 * g * gamma * gamma - 1;
        result = g * gamma * i;
    }
    else
    {
        bigint Z = g;
        bigint P = 1;

        while ( 1 )
        {
            bigint D = ( P * P - 4 * g ) % p; 

            if ( D < 0 ) D += p;

            if ( D == 0 )
            {
                result = half( P, p );
                break;
            }

            if ( modexp( D, ( p - 1 ) / 2, p ) != 1 )
            {
                result = half( lucas( P, Z, ( p + 1 ) / 2, p ), p );
                break;
            }

            P += 1; // Is this ok (efficient?)
        }
    }

    result = result % p;
    if ( result < 0 ) result += p;

    return result;
}
