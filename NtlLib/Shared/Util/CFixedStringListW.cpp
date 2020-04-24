//*****************************************************************************
// File       : CFixedStringListW.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "VersionLog.h"

#include "CFixedStringW.h"

#include "CFixedStringListW.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( str )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

const SVersionInfo CFixedStringListW::VERSION( _VERSION_MAJOR_, _VERSION_MINOR_, _VERSION_BUILD_, _VERSION_LABEL_ );

const UInt32 CFixedStringListW::POSITION_FRONT		= C_STRING_LIST_POSITION_FRONT;
const UInt32 CFixedStringListW::POSITION_BACK		= C_STRING_LIST_POSITION_BACK;

CFixedStringW CFixedStringListW::SEPARATORS			= CFixedStringW( FIXED_STRING_SEPARATORS_DEFAULT_W );
CFixedStringW CFixedStringListW::PRE_SEPARATORS		= CFixedStringW( L"<" );
CFixedStringW CFixedStringListW::POST_SEPARATORS	= CFixedStringW( L">" );

CFixedStringW CFixedStringListW::TEXT_INDICATORS	= CFixedStringW( FIXED_STRING_TEXT_INDICATOR_DEFAULT_W );
CFixedStringW CFixedStringListW::COMMENT_INDICATOR	= CFixedStringW( FIXED_STRING_COMMENT_INDICATOR_DEFAULT_W );


//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringListW::Initialize( void )
{
	m_ptList = new TFixedStringListW;

	m_tCurrentPosition = m_ptList->end();

	m_nStringCount = 0;

	return;
}


//-----------------------------------------------------------------------------
// Name			: ~CFixedStringListW
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedStringListW::~CFixedStringListW( void )
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

const CFixedStringW * const CFixedStringListW::Get( void ) const
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

CFixedStringW * CFixedStringListW::GetAt( UInt32 nPosition ) const
{
	if( nPosition > m_nStringCount )
	{
		nPosition = m_nStringCount;
	}

	TFixedStringListIteratorW	tPosition = m_ptList->begin();

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
// Name			: GetPrevIterator
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

TFixedStringListIteratorW CFixedStringListW::GetPrevIterator( TFixedStringListIteratorW tSearchPosition ) const
{
	TFixedStringListIteratorW	tPosition = m_ptList->begin();
	TFixedStringListIteratorW	tPrevPositon = tPosition;

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

void CFixedStringListW::Push( const CFixedStringW * const pcString )
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

void CFixedStringListW::PushFast( CFixedStringW * const pcString )
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

void CFixedStringListW::PushAt( const CFixedStringW * const pcString, UInt32 nPosition )
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

void CFixedStringListW::PushAtFast( CFixedStringW * const pcString, UInt32 nPosition )
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
				TFixedStringListIteratorW	tPosition = m_ptList->begin();

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

CFixedStringW * CFixedStringListW::Pop( bool bDestroy )
{
	if( m_tCurrentPosition == m_ptList->end() )
	{
		Assert( !"Can't Pop at End Position" );

		return NULL;
	}

	TFixedStringListIteratorW	tNextPosition = m_tCurrentPosition;
	++tNextPosition;

	m_ptList->erase( m_tCurrentPosition );

	CFixedStringW * pcReturn = *m_tCurrentPosition;
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

CFixedStringW * CFixedStringListW::PopAt( UInt32 nPosition, bool bDestroy )
{
	TFixedStringListIteratorW	tPosition;
	TFixedStringListIteratorW	tNextPosition = m_tCurrentPosition;
	++tNextPosition;

	switch( nPosition )
	{
		case POSITION_FRONT:
		{
			tPosition = m_ptList->begin();

			if( tPosition == m_tCurrentPosition )
				++m_tCurrentPosition;

			m_ptList->pop_front();

			break;
		}

		case POSITION_BACK:
		{
			tPosition = GetPrevIterator( m_ptList->end() );

			if( tPosition == m_tCurrentPosition )
				m_tCurrentPosition = m_ptList->end();

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
					++m_tCurrentPosition;

				m_ptList->erase( tPosition );
			}
		}
	}

	m_tCurrentPosition = tNextPosition;

	CFixedStringW * pcReturn = *tPosition;

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

void CFixedStringListW::PopAll( bool bDestroy )
{
	if( bDestroy == true )
	{
		CFixedStringW *		pcString;

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

bool CFixedStringListW::MoveFront( void )
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

bool CFixedStringListW::MoveBack( void )
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

bool CFixedStringListW::MovePrev( void )
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

bool CFixedStringListW::MoveNext( void )
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

bool CFixedStringListW::MoveAt( UInt32 nPosition )
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

size_t CFixedStringListW::GetBufferSize( void ) const
{
	TFixedStringListIteratorW	tPosition = m_ptList->begin();
	size_t						nBufferSize = 0;
	CFixedStringW *				pcString;

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

UInt32 CFixedStringListW::FindEqual( const CFixedStringW & rcString, UInt32 nBeginIndex, UInt32 nEndIndex ) const
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

UInt32 CFixedStringListW::FindEquivalent( const CFixedStringW & rcString, UInt32 nBeginIndex, UInt32 nEndIndex ) const
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

size_t CFixedStringListW::CreateFrom( const CFixedStringW & rcBuffer, size_t nLetterCount )
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

	wchar_t *	pchTempBuffer = new wchar_t[nLetterCount + 1];
	memcpy( pchTempBuffer, rcBuffer.GetBuffer(), nLetterCount * sizeof( wchar_t ) );
	pchTempBuffer[nLetterCount] = NULL_CHAR;

	size_t		nResultCount = ParseAndCreate( pchTempBuffer, nLetterCount );

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

size_t CFixedStringListW::CreateFromFile( const CFixedStringW & rcFileName, size_t nLetterCount )
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

size_t CFixedStringListW::CreateFromFile( const wchar_t * const pchFileName, size_t nLetterCount )
{
	HANDLE	hFile = ::CreateFileSafeW( pchFileName, GENERIC_READ );
	if( hFile == INVALID_HANDLE_VALUE )
	{
		return 0;
	}

	size_t	nResultSize = CreateFromFile( hFile, nLetterCount );

	CloseHandle( hFile );

	return nResultSize;
}


//-----------------------------------------------------------------------------
// Name			: CreateFrom
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

size_t CFixedStringListW::CreateFrom( const wchar_t * const pchBuffer, size_t nLetterCount )
{
	Assert( pchBuffer != NULL );

	if( nLetterCount == 0 )
	{
		nLetterCount = wcslen( pchBuffer );
		if( nLetterCount == 0 )
		{
			return 0;
		}
	}

	wchar_t *	pchTempBuffer = new wchar_t[nLetterCount + 1];
	memcpy( pchTempBuffer, pchBuffer, nLetterCount * sizeof( wchar_t ) );
	pchTempBuffer[nLetterCount] = NULL_CHAR;

	size_t		nResultCount = ParseAndCreate( pchTempBuffer, nLetterCount );

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

size_t CFixedStringListW::CreateFromFile( HANDLE hFile, size_t nLetterCount )
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

	wchar_t *	pchBuffer;
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

		pchBuffer = new wchar_t[nLetterCount + 1];
		Assert( pchBuffer != NULL );
		pchBuffer[nLetterCount] = NULL_CHAR;

		SetFilePointer( hFile, 2, NULL, FILE_BEGIN );
		if( ReadFile( hFile, pchBuffer, static_cast<DWORD>(nLetterCount * 2), &dwSizeRead, NULL ) == FALSE )
		{
			delete[] pchBuffer;

			return 0;
		}
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

		nLetterCount = MultiByteToWideChar( CP_UTF8, CFixedStringW::CONVERSION_FLAGS,
											pchTempBuffer, -1,
											NULL, 0 );
		Assert( nLetterCount != 0 );

		pchBuffer = new wchar_t[nLetterCount + 1];
		Assert( pchBuffer != NULL );
		pchBuffer[nLetterCount] = NULL_CHAR;

		nLetterCount = MultiByteToWideChar( CP_UTF8, CFixedStringW::CONVERSION_FLAGS,
											pchTempBuffer, -1,
											pchBuffer, static_cast<int>(nLetterCount) );
		Assert( nLetterCount != 0 );

		delete[] pchTempBuffer;
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

		char *	pchTempBuffer = new char[nLetterCount + 1];
		Assert( pchTempBuffer != NULL );
		pchTempBuffer[nLetterCount] = NULL_CHAR;

		SetFilePointer( hFile, 0, NULL, FILE_BEGIN );
		if( ReadFile( hFile, pchTempBuffer, static_cast<DWORD>(nLetterCount), &dwSizeRead, NULL ) == FALSE )
		{
			delete[] pchTempBuffer;

			return 0;
		}

		nLetterCount = MultiByteToWideChar( CP_ACP, CFixedStringW::CONVERSION_FLAGS,
											pchTempBuffer, -1,
											NULL, 0 );
		Assert( nLetterCount != 0 );

		pchBuffer = new wchar_t[nLetterCount + 1];
		Assert( pchBuffer != NULL );
		pchBuffer[nLetterCount] = NULL_CHAR;

		nLetterCount = MultiByteToWideChar( CP_ACP, CFixedStringW::CONVERSION_FLAGS,
											pchTempBuffer, -1,
											pchBuffer, static_cast<int>(nLetterCount) );
		Assert( nLetterCount != 0 );

		delete[] pchTempBuffer;
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

size_t CFixedStringListW::Serialize( CFixedStringW & rcBuffer, wchar_t chSeparator ) const
{
	size_t		nLetterCount = 0;
	size_t		nSizeofSeparator = ( ( chSeparator == '\0' ) ? 0 : sizeof( wchar_t ) );
	TFixedStringListIteratorW	tPosition = m_ptList->begin();
	CFixedStringW *		pcString;

	while( tPosition != m_ptList->end() )
	{
		pcString = *tPosition;

		nLetterCount += pcString->GetLength();
		nLetterCount += nSizeofSeparator;								// for Separator

		++tPosition;
	}

	wchar_t *	pchBuffer = new wchar_t[nLetterCount + 1];
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

size_t CFixedStringListW::Serialize( wchar_t * const pchBuffer, size_t nLetterCount, wchar_t chSeparator ) const
{
	if( nLetterCount == 0 )
	{
		nLetterCount = wcslen( pchBuffer );
		Assert( nLetterCount > 0 );
	}

	size_t		nSizeofSeparator = ( ( chSeparator == '\0' ) ? 0 : sizeof( wchar_t ) );
	size_t		nLetterIndex = 0;
	TFixedStringListIteratorW	tPosition = m_ptList->begin();
	CFixedStringW *		pcString;
	bool	bEndWithSeparator = true;

	while( tPosition != m_ptList->end() )
	{
		pcString = *tPosition;

		if( pcString->GetLength() == 0 )
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

		if( nSizeofSeparator > 0 )
		{
			bEndWithSeparator = CFixedStringW::IsSeparator( pcString->GetAt( pcString->GetLength() - 1 ) );
		}

		++tPosition;
	}

	pchBuffer[nLetterIndex] = NULL_CHAR;

	return nLetterIndex * sizeof( wchar_t );
}


//-----------------------------------------------------------------------------
// Name			: Serialize
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

size_t CFixedStringListW::Serialize( HANDLE hFile, wchar_t chSeparator, bool bBOM ) const
{
	DWORD		dwWrittenSize;
	DWORD		dwTotalWrittenSize = 0;

	if( bBOM == true )
	{
		static const WORD	BOM = 0xFEFF;

		WriteFile( hFile, &BOM, sizeof( WORD ), &dwWrittenSize, NULL );
	}

	size_t		nSizeofSeparator = ( ( chSeparator == '\0' ) ? 0 : sizeof( wchar_t ) );
	TFixedStringListIteratorW	tPosition = m_ptList->begin();
	CFixedStringW *		pcString;
	bool	bEndWithSeparator = true;

	while( tPosition != m_ptList->end() )
	{
		pcString = *tPosition;

		if( WriteFile( hFile, pcString->GetBuffer(), static_cast<DWORD>(pcString->GetBufferSize()), &dwWrittenSize, NULL ) == FALSE )
		{
			dwWrittenSize = 0;
		}

		dwTotalWrittenSize += dwWrittenSize;

		if( bEndWithSeparator == false )
		{
			if( WriteFile( hFile, &chSeparator, static_cast<DWORD>(nSizeofSeparator), &dwWrittenSize, NULL ) == FALSE )
			{
				dwWrittenSize = 0;
			}
		}

		if( nSizeofSeparator > 0 )
		{
			bEndWithSeparator = CFixedStringW::IsSeparator( pcString->GetAt( pcString->GetLength() - 1 ) );
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

size_t CFixedStringListW::ParseAndCreate( wchar_t * const pchBuffer, size_t nBufferSize )
{
	EFixedStringListParseState	ePreviousState	= E_FIXED_STRING_LIST_PARSE_STATE_NONE;
	EFixedStringListParseState	eNextState		= E_FIXED_STRING_LIST_PARSE_STATE_NONE;

	wchar_t *	pchBufferPosition = pchBuffer;
	size_t		nBeginPosition = 0;
	size_t		nPosition = 0;

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

EFixedStringListParseState CFixedStringListW::DetermineCurrentState( EFixedStringListParseState & reCurrentState, wchar_t * pchPosition )
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
					( SEPARATORS.FindLetter( *pchPosition ) ) != CFixedStringW::SEARCH_FAIL )
			{
				reCurrentState = E_FIXED_STRING_LIST_PARSE_STATE_NONE;

				return E_FIXED_STRING_LIST_PARSE_STATE_NONE;
			}
			else if( TEXT_INDICATORS.FindLetter( *pchPosition ) != CFixedStringW::SEARCH_FAIL )
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
					( SEPARATORS.FindLetter( *pchPosition ) != CFixedStringW::SEARCH_FAIL ) )
			{
				reCurrentState = E_FIXED_STRING_LIST_PARSE_STATE_NONE;

				return E_FIXED_STRING_LIST_PARSE_STATE_END_STRING;
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
			else if( COMMENT_INDICATOR.IsEqual( pchPosition, COMMENT_INDICATOR.GetLength() ) == true )
			{
				reCurrentState = E_FIXED_STRING_LIST_PARSE_STATE_BEGIN_COMMENT;

				return E_FIXED_STRING_LIST_PARSE_STATE_END_STRING;
			}
			else if( TEXT_INDICATORS.FindLetter( *pchPosition ) != CFixedStringW::SEARCH_FAIL )
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
			else if( TEXT_INDICATORS.FindLetter( *pchPosition ) != CFixedStringW::SEARCH_FAIL )
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

bool CFixedStringListW::EndPreviousState( EFixedStringListParseState ePreviousState, wchar_t * const pchBuffer, size_t nLength )
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
			wchar_t		chTemp = pchBuffer[nLength];
			pchBuffer[nLength] = NULL_CHAR;

			CFixedStringW *		pNewString = new CFixedStringW( pchBuffer );

			pchBuffer[nLength] = chTemp;

			PushFast( pNewString );

			return true;
		}

		case E_FIXED_STRING_LIST_PARSE_STATE_END_STRING_POST:
		{
			wchar_t		chTemp = pchBuffer[nLength + 1];
			pchBuffer[nLength + 1] = NULL_CHAR;

			CFixedStringW *		pNewString = new CFixedStringW( pchBuffer );

			pchBuffer[nLength + 1] = chTemp;

			PushFast( pNewString );

			return true;
		}

		case E_FIXED_STRING_LIST_PARSE_STATE_END_TEXT:
		{
			wchar_t		chTemp = pchBuffer[nLength];
			pchBuffer[nLength] = NULL_CHAR;

			CFixedStringW *		pNewString = new CFixedStringW( pchBuffer );

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

EFixedStringListParseState CFixedStringListW::BeginNextState( EFixedStringListParseState eCurrentState, size_t & rnBeginPosition, size_t nPosition )
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
// End of File : CFixedStringListW.cpp
//
//*****************************************************************************