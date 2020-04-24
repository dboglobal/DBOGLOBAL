//*****************************************************************************
// File       : CEntityCommand.cpp
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
#include "CFixedStringList.h"
#include "CSerializer.h"

#include "CEntity.h"

#include "CEntityCommand.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

RwBool CEntityCommand::m_bInParsing		= FALSE;
RwBool CEntityCommand::m_bInWaiting		= FALSE;
RwBool CEntityCommand::m_bInExecuting	= FALSE;


//-----------------------------------------------------------------------------
// Name			: Parse
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCommand::Parse( RwChar * pCommandStringSrc, EEntityCommandCategory eCategory )
{
	Assert( pCommandStringSrc != NULL );

	//pCommandString = _tcslwr( _tcsdup( pCommandString ) );
	RwChar * pCommandString = NTL_NEW RwChar[rwstrlen( pCommandStringSrc ) + 1];
	memcpy( pCommandString, pCommandStringSrc, rwstrlen( pCommandStringSrc ) );
	pCommandString[rwstrlen( pCommandStringSrc )] = '\0';

	CSerializer *	pSerializer = NTL_NEW( CSerializer );
	Assert( pSerializer != NULL );

	RwBool			bResult;

	bResult = pSerializer->LoadFromString( pCommandString );
	if( bResult == FALSE )
	{
		Assert( !"LoadFromString() Fail" );

		NTL_DELETE( pSerializer );
		NTL_ARRAY_DELETE( pCommandString );

		return FALSE;
	}

	RwInt32		iIndex = 0;

	bResult = Parse( pSerializer, iIndex, eCategory );
	if( bResult == FALSE )
	{
		Assert( !"Parse() Fail" );
	}

	NTL_DELETE( pSerializer );
	NTL_ARRAY_DELETE( pCommandString );

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: Parse
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCommand::Parse( CSerializer * pSerializer, RwInt32 & riPosition, EEntityCommandCategory eCategory )
{
	if( m_bInParsing != FALSE || m_bInWaiting != FALSE )//|| m_bInExecuting != FALSE )
	{
		Assert( !"Locked Command" );

		return FALSE;
	}

	m_bInParsing = TRUE;

	m_eCategory	= eCategory;

	const SWordNode *	pWord = NULL;
	RwBool				bHeaderFound = FALSE;

	while( pSerializer->GetSize() > riPosition )
	{
		pWord = pSerializer->GetWord( riPosition );

		if( pWord != NULL && pWord->IsString() != FALSE && (pWord->GetString())[0] == '#' )
		{
			bHeaderFound = TRUE;

			break;
		}

		++riPosition;
	}

	if( pWord == NULL || bHeaderFound == FALSE )
	{
		m_bInParsing = FALSE;

		m_eType = ENTITY_COMMAND_TYPE_ETCS;

//		Assert( !"Header was not Found" );

		return FALSE;
	}

	if( _tcscmp( pWord->GetString(), RWSTRING( "#create" ) ) == 0 )
	{
		if( ParseCreate( pSerializer, ++riPosition ) == FALSE )
		{
			m_bInParsing = FALSE;

			Assert( !"ParseCreate() Fail" );

			return FALSE;
		}
	}
	else if( _tcscmp( pWord->GetString(), RWSTRING( "#destroy" ) ) == 0 )
	{
		if( ParseDestroy( pSerializer, ++riPosition ) == FALSE )
		{
			m_bInParsing = FALSE;

			Assert( !"ParseDestroy() Fail" );

			return FALSE;
		}
	}
	else if( _tcscmp( pWord->GetString(), RWSTRING( "#move" ) ) == 0 )
	{
		if( ParseMove( pSerializer, ++riPosition ) == FALSE )
		{
			m_bInParsing = FALSE;

			Assert( !"ParseMove() Fail" );

			return FALSE;
		}
	}
	else if( _tcscmp( pWord->GetString(), RWSTRING( "#rotate" ) ) == 0 )
	{
		if( ParseRotate( pSerializer, ++riPosition ) == FALSE )
		{
			m_bInParsing = FALSE;

			Assert( !"ParseRotate() Fail" );

			return FALSE;
		}
	}
	else if( _tcscmp( pWord->GetString(), RWSTRING( "#scale" ) ) == 0 )
	{
		if( ParseScale( pSerializer, ++riPosition ) == FALSE )
		{
			m_bInParsing = FALSE;

			Assert( !"ParseScale() Fail" );

			return FALSE;
		}
	}
	else if( _tcscmp( pWord->GetString(), RWSTRING( "#sector" ) ) == 0 )
	{
		if( ParseSector( pSerializer, ++riPosition ) == FALSE )
		{
			m_bInParsing = FALSE;

			Assert( !"ParseSector() Fail" );

			return FALSE;
		}
	}
	else
	{
		//g_Log( const_cast<RwChar *>(pWord->GetString()) );

		Assert( !"Undefinded Command" );

		m_bInParsing = FALSE;

		return FALSE;
	}

	m_bInParsing = FALSE;
	m_bInWaiting = TRUE;

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityCommand::Destroy( void )
{
	if( m_pExtData != NULL )
	{
		NTL_DELETE( m_pExtData );
		m_pExtData = NULL;
	}

	if( m_pPriData != NULL )
	{
		NTL_DELETE( m_pPriData );
		m_pPriData = NULL;
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: Execute
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCommand::Execute( RwChar * pResultString )
{
	if( m_bInWaiting != FALSE )
	{
		m_bInWaiting = FALSE;
	}

	if( m_bInParsing != FALSE ||  m_bInWaiting != FALSE )//|| m_bInExecuting != FALSE )
	{
		Assert( !"Locked Command" );

		return FALSE;
	}

	m_bInExecuting = TRUE;

	RwBool		bResult;

	switch( m_eType )
	{
		case ENTITY_COMMAND_TYPE_CREATE:
		{
			bResult = ExecuteCommandCreate( pResultString );

			break;
		}

		case ENTITY_COMMAND_TYPE_DESTROY:
		{
			bResult = ExecuteCommandDestroy( pResultString );

			break;
		}

		case ENTITY_COMMAND_TYPE_MOVE:
		{
			bResult = ExecuteCommandMove( pResultString );

			break;
		}

		case ENTITY_COMMAND_TYPE_ROTATE:
		{
			bResult = ExecuteCommandRotate( pResultString );

			break;
		}

		case ENTITY_COMMAND_TYPE_SCALE:
		{
			bResult = ExecuteCommandScale( pResultString );

			break;
		}

		case ENTITY_COMMAND_TYPE_ETCS:
		{
			bResult = TRUE;

			break;
		}

		default:
		{
			Assert( !"Undefined Command Type" );

			bResult = FALSE;
		}
	}

	m_bInExecuting = FALSE;

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityCommand::Initialize( void )
{
	m_eType			= ENTITY_COMMAND_TYPE_NONE;
	m_eCategory		= ENTITY_COMMAND_CATEGORY_NONE;
	m_pPriData		= NULL;
	m_pExtData		= NULL;
	m_pResultData	= NULL;

	return;
}


//-----------------------------------------------------------------------------
// Name			: ParseFromFile
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCommand::ParseFromFile( RwChar * pFilePath )
{
	HANDLE				hFile;

	Assert( pFilePath != NULL );

	hFile = CreateFileSafe( pFilePath, true );

	if( hFile == INVALID_HANDLE_VALUE )
	{
		Assert( !"CreateFile() Failure" );

		return FALSE;
	}

	TCHAR *				lptTextBuffer;
	RwInt32				iFileSize;
	DWORD				dwReadSize;

	iFileSize = GetFileSize( hFile, NULL );
	if( iFileSize == 0L )
	{
		return FALSE;
	}

	lptTextBuffer = NTL_NEW char[iFileSize + sizeof( RwChar )];
	if( lptTextBuffer == NULL )
	{
		return FALSE;
	}

	ZeroMemory( lptTextBuffer, iFileSize + sizeof( RwChar ) );
	if( ReadFile( hFile, (void *)lptTextBuffer, iFileSize, &dwReadSize, (LPOVERLAPPED)NULL ) == false )
	{
		delete[] lptTextBuffer;

		return FALSE;
	}

	CloseHandle( hFile );

	RwInt32		iPosition = 0;

	while( lptTextBuffer[iPosition] != '\0' )
	{
		RwInt32		iBeginPosition = iPosition;

		while( lptTextBuffer[iPosition] != '\n' )
		{
			++iPosition;
		}

		RwChar		Char = '\0';
		Swap( lptTextBuffer[iPosition + 1], Char );

		CEntityCommand * pCommand = NTL_NEW( CEntityCommand );
		Assert( pCommand != NULL );

		if( pCommand->Parse( &lptTextBuffer[iBeginPosition] ) != FALSE )
		{
			if( pCommand->Execute() != FALSE )
			{
				//g_Log( &(lptTextBuffer[iBeginPosition]) );
			}
			else
			{
				Assert( !"Command->Execute() Fail" );
			}
		}
		else
		{
			Assert( !"Command->Parse() Fail" );
		}

		NTL_DELETE( pCommand );

		Swap( lptTextBuffer[iPosition + 1], Char );

		++iPosition;
	}

	NTL_ARRAY_DELETE( lptTextBuffer );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: CreateCommand
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntityCommand * CEntityCommand::CreateCommand( void )
{
	return NTL_NEW( CEntityCommand );
}


//-----------------------------------------------------------------------------
// Name			: CreateCommand
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntityCommand * CEntityCommand::CreateCommand( RwChar * pCommandString, EEntityCommandCategory eCategory )
{
	CEntityCommand *	pNewCommand = CEntityCommand::CreateCommand();

	if( pNewCommand->Parse( pCommandString, eCategory ) == FALSE )
	{
		NTL_DELETE( pNewCommand );

		return NULL;
	}

	return pNewCommand;
}


//-----------------------------------------------------------------------------
// Name			: CreateCommand
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntityCommand * CEntityCommand::CreateCommand( CSerializer * pSerializer, RwInt32 & riPosition, EEntityCommandCategory eCategory )
{
	CEntityCommand *	pNewCommand = CEntityCommand::CreateCommand();

	if( pNewCommand->Parse( pSerializer, riPosition, eCategory ) == FALSE )
	{
		NTL_DELETE( pNewCommand );

//		Assert( !"pNewCommand->Parse() Fail" );

		return NULL;
	}

	return pNewCommand;
}


//-----------------------------------------------------------------------------
// Name			: DestroyCommand
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityCommand::DestroyCommand( CEntityCommand ** ppEntityCommand )
{
	if( ppEntityCommand != NULL )
	{
		if( *ppEntityCommand != NULL )
		{
			NTL_DELETE( *ppEntityCommand );
			*ppEntityCommand = NULL;
		}
	}

	return;
}


//*****************************************************************************
//
// End of File : CEntityCommand.cpp
//
//*****************************************************************************