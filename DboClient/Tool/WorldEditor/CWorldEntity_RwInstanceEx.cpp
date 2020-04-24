//*****************************************************************************
// File       : CWorldEntity_RpInstanceEx.cpp
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

#include "CWorldEntity.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

RwUInt32	CWorldEntity::m_iRwInstanceExPlugInOffset	= 0;


//-----------------------------------------------------------------------------
// Name			: RwInstanceExPlugInAttach
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldEntity::RwInstanceExPlugInAttach( void )
{
	m_iRwInstanceExPlugInOffset = RpWorldRegisterPlugin( sizeof( SRwInstanceEx ),
														MAKECHUNKID( rwVENDORID_CRITERIONRM, rwID_WORLD_EDITOR_PLUG_IN_ID ),
														CWorldEntity::RwInstanceExPlugInConstructor,
														NULL,
														CWorldEntity::RwInstanceExPlugInCopier );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: RwInstanceExPlugInConstructor
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void * CWorldEntity::RwInstanceExPlugInConstructor( void * pWorld, RwInt32 iOffsetInObject, RwInt32 iSizeInObject )
{
	NTL_FUNCTION( "RwInstanceExPlugInConstructor" );

	SRwInstanceEx *		pPlugInData = GetPlugInDataFromRpWorld( (RpWorld *)pWorld );
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

void * CWorldEntity::RwInstanceExPlugInCopier( void * pDstRpWorld, const void * pSrcRpWorld, RwInt32 iOffsetInObject, RwInt32 iSizeInObject )
{
	NTL_FUNCTION( "RwInstanceExPlugInCopier" );

	SRwInstanceEx *		pSrcPlugInData = GetPlugInDataFromRpWorld( (RpWorld *)pSrcRpWorld );

	SetPlugInDataToRpWorld( (RpWorld *)pDstRpWorld, pSrcPlugInData );

	NTL_RETURN( GetPlugInDataFromRpWorld( (RpWorld *)pDstRpWorld ) );
}


//-----------------------------------------------------------------------------
// Name			: SetPlugInData
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldEntity::SetPlugInData( void )
{
	SRwInstanceEx		sRwInstanceEx( this );

	SetPlugInDataToRpWorld( GetRpWorld(), &sRwInstanceEx );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: ResetPlugInData
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldEntity::ResetPlugInData( void )
{
	SRwInstanceEx		sRwInstanceEx( NULL );

	SetPlugInDataToRpWorld( GetRpWorld(), &sRwInstanceEx );

	return TRUE;
}


//*****************************************************************************
//
// End of File : CWorldEntity_RpInstanceEx.cpp
//
//*****************************************************************************