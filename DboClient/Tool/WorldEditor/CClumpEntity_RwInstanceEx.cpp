//*****************************************************************************
// File       : CClumpEntity_RpClump.cpp
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

#include "CClumpEntity.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

RwUInt32	CClumpEntity::m_iRwInstanceExPlugInOffset	= 0;


//-----------------------------------------------------------------------------
// Name			: RwInstanceExPlugInAttach
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CClumpEntity::RwInstanceExPlugInAttach( void )
{
	m_iRwInstanceExPlugInOffset = RpClumpRegisterPlugin( sizeof( SRwInstanceEx ),
														MAKECHUNKID( rwVENDORID_CRITERIONRM, rwID_WORLD_EDITOR_PLUG_IN_ID ),
														CClumpEntity::RwInstanceExPlugInConstructor,
														NULL,
														CClumpEntity::RwInstanceExPlugInCopier );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: RwInstanceExPlugInConstructor
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void * CClumpEntity::RwInstanceExPlugInConstructor( void * pClump, RwInt32 iOffsetInObject, RwInt32 iSizeInObject )
{
	NTL_FUNCTION( "RwInstanceExPlugInConstructor" );

	SRwInstanceEx *		pPlugInData = GetPlugInDataFromRpClump( (RpClump *)pClump );
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

void * CClumpEntity::RwInstanceExPlugInCopier( void * pDstRpClump, const void * pSrcRpClump, RwInt32 iOffsetInObject, RwInt32 iSizeInObject )
{
	NTL_FUNCTION( "RwInstanceExPlugInCopier" );

	SRwInstanceEx *		pSrcPlugInData = GetPlugInDataFromRpClump( (RpClump *)pSrcRpClump );

	SetPlugInDataToRpClump( (RpClump *)pDstRpClump, pSrcPlugInData );

	NTL_RETURN( GetPlugInDataFromRpClump( (RpClump *)pDstRpClump ) );
}


//-----------------------------------------------------------------------------
// Name			: SetPlugInData
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CClumpEntity::SetPlugInData( void )
{
	SRwInstanceEx		sRwInstanceEx( this );

	SetPlugInDataToRpClump( GetRpClump(), &sRwInstanceEx );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: ResetPlugInData
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CClumpEntity::ResetPlugInData( void )
{
	SRwInstanceEx		sRwInstanceEx( NULL );

	SetPlugInDataToRpClump( GetRpClump(), &sRwInstanceEx );

	return TRUE;
}


//*****************************************************************************
//
// End of File : CClumpEntity_RpInstanceEx.cpp
//
//*****************************************************************************