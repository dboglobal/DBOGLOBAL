//*****************************************************************************
// File       : CSerializer.cpp
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

#include "CSerializer.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CSerializer::Destroy( void )
{
	SWordNode *				pWordNodeToDestroyed;

	for( RwInt32 iIndex = 0; iIndex < m_iSize; ++iIndex )
	{
		pWordNodeToDestroyed = m_tList[iIndex];

		if( pWordNodeToDestroyed != NULL )
		{
			NTL_DELETE( pWordNodeToDestroyed );
			pWordNodeToDestroyed = NULL;
		}
	}

	Initialize();

	return;
}


//-----------------------------------------------------------------------------
// Name			: GetWord
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

const SWordNode * CSerializer::GetWord( RwInt32 iIndex )
{
	if( iIndex < 0 && iIndex >= m_iSize )
	{
		return NULL;
	}

	return m_tList[iIndex];
}


//-----------------------------------------------------------------------------
// Name			: LoadFromFile
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwInt32	CSerializer::LoadFromFile( TCHAR * pFilePath )
{
	HANDLE				hFile;

	Assert( pFilePath != NULL );

	hFile = CreateFileSafe( pFilePath, true );
	if( hFile == INVALID_HANDLE_VALUE )
	{
		Assert( !"CreateFile() Failure" );

		return 0L;
	}

	TCHAR *				lptTextBuffer;
	RwInt32				iFileSize;
	DWORD				dwReadSize;

	iFileSize = GetFileSize( hFile, NULL );
	if( iFileSize == 0L )
	{
		return 0L;
	}

	lptTextBuffer = new char[iFileSize + sizeof( RwChar )];
	if( lptTextBuffer == NULL )
	{
		return 0L;
	}

	ZeroMemory( lptTextBuffer, iFileSize + sizeof( RwChar ) );
	if( ReadFile( hFile, (void *)lptTextBuffer, iFileSize, &dwReadSize, (LPOVERLAPPED)NULL ) == false )
	{
		delete[] lptTextBuffer;

		return 0L;
	}

	CloseHandle( hFile );

	RwInt32		nCurrentSize = m_iSize;

	ParseText( lptTextBuffer, iFileSize );

	delete[] lptTextBuffer;

	return m_iSize - nCurrentSize;
}


//-----------------------------------------------------------------------------
// Name			: LoadFromString
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwInt32 CSerializer::LoadFromString( TCHAR * pString )
{
	Assert( pString != NULL );

	RwInt32		nCurrentSize = m_iSize;

	ParseText( pString, static_cast<RwInt32>(_tcslen( pString )) );

	return m_iSize - nCurrentSize;
}


//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CSerializer::Initialize( void )
{
	m_tList.reserve( 16384 );

	m_iSize = 0;

	return;
}


//-----------------------------------------------------------------------------
// Name			: ParseText
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CSerializer::ParseText( RwChar * pText, RwInt32 iTextSize )
{
	SParseState					ParseState;
	ParseState.eWordState		= E_WORD_STATE_NONE;
    ParseState.pText			= pText;
	ParseState.iTextSize		= iTextSize;
	ParseState.iParsePosition	= 0;
	ParseState.pParseWord		= NULL;
	ParseState.iLinePosition	= 1;
	ParseState.iWordPosition	= 1;
	ParseState.bFloat			= FALSE;

	if( iTextSize >= sizeof( WORD ) )
	{
		WORD	wHeader;

		memcpy(	&wHeader, pText, sizeof( WORD ) );

		if( wHeader == 0xFFFE || wHeader == 0xFEFF )
		{
			ParseState.iParsePosition += sizeof( WORD );
		}
	}

	while( ParseState.IsEndOfText() == FALSE )
	{
		if( DetermineState( &ParseState ) == FALSE )
		{
			return FALSE;
		}

		if( ProcessState( &ParseState ) == FALSE )
		{
			return FALSE;
		}

		++(ParseState.iParsePosition);
	}

	if( ProcessState( &ParseState ) == FALSE )
	{
		return FALSE;
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: DetermineState
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CSerializer::DetermineState( SParseState * pParseState )
{
	RwChar	CurrentChar = pParseState->GetCurrentChar();
	RwChar	NextChar = pParseState->GetCurrentChar();

	switch( pParseState->GetState() )
	{
		case E_WORD_STATE_NONE:
		{
			if( CurrentChar <= ' ' || ( CurrentChar == ',' ) )		// Control Character
			{
				break;
			}

			if( CurrentChar == '"' )		// Begin of String
			{
				pParseState->SetState( E_WORD_STATE_BEGIN_OF_STRING );

				break;
			}

			if( ( ( CurrentChar >= '0' ) && ( CurrentChar <= '9' ) )
				|| ( ( CurrentChar == '+' ) || ( CurrentChar == '-' ) ) )	// Begin of Value
			{
				pParseState->SetState( E_WORD_STATE_BEGIN_OF_VALUE );

				break;
			}

			if( CurrentChar == '/' &&  NextChar == '/' )
			{
				pParseState->SetState( E_WORD_STATE_BEGIN_OF_COMMENT );

				break;
			}

			pParseState->SetState( E_WORD_STATE_BEGIN_OF_WORD );

			break;
		}

		case E_WORD_STATE_IN_WORD:
		{
			if( ( CurrentChar <= ' ' ) || ( CurrentChar == ',' ) )		// Control Character
			{
				pParseState->SetState( E_WORD_STATE_END_OF_WORD );

				break;
			}

			break;
		}

		case E_WORD_STATE_IN_STRING:
		{
			if( CurrentChar == '"' )		// End of String
			{
				pParseState->SetState( E_WORD_STATE_END_OF_STRING );

				break;
			}

			break;
		}

		case E_WORD_STATE_IN_VALUE:
		{
			if( !( ( ( CurrentChar >= '0' ) && ( CurrentChar <= '9' ) )
				|| ( ( CurrentChar >= 'a' ) && ( CurrentChar <= 'f' ) )
				|| ( ( CurrentChar >= 'A' ) && ( CurrentChar <= 'F' ) )
				|| ( CurrentChar == 'x' ) || ( CurrentChar == 'X' )
				|| ( CurrentChar == '+' ) || ( CurrentChar == '-' ) || ( CurrentChar == '.' ) ) )
			{
				pParseState->SetState( E_WORD_STATE_END_OF_VALUE );
			}

			break;
		}

		case E_WORD_STATE_IN_COMMENT:
		{
			if( ( CurrentChar == '\n' ) || ( CurrentChar == '\0' ) )		// Control Character
			{
				pParseState->SetState( E_WORD_STATE_END_OF_COMMENT );

				break;
			}

			break;
		}

		default:
		{
			return FALSE;
		}
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: ProcessState
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CSerializer::ProcessState( SParseState * pParseState )
{
	RwChar	CurrentChar = pParseState->GetCurrentChar();

	switch( pParseState->GetState() )
	{
		case E_WORD_STATE_NONE:
		case E_WORD_STATE_IN_COMMENT:
		case E_WORD_STATE_IN_WORD:
		case E_WORD_STATE_IN_STRING:
		{
			if( CurrentChar == '\n' )
			{
				++(pParseState->iLinePosition);
				pParseState->iWordPosition = 0;
			}

			break;
		}

		case E_WORD_STATE_IN_VALUE:
		{
			if( CurrentChar == '.' )
			{
				pParseState->bFloat = TRUE;
			}

			break;
		}

		case E_WORD_STATE_BEGIN_OF_COMMENT:
		{
			pParseState->SetState( E_WORD_STATE_IN_COMMENT );

			break;
		}

		case E_WORD_STATE_BEGIN_OF_WORD:
		{
			pParseState->SetBegin();

			pParseState->SetState( E_WORD_STATE_IN_WORD );

			break;
		}

		case E_WORD_STATE_BEGIN_OF_STRING:
		{
			pParseState->SetBegin();

			pParseState->SetState( E_WORD_STATE_IN_STRING );

			break;
		}

		case E_WORD_STATE_BEGIN_OF_VALUE:
		{
			pParseState->SetBegin();

			pParseState->bFloat = FALSE;

			pParseState->SetState( E_WORD_STATE_IN_VALUE );

			break;
		}

		case E_WORD_STATE_END_OF_COMMENT:
		{
			if( CurrentChar == RWSTRING( '\n' ) )
			{
				++(pParseState->iLinePosition);
				pParseState->iWordPosition = 0;
			}

			pParseState->SetState( E_WORD_STATE_NONE );

			break;
		}

		case E_WORD_STATE_END_OF_WORD:
		{
			pParseState->SetEnd();

			RwChar TempChar = RWSTRING( '\0' );
			Swap( pParseState->pText[pParseState->iEndPosition], TempChar );

			pParseState->pParseWord = NTL_NEW( SWordNode );
			pParseState->pParseWord->Create( &pParseState->pText[pParseState->iBeginPosition] );

			Swap( pParseState->pText[pParseState->iEndPosition], TempChar );

			++(pParseState->iWordPosition);
			if( CurrentChar == RWSTRING( '\n' ) )
			{
				++(pParseState->iLinePosition);
				pParseState->iWordPosition = 0;
			}

			AddWordNode( pParseState->pParseWord );

			pParseState->SetState( E_WORD_STATE_NONE );

			break;
		}

		case E_WORD_STATE_END_OF_STRING:
		{
			pParseState->SetEnd();

			RwChar TempChar = '\0';
			Swap( pParseState->pText[pParseState->iEndPosition], TempChar );

			pParseState->pParseWord = NTL_NEW( SWordNode );
			pParseState->pParseWord->Create( &pParseState->pText[pParseState->iBeginPosition + 1] );

			Swap( pParseState->pText[pParseState->iEndPosition], TempChar );

			++(pParseState->iWordPosition);
			if( CurrentChar == RWSTRING( '\n' ) )
			{
				++(pParseState->iLinePosition);
				pParseState->iWordPosition = 0;
			}

			AddWordNode( pParseState->pParseWord );

			pParseState->SetState( E_WORD_STATE_NONE );

			break;
		}

		case E_WORD_STATE_END_OF_VALUE:
		{
			pParseState->SetEnd();

			RwBool		bNegative;

			if( pParseState->GetBeginChar()  == '-' )
			{
				bNegative = TRUE;

				++pParseState->iBeginPosition;
			}
			else if( pParseState->GetBeginChar() == '+' )
			{
				bNegative = FALSE;

				++pParseState->iBeginPosition;
			}
			else
			{
				bNegative = FALSE;
			}

			if( pParseState->bFloat != FALSE )							// Float Type
			{
				RwInt32		nValue = 0;
				RwInt32		iIndex;
				RwChar		Char;

				for( iIndex = pParseState->iBeginPosition; iIndex < pParseState->iEndPosition; ++iIndex )
				{
					Char = pParseState->pText[iIndex];

					if( Char == '.' )
					{
						break;
					}
					else
					{
						nValue = nValue * 10 + ( Char -  '0' );
					}
				}

				RwReal		fValue = 0.0f;

				for( iIndex = pParseState->iEndPosition - 1; iIndex >= pParseState->iBeginPosition; --iIndex )
				{
					Char = pParseState->pText[iIndex];

					if( Char == '.' )
					{
						break;
					}
					else
					{
						fValue = fValue * 0.1f + static_cast<float>( Char -  '0' ) * 0.1f;
					}
				}

				fValue += (float)nValue;

				if( bNegative != FALSE )
				{
					fValue = -fValue;
				}

				pParseState->pParseWord = NTL_NEW( SWordNode );
				pParseState->pParseWord->Create( fValue );
			}
			else if( pParseState->GetBeginChar() == '0'
				&& ( pParseState->GetChar( pParseState->iBeginPosition + 1 ) == 'x' || pParseState->GetChar( pParseState->iBeginPosition + 1 ) == 'X' ) )
			{
				RwInt32		nValue = 0;
				RwInt32		iIndex;
				RwChar		Char;

				for( iIndex = pParseState->iBeginPosition + 2; iIndex < pParseState->iEndPosition; ++iIndex )
				{
					Char = pParseState->GetChar( iIndex );

					if( ( Char >= 'a' ) && ( Char <= 'f' ) )
					{
						nValue = ( nValue << 4 ) + ( Char - 'a' ) + 10;
					}	
					else if( ( Char >= 'A' ) && ( Char <= 'F' ) )
					{
						nValue = ( nValue << 4 ) + ( Char - 'A' ) + 10;
					}
					else if( ( Char >= '0' ) && ( Char <= '9' ) )
					{
						nValue = ( nValue << 4 ) + ( Char - '0' );
					}
					else
					{
						return FALSE;
					}
				}

				pParseState->pParseWord = NTL_NEW( SWordNode );
				pParseState->pParseWord->Create( nValue );
			}
			else
			{
				RwInt32		nValue = 0;
				RwInt32		iIndex;
				RwChar		Char;

				for( iIndex = pParseState->iBeginPosition; iIndex < pParseState->iEndPosition; ++iIndex )
				{
					Char = pParseState->GetChar( iIndex );

					nValue = nValue * 10 + ( Char -  '0' );
				}

				if( bNegative != FALSE )
				{
					nValue = -nValue;
				}

				pParseState->pParseWord = NTL_NEW( SWordNode );
				pParseState->pParseWord->Create( nValue );
			}

			++(pParseState->iWordPosition);
			if( CurrentChar == RWSTRING( '\n' ) )
			{
				++(pParseState->iLinePosition);
				pParseState->iWordPosition = 0;
			}

			AddWordNode( pParseState->pParseWord );

			pParseState->SetState( E_WORD_STATE_NONE );

			break;
		}

		default:
		{
			return FALSE;
		}
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: AddWordNode
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CSerializer::AddWordNode( SWordNode * pNode )
{
	m_tList.insert( m_tList.begin() + m_iSize, pNode );

	++m_iSize;

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: SearchString
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwInt32 CSerializer::SearchString( RwChar * pString )
{
	Assert( pString != NULL );

	SWordNode *				pWordNodeToSearch;

	for( RwInt32 iIndex = 0; iIndex < m_iSize; ++iIndex )
	{
		pWordNodeToSearch = m_tList[iIndex];

		if( pWordNodeToSearch != NULL &&
			pWordNodeToSearch->IsString() &&
			rwstrcmp( pWordNodeToSearch->GetString(), pString ) == 0 )
		{
			return iIndex;
		}
	}

	return -1;
}


//-----------------------------------------------------------------------------
// Name			: SearchStringAndGetWord
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

const SWordNode * CSerializer::SearchStringAndGetWord( RwChar * pString )
{
	Assert( pString != NULL );

	TWordNodeListIterator	tIterator = m_tList.begin();

	while( tIterator != m_tList.end() )
	{
		Assert( *tIterator != NULL );

		if( (*tIterator)->IsString() == FALSE )
		{
			++tIterator;

			continue;
		}

		const RwChar * pStr = (*tIterator)->GetString();

		if( _tcscmp( pStr, pString ) == 0 )
		{
			++tIterator;

			if( tIterator == m_tList.end() )
			{
				return NULL;
			}

			return (*tIterator);
		}

		++tIterator;
	}

	return NULL;
}


//-----------------------------------------------------------------------------
// Name			: SearchStringAndGetString
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

const RwChar * CSerializer::SearchStringAndGetString( RwChar * pString )
{
	const SWordNode *	pWord = SearchStringAndGetWord( pString );

	if( pWord == NULL )
	{
		return NULL;
	}

	if( pWord->IsString() == FALSE )
	{
		return NULL;
	}

	return pWord->GetString();
}


//-----------------------------------------------------------------------------
// Name			: SearchStringAndGetInteger
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwInt32 CSerializer::SearchStringAndGetInteger( RwChar * pString )
{
	const SWordNode *	pWord = SearchStringAndGetWord( pString );

	if( pWord == NULL )
	{
		return 0;
	}

	if( pWord->IsInteger() != FALSE )
	{
		return pWord->GetInteger();
	}
	else if( pWord->IsFloat() != FALSE )
	{
		return static_cast<RwInt32>(pWord->GetFloat());
	}
	else
	{
		return 0.0f;
	}
}


//-----------------------------------------------------------------------------
// Name			: SearchStringAndGetInteger
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwReal CSerializer::SearchStringAndGetFloat( RwChar * pString )
{
	const SWordNode *	pWord = SearchStringAndGetWord( pString );

	if( pWord == NULL )
	{
		return 0.0f;
	}

	if( pWord->IsFloat() != FALSE )
	{
		return pWord->GetFloat();
	}
	else if( pWord->IsInteger() != FALSE )
	{
		return static_cast<RwReal>(pWord->GetInteger());
	}
	else
	{
		return 0.0f;
	}
}


//-----------------------------------------------------------------------------
// Name			: Report
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwInt32 CSerializer::Report( LPSTR pFilePath )
{
	DWORD				dwWriteSize = 0L;
	HANDLE				hFile;
	RwInt32				iIndex;
	const SWordNode *	pWord;
	char				szTextBuffer[MAX_DESC_LENGTH];


	if( pFilePath == NULL )
	{
		return -1;
	}

	if( m_iSize == 0 )
	{
		return -1;
	}

	hFile = CreateFile( pFilePath, GENERIC_WRITE, FILE_SHARE_READ,
						(LPSECURITY_ATTRIBUTES)NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	if( hFile == INVALID_HANDLE_VALUE )
	{
		return -1;
	}

	for( iIndex = 0; iIndex < m_iSize; ++iIndex )
	{
		pWord = m_tList[iIndex];

		if( pWord == NULL )
		{
			CloseHandle( hFile );

			return -1;
		}

		switch( pWord->GetType() )
		{
			case E_WORD_TYPE_STRING:
			{
				sprintf( szTextBuffer, "Index %4d, String : %s\r\n", iIndex, pWord->GetString() );

				if( WriteFile( hFile, (LPVOID)szTextBuffer, static_cast<DWORD>(strlen( szTextBuffer )), &dwWriteSize, (LPOVERLAPPED)NULL ) == false )
				{
					CloseHandle( hFile );

					return -1;
				}

				continue;
			}

			case E_WORD_TYPE_INTEGER:
			{
				sprintf( szTextBuffer, "Index %4d, Integer : %d\r\n", iIndex, pWord->GetInteger() );

				if( WriteFile( hFile, (LPVOID)szTextBuffer, static_cast<DWORD>(strlen( szTextBuffer )), &dwWriteSize, (LPOVERLAPPED)NULL ) == false )
				{
					CloseHandle( hFile );

					return -1;
				}

				continue;
			}

			case E_WORD_TYPE_FLOAT:
			{
				sprintf( szTextBuffer, "Index %4d, Float : %f\r\n", iIndex, pWord->GetFloat() );

				if( WriteFile( hFile, (LPVOID)szTextBuffer, static_cast<DWORD>(strlen( szTextBuffer )), &dwWriteSize, (LPOVERLAPPED)NULL ) == false )
				{
					CloseHandle( hFile );

					return -1;
				}

				continue;
			}

			default:
			{
				sprintf( szTextBuffer, "Index %4d, Undefined\r\n", iIndex );

				if( WriteFile( hFile, (LPVOID)szTextBuffer, static_cast<DWORD>(strlen( szTextBuffer )), &dwWriteSize, (LPOVERLAPPED)NULL ) == false )
				{
					CloseHandle( hFile );

					return -1;
				}
			}
		}
	}

	CloseHandle( hFile );

	return iIndex;
}


//*****************************************************************************
//
// End of File : CSerializer.cpp
//
//*****************************************************************************