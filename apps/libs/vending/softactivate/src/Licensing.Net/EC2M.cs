//
// Copyright (c) 2014 SoftActivate. All rights reserved.
//

using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;

namespace SoftActivate.Licensing
{
    class Constants
    {
        Constants()
        {

        }

        public const int MaxL = 16;
        public const int MaxK = 193;
    }

    class SmallField : ICloneable
    {
	    public SmallField(uint l0, uint root)
        {
            l = l0;
            Base = 1u << (int)l0;
            BaseM1 = ( 1u << (int)l0 ) - 1;
            log = new ushort[ 1u << (int)l0 ];
            alog = new ushort[ 1u << (int)l0 ];

            Debug.Assert(l0 <= sizeof(ushort) * 8);

            root |= Base;
            uint j = 1;

            for (uint i = 0; i < BaseM1; i += 1)
            {
                log[j] = (ushort)i;
                alog[i] = (ushort)j;
                j *= 2; // 2 is primitive element
                if ((j & Base) != 0)
                    j ^= root;
            }

            log[0] = (ushort)BaseM1; // used by field::mul
        }

        protected SmallField(SmallField s)
        {
            alog   = (ushort[])s.alog.Clone();
            log    = (ushort[])s.log.Clone();
            l      = s.l;
            Base   = s.Base;
            BaseM1 = s.BaseM1;
        }

	    ~SmallField()
        {

        }

	    public uint[] CurveOrder
        {
            get {
                uint[] result = new uint[ Base ]; // result[0] not defined
                uint b, x, y, i;

                for (b = 1; b < Base; b += 1)
                    result[b] = 2; /* 0,0 and 0, sqrt(b) */

                for (x = 1; x < Base; x += 1)
                {
                    uint lx = log[x];
                    uint x3 = alog[(3 * lx) % BaseM1];
                    result[x3] += 2; // y = 0 and y = x

                    for (y = 1; y < x; y += 1)
                    {
                        i = (uint)(log[y] + log[x ^ y]);

                        if (i >= BaseM1)
                            i -= BaseM1;

                        b = x3 ^ alog[i];
                        result[b] += 1;
                    }

                    for (y = x + 1; y < Base; y += 1)
                    {
                        i = (uint)(log[y] + log[x ^ y]);

                        if (i >= BaseM1)
                            i -= BaseM1;

                        b = x3 ^ alog[i];
                        result[b] += 1;
                    }
                }

                return result;
            }
        }

	    public ushort[] alog; // index range is [0..(BASE-2)]
	    public ushort[] log;  // index range is [1..(BASE-1)], but log[0] is set to (BASE-1)
	    public uint l;
        public uint Base;
        public uint BaseM1;

        #region ICloneable Members

        public object Clone()
        {
            return new SmallField(this);
        }

        #endregion
    }

    class Field : SmallField, ICloneable
    {
// construction
        protected Field(Field f) : base(f)
        {
            M = f.M;
            K = f.K;
            T = f.T;
    	    prng = f.prng;
            nzt = (uint[])f.nzt.Clone();
            tm = (uint[])f.tm.Clone();
        }

	    public Field( FullCurveParameter a ) : base(a.l, a.root)
        {
            M = a.l * a.k;
            K = a.k;
            T = a.t;

            Debug.Assert(K <= Constants.MaxK);

            prng = 0;

            if (a.tm == 0)
            {
                // Calculate trace mask
                uint[] x = new uint[2 * Constants.MaxK];
                for (uint i = 1; i <= K; i += 1)
                {
                    x[0] = i;
                    uint m = 1;
                    uint w = 0;

                    for (uint j = 0; j < l; j += 1)
                    {
                        x[i] = m;

                        if (SlowTrace(x) != 0)
                            w ^= m;

                        m *= 2;
                    }

                    x[i] = 0;
                    tm[i] = w;

                    if (w != 0)
                        tm[0] = i;
                }

                a.tm = Pack(tm);
            }
            else
                Unpack(a.tm, tm);

            if (M % 2 == 0)
            {
                nzt[0] = 1;
                nzt[1] = 1;

                while ((nzt[1] & tm[1]) == 0)
                    nzt[1] *= 2;

                Debug.Assert(Trace(nzt) != 0);
            }
        }

    	public virtual uint Rand( uint max )
        {
            prng += 1;

            return prng % max;
        }

        public uint[] Add( uint[] p1, uint[] p2 )
        {
            uint[] p = new uint[2 * Constants.MaxK];

            if (p1[0] < p2[0])
                return Add(p2, p1);
            else
            {
                uint i;

                for (i = 0; i <= p2[0]; i += 1)
                    p[i] = p1[i] ^ p2[i];

                for (; i <= p1[0]; i += 1)
                    p[i] = p1[i];

                p[0] = p1[0];
            }

            while (p[0] != 0 && p[ p[0] ] == 0)
                p[0] -= 1;

            return p;
        }

	    public static void Copy( uint[] psrc, uint[] pdst )
        {
            for (uint i = 0; i <= psrc[0]; i += 1)
                pdst[i] = psrc[i];
        }

	    public static bool Equal( uint[] p1, uint[] p2 )
        {
            uint i = 0;

            while (true)
            {
                if (p1[i] != p2[i])
                    return false;

                if (i == p1[0])
                    return true;

                i += 1;
            }
        }

	    public void Div( uint[] poly, uint b )
        {
            uint lb = log[b];

            for (uint i = 1; i <= poly[0]; i += 1)
            {
                uint ix = poly[i];
                if (ix != 0)
                {
                    ix = log[ix] + BaseM1 - lb;

                    if (ix >= Base - 1)
                        ix -= BaseM1;

                    poly[i] = alog[ix];
                }
            }
        }

	    public void SetRandom( uint[] poly )
        {
            poly[0] = K;

            for (uint i = 1; i <= K; i += 1)
                poly[i] = Rand(Base);

            while (poly[0] != 0 && poly[poly[0]] == 0)
                poly[0] -= 1;
        }

	    public void Reduce( uint[] p )
        {
            for (uint i = p[0]; i > K; i -= 1)
            {
                p[i - K] = p[i - K] ^ p[i];
                p[i + T - K] = p[i + T - K] ^ p[i];
                p[i] = 0;
            }

            if (p[0] > K)
            {
                p[0] = K;

                while (p[0] != 0 && p[p[0]] == 0)
                    p[0] -= 1;
            }
        }

	    public uint[] Mul( uint[] p1, uint[] p2 )
        {
            uint[] p = new uint[2 * Constants.MaxK];
	        uint i, j;

	        Debug.Assert( p1[0] <= K );
	        Debug.Assert( p2[0] <= K );

	        if ( p1[ 0 ] == 0 || p2[ 0 ] == 0 )
	        {
		        p[ 0 ] = 0;
		        return p;
	        }

	        p[ 0 ] = p1[ 0 ] + p2[ 0 ] - 1;

	        for ( i = 1; i <= p[ 0 ]; i += 1 )
		        p[ i ] = 0;

	        // pre-computation of log[p2[j]] (to reduce table lookups)
	        uint[] lb = new uint[ Constants.MaxK + 1 ];
	        uint b0 = p2[ 0 ];

	        for ( j = 1; j <= b0; j += 1 )
		        lb[ j ] = log[ p2[ j ] ];

	        for ( i = 1; i <= p1[ 0 ]; i += 1 )
	        {
		        uint lai = log[ p1[ i ] ];
		        if ( lai != BaseM1 )
		        {
			        for ( j = 1; j <= b0; j += 1 )
			        {
				        uint lbj = lb[ j ];
				        if ( lbj != BaseM1 )
				        {
					        uint x = lai + lbj;
        					
					        if ( x >= BaseM1 ) 
						        x -= BaseM1;

					        p[ i + j - 1 ] ^= alog[ x ];
				        }
			        }
		        }
	        }

            return p;
        }

	    public uint[] Square( uint[] p1 )
        {
            uint i;
            uint[] p = new uint[2 * Constants.MaxK];

            if (p1[0] == 0)
            {
                p[0] = 0;
                return p;
            }

            p[0] = 2 * p1[0] - 1;

            for (i = 1; ; i += 1)
            {
                uint x = 0;
                if (p1[i] != 0)
                {
                    x = (uint)(2 * log[p1[i]]);

                    if (x >= BaseM1)
                        x -= BaseM1;

                    x = alog[x];
                }

                p[2 * i - 1] = x;

                if (i == p1[0])
                    break;

                p[2 * i] = 0;
            }

            return p;
        }

	    public uint[] Inverse( uint[] p1 )
        {
            uint[] p = new uint[2 * Constants.MaxK];
            uint[] c = new uint[2 * Constants.MaxK];
            uint[] f = new uint[2 * Constants.MaxK];
            uint[] g = new uint[2 * Constants.MaxK];
            uint j, ix, alpha;

            p[0] = 1;
            p[1] = 1;
            c[0] = 0;
            g[0] = K + 1;

            Copy(p1, f);

            for (uint i = 2; i <= K; i += 1)
                g[i] = 0;

            g[1] = 1;
            g[T + 1] = 1;
            g[K + 1] = 1;

            while (true)
            {
                if (f[0] == 1)
                {
                    Div(p, f[1]);
                    return p;
                }
                if (f[0] < g[0]) // basically same code with p,c,f,g swapped
                {
                    while (true)
                    {
                        j = g[0] - f[0];
                        ix = (uint)(log[g[g[0]]] - log[f[f[0]]] + BaseM1);

                        if (ix >= BaseM1)
                            ix -= BaseM1;

                        alpha = alog[ix];
                        AddMul(g, alpha, j, f);
                        AddMul(c, alpha, j, p);

                        if (g[0] == 1)
                        {
                            Div(c, g[1]);
                            Copy(c, p);
                            return p;
                        }

                        if (g[0] < f[0])
                        {
                            break;
                        }
                    }
                }

                j = f[0] - g[0];
                ix = (uint)(log[f[f[0]]] - log[g[g[0]]] + BaseM1);

                if (ix >= BaseM1)
                    ix -= BaseM1;

                alpha = alog[ix];
                AddMul(f, alpha, j, g);
                AddMul(p, alpha, j, c);
            }

            return p; 
        }

	    public int Trace( uint[] p )
        {
            uint w = 0;
            uint min = p[0];

            if (min > tm[0])
                min = tm[0];

            for (uint i = 1; i <= min; i += 1)
                w ^= p[i] & tm[i];

            // compute parity of w
            uint result = 0;

            while (w != 0)
            {
                result ^= w & 1;
                w >>= 1;
            }

            Debug.Assert(result == SlowTrace( p ));

            return (int)result;
        }

	    public int SlowTrace( uint[] p )
        {
            uint[] t1 = new uint[2 * Constants.MaxK];
            uint[] t2 = new uint[2 * Constants.MaxK];

            Copy(p, t1);

            for (uint i = 1; i < M; i += 1)
            {
                t2 = Square(t1);
                Reduce(t2);
                t1 = Add(t2, p);
            }

            return (t1[0] != 0) ? 1 : 0;
        }

	    public uint[] QuadSolve( uint[] p1 )
        {
            uint[] p = new uint[2 * Constants.MaxK];
            uint[] t1 = new uint[2 * Constants.MaxK];
            uint[] t2 = new uint[2 * Constants.MaxK];
            
            if (M % 2 == 1) // M is odd - compute half-trace
            {
                Copy(p1, p);

                for (uint i = 0; i < M / 2; i += 1)
                {
                    t1 = Square(p); Reduce(t1);
                    t2 = Square(t1); Reduce(t2);
                    p = Add(t2, p1);
                }
            }
            else
            {
                uint[] d = new uint[2 * Constants.MaxK];
                p[0] = 0;
                Copy(p1, d);

                for (uint i = 1; i < M; i += 1)
                {
                    t1 = Square(d); Reduce(t1);
                    t2 = Mul(nzt, t1); // N.B. make nzt the first parameter, since it is mostly zero
                    t1 = Square(p);
                    p = Add(t1, t2); Reduce(p);
                    t1 = Square(d); Reduce(t1);
                    d = Add(t1, p1);
                }
            }

            return p;
        }

	    public void Sqrt( uint[] p1, uint[] p )
        {
            uint i = M - 1;
            uint[] t1 = new uint[2 * Constants.MaxK];

            Copy(p1, p);

            while (i-- != 0)
            {
                t1 = Square(p);
                Reduce(t1);
                Copy(t1, p);
            }
        }

	    public void AddMul( uint[] p1, uint alpha, uint j, uint[] p )
        {
            uint la = log[alpha];

            for (uint i = 1; i <= p[0]; i += 1)
            {
                while (p1[0] < j + i)
                {
                    p1[0] += 1;
                    p1[p1[0]] = 0;
                }

                uint x = p[i];

                if (x != 0)
                {
                    x = log[x] + la;

                    if (x >= BaseM1)
                        x -= BaseM1;

                    p1[i + j] ^= alog[x];
                }
            }

            while (p1[0] != 0 && p1[p1[0]] == 0)
                p1[0] -= 1;
        }
        
	    public void  Unpack( BigInt xp, uint[] p )
        {
            BigInt x = xp;
            uint n = 0;

            while (x != 0)
            {
                n += 1;
                p[n] = x & BaseM1;
                x = x >> (int)l;
            }

            p[0] = n;
        }

        public BigInt Pack(uint[] p)
        {
            BigInt result = 0;
            uint i = p[0];

            while (i != 0)
            {
                result = (result << (int)l) + p[i];
                i -= 1;
            }

            return result;
        }

	    public uint M, K, T;
	    protected uint prng;
	    private uint[] nzt = new uint[2 * SoftActivate.Licensing.Constants.MaxK]; // element with non-zero trace
	    private uint[] tm = new uint[2 * SoftActivate.Licensing.Constants.MaxK];  // trace mask ( trace(x) is number of bits in x & tm )

        #region ICloneable Members

        new public object Clone()
        {
            return new Field(this);
        }

        #endregion
    }

    class FieldElement : ICloneable
    {
        protected FieldElement(FieldElement fe)
        {
            f = (Field)fe.f.Clone();
            v = (uint[])fe.v.Clone();
        }

        public FieldElement()
        {
            v[0] = 0;
        }

        public FieldElement(Field fp)
        {
            f = fp;
            v[0] = 0;
        }

        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

        public override bool Equals(object obj)
        {
            return base.Equals(obj);
        }

        public static bool operator !=(FieldElement y, FieldElement x)
        {
            return !Field.Equal(y.v, x.v);
        }

        public static bool operator ==(FieldElement y, FieldElement x)
        {
            return Field.Equal(y.v, x.v);
        }

        public static bool operator !=(FieldElement y, uint x)
        {
            return !((y.v[0] == 0 && x == 0) || (y.v[1] == 1 && x == y.v[1]));
        }

        public static bool operator ==(FieldElement y, uint x)
        {
            return (y.v[0] == 0 && x == 0) || (y.v[1] == 1 && x == y.v[1]);
        }

        public static FieldElement operator +(FieldElement x, FieldElement y)
        {
	        FieldElement result = new FieldElement( x.f );

	        result.v = x.f.Add( y.v, x.v );

	        return result;
        }

        public static FieldElement operator *(FieldElement x, FieldElement y)
        {
 	        FieldElement result = new FieldElement( x.f );

	        result.v = x.f.Mul( y.v, x.v );
	        x.f.Reduce( result.v );

	        return result;           
        }

        public static FieldElement operator /(FieldElement y, FieldElement x)
        {
	        FieldElement result = new FieldElement( x.f );
	        uint[] tmp = new uint[2 * Constants.MaxK];

	        tmp = x.f.Inverse( x.v );
	        result.v = x.f.Mul( y.v, tmp );
	        x.f.Reduce( result.v );

	        return result;
        }

        public static FieldElement Sqrt(FieldElement x)
        {
	        FieldElement result = new FieldElement( x.f );

	        x.f.Sqrt( x.v, result.v );

	        return result;
        }

	    public Field f;
	    public uint[] v = new uint[2 * Constants.MaxK];

        #region ICloneable Members

        public object Clone()
        {
            return new FieldElement(this);
        }

        #endregion
    }

    class Point : ICloneable
    {
        public Point()
        {

        }

    	protected Point(Point p)
        {
            c = (Curve)p.c;//.Clone();
            x = (FieldElement)p.x.Clone();
            y = (FieldElement)p.y.Clone();
        }

    	public Point(Curve cp)
        {
            x = new FieldElement( (Field)cp );
            y = new FieldElement( (Field)cp );
            c = cp;
        }

        public static Point operator *(BigInt k, Point p)
        {
	        return Curve.Mul( p, k);
        }

	    public static Point operator +(Point p, Point q)
        {
	        return p.c.Add(p,  q);
        }

        public static Point operator -(Point p, Point q)
        {
	        return p.c.Sub(p, q);
        }

	    public Curve c;
	    public FieldElement x, y;

        #region ICloneable Members

        public object Clone()
        {
            return new Point(this);
        }

        #endregion
    }

    class CurveParameter : ICloneable
    {
        public CurveParameter()
        {

        }

        protected CurveParameter(CurveParameter cp)
        {
            l = cp.l;
            k = cp.k;
            t = cp.t;
            root = cp.root;
            b    = cp.b;
            nso  = cp.nso;
            ntf  = cp.ntf;
        }

        public CurveParameter(uint lp, uint kp, uint tp, uint rootp, uint bp, uint nsop, uint ntfp)
        {
            l = lp;
            k = kp;
            t = tp;
            root = rootp;
            b = bp;
            nso = nsop;
            ntf = ntfp;
        }

        public uint l;
        public uint k;
        public uint t;
        public uint root;
        public uint b;
        public uint nso;
        public uint ntf;

        #region ICloneable Members

        public object Clone()
        {
            return new CurveParameter(this);
        }

        #endregion
    }

    class FullCurveParameter : CurveParameter, ICloneable
    {
        public FullCurveParameter(CurveParameter bp)
        {
            l = bp.l;
            k = bp.k;
            t = bp.t;
            root = bp.root;
            b = bp.b;
            nso = bp.nso;
            ntf = bp.ntf;
            p0 = 0;
            p1 = 0;
            tm = 0;
        }

        protected FullCurveParameter(FullCurveParameter fcp) : base(fcp)
        {
            tm = (BigInt)fcp.tm.Clone();
            p0 = (BigInt)fcp.p0.Clone();
            p1 = (BigInt)fcp.p1.Clone();
        }

	    public BigInt tm;
        public BigInt p0;
        public BigInt p1;

        #region ICloneable Members

        new public object Clone()
        {
            return new FullCurveParameter(this);
        }

        #endregion
    }

    class Curve : Field, ICloneable
    {
        protected Curve(Curve c) : base(c)
        {
            if (c.PP != null) PP = (Point)c.PP.Clone();
            p  = (BigInt)c.p.Clone();
            b  = (FieldElement)c.b.Clone();
        }

	    public Curve(FullCurveParameter a) : base(a)
        {
            b.f = this;
            b.v[0] = 1;
            b.v[1] = a.b;

            uint SQRT_BASE = 1u << (int)((a.l + 1) / 2); // upper bound
            uint so_min = Base - 2 * SQRT_BASE;
            uint so = so_min + 2 * a.nso;
            BigInt sf = so * a.ntf;

            p = a.p0;

            if (p == 0)
            {
                BigInt Z = BigInt.Pow2(l);
                p = BigInt.Pow2(M) + 1 - SmallLucas(Z - (so - 1), Z, K);
                p = p / sf;
                a.p0 = p;
            }

            Debug.Assert(PrimeFactory.IsProbablePrime(p) && (Mov(M / 8, BigInt.Pow2(M), p) != (BigInt)0));

            if (a.p1 == 0)
            {
                prng = 0; // to make sure P is reproducible

                do
                {
                    PP = sf * RandomPoint();
                } while (PP.x.v[0] + PP.y.v[0] == 0);

                a.p1 = Pack(PP);
            }
            else
                PP = Unpack(a.p1);
        }

	    public Point RandomPoint()
        {
	        Point R = new Point( this );

	        do {
		        base.SetRandom(R.x.v);
	        } while ( CalcY( R ) == 0 );

	        return R;
        }

	    public static BigInt Pack(Point p)
        {
            BigInt result = 0;

            if (p.x.v[0] != 0)
            {
                result = ToVLong(p);
                result = result * 2 + YBit(p.y / p.x);
            }
            else
                if (p.y.v[0] != 0)
                    result = 1;
                else
                    result = 0;

            return result;
        }

	    public Point Unpack(BigInt xp)
        {
	        BigInt x = xp;
	        uint yb = x & 1; 
	        Point R = new Point( this );

            x = x >> 1;
	        base.Unpack( x, R.x.v );

	        if ( R.x.v[0] != 0 || yb != 0 )
		        CalcY( R, yb ); // will return 1 provided X is valid
	        else
		        R.y = R.x; // zero

	        return R;
        }

	    public static BigInt ToVLong(Point p)
        {
	        return p.c.Pack( p.x.v );
        }

	    public Point Add(Point p, Point q)
        {
            Point r = new Point(p.c);

	        if ( p.x.v[ 0 ] + p.y.v[ 0 ] == 0 )
		        r = q;
	        else 
	        if ( q.x.v[ 0 ] + q.y.v[ 0 ] == 0 )
		        r = p;
	        else
	        {
		        uint[] ld = new uint[2 * Constants.MaxK], 
                       t1 = new uint[2 * Constants.MaxK],
                       t2 = new uint[2 * Constants.MaxK],
                       t3 = new uint[2 * Constants.MaxK];

		        t1 = base.Add( p.x.v, q.x.v );

		        if ( t1[ 0 ] == 0 )
		        {
			        if ( Field.Equal( p.y.v, q.y.v ) ) // points are equal
			        {
				        // field_element LD = P.x + P.y/P.x;
				        t1 = base.Inverse( p.x.v );
				        t2 = base.Mul( t1, p.y.v ); Reduce( t2 );
				        ld = base.Add( t2, p.x.v );
				        // R.x = sq(LD) + LD /*+ a*/;
				        t1 = Square( ld ); Reduce( t1 );
				        r.x.v = base.Add( t1, ld );
				        // R.y = sq(P.x) + LD*R.x + R.x;
				        t1 = Square( p.x.v );
				        t2 = base.Mul( ld, r.x.v );
				        t3 = base.Add( t1, t2 );
				        Reduce( t3 );
				        r.y.v = base.Add( t3, r.x.v );
			        }
			        else // must be inverse - result is zero
			        {
				        r.x.v[0] = 0;
				        r.y.v[0] = 0;
			        }
		        }
		        else // P,Q are distinct, non-zero, and P != -Q
		        {
			        //field_element LD = ( P.y + Q.y ) / ( P.x + Q.x );
			        t2 = Inverse( t1 );
			        t1 = base.Add( p.y.v, q.y.v );
			        ld = base.Mul( t1, t2 ); Reduce(ld);
			        //R.x = sq(LD) + LD + P.x + Q.x /*+ a*/;
			        t1 = Square( ld ); Reduce( t1 );
			        t2 = base.Add( t1, ld );
			        t1 = base.Add( t2, p.x.v );
			        r.x.v = base.Add( t1, q.x.v );
			        //R.y = LD*(P.x+R.x) + R.x + P.y;
			        t1 = base.Add( p.x.v, r.x.v );
			        t2 = base.Mul( ld, t1 ); Reduce( t2 );
			        t1 = base.Add( t2, r.x.v );
			        r.y.v = base.Add( t1, p.y.v );
		        }
	        }

        return r;
        }

	    public Point Sub(Point p, Point q)
        {
            Point t1 = new Point(this);

	        Copy( q.x.v, t1.x.v );
	        t1.y.v = base.Add( q.x.v, q.y.v );
	        return Add( p, t1 );
        }

	    public static Point Mul(Point p, BigInt x)
        {
            Point q = new Point(p.c);

            BigInt h = x * 3;
            q.x.v[0] = 0;
            q.y.v[0] = 0;

            if (h.Bits == 0)
                return q;

            Point rp = (Point)p.Clone();

            uint r = h.Bits - 1; // so h.bit(r)==1
            uint i = 1;

            while (true)
            {
                int hi = h.Bit(i);
                int ai = x.Bit(i);

                if (hi == 1 && ai == 0)
                    q = q + rp;

                if (hi == 0 && ai == 1)
                    q = q - rp;

                if (i == r)
                    break;

                i += 1;
                rp = rp + rp;
            }

            return q;
        }

	    private int CalcY(Point R)
        {
            return CalcY(R, 0);
        }

	    private int CalcY(Point R, uint yb)
        {
	        if ( R.x.v[ 0 ] == 0 )
		        Sqrt( b.v, R.y.v );
	        else
	        {
		        FieldElement alpha = SQ( R.x ) * ( R.x /*+ a*/ ) + b;
		        R.y.v[ 0 ] = 0;

		        if ( alpha.v[ 0 ] != 0 )
		        {
			        FieldElement t1 = new FieldElement( this );

			        FieldElement beta = alpha / SQ(R.x);

			        if ( Trace( beta.v ) != 0 )
				        return 0;

			        t1.v = QuadSolve(beta.v);

			        Debug.Assert( t1 * t1 + t1 == beta );
        			
			        if ( YBit( t1 ) != yb )
				        t1.v[ 1 ] ^= 1;

			        R.y = R.x * t1;
		        }
	        }

	        Debug.Assert( Curve.SQ( R.y ) + R.x * R.y == R.x * R.x * R.x + b );
        	
	        return 1;
        }

	    public static int Mov(uint b, BigInt q, BigInt r)
        {
            BigInt x = 1;

            for (uint i = 1; i < b; i += 1)
            {
                x = (x * q) % r;
                if (x == 1)
                    return 0; // what a pity ...
            }

            return 1;
        }

	    public static BigInt SmallLucas(BigInt p, BigInt z, uint ik)
        {
            BigInt v0 = 2;
            BigInt v1 = (BigInt)p.Clone();

            while (ik > 1)
            {
                BigInt v2 = p * v1 - z * v0;
                v0 = v1;
                v1 = v2;
                ik -= 1;
            }

            return v1;
        }

	    private static uint YBit(FieldElement x)
        {
            if (x.v[0] == 0)
                return 0;

            return x.v[1] & 1;
        }

        private static FieldElement SQ(FieldElement x)
        {
	        FieldElement result = new FieldElement(x.f);

	        result.v = x.f.Square( x.v );
	        x.f.Reduce( result.v );

	        return result;
        }

	    public Point PP; // point with prime order
	    public BigInt p; // prime order of P
	    private FieldElement b = new FieldElement();

        #region ICloneable Members

        new public object Clone()
        {
            return new Curve(this);
        }

        #endregion
    }

    class CurveFactory : ICloneable
    {
        CurveFactory()
        {

        }

        protected CurveFactory(CurveFactory cf)
        {
            L = cf.L;
	        root = cf.root;
	        so_min = cf.so_min;
	        so_max = cf.so_max;
	        so_set = (uint[])cf.so_set.Clone();
            comp = (BigInt)cf.comp.Clone();
        }

    	public CurveFactory(uint l)
        {
            this.L = l;
	        uint BASE = 1u << (int)l;
	        uint SQRT_BASE = 1u << (int)( ( l + 1 ) / 2 ); // upper bound
	        uint[] ob;
	        uint so;

	        if ( l > Constants.MaxL )
		        return;

	        so_set = null;

	        {
		        root = CalcRoot( l );

		        if ( root == 0 ) 
			        return;

		        SmallField f = new SmallField( l, root );
		        so_min = BASE - 2 * SQRT_BASE;
		        so_max = BASE + 2 * SQRT_BASE;
		        ob = f.CurveOrder;
	        }

	        so_set = new uint[ so_max - so_min + 1 ];

	        for ( so = so_min; so <= so_max; so += 1 ) 
		        so_set[ so-so_min ] = 0;

	        for ( uint b = 1; b < BASE; b += 1 )
	        {
		        so = ob[ b ];

		        Debug.Assert( so >= so_min );
		        Debug.Assert( so <= so_max );
		        Debug.Assert( ( so & 1 ) == 0 );

		        if ( so_set[ so - so_min ] == 0 )
			        so_set[ so - so_min ] = b;
	        }

	        {
		        PrimeFactory pf = new PrimeFactory( 1000 );
		        comp = 1;

		        for ( uint i = 0; i < pf.np; i += 1 )
			        comp = comp * pf.pl[ i ];
	        }
        }

	    ~CurveFactory()
        {

        }

        static bool IsPrime(uint p)
        {
            for (uint d = 2; d * d <= p; d += 1)
                if (p % d == 0)
                    return false;

            return true;
        }

        static BigInt PolyRem(BigInt x, BigInt y)
        {
	        uint m = y.Bits - 1;
	        BigInt rem = x;
        	
	        while ( true )
	        {
		        uint rb = rem.Bits;
        		
		        if ( rb <= m ) 
			        break;

		        rem ^= y << (int)( rb - m - 1 );
	        }

	        return rem;
        }

        static BigInt PolyMul(BigInt x, BigInt y)
        {
	        BigInt result = 0;
	        uint yb = y.Bits;

	        for ( uint i = 0; i < yb; i += 1 )
	        {
		        if ( y.Bit( i ) != 0 )
			        result ^= x << (int)i;
	        }

	        return result;
        }

        static BigInt PolyGCD( BigInt x, BigInt y ) // field is F2
        {
	        BigInt a = x;
	        BigInt b = y;

	        while ( b != 0 )
	        {
		        BigInt c = PolyRem( a, b );
		        a = b;
		        b = c;
	        }

	        return a;
        }

        static bool PolyIrreducible( BigInt x )
        {
	        uint d = x.Bits - 1;
	        BigInt u = 2;

	        for ( uint i = 1; i < d; i += 1 )
	        {
		        u = PolyRem( PolyMul( u, u ), x );
		        BigInt g = PolyGCD( u^2, x );

		        if ( g != 1 ) 
			        return false;
	        }

	        return true;
        }

        static BigInt PolyPower(BigInt x, BigInt k, BigInt g)
        {
	        BigInt result = 1;

	        for ( uint i = 0; i < k.Bits; i += 1 )
	        {
		        if ( k.Bit( i ) != 0 )
		        {
			        result = PolyRem( PolyMul( result, x ), g );
		        }

		        x = PolyRem( PolyMul( x, x ), g );
	        }

	        return result;
        }

        uint CalcRoot( uint L )
        {
	        // find irreducible poly g such that 2 is primitive
	        BigInt _base = BigInt.Pow2( L );

	        for ( BigInt g = _base + 1; g < 2 * _base; g += 2 ) 
		        if ( PolyIrreducible( g ) )
		        {
			        BigInt r = _base - 1;
			        BigInt f = 2;

			        while ( true )
			        {
				        if ( f != _base - 1 && r % f == 0 )
				        {
					        if ( PolyPower( 2, ( _base - 1 ) / f, g ) == 1 ) 
						        break;

					        do {
						        r = r / f; 
					        } while ( r % f == 0 );
				        }

				        if ( f >= r )
					        return (uint)(g - _base);

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

    	public bool Find(uint k, CurveParameter result)
        {
	        uint t = 1;

	        if ( so_set == null || k < 3 || !IsPrime( k ) || L % k == 0 )
		        return false;

	        // calculate T
	        while ( true )
	        {
		        if ( PolyIrreducible( 1 + BigInt.Pow2( t ) + BigInt.Pow2( k ) ) )
			        break;

		        t += 1;

		        if ( t > k / 2 )
			        return false;
	        }

	        BigInt best_p = 1000;
	        bool found = false;

	        for ( uint so = so_min; so <= so_max; so += 1 )
	        {
		        uint M = L * k;
		        uint b = so_set[ so - so_min ];
		        if ( b != 0 )
		        {
			        BigInt Z = BigInt.Pow2( L );
			        BigInt p = BigInt.Pow2( M ) + 1 - Curve.SmallLucas( Z - ( so - 1 ), Z, k );
			        Debug.Assert( p % so == 0 );
			        p = p / so;
			        BigInt ntf = 1;
			        BigInt factor = comp;

			        while ( true )
			        {
				        factor = BigInt.Gcd( p, factor );
				        // printf("L=%u K=%u so=%u factor=%d\n",L,K,so,to_unsigned(factor));
				        if ( factor == 1 )
				        {
					        if ( PrimeFactory.IsProbablePrime( p ) && Curve.Mov( 10, BigInt.Pow2( M ), p ) != 0)
					        {
						        result.l = L;
						        result.k = k;
						        result.t = t;
						        result.root = root;
						        result.b = b;
						        result.nso = ( so - so_min ) / 2;
						        result.ntf = (uint)(ntf);
						        found = true;
						        best_p = p;

						        if ( result.ntf == 1 )
							        return true;
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

	    private uint L;
	    private uint root;
	    private uint so_min;
	    private uint so_max;
	    uint[] so_set;
	    BigInt comp;

        #region ICloneable Members

        public object Clone()
        {
            return new CurveFactory(this);
        }

        #endregion
    }

    class PrimeFactory
    {
        public PrimeFactory() : this(2000)
        {

        }

	    public PrimeFactory(uint mp) // sieve size
        {
            // Initialise pl
            uint p = 2;
            sbyte[] b = new sbyte[mp + 1]; // one extra to stop search

            for (uint i = 0; i <= mp; i += 1)
                b[i] = 1;

            np = 0;

            while (true)
            {
                // skip composites
                while (b[p] == 0)
                    p += 1;

                if (p == mp)
                    break;

                np += 1;

                // cross off multiples
                uint c = p * 2;

                while (c < mp)
                {
                    b[c] = 0;
                    c += p;
                }

                p += 1;
            }

            pl = new uint[np];
            np = 0;

            for (p = 2; p < mp; p += 1)
                if (b[p] != 0)
                {
                    pl[np] = p;
                    np += 1;
                }
        }

	    ~PrimeFactory()
        {

        }

	    public BigInt FindPrime(BigInt start)
        {
            uint SS = 1000; // should be enough unless we are unlucky
            sbyte[] b = new sbyte[SS]; // bitset of candidate primes

            while (true)
            {
                uint i;

                for (i = 0; i < SS; i += 1)
                    b[i] = 1;

                for (i = 0; i < np; i += 1)
                {
                    uint p = pl[i];
                    uint r = start % p; // not as fast as it should be - could do with special routine

                    if (r != 0)
                        r = p - r;

                    // cross off multiples of p
                    while (r < SS)
                    {
                        b[r] = 0;
                        r += p;
                    }
                }

                // now test candidates
                for (i = 0; i < SS; i += 1)
                {
                    if (b[i] != 0)
                    {
                        if (IsProbablePrime(start))
                        {
                            return start;
                        }
                    }

                    start += 1;
                }
            }
        }

	    public bool MakePrime(BigInt r, BigInt k, BigInt minP)
        {
            k = 1;
            for (uint i = 0; i < np; i += 1)
            {
                uint p = pl[i];
                // maybe pre-computing product of several primes
                // and then GCD(r,p) would be faster ?
                while (r % p == 0)
                {
                    if (r == p)
                        return true; // can only happen if min_p is small

                    r = r / p;
                    k = k * p;

                    if (r < minP)
                        return false;
                }
            }

            return IsProbablePrime(r);
        }

        BigInt FindSpecial(BigInt start)
        {
            return FindSpecial(start, (BigInt)0);
        }

	    BigInt FindSpecial(BigInt start, BigInt _base)
        {
            // Returns a (probable) prime number x > start such that
            // x and x*2+1 are both probable primes,
            // i.e. x is a probable Sophie-Germain prime
            uint SS = 40000; // should be enough unless we are unlucky
            sbyte[] b1 = new sbyte[SS]; // bitset of candidate primes
            sbyte[] b2 = new sbyte[2 * SS];

            while (true)
            {
                uint i;

                for (i = 0; i < SS; i += 1)
                    b1[i] = 1;

                for (i = 0; i < 2 * SS; i += 1)
                    b2[i] = 1;

                for (i = 0; i < np; i += 1)
                {
                    uint p = pl[i];
                    uint r = start % p; // not as fast as it should be - could do with special routine

                    if (r != 0)
                        r = p - r;

                    // cross off multiples of p
                    while (r < SS)
                    {
                        b1[r] = 0;
                        r += p;
                    }

                    r = (start * 2 + 1) % p;

                    if (r != 0)
                        r = p - r;

                    while (r < 2 * SS)
                    {
                        b2[r] = 0;
                        r += p;
                    }
                }

                // now test candidates
                for (i = 0; i < SS; i += 1)
                {
                    if (b1[i] != 0 && b2[i * 2] != 0)
                    {
                        //printf("D=%u\n", to_unsigned(start * 2 + 1 - base));

                        if (MaybePrime(start)
                            && MaybePrime(start * 2 + 1)
                            && IsProbablePrime(start)
                            && IsProbablePrime(start * 2 + 1)
                            )
                        {
                            return start;
                        }
                    }

                    start += 1;
                }
            }
        }

        public static bool MaybePrime(BigInt p)
        {
	        return BigInt.ModExp( 2, p - 1, p ) == 1;
        }

        public static bool FermatIsProbablePrime(BigInt p)
        {
	        // Test based on Fermats theorem a**(p-1) = 1 mod p for prime p
	        // For 1000 bit numbers this can take quite a while
	        const int rep = 4;
	        uint[] any = new uint[rep]{ 2,3,5,7 /*,11,13,17,19,23,29,31,37..*/ };

	        for ( uint i = 0; i < rep; i += 1 )
		        if ( BigInt.ModExp( any[ i ], p - 1, p ) != 1 )
			        return false;

	        return true;
        }

        public static BigInt Random(BigInt n)
        {
 	        BigInt x = 0;
            uint rnd;

            while (x < n)
            {
                lock (random)
                {
                    rnd = (uint)random.Next(0x7FFF);
                }

                x = x * 0x7FFF + rnd;
            }

	        return x % n;
        }

        public static bool MillerRabinIsProbablePrime(BigInt n)
        {
	        uint T = 100;
	        uint v = 0;
	        BigInt w = n - 1;

	        //srand( (unsigned) time( 0 ) );

	        while ( w % 2 != 0 )
	        {
		        v += 1;
		        w = w / 2;
	        }

	        for ( uint j = 1; j <= T; j += 1 )
	        {
		        BigInt a = 1 + Random(n);
		        BigInt b = BigInt.ModExp( a, w, n );
        		
		        if ( b != 1 && b != n - 1 )
		        {
			        uint i = 1;

			        while ( true )
			        {
				        if ( i == v ) 
					        return false;
        				
				        b = ( b * b ) % n;

				        if ( b == n - 1 )
					        break;

				        if ( b == 1 ) 
					        return false;

				        i += 1;
			        }
		        }
	        }

	        return true;
        }

        public static bool IsProbablePrime(BigInt n)
        {
	        return FermatIsProbablePrime( n ) 
		           && MillerRabinIsProbablePrime( n );
        }

	    public uint np;
	    public uint[] pl;
        private static Random random = new System.Random();
    }

    class Pair
    {
        public Pair()
        {

        }

        public Pair(BigInt r, BigInt s)
        {
            this.r = r;
            this.s = s;
        }

        public BigInt r = new BigInt();
        public BigInt s = new BigInt();
    }

    class EccBase : Curve
    {
        public EccBase(int curveType) : this(new FullCurveParameter(eccCurveTable[curveType]))
        {

        }

        public EccBase(FullCurveParameter a)
            : base(a)
        {
            bits = p.Bits;
        }

    	~EccBase()
        {

        }

        public delegate BigInt HashCallback(BigInt b, BigInt p);

	    new public uint Rand(uint max)
        {
            uint rnd;

            lock (random)
            {
                rnd = (uint)random.Next((int)max);
            }

            return rnd;
        }

	    public BigInt CreatePrivateKey()
        {
            BigInt x = 0;

            while (x < p)
            {
                x = (x << 15) + Rand(1u << 15);
            }

            x = 2 + x % (p - 3);

            return x;
        }

	    public BigInt CreatePublicKey(BigInt privateKey)
        {
            return Pack(privateKey * PP);
        }

	    public BigInt Encrypt(BigInt publicKey, BigInt message)
        {
            BigInt sr = CreatePrivateKey();

            message = Pack(sr * PP);

            return ToVLong(sr * Unpack(publicKey));
        }

	    public BigInt Decrypt(BigInt privateKey, BigInt message)
        {
            Point D = privateKey * Unpack(message);

            return ToVLong(D);
        }

	    public Pair SchnorrSign(BigInt data, BigInt privateKey, HashCallback hash, uint hashbits)
        {
            Pair sig = new Pair();
            BigInt k;

            do
            {
                k = CreatePrivateKey();
                BigInt z = ToVLong(k * PP);
                sig.r = (hash != null) ? hash(z + data, p) % (BigInt.Pow2(hashbits) - 1) : (z + data) % p;
            } while (sig.r == 0);

            sig.s = (k + privateKey * sig.r) % p;

            return sig;
        }

	    public bool SchnorrVerify(BigInt e, Pair sig, BigInt publicKey, HashCallback hash, uint hashbits)
        {
            BigInt bz = ToVLong(sig.s * PP - sig.r * Unpack(publicKey));
            BigInt data = (hash != null) ? hash(e + bz, p) % (BigInt.Pow2(hashbits) - 1) : (e + bz) % p;

            return (data == sig.r) ? true : false; // avoid compiler warning
        }

	    public Pair DsaSign(BigInt msg, BigInt privateKey, HashCallback hash)
        {
            Pair sig = new Pair();
            BigInt k;

            do
            {
                k = CreatePrivateKey();
                BigInt z = ToVLong(k * PP);
                sig.r = z % p;
            } while (sig.r == 0);

            sig.s = (BigInt.ModInv(k, p) * (hash(msg, p) + privateKey * sig.r)) % p;

            return sig;
        }

	    public bool DsaVerify( BigInt e, Pair sig, BigInt publicKey, HashCallback hash)
        {
            BigInt c = BigInt.ModInv(sig.s, p);
            BigInt u1 = (hash(e, p) * c) % p;
            BigInt u2 = (sig.r * c) % p;
            BigInt z = ToVLong(u1 * PP + u2 * Unpack(publicKey)) % p;

            return (z == sig.r) ? true : false;
        }

	    private static uint bits; // number of bits in prime order
        private static System.Random random = new System.Random();

        private static CurveParameter[] eccCurveTable = new CurveParameter[10]{ 
                                              new CurveParameter( 9, 7, 1, 17, 43, 48, 1),
                        	  				  new CurveParameter( 12, 7, 1, 83, 2049, 10, 1 ), 
										      new CurveParameter( 13, 7, 1, 27, 124, 52, 1),
											  new CurveParameter( 15, 7, 1, 3, 977, 110, 1),
											  new CurveParameter( 16, 7, 1, 45, 2902, 6, 1),
											  new CurveParameter( 10, 11, 2, 9, 139, 34, 1),
											  new CurveParameter( 12, 11, 2, 83, 2083, 66, 1),
											  new CurveParameter( 13, 11, 2, 27, 773, 48, 1),
											  new CurveParameter( 14, 11, 2, 43, 146, 40, 1),
											  new CurveParameter( 10, 17, 3, 9, 48, 28, 1) };
    }
}
