#pragma once
#include "Cipher.h"

class CDes :public CCipher
{
public:
	CDes(void);
	~CDes(void);
	void DesKeyInit(char *keyValue);
	void DesWriteBlock(void *buf, int len);
	void DesReadBlock(void *buf, int len);
	
	virtual int SetKey(unsigned char *cipherKey,int cipherKeyLength);
	virtual int Encrypt( char *originalData, int originalDataLength, char *encData, int encDataSize);
	virtual int Decrypt( char *originalData, int originalDataLength, char *decData, int decDataSize);
	virtual void ReleaseCipher(void){return;};

private:
	char mpc1m[56];                           /* place to modify pc1 into     */
	char mpcr[56];                            /* place to rotate pc1 into     */
	char miperm[16][16][8], mfperm[16][16][8];/* inital and final permutations*/
	char ms[4][4096];                 /* S1 thru S8 precomputed       */
	char mp32[4][256][4];                     /* for permuting 32-bit f output*/
	char mkn[16][6];                          /* key selections  */	


	void permute( char *inblock, char perm[16][16][8], char *outblock );
	void perm32(char *inblock, char *outblock);
	void expand(char *right, char *bigright);
	void contract(char *in48, char *out32);
	void f(char *right, int num, char *fret);
	void iter(int num, char *inblock, char *outblock);
	int getcomp(int k, int v);
	void sinit(void);
	void kinit(char *key64bit);
	void p32init(void);
	void perminit(char perm[16][16][8], char p[64]);
	void endes(char *inblock, char *outblock);
	void dedes(char *inblock, char *outblock);
};

