#include "precomp_ntlpresentation.h"
#include "NtlPLPicking.h"

// core
#include "NtlAtomic.h"
#include "NtlMath.h"

// presentation
#include "NtlPLGlobal.h"
#include "NtlPLVisualManager.h"

#include "NtlPLWorldEntity.h"
#include "NtlPLObject.h"
#include "NtlPLObjectType.h"

#include "NtlCollis.h"


SWorldCameraIntersectionData g_CameraInterParam;

const RwSphere* NtlCustomBoundingSphere( RpAtomic * atomic, void* pData )
{
	static const RwReal MIN_CLIPPING_SQUARE_SCALE_SIZE = 1.f * 1.f;
	static const RwReal MAX_CLIPPING_SQUARE_SCALE_SIZE = 2.5f * 2.5f;
	static const RwReal RATIO_SCALE_PER_DISTANCE = 0.05f;
	static const RwSphere* pOriginSphere;
	static RwSphere Sphere;
	static RwReal fScaleRatio;

	pOriginSphere = RpAtomicGetWorldBoundingSphere( atomic );

	CNtlPLEntity* pPLEntity = reinterpret_cast<CNtlPLEntity*>( RpNtlAtomicGetData(atomic) );
	if ( NULL == pPLEntity ||
		!(PLENTITY_CHARACTER == pPLEntity->GetClassType() || PLENTITY_ITEM == pPLEntity->GetClassType() ) ||
		!(pPLEntity->GetFlags() & NTL_PLEFLAG_RESIZE_PICKING_BBOX) )
	{
		return pOriginSphere;
	}

	// 바운딩의 크기가 0.7M 이하인 경우의 Bounding box resizing 알고리즘
	if ( pOriginSphere->radius < 0.7f )
	{
		RwV3d* pCamPos = (RwV3d*)pData;

		Sphere = *pOriginSphere;

		fScaleRatio = (Sphere.center.x - pCamPos->x) * (Sphere.center.x - pCamPos->x) +
			(Sphere.center.y - pCamPos->y) * (Sphere.center.y - pCamPos->y) +
			(Sphere.center.z - pCamPos->z) * (Sphere.center.z - pCamPos->z);

		fScaleRatio *= RATIO_SCALE_PER_DISTANCE;
		if ( fScaleRatio - MIN_CLIPPING_SQUARE_SCALE_SIZE < 0.f ) fScaleRatio = MIN_CLIPPING_SQUARE_SCALE_SIZE;
		if ( MAX_CLIPPING_SQUARE_SCALE_SIZE - fScaleRatio < 0.f ) fScaleRatio = MAX_CLIPPING_SQUARE_SCALE_SIZE;

		Sphere.radius *= sqrtf( fScaleRatio );

		return &Sphere;
	}
	else
	{
		return pOriginSphere;
	}
}

RpCollisionTriangle* Pick_IntersectionLineTriangleCB( RpIntersection *pIntersection, RpCollisionTriangle *pRpCollisionTriangle, RwReal fRatio, void* pData )
{
	SWorldPickInfo *pPickInfo = (SWorldPickInfo *)pData;

	CNtlPLEntity *pPLEntity = reinterpret_cast<CNtlPLEntity*>( RpNtlAtomicGetData(pPickInfo->pAtomic) );
	if(pPLEntity->GetClassType() == PLENTITY_OBJECT)
	{
		RwLine *pLine = &pIntersection->t.line;

		RwV3d vCollPos = pLine->start + fRatio * (pLine->end - pLine->start);;		
        RwReal fDist = CNtlMath::GetLength(vCollPos, pLine->start);
		if( fDist < pPickInfo->fMinDist )
		{
			pPickInfo->fMinDist = fDist;
			pPickInfo->pPLEntity = pPLEntity;
			RwV3dAssign(&pPickInfo->vPickPos, &vCollPos);
		}
	}

	return pRpCollisionTriangle;
}

RpAtomic* Pick_IntersectionLineAtomicCB( RpIntersection* pIntersection, RpWorldSector* pWorldSector, RpAtomic* pAtomic, RwReal fDistance, void* pData )
{
	SWorldPickInfo *pPickInfo = (SWorldPickInfo *)pData;

	CNtlPLEntity *pPLEntity = reinterpret_cast<CNtlPLEntity*>( RpNtlAtomicGetData(pAtomic) );
	if( pPLEntity)
	{
		if(pPLEntity->GetClassType() == PLENTITY_OBJECT)
		{
			if(RpNtlAtomicGetFlag(pAtomic) & NTL_COLLISION)
			{
				pPickInfo->pAtomic = pAtomic;
				RpAtomicForAllIntersections(pAtomic, pIntersection, Pick_IntersectionLineTriangleCB, pPickInfo);
			}
		}
		else
		{
			if(pPLEntity->GetFlags() & NTL_PLEFLAG_PICKING)
			{
				RwSphere *pCollSphere = &pAtomic->worldBoundingSphere;
				RwV3d vSubDist;
				RwV3dSubMacro(&vSubDist, &pIntersection->t.line.start, &pCollSphere->center);
				RwReal fDist = RwV3dLength(&vSubDist);
				if(fDist <= pPickInfo->fMinDist)
				{
					pPickInfo->fMinDist = fDist;
					pPickInfo->pPLEntity = pPLEntity;
				}
			}
		}
	}

	return pAtomic;
}

RpAtomic* Collision_IntersectionObjectCB( RpIntersection* pIntersection, RpWorldSector* pWorldSector, RpAtomic* pAtomic, RwReal fDistance, void* pData )
{
	SWorldPickInfo* pPickInfo = (SWorldPickInfo*)pData;
	CNtlPLEntity *pPLEntity = reinterpret_cast<CNtlPLEntity*>( RpNtlAtomicGetData(pAtomic) );

	if(pPLEntity == NULL)
		return pAtomic;
	if(!pPLEntity->IsVisible(0))
		return pAtomic;
	if(pPLEntity->GetClassType() != PLENTITY_OBJECT)
		return pAtomic;
    if(RpNtlAtomicGetFlag(pAtomic) & NTL_NOT_VISIBLE)
        return pAtomic;

	pPickInfo->pAtomic = pAtomic;

	RpAtomicForAllIntersections(pAtomic, pIntersection, Pick_IntersectionLineTriangleCB, pPickInfo);
	if(!pPickInfo->pPLEntity)			
		return pAtomic;

	return NULL;
}

RpAtomic* CollisionTarget_IntersectionObjectCB( RpIntersection* pIntersection, RpWorldSector* pWorldSector, RpAtomic* pAtomic, RwReal fDistance, void* pData )
{
	SWorldPickInfo* pPickInfo = (SWorldPickInfo*)pData;
	CNtlPLEntity *pPLEntity = reinterpret_cast<CNtlPLEntity*>( RpNtlAtomicGetData(pAtomic) );

	if(pPLEntity == NULL)
		return pAtomic;
	if(!pPLEntity->IsVisible(0))
		return pAtomic;
	if(pPLEntity->GetClassType() != PLENTITY_OBJECT)
		return pAtomic;
	if(RpNtlAtomicGetFlag(pAtomic) & NTL_NOT_VISIBLE)
		return pAtomic;
	if(!(RpNtlAtomicGetFlag(pAtomic) & NTL_COLLISION))
		return pAtomic;

	pPickInfo->pAtomic = pAtomic;

	RpAtomicForAllIntersections(pAtomic, pIntersection, Pick_IntersectionLineTriangleCB, pPickInfo);
	if(!pPickInfo->pPLEntity)			
		return pAtomic;

	return NULL;
}

RpCollisionTriangle* Collision_HeightFieldIntersectionLineTriangleTopDownCB( RpIntersection *pIntersection, RpCollisionTriangle *pRpCollisionTriangle, RwReal fRatio, void* pData )
{
	if(fRatio < 0.0f)
		return pRpCollisionTriangle;

	SWorldIntersect * pIntersectParam = (SWorldIntersect *)pData;
	RwReal fOldDist = pIntersectParam->fMinDist;

	RwLine *pLine = &pIntersectParam->Intersection.t.line;

	RwV3d vCollPos;

	vCollPos.x = pLine->start.x + fRatio * ( pLine->end.x - pLine->start.x );
	vCollPos.y = pLine->start.y + fRatio * ( pLine->end.y - pLine->start.y );
	vCollPos.z = pLine->start.z + fRatio * ( pLine->end.z - pLine->start.z );

	RwV3d vSub;
	RwV3dSubMacro(&vSub, &vCollPos, &pLine->start);

	RwReal fDist =  RwV3dLength( &vSub );
	if( fDist < pIntersectParam->fMinDist )
	{
		if (GetSceneManager()->GetWorldAttribute(vCollPos) & dNMAP_TRANSPARENCY_TILE_FLAG)
		{
			return pRpCollisionTriangle;
		}

		pIntersectParam->fMinDist = fDist;
		RwV3dAssign(&pIntersectParam->vCollPos, &vCollPos);
		pIntersectParam->bCollision = TRUE;

		RwV3d vTrans[3];
		RwMatrix *pMatLTM = RwFrameGetLTM(RpAtomicGetFrame(pIntersectParam->pAtomic));
		RwV3dTransformPoints(&vTrans[0], pRpCollisionTriangle->vertices[0], 1, pMatLTM);
		RwV3dTransformPoints(&vTrans[1], pRpCollisionTriangle->vertices[1], 1, pMatLTM);
		RwV3dTransformPoints(&vTrans[2], pRpCollisionTriangle->vertices[2], 1, pMatLTM);

		RwV3d v0, v1;
		RwV3dSubMacro(&v0, &vTrans[1], &vTrans[0]);
		RwV3dSubMacro(&v1, &vTrans[2], &vTrans[0]);
		RwV3dCrossProduct(&pIntersectParam->vNormal, &v0, &v1);
		RwV3dNormalize(&pIntersectParam->vNormal, &pIntersectParam->vNormal);
	}

	return pRpCollisionTriangle;
}

RpCollisionTriangle* Collision_IntersectionLineTriangleTopDownCB( RpIntersection *pIntersection, RpCollisionTriangle *pRpCollisionTriangle, RwReal fRatio, void* pData )
{
	if(fRatio < 0.0f)
		return pRpCollisionTriangle;

	SWorldIntersect * pIntersectParam = (SWorldIntersect *)pData;
	RwReal fOldDist = pIntersectParam->fMinDist;

	RwLine *pLine = &pIntersectParam->Intersection.t.line;

	RwV3d vCollPos;

	vCollPos.x = pLine->start.x + fRatio * ( pLine->end.x - pLine->start.x );
	vCollPos.y = pLine->start.y + fRatio * ( pLine->end.y - pLine->start.y );
	vCollPos.z = pLine->start.z + fRatio * ( pLine->end.z - pLine->start.z );

	RwV3d vSub;
	RwV3dSubMacro(&vSub, &vCollPos, &pLine->start);

	RwReal fDist =  RwV3dLength( &vSub );
	if( fDist < pIntersectParam->fMinDist )
	{
		pIntersectParam->fMinDist = fDist;
		RwV3dAssign(&pIntersectParam->vCollPos, &vCollPos);
		pIntersectParam->bCollision = TRUE;

		RwV3d vTrans[3];
		RwMatrix *pMatLTM = RwFrameGetLTM(RpAtomicGetFrame(pIntersectParam->pAtomic));
		RwV3dTransformPoints(&vTrans[0], pRpCollisionTriangle->vertices[0], 1, pMatLTM);
		RwV3dTransformPoints(&vTrans[1], pRpCollisionTriangle->vertices[1], 1, pMatLTM);
		RwV3dTransformPoints(&vTrans[2], pRpCollisionTriangle->vertices[2], 1, pMatLTM);

		RwV3d v0, v1;
		RwV3dSubMacro(&v0, &vTrans[1], &vTrans[0]);
		RwV3dSubMacro(&v1, &vTrans[2], &vTrans[0]);
		RwV3dCrossProduct(&pIntersectParam->vNormal, &v0, &v1);
		RwV3dNormalize(&pIntersectParam->vNormal, &pIntersectParam->vNormal);
	}

	return pRpCollisionTriangle;
}

RpCollisionTriangle* Collision_IntersectionLineWorldSectorTopDownCB( RpIntersection * pIntersection, RpWorldSector * pRpWorldSector,
																	RpCollisionTriangle * pRpCollisionTriangle, RwReal fRatio, void *pData )
{
	if(fRatio < 0.0f)
		return pRpCollisionTriangle;

	SWorldIntersect * pIntersectParam = (SWorldIntersect *)pData;
	RwReal fOldDist = pIntersectParam->fMinDist;

	RwLine *pLine = &pIntersectParam->Intersection.t.line;

	RwV3d vCollPos;

	vCollPos.x = pLine->start.x + fRatio * ( pLine->end.x - pLine->start.x );
	vCollPos.y = pLine->start.y + fRatio * ( pLine->end.y - pLine->start.y );
	vCollPos.z = pLine->start.z + fRatio * ( pLine->end.z - pLine->start.z );

	RwV3d vSub;
	RwV3dSubMacro(&vSub, &vCollPos, &pLine->start);

	RwReal fDist =  RwV3dLength( &vSub );
	if( fDist < pIntersectParam->fMinDist )
	{
		pIntersectParam->fMinDist = fDist;
		RwV3dAssign(&pIntersectParam->vCollPos, &vCollPos);
		pIntersectParam->bCollision = TRUE;

		RwV3d v0, v1;
		RwV3dSubMacro(&v0, pRpCollisionTriangle->vertices[1], pRpCollisionTriangle->vertices[0]);
		RwV3dSubMacro(&v1, pRpCollisionTriangle->vertices[2], pRpCollisionTriangle->vertices[0]);
		RwV3dCrossProduct(&pIntersectParam->vNormal, &v0, &v1);
		RwV3dNormalize(&pIntersectParam->vNormal, &pIntersectParam->vNormal);
	}

	return pRpCollisionTriangle;
}

RpAtomic* Collision_IntersectionLineAtomicTopDownCB( RpIntersection* pIntersection, RpWorldSector* pWorldSector, RpAtomic* pAtomic, RwReal fDistance, void* pData )
{
	CNtlPLEntity *pPLEntity = reinterpret_cast<CNtlPLEntity*>( RpNtlAtomicGetData(pAtomic) );

	if(pPLEntity == NULL)
		return pAtomic;
	if(!pPLEntity->IsVisible(0))
		return pAtomic;
	if(pPLEntity->GetClassType() != PLENTITY_OBJECT)
		return pAtomic;
	if( !(pPLEntity->GetFlags() & NTL_PLEFLAG_COLLISION) )
		return pAtomic;
	if( !(RpNtlAtomicGetFlag(pAtomic) & NTL_COLLISION) )
		return pAtomic;

	SWorldIntersect *pIntersectParam = (SWorldIntersect*)pData;

	RpAtomic *pOldAtomic = pIntersectParam->pAtomic;
	RwReal fOldMinDist = pIntersectParam->fMinDist;

	pIntersectParam->pAtomic = pAtomic;

	RpAtomicForAllIntersections(pAtomic, pIntersection, Collision_IntersectionLineTriangleTopDownCB, pData);

	if(pIntersectParam->fMinDist >= fOldMinDist - 0.00001f )
		pIntersectParam->pAtomic = pOldAtomic;

	return pAtomic;
}

RpAtomic* Collision_IntersectionMiniIndoorLineTopDownCB( RpIntersection* pIntersection, RpWorldSector* pWorldSector, RpAtomic* pAtomic, RwReal fDistance, void* pData )
{
	CNtlPLEntity *pPLEntity = reinterpret_cast<CNtlPLEntity*>( RpNtlAtomicGetData(pAtomic) );

	if(pPLEntity == NULL)
		return pAtomic;
	if(!pPLEntity->IsVisible(0))
		return pAtomic;
	if(pPLEntity->GetClassType() != PLENTITY_OBJECT)
		return pAtomic;
	if( !(pPLEntity->GetFlags() & NTL_PLEFLAG_COLLISION) )
		return pAtomic;
	if( !(RpNtlAtomicGetFlag(pAtomic) & NTL_COLLISION) )
		return pAtomic;
	if (static_cast<CNtlPLObject*>(pPLEntity)->GetObjectType() != EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE &&
		static_cast<CNtlPLObject*>(pPLEntity)->GetObjectType() != EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
		return pAtomic;

	SWorldIntersect *pIntersectParam = (SWorldIntersect*)pData;

	RpAtomic *pOldAtomic = pIntersectParam->pAtomic;
	RwReal fOldMinDist = pIntersectParam->fMinDist;

	pIntersectParam->pAtomic = pAtomic;

	RpAtomicForAllIntersections(pAtomic, pIntersection, Collision_IntersectionLineTriangleTopDownCB, pData);

	if(pIntersectParam->fMinDist >= fOldMinDist - 0.00001f )
		pIntersectParam->pAtomic = pOldAtomic;

	return pAtomic;
}

RpCollisionTriangle* Collision_IntersectionCharacterLineTriangleCB( RpIntersection *pIntersection, RpCollisionTriangle *pRpCollisionTriangle, RwReal fRatio, void* pData )
{
	SWorldCharIntersectionData *pCharData = (SWorldCharIntersectionData *)pData;
	pCharData->bCollision = TRUE;

	RwLine *pLine = &pIntersection->t.line;

	RwV3d vCollPos;
	vCollPos.x = pLine->start.x + fRatio * ( pLine->end.x - pLine->start.x );
	vCollPos.y = pLine->start.y + fRatio * ( pLine->end.y - pLine->start.y );
	vCollPos.z = pLine->start.z + fRatio * ( pLine->end.z - pLine->start.z );

	RwV3d vDelta;
	RwV3dSubMacro( &vDelta, &vCollPos, &pLine->start);

	RwReal fDist =  RwV3dLength( &vDelta );
	if( fDist < pCharData->fMinDist )
	{
		pCharData->fMinDist = fDist;
		RwV3dAssign(&pCharData->vCollPos, &vCollPos);

		RwV3d vTrans[3];
		RwMatrix *pMatLTM = RwFrameGetLTM(RpAtomicGetFrame(pCharData->pAtomic));
		RwV3dTransformPoints(&vTrans[0], pRpCollisionTriangle->vertices[0], 1, pMatLTM);
		RwV3dTransformPoints(&vTrans[1], pRpCollisionTriangle->vertices[1], 1, pMatLTM);
		RwV3dTransformPoints(&vTrans[2], pRpCollisionTriangle->vertices[2], 1, pMatLTM);

		RwV3d v0, v1;
		RwV3dSubMacro(&v0, &vTrans[1], &vTrans[0]);
		RwV3dSubMacro(&v1, &vTrans[2], &vTrans[0]);
		RwV3dCrossProduct(&pCharData->vNormal, &v0, &v1);
		RwV3dNormalize(&pCharData->vNormal, &pCharData->vNormal);
	}

	return pRpCollisionTriangle;
}

RpCollisionTriangle* Collision_IntersectionCharacterLineTriangleCB( RpIntersection *pIntersection, RpWorldSector* pRpWorldSector, RpCollisionTriangle *pRpCollisionTriangle, RwReal fRatio, void* pData )
{
	SWorldCharIntersectionData *pCharData = (SWorldCharIntersectionData *)pData;
	pCharData->bCollision = TRUE;

	RwLine *pLine = &pIntersection->t.line;

	RwV3d vCollPos;
	vCollPos.x = pLine->start.x + fRatio * ( pLine->end.x - pLine->start.x );
	vCollPos.y = pLine->start.y + fRatio * ( pLine->end.y - pLine->start.y );
	vCollPos.z = pLine->start.z + fRatio * ( pLine->end.z - pLine->start.z );

	RwV3d vDelta;
	RwV3dSubMacro( &vDelta, &vCollPos, &pLine->start);

	RwReal fDist =  RwV3dLength( &vDelta );
	if( fDist < pCharData->fMinDist )
	{
		pCharData->fMinDist = fDist;
		RwV3dAssign(&pCharData->vCollPos, &vCollPos);

		RwV3d v0, v1;
		RwV3dSubMacro(&v0, pRpCollisionTriangle->vertices[1], pRpCollisionTriangle->vertices[0]);
		RwV3dSubMacro(&v1, pRpCollisionTriangle->vertices[2], pRpCollisionTriangle->vertices[0]);
		RwV3dCrossProduct(&pCharData->vNormal, &v0, &v1);
		RwV3dNormalize(&pCharData->vNormal, &pCharData->vNormal);
	}

	return pRpCollisionTriangle;
}


RpAtomic* Collision_IntersectionCharacterBoxAtomic( RpIntersection* pIntersection, RpWorldSector* pWorldSector, RpAtomic* pAtomic, RwReal fDistance, void* pData )
{
	SWorldCharIntersectionInfo *pCharInfo = (SWorldCharIntersectionInfo *)pData;
	if(pCharInfo->byAtomicCount >= NTL_MAX_CHAR_INTERSECION)
		return pAtomic;

	CNtlPLEntity *pPLEntity = reinterpret_cast<CNtlPLEntity*>( RpNtlAtomicGetData(pAtomic) );

	if(pPLEntity == NULL)
		return pAtomic;
	if(!pPLEntity->IsVisible(0))
		return pAtomic;

	RwUInt32 uiClassType = pPLEntity->GetClassType();

	if(uiClassType == PLENTITY_OBJECT)
	{
		if( !(pPLEntity->GetFlags() & NTL_PLEFLAG_COLLISION) )
			return pAtomic;
		if( !(RpNtlAtomicGetFlag(pAtomic) & NTL_COLLISION) )
			return pAtomic;
	}
	else if(uiClassType == PLENTITY_CHARACTER)
	{
		if( !(RpNtlAtomicGetFlag(pAtomic) & NTL_COLLISION) )
			return pAtomic;
	}
	else
	{
		return pAtomic;
	}

	pCharInfo->pAtomic[pCharInfo->byAtomicCount] = pAtomic;
	pCharInfo->byAtomicCount++;

	return pAtomic;
}

// 오름 차순 sorting(가까운 것이 앞에 온다)
int Pick_DepthSortFunc(const void *a, const void *b)
{
	/*
	const SPickAtomic *ptrA = (const SPickAtomic*)(a);
	const SPickAtomic *ptrB = (const SPickAtomic*)(b);

	RwReal fDepthA = ptrA->fDepth;
	RwReal fDepthB = ptrB->fDepth;

	if (fDepthA > fDepthB) return -1;
	else if (fDepthA == fDepthB) return 0;
	else return 1;
	*/

	return 1;
}

/**
* 충돌 가능 Atomic인지 검사한다
**/
RwBool IsCollisionAtomic(RpAtomic* pAtomic)
{
	CNtlPLEntity *pPLEntity = reinterpret_cast<CNtlPLEntity*>( RpNtlAtomicGetData( pAtomic ) );

	if ( pPLEntity == NULL ||
		 pPLEntity->GetClassType() != PLENTITY_OBJECT )
	{
		return FALSE;
	}

	if ( !pPLEntity->IsVisible(0) )
	{
		return FALSE;
	}

	if ( !( pPLEntity->GetFlags() & NTL_PLEFLAG_COLLISION ) )
	{
		return FALSE;
	}

	RwUInt32 uiAtomicFlag = RpNtlAtomicGetFlag( pAtomic );

	if ( !( uiAtomicFlag & NTL_COLLISION ) )
	{
		return FALSE;
	}

	if ( (uiAtomicFlag & NTL_NOT_VISIBLE) &&
		 !( uiAtomicFlag & NTL_CAMERA_COLLISION ) )
	{
		return FALSE;
	}

	return TRUE;
}

/**
* mouse picking을 한다.
*/
void Pick_WorldIntersectionLine(RwLine& Line, SWorldPickInfo& sPickInfo, RwReal fRayDist)
{
	sPickInfo.pPLEntity = NULL;
	sPickInfo.fMinDist = RwRealMAXVAL;

	RpIntersection Intersection;

	Intersection.t.line	= Line;
	Intersection.type = rpINTERSECTLINE;    

	RwV3d vRayDir;
	RwV3dSubMacro(&vRayDir, &Intersection.t.line.end, &Intersection.t.line.start);
	RwV3dNormalize(&vRayDir, &vRayDir);

	Intersection.t.line.end.x = Intersection.t.line.start.x + vRayDir.x*fRayDist;
	Intersection.t.line.end.y = Intersection.t.line.start.y + vRayDir.y*fRayDist;
	Intersection.t.line.end.z = Intersection.t.line.start.z + vRayDir.z*fRayDist;

	//	RpWorldForAllAtomicIntersections( CNtlPLGlobal::m_pRpWorld, &Intersection, Pick_IntersectionLineAtomicCB, &sPickInfo);
	NtlWorldForAllAtomicIntersections_CustomBBox( CNtlPLGlobal::m_pRpWorld, &Intersection, NtlCustomBoundingSphere, &RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)->modelling.pos, Pick_IntersectionLineAtomicCB, &sPickInfo );

	// 좀더 빠른 속도를 위하여... qsort도 안하는 방법을 생각해 봐야 한다.(예를 들면 depth 를 비교문으로 하여 쓴다.)
	// qsort((void*)sPickInfo.sAtomic, sPickInfo.byAtomicCnt, sizeof(SPickAtomic), Pick_DepthSortFunc);
}

/**
* 충돌되는 height field 높이를 구하는 collision logic에 optimize 되어 있다.
*/
RwBool Collision_HeightFieldIntersectionLineTopDown(RwLine& Line, SWorldIntersect& sInter)
{
	sInter.Intersection.t.line	= Line;
	sInter.Intersection.type = rpINTERSECTLINE;    
	sInter.fMinDist	= RwRealMAXVAL;
	sInter.bCollision = FALSE;

	CNtlPLWorldEntity *pWorldEntity = reinterpret_cast<CNtlPLVisualManager*>( GetSceneManager() )->GetWorld();
	RpAtomic *pAtomic = pWorldEntity->GetAtomic(Line.start);
	if(pAtomic == NULL)
		return FALSE;

	sInter.pAtomic = pAtomic;

	RpCollisionGeometryForAllIntersections(RpAtomicGetGeometry(pAtomic), &sInter.Intersection, Collision_HeightFieldIntersectionLineTriangleTopDownCB, (void *)&sInter);

	return sInter.bCollision; 
}

/**
* 충돌되는 Indoor 높이를 구하는 collision logic에 optimize 되어 있다.
*/
RwBool Collision_IndoorIntersectionLineTopDown(RwLine& Line, SWorldIntersect& sInter)
{
	sInter.Intersection.t.line	= Line;
	sInter.Intersection.type = rpINTERSECTLINE;    
	sInter.fMinDist	= RwRealMAXVAL;
	sInter.bCollision = FALSE;

	RpCollisionWorldForAllIntersections(CNtlPLGlobal::m_pRpWorld, &sInter.Intersection, Collision_IntersectionLineWorldSectorTopDownCB, &sInter);

	return sInter.bCollision;
}


/**
* 충돌되는 object 높이를 구하는 collision logic에 optimize 되어 있다.
*/
RwBool Collision_WorldIntersectionLineTopDown(RwLine& Line, SWorldIntersect& sInter)
{
	sInter.Intersection.t.line	= Line;
	sInter.Intersection.type = rpINTERSECTLINE;    
	sInter.fMinDist	= RwRealMAXVAL;
	sInter.pAtomic = NULL;	
	sInter.bCollision = FALSE;

	RpWorldForAllAtomicIntersections( CNtlPLGlobal::m_pRpWorld, &sInter.Intersection, Collision_IntersectionLineAtomicTopDownCB, &sInter);

	return sInter.bCollision; 
}

/**
* 충돌되는 mini indoor object 높이를 구하는 collision logic에 optimize 되어 있다.
*/
RwBool Collision_MiniIndoorIntersectionLineTopDown(RwLine& Line, SWorldIntersect& sInter)
{
	sInter.Intersection.t.line	= Line;
	sInter.Intersection.type = rpINTERSECTLINE;    
	sInter.fMinDist	= RwRealMAXVAL;
	sInter.pAtomic = NULL;	
	sInter.bCollision = FALSE;

	RpWorldForAllAtomicIntersections( CNtlPLGlobal::m_pRpWorld, &sInter.Intersection, Collision_IntersectionMiniIndoorLineTopDownCB, &sInter);

	return sInter.bCollision; 
}

RwBool Collision_WorldIntersectionObject( const RwLine& line, SWorldPickInfo* pPickInfo ) 
{
	RpIntersection intersection;
	intersection.t.line = line;
	intersection.type = rpINTERSECTLINE;

	RpWorldForAllAtomicIntersections( CNtlPLGlobal::m_pRpWorld, &intersection, Collision_IntersectionObjectCB, pPickInfo);

	if(pPickInfo->pPLEntity)
		return TRUE;
	else 
		return FALSE;
}

RwBool Collision_WorldIntersectionObjectForTarget( const RwLine& line, SWorldPickInfo* pPickInfo ) 
{
	RpIntersection intersection;
	intersection.t.line = line;
	intersection.type = rpINTERSECTLINE;

	RpWorldForAllAtomicIntersections( CNtlPLGlobal::m_pRpWorld, &intersection, CollisionTarget_IntersectionObjectCB, pPickInfo);

	if(pPickInfo->pPLEntity)
		return TRUE;
	else 
		return FALSE;
}

/**
* world object중에 충돌되는 atomic을 구한다.
*/
RwBool Collision_WorldIntersectionCharacter(RwBBox& box, SWorldCharIntersectionInfo& sCharInfo)
{
	sCharInfo.byAtomicCount = 0;

	RpIntersection Intersection;

	Intersection.type = rpINTERSECTBOX;    
	Intersection.t.box = box;

	RpWorldForAllAtomicIntersections( CNtlPLGlobal::m_pRpWorld, &Intersection, Collision_IntersectionCharacterBoxAtomic, &sCharInfo);

	if(sCharInfo.byAtomicCount > 0)
		return TRUE;

	return FALSE;
}


RwBool Collision_WorldIntersectionWorldSector(RwLine& Line, SWorldCharIntersectionData& sCharDataInfo)
{
	RpIntersection Intersection;

	Intersection.t.line	= Line;
	Intersection.type = rpINTERSECTLINE;    

	sCharDataInfo.bCollision = FALSE;
	sCharDataInfo.fMinDist = RwRealMAXVAL;
	sCharDataInfo.pAtomic = NULL;

	RpCollisionWorldForAllIntersections(CNtlPLGlobal::m_pRpWorld, &Intersection, Collision_IntersectionCharacterLineTriangleCB, &sCharDataInfo);

	return sCharDataInfo.bCollision;
}

/**
* world object중에 충돌되는 atomic을 구한다.
*/
RwBool Collision_WorldIntersectionCharacterData(RwLine& Line, RpAtomic *pAtomic, SWorldCharIntersectionData& sCharDataInfo)
{
	RpIntersection Intersection;

	Intersection.t.line	= Line;
	Intersection.type = rpINTERSECTLINE;    

	sCharDataInfo.bCollision = FALSE;
	sCharDataInfo.fMinDist = RwRealMAXVAL;
	sCharDataInfo.pAtomic = pAtomic;

	RpAtomicForAllIntersections(pAtomic, &Intersection, Collision_IntersectionCharacterLineTriangleCB, &sCharDataInfo);

	return sCharDataInfo.bCollision;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// camera collision

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


RpCollisionTriangle* Collision_CameraLineTriangleCB( RpIntersection *pIntersection, RpCollisionTriangle *pRpCollisionTriangle, RwReal fRatio, void* pData )
{
	RwLine *pLine = &pIntersection->t.line;

	RwV3d vCollPos;

	vCollPos.x = pLine->start.x + fRatio * ( pLine->end.x - pLine->start.x );
	vCollPos.y = pLine->start.y + fRatio * ( pLine->end.y - pLine->start.y );
	vCollPos.z = pLine->start.z + fRatio * ( pLine->end.z - pLine->start.z );

	RwV3d vDelta;
	RwV3dSubMacro( &vDelta, &vCollPos, &pLine->start);
	RwReal fDist =  RwV3dLength( &vDelta );

	RwUInt8 byCollCount = g_CameraInterParam.sLineAtomicData.byCollCount;

	if ( fDist < g_CameraInterParam.sLineAtomicData.fMinDist[byCollCount] )
	{
		g_CameraInterParam.sLineAtomicData.fMinDist[byCollCount] = fDist;
		g_CameraInterParam.sLineAtomicData.vCollPos[byCollCount] = vCollPos;
	}

	g_CameraInterParam.sLineAtomicData.bCollision[byCollCount] = TRUE;

	return pRpCollisionTriangle;
}

RpAtomic* Collision_CameraAtomicLineCB( RpIntersection* pIntersection, RpWorldSector* pWorldSector, RpAtomic* pAtomic, RwReal fDistance, void* pData )
{
	if ( !IsCollisionAtomic( pAtomic ) )
	{
		return pAtomic;
	}

	if ( g_CameraInterParam.sLineAtomicData.byCollCount + 1 >= NTL_MAX_CAMERA_INTERSECION )
	{
		return pAtomic;
	}

	RwUInt8 byCollCount = g_CameraInterParam.sLineAtomicData.byCollCount;

	g_CameraInterParam.sLineAtomicData.pAtomic[byCollCount]		= pAtomic;
	g_CameraInterParam.sLineAtomicData.bCollision[byCollCount]	= FALSE;
	g_CameraInterParam.sLineAtomicData.fMinDist[byCollCount]	= RwRealMAXVAL;

	RpAtomicForAllIntersections( pAtomic, pIntersection, Collision_CameraLineTriangleCB, NULL );

	if ( g_CameraInterParam.sLineAtomicData.bCollision[byCollCount] )
	{
		g_CameraInterParam.sLineAtomicData.byCollCount++;
	}

	return pAtomic;
}

RpAtomic* Collision_CameraAtomicSphereCB( RpIntersection* pIntersection, RpWorldSector* pSector, RpAtomic* pAtomic, RwReal fDistance, void* pData )
{
	if ( !IsCollisionAtomic( pAtomic ) )
	{
		return pAtomic;
	}

	if ( g_CameraInterParam.sLineAtomicData.byCollCount + 1 >= NTL_MAX_CAMERA_INTERSECION )
	{
		return pAtomic;
	}

	RwUInt8 byCollCount = g_CameraInterParam.sLineAtomicData.byCollCount;

	g_CameraInterParam.sLineAtomicData.pAtomic[byCollCount] = pAtomic;
	g_CameraInterParam.sLineAtomicData.bCollision[byCollCount] = TRUE;
	g_CameraInterParam.sLineAtomicData.fMinDist[byCollCount] = RwRealMAXVAL;

	g_CameraInterParam.sLineAtomicData.byCollCount++;

	return pAtomic;
}


/**
* world상의 공간에서 camera 충돌 정보를 구한다.
*/

RwBool Collision_CameraCenterIntersection( const RwV3d& vCameraPos, const RwV3d& vLookAt, RwReal fRadius, RwV3d& vNewPos )
{
	// 캐릭터에서 카메라를 바라보는 벡터 계산
	RwV3d vCharToCamDir = vCameraPos - vLookAt;
	RwReal fCharToCamDist = RwV3dNormalize( &vCharToCamDir, &vCharToCamDir );
	if ( fCharToCamDist <= 0.0001f )
	{
		return FALSE;
	}

	// Line 충돌 정보 초기화
	g_CameraInterParam.sLineAtomicData.byCollCount = 0;

	// Center intersection ray 구성
	RpIntersection sCenterIntersData;
	sCenterIntersData.type = rpINTERSECTLINE;
	sCenterIntersData.t.line.start = vLookAt;
	sCenterIntersData.t.line.end = vCameraPos;

	// Center intersection 검사
	RpWorldForAllAtomicIntersections( CNtlPLGlobal::m_pRpWorld, &sCenterIntersData, Collision_CameraAtomicLineCB, NULL );

	// Center intersection 결과 처리
	if ( g_CameraInterParam.sLineAtomicData.byCollCount > 0 )
	{
		RwUInt8 byIndex = 255;
		RwReal fMinDist = RwRealMAXVAL;

		for ( RwUInt8 byCnt = 0; byCnt < g_CameraInterParam.sLineAtomicData.byCollCount; ++byCnt )
		{
			if ( g_CameraInterParam.sLineAtomicData.fMinDist[byCnt] < fMinDist )
			{
				fMinDist = g_CameraInterParam.sLineAtomicData.fMinDist[byCnt];
				byIndex = byCnt;
			}
		}

		if ( 255 != byIndex )
		{
			if ( fMinDist - fRadius < 0.0001f )
			{
				vNewPos = vLookAt + vCharToCamDir * 0.01f;
			}
			else
			{
				vNewPos = g_CameraInterParam.sLineAtomicData.vCollPos[byIndex] - vCharToCamDir * fRadius;
			}

			return TRUE;
		}
	}

	return FALSE;
}

RwBool Collision_CameraBoundaryIntersection( const RwV3d& vCameraPos, const RwV3d& vLookAt, RwReal fRadius, RwV3d& vNewPos )
{
	// 캐릭터에서 카메라를 바라보는 벡터 계산
	RwV3d vCharToCamDir = vCameraPos - vLookAt;
	RwReal fCharToCamDist = RwV3dNormalize( &vCharToCamDir, &vCharToCamDir );
	if ( fCharToCamDist <= 0.0001f )
	{
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	// Sphere 충돌
	//////////////////////////////////////////////////////////////////////////

	// Sphere 충돌 정보 초기화
	g_CameraInterParam.sLineAtomicData.byCollCount = 0;

	// Sphere intersection ray 구성
	RpIntersection sSphereIntersData;
	sSphereIntersData.type = rpINTERSECTSPHERE;
	sSphereIntersData.t.sphere.center = vCameraPos;
	sSphereIntersData.t.sphere.radius = fRadius;

	// Sphere intersection 검사
	RpWorldForAllAtomicIntersections( CNtlPLGlobal::m_pRpWorld, &sSphereIntersData, Collision_CameraAtomicSphereCB, NULL );

	// Sphere intersection 결과 처리
	SWorldCameraLineAtomicData sSphereIntersResult;
	memcpy( &sSphereIntersResult, &g_CameraInterParam.sLineAtomicData, sizeof( SWorldCameraLineAtomicData ) );

	//////////////////////////////////////////////////////////////////////////
	// Boundary ray 충돌
	//////////////////////////////////////////////////////////////////////////

	// Boundary ray intersection 정보 초기화
	g_CameraInterParam.sLineAtomicData.byCollCount = 0;

	// Boundary ray 구성
	RwV3d vRight;
	RwV3dCrossProduct( &vRight, &CNtlPLGlobal::m_vYAxisV3, &vCharToCamDir );
	if ( RwV3dNormalize( &vRight, &vRight ) < 0.0001f )
	{
		return FALSE;
	}

	RwV3d vUp;
	RwV3dCrossProduct( &vUp, &vCharToCamDir, &vRight );
	if ( RwV3dNormalize( &vUp, &vUp ) < 0.0001f )
	{
		return FALSE;
	}

	RpIntersection sBoundaryIntersData[6];

	for ( int x = 0; x < 2; ++x )
	{
		for ( int y = -1; y < 2; ++y )
		{
			sBoundaryIntersData[x + 2 * (y+1)].type = rpINTERSECTLINE;
			sBoundaryIntersData[x + 2 * (y+1)].t.line.start = vLookAt;
			sBoundaryIntersData[x + 2 * (y+1)].t.line.end = vCameraPos + (vRight * ((0 == x) ? (RwReal)-fRadius : (RwReal)fRadius )) + (vUp * (RwReal)y * fRadius);
		}
	}

	// Boundary ray intersection 검사

	for ( int j = 0; j < 6; ++j )
	{
		for ( RwUInt8 i = 0; i < sSphereIntersResult.byCollCount; ++i )
		{
			RwUInt8 byCollCount = g_CameraInterParam.sLineAtomicData.byCollCount;
			g_CameraInterParam.sLineAtomicData.pAtomic[byCollCount] = sSphereIntersResult.pAtomic[i];
			g_CameraInterParam.sLineAtomicData.bCollision[byCollCount] = FALSE;
			g_CameraInterParam.sLineAtomicData.fMinDist[byCollCount] = RwRealMAXVAL;

			RpAtomicForAllIntersections( sSphereIntersResult.pAtomic[i], &sBoundaryIntersData[j], Collision_CameraLineTriangleCB, NULL );

			if ( g_CameraInterParam.sLineAtomicData.bCollision[byCollCount] )
			{
				g_CameraInterParam.sLineAtomicData.byCollCount++;
			}
		}
	}

	if ( g_CameraInterParam.sLineAtomicData.byCollCount > 0 )
	{
		RwUInt8 byIndex = 255;
		RwReal fMinDist = RwRealMAXVAL;

		for ( RwUInt8 byCnt = 0; byCnt < g_CameraInterParam.sLineAtomicData.byCollCount; ++byCnt )
		{
			if ( g_CameraInterParam.sLineAtomicData.fMinDist[byCnt] < fMinDist )
			{
				fMinDist = g_CameraInterParam.sLineAtomicData.fMinDist[byCnt];
				byIndex = byCnt;
			}
		}

		if ( 255 != byIndex )
		{
			RwV3d* pCollPos = &g_CameraInterParam.sLineAtomicData.vCollPos[byIndex];

			RwV3d vCharToCollDist = *pCollPos - vLookAt;

			RwReal fCamDist = RwV3dDotProduct( &vCharToCollDist, &vCharToCamDir );

			if ( fCamDist - fRadius < 0.0001f )
			{
				vNewPos = vLookAt + vCharToCamDir * 0.01f;
			}
			else
			{
				vNewPos = vLookAt + vCharToCamDir * (fCamDist - fRadius);
			}

			return TRUE;
		}
	}

	return FALSE;
}

RwBool Collision_WorldIntersectionCameraData(const RwV3d *pCameraPos, const RwV3d *pLookAt, RwReal fRadius, RwV3d& vNewPos)
{
	RwV3d vLookAt = *pLookAt;
	RwV3d vCamPos = *pCameraPos;

	RwV3d vBoundaryResult, vCenterResult;

	if ( Collision_CameraBoundaryIntersection( vCamPos, vLookAt, fRadius, vBoundaryResult ) )
	{
		vCamPos = vBoundaryResult;

		if ( Collision_CameraCenterIntersection( vCamPos, vLookAt, fRadius, vCenterResult ) )
		{
			vCamPos = vCenterResult;

			if ( Collision_CameraBoundaryIntersection( vCamPos, vLookAt, fRadius, vBoundaryResult ) )
			{
				vNewPos = vBoundaryResult;
			}
			else
			{
				vNewPos = vCenterResult;
			}
		}
		else
		{
			vNewPos = vBoundaryResult;
		}

		return TRUE;
	}
	else
	{
		if ( Collision_CameraCenterIntersection( vCamPos, vLookAt, fRadius, vCenterResult ) )
		{
			vCamPos = vCenterResult;

			if ( Collision_CameraBoundaryIntersection( vCamPos, vLookAt, fRadius, vBoundaryResult ) )
			{
				vNewPos = vBoundaryResult;
			}
			else
			{
				vNewPos = vCenterResult;
			}

			return TRUE;
		}
		else
		{
			return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}
