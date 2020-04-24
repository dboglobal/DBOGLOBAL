#include "stdafx.h"
#include "Cipher.h"

CCipher::CCipher(void)
{
}

CCipher::~CCipher(void)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Generate Key Class
NCGenRandomKey::NCGenRandomKey(void)
{
	mhProv = 0;
}

NCGenRandomKey::~NCGenRandomKey(void)
{
	if (mhProv) 
		CryptReleaseContext(mhProv,0);
}
// initialize cryptograpic context
bool NCGenRandomKey::Init(void)
{
	bool ret = true;

	if (!mhProv) {
		if(!CryptAcquireContext(&mhProv,NULL,MS_ENHANCED_PROV,PROV_RSA_FULL,0)) {
			ret = false;
			if (GetLastError() == NTE_BAD_KEYSET)  {
				if (CryptAcquireContext(&mhProv,NULL,MS_ENHANCED_PROV,PROV_RSA_FULL,CRYPT_NEWKEYSET))
					ret = true;
			}
		}
	}
	return ret;
}

// generate random key
//	[IN]		cipherKeyLength : Number of bytes of random data to be generated. 
//	[OUT]		cipherKey		: Buffer to receive the returned data. This buffer must be at least dwLen bytes in length. 
bool NCGenRandomKey::GenerateKey(int cipherKeyLength,unsigned char *cipherKey)
{
	bool ret = false;

	if ((cipherKeyLength < 0) || (!mhProv) || (!cipherKey))
		return false;

	if(CryptGenRandom(mhProv,cipherKeyLength,cipherKey))
		ret = true;

	return ret;
}

// generate rsa cipher public random key
//	[IN]		cipherKeySize	: cipherKey buffer size
//	[OUT]		cipherKey		: Buffer to receive the returned data.
//  [RETURN]	public key length 
int NCGenRandomKey::GenerateRSAKey(int cipherKeySize, unsigned char *cipherKey)
{
	int rsaKeyLen = 0;

	HCRYPTKEY	hRSAKey;
	unsigned char publicKey[BASE_BUFFER_SIZE];

	if ((!mhProv) || (!cipherKey) || (cipherKeySize < RSA1024BIT_KEY_SIZE +20) )
		return rsaKeyLen;

	memset(publicKey,0,sizeof(publicKey));
	if (CryptGenKey(mhProv,CALG_RSA_KEYX, RSA1024BIT_KEY|AT_KEYEXCHANGE,&hRSAKey)) {
		rsaKeyLen = cipherKeySize;
		if(CryptExportKey(hRSAKey,0,PUBLICKEYBLOB,0,publicKey,(DWORD*)&rsaKeyLen)) {
			publicKey[rsaKeyLen] ='\0';
			//rsa cipher algorithm padding 20bytes
			//rsaKeyLen -= 20;
			memcpy(cipherKey,publicKey,rsaKeyLen);
		} else {
			rsaKeyLen = 0;
		}
		CryptDestroyKey(hRSAKey);
	}

	return rsaKeyLen;
}

// generate rsa cipher private random key
//	[IN]		cipherKeySize	: cipherKey buffer size
//	[OUT]		cipherKey		: Buffer to receive the returned data.
//  [RETURN]	public key length 
int NCGenRandomKey::GenerateRSAPrivateKey(int cipherKeySize, unsigned char *cipherKey)
{
	int rsaKeyLen = 0;

	HCRYPTKEY	hRSAKey;
	unsigned char privateKey[BASE_BUFFER_SIZE];

	if ((!mhProv) || (!cipherKey) || (cipherKeySize < RSA1024BIT_KEY_SIZE +20) )
		return rsaKeyLen;

	memset(privateKey,0,sizeof(privateKey));
	if (CryptGenKey(mhProv,CALG_RSA_KEYX, RSA1024BIT_KEY|AT_KEYEXCHANGE,&hRSAKey)) {
		rsaKeyLen = cipherKeySize;
		if(CryptExportKey(hRSAKey,0,PRIVATEKEYBLOB,0,privateKey,(DWORD*)&rsaKeyLen)) {
			privateKey[rsaKeyLen] ='\0';
			//rsa cipher algorithm padding 20bytes
			//rsaKeyLen -= 20;
			memcpy(cipherKey,privateKey,rsaKeyLen);
		} else {
			rsaKeyLen = 0;
		}
		CryptDestroyKey(hRSAKey);
	}

	return rsaKeyLen;
}
