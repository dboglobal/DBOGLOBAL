#pragma once
#include "Cipher.h"

#pragma warning( disable : 4244 )

/*
 * Twofish for CryptoAPI
 *
 * Originaly Twofish for GPG
 * By Matthew Skala <mskala@ansuz.sooke.bc.ca>, July 26, 1998
 * 256-bit key length added March 20, 1999
 * Some modifications to reduce the text size by Werner Koch, April, 1998
 * Ported to the kerneli patch by Marc Mutz <Marc@Mutz.com>
 * Ported to CryptoAPI by Colin Slater <hoho@tacomeat.net>
 *
 * The original author has disclaimed all copyright interest in this
 * code and thus put it in the public domain. The subsequent authors 
 * have put this under the GNU General Public License.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
 * USA
 *
 * This code is a "clean room" implementation, written from the paper
 * _Twofish: A 128-Bit Block Cipher_ by Bruce Schneier, John Kelsey,
 * Doug Whiting, David Wagner, Chris Hall, and Niels Ferguson, available
 * through http://www.counterpane.com/twofish.html
 *
 * For background information on multiplication in finite fields, used for
 * the matrix operations in the key schedule, see the book _Contemporary
 * Abstract Algebra_ by Joseph A. Gallian, especially chapter 22 in the
 * Third Edition.
 */

/* The large precomputed tables for the Twofish cipher (twofish.c)
 * Taken from the same source as twofish.c
 * Marc Mutz <Marc@Mutz.com>
*/

/* Structure for an expanded Twofish key.  s contains the key-dependent
 * S-boxes composed with the MDS matrix; w contains the eight "whitening"
 * subkeys, K[0] through K[7].	k holds the remaining, "round" subkeys.  Note
 * that k[i] corresponds to what the Twofish paper calls K[i+8]. */
typedef struct {
   uint32  sBox[4][256];			/* S -BOX */	 
   uint32  white[8];				/*whitening*/
   uint32  rs[32];						/*subkeys , round subkeys*/
}twofish_context;

class CTwofishCipher
	:public CCipher
{
public:
	CTwofishCipher(void);
	~CTwofishCipher(void);
	
	virtual int SetKey(unsigned char* cipherKey, int cipherKeyLength);
	virtual int Encrypt( char *originalData,  int originalDataLength, char* encData, int encDataSize);
	virtual int Decrypt( char *originalData,  int originalDataLength, char* decData, int decDataSize);
	virtual void ReleaseCipher(void){return;};

private:
	twofish_context		mTwofishContext;

	void EncryptByBlock(unsigned char *originalData, unsigned char* outputData);
	void DecryptByBlock(unsigned char *originalData, unsigned char* outputData);
};
