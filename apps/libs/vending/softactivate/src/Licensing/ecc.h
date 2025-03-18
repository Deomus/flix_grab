#ifndef __ECC_H
#define __ECC_H

namespace ECC {

enum KEY_TYPE {
	ECC_54,
	ECC_73,
	ECC_79,
	ECC_91,
	ECC_97,
	ECC_100,
	ECC_120,
	ECC_131,
	ECC_141,
	ECC_161,
	ECC
};

class KeyImpl;

class Key {
public:
	Key();
	Key(const void * keyData, int keyLen, KEY_TYPE keyType = ECC);
	Key(const char_t * base64Key, KEY_TYPE keyType = ECC);
	~Key();

	void Load(const void * keyData, int keyLen, KEY_TYPE keyType = ECC);
	bool Store(void * buf, int * bufLen) const;
	const char_t * ToString();

	KEY_TYPE GetKeyType();

	const Key & operator = (const Key & key);

public:
	KeyImpl & m_Impl;
};

typedef unsigned (*HashFunction)(const void *, int, void *, unsigned *);

class SignerImpl;

class Signer
{
public:
	Signer();
	~Signer();

	void SetPrivateKey(const Key * privKey);
	void SetHashSize(int hashBits);

	void Sign(const void * msg, int msgLen, void * sigBuf, int * sigLen, int *sigLenInBits = (int *)0);

private:
	SignerImpl & m_Impl;
};

class VerifierImpl;

class Verifier {
public:
	Verifier();
	~Verifier();

	void SetPublicKey(const Key * pubKey);
	void SetHashSize(int hashBits);

	bool Verify(const void * msg, int msgLen, const void * sig, int sigLen, int sigLenInBits = 0);

private:
	VerifierImpl & m_Impl;
};

void GenerateKeyPair(KEY_TYPE keyType, Key **privKey, Key **pubKey);

};

#endif
