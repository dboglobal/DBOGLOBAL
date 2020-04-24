//*****************************************************************************
// File       : CEntityContainer.cpp
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

#include "CEntityContainer.h"
#include "CSceneManager.h"
#include "CEntityFactory.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityContainer::Initialize( void )
{
	m_iCount = 0;

	return;
}


//-----------------------------------------------------------------------------
// Name			: DestroyEntity
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityContainer::DestroyEntity( CEntity * pEntityToDestroyed )
{
	if( GetSceneManager() != NULL )
	{
		return GetSceneManager()->DestroyEntity( &pEntityToDestroyed );
	}
	else
	{
		g_pEntityFactory->DestroyEntity( &pEntityToDestroyed );

		return TRUE;
	}
}


//-----------------------------------------------------------------------------
// Name			: ReportState
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityContainer::ReportState( CLogMessageManager * pLogMessageManager, RwInt32 iTab )
{
	RwBool			bResult = TRUE;

	if( ReportSelfState( pLogMessageManager, iTab ) == FALSE )
	{
		bResult = FALSE;
	}

	if( ReportChildState( pLogMessageManager, iTab ) == FALSE )
	{
		bResult = FALSE;
	}

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: ReportSelfState
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityContainer::ReportSelfState( CLogMessageManager * pLogMessageManager, RwInt32 iTab )
{
	RwChar		szBuffer[256];
	rwsprintf( szBuffer, RWSTRING( "Count : %d" ), m_iCount );

	pLogMessageManager->SetLastLogMessageWithTab( szBuffer, iTab );

	// m_pProperty->ReportState( pLogMessageManager, iTab + 1 );

	return TRUE;
}


//*****************************************************************************
//
// End of File : CEntityContainer.cpp
//
//*****************************************************************************