//*****************************************************************************
// File       : CWorldSectorEntity_RpInstanceEx.cpp
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

#include "CWorldSectorEntity.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

RwUInt32	CWorldSectorEntity::m_iRwInstanceExPlugInOffset	= 0;


//-----------------------------------------------------------------------------
// Name			: RwInstanceExPlugInAttach
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldSectorEntity::RwInstanceExPlugInAttach( void )
{
	m_iRwInstanceExPlugInOffset = RpWorldSectorRegisterPlugin( sizeof( SRwInstanceEx ),
																MAKECHUNKID( rwVENDORID_CRITERIONRM, rwID_WORLD_EDITOR_PLUG_IN_ID ),
																CWorldSectorEntity::RwInstanceExPlugInConstructor,
																NULL,
																CWorldSectorEntity::RwInstanceExPlugInCopier );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: RwInstanceExPlugInConstructor
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void * CWorldSectorEntity::RwInstanceExPlugInConstructor( void * pWorldSector, RwInt32 iOffsetInObject, RwInt32 iSizeInObject )
{
	NTL_FUNCTION( "RpWorldSectorExPlugInConstructor" );

	SRwInstanceEx *		pPlugInData = GetPlugInDataFromRpWorldSector( (RpWorldSector *)pWorldSector );
	pPlugInData->SetEntity( NULL );

	NTL_RETURN( pPlugInData );
}


//-----------------------------------------------------------------------------
// Name			: RwInstanceExPlugInCopier
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void * CWorldSectorEntity::RwInstanceExPlugInCopier( void * pDstRpWorldSector, const void * pSrcRpWorldSector, RwInt32 iOffsetInObject, RwInt32 iSizeInObject )
{
	NTL_FUNCTION( "RpWorldSectorExPlugInCopier" );

	SRwInstanceEx *		pSrcPlugInData = GetPlugInDataFromRpWorldSector( (RpWorldSector *)pSrcRpWorldSector );

	SetPlugInDataToRpWorldSector( (RpWorldSector *)pDstRpWorldSector, pSrcPlugInData );

	NTL_RETURN( GetPlugInDataFromRpWorldSector( (RpWorldSector *)pDstRpWorldSector ) );
}


//-----------------------------------------------------------------------------
// Name			: SetPlugInData
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldSectorEntity::SetPlugInData( void )
{
	SRwInstanceEx		sRwInstanceEx( this );

	SetPlugInDataToRpWorldSector( GetRpWorldSector(), &sRwInstanceEx );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: ResetPlugInData
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldSectorEntity::ResetPlugInData( void )
{
	SRwInstanceEx		sRwInstanceEx( NULL );

	SetPlugInDataToRpWorldSector( GetRpWorldSector(), &sRwInstanceEx );

	return TRUE;
}


//*****************************************************************************
//
// End of File : CWorldSectorEntity_RpInstanceEx.cpp
//
//*****************************************************************************