//
// Copyright (c) 2014 FreeGrabApp. All rights reserved.
//

// elliptic curve implementation
#include "precomp.h"
#include "bigint.h"
#include "ec2m.h"
#include "prime.h"
#include <stdlib.h>
#include <time.h>

#define assert_on        0      // Consistency checking (for debugging)
#define use_field_op     0      // Use operators

#if assert_on
#include "prime.hpp"
static void assert0(int x){ if (x==0) while (1/x); }
#define assert(x) assert0(x)
#else
//#define assert(x)
#endif

#if 0
void biPrint2( const bigint & data )
{
    int n;
    unsigned * u;

    n = ( data.bits() + 31 ) / 32;

    u = new unsigned[ n ];

    data.store( u, n );

    printf( "%d ", data.bits() );

    for ( int i = n - 1; i >= 0; i-- )
    {
        printf( "%08X ", u[i] );
    }

    printf("\n");

    delete [] u;
}
#endif

// polynomial operations

int is_prime( unsigned p )
{
    for ( unsigned d = 2; d * d <= p; d += 1 )
        if ( p % d == 0 ) 
            return 0;

    return 1;
}

// Routines to perform operations on polynomials
// over F2 represented as bit strings (bigint)

bigint poly_div( const bigint & x, const bigint & y )
{
    unsigned m = y.bits() - 1;
    bigint d = 0;
    bigint rem = x;

    while ( 1 )
    {
        unsigned rb = rem.bits();

        if ( rb <= m ) 
            break;

        rem ^= y << ( rb - m - 1 );
        d ^= 1 << ( rb - m - 1 );
    }

    return d;
}

bigint poly_rem( const bigint & x, const bigint & y )
{
    unsigned m = y.bits() - 1;
    bigint rem = x;
    
    while ( 1 )
    {
        unsigned rb = rem.bits();
        
        if ( rb <= m ) 
            break;

        rem ^= y << ( rb - m - 1 );
    }

    return rem;
}

bigint poly_mul( const bigint & x, const bigint & y )
{
    bigint result = 0;
    unsigned yb = y.bits();

    for ( unsigned i = 0; i < yb; i += 1 )
    {
        if ( y.bit( i ) )
            result ^= x << i;
    }

    return result;
}

bigint poly_gcd( const bigint &x, const bigint &y ) // field is F2
{
    bigint a = x;
    bigint b = y;

    while ( b != 0 )
    {
        bigint c = poly_rem( a, b );
        a = b;
        b = c;
    }

    return a;
}

int poly_irreducible( const bigint & x )
{
    unsigned d = x.bits() - 1;
    bigint u = 2;

    for ( unsigned i = 1; i < d; i += 1 )
    {
        u = poly_rem( poly_mul( u, u ), x );
        bigint g = poly_gcd( u^2, x );

        if ( g != 1 ) 
            return 0;
    }

    return 1;
}

bigint poly_power( bigint x, bigint k, bigint g )
{
    bigint result = 1;

    for ( unsigned i = 0; i < k.bits(); i += 1 )
    {
        if ( k.bit( i ) )
        {
            result = poly_rem( poly_mul( result, x ), g );
        }

        x = poly_rem( poly_mul( x, x ), g );
    }

    return result;
}

unsigned calc_root( unsigned L )
{
    // find irreducible poly g such that 2 is primitive
    bigint base = pow2( L );

    for ( bigint g = base + 1; g < 2 * base; g += 2 ) 
        if ( poly_irreducible( g ) )
        {
            bigint r = base - 1;
            bigint f = 2;

            while ( 1 )
            {
                if ( f != base - 1 && r % f == 0 )
                {
                    if ( poly_power( 2, ( base - 1 ) / f, g ) == 1 ) 
                        break;

                    do {
                        r = r / f; 
                    } while ( r % f == 0 );
                }

                if ( f >= r ) 
                    return to_unsigned( g - base );

                if ( f == 2 ) 
                    f = 3; 
                else 
                    f += 2;

                if ( f * f > r ) 
                    f = r;
            }
        }

    return 0; // should not happen, at least for reasonable L
}

// small_field implementation

small_field::small_field( unsigned L0, unsigned root )
: L( L0 ),
BASE( 1u << L0 ),
BASE_M1( ( 1u << L0 ) - 1 ),
log( new lunit[ 1u << L0 ] ),
alog( new lunit[ 1u << L0 ] )
{
    assert( L0 <= sizeof(lunit) * 8 );

    root |= BASE;
    unsigned j = 1;

    for ( unsigned i = 0; i < BASE_M1; i += 1 )
    {
        log[ j ] = i;
        alog[ i ] = j;
        j *= 2; // 2 is primitive element
        if ( j & BASE )
            j ^= root;
    }

    log[ 0 ] = BASE_M1; // used by field::mul
}

small_field::~small_field()
{
    delete [] log;
    delete [] alog;
}

unsigned * small_field::curve_order()
{
    unsigned * result = new unsigned[ BASE ]; // result[0] not defined
    unsigned b, x, y, i;

    for ( b = 1; b < BASE; b += 1 )
        result[ b ] = 2; /* 0,0 and 0, sqrt(b) */

    for ( x = 1; x < BASE; x += 1 )
    {
        unsigned lx = log[ x ];
        unsigned x3 = alog[ ( 3 * lx ) % BASE_M1 ];
        result[ x3 ] += 2; // y = 0 and y = x

        for ( y = 1; y < x; y += 1 )
        {
            i = log[ y ] + log[ x^y ];

            if ( i >= BASE_M1 ) 
                i -= BASE_M1;

            b = x3 ^ alog[ i ];
            result[ b ] += 1;
        }

        for ( y = x + 1; y < BASE; y += 1 )
        {
            i = log[ y ] + log[ x^y ];

            if ( i >= BASE_M1 ) 
                i -= BASE_M1;

            b = x3 ^ alog[ i ];
            result[ b ] += 1;
        }
    }

    return result;
}

// field implementation

void field::add( const poly a, const poly b, poly c )
{
    if ( a[ 0 ] < b[ 0 ] )
        add( b, a, c );
    else
    {
        unsigned i;

        for ( i = 0; i <= b[ 0 ]; i += 1 )
            c[ i ] = a[ i ] ^ b[ i ];

        for ( ; i <= a[ 0 ]; i += 1 )
            c[i] = a[i];

        c[ 0 ] = a[ 0 ];
    }

    while ( c[ 0 ] && c[ c[ 0 ] ] == 0 )
        c[ 0 ] -= 1;
}

void field::copy ( const poly a, poly b )
{
    for ( unsigned i = 0; i <= a[ 0 ]; i += 1 )
        b[ i ] = a[ i ];
}

int field::equal( const poly a, const poly b )
{
    unsigned i = 0;

    while ( 1 )
    {
        if ( a[ i ] != b[ i ] ) 
            return 0;

        if ( i == a[ 0 ] )
            return 1;

        i += 1;
    }
}

void field::set_random( poly a )
{
    a[0] = K;

    for ( unsigned i = 1; i <= K; i += 1 )
        a[ i ] = rand( BASE );

    while ( a[ 0 ] && a[ a[ 0 ] ] == 0 )
        a[ 0 ] -= 1;
}

void field::reduce( poly a )
{
    for ( unsigned i = a[ 0 ]; i > K; i -= 1 )
    {
        a[ i - K ] = a[ i - K ] ^ a[ i ];
        a[ i + T - K ] = a[ i + T - K ] ^ a[ i ];
        a[ i ] = 0;
    }

    if ( a[0] > K )
    {
        a[0] = K;

        while ( a[ 0 ] && a[ a[ 0 ] ] == 0 ) 
            a[ 0 ] -= 1;
    }
}

// field_element implementation

field_element & field_element:: operator = ( const field_element & x )
{
    f = x.f;

    field::copy( x.v, v );

    return *this;
}

field_element::field_element()
{
    f = 0;
    v[ 0 ] = 0;
}

field_element::field_element( field * F )
{
    f = F;
    v[ 0 ] = 0;
}

field_element::field_element( const field_element & x )
{
    f = x.f;
    field::copy( x.v, v );
}

int field_element::operator == ( const field_element & x ) const
{
    return field::equal( v, x.v );
}

int field_element::operator == ( unsigned x ) const
{
    return ( v[ 0 ] == 0 && x == 0 ) || ( v[ 1 ] == 1 && x == v[ 1 ] );
}

field_element field_element::operator + ( const field_element & x ) const
{
    field_element result( f );

    field::add( v, x.v, result.v );

    return result;
}

field_element field_element::operator * ( const field_element & x ) const
{
    field_element result( f );

    f->mul( v, x.v, result.v );
    f->reduce( result.v );

    return result;
}

field_element curve::sq( const field_element & x )
{
    field_element result( x.f );

    x.f->square( x.v, result.v );
    x.f->reduce( result.v );

    return result;
}

field_element field_element::operator / ( const field_element & x ) const
{
    field_element result( f );
    field::poly   tmp;

    f->inverse( x.v, tmp );
    f->mul( v, tmp, result.v );
    f->reduce( result.v );

    return result;
}

field_element sqrt( const field_element x )
{
    field_element result( x.f );

    x.f->sqrt( x.v, result.v );

    return result;
}

// field implementation

int field::slow_trace( const poly a )
{
    field::poly t1;

    copy( a, t1 );

    for ( unsigned i = 1; i < M; i += 1 )
    {
        field::poly t2;
        square( t1, t2 );
        reduce( t2 );
        add( t2, a, t1 );
    }

    return t1[ 0 ] != 0;
}

int field::trace( const poly a )
{
    unsigned w = 0;
    unsigned min = a[ 0 ]; 

    if ( min > tm[ 0 ] ) 
        min = tm[ 0 ];

    for ( unsigned i = 1; i <= min; i += 1 )
        w ^= a[ i ] & tm[ i ];

    // compute parity of w
    unsigned result = 0;

    while ( w )
    {
        result ^= w & 1;
        w >>= 1;
    }

    assert( result == (unsigned)slow_trace( a ) );
    
    return result;
}

void field::quad_solve( const poly a, poly b )
{
    if ( M % 2 == 1 ) // M is odd - compute half-trace
    {
        copy( a, b );

        for ( unsigned i = 0; i < M / 2; i += 1 )
        {
            field::poly t1, t2;
            square( b, t1 ); reduce( t1 );
            square( t1, t2 ); reduce( t2 );
            add( t2, a, b );
        }
    }
    else
    {
        field::poly d;
        b[ 0 ] = 0;
        copy( a, d );

        for ( unsigned i = 1; i < M; i += 1 )
        {
            field::poly t1, t2;
            square( d, t1 ); reduce( t1 );
            mul( nzt, t1, t2 ); // N.B. make nzt the first parameter, since it is mostly zero
            square( b, t1 );
            add( t1, t2, b ); reduce( b );
            square( d, t1 ); reduce( t1 );
            add( t1, a, d );
        }
    }
}

unsigned field::rand( unsigned base )
{
    // A very bad prng - but should be good enough for initialisation
    // You can use whatever prng you like here
    // but it needs to be standard so that
    // the same point C.P is always generated
    // for a given root,CB

    prng += 1;

    return prng % base;
}

field::field( full_curve_parameter & a )
:small_field( a.L, a.root ), M( a.L * a.K ), K( a.K ), T( a.T )
{
    assert( K <= MAXK );

    prng = 0;

    if ( a.tm == 0 )
    {
        // Calculate trace mask
        poly x;
        for ( unsigned i = 1; i <= K; i += 1 )
        {
            x[ 0 ] = i;
            unsigned m = 1;
            unsigned w = 0;

            for ( unsigned j = 0; j < L; j += 1 )
            {
                x[ i ] = m;

                if ( slow_trace( x ) )
                    w ^= m;

                m *= 2;
            }

             x[ i ] = 0;
            tm[ i ] = w;

            if ( w )
                tm[ 0 ] = i;
        }

        a.tm = pack( tm );
    }
    else
        unpack( a.tm, tm );

    if ( M % 2 == 0 )
    {
        nzt[ 0 ] = 1;
        nzt[ 1 ] = 1;

        while ( ( nzt[ 1 ] & tm[ 1 ] ) == 0 )
            nzt[ 1 ] *= 2;

        assert( trace( nzt ) != 0 );
    }

#if assert_on // test basic field operations
    for ( unsigned i = 0; i < 10; i += 1 )
    {
        field_element a( this ), b( this ), c( this );
        
        set_random( a.v );
        set_random( b.v ); 
        set_random( c.v );
        
        assert( a + b == b + a );
        assert( a * b == b * a );
        assert( a * ( b + c ) == a * b + a * c );

        if ( b.v[ 0 ] )
            assert( a == b * ( a / b ) );
        
        assert( ::sqrt( a ) * ::sqrt( a ) == a );
    }
#endif
}

void field::mul( const poly a, const poly b, poly c )
{
    unsigned i, j;

    assert( a[0] <= K );
    assert( b[0] <= K );

    if ( a[ 0 ] == 0 || b[ 0 ] == 0 )
    {
        c[ 0 ] = 0;
        return;
    }

    c[ 0 ] = a[ 0 ] + b[ 0 ] - 1;

    for ( i = 1; i <= c[ 0 ]; i += 1 )
        c[ i ] = 0;

    // pre-computation of log[b[j]] (to reduce table lookups)
    unsigned lb[ MAXK + 1 ];
    unsigned b0 = b[ 0 ];

    for ( j = 1; j <= b0; j += 1 )
        lb[ j ] = log[ b[ j ] ];

    for ( i = 1; i <= a[ 0 ]; i += 1 )
    {
        unsigned lai = log[ a[ i ] ];
        if ( lai != BASE_M1 )
        {
            for ( j = 1; j <= b0; j += 1 )
            {
                unsigned lbj = lb[ j ];
                if ( lbj != BASE_M1 )
                {
                    unsigned x = lai + lbj;
                    
                    if ( x >= BASE_M1 ) 
                        x -= BASE_M1;

                    c[ i + j - 1 ] ^= alog[ x ];
                }
            }
        }
    }
}

void field::square( const poly a, poly c )
{
    unsigned i;

    if ( a[ 0 ] == 0 )
    {
        c[ 0 ] = 0;
        return;
    }

    c[ 0 ] = 2 * a[ 0 ] - 1;

    for ( i = 1; ; i += 1 )
    {
        unsigned x = 0;
        if ( a[ i ] )
        {
            x = 2 * log[ a[ i ] ];

            if ( x >= BASE_M1 ) 
                x -= BASE_M1;

            x = alog[ x ];
        }

        c[ 2 * i - 1 ] = x;

        if ( i == a[ 0 ] )
            break;

        c[ 2 * i ] = 0;
    }
}

void field::addmul( poly a, unsigned alpha, unsigned j, const poly b )
{
    unsigned la = log[ alpha ];

    for ( unsigned  i = 1; i <= b[ 0 ]; i += 1 )
    {
        while ( a[ 0 ] < j + i )
        {
            a[ 0 ] += 1;
            a[ a[ 0 ] ] = 0; 
        }

        unsigned x = b[i];

        if ( x )
        {
            x = log[ x ] + la;

            if ( x >= BASE_M1 ) 
                x -= BASE_M1;

            a[ i + j ] ^= alog[ x ];
        }
    }

    while ( a[ 0 ] && a[ a[ 0 ] ] == 0 )
        a[ 0 ] -= 1;
}

void field::div( poly a, unsigned b )
{
    unsigned lb = log[b];

    for ( unsigned i = 1; i <= a[ 0 ]; i += 1 )
    {
        unsigned ix = a[ i ];
        if ( ix )
        {
            ix = log[ ix ] + BASE_M1 - lb;

            if ( ix >= BASE - 1 ) 
                ix -= BASE_M1;
            
            a[ i ] = alog[ ix ];
        }
    }
}

void field::inverse( const poly a, poly b )
{
    field::poly c, f, g;

    b[ 0 ] = 1; 
    b[ 1 ] = 1;
    c[ 0 ] = 0;
    g[ 0 ] = K + 1;

    copy( a, f );

    for ( unsigned i = 2; i <= K; i += 1 )
        g[ i ] = 0;

    g[ 1 ] = 1; 
    g[ T + 1 ] = 1; 
    g[ K + 1 ] = 1;

    while ( 1 )
    {
        if ( f[ 0 ] == 1 )
        {
            div( b, f[1] );
            return;
        }
        if ( f[ 0 ] < g[ 0 ] ) // basically same code with b,c,f,g swapped
        {
            while ( 1 )
            {
                unsigned j = g[ 0 ] - f[ 0 ];
                unsigned ix = log[ g[ g[ 0 ] ] ] - log[ f[ f[ 0 ] ] ] + BASE_M1;

                if ( ix >= BASE_M1 ) 
                    ix -= BASE_M1;

                unsigned alpha = alog[ ix ];
                addmul( g, alpha, j, f );
                addmul( c, alpha, j, b );

                if ( g[ 0 ] == 1 )
                {
                    div( c, g[ 1 ] );
                    copy( c, b );
                    return;
                }

                if ( g[ 0 ] < f[ 0 ] ) 
                    break;
            }
        }
    
        unsigned j = f[ 0 ] - g[ 0 ];
        unsigned ix = log[ f[ f[ 0 ] ] ] - log[ g[ g[ 0 ] ] ] + BASE_M1;

        if ( ix >= BASE_M1 ) 
            ix -= BASE_M1;

        unsigned alpha = alog[ ix ];
        addmul( f, alpha, j, g );
        addmul( b, alpha, j, c );
    }
}

void field::sqrt( const poly a, poly b ) // b = a^( 2^(M-1) );
{
    unsigned i = M-1;

    copy( a, b );

    while ( i-- )
    {
        field::poly t1;
        square( b, t1 );
        reduce( t1 );
        copy( t1, b );
    }
}

bigint field::pack( const poly a )
{
    bigint result;
    unsigned i = a[ 0 ];

    while ( i )
    {
        result = ( result << L ) + a[ i ];
        i -= 1;
    }

    return result;
}

void field::unpack( const bigint & X, poly a )
{
    bigint x = X;
    unsigned n = 0;

    while ( x != 0 )
    {
        n += 1;
        a[ n ] = to_unsigned( x & BASE_M1 );
        x >>= L;
    }

    a[ 0 ] = n;
}

// point implementation

point::point()
{
    c = 0;
}

point::point( curve * C ) : x(C), y(C)
{
    c = C;
}

point::point( const point & P )
{
    c = P.c;
    x = P.x;
    y = P.y;
}

point operator * ( const bigint & k, const point & P )
{
    point result( P.c );
    P.c->mul( P, k, result );

    return result;
}

point point::operator + ( const point &P ) const
{
    point result( P.c );
    c->add( *this, P, result );

    return result;
}

point point::operator - ( const point & P ) const
{
    point result( P.c );
    c->sub( *this, P, result );

    return result;
}

point & point::operator = ( const point & P )
{
    c = P.c;
    x = P.x;
    y = P.y;

    return *this;
}

// curve implementation

curve::curve( full_curve_parameter & a ) : field( a )
{
    b.f = this;
    b.v[0] = 1;
    b.v[1] = a.b;

    unsigned SQRT_BASE = 1u << ( ( a.L + 1 ) / 2 ); // upper bound
    unsigned so_min = BASE - 2 * SQRT_BASE;
    unsigned so = so_min + 2 * a.nso;
    bigint sf = so * a.ntf;

    p = a.p0;

    if ( p == 0 )
    {
        bigint Z = pow2( L );
        p = pow2( M ) + 1 - small_lucas( Z - ( so-1 ), Z, K );
        p = p / sf;
        a.p0 = p;
    }

    assert( is_probable_prime( p ) && MOV( M / 8, pow2( M ), p ) );

    if ( a.P0 == 0 )
    {
        prng = 0; // to make sure P is reproducible

        do {
            PP = sf * random_point();
        } while ( PP.x.v[ 0 ] + PP.y.v[ 0 ] == 0 );

        a.P0 = pack( PP );
    }
    else
        PP = unpack( a.P0 );

#if assert_on
    point t2 = p * PP;
    assert( t2.x.v[ 0 ] + t2.y.v[ 0 ] == 0 ); // fingers crossed!
#endif
};

unsigned curve::ybit( const field_element & x )
{
    if ( x.v[ 0 ] == 0 )
        return 0;

    return x.v[ 1 ] & 1;
}

bigint curve::to_vlong( const point & P )
{
    return P.c->field::pack( P.x.v );
}

bigint curve::pack( const point & P )
{
    bigint result = 0;

    if ( P.x.v[0] )
    {
        result = to_vlong( P );
        result = result * 2 + ybit( P.y / P.x );
    }
    else 
    if ( P.y.v[0] )
        result = 1;
    else
        result = 0;

    return result;
}

point curve::unpack( const bigint & X )
{
    bigint x = X;
    unsigned yb = to_unsigned( x & 1 ); 
    point R( this );

    x >>= 1;
    field::unpack( x, R.x.v );

    if ( R.x.v[0] || yb )
        calc_y( R, yb ); // will return 1 provided X is valid
    else
        R.y = R.x; // zero

    return R;
}

int curve::MOV( unsigned B, const bigint & q, const bigint & r )
{
    bigint x = 1;

    for ( unsigned i = 1; i < B; i += 1 )
    {
        x = ( x * q ) % r;
        if ( x == 1 )
            return 0; // what a pity ...
    }

    return 1;
}

bigint curve::small_lucas( bigint P, bigint Z, unsigned ik )
{
    bigint V0 = 2;
    bigint V1 = P;

    while ( ik > 1 )
    {
        bigint V2 = P * V1 - Z * V0;
        V0 = V1;
        V1 = V2;
        ik -= 1;
    }

    return V1;
}

#if use_field_op

void curve::sub( const point & P, const point & Q, point & R )
{
    point t1 = Q;

    t1.y = t1.y + t1.x;
    add( P, t1, R );
}

void curve::add( const point & P, const point & Q, point & R )
{
    if ( P.x == 0 && P.y == 0 )
        R = Q;
    else 
    if ( Q.x == 0 && Q.y == 0 )
        R = P;
    else
    {
        if ( P.x == Q.x )
        {
            if ( P.y == Q.y ) // points are equal
            {
                field_element LD = P.x + P.y / P.x;
                R.x = sq( LD ) + LD /*+ a*/;
                R.y = sq( P.x ) + LD * R.x + R.x;
            }
            else // must be inverse - result is zero
            {
                R.x.v[ 0 ] = 0;
                R.y.v[ 0 ] = 0;
            }
        }
        else // P,Q are distinct, non-zero, and P != -Q
        {
            field_element LD = ( P.y + Q.y ) / ( P.x + Q.x );
            R.x = sq( LD ) + LD + P.x + Q.x /*+ a*/;
            R.y = LD * ( P.x + R.x ) + R.x + P.y;
        }
    }
}

#else

// optimised version - about 5% faster ?
// could still do better

void curve::sub( const point & P, const point & Q, point & R )
{
    point t1;

    t1.c = this;
    copy( Q.x.v, t1.x.v );
    field::add( Q.x.v, Q.y.v, t1.y.v );
    add( P, t1, R );
}

void curve::add( const point & P, const point & Q, point & R )
{
    if ( P.x.v[ 0 ] + P.y.v[ 0 ] == 0 )
        R = Q;
    else 
    if ( Q.x.v[ 0 ] + Q.y.v[ 0 ] == 0 )
        R = P;
    else
    {
        field::poly LD, t1, t2, t3;
        field::add( P.x.v, Q.x.v, t1 );

        if ( t1[ 0 ] == 0 )
        {
            if ( equal( P.y.v, Q.y.v ) ) // points are equal
            {
                // field_element LD = P.x + P.y/P.x;
                inverse( P.x.v, t1 );
                field::mul( t1, P.y.v, t2 ); reduce( t2 );
                field::add( t2, P.x.v, LD );
                // R.x = sq(LD) + LD /*+ a*/;
                square( LD, t1 ); reduce( t1 );
                field::add( t1, LD, R.x.v );
                // R.y = sq(P.x) + LD*R.x + R.x;
                square( P.x.v, t1 );
                field::mul( LD, R.x.v, t2 );
                field::add( t1, t2, t3 );
                reduce( t3 );
                field::add( t3, R.x.v, R.y.v );
            }
            else // must be inverse - result is zero
            {
                R.x.v[0] = 0;
                R.y.v[0] = 0;
            }
        }
        else // P,Q are distinct, non-zero, and P != -Q
        {
            //field_element LD = ( P.y + Q.y ) / ( P.x + Q.x );
            inverse( t1, t2 );
            field::add( P.y.v, Q.y.v, t1 );
            field::mul( t1, t2, LD ); reduce(LD);
            //R.x = sq(LD) + LD + P.x + Q.x /*+ a*/;
            square( LD, t1 ); reduce( t1 );
            field::add( t1, LD, t2 );
            field::add( t2, P.x.v, t1 );
            field::add( t1, Q.x.v, R.x.v );
            //R.y = LD*(P.x+R.x) + R.x + P.y;
            field::add( P.x.v, R.x.v, t1 );
            field::mul( LD, t1, t2 ); reduce( t2 );
            field::add( t2, R.x.v, t1 );
            field::add( t1, P.y.v, R.y.v );
        }
    }
}

#endif

point curve::random_point()
{
    point R( this );

    do {
        field::set_random( R.x.v );
    } while ( calc_y( R ) == 0 );

    return R;
}

int curve::calc_y( point & R, unsigned yb )
{
    if ( R.x.v[ 0 ] == 0 )
        sqrt( b.v, R.y.v );
    else
    {
        field_element alpha = sq( R.x ) * ( R.x /*+ a*/ ) + b;
        R.y.v[ 0 ] = 0;

        if ( alpha.v[ 0 ] != 0 )
        {
            field_element t1( this );

            field_element beta = alpha / sq( R.x );

            if ( trace( beta.v ) != 0 )
                return 0;

            quad_solve( beta.v, t1.v );

            assert( t1 * t1 + t1 == beta );
            
            if ( ybit( t1 ) != yb )
                t1.v[ 1 ] ^= 1;

            R.y = R.x * t1;
        }
    }

    assert( sq( R.y ) + R.x * R.y == R.x * R.x * R.x + b );
    
    return 1;
}

void curve::mul( const point & P, const bigint & x, point & Q )
{
    bigint h = x*3;
    Q.x.v[0] = 0;
    Q.y.v[0] = 0;

    if (h.bits() == 0)
        return;

    point R = P;

    unsigned r = h.bits()-1; // so h.bit(r)==1

    unsigned i = 1;

    while ( 1 )
    {
        int hi = h.bit( i );
        int ai = x.bit( i );

        if ( hi == 1 && ai == 0 )
            Q = Q + R;
        
        if ( hi == 0 && ai == 1 )
            Q = Q - R;
        
        if ( i == r ) 
            break;

        i += 1;
        R = R + R;
    }
}

// full_curve_parameter implementation

full_curve_parameter::full_curve_parameter( const curve_parameter & bp )
{
    L = bp.L;
    K = bp.K;
    T = bp.T;
    root = bp.root;
    b = bp.b;
    nso = bp.nso;
    ntf = bp.ntf;
    p0 = 0;
    P0 = 0;
    tm = 0;
}

// curve_factory implementation

curve_factory::curve_factory( unsigned L ) 
: L( L )
{
    unsigned BASE = 1u << L;
    unsigned SQRT_BASE = 1u << ( ( L+1 ) / 2 ); // upper bound
    unsigned * ob;
    unsigned so;

    if ( L > MAXL )
        return;

    so_set = 0;

    {
        root = calc_root( L );

        if ( root == 0 ) 
            return;

        small_field f( L, root );
        so_min = BASE - 2 * SQRT_BASE;
        so_max = BASE + 2 * SQRT_BASE;
        ob = f.curve_order();
    }

    so_set = new unsigned[ so_max - so_min + 1 ];

    for ( so = so_min; so <= so_max; so += 1 ) 
        so_set[ so-so_min ] = 0;

    for ( unsigned b = 1; b < BASE; b += 1 )
    {
        so = ob[ b ];

        assert( so >= so_min );
        assert( so <= so_max );
        assert( ( so & 1 ) == 0 );

        if ( so_set[ so - so_min ] == 0 )
            so_set[ so - so_min ] = b;
    }

    delete [] ob;

    {
        prime_factory pf( 1000 );
        comp = 1;

        for ( unsigned i = 0; i < pf.np; i += 1 )
            comp = comp * pf.pl[ i ];
    }
}

curve_factory::~curve_factory()
{
    delete [] so_set;
}

int curve_factory::find( unsigned K, curve_parameter & result )
{
    unsigned T = 1;

    if ( !so_set || K < 3 || !is_prime( K ) || L % K == 0 )
        return 0;

    // calculate T
    while ( 1 )
    {
        if ( poly_irreducible( 1 + pow2( T ) + pow2( K ) ) )
            break;

        T += 1;

        if ( T > K / 2 )
            return 0;
    }

    bigint best_p = 1000;
    int found = 0;

    for ( unsigned so = so_min; so <= so_max; so += 1 )
    {
        unsigned M = L * K;
        unsigned b = so_set[ so - so_min ];
        if ( b )
        {
            bigint Z = pow2( L );
            bigint p = pow2( M ) + 1 - curve::small_lucas( Z - ( so - 1 ), Z, K );
            assert( p % so == 0 );
            p = p / so;
            bigint ntf = 1;
            bigint factor = comp;

            while ( 1 )
            {
                factor = gcd( p, factor );
                // printf("L=%u K=%u so=%u factor=%d\n",L,K,so,to_unsigned(factor));
                if ( factor == 1 )
                {
                    if ( is_probable_prime( p ) && curve::MOV( 10, pow2( M ), p ) )
                    {
                        result.L = L;
                        result.K = K;
                        result.T = T;
                        result.root = root;
                        result.b = b;
                        result.nso = ( so - so_min ) / 2;
                        result.ntf = to_unsigned( ntf );
                        found = 1;
                        best_p = p;

                        if ( result.ntf == 1 )
                            return 1;
                    }

                    break;
                }
                else
                {
                    ntf = ntf * factor;

                    if ( ntf > 0xffff )
                        break; // limit on non-trivial factors

                    p = p / factor;

                    if ( p <= best_p )
                        break;
                }
            }
        }
    }

    return found;
}

// ECC ( Elliptic Curve Cryptography ) implementation

ECC_BASE::ECC_BASE( full_curve_parameter & a )
: curve( a ), bits( p.bits() )
{
#ifdef WIN32
    if (!CryptAcquireContext(&m_cryptProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
        m_cryptProv = NULL;
#else
    ::srand((unsigned)time(NULL));
#endif
}

ECC_BASE::~ECC_BASE()
{
#ifdef WIN32
    CryptReleaseContext(m_cryptProv, 0L);
#endif
}

bigint ECC_BASE::create_private_key()
{
    bigint x;

    while ( x < p )
    {
        x = ( x << 15 ) + rand( 1u << 15 );
    }

    x = 2 + x % ( p - 3 );

    return x;
}

unsigned ECC_BASE::rand(unsigned base)
{
#ifdef WIN32
    unsigned rnd;
    CryptGenRandom(m_cryptProv, sizeof(base), (BYTE *)&rnd);
    return rnd % base;
#else
    return ::rand();
#endif
}

bigint ECC_BASE::create_public_key( const bigint & private_key )
{
    return pack( private_key * PP );
}

bigint ECC_BASE::encrypt( const bigint & public_key, bigint & message )
{
    bigint sr = create_private_key();

    message = pack( sr * PP );

    return to_vlong( sr * unpack( public_key ) );
}

bigint ECC_BASE::decrypt( const bigint & private_key, const bigint & message )
{
    point D = private_key * unpack( message );

    return to_vlong( D );
}

pair ECC_BASE::schnorr_sign( const bigint & data, const bigint & private_key, bigint (*hash)(const bigint &, const bigint &), unsigned hashbits )
{
    pair sig;
    bigint k;

    do
    {
        k = create_private_key();
        bigint z = to_vlong( k*PP );
        sig.r = (hash) ? hash( z + data, p) % (pow2(hashbits) - 1) : ( z + data ) % p;
    } while ( sig.r == 0 );

    sig.s = ( k + private_key * sig.r ) % p;

    return sig;
}

bool ECC_BASE::schnorr_verify( const bigint & e, const pair & sig, const bigint & public_key, bigint (*hash)(const bigint &, const bigint &), unsigned hashbits )
{
    bigint bz = to_vlong( sig.s * PP - sig.r * unpack( public_key ) );
    bigint data = (hash) ? hash( e + bz, p ) % (pow2(hashbits) - 1) : ( e + bz ) % p;

    return (data == sig.r) ? true : false; // avoid compiler warning
}

pair ECC_BASE::dsa_sign( const bigint & data, const bigint & private_key, bigint (*hash)(const bigint &, const bigint &))
{
    pair sig;
    bigint k;

    do
    {
        k = create_private_key();
        bigint z = to_vlong( k*PP );
        sig.r = z % p;
    } while ( sig.r == 0 );

    sig.s = ( modinv( k, p ) * ( hash( data, p ) + private_key * sig.r ) ) % p;

    return sig;
}

bool ECC_BASE::dsa_verify( const bigint & e, const pair & sig, const bigint & public_key, bigint (*hash)(const bigint &, const bigint &))
{
    bigint c = modinv( sig.s, p );
    bigint u1 = ( hash( e, p ) * c ) % p;
    bigint u2 = ( sig.r * c ) % p;
    bigint z = to_vlong( u1 * PP + u2 * unpack( public_key ) ) % p;

    return (z == sig.r) ? true : false;
}
