
#pragma once

// AES chiper algorithm이다. (FIPS-197)
// 128 bit block size  암호화 알고리즘
// key size 는 16(128 bit), 24(128 bit), 32(256 bit)가 될 수 있다.
// key size 에 따라 number of rounds는 10,12,14가 된다.

#ifndef		uint8
#define		uint8		unsigned char
#endif

#ifndef		uint32
#define		uint32		unsigned long int
#endif

#define		KEY_SIZE_16			16
#define		KEY_SIZE_24			24
#define		KEY_SIZE_32			32


#define		BASE_BUFFER_SIZE			2048
#define		BUFFER_SIZE					2048


#define		AES_BLOCK_SIZE		16
#define		DES_BLOCK_SIZE		8
#define		TRIPLEDES_MAX_KEY_SIZE			128

#define		RSA1024BIT_KEY_SIZE				148
#define		RSA1024BIT_ENCBLOCK_SIZE			117
#define		RSA1024BIT_DECBLOCK_SIZE			128

#define		MD5_SIZE			16
// define  cipher class 
enum CIPHER{
	DES_CIPHER =1,
	TWOFISH_CIPHER,
	AES_CIPHER,
	TRIPLEDES_CIPHER,
	BLOWFISH_CIPHER,
	MD5_CIPHER,
	RSA_CIPHER,
	AES_CIPHER_NEW,
};

enum {
	MS_ACQCONTEXT_ERROR = -11,
	NULL_CIPHER_KEY,
	CIPHER_OBJECT_ERROR,
	INVALID_CIPHER_TYPE,
	INVALID_KEY_LENGTH,
	INVALID_INPUTDATA_LENGTH,
	NULL_INPUTDATA,
	INVALID_OUTPUTDATA_SIZE,
	NULL_OUTPUTDATA,
	CONVERT_HEXA_ERROR,
	RSA_SYSTEM_ERROR,
	CIPHER_ERROR = 0,
	CIPHER_SUCCESS,

};