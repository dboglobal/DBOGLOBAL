#pragma once

#include <WinCrypt.h>
#include "Constant.h"

#pragma warning(disable : 4100 )

// This is cipher object class
class CCipher
{
public:
	CCipher(void);
	~CCipher(void);
	virtual	int SetKey(unsigned char * cipherKey, int cipherKeyLength ) = 0;
	virtual int Encrypt( char* originalData, int originalDataLength, char* encData,int encDataSize) {return 0;};
	virtual int Decrypt( char* originalData, int originalDataLength, char* decData,int decDataSize) {return 0;};
	virtual void ReleaseCipher(void){return;};

};

//generate random key class
//using by CryptGenRandom funtion 
class NCGenRandomKey
{
public:
	NCGenRandomKey(void);
	~NCGenRandomKey(void);
	
	bool Init(void);
	bool GenerateKey(int cipherKeyLength,unsigned char *cipherKey);
	int GenerateRSAKey(int cipherKeySize,unsigned char *cipherKey);
	int GenerateRSAPrivateKey(int cipherKeySize,unsigned char *cipherKey);

private:
	HCRYPTPROV		mhProv;
	
};