//*****************************************************************************
// File       : CFixedStringW.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "VersionLog.h"

#include "CFixedStringA.h"

#include "CFixedStringW.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( str )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

const SVersionInfo CFixedStringW::VERSION( _VERSION_MAJOR_, _VERSION_MINOR_, _VERSION_BUILD_, _VERSION_LABEL_ );

const wchar_t CFixedStringW::END_OF_STRING		= L'\0';
const UInt32 CFixedStringW::SEARCH_FAIL			= FIXED_STRING_SEARCH_FAIL;

const CFixedStringW CFixedStringW::NULLSTRING	= CFixedStringW();
CFixedStringW CFixedStringW::SEPARATORS			= CFixedStringW( FIXED_STRING_SEPARATORS_DEFAULT_W );

UINT CFixedStringW::CONVERSION_CODE_PAGE		= CP_ACP;
DWORD CFixedStringW::CONVERSION_FLAGS			= MB_ERR_INVALID_CHARS;

UInt32 CFixedStringW::m_nLastError				= ERROR_SUCCESS;


//-----------------------------------------------------------------------------
// Name			: Clear
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringW::Clear( void )
{
	DestroyBuffer();
	ResetBuffer();

	return;
}


//-----------------------------------------------------------------------------
// Name			: Assign
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringW::Assign( const CFixedStringW & rcSourceString )
{
	if( this == &rcSourceString )
	{
		return;
	}

	Clear();

	if( rcSourceString.GetLength() == 0 )
	{
		return;
	}

	CreateBuffer( rcSourceString.GetBuffer(), rcSourceString.GetLength() );

	GenerateKey();

	return;
}


//-----------------------------------------------------------------------------
// Name			: Assign
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringW::Assign( const CFixedStringA & rcSourceString )
{
	Clear();

	if( rcSourceString.GetLength() == 0 )
	{
		return;
	}

	CreateBuffer( rcSourceString.GetBuffer(), rcSourceString.GetLength() );

	GenerateKey();

	return;
}


//-----------------------------------------------------------------------------
// Name			: Assign
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringW::Assign( const wchar_t * pchSourceString )
{
	if( m_pchBuffer == pchSourceString )
	{
		return;
	}

	Clear();

	if( pchSourceString == NULL )
	{
		return;
	}

	size_t	nLength = wcslen( pchSourceString );
	if( nLength == 0 )
	{
		return;
	}

	CreateBuffer( pchSourceString, nLength );

	GenerateKey();

	return;
}


//-----------------------------------------------------------------------------
// Name			: Assign
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringW::Assign( const char * pchSourceString )
{
	Clear();

	if( pchSourceString == NULL )
	{
		return;
	}

	size_t	nLength = strlen( pchSourceString );
	if( nLength == 0 )
	{
		return;
	}

	CreateBuffer( pchSourceString, nLength );

	GenerateKey();

	return;
}


//-----------------------------------------------------------------------------
// Name			: Append
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringW::Append( const CFixedStringW & rcSourceString )
{
	if( rcSourceString.GetLength() == 0 )
	{
		return;
	}

	ExpandBuffer( rcSourceString.GetBuffer(), rcSourceString.GetLength() );

	GenerateKey();

	return;
}


//-----------------------------------------------------------------------------
// Name			: Append
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringW::Append( const CFixedStringA & rcSourceString )
{
	if( rcSourceString.GetLength() == 0 )
	{
		return;
	}

	ExpandBuffer( rcSourceString.GetBuffer(), rcSourceString.GetLength() );

	GenerateKey();

	return;
}


//-----------------------------------------------------------------------------
// Name			: Append
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringW::Append( const wchar_t * pchCompareString )
{
	if( pchCompareString == NULL )
	{
		return;
	}

	size_t		nLength = wcslen( pchCompareString );

	if( nLength == 0 )
	{
		return;
	}

	ExpandBuffer( pchCompareString, nLength );

	GenerateKey();

	return;
}


//-----------------------------------------------------------------------------
// Name			: Append
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringW::Append( const char * pchCompareString )
{
	if( pchCompareString == NULL )
	{
		return;
	}

	size_t		nLength = strlen( pchCompareString );

	if( nLength == 0 )
	{
		return;
	}

	ExpandBuffer( pchCompareString, nLength );

	GenerateKey();

	return;
}


//-----------------------------------------------------------------------------
// Name			: Clone
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedStringW * CFixedStringW::Clone( void ) const
{
	CFixedStringW *		pcClonedString = new CFixedStringW( *this );
	Assert( pcClonedString != NULL );

	return pcClonedString;
}


//-----------------------------------------------------------------------------
// Name			: CreateSubString
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedStringW * CFixedStringW::CreateSubString( size_t nBegin, size_t nEnd ) const
{
	CFixedStringW *		pcSubString = new CFixedStringW;

	pcSubString->CreateSubStringFrom( *this, nBegin, nEnd );

	return pcSubString;
}


//-----------------------------------------------------------------------------
// Name			: CreateSubString
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringW::CreateSubStringFrom( const CFixedStringW & rcSourceString, size_t nBegin, size_t nEnd )
{
	Clear();

	if( nBegin > nEnd )
	{
		return;
	}

	if( nEnd > rcSourceString.GetLength() )
	{
		return;
	}

	CreateBuffer( &(rcSourceString.GetBuffer()[nBegin]), nEnd + 1 - nBegin );

	return;
}


//-----------------------------------------------------------------------------
// Name			: DestroyBuffer
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringW::DestroyBuffer( void )
{
	Assert( IsValid() == true );

	if( m_nLength != 0 )
	{
		delete[] m_pchBuffer;
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: ResetBuffer
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringW::ResetBuffer( void )
{
	m_pchBuffer = const_cast<wchar_t *>(reinterpret_cast<const wchar_t *>(&END_OF_STRING));
	m_nLength	= 0;
	m_nKey		= 0;

	return;
}


//-----------------------------------------------------------------------------
// Name			: CreateBuffer
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringW::CreateBuffer( const wchar_t * pchBuffer, size_t nLength )
{
	Assert( pchBuffer != NULL);
	Assert( nLength != 0 );

	m_pchBuffer = new wchar_t[nLength + 1];
	Assert( m_pchBuffer != NULL );

	memcpy( m_pchBuffer, pchBuffer, sizeof( wchar_t ) * nLength );

	m_nLength	= nLength;
	m_pchBuffer[m_nLength] = END_OF_STRING;

	return;
}


//-----------------------------------------------------------------------------
// Name			: CreateBuffer
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringW::CreateBuffer( const char * pchBuffer, size_t nLength )
{
	Assert( pchBuffer != NULL );
	Assert( nLength != 0 );

	nLength = MultiByteToWideChar( CP_ACP, CONVERSION_FLAGS,
									pchBuffer, -1,
									NULL, 0 );
	Assert( nLength != 0 );

	m_pchBuffer = new wchar_t[nLength];
	Assert( m_pchBuffer != NULL );

	nLength = MultiByteToWideChar( CP_ACP, CONVERSION_FLAGS,
									pchBuffer, -1,
									m_pchBuffer, static_cast<int>(nLength) );
	if( nLength == 0 )
	{
		SetLastError( ::GetLastError() );

		Assert( !"MultiByteToWideChar() First Fail in CFixedStringW::CreateBuffer()" );

		ResetBuffer();
	}

	m_nLength = nLength - 1;

	return;
}


//-----------------------------------------------------------------------------
// Name			: ExpandBuffer
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringW::ExpandBuffer( const wchar_t * pchBuffer, size_t nExpandLength )
{
	Assert( pchBuffer != NULL );
	Assert( nExpandLength != 0 );

	wchar_t *	pchNewBuffer = new wchar_t[m_nLength + nExpandLength + 1];
	Assert( pchNewBuffer != NULL );

	memcpy( pchNewBuffer, m_pchBuffer, GetBufferSize() );
	memcpy( &(pchNewBuffer[m_nLength]), pchBuffer, sizeof( wchar_t ) * nExpandLength );

	DestroyBuffer();

	m_pchBuffer = pchNewBuffer;
	m_nLength += nExpandLength;
	m_pchBuffer[m_nLength] = END_OF_STRING;

	return;
}


//-----------------------------------------------------------------------------
// Name			: ExpandBuffer
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringW::ExpandBuffer( const char * pchBuffer, size_t nExpandLength )
{
	Assert( pchBuffer != NULL );
	Assert( nExpandLength != 0 );

	nExpandLength = MultiByteToWideChar( CP_ACP, CONVERSION_FLAGS,
										pchBuffer, -1,
										NULL, 0 );
	Assert( nExpandLength != 0 );

	wchar_t *	pchNewBuffer = new wchar_t[m_nLength + nExpandLength];
	Assert( pchNewBuffer != NULL );

	if( m_nLength > 0 )
	{
		memcpy( pchNewBuffer, m_pchBuffer, GetBufferSize() );
	}

	nExpandLength = MultiByteToWideChar( CP_ACP, CONVERSION_FLAGS,
									pchBuffer, -1,
									&(pchNewBuffer[m_nLength]), static_cast<int>(nExpandLength) );
	if( nExpandLength == 0 )
	{
		SetLastError( ::GetLastError() );

		Assert( !"MultiByteToWideChar() First Fail in CFixedStringW::ExpandBuffer()" );

		delete[] pchNewBuffer;
	}
	else
	{
		DestroyBuffer();

		m_pchBuffer = pchNewBuffer;
		m_nLength += nExpandLength - 1;
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: GetAt
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

const wchar_t CFixedStringW::GetAt( size_t nIndex ) const
{
	Assert( IsValid() == true );

	if( nIndex > m_nLength )
	{
		return m_pchBuffer[m_nLength];
	}
	else
	{
		return m_pchBuffer[nIndex];
	}
}


//-----------------------------------------------------------------------------
// Name			: SetAt
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringW::SetAt( size_t nIndex, wchar_t chLetter )
{
	Assert( IsValid() == true );

	if( nIndex >= m_nLength )
	{
		return;
	}

	m_pchBuffer[nIndex] = chLetter;

	if( chLetter == NULL_CHAR )
	{
		Assert( !L"String Length Modified" );

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

size_t CFixedStringW::FindLetter( wchar_t chLetterToFind ) const
{
	Assert( IsValid() == true );

	if( m_nLength == 0 )
	{
		return SEARCH_FAIL;
	}

	for( size_t nIndex = 0; nIndex < m_nLength; ++nIndex )
	{
		if( m_pchBuffer[nIndex] == chLetterToFind )
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

size_t CFixedStringW::FindLetter( wchar_t chLetterToFind, size_t nPositionToBegin, size_t nPositionToEnd ) const
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

	for( size_t nIndex = nPositionToBegin; nIndex <= nPositionToEnd; ++nIndex )
	{
		if( m_pchBuffer[nIndex] == chLetterToFind )
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

#ifdef _FAST_COMPARE_
bool CFixedStringW::IsEqual( const CFixedStringW & rcCompareString ) const
{
#ifdef _DEBUG_STRING_
	Assert( !memcmp( m_pchBuffer, rcCompareString.GetBuffer(), GetBufferSize() ) ==
			( GetKey() == rcCompareString.GetKey() ) );
#endif //_DEBUG_STRING_

	return ( GetKey() == rcCompareString.GetKey() );
}
#else //_FAST_COMPARE_
bool CFixedStringW::IsEqual( const CFixedStringW & rcCompareString ) const
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

	return !memcmp( m_pchBuffer, rcCompareString.GetBuffer(), GetBufferSize() );
}
#endif //_FAST_COMPARE_


//-----------------------------------------------------------------------------
// Name			: IsEqual
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

#ifdef _FAST_COMPARE_
bool CFixedStringW::IsEqual( const CFixedStringA & rcCompareString ) const
{
	CFixedStringW	rcConvertedCompareString( rcCompareString );

#ifdef _DEBUG_STRING_
	Assert( !memcmp( m_pchBuffer, cCovertedComrareString.GetBuffer(), GetBufferSize() ) ==
			( GetKey() == cCovertedComrareString.GetKey() ) );
#endif //_DEBUG_STRING_

	return ( GetKey() == rcConvertedCompareString.GetKey() );
}
#else //_FAST_COMPARE_
bool CFixedStringW::IsEqual( const CFixedStringA & rcCompareString ) const
{
	Assert( IsValid() == true );

	if( m_nLength != rcCompareString.GetLength() )
	{
		return false;
	}

	CFixedStringA	cConvertedCompareString( rcCompareString );

	if( GetKey() != cConvertedCompareString.GetKey() )
	{
		return false;
	}

	return !memcmp( m_pchBuffer, cConvertedCompareString.GetBuffer(), GetBufferSize() );
}
#endif //_FAST_COMPARE_


//-----------------------------------------------------------------------------
// Name			: IsEqual
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CFixedStringW::IsEqual( const wchar_t * pchCompareString ) const
{
	Assert( IsValid() == true );

	wchar_t *	pchBufferString = m_pchBuffer;

	while( *pchCompareString == *pchBufferString )
	{
		if( *pchCompareString == NULL_CHAR )
		{
			return true;
		}

		++pchCompareString;
		++pchBufferString;
	}

	return false;
}


//-----------------------------------------------------------------------------
// Name			: IsEqual
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CFixedStringW::IsEqual( const CFixedStringW & rcCompareString, size_t nLength ) const
{
	Assert( IsValid() == true );
	Assert( nLength >= 0 );

	for( size_t nIndex = 0; nIndex < nLength; ++nIndex )
	{
		if( m_pchBuffer[nIndex] != rcCompareString.GetAtFast( nIndex )  )
		{
			return false;
		}

		if( m_pchBuffer[nIndex] == NULL_CHAR )
		{
			return true;
		}
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: IsEqual
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CFixedStringW::IsEqual( const CFixedStringA & rcCompareString, size_t nLength ) const
{
	Assert( IsValid() == true );
	Assert( nLength >= 0 );

	if( m_nLength != rcCompareString.GetLength() )
	{
		return false;
	}

	CFixedStringA	cCovertedComrareString( rcCompareString );

	return IsEqual( cCovertedComrareString, nLength );
}


//-----------------------------------------------------------------------------
// Name			: IsEqual
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CFixedStringW::IsEqual( const wchar_t * pchCompareString, size_t nLength ) const
{
	Assert( IsValid() == true );
	Assert( nLength >= 0 );

	wchar_t *	pchBufferString = m_pchBuffer;

	while( *pchCompareString == *pchBufferString )
	{
		if( *pchCompareString == NULL_CHAR || nLength == 0 )
		{
			return true;
		}

		++pchCompareString;
		++pchBufferString;
		--nLength;
	}

	if( nLength == 0 )
	{
		return true;
	}
	else
	{
		return false;
	}
}


//-----------------------------------------------------------------------------
// Name			: IsEquivalent
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CFixedStringW::IsEquivalent( const CFixedStringW & rcCompareString ) const
{
	Assert( IsValid() == true );

	if( m_nLength != rcCompareString.GetLength() )
	{
		return false;
	}

	wchar_t		chLetterOfThis;
	wchar_t		chLetterOfParam;

	for( size_t nIndex = 0; nIndex < m_nLength; ++nIndex )
	{
		chLetterOfThis	= ::ConvertToLowercase( m_pchBuffer[nIndex] );
		chLetterOfParam	= ::ConvertToLowercase( rcCompareString.GetAtFast( nIndex ) );

		if( chLetterOfThis != chLetterOfParam )
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

bool CFixedStringW::IsEquivalent( const CFixedStringA & rcCompareString ) const
{
	Assert( IsValid() == true );

	if( m_nLength != rcCompareString.GetLength() )
	{
		return false;
	}

	CFixedStringA	rcConvertedCompareString( rcCompareString );

	wchar_t		chLetterOfThis;
	wchar_t		chLetterOfParam;

	for( size_t nIndex = 0; nIndex < m_nLength; ++nIndex )
	{
		chLetterOfThis	= ::ConvertToLowercase( m_pchBuffer[nIndex] );
		chLetterOfParam	= ::ConvertToLowercase( rcConvertedCompareString.GetAtFast( nIndex ) );

		if( chLetterOfThis != chLetterOfParam )
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

bool CFixedStringW::IsEquivalent( const wchar_t * pchCompareString ) const
{
	Assert( IsValid() == true );

	if( m_pchBuffer == pchCompareString )
	{
		return true;
	}

	if( m_pchBuffer == NULL || pchCompareString == NULL )
	{
		return false;
	}

	wchar_t *	pchBufferString = m_pchBuffer;

	while( ::ConvertToLowercase( *pchCompareString ) == ::ConvertToLowercase( *pchBufferString ) )
	{
		if( *pchCompareString == NULL_CHAR )
		{
			return true;
		}

		++pchCompareString;
		++pchBufferString;
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

bool CFixedStringW::IsEquivalent( const CFixedStringW & rcCompareString, size_t nLength ) const
{
	Assert( IsValid() == true );

	if( m_nLength != rcCompareString.GetLength() )
	{
		return false;
	}

	wchar_t		chLetterOfThis;
	wchar_t		chLetterOfParam;

	for( size_t nIndex = 0; nIndex < nLength; ++nIndex )
	{
		chLetterOfThis	= ::ConvertToLowercase( m_pchBuffer[nIndex] );
		chLetterOfParam	= ::ConvertToLowercase( rcCompareString.GetAtFast( nIndex ) );

		if( chLetterOfThis != chLetterOfParam )
		{
			return false;
		}

		if( m_pchBuffer[nIndex] == NULL_CHAR )
		{
			return true;
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

bool CFixedStringW::IsEquivalent( const CFixedStringA & rcCompareString, size_t nLength ) const
{
	Assert( IsValid() == true );

	if( m_nLength != rcCompareString.GetLength() )
	{
		return false;
	}

	CFixedStringA	cCovertedComrareString( rcCompareString );

	return IsEquivalent( cCovertedComrareString, nLength );
}


//-----------------------------------------------------------------------------
// Name			: IsEquivalent
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CFixedStringW::IsEquivalent( const wchar_t * pchCompareString, size_t nLength ) const
{
	Assert( IsValid() == true );

	if( m_pchBuffer == pchCompareString )
	{
		return true;
	}

	if( m_pchBuffer == NULL || pchCompareString == NULL )
	{
		return false;
	}

	wchar_t *	pchBufferString = m_pchBuffer;

	while( ::ConvertToLowercase( *pchCompareString ) == ::ConvertToLowercase( *pchBufferString ) )
	{
		if( *pchBufferString == NULL_CHAR || nLength == 0 )
		{
			return true;
		}

		++pchCompareString;
		++pchBufferString;
		--nLength;
	}

	if( nLength == 0 )
	{
		return true;
	}
	else
	{
		return false;
	}
}


//-----------------------------------------------------------------------------
// Name			: GenerateKey
// Desc			: 
// Privilege	: statuc public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

UInt32 CFixedStringW::GenerateKey( wchar_t * pszBuffer, size_t nLength )
{
if( pszBuffer == NULL || nLength == 0 )
	{
		return 0x00000000;
	}

	register UInt32	nKey = static_cast<UInt32>(nLength);
	register UInt32	nTemp;
	UInt32	nRemain =  static_cast<UInt32>(nLength) & 3;
	nLength >>= 2;

	while( nLength-- > 0 )
	{
		nKey += Get16BitsFromWchar( pszBuffer );
		nTemp = ( Get16BitsFromWchar( pszBuffer + 1 ) << 11 ) ^ nKey;
		nKey = ( nKey << 16 ) ^ nTemp;
		pszBuffer += 4;
		nKey += (nKey >> 11);
	}

   switch( nRemain )
   {
        case 3:
		{
			nKey += Get16BitsFromWchar( pszBuffer );
			nKey ^= nKey << 16;
			nKey ^= pszBuffer[2] << 18;
			nKey += nKey >> 11;

			break;
		}

        case 2:
		{
			nKey += Get16BitsFromWchar( pszBuffer );
            nKey ^= nKey << 11;
			nKey += nKey >> 17;

			break;
		}

        case 1:
		{
			nKey += *pszBuffer;
			nKey ^= nKey << 10;
			nKey += nKey >> 1;
		}
    }

    nKey ^= nKey << 3;
    nKey += nKey >> 5;
    nKey ^= nKey << 4;
    nKey += nKey >> 17;
    nKey ^= nKey << 25;
    nKey += nKey >> 6;

	return nKey;
}


//-----------------------------------------------------------------------------
// Name			: ConvertToUppercase
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

size_t CFixedStringW::ConvertToUppercase( void )
{
	Assert( IsValid() == true );

	size_t	nCount = 0;
	wchar_t	chLetter;

	for( size_t nIndex = 0; nIndex < m_nLength; ++nIndex )
	{
		chLetter = ::ConvertToUppercase( m_pchBuffer[nIndex] );

		if( chLetter != m_pchBuffer[nIndex] )
		{
			m_pchBuffer[nIndex] = chLetter;
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

size_t CFixedStringW::ConvertToLowercase( void )
{
	Assert( IsValid() == true );

	size_t	nCount = 0;
	wchar_t	chLetter;

	for( size_t nIndex = 0; nIndex < m_nLength; ++nIndex )
	{
		chLetter = ::ConvertToLowercase( m_pchBuffer[nIndex] );

		if( chLetter != m_pchBuffer[nIndex] )
		{
			m_pchBuffer[nIndex] = chLetter;
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
// Name			: ConvertToInt32
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

Int32 CFixedStringW::ConvertToInt32( void ) const
{
	Assert( IsValid() == true );

	bool	bPositive;
	UInt32	nPosition;

	if( m_nLength > 1 && m_pchBuffer[0] == '0' )
	{
		bPositive = true;
		nPosition = 1;
	}
	else if( m_nLength > 2 && m_pchBuffer[1] == '0' )
	{
		if( m_pchBuffer[0] == '+' )
		{
			bPositive = true;
		}
		else if( m_pchBuffer[0] == '-' )
		{
			bPositive = false;
		}
		else
		{
			return _wtoi( m_pchBuffer );
		}

		nPosition = 2;
	}
	else
	{
		return _wtoi( m_pchBuffer );
	}

	switch( m_pchBuffer[1] )
	{
		case 'x':
		case 'X':
		{
			return ConvertHexToInt32( bPositive, ++nPosition );
		}

		case 'b':
		case 'B':
		{
			return ConvertBinToInt32( bPositive, ++nPosition );
		}

		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		{
			return ConvertOctToInt32( bPositive, ++nPosition );
		}

		default:
		{
			return _wtoi( m_pchBuffer );
		}
	}
}


//-----------------------------------------------------------------------------
// Name			: ConvertBinToInt32
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

Int32 CFixedStringW::ConvertBinToInt32( bool bPositive, size_t nPosition ) const
{
	Int32	nResultValue = 0;
	wchar_t	chLetter;

	while( nPosition < m_nLength )
	{
		chLetter = m_pchBuffer[nPosition];

		nResultValue <<= 1;

		if( chLetter == L'1' )
		{
			++nResultValue;
		}
		else if( chLetter != L'0' )
		{
			return 0;
		}

		++nPosition;
	}

	if( bPositive == true )
	{
		return nResultValue;
	}
	else
	{
		return -nResultValue;
	}
}


//-----------------------------------------------------------------------------
// Name			: ConvertOctToInt32
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

Int32 CFixedStringW::ConvertOctToInt32( bool bPositive, size_t nPosition ) const
{
	Int32	nResultValue = 0;
	wchar_t	chLetter;

	while( nPosition < m_nLength )
	{
		chLetter = m_pchBuffer[nPosition];

		if( ( chLetter >= L'0' ) && ( chLetter <= L'7' ) )
		{
			nResultValue = ( nResultValue << 3 ) + ( chLetter - L'0' );
		}	
		else
		{
			return 0;
		}

		++nPosition;
	}

	if( bPositive == true )
	{
		return nResultValue;
	}
	else
	{
		return -nResultValue;
	}
}


//-----------------------------------------------------------------------------
// Name			: ConvertHexToInt32
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

Int32 CFixedStringW::ConvertHexToInt32( bool bPositive, size_t nPosition ) const
{
	Int32	nResultValue = 0;
	wchar_t	chLetter;

	while( nPosition < m_nLength )
	{
		chLetter = m_pchBuffer[nPosition];

		if( ( chLetter >= L'0' ) && ( chLetter <= L'9' ) )
		{
			nResultValue = ( nResultValue << 4 ) + ( chLetter - L'0' );
		}	
		else if( ( chLetter >= L'A' ) && ( chLetter <= L'F' ) )
		{
			nResultValue = ( nResultValue << 4 ) + ( chLetter - L'A' ) + 10;
		}
		else if( ( chLetter >= L'a' ) && ( chLetter <= L'f' ) )
		{
			nResultValue = ( nResultValue << 4 ) + ( chLetter - L'a' ) + 10;
		}
		else
		{
			return 0;
		}

		++nPosition;
	}

	if( bPositive == true )
	{
		return nResultValue;
	}
	else
	{
		return -nResultValue;
	}
}


//-----------------------------------------------------------------------------
// Name			: ConvertToLowercase
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

Float32 CFixedStringW::ConvertToFloat32( void ) const
{
	Assert( IsValid() == true );

	return static_cast<Float32>(_wtof( m_pchBuffer ));
}


//-----------------------------------------------------------------------------
// Name			: IsValid
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CFixedStringW::IsValid( void ) const
{
	if( this == NULL )
	{
		Assert( !"this == NULL" );

		return false;
	}

	if( m_pchBuffer == NULL )
	{
		Assert( !"m_pchBuffer == NULL" );

		return false;
	}

	if( wcslen( m_pchBuffer ) != m_nLength )
	{
		Assert( !"wcslen( m_pchBuffer ) != m_nLength" );

		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: ReportState
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CFixedStringW::ReportState( void )
{
	return IsValid();
}


//-----------------------------------------------------------------------------
// Name			: IsSeparator
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CFixedStringW::IsSeparator( wchar_t chLetter )
{
	for( size_t nIndex = 0; nIndex < SEPARATORS.GetLength(); ++nIndex )
	{
		if( SEPARATORS.GetAt( nIndex ) == chLetter )
		{
			return true;
		}
	}

	return false;
}


//*****************************************************************************
//
// End of File : CFixedStringW.cpp
//
//*****************************************************************************