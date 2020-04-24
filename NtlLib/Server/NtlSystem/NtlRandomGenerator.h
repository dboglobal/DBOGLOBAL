//***********************************************************************************
//
//	File		:	NtlRandomGenerator.h
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

#pragma once


//-----------------------------------------------------------------------------------
//		Random Generator Class
//-----------------------------------------------------------------------------------
//	A simple pseudorandom number generator.  It's not that great, but at least it
//	generates 32 bit random numbers (most rand()'s generate 15 bits).
//	If no seed is given in the constructor, the state is set to a fixed value.
//	This can be useful for reproducing a stream of random numbers.
//	Adapted from the book "Inner Loops" by Rick Booth (Addison-Wesley: 1997).
//-----------------------------------------------------------------------------------


class CNtlRandomGenerator
{
public:

	CNtlRandomGenerator(void);

	CNtlRandomGenerator(unsigned int seed);

	CNtlRandomGenerator& operator=(CNtlRandomGenerator & rRandGenerator);

	virtual ~CNtlRandomGenerator(void);


public:

	unsigned int				Generate();

	void						Reset();


protected:

	enum
	{
		TABLE_SIZE = 250,
	};


	void						Init(unsigned int seed);


	void						GenerateSeeds();


	unsigned int				GenerateSimple();



private:


	unsigned int				m_nState;

	int							m_nIndex1;

	int							m_nIndex2;

	unsigned int				m_seed;

	unsigned int				m_table[TABLE_SIZE];
};

