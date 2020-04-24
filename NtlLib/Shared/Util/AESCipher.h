#pragma once
#include "Cipher.h"

/*
 *  FIPS-197 compliant AES implementation
 *
 *  Copyright (C) 2001-2004  Christophe Devine
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


typedef struct {
	uint32		erk[64];	/* encryption round keys */
	uint32		drk[64];    /* decryption round keys */
	int			nr;			 /* number of rounds */
}aes_context;


class CAESCipher
	:public CCipher
{
public:
	CAESCipher(void);
	~CAESCipher(void);
	
	virtual	int SetKey(unsigned char* cipherKey, int cipherKeyLength);
	virtual int Encrypt( char *originalData,  int originalDataLength, char* encData, int encDataSize);
	virtual int Decrypt( char *originalData,  int originalDataLength, char* decData, int decDataSize);
	virtual void ReleaseCipher(void){return;};

private:
	aes_context		mAESContext;

	void EncryptByBlock(unsigned int *originalData, unsigned int* outputData);
	void DecryptByBlock(unsigned int *originalData, unsigned int* outputData);
};


