//*****************************************************************************
// File       : CLightMapManager.cpp
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

#include "CEntity.h"

#include "CLightMapManager.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

CLightMapManager * CLightMapManager::m_pInstance	= NULL;


//-----------------------------------------------------------------------------
// Name			: CreateInstance
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CLightMapManager * CLightMapManager::CreateInstance( void )
{
	if( m_pInstance == NULL )
	{
		m_pInstance = NTL_NEW CLightMapManager;
	}

	return m_pInstance;
}


//-----------------------------------------------------------------------------
// Name			: DestroyInstance
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CLightMapManager::DestroyInstance( void )
{
	if( m_pInstance != NULL )
	{
		NTL_DELETE( m_pInstance );
		m_pInstance = NULL;
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: CLightMapManager
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CLightMapManager::CLightMapManager()
{
	Init();

	return;
}


//-----------------------------------------------------------------------------
// Name			: ~CLightMapManager
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CLightMapManager::~CLightMapManager()
{
	Destroy();

	return;
}


//-----------------------------------------------------------------------------
// Name			: Init
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CLightMapManager::Init( void )
{
	m_pAreaLightGroup	= NULL;
	m_pLightingSession	= NULL;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CLightMapManager::Create( CEntity * pWorld )
{
	BEGIN_FUNCTION( "CLightMapManager::Create" );

//	m_pLightingSession = NTL_NEW RtLtMapLightingSession;
//	RwCamera *			pCameraBackup = m_pLightingSession->camera;
//	m_pLightingSession->camera = NULL;

//	RtLtMapLightingSessionInitialize( m_pLightingSession, reinterpret_cast<RpWorld *>(pWorld->GetRwInstance()) );

//	RtLtMapLightMapsCreate( m_pLightingSession, 1.0f, (RwRGBA *)NULL );

//	m_pLightingSession->camera = pCameraBackup;

	END_FUNCTION( TRUE );
}


//-----------------------------------------------------------------------------
// Name			: Clear
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CLightMapManager::Clear( void )
{
	BEGIN_FUNCTION( "CLightMapManager::Destroy" );

	RtLtMapLightMapsClear( m_pLightingSession, (RwRGBA *) NULL);

	END_FUNCTION_VOID();
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CLightMapManager::Destroy( void )
{
	BEGIN_FUNCTION( "CLightMapManager::Destroy" );

//	RwCamera *			pCameraBackup = m_pLightingSession->camera;
//	m_pLightingSession->camera = NULL;

//	RtLtMapLightMapsDestroy( m_pLightingSession );

//	m_pLightingSession->camera = pCameraBackup;

	RwResourcesEmptyArena();

	END_FUNCTION_VOID();
}


RwBool CLightMapManager::RenderFrame( CEntity * pRenderer )
{
	m_pLightingSession->totalObj = 0;
	m_pLightingSession->numObj = 0;
	m_pLightingSession->camera = NULL;//(RwCamera *)pRenderer->GetRwInstance();

	RwCameraBeginUpdate( (RwCamera *)pRenderer->GetRwInstance() );
	RwCameraEndUpdate( (RwCamera *)pRenderer->GetRwInstance() );

	RtLtMapIlluminate( m_pLightingSession, NULL, 1.0f );

	return TRUE;
}


RpClump * ClumpPreLightClearAll128CB(RpClump *clump, void *data)
{
    RpClumpForAllAtomics(clump, AtomicPreLightClearAll128CB, data);
    return(clump);
}


RpAtomic * AtomicPreLightClearAll128CB(RpAtomic *atomic, void *data )
{
    RwBool clearVertexLitObjects = *(RwBool *)data;
    RpGeometry *geom = RpAtomicGetGeometry(atomic);
    static RwRGBA clearCol = {128, 128, 128, 255};
    RwUInt32 i;

    /* Are there any prelights to clear? */
    if (RpGeometryGetPreLightColors(geom) == NULL)
        return atomic;

    /* We only clear vertex-lit objects if we're explicitly told to */
    if ((FALSE == clearVertexLitObjects) &&
        (RtLtMapAtomicGetFlags(atomic) & rtLTMAPOBJECTVERTEXLIGHT))
        return atomic;

    for (i = 0;i < (RwUInt32)RpGeometryGetNumVertices(geom);i++)
    {
        RpGeometryGetPreLightColors(geom)[i] = clearCol;
    }

    return(atomic);
}



//*****************************************************************************
//
// End of File : CLightMapManager.cpp
//
//*****************************************************************************