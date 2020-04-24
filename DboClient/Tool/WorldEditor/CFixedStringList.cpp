//*****************************************************************************
// File       : CFixedStringList.cpp
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

#include "CFixedStringList.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

const UINT CFixedStringList::POSITION_FRONT				= C_STRING_LIST_POSITION_FRONT;
const UINT CFixedStringList::POSITION_BACK				= C_STRING_LIST_POSITION_BACK;


//-----------------------------------------------------------------------------
// Name			: CFixedStringList
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedStringList::CFixedStringList( void )
{
	m_ptList = new TFixedStringList;

	m_tCurrentPosition = m_ptList->end();

	m_nCount = 0;

	return;
}


//-----------------------------------------------------------------------------
// Name			: ~CFixedStringList
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedStringList::~CFixedStringList( void )
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

const CFixedString * CFixedStringList::Get( void ) const
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

const CFixedString * CFixedStringList::GetAt( UINT nPosition ) const
{
	if( nPosition > m_nCount )
		nPosition = m_nCount;

	TFixedStringListIterator	tPosition = m_ptList->begin();

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

TFixedStringListIterator CFixedStringList::GetPrevIterator( TFixedStringListIterator tSearchPosition ) const
{
	TFixedStringListIterator	tPosition = m_ptList->begin();
	TFixedStringListIterator	tPrevPositon = tPosition;

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

void CFixedStringList::Push( CFixedString * lpcString )
{
	PushFast( lpcString->Clone() );

	return;
}


//-----------------------------------------------------------------------------
// Name			: PushFast
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringList::PushFast( CFixedString * lpcString )
{
	Assert( lpcString != NULL );

	m_ptList->insert( m_tCurrentPosition, lpcString );

	++m_nCount;
	Assert( m_nCount == m_ptList->size() );

	return;
}


//-----------------------------------------------------------------------------
// Name			: PushAt
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringList::PushAt( CFixedString * lpcString, UINT nPosition )
{
	PushAtFast( lpcString->Clone(), nPosition );

	return;
}


//-----------------------------------------------------------------------------
// Name			: PushAtFast
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringList::PushAtFast( CFixedString * lpcString, UINT nPosition )
{
	Assert( lpcString != NULL );

	switch( nPosition )
	{
		case POSITION_FRONT:
		{
			m_ptList->push_front( lpcString );

			break;
		}

		case POSITION_BACK:
		{
			m_ptList->push_back( lpcString );

			break;
		}

		default:
		{
			if( nPosition > m_nCount )
			{
				m_ptList->push_back( lpcString );
			}
			else
			{
				TFixedStringListIterator		tPosition = m_ptList->begin();

				while( nPosition > 0 )
				{
					++tPosition;
					--nPosition;
				}

				m_ptList->insert( tPosition, lpcString );
			}
		}
	}

	++m_nCount;
	Assert( m_nCount == m_ptList->size() );

	return;
}


//-----------------------------------------------------------------------------
// Name			: Pop
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedString * CFixedStringList::Pop( bool bDestroy )
{
	if( m_tCurrentPosition == m_ptList->end() )
	{
		Assert( !RWSTRING( "Can't Pop at End Position" ) );

		return NULL;
	}

	TFixedStringListIterator	tNextPosition = m_tCurrentPosition;
	++tNextPosition;

	m_ptList->erase( m_tCurrentPosition );

	CFixedString * lpcReturn = *m_tCurrentPosition;
	m_tCurrentPosition = tNextPosition;

	if( bDestroy == true )
	{
		delete lpcReturn;
		lpcReturn = NULL;
	}

	--m_nCount;
	Assert( m_nCount == m_ptList->size() );

	return lpcReturn;
}


//-----------------------------------------------------------------------------
// Name			: PopAt
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedString * CFixedStringList::PopAt( UINT nPosition, bool bDestroy )
{
	TFixedStringListIterator	tPosition;
	TFixedStringListIterator	tNextPosition = m_tCurrentPosition;
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
			if( nPosition >= m_nCount )
			{
				tPosition =  GetPrevIterator( m_ptList->end() );

				if( tPosition == m_tCurrentPosition )
					m_tCurrentPosition = m_ptList->end();

				m_ptList->pop_back();
			}
			else
			{
				TFixedStringListIterator	tPosition = m_ptList->begin();

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

	CFixedString * lpcReturn = *tPosition;

	if( bDestroy == true )
	{
		delete lpcReturn;
		lpcReturn = NULL;
	}

	--m_nCount;
	Assert( m_nCount == m_ptList->size() );

	return lpcReturn;
}


//-----------------------------------------------------------------------------
// Name			: PopAll
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringList::PopAll( bool bDestroy )
{
	if( bDestroy == true )
	{
		CFixedString *		pcString;

		m_tCurrentPosition = m_ptList->begin();

		while( m_tCurrentPosition != m_ptList->end() )
		{
			pcString = *m_tCurrentPosition;
			++m_tCurrentPosition;

			delete pcString;
		}
	}

	m_ptList->clear();
	m_nCount = 0;

	return;
}


//-----------------------------------------------------------------------------
// Name			: MoveFront
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CFixedStringList::MoveFront( void )
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

bool CFixedStringList::MoveBack( void )
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

bool CFixedStringList::MovePrev( void )
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

bool CFixedStringList::MoveNext( void )
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

bool CFixedStringList::MoveAt( UINT nPosition )
{
	if( nPosition >= m_nCount )
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

UINT CFixedStringList::GetBufferSize( void ) const
{
	TFixedStringListIterator	tPosition = m_ptList->begin();
	UINT						nBufferSize = 0;
	CFixedString *				lpcString;

	while( tPosition != m_ptList->end() )
	{
		lpcString = *tPosition;
		nBufferSize += lpcString->GetBufferSize();
		++tPosition;
	}

	return nBufferSize;
}


//-----------------------------------------------------------------------------
// Name			: Serialize
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

UINT CFixedStringList::Serialize( TCHAR * lptBuffer, UINT nBufferSize ) const
{
	UINT		nBufferIndex = 0;

	TFixedStringListIterator	tPosition = m_ptList->begin();
	CFixedString *				lpcString;

	while( tPosition != m_ptList->end() )
	{
		lpcString = *tPosition;

		if( nBufferIndex + lpcString->GetBufferSize() >= nBufferSize )
		{
			return 0;
		}

		memcpy( &(lptBuffer[nBufferIndex]), lpcString->GetBuffer(), lpcString->GetBufferSize() );
		nBufferIndex += lpcString->GetBufferSize();

		if( lpcString->IsEqual( RWSTRING( "\r\n" ) ) == FALSE )
		{
			memcpy( &(lptBuffer[nBufferIndex]), _RWSTRING( " \0" ), sizeof( RwChar ) * 2 );
			nBufferIndex += sizeof( RwChar );
		}

		++tPosition;
	}

	lptBuffer[nBufferIndex] = _T( '\0' );

	return nBufferIndex;
}


//-----------------------------------------------------------------------------
// Name			: Serialize
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

UINT CFixedStringList::Serialize( HANDLE hFile ) const
{
	UINT		nBufferSize = GetBufferSize() + m_nCount + 1;

	TCHAR *		ptBuffer = new TCHAR[nBufferSize];
	Assert( ptBuffer != NULL );
	ZeroMemory( ptBuffer, sizeof( TCHAR ) * nBufferSize );

	UINT		nWriteSize = Serialize( ptBuffer, nBufferSize );
	DWORD		dwWrittenSize = 0;

	if( nWriteSize > 0 )
	{
		if( WriteFile( hFile, ptBuffer, nWriteSize, &dwWrittenSize, NULL ) == FALSE )
		{
			dwWrittenSize = 0;
		}
	}

	delete[] ptBuffer;

	return dwWrittenSize;
}


//-----------------------------------------------------------------------------
// Name			: CreateString
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedString * CFixedStringList::CreateString( const TCHAR * lptRawString )
{
	CFixedString *	lpNewString = new CFixedString( lptRawString );

	return lpNewString;
}


//-----------------------------------------------------------------------------
// Name			: DestroyString
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFixedStringList::DestroyString( CFixedString ** pptString )
{
	Assert( pptString != NULL );
	Assert( *pptString != NULL );

	delete *pptString;
	*pptString = NULL;

	return;
}


//*****************************************************************************
//
// End of File : CFixedStringList.cpp
//
//*****************************************************************************