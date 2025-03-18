//
// Copyright (c) 2014 FreeGrabApp. All rights reserved.
//

#include "precomp.h"
#include "ecc.h"
#include "bigint.h"
#include "ec2m.h"
#include "sha1.h"
#include "except.h"
#include "base64.h"
#include "uniconv.h"

#ifndef _WIN32
#include <alloca.h>
#endif

namespace ECC {

#if 0
void biPrint( bigint & data )
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

static const curve_parameter eccCurveTable[ 10 ] = { { 9, 7, 1, 17, 43, 48, 1},
                                              { 12, 7, 1, 83, 2049, 10, 1 }, 
                                              { 13, 7, 1, 27, 124, 52, 1},
                                              { 15, 7, 1, 3, 977, 110, 1},
                                              { 16, 7, 1, 45, 2902, 6, 1},
                                              { 10, 11, 2, 9, 139, 34, 1},
                                              { 12, 11, 2, 83, 2083, 66, 1},
                                              { 13, 11, 2, 27, 773, 48, 1},
                                              { 14, 11, 2, 43, 146, 40, 1},
                                              { 10, 17, 3, 9, 48, 28, 1} };

static void UintArrayToByteArray(unsigned * srcArray, int bitCount, unsigned char * dstArray)
{
    int dstLength = ((int)bitCount + 7) >> 3;
    int srcLength = (dstLength + 3) >> 2;

    int i = 0;
    int j = 0;
    unsigned currentUint;

    if (dstLength > 3)
    {
        while (i < dstLength - 3)
        {
            currentUint = (j < srcLength) ? srcArray[j] : 0;

            dstArray[i + 3] = (unsigned char)(currentUint >> 24);
            dstArray[i + 2] = (unsigned char)((currentUint >> 16) & 0xFF);
            dstArray[i + 1] = (unsigned char)((currentUint >> 8) & 0xFF);
            dstArray[i] = (unsigned char)(currentUint & 0xFF);
            i += 4;
            j += 1;
        }
    }

    currentUint = (j < srcLength) ? srcArray[j] : 0;
    if (i < dstLength) dstArray[i] = (unsigned char)(currentUint & 0xFF); i++;
    if (i < dstLength) dstArray[i] = (unsigned char)((currentUint >> 8) & 0xFF); i++;
    if (i < dstLength) dstArray[i] = (unsigned char)((currentUint >> 16) & 0xFF);
}

static void ByteArrayToUintArray(unsigned char * srcArray, int srcLength, unsigned * dstArray)
{
    int roundedLen = (srcLength + sizeof(unsigned) - 1) / sizeof(unsigned);

    int i = 0;
    int j = 0;
    while (i < roundedLen - 1)
    {
        dstArray[i] = (unsigned)(((unsigned)srcArray[j + 3] << 24) | ((unsigned)srcArray[j + 2] << 16) | ((unsigned)srcArray[j + 1] << 8) | (unsigned)srcArray[j]);
        i += 1;
        j += 4;
    }

    dstArray[roundedLen - 1] = 0;

    if (j < srcLength) dstArray[roundedLen - 1] = (unsigned)(srcArray[j]); j++;
    if (j < srcLength) dstArray[roundedLen - 1] |= (unsigned)((unsigned)srcArray[j] << 8); j++;
    if (j < srcLength) dstArray[roundedLen - 1] |= (unsigned)((unsigned)srcArray[j] << 16); j++;
    if (j < srcLength) dstArray[roundedLen - 1] |= (unsigned)((unsigned)srcArray[j] << 24);
}

class KeyImpl {
friend class Key;
friend class SignerImpl;
friend class VerifierImpl;

friend void GenerateKeyPair(KEY_TYPE keyType, Key **privKey, Key **pubKey);

private:
    KeyImpl()
    {

    }

    KeyImpl(const void * keyBuf, int bufLen, KEY_TYPE keyType = ECC)
    {
        Load(keyBuf, bufLen, keyType);      
    }

    KeyImpl(const char_t * base64Key, KEY_TYPE keyType = ECC)
    {
        USES_CONVERSION;
        BASE64 base64;

        int len;
        unsigned char * buf = base64.decode(T2A(base64Key), t_strlen(base64Key), &len);
        Load(buf, len, keyType);
        delete buf;
    }

    void Load(const void * keyBuf, int bufLen, KEY_TYPE keyType = ECC)
    {
        if (keyType == ECC)
        {
            unsigned char * keyBytes = (unsigned char *)keyBuf;

            if ((keyBytes[0] >> 5) != 0)
                throw new KeyExceptionImpl(STATUS_INVALID_PARAM, _T("unsupported key version"));

            if (keyBytes[1] + 2 != bufLen)
                throw new KeyExceptionImpl(STATUS_INVALID_PARAM, _T("invalid key size"));

            isPrivate = ((keyBytes[0] & 0x10) != 0);
            SetKeyType((KEY_TYPE)(keyBytes[0] & 0x0F));

            int roundedLen = sizeof(unsigned) * ((bufLen - 2 + sizeof(unsigned) - 1) / sizeof(unsigned));
            unsigned * buf = (unsigned *)calloc(roundedLen, 1);
            if (!buf) return;

            ByteArrayToUintArray(keyBytes + 2, bufLen - 2, buf);

            m_key.load(buf, (unsigned)(roundedLen / sizeof(unsigned)));
        } else
        {
            SetKeyType(keyType);

            int roundedLen = sizeof(unsigned) * ((bufLen + sizeof(unsigned) - 1) / sizeof(unsigned));
            unsigned * buf = (unsigned *)calloc(roundedLen, 1);
            if (!buf) return;

            ByteArrayToUintArray((unsigned char *)keyBuf, bufLen, buf);

            m_key.load(buf, (unsigned)(roundedLen / sizeof(unsigned)));
        }
    }

    bool Store(void * buf, int * bufLen)
    {
        unsigned char * keyBytes = (unsigned char *)buf;

        int numBits = m_key.bits();

        int numBytes = (numBits + 7) >> 3;

        if (*bufLen < 2 + numBytes)
            return false;

        keyBytes[0] &= 0x1F; // set first 3 bits to 0 - version 1
        if (isPrivate) keyBytes[0] |= 0x10; else keyBytes[0] &= 0xEF; // the 4th bit is 1 if it's a private key
        keyBytes[0] = (keyBytes[0] & 0xF0) | (((unsigned char)m_keyType) & 0x0F); // the next 4 bits from 1st byte are the key type
        keyBytes[1] = numBytes; // the second byte specifies the key size

        int numUints = (numBytes + sizeof(unsigned) - 1) / sizeof(unsigned);
        unsigned * temp = (unsigned int *)_alloca(numUints * sizeof(unsigned));
        m_key.store(temp, numUints);

        UintArrayToByteArray(temp, numBits, keyBytes + 2);

        *bufLen = numBytes + 2;

        return true;
    }

    void SetKeyType(KEY_TYPE keyType)
    {
        m_keyType = keyType;
        switch (keyType)
        {
        case ECC_54: m_keySize = 54;break;
        case ECC_73: m_keySize = 73;break;
        case ECC_79: m_keySize = 79;break;
        case ECC_91: m_keySize = 91;break;
        case ECC_97: m_keySize = 97;break;
        case ECC_100: m_keySize = 100;break;
        case ECC_120: m_keySize = 120;break;
        case ECC_131: m_keySize = 131;break;
        case ECC_141: m_keySize = 141;break;
        case ECC_161: m_keySize = 161;break;
        }
    }

    KEY_TYPE GetKeyType()
    {
        return m_keyType;
    }

    KEY_TYPE m_keyType;
    int m_keySize;
    bigint m_key;
    bool isPrivate;
};

class SignerImpl {
public:
    SignerImpl()
    {
        m_hashBits = 0; // thus select ECDSA as the signing algorithm
    }

    ~SignerImpl()
    {

    }

    static bigint Hash(const bigint & x, const bigint & p)
    {
        unsigned int n = ( x.bits() + ( ( sizeof(unsigned) << 3 ) - 1 ) ) / ( sizeof( unsigned ) << 3 );
        unsigned int * a = (unsigned *)_alloca(n * sizeof(unsigned));
        unsigned int h[ 5 ];

        x.store(a, n);

        SHA1 sha;
        sha.Update((unsigned char *)a, n * sizeof(unsigned));
        sha.Final();
        sha.GetHash((unsigned char *)h);

        bigint r;
        r.load( h, 5 );

        return r % p;
    }

    void SetHashSize(int hashBits)
    {
        m_hashBits = hashBits;
    }

    void SetPrivateKey(const KeyImpl & privKey)
    {
        m_privateKey = privKey;
    }

    void Sign(const void * msg, int msgLen, void * sigBuf, int * sigLen, int * sigLenBits)
    {
        full_curve_parameter param(eccCurveTable[ m_privateKey.m_keyType ]);
        ECC_BASE ecc(param);
        bigint message;
        bigint temp;
        pair signature;

        *sigLenBits = (m_hashBits && m_hashBits < m_privateKey.m_keySize) ? m_hashBits + m_privateKey.m_keySize : m_privateKey.m_keySize << 1;
        *sigLen = (*sigLenBits + 7) >> 3;

        unsigned * buf = (unsigned *)_alloca(msgLen + *sigLen + sizeof(unsigned) - 1);
        // take care of endianess here, do not do just a memcpy()
        memcpy(buf, msg, msgLen);
        memset((char *)buf + msgLen, 0, sizeof(unsigned) - 1);
        
        unsigned msgHash[5];

        SHA1 sha;
        sha.Update((unsigned char *)msg, msgLen);
        sha.Final();

        sha.GetHash((unsigned char *)msgHash);

        message.load(msgHash, 5);

        signature = (m_hashBits && m_hashBits < m_privateKey.m_keySize) ? ecc.schnorr_sign(message, m_privateKey.m_key, Hash, m_hashBits) : ecc.dsa_sign(message, m_privateKey.m_key, Hash);

        temp = (signature.r << m_privateKey.m_keySize); temp += signature.s;

        temp.store(buf, (unsigned)((*sigLen + sizeof(unsigned) - 1) / sizeof(unsigned)));
         
        // take care of endianess here
        memcpy(sigBuf, buf, *sigLen);
    }

private:
    KeyImpl m_privateKey;
    int m_hashBits;
};

class VerifierImpl {
public:
    VerifierImpl()
    {
        m_hashBits = 0;
    }

    ~VerifierImpl()
    {

    }

    static bigint Hash(const bigint & x, const bigint & p)
    {
        int n = ( x.bits() + ( ( sizeof(unsigned) << 3 ) - 1 ) ) / ( sizeof( unsigned ) << 3 );
        unsigned int * a = (unsigned *)_alloca(n * sizeof(unsigned));
        unsigned int h[ 5 ];

        x.store(a, n);

        SHA1 sha;
        sha.Update((unsigned char *)a, (x.bits() + 7) >> 3);
        sha.Final();
        sha.GetHash((unsigned char *)h);

        bigint r;
        r.load( h, 5 );

        return r % p;
    }

    void SetHashSize(int hashBits)
    {
        m_hashBits = hashBits;
    }

    void SetPublicKey(const KeyImpl & pubKey)
    {
        m_publicKey = pubKey;
    }

    bool Verify(const void * msg, int msgLen, const void * sig, int sigLen, int sigLenBits)
    {
        full_curve_parameter param(eccCurveTable[ m_publicKey.m_keyType ]);
        ECC_BASE ecc(param);
        bigint message;
        pair signature;

        int msgBufLen = (msgLen +  sizeof(unsigned) - 1) / sizeof(unsigned);
        int sigBufLen = (sigLen +  sizeof(unsigned) - 1) / sizeof(unsigned);

        unsigned * msgBuf = (unsigned *)_alloca(msgBufLen * sizeof(unsigned));
        unsigned * sigBuf = (unsigned *)_alloca(sigBufLen * sizeof(unsigned));
        memset(msgBuf, 0, msgBufLen * sizeof(unsigned));
        memcpy(msgBuf, msg, msgLen);
        memset(sigBuf, 0, sigBufLen * sizeof(unsigned));
        memcpy(sigBuf, sig, sigLen);

        unsigned int msgHash[5];

        SHA1 sha;
        sha.Update((unsigned char *)msg, msgLen);
        sha.Final();

        sha.GetHash((unsigned char *)msgHash);

        message.load(msgHash, 5);

        signature.r.load(sigBuf, sigBufLen); signature.r >>= m_publicKey.m_keySize; signature.r &= (pow2((unsigned)(sigLenBits - m_publicKey.m_keySize)) - 1);
        signature.s.load(sigBuf, sigBufLen); signature.s &= (pow2(m_publicKey.m_keySize) - 1);

        return (m_hashBits && m_hashBits < m_publicKey.m_keySize) ? ecc.schnorr_verify(message, signature, m_publicKey.m_key, Hash, m_hashBits) : ecc.dsa_verify(message, signature, m_publicKey.m_key, Hash);
    }

private:
    int m_hashBits;
    KeyImpl m_publicKey;
};

void GenerateKeyPair(KEY_TYPE keyType, Key **privKey, Key **pubKey)
{
    full_curve_parameter param(eccCurveTable[ keyType ]);
    ECC_BASE ecc(param);
    Key * privateKey = new Key(), 
        * publicKey = new Key();

    privateKey->m_Impl.m_key = ecc.create_private_key();
    privateKey->m_Impl.SetKeyType(keyType);
    privateKey->m_Impl.isPrivate = true;

    publicKey->m_Impl.m_key = ecc.create_public_key(privateKey->m_Impl.m_key);
    publicKey->m_Impl.SetKeyType(keyType);

    *privKey = privateKey;
    *pubKey = publicKey;
}

Key::Key():
m_Impl( *new KeyImpl() )
{

}

Key::Key(const void * keyBuf, int bufLen, KEY_TYPE keyType):
m_Impl( *new KeyImpl(keyBuf, bufLen, keyType) )
{

}

Key::Key(const char_t * base64Key, KEY_TYPE keyType):
m_Impl( *new KeyImpl(base64Key, keyType))
{

}

Key::~Key()
{
    delete & m_Impl;
}

void Key::Load(const void * keyData, int keyLen, KEY_TYPE keyType)
{
    m_Impl.Load(keyData, keyLen, keyType);
}

bool Key::Store(void * buf, int * bufLen) const
{
    return m_Impl.Store(buf, bufLen);
}

const char_t * Key::ToString()
{
    USES_CONVERSION;
    static string_t base64key;

    int len = 2 + ((m_Impl.m_key.bits() + 7) >> 3);
    unsigned char * buf = (unsigned char *)_alloca(len);
    Store(buf, &len);

    BASE64 base64;

    char * base64Buf = base64.encode(buf, len, true);
    if (!base64Buf)
        throw new KeyExceptionImpl(STATUS_GENERIC_ERROR, _T("could not encode buffer to base64"));

    base64key = A2T(base64Buf);
    free(base64Buf);

    return base64key.c_str();
}

KEY_TYPE Key::GetKeyType()
{
    return m_Impl.GetKeyType();
}

const Key & Key::operator = (const Key & key)
{
    m_Impl = key.m_Impl;
    return (const Key &)(*this);
}

Signer::Signer():
m_Impl(*new SignerImpl())
{

}

Signer::~Signer()
{
    delete & m_Impl;
}

void Signer::SetHashSize(int hashBits)
{
    m_Impl.SetHashSize(hashBits);
}

void Signer::SetPrivateKey(const Key * privKey)
{
    m_Impl.SetPrivateKey(privKey->m_Impl);
}

void Signer::Sign(const void * msg, int msgLen, void * sigBuf, int * sigLen, int * sigLenBits)
{
    m_Impl.Sign(msg, msgLen, sigBuf, sigLen, sigLenBits);
}

Verifier::Verifier():
m_Impl( *new VerifierImpl() )
{

}

Verifier::~Verifier()
{
    delete & m_Impl;
}

void Verifier::SetHashSize(int hashBits)
{
    m_Impl.SetHashSize(hashBits);
}

void Verifier::SetPublicKey(const Key * pubKey)
{
    m_Impl.SetPublicKey(pubKey->m_Impl);
}

bool Verifier::Verify(const void * msg, int msgLen, const void * sig, int sigLen, int sigLenBits)
{
    return m_Impl.Verify(msg, msgLen, sig, sigLen, sigLenBits);
}

}
