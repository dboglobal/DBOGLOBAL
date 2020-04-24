#pragma once
#include "Cipher.h"
/*
 *  FIPS-46-3 compliant 3DES implementation
 *
 *  Copyright (C) 2001-2003  Christophe Devine
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

typedef struct
{
    uint32 esk[96];     /* Triple-DES encryption subkeys */
    uint32 dsk[96];     /* Triple-DES decryption subkeys */
}des3_context;

class CTripleDes
	:public CCipher
{
public:
	CTripleDes(void);
	~CTripleDes(void);

	virtual int SetKey(unsigned char *cipherKey,int cipherKeyLength);
	virtual int Encrypt( char *originalData, int originalDataLength, char *encData, int encDataSize);
	virtual int Decrypt( char *originalData, int originalDataLength, char *decData, int decDataSize);
	virtual void ReleaseCipher(void){return;};

private:
	des3_context	m_ctx;
	void CryptByBlock(uint32 SK[96], char* originalData,  char *outputData);
};
