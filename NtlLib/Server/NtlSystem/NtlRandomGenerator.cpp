//***********************************************************************************
//
//	File		:	NtlRandomGenerator.cpp
//
//	Begin		:	2007-02-01
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#include "stdafx.h"
#include "NtlRandomGenerator.h"


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlRandomGenerator::CNtlRandomGenerator(void)
{
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlRandomGenerator::CNtlRandomGenerator(unsigned int seed)
{
	Init( seed );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlRandomGenerator& CNtlRandomGenerator::operator=(CNtlRandomGenerator & rRandGenerator)
{
	m_seed = rRandGenerator.m_seed;
	m_nState = rRandGenerator.m_nState;

	m_nIndex1 = rRandGenerator.m_nIndex1;
	m_nIndex2 = rRandGenerator.m_nIndex2;

	memcpy( m_table, rRandGenerator.m_table, sizeof(unsigned int) * TABLE_SIZE );

	return *this;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlRandomGenerator::~CNtlRandomGenerator(void)
{
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlRandomGenerator::Init(unsigned int seed)
{
	m_seed = seed;
	m_nState = seed;
	GenerateSeeds();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlRandomGenerator::Reset()
{
	m_nState = m_seed;
	GenerateSeeds();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
unsigned int CNtlRandomGenerator::Generate()
{
	unsigned int retval = (m_table[m_nIndex1] ^= m_table[m_nIndex2]); 

	m_nIndex1++;
	if (m_nIndex1 == TABLE_SIZE)
		m_nIndex1 = 0;

	m_nIndex2++;
	if (m_nIndex2 == TABLE_SIZE)
		m_nIndex2 = 0;


	return retval;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlRandomGenerator::GenerateSeeds()
{
	int n;
	unsigned int msk, bit;

	m_nIndex1 = 0;
	m_nIndex2 = 103;

	for ( n = TABLE_SIZE - 1; n >= 0; n-- )
		m_table[n] = GenerateSimple();

	for ( n = 3, msk = 0xffffffff, bit = 0x80000000; bit; n += 7 )
	{
		m_table[n] = (m_table[n] & msk) | bit;
		msk >>= 1;
		bit >>= 1;
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
unsigned int CNtlRandomGenerator::GenerateSimple()
{
	/* IL_Shift32Random from Inner Loops book */
	unsigned int n, bit, temp;
	temp = m_nState;

	for (n=0; n < 32; n++)
	{
		bit = ((temp >> 0) ^ (temp >> 1) ^ (temp >> 2) ^ (temp >> 3) ^ (temp >> 5) ^ (temp >> 7)) & 1;
		temp = (((temp >> 1) | (temp << 31)) & ~1) | bit;
	}

	m_nState = temp;

	return m_nState;
}
