//*****************************************************************************
// File       : CAtomicEntity_RpAtomic.cpp
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

#include "CAtomicEntity.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

RwUInt32	CAtomicEntity::m_iRwInstanceExPlugInOffset	= 0;


//-----------------------------------------------------------------------------
// Name			: RwInstanceExPlugInAttach
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CAtomicEntity::RwInstanceExPlugInAttach( void )
{
	m_iRwInstanceExPlugInOffset = RpAtomicRegisterPlugin( sizeof( SRwInstanceEx ),
															MAKECHUNKID( rwVENDORID_CRITERIONRM, rwID_WORLD_EDITOR_PLUG_IN_ID ),
															CAtomicEntity::RwInstanceExPlugInConstructor,
															NULL,
															CAtomicEntity::RwInstanceExPlugInCopier );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: RwInstanceExPlugInConstructor
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void * CAtomicEntity::RwInstanceExPlugInConstructor( void * pAtomic, RwInt32 iOffsetInObject, RwInt32 iSizeInObject )
{
	NTL_FUNCTION( "RwInstanceExPlugInConstructor" );

	SRwInstanceEx *		pPlugInData = GetPlugInDataFromRpAtomic( (RpAtomic *)pAtomic );
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

void * CAtomicEntity::RwInstanceExPlugInCopier( void * pDstRpAtomic, const void * pSrcRpAtomic, RwInt32 iOffsetInObject, RwInt32 iSizeInObject )
{
	NTL_FUNCTION( "RwInstanceExPlugInCopier" );

	SRwInstanceEx *		pSrcPlugInData = GetPlugInDataFromRpAtomic( (RpAtomic *)pSrcRpAtomic );

	SetPlugInDataToRpAtomic( (RpAtomic *)pDstRpAtomic, pSrcPlugInData );

	NTL_RETURN( GetPlugInDataFromRpAtomic( (RpAtomic *)pDstRpAtomic ) );
}


//-----------------------------------------------------------------------------
// Name			: SetPlugInData
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CAtomicEntity::SetPlugInData( void )
{
	SRwInstanceEx		sRwInstanceEx( this );

	SetPlugInDataToRpAtomic( GetRpAtomic(), &sRwInstanceEx );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: ResetPlugInData
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CAtomicEntity::ResetPlugInData( void )
{
	SRwInstanceEx		sRwInstanceEx( NULL );

	SetPlugInDataToRpAtomic( GetRpAtomic(), &sRwInstanceEx );

	return TRUE;
}


//*****************************************************************************
//
// End of File : CAtomicEntity_RpInstanceEx.cpp
//
//*****************************************************************************