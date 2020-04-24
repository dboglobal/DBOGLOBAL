//*****************************************************************************
// File       : CMeshEntity_RpMesh.cpp
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

#include "CMeshEntity.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

RwUInt32	CMeshEntity::m_iRwInstanceExPlugInOffset	= 0;


//-----------------------------------------------------------------------------
// Name			: RwInstanceExPlugInAttach
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CMeshEntity::RwInstanceExPlugInAttach( void )
{
//	m_iRwInstanceExPlugInOffset = RpMeshRegisterPlugin( sizeof( SRwInstanceEx ),
//														MAKECHUNKID( rwVENDORID_CRITERIONRM, rwID_WORLD_EDITOR_PLUG_IN_ID ),
//														CMeshEntity::RwInstanceExPlugInConstructor,
//														NULL,
//														CMeshEntity::RwInstanceExPlugInCopier );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: RwInstanceExPlugInConstructor
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void * CMeshEntity::RwInstanceExPlugInConstructor( void * pMesh, RwInt32 iOffsetInObject, RwInt32 iSizeInObject )
{
	NTL_FUNCTION( "RwInstanceExPlugInConstructor" );

//	SRwInstanceEx *		pPlugInData = GetPlugInDataFromRpMesh( (RpMesh *)pMesh );
//	pPlugInData->SetEntity( NULL );

//	NTL_RETURN( pPlugInData );
	NTL_RETURN( NULL );
}


//-----------------------------------------------------------------------------
// Name			: RwInstanceExPlugInCopier
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void * CMeshEntity::RwInstanceExPlugInCopier( void * pDstRpMesh, const void * pSrcRpMesh, RwInt32 iOffsetInObject, RwInt32 iSizeInObject )
{
	NTL_FUNCTION( "RwInstanceExPlugInCopier" );

//	SRwInstanceEx *		pSrcPlugInData = GetPlugInDataFromRpMesh( (RpMesh *)pSrcRpMesh );

//	SetPlugInDataToRpMesh( (RpMesh *)pDstRpMesh, pSrcPlugInData );

	NTL_RETURN( GetPlugInDataFromRpMesh( (RpMesh *)pDstRpMesh ) );
}


//-----------------------------------------------------------------------------
// Name			: SetPlugInData
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CMeshEntity::SetPlugInData( void )
{
	SRwInstanceEx		sRwInstanceEx( this );

//	SetPlugInDataToRpMesh( GetRpMesh(), &sRwInstanceEx );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: ResetPlugInData
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CMeshEntity::ResetPlugInData( void )
{
	SRwInstanceEx		sRwInstanceEx( NULL );

//	SetPlugInDataToRpMesh( GetRpMesh(), &sRwInstanceEx );

	return TRUE;
}


//*****************************************************************************
//
// End of File : CMeshEntity_RpInstanceEx.cpp
//
//*****************************************************************************