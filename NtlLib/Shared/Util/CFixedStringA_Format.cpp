//*****************************************************************************
// File       : CFixedStringA_Format.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CFixedStringW.h"

#include "CFixedStringA.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( str )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: CFixedStringA
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------
/*
CFixedStringA::CFixedStringA( char * pchFormat, void * pParam1, ... )
: m_pchBuffer( NULL ), m_nLength( 0 ), m_nKey( 0 )
{
	if( pchFormat == NULL )
	{
		return;
	}

	_TFixedStringSegmentListA	tList;

	// Parse Arguments

	va_list		tArgumentList;
	va_start( tArgumentList, pchFormat );
	size_t		nTotalLength = ParseFormat( pchFormat, &tList, tArgumentList ); 
	va_end( tArgumentList );

	// Write Arguments

	char *		pResultBuffer = new char[nTotalLength + 1];

	if( WriteFormat( pResultBuffer, &tList ) != nTotalLength )
	{
		Assert( !"Invalid length" );

		delete[] pResultBuffer;

		return;
	}

	Clear();

	// Create Buffer

	CreateBuffer( pResultBuffer, nTotalLength );

	delete[] pResultBuffer;

	GenerateKey();

	return;
}
*/


//-----------------------------------------------------------------------------
// Name			: Format
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

size_t CFixedStringA::Format( char * pchFormat, ... )
{
	if( pchFormat == NULL )
	{
		return 0L;
	}

//	_SFixedStringSegmentListA	sSegmentList;

	// Parse Arguments

	va_list		tArgumentList;
	va_start( tArgumentList, pchFormat );

	Int32	nTotalLength = _vscprintf( pchFormat, tArgumentList );

	// Write Arguments

	char *	pchResultBuffer = new char[nTotalLength + 1];
	Assert( pchResultBuffer != NULL );

	if( vsprintf_s( pchResultBuffer, nTotalLength + 1, pchFormat, tArgumentList ) != nTotalLength )
	{
		Assert( !"Invalid length" );

		va_end( tArgumentList );

		delete[] pchResultBuffer;

		return 0L;
	}

	va_end( tArgumentList );

	Clear();

	// Create Buffer

	CreateBuffer( pchResultBuffer, nTotalLength );

	delete[] pchResultBuffer;

	GenerateKey();

	return nTotalLength;
}


//-----------------------------------------------------------------------------
// Name			: AppendFormat
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

size_t CFixedStringA::AppendFormat( char * pchFormat, ... )
{
	if( pchFormat == NULL )
	{
		return 0L;
	}

//	_SFixedStringSegmentListA	sList;

	// Parse Arguments

	va_list		tArgumentList;
	va_start( tArgumentList, pchFormat );

	Int32	nTotalLength = _vscprintf( pchFormat, tArgumentList );

	// Write Arguments

	char *	pchResultBuffer = new char[nTotalLength + 1];
	Assert( pchResultBuffer != NULL );

	if( vsprintf_s( pchResultBuffer, nTotalLength + 1, pchFormat, tArgumentList ) != nTotalLength )
	{
		Assert( !"Invalid length" );

		va_end( tArgumentList );

		delete[] pchResultBuffer;

		return 0L;
	}

	va_end( tArgumentList );

	// Expand Buffer

	ExpandBuffer( pchResultBuffer, nTotalLength );

	delete[] pchResultBuffer;

	GenerateKey();

	return nTotalLength;
}


//*****************************************************************************
//
// End of File : CFixedStringA_Format.cpp
//
//*****************************************************************************