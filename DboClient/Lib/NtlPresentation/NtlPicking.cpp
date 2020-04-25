#pragma warning ( disable : 4819 )

#include "precomp_ntlpresentation.h"
#include <d3d9.h>
#include <list>
#include <unordered_map>
#include <vector>
#include "rwcore.h"
#include "rpworld.h"
#include "rtpick.h"
#include "rpcollis.h"
#include "NtlPLEntity.h"
#include "NtlAtomic.h"
#include "NtlMath.h"
#include "NtlCollis.h"
#include "NtlPicking.h"
#include "NtlPLObject.h"
#include "NtlPLObjectProperty.h"


//////////////////////////////////////////////////////////////////////////
//
//	CPickEntityPool class
//
//////////////////////////////////////////////////////////////////////////


CNtlPicking::CPickEntityPool::CPickEntityPool( void )
{
	for ( int i = 0; i < eRESIZING_UNIT; ++i )
	{
		m_FreeList.push_back( NTL_NEW SPICK_INFO );
	}
}

CNtlPicking::CPickEntityPool::~CPickEntityPool( void )
{
	listdef_PICK_INFO_LIST::iterator it = m_FreeList.begin();
	for ( ; it != m_FreeList.end(); ++it )
	{
		NTL_DELETE( *it );
	}
	m_FreeList.clear();
}

CNtlPicking::SPICK_INFO* CNtlPicking::CPickEntityPool::New( void )
{
	if ( m_FreeList.empty() )
	{
		for ( int i = 0; i < eRESIZING_UNIT; ++i )
		{
			m_FreeList.push_back( NTL_NEW SPICK_INFO );
		}
	}

	listdef_PICK_INFO_LIST::iterator it = m_FreeList.begin();

	CNtlPicking::SPICK_INFO* pEntity = *it;
	pEntity->Init();

	m_FreeList.erase( it );

	return pEntity;
}

void CNtlPicking::CPickEntityPool::Delete( CNtlPicking::SPICK_INFO*& pEntity )
{
	if ( NULL == pEntity ) return;

	m_FreeList.push_front( pEntity );
	pEntity = NULL;
}

void CNtlPicking::CPickEntityPool::Delete( CNtlPicking::hashdef_PICK_INFO_LIST* pEntityList )
{
	hashdef_PICK_INFO_LIST::iterator it = pEntityList->begin();
	for ( ; it != pEntityList->end(); ++it )
	{
		m_FreeList.push_front( it->second );
	}
	pEntityList->clear();
}


//////////////////////////////////////////////////////////////////////////
//
//	CNtlPicking class
//
//////////////////////////////////////////////////////////////////////////


RwUInt16 CNtlPicking::s_ID = 0;
RwUInt16 CNtlPicking::s_RenderID = 0;
RwReal CNtlPicking::s_fMinDist = 0.f;

CNtlPicking::CNtlPicking( void )
{
	m_pPickRaster = NULL;
	m_pPickZRaster = NULL;
	m_pPickTexture = NULL;
	m_pPickPipeline = NULL;

	m_pPickEntityPool = NULL;

	m_bPickCharacter = false;
	m_bCharacterExclusion = false;
}

CNtlPicking::~CNtlPicking( void )
{
	Delete();
}

bool CNtlPicking::Create( void )
{
	m_pPickEntityPool = NTL_NEW CPickEntityPool;
	if ( NULL == m_pPickEntityPool ) goto Error;

	m_pPickEntityList = NTL_NEW CPickEntityList;
	if ( NULL == m_pPickEntityList ) goto Error;

	if ( !CreateRaster( m_pPickRaster, m_pPickZRaster, m_pPickTexture ) )
	{
		goto Error;
	}

	m_pPickPipeline = CreatePipeLine();
	if ( NULL == m_pPickPipeline ) goto Error;

	return true;

Error:

	Delete();

	return false;
}

void CNtlPicking::Delete( void )
{
	if ( m_pPickEntityPool && m_pPickEntityList )
	{
		m_pPickEntityList->Clear( m_pPickEntityPool );
	}

	DeletePipeLine( m_pPickPipeline );

	DeleteRaster( m_pPickRaster, m_pPickZRaster, m_pPickTexture );

	NTL_DELETE( m_pPickEntityList );
	NTL_DELETE( m_pPickEntityPool );
}

bool CNtlPicking::Pick( RwCamera* pCamera, RpWorld* pWorld, RwV2d& v2ScrPos, RwReal fPickDist, SWorldPickInfo& sPickInfo )
{
	static RwRGBA cBackGround = { 255, 255, 255, 255 };

	// The default camera must be passed to the parameter
	if ( NULL == pCamera ) return false;

	// World must exist
	if ( NULL == pWorld ) return false;

	// Picked in 2D Returns if the position exceeds the client's area
	RwReal fScrWidth = (RwReal)RwRasterGetWidth( RwCameraGetRaster( pCamera ) );
	RwReal fScrHeight = (RwReal)RwRasterGetHeight( RwCameraGetRaster( pCamera ) );
	if ( v2ScrPos.x < 0 || v2ScrPos.x >= fScrWidth ) return false;
	if ( v2ScrPos.y < 0 || v2ScrPos.y >= fScrHeight ) return false;

    m_bCharacterExclusion = sPickInfo.bCharacterExclusion ? true : false;

	// Picking ray를 계산한다
	RwLine PickRay;
	RwCameraCalcPixelRay( pCamera, &PickRay, &v2ScrPos );

	RwV3d PickDir;
	RwV3dSubMacro( &PickDir, &PickRay.end, &PickRay.start );
	RwV3dNormalize( &PickDir, &PickDir );

	PickRay.end.x = PickRay.start.x + fPickDist * PickDir.x;
	PickRay.end.y = PickRay.start.y + fPickDist * PickDir.y;
	PickRay.end.z = PickRay.start.z + fPickDist * PickDir.z;

	//////////////////////////////////////////////////////////////////////////
	//
	// Bounding box check routine << Retrieves characters that are in conflict with Ray >>
	//
	//////////////////////////////////////////////////////////////////////////

	// Initialize the previously picked atomic list
	m_bPickCharacter = false;
	m_pPickEntityList->Clear( m_pPickEntityPool );

	// Initialize the ID
	s_ID = 0;

	RpIntersection Inters;
	Inters.type = rpINTERSECTLINE;
	Inters.t.line.start = PickRay.start;
	Inters.t.line.end = PickRay.end;

	s_fMinDist = fPickDist;

	m_v3CamPos = PickRay.start;
	NtlWorldForAllAtomicIntersections_CustomBBox( pWorld, &Inters, ResizeBoundingSphereCB, this, PickAtomicBoundBoxCB, this );

	// return false if list is empty
	if ( m_pPickEntityList->IsEmpty() ) 
		return false;

	// Sort the objects
	m_pPickEntityList->DepthSort();

	// Clipping the pick list to the maximum number of atomic numbers to render.
	m_pPickEntityList->ClippingRenderingEntity( MAX_NUM_OF_MAX_ATOMIC );

	RwUInt16 uiRenderID = 0xffff;

	// Rendering picking is performed only when the character's bounding picking is detected
	if ( m_bPickCharacter )
	{
		//////////////////////////////////////////////////////////////////////////
		//
		// Polygon check routine << Bounding box와 충돌된 Atomic만 렌더링한다 >>
		//
		//////////////////////////////////////////////////////////////////////////

		RwCamera* pOldCamera = RwCameraGetCurrentCamera();

		if ( pOldCamera )
		{
			RwCameraEndUpdate( pOldCamera );
		}

		// Polygon rendering 시작
		UpdateCamera_Begin( pCamera );

		// Initialize to an invalid PLEntity value
		if (RwCameraClear( pCamera, &cBackGround, rwCAMERACLEARIMAGE ))
		{
			// Render atomic detected BBox collision at the top
			if ( RwCameraBeginUpdate( pCamera ) )
			{
				// Project matrix 조작
				D3DXMATRIX matOldProj;
				RwD3D9GetTransform( D3DTS_PROJECTION, &matOldProj );

				D3DXMATRIX matC;
				D3DXMatrixIdentity( &matC );
				matC.m[3][0] = -1 - 2 * (v2ScrPos.x / fScrWidth - 0.5f);
				matC.m[3][1] = 1 + 2 * (v2ScrPos.y / fScrHeight - 0.5f);

				D3DXMATRIX matProj;
				D3DXMatrixMultiply( &matProj, &matOldProj, &matC );

				RwD3D9SetTransform( D3DTS_PROJECTION, &matProj );

				// Render atomics
				RenderPickedAtoms( m_pPickPipeline );

				// Project matrix 복구
				RwD3D9SetTransform( D3DTS_PROJECTION, &matOldProj );

				RwCameraEndUpdate( pCamera );
			}
		}

		// Polygon rendering 종료 
		UpdateCamera_End( pCamera );

		if (pOldCamera)
		{
			RwCameraBeginUpdate(pOldCamera);
		}

		//////////////////////////////////////////////////////////////////////////
		//
		// 해당 Rasterize를 대상으로 충돌 검사
		//
		//////////////////////////////////////////////////////////////////////////

		RwRaster* pRaster = m_pPickTexture->raster;
		if ( NULL == pRaster ) return false;

		RwUInt8* pColor = RwRasterLock( pRaster, 0, rwRASTERLOCKREAD|rwRASTERLOCKRAW );
		if ( pColor )
		{
			uiRenderID = *((RwUInt16*)(pColor));

			RwRasterUnlock( pRaster );
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// 최상의 Picking entity를 찾는다
	//
	//////////////////////////////////////////////////////////////////////////

	SPICK_INFO* pPickInfo;
	vecdef_PICK_INFO_LIST* pPickInfoList = &m_pPickEntityList->m_defSortedInfoList;
	RwInt32 iCnt = pPickInfoList->size();

	// If there is rendering picking information
	if ( 0xffff != uiRenderID )
	{
		for ( RwInt32 i = 0; i < iCnt; ++i )
		{
			pPickInfo = (*pPickInfoList)[i];

			// disabled by daneos to fix fetching characters with mouse
			/*if (!pPickInfo->bRenderPolygonCheck)
			{
				sPickInfo.pPLEntity = pPickInfo->pPLEntity;
				sPickInfo.fMinDist = pPickInfo->fDist;
				sPickInfo.pAtomic = pPickInfo->pAtomic;
				sPickInfo.vPickPos = pPickInfo->v3PickPos;

				return true;
			}*/

			// In case of picking of character related entities
			if (pPickInfo->uiID == uiRenderID)
			{
				sPickInfo.pPLEntity = pPickInfo->pPLEntity;
				sPickInfo.fMinDist = pPickInfo->fDist;
				sPickInfo.pAtomic = pPickInfo->pAtomic;
				sPickInfo.vPickPos = pPickInfo->v3PickPos;

				return true;
			}

		}
	}
	else
	{
		static const RwInt32 MAX_STEP = 8;
		static RwReal arfDist[MAX_STEP]				= { 0.f,	5.f,	10.f,	25.f,	50.f,	70.f,	120.f,	512.f };
		static RwReal arfOptimizeRadius[MAX_STEP]	= { 0.f,	0.f,	1.5f,	1.6f,	2.6f,	3.5f,	4.5f,	6.f };

		for ( RwInt32 i = 0; i < iCnt; ++i )
		{
			pPickInfo = (*pPickInfoList)[i];

			// When picking object entities
			if ( !pPickInfo->bRenderPolygonCheck )
			{
				sPickInfo.pPLEntity = pPickInfo->pPLEntity;
				sPickInfo.fMinDist = pPickInfo->fDist;
				sPickInfo.pAtomic = pPickInfo->pAtomic;
				sPickInfo.vPickPos = pPickInfo->v3PickPos;

				return true;
			}

			// In case of picking of character related entities
			for ( RwInt32 j = 1; j < MAX_STEP; ++j )
			{
				if ( arfDist[j-1] - pPickInfo->fDist <= 0.0001f && pPickInfo->fDist - arfDist[j] <= 0.0001f )
				{
					if ( pPickInfo->fRadius < arfOptimizeRadius[j] )
					{
						sPickInfo.pPLEntity = pPickInfo->pPLEntity;
						sPickInfo.fMinDist = pPickInfo->fDist;
						sPickInfo.pAtomic = pPickInfo->pAtomic;
						sPickInfo.vPickPos = pPickInfo->v3PickPos;

						return true;
					}
				}
			}
		}
	}

	return false;
}

void CNtlPicking::Render_PickCamera( void )
{
	CNtlPLGlobal::m_RwCamera;

	RwIm2DVertex vert[4];
	vert[0].x = 0.f;
	vert[0].y = (RwReal)RASTER_HEIGHT;
	vert[0].z = 0.f;
	vert[0].rhw = 1.f;
	vert[0].emissiveColor = 0xffffffff;
	vert[0].u = 0.f;
	vert[0].v = 1.f;

	vert[1].x = 0.f;
	vert[1].y = 0.f;
	vert[1].z = 0.f;
	vert[1].rhw = 1.f;
	vert[1].emissiveColor = 0xffffffff;
	vert[1].u = 0.f;
	vert[1].v = 0.f;

	vert[2].x = (RwReal)RASTER_WIDTH;
	vert[2].y = (RwReal)RASTER_HEIGHT;
	vert[2].z = 0.f;
	vert[2].rhw = 1.f;
	vert[2].emissiveColor = 0xffffffff;
	vert[2].u = 1.f;
	vert[2].v = 1.f;

	vert[3].x = (RwReal)RASTER_WIDTH;
	vert[3].y = 0.f;
	vert[3].z = 0.f;
	vert[3].rhw = 1.f;
	vert[3].emissiveColor = 0xffffffff;
	vert[3].u = 1.f;
	vert[3].v = 0.f;

	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void*)m_pPickTexture->raster );

	LPDIRECT3DDEVICE9 pDev = (LPDIRECT3DDEVICE9)RwD3D9GetCurrentD3DDevice();
	pDev->SetFVF( D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1 );
	pDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vert, sizeof(vert[0]) );
}

void CNtlPicking::SaveAsFile( const char* pFileName )
{
	RwImage* pImage = RwImageCreate( RwRasterGetWidth( m_pPickTexture->raster ), RwRasterGetHeight( m_pPickTexture->raster ), 32 );
	if ( pImage )
	{
		RwImageAllocatePixels( pImage );
		RwImageSetFromRaster( pImage, m_pPickTexture->raster );
		RwImageWrite( pImage, pFileName );
		RwImageDestroy( pImage );
	}
}

const RwSphere* CNtlPicking::ResizeBoundingSphereCB( RpAtomic* pAtomic, void* pData )
{
	static const RwInt32 MAX_STEP = 6;
	static RwReal arfDist[MAX_STEP]				= { 5.f,	10.f,	25.f,	50.f,	70.f,	512.f };
	static RwReal arfOptimizeRadius[MAX_STEP]	= { 0.f,	0.5f,	1.0f,	1.8f,	2.f,	2.4f };

	static const RwSphere* pOriginSphere;
	static RwSphere OptimizeSphere;

	pOriginSphere = RpAtomicGetWorldBoundingSphere( pAtomic );

	// If you need to exclude character-related picking, you do not need to resize the Bounding size.
	if ( ((CNtlPicking*)pData)->m_bCharacterExclusion )
	{
		return pOriginSphere;
	}
	
	CNtlPLEntity* pPLEntity = reinterpret_cast<CNtlPLEntity*>( RpNtlAtomicGetData( pAtomic ) );
	if ( NULL == pPLEntity ||
		 !(PLENTITY_CHARACTER == pPLEntity->GetClassType() || PLENTITY_ITEM == pPLEntity->GetClassType() ) ||
		 !(pPLEntity->GetFlags() & NTL_PLEFLAG_RESIZE_PICKING_BBOX) )
	{
		return pOriginSphere;
	}


	RwV3d* pCamPos = &((CNtlPicking*)pData)->m_v3CamPos;
	RwReal fDist = sqrtf( (pOriginSphere->center.x - pCamPos->x) * (pOriginSphere->center.x - pCamPos->x) +
						  (pOriginSphere->center.y - pCamPos->y) * (pOriginSphere->center.y - pCamPos->y) +
						  (pOriginSphere->center.z - pCamPos->z) * (pOriginSphere->center.z - pCamPos->z) );

	RwReal fOptimizeRadius = pOriginSphere->radius;
	for ( RwInt32 i = 0; i < MAX_STEP; ++i )
	{
		if ( fDist < arfDist[i]  )
		{
			fOptimizeRadius = (0 == i ? pOriginSphere->radius : arfOptimizeRadius[i]);
			break;
		}
	}

	OptimizeSphere.center = pOriginSphere->center;
	OptimizeSphere.radius = fOptimizeRadius > pOriginSphere->radius ? fOptimizeRadius : pOriginSphere->radius;

	((CNtlPicking*)pData)->m_fResizedRadius = OptimizeSphere.radius;

	return &OptimizeSphere;
}

RpAtomic* CNtlPicking::PickAtomicBoundBoxCB( RpIntersection* pInters, RpWorldSector* pWorldSector, RpAtomic* pAtomic, RwReal fDistance, void* pData )
{
	CNtlPicking* pPickObj = (CNtlPicking*) pData;
	CNtlPLEntity* pPLEntity = reinterpret_cast<CNtlPLEntity*>( RpNtlAtomicGetData(pAtomic) );

	if ( pPLEntity )
	{
		// Object의 경우 Polygon 검사를 실행 한다
		if ( pPLEntity->GetClassType() == PLENTITY_OBJECT )
		{
            CNtlPLObject* pObject = (CNtlPLObject*)pPLEntity;
			// Woody: If there is a problem with placement and picking at the same time in the tool, add NTL_PLEFLAG_TOOL_PICK.
			// Alternatively, the tool can randomly check on model tools without picking.
			if(	pPLEntity->IsVisible(0) &&
				(pPLEntity->GetFlags() & ~NTL_PLEFLAG_TOOL_PICK) &&
				((RpNtlAtomicGetFlag(pAtomic) & NTL_COLLISION) || pObject->GetProperty()->IsForcePicking()) &&                    
				(!(RpNtlAtomicGetFlag(pAtomic) & NTL_NOT_VISIBLE)||((RpNtlAtomicGetFlag(pAtomic) & NTL_NOT_VISIBLE) && (RpNtlAtomicGetFlag(pAtomic) & NTL_CAMERA_COLLISION))))
			{
				SPICK_INFO* pPickInfo = pPickObj->m_pPickEntityPool->New();
				pPickInfo->bRenderPolygonCheck = false;
				pPickInfo->pAtomic = pAtomic;

				RpAtomicForAllIntersections( pAtomic, pInters, PickAtomicPolygonCB, pPickInfo );

				if ( pPickInfo->pPLEntity )
				{
					const RwSphere* pCollSphere = RpAtomicGetWorldBoundingSphere( pAtomic );

					pPickInfo->uiID = s_ID;
					pPickInfo->fRadius = pCollSphere->radius;
					pPickInfo->fResizedRadius = pPickObj->m_fResizedRadius;
					pPickObj->m_pPickEntityList->Push( s_ID++, pPickInfo );
				}
				else
				{
					pPickObj->m_pPickEntityPool->Delete( pPickInfo );
				}
			}
		}
		// Executes an Object other than Polygon check
		// However, if character picking is excluded, polygon checking is not performed.
		else if ( !pPickObj->m_bCharacterExclusion )
		{
			if ( pPLEntity->IsVisible(0) &&
				 pPLEntity->GetFlags() & NTL_PLEFLAG_PICKING )
			{
				const RwSphere* pCollSphere = RpAtomicGetWorldBoundingSphere( pAtomic );

				RwV3d vLineVector = pInters->t.line.end - pInters->t.line.start;
				RwReal fLineLength = RwV3dLength( &vLineVector );

				RwV3d vRetPosOfCol = fDistance * vLineVector;
				RwReal fRetDistOfCol = RwV3dLength( &vRetPosOfCol );

				RwV3d vSubVert = pCollSphere->center - pInters->t.line.start - vRetPosOfCol;
				RwReal fDistVert = RwV3dLength( &vSubVert );

				RwReal fDistProj = (fRetDistOfCol - sqrtf( pPickObj->m_fResizedRadius * pPickObj->m_fResizedRadius - fDistVert * fDistVert )) / fLineLength;
				if ( fDistProj < 0.f ) fDistProj = fDistance;

				RwV3d vColDir = fDistProj * vLineVector;
				RwReal fColDist = RwV3dLength( &vColDir );
				RwV3d vColPos = pInters->t.line.start + fDistProj * vLineVector;

				// If it is not an object, it must be included in the list for the polygon check on the entities that follow
				// The reason for reducing the total pickable distance as shown below is that by reducing the picking distance in the case of the object,
				// In order to improve ...
				if (fColDist <= s_fMinDist)
				{
					s_fMinDist = fColDist;
				}

				pPickObj->m_bPickCharacter = true;

				SPICK_INFO* pPickInfo = pPickObj->m_pPickEntityPool->New();
				pPickInfo->bRenderPolygonCheck = true;
				pPickInfo->pAtomic = pAtomic;
				pPickInfo->fDist = fColDist;
				pPickInfo->fRadius = pCollSphere->radius;
				pPickInfo->fResizedRadius = pPickObj->m_fResizedRadius;
				pPickInfo->pPLEntity = pPLEntity;
				RwV3dAssign( &pPickInfo->v3PickPos, &vColPos );

				pPickInfo->uiID = s_ID;

				pPickObj->m_pPickEntityList->Push( s_ID++, pPickInfo );
			}
		}
	}

	return pAtomic;
}

RpCollisionTriangle* CNtlPicking::PickAtomicPolygonCB( RpIntersection* pInters, RpCollisionTriangle* pRpColTri, RwReal fRatio, void* pData )
{
	SPICK_INFO* pPickInfo = (SPICK_INFO*)pData;
	CNtlPLEntity *pPLEntity = reinterpret_cast<CNtlPLEntity*>( RpNtlAtomicGetData(pPickInfo->pAtomic) );

	if ( pPLEntity->GetClassType() == PLENTITY_OBJECT )
	{
		RwLine* pLine = &pInters->t.line;
		RwV3d vCollPos = pLine->start + fRatio * (pLine->end - pLine->start);
		RwV3d vSub = vCollPos - pLine->start;
		RwReal fDist =  RwV3dLength( &vSub );

		if ( fDist < s_fMinDist )
		{
			s_fMinDist = fDist;

			pPickInfo->fDist = fDist;
			pPickInfo->pPLEntity = pPLEntity;
			RwV3dAssign( &pPickInfo->v3PickPos, &vCollPos );
		}
	}

	return pRpColTri;
}

bool CNtlPicking::CreateRaster( RwRaster*& pRaster, RwRaster*& pZRaster, RwTexture*& pTexture )
{
#define CHECK_CAMERA_CREATE_ERROR( p ) if ( NULL == p ) goto Error;

	pRaster = NULL;
	pZRaster = NULL;
	pTexture = NULL;

	CHECK_CAMERA_CREATE_ERROR( (pRaster = RwRasterCreate( RASTER_WIDTH, RASTER_HEIGHT, 0, rwRASTERTYPECAMERATEXTURE )) );
	CHECK_CAMERA_CREATE_ERROR( (pTexture = RwTextureCreate( pRaster )) );

	return true;

Error:

	DeleteRaster( pRaster, pZRaster, pTexture );

	return false;
}

void CNtlPicking::DeleteRaster( RwRaster*& pRaster, RwRaster*& pZRaster, RwTexture*& pTexture )
{
	if ( pRaster ) { RwRasterDestroy( pRaster ); pRaster = NULL; }
	if ( pTexture ) { RwTextureSetRaster( pTexture, NULL ); RwTextureDestroy( pTexture ); pTexture = NULL; }
}

void CNtlPicking::UpdateCamera_Begin( RwCamera* pCamera )
{
	m_pTempRaster = RwCameraGetRaster( pCamera );
	m_pTempZRaster = RwCameraGetZRaster( pCamera );
	RwCameraSetRaster( pCamera, m_pPickRaster );
	RwCameraSetZRaster( pCamera, m_pPickZRaster );
}

void CNtlPicking::UpdateCamera_End( RwCamera* pCamera )
{
	RwCameraSetRaster( pCamera, m_pTempRaster );
	RwCameraSetZRaster( pCamera, m_pTempZRaster );
}

RxPipeline* CNtlPicking::CreatePipeLine( void )
{
	RxPipeline* pPipeLine = RxPipelineCreate();
	if ( pPipeLine )
	{
		RxLockedPipe* pLockPipeLine = RxPipelineLock( pPipeLine );
		if ( pLockPipeLine )
		{
			RxNodeDefinition* pInstanceNode = RxNodeDefinitionGetD3D9AtomicAllInOne();

			pLockPipeLine = RxLockedPipeAddFragment( pLockPipeLine, NULL, pInstanceNode, NULL );

			pLockPipeLine = RxLockedPipeUnlock( pLockPipeLine );

			RxPipelineNode* pNode = RxPipelineFindNodeByName( pPipeLine, pInstanceNode->name, NULL, NULL );

			RxD3D9AllInOneSetRenderCallBack( pNode, PickingPipeLineCB );

			return pPipeLine;
		}

		RxPipelineDestroy( pPipeLine );
	}

	return NULL;
}

void CNtlPicking::DeletePipeLine( RxPipeline*& pPipeLine )
{
	if ( pPipeLine )
	{
		RxPipelineDestroy( pPipeLine );
		pPipeLine = NULL;
	}
}

void CNtlPicking::PickingPipeLineCB( RwResEntry* pRepEntry, void* pObject, RwUInt8 byType, RwUInt32 uiFlags )
{
	RxD3D9ResEntryHeader* pResEntryHeader	= (RxD3D9ResEntryHeader*)(pRepEntry + 1);
	RxD3D9InstanceData*	pInstancedData		= (RxD3D9InstanceData*)(pResEntryHeader + 1);

	RwBool bFogState;
	RwD3D9GetRenderState( D3DRS_FOGENABLE, (void*)&bFogState);
	RwD3D9SetRenderState( D3DRS_FOGENABLE, FALSE );

	RwBool bLightState;
	RwD3D9GetRenderState( D3DRS_LIGHTING, (void*)&bLightState);
	RwD3D9SetRenderState( D3DRS_LIGHTING, FALSE );

	RwD3D9SetRenderState( D3DRS_ZENABLE, FALSE );
	RwD3D9SetRenderState( D3DRS_CLIPPING, TRUE );	
	RwD3D9SetRenderState( D3DRS_TEXTUREFACTOR, s_RenderID );

	RwRenderStateSet( rwRENDERSTATEVERTEXALPHAENABLE, (void*)FALSE );

	RwD3D9SetTexture( NULL, 0 );

	RwD3D9SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG2 );
	RwD3D9SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TFACTOR );
	RwD3D9SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG2 );
	RwD3D9SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR );

	RwD3D9SetVertexDeclaration( pResEntryHeader->vertexDeclaration );

	RwD3D9SetIndices( pResEntryHeader->indexBuffer );

	_rwD3D9SetStreams( pResEntryHeader->vertexStream, pResEntryHeader->useOffsets );

	RwUInt32 nNumMeshes = pResEntryHeader->numMeshes;
	while ( nNumMeshes-- )
	{
		if ( pResEntryHeader->indexBuffer )
		{
			RwD3D9DrawIndexedPrimitive( (D3DPRIMITIVETYPE)pResEntryHeader->primType,
										pInstancedData->baseIndex,
										0,
										pInstancedData->numVertices,
										pInstancedData->startIndex, pInstancedData->numPrimitives );
		}
		else
		{
			RwD3D9DrawPrimitive( (D3DPRIMITIVETYPE)pResEntryHeader->primType,
								 pInstancedData->baseIndex,
								 pInstancedData->numVertices );
		}

		pInstancedData++;
	}

	RwD3D9SetRenderState( D3DRS_ZENABLE, TRUE );

	RwD3D9SetRenderState( D3DRS_LIGHTING,	bLightState);
	RwD3D9SetRenderState( D3DRS_FOGENABLE,	bFogState);
}

void CNtlPicking::RenderPickedAtoms( RxPipeline* pPipeLine )
{
	SPICK_INFO* pPickInfo;
	RxPipeline* pOldPipe;
	RpAtomicCallBackRender pOldRenderCB;

	int nCnt = 0;
	vecdef_PICK_INFO_LIST::reverse_iterator it = m_pPickEntityList->m_defSortedInfoList.rbegin();
	for ( ; it != m_pPickEntityList->m_defSortedInfoList.rend(); ++it )
	{
		pPickInfo = *it;

		if ( !pPickInfo->bRenderPolygonCheck ) continue;

		s_RenderID = pPickInfo->uiID;

		RpAtomicGetPipeline( pPickInfo->pAtomic, &pOldPipe );
		RpAtomicSetPipeline( pPickInfo->pAtomic, pPipeLine );

		pOldRenderCB = RpAtomicGetRenderCallBack( pPickInfo->pAtomic );
		RpAtomicSetRenderCallBack( pPickInfo->pAtomic, AtomicDefaultRenderCallBack );

		RpAtomicRender( pPickInfo->pAtomic );

		RpAtomicSetRenderCallBack( pPickInfo->pAtomic, pOldRenderCB );

		RpAtomicSetPipeline( pPickInfo->pAtomic, pOldPipe );

		if ( ++nCnt >= MAX_NUM_OF_MAX_ATOMIC ) break;
	}
}


//////////////////////////////////////////////////////////////////////////
//
//	Bounding box resizing 최적의 크기를 구하기 위한 테스트용 클래스
//
//////////////////////////////////////////////////////////////////////////




#include "NtlPLApi.h"
#include "NtlPLEntityRenderHelpers.h"


void COptimizeBBox::Render( void )
{
	//static const RwInt32 MAX_STEP = 6;
	//static RwReal arfDist[MAX_STEP]				= { 5.f,	10.f,	25.f,	50.f,	70.f,	512.f };
	//static RwReal arfOptimizeRadius[MAX_STEP]		= { 0.f,	0.5f,	1.0f,	1.8f,	2.f,	2.4f };
	//static RwRGBA arfRGBA[MAX_STEP]				= { {255, 255, 255, 255},
	//												{255, 255,   0, 255},
	//												{255,   0, 255, 255},
	//												{  0, 255, 255, 255},
	//												{255,   0, 255, 255},
	//												{  0,   0,   0, 255} };

	static const RwInt32 MAX_STEP = 8;
	static RwReal arfDist[MAX_STEP]				= { 0.f,	5.f,	10.f,	25.f,	50.f,	70.f,	120.f,	512.f };
	static RwReal arfOptimizeRadius[MAX_STEP]	= { 0.f,	0.f,	1.5f,	1.6f,	2.6f,	3.5f,	4.5f,	6.f };
	static RwRGBA arfRGBA[MAX_STEP]				= { {255, 255, 255, 255},
													{255, 255,   0, 255},
													{255,   0, 255, 255},
													{  0, 255, 255, 255},
													{255,   0, 255, 255},
													{  0,   0,   0, 255},
													{  0, 255,   0, 255},
													{100, 255, 100, 255}};

	RwCamera* pCamera = CNtlPLGlobal::m_RwCamera;
	if ( NULL == pCamera ) return;

	RwV3d v3CamPos = RwCameraGetFrame( pCamera )->modelling.pos;
	RwV3d v3CamDir = RwCameraGetFrame( pCamera )->modelling.at;

	RwSphere Sphere;

	for ( RwInt32 i = 0; i < MAX_STEP; ++i )
	{
		RwV3d v3CenterPos = v3CamPos + v3CamDir * arfDist[i];
		RwV2d v2CenterPos = API_PL_Calc3DPosTo2D( &v3CenterPos, pCamera->frameBuffer->width, pCamera->frameBuffer->height );

		Sphere.center = v3CenterPos;
		Sphere.radius = arfOptimizeRadius[i];

		Helper_ClumpWorldBoundingSphereRender( &Sphere, arfRGBA[i].red, arfRGBA[i].green, arfRGBA[i].blue, arfRGBA[i].alpha );
	}
}


