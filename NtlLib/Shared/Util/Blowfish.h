#pragma once
#include "Cipher.h"

#define MAXKEYBYTES 56      /* 448 bits */
#define bf_N             16
#define noErr            0
#define DATAERROR         -1
#define KEYBYTES         8
//#define subkeyfilename   "Blowfish.dat"

#define UWORD_32bits  unsigned long
#define UWORD_16bits  unsigned short
#define UBYTE_08bits  unsigned char

/* choose a byte order for your hardware. ABCD - big endian - motorola */
#ifdef ORDER_ABCD
union aword {
  UWORD_32bits word;
  UBYTE_08bits byte [4];
  struct {
    unsigned int byte0:8;
    unsigned int byte1:8;
    unsigned int byte2:8;
    unsigned int byte3:8;
  } w;
};
#endif  /* ORDER_ABCD */

/* DCBA - little endian - intel */
#ifdef ORDER_DCBA
union aword {
  UWORD_32bits word;
  UBYTE_08bits byte [4];
  struct {
    unsigned int byte3:8;
    unsigned int byte2:8;
    unsigned int byte1:8;
    unsigned int byte0:8;
  } w;
};
#endif  /* ORDER_DCBA */

/* BADC - vax */
#ifdef ORDER_BADC
union aword {
  UWORD_32bits word;
  UBYTE_08bits byte [4];
  struct {
    unsigned int byte1:8;
    unsigned int byte0:8;
    unsigned int byte3:8;
    unsigned int byte2:8;
  } w;
};
#endif  /* ORDER_BADC */

#ifdef __cplusplus	// for avoiding c++ identifier decoration
extern "C"
{
#endif

typedef struct {
		UWORD_32bits S[4][256], P[bf_N+2];
} Blowfish_matrix;

void Blowfish_encipher(unsigned long *xl, unsigned long *xr);
void Blowfish_decipher(unsigned long *xl, unsigned long *xr);
// Length must be multiple of 8
void BlowfishEncrypt(unsigned char *source, int length);
void BlowfishDecrypt(unsigned char *dest, int length);
short InitializeBlowfish(unsigned char key[], short keybytes);

short InitializeBlowfishLocal( Blowfish_matrix *mtx, unsigned char key[], short keybytes );
void Blowfish_encipher_local(Blowfish_matrix *mtx, UWORD_32bits *xl, UWORD_32bits *xr);
void Blowfish_decipher_local(Blowfish_matrix *mtx, UWORD_32bits *xl, UWORD_32bits *xr);
int BlowfishDecryptLocal(Blowfish_matrix *mtx, unsigned char *buf, int length);
int BlowfishEncryptLocal(Blowfish_matrix *mtx, unsigned char *buf, int length);

class CBlowfish:
	public CCipher
{
public:
	CBlowfish(void);
	~CBlowfish(void);

	virtual int SetKey(unsigned char *cipherKey,int cipherKeyLength);
	virtual int Encrypt(char *originalData, int originalDataLength, char *encData, int encDataSize);
	virtual int Decrypt(char *originalData, int originalDataLength, char *decData, int decDataSize);
	virtual void ReleaseCipher(void){return;};

private :
	Blowfish_matrix		mBlowfishmatrix;
};

#ifdef __cplusplus	// for avoiding c++ identifier decoration
}
#endif


