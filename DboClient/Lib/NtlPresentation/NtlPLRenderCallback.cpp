#include "precomp_ntlpresentation.h"
#include "NtlPLRenderCallback.h"
#include "NtlDebug.h"
#include "NtlMath.h"
#include "NtlAtomic.h"
#include "NtlPLAtomicSorter.h"
#include "NtlPLEntity.h"
#include "ntlworldplugin.h"
#include "NtlPLRenderState.h"
#include "NtlPLSceneManager.h"
#include "NtlWorldSector.h"
#include "NtlProfiler.h"
#include "NtlWorldInterface.h"

#include "NtlPLWorldState.h"

#include "NtlPLRenderGeometry.h"

#include "NtlPLWorldEntity.h"
#include "NtlPLObject.h"
#include "NtlPLPlant.h"


#include "NtlWorldFieldManager.h"
#include "NtlWorldFieldManager4RwWorld.h"

//------------------------------------------------------------------------------------------------
// render callback

static RpWorldSectorCallBackRender g_OldWorldSectorRenderCallback;

RpWorldSector *NtlWorldSectorWaterRender(RpWorldSector *pRpWorldSector, const RwV3d *pCameraPos)
{
	NTL_SPROFILE("NtlWorldSectorWaterRender")

	sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(pRpWorldSector, pNtlSector);

	if(pNtlSector && pNtlSector->pNtlWorldSector)
	{
		if(pNtlSector->pNtlWorldSector->m_pWater)
		{
			RpAtomic *pAtomic = pNtlSector->pNtlWorldSector->m_pWater->_pAtom;

			if(pAtomic)
			{
				if (RwCameraFrustumTestSphere((RwCamera*)RWSRCGLOBAL(curCamera), RpAtomicGetWorldBoundingSphere(pAtomic)) != rwSPHEREOUTSIDE)
				{
					const RwSphere* atomicBoundingSphere = RpAtomicGetWorldBoundingSphere( pAtomic );
					GetAtomicSorterContainer()->Push(PLSORT_WATER, pNtlSector->pNtlWorldSector, pCameraPos);
				}
			}
		}
	}

	NTL_RPROFILE(pRpWorldSector)
}

RpWorldSector *NtlWorldSectorPlantRender(RpWorldSector *pRpWorldSector)
{
	NTL_SPROFILE("NtlWorldSectorPlantRender")

	sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(pRpWorldSector, pNtlSector);

	if(pNtlSector && pNtlSector->pNtlWorldSector && !pNtlSector->pNtlWorldSector->m_bCullCheck)
	{
		if(GetSceneManager()->GetPlant()&& GetSceneManager()->GetPlant()->IsVisible() && !pNtlSector->pNtlWorldSector->m_vecPlantSet.empty())
		{
			GetSceneManager()->GetPlant()->RenderSector(pNtlSector->pNtlWorldSector);
		}
	}

	NTL_RPROFILE(pRpWorldSector)
}

void NtlAtomicRender(RpAtomic * pAtomic, const RwV3d *pCameraPos)
{
	NTL_SPROFILE("NtlAtomicRender")

		RwUInt32 uiRenderFlags = RpNtlAtomicGetFlag(pAtomic);
	if(uiRenderFlags & NTL_FINAL_SORT)
	{
		// sorter에 atomic을 추가한다.	
		GetAtomicSorterContainer()->Push(PLSORT_ATOMIC, pAtomic, pCameraPos);
	}
	else
	{
		if(uiRenderFlags & NTL_ALPHATEST)
			BeginNtlAotmicAlphaTestState();
		if(uiRenderFlags & NTL_TWOSIDE)
			BeginNtlAtomicTwoSideState();

		pAtomic->renderCallBack(pAtomic);

		if(uiRenderFlags & NTL_TWOSIDE)
			EndNtlAtomicTwoSideState();
		if(uiRenderFlags & NTL_ALPHATEST)
			EndNtlAtomicAlphaTestState();
	}

	NTL_EPROFILE()
}

//#ifdef dNTL_WORLD_TOOL_MODE
RpWorldSector* NtlWorldSectorAtomicCallback(RpWorldSector *pWorldSector, void *data) 
{
	EActiveWorldType eActiveWorldType = GetSceneManager()->GetActiveWorldType();
	switch (eActiveWorldType)
	{
	case AW_HEGITHFIELD:
		{
// 			sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(pWorldSector, pNtlSector);
// 
// 			if(pNtlSector && pNtlSector->pNtlWorldSector)
// 			{
// 				if (pNtlSector->pNtlWorldSector->m_bCullCheck)
// 				{
// 					return pWorldSector;
// 				}
// 			}
		}
		break;
	case AW_RWWORLD:
		{
			if (!RpPVSWorldSectorVisible(pWorldSector) && CNtlPLGlobal::m_UseTerrainPVSMode)
			{
				return pWorldSector;
			}
		}
		break;
	}

	RwLLLink			*cur, *end;
	RpTie				*tie;
	RpAtomic			*atomic;
	CNtlPLEntity		*pPLEntity = NULL;


	RwFrame *cameraframe	= RwCameraGetFrame(RwCameraGetCurrentCamera());
	RwV3d	*camPos			= RwMatrixGetPos(RwFrameGetLTM(cameraframe));

	CNtlPLAtomicSorterContainter *pSorterContainer = GetAtomicSorterContainer();

	/* Coll objects first */
	cur = rwLinkListGetFirstLLLink(&pWorldSector->collAtomicsInWorldSector);
	end = rwLinkListGetTerminator(&pWorldSector->collAtomicsInWorldSector);

	while (cur != end)
	{
		/* Render the sucker */
		tie			= rwLLLinkGetData(cur, RpTie, lAtomicInWorldSector);
		atomic		= tie->apAtom;
		pPLEntity	= reinterpret_cast<CNtlPLEntity*>(RpNtlAtomicGetData(atomic));

		if (rwObjectTestFlags(atomic, rpATOMICRENDER))
		{
			/* Don't render objects multiple times within one frame - there's no point */
			if (atomic->renderFrame != RWSRCGLOBAL(renderFrame))
			{				
				if (pPLEntity)
				{
					ENtlPLEntityType eNtlPLEntityType = pPLEntity->GetClassType();

#ifdef dNTL_WORLD_CULLING_NEW // CullingTest의 성공 유무는 IsVisible을 체크 하는 것과 같다.
					if (eNtlPLEntityType == PLENTITY_OBJECT || eNtlPLEntityType == PLENTITY_CHARACTER || eNtlPLEntityType == PLENTITY_ITEM)
					{
						if (pPLEntity->CullingTest((RwCamera*)RWSRCGLOBAL(curCamera), CNtlPLGlobal::m_uiRenderFrame))
						{
							cur					= rwLLLinkGetNext(cur);
							continue;
						}
					}
					else
					{
						if (!pPLEntity->IsVisible())
						{
							cur					= rwLLLinkGetNext(cur);
							continue;
						}

						if (RwCameraFrustumTestSphere((RwCamera*)RWSRCGLOBAL(curCamera), RpAtomicGetWorldBoundingSphere(atomic)) == rwSPHEREOUTSIDE)
						{
							cur					= rwLLLinkGetNext(cur);
							continue;
						}
					}
#else
					if (!pPLEntity->IsVisible())
					{
						cur					= rwLLLinkGetNext(cur);
						continue;
					}					
#endif						

#ifdef dNTL_WORLD_TOOL_MODE
					if (CNtlPLGlobal::m_IsMinimapRendering && !(pPLEntity->GetMinimapLayer() & CNtlPLGlobal::m_uiMinimapLayer))
					{
						cur					= rwLLLinkGetNext(cur);
						continue;
					}
#endif

					if (!CNtlPLGlobal::m_bCollObjVisible && (RpNtlAtomicGetFlag(atomic) & NTL_NOT_VISIBLE))
					{
						cur					= rwLLLinkGetNext(cur);
						continue;
					}

					if (eNtlPLEntityType == PLENTITY_OBJECT)
					{
						if (eActiveWorldType == AW_RWWORLD && CNtlPLGlobal::m_UseTerrainPVSMode)
						{

							if (!static_cast<CNtlPLObject*>(pPLEntity)->IsRpWorldSectorList(pWorldSector))
							{
								cur					= rwLLLinkGetNext(cur);
								continue;
							}
						}

#ifdef dNTL_WORLD_TOOL_MODE
						if (!CNtlPLGlobal::m_bObjectRender)
						{
							cur					= rwLLLinkGetNext(cur);
							continue;
						}
						
						if(CNtlPLGlobal::m_IsMinimapRendering)
						{
							RwBool bMiniMapPassObject = FALSE;
							for (int i = 0; i < (int)CNtlPLGlobal::m_vecPathMinimapObject.size(); ++i)
							{
								if (!strcmp(CNtlPLGlobal::m_vecPathMinimapObject.at(i).c_str() , static_cast<CNtlPLObject*>(pPLEntity)->GetName()))
								{
									bMiniMapPassObject = TRUE;
									break;
								}
							}
							if (bMiniMapPassObject)
							{
								cur					= rwLLLinkGetNext(cur);
								continue;
							}
						}
#endif					
#ifdef dNTL_WORLD_TOOL_MODE
						if (CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_MINI_INDOOR_CULL))
						{
#endif
						if (dGET_BRUSH_ENTITY() != static_cast<CNtlPLObject*>(pPLEntity))
						{

							if (CNtlPLWorldState::GetActiveMiniIndoor())
							{
								if (CNtlPLWorldState::GetActiveMiniIndoor()->GetObjectType() == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE)
								{
									if (static_cast<CNtlPLObject*>(pPLEntity)->GetObjectType() == EPL_OBJECT_TYPE_OUTDOOR_OBJECT)
									{
										cur					= rwLLLinkGetNext(cur);
										continue;
									}
								}
							}
							else
							{
								if (static_cast<CNtlPLObject*>(pPLEntity)->GetObjectType() == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE)
								{
									cur					= rwLLLinkGetNext(cur);
									continue;
								}
							}
						}
#ifdef dNTL_WORLD_TOOL_MODE
						}
#endif
					}

					NtlAtomicRender(atomic, camPos);	
#ifdef _DEBUG
					++CNtlPLGlobal::m_uiRenderAtomicCnt;
#endif

					// Soft spots won't be deleted without this
					_rwD3D9LightsEnable(TRUE, rpATOMIC);
				}
				else
				{
					// atomic render
					if (RwCameraFrustumTestSphere((RwCamera*)RWSRCGLOBAL(curCamera), RpAtomicGetWorldBoundingSphere(atomic)) != rwSPHEREOUTSIDE)
					{
						NtlAtomicRender(atomic, camPos);

						// Soft spots won't be deleted without this
						_rwD3D9LightsEnable(TRUE, rpATOMIC);
					}
				}

				/* We don't really want to do the frustrum test for
				* the next sector too - it'll give the same answer
				*/
				atomic->renderFrame = RWSRCGLOBAL(renderFrame);
			}
		}

		/* Next */
		cur = rwLLLinkGetNext(cur);
	}

	return pWorldSector;
}
// #else
// RpWorldSector* NtlWorldSectorAtomicCallback(RpWorldSector *pWorldSector, void *data) 
// {
// 	if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
// 	{
// 		if (!RpPVSWorldSectorVisible(pWorldSector) && CNtlPLGlobal::m_UseTerrainPVSMode)
// 		{
// 			return pWorldSector;
// 		}
// 	}
// 
// 	RwLLLink           *cur, *end;
// 	RpTie              *tie;
// 	RpAtomic           *atomic;
// 
// 	RwFrame *cameraframe = RwCameraGetFrame(RwCameraGetCurrentCamera());
// 	RwV3d *camPos = RwMatrixGetPos(RwFrameGetLTM(cameraframe));
// 
// 	CNtlPLEntity *pPLEntity;
// 	CNtlPLAtomicSorterContainter *pSorterContainer = GetAtomicSorterContainer();
// 
// 	/* Coll objects first */
// 	cur = rwLinkListGetFirstLLLink(&pWorldSector->collAtomicsInWorldSector);
// 	end = rwLinkListGetTerminator(&pWorldSector->collAtomicsInWorldSector);
// 
// 	while (cur != end)
// 	{
// 		/* Render the sucker */
// 		tie = rwLLLinkGetData(cur, RpTie, lAtomicInWorldSector);
// 		atomic = tie->apAtom;
// 
// 		if (rwObjectTestFlags(atomic, rpATOMICRENDER))
// 		{
// 			/* Don't render objects multiple times within one frame - there's no point */
// 			if (atomic->renderFrame != RWSRCGLOBAL(renderFrame))
// 			{
// 				//const RwSphere     *atomicBoundingSphere;
// 
// 				/* Frustum test it */
// 				//atomicBoundingSphere = RpAtomicGetWorldBoundingSphere(atomic);
// 
// 				//RWS_PRE(atomicBoundingSphere);
// 
// 				//if (RwCameraFrustumTestSphere((RwCamera *) RWSRCGLOBAL(curCamera),	atomicBoundingSphere) != rwSPHEREOUTSIDE)
// 				{
// 					pPLEntity = reinterpret_cast<CNtlPLEntity*>(RpNtlAtomicGetData(atomic));
// 					if(pPLEntity)
// 					{						 
// 						if (pPLEntity->IsVisible() && (RwCameraFrustumTestSphere((RwCamera*)RWSRCGLOBAL(curCamera), RpAtomicGetWorldBoundingSphere(atomic)) != rwSPHEREOUTSIDE))
// 						{							
// 							if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD && pPLEntity->GetClassType() == PLENTITY_OBJECT && CNtlPLGlobal::m_UseTerrainPVSMode)
// 							{
// 								RwBBox bbox = pPLEntity->GetTriggerAABBInfo();
// 								if (bbox.inf != bbox.sup && !CNtlMath::BBoxBBoxCollision(&bbox, RpWorldSectorGetBBox(pWorldSector)))
// 								{									
// 									//atomic->renderFrame	= RWSRCGLOBAL(renderFrame);
// 									cur					= rwLLLinkGetNext(cur);
// 									continue;
// 								}
// 							}
// 							if(CNtlPLGlobal::m_bCollObjVisible)
// 							{
// 								NtlAtomicRender(atomic, camPos);
// 							}
// 							else
// 							{
// #ifdef dNTL_WORLD_TOOL_MODE
// 								// World Tool에서 Object Visible을 Off 시킨 경우 Object는 렌더링 하지 않는다. (07.10.24 by agebreak)
// 								if(CNtlPLGlobal::m_bObjectRender || pPLEntity->GetClassType() != PLENTITY_OBJECT )
// 								{
// 									if( !(RpNtlAtomicGetFlag(atomic) & NTL_NOT_VISIBLE) )
// 									{
// 										NtlAtomicRender(atomic, camPos);	
// 
// 										// Soft spots won't be deleted without this
// 										_rwD3D9LightsEnable(TRUE, rpATOMIC);
// 									}
// 								}
// #else
// 								if( !(RpNtlAtomicGetFlag(atomic) & NTL_NOT_VISIBLE) )
// 								{
// 									NtlAtomicRender(atomic, camPos);	
// 
// 									// Soft spots won't be deleted without this
// 									_rwD3D9LightsEnable(TRUE, rpATOMIC);
// 								}
// #endif
// 
// 							}
// 						}
// 					}
// 					else
// 					{
// 						// atomic render
// 						if (RwCameraFrustumTestSphere((RwCamera *) RWSRCGLOBAL(curCamera),	RpAtomicGetWorldBoundingSphere(atomic)) != rwSPHEREOUTSIDE)
// 						{
// 							NtlAtomicRender(atomic, camPos);
// 						}
// 					}
// 				}
// 
// 				/* We don't really want to do the frustrum test for
// 				* the next sector too - it'll give the same answer
// 				*/
// 				atomic->renderFrame = RWSRCGLOBAL(renderFrame);
// 			}
// 		}
// 
// 		/* Next */
// 		cur = rwLLLinkGetNext(cur);
// 	}
// 
// 
// 	if(GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
// 	{
// 		// height terrain sector water
// 		NtlWorldSectorPlantRender( pWorldSector );
// 		NtlWorldSectorWaterRender( pWorldSector, camPos);
// 	}
// 
// 	return pWorldSector;
// }
// #endif

RpWorldSector* NtlWorldSectorRenderCallback(RpWorldSector *pWorldSector, void *data)
{
	NTL_SPROFILE("NtlWorldSectorRenderCallback")

#ifdef dNTL_WORLD_TOOL_MODE
	if (CNtlPLGlobal::m_IsMinimapRendering && !(CNtlPLGlobal::m_uiMinimapLayer & NTL_PLEFLAG_MINIMAP_LAYER_ONE))
	{
		NTL_RPROFILE(pWorldSector)
	}
#endif

	// world terrain rendering
	if(CNtlPLGlobal::m_bWorldTerrainVisible)
	{
		switch (GetSceneManager()->GetActiveWorldType())
		{
		case AW_RWWORLD:
			{
				NTL_SPROFILE("RpWorldSectorRender")

					RpWorldSectorRender(pWorldSector);

				// height terrain sector water / plant
				((CNtlWorldFieldManager4RWWorld*)GetSceneManager()->GetWorld()->GetWorldFieldMgr())->AddRenderSector(pWorldSector);

				NTL_EPROFILE()
			}
			break;
		case AW_HEGITHFIELD:
			{
				NTL_SPROFILE("RpNtlWorldSectorRender")

				// height field terrain sector
				RpNtlWorldSectorRender(pWorldSector);		

				// height terrain sector water / plant
				NtlWorldSectorPlantRender( pWorldSector );
				NtlWorldSectorWaterRender( pWorldSector, RwMatrixGetPos(RwFrameGetLTM(RwCameraGetFrame(RwCameraGetCurrentCamera()))));

				NTL_EPROFILE()
			}
			break;
		}
	}

	NTL_RPROFILE(pWorldSector)
}

// #ifndef dNTL_WORLD_CULLING_NEW
// RpWorldSector* NtlWorldSectorCullingCallback(RpWorldSector *pWorldSector, void *data)
// {
// 	NTL_SPROFILE("NtlWorldSectorCullingCallback")
// 
// 	if(GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
// 	{
// 		CNtlWorldFieldManager4RWWorld* pMgr = (CNtlWorldFieldManager4RWWorld*)GetSceneManager()->GetWorld()->GetWorldFieldMgr();
// 
// 		CNtlWorldFieldManager4RWWorld::WORLD_SECTOR_VEC::const_iterator it		= pMgr->GetRenderSector()->begin();
// 		CNtlWorldFieldManager4RWWorld::WORLD_SECTOR_VEC::const_iterator itEnd	= pMgr->GetRenderSector()->end();
// 
// 		while (it != itEnd)
// 		{
// 			CNtlWorldSector* pNtlWorldSector = *it;
// 			std::vector<CNtlPLEntity*>::iterator itEntity		= pNtlWorldSector->m_vecNtlPLEntityOrnament.begin();
// 			std::vector<CNtlPLEntity*>::iterator itEntityEnd	= pNtlWorldSector->m_vecNtlPLEntityOrnament.end();
// 
// 			while (itEntity != itEntityEnd)
// 			{
// 				(*itEntity)->CullingTest((RwCamera*)data);
// 				++itEntity;
// 			}
// 			++it;
// 		}
// 
// 		NTL_RPROFILE(NULL);
// 	}
// 	else if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
// 	{		
// 		sNtlWorldSector* pNtlSector = dNTL_WORLD_LOCAL(pWorldSector, pNtlSector);
// 		if(pNtlSector && pNtlSector->pNtlWorldSector)
// 		{
// 			std::vector<CNtlPLEntity*>::iterator itEntity		= pNtlSector->pNtlWorldSector->m_vecNtlPLEntityOrnament.begin();
// 			std::vector<CNtlPLEntity*>::iterator itEntityEnd	= pNtlSector->pNtlWorldSector->m_vecNtlPLEntityOrnament.end();
// 
// 			while (itEntity != itEntityEnd)
// 			{
// 				(*itEntity)->CullingTest((RwCamera*)data);
// 				++itEntity;
// 			}
// 		}
// 	}
// 
// 	NTL_RPROFILE(pWorldSector)
// }
// #endif

// 인도어
RpWorldSector* NtlWorldSectorRenderCallBack_PVSBBox(RpWorldSector* worldSector)
{
	RwRGBA			rgbaColor	= { 0, 0, 0, 25};
	const RwBBox*	bbox		= RpWorldSectorGetBBox(worldSector);
	RwV3d*			pViewPos	= RwMatrixGetPos(RwFrameGetLTM(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));

	if ((CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_PVS_LOCKING) && worldSector == CNtlPLGlobal::m_RpWorldSector) ||
		(pViewPos->x <= bbox->sup.x && pViewPos->x >= bbox->inf.x && pViewPos->y <= bbox->sup.y &&
		pViewPos->y >= bbox->inf.y && pViewPos->z <= bbox->sup.z && pViewPos->z >= bbox->inf.z))
	{
		rgbaColor.red	= 255;
		rgbaColor.green	= 255;
		rgbaColor.blue	= 255;
	}

	RenderGeometryBox(NULL, bbox, &rgbaColor, &rgbaColor, 0.0f, FALSE);

	return worldSector;
}

RpWorldSector* NtlWorldSectorRenderCallBack_PVSWire(RpWorldSector* worldSector)
{
	if (worldSector->numTriangles <= 0)
	{
		return worldSector;
	}

	RwIm3DVertex*		pVertices	= NTL_NEW RwIm3DVertex[worldSector->numVertices];
	RwImVertexIndex*	pIndices	= NTL_NEW RwImVertexIndex[worldSector->numTriangles * 3];

	RwRGBA				rgbaColor	= { 255, 0, 0, 255 };

	for (int i = 0; i < worldSector->numVertices; ++i)
	{
		RwIm3DVertexSetPos(&pVertices[i], worldSector->vertices[i].x, worldSector->vertices[i].y, worldSector->vertices[i].z);
		RwIm3DVertexSetRGBA(&pVertices[i], rgbaColor.red, rgbaColor.green, rgbaColor.blue, rgbaColor.alpha);
	}

	for (int i = 0; i < worldSector->numTriangles; ++i)
	{
		pIndices[i * 3 + 0] = worldSector->triangles[i].vertIndex[0];
		pIndices[i * 3 + 1] = worldSector->triangles[i].vertIndex[1];
		pIndices[i * 3 + 2] = worldSector->triangles[i].vertIndex[2];
	}

	RwD3D9SetTexture(NULL, 0);
	RwD3D9SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	if( RwIm3DTransform(pVertices, worldSector->numVertices, (RwMatrix *)NULL, rwIM3D_ALLOPAQUE) )
	{
		RwIm3DRenderIndexedPrimitive(rwPRIMTYPETRILIST, pIndices, worldSector->numTriangles * 3);

		RwIm3DEnd();
	}
	RwD3D9SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	
	NTL_ARRAY_DELETE(pIndices);
	NTL_ARRAY_DELETE(pVertices);

	return worldSector;
}

RpWorldSector* NtlWorldSectorRenderCallBack_PVS(RpWorldSector* worldSector)
{
	NTL_SPROFILE("NtlWorldSectorRenderCallBack_PVS")

 	if ((CNtlPLGlobal::m_RpWorldSector && RpPVSWorldSectorVisible(worldSector)) || !CNtlPLGlobal::m_UseTerrainPVSMode)
	{	
		CNtlWorldFieldManager4RWWorld* pMgr = (CNtlWorldFieldManager4RWWorld*)GetSceneManager()->GetWorld()->GetWorldFieldMgr();

		if (!pMgr->RpSectorOccluderQuery(worldSector))
		{
			_rpSectorDefaultRenderCallBack(worldSector);
		}
	}
#ifdef dNTL_WORLD_TOOL_MODE
	else if (CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_BSP_WIREFRAME_NV))
	{
		NtlWorldSectorRenderCallBack_PVSWire(worldSector);
	}

	if (CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_BSP_WIREFRAME))
	{
		NtlWorldSectorRenderCallBack_PVSWire(worldSector);
	}

	if (CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_BSP_SECTOR))
	{
		NtlWorldSectorRenderCallBack_PVSBBox(worldSector);
	}
#endif

	NTL_RPROFILE(worldSector);
}