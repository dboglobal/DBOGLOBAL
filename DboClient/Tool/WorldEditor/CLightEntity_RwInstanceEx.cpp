//*****************************************************************************
// File       : CLightEntity_RpLight.cpp
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

#include "CLightEntity.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

RwUInt32	CLightEntity::m_iRwInstanceExPlugInOffset	= 0;


//-----------------------------------------------------------------------------
// Name			: RwInstanceExPlugInAttach
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CLightEntity::RwInstanceExPlugInAttach( void )
{
	m_iRwInstanceExPlugInOffset = RpLightRegisterPlugin( sizeof( SRwInstanceEx ),
														MAKECHUNKID( rwVENDORID_CRITERIONRM, rwID_WORLD_EDITOR_PLUG_IN_ID ),
														CLightEntity::RwInstanceExPlugInConstructor,
														NULL,
														CLightEntity::RwInstanceExPlugInCopier );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: RwInstanceExPlugInConstructor
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void * CLightEntity::RwInstanceExPlugInConstructor( void * pLight, RwInt32 iOffsetInObject, RwInt32 iSizeInObject )
{
	NTL_FUNCTION( "RwInstanceExPlugInConstructor" );

	SRwInstanceEx *		pPlugInData = GetPlugInDataFromRpLight( (RpLight *)pLight );
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

void * CLightEntity::RwInstanceExPlugInCopier( void * pDstRpLight, const void * pSrcRpLight, RwInt32 iOffsetInObject, RwInt32 iSizeInObject )
{
	NTL_FUNCTION( "RwInstanceExPlugInCopier" );

	SRwInstanceEx *		pSrcPlugInData = GetPlugInDataFromRpLight( (RpLight *)pSrcRpLight );

	SetPlugInDataToRpLight( (RpLight *)pDstRpLight, pSrcPlugInData );

	NTL_RETURN( GetPlugInDataFromRpLight( (RpLight *)pDstRpLight ) );
}


//-----------------------------------------------------------------------------
// Name			: SetPlugInData
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CLightEntity::SetPlugInData( void )
{
	SRwInstanceEx		sRwInstanceEx( this );

	SetPlugInDataToRpLight( GetRpLight(), &sRwInstanceEx );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: ResetPlugInData
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CLightEntity::ResetPlugInData( void )
{
	SRwInstanceEx		sRwInstanceEx( NULL );

	SetPlugInDataToRpLight( GetRpLight(), &sRwInstanceEx );

	return TRUE;
}


//*****************************************************************************
//
// End of File : CLightEntity_RpInstanceEx.cpp
//
//*****************************************************************************