//*****************************************************************************
// File       : CEditorApplication_File.cpp
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

#include "CSceneManager.h"
#include "CFixedStringList.h"
#include "CSerializer.h"
#include "CEntityCommand.h"

#include "CEditorApplication.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

CFixedString CEditorApplication::m_AppPath	= CFixedString( NULL );
CFixedString CEditorApplication::m_FilePath	= CFixedString( _T( "world\\structure" ) );
CFixedString CEditorApplication::m_FileExt	= CFixedString( _T( "prj" ) );


//-----------------------------------------------------------------------------
// Name			: OnFileNew
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::OnFileNew( void )
{
	SRasterInfo		sRasterInfo;
	sRasterInfo.Set( g_pSceneManager->GetRasterInfo() );

	g_pSceneManager->Destroy();
	g_pSceneManager->Create( &sRasterInfo );

	RwChar			szDefaultFilePath[256];
	rwsprintf( szDefaultFilePath, RWSTRING( "%s\\default.prj" ), m_FilePath.GetBuffer() );
	OnFileLoad( szDefaultFilePath );
/*
	m_Name.Assign( RWSTRING( "default" ) );

	m_pSceneManager->SetName( RWSTRING( "field" ) );

	RwChar		szFileName[256];
	rwsprintf( szFileName, RWSTRING( "%s\\%s\\%s.%s" ), m_FilePath.GetBuffer(), GetName()->GetBuffer(), m_pSceneManager->GetName()->GetBuffer(), m_pSceneManager->GetFileExt()->GetBuffer() );

//	m_pSceneManager->ExecuteCommand( RWSTRING( "#create defworld 0x50000000 0 0 world field #custom -500.0 -500.0 -500.0 500.0 500.0 500.0 -2 0 -2 1 0 1\n" ) );
//	m_pSceneManager->ExecuteCommand( RWSTRING( "#create defworld 0x50000000 0 0 world world -50000.0 -5000.0 -50000.0 50000.0 5000.0 50000.0 0 0 0 0 0 0\n" ) );

	m_pSceneManager->ExecuteCommand( RWSTRING( "#create 0x40000000 0x50000000 0 camera #base 0.000000 100.000000 0.000000 0.000000 45.000000 0.0 1.0 1000.0 0x80 0x80 0x80 0xFF\n" ) );
	m_pSceneManager->ExecuteCommand( RWSTRING( "#create 0x80000001 0x40000000 0 fog exponentialsquered 1.000000 1.000000 1.000000 1.000000 5.0 1.0\n" ) );

	m_pSceneManager->ExecuteCommand( RWSTRING( "#create 0x30000000 0x50000000 0 light ambient 0.200000 0.200000 0.20000 1.000000 0.0 0.0 0.0\n" ) );
	m_pSceneManager->ExecuteCommand( RWSTRING( "#create 0x30000001 0x50000000 0 light directional 0.600000 0.600000 0.600000 1.000000 0.0 45.0 0.0\n" ) );
//	m_pSceneManager->ExecuteCommand( RWSTRING( "#create 0x30000102 0x50000000 0 light point 0.000000 0.000000 1.000000 1.000000 10.0 20.0 10.0 200.0\n" ) );
//	m_pSceneManager->ExecuteCommand( RWSTRING( "#create 0x30000103 0x50000000 0 light spot 0.600000 0.000000 0.000000 1.000000 10.0 20.0 10.0 0.0 90.0 0.0 100.0 100.0\n" ) );
	m_pSceneManager->ExecuteCommand( RWSTRING( "#create 0x60000001 0x40000000 0 sky\n" ) );
*/
	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: OnFileLoad
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::OnFileLoad( RwChar * pName )
{
	CSerializer			Serializer;
	RwBool				bResult;
	RwInt32				iIndex = 0;
	const SWordNode *	pWord;
	RwChar				szFileName[256];

	SRasterInfo			sRasterInfo;
	sRasterInfo.Set( g_pSceneManager->GetRasterInfo() );

	g_pSceneManager->Destroy();
	g_pSceneManager->Create( &sRasterInfo );

	bResult = Serializer.LoadFromFile( pName );

	pWord = Serializer.GetWord( iIndex );
	m_Name.Assign( pWord->GetString() );

	pWord = Serializer.GetWord( ++iIndex );
	m_pSceneManager->SetName( pWord->GetString() );

	rwsprintf( szFileName, RWSTRING( "%s\\%s\\%s.%s" ), m_FilePath.GetBuffer(), GetName()->GetBuffer(), m_pSceneManager->GetName()->GetBuffer(), m_pSceneManager->GetFileExt()->GetBuffer() );
	Serializer.Destroy();

	iIndex = 0;
	bResult = Serializer.LoadFromFile( szFileName );

	if( bResult == FALSE )
	{
		return OnFileNew();
	}

	while( bResult != FALSE )
	{
		CEntityCommand *	pCommand = CEntityCommand::CreateCommand( &Serializer, iIndex );
		CEntity *			pEntity;
		if( pCommand != NULL )
		{
			bResult = pCommand->Execute();
			if( bResult != FALSE )
			{
				pEntity = reinterpret_cast<CEntity *>(pCommand->GetResultData());

			}
			else
			{
				Assert( !"pCommand->Execute() Fail" );

				bResult = FALSE;
			}

			//++iIndex;

			CEntityCommand::DestroyCommand( &pCommand );
		}
		else
		{
	//		Assert( !"CEntityCommand::CreateCommand() Fail" );

			break;
		}
	}

	Serializer.Destroy();

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: OnFileSave
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::OnFileSave( RwChar * pName )
{
	CFixedStringList	StringList;
	CFixedString		ProjectName( pName );

	if( pName == NULL )
	{
		ProjectName.Assign( *GetName() );
	}

	StringList.Push( &ProjectName );

	SetName( &ProjectName );
	m_pSceneManager->Export( &StringList, &ProjectName );

	RwChar			szFileName[256];
	rwsprintf( szFileName, RWSTRING( "%s\\%s.%s" ), m_FilePath.GetBuffer(), ProjectName.GetBuffer(), m_FileExt.GetBuffer() );

	HANDLE			hFile = CreateFileSafe( szFileName, false );
	if( hFile == INVALID_HANDLE_VALUE )
	{
		Assert( !"CreateFileSafe() Failure" );

		return FALSE;
	}

	StringList.Serialize( hFile );

	CloseHandle( hFile );

	return TRUE;
}


//*****************************************************************************
//
// End of File : CEditorApplication_File.cpp
//
//*****************************************************************************