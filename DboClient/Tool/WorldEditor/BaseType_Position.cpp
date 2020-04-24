//*****************************************************************************
// File       : BaseType_Position.cpp
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

#include "BaseType_Position.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

const SLocation3D SLocation3D::LOCATION_DEFAULT	= SLocation3D( 0.0f, 0.0f, 0.0f );
const SLocation3D SLocation3D::SCALE_DEFAULT	= SLocation3D( 1.0f, 1.0f, 1.0f );

const SRotation3D SRotation3D::ROTATION_DEFAULT	= SRotation3D( 0.0f, 0.0f, 0.0f );

const SPosition3D SPosition3D::POSITION_DEFAULT	= SPosition3D( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );


//-----------------------------------------------------------------------------
// Name			: GetPositionInLine
// Desc			: 
// Privilege	: global
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwV3d * GetPositionInLine( const RwLine * pLine, const RwReal fRatio, RwV3d * pPosition )
{
	Assert( pLine != NULL );
	Assert( pPosition != NULL );

	pPosition->x = pLine->start.x + fRatio * ( pLine->end.x - pLine->start.x );
	pPosition->y = pLine->start.y + fRatio * ( pLine->end.y - pLine->start.y );
	pPosition->z = pLine->start.z + fRatio * ( pLine->end.z - pLine->start.z );

	return pPosition;
}


//-----------------------------------------------------------------------------
// Name			: IsPointInSphere
// Desc			: 
// Privilege	: global
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool IsPointInSphere( const RwV3d * pPoint, const RwSphere * pSphere )
{
	RwV3d		CenterToPoint;

	RwV3dSubMacro( &CenterToPoint, pPoint, &(pSphere->center) );

	if( RwV3dLength( &CenterToPoint ) <= pSphere->radius )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


//-----------------------------------------------------------------------------
// Name			: IsPointInBBox
// Desc			: 
// Privilege	: global
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool IsPointInBBox( const RwV3d * pPoint, const RwBBox * pBBox )
{
	if( pPoint->x < pBBox->inf.x || pPoint->x > pBBox->sup.x
		|| pPoint->y < pBBox->inf.y || pPoint->y > pBBox->sup.y
		|| pPoint->z < pBBox->inf.z || pPoint->z > pBBox->sup.z )
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}


//-----------------------------------------------------------------------------
// Name			: IsLineInSphere
// Desc			: 
// Privilege	: global
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool IsLineInSphere( const RwLine * pLine, const RwSphere * pSphere )
{
	RwV3d		EndToStart;
	RwV3dSubMacro( &EndToStart, &(pLine->end), &(pLine->start) );

	RwReal		fLineLength = RwV3dLength( &EndToStart );

	if( fLineLength == 0.0f )
	{
		return IsPointInSphere( &(pLine->start), pSphere );
	}

	RwV3d		CenterToStart;
	RwV3dSubMacro( &CenterToStart, &(pSphere->center), &(pLine->start) );

	RwReal		fETSDotCTS = RwV3dDotProduct( &EndToStart, &CenterToStart);

	if( fETSDotCTS < 0.0f &&
		fLineLength > pSphere->radius )
	{
		return FALSE;
	}

	RwReal		fClosestSqr = fLineLength * fLineLength - fETSDotCTS * fETSDotCTS;
	RwReal		fRadiusSqr = pSphere->radius * pSphere->radius;

	if( fClosestSqr > fRadiusSqr )
	{
		return FALSE;
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: IsLineInSphere
// Desc			: 
// Privilege	: global
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool IsLineInBox( const RwLine * pLine, const RwBBox * pBBox )
{
	if( IsPointInBBox( &(pLine->start), pBBox ) != FALSE )
	{
		return TRUE;
	}

	if( IsPointInBBox( &(pLine->end), pBBox ) != FALSE )
	{
		return TRUE;
	}

	RwV3d		BoxCenter;

	BoxCenter.x = ( pBBox->sup.x + pBBox->inf.x ) * 0.5f;
	BoxCenter.y = ( pBBox->sup.y + pBBox->inf.y ) * 0.5f;
	BoxCenter.z = ( pBBox->sup.z + pBBox->inf.z ) * 0.5f;

	RwV3d		BoxHalfSize;

	BoxHalfSize.x = ( pBBox->sup.x - pBBox->inf.x ) * 0.5f;
	BoxHalfSize.y = ( pBBox->sup.y - pBBox->inf.y ) * 0.5f;
	BoxHalfSize.z = ( pBBox->sup.z - pBBox->inf.z ) * 0.5f;

	RwLine		LocalizedLine;

	LocalizedLine.start.x = pLine->start.x - BoxCenter.x;
	LocalizedLine.start.y = pLine->start.y - BoxCenter.y;
	LocalizedLine.start.z = pLine->start.z - BoxCenter.z;
	LocalizedLine.end.x = pLine->end.x - BoxCenter.x;
	LocalizedLine.end.y = pLine->end.y - BoxCenter.y;
	LocalizedLine.end.z = pLine->end.z - BoxCenter.z;

	RwV3d		LocalizedLineCenter;

	LocalizedLineCenter.x = ( LocalizedLine.end.x + LocalizedLine.start.x ) * 0.5f;
	LocalizedLineCenter.y = ( LocalizedLine.end.y + LocalizedLine.start.y ) * 0.5f;
	LocalizedLineCenter.z = ( LocalizedLine.end.z + LocalizedLine.start.z ) * 0.5f;

	RwV3d		LineHalfVector;

	LineHalfVector.x = LocalizedLineCenter.x - LocalizedLine.start.x;
	LineHalfVector.y = LocalizedLineCenter.y - LocalizedLine.start.y;
	LineHalfVector.z = LocalizedLineCenter.z - LocalizedLine.start.z;

	RwV3d		LineHalfLength;

	LineHalfLength.x = fabs( LineHalfVector.x );
	LineHalfLength.y = fabs( LineHalfVector.y );
	LineHalfLength.z = fabs( LineHalfVector.z );

	if( fabs( LocalizedLineCenter.x ) > fabs( LineHalfLength.x ) + BoxHalfSize.x )
	{
		return FALSE;
	}

	if( fabs( LocalizedLineCenter.y ) > fabs( LineHalfLength.y ) + BoxHalfSize.y )
	{
		return FALSE;
	}

	if( fabs( LocalizedLineCenter.z ) > fabs( LineHalfLength.z ) + BoxHalfSize.z )
	{
		return FALSE;
	}

	if( fabs( LocalizedLineCenter.x * LineHalfVector.y - LocalizedLineCenter.y * LineHalfVector.x ) >
		fabs( BoxHalfSize.x * LineHalfLength.y + BoxHalfSize.y * LineHalfLength.x ) )
	{
		return FALSE;
	}

	if( fabs( LocalizedLineCenter.y * LineHalfVector.z - LocalizedLineCenter.z * LineHalfVector.y ) >
		fabs( BoxHalfSize.y * LineHalfLength.z + BoxHalfSize.z * LineHalfLength.y ) )
	{
		return FALSE;
	}

	if( fabs( LocalizedLineCenter.z * LineHalfVector.x - LocalizedLineCenter.x * LineHalfVector.z ) >
		fabs( BoxHalfSize.z * LineHalfLength.x + BoxHalfSize.x * LineHalfLength.z ) )
	{
		return FALSE;
	}

	return TRUE;
}



//-----------------------------------------------------------------------------
// Name			: SetFrame
// Desc			: 
// Privilege	: global
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void SetFrame( RwFrame * pFrame, SPosition3D * pPosition )
{
	Assert( pFrame != NULL );

	SetMatrix( RwFrameGetMatrix( pFrame ), pPosition );

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetFrame
// Desc			: 
// Privilege	: global
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void SetFrame( RwFrame * pFrame, SPosition3D * pPosition, SLocation3D * pScale )
{
	Assert( pFrame != NULL );

	SetMatrix( RwFrameGetMatrix( pFrame ), pPosition, pScale );

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetMatrix
// Desc			: 
// Privilege	: global
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void SetMatrix( RwMatrix * pMatrix, SPosition3D * pPosition )
{
	Assert( pMatrix != NULL );
	Assert( pPosition != NULL );

	RwMatrix		Matrix;
	RwV3d			Front, Left, Up;

	RwMatrixRotate( &Matrix, &AXIS_Y, pPosition->GetRotation()->GetYaw(), rwCOMBINEREPLACE );

	RwV3dTransformVector( &Front, &AXIS_Z, &Matrix );
	RwV3dTransformVector( &Left, &AXIS_X, &Matrix );

	RwMatrixRotate( &Matrix, &Left, pPosition->GetRotation()->GetPitch(), rwCOMBINEREPLACE );

	RwV3dTransformVector( &Front, &Front, &Matrix );
	RwV3dTransformVector( &Up, &AXIS_Y, &Matrix );

	RwMatrixRotate( pMatrix, &AXIS_Y, pPosition->GetRotation()->GetYaw(), rwCOMBINEREPLACE );
	RwMatrixRotate( pMatrix, &Left, pPosition->GetRotation()->GetPitch(), rwCOMBINEPOSTCONCAT );
	RwMatrixRotate( pMatrix, &Front, pPosition->GetRotation()->GetRoll(), rwCOMBINEPOSTCONCAT );
	pMatrix = RwMatrixTranslate( pMatrix, pPosition->GetLocation()->GetRwV3d(), rwCOMBINEPOSTCONCAT );

	return;
}



//-----------------------------------------------------------------------------
// Name			: SetMatrix
// Desc			: 
// Privilege	: global
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void SetMatrix( RwMatrix * pMatrix, SPosition3D * pPosition, SLocation3D * pScale )
{
	Assert( pMatrix != NULL );
	Assert( pPosition != NULL );
	Assert( pScale != NULL );

	RwMatrix		Matrix;
	RwV3d			Front, Left, Up;

	RwMatrixRotate( &Matrix, &AXIS_Y, pPosition->GetRotation()->GetYaw(), rwCOMBINEREPLACE );

	RwV3dTransformVector( &Front, &AXIS_Z, &Matrix );
	RwV3dTransformVector( &Left, &AXIS_X, &Matrix );

	RwMatrixRotate( &Matrix, &Left, pPosition->GetRotation()->GetPitch(), rwCOMBINEREPLACE );

	RwV3dTransformVector( &Front, &Front, &Matrix );
	RwV3dTransformVector( &Up, &AXIS_Y, &Matrix );

	RwMatrixScale( pMatrix, pScale, rwCOMBINEREPLACE );
	RwMatrixRotate( pMatrix, &AXIS_Y, pPosition->GetRotation()->GetYaw(), rwCOMBINEPOSTCONCAT );
	RwMatrixRotate( pMatrix, &Left, pPosition->GetRotation()->GetPitch(), rwCOMBINEPOSTCONCAT );
	RwMatrixRotate( pMatrix, &Front, pPosition->GetRotation()->GetRoll(), rwCOMBINEPOSTCONCAT );
	pMatrix = RwMatrixTranslate( pMatrix, pPosition->GetLocation()->GetRwV3d(), rwCOMBINEPOSTCONCAT );

	return;
}


RwV3d * SRotation3D::GetDirectionVector( RwV3d * pNewVector )
{
	RwMatrix		sMatrix;
	SPosition3D		sPosition( SLocation3D::LOCATION_DEFAULT, *this );

	SetMatrix( &sMatrix, &sPosition );

	RwV3dTransformVector( pNewVector, &AXIS_Z, &sMatrix );

	return pNewVector;
}




//*****************************************************************************
//
// End of File : BaseType_Position.cpp
//
//*****************************************************************************