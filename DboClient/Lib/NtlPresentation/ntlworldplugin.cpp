/******************************************************************************************************************************
 *
 * ntlworldplugin.cpp
 *
 * copyright (c) 2005 ntl-inc Ltd.
 *
 * author : woody1019
 *
 * purpose : ntl world plugin
 *
 ******************************************************************************************************************************/
#include "precomp_ntlpresentation.h"
#include "ntlworldcommon.h"
#include "ntlworldplugin.h"
#include "ntlworldinterface.h"
#include "NtlWorldMaterialPlugin.h"
#include "ntlworldsectorinfo.h"
#include "NtlPLResourceManager.h"
#include "Ntlcoreutil.h"
#include "NtlWorldSectorLOD.h"
#include "ntlworlddi.h"
#include "NtlPLRenderState.h"
#include "NtlDebug.h"
#include "NtlPLWorldState.h"
#include "NtlPLVisualManager.h"
#include "NtlPLWorldEntity.h"
#include "NtlWorldShadow.h"
#include "NtlWorldSectorPVS.h"
#include "ntlworldinterface.h"
#include "TextureStageStateView.h"
#include "NtlProfiler.h"
#include "NtlPLObject.h"
#include "NtlPLOccluder_Base.h"

#include "NtlPLOptionManager.h"
#include "NtlWorldFieldManager.h"

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifdef dNTL_WORLD_FILE

RwBool	_RpNtlWorldSectorIsFullFogged(CNtlWorldSector *pNtlWorldSector)
{
#ifdef dNTL_WORLD_TOOL_MODE
	if (!CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_FOG_ACTIVATION))
	{
		return pNtlWorldSector->m_bFullFogged = FALSE;
	}
#endif 
	RwMatrix*	pmatCamera = RwFrameGetLTM(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera));
	RwPlane		plane;
	RwUInt32	uiTemp = 0;

	RwD3D9GetRenderState(D3DRS_FOGEND, (void*)&uiTemp);

	plane.normal	= pmatCamera->at;
	plane.distance	= RwV3dDotProduct(&(pmatCamera->at * CNtlMath::DWtoF(uiTemp) + pmatCamera->pos), &pmatCamera->at);

	if (RpNtlFarPlaneFrustumTestSphere(CNtlPLGlobal::m_RwCamera, &pNtlWorldSector->m_pAtomic->worldBoundingSphere, &plane))
	{
		return pNtlWorldSector->m_bFullFogged = FALSE;
	}
	return pNtlWorldSector->m_bFullFogged = TRUE;

// 	CNtlPLVisualManager* pSceneManager = static_cast<CNtlPLVisualManager*>(GetSceneManager());
// 
// 	RwV3d SamplePt;
// 	SamplePt.x = pNtlWorldSector->DatumPoint.x;
// 	SamplePt.z = pNtlWorldSector->DatumPoint.z;
// 	SamplePt.y = pSceneManager->GetWorld()->GetWorldSectorHeight(SamplePt);
// 
// 	RwV3d	CurPos = *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
// 	RwReal	CurDis = RwV3dLength(&(CurPos - SamplePt));
// 
// 	RwUInt32 i = 0;
// 	RwD3D9GetRenderState(D3DRS_FOGEND, (void*)&i);
// 	RwReal FloatDist = CNtlMath::DWtoF(i);
// 
// 	if(CurDis < FloatDist)
// 	{
// 		return pNtlWorldSector->m_bFullFogged = FALSE;
// 	}
// 
// 	SamplePt.x = pNtlWorldSector->DatumPoint.x - dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
// 	SamplePt.z = pNtlWorldSector->DatumPoint.z - dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
// 	SamplePt.y = pSceneManager->GetWorld()->GetWorldSectorHeight(SamplePt);
// 	CurDis = RwV3dLength(&(CurPos - SamplePt));
// 	if(CurDis < FloatDist)
// 	{
// 		return pNtlWorldSector->m_bFullFogged = FALSE;
// 	}
// 
// 	SamplePt.x = pNtlWorldSector->DatumPoint.x + dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
// 	SamplePt.z = pNtlWorldSector->DatumPoint.z - dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
// 	SamplePt.y = pSceneManager->GetWorld()->GetWorldSectorHeight(SamplePt);
// 	CurDis = RwV3dLength(&(CurPos - SamplePt));
// 	if(CurDis < FloatDist)
// 	{
// 		return pNtlWorldSector->m_bFullFogged = FALSE;
// 	}
// 
// 	SamplePt.x = pNtlWorldSector->DatumPoint.x + dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
// 	SamplePt.z = pNtlWorldSector->DatumPoint.z + dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
// 	SamplePt.y = pSceneManager->GetWorld()->GetWorldSectorHeight(SamplePt);
// 	CurDis = RwV3dLength(&(CurPos - SamplePt));
// 	if(CurDis < FloatDist)
// 	{
// 		return pNtlWorldSector->m_bFullFogged = FALSE;
// 	}
// 
// 	SamplePt.x = pNtlWorldSector->DatumPoint.x - dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
// 	SamplePt.z = pNtlWorldSector->DatumPoint.z + dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
// 	SamplePt.y = pSceneManager->GetWorld()->GetWorldSectorHeight(SamplePt);
// 	CurDis = RwV3dLength(&(CurPos - SamplePt));
// 	if(CurDis < FloatDist)
// 	{
// 		return pNtlWorldSector->m_bFullFogged = FALSE;
// 	}
// 
// 	return pNtlWorldSector->m_bFullFogged = TRUE;
}

RwBool _RpNtlWorldSectorCullCheck(CNtlWorldSector *pNtlWorldSector)
{
	// not render
	if(CNtlPLGlobal::m_UseTerrainPVSMode && !pNtlWorldSector->m_pNtlWorldSectorPVS->GetVisibility())
	{
		return pNtlWorldSector->m_bCullCheck = TRUE;
	}

	if(pNtlWorldSector->m_IsCulled)
	{
		return pNtlWorldSector->m_bCullCheck = TRUE;
	}

#ifdef dNTL_WORLD_TOOL_MODE
	if (CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_MINI_INDOOR_CULL))
	{
#endif
		if (CNtlPLWorldState::GetActiveMiniIndoor() && CNtlPLWorldState::GetActiveMiniIndoor()->GetObjectType() == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE)
		{
			return pNtlWorldSector->m_bCullCheck = TRUE;
		}
#ifdef dNTL_WORLD_TOOL_MODE
	}
#endif

	if (RwCameraFrustumTestSphere((RwCamera*)RWSRCGLOBAL(curCamera), RpAtomicGetWorldBoundingSphere(pNtlWorldSector->m_pAtomic)) == rwSPHEREOUTSIDE)
	{
		return pNtlWorldSector->m_bCullCheck = TRUE;
	}

	if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD && pNtlWorldSector->OccluderQuery(COccluderProxy::EOCCLUDER_SPHERE, EPLOCCLUDER_FUNC_TERRAIN, &pNtlWorldSector->m_pAtomic->worldBoundingSphere, 0))
	{
		return pNtlWorldSector->m_bCullCheck = TRUE;
	}

	return pNtlWorldSector->m_bCullCheck = FALSE;
}

void _RpNtlWorldSectorBLayerRender(RxD3D9InstanceData *pInstancedData, RxD3D9ResEntryHeader *pResEntryHeader, RwBool bRenderTex)
{
	sRpNtlWorldSectorSplat	*pNtlWorldSectorSplat	= RpNtlWorldGetSectorSplat(pInstancedData->material);
	CNtlWorldSector			*pNtlWorldSector		= pNtlWorldSectorSplat->pNtlWorldSector;

	RwD3D9SetTexture(NULL, 0);

	if(bRenderTex)
	{
		RwD3D9SetTexture(pNtlWorldSector->m_pTextureBase, 1);
	}
	else
	{
		RwD3D9SetTexture(NULL, 1);		
	}

	RwD3D9SetTransform(D3DTS_TEXTURE0, &pNtlWorldSector->m_TextureMatBase[0]);
	RwD3D9SetTransform(D3DTS_TEXTURE1, &pNtlWorldSector->m_TextureMatBase[1]);

	_RpNtlWorldSectorDrawPrimitive(pInstancedData, pResEntryHeader);
}

void _RpNtlWorldSectorDLayerRender(RxD3D9InstanceData *pInstancedData, RxD3D9ResEntryHeader *pResEntryHeader)
{
	sRpNtlWorldSectorSplat	*pNtlWorldSectorSplat	= RpNtlWorldGetSectorSplat(pInstancedData->material);
	CNtlWorldSector			*pNtlWorldSector		= pNtlWorldSectorSplat->pNtlWorldSector;

	for(RwInt32 i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
	{
		if(pNtlWorldSector->m_pTextureSplat[i] && pNtlWorldSector->m_pTextureAlpha[i])
		{
			RwD3D9SetTexture(pNtlWorldSector->m_pTextureAlpha[i], 0);
			RwD3D9SetTexture(pNtlWorldSector->m_pTextureSplat[i], 1);
			RwD3D9SetTransform(D3DTS_TEXTURE0, &pNtlWorldSector->m_TextureMat[i][0]);
			RwD3D9SetTransform(D3DTS_TEXTURE1, &pNtlWorldSector->m_TextureMat[i][1]);

			_RpNtlWorldSectorDrawPrimitive(pInstancedData, pResEntryHeader);
		}
	}
}

void _rpNtlWorldWaterD3D9RenderCallback(RwResEntry *repEntry, void *object, RwUInt8 type, RwUInt32 flags)
{
	NTL_SPROFILE("_rpNtlWorldWaterD3D9RenderCallback")

	RxD3D9ResEntryHeader *pResEntryHeader;
	RxD3D9InstanceData *pInstancedData;
	RwInt32 numMeshes;

	// get d3d devices
	LPDIRECT3DDEVICE9 pD3DDev = static_cast<LPDIRECT3DDEVICE9>(RwD3D9GetCurrentD3DDevice());

	// Set the Default Pixel shader
	RwD3D9SetPixelShader(NULL);

	// Set clipping
	_rwD3D9EnableClippingIfNeeded(object, type);

	// Get header
	pResEntryHeader = (RxD3D9ResEntryHeader *)(repEntry + 1);

	// Vertex Declaration
	RwD3D9SetVertexDeclaration(pResEntryHeader->vertexDeclaration);

	//* Get the instanced data */
	pInstancedData = (RxD3D9InstanceData*)(pResEntryHeader + 1);

	/* Get the number of meshes */
	numMeshes = pResEntryHeader->numMeshes;
	if (numMeshes)
	{
		do
		{
			RpMaterial*	pMaterial	= pInstancedData->material;
			RwRGBA*		pMatcolor	= &(pMaterial->color);

			RwD3D9SetVertexShader(pInstancedData->vertexShader); 

			sRpNtlWorldSectorSplat*	pNtlWorldSectorSplat	= RpNtlWorldGetSectorSplat(pInstancedData->material);
			CNtlWorldSector*		pNtlWorldSector			= pNtlWorldSectorSplat->pNtlWorldSector;
			CNtlPLVisualManager*	pSceneManager			= static_cast<CNtlPLVisualManager*>(GetSceneManager());

// 			if(_RpNtlWorldSectorCullCheck(pNtlWorldSector))
// 			{
// 				pInstancedData++;
// 				continue;
// 			}
			pSceneManager->GetWorld()->RenderWater(pNtlWorldSector, pInstancedData, pResEntryHeader);
		} while (++pInstancedData, --numMeshes);
	}

	NTL_EPROFILE()
}

void _RpNtlWorldD3D9RenderCallback(RwResEntry *repEntry, void *object, RwUInt8 type, RwUInt32 flags)
{
	NTL_SPROFILE("_RpNtlWorldD3D9RenderCallback")

	RxD3D9ResEntryHeader *pResEntryHeader;
	RxD3D9InstanceData *pInstancedData;
	RwInt32 numMeshes;

	// get d3d devices
	LPDIRECT3DDEVICE9 pD3DDev = static_cast<LPDIRECT3DDEVICE9>(RwD3D9GetCurrentD3DDevice());

	// Set the Default Pixel shader
	RwD3D9SetPixelShader(NULL);

	// Set clipping
	_rwD3D9EnableClippingIfNeeded(object, type);

	// Get header
	pResEntryHeader = (RxD3D9ResEntryHeader *)(repEntry + 1);

	// Vertex Declaration
	RwD3D9SetVertexDeclaration(pResEntryHeader->vertexDeclaration);

	/* Get the instanced data */
	pInstancedData = (RxD3D9InstanceData*)(pResEntryHeader + 1);

	/* Get the number of meshes */
	numMeshes = pResEntryHeader->numMeshes;
	if (numMeshes)
	{
		do
		{
			RpMaterial *pMaterial;
			RwRGBA *pMatcolor;

			pMaterial = pInstancedData->material;
			pMatcolor = &(pMaterial->color);

			// Vertex shader
			RwD3D9SetVertexShader(pInstancedData->vertexShader);

			sRpNtlWorldSectorSplat	*pNtlWorldSectorSplat	= RpNtlWorldGetSectorSplat(pInstancedData->material);
			CNtlWorldSector			*pNtlWorldSector		= pNtlWorldSectorSplat->pNtlWorldSector;

// 			if(_RpNtlWorldSectorCullCheck(pNtlWorldSector))
// 			{
// 				continue;
// 			}

			RwBool bRenderTex = !_RpNtlWorldSectorIsFullFogged(pNtlWorldSector);
			if(bRenderTex && pNtlWorldSector->m_pNtlWorldShadow->m_pTexShadow)
			{

//#ifdef dNTL_WORLD_TOOL_MODE
				if(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_PROJECTION_SHADOW))
//#endif
				{
					BeginTerrrainShadowState();

					RwD3D9SetTransform(D3DTS_TEXTURE2, &pNtlWorldSector->m_pNtlWorldShadow->m_matTex);
					RwD3D9SetTexture(pNtlWorldSector->m_pNtlWorldShadow->m_pTexShadow, 2);
				}
			}

			BegTerrainLayerCommonState();
			{
				BegTerrainBaseLayerState(pNtlWorldSector->m_IsTileTransparency);
				{
					_RpNtlWorldSectorBLayerRender(pInstancedData, pResEntryHeader, bRenderTex);
				}
				EndTerrainBaseLayerState(pNtlWorldSector->m_IsTileTransparency);

				if (bRenderTex)
				{
					BegTerrainDetailLayerState(pNtlWorldSector->m_IsTileTransparency);
					{
						_RpNtlWorldSectorDLayerRender(pInstancedData, pResEntryHeader);
					}
					EndTerrainDetailLayerState(pNtlWorldSector->m_IsTileTransparency);
				}

#ifdef dNTL_WORLD_TOOL_MODE
				_RpNtlWorldSectorWireSolid(pInstancedData, pResEntryHeader);
#endif
			}
			EndTerrainLayerCommonState();

			if(bRenderTex && pNtlWorldSector->m_pNtlWorldShadow->m_pTexShadow)
			{
//#ifdef dNTL_WORLD_TOOL_MODE
				if(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_PROJECTION_SHADOW))
//#endif
				{
					EndTerrrainShadowState();
				}
			}
		} while(++pInstancedData, --numMeshes); // Move onto the next pInstancedData
	}

	NTL_EPROFILE()
}

/******************************************************************************************************************************
* _rpNtlWorldSectorAtomicPipeline : 
******************************************************************************************************************************/
static RxPipeline*	_rpNtlWorldSectorAtomicPipeline(void)
{
	RxPipeline *pipe = RxPipelineCreate();

	if(pipe)
	{
		RxLockedPipe	*lpipe;

		pipe->pluginId = rwID_NTL_WORLD;

		lpipe = RxPipelineLock(pipe);
		if(lpipe)
		{
			RxNodeDefinition *pInstanceNode;

			/* Get the instance pNode definition */
			pInstanceNode = RxNodeDefinitionGetD3D9AtomicAllInOne();

			/* Add the pNode to the pipeline	*/
			lpipe = RxLockedPipeAddFragment(lpipe, NULL, pInstanceNode, NULL);

			/* Unlock the pipeline */
			lpipe = RxLockedPipeUnlock(lpipe);

			return pipe;
		}

		RxPipelineDestroy(pipe);
	}

	return NULL;
}

void rpNtlWorldSectorPipeLineDelete(void)
{
	RxPipelineDestroy(D3D9NtlWorldSectorAtomicPipeline);
	D3D9NtlWorldSectorAtomicPipeline = NULL;

	RxPipelineDestroy(D3D9NtlWorldWaterSectorAtomicPipeline);
	D3D9NtlWorldWaterSectorAtomicPipeline = NULL;
}

RwBool RpNtlWorldSectorGeomSetRenderCB()
{
	// set Lighting callback - just turn it off for now to prevent hw lighting being set up
	RxD3D9AllInOneSetLightingCallBack(g_pNodeWorld, NULL);

	static RxD3D9AllInOneRenderCallBack rxD3D9AllInOneRenderCallBack = NULL;
	static RwBool						bCombine	= FALSE;

#ifdef dNTL_WORLD_COMBINE
	if (CNtlPLOptionManager::GetInstance()->GetTerrainTextrueCombine())
	{
		rxD3D9AllInOneRenderCallBack = _RpNtlWorldD3D9RenderCallback02;
	}
	else
	{
		rxD3D9AllInOneRenderCallBack = NULL;
	}
#endif

	if(!rxD3D9AllInOneRenderCallBack)
	{
		const D3DCAPS9* pD3DCaps = (const D3DCAPS9*)RwD3D9GetCaps();

		if(pD3DCaps->MaxSimultaneousTextures >= 8)
		{
			rxD3D9AllInOneRenderCallBack = _RpNtlWorldD3D9RenderCallback01;
		}	
		else
		{
			rxD3D9AllInOneRenderCallBack = _RpNtlWorldD3D9RenderCallback;
		}
	}

	RxD3D9AllInOneSetRenderCallBack(g_pNodeWorld, rxD3D9AllInOneRenderCallBack);

	return TRUE;
}

RwBool RpNtlWorldSectorWaterGeomSetRenderCB()
{
	// set Lighting callback - just turn it off for now to prevent hw lighting being set up
	RxD3D9AllInOneSetLightingCallBack(g_pNodeWorldWater, NULL);

	// node rendercallback 
	RxD3D9AllInOneSetRenderCallBack(g_pNodeWorldWater, _rpNtlWorldWaterD3D9RenderCallback);

	return TRUE;
}

RwBool rpNtlWorldSectorPipeLineCreate(void)
{
	NTL_FUNCTION("rpNtlWorldSectorPipeLineCreate");

	RxNodeDefinition*	pInstanceNode;
	

	/* create a new atomic pipeline */
	D3D9NtlWorldSectorAtomicPipeline		= _rpNtlWorldSectorAtomicPipeline();
	D3D9NtlWorldWaterSectorAtomicPipeline	= _rpNtlWorldSectorAtomicPipeline();

	if(!D3D9NtlWorldSectorAtomicPipeline || !D3D9NtlWorldWaterSectorAtomicPipeline)
		NTL_RETURN(FALSE);

	pInstanceNode = RxNodeDefinitionGetD3D9AtomicAllInOne();

	g_pNodeWorld		= RxPipelineFindNodeByName(D3D9NtlWorldSectorAtomicPipeline, pInstanceNode->name, NULL, NULL);
	g_pNodeWorldWater	= RxPipelineFindNodeByName(D3D9NtlWorldWaterSectorAtomicPipeline, pInstanceNode->name, NULL, NULL);

	NTL_RETURN(TRUE);
}

/******************************************************************************************************************************
 * NtlWorldGlobalDataConstructor :
 ******************************************************************************************************************************/
static void *NtlWorldGlobalDataConstructor(void *unused, RwInt32 offset __RWUNUSED__, RwInt32 size __RWUNUSED__)
{
	NTL_FUNCTION("NtlWorldGlobalDataConstructor");

	dNTL_WORLD_GLOBAL(WorldSectorCBRender) = RpNtlWorldSectorRender;
	dNTL_WORLD_GLOBAL(pSectorFreelist) = RwFreeListCreate(sizeof(sNtlWorldSector), 4, 16, rwMEMHINTDUR_GLOBAL);
	//RwFreeListCreateAndPreallocateSpace(sizeof(sNtlWorldSector), dGET_WORLD_PARAM()->WorldSectorNum * dGET_WORLD_PARAM()->WorldSectorNum, 4, 1, NULL, rwMEMHINTDUR_GLOBAL);

	rpNtlWorldSectorPipeLineCreate();

	NTL_RETURN(unused);
}

/******************************************************************************************************************************
 * NtlWorldGlobalDataDestructor :
 ******************************************************************************************************************************/
static void *NtlWorldGlobalDataDestructor(void *unused, RwInt32 offset __RWUNUSED__, RwInt32 size __RWUNUSED__)
{
	NTL_FUNCTION("NtlWorldGlobalDataDestructor");

	RwFreeListDestroy(dNTL_WORLD_GLOBAL(pSectorFreelist));

	rpNtlWorldSectorPipeLineDelete();

	NTL_RETURN(unused);
}

/******************************************************************************************************************************
 * NtlWorldLocalDataConstructor :
 ******************************************************************************************************************************/
static void *NtlWorldLocalDataConstructor(void *pWorldSector, RwInt32 offset __RWUNUSED__, RwInt32 size __RWUNUSED__)
{
    if(NtlWorldLocalOffset > 0)
		dNTL_WORLD_LOCAL(pWorldSector, pNtlSector) = NULL;

    return pWorldSector;
}


/******************************************************************************************************************************
 * NtlWorldLocalDataDestructor :
 ******************************************************************************************************************************/
static void *NtlWorldLocalDataDestructor(void *pWorldSector, RwInt32 offset __RWUNUSED__, RwInt32 size __RWUNUSED__)
{
	if(dNTL_WORLD_LOCAL(pWorldSector, pNtlSector))
		dNTL_WORLD_LOCAL(pWorldSector, pNtlSector) = NULL;

    return pWorldSector;
}


/******************************************************************************************************************************
 * NtlWorldLocalDataCopier :
 ******************************************************************************************************************************/
static void *NtlWorldLocalDataCopier(void *pDstWorldSector, const void *pSrcWorldSector, RwInt32 offset __RWUNUSED__, RwInt32 size __RWUNUSED__)
{
	/* Note that copying a world in this way will cause problems on destruction */
    dNTL_WORLD_LOCAL(pDstWorldSector, pNtlSector) = dNTL_WORLD_LOCAL(pSrcWorldSector, pNtlSector);

    return pDstWorldSector;
}

/******************************************************************************************************************************
* NtlWorldSectorDataConstructor :
******************************************************************************************************************************/
static void *NtlWorldSectorLocalDataConstructor(void *pWorldSector, RwInt32 offset __RWUNUSED__, RwInt32 size __RWUNUSED__)
{
	if(NtlWorldSectorLocalOffset > 0)
	{
		dNTL_WORLD_SECTOR_LOCAL(pWorldSector, pParentFrame) = NULL;
	}

	return pWorldSector;
}


/******************************************************************************************************************************
* NtlWorldSectorDataDestructor :
******************************************************************************************************************************/
static void *NtlWorldSectorLocalDataDestructor(void *pWorldSector, RwInt32 offset __RWUNUSED__, RwInt32 size __RWUNUSED__)
{
	if(dNTL_WORLD_SECTOR_LOCAL(pWorldSector, pParentFrame))
	{
		/* Destroy the frame once all the atomics that use it are gone */
		RwFrameDestroy(dNTL_WORLD_SECTOR_LOCAL(pWorldSector, pParentFrame));
		dNTL_WORLD_SECTOR_LOCAL(pWorldSector, pParentFrame) = NULL;
	}

	return pWorldSector;
}


/******************************************************************************************************************************
* NtlWorldSectorDataCopier :
******************************************************************************************************************************/
static void *NtlWorldSectorLocalDataCopier(void *pDstWorldSector, const void *pSrcWorldSector, RwInt32 offset __RWUNUSED__, RwInt32 size __RWUNUSED__)
{
	/* Copying the frame from one world to another like this will cause problems on destruction */
	dNTL_WORLD_SECTOR_LOCAL(pDstWorldSector, pParentFrame) = dNTL_WORLD_SECTOR_LOCAL(pSrcWorldSector, pParentFrame);

	return pDstWorldSector;
}

/******************************************************************************************************************************
 * RpNtlWorldAttach : attach ntl wolrd plugin 
 ******************************************************************************************************************************/
RwBool RpNtlWorldAttach(void)
{
    /* registers a plugin and reserves some space within the global engine space */
    NtlWorldGlobalOffset = RwEngineRegisterPlugin(sizeof(sNtlWorldGlobals), MAKECHUNKID(rwVENDORID_CRITERIONTK, dID_NTL_WORLD),
												  NtlWorldGlobalDataConstructor,
												  NtlWorldGlobalDataDestructor);

    if(NtlWorldGlobalOffset < 0)
        return FALSE;

    /* register a plugin and reserve some space within a world sector */
    NtlWorldLocalOffset = RpWorldSectorRegisterPlugin(sizeof(sNtlWorldLocals), MAKECHUNKID(rwVENDORID_CRITERIONTK, dID_NTL_WORLD),
													  NtlWorldLocalDataConstructor,
													  NtlWorldLocalDataDestructor,
													  NtlWorldLocalDataCopier);

    if(NtlWorldLocalOffset < 0)
        return FALSE;

    /* register a plugin and reserve some space within a world */
    NtlWorldSectorLocalOffset = RpWorldRegisterPlugin(sizeof(sNtlSectorLocals), MAKECHUNKID(rwVENDORID_CRITERIONTK, dID_NTL_WORLD),
													  NtlWorldSectorLocalDataConstructor,
													  NtlWorldSectorLocalDataDestructor,
													  NtlWorldSectorLocalDataCopier);

    if(NtlWorldSectorLocalOffset < 0)
        return FALSE;

    return TRUE;
}


#ifdef dNTL_WORLD_TOOL_MODE

RwBool RpNtlWorldSectorGeometryCreate(CNtlWorldSector *pNtlWorldSector)
{
	NTL_FUNCTION("RpNtlWorldSectorGeometryCreate");

	// 인도어
	switch (GetSceneManager()->GetActiveWorldType())
	{
	case AW_HEGITHFIELD:
		{
			NTL_ASSERTE(dNTL_WORLD_LOCAL(pNtlWorldSector->m_pWorldSector, pNtlSector) == NULL);

			dNTL_WORLD_LOCAL(pNtlWorldSector->m_pWorldSector, pNtlSector) = static_cast<sNtlWorldSector*>(RwFreeListAlloc(dNTL_WORLD_GLOBAL(pSectorFreelist), rwMEMHINTDUR_GLOBAL));
			sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(pNtlWorldSector->m_pWorldSector, pNtlSector);

			pNtlSector->pNtlWorldSector = pNtlWorldSector;
			pNtlSector->pNtlWorldSector->MaterialContructor();

			GetNtlWorldSectorInfo()->NtlWorldSectorGeometryCreate(pNtlWorldSector);			
		}
		break;
	case AW_RWWORLD:
		{
			pNtlWorldSector->MaterialContructor();
		}
		break;
	}

	pNtlWorldSector->m_bLoaded = TRUE;

	NTL_RETURN(TRUE);
}

#endif

RwBool RpNtlWorldSectorGeometryDelete(CNtlWorldSector *pNtlWorldSector)
{
	NTL_FUNCTION("RpNtlWorldSectorGeometryDelete");

	GetNtlWorldSectorInfo()->NtlWorldSectorGeometryDelete(pNtlWorldSector);

	NTL_RETURN(TRUE);
}

RwBool RpNtlWorldSectorGeometryDeleteWithoutSwapFileSaving(CNtlWorldSector *pNtlWorldSector)
{
	NTL_FUNCTION("RpNtlWorldSectorGeometryDelete");

	GetNtlWorldSectorInfo()->NtlWorldSectorGeometryDeleteWithoutSwapFileSaving(pNtlWorldSector);

	NTL_RETURN(TRUE);
}

RwBool RpNtlWorldSectorGeometryCreateFromFile(sCUR_FIELD_TEX_INFO& CurFieldTexInfo, CNtlWorldSector *pNtlWorldSector, FILE* pFile)
{
	NTL_FUNCTION("RpNtlWorldSectorGeometryCreateFromFile");

	// 인도어
	switch (GetSceneManager()->GetActiveWorldType())
	{
	case AW_HEGITHFIELD:
		{
			NTL_ASSERTE(dNTL_WORLD_LOCAL(pNtlWorldSector->m_pWorldSector, pNtlSector) == NULL);

			dNTL_WORLD_LOCAL(pNtlWorldSector->m_pWorldSector, pNtlSector) = static_cast<sNtlWorldSector*>(RwFreeListAlloc(dNTL_WORLD_GLOBAL(pSectorFreelist), rwMEMHINTDUR_GLOBAL));
			sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(pNtlWorldSector->m_pWorldSector, pNtlSector);

			pNtlSector->pNtlWorldSector = pNtlWorldSector;
			pNtlSector->pNtlWorldSector->MaterialContructor();

			GetNtlWorldSectorInfo()->NtlWorldSectorGeometryLoadFromFile(pNtlWorldSector, pFile, CurFieldTexInfo);
		}
		break;
	case AW_RWWORLD:
		{
			pNtlWorldSector->MaterialContructor();

			GetNtlWorldSectorInfo()->NtlWorldSectorGeometryLoadFromFileIndoor(pNtlWorldSector, pFile);
		}
		break;
	}

	pNtlWorldSector->m_bLoaded = TRUE;

	NTL_RETURN(TRUE);
}

/*
RwBool RpNtlWorldSectorGeometryCreateFromFile(sCUR_FIELD_TEX_INFO& CurFieldTexInfo, CNtlWorldSector *pNtlWorldSector, BYTE* pPosCurFileMem)
{
	NTL_FUNCTION("RpNtlWorldSectorGeometryCreateFromFile");

	dNTL_WORLD_LOCAL(pNtlWorldSector->m_pWorldSector, pNtlSector) = static_cast<sNtlWorldSector*>(RwFreeListAlloc(dNTL_WORLD_GLOBAL(pSectorFreelist), rwMEMHINTDUR_GLOBAL));
	sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(pNtlWorldSector->m_pWorldSector, pNtlSector);

	pNtlSector->pNtlWorldSector = pNtlWorldSector;
	pNtlSector->pNtlWorldSector->MaterialContructor();

	_RpNtlWorldSectorGeometryLoadFromFile(CurFieldTexInfo, pNtlWorldSector, pPosCurFileMem);

	NTL_RETURN(TRUE);
}
*/

//인도어
// RwBool RpNtlWorldSectorGeometryDeleteIntoFile(RpWorldSector *pRpWorldSector, FILE* pFile)
// {
// 	NTL_FUNCTION("RpNtlWorldSectorGeometryDeleteIntoFile");
// 
// 	_RpNtlWorldSectorGeometrySaveIntoFile(pRpWorldSector, pFile);
// 
// 	NTL_RETURN(TRUE);
// }

RwBool RpNtlWorldSectorGeometryDeleteIntoFile(CNtlWorldSector *pNtlWorldSector, FILE* pFile)
{
	NTL_FUNCTION("RpNtlWorldSectorGeometryDeleteIntoFile");

	//인도어
	switch (GetSceneManager()->GetActiveWorldType())
	{
	case AW_HEGITHFIELD:
		{
			GetNtlWorldSectorInfo()->NtlWorldSectorGeometrySaveIntoFile(pNtlWorldSector, pFile);
		}
		break;
	case AW_RWWORLD:
		{
			GetNtlWorldSectorInfo()->NtlWorldSectorGeometrySaveIntoFileIndoor(pNtlWorldSector, pFile);
		}
		break;
	}

	NTL_RETURN(TRUE);
}

RwBool RpNtlWorldSectorUpdateTexLyr(sCUR_FIELD_TEX_INFO& CurFieldTexInfo, CNtlWorldSector *pNtlWorldSector, RwInt32 IdxLyr, RwInt32 IdxMenu)
{
	return GetNtlWorldSectorInfo()->NtlWorldSectorUpdateTexLyr(CurFieldTexInfo, pNtlWorldSector, IdxLyr, IdxMenu);
}

RwBool RpNtlWorldSectorUpdateTexMat(sCUR_FIELD_TEX_INFO& CurFieldTexInfo, CNtlWorldSector *pNtlWorldSector, RwInt32 IdxLyr)
{
	return GetNtlWorldSectorInfo()->NtlWorldSectorUpdateTexMat(CurFieldTexInfo, pNtlWorldSector, IdxLyr);
}

/******************************************************************************************************************************
 * RpNtlWorldSectorRender :
 ******************************************************************************************************************************/
RpWorldSector *RpNtlWorldSectorRender(RpWorldSector *pRpWorldSector)
{
	NTL_SPROFILE("RpNtlWorldSectorRender")

	sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(pRpWorldSector, pNtlSector);

	if(pNtlSector && pNtlSector->pNtlWorldSector)
	{
		RpAtomic *pAtomic = pNtlSector->pNtlWorldSector->m_pAtomic;

		if(pAtomic)
		{
			if (pAtomic->renderFrame != RWSRCGLOBAL(renderFrame))
			{
				if(!_RpNtlWorldSectorCullCheck(pNtlSector->pNtlWorldSector))
				{
					RpNtlWorldSectorGeomSetRenderCB();
					RpAtomicRender(pAtomic);
				}				
				pAtomic->renderFrame = RWSRCGLOBAL(renderFrame);
			}
		}
	}
	
	NTL_RPROFILE(pRpWorldSector)
}

// RpWorldSector *RpNtlWorldSectorWaterRender(RpWorldSector *pRpWorldSector)
// {
// 	NTL_SPROFILE("RpNtlWorldSectorWaterRender")
// 
// 	sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(pRpWorldSector, pNtlSector);
// 
// 	if(pNtlSector && pNtlSector->pNtlWorldSector)
// 	{
// 		if(pNtlSector->pNtlWorldSector->m_pWater)
// 		{
// 			RpAtomic *pAtomic = pNtlSector->pNtlWorldSector->m_pWater->_pAtom;
// 
// 			if(pAtomic)
// 			{
// 				RpNtlWorldSectorWaterGeomSetRenderCB();
// 				RpAtomicRender(pAtomic);
// 			}
// 		}
// 	}
// 
// 	NTL_RPROFILE(pRpWorldSector)
// }

CNtlWorldSector *RpNtlWorldSectorWaterRender(CNtlWorldSector *pNtlWorldSector)
{
	NTL_SPROFILE("RpNtlWorldSectorWaterRender")
	if (pNtlWorldSector && pNtlWorldSector->m_pWater)
	{
		RpAtomic* pAtomic = pNtlWorldSector->m_pWater->_pAtom;
		if(pAtomic)
		{
			if (pAtomic->renderFrame != RWSRCGLOBAL(renderFrame))
			{				
				RpNtlWorldSectorWaterGeomSetRenderCB();
				RpAtomicRender(pAtomic);

				pAtomic->renderFrame = RWSRCGLOBAL(renderFrame);
			}
		}
	}
	NTL_RPROFILE(pNtlWorldSector)
}

void _RpNtlWorldD3D9RenderCallback01(RwResEntry *repEntry, void *object, RwUInt8 type, RwUInt32 flags)
{
	RxD3D9ResEntryHeader *pResEntryHeader;
	RxD3D9InstanceData *pInstancedData;
	RwInt32 numMeshes;

	// get d3d devices
	LPDIRECT3DDEVICE9 pD3DDev = static_cast<LPDIRECT3DDEVICE9>(RwD3D9GetCurrentD3DDevice());

	// Set the Default Pixel shader
	RwD3D9SetPixelShader(NULL);

	// Set clipping
	_rwD3D9EnableClippingIfNeeded(object, type);

	// Get header
	pResEntryHeader = (RxD3D9ResEntryHeader *)(repEntry + 1);

	// Vertex Declaration
	RwD3D9SetVertexDeclaration(pResEntryHeader->vertexDeclaration);

	/* Get the instanced data */
	pInstancedData = (RxD3D9InstanceData*)(pResEntryHeader + 1);

	/* Get the number of meshes */
	numMeshes = pResEntryHeader->numMeshes;
	if (numMeshes)
	{
		do
		{
			RpMaterial *pMaterial;
			RwRGBA *pMatcolor;

			pMaterial = pInstancedData->material;
			pMatcolor = &(pMaterial->color);

			// Vertex shader
			RwD3D9SetVertexShader(pInstancedData->vertexShader);

			sRpNtlWorldSectorSplat	*pNtlWorldSectorSplat	= RpNtlWorldGetSectorSplat(pInstancedData->material);
			CNtlWorldSector			*pNtlWorldSector		= pNtlWorldSectorSplat->pNtlWorldSector;

// 			if(_RpNtlWorldSectorCullCheck(pNtlWorldSector))
// 			{
// 				continue;
// 			}

			// CodeInDeviceState : Setting
			BeginLightState(FALSE);
			RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, dGET_WORLD_PARAM()->ClrDayAndNight);

			if (_RpNtlWorldSectorIsFullFogged(pNtlWorldSector))
			{
				RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
				RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
				RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

				RwD3D9SetTexture(NULL, 0);

				if (pNtlWorldSector->m_IsTileTransparency)
				{
					RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)TRUE); 
					RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
					RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
				}
				else
				{
					RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
				}				

				_RpNtlWorldSectorDrawPrimitive(pInstancedData, pResEntryHeader);

				RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
				RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
				RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
				RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
				RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
				RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
				RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
				RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
				RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)FALSE); 
			}
			else
			{
				RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)TRUE); 
				RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
				RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
				
				vector<RwInt32>	vecDetailLayer;
				for (int i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
				{
					if (pNtlWorldSector->m_pTextureAlpha[i] && pNtlWorldSector->m_pTextureSplat[i])
					{
						vecDetailLayer.push_back(i);
					}
				}

				RwInt32			iRenderStage;
				RwBool			bBaseLayer		= TRUE;
				RwInt32			iDetailLayer	= 0;
				do 
				{
					iRenderStage = 0;

					if (bBaseLayer)
					{
						RwD3D9SetTextureStageState(iRenderStage, D3DTSS_RESULTARG, D3DTA_CURRENT);
						RwD3D9SetTextureStageState(iRenderStage, D3DTSS_TEXCOORDINDEX, 0);
						RwD3D9SetTextureStageState(iRenderStage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);

						RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
						RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLORARG1, D3DTA_TEXTURE);
 						RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
 						RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
//						RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

						RwD3D9SetTexture(pNtlWorldSector->m_pTextureBase, iRenderStage);
						RwD3D9SetTransform(D3DTS_TEXTURE0 + iRenderStage, &pNtlWorldSector->m_TextureMatBase[0]);

						++iRenderStage;

						for (int i = 0; i < 2; ++i)
						{
							if (iDetailLayer < (RwInt32)vecDetailLayer.size())
							{
								RwInt32 iDetailLayerIdx = vecDetailLayer.at(iDetailLayer);

								RwD3D9SetTextureStageState(iRenderStage, D3DTSS_RESULTARG, D3DTA_TEMP); 
								RwD3D9SetTextureStageState(iRenderStage, D3DTSS_TEXCOORDINDEX, 0);
								RwD3D9SetTextureStageState(iRenderStage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
								RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLOROP, D3DTOP_SELECTARG1); 
								RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLORARG1, D3DTA_TEXTURE); 
								RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1); 
 								RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
//								RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

								RwD3D9SetTexture(pNtlWorldSector->m_pTextureSplat[iDetailLayerIdx], iRenderStage);
								RwD3D9SetTransform(D3DTS_TEXTURE0 + iRenderStage, &pNtlWorldSector->m_TextureMat[iDetailLayerIdx][1]);

								++iRenderStage;

								RwD3D9SetTextureStageState(iRenderStage, D3DTSS_RESULTARG, D3DTA_CURRENT); 
								RwD3D9SetTextureStageState(iRenderStage, D3DTSS_TEXCOORDINDEX, 0);
								RwD3D9SetTextureStageState(iRenderStage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
								RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLOROP, D3DTOP_BLENDTEXTUREALPHA); 
								RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLORARG1, D3DTA_TEMP);
								RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLORARG2, D3DTA_CURRENT);
 								RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAOP, D3DTOP_BLENDTEXTUREALPHA); 
 								RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAARG1, D3DTA_TEMP);
 								RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
//								RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

								RwD3D9SetTexture(pNtlWorldSector->m_pTextureAlpha[iDetailLayerIdx], iRenderStage);
								RwD3D9SetTransform(D3DTS_TEXTURE0 + iRenderStage, &pNtlWorldSector->m_TextureMat[iDetailLayerIdx][0]);

								++iRenderStage;
								++iDetailLayer;
							}
						}

						bBaseLayer = FALSE;
					}
					else
					{
						if (iDetailLayer < (RwInt32)vecDetailLayer.size())
						{
							RwInt32 iDetailLayerIdx = vecDetailLayer.at(iDetailLayer);

							RwD3D9SetTextureStageState(iRenderStage, D3DTSS_RESULTARG, D3DTA_CURRENT); 
							RwD3D9SetTextureStageState(iRenderStage, D3DTSS_TEXCOORDINDEX, 0);
							RwD3D9SetTextureStageState(iRenderStage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
							RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLOROP, D3DTOP_SELECTARG1); 
							RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLORARG1, D3DTA_TEXTURE); 
							RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1); 
							RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

							RwD3D9SetTexture(pNtlWorldSector->m_pTextureSplat[iDetailLayerIdx], iRenderStage);
							RwD3D9SetTransform(D3DTS_TEXTURE0 + iRenderStage, &pNtlWorldSector->m_TextureMat[iDetailLayerIdx][1]);

							++iRenderStage;

							RwD3D9SetTextureStageState(iRenderStage, D3DTSS_RESULTARG, D3DTA_CURRENT); 
							RwD3D9SetTextureStageState(iRenderStage, D3DTSS_TEXCOORDINDEX, 0);
							RwD3D9SetTextureStageState(iRenderStage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
							RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLOROP, D3DTOP_SELECTARG1); 
							RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLORARG1, D3DTA_CURRENT);
							RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1); 
							RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); 

							RwD3D9SetTexture(pNtlWorldSector->m_pTextureAlpha[iDetailLayerIdx], iRenderStage);
							RwD3D9SetTransform(D3DTS_TEXTURE0 + iRenderStage, &pNtlWorldSector->m_TextureMat[iDetailLayerIdx][0]);

							++iRenderStage;
							++iDetailLayer;
						}
					}

					if (pNtlWorldSector->m_pNtlWorldShadow->m_pTexShadow)
					{
//#ifdef dNTL_WORLD_TOOL_MODE
						if(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_PROJECTION_SHADOW))
//#endif
						{
						RwD3D9SetTextureStageState(iRenderStage, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION);
						RwD3D9SetTextureStageState(iRenderStage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
						RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLOROP, D3DTOP_MODULATE);
						RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLORARG1, D3DTA_TEXTURE);
						RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLORARG2, D3DTA_CURRENT);						
						RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
						RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAARG1, D3DTA_CURRENT);

						RwD3D9SetTransform(D3DTS_TEXTURE0 + iRenderStage, &pNtlWorldSector->m_pNtlWorldShadow->m_matTex);
						RwD3D9SetTexture(pNtlWorldSector->m_pNtlWorldShadow->m_pTexShadow, iRenderStage);

						++iRenderStage;
						}
					}

					RwD3D9SetTextureStageState(iRenderStage, D3DTSS_RESULTARG, D3DTA_CURRENT);
					RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLOROP, D3DTOP_MODULATE);
					RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
					RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLORARG2, D3DTA_CURRENT);						
					RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
					RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
					RwD3D9SetTexture(NULL, iRenderStage);

					++iRenderStage;

					RwD3D9SetTextureStageState(iRenderStage, D3DTSS_RESULTARG, D3DTA_CURRENT);
					RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
					RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLORARG1, D3DTA_CURRENT);
					RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLORARG2, D3DTA_TFACTOR);	
					if (pNtlWorldSector->m_IsTileTransparency)
					{
						RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
						RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
						RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
					}
					else
					{
						RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
						RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
					}

					RwD3D9SetTexture(NULL, iRenderStage);

					++iRenderStage;

					_RpNtlWorldSectorDrawPrimitive(pInstancedData, pResEntryHeader);

					// CodeInTextrueStageState : Restore
					D3DXMATRIXA16 mIdentity;
					D3DXMatrixIdentity(&mIdentity);

					RwD3D9SetTexture(NULL, 0);					
					RwD3D9SetTransform(D3DTS_TEXTURE0, &mIdentity);
					RwD3D9SetTextureStageState(0, D3DTSS_RESULTARG, D3DTA_CURRENT); 
					RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
					RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
					RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
					RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
					RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
					RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
					RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
					RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

					for (int i = 1; i < iRenderStage; ++i)
					{
						RwD3D9SetTexture(NULL, i);
						RwD3D9SetTransform(D3DTS_TEXTURE0 + i, &mIdentity);
						RwD3D9SetTextureStageState(i, D3DTSS_RESULTARG, D3DTA_CURRENT);
						RwD3D9SetTextureStageState(i, D3DTSS_TEXCOORDINDEX, i);
						RwD3D9SetTextureStageState(i, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
						RwD3D9SetTextureStageState(i, D3DTSS_COLOROP, D3DTOP_DISABLE);
						RwD3D9SetTextureStageState(i, D3DTSS_COLORARG1, D3DTA_TEXTURE);
						RwD3D9SetTextureStageState(i, D3DTSS_COLORARG2, D3DTA_CURRENT);
						RwD3D9SetTextureStageState(i, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
						RwD3D9SetTextureStageState(i, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
						RwD3D9SetTextureStageState(i, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
					}
				} while(iDetailLayer < (RwInt32)vecDetailLayer.size());
				
				RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)FALSE); 
				RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)rwBLENDSRCALPHA);
				RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)rwBLENDINVSRCALPHA);
			}
			
#ifdef dNTL_WORLD_TOOL_MODE
			_RpNtlWorldSectorWireSolid(pInstancedData, pResEntryHeader);
#endif
			
			RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);
			EndLightState();

			/*
			toon test
			{
				RwD3D9SetRenderState(D3DRS_LIGHTING, FALSE);
				RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLFRONT);

				RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, 0xff000000);
				RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2); 
				RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_CURRENT);
				RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
				RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
				RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CURRENT);

				RwMatrix matWireWorld;
				RwV3d Pos;
				Pos.x = 1.0001f;
				Pos.y = 1.0001f;
				Pos.z = 1.0001f;

				RwMatrixScale(&matWireWorld, &Pos, rwCOMBINEREPLACE);
				RwD3D9SetTransformWorld(&matWireWorld);

				sRpNtlWorldSectorSplat*	pNtlWorldSectorSplat	= RpNtlWorldGetSectorSplat(pInstancedData->material);
				CNtlWorldSector*		pNtlWorldSector			= pNtlWorldSectorSplat->pNtlWorldSector;
				RwD3D9SetStreamSource(0, (IDirect3DVertexBuffer9*)pResEntryHeader->vertexStream->vertexBuffer, 0, pResEntryHeader->vertexStream->stride);
				RwD3D9SetIndices((IDirect3DIndexBuffer9*)pResEntryHeader->indexBuffer);
				RwD3D9DrawIndexedPrimitive((D3DPRIMITIVETYPE)pResEntryHeader->primType,
					pInstancedData->baseIndex,
					0,
					pInstancedData->numVertices,
					pInstancedData->startIndex,
					pInstancedData->numPrimitives);

				RwD3D9SetTransformWorld(NULL);

				RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);
				RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE); 
				RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE); 
				RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
				RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1); 
				RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
				RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

				RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLBACK);
				RwD3D9SetRenderState(D3DRS_LIGHTING, TRUE);
			}
			*/

		} while(++pInstancedData, --numMeshes); // Move onto the next pInstancedData
	}
}

void _RpNtlWorldSectorDrawPrimitive(RxD3D9InstanceData *pInstancedData, RxD3D9ResEntryHeader *pResEntryHeader)
{
	RwInt32					iPrimitiveNum			= 0;
	sRpNtlWorldSectorSplat*	pNtlWorldSectorSplat	= RpNtlWorldGetSectorSplat(pInstancedData->material);
	CNtlWorldSector*		pNtlWorldSector			= pNtlWorldSectorSplat->pNtlWorldSector;
	if(dGET_WORLD_PARAM()->LODEnable)
	{
		RwD3D9SetStreamSource(0, (IDirect3DVertexBuffer9*)pResEntryHeader->vertexStream->vertexBuffer, pResEntryHeader->vertexStream->offset, pResEntryHeader->vertexStream->stride);

		if(pNtlWorldSector->GetBodyIndexBuff(dGET_SECTOR_LOD()))
		{
			iPrimitiveNum = pNtlWorldSector->GetBodyPrimitiveNum(dGET_SECTOR_LOD());
			RwD3D9SetIndices(pNtlWorldSector->GetBodyIndexBuff(dGET_SECTOR_LOD()));
			RwD3D9DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum, 0, iPrimitiveNum);
			
			dGET_WORLD_PARAM()->WorldPolyCnt += iPrimitiveNum;
		}

		for(int i = 0; i < 5; ++i)
		{
			if(pNtlWorldSector->GetLinkIndexBuff(dGET_SECTOR_LOD(), i))
			{
				iPrimitiveNum = pNtlWorldSector->GetLinkPrimitiveNum(dGET_SECTOR_LOD(), i);
				RwD3D9SetIndices(pNtlWorldSector->GetLinkIndexBuff(dGET_SECTOR_LOD(), i));
				RwD3D9DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum, 0, iPrimitiveNum);
				
				dGET_WORLD_PARAM()->WorldPolyCnt += iPrimitiveNum;
			}
		}
	}
	else
	{
		RwD3D9SetStreamSource(0, (IDirect3DVertexBuffer9*)pResEntryHeader->vertexStream->vertexBuffer, 0, pResEntryHeader->vertexStream->stride);
		RwD3D9SetIndices((IDirect3DIndexBuffer9*)pResEntryHeader->indexBuffer);
		RwD3D9DrawIndexedPrimitive((D3DPRIMITIVETYPE)pResEntryHeader->primType,
									pInstancedData->baseIndex,
									0,
									pInstancedData->numVertices,
									pInstancedData->startIndex,
									pInstancedData->numPrimitives);

		dGET_WORLD_PARAM()->WorldPolyCnt += pInstancedData->numPrimitives;
	}
}

void _RpNtlWorldSectorWireSolid(RxD3D9InstanceData *pInstancedData, RxD3D9ResEntryHeader *pResEntryHeader)
{
	if(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_WIRE_SOLID))
	{
		BeginTerrainWireFrameState();
		{
			RwMatrix matWireWorld;
			RwV3d Pos;
			Pos.x = 0.0f;
			Pos.y = 0.3f;
			Pos.z = 0.0f;

			RwMatrixTranslate(&matWireWorld, &Pos, rwCOMBINEREPLACE);

			RwD3D9SetTransformWorld(&matWireWorld);

			RwD3D9SetStreamSource(0, (IDirect3DVertexBuffer9*)pResEntryHeader->vertexStream->vertexBuffer, 0, pResEntryHeader->vertexStream->stride);
			RwD3D9SetIndices((IDirect3DIndexBuffer9*)pResEntryHeader->indexBuffer);
			RwD3D9DrawIndexedPrimitive(	(D3DPRIMITIVETYPE)pResEntryHeader->primType,
				pInstancedData->baseIndex,
				0,
				pInstancedData->numVertices,
				pInstancedData->startIndex,
				pInstancedData->numPrimitives);

			RwD3D9SetTransformWorld(NULL);
		}
		EndTerrainWireFrameState();					
	}
}

#ifdef dNTL_WORLD_COMBINE
void _RpNtlWorldD3D9RenderCallback02(RwResEntry *repEntry, void *object, RwUInt8 type, RwUInt32 flags)
{
	NTL_SPROFILE("_RpNtlWorldD3D9RenderCallback")

	RxD3D9ResEntryHeader *pResEntryHeader;
	RxD3D9InstanceData *pInstancedData;
	RwInt32 numMeshes;

	// get d3d devices
	LPDIRECT3DDEVICE9 pD3DDev = static_cast<LPDIRECT3DDEVICE9>(RwD3D9GetCurrentD3DDevice());

	// Set the Default Pixel shader
	RwD3D9SetPixelShader(NULL);

	// Set clipping
	_rwD3D9EnableClippingIfNeeded(object, type);

	// Get header
	pResEntryHeader = (RxD3D9ResEntryHeader *)(repEntry + 1);

	// Vertex Declaration
	RwD3D9SetVertexDeclaration(pResEntryHeader->vertexDeclaration);

	/* Get the instanced data */
	pInstancedData = (RxD3D9InstanceData*)(pResEntryHeader + 1);

	/* Get the number of meshes */
	numMeshes = pResEntryHeader->numMeshes;
	if (numMeshes)
	{
		do
		{
			RpMaterial *pMaterial;
			RwRGBA *pMatcolor;

			pMaterial = pInstancedData->material;
			pMatcolor = &(pMaterial->color);

			// Vertex shader
			RwD3D9SetVertexShader(pInstancedData->vertexShader);

			sRpNtlWorldSectorSplat	*pNtlWorldSectorSplat	= RpNtlWorldGetSectorSplat(pInstancedData->material);
			CNtlWorldSector			*pNtlWorldSector		= pNtlWorldSectorSplat->pNtlWorldSector;

			if(_RpNtlWorldSectorCullCheck(pNtlWorldSector))
			{
				continue;
			}

			RwBool bRenderTex = !_RpNtlWorldSectorIsFullFogged(pNtlWorldSector);
			if(bRenderTex && pNtlWorldSector->m_pNtlWorldShadow->m_pTexShadow)
			{

				//#ifdef dNTL_WORLD_TOOL_MODE
				if(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_PROJECTION_SHADOW))
					//#endif
				{
					BeginTerrrainShadowState();

					RwD3D9SetTransform(D3DTS_TEXTURE2, &pNtlWorldSector->m_pNtlWorldShadow->m_matTex);
					RwD3D9SetTexture(pNtlWorldSector->m_pNtlWorldShadow->m_pTexShadow, 2);
				}
			}

			BegTerrainLayerCommonState();
			{
				BegTerrainBaseLayerState(pNtlWorldSector->m_IsTileTransparency);
				{
					sRpNtlWorldSectorSplat	*pNtlWorldSectorSplat	= RpNtlWorldGetSectorSplat(pInstancedData->material);
					CNtlWorldSector			*pNtlWorldSector		= pNtlWorldSectorSplat->pNtlWorldSector;

					RwD3D9SetTexture(NULL, 0);

					if(bRenderTex)
					{
						RwD3D9SetTexture(pNtlWorldSector->m_pTextureCombine, 1);
					}
					else
					{
						RwD3D9SetTexture(NULL, 1);		
					}

					RwD3D9SetTransform(D3DTS_TEXTURE0, &pNtlWorldSector->m_TextureMatBase[0]);
					RwD3D9SetTransform(D3DTS_TEXTURE1, &pNtlWorldSector->m_TextureMatCombine);

					_RpNtlWorldSectorDrawPrimitive(pInstancedData, pResEntryHeader);
				}
				EndTerrainBaseLayerState(pNtlWorldSector->m_IsTileTransparency);
#ifdef dNTL_WORLD_TOOL_MODE
				_RpNtlWorldSectorWireSolid(pInstancedData, pResEntryHeader);
#endif
			}
			EndTerrainLayerCommonState();

			if(bRenderTex && pNtlWorldSector->m_pNtlWorldShadow->m_pTexShadow)
			{
				//#ifdef dNTL_WORLD_TOOL_MODE
				if(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_PROJECTION_SHADOW))
					//#endif
				{
					EndTerrrainShadowState();
				}
			}
		} while(++pInstancedData, --numMeshes); // Move onto the next pInstancedData
	}

	NTL_EPROFILE()
}

static void
CombineRenderCallback(RwResEntry *repEntry, void *object, RwUInt8 type, RwUInt32 flags)
{
	RxD3D9ResEntryHeader *pResEntryHeader;
	RxD3D9InstanceData *pInstancedData;
	RwInt32 numMeshes;

	// get d3d devices
	LPDIRECT3DDEVICE9 pD3DDev = static_cast<LPDIRECT3DDEVICE9>(RwD3D9GetCurrentD3DDevice());

	// Set the Default Pixel shader
	RwD3D9SetPixelShader(NULL);

	// Set clipping
	_rwD3D9EnableClippingIfNeeded(object, type);

	// Get header
	pResEntryHeader = (RxD3D9ResEntryHeader *)(repEntry + 1);

	// Vertex Declaration
	RwD3D9SetVertexDeclaration(pResEntryHeader->vertexDeclaration);

	/* Get the instanced data */
	pInstancedData = (RxD3D9InstanceData*)(pResEntryHeader + 1);

	/* Get the number of meshes */
	numMeshes = pResEntryHeader->numMeshes;
	if (numMeshes)
	{
		do
		{
			RpMaterial *pMaterial;
			RwRGBA *pMatcolor;

			pMaterial = pInstancedData->material;
			pMatcolor = &(pMaterial->color);

			// Vertex shader
			RwD3D9SetVertexShader(pInstancedData->vertexShader);

			sRpNtlWorldSectorSplat	*pNtlWorldSectorSplat	= RpNtlWorldGetSectorSplat(pInstancedData->material);
			CNtlWorldSector			*pNtlWorldSector		= pNtlWorldSectorSplat->pNtlWorldSector;

			RwD3D9SetRenderState(D3DRS_LIGHTING, FALSE);

			if (pNtlWorldSector->m_IsTileTransparency)
			{
				RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)TRUE);
			}

			RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
			RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);

			RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
			RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); 
			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

			if (pNtlWorldSector->m_IsTileTransparency)
			{
				RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
				RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
				RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			}
			else
			{
				RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
				RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
			}


			RwD3D9SetTexture(pNtlWorldSector->m_pTextureBase, 0);

			RwD3D9SetTransform(D3DTS_TEXTURE0, &pNtlWorldSector->m_TextureMatBase[1]);

			_RpNtlWorldSectorDrawPrimitive(pInstancedData, pResEntryHeader);

			RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)TRUE); 
			RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
			RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);

			RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_CURRENT);
			RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
			RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); 
			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

			RwD3D9SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			RwD3D9SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
			RwD3D9SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

			if (pNtlWorldSector->m_IsTileTransparency)
			{
				RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
				RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
				RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			}
			else
			{
				RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
				RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
				RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
			}

			for(RwInt32 i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
			{
				if(pNtlWorldSector->m_pTextureSplat[i] && pNtlWorldSector->m_pTextureAlpha[i])
				{
					RwD3D9SetTexture(pNtlWorldSector->m_pTextureAlpha[i], 0);
					RwD3D9SetTexture(pNtlWorldSector->m_pTextureSplat[i], 1);
					RwD3D9SetTransform(D3DTS_TEXTURE0, &pNtlWorldSector->m_TextureMat[i][0]);
					RwD3D9SetTransform(D3DTS_TEXTURE1, &pNtlWorldSector->m_TextureMat[i][1]);

					_RpNtlWorldSectorDrawPrimitive(pInstancedData, pResEntryHeader);
				}
			}



			RwD3D9SetRenderState(D3DRS_LIGHTING, TRUE);
			RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);

			RwD3D9SetTexture(NULL, 0);
			RwD3D9SetTexture(NULL, 1);

			D3DXMATRIXA16 mIdentity;
			D3DXMatrixIdentity(&mIdentity);
			RwD3D9SetTransform(D3DTS_TEXTURE0, &mIdentity);
			RwD3D9SetTransform(D3DTS_TEXTURE1, &mIdentity);

			RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
			RwD3D9SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
			RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
			RwD3D9SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);

			RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)FALSE); 
			RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)rwBLENDSRCALPHA);
			RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)rwBLENDINVSRCALPHA);

			RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE); 
			RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT); 
			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

			RwD3D9SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
			RwD3D9SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			RwD3D9SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
			RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
			RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
		} while(++pInstancedData, --numMeshes); // Move onto the next pInstancedData
	}
}


RwTexture* NtlTerrainCombineTextureGenerator(RwInt32 iSectorIdx)
{
	RwCamera* pOldCamera = RwCameraGetCurrentCamera();
	if (pOldCamera)
	{
		RwCameraEndUpdate(pOldCamera);
	}
	
	CNtlWorldSector*	pSector		= &GetSceneManager()->GetWorld()->GetWorldFieldMgr()->m_pSectors[iSectorIdx];
	RwV3d				vSectorMid;	vSectorMid.x = pSector->DatumPoint.x; vSectorMid.y = pSector->DatumPoint.y; vSectorMid.z = pSector->DatumPoint.z;

	RwCamera*			pCamera		= RwCameraCreate();
	RwFrame*			pFrame		= RwFrameCreate();

	RwRaster*			pRaster		= RwRasterCreate(512, 512, 0, rwRASTERFORMAT8888 | rwRASTERTYPECAMERATEXTURE);
	RwRaster*			pRasterZ	= RwRasterCreate(512, 512, 0, rwRASTERTYPEZBUFFER);
	RwTexture*			pTexture	= RwTextureCreate(NULL);

	RwCameraSetFrame(pCamera, pFrame);
	RwCameraSetRaster(pCamera, pRaster);
	RwCameraSetZRaster(pCamera, pRasterZ);
	RwTextureSetRaster(pTexture, pRaster);

	RwCameraSetProjection(pCamera, rwPARALLEL);
	RwCameraSetNearClipPlane(pCamera, 0.001f);
	RwCameraSetFarClipPlane(pCamera, 2048.0f);

	RwV2d vw; vw.x = vw.y = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize / 2);
	RwCameraSetViewWindow(pCamera, &vw);

	RwMatrix*	pMatrix	= RwFrameGetMatrix(pFrame);

	CNtlMath::MathRwV3dAssign(RwMatrixGetAt(pMatrix), 0.0f, -1.0f, 0.0f);
	CNtlMath::MathRwV3dAssign(RwMatrixGetRight(pMatrix), 1.0f, 0.0f, 0.0f);
	CNtlMath::MathRwV3dAssign(RwMatrixGetUp(pMatrix), 0.0f, 0.0f, 1.0f);

	*RwMatrixGetPos(pMatrix)	= vSectorMid;	
	RwV3dIncrementScaled(RwMatrixGetPos(pMatrix), RwMatrixGetAt(pMatrix), -0.5f * RwCameraGetFarClipPlane(pCamera));

	RwMatrixUpdate(pMatrix);
	RwFrameUpdateObjects(pFrame);

	static RwRGBA rgba = { 0, 0, 0, 0 };
	if (RwCameraClear(pCamera, &rgba, rwCAMERACLEARZ | rwCAMERACLEARIMAGE))
	{
		if(RwCameraBeginUpdate(pCamera))
		{
			RxD3D9AllInOneSetLightingCallBack(g_pNodeWorld, NULL);
			RxD3D9AllInOneSetRenderCallBack(g_pNodeWorld, CombineRenderCallback);
			RpAtomicRender(pSector->m_pAtomic);

			RwCameraEndUpdate(RwCameraGetCurrentCamera());
		}
	}

	RwRaster* pNewRaster	= RwRasterCreate( RwRasterGetWidth(pRaster), RwRasterGetHeight(pRaster), RwRasterGetDepth(pRaster), rwRASTERTYPETEXTURE | rwRASTERFORMAT8888 );
	RwTexture* pNewTexture	= RwTextureCreate(pNewRaster);
	RwTextureSetFilterMode(pNewTexture, RwTextureGetFilterMode(pTexture));
	RwTextureSetAddressingU(pNewTexture, RwTextureGetAddressingU(pTexture));
	RwTextureSetAddressingV(pNewTexture, RwTextureGetAddressingV(pTexture));

	CNtlPLResourceManager::GetInstance()->CopyRaster(pTexture, pNewTexture);

	RwCameraSetFrame(pCamera, NULL);
	RwCameraSetRaster(pCamera, NULL);
	RwCameraSetZRaster(pCamera, NULL);
	RwTextureSetRaster(pTexture, NULL);

	RwRasterDestroy(pRasterZ);
	RwRasterDestroy(pRaster);
	RwFrameDestroy(pFrame);
	RwCameraDestroy(pCamera);
	RwTextureDestroy(pTexture);

	if (pOldCamera)
	{
		RwCameraBeginUpdate(pOldCamera);
	}

	return pNewTexture;
}

#endif

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#else

RwBool	_RpNtlWorldSectorIsFullFogged(CNtlWorldSector *pNtlWorldSector)
{
#ifdef dNTL_WORLD_TOOL_MODE
	if (!CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_FOG_ACTIVATION))
	{
		return pNtlWorldSector->m_bFullFogged = FALSE;
	}
#endif 
	RwMatrix*	pmatCamera = RwFrameGetLTM(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera));
	RwPlane		plane;
	RwUInt32	uiTemp = 0;

	RwD3D9GetRenderState(D3DRS_FOGEND, (void*)&uiTemp);

	plane.normal	= pmatCamera->at;
	plane.distance	= RwV3dDotProduct(&(pmatCamera->at * CNtlMath::DWtoF(uiTemp) + pmatCamera->pos), &pmatCamera->at);

	if (RpNtlFarPlaneFrustumTestSphere(CNtlPLGlobal::m_RwCamera, &pNtlWorldSector->m_pAtomic->worldBoundingSphere, &plane))
	{
		return pNtlWorldSector->m_bFullFogged = FALSE;
	}
	return pNtlWorldSector->m_bFullFogged = TRUE;

	// 	CNtlPLVisualManager* pSceneManager = static_cast<CNtlPLVisualManager*>(GetSceneManager());
	// 
	// 	RwV3d SamplePt;
	// 	SamplePt.x = pNtlWorldSector->DatumPoint.x;
	// 	SamplePt.z = pNtlWorldSector->DatumPoint.z;
	// 	SamplePt.y = pSceneManager->GetWorld()->GetWorldSectorHeight(SamplePt);
	// 
	// 	RwV3d	CurPos = *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
	// 	RwReal	CurDis = RwV3dLength(&(CurPos - SamplePt));
	// 
	// 	RwUInt32 i = 0;
	// 	RwD3D9GetRenderState(D3DRS_FOGEND, (void*)&i);
	// 	RwReal FloatDist = CNtlMath::DWtoF(i);
	// 
	// 	if(CurDis < FloatDist)
	// 	{
	// 		return pNtlWorldSector->m_bFullFogged = FALSE;
	// 	}
	// 
	// 	SamplePt.x = pNtlWorldSector->DatumPoint.x - dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
	// 	SamplePt.z = pNtlWorldSector->DatumPoint.z - dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
	// 	SamplePt.y = pSceneManager->GetWorld()->GetWorldSectorHeight(SamplePt);
	// 	CurDis = RwV3dLength(&(CurPos - SamplePt));
	// 	if(CurDis < FloatDist)
	// 	{
	// 		return pNtlWorldSector->m_bFullFogged = FALSE;
	// 	}
	// 
	// 	SamplePt.x = pNtlWorldSector->DatumPoint.x + dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
	// 	SamplePt.z = pNtlWorldSector->DatumPoint.z - dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
	// 	SamplePt.y = pSceneManager->GetWorld()->GetWorldSectorHeight(SamplePt);
	// 	CurDis = RwV3dLength(&(CurPos - SamplePt));
	// 	if(CurDis < FloatDist)
	// 	{
	// 		return pNtlWorldSector->m_bFullFogged = FALSE;
	// 	}
	// 
	// 	SamplePt.x = pNtlWorldSector->DatumPoint.x + dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
	// 	SamplePt.z = pNtlWorldSector->DatumPoint.z + dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
	// 	SamplePt.y = pSceneManager->GetWorld()->GetWorldSectorHeight(SamplePt);
	// 	CurDis = RwV3dLength(&(CurPos - SamplePt));
	// 	if(CurDis < FloatDist)
	// 	{
	// 		return pNtlWorldSector->m_bFullFogged = FALSE;
	// 	}
	// 
	// 	SamplePt.x = pNtlWorldSector->DatumPoint.x - dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
	// 	SamplePt.z = pNtlWorldSector->DatumPoint.z + dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
	// 	SamplePt.y = pSceneManager->GetWorld()->GetWorldSectorHeight(SamplePt);
	// 	CurDis = RwV3dLength(&(CurPos - SamplePt));
	// 	if(CurDis < FloatDist)
	// 	{
	// 		return pNtlWorldSector->m_bFullFogged = FALSE;
	// 	}
	// 
	// 	return pNtlWorldSector->m_bFullFogged = TRUE;
}

RwBool _RpNtlWorldSectorCullCheck(CNtlWorldSector *pNtlWorldSector)
{
	// not render
	if(CNtlPLGlobal::m_UseTerrainPVSMode && !pNtlWorldSector->m_pNtlWorldSectorPVS->GetVisibility())
	{
		return pNtlWorldSector->m_bCullCheck = TRUE;
	}

	if(pNtlWorldSector->m_IsCulled)
	{
		return pNtlWorldSector->m_bCullCheck = TRUE;
	}

#ifdef dNTL_WORLD_TOOL_MODE
	if (CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_MINI_INDOOR_CULL))
	{
#endif
		if (CNtlPLWorldState::GetActiveMiniIndoor() && CNtlPLWorldState::GetActiveMiniIndoor()->GetObjectType() == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE)
		{
			return pNtlWorldSector->m_bCullCheck = TRUE;
		}
#ifdef dNTL_WORLD_TOOL_MODE
	}
#endif

	if (RwCameraFrustumTestSphere((RwCamera*)RWSRCGLOBAL(curCamera), RpAtomicGetWorldBoundingSphere(pNtlWorldSector->m_pAtomic)) == rwSPHEREOUTSIDE)
	{
		return pNtlWorldSector->m_bCullCheck = TRUE;
	}

	if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD && pNtlWorldSector->OccluderQuery(COccluderProxy::EOCCLUDER_SPHERE, EPLOCCLUDER_FUNC_TERRAIN, &pNtlWorldSector->m_pAtomic->worldBoundingSphere, 0))
	{
		return pNtlWorldSector->m_bCullCheck = TRUE;
	}

	return pNtlWorldSector->m_bCullCheck = FALSE;
}

void _RpNtlWorldSectorBLayerRender(RxD3D9InstanceData *pInstancedData, RxD3D9ResEntryHeader *pResEntryHeader, RwBool bRenderTex)
{
	sRpNtlWorldSectorSplat	*pNtlWorldSectorSplat	= RpNtlWorldGetSectorSplat(pInstancedData->material);
	CNtlWorldSector			*pNtlWorldSector		= pNtlWorldSectorSplat->pNtlWorldSector;

	RwD3D9SetTexture(NULL, 0);

	if(bRenderTex)
	{
		RwD3D9SetTexture(pNtlWorldSector->m_pTextureBase, 1);
	}
	else
	{
		RwD3D9SetTexture(NULL, 1);		
	}

	RwD3D9SetTransform(D3DTS_TEXTURE0, &pNtlWorldSector->m_TextureMatBase[0]);
	RwD3D9SetTransform(D3DTS_TEXTURE1, &pNtlWorldSector->m_TextureMatBase[1]);

	_RpNtlWorldSectorDrawPrimitive(pInstancedData, pResEntryHeader);
}

void _RpNtlWorldSectorDLayerRender(RxD3D9InstanceData *pInstancedData, RxD3D9ResEntryHeader *pResEntryHeader)
{
	sRpNtlWorldSectorSplat	*pNtlWorldSectorSplat	= RpNtlWorldGetSectorSplat(pInstancedData->material);
	CNtlWorldSector			*pNtlWorldSector		= pNtlWorldSectorSplat->pNtlWorldSector;

	for(RwInt32 i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
	{
		if(pNtlWorldSector->m_pTextureSplat[i] && pNtlWorldSector->m_pTextureAlpha[i])
		{
			RwD3D9SetTexture(pNtlWorldSector->m_pTextureAlpha[i], 0);
			RwD3D9SetTexture(pNtlWorldSector->m_pTextureSplat[i], 1);
			RwD3D9SetTransform(D3DTS_TEXTURE0, &pNtlWorldSector->m_TextureMat[i][0]);
			RwD3D9SetTransform(D3DTS_TEXTURE1, &pNtlWorldSector->m_TextureMat[i][1]);

			_RpNtlWorldSectorDrawPrimitive(pInstancedData, pResEntryHeader);
		}
	}
}

void _rpNtlWorldWaterD3D9RenderCallback(RwResEntry *repEntry, void *object, RwUInt8 type, RwUInt32 flags)
{
	NTL_SPROFILE("_rpNtlWorldWaterD3D9RenderCallback")

		RxD3D9ResEntryHeader *pResEntryHeader;
	RxD3D9InstanceData *pInstancedData;
	RwInt32 numMeshes;

	// get d3d devices
	LPDIRECT3DDEVICE9 pD3DDev = static_cast<LPDIRECT3DDEVICE9>(RwD3D9GetCurrentD3DDevice());

	// Set the Default Pixel shader
	RwD3D9SetPixelShader(NULL);

	// Set clipping
	_rwD3D9EnableClippingIfNeeded(object, type);

	// Get header
	pResEntryHeader = (RxD3D9ResEntryHeader *)(repEntry + 1);

	// Vertex Declaration
	RwD3D9SetVertexDeclaration(pResEntryHeader->vertexDeclaration);

	//* Get the instanced data */
	pInstancedData = (RxD3D9InstanceData*)(pResEntryHeader + 1);

	/* Get the number of meshes */
	numMeshes = pResEntryHeader->numMeshes;
	if (numMeshes)
	{
		do
		{
			RpMaterial*	pMaterial	= pInstancedData->material;
			RwRGBA*		pMatcolor	= &(pMaterial->color);

			RwD3D9SetVertexShader(pInstancedData->vertexShader); 

			sRpNtlWorldSectorSplat*	pNtlWorldSectorSplat	= RpNtlWorldGetSectorSplat(pInstancedData->material);
			CNtlWorldSector*		pNtlWorldSector			= pNtlWorldSectorSplat->pNtlWorldSector;
			CNtlPLVisualManager*	pSceneManager			= static_cast<CNtlPLVisualManager*>(GetSceneManager());

			// 			if(_RpNtlWorldSectorCullCheck(pNtlWorldSector))
			// 			{
			// 				pInstancedData++;
			// 				continue;
			// 			}
			pSceneManager->GetWorld()->RenderWater(pNtlWorldSector, pInstancedData, pResEntryHeader);
		} while (++pInstancedData, --numMeshes);
	}

	NTL_EPROFILE()
}

void _RpNtlWorldD3D9RenderCallback(RwResEntry *repEntry, void *object, RwUInt8 type, RwUInt32 flags)
{
	NTL_SPROFILE("_RpNtlWorldD3D9RenderCallback")

		RxD3D9ResEntryHeader *pResEntryHeader;
	RxD3D9InstanceData *pInstancedData;
	RwInt32 numMeshes;

	// get d3d devices
	LPDIRECT3DDEVICE9 pD3DDev = static_cast<LPDIRECT3DDEVICE9>(RwD3D9GetCurrentD3DDevice());

	// Set the Default Pixel shader
	RwD3D9SetPixelShader(NULL);

	// Set clipping
	_rwD3D9EnableClippingIfNeeded(object, type);

	// Get header
	pResEntryHeader = (RxD3D9ResEntryHeader *)(repEntry + 1);

	// Vertex Declaration
	RwD3D9SetVertexDeclaration(pResEntryHeader->vertexDeclaration);

	/* Get the instanced data */
	pInstancedData = (RxD3D9InstanceData*)(pResEntryHeader + 1);

	/* Get the number of meshes */
	numMeshes = pResEntryHeader->numMeshes;
	if (numMeshes)
	{
		do
		{
			RpMaterial *pMaterial;
			RwRGBA *pMatcolor;

			pMaterial = pInstancedData->material;
			pMatcolor = &(pMaterial->color);

			// Vertex shader
			RwD3D9SetVertexShader(pInstancedData->vertexShader);

			sRpNtlWorldSectorSplat	*pNtlWorldSectorSplat	= RpNtlWorldGetSectorSplat(pInstancedData->material);
			CNtlWorldSector			*pNtlWorldSector		= pNtlWorldSectorSplat->pNtlWorldSector;

			// 			if(_RpNtlWorldSectorCullCheck(pNtlWorldSector))
			// 			{
			// 				continue;
			// 			}

			RwBool bRenderTex = !_RpNtlWorldSectorIsFullFogged(pNtlWorldSector);
			if(bRenderTex && pNtlWorldSector->m_pNtlWorldShadow->m_pTexShadow)
			{

				//#ifdef dNTL_WORLD_TOOL_MODE
				if(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_PROJECTION_SHADOW))
					//#endif
				{
					BeginTerrrainShadowState();

					RwD3D9SetTransform(D3DTS_TEXTURE2, &pNtlWorldSector->m_pNtlWorldShadow->m_matTex);
					RwD3D9SetTexture(pNtlWorldSector->m_pNtlWorldShadow->m_pTexShadow, 2);
				}
			}

			BegTerrainLayerCommonState();
			{
				BegTerrainBaseLayerState(pNtlWorldSector->m_IsTileTransparency);
				{
					_RpNtlWorldSectorBLayerRender(pInstancedData, pResEntryHeader, bRenderTex);
				}
				EndTerrainBaseLayerState(pNtlWorldSector->m_IsTileTransparency);

				if (bRenderTex)
				{
					BegTerrainDetailLayerState(pNtlWorldSector->m_IsTileTransparency);
					{
						_RpNtlWorldSectorDLayerRender(pInstancedData, pResEntryHeader);
					}
					EndTerrainDetailLayerState(pNtlWorldSector->m_IsTileTransparency);
				}

#ifdef dNTL_WORLD_TOOL_MODE
				_RpNtlWorldSectorWireSolid(pInstancedData, pResEntryHeader);
#endif
			}
			EndTerrainLayerCommonState();

			if(bRenderTex && pNtlWorldSector->m_pNtlWorldShadow->m_pTexShadow)
			{
				//#ifdef dNTL_WORLD_TOOL_MODE
				if(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_PROJECTION_SHADOW))
					//#endif
				{
					EndTerrrainShadowState();
				}
			}
		} while(++pInstancedData, --numMeshes); // Move onto the next pInstancedData
	}

	NTL_EPROFILE()
}

/******************************************************************************************************************************
* _rpNtlWorldSectorAtomicPipeline : 
******************************************************************************************************************************/
static RxPipeline*	_rpNtlWorldSectorAtomicPipeline(void)
{
	RxPipeline *pipe = RxPipelineCreate();

	if(pipe)
	{
		RxLockedPipe	*lpipe;

		pipe->pluginId = rwID_NTL_WORLD;

		lpipe = RxPipelineLock(pipe);
		if(lpipe)
		{
			RxNodeDefinition *pInstanceNode;

			/* Get the instance pNode definition */
			pInstanceNode = RxNodeDefinitionGetD3D9AtomicAllInOne();

			/* Add the pNode to the pipeline	*/
			lpipe = RxLockedPipeAddFragment(lpipe, NULL, pInstanceNode, NULL);

			/* Unlock the pipeline */
			lpipe = RxLockedPipeUnlock(lpipe);

			return pipe;
		}

		RxPipelineDestroy(pipe);
	}

	return NULL;
}

void rpNtlWorldSectorPipeLineDelete(void)
{
	RxPipelineDestroy(D3D9NtlWorldSectorAtomicPipeline);
	D3D9NtlWorldSectorAtomicPipeline = NULL;

	RxPipelineDestroy(D3D9NtlWorldWaterSectorAtomicPipeline);
	D3D9NtlWorldWaterSectorAtomicPipeline = NULL;
}

RwBool RpNtlWorldSectorGeomSetRenderCB()
{
	// set Lighting callback - just turn it off for now to prevent hw lighting being set up
	RxD3D9AllInOneSetLightingCallBack(g_pNodeWorld, NULL);

	static RxD3D9AllInOneRenderCallBack rxD3D9AllInOneRenderCallBack = NULL;
	static RwBool						bCombine	= FALSE;

#ifdef dNTL_WORLD_COMBINE
	if (CNtlPLOptionManager::GetInstance()->GetTerrainTextrueCombine())
	{
		rxD3D9AllInOneRenderCallBack = _RpNtlWorldD3D9RenderCallback02;
	}
	else
	{
		rxD3D9AllInOneRenderCallBack = NULL;
	}
#endif

	if(!rxD3D9AllInOneRenderCallBack)
	{
		const D3DCAPS9* pD3DCaps = (const D3DCAPS9*)RwD3D9GetCaps();

		if(pD3DCaps->MaxSimultaneousTextures >= 8)
		{
			rxD3D9AllInOneRenderCallBack = _RpNtlWorldD3D9RenderCallback01;
		}	
		else
		{
			rxD3D9AllInOneRenderCallBack = _RpNtlWorldD3D9RenderCallback;
		}
	}

	RxD3D9AllInOneSetRenderCallBack(g_pNodeWorld, rxD3D9AllInOneRenderCallBack);

	return TRUE;
}

RwBool RpNtlWorldSectorWaterGeomSetRenderCB()
{
	// set Lighting callback - just turn it off for now to prevent hw lighting being set up
	RxD3D9AllInOneSetLightingCallBack(g_pNodeWorldWater, NULL);

	// node rendercallback 
	RxD3D9AllInOneSetRenderCallBack(g_pNodeWorldWater, _rpNtlWorldWaterD3D9RenderCallback);

	return TRUE;
}

RwBool rpNtlWorldSectorPipeLineCreate(void)
{
	NTL_FUNCTION("rpNtlWorldSectorPipeLineCreate");

	RxNodeDefinition*	pInstanceNode;


	/* create a new atomic pipeline */
	D3D9NtlWorldSectorAtomicPipeline		= _rpNtlWorldSectorAtomicPipeline();
	D3D9NtlWorldWaterSectorAtomicPipeline	= _rpNtlWorldSectorAtomicPipeline();

	if(!D3D9NtlWorldSectorAtomicPipeline || !D3D9NtlWorldWaterSectorAtomicPipeline)
		NTL_RETURN(FALSE);

	pInstanceNode = RxNodeDefinitionGetD3D9AtomicAllInOne();

	g_pNodeWorld		= RxPipelineFindNodeByName(D3D9NtlWorldSectorAtomicPipeline, pInstanceNode->name, NULL, NULL);
	g_pNodeWorldWater	= RxPipelineFindNodeByName(D3D9NtlWorldWaterSectorAtomicPipeline, pInstanceNode->name, NULL, NULL);

	NTL_RETURN(TRUE);
}

/******************************************************************************************************************************
* NtlWorldGlobalDataConstructor :
******************************************************************************************************************************/
static void *NtlWorldGlobalDataConstructor(void *unused, RwInt32 offset __RWUNUSED__, RwInt32 size __RWUNUSED__)
{
	NTL_FUNCTION("NtlWorldGlobalDataConstructor");

	dNTL_WORLD_GLOBAL(WorldSectorCBRender) = RpNtlWorldSectorRender;
	dNTL_WORLD_GLOBAL(pSectorFreelist) = RwFreeListCreate(sizeof(sNtlWorldSector), 4, 16, rwMEMHINTDUR_GLOBAL);
	//RwFreeListCreateAndPreallocateSpace(sizeof(sNtlWorldSector), dGET_WORLD_PARAM()->WorldSectorNum * dGET_WORLD_PARAM()->WorldSectorNum, 4, 1, NULL, rwMEMHINTDUR_GLOBAL);

	rpNtlWorldSectorPipeLineCreate();

	NTL_RETURN(unused);
}

/******************************************************************************************************************************
* NtlWorldGlobalDataDestructor :
******************************************************************************************************************************/
static void *NtlWorldGlobalDataDestructor(void *unused, RwInt32 offset __RWUNUSED__, RwInt32 size __RWUNUSED__)
{
	NTL_FUNCTION("NtlWorldGlobalDataDestructor");

	RwFreeListDestroy(dNTL_WORLD_GLOBAL(pSectorFreelist));

	rpNtlWorldSectorPipeLineDelete();

	NTL_RETURN(unused);
}

/******************************************************************************************************************************
* NtlWorldLocalDataConstructor :
******************************************************************************************************************************/
static void *NtlWorldLocalDataConstructor(void *pWorldSector, RwInt32 offset __RWUNUSED__, RwInt32 size __RWUNUSED__)
{
	if(NtlWorldLocalOffset > 0)
		dNTL_WORLD_LOCAL(pWorldSector, pNtlSector) = NULL;

	return pWorldSector;
}


/******************************************************************************************************************************
* NtlWorldLocalDataDestructor :
******************************************************************************************************************************/
static void *NtlWorldLocalDataDestructor(void *pWorldSector, RwInt32 offset __RWUNUSED__, RwInt32 size __RWUNUSED__)
{
	if(dNTL_WORLD_LOCAL(pWorldSector, pNtlSector))
		dNTL_WORLD_LOCAL(pWorldSector, pNtlSector) = NULL;

	return pWorldSector;
}


/******************************************************************************************************************************
* NtlWorldLocalDataCopier :
******************************************************************************************************************************/
static void *NtlWorldLocalDataCopier(void *pDstWorldSector, const void *pSrcWorldSector, RwInt32 offset __RWUNUSED__, RwInt32 size __RWUNUSED__)
{
	/* Note that copying a world in this way will cause problems on destruction */
	dNTL_WORLD_LOCAL(pDstWorldSector, pNtlSector) = dNTL_WORLD_LOCAL(pSrcWorldSector, pNtlSector);

	return pDstWorldSector;
}

/******************************************************************************************************************************
* NtlWorldSectorDataConstructor :
******************************************************************************************************************************/
static void *NtlWorldSectorLocalDataConstructor(void *pWorldSector, RwInt32 offset __RWUNUSED__, RwInt32 size __RWUNUSED__)
{
	if(NtlWorldSectorLocalOffset > 0)
	{
		dNTL_WORLD_SECTOR_LOCAL(pWorldSector, pParentFrame) = NULL;
	}

	return pWorldSector;
}


/******************************************************************************************************************************
* NtlWorldSectorDataDestructor :
******************************************************************************************************************************/
static void *NtlWorldSectorLocalDataDestructor(void *pWorldSector, RwInt32 offset __RWUNUSED__, RwInt32 size __RWUNUSED__)
{
	if(dNTL_WORLD_SECTOR_LOCAL(pWorldSector, pParentFrame))
	{
		/* Destroy the frame once all the atomics that use it are gone */
		RwFrameDestroy(dNTL_WORLD_SECTOR_LOCAL(pWorldSector, pParentFrame));
		dNTL_WORLD_SECTOR_LOCAL(pWorldSector, pParentFrame) = NULL;
	}

	return pWorldSector;
}


/******************************************************************************************************************************
* NtlWorldSectorDataCopier :
******************************************************************************************************************************/
static void *NtlWorldSectorLocalDataCopier(void *pDstWorldSector, const void *pSrcWorldSector, RwInt32 offset __RWUNUSED__, RwInt32 size __RWUNUSED__)
{
	/* Copying the frame from one world to another like this will cause problems on destruction */
	dNTL_WORLD_SECTOR_LOCAL(pDstWorldSector, pParentFrame) = dNTL_WORLD_SECTOR_LOCAL(pSrcWorldSector, pParentFrame);

	return pDstWorldSector;
}

/******************************************************************************************************************************
* RpNtlWorldAttach : attach ntl wolrd plugin 
******************************************************************************************************************************/
RwBool RpNtlWorldAttach(void)
{
	/* registers a plugin and reserves some space within the global engine space */
	NtlWorldGlobalOffset = RwEngineRegisterPlugin(sizeof(sNtlWorldGlobals), MAKECHUNKID(rwVENDORID_CRITERIONTK, dID_NTL_WORLD),
		NtlWorldGlobalDataConstructor,
		NtlWorldGlobalDataDestructor);

	if(NtlWorldGlobalOffset < 0)
		return FALSE;

	/* register a plugin and reserve some space within a world sector */
	NtlWorldLocalOffset = RpWorldSectorRegisterPlugin(sizeof(sNtlWorldLocals), MAKECHUNKID(rwVENDORID_CRITERIONTK, dID_NTL_WORLD),
		NtlWorldLocalDataConstructor,
		NtlWorldLocalDataDestructor,
		NtlWorldLocalDataCopier);

	if(NtlWorldLocalOffset < 0)
		return FALSE;

	/* register a plugin and reserve some space within a world */
	NtlWorldSectorLocalOffset = RpWorldRegisterPlugin(sizeof(sNtlSectorLocals), MAKECHUNKID(rwVENDORID_CRITERIONTK, dID_NTL_WORLD),
		NtlWorldSectorLocalDataConstructor,
		NtlWorldSectorLocalDataDestructor,
		NtlWorldSectorLocalDataCopier);

	if(NtlWorldSectorLocalOffset < 0)
		return FALSE;

	return TRUE;
}


#ifdef dNTL_WORLD_TOOL_MODE

RwBool RpNtlWorldSectorGeometryCreate(CNtlWorldSector *pNtlWorldSector)
{
	NTL_FUNCTION("RpNtlWorldSectorGeometryCreate");

	// 인도어
	switch (GetSceneManager()->GetActiveWorldType())
	{
	case AW_HEGITHFIELD:
		{
			NTL_ASSERTE(dNTL_WORLD_LOCAL(pNtlWorldSector->m_pWorldSector, pNtlSector) == NULL);

			dNTL_WORLD_LOCAL(pNtlWorldSector->m_pWorldSector, pNtlSector) = static_cast<sNtlWorldSector*>(RwFreeListAlloc(dNTL_WORLD_GLOBAL(pSectorFreelist), rwMEMHINTDUR_GLOBAL));
			sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(pNtlWorldSector->m_pWorldSector, pNtlSector);

			pNtlSector->pNtlWorldSector = pNtlWorldSector;
			pNtlSector->pNtlWorldSector->MaterialContructor();

			_RpNtlWorldSectorGeometryCreate(pNtlWorldSector);
		}
		break;
	case AW_RWWORLD:
		{
			pNtlWorldSector->MaterialContructor();
		}
		break;
	}

	pNtlWorldSector->m_bLoaded = TRUE;

	NTL_RETURN(TRUE);
}

#endif

RwBool RpNtlWorldSectorGeometryDelete(CNtlWorldSector *pNtlWorldSector)
{
	NTL_FUNCTION("RpNtlWorldSectorGeometryDelete");

	_RpNtlWorldSectorGeometryDelete(pNtlWorldSector);

	NTL_RETURN(TRUE);
}

RwBool RpNtlWorldSectorGeometryCreateFromFile(sCUR_FIELD_TEX_INFO& CurFieldTexInfo, CNtlWorldSector *pNtlWorldSector, FILE* pFile)
{
	NTL_FUNCTION("RpNtlWorldSectorGeometryCreateFromFile");

	// 인도어
	switch (GetSceneManager()->GetActiveWorldType())
	{
	case AW_HEGITHFIELD:
		{
			NTL_ASSERTE(dNTL_WORLD_LOCAL(pNtlWorldSector->m_pWorldSector, pNtlSector) == NULL);

			dNTL_WORLD_LOCAL(pNtlWorldSector->m_pWorldSector, pNtlSector) = static_cast<sNtlWorldSector*>(RwFreeListAlloc(dNTL_WORLD_GLOBAL(pSectorFreelist), rwMEMHINTDUR_GLOBAL));
			sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(pNtlWorldSector->m_pWorldSector, pNtlSector);

			pNtlSector->pNtlWorldSector = pNtlWorldSector;
			pNtlSector->pNtlWorldSector->MaterialContructor();

			_RpNtlWorldSectorGeometryLoadFromFile(CurFieldTexInfo, pNtlWorldSector, pFile);
		}
		break;
	case AW_RWWORLD:
		{
			pNtlWorldSector->MaterialContructor();

			_RpNtlWorldSectorGeometryLoadFromFileIndoor(pNtlWorldSector, pFile);
		}
		break;
	}

	pNtlWorldSector->m_bLoaded = TRUE;

	NTL_RETURN(TRUE);
}

/*
RwBool RpNtlWorldSectorGeometryCreateFromFile(sCUR_FIELD_TEX_INFO& CurFieldTexInfo, CNtlWorldSector *pNtlWorldSector, BYTE* pPosCurFileMem)
{
NTL_FUNCTION("RpNtlWorldSectorGeometryCreateFromFile");

dNTL_WORLD_LOCAL(pNtlWorldSector->m_pWorldSector, pNtlSector) = static_cast<sNtlWorldSector*>(RwFreeListAlloc(dNTL_WORLD_GLOBAL(pSectorFreelist), rwMEMHINTDUR_GLOBAL));
sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(pNtlWorldSector->m_pWorldSector, pNtlSector);

pNtlSector->pNtlWorldSector = pNtlWorldSector;
pNtlSector->pNtlWorldSector->MaterialContructor();

_RpNtlWorldSectorGeometryLoadFromFile(CurFieldTexInfo, pNtlWorldSector, pPosCurFileMem);

NTL_RETURN(TRUE);
}
*/

//인도어
// RwBool RpNtlWorldSectorGeometryDeleteIntoFile(RpWorldSector *pRpWorldSector, FILE* pFile)
// {
// 	NTL_FUNCTION("RpNtlWorldSectorGeometryDeleteIntoFile");
// 
// 	_RpNtlWorldSectorGeometrySaveIntoFile(pRpWorldSector, pFile);
// 
// 	NTL_RETURN(TRUE);
// }

RwBool RpNtlWorldSectorGeometryDeleteIntoFile(CNtlWorldSector *pNtlWorldSector, FILE* pFile)
{
	NTL_FUNCTION("RpNtlWorldSectorGeometryDeleteIntoFile");

	//인도어
	switch (GetSceneManager()->GetActiveWorldType())
	{
	case AW_HEGITHFIELD:
		{
			_RpNtlWorldSectorGeometrySaveIntoFile(pNtlWorldSector->m_pWorldSector, pFile);
		}
		break;
	case AW_RWWORLD:
		{
			_RpNtlWorldSectorGeometrySaveIntoFileIndoor(pNtlWorldSector, pFile);
		}
		break;
	}



	NTL_RETURN(TRUE);
}

RwBool RpNtlWorldSectorUpdateTexLyr(sCUR_FIELD_TEX_INFO& CurFieldTexInfo, CNtlWorldSector *pNtlWorldSector, RwInt32 IdxLyr, RwInt32 IdxMenu)
{
	return _RpNtlWorldSectorUpdateTexLyr(CurFieldTexInfo, pNtlWorldSector, IdxLyr, IdxMenu);
}

RwBool RpNtlWorldSectorUpdateTexMat(sCUR_FIELD_TEX_INFO& CurFieldTexInfo, CNtlWorldSector *pNtlWorldSector, RwInt32 IdxLyr)
{
	return _RpNtlWorldSectorUpdateTexMat(CurFieldTexInfo, pNtlWorldSector, IdxLyr);
}

/******************************************************************************************************************************
* RpNtlWorldSectorRender :
******************************************************************************************************************************/
RpWorldSector *RpNtlWorldSectorRender(RpWorldSector *pRpWorldSector)
{
	NTL_SPROFILE("RpNtlWorldSectorRender")

		sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(pRpWorldSector, pNtlSector);

	if(pNtlSector && pNtlSector->pNtlWorldSector)
	{
		RpAtomic *pAtomic = pNtlSector->pNtlWorldSector->m_pAtomic;

		if(pAtomic)
		{
			if (pAtomic->renderFrame != RWSRCGLOBAL(renderFrame))
			{
				if(!_RpNtlWorldSectorCullCheck(pNtlSector->pNtlWorldSector))
				{
					RpNtlWorldSectorGeomSetRenderCB();
					RpAtomicRender(pAtomic);
				}				
				pAtomic->renderFrame = RWSRCGLOBAL(renderFrame);
			}
		}
	}

	NTL_RPROFILE(pRpWorldSector)
}

// RpWorldSector *RpNtlWorldSectorWaterRender(RpWorldSector *pRpWorldSector)
// {
// 	NTL_SPROFILE("RpNtlWorldSectorWaterRender")
// 
// 	sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(pRpWorldSector, pNtlSector);
// 
// 	if(pNtlSector && pNtlSector->pNtlWorldSector)
// 	{
// 		if(pNtlSector->pNtlWorldSector->m_pWater)
// 		{
// 			RpAtomic *pAtomic = pNtlSector->pNtlWorldSector->m_pWater->_pAtom;
// 
// 			if(pAtomic)
// 			{
// 				RpNtlWorldSectorWaterGeomSetRenderCB();
// 				RpAtomicRender(pAtomic);
// 			}
// 		}
// 	}
// 
// 	NTL_RPROFILE(pRpWorldSector)
// }

CNtlWorldSector *RpNtlWorldSectorWaterRender(CNtlWorldSector *pNtlWorldSector)
{
	NTL_SPROFILE("RpNtlWorldSectorWaterRender")
		if (pNtlWorldSector && pNtlWorldSector->m_pWater)
		{
			RpAtomic* pAtomic = pNtlWorldSector->m_pWater->_pAtom;
			if(pAtomic)
			{
				if (pAtomic->renderFrame != RWSRCGLOBAL(renderFrame))
				{				
					RpNtlWorldSectorWaterGeomSetRenderCB();
					RpAtomicRender(pAtomic);

					pAtomic->renderFrame = RWSRCGLOBAL(renderFrame);
				}
			}
		}
		NTL_RPROFILE(pNtlWorldSector)
}

void _RpNtlWorldD3D9RenderCallback01(RwResEntry *repEntry, void *object, RwUInt8 type, RwUInt32 flags)
{
	RxD3D9ResEntryHeader *pResEntryHeader;
	RxD3D9InstanceData *pInstancedData;
	RwInt32 numMeshes;

	// get d3d devices
	LPDIRECT3DDEVICE9 pD3DDev = static_cast<LPDIRECT3DDEVICE9>(RwD3D9GetCurrentD3DDevice());

	// Set the Default Pixel shader
	RwD3D9SetPixelShader(NULL);

	// Set clipping
	_rwD3D9EnableClippingIfNeeded(object, type);

	// Get header
	pResEntryHeader = (RxD3D9ResEntryHeader *)(repEntry + 1);

	// Vertex Declaration
	RwD3D9SetVertexDeclaration(pResEntryHeader->vertexDeclaration);

	/* Get the instanced data */
	pInstancedData = (RxD3D9InstanceData*)(pResEntryHeader + 1);

	/* Get the number of meshes */
	numMeshes = pResEntryHeader->numMeshes;
	if (numMeshes)
	{
		do
		{
			RpMaterial *pMaterial;
			RwRGBA *pMatcolor;

			pMaterial = pInstancedData->material;
			pMatcolor = &(pMaterial->color);

			// Vertex shader
			RwD3D9SetVertexShader(pInstancedData->vertexShader);

			sRpNtlWorldSectorSplat	*pNtlWorldSectorSplat	= RpNtlWorldGetSectorSplat(pInstancedData->material);
			CNtlWorldSector			*pNtlWorldSector		= pNtlWorldSectorSplat->pNtlWorldSector;

			// 			if(_RpNtlWorldSectorCullCheck(pNtlWorldSector))
			// 			{
			// 				continue;
			// 			}

			// CodeInDeviceState : Setting
			BeginLightState(FALSE);
			RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, dGET_WORLD_PARAM()->ClrDayAndNight);

			if (_RpNtlWorldSectorIsFullFogged(pNtlWorldSector))
			{
				RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
				RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
				RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

				RwD3D9SetTexture(NULL, 0);

				if (pNtlWorldSector->m_IsTileTransparency)
				{
					RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)TRUE); 
					RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
					RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
				}
				else
				{
					RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
				}				

				_RpNtlWorldSectorDrawPrimitive(pInstancedData, pResEntryHeader);

				RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
				RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
				RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
				RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
				RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
				RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
				RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
				RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
				RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)FALSE); 
			}
			else
			{
				RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)TRUE); 
				RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
				RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);

				vector<RwInt32>	vecDetailLayer;
				for (int i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
				{
					if (pNtlWorldSector->m_pTextureAlpha[i] && pNtlWorldSector->m_pTextureSplat[i])
					{
						vecDetailLayer.push_back(i);
					}
				}

				RwInt32			iRenderStage;
				RwBool			bBaseLayer		= TRUE;
				RwInt32			iDetailLayer	= 0;
				do 
				{
					iRenderStage = 0;

					if (bBaseLayer)
					{
						RwD3D9SetTextureStageState(iRenderStage, D3DTSS_RESULTARG, D3DTA_CURRENT);
						RwD3D9SetTextureStageState(iRenderStage, D3DTSS_TEXCOORDINDEX, 0);
						RwD3D9SetTextureStageState(iRenderStage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);

						RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
						RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLORARG1, D3DTA_TEXTURE);
						RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
						RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
						//						RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

						RwD3D9SetTexture(pNtlWorldSector->m_pTextureBase, iRenderStage);
						RwD3D9SetTransform(D3DTS_TEXTURE0 + iRenderStage, &pNtlWorldSector->m_TextureMatBase[0]);

						++iRenderStage;

						for (int i = 0; i < 2; ++i)
						{
							if (iDetailLayer < (RwInt32)vecDetailLayer.size())
							{
								RwInt32 iDetailLayerIdx = vecDetailLayer.at(iDetailLayer);

								RwD3D9SetTextureStageState(iRenderStage, D3DTSS_RESULTARG, D3DTA_TEMP); 
								RwD3D9SetTextureStageState(iRenderStage, D3DTSS_TEXCOORDINDEX, 0);
								RwD3D9SetTextureStageState(iRenderStage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
								RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLOROP, D3DTOP_SELECTARG1); 
								RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLORARG1, D3DTA_TEXTURE); 
								RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1); 
								RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
								//								RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

								RwD3D9SetTexture(pNtlWorldSector->m_pTextureSplat[iDetailLayerIdx], iRenderStage);
								RwD3D9SetTransform(D3DTS_TEXTURE0 + iRenderStage, &pNtlWorldSector->m_TextureMat[iDetailLayerIdx][1]);

								++iRenderStage;

								RwD3D9SetTextureStageState(iRenderStage, D3DTSS_RESULTARG, D3DTA_CURRENT); 
								RwD3D9SetTextureStageState(iRenderStage, D3DTSS_TEXCOORDINDEX, 0);
								RwD3D9SetTextureStageState(iRenderStage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
								RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLOROP, D3DTOP_BLENDTEXTUREALPHA); 
								RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLORARG1, D3DTA_TEMP);
								RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLORARG2, D3DTA_CURRENT);
								RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAOP, D3DTOP_BLENDTEXTUREALPHA); 
								RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAARG1, D3DTA_TEMP);
								RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
								//								RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

								RwD3D9SetTexture(pNtlWorldSector->m_pTextureAlpha[iDetailLayerIdx], iRenderStage);
								RwD3D9SetTransform(D3DTS_TEXTURE0 + iRenderStage, &pNtlWorldSector->m_TextureMat[iDetailLayerIdx][0]);

								++iRenderStage;
								++iDetailLayer;
							}
						}

						bBaseLayer = FALSE;
					}
					else
					{
						if (iDetailLayer < (RwInt32)vecDetailLayer.size())
						{
							RwInt32 iDetailLayerIdx = vecDetailLayer.at(iDetailLayer);

							RwD3D9SetTextureStageState(iRenderStage, D3DTSS_RESULTARG, D3DTA_CURRENT); 
							RwD3D9SetTextureStageState(iRenderStage, D3DTSS_TEXCOORDINDEX, 0);
							RwD3D9SetTextureStageState(iRenderStage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
							RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLOROP, D3DTOP_SELECTARG1); 
							RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLORARG1, D3DTA_TEXTURE); 
							RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1); 
							RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

							RwD3D9SetTexture(pNtlWorldSector->m_pTextureSplat[iDetailLayerIdx], iRenderStage);
							RwD3D9SetTransform(D3DTS_TEXTURE0 + iRenderStage, &pNtlWorldSector->m_TextureMat[iDetailLayerIdx][1]);

							++iRenderStage;

							RwD3D9SetTextureStageState(iRenderStage, D3DTSS_RESULTARG, D3DTA_CURRENT); 
							RwD3D9SetTextureStageState(iRenderStage, D3DTSS_TEXCOORDINDEX, 0);
							RwD3D9SetTextureStageState(iRenderStage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
							RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLOROP, D3DTOP_SELECTARG1); 
							RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLORARG1, D3DTA_CURRENT);
							RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1); 
							RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); 

							RwD3D9SetTexture(pNtlWorldSector->m_pTextureAlpha[iDetailLayerIdx], iRenderStage);
							RwD3D9SetTransform(D3DTS_TEXTURE0 + iRenderStage, &pNtlWorldSector->m_TextureMat[iDetailLayerIdx][0]);

							++iRenderStage;
							++iDetailLayer;
						}
					}

					if (pNtlWorldSector->m_pNtlWorldShadow->m_pTexShadow)
					{
						//#ifdef dNTL_WORLD_TOOL_MODE
						if(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_PROJECTION_SHADOW))
							//#endif
						{
							RwD3D9SetTextureStageState(iRenderStage, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION);
							RwD3D9SetTextureStageState(iRenderStage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
							RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLOROP, D3DTOP_MODULATE);
							RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLORARG1, D3DTA_TEXTURE);
							RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLORARG2, D3DTA_CURRENT);						
							RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
							RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAARG1, D3DTA_CURRENT);

							RwD3D9SetTransform(D3DTS_TEXTURE0 + iRenderStage, &pNtlWorldSector->m_pNtlWorldShadow->m_matTex);
							RwD3D9SetTexture(pNtlWorldSector->m_pNtlWorldShadow->m_pTexShadow, iRenderStage);

							++iRenderStage;
						}
					}

					RwD3D9SetTextureStageState(iRenderStage, D3DTSS_RESULTARG, D3DTA_CURRENT);
					RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLOROP, D3DTOP_MODULATE);
					RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
					RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLORARG2, D3DTA_CURRENT);						
					RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
					RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
					RwD3D9SetTexture(NULL, iRenderStage);

					++iRenderStage;

					RwD3D9SetTextureStageState(iRenderStage, D3DTSS_RESULTARG, D3DTA_CURRENT);
					RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
					RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLORARG1, D3DTA_CURRENT);
					RwD3D9SetTextureStageState(iRenderStage, D3DTSS_COLORARG2, D3DTA_TFACTOR);	
					if (pNtlWorldSector->m_IsTileTransparency)
					{
						RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
						RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
						RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
					}
					else
					{
						RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
						RwD3D9SetTextureStageState(iRenderStage, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
					}

					RwD3D9SetTexture(NULL, iRenderStage);

					++iRenderStage;

					_RpNtlWorldSectorDrawPrimitive(pInstancedData, pResEntryHeader);

					// CodeInTextrueStageState : Restore
					D3DXMATRIXA16 mIdentity;
					D3DXMatrixIdentity(&mIdentity);

					RwD3D9SetTexture(NULL, 0);					
					RwD3D9SetTransform(D3DTS_TEXTURE0, &mIdentity);
					RwD3D9SetTextureStageState(0, D3DTSS_RESULTARG, D3DTA_CURRENT); 
					RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
					RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
					RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
					RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
					RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
					RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
					RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
					RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

					for (int i = 1; i < iRenderStage; ++i)
					{
						RwD3D9SetTexture(NULL, i);
						RwD3D9SetTransform(D3DTS_TEXTURE0 + i, &mIdentity);
						RwD3D9SetTextureStageState(i, D3DTSS_RESULTARG, D3DTA_CURRENT);
						RwD3D9SetTextureStageState(i, D3DTSS_TEXCOORDINDEX, i);
						RwD3D9SetTextureStageState(i, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
						RwD3D9SetTextureStageState(i, D3DTSS_COLOROP, D3DTOP_DISABLE);
						RwD3D9SetTextureStageState(i, D3DTSS_COLORARG1, D3DTA_TEXTURE);
						RwD3D9SetTextureStageState(i, D3DTSS_COLORARG2, D3DTA_CURRENT);
						RwD3D9SetTextureStageState(i, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
						RwD3D9SetTextureStageState(i, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
						RwD3D9SetTextureStageState(i, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
					}
				} while(iDetailLayer < (RwInt32)vecDetailLayer.size());

				RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)FALSE); 
				RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)rwBLENDSRCALPHA);
				RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)rwBLENDINVSRCALPHA);
			}

#ifdef dNTL_WORLD_TOOL_MODE
			_RpNtlWorldSectorWireSolid(pInstancedData, pResEntryHeader);
#endif

			RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);
			EndLightState();

			/*
			toon test
			{
			RwD3D9SetRenderState(D3DRS_LIGHTING, FALSE);
			RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLFRONT);

			RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, 0xff000000);
			RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2); 
			RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_CURRENT);
			RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CURRENT);

			RwMatrix matWireWorld;
			RwV3d Pos;
			Pos.x = 1.0001f;
			Pos.y = 1.0001f;
			Pos.z = 1.0001f;

			RwMatrixScale(&matWireWorld, &Pos, rwCOMBINEREPLACE);
			RwD3D9SetTransformWorld(&matWireWorld);

			sRpNtlWorldSectorSplat*	pNtlWorldSectorSplat	= RpNtlWorldGetSectorSplat(pInstancedData->material);
			CNtlWorldSector*		pNtlWorldSector			= pNtlWorldSectorSplat->pNtlWorldSector;
			RwD3D9SetStreamSource(0, (IDirect3DVertexBuffer9*)pResEntryHeader->vertexStream->vertexBuffer, 0, pResEntryHeader->vertexStream->stride);
			RwD3D9SetIndices((IDirect3DIndexBuffer9*)pResEntryHeader->indexBuffer);
			RwD3D9DrawIndexedPrimitive((D3DPRIMITIVETYPE)pResEntryHeader->primType,
			pInstancedData->baseIndex,
			0,
			pInstancedData->numVertices,
			pInstancedData->startIndex,
			pInstancedData->numPrimitives);

			RwD3D9SetTransformWorld(NULL);

			RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);
			RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE); 
			RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE); 
			RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1); 
			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

			RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLBACK);
			RwD3D9SetRenderState(D3DRS_LIGHTING, TRUE);
			}
			*/

		} while(++pInstancedData, --numMeshes); // Move onto the next pInstancedData
	}
}

void _RpNtlWorldSectorDrawPrimitive(RxD3D9InstanceData *pInstancedData, RxD3D9ResEntryHeader *pResEntryHeader)
{
	RwInt32					iPrimitiveNum			= 0;
	sRpNtlWorldSectorSplat*	pNtlWorldSectorSplat	= RpNtlWorldGetSectorSplat(pInstancedData->material);
	CNtlWorldSector*		pNtlWorldSector			= pNtlWorldSectorSplat->pNtlWorldSector;
	if(dGET_WORLD_PARAM()->LODEnable)
	{
		RwD3D9SetStreamSource(0, (IDirect3DVertexBuffer9*)pResEntryHeader->vertexStream->vertexBuffer, pResEntryHeader->vertexStream->offset, pResEntryHeader->vertexStream->stride);

		if(pNtlWorldSector->GetBodyIndexBuff(dGET_SECTOR_LOD()))
		{
			iPrimitiveNum = pNtlWorldSector->GetBodyPrimitiveNum(dGET_SECTOR_LOD());
			RwD3D9SetIndices(pNtlWorldSector->GetBodyIndexBuff(dGET_SECTOR_LOD()));
			RwD3D9DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum, 0, iPrimitiveNum);

			dGET_WORLD_PARAM()->WorldPolyCnt += iPrimitiveNum;
		}

		for(int i = 0; i < 5; ++i)
		{
			if(pNtlWorldSector->GetLinkIndexBuff(dGET_SECTOR_LOD(), i))
			{
				iPrimitiveNum = pNtlWorldSector->GetLinkPrimitiveNum(dGET_SECTOR_LOD(), i);
				RwD3D9SetIndices(pNtlWorldSector->GetLinkIndexBuff(dGET_SECTOR_LOD(), i));
				RwD3D9DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum, 0, iPrimitiveNum);

				dGET_WORLD_PARAM()->WorldPolyCnt += iPrimitiveNum;
			}
		}
	}
	else
	{
		RwD3D9SetStreamSource(0, (IDirect3DVertexBuffer9*)pResEntryHeader->vertexStream->vertexBuffer, 0, pResEntryHeader->vertexStream->stride);
		RwD3D9SetIndices((IDirect3DIndexBuffer9*)pResEntryHeader->indexBuffer);
		RwD3D9DrawIndexedPrimitive((D3DPRIMITIVETYPE)pResEntryHeader->primType,
			pInstancedData->baseIndex,
			0,
			pInstancedData->numVertices,
			pInstancedData->startIndex,
			pInstancedData->numPrimitives);

		dGET_WORLD_PARAM()->WorldPolyCnt += pInstancedData->numPrimitives;
	}
}

void _RpNtlWorldSectorWireSolid(RxD3D9InstanceData *pInstancedData, RxD3D9ResEntryHeader *pResEntryHeader)
{
	if(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_WIRE_SOLID))
	{
		BeginTerrainWireFrameState();
		{
			RwMatrix matWireWorld;
			RwV3d Pos;
			Pos.x = 0.0f;
			Pos.y = 0.3f;
			Pos.z = 0.0f;

			RwMatrixTranslate(&matWireWorld, &Pos, rwCOMBINEREPLACE);

			RwD3D9SetTransformWorld(&matWireWorld);

			RwD3D9SetStreamSource(0, (IDirect3DVertexBuffer9*)pResEntryHeader->vertexStream->vertexBuffer, 0, pResEntryHeader->vertexStream->stride);
			RwD3D9SetIndices((IDirect3DIndexBuffer9*)pResEntryHeader->indexBuffer);
			RwD3D9DrawIndexedPrimitive(	(D3DPRIMITIVETYPE)pResEntryHeader->primType,
				pInstancedData->baseIndex,
				0,
				pInstancedData->numVertices,
				pInstancedData->startIndex,
				pInstancedData->numPrimitives);

			RwD3D9SetTransformWorld(NULL);
		}
		EndTerrainWireFrameState();					
	}
}

#ifdef dNTL_WORLD_COMBINE
void _RpNtlWorldD3D9RenderCallback02(RwResEntry *repEntry, void *object, RwUInt8 type, RwUInt32 flags)
{
	NTL_SPROFILE("_RpNtlWorldD3D9RenderCallback")

		RxD3D9ResEntryHeader *pResEntryHeader;
	RxD3D9InstanceData *pInstancedData;
	RwInt32 numMeshes;

	// get d3d devices
	LPDIRECT3DDEVICE9 pD3DDev = static_cast<LPDIRECT3DDEVICE9>(RwD3D9GetCurrentD3DDevice());

	// Set the Default Pixel shader
	RwD3D9SetPixelShader(NULL);

	// Set clipping
	_rwD3D9EnableClippingIfNeeded(object, type);

	// Get header
	pResEntryHeader = (RxD3D9ResEntryHeader *)(repEntry + 1);

	// Vertex Declaration
	RwD3D9SetVertexDeclaration(pResEntryHeader->vertexDeclaration);

	/* Get the instanced data */
	pInstancedData = (RxD3D9InstanceData*)(pResEntryHeader + 1);

	/* Get the number of meshes */
	numMeshes = pResEntryHeader->numMeshes;
	if (numMeshes)
	{
		do
		{
			RpMaterial *pMaterial;
			RwRGBA *pMatcolor;

			pMaterial = pInstancedData->material;
			pMatcolor = &(pMaterial->color);

			// Vertex shader
			RwD3D9SetVertexShader(pInstancedData->vertexShader);

			sRpNtlWorldSectorSplat	*pNtlWorldSectorSplat	= RpNtlWorldGetSectorSplat(pInstancedData->material);
			CNtlWorldSector			*pNtlWorldSector		= pNtlWorldSectorSplat->pNtlWorldSector;

			if(_RpNtlWorldSectorCullCheck(pNtlWorldSector))
			{
				continue;
			}

			RwBool bRenderTex = !_RpNtlWorldSectorIsFullFogged(pNtlWorldSector);
			if(bRenderTex && pNtlWorldSector->m_pNtlWorldShadow->m_pTexShadow)
			{

				//#ifdef dNTL_WORLD_TOOL_MODE
				if(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_PROJECTION_SHADOW))
					//#endif
				{
					BeginTerrrainShadowState();

					RwD3D9SetTransform(D3DTS_TEXTURE2, &pNtlWorldSector->m_pNtlWorldShadow->m_matTex);
					RwD3D9SetTexture(pNtlWorldSector->m_pNtlWorldShadow->m_pTexShadow, 2);
				}
			}

			BegTerrainLayerCommonState();
			{
				BegTerrainBaseLayerState(pNtlWorldSector->m_IsTileTransparency);
				{
					sRpNtlWorldSectorSplat	*pNtlWorldSectorSplat	= RpNtlWorldGetSectorSplat(pInstancedData->material);
					CNtlWorldSector			*pNtlWorldSector		= pNtlWorldSectorSplat->pNtlWorldSector;

					RwD3D9SetTexture(NULL, 0);

					if(bRenderTex)
					{
						RwD3D9SetTexture(pNtlWorldSector->m_pTextureCombine, 1);
					}
					else
					{
						RwD3D9SetTexture(NULL, 1);		
					}

					RwD3D9SetTransform(D3DTS_TEXTURE0, &pNtlWorldSector->m_TextureMatBase[0]);
					RwD3D9SetTransform(D3DTS_TEXTURE1, &pNtlWorldSector->m_TextureMatCombine);

					_RpNtlWorldSectorDrawPrimitive(pInstancedData, pResEntryHeader);
				}
				EndTerrainBaseLayerState(pNtlWorldSector->m_IsTileTransparency);
#ifdef dNTL_WORLD_TOOL_MODE
				_RpNtlWorldSectorWireSolid(pInstancedData, pResEntryHeader);
#endif
			}
			EndTerrainLayerCommonState();

			if(bRenderTex && pNtlWorldSector->m_pNtlWorldShadow->m_pTexShadow)
			{
				//#ifdef dNTL_WORLD_TOOL_MODE
				if(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_PROJECTION_SHADOW))
					//#endif
				{
					EndTerrrainShadowState();
				}
			}
		} while(++pInstancedData, --numMeshes); // Move onto the next pInstancedData
	}

	NTL_EPROFILE()
}

static void
CombineRenderCallback(RwResEntry *repEntry, void *object, RwUInt8 type, RwUInt32 flags)
{
	RxD3D9ResEntryHeader *pResEntryHeader;
	RxD3D9InstanceData *pInstancedData;
	RwInt32 numMeshes;

	// get d3d devices
	LPDIRECT3DDEVICE9 pD3DDev = static_cast<LPDIRECT3DDEVICE9>(RwD3D9GetCurrentD3DDevice());

	// Set the Default Pixel shader
	RwD3D9SetPixelShader(NULL);

	// Set clipping
	_rwD3D9EnableClippingIfNeeded(object, type);

	// Get header
	pResEntryHeader = (RxD3D9ResEntryHeader *)(repEntry + 1);

	// Vertex Declaration
	RwD3D9SetVertexDeclaration(pResEntryHeader->vertexDeclaration);

	/* Get the instanced data */
	pInstancedData = (RxD3D9InstanceData*)(pResEntryHeader + 1);

	/* Get the number of meshes */
	numMeshes = pResEntryHeader->numMeshes;
	if (numMeshes)
	{
		do
		{
			RpMaterial *pMaterial;
			RwRGBA *pMatcolor;

			pMaterial = pInstancedData->material;
			pMatcolor = &(pMaterial->color);

			// Vertex shader
			RwD3D9SetVertexShader(pInstancedData->vertexShader);

			sRpNtlWorldSectorSplat	*pNtlWorldSectorSplat	= RpNtlWorldGetSectorSplat(pInstancedData->material);
			CNtlWorldSector			*pNtlWorldSector		= pNtlWorldSectorSplat->pNtlWorldSector;

			RwD3D9SetRenderState(D3DRS_LIGHTING, FALSE);

			if (pNtlWorldSector->m_IsTileTransparency)
			{
				RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)TRUE);
			}

			RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
			RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);

			RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
			RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); 
			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

			if (pNtlWorldSector->m_IsTileTransparency)
			{
				RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
				RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
				RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			}
			else
			{
				RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
				RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
			}


			RwD3D9SetTexture(pNtlWorldSector->m_pTextureBase, 0);

			RwD3D9SetTransform(D3DTS_TEXTURE0, &pNtlWorldSector->m_TextureMatBase[1]);

			_RpNtlWorldSectorDrawPrimitive(pInstancedData, pResEntryHeader);

			RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)TRUE); 
			RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
			RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);

			RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_CURRENT);
			RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
			RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); 
			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

			RwD3D9SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			RwD3D9SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
			RwD3D9SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

			if (pNtlWorldSector->m_IsTileTransparency)
			{
				RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
				RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
				RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			}
			else
			{
				RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
				RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
				RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
			}

			for(RwInt32 i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
			{
				if(pNtlWorldSector->m_pTextureSplat[i] && pNtlWorldSector->m_pTextureAlpha[i])
				{
					RwD3D9SetTexture(pNtlWorldSector->m_pTextureAlpha[i], 0);
					RwD3D9SetTexture(pNtlWorldSector->m_pTextureSplat[i], 1);
					RwD3D9SetTransform(D3DTS_TEXTURE0, &pNtlWorldSector->m_TextureMat[i][0]);
					RwD3D9SetTransform(D3DTS_TEXTURE1, &pNtlWorldSector->m_TextureMat[i][1]);

					_RpNtlWorldSectorDrawPrimitive(pInstancedData, pResEntryHeader);
				}
			}



			RwD3D9SetRenderState(D3DRS_LIGHTING, TRUE);
			RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);

			RwD3D9SetTexture(NULL, 0);
			RwD3D9SetTexture(NULL, 1);

			D3DXMATRIXA16 mIdentity;
			D3DXMatrixIdentity(&mIdentity);
			RwD3D9SetTransform(D3DTS_TEXTURE0, &mIdentity);
			RwD3D9SetTransform(D3DTS_TEXTURE1, &mIdentity);

			RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
			RwD3D9SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
			RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
			RwD3D9SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);

			RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)FALSE); 
			RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)rwBLENDSRCALPHA);
			RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)rwBLENDINVSRCALPHA);

			RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE); 
			RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT); 
			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

			RwD3D9SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
			RwD3D9SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			RwD3D9SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
			RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
			RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
		} while(++pInstancedData, --numMeshes); // Move onto the next pInstancedData
	}
}


RwTexture* NtlTerrainCombineTextureGenerator(RwInt32 iSectorIdx)
{
	RwCamera* pOldCamera = RwCameraGetCurrentCamera();
	if (pOldCamera)
	{
		RwCameraEndUpdate(pOldCamera);
	}

	CNtlWorldSector*	pSector		= &GetSceneManager()->GetWorld()->GetWorldFieldMgr()->m_pSectors[iSectorIdx];
	RwV3d				vSectorMid;	vSectorMid.x = pSector->DatumPoint.x; vSectorMid.y = pSector->DatumPoint.y; vSectorMid.z = pSector->DatumPoint.z;

	RwCamera*			pCamera		= RwCameraCreate();
	RwFrame*			pFrame		= RwFrameCreate();

	RwRaster*			pRaster		= RwRasterCreate(512, 512, 0, rwRASTERFORMAT8888 | rwRASTERTYPECAMERATEXTURE);
	RwRaster*			pRasterZ	= RwRasterCreate(512, 512, 0, rwRASTERTYPEZBUFFER);
	RwTexture*			pTexture	= RwTextureCreate(NULL);

	RwCameraSetFrame(pCamera, pFrame);
	RwCameraSetRaster(pCamera, pRaster);
	RwCameraSetZRaster(pCamera, pRasterZ);
	RwTextureSetRaster(pTexture, pRaster);

	RwCameraSetProjection(pCamera, rwPARALLEL);
	RwCameraSetNearClipPlane(pCamera, 0.001f);
	RwCameraSetFarClipPlane(pCamera, 2048.0f);

	RwV2d vw; vw.x = vw.y = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize / 2);
	RwCameraSetViewWindow(pCamera, &vw);

	RwMatrix*	pMatrix	= RwFrameGetMatrix(pFrame);

	CNtlMath::MathRwV3dAssign(RwMatrixGetAt(pMatrix), 0.0f, -1.0f, 0.0f);
	CNtlMath::MathRwV3dAssign(RwMatrixGetRight(pMatrix), 1.0f, 0.0f, 0.0f);
	CNtlMath::MathRwV3dAssign(RwMatrixGetUp(pMatrix), 0.0f, 0.0f, 1.0f);

	*RwMatrixGetPos(pMatrix)	= vSectorMid;	
	RwV3dIncrementScaled(RwMatrixGetPos(pMatrix), RwMatrixGetAt(pMatrix), -0.5f * RwCameraGetFarClipPlane(pCamera));

	RwMatrixUpdate(pMatrix);
	RwFrameUpdateObjects(pFrame);

	static RwRGBA rgba = { 0, 0, 0, 0 };
	if (RwCameraClear(pCamera, &rgba, rwCAMERACLEARZ | rwCAMERACLEARIMAGE))
	{
		if(RwCameraBeginUpdate(pCamera))
		{
			RxD3D9AllInOneSetLightingCallBack(g_pNodeWorld, NULL);
			RxD3D9AllInOneSetRenderCallBack(g_pNodeWorld, CombineRenderCallback);
			RpAtomicRender(pSector->m_pAtomic);

			RwCameraEndUpdate(RwCameraGetCurrentCamera());
		}
	}

	RwRaster* pNewRaster	= RwRasterCreate( RwRasterGetWidth(pRaster), RwRasterGetHeight(pRaster), RwRasterGetDepth(pRaster), rwRASTERTYPETEXTURE | rwRASTERFORMAT8888 );
	RwTexture* pNewTexture	= RwTextureCreate(pNewRaster);
	RwTextureSetFilterMode(pNewTexture, RwTextureGetFilterMode(pTexture));
	RwTextureSetAddressingU(pNewTexture, RwTextureGetAddressingU(pTexture));
	RwTextureSetAddressingV(pNewTexture, RwTextureGetAddressingV(pTexture));

	CNtlPLResourceManager::GetInstance()->CopyRaster(pTexture, pNewTexture);

	RwCameraSetFrame(pCamera, NULL);
	RwCameraSetRaster(pCamera, NULL);
	RwCameraSetZRaster(pCamera, NULL);
	RwTextureSetRaster(pTexture, NULL);

	RwRasterDestroy(pRasterZ);
	RwRasterDestroy(pRaster);
	RwFrameDestroy(pFrame);
	RwCameraDestroy(pCamera);
	RwTextureDestroy(pTexture);

	if (pOldCamera)
	{
		RwCameraBeginUpdate(pOldCamera);
	}

	return pNewTexture;
}

#endif

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#endif
