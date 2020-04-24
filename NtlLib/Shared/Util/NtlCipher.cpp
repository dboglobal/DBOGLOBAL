#include "StdAfx.h"
#include "CDes.h"
#include "TwofishCipher.h"
#include "AESCipher.h"
#include "CryptRSA.h"
#include "blowfish.h"
#include "md5.h"
#include "TripleDes.h"
#include "NtlCipher.h"

#include <wchar.h>

CNtlCipher::CNtlCipher(void)
{
	mCipherPtr = NULL;
}


CNtlCipher::~CNtlCipher(void)
{	
	if (mCipherPtr) {
		mCipherPtr->ReleaseCipher();
		delete mCipherPtr;
		mCipherPtr = NULL;
	}	
}

// choose cipher algorithme , intialize cipher key
// [IN]		cipher			:cipher algorithme
//			cipherKey		:algorithem key
//			cipherKeyLength	: cipherKey length
// [ret]	boolean 
int  CNtlCipher::SetKey(unsigned char cipher, const char * cipherKey, int cipherKeyLength)
{
	
	if ((cipher < DES_CIPHER )|| (cipher > AES_CIPHER_NEW))
		return INVALID_CIPHER_TYPE;

	if (mCipherPtr != NULL) 
		return CIPHER_OBJECT_ERROR;

	if ((cipher != MD5_CIPHER) && (!cipherKey))
		return CIPHER_OBJECT_ERROR;
	
	if (cipher == DES_CIPHER)
	{
		if ((cipherKeyLength < 0) ||   (cipherKeyLength > 40))
			return INVALID_KEY_LENGTH;
	} 
	else if (cipher == BLOWFISH_CIPHER) 
	{
		if ((cipherKeyLength < KEY_SIZE_16) || (cipherKeyLength > 56))
			return INVALID_KEY_LENGTH;
		else if ((cipherKeyLength % 4 ) != 0)
			return INVALID_KEY_LENGTH;
	} 
	else if ((cipher == TWOFISH_CIPHER) || (cipher == AES_CIPHER)) 
	{
		if ((cipherKeyLength != KEY_SIZE_16) && (cipherKeyLength != KEY_SIZE_24) && (cipherKeyLength !=KEY_SIZE_32))
			return INVALID_KEY_LENGTH;
	} 
	else if (cipher == TRIPLEDES_CIPHER) 
	{
		if((cipherKeyLength < KEY_SIZE_16) || (cipherKeyLength > KEY_SIZE_24))
			return INVALID_KEY_LENGTH;
	}
	
	mCipherType = cipher;
	switch (mCipherType) 
	{
	case DES_CIPHER:
		mCipherPtr = new CDes;	
		break;
	case TWOFISH_CIPHER:
		mCipherPtr = new CTwofishCipher;
		break;
	case AES_CIPHER:
		mCipherPtr = new CAESCipher;
		break;
	case TRIPLEDES_CIPHER:
		//mCipherPtr = new CCryptTripleDes;
		mCipherPtr = new CTripleDes;
		break;
	case BLOWFISH_CIPHER:
		mCipherPtr = new CBlowfish;
		break;
	case RSA_CIPHER:
		mCipherPtr = new CCryptRSA;
		break;
	case AES_CIPHER_NEW:
		mCipherPtr = new CAESCipher;
		break;
	default:
		break;
	}
	
	int ret;
	if (!mCipherPtr) {
		return CIPHER_OBJECT_ERROR;
	} else {
		//BY WONWOO FOR TEST 2005-07-29
		/*FILE *fp = _wfopen(L"c:\\test.txt",L"a+");
		if (fp) {
			fwprintf(fp,L"new\r\n");
			fclose(fp);
		}*/
	}
	if (cipher == MD5_CIPHER)
		ret = CIPHER_SUCCESS;
	else
		ret = mCipherPtr->SetKey((unsigned char*)cipherKey,cipherKeyLength);
	
	return ret;
}

// encrypt data
// [IN]		originalData			: original data before encrypt.
//			originalDataLength		: originalData length
//			encDataSize				: encData size
// [out]	encData					: encrypt Data
// [ret]	Encrypt Data Length
int CNtlCipher::Encrypt( char* originalData, int originalDataLength, char* encData,int encDataSize)
{
	int retLength = 0;
	
	if (!mCipherPtr) 
		return CIPHER_OBJECT_ERROR;
	
	if (!encData) 
		return NULL_OUTPUTDATA;
	
	if (!originalData)
		return NULL_INPUTDATA;

	
	if (originalDataLength < 1)
		return INVALID_INPUTDATA_LENGTH;
	
	if (encDataSize < 1)
		return INVALID_OUTPUTDATA_SIZE;

	retLength = mCipherPtr->Encrypt(originalData, originalDataLength, encData, encDataSize);

	return retLength;
}

// decrypt data
// [IN]		originalData			: encrypt data
//			originalDataLength		: originalData length
//			decDataSize				: decData size
// [out]	decData					: decrypt Data
// [ret]	Decrypt Data Length
int CNtlCipher::Decrypt( char* originalData, int originalDataLength, char* decData,int decDataSize)
{	
	int retLength =0 ;

	if (!mCipherPtr) 
		return CIPHER_OBJECT_ERROR;

	if (!decData) 
		return NULL_OUTPUTDATA;
	
	if (!originalData)
		return NULL_INPUTDATA;
	
	if (originalDataLength < 1 || !originalData)
		return INVALID_INPUTDATA_LENGTH;
	
	if ((decDataSize < 1) || !decData)
		return INVALID_OUTPUTDATA_SIZE;
	
	if (mCipherType != MD5_CIPHER)
		retLength = mCipherPtr->Decrypt(originalData,originalDataLength ,decData, decDataSize);
	return retLength;
}
