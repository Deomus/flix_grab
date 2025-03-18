//
// Copyright (c) 2014 SoftActivate. All rights reserved.
// 

using System.Text;

namespace SoftActivate.Licensing
{
    class BigInt
    {
        public BigInt()
        {

        }

        public BigInt(uint x)
        {
            Set(0u, x);
        }

        protected BigInt(BigInt x)
        {
            z = x.z;
            if (x.a != null) a = (uint[])x.a.Clone();
            n = x.n;
            negative = x.negative;
        }

        ~BigInt()
        {

        }

        public override string ToString()
        {
            StringBuilder result = new StringBuilder("(" + Bits.ToString() + ",");

            for (int i = 0; i < n; i++)
            {
                result.Append(a[i].ToString("X"));
                result.Append(",");
            }

            result.Remove(result.Length - 1, 1);
           
            result.Append(")");

            return result.ToString();
        }

        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

        public override bool Equals(System.Object obj)
        {
            if (obj == null)
                return false;

            return (this == (BigInt)obj);
        }

        public uint Get(uint i)
        {
            if (i >= n)
                return 0;

            return a[i];
        }

        private void Clear()
        {
            n = 0;
        }

        public uint Bits
        {
            get
            {
                uint x = n;

                if (x > 0)
                {
                    uint w = BPU;
                    uint msw = Get(x - 1);

                    x = (x - 1) * BPU;

                    do
                    {
                        w >>= 1;
                        if (msw >= (1u << (int)w))
                        {
                            x += w;
                            msw >>= (int)w;
                        }
                    } while (w > 8);

                    x += bittab[msw];
                }

                return x;
            }
        }

        public int Bit(uint i)
        {
            return ((Get(i / BPU) & (1u << (int)(i % BPU))) != 0) ? 1 : 0;
        }

        public static implicit operator BigInt(uint x)
        {
            return new BigInt(x);
        }

        public static implicit operator uint(BigInt x)
        {
            return x.Get(0);
        }

        // Standard arithmetic operators
        public static BigInt operator +(BigInt x, BigInt y)
        {
            BigInt result = (BigInt)x.Clone();

            result.Add(y);

            return result;
        }

        public static BigInt operator -(BigInt x, BigInt y)
        {
            BigInt result = (BigInt)x.Clone();

            result.Substract(y);

            return result;
        }

        public static BigInt operator *(BigInt x, BigInt y)
        {
            BigInt result = new BigInt();

            result.Multiply(x, y);
            result.negative = x.negative ^ y.negative;

            return result;
        }

        public static BigInt operator /(BigInt x, BigInt y)
        {
            BigInt result = new BigInt();
            BigInt rem = new BigInt();

            result.Divide(x, y, rem);
            result.negative = x.negative ^ y.negative;

            return result;
        }

        public static BigInt operator %(BigInt x, BigInt y)
        {
            BigInt result = new BigInt();
            BigInt divide = new BigInt();

            divide.Divide(x, y, result);
            result.negative = x.negative; // not sure about this?

            return result;
        }

        public static BigInt operator ^(BigInt x, BigInt y)
        {
            BigInt result = (BigInt)x.Clone();

            result.Xor(y);

            return result;
        }

        public static BigInt Pow2(uint n)
        {
            BigInt result = new BigInt();

            result.SetBit(n);

            return result;
        }

        public static BigInt operator &(BigInt x, BigInt y)
        {
            BigInt result = (BigInt)x.Clone();

            result.And(y);

            return result;
        }

        public static BigInt operator <<(BigInt x, int n)
        {
            BigInt result = (BigInt)x.Clone();

            while (n > 0)
            {
                n -= 1;
                result = result + result;
            }

            return result;
        }

        public static BigInt operator >>(BigInt x, int n)
        {
            BigInt result = (BigInt)x.Clone();
            result.Shr((uint)n);
            return result;
        }

        // Standard comparison operators
        public static bool operator !=(BigInt x, BigInt y)
        {
            return x.CF(y) != 0;
        }

        public static bool operator ==(BigInt x, BigInt y)
        {
            return x.CF(y) == 0;
        }

        public static bool operator >=(BigInt x, BigInt y)
        {
            return x.CF(y) >= 0;
        }

        public static bool operator <=(BigInt x, BigInt y)
        {
            return x.CF(y) <= 0;
        }

        public static bool operator >(BigInt x, BigInt y)
        {
            return x.CF(y) > 0;
        }

        public static bool operator <(BigInt x, BigInt y)
        {
            return x.CF(y) < 0;
        }

        // Absolute value
        public static BigInt Abs(BigInt x)
        {
            BigInt result = (BigInt)x.Clone();

            result.negative = false;

            return result;
        }

        public static BigInt Gcd(BigInt px, BigInt py)
        {
            BigInt x = (BigInt)px.Clone(),
                   y = (BigInt)py.Clone();

            while (true)
            {
                if (y == 0) return x;
                x = x % y;
                if (x == 0) return y;
                y = y % x;
            }
        }

        public BigInt Ror(uint n)
        {
            BigInt result = (BigInt)Clone();

            bool carry = (result.Shr() != 0);
            if (carry) result.SetBit(result.n - 1);

            return result;
        }

        public BigInt Rol(uint n) // single bit rotate
        {
            BigInt result = (BigInt)Clone();

            int carry = result.Bit(n - 1);
            if (carry != 0) result.ClearBit(n - 1);
            Shl();
            if (carry != 0) result.SetBit(0);

            return result;
        }

        public void Load(uint[] a, uint n)
        {
            Clear();
            for (uint i = 0; i < n; i += 1)
                Set(i, a[i]);
        }

        public void Store(out uint[] a)
        {
            a = new uint[(Bits + (sizeof(uint) << 3) - 1) / (sizeof(uint) << 3)];
            for (uint i = 0; i < n; i += 1)
                a[i] = Get(i);
        }

        public static BigInt ModInv(BigInt a, BigInt m) // modular inverse
        // returns i in range 1..m-1 such that i*a = 1 mod m
        // a must be in range 1..m-1
        {
            BigInt j = new BigInt(1u),
                   i = new BigInt(0u),
                   b = (BigInt)m.Clone(),
                   c = (BigInt)a.Clone(),
                   x = new BigInt(),
                   y = new BigInt();

            while (!c.IsZero())
            {
                x = b / c;
                y = b - x * c;
                b = c;
                c = y;
                y = j;
                j = i - j * x;
                i = y;
            }

            if (i.negative)
                i += m;

            return i;
        }

        public static BigInt ModExp(BigInt x, BigInt e, BigInt m)
        {
            Monty me = new Monty(m);
            return me.Exp(x, e);
        }

        public static BigInt Half(BigInt a, BigInt p)
        {
            if (a.Bit(0) != 0)
                return (a + p) / 2;

            return a / 2;
        }

        public static BigInt Lucas(BigInt p, BigInt z, BigInt k, BigInt p2) // P^2 - 4Z != 0
        {
            BigInt D = p * p - 4 * z,
                   U = 1,
                   V = new BigInt(p),
                   U1,
                   V1;
            uint i = k.Bits - 1;

            while (i > 0)
            {
                i -= 1;

                U1 = U * V;
                V1 = V * V + D * U * U;
                U = U1 % p2;
                V = Half(V1 % p2, p2);

                if (k.Bit(i) != 0)
                {
                    U1 = p * U + V;
                    V1 = p * V + D * U;
                    U = Half(U1 % p2, p2);
                    V = Half(V1 % p2, p2);
                }
            }

            return V;
        }

        public static BigInt Sqrt(BigInt g, BigInt p)
        {
            BigInt result;

            if (p % 4 == 3)
                result = ModExp(g, p / 4 + 1, p);
            else if (p % 8 == 5)
            {
                BigInt gamma = ModExp(2 * g, p / 8, p);
                BigInt i = 2 * g * gamma * gamma - 1;
                result = g * gamma * i;
            }
            else
            {
                BigInt Z = new BigInt(g);
                BigInt P = 1;

                while (true)
                {
                    BigInt D = (P * P - 4 * g) % p;

                    if (D < 0) D += p;

                    if (D == 0)
                    {
                        result = Half(P, p);
                        break;
                    }

                    if (ModExp(D, (p - 1) / 2, p) != 1)
                    {
                        result = Half(Lucas(P, Z, (p + 1) / 2, p), p);
                        break;
                    }

                    P += 1; // Is this ok (efficient?)
                }
            }

            result = result % p;
            if (result < 0) result += p;

            return result;
        }

        private void Set(uint i, uint x)
        {
            if (i < n)
            {
                a[i] = x;

                if (x == 0)
                {
                    while (n > 0 && a[n - 1] == 0)
                        n -= 1; // normalise
                }
            }
            else if (x > 0)
            {
                Reserve(i + 1);

                for (uint j = n; j < i; j += 1)
                    a[j] = 0;

                a[i] = x;
                n = i + 1;
            }
        }

        private void Reserve(uint x)
        {
            if (x > z)
            {
                uint[] na = new uint[x];

                for (uint i = 0; i < n; i += 1)
                    na[i] = a[i];

                a = na;
                z = x;
            }
        }

        private static uint DoInner(uint n, uint m, uint[] a, uint offset, uint[] ya)
        {
            uint i = offset, j = 0, c = 0;

            while (n-- > 0)
            {
                uint v = a[i], p = ya[j];
                j++;
                v += c;
                c = (v < c) ? 1u : 0u;
                uint w;
                w = lo(p) * lo(m);
                v += w;
                c += (v < w) ? 1u : 0u;
                w = lo(p) * hi(m);
                c += hi(w);
                w = lh(w);
                v += w;
                c += (v < w) ? 1u : 0u;
                w = hi(p) * lo(m);
                c += hi(w);
                w = lh(w);
                v += w;
                c += (v < w) ? 1u : 0u;
                c += hi(p) * hi(m);
                a[i] = v; i++;
            }

            return c;
        }

        private void FastMul(BigInt x, BigInt y, uint keep)
        {
            uint i, limit = (keep + BPU - 1) / BPU; // size of result in words

            Reserve(limit);

            for (i = 0; i < limit; i += 1)
                a[i] = 0;

            uint min2 = x.n;

            if (min2 > limit)
                min2 = limit;

            for (i = 0; i < min2; i += 1)
            {
                uint m = x.a[i];
                uint min = i + y.n;
                if (min > limit)
                    min = limit;

                uint c = DoInner(min - i, m, a, i, y.a);
                uint j = min;

                while (c > 0 && j < limit)
                {
                    a[j] += c;
                    c = (uint)((a[j] < c) ? 1 : 0);
                    j += 1;
                }
            }

            // eliminate unwanted bits
            keep %= BPU;
            if (keep > 0)
                a[limit - 1] &= (1u << (int)keep) - 1;

            // calculate n
            while (limit > 0 && a[limit - 1] == 0)
                limit -= 1;

            n = limit;
        }

        private bool IsZero()
        {
            return (n == 0);
        }

        private void SetBit(uint i)
        {
            Set(i / BPU, Get(i / BPU) | (1u << (int)(i % BPU)));
        }

        private void ClearBit(uint i)
        {
            Set(i / BPU, Get(i / BPU) & ~(1u << (int)(i % BPU)));
        }

        private int CF(BigInt x)
        {
            if (n > x.n)
                return +1;

            if (n < x.n)
                return -1;

            uint i = n;

            while (i > 0)
            {
                i -= 1;

                if (Get(i) > x.Get(i))
                    return +1;

                if (Get(i) < x.Get(i))
                    return -1;
            }

            return 0;
        }

        private void Shl()
        {
            uint carry = 0;
            uint N = n; // necessary, since n can change

            for (uint i = 0; i <= N; i += 1)
            {
                uint u = Get(i);
                Set(i, (u << 1) + carry);
                carry = u >> (int)(BPU - 1);
            }
        }

        private int Shr() // result is carry
        {
            uint carry = 0;
            uint i = n;

            while (i > 0)
            {
                i -= 1;
                uint u = Get(i);
                Set(i, (u >> 1) + carry);
                carry = u << (int)(BPU - 1);
            }

            return (carry != 0) ? 1 : 0;
        }

        private void Shr(uint x)
        {
            uint delta = x / BPU;

            x %= BPU;

            for (uint i = 0; i < n; i += 1)
            {
                uint u = Get(i + delta);

                if (x > 0)
                {
                    u >>= (int)x;
                    u += Get(i + delta + 1) << (int)(BPU - x);
                }

                Set(i, u);
            }
        }

        private void UnitAdd(BigInt x)
        {
            uint carry = 0;
            uint max = n;

            if (max < x.n)
                max = x.n;

            Reserve(max);

            for (uint i = 0; i < max + 1; i += 1)
            {
                uint u = Get(i);
                u = u + carry;
                carry = (u < carry) ? 1u : 0u;
                uint ux = x.Get(i);
                u = u + ux; carry += (u < ux) ? 1u : 0u;
                Set(i, u);
            }
        }

        private void Add(BigInt x)
        {
            if (negative == x.negative)
            {
                UnitAdd(x);
            }
            else if (CF(x) >= 0)
            {
                UnitSub(x);
            }
            else
            {
                BigInt tmp = new BigInt(this);
                Copy(x);
                Add(tmp);
            }
        }

        private void Substract(BigInt y)
        {
            if (negative != y.negative)
            {
                UnitAdd(y);
            }
            else
                if (CF(y) >= 0)
                {
                    UnitSub(y);
                }
                else
                {
                    BigInt x = new BigInt(this);
                    Copy(y);
                    Substract(x);
                    negative = !negative;
                }
        }

        private void Xor(BigInt x)
        {
            uint max = n;

            if (max < x.n)
                max = x.n;

            Reserve(max);

            for (uint i = 0; i < max; i += 1)
            {
                Set(i, Get(i) ^ x.Get(i));
            }
        }

        private void And(BigInt x)
        {
            uint max = n;

            if (max < x.n)
                max = x.n;

            Reserve(max);

            for (uint i = 0; i < max; i += 1)
            {
                Set(i, Get(i) & x.Get(i));
            }
        }

        private void UnitSub(BigInt x)
        {
            uint carry = 0;
            uint N = n;

            for (uint i = 0; i < N; i += 1)
            {
                uint ux = x.Get(i);
                ux += carry;
                if (ux >= carry)
                {
                    uint u = Get(i);
                    uint nu = u - ux;
                    carry = (nu > u) ? 1u : 0u;
                    Set(i, nu);
                }
            }
        }

        private void Init(uint x)
        {
            Clear();
            Set(0u, x);
        }

        private void Copy(BigInt x)
        {
            uint i = x.n;

            Clear();

            while (i > 0)
            {
                i -= 1;
                Set(i, x.Get(i));
            }

            negative = x.negative;
        }

        public uint ToUnsigned() // Unsafe conversion to unsigned
        {
            return Get(0);
        }

        private void Multiply(BigInt x, BigInt y)
        {
           FastMul(x, y, x.Bits + y.Bits);
        }

        private void Divide(BigInt x, BigInt y, BigInt r)
        {
            BigInt m = new BigInt(), s = new BigInt();

            Init(0);
            r.Copy(x);
            m.Copy(y);
            s.Init(1u);

            while (r.CF(m) > 0)
            {
                m.Shl();
                s.Shl();
            }

            while (r.CF(y) >= 0)
            {
                while (r.CF(m) < 0)
                {
                    m.Shr();
                    s.Shr();
                }

                r.UnitSub(m);

                UnitAdd(s);
            }
        }

        #region ICloneable Members

        public object Clone()
        {
            return new BigInt(this);
        }

        #endregion

        public bool negative;
        private uint n;
        private uint[] a;
        private uint z;
        private const uint BPU = 8 * sizeof(uint);
        private static uint lo(uint x) { return ((x) & ((1u << (int)(BPU / 2)) - 1)); } // lower half of unsigned
        private static uint hi(uint x) { return ((x) >> (int)(BPU / 2)); }         // upper half
        private static uint lh(uint x) { return ((x) << (int)(BPU / 2)); }         // make upper half
        private static byte[] bittab =
        { 
            0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
            6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
            7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
            7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
            8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
            8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
            8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
            8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8 
        };

        class Monty // class for montgomery modular exponentiation
        {
            public Monty(BigInt mp)
            {
                m = (BigInt)mp.Clone();
                // N = 0;
                R = 1;
                T = new BigInt();
                k = new BigInt();

                while (R < mp)
                {
                    R = R + R;
                    N = N + 1;
                }

                R1 = BigInt.ModInv(R - m, m);
                n1 = R - BigInt.ModInv(m, R);
            }

            ~Monty()
            {

            }

            public BigInt Exp(BigInt x, BigInt e)
            {
                return (MontyExp((x * R) % m, e) * R1) % m;
            }

            public BigInt MontyExp(BigInt x, BigInt e)
            {
                BigInt result = R - m,
                       t = (BigInt)x.Clone(); // don't convert input
                uint bits = e.Bits;
                uint i = 0;

                while (true)
                {
                    if (e.Bit(i) != 0)
                        Mul(result, t);
                    i += 1;
                    if (i == bits) break;
                    Mul(t, t);
                }

                return result; // don't convert output
            }

            private void Mul(BigInt x, BigInt y)
            {
                // T = x*y;
                T.FastMul(x, y, N * 2);

                // k = ( T * n1 ) % R;
                k.FastMul(T, n1, N);

                // x = ( T + k*m ) / R;
                x.FastMul(k, m, N * 2);
                x.UnitAdd(T);
                x.Shr(N);

                if (x >= m) x.UnitSub(m);
            }

            private BigInt R;
            private BigInt R1;

            private BigInt m, n1;
            private BigInt T, k;   // work registers
            private uint N;  // bits for R

        }
    }
}
