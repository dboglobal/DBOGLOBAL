//*****************************************************************************
// File       : CFixedStringW_Format.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CFixedStringA.h"

#include "CFixedStringW.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( str )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: CFixedStringW
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------
/*
CFixedStringW::CFixedStringW( wchar_t * pchFormat, ... )
: m_pchBuffer( NULL ), m_nLength( 0 ), m_nKey( 0 )
{
	if( pchFormat == NULL )
	{
		return;
	}

	_TFixedStringSegmentListW	tList;

	// Parse Arguments

	va_list		tArgumentList;
	va_start( tArgumentList, pchFormat );
	size_t		nTotalLength = ParseFormat( pchFormat, &tList, tArgumentList ); 
	va_end( tArgumentList );

	// Write Arguments

	wchar_t *		pResultBuffer = new wchar_t[nTotalLength + 1];

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

size_t CFixedStringW::Format( wchar_t * pchFormat, ... )
{
	if( pchFormat == NULL )
	{
		return 0;
	}

//	_TFixedStringSegmentListW	tList;

	// Parse Arguments

	va_list		tArgumentList;
	va_start( tArgumentList, pchFormat );

	Int32	nTotalLength = _vscwprintf( pchFormat, tArgumentList );

	// Write Arguments

	wchar_t *	pchResultBuffer = new wchar_t[nTotalLength + 1];
	Assert( pchResultBuffer != NULL );

	if( vswprintf_s( pchResultBuffer, nTotalLength + 1, pchFormat, tArgumentList ) != nTotalLength )
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
// Name			: Format
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

size_t CFixedStringW::AppendFormat( wchar_t * pchFormat, ... )
{
	if( pchFormat == NULL )
	{
		return 0;
	}

//	_TFixedStringSegmentListW	tList;

	// Parse Arguments

	va_list		tArgumentList;
	va_start( tArgumentList, pchFormat );

	Int32	nTotalLength = _vscwprintf( pchFormat, tArgumentList );

	// Write Arguments

	wchar_t *	pchResultBuffer = new wchar_t[nTotalLength + 1];
	Assert( pchResultBuffer != NULL );

	if( vswprintf_s( pchResultBuffer, nTotalLength + 1, pchFormat, tArgumentList ) != nTotalLength )
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
// End of File : CFixedStringW_Format.cpp
//
//*****************************************************************************