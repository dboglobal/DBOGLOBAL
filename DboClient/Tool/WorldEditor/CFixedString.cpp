//*****************************************************************************
// File       : CFixedString.cpp
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CFixedString.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

const TCHAR CFixedString::END_OF_STRING		= _T( '\0' );
const UINT CFixedString::SEARCH_FAIL		= FIXED_STRING_SEARCH_FAIL;


//-----------------------------------------------------------------------------
// Name			: CFixedString
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedString::CFixedString( void )
{
	m_ptBuffer		= NULL;
	m_nLength		= 0;
	m_nKey			= 0;

	Assign( NULL );

	return;
}


//-----------------------------------------------------------------------------
// Name			: CFixedString
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedString::CFixedString( const TCHAR * lptSourceString )
{
	m_ptBuffer		= NULL;
	m_nLength		= 0;
	m_nKey			= 0;

	Assign( lptSourceString );

	return;
}


//-----------------------------------------------------------------------------
// Name			: CFixedString
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedString::CFixedString( const CFixedString & rcSourceString )
{
	m_ptBuffer	= NULL;
	m_nLength	= 0;
	m_nKey		= 0;

	Assign( rcSourceString );

	return;
}


//-----------------------------------------------------------------------------
// Name			: ~CFixedString
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedString::~CFixedString( void )
{
	Assert( m_ptBuffer != NULL );

	Clear();

	return;
}


//-----------------------------------------------------------------------------
// Name			: Clear
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedString::Clear( void )
{
	if( m_ptBuffer != NULL )
	{
		DestroyBuffer();
		m_nLength	= 0;
		m_nKey		= 0;
	}

	Assert( m_nLength == 0 );

	return;
}


//-----------------------------------------------------------------------------
// Name			: DestroyBuffer
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedString::DestroyBuffer( void )
{
	if( m_nLength > 0 )
	{
		delete[] m_ptBuffer;
	}

	m_ptBuffer	= NULL;

	return;
}


//-----------------------------------------------------------------------------
// Name			: CreateBuffer
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedString::CreateBuffer( const TCHAR * lptBuffer )
{
	if( lptBuffer == NULL )
	{
		m_nLength	= 0;
	}
	else
	{
		m_nLength	= static_cast<UINT>(_tcslen( lptBuffer ));
	}

	if( m_nLength > 0 )
	{
		m_ptBuffer = new TCHAR[m_nLength + 1];
		Assert( m_ptBuffer != NULL );

		memcpy( m_ptBuffer, lptBuffer, GetBufferSize() );

		m_ptBuffer[m_nLength] = END_OF_STRING;
	}
	else
	{
		m_ptBuffer = const_cast<TCHAR *>(reinterpret_cast<const TCHAR *>(&END_OF_STRING));
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: CreateBuffer
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedString::CreateBuffer( const CFixedString & rString )
{
	m_nLength	= rString.GetLength();

	if( m_nLength > 0 )
	{
		m_ptBuffer = new TCHAR[m_nLength + 1];
		Assert( m_ptBuffer != NULL );

		Assert( rString.GetBuffer() != m_ptBuffer );

		memcpy( m_ptBuffer, rString.GetBuffer(), GetBufferSize() );

		m_ptBuffer[m_nLength] = END_OF_STRING;
	}
	else
	{
		m_ptBuffer = const_cast<TCHAR *>(reinterpret_cast<const TCHAR *>(&END_OF_STRING));
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: ExpandBuffer
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedString::ExpandBuffer( const TCHAR * lptBuffer )
{
	Assert( IsValid() == true );

	UINT		nEnpandLength;

	if( lptBuffer != NULL )
	{
		nEnpandLength = static_cast<UINT>(_tcslen( lptBuffer ));
	}
	else
	{
		nEnpandLength = 0;
	}

	if( nEnpandLength == 0 )
	{
		return;
	}

	TCHAR * lptNewBuffer = new TCHAR[m_nLength + nEnpandLength + 1];
	Assert( lptNewBuffer != NULL );

	if( m_nLength > 0 )
	{
		memcpy( lptNewBuffer, m_ptBuffer, m_nLength );
	}

	memcpy( &(lptNewBuffer[m_nLength]), lptBuffer, nEnpandLength * sizeof( TCHAR ) );

	DestroyBuffer();

	m_ptBuffer = lptNewBuffer;

	m_nLength += nEnpandLength;

	m_ptBuffer[m_nLength] = END_OF_STRING;

	return;
}


//-----------------------------------------------------------------------------
// Name			: ExpandBuffer
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedString::ExpandBuffer( const CFixedString & rString )
{
	Assert( IsValid() == true );

	if( rString.GetLength() == 0 )
	{
		return;
	}

	UINT	nNewLength = m_nLength + rString.GetLength();
	TCHAR * lptNewBuffer = new TCHAR[nNewLength + 1];
	Assert( lptNewBuffer != NULL );

	if( m_nLength > 0 )
	{
		memcpy( lptNewBuffer, m_ptBuffer, m_nLength );
	}

	memcpy( &(lptNewBuffer[m_nLength]), rString.GetBuffer(), rString.GetBufferSize() );
	lptNewBuffer[nNewLength] = END_OF_STRING;

	DestroyBuffer();

	m_ptBuffer = lptNewBuffer;

	m_nLength = nNewLength;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Assign
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedString & CFixedString::Assign( const CFixedString & rcSourceString )
{
	if( this == &rcSourceString )
	{
		return *this;
	}

	Clear();

	CreateBuffer( rcSourceString );

	GenerateKey();

	return *this;
}


//-----------------------------------------------------------------------------
// Name			: Assign
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedString & CFixedString::Assign( const TCHAR * lptSourceString )
{
	if( lptSourceString != NULL && m_ptBuffer == lptSourceString )
	{
		return *this;
	}

	Clear();

	CreateBuffer( lptSourceString );

	GenerateKey();

	return *this;
}


//-----------------------------------------------------------------------------
// Name			: AssignInteger
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedString & CFixedString::AssignInteger( long nValue, bool bHexa )
{
	Clear();

	TCHAR		szBuffer[64];

	if( bHexa == true )
	{
		_stprintf( szBuffer, _T( "0x%08X" ), nValue );
	}
	else
	{
		_stprintf( szBuffer, _T( "%d" ), nValue );
	}

	return Assign( szBuffer );
}


//-----------------------------------------------------------------------------
// Name			: AssignFloat
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedString & CFixedString::AssignFloat( float fValue, long nFraction )
{
	Clear();

	TCHAR		szBuffer[64];

	if( nFraction < 0 )
	{
		_stprintf( szBuffer, _T( "%f" ), fValue );
	}
	else
	{
		_stprintf( szBuffer, _T( "%f" ), fValue );
	}

	return Assign( szBuffer );
}


//-----------------------------------------------------------------------------
// Name			: Append
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedString & CFixedString::Append( const CFixedString & rcSourceString )
{
	Assert( IsValid() == true );

	ExpandBuffer( rcSourceString );

	GenerateKey();

	return *this;
}


//-----------------------------------------------------------------------------
// Name			: Append
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedString & CFixedString::Append( const TCHAR * lptCompareString )
{
	Assert( IsValid() == true );

	ExpandBuffer( lptCompareString );

	GenerateKey();

	return *this;
}


//-----------------------------------------------------------------------------
// Name			: Clone
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedString * CFixedString::Clone( void ) const
{
	CFixedString *		pClonedString = new CFixedString;
	if( pClonedString != NULL )
	{
		pClonedString->Assign( *this );
	}

	return pClonedString;
}


//-----------------------------------------------------------------------------
// Name			: GetAt
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

const TCHAR CFixedString::GetAt( UINT nIndex ) const
{
	Assert( IsValid() == true );

	if( nIndex > m_nLength )
	{
		return m_ptBuffer[m_nLength];
	}
	else
	{
		return m_ptBuffer[nIndex];
	}
}


//-----------------------------------------------------------------------------
// Name			: SetAt
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedString::SetAt( UINT nIndex, TCHAR tLetter )
{
	Assert( IsValid() == true );

	if( nIndex >= m_nLength )
	{
		return;
	}

	m_ptBuffer[nIndex] = tLetter;

	if( tLetter == NULL_CHAR )
	{
		Assert( !_T( "String Length Modified" ) );

		m_nLength = nIndex;
	}

	GenerateKey();

	return;
}


//-----------------------------------------------------------------------------
// Name			: FindLetter
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

UINT CFixedString::FindLetter( TCHAR tLetterToFind )
{
	Assert( IsValid() == true );

	if( m_nLength == 0 )
	{
		return SEARCH_FAIL;
	}

	for( UINT nIndex = 0L; nIndex < m_nLength; ++nIndex )
	{
		if( m_ptBuffer[nIndex] == tLetterToFind )
		{
			return nIndex;
		}
	}

	return SEARCH_FAIL;
}


//-----------------------------------------------------------------------------
// Name			: FindLetter
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

UINT CFixedString::FindLetter( TCHAR tLetterToFind, UINT nPositionToBegin, UINT nPositionToEnd )
{
	Assert( IsValid() == true );

	if( m_nLength == 0 || nPositionToBegin >= m_nLength || nPositionToBegin > nPositionToEnd )
	{
		return SEARCH_FAIL;
	}

	if( nPositionToEnd >= m_nLength )
	{
		nPositionToEnd = m_nLength - 1;
	}

	for( UINT nIndex = nPositionToBegin; nIndex <= nPositionToEnd; ++nIndex )
	{
		if( m_ptBuffer[nIndex] == tLetterToFind )
		{
			return nIndex;
		}
	}

	return SEARCH_FAIL;
}


//-----------------------------------------------------------------------------
// Name			: IsEqual
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CFixedString::IsEqual( const CFixedString & rcCompareString ) const
{
	Assert( IsValid() == true );

	if( m_nLength != rcCompareString.GetLength() )
	{
		return false;
	}

	if( GetKey() != rcCompareString.GetKey() )
	{
		return false;
	}

	return !memcmp( m_ptBuffer, rcCompareString.GetBuffer(), GetBufferSize() );
}


//-----------------------------------------------------------------------------
// Name			: IsEqual
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CFixedString::IsEqual( const TCHAR * ptCompareString ) const
{
	Assert( IsValid() == true );

	TCHAR *	ptBufferString = m_ptBuffer;

	while( *ptCompareString == *ptBufferString )
	{
		if( *ptCompareString == NULL_CHAR )
		{
			return true;
		}

		++ptCompareString;
		++ptBufferString;
	}

	return false;
}


//-----------------------------------------------------------------------------
// Name			: IsEquivalent
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CFixedString::IsEquivalent( const CFixedString & rcCompareString ) const
{
	Assert( IsValid() == true );

	if( m_nLength != rcCompareString.GetLength() )
	{
		return false;
	}

	TCHAR		tLetterOfThis;
	TCHAR		tLetterOfParam;

	for( UINT nIndex = 0; nIndex < m_nLength; ++nIndex )
	{
		tLetterOfThis	= ::ConvertToLowercase( m_ptBuffer[nIndex] );
		tLetterOfParam	= ::ConvertToLowercase( rcCompareString.GetAtFast( nIndex ) );

		if( tLetterOfThis != tLetterOfParam )
		{
			return false;
		}
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: IsEquivalent
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CFixedString::IsEquivalent( const TCHAR * ptCompareString ) const
{
	Assert( IsValid() == true );

	if( m_ptBuffer == ptCompareString )
	{
		return true;
	}

	if( m_ptBuffer == NULL || ptCompareString == NULL )
	{
		return false;
	}

	TCHAR *	ptBufferString = m_ptBuffer;

	while( ::ConvertToLowercase( *ptCompareString ) == ::ConvertToLowercase( *ptBufferString ) )
	{
		if( *ptCompareString == NULL_CHAR )
		{
			return true;
		}

		++ptCompareString;
		++ptBufferString;
	}

	return false;
}


//-----------------------------------------------------------------------------
// Name			: GenerateKey
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedString::GenerateKey( void )
{
	Assert( IsValid() == true );

	m_nKey = 0x00000001;

	for( UINT nIndex = 0; nIndex < m_nLength; ++nIndex )
	{
#ifdef UNICODE
		m_nKey ^= LOBYTE( m_ptBuffer[nIndex] ) ^ HIBYTE( m_ptBuffer[nIndex] );
#else
		m_nKey ^= m_ptBuffer[nIndex];
#endif
		if( m_nKey & 0x80000000 )
		{
			m_nKey <<= 1;
			m_nKey ^= 0x00000001;
		}
		else
		{
			m_nKey <<= 1;
		}
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: ConvertToUppercase
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

UINT CFixedString::ConvertToUppercase( void )
{
	Assert( IsValid() == true );

	UINT		nCount = 0;
	TCHAR		tChar;

	for( UINT nIndex = 0; nIndex < m_nLength; ++nIndex )
	{
		tChar = ::ConvertToUppercase( m_ptBuffer[nIndex] );

		if( tChar != m_ptBuffer[nIndex] )
		{
			m_ptBuffer[nIndex] = tChar;
			++nCount;
		}
	}

	if( nCount > 0 )
	{
		GenerateKey();
	}

	return nCount;
}


//-----------------------------------------------------------------------------
// Name			: ConvertToLowercase
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

UINT CFixedString::ConvertToLowercase( void )
{
	Assert( IsValid() == true );

	UINT		nCount = 0;
	TCHAR		tChar;

	for( UINT nIndex = 0; nIndex < m_nLength; ++nIndex )
	{
		tChar = ::ConvertToLowercase( m_ptBuffer[nIndex] );

		if( tChar != m_ptBuffer[nIndex] )
		{
			m_ptBuffer[nIndex] = tChar;
			++nCount;
		}
	}

	if( nCount > 0 )
	{
		GenerateKey();
	}

	return nCount;
}


//-----------------------------------------------------------------------------
// Name			: IsValid
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CFixedString::IsValid( void ) const
{
	if( this == NULL )
	{
		return FALSE;
	}

	if( m_ptBuffer == NULL )
	{
		return FALSE;
	}

	if( _tcslen( m_ptBuffer ) != m_nLength )
	{
		return FALSE;
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: ReportState
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CFixedString::ReportState( void )
{
	return IsValid();
}


//*****************************************************************************
//
// End of File : CFixedString.cpp
//
//*****************************************************************************