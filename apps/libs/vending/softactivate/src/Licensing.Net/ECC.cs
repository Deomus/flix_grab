//
// Copyright (c) 2014 SoftActivate. All rights reserved.
//

using System;
using System.Collections.Generic;
using System.Text;
using System.Security.Cryptography;

namespace SoftActivate.Licensing
{
    sealed class Ecc
    {
        internal enum KeyType
        {
            Ecc54,
            Ecc73,
            Ecc79,
            Ecc91,
            Ecc97,
            Ecc100,
            Ecc120,
            Ecc131,
            Ecc141,
            Ecc161,
            Ecc,
            EccCustom,
        }

        Ecc()
        {

        }

        public static void GenerateKeyPair(KeyType keyType, out Key privKey, out Key pubKey)
        {
            EccBase ecc = new EccBase((int)keyType);
            privKey = new Key();
            privKey.KeyType = keyType;
            privKey.BigIntKey     = ecc.CreatePrivateKey();
            pubKey = new Key();
            pubKey.KeyType  = keyType;
            pubKey.BigIntKey  = ecc.CreatePublicKey(privKey.BigIntKey);
        }

        public static void UintArrayToByteArray(uint[] srcArray, uint bitCount, out byte[] dstArray)
        {
            int dstLength = ((int)bitCount + 7) >> 3;
            dstArray = new byte[dstLength];

            int i = 0;
            int j = 0;
            uint currentUint;

            if (dstLength > 3)
            {
                while (i < dstLength - 3)
                {
                    currentUint = (j < srcArray.Length) ? srcArray[j] : 0;

                    dstArray[i + 3] = (byte)(currentUint >> 24);
                    dstArray[i + 2] = (byte)((currentUint >> 16) & 0xFF);
                    dstArray[i + 1] = (byte)((currentUint >> 8) & 0xFF);
                    dstArray[i] = (byte)(currentUint & 0xFF);
                    i += 4;
                    j += 1;
                }
            }

            currentUint = (j < srcArray.Length) ? srcArray[j] : 0;
            if (i < dstLength) dstArray[i] = (byte)(currentUint & 0xFF); i++;
            if (i < dstLength) dstArray[i] = (byte)((currentUint >> 8) & 0xFF); i++;
            if (i < dstLength) dstArray[i] = (byte)((currentUint >> 16) & 0xFF);
        }

        public static void ByteArrayToUintArray(byte[] srcArray, int offset, int count, out uint[] dstArray)
        {
            int roundedLen = (count + sizeof(uint) - 1) / sizeof(uint);
            dstArray = new uint[ roundedLen ];

            int i = 0;
            int j = offset;
            while (i < roundedLen - 1)
            {
                dstArray[i] = (uint)((srcArray[j + 3] << 24) | (srcArray[j + 2] << 16) | (srcArray[j + 1] << 8) | srcArray[j]);
                i += 1;
                j += 4;
            }

            dstArray[roundedLen - 1] = 0;

            int srcEndOffset = offset + count;

            if (j < srcEndOffset) dstArray[roundedLen - 1] = (uint)(srcArray[j]); j++;
            if (j < srcEndOffset) dstArray[roundedLen - 1] |= (uint)(srcArray[j] << 8); j++;
            if (j < srcEndOffset) dstArray[roundedLen - 1] |= (uint)(srcArray[j] << 16); j++;
            if (j < srcEndOffset) dstArray[roundedLen - 1] |= (uint)(srcArray[j] << 24);
        }
    }

    class Key
    {
    	public Key()
        {

        }

        public Key(string base64key, Ecc.KeyType keyType = Ecc.KeyType.Ecc)
        {
            Load(Convert.FromBase64String(base64key), keyType);
        }

    	public Key(byte[] keyData)
        {
            Load(keyData);
        }

	    public void Load(byte[] keyData, Ecc.KeyType keyType = Ecc.KeyType.Ecc)
        {
            if (keyType == Ecc.KeyType.Ecc)
            {
                if ((keyData[0] >> 5) != 0)
                    throw new Exception("unsupported key version");

                if (2 + keyData[1] != keyData.Length)
                    throw new Exception("invalid embedded key length");

                isPrivate = ((keyData[0] & 0x10) != 0);

                uint[] buf;

                Ecc.ByteArrayToUintArray(keyData, 2, keyData.Length - 2, out buf);

                key = new BigInt();
                key.Load(buf, (uint)buf.Length);

                SetKeyType((Ecc.KeyType)(keyData[0] & 0x0F));
            }
            else
            {
                uint[] buf;

                Ecc.ByteArrayToUintArray(keyData, 0, keyData.Length, out buf);

                key = new BigInt();
                key.Load(buf, (uint)buf.Length);

                SetKeyType(keyType);
            }
        }

	    public byte[] Export()
        {
            uint[] temp;
            byte[] keyBuf;

            key.Store(out temp);
            Ecc.UintArrayToByteArray(temp, key.Bits, out keyBuf);

            byte[] buf = new byte[2 + keyBuf.Length];

            buf[0] = (byte)(((byte)0 << 5) | ((byte)keyType & 0x0F));

            if (isPrivate) buf[0] |= (0x10);

            buf[1] = (byte)keyBuf.Length;

            Buffer.BlockCopy(keyBuf, 0, buf, 2, keyBuf.Length);

            return buf;
        }
        
        private void SetKeyType(Ecc.KeyType type)
        {
            keyType = type;
            switch (keyType)
            {
                case Ecc.KeyType.Ecc54: keySize = 54; break;
                case Ecc.KeyType.Ecc73: keySize = 73; break;
                case Ecc.KeyType.Ecc79: keySize = 79; break;
                case Ecc.KeyType.Ecc91: keySize = 91; break;
                case Ecc.KeyType.Ecc97: keySize = 97; break;
                case Ecc.KeyType.Ecc100: keySize = 100; break;
                case Ecc.KeyType.Ecc120: keySize = 120; break;
                case Ecc.KeyType.Ecc131: keySize = 131; break;
                case Ecc.KeyType.Ecc141: keySize = 141; break;
                case Ecc.KeyType.Ecc161: keySize = 161; break;
            }
        }

        public uint KeySize
        {
            get
            {
                return keySize;
            }
        }

        public BigInt BigIntKey
        {
            get
            {
                return key;
            }
            set
            {
                key = value;
            }
        }

        public Ecc.KeyType KeyType
        {
            get
            {
                return keyType;
            }
            set
            {
                SetKeyType(value);
            }
        }

        public bool IsPrivate
        {
            get
            {
                return isPrivate;
            }
            set
            {
                isPrivate = value;
            }
        }

        private Ecc.KeyType keyType;
        private uint keySize;
        private BigInt key;
        private bool isPrivate;
    }

    delegate uint HashCallback(uint[] data, uint dataBits, uint[] hashedData, uint hashBits);

    class Signer
    {
	    public Signer()
        {

        }

	    ~Signer()
        {

        }

	    public void SetPrivateKey(Key privKey)
        {
            privateKey = privKey;
        }

	    public void SetHashSize(int hashBits)
        {
            this.hashBits = (uint)hashBits;
        }

	    BigInt Hash(BigInt x, BigInt p)
	    {
            uint[] uintX;
            x.Store(out uintX);

            byte[] byteX;
            Ecc.UintArrayToByteArray(uintX, x.Bits, out byteX);

            byte[] hash = sha1.ComputeHash(byteX);
            
            uint[] uintHash;
            Ecc.ByteArrayToUintArray(hash, 0, hash.Length, out uintHash);

            BigInt h = new BigInt();
            h.Load(uintHash, (uint)uintHash.Length);

            return h % p;
        }

        public void Sign(byte[] msg, int offset, int count, out byte[] sig, out int sigBits)
        {
            EccBase ecc = new EccBase((int)privateKey.KeyType);
		    BigInt message = new BigInt();
		    BigInt temp;
            Pair signature = new Pair();

            msg = sha1.ComputeHash(msg, offset, count);

            sigBits = (int)((hashBits > 0 && hashBits < privateKey.KeySize) ? hashBits + privateKey.KeySize : privateKey.KeySize << 1);

            uint[] buf;
            Ecc.ByteArrayToUintArray(msg, 0, msg.Length, out buf);

            message.Load(buf, (uint)buf.Length);

            //System.Diagnostics.Debug.WriteLine("[sign] Private Key: " + privateKey.Key.ToString());
            //System.Diagnostics.Debug.WriteLine("[sign] Message: " + message.ToString());

		    signature = (hashBits > 0 && hashBits < privateKey.KeySize) ? ecc.SchnorrSign(message, privateKey.BigIntKey, Hash, hashBits) : ecc.DsaSign(message, privateKey.BigIntKey, Hash);

            //System.Diagnostics.Debug.WriteLine("[sign] Signature.R: " + signature.r.ToString());
            //System.Diagnostics.Debug.WriteLine("[sign] Signature.S: " + signature.s.ToString());

		    temp = (signature.r << (int)privateKey.KeySize); temp += signature.s;

            temp.Store(out buf);

            Ecc.UintArrayToByteArray(buf, (uint)sigBits, out sig); 
        }

        private Key privateKey;
        private uint hashBits;
        private SHA1CryptoServiceProvider sha1 = new SHA1CryptoServiceProvider();
    }

    class Verifier 
    {
    	public Verifier()
        {

        }

    	~Verifier()
        {

        }

        BigInt Hash(BigInt x, BigInt p)
        {
            uint[] uintX;
            x.Store(out uintX);

            byte[] byteX;
            Ecc.UintArrayToByteArray(uintX, x.Bits, out byteX);

            byte[] hash = sha1.ComputeHash(byteX);

            uint[] uintHash;
            Ecc.ByteArrayToUintArray(hash, 0, hash.Length, out uintHash);

            BigInt h = new BigInt();
            h.Load(uintHash, (uint)uintHash.Length);

            return h % p;
        }

    	public void SetPublicKey(Key publicKey)
        {
            this.publicKey = publicKey;
        }

    	public void SetHashSize(uint hashBits)
        {
            this.hashBits = hashBits;
        }

        public bool Verify(byte[] msg, int msgOffset, int msgCount, byte[] sig, int sigOffset, int sigCount, int sigLenBits)
        {
            EccBase ecc = new EccBase((int)publicKey.KeyType);
		    BigInt message = new BigInt();
		    Pair signature = new Pair();

            msg = sha1.ComputeHash(msg, msgOffset, msgCount);

            uint[] msgBuf;
            Ecc.ByteArrayToUintArray(msg, 0, msg.Length, out msgBuf);

            uint[] sigBuf;
            Ecc.ByteArrayToUintArray(sig, sigOffset, sigCount, out sigBuf);

		    message.Load(msgBuf, (uint)msgBuf.Length);
		    signature.r.Load(sigBuf, (uint)sigBuf.Length); signature.r = signature.r >> (int)publicKey.KeySize; signature.r &= (BigInt.Pow2((uint)(sigLenBits - publicKey.KeySize)) - 1);
		    signature.s.Load(sigBuf, (uint)sigBuf.Length); signature.s &= (BigInt.Pow2(publicKey.KeySize) - 1);

            //System.Diagnostics.Debug.WriteLine("[verify] Public Key: " + publicKey.Key.ToString());
            //System.Diagnostics.Debug.WriteLine("[verify] Message: " + message.ToString());

            //System.Diagnostics.Debug.WriteLine("[verify] Signature.R: " + signature.r.ToString());
            //System.Diagnostics.Debug.WriteLine("[verify] Signature.S: " + signature.s.ToString());

		    return (hashBits != 0 && hashBits < publicKey.KeySize) ? ecc.SchnorrVerify(message, signature, publicKey.BigIntKey, Hash, hashBits) : ecc.DsaVerify(message, signature, publicKey.BigIntKey, Hash);
        }

        private uint hashBits;
        private Key publicKey;
        private SHA1CryptoServiceProvider sha1 = new SHA1CryptoServiceProvider();
    }
}
