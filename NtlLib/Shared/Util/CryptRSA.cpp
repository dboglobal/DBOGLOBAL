#include "StdAfx.h"
#include "cryptrsa.h"


///////////////////////////// by using crypt Platform SDK: Security
CCryptGenRSA::CCryptGenRSA(void)
{
	mhProv = 0;
	mhRSAKey = 0;
}

CCryptGenRSA::~CCryptGenRSA(void)
{
	FreeHandle();
}

// Acquire a cryptographic provider context handle.
bool CCryptGenRSA::SetCryptAPIHandle(void)
{	
	DWORD errNum = 0;
	if (!CryptAcquireContext(&mhProv,NULL,MS_ENHANCED_PROV,PROV_RSA_FULL,0)) {
		errNum = GetLastError();
		if (errNum == NTE_BAD_KEYSET) {
			if (!CryptAcquireContext(&mhProv,NULL,MS_ENHANCED_PROV,PROV_RSA_FULL,CRYPT_NEWKEYSET)) 
				errNum = GetLastError();
			else
				errNum = 0;
		}
	}

	if (errNum) {
		wchar_t errMessage[1024];
		PrintError(errNum,1024,errMessage);
		return false;
	} 
	return true;
}

// display formats a message string
void CCryptGenRSA::PrintError(DWORD errNum, int errMessageSize, wchar_t *errMessage)
{
	if ((errNum > 0) && (errMessage) ) {
		int len =0;

		len = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS , NULL, errNum,
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),errMessage,errMessageSize,0);
		errMessage[len] = '\0';
	}
}

// Generate a session key.
int CCryptGenRSA::GetPublicKey(int publicKeySize,unsigned char *publicKeys)
{	
	DWORD keyLen = 0;
	DWORD errNum = 0;
	
	if (!mhProv) 
		return 0;
	
	if (!mhRSAKey) {
		if(!CryptGenKey(mhProv,CALG_RSA_KEYX,RSA1024BIT_KEY|AT_KEYEXCHANGE,&mhRSAKey))
			errNum =GetLastError();
	}
	
	if (mhRSAKey) {
		keyLen = publicKeySize;
		if(!CryptExportKey(mhRSAKey,0,PUBLICKEYBLOB,0,publicKeys,&keyLen)) {
			errNum = GetLastError();
			keyLen = 0;
		} else {
			publicKeys[keyLen] ='\0';
		}
	}
	
	if (errNum > 0) {
		wchar_t errMessage[1024];
		PrintError(errNum,1024,errMessage);
	}

	return keyLen;
}

void CCryptGenRSA::FreeHandle(void)
{	
	//destroy session key
	if (mhRSAKey)
		CryptDestroyKey(mhRSAKey);
	// Release the provider handle.
	if (mhProv)
		CryptReleaseContext(mhProv,0);
}

int CCryptGenRSA::GetPrivateKey(int privateKeySize, unsigned char *privateKeys)
{
	DWORD keyLen = 0;
	DWORD errNum = 0;
	
	if (!mhProv) 
		return keyLen;

	if (!mhRSAKey) {
		if(!CryptGenKey(mhProv,CALG_RSA_KEYX,RSA1024BIT_KEY|AT_KEYEXCHANGE,&mhRSAKey))
			errNum =GetLastError();
	}
	
	if (mhRSAKey) {
		keyLen = privateKeySize;
		if(!CryptExportKey(mhRSAKey,0,PRIVATEKEYBLOB,0,privateKeys,&keyLen)) {
			errNum = GetLastError();
			keyLen = 0;
		} else {
			privateKeys[keyLen] ='\0';
		}
	}
	
	if (errNum > 0) {
		wchar_t errMessage[1024];
		PrintError(errNum,1024,errMessage);
	}

	return keyLen;
}


// importing a key from a key BLOB
bool CCryptGenRSA::SetPublicKey(int keyLength, unsigned char *publicKeys)
{
	if (!mhProv)
		return false;

	if(!CryptImportKey(mhProv,publicKeys,keyLength,0,0,&mhRSAKey)) {
		wchar_t errMessage[1024];
		PrintError(GetLastError(),1024,errMessage);
		return false;
	} 

	return true;
}

int CCryptGenRSA::Encrypt(unsigned char *originalData, int dataLength, unsigned char *outputData)
{	
	DWORD resultLen  = 0;

	if ((!mhProv) || (!mhRSAKey))
		return resultLen;
	
	unsigned char encBuf[1024];
	resultLen = dataLength;
	memset(encBuf,0,sizeof(encBuf));
	memcpy(encBuf,originalData,resultLen);

	if(!CryptEncrypt(mhRSAKey,0,TRUE,0,encBuf,&resultLen,1024)) {
		resultLen = 0;
		wchar_t errMessage[1024];
		PrintError(GetLastError(),1024,errMessage);
	}else{
		memcpy(outputData,encBuf,resultLen);
	}
	
	return resultLen ;
}


int CCryptGenRSA::Decrypt(unsigned char *originalData, int dataLength, unsigned char *outputData)
{
	DWORD resultLen  = 0;

	if ((!mhProv) || (!mhRSAKey))
		return resultLen;
	
	unsigned char decBuf[1024];
	resultLen = dataLength;

	memset(decBuf,0,sizeof(decBuf));
	memcpy(decBuf,originalData,resultLen);

	if(!CryptDecrypt(mhRSAKey,0,TRUE,0,decBuf,&resultLen)) {
		resultLen = 0;
		wchar_t errMessage[1024];
		PrintError(GetLastError(),1024,errMessage);
	} else {
		memcpy(outputData,decBuf,resultLen);
	}
	
	return resultLen ;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// RSA algorithm (with windows cryptographic)
CCryptRSA::CCryptRSA(void)
{
	mhProv = 0;
	mhRSAKey = 0;
}

CCryptRSA::~CCryptRSA(void)
{
	ReleaseCipher();
}

void CCryptRSA::ReleaseCipher(void)
{
	if (mhRSAKey) {
		CryptDestroyKey(mhRSAKey);
		mhRSAKey = 0;
	}
	if (mhProv) {
		CryptReleaseContext(mhProv,0);
		mhProv = 0;
	}
}

// display formats a message string
void CCryptRSA::PrintError(DWORD errNum, int errMessageSize, wchar_t *errMessage)
{
	if ((errNum > 0) && (errMessage) ) {
		int len =0;
		len = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS , NULL, errNum,
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),errMessage,errMessageSize,0);
		errMessage[len] = '\0';
	}
}

int CCryptRSA::SetKey(unsigned char *cipherKey, int cipherKeyLength)
{	
	int ret = MS_ACQCONTEXT_ERROR;
	
	if (!cipherKey)
		return NULL_CIPHER_KEY;

	if (cipherKeyLength > RSA1024BIT_KEY_SIZE +1)
		return INVALID_KEY_LENGTH;
	
	DWORD errNum = 0;

	if (!CryptAcquireContext(&mhProv,NULL,MS_ENHANCED_PROV,PROV_RSA_FULL,0 )) {
		errNum = GetLastError();
		if (errNum == NTE_BAD_KEYSET) {
			if(!CryptAcquireContext(&mhProv,NULL,MS_ENHANCED_PROV,PROV_RSA_FULL,CRYPT_NEWKEYSET )) 
				errNum = GetLastError();
			else 
				errNum = 0;
		}

	} 
	
	if(!errNum) {
		if(!CryptImportKey(mhProv,cipherKey,cipherKeyLength,0,0,&mhRSAKey)) 
			errNum =GetLastError();
		else 
			ret = CIPHER_SUCCESS;
	}
	
	if (errNum)  {
		wchar_t errMsg[BASE_BUFFER_SIZE];
		PrintError(errNum,BASE_BUFFER_SIZE,errMsg);
	}

	return ret;
}

// this cipher algorithm  is 128 byte block cipher
// CryptEncrypt with an RSA key is the length of the key modulus minus eleven bytes. 
//The eleven bytes is the chosen minimum for PKCS #1 padding. The ciphertext is returned in little-endian format.
int CCryptRSA::Encrypt( char *originalData, int originalDataLength, char *encData, int encDataSize)
{	
	int encDataLen = CIPHER_ERROR;

	if  ((!mhProv) || (!mhRSAKey) )
		return RSA_SYSTEM_ERROR;

	if (!originalData) 
		return NULL_INPUTDATA;

	if (!encData)
		return NULL_OUTPUTDATA;
	
	if ((encDataSize > (BUFFER_SIZE-1))|| (encDataSize <originalDataLength))
		return INVALID_OUTPUTDATA_SIZE;

	if (originalDataLength > (BUFFER_SIZE-1)) 
		return INVALID_INPUTDATA_LENGTH;

	int encryptLen = 0, encLoop =0, i=0;
	unsigned char buf[BASE_BUFFER_SIZE];

	if (originalDataLength > RSA1024BIT_ENCBLOCK_SIZE ) 
		encLoop = originalDataLength / RSA1024BIT_ENCBLOCK_SIZE;

	if (originalDataLength % RSA1024BIT_ENCBLOCK_SIZE )
		encLoop++;

	wchar_t errMsg[BASE_BUFFER_SIZE];

	for (i=0; i< encLoop; i++) {
		memset(buf,0,sizeof(buf));

		if(i == (encLoop -1)) {			//last block
			encDataLen = originalDataLength % RSA1024BIT_ENCBLOCK_SIZE;
			memcpy(buf,originalData+i*RSA1024BIT_ENCBLOCK_SIZE, encDataLen);

			if(!CryptEncrypt(mhRSAKey,0,TRUE,0,buf,(DWORD*)&encDataLen,encDataSize)) {
				PrintError(GetLastError(),BASE_BUFFER_SIZE,errMsg);
				encDataLen = CIPHER_ERROR;
				break;
			}
		} else {
			encDataLen = RSA1024BIT_ENCBLOCK_SIZE;
			memcpy(buf,originalData+i*RSA1024BIT_ENCBLOCK_SIZE, RSA1024BIT_ENCBLOCK_SIZE);

			if(!CryptEncrypt(mhRSAKey,0,FALSE,0,buf,(DWORD*)&encDataLen,encDataSize)) {
				PrintError(GetLastError(),BASE_BUFFER_SIZE,errMsg);
				encDataLen = CIPHER_ERROR;
				break;
			}
		} 

		if(encDataLen) {
			memcpy(encData+encryptLen,buf,encDataLen);
			encryptLen += encDataLen;
		}
	}
	
	return encryptLen;
}

// decryption function
//128 bytes block
int CCryptRSA::Decrypt( char *originalData, int originalDataLength, char *decData, int decDataSize)
{	
	int decDataLen = CIPHER_ERROR;

	if  ((!mhProv) || (!mhRSAKey))
		return RSA_SYSTEM_ERROR;

	if (!originalData) 
		return NULL_INPUTDATA;

	if  (!decData)
		return NULL_OUTPUTDATA;
	
	if ((decDataSize > (BUFFER_SIZE-1))  || (decDataSize <originalDataLength))
		return INVALID_OUTPUTDATA_SIZE;
	
	if (originalDataLength > (BUFFER_SIZE-1))
		return INVALID_INPUTDATA_LENGTH;

	int i =0, decLoop = 0,decryptLen = 0;
	unsigned char buf[BASE_BUFFER_SIZE];
	wchar_t errMsg[BASE_BUFFER_SIZE];

	decLoop = originalDataLength / RSA1024BIT_DECBLOCK_SIZE;

	for (i=0; i<decLoop ; i++) {
		memset(buf,0,sizeof(buf));
		decDataLen = RSA1024BIT_DECBLOCK_SIZE;
		memcpy(buf,originalData+i*RSA1024BIT_DECBLOCK_SIZE, RSA1024BIT_DECBLOCK_SIZE);

		if(i == (decLoop -1)) {			//last block
			if(!CryptDecrypt(mhRSAKey,0,TRUE,0,buf,(DWORD*)&decDataLen)) {
				PrintError(GetLastError(),BASE_BUFFER_SIZE,errMsg);
				decDataLen = CIPHER_ERROR;
				break;
			}
		} else {
			if(!CryptDecrypt(mhRSAKey,0,FALSE,0,buf,(DWORD*)&decDataLen)) {
				PrintError(GetLastError(),BASE_BUFFER_SIZE,errMsg);
				decDataLen = CIPHER_ERROR;
				break;
			}
		}

		if (decDataLen ) {
			memcpy(decData+decryptLen,buf,decDataLen);
			decryptLen += decDataLen;
		}
	
	}

	return decryptLen;
}


//////////////////////////////////////////////////////////////////////
// 3Des algorithm (with windows cryptographic)
CCryptTripleDes::CCryptTripleDes(void)
{
	mhProv =0;
	mhTripleDesKey = 0;
}
CCryptTripleDes::~CCryptTripleDes(void)
{
	ReleaseCipher();
}

void CCryptTripleDes::ReleaseCipher(void)
{
	if (mhTripleDesKey){
		CryptDestroyKey(mhTripleDesKey);
		mhTripleDesKey = 0;
	}

	if (mhProv) {
		CryptReleaseContext(mhProv,0);
		mhProv = 0;
	}
}
// Setting cipher Key 
// [IN]		cipherKey		: 3des cipher key
//			cipherKeyLength	: cipherKey Length
int CCryptTripleDes::SetKey(unsigned char *cipherKey, int cipherKeyLength)
{
	int ret = MS_ACQCONTEXT_ERROR;
	
	if (!cipherKey)
		return NULL_CIPHER_KEY;

	if (cipherKeyLength >  TRIPLEDES_MAX_KEY_SIZE)
		return INVALID_KEY_LENGTH;
	
	DWORD errNum = 0;		
	HCRYPTHASH	hHash;

	if(!CryptAcquireContext(&mhProv,NULL,MS_ENHANCED_PROV, PROV_RSA_FULL,0)) {
		errNum = GetLastError();
		if (errNum == NTE_BAD_KEYSET) {
			if(!CryptAcquireContext(&mhProv,NULL,MS_ENHANCED_PROV,PROV_RSA_FULL,CRYPT_NEWKEYSET )) 
				errNum = GetLastError();
			else
				errNum = 0;
		}
	} 
		
	if (!errNum ) {
		if(CryptCreateHash(mhProv,CALG_MD5,0, 0,&hHash)) {
			//Hash the password string.
			if(CryptHashData(hHash,cipherKey,cipherKeyLength,0)) {
				if(CryptDeriveKey(mhProv,CALG_3DES,hHash,0,&mhTripleDesKey)) {
					if(CryptSetKeyParam(mhTripleDesKey,KP_IV,cipherKey,0))
						ret = CIPHER_SUCCESS;
					else
						errNum = GetLastError();
				}else {
					errNum = GetLastError();
				}
			} else {
					errNum = GetLastError();
			}

			CryptDestroyHash(hHash);
		} else {
			errNum =GetLastError();
		}
	}

	if (errNum) {
		wchar_t errMsg[BASE_BUFFER_SIZE];
		PrintError(errNum,BASE_BUFFER_SIZE,errMsg);
	} 

	return ret;
}

// display formats a message string
// [IN]		errNum			: error number
//			errMessageSize	: error message buffer size
// [OUT]	errMessage		: error message buffer
void CCryptTripleDes::PrintError(DWORD errNum, int errMessageSize, wchar_t *errMessage)
{
	if ((errNum > 0) && (errMessage) ) {
		int len =0;

		len = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS , NULL, errNum,
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),errMessage,errMessageSize,0);
		errMessage[len] = '\0';
	}
}

// this cipher algorithm  is 64 bits(8bytes) block encryption routine 
int CCryptTripleDes::Encrypt(unsigned char *originalData, int originalDataLength, unsigned char *encData, int encDataSize)
{	
	int encDataLen = CIPHER_ERROR;
	DWORD encBlockDataLen = 0;

	if  ((!mhProv) || (!mhTripleDesKey))
		return encDataLen;
		
	if  (!originalData) 
		return NULL_INPUTDATA;

	if (!encData)
		return NULL_OUTPUTDATA;
	
	encBlockDataLen = originalDataLength / DES_BLOCK_SIZE + DES_BLOCK_SIZE;
	if ((encDataSize > (BUFFER_SIZE-1))  || (encDataSize < (int)encBlockDataLen ))
		return INVALID_OUTPUTDATA_SIZE;
	
	if (originalDataLength > (BUFFER_SIZE-1))
		return INVALID_INPUTDATA_LENGTH;

	unsigned char buf[BUFFER_SIZE];
	
	memset(buf,0,sizeof(buf));
	memcpy(buf,originalData, originalDataLength);
	
	encDataLen = originalDataLength;

	if(!CryptEncrypt(mhTripleDesKey,0,TRUE,0,buf,(DWORD*)&encDataLen,encDataSize)){
		wchar_t errMsg[BASE_BUFFER_SIZE];
		PrintError(GetLastError(),BASE_BUFFER_SIZE,errMsg);
		encDataLen = CIPHER_ERROR;
	} 

	if(encDataLen)
		memcpy(encData,buf,encDataLen);
	
	return encDataLen;
}

int CCryptTripleDes::Decrypt(unsigned char *originalData, int originalDataLength, unsigned char *decData, int decDataSize)
{	
	int decDataLen = CIPHER_ERROR;

	if  ((!mhProv) || (!mhTripleDesKey))
		return decDataLen;
		
	if (!originalData) 
		return NULL_INPUTDATA;

	if  (!decData)
		return NULL_OUTPUTDATA;
	
	if ((decDataSize > (BUFFER_SIZE-1))  || (decDataSize < originalDataLength))
		return INVALID_OUTPUTDATA_SIZE;
	if (originalDataLength > (BUFFER_SIZE-1))
		return INVALID_INPUTDATA_LENGTH;
	
	unsigned char buf[BUFFER_SIZE];

	
	memset(buf,0,sizeof(buf));
	memcpy(buf,originalData, originalDataLength);	
	decDataLen = originalDataLength;
	if(!CryptDecrypt(mhTripleDesKey,0,TRUE,0,buf,(DWORD*)&decDataLen)){
		wchar_t errMsg[BASE_BUFFER_SIZE];
		PrintError(GetLastError(),BASE_BUFFER_SIZE,errMsg);
		decDataLen = CIPHER_ERROR;
	} 

	if(decDataLen)
		memcpy(decData,buf,decDataLen);

	return decDataLen;
}
