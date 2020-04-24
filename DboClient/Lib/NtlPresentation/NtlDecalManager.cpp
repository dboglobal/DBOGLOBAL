#include "precomp_ntlpresentation.h"
#include "NtlDebug.h"
#include "NtlAtomic.h"
#include "NtlDecalManager.h"
#include "NtlMath.h"
#include "NtlPLRenderState.h"
#include "NtlPLVisualManager.h"
#include "NtlPLWorldEntity.h"

//RpIntersection g_InterZone;

RwUInt8 g_byDecalAlpha = 255;
RwUInt8 g_byDecalRed = 255;
RwUInt8 g_byDecalGreen = 255;
RwUInt8 g_byDecalBlue = 255;

//void CalcIntersectionZone(RwV3d *pPos, RwReal fIntersectionRadius, RwReal fHeightRatio = 1.f)
//{
//	RwV3d v3Box;
//
//	g_InterZone.type = rpINTERSECTBOX;
//
//	v3Box.x = v3Box.y = v3Box.z = fIntersectionRadius;
//	
//	v3Box.y = v3Box.y * fHeightRatio;
//
//	RwV3dAdd(&g_InterZone.t.box.sup, pPos, &v3Box);
//	RwV3dSub(&g_InterZone.t.box.inf, pPos, &v3Box);
//}

static RpCollisionTriangle *
ShadowRenderTriangleCB(RpIntersection * intersection __RWUNUSED__,
                       RpWorldSector * worldSector __RWUNUSED__,
                       RpCollisionTriangle * collTriangle,
                       RwReal distance __RWUNUSED__, void *data)
{

	RwV3d vIn[3], vInTex[3], vShad[3];

	DecalCallbackParam *pDecalParam = (DecalCallbackParam*) data;

	vIn[0] = *(collTriangle->vertices[0]);
    vIn[1] = *(collTriangle->vertices[1]);
    vIn[2] = *(collTriangle->vertices[2]);

	RwV3dAdd(&vInTex[0], &vIn[0], &pDecalParam->vPos); 
	RwV3dAdd(&vInTex[1], &vIn[1], &pDecalParam->vPos);
	RwV3dAdd(&vInTex[2], &vIn[2], &pDecalParam->vPos);

	RwV3dTransformPoints(&vShad[0], &vInTex[0], 3, pDecalParam->matTrans);

	RwInt32 nBuffCnt = *(pDecalParam->pBufferCnt);  
	RwIm3DVertex *pImv = pDecalParam->pVertices + nBuffCnt;

	RwIm3DVertexSetPos(pImv, vIn[0].x, vIn[0].y, vIn[0].z);
    RwIm3DVertexSetPos(pImv+1, vIn[1].x, vIn[1].y, vIn[1].z);
    RwIm3DVertexSetPos(pImv+2, vIn[2].x, vIn[2].y, vIn[2].z);

	RwIm3DVertexSetRGBA(pImv, g_byDecalRed, g_byDecalGreen, g_byDecalBlue, g_byDecalAlpha);
	RwIm3DVertexSetRGBA(pImv+1, g_byDecalRed, g_byDecalGreen, g_byDecalBlue, g_byDecalAlpha);
	RwIm3DVertexSetRGBA(pImv+2, g_byDecalRed, g_byDecalGreen, g_byDecalBlue, g_byDecalAlpha);

	RwIm3DVertexSetU(pImv, vShad[0].x);
    RwIm3DVertexSetU(pImv+1, vShad[1].x);
    RwIm3DVertexSetU(pImv+2, vShad[2].x);

    RwIm3DVertexSetV(pImv, vShad[0].z);
    RwIm3DVertexSetV(pImv+1, vShad[1].z);
    RwIm3DVertexSetV(pImv+2, vShad[2].z);

	*(pDecalParam->pBufferCnt) += 3;

    return collTriangle;
}

static RpCollisionTriangle *
ShadowRenderAtomicTriangleCB( RpIntersection * intersection __RWUNUSED__,
                              //RpWorldSector * worldSector __RWUNUSED__,
                              RpCollisionTriangle * collTriangle,
                              RwReal distance __RWUNUSED__, void *data)
{

	RwV3d vIn[3], vInTex[3], vShad[3];

	DecalCallbackParam *pDecalParam = (DecalCallbackParam*) data;

	vIn[0] = *(collTriangle->vertices[0]);
    vIn[1] = *(collTriangle->vertices[1]);
    vIn[2] = *(collTriangle->vertices[2]);

	RwV3dAdd(&vInTex[0], &vIn[0], &pDecalParam->vPos); 
	RwV3dAdd(&vInTex[1], &vIn[1], &pDecalParam->vPos);
	RwV3dAdd(&vInTex[2], &vIn[2], &pDecalParam->vPos);

	RwV3dTransformPoints(&vShad[0], &vInTex[0], 3, pDecalParam->matTrans);
	
	RwInt32 nBuffCnt = *(pDecalParam->pBufferCnt);  
	RwIm3DVertex *pImv = pDecalParam->pVertices + nBuffCnt;

	RwIm3DVertexSetPos(pImv, vIn[0].x, vIn[0].y, vIn[0].z);
    RwIm3DVertexSetPos(pImv+1, vIn[1].x, vIn[1].y, vIn[1].z);
    RwIm3DVertexSetPos(pImv+2, vIn[2].x, vIn[2].y, vIn[2].z);

	RwIm3DVertexSetRGBA(pImv, g_byDecalRed, g_byDecalGreen, g_byDecalBlue, g_byDecalAlpha);
	RwIm3DVertexSetRGBA(pImv+1, g_byDecalRed, g_byDecalGreen, g_byDecalBlue, g_byDecalAlpha);
	RwIm3DVertexSetRGBA(pImv+2, g_byDecalRed, g_byDecalGreen, g_byDecalBlue, g_byDecalAlpha);

	RwIm3DVertexSetU(pImv, vShad[0].x);
    RwIm3DVertexSetU(pImv+1, vShad[1].x);
    RwIm3DVertexSetU(pImv+2, vShad[2].x);

    RwIm3DVertexSetV(pImv, vShad[0].z);
    RwIm3DVertexSetV(pImv+1, vShad[1].z);
    RwIm3DVertexSetV(pImv+2, vShad[2].z);

	*(pDecalParam->pBufferCnt) += 3;

    return collTriangle;
}

//static RpCollisionTriangle *
//ShadowRenderAtomicObjectTriangleCB( RpIntersection * intersection __RWUNUSED__,
//                              //RpWorldSector * worldSector __RWUNUSED__,
//                              RpCollisionTriangle * collTriangle,
//                              RwReal distance __RWUNUSED__, void *data)
//{
//
//	RwV3d vIn[3], vInTex[3], vShad[3];
//
//	DecalCallbackParam *pDecalParam = (DecalCallbackParam*) data;
//
//	vIn[0] = *(collTriangle->vertices[0]);
//    vIn[1] = *(collTriangle->vertices[1]);
//    vIn[2] = *(collTriangle->vertices[2]);
//
//	RwV3dAdd(&vInTex[0], &vIn[0], &pDecalParam->vPos); 
//	RwV3dAdd(&vInTex[1], &vIn[1], &pDecalParam->vPos);
//	RwV3dAdd(&vInTex[2], &vIn[2], &pDecalParam->vPos);
//
//	RwV3dTransformPoints(&vShad[0], &vInTex[0], 3, pDecalParam->matTrans);
//	RwV3dTransformPoints(&vIn[0], &vIn[0], 3, pDecalParam->matAtomicTrans);
//
//	RwInt32 nBuffCnt = *(pDecalParam->pBufferCnt);  
//	RwIm3DVertex *pImv = pDecalParam->pVertices + nBuffCnt;
//
//	RwIm3DVertexSetPos(pImv, vIn[0].x, vIn[0].y, vIn[0].z);
//    RwIm3DVertexSetPos(pImv+1, vIn[1].x, vIn[1].y, vIn[1].z);
//    RwIm3DVertexSetPos(pImv+2, vIn[2].x, vIn[2].y, vIn[2].z);
//
//	RwIm3DVertexSetRGBA(pImv, g_byDecalRed, g_byDecalGreen, g_byDecalBlue, g_byDecalAlpha);
//	RwIm3DVertexSetRGBA(pImv+1, g_byDecalRed, g_byDecalGreen, g_byDecalBlue, g_byDecalAlpha);
//	RwIm3DVertexSetRGBA(pImv+2, g_byDecalRed, g_byDecalGreen, g_byDecalBlue, g_byDecalAlpha);
//
//	RwIm3DVertexSetU(pImv, vShad[0].x);
//    RwIm3DVertexSetU(pImv+1, vShad[1].x);
//    RwIm3DVertexSetU(pImv+2, vShad[2].x);
//
//    RwIm3DVertexSetV(pImv, vShad[0].z);
//    RwIm3DVertexSetV(pImv+1, vShad[1].z);
//    RwIm3DVertexSetV(pImv+2, vShad[2].z);
//
//	*(pDecalParam->pBufferCnt) += 3;
//
//    return collTriangle;
//}
//RpAtomic* ShadowRenderAtomicCB( RpIntersection* pIntersection, RpWorldSector* pWorldSector, RpAtomic* pAtomic, RwReal fDistance, void* pData )
//{
//	if(RpNtlAtomicGetRenderFlag(pAtomic) & NTL_COLLISION) 
//	{
//		CNtlPLEntity *pPLEntity = reinterpret_cast<CNtlPLEntity*>( RpNtlAtomicGetData(pAtomic) );
//		if(pPLEntity && pPLEntity->GetClassType() == PLENTITY_OBJECT)
//		{
//			RwFrame *pFrame = RpAtomicGetFrame(pAtomic);
//			RwMatrix *pMatrix = RwFrameGetLTM(pFrame);
//
//			RwMatrix *pMatInvLtm = RwMatrixCreate();
//			
//			RwMatrixInvert(pMatInvLtm, pMatrix);
//
//			RwV3d vModelPos;
//			RwV3dTransformPoint(&vModelPos, &g_InterZone.t.point, pMatInvLtm);
//			
//			
//			//vModelPos.x = g_InterZone.t.point.x - pMatrix->pos.x;
//			//vModelPos.y = g_InterZone.t.point.y - pMatrix->pos.y;
//			//vModelPos.z = g_InterZone.t.point.z - pMatrix->pos.z;
//			
//			DecalCallbackParam *param = (DecalCallbackParam *)pData;
//			CalcIntersectionZone(&vModelPos, param->fIntersectionRadius, 0.3f);
//
//			CNtlMath::MathRwV3dAssign(&param->vPos, -vModelPos.x+param->fScale/2.0f, -vModelPos.y, -vModelPos.z+param->fScale/2.0f);
//			
//			RwMatrix *pMatAtomicLtm = RwMatrixCreate();
//			RwMatrixTransform(pMatAtomicLtm, pMatrix, rwCOMBINEREPLACE);
//			pMatAtomicLtm->pos.y += 0.01f;
//			param->matAtomicTrans = pMatAtomicLtm;
//			
//			RpCollisionGeometryForAllIntersections(RpAtomicGetGeometry(pAtomic), &g_InterZone, ShadowRenderAtomicObjectTriangleCB, param);
//			
//			RwMatrixDestroy(pMatInvLtm);
//			RwMatrixDestroy(pMatAtomicLtm);
//		}
//	}
//	
//	return pAtomic;
//}


CNtlDecalManager &GetDecalManager()
{
	static CNtlDecalManager g_DecalManager;
	return g_DecalManager;
}

CNtlDecalManager::CNtlDecalManager()
{
	m_nIm3DBufferCnt = 0;
	memset(m_pIm3DBuffer,0 ,sizeof(RwIm3DVertex)*MAX_DECAL_IM3D); 
}

CNtlDecalManager::~CNtlDecalManager()
{
}

RwBool CNtlDecalManager::Create(void)
{
	return true;
}

void CNtlDecalManager::Destroy(void)
{
	
}


void CNtlDecalManager::RwRender(RpWorld *pWorld, RwCamera *pCamera, RwV3d *pPos,
								RwReal fIntersectionRadius, RwReal fScale, RwReal fVisibleSquaredDist, RwTexture *pTexture)
{
	if(GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
		return;

	if(!GetSceneManager()->IsWorldReady())
		return;

	// 카메라 컬링.
	RwSphere sphere;
	sphere.center = *pPos;
	sphere.radius = 1.0f;
	
	RwFrustumTestResult ret = RwCameraFrustumTestSphere(pCamera, &sphere);
	if(ret == rwSPHEREOUTSIDE)
		return;
	else
	{
		RwReal fCameraDist;
		RwV3d vDist;
		RwV3dSub(&vDist, &sphere.center, &RwCameraGetFrame(pCamera)->modelling.pos);
		fCameraDist = CNtlMath::MathRwV3dSquaredLength(&vDist);
		if(fCameraDist > fVisibleSquaredDist)
			return;
	}
	
	// decal 구하기
	m_nIm3DBufferCnt = 0;
	DecalCallbackParam param;

	CNtlMath::MathRwV3dAssign(&param.vPos, -pPos->x+fScale/2.0f, -pPos->y, -pPos->z+fScale/2.0f);
	param.pBufferCnt = (RwInt32*)&m_nIm3DBufferCnt;
	param.pVertices = m_pIm3DBuffer;
	param.matTrans = &m_matTrans;
	param.fIntersectionRadius = fIntersectionRadius;
	param.fScale = fScale;

	RwMatrixSetIdentity(&m_matTrans); 

	RwV3d scl;
	scl.x = scl.y = scl.z = 1.0f/fScale;
    RwMatrixScale(&m_matTrans, &scl, rwCOMBINEPOSTCONCAT);
	
	//CalcIntersectionZone(pPos, fIntersectionRadius);
	
	//g_InterZone.t.point.x = pPos->x;
	//g_InterZone.t.point.y = pPos->y;
	//g_InterZone.t.point.z = pPos->z;
	
	RpIntersection InterZone;
	InterZone.type = rpINTERSECTSPHERE;
	RwV3dAssign(&InterZone.t.sphere.center, pPos);
	InterZone.t.sphere.radius = fIntersectionRadius;

	//RpWorldForAllAtomicIntersections(pWorld, &InterZone, ShadowRenderAtomicCB, (void *) &param);

	if(pTexture)
	{
		RwD3D9SetTexture(pTexture, 0);
	}

	if(m_nIm3DBufferCnt > 0)
	{
		if(RwIm3DTransform(m_pIm3DBuffer, m_nIm3DBufferCnt, NULL, rwIM3D_VERTEXUV | rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA))
		{
			RwIm3DRenderPrimitive(rwPRIMTYPETRILIST);
			RwIm3DEnd();
		}
	}
}

void CNtlDecalManager::TerrainRender(RpWorld *pWorld, RwCamera *pCamara, const RwV3d& pPos, const RwV3d& size, RwReal fVisibleSquaredDist, RwTexture *pTexture)
{
	if(GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
		return;

	if(!GetSceneManager()->IsWorldReady())
		return;

	// 카메라 컬링.
	RwSphere sphere;
	sphere.center = pPos;
	sphere.radius = 1.f;

	RwFrustumTestResult ret = RwCameraFrustumTestSphere(pCamara, &sphere);
	if(ret == rwSPHEREOUTSIDE)
		return;
	else
	{
		RwReal fCameraDist;
		RwV3d vDist;
		RwV3dSub(&vDist, &sphere.center, &RwCameraGetFrame(pCamara)->modelling.pos);
		fCameraDist = CNtlMath::MathRwV3dSquaredLength(&vDist);
		if(fCameraDist > fVisibleSquaredDist)
			return;
	}


	CNtlPLWorldEntity *pWorldEntity = reinterpret_cast<CNtlPLVisualManager*>( GetSceneManager() )->GetWorld();
	if(!pWorldEntity)
		return;
	
	RwInt32 nRenderVertexCnt;
	RwBool bResult = pWorldEntity->GetWorldDecal((RwV3d)pPos, (RwV3d)size, MAX_DECAL_IM3D, nRenderVertexCnt, m_pIm3DBuffer, 0.01f);
	if(!bResult)
		return;

	for(int i = 0; i < nRenderVertexCnt; ++i)
	{
		RwIm3DVertexSetRGBA(&m_pIm3DBuffer[i], g_byDecalRed, g_byDecalGreen, g_byDecalBlue, g_byDecalAlpha);
	}
	


	//RpAtomic *pAtomic = pWorldEntity->GetAtomic(*pPos);
	//if(pAtomic == NULL)
	//	return;



	//// decal 구하기
	//m_nIm3DBufferCnt = 0;
	//DecalCallbackParam param;

	//CNtlMath::MathRwV3dAssign(&param.vPos, -pPos->x+fScale/2.0f, -pPos->y, -pPos->z+fScale/2.0f);

	//param.pBufferCnt = (RwInt32*)&m_nIm3DBufferCnt;
	//param.pVertices = m_pIm3DBuffer;
	//param.matTrans = &m_matTrans;

	//RwMatrixSetIdentity(&m_matTrans); 

	//RwV3d scl;
	//scl.x = scl.y = scl.z = 1.0f/fScale;
	//RwMatrixScale(&m_matTrans, &scl, rwCOMBINEPOSTCONCAT);

	//CalcIntersectionZone(pPos, fIntersectionRadius);

	//RpCollisionGeometryForAllIntersections(RpAtomicGetGeometry(pAtomic), &g_InterZone, ShadowRenderAtomicTriangleCB, (void *)&param);

	if(pTexture)
	{
		RwD3D9SetTexture(pTexture, 0);
	}

	if(nRenderVertexCnt > 0)
	{
		if(RwIm3DTransform(m_pIm3DBuffer, nRenderVertexCnt, NULL, rwIM3D_VERTEXUV | rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA))
		{
			RwIm3DRenderPrimitive(rwPRIMTYPETRILIST);
			RwIm3DEnd();
		}
	}
}



void CNtlDecalManager::SetAlpha(RwUInt8 byAlpha)
{
	g_byDecalAlpha = byAlpha;
}

void CNtlDecalManager::SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue)
{
	g_byDecalRed = byRed;
	g_byDecalGreen = byGreen;
	g_byDecalBlue = byBlue;
}