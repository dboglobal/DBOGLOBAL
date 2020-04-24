//*****************************************************************************
// File       : CAtomicEntity_Render.cpp
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

#include "Helper.h"

#include "CAtomicEntity.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

RwBool	CAtomicEntity::m_bRenderSolidForTerrain					= TRUE;
RwBool	CAtomicEntity::m_bRenderWireFrameForTerrain				= FALSE;
RwBool	CAtomicEntity::m_bRenderBoundingForTerrain				= FALSE;

RwBool	CAtomicEntity::m_bRenderSolidForObject					= TRUE;
RwBool	CAtomicEntity::m_bRenderWireFrameForObject				= FALSE;
RwBool	CAtomicEntity::m_bRenderBoundingForObject				= FALSE;

RwBool	CAtomicEntity::m_bRenderSolidForSelectedTerrain			= TRUE;
RwBool	CAtomicEntity::m_bRenderWireFrameForSelectedTerrain		= FALSE;
RwBool	CAtomicEntity::m_bRenderBoundingForSelectedTerrain		= TRUE;

RwBool	CAtomicEntity::m_bRenderSolidForSelectedObject			= TRUE;
RwBool	CAtomicEntity::m_bRenderWireFrameForSelectedObject		= FALSE;
RwBool	CAtomicEntity::m_bRenderBoundingForSelectedObject		= TRUE;

RwRGBA	CAtomicEntity::m_WireFrameColorForTerrain				= { 0, 128, 128, 255 };
RwRGBA	CAtomicEntity::m_BoundingColorForTerrain				= { 0, 128, 0, 255 };

RwRGBA	CAtomicEntity::m_WireFrameColorForSelectedTerrain		= { 0, 255, 255, 255 };
RwRGBA	CAtomicEntity::m_BoundingColorForSelectedTerrain		= { 0, 255, 0, 255 };

RwRGBA	CAtomicEntity::m_WireFrameColorForObject				= { 128, 128, 0, 255 };
RwRGBA	CAtomicEntity::m_BoundingColorForObject					= { 128, 0, 0, 255 };

RwRGBA	CAtomicEntity::m_WireFrameColorForSelectedObject		= { 255, 255, 0, 255 };
RwRGBA	CAtomicEntity::m_BoundingColorForSelectedObject			= { 255, 0, 0, 255 };


//-----------------------------------------------------------------------------
// Name			: CreateWireFrame
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CAtomicEntity::CreateWireFrame( void )
{
	RpAtomic * pAtomic = GetRpAtomic();

	if( pAtomic == NULL )
	{
		return FALSE;
	}

	RwReal			fOffset;

	if( IsTerrain() != FALSE )
	{
		fOffset = 0.1f;
	}
	else
	{
		fOffset = 0.0f;
	}

	RpGeometry *	pGeometry = RpAtomicGetGeometry( pAtomic );
	if( pGeometry == NULL )
	{
		return FALSE;
	}

	RwInt32			iPolygonCount = RpGeometryGetNumTriangles( pGeometry );
	if( iPolygonCount == 0 )
	{
		return FALSE;
	}

	RwInt32			iMorphTargetCount = RpGeometryGetNumMorphTargets( pGeometry );
	RpMorphTarget *	pMorphTarget;
	RwV3d *			pVertexStart					= (RwV3d *)NULL;
	RwV3d *			pVertexEnd						= (RwV3d *)NULL;
	RwReal			fInterpolationPosition			= 1.0f;
	RwReal			fInverseInterpolationPosition	= 0.0f;

	if( iMorphTargetCount > 1 )
	{
		RpInterpolator *	pInterpolation = RpAtomicGetInterpolator( pAtomic );
        RwInt32				iMorphTargetStart, iMorphTargetEnd;

		fInterpolationPosition			= RpInterpolatorGetValue( pInterpolation ) / RpInterpolatorGetScale( pInterpolation );
		fInverseInterpolationPosition	= 1.0f - fInterpolationPosition;

		iMorphTargetStart	= RpInterpolatorGetStartMorphTarget( pInterpolation );
		iMorphTargetEnd		= RpInterpolatorGetEndMorphTarget( pInterpolation );

		pMorphTarget		= RpGeometryGetMorphTarget( pGeometry, iMorphTargetStart );
		pVertexStart		= RpMorphTargetGetVertices( pMorphTarget );

		pMorphTarget		= RpGeometryGetMorphTarget( pGeometry, iMorphTargetEnd );
		pVertexEnd			= RpMorphTargetGetVertices( pMorphTarget );
	}
	else
	{
		pMorphTarget		= RpGeometryGetMorphTarget( pGeometry, 0 );
		pVertexStart		= RpMorphTargetGetVertices( pMorphTarget );
	}

	Assert( pMorphTarget != NULL );

	RwInt32			iTrangleCount		= RpGeometryGetNumTriangles( pGeometry );
	RpTriangle *	pTriangles			= RpGeometryGetTriangles( pGeometry );

	DestroyWireFrame();
	
	m_WireFrameRenderInfo.iVertexCount	= iTrangleCount * 6;
	m_WireFrameRenderInfo.pVertices		= (RwIm3DVertex *)RwMalloc( m_WireFrameRenderInfo.iVertexCount * sizeof( RwIm3DVertex ), rwID_NAOBJECT );
	Assert( m_WireFrameRenderInfo.pVertices != NULL );

	RwV3d			TempVector[2];
	RwUInt16		VertexIndex[3];
	RwV3d			Vertex[3];

	m_BBox.inf.x = +RwRealMAXVAL;
	m_BBox.inf.y = +RwRealMAXVAL;
	m_BBox.inf.z = +RwRealMAXVAL;
	m_BBox.sup.x = -RwRealMAXVAL;
	m_BBox.sup.y = -RwRealMAXVAL;
	m_BBox.sup.z = -RwRealMAXVAL;

	for( RwInt32 iTriangle = 0; iTriangle < iTrangleCount; ++iTriangle )
	{
		RpGeometryTriangleGetVertexIndices( pGeometry, pTriangles, &VertexIndex[0], &VertexIndex[1], &VertexIndex[2] );

		if( iMorphTargetCount > 1 )
		{
			for( RwInt32 iIndex = 0; iIndex < 3; ++iIndex )
			{
				RwV3dScale( &TempVector[0], &pVertexStart[VertexIndex[iIndex]], fInverseInterpolationPosition );
				RwV3dScale( &TempVector[1], &pVertexEnd[VertexIndex[iIndex]], fInterpolationPosition );
				RwV3dAdd( &Vertex[iIndex], &TempVector[0], &TempVector[1] );
			}
		}
		else
		{
			for( RwInt32 iIndex = 0; iIndex < 3; ++iIndex )
			{
				Vertex[iIndex] = pVertexStart[VertexIndex[iIndex]];
			}
		}

		for( RwInt32 iIndex = 0; iIndex < 6; ++iIndex )
		{
			RwInt32		iOffset = ((iIndex + 1) / 2) % 3;

			if( m_BBox.inf.x > Vertex[iOffset].x )
			{
				m_BBox.inf.x = Vertex[iOffset].x;
			}

			if( m_BBox.sup.x < Vertex[iOffset].x )
			{
				m_BBox.sup.x = Vertex[iOffset].x;
			}

			if( m_BBox.inf.y > Vertex[iOffset].y )
			{
				m_BBox.inf.y = Vertex[iOffset].y;
			}

			if( m_BBox.sup.y < Vertex[iOffset].y )
			{
				m_BBox.sup.y = Vertex[iOffset].y;
			}

			if( m_BBox.inf.z > Vertex[iOffset].z )
			{
				m_BBox.inf.z = Vertex[iOffset].z;
			}

			if( m_BBox.sup.z < Vertex[iOffset].z )
			{
				m_BBox.sup.z = Vertex[iOffset].z;
			}

			RwIm3DVertexSetPos( &m_WireFrameRenderInfo.pVertices[iTriangle * 6 + iIndex], Vertex[iOffset].x, Vertex[iOffset].y + fOffset, Vertex[iOffset].z );

			if( IsTerrain() != FALSE )
			{
				RwIm3DVertexSetRGBA( &m_WireFrameRenderInfo.pVertices[iTriangle * 6 + iIndex],
									m_WireFrameColorForTerrain.red,
									m_WireFrameColorForTerrain.green,
									m_WireFrameColorForTerrain.blue,
									m_WireFrameColorForTerrain.alpha );
			}
			else
			{
				RwIm3DVertexSetRGBA( &m_WireFrameRenderInfo.pVertices[iTriangle * 6 + iIndex],
									m_WireFrameColorForObject.red,
									m_WireFrameColorForObject.green,
									m_WireFrameColorForObject.blue,
									m_WireFrameColorForObject.alpha );
			}
		}

		++pTriangles;
	}

	// Create Bounding Info

	m_BoundingRenderInfo.iVertexCount	= 8;
	m_BoundingRenderInfo.pVertices		= (RwIm3DVertex *)RwMalloc( m_WireFrameRenderInfo.iVertexCount * sizeof( RwIm3DVertex ), rwID_NAOBJECT );
	Assert( m_BoundingRenderInfo.pVertices != NULL );

	for( RwInt32 iVertex =0; iVertex < 8; ++iVertex )
    {
		RwIm3DVertexSetPos( &m_BoundingRenderInfo.pVertices[iVertex], 
							iVertex & 1 ? m_BBox.sup.x : m_BBox.inf.x,
							iVertex & 2 ? m_BBox.sup.y : m_BBox.inf.y,
							iVertex & 4 ? m_BBox.sup.z : m_BBox.inf.z);

		if( IsTerrain() != FALSE )
		{
			if( IsSelected() != FALSE )
			{
                RwIm3DVertexSetRGBA( &m_BoundingRenderInfo.pVertices[iVertex], 
									m_BoundingColorForSelectedTerrain.red,
									m_BoundingColorForSelectedTerrain.green,
									m_BoundingColorForSelectedTerrain.blue,
									m_BoundingColorForSelectedTerrain.alpha );
			}
			else
			{
                RwIm3DVertexSetRGBA( &m_BoundingRenderInfo.pVertices[iVertex], 
									m_BoundingColorForTerrain.red,
									m_BoundingColorForTerrain.green,
									m_BoundingColorForTerrain.blue,
									m_BoundingColorForTerrain.alpha );
			}
		}
		else
		{
			if( IsSelected() != FALSE )
			{
                RwIm3DVertexSetRGBA( &m_BoundingRenderInfo.pVertices[iVertex], 
									m_BoundingColorForSelectedObject.red,
									m_BoundingColorForSelectedObject.green,
									m_BoundingColorForSelectedObject.blue,
									m_BoundingColorForSelectedObject.alpha );
			}
			else
			{
                RwIm3DVertexSetRGBA( &m_BoundingRenderInfo.pVertices[iVertex], 
									m_BoundingColorForObject.red,
									m_BoundingColorForObject.green,
									m_BoundingColorForObject.blue,
									m_BoundingColorForObject.alpha );
			}
		}
	}
	
	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: DestroyWireFrame
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CAtomicEntity::DestroyWireFrame( void )
{
	if( m_WireFrameRenderInfo.pVertices != NULL )
	{
		RwFree( m_WireFrameRenderInfo.pVertices );

		m_WireFrameRenderInfo.pVertices = NULL;
	}

	m_WireFrameRenderInfo.iVertexCount = 0;


	if( m_BoundingRenderInfo.pVertices != NULL )
	{
		RwFree( m_BoundingRenderInfo.pVertices );

		m_BoundingRenderInfo.pVertices = NULL;
	}

	m_BoundingRenderInfo.iVertexCount = 0;

	return;
}


//-----------------------------------------------------------------------------
// Name			: ApplyColorToWireFrame
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CAtomicEntity::ApplyColorToWireFrame( RwRGBA * pColor )
{
	if( m_WireFrameRenderInfo.pVertices == NULL )
	{
		return FALSE;
	}

	for( RwInt32 iIndex = 0; iIndex < m_WireFrameRenderInfo.iVertexCount; ++iIndex )
	{
		RwIm3DVertexSetRGBA( &m_WireFrameRenderInfo.pVertices[iIndex], pColor->red, pColor->green, pColor->blue, pColor->alpha );
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: ApplyColorToWireFrame
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CAtomicEntity::ApplyColorToBounding( RwRGBA * pColor )
{
	if( m_BoundingRenderInfo.pVertices == NULL )
	{
		return FALSE;
	}

	for( RwInt32 iIndex = 0; iIndex < m_BoundingRenderInfo.iVertexCount; ++iIndex )
	{
		RwIm3DVertexSetRGBA( &m_BoundingRenderInfo.pVertices[iIndex], pColor->red, pColor->green, pColor->blue, pColor->alpha );
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: RenderSolid
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

// RwRenderStateSet( rwRENDERSTATESHADEMODE, (void *)rwSHADEMODEGOURAUD );
//¿ìµð
RwBool CAtomicEntity::RenderSolid( void )
{
//	InsertRenderManager();

//	RwRenderStateSet( rwRENDERSTATETEXTUREADDRESS, (void *)rwTEXTUREADDRESSCLAMP );

	if( IsTerrain() != FALSE )
	{
		Assert( GetParent() != NULL && GetParent()->GetRwInstance() != NULL );

		RpTie *			m_pTie = NTL_NEW RpTie;
		m_pTie->apAtom	= GetRpAtomic();

		CEntity *		pParentEntity = GetParent();
//		RpWorldSector *	pWorldSector = (RpWorldSector *)pParentEntity->GetRwInstance();

		Assert( pParentEntity != NULL );
		m_pTie->worldSector	= reinterpret_cast<RpWorldSector *>(pParentEntity->GetRwInstance());

//		rwLinkListAddLLLink( &(m_pTie->worldSector->collAtomicsInWorldSector), &(m_pTie->lAtomicInWorldSector) );
//		rwLinkListAddLLLink( &(m_pTie->apAtom->llWorldSectorsInAtomic), &(m_pTie->lWorldSectorInAtomic) );

//		RwRenderStateSet( rwRENDERSTATEFOGENABLE, (void *)FALSE );
		RwRenderStateSet( rwRENDERSTATECULLMODE, (void *)rwCULLMODECULLBACK );

		RwRenderStateSet( rwRENDERSTATEALPHATESTFUNCTIONREF, (void *)0x80 );
		RwRenderStateSet( rwRENDERSTATEALPHATESTFUNCTION, (void *)rwALPHATESTFUNCTIONGREATEREQUAL );

		RpAtomicRender( GetRpAtomic() );

//		RwRenderStateSet( rwRENDERSTATEFOGENABLE, (void *)TRUE );
		RwRenderStateSet( rwRENDERSTATECULLMODE, (void *)rwCULLMODECULLNONE );

		RwRenderStateSet( rwRENDERSTATEALPHATESTFUNCTIONREF, (void *)0xD0 );
		RwRenderStateSet( rwRENDERSTATEALPHATESTFUNCTION, (void *)rwALPHATESTFUNCTIONGREATEREQUAL );

//		rwLinkListRemoveLLLink( &(m_pTie->lWorldSectorInAtomic) );
//		rwLinkListRemoveLLLink( &(m_pTie->lAtomicInWorldSector) );

		NTL_DELETE( m_pTie );
		m_pTie = NULL;
	}
	else
	{
		if( IsSky() != FALSE )
		{
			RwRenderStateSet( rwRENDERSTATECULLMODE, (void *)rwCULLMODECULLBACK );

			RwRenderStateSet( rwRENDERSTATEALPHATESTFUNCTIONREF, (void *)0x80 );
			RwRenderStateSet( rwRENDERSTATEALPHATESTFUNCTION, (void *)rwALPHATESTFUNCTIONALWAYS );

			RwD3D9SetRenderState( D3DRS_ZENABLE, FALSE );
		}

		if( GetAtomicProperty()->IsAntiFog() != FALSE )
		{
			RwRenderStateSet( rwRENDERSTATEFOGENABLE, (void *)FALSE );
		}

		RpAtomicRender( GetRpAtomic() );

		if( GetAtomicProperty()->IsAntiFog() != FALSE )
		{
			RwRenderStateSet( rwRENDERSTATEFOGENABLE, (void *)TRUE );
		}

		if( GetParent()->GetType() == E_ENTITY_TYPE_SKY )
		{
			RwRenderStateSet( rwRENDERSTATECULLMODE, (void *)rwCULLMODECULLNONE );

			RwRenderStateSet( rwRENDERSTATEALPHATESTFUNCTIONREF, (void *)0xD0 );
			RwRenderStateSet( rwRENDERSTATEALPHATESTFUNCTION, (void *)rwALPHATESTFUNCTIONGREATEREQUAL );

			RwD3D9SetRenderState( D3DRS_ZENABLE, TRUE );
		}
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: RenderWireFrame
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CAtomicEntity::RenderWireFrame( void )
{
	if( m_WireFrameRenderInfo.pVertices == NULL )
	{
		return FALSE;
	}

	RwRenderStateSet( rwRENDERSTATETEXTURERASTER, NULL );
	RwRenderStateSet( rwRENDERSTATEZTESTENABLE, (void *)TRUE );
	RwRenderStateSet( rwRENDERSTATEZWRITEENABLE, (void *)TRUE );

	const RwInt32	iDispatchSize	= 1024;
	RwInt32			iRenderingCount	= 0;
	RwInt32			iRemainCount	= m_WireFrameRenderInfo.iVertexCount;
	RwMatrix *		pLocalTransform	= RwFrameGetLTM( RpAtomicGetFrame( GetRpAtomic() ) );

	while( iRemainCount > iDispatchSize )
	{
		if( RwIm3DTransform( &m_WireFrameRenderInfo.pVertices[iRenderingCount], iDispatchSize, pLocalTransform, 0 ) )
		{
			RwIm3DRenderPrimitive( rwPRIMTYPELINELIST );

			RwIm3DEnd();
		}

		iRemainCount	-= iDispatchSize;
		iRenderingCount	+= iDispatchSize;
	}

	if( RwIm3DTransform( &m_WireFrameRenderInfo.pVertices[iRenderingCount], iRemainCount, pLocalTransform, 0 ) )
	{
		RwIm3DRenderPrimitive( rwPRIMTYPELINELIST );

		RwIm3DEnd();
	}


	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: RenderIndicator
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CAtomicEntity::RenderIndicator( void )
{
	SImediateRenderInfo		RenderInfo[3];
	RwUInt32				iAxis;
	RwV2d					Size;

	RwRenderStateSet( rwRENDERSTATETEXTURERASTER, NULL );
	RwRenderStateSet( rwRENDERSTATEZTESTENABLE, (void *)TRUE );
	RwRenderStateSet( rwRENDERSTATEZWRITEENABLE, (void *)TRUE );

	Size.x	= 1.0f;
	Size.y	= 4.0f;

	// Create

	for( iAxis = 0; iAxis < 3; ++iAxis )
	{
		RenderInfo[iAxis].iVertexCount = 30;
		RenderInfo[iAxis].pVertices = (RwIm3DVertex *)RwMalloc( RenderInfo[iAxis].iVertexCount * sizeof( RwIm3DVertex ), rwID_NAOBJECT );

		CreateCone( &RenderInfo[iAxis], &Size, iAxis );
	}

	SLocation3D		Scale( *(GetParent()->GetScale()) );

	RwMatrix		Local;
	RwMatrix		Global;
	RwV3d			Distance;

//	RwMatrix *		pMatrix = RwFrameGetLTM( RpAtomicGetFrame( GetRpAtomic() ) );

	RwMatrixRotate( &Global, &m_Up, GetParent()->GetPosition()->GetRotation()->GetYaw(), rwCOMBINEREPLACE );
	RwMatrixRotate( &Global, &m_Left, GetParent()->GetPosition()->GetRotation()->GetPitch(), rwCOMBINEPOSTCONCAT );
	RwMatrixRotate( &Global, &m_Front, GetParent()->GetPosition()->GetRotation()->GetRoll(), rwCOMBINEPOSTCONCAT );
	RwMatrixTranslate( &Global, GetParent()->GetPosition()->GetLocation()->GetConstRwV3d(), rwCOMBINEPOSTCONCAT );

	// Upper

	Distance.x = 0.0f;
	Distance.y = Scale.GetY() * m_BBox.sup.y;
	Distance.z = 0.0f;

	RwMatrixTranslate( &Local, &Distance, rwCOMBINEREPLACE );
	RwMatrixTransform( &Local, &Global, rwCOMBINEPOSTCONCAT );
	RenderCone( &RenderInfo[1], &Local );

	// Lower

	Distance.x = 0.0f;
	Distance.y = -Scale.GetY() * m_BBox.inf.y;
	Distance.z = 0.0f;

	RwMatrixTranslate( &Local, &Distance, rwCOMBINEREPLACE );
	RwMatrixRotate( &Local, &AXIS_X, 180.0f, rwCOMBINEPOSTCONCAT );
	RwMatrixTransform( &Local, &Global, rwCOMBINEPOSTCONCAT );
	RenderCone( &RenderInfo[1], &Local );

	// Left

	Distance.x = Scale.GetX() * m_BBox.sup.x;
	Distance.y = 0.0f;
	Distance.z = 0.0f;

	RwMatrixTranslate( &Local, &Distance, rwCOMBINEREPLACE );
	RwMatrixTransform( &Local, &Global, rwCOMBINEPOSTCONCAT );

	RenderCone( &RenderInfo[0], &Local );

	// Right

	Distance.x = -Scale.GetX() * m_BBox.inf.x;
	Distance.y = 0.0f;
	Distance.z = 0.0f;

	RwMatrixTranslate( &Local, &Distance, rwCOMBINEREPLACE );
	RwMatrixRotate( &Local, &AXIS_Z, 180.0f, rwCOMBINEPOSTCONCAT );
	RwMatrixTransform( &Local, &Global, rwCOMBINEPOSTCONCAT );

	RenderCone( &RenderInfo[0], &Local );

	// Front

	Distance.x = 0.0f;
	Distance.y = 0.0f;
	Distance.z = Scale.GetZ() * m_BBox.sup.z;

	RwMatrixTranslate( &Local, &Distance, rwCOMBINEREPLACE );
	RwMatrixTransform( &Local, &Global, rwCOMBINEPOSTCONCAT );

	RenderCone( &RenderInfo[2], &Local );

	// Back

	Distance.x = 0.0f;
	Distance.y = 0.0f;
	Distance.z = -Scale.GetZ() * m_BBox.inf.z;

	RwMatrixTranslate( &Local, &Distance, rwCOMBINEREPLACE );
	RwMatrixRotate( &Local, &AXIS_Y, 180.0f, rwCOMBINEPOSTCONCAT );
	RwMatrixTransform( &Local, &Global, rwCOMBINEPOSTCONCAT );

	RenderCone( &RenderInfo[2], &Local );

	// Destroy

	for( iAxis = 0; iAxis < 3; ++iAxis )
	{
		RwFree( RenderInfo[iAxis].pVertices );

		RenderInfo[iAxis].pVertices = NULL;
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: Select
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwUInt32 CAtomicEntity::Select( void )
{
	if( m_bSelected == FALSE )
	{
		if( IsTerrain() != FALSE )
		{
			ApplyColorToBounding( &m_BoundingColorForSelectedTerrain );
			ApplyColorToWireFrame( &m_WireFrameColorForSelectedTerrain );
		}
		else
		{
			ApplyColorToBounding( &m_BoundingColorForSelectedObject );
			ApplyColorToWireFrame( &m_WireFrameColorForSelectedObject );
		}
	}

	return CEntity::Select();
}


//-----------------------------------------------------------------------------
// Name			: Unselect
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwUInt32 CAtomicEntity::Unselect( void )
{
	if( m_bSelected != FALSE )
	{
		if( IsTerrain() != FALSE )
		{
			ApplyColorToBounding( &m_BoundingColorForTerrain );
			ApplyColorToWireFrame( &m_WireFrameColorForTerrain );
		}
		else
		{
			ApplyColorToBounding( &m_BoundingColorForObject );
			ApplyColorToWireFrame( &m_WireFrameColorForObject );
		}
	}

	return CEntity::Unselect();
}


//-----------------------------------------------------------------------------
// Name			: RenderBounding
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CAtomicEntity::RenderBounding( void )
{
	if( m_BoundingRenderInfo.pVertices == NULL )
	{
		return FALSE;
	}

	RwRenderStateSet( rwRENDERSTATETEXTURERASTER, NULL );
	RwRenderStateSet( rwRENDERSTATEZTESTENABLE, (void *)TRUE );
	RwRenderStateSet( rwRENDERSTATEZWRITEENABLE, (void *)TRUE );

	RwMatrix *			pLocalTrasform = RwFrameGetLTM( RpAtomicGetFrame( GetRpAtomic() ) );

	static RwImVertexIndex INDICES[24] =
	{
		0, 1, 1, 3, 3, 2, 2, 0, 4, 5, 5, 7,
		7, 6, 6, 4, 0, 4, 1, 5, 2, 6, 3, 7
	};

	if( RwIm3DTransform( m_BoundingRenderInfo.pVertices, 8, pLocalTrasform, rwIM3D_ALLOPAQUE ) )
    {
        RwIm3DRenderIndexedPrimitive( rwPRIMTYPELINELIST, INDICES, 24 );

        RwIm3DEnd();
    }

	return TRUE;
}


//*****************************************************************************
//
// End of File : CAtomicEntity_Render.cpp
//
//*****************************************************************************