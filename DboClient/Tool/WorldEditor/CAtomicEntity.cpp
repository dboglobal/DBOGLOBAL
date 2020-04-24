//*****************************************************************************
// File       : CAtomicEntity.cpp
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

#include "CFixedString.h"
#include "CFixedStringList.h"

#include "CEntityContainer.h"
#include "CEntityFactory.h"
#include "CEntityResource.h"

#include "Collision.h"
#include "CEntityCollisionList.h"

#include "CCameraEntity.h"
#include "CLightEntity.h"
#include "CMeshEntity.h"

#include "CEntityCollision.h"
#include "CSceneManager.h"
#include "CEditorApplication.h"

#include "CAtomicEntity.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: 
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CAtomicEntity::Initialize( void )
{
	CEntity::Initialize();

	m_Front.Set( &AXIS_Z );
	m_Up.Set( &AXIS_Y );
	m_Left.Set( &AXIS_X );

	ZeroMemory( &m_WireFrameRenderInfo, sizeof( SImediateRenderInfo ) );
	ZeroMemory( &m_BoundingRenderInfo, sizeof( SImediateRenderInfo ) );

	m_pTie		= NULL;
	ZeroMemory( m_pUVAnimationMatrix, sizeof( RwMatrix * ) * E_UV_ANIMATION_LAYER_COUNT );

	SetUseLightMap( TRUE );

	return;
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CAtomicEntity::Create( const CEntityProperty * pProperty, SEntityCreateProperty * pCreateProperty )
{
	BEGIN_FUNCTION( "CAtomicEntity::Create" );

	Assert( pProperty != NULL );
	Assert( pProperty->GetType() == E_ENTITY_TYPE_ATOMIC );

	END_FUNCTION( CEntity::Create( pProperty, pCreateProperty ) );
}


//-----------------------------------------------------------------------------
// Name			: PostCreate
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CAtomicEntity::PostCreate( void )
{
	if( GetParent() == NULL || GetParent()->GetType() != E_ENTITY_TYPE_SKY )
	{
		CreateWireFrame();
	}

	CreateInternalMeshEntity();

	if( HasUVAnimation() != FALSE )
	{
		if( CreateUVAnimation() == FALSE )
		{
			return FALSE;
		}
	}

	if( GetUseLightMap() != FALSE )
	{
		RwUInt32	iAtomicFlag = RtLtMapAtomicGetFlags( GetRpAtomic() );

		iAtomicFlag |= rtLTMAPOBJECTVERTEXLIGHT;
		iAtomicFlag &= ~rtLTMAPOBJECTLIGHTMAP;

		RtLtMapAtomicSetFlags( GetRpAtomic(), iAtomicFlag );

		RpAtomic *		pAtomic = GetRpAtomic();
		RpGeometry *	pGeometry = RpAtomicGetGeometry( pAtomic );
		if( pGeometry != NULL )
		{
			if( IsTerrain() != FALSE )
			{
				RwUInt32	iFlags = RpGeometryGetFlags( pGeometry );
				iFlags &= ~rpGEOMETRYNORMALS;
				iFlags &= ~rpGEOMETRYLIGHT;
				RpGeometrySetFlags( pGeometry, iFlags );
			}
		}
	}

	return CEntity::PostCreate();
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CAtomicEntity::Destroy( void )
{
	BEGIN_FUNCTION( "CAtomicEntity::Destroy" );

	if( GetCategory() == E_ENTITY_CATEGORY_EXTERNAL )
	{
		DestroyWireFrame();
	}

	CEntity::Destroy();

	END_FUNCTION_VOID();
}


//-----------------------------------------------------------------------------
// Name			: PreDestroy
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CAtomicEntity::PreDestroy( void )
{
	CEntity::PreDestroy();

	if( HasUVAnimation() != FALSE )
	{
		DestroyUVAnimation();
	}

	DestroyWireFrame();

	return;
}


//-----------------------------------------------------------------------------
// Name			: UpdateFrame
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CAtomicEntity::UpdateFrame( void )
{
	BEGIN_FUNCTION( "CAtomicEntity::UpdateFrame" );

	if( IsModified() != FALSE )
	{
		if( GetCategory() == E_ENTITY_CATEGORY_EXTERNAL )
		{
			if( IsTerrain() == FALSE )
			{
				RwFrame *		pFrame = const_cast<RwFrame *>(GetRwFrame());
				RwMatrix		Matrix;
				RwMatrix		MatrixLocal;

				RwMatrixRotate( &Matrix, &AXIS_Y, GetPosition()->GetRotation()->GetYaw(), rwCOMBINEREPLACE );
				RwMatrixCopy( &MatrixLocal, &Matrix );

				RwV3dTransformVector( &m_Front, &AXIS_Z, &Matrix );
				RwV3dTransformVector( &m_Left, &AXIS_X, &Matrix );

				RwMatrixRotate( &Matrix, &m_Left, GetPosition()->GetRotation()->GetPitch(), rwCOMBINEREPLACE );
				RwMatrixMultiply( &MatrixLocal, &MatrixLocal, &Matrix );

				RwV3dTransformVector( &m_Up, &AXIS_Y, &Matrix );
				RwV3dTransformVector( &m_Front, &m_Front, &Matrix );

				RwMatrixRotate( &Matrix, &m_Front, GetPosition()->GetRotation()->GetRoll(), rwCOMBINEREPLACE );
				RwMatrixMultiply( &MatrixLocal, &MatrixLocal, &Matrix );

				RwV3dTransformVector( &m_Up, &m_Up, &Matrix );
				RwV3dTransformVector( &m_Left, &m_Left, &Matrix );

				RwV3dNormalize( &m_Direction, &m_Front );

				RwFrameTransform( pFrame, &MatrixLocal, rwCOMBINEREPLACE );
				pFrame = RwFrameTranslate( pFrame, GetPosition()->GetLocation()->GetRwV3d(), rwCOMBINEPOSTCONCAT );

				RwFrameUpdateObjects( pFrame );
			}
		}
	}

	SetVisible( DetermineVisiblilty() );

	END_FUNCTION( CEntity::UpdateFrame() );
}


//-----------------------------------------------------------------------------
// Name			: RenderFrame
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CAtomicEntity::RenderFrame( CEntity * pRenderer )
{
	BEGIN_FUNCTION( "CAtomicEntity::RenderFrame" );

	if( pRenderer->GetFrameID() <= GetFrameID() )
	{
		END_FUNCTION( TRUE );
	}
	else
	{
		SetFrameID( pRenderer->GetFrameID() );
	}

	if( IsVisible() == FALSE )
	{
		END_FUNCTION( TRUE );
	}

	if( IsSelected() != FALSE )
	{
		if( IsTerrain() != FALSE )
		{
			if( IsRenderSolidForSelectedTerrain() != FALSE )
			{
				RenderSolid();
			}

			if( IsRenderWireFrameForSelectedTerrain() != FALSE )
			{
				RenderWireFrame();
			}

			if( IsRenderBoundingForSelectedTerrain() != FALSE )
			{
				RenderBounding();
			}
		}
		else
		{
			if( IsRenderSolidForSelectedObject() != FALSE )
			{
				RenderSolid();
			}

			if( IsRenderWireFrameForSelectedObject() != FALSE )
			{
				RenderWireFrame();
			}

			if( IsRenderBoundingForSelectedObject() != FALSE )
			{
				RenderBounding();
			}
		}

	//	RenderIndicator();
	}
	else
	{
		if( IsTerrain() != FALSE )
		{
			if( IsRenderSolidForTerrain() != FALSE )
			{
				RenderSolid();
			}

			if( IsRenderWireFrameForTerrain() != FALSE )
			{
				RenderWireFrame();
			}

			if( IsRenderBoundingForTerrain() != FALSE )
			{
				RenderBounding();
			}
		}
		else
		{
			if( IsRenderSolidForObject() != FALSE )
			{
				RenderSolid();
			}

			if( IsRenderWireFrameForObject() != FALSE )
			{
				RenderWireFrame();
			}

			if( IsRenderBoundingForObject() != FALSE )
			{
				RenderBounding();
			}
		}
	}

	g_pSceneManager->IncreaseRenderCountAtomic();

	END_FUNCTION( CEntity::RenderFrame( pRenderer ) );
}



//-----------------------------------------------------------------------------
// Name			: Attach
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CAtomicEntity::Attach( CEntity * pChild )
{
	Assert( pChild != NULL );

	if( InsertChild( pChild ) == FALSE )
	{
		return FALSE;
	}

	// Link To World

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: Detach
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CAtomicEntity::Detach( CEntity * pChild )
{
	Assert( pChild != NULL );
	Assert( m_pChildren != NULL );

	if( RemoveChild( pChild ) == FALSE )
	{
		return FALSE;
	}

	// Unlink To World

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: RenderPrimitive
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CAtomicEntity::RenderPrimitive( void )
{
	BEGIN_FUNCTION( "CAtomicEntity::RenderPrimitive" );

	RpAtomicRender( GetRpAtomic() );

	END_FUNCTION( TRUE );
}


//-----------------------------------------------------------------------------
// Name			: SetRwFrame
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CAtomicEntity::SetRwFrame( const RwFrame * pRwFrame )
{
	if( GetRpAtomic() == NULL )
	{
		return;
	}

	RpAtomicSetFrame( GetRpAtomic(), const_cast<RwFrame *>(pRwFrame) );

	return;
}


//-----------------------------------------------------------------------------
// Name			: GetRwFrame
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwFrame * CAtomicEntity::GetRwFrame( void ) const
{
	if( GetRpAtomic() == NULL )
	{
		return NULL;
	}

	return RpAtomicGetFrame( GetRpAtomic() );
}


//-----------------------------------------------------------------------------
// Name			: IsPointInBounding
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CAtomicEntity::IsPointInBounding( const SLocation3D * pLocation ) const
{
	const RwSphere *	pBoundingSphere = &(GetRpAtomic()->boundingSphere);

	RwReal	fDistanceSquared = ( ( pBoundingSphere->center.x - pLocation->GetX() ) * ( pBoundingSphere->center.x - pLocation->GetX() ) +
								( pBoundingSphere->center.y - pLocation->GetY() ) * ( pBoundingSphere->center.y - pLocation->GetY() ) +
								( pBoundingSphere->center.z - pLocation->GetZ() ) * ( pBoundingSphere->center.z - pLocation->GetZ() ) );

	return ( ( fDistanceSquared > ( pBoundingSphere->radius * pBoundingSphere->radius ) ) ? FALSE : TRUE );
}



//-----------------------------------------------------------------------------
// Name			: CalculateBounding
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CAtomicEntity::CalculateBounding( void )
{
	if( GetRpAtomic() == NULL )
	{
		ZeroMemory( &m_sBounding, sizeof( RwSphere ) );
	}
	else
	{
		memcpy( &m_sBounding, &(GetRpAtomic()->boundingSphere), sizeof( RwSphere ) );
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: DetectCollision
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CAtomicEntity::DetectCollision( CCollisionDetectionInfo * pCollisionInfo ) const
{
//	RwBool		bResult = TRUE;
//	RwBool		bCollide = TRUE;

	pCollisionInfo->SetRpAtomic( GetRpAtomic() );

	if( IsTerrain() != FALSE )
	{
		switch( pCollisionInfo->GetCondition()->GetTerrainDetectionLevel() )
		{
			case E_COLLISION_DETECTION_LEVEL_MESH:
			{
				RpAtomicForAllIntersections( GetRpAtomic(), const_cast<RpIntersection *>(pCollisionInfo->GetCondition()->GetIntersection()), DetectPositionInAtomicCB, pCollisionInfo );

				return TRUE;
			}

			case E_COLLISION_DETECTION_LEVEL_BOUNDING:
			{
				CEntityCollision	sNewResult;
				sNewResult.SetEntity( const_cast<CAtomicEntity *>(this) );
				sNewResult.SetLocation( GetPosition()->GetLocation() );

				RwV3d				sDistaceVector;

				GetLocationFromIntersection( &sDistaceVector, const_cast<RpIntersection *>(pCollisionInfo->GetCondition()->GetIntersection()) );

				RwV3dSubMacro( &sDistaceVector, GetPosition()->GetLocation(), &sDistaceVector );

				RwReal				fDistance = RwV3dLength( &sDistaceVector );

				sNewResult.SetDistance( fDistance );

				pCollisionInfo->GetResultList()->Insert( &sNewResult );

				return TRUE;
			}

			default:
			{
				return TRUE;
			}
		}
	}
	else
	{
		switch( pCollisionInfo->GetCondition()->GetObjectDetectionLevel() )
		{
			case E_COLLISION_DETECTION_LEVEL_MESH:
			{
				RpAtomicForAllIntersections( GetRpAtomic(), const_cast<RpIntersection *>(pCollisionInfo->GetCondition()->GetIntersection()), DetectPositionInAtomicCB, pCollisionInfo );

				return TRUE;
			}

			case E_COLLISION_DETECTION_LEVEL_BOUNDING:
			{
				CEntityCollision	sNewResult;
				sNewResult.SetEntity( const_cast<CAtomicEntity *>(this) );
				sNewResult.SetLocation( GetParent()->GetPosition()->GetLocation() );

				RwV3d				sDistaceVector;

				GetLocationFromIntersection( &sDistaceVector, const_cast<RpIntersection *>(pCollisionInfo->GetCondition()->GetIntersection()) );

				RwV3dSubMacro( &sDistaceVector, GetPosition()->GetLocation(), &sDistaceVector );

				RwReal				fDistance = RwV3dLength( &sDistaceVector );

				fDistance -= GetBounding()->radius;
				if( fDistance < 0.0f )
				{
					fDistance = 0.0f;
				}

				sNewResult.SetDistance( fDistance );

				pCollisionInfo->GetResultList()->Insert( &sNewResult );

				return TRUE;
			}

			default:
			{
				return TRUE;
			}
		}
	}
}


//-----------------------------------------------------------------------------
// Name			: CreateInternalMeshEntity
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwInt32 CAtomicEntity::CreateInternalMeshEntity( void )
{
	RpAtomic *		pRpAtomc = GetRpAtomic();
	if( pRpAtomc == NULL )
	{
		return 0;
	}

	RpGeometry *		pRpGeometry = pRpAtomc->geometry;
	if( pRpGeometry == NULL )
	{
		return 0;
	}

	RpMeshHeader *		pRpMeshHeader = pRpGeometry->mesh;
	if( pRpMeshHeader == NULL || pRpMeshHeader->numMeshes == 0 )
	{
		return 0;
	}

	return CMeshEntity::CreateInternalMeshFromMeshHeader( this, pRpMeshHeader );
}


//-----------------------------------------------------------------------------
// Name			: Export
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwInt32 CAtomicEntity::Export( CFixedStringList * pStringList, CFixedString * pName )
{
	if( GetCategory() == E_ENTITY_CATEGORY_INTERNAL )
	{
		return ExportChild( pStringList, pName );
	}

	if( ExportHeader( pStringList, pName ) == FALSE )
	{
		return -1;
	}

	CFixedString	String;

	// Name

	String.Assign( *(GetAtomicProperty()->GetName()) );
	pStringList->Push( &String );

	if( GetAtomicProperty()->IsTerrain() != FALSE )
	{
		String.Assign( "#terrain" );
		pStringList->Push( &String );

		m_pResource->Export( GetProperty()->GetName() );
	}

	if( GetAtomicProperty()->IsSky() != FALSE )
	{
		String.Assign( "#sky" );
		pStringList->Push( &String );
	}

	if( GetAtomicProperty()->IsAntiFog() != FALSE )
	{
		String.Assign( "#antifog" );
		pStringList->Push( &String );
	}

	if( GetAtomicProperty()->GetUVAnimationLayer() > 0 )
	{
		String.Assign( "#uva2" );
		pStringList->Push( &String );
	}

	// Position

	RwBool		bWriteLocation;
	RwBool		bWriteRotation;
	RwBool		bWriteScale;

	if( GetScale()->IsEqual( SLocation3D::SCALE_DEFAULT ) == FALSE )
	{
		bWriteScale		= TRUE;
		bWriteRotation	= TRUE;
		bWriteLocation	= TRUE;
	}
	else
	{
		bWriteScale		= FALSE;

		if( GetPosition()->GetRotation()->IsEqual( SRotation3D::ROTATION_DEFAULT ) == FALSE )
		{
			bWriteRotation	= TRUE;
			bWriteLocation	= TRUE;
		}
		else
		{
			bWriteRotation	= FALSE;

			if( GetPosition()->GetLocation()->IsEqual( SLocation3D::LOCATION_DEFAULT ) == FALSE )
			{
				bWriteLocation	= TRUE;
			}
			else
			{
				bWriteLocation	= FALSE;
			}
		}
	}

	// Location

	if( bWriteLocation != FALSE )
	{
		String.AssignFloat( GetPosition()->GetLocation()->GetX() );
		pStringList->Push( &String );

		String.AssignFloat( GetPosition()->GetLocation()->GetY() );
		pStringList->Push( &String );

		String.AssignFloat( GetPosition()->GetLocation()->GetZ() );
		pStringList->Push( &String );
	}

	// Rotation

	if( bWriteRotation != FALSE )
	{
		String.AssignFloat( GetPosition()->GetRotation()->GetYaw() );
		pStringList->Push( &String );

		String.AssignFloat( GetPosition()->GetRotation()->GetPitch() );
		pStringList->Push( &String );

		String.AssignFloat( GetPosition()->GetRotation()->GetRoll() );
		pStringList->Push( &String );
	}

	// Scale

	if( bWriteScale != FALSE )
	{
		String.AssignFloat( GetScale()->GetX() );
		pStringList->Push( &String );

		String.AssignFloat( GetScale()->GetY() );
		pStringList->Push( &String );

		String.AssignFloat( GetScale()->GetZ() );
		pStringList->Push( &String );
	}

	String.Assign( RWSTRING( "\r\n" ) );
	pStringList->Push( &String );

	RwInt32			nExportCount = ExportChild( pStringList, pName );
	if( nExportCount < 0 )
	{
		return nExportCount;
	}

	return ++nExportCount;
}


//*****************************************************************************
//
// End of File : CAtomicEntity.cpp
//
//*****************************************************************************