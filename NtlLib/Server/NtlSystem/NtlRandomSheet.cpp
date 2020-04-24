#include "stdafx.h"
#include "NtlRandomSheet.h"
#include "stdio.h"

#define MAKEQWORD(l, h)  ((uint64_t) (((DWORD) (l)) | ((uint64_t) ((DWORD) (h))) << 32))

/***************************************************************/
CNtlRandomSheet::CNtlRandomSheet()
{
	state = 0x91AB248980AC1937ui64;
	GenerateSeeds();
}
/***************************************************************/
CNtlRandomSheet::CNtlRandomSheet(uint64_t seed)
{
	state = seed;
	GenerateSeeds();
}
/***************************************************************/
CNtlRandomSheet::~CNtlRandomSheet()
{
}
/***************************************************************/
uint64_t CNtlRandomSheet::GetSheet(int sheetid)
{
	return table[sheetid];
}
/***************************************************************/
void CNtlRandomSheet::GenerateSeeds()
{
	/*uint64_t n = 0, j = 0, nSheetNo = 0;
	uint64_t bit, msk;

	for (j = 0; j < SHEET_SIZE; j++)
	{
		for (n = 0; n < TABLE_SIZE; n++)
		{
			nSheetNo = (j * TABLE_SIZE) + n;

			table[nSheetNo] = GenerateSimple();
		}

		n = 3;
		msk = -1i64;
		bit = 0x8000000000000000i64;
		while (bit && n < TABLE_SIZE)
		{
			nSheetNo = (j * TABLE_SIZE) + n;

			table[nSheetNo] = bit | msk & table[nSheetNo];

			msk >>= 1;
			bit >>= 1;
			n += 7;
		}
	}*/
}
/***************************************************************/
uint64_t CNtlRandomSheet::GenerateSimple()
{
	uint64_t temp;
	uint64_t value1, n;
	temp = state;

	for (n = 0; n < 64; n++)
	{
		value1 = ((temp >> 7) ^ ((temp >> 5) ^ (temp >> 3) ^ (temp >> 2) ^ (temp >> 1) ^ temp)) & 1;
		temp = value1 | ((temp << 63) | (temp >> 1)) & 0xFFFFFFFFFFFFFFFEui64;
	}

	state = temp;
	return state;

	/*
	//====== Reversed step by step ======
	uint32_t a1,a2,b1,b2,c1,c2,n;
	a1 = LODWORD(state);
	a2 = HIDWORD(state);

	for (n=0; n < 64; n++)
	{
	b1 = a1;
	b2 = a2;

	b1 = LODWORD(MAKEQWORD(b1,b2)>>2);
	b2 = b2 >> 2;
	b1 = b1 ^ a1;
	b2 = b2 ^ a2;

	b1 = LODWORD(MAKEQWORD(b1,b2)>>3);
	b2 = b2 >> 3;
	c1 = a1;
	c2 = a2;
	c1 = LODWORD(MAKEQWORD(c1,c2)>>1);
	c2 = c2 >> 1;
	b1 = b1 ^ c1 ^ a1;
	b2 = b2 ^ c2 ^ a2;

	b1 = LODWORD(MAKEQWORD(b1,b2)>>1);
	b2 = b2 >> 1;
	b1 = b1 ^ a1;
	b2 = b2 ^ a2;

	b1 = LODWORD(MAKEQWORD(b1,b2)>>1);
	a2 = b1;
	a2 = a2 ^ c1 ^ a1;
	a1 = a1 << 31;
	b2 = 0;
	a2 = a2 & 0x00000001;
	b2 = b2 | c1;
	a1 = a1 | c2;
	b1 = 0;
	a2 = a2 ^ b2;
	b1 = b1 ^ a1;
	a1 = a2;
	a2 = b1;
	}
	state = MAKEQWORD(a1,a2);
	return state;
	*/
}



CNtlRandomChecksum::CNtlRandomChecksum()
{
	state = 0x91AB248980B109E8;
	GenerateSeeds();
}
/***************************************************************/
CNtlRandomChecksum::CNtlRandomChecksum(uint64_t seed)
{
	state = seed;
	GenerateSeeds();
}
/***************************************************************/
CNtlRandomChecksum::~CNtlRandomChecksum()
{
}
/***************************************************************/
uint64_t CNtlRandomChecksum::GetValue(int position)
{
	return table[position];
}
/***************************************************************/
void CNtlRandomChecksum::GenerateSeeds()
{
	uint64_t n;
	uint64_t msk2, bit2;

	{
		for (n = 0; n < TABLE_SIZE; n++)
			table[n] = GenerateSimple();

		n = 3;
		msk2 = 0xFFFFFFFFFFFFFFFEui64;
		bit2 = 0x8000000000000000ui64;
		while (bit2 && n < TABLE_SIZE)
		{
			table[n] = bit2 | table[n] & msk2;
			msk2 >>= 1;
			bit2 >>= 1;
			n += 7;
		}
	}
}
/***************************************************************/
uint64_t CNtlRandomChecksum::GenerateSimple()
{
	uint64_t temp;
	uint64_t value1, n;
	temp = state;
	for (n = 0; n < 64; n++)
	{
		value1 = ((temp >> 7) ^ ((temp >> 5) ^ (temp >> 3) ^ (temp >> 2) ^ (temp >> 1) ^ temp)) & 1;
		temp = value1 | ((temp << 63) | (temp >> 1)) & 0xFFFFFFFFFFFFFFFEui64;
	}

	state = temp;
	return state;
}


