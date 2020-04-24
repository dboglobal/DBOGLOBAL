#pragma once

#include "Cipher.h"

class CCryptRSA
	:public CCipher
{
public:
	CCryptRSA(void);
	~CCryptRSA(void);
	
	virtual int SetKey(unsigned char *cipherKey, int cipherKeyLength);
	virtual int  Encrypt( char *originalData, int originalDataLength, char *encData, int encDataSize);
	virtual int  Decrypt( char *originalData, int originalDataLength, char *decData, int decDataSize);
	virtual void ReleaseCipher(void);

private:
	HCRYPTPROV				mhProv;       // CSP handle
	HCRYPTKEY				mhRSAKey;	  // RSA key handle
	void PrintError(DWORD errNum, int errMessageSize, wchar_t *errMessage);
};

// this class include genaration private key , public key funtion
class CCryptGenRSA
{
public:
	CCryptGenRSA(void);
	~CCryptGenRSA(void);
	bool SetCryptAPIHandle(void);
	int GetPublicKey(int publicKeySize, unsigned char * publicKeys);
	void FreeHandle(void);
	int GetPrivateKey(int privateKeySize, unsigned char *privateKeys);
	bool SetPublicKey(int keyLength, unsigned char *publicKeys);
	int Encrypt(unsigned char *originalData, int dataLength, unsigned char *outputData);
	int Decrypt(unsigned char *originalData, int dataLength, unsigned char *outputData);

private:
	HCRYPTPROV				mhProv;       // CSP handle
	HCRYPTKEY				mhRSAKey;	  // RSA key handle
	void PrintError(DWORD errNum, int errMessageSize, wchar_t *errMessage);
};

/// 3des algorithm
class CCryptTripleDes
	:public CCipher
{
public:
	CCryptTripleDes(void);
	~CCryptTripleDes(void);

	virtual void ReleaseCipher(void);
	virtual int SetKey(unsigned char *cipherKey, int cipherKeyLength);
	virtual int  Encrypt(unsigned char *originalData, int originalDataLength, unsigned char *encData, int encDataSize);
	virtual int  Decrypt(unsigned char *originalData, int originalDataLength, unsigned char *decData, int decDataSize);

private:
	HCRYPTPROV				mhProv;       // CSP handle
	HCRYPTKEY				mhTripleDesKey;	  // 3Des key handle

	void PrintError(DWORD errNum, int errMessageSize, wchar_t *errMessage);
};
