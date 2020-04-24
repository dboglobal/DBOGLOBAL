//*****************************************************************************
// File       : CFixedStringListA.cpp
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

#include "CFixedStringListA.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( str )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

const SVersionInfo CFixedStringListA::VERSION( _VERSION_MAJOR_, _VERSION_MINOR_, _VERSION_BUILD_, _VERSION_LABEL_ );

const UInt32 CFixedStringListA::POSITION_FRONT		= C_STRING_LIST_POSITION_FRONT;
const UInt32 CFixedStringListA::POSITION_BACK		= C_STRING_LIST_POSITION_BACK;

CFixedStringA CFixedStringListA::SEPARATORS			= CFixedStringA( FIXED_STRING_SEPARATORS_DEFAULT_A );
CFixedStringA CFixedStringListA::PRE_SEPARATORS		= CFixedStringA( L"<" );
CFixedStringA CFixedStringListA::POST_SEPARATORS	= CFixedStringA( L">" );
CFixedStringA CFixedStringListA::TEXT_INDICATORS	= CFixedStringA( FIXED_STRING_TEXT_INDICATOR_DEFAULT_A );
CFixedStringA CFixedStringListA::COMMENT_INDICATOR	= CFixedStringA( FIXED_STRING_COMMENT_INDICATOR_DEFAULT_A );


//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringListA::Initialize( void )
{
	m_ptList = new TFixedStringListA;

	m_tCurrentPosition = m_ptList->end();

	m_nStringCount = 0L;

	return;
}


//-----------------------------------------------------------------------------
// Name			: ~CFixedStringListA
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedStringListA::~CFixedStringListA( void )
{
	PopAll( true );

	delete m_ptList;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Get
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

const CFixedStringA * const CFixedStringListA::Get( void ) const
{
	if( m_tCurrentPosition == m_ptList->end() )
	{
		return NULL;
	}

	return *m_tCurrentPosition;
}


//-----------------------------------------------------------------------------
// Name			: GetAt
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedStringA * CFixedStringListA::GetAt( UInt32 nPosition ) const
{
	if( nPosition > m_nStringCount )
	{
		nPosition = m_nStringCount;
	}

	TFixedStringListIteratorA	tPosition = m_ptList->begin();

	while( nPosition > 0 )
	{
		++tPosition;
		--nPosition;
	}

	if( tPosition == m_ptList->end() )
	{
		return NULL;
	}

	return *tPosition;
}


//-----------------------------------------------------------------------------
// Name			: GetLastIterator
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

TFixedStringListIteratorA CFixedStringListA::GetPrevIterator( TFixedStringListIteratorA tSearchPosition ) const
{
	TFixedStringListIteratorA	tPosition = m_ptList->begin();
	TFixedStringListIteratorA	tPrevPositon = tPosition;

	while( tPosition != tSearchPosition )
	{
		if( tPosition == m_ptList->end() )
		{
			return tPosition;
		}

		tPrevPositon = tPosition++;
	}

	return tPrevPositon;
}


//-----------------------------------------------------------------------------
// Name			: Push
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringListA::Push( const CFixedStringA * const pcString )
{
	PushFast( pcString->Clone() );

	return;
}


//-----------------------------------------------------------------------------
// Name			: PushFast
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringListA::PushFast( CFixedStringA * const pcString )
{
	Assert( pcString != NULL );

	m_ptList->insert( m_tCurrentPosition, pcString );

	++m_nStringCount;
	Assert( m_nStringCount == m_ptList->size() );

	return;
}


//-----------------------------------------------------------------------------
// Name			: PushAt
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringListA::PushAt( const CFixedStringA * const pcString, UInt32 nPosition )
{
	PushAtFast( pcString->Clone(), nPosition );

	return;
}


//-----------------------------------------------------------------------------
// Name			: PushAtFast
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringListA::PushAtFast( CFixedStringA * const pcString, UInt32 nPosition )
{
	Assert( pcString != NULL );

	switch( nPosition )
	{
		case POSITION_FRONT:
		{
			m_ptList->push_front( pcString );

			break;
		}

		case POSITION_BACK:
		{
			m_ptList->push_back( pcString );

			break;
		}

		default:
		{
			if( nPosition > m_nStringCount )
			{
				m_ptList->push_back( pcString );
			}
			else
			{
				TFixedStringListIteratorA	tPosition = m_ptList->begin();

				while( nPosition > 0 )
				{
					++tPosition;
					--nPosition;
				}

				m_ptList->insert( tPosition, pcString );
			}
		}
	}

	++m_nStringCount;
	Assert( m_nStringCount == m_ptList->size() );

	return;
}


//-----------------------------------------------------------------------------
// Name			: Pop
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedStringA * CFixedStringListA::Pop( bool bDestroy )
{
	if( m_tCurrentPosition == m_ptList->end() )
	{
		Assert( !"Can't Pop at End Position" );

		return NULL;
	}

	TFixedStringListIteratorA	tNextPosition = m_tCurrentPosition;
	++tNextPosition;

	m_ptList->erase( m_tCurrentPosition );

	CFixedStringA * pcReturn = *m_tCurrentPosition;
	m_tCurrentPosition = tNextPosition;

	if( bDestroy == true )
	{
		delete pcReturn;
		pcReturn = NULL;
	}

	--m_nStringCount;
	Assert( m_nStringCount == m_ptList->size() );

	return pcReturn;
}


//-----------------------------------------------------------------------------
// Name			: PopAt
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedStringA * CFixedStringListA::PopAt( UInt32 nPosition, bool bDestroy )
{
	TFixedStringListIteratorA	tPosition;
	TFixedStringListIteratorA	tNextPosition = m_tCurrentPosition;
	++tNextPosition;

	switch( nPosition )
	{
		case POSITION_FRONT:
		{
			tPosition = m_ptList->begin();

			if( tPosition == m_tCurrentPosition )
			{
				++m_tCurrentPosition;
			}

			m_ptList->pop_front();

			break;
		}

		case POSITION_BACK:
		{
			tPosition = GetPrevIterator( m_ptList->end() );

			if( tPosition == m_tCurrentPosition )
			{
				m_tCurrentPosition = m_ptList->end();
			}

			m_ptList->pop_back();

			break;
		}

		default:
		{
			if( nPosition >= m_nStringCount )
			{
				tPosition =  GetPrevIterator( m_ptList->end() );

				if( tPosition == m_tCurrentPosition )
					m_tCurrentPosition = m_ptList->end();

				m_ptList->pop_back();
			}
			else
			{
				tPosition = m_ptList->begin();

				while( nPosition > 0 )
				{
					++tPosition;
					--nPosition;
				}

				if( tPosition == m_tCurrentPosition )
				{
					++m_tCurrentPosition;
				}

				m_ptList->erase( tPosition );
			}
		}
	}

	m_tCurrentPosition = tNextPosition;

	CFixedStringA *	pcReturn = *tPosition;

	if( bDestroy == true )
	{
		delete pcReturn;
		pcReturn = NULL;
	}

	--m_nStringCount;
	Assert( m_nStringCount == m_ptList->size() );

	return pcReturn;
}


//-----------------------------------------------------------------------------
// Name			: PopAll
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringListA::PopAll( bool bDestroy )
{
	if( bDestroy == true )
	{
		CFixedStringA *		pcString;

		m_tCurrentPosition = m_ptList->begin();

		while( m_tCurrentPosition != m_ptList->end() )
		{
			pcString = *m_tCurrentPosition;
			++m_tCurrentPosition;

			delete pcString;
		}
	}

	m_ptList->clear();
	m_nStringCount = 0;

	return;
}


//-----------------------------------------------------------------------------
// Name			: MoveFront
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CFixedStringListA::MoveFront( void )
{
	m_tCurrentPosition = m_ptList->begin();

	return true;
}


//-----------------------------------------------------------------------------
// Name			: MoveBack
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CFixedStringListA::MoveBack( void )
{
	m_tCurrentPosition = m_ptList->end();

	return true;
}


//-----------------------------------------------------------------------------
// Name			: MovePrev
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CFixedStringListA::MovePrev( void )
{
	if( m_tCurrentPosition == m_ptList->begin() )
	{
		return false;
	}

	m_tCurrentPosition = GetPrevIterator( m_tCurrentPosition );

	Assert( m_tCurrentPosition != m_ptList->end() );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: MoveNext
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CFixedStringListA::MoveNext( void )
{
	if( m_tCurrentPosition == m_ptList->end() )
	{
		return false;
	}

	++m_tCurrentPosition;

	return true;
}


//-----------------------------------------------------------------------------
// Name			: MoveAt
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CFixedStringListA::MoveAt( UInt32 nPosition )
{
	if( nPosition >= m_nStringCount )
	{
		return MoveBack();
	}

	m_tCurrentPosition = m_ptList->begin();

	while( nPosition > 0 )
	{
		++m_tCurrentPosition;
		--nPosition;
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: GetBufferSize
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

size_t CFixedStringListA::GetBufferSize( void ) const
{
	TFixedStringListIteratorA	tPosition = m_ptList->begin();
	size_t						nBufferSize = 0;
	CFixedStringA *				pcString;

	while( tPosition != m_ptList->end() )
	{
		pcString = *tPosition;
		nBufferSize += pcString->GetBufferSize();
		++tPosition;
	}

	return nBufferSize;
}


//-----------------------------------------------------------------------------
// Name			: FindEqual
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

UInt32 CFixedStringListA::FindEqual( const CFixedStringA & rcString, UInt32 nBeginIndex, UInt32 nEndIndex ) const
{
	Assert( nBeginIndex < m_nStringCount );
	Assert( nEndIndex < m_nStringCount );
	Assert( nBeginIndex <= nEndIndex );

	for( UInt32 nIndex = nBeginIndex; nIndex <= nEndIndex; ++nIndex )
	{
		Assert( GetAt( nIndex ) != NULL );

		if( GetAt( nIndex )->IsEqual( rcString ) == true )
		{
			return nIndex;
		}
	}

	return FIXED_STRING_SEARCH_FAIL;
}


//-----------------------------------------------------------------------------
// Name			: FindEquivalent
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

UInt32 CFixedStringListA::FindEquivalent( const CFixedStringA & rcString, UInt32 nBeginIndex, UInt32 nEndIndex ) const
{
	Assert( nBeginIndex < m_nStringCount );
	Assert( nEndIndex < m_nStringCount );
	Assert( nBeginIndex <= nEndIndex );

	for( UInt32 nIndex = nBeginIndex; nIndex <= nEndIndex; ++nIndex )
	{
		Assert( GetAt( nIndex ) != NULL );

		if( GetAt( nIndex )->IsEquivalent( rcString ) == true )
		{
			return nIndex;
		}
	}

	return FIXED_STRING_SEARCH_FAIL;
}


//-----------------------------------------------------------------------------
// Name			: CreateFrom
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

size_t CFixedStringListA::CreateFrom( const CFixedStringA & rcBuffer, size_t nLetterCount )
{
	if( rcBuffer.GetLength() == 0 )
	{
		return 0;
	}

	if( nLetterCount == 0 )
	{
		nLetterCount = rcBuffer.GetLength();
		if( nLetterCount == 0 )
		{
			return 0;
		}
	}

	char *		pchTempBuffer = new char[nLetterCount + 1];
	memcpy( pchTempBuffer, rcBuffer.GetBuffer(), sizeof( char ) * nLetterCount );
	pchTempBuffer[nLetterCount] = NULL_CHAR;

	size_t		nResultCount = ParseAndCreate( pchTempBuffer, nLetterCount );

	delete[] pchTempBuffer;

	return nResultCount;
}


//-----------------------------------------------------------------------------
// Name			: CreateFrom
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

size_t CFixedStringListA::CreateFrom( const char * const pchBuffer, size_t nLetterCount )
{
	Assert( pchBuffer != NULL );

	if( nLetterCount == 0 )
	{
		nLetterCount = strlen( pchBuffer );
		if( nLetterCount == 0 )
		{
			return 0;
		}
	}

	char *	pchTempBuffer = new char[nLetterCount + 1];
	memcpy( pchTempBuffer, pchBuffer, nLetterCount );
	pchTempBuffer[nLetterCount] = NULL_CHAR;

	size_t	nResultCount = ParseAndCreate( pchTempBuffer, nLetterCount );

	delete[] pchTempBuffer;

	return nResultCount;
}


//-----------------------------------------------------------------------------
// Name			: CreateFromFile
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

size_t CFixedStringListA::CreateFromFile( const CFixedStringA & rcFileName, size_t nLetterCount )
{
	return CreateFromFile( rcFileName.GetBuffer(), nLetterCount );
}


//-----------------------------------------------------------------------------
// Name			: CreateFromFile
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

size_t CFixedStringListA::CreateFromFile( const char * const pchFileName, size_t nLetterCount )
{
	HANDLE	hFile = ::CreateFileSafeA( pchFileName, GENERIC_READ );
	if( hFile == INVALID_HANDLE_VALUE )
	{
		return 0;
	}

	size_t	nResultSize = CreateFromFile( hFile, nLetterCount );

	CloseHandle( hFile );

	return nResultSize;
}


//-----------------------------------------------------------------------------
// Name			: CreateFromFile
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

size_t CFixedStringListA::CreateFromFile( HANDLE hFile, size_t nLetterCount )
{
	Assert( hFile != INVALID_HANDLE_VALUE );

	DWORD	dwFileSize = GetFileSize( hFile, NULL );
	DWORD	dwSizeRead;
	BYTE	abtBOM[4] = { 0, };

	// Parse File Format & Convert to Unicode

	if( dwFileSize >= 3 )
	{
		if( ReadFile( hFile, abtBOM, 3, &dwSizeRead, NULL ) == FALSE )
		{
			return 0;
		}
	}
	else if( dwFileSize >= 2 )
	{
		if( ReadFile( hFile, abtBOM, 2, &dwSizeRead, NULL ) == FALSE )
		{
			return 0;
		}
	}

	char *	pchBuffer;
	if( abtBOM[0] == 0xFF && abtBOM[1] == 0xFE )								// UTF-16 Little Endian
	{
		if( nLetterCount == 0 )
		{
			nLetterCount = static_cast<size_t>((dwFileSize - 2) / 2);
		}
		else
		{
			nLetterCount = Min( nLetterCount, static_cast<size_t>((dwFileSize - 2) / 2) );
		}

		wchar_t *	pchTempBuffer = new wchar_t[nLetterCount + 1];
		Assert( pchTempBuffer != NULL );
		pchTempBuffer[nLetterCount] = NULL_CHAR;

		SetFilePointer( hFile, 2, NULL, FILE_BEGIN );
		if( ReadFile( hFile, pchTempBuffer, static_cast<DWORD>(nLetterCount * 2), &dwSizeRead, NULL ) == FALSE )
		{
			delete[] pchTempBuffer;

			return 0;
		}

		nLetterCount = WideCharToMultiByte( CP_ACP, CFixedStringA::CONVERSION_FLAGS,
											pchTempBuffer, -1,
											NULL, 0,
											NULL, NULL );
		Assert( nLetterCount != 0 );

		pchBuffer = new char[nLetterCount + 1];
		Assert( pchBuffer != NULL );

		nLetterCount = WideCharToMultiByte( CP_ACP, CFixedStringA::CONVERSION_FLAGS,
											pchTempBuffer, -1,
											pchBuffer, static_cast<int>(nLetterCount + 1),
											NULL, NULL );
		Assert( nLetterCount != 0 );

		pchBuffer[nLetterCount] = NULL_CHAR;

		delete[] pchTempBuffer;
	}
	else if( abtBOM[0] == 0xFE && abtBOM[1] == 0xFF )							// UTF-16 Big Endian
	{
		Assert( !"Unsupported Endian" );

		return 0;
	}
	else if( abtBOM[0] == 0xEF && abtBOM[1] == 0xBB && abtBOM[2] == 0xBF )		// UTF-8 Little Endian
	{
		if( nLetterCount == 0 )
		{
			nLetterCount = static_cast<size_t>(dwFileSize - 3);
		}
		else
		{
			nLetterCount = Min( nLetterCount, static_cast<size_t>(dwFileSize - 3) );
		}

		char *	pchTempBuffer = new char[nLetterCount + 1];
		Assert( pchTempBuffer != NULL );
		pchTempBuffer[nLetterCount] = NULL_CHAR;

		SetFilePointer( hFile, 3, NULL, FILE_BEGIN );
		if( ReadFile( hFile, pchTempBuffer, static_cast<DWORD>(nLetterCount), &dwSizeRead, NULL ) == FALSE )
		{
			delete[] pchTempBuffer;

			return 0;
		}

		// Convert UTF-8 to UTF-16

		nLetterCount = MultiByteToWideChar( CP_UTF8, CFixedStringW::CONVERSION_FLAGS,
											pchTempBuffer, -1,
											NULL, 0 );
		Assert( nLetterCount != 0 );

		wchar_t *	pchTemp2Buffer = new wchar_t[nLetterCount + 1];
		Assert( pchTemp2Buffer != NULL );
		pchTemp2Buffer[nLetterCount] = NULL_CHAR;

		nLetterCount = MultiByteToWideChar( CP_UTF8, CFixedStringW::CONVERSION_FLAGS,
											pchTempBuffer, -1,
											pchTemp2Buffer, static_cast<int>(nLetterCount) );
		Assert( nLetterCount != 0 );

		delete[] pchTempBuffer;

		// Convert UTF-16 to MBCS

		nLetterCount = WideCharToMultiByte( CP_ACP, CFixedStringA::CONVERSION_FLAGS,
											pchTemp2Buffer, -1,
											NULL, 0,
											NULL, NULL );
		Assert( nLetterCount != 0 );

		pchBuffer = new char[nLetterCount + 1];
		Assert( pchBuffer != NULL );

		nLetterCount = WideCharToMultiByte( CP_ACP, CFixedStringA::CONVERSION_FLAGS,
											pchTemp2Buffer, -1,
											pchBuffer, static_cast<int>(nLetterCount + 1),
											NULL, NULL );
		Assert( nLetterCount != 0 );

		pchBuffer[nLetterCount] = NULL_CHAR;

		delete[] pchTemp2Buffer;
	}
	else if( abtBOM[0] == 0xBF && abtBOM[1] == 0xBB && abtBOM[2] == 0xEF )		// UTF-8 Big Endian
	{
		Assert( !"Unsupported Endian" );

		return 0;
	}
	else																		// MBCS
	{
		if( nLetterCount == 0 )
		{
			nLetterCount = static_cast<size_t>(dwFileSize);
		}
		else
		{
			nLetterCount = Min( nLetterCount, static_cast<size_t>(dwFileSize) );
		}

		pchBuffer = new char[nLetterCount + 1];
		Assert( pchBuffer != NULL );
		pchBuffer[nLetterCount] = NULL_CHAR;

		SetFilePointer( hFile, 0, NULL, FILE_BEGIN );
		if( ReadFile( hFile, pchBuffer, static_cast<DWORD>(nLetterCount), &dwSizeRead, NULL ) == FALSE )
		{
			delete[] pchBuffer;

			return 0;
		}
	}

	// Parse & Create

	size_t	nResultCount = ParseAndCreate( pchBuffer, nLetterCount );

	delete[] pchBuffer;

	return nResultCount;
}


//-----------------------------------------------------------------------------
// Name			: Serialize
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

size_t CFixedStringListA::Serialize( CFixedStringA & rcBuffer, char chSeparator ) const
{
	size_t		nLetterCount = 0L;
	size_t		nSizeofSeparator = ( ( chSeparator == '\0' ) ? 0L : 1L );
	TFixedStringListIteratorA	tPosition = m_ptList->begin();
	CFixedStringA *		pcString;

	while( tPosition != m_ptList->end() )
	{
		pcString = *tPosition;

		nLetterCount += pcString->GetLength();
		nLetterCount += nSizeofSeparator;								// for Separator

		++tPosition;
	}

	char *		pchBuffer = new char[nLetterCount + 1];
	Serialize( pchBuffer, nLetterCount, chSeparator );
	rcBuffer.Assign( pchBuffer );

	return rcBuffer.GetBufferSize();
}


//-----------------------------------------------------------------------------
// Name			: Serialize
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

size_t CFixedStringListA::Serialize( char * const pchBuffer, size_t nLetterCount, char chSeparator ) const
{
	if( nLetterCount == 0L )
	{
		nLetterCount = strlen( pchBuffer );
		Assert( nLetterCount > 0L );
	}

	size_t		nSizeofSeparator = ( ( chSeparator == '\0' ) ? 0L : sizeof( char ) );
	size_t		nLetterIndex = 0L;
	TFixedStringListIteratorA	tPosition = m_ptList->begin();
	CFixedStringA *		pcString;
	bool	bEndWithSeparator = true;

	while( tPosition != m_ptList->end() )
	{
		pcString = *tPosition;

		if( pcString->GetLength() == 0L )
		{
			++tPosition;

			continue;
		}

		if( nLetterIndex + pcString->GetLength() >= nLetterCount )
		{
			pchBuffer[nLetterIndex] = NULL_CHAR;

			return nLetterIndex;
		}

		if( bEndWithSeparator == false )
		{
			memcpy( &(pchBuffer[nLetterIndex]), &chSeparator, nSizeofSeparator );
			++nLetterIndex;
		}

		memcpy( &(pchBuffer[nLetterIndex]), pcString->GetBuffer(), pcString->GetBufferSize() );
		nLetterIndex += pcString->GetLength();

		if( nSizeofSeparator > 0L )
		{
			bEndWithSeparator = CFixedStringA::IsSeparator( pcString->GetAt( pcString->GetLength() - 1 ) );
		}

		++tPosition;
	}

	pchBuffer[nLetterIndex] = NULL_CHAR;

	return nLetterIndex * sizeof( char );
}


//-----------------------------------------------------------------------------
// Name			: Serialize
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

size_t CFixedStringListA::Serialize( HANDLE hFile, char	chSeparator, bool bBOM ) const
{
	UNREFERENCED_PARAMETER( bBOM );

	DWORD		dwWrittenSize;
	DWORD		dwTotalWrittenSize = 0L;

	size_t		nSizeofSeparator = ( ( chSeparator == '\0' ) ? 0L : sizeof( char ) );
	TFixedStringListIteratorA	tPosition = m_ptList->begin();
	CFixedStringA *		pcString;
	bool	bEndWithSeparator = true;

	while( tPosition != m_ptList->end() )
	{
		pcString = *tPosition;

		if( WriteFile( hFile, pcString->GetBuffer(), static_cast<DWORD>(pcString->GetBufferSize()), &dwWrittenSize, NULL ) == FALSE )
		{
			dwWrittenSize = 0L;
		}

		dwTotalWrittenSize += dwWrittenSize;

		if( bEndWithSeparator == false )
		{
			if( WriteFile( hFile, &chSeparator, static_cast<DWORD>(nSizeofSeparator), &dwWrittenSize, NULL ) == FALSE )
			{
				dwWrittenSize = 0L;
			}
		}

		if( nSizeofSeparator > 0L )
		{
			bEndWithSeparator = CFixedStringA::IsSeparator( pcString->GetAt( pcString->GetLength() - 1 ) );
		}

		dwTotalWrittenSize += dwWrittenSize;
		++tPosition;
	}

	return dwTotalWrittenSize;
}


//-----------------------------------------------------------------------------
// Name			: ParseAndCreate
// Desc			: 
// Privilege	: Protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

size_t CFixedStringListA::ParseAndCreate( char * const pchBuffer, size_t nBufferSize )
{
	EFixedStringListParseState	ePreviousState	= E_FIXED_STRING_LIST_PARSE_STATE_NONE;
	EFixedStringListParseState	eNextState		= E_FIXED_STRING_LIST_PARSE_STATE_NONE;

	char *		pchBufferPosition = pchBuffer;
	size_t		nBeginPosition = 0L;
	size_t		nPosition = 0L;

	while( eNextState != E_FIXED_STRING_LIST_PARSE_STATE_COUNT && nPosition < nBufferSize )
	{
		ePreviousState = DetermineCurrentState( eNextState, pchBufferPosition );
		EndPreviousState( ePreviousState, &pchBuffer[nBeginPosition], nPosition - nBeginPosition );
		eNextState = BeginNextState( eNextState, nBeginPosition, nPosition );

		++nPosition;
		++pchBufferPosition;
	}

	return --nPosition;
}


//-----------------------------------------------------------------------------
// Name			: DetermineCurrentState
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

EFixedStringListParseState CFixedStringListA::DetermineCurrentState( EFixedStringListParseState & reCurrentState, char * pchPosition )
{
	switch( reCurrentState )
	{
		case E_FIXED_STRING_LIST_PARSE_STATE_NONE:
		{
			if( *pchPosition == NULL_CHAR )
			{
				reCurrentState = E_FIXED_STRING_LIST_PARSE_STATE_COUNT;

				return E_FIXED_STRING_LIST_PARSE_STATE_NONE;
			}
			else if( ( *pchPosition <= 0x20 ) ||
					( SEPARATORS.FindLetter( *pchPosition ) ) != CFixedStringA::SEARCH_FAIL )
			{
				reCurrentState = E_FIXED_STRING_LIST_PARSE_STATE_NONE;

				return E_FIXED_STRING_LIST_PARSE_STATE_NONE;
			}
			else if( PRE_SEPARATORS.IsEqual( pchPosition, PRE_SEPARATORS.GetLength() ) == true )
			{
				reCurrentState = E_FIXED_STRING_LIST_PARSE_STATE_BEGIN_STRING_PRE;

				return E_FIXED_STRING_LIST_PARSE_STATE_END_STRING;
			}
			else if( POST_SEPARATORS.IsEqual( pchPosition, POST_SEPARATORS.GetLength() ) == true )
			{
				reCurrentState = E_FIXED_STRING_LIST_PARSE_STATE_NONE;

				return E_FIXED_STRING_LIST_PARSE_STATE_END_STRING_POST;
			}
			else if( TEXT_INDICATORS.FindLetter( *pchPosition ) != CFixedStringA::SEARCH_FAIL )
			{
				reCurrentState = E_FIXED_STRING_LIST_PARSE_STATE_BEGIN_TEXT;

				return E_FIXED_STRING_LIST_PARSE_STATE_NONE;
			}
			else if( COMMENT_INDICATOR.IsEqual( pchPosition, COMMENT_INDICATOR.GetLength() ) == true )
			{
				reCurrentState = E_FIXED_STRING_LIST_PARSE_STATE_BEGIN_COMMENT;

				return E_FIXED_STRING_LIST_PARSE_STATE_NONE;
			}
			else
			{
				reCurrentState = E_FIXED_STRING_LIST_PARSE_STATE_BEGIN_STRING;

				return E_FIXED_STRING_LIST_PARSE_STATE_NONE;
			}
		}

		case E_FIXED_STRING_LIST_PARSE_STATE_IN_COMMENT:
		{
			if( *pchPosition == LINE_FEED_CHAR )
			{
				reCurrentState = E_FIXED_STRING_LIST_PARSE_STATE_NONE;

				return E_FIXED_STRING_LIST_PARSE_STATE_END_COMMENT;
			}
			else if( *pchPosition == NULL_CHAR )
			{
				reCurrentState = E_FIXED_STRING_LIST_PARSE_STATE_COUNT;

				return E_FIXED_STRING_LIST_PARSE_STATE_END_COMMENT;
			}
			else
			{
				reCurrentState = E_FIXED_STRING_LIST_PARSE_STATE_IN_COMMENT;

				return E_FIXED_STRING_LIST_PARSE_STATE_NONE;
			}
		}

		case E_FIXED_STRING_LIST_PARSE_STATE_IN_STRING:
		{
			if( *pchPosition == NULL_CHAR )
			{
				reCurrentState = E_FIXED_STRING_LIST_PARSE_STATE_COUNT;

				return E_FIXED_STRING_LIST_PARSE_STATE_END_STRING;
			}
			else if( ( *pchPosition <= 0x20 ) ||
					( SEPARATORS.FindLetter( *pchPosition ) != CFixedStringA::SEARCH_FAIL ) )
			{
				reCurrentState = E_FIXED_STRING_LIST_PARSE_STATE_NONE;

				return E_FIXED_STRING_LIST_PARSE_STATE_END_STRING;
			}
			else if( COMMENT_INDICATOR.IsEqual( pchPosition, COMMENT_INDICATOR.GetLength() ) == true )
			{
				reCurrentState = E_FIXED_STRING_LIST_PARSE_STATE_BEGIN_COMMENT;

				return E_FIXED_STRING_LIST_PARSE_STATE_END_STRING;
			}
			else if( TEXT_INDICATORS.FindLetter( *pchPosition ) != CFixedStringA::SEARCH_FAIL )
			{
				reCurrentState = E_FIXED_STRING_LIST_PARSE_STATE_BEGIN_TEXT;

				return E_FIXED_STRING_LIST_PARSE_STATE_END_STRING;
			}
			else
			{
				reCurrentState = E_FIXED_STRING_LIST_PARSE_STATE_IN_STRING;

				return E_FIXED_STRING_LIST_PARSE_STATE_NONE;
			}
		}

		case E_FIXED_STRING_LIST_PARSE_STATE_IN_TEXT:
		{
			if( *pchPosition == NULL_CHAR )
			{
				reCurrentState = E_FIXED_STRING_LIST_PARSE_STATE_COUNT;

				return E_FIXED_STRING_LIST_PARSE_STATE_END_TEXT;
			}
			else if( TEXT_INDICATORS.FindLetter( *pchPosition ) != CFixedStringA::SEARCH_FAIL )
			{
				reCurrentState = E_FIXED_STRING_LIST_PARSE_STATE_NONE;

				return E_FIXED_STRING_LIST_PARSE_STATE_END_TEXT;
			}
			else if( COMMENT_INDICATOR.IsEqual( pchPosition, COMMENT_INDICATOR.GetLength() ) == true )
			{
				reCurrentState = E_FIXED_STRING_LIST_PARSE_STATE_BEGIN_COMMENT;

				return E_FIXED_STRING_LIST_PARSE_STATE_END_TEXT;
			}
			else
			{
				reCurrentState = E_FIXED_STRING_LIST_PARSE_STATE_IN_TEXT;

				return E_FIXED_STRING_LIST_PARSE_STATE_NONE;
			}
		}

		default:
		{
			Assert( !"Invalid State" );

			if( *pchPosition == NULL_CHAR )
			{
				reCurrentState = E_FIXED_STRING_LIST_PARSE_STATE_COUNT;

				return E_FIXED_STRING_LIST_PARSE_STATE_NONE;
			}
			else
			{
				reCurrentState = E_FIXED_STRING_LIST_PARSE_STATE_NONE;

				return E_FIXED_STRING_LIST_PARSE_STATE_NONE;
			}
		}
	}
}


//-----------------------------------------------------------------------------
// Name			: EndPreviousState
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CFixedStringListA::EndPreviousState( EFixedStringListParseState ePreviousState, char * const pchBuffer, size_t nLength )
{
	switch( ePreviousState )
	{
		case E_FIXED_STRING_LIST_PARSE_STATE_NONE:
		{
			return true;
		}

		case E_FIXED_STRING_LIST_PARSE_STATE_END_COMMENT:
		{
			return true;
		}

		case E_FIXED_STRING_LIST_PARSE_STATE_END_STRING:
		{
			char	chTemp = pchBuffer[nLength];
			pchBuffer[nLength] = '\0';

			CFixedStringA *		pNewString = new CFixedStringA( pchBuffer );

			pchBuffer[nLength] = chTemp;

			PushFast( pNewString );

			return true;
		}

		case E_FIXED_STRING_LIST_PARSE_STATE_END_STRING_POST:
		{
			char		chTemp = pchBuffer[nLength + 1];
			pchBuffer[nLength + 1] = NULL_CHAR;

			CFixedStringA *		pNewString = new CFixedStringA( pchBuffer );

			pchBuffer[nLength + 1] = chTemp;

			PushFast( pNewString );

			return true;
		}

		case E_FIXED_STRING_LIST_PARSE_STATE_END_TEXT:
		{
			char	chTemp = pchBuffer[nLength];
			pchBuffer[nLength] = NULL_CHAR;

			CFixedStringA *		pNewString = new CFixedStringA( pchBuffer );

			pchBuffer[nLength] = chTemp;

			PushFast( pNewString );

			return true;
		}

		default:
		{
			Assert( !"Invalid State" );

			return false;
		}
	}
}


//-----------------------------------------------------------------------------
// Name			: BeginNextState
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

EFixedStringListParseState CFixedStringListA::BeginNextState( EFixedStringListParseState eCurrentState, size_t & rnBeginPosition, size_t nPosition )
{
	switch( eCurrentState )
	{
		case E_FIXED_STRING_LIST_PARSE_STATE_NONE:
		case E_FIXED_STRING_LIST_PARSE_STATE_IN_COMMENT:
		case E_FIXED_STRING_LIST_PARSE_STATE_IN_STRING:
		case E_FIXED_STRING_LIST_PARSE_STATE_IN_TEXT:
		case E_FIXED_STRING_LIST_PARSE_STATE_COUNT:
		{
			return eCurrentState;
		}

		case E_FIXED_STRING_LIST_PARSE_STATE_BEGIN_COMMENT:
		{
			return E_FIXED_STRING_LIST_PARSE_STATE_IN_COMMENT;
		}

		case E_FIXED_STRING_LIST_PARSE_STATE_BEGIN_STRING:
		case E_FIXED_STRING_LIST_PARSE_STATE_BEGIN_STRING_PRE:
		{
			rnBeginPosition = nPosition;

			return E_FIXED_STRING_LIST_PARSE_STATE_IN_STRING;
		}

		case E_FIXED_STRING_LIST_PARSE_STATE_BEGIN_TEXT:
		{
			rnBeginPosition = nPosition + 1;

			return E_FIXED_STRING_LIST_PARSE_STATE_IN_TEXT;
		}

		default:
		{
			Assert( !"Invalid State" );

			return E_FIXED_STRING_LIST_PARSE_STATE_NONE;
		}
	}
}


//*****************************************************************************
//
// End of File : CFixedStringListA.cpp
//
//*****************************************************************************