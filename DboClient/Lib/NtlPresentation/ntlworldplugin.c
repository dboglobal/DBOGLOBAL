/******************************************************************************************************************************
 *
 * ntlworldplugin.h
 *
 * copyright (c) 2005 ntl-inc Ltd.
 *
 * author : woody1019
 *
 * purpose : creates a bsp in a quad-tree arrangement for the interlocking tiles to reside in.
 *
 ******************************************************************************************************************************/


#include <d3d9.h>
#include "ntlworldplugin.h"
#include "ntlworldinterface.h"
#include "NtlWorldMaterialPlugin.h"
#include <rwcore.h>


void _rpNtlWorldD3D9RenderCallback(RwResEntry *repEntry, void *object, RwUInt8 type, RwUInt32 flags)
{
	RxD3D9ResEntryHeader    *resEntryHeader;
	RxD3D9InstanceData      *instancedData;
	RwInt32                 numMeshes;
	RwBool                  lighting;
	RwBool                  vertexAlphaBlend;
	RwUInt32                lastRenderFlags;
	sRpNtlWorldSectorSplat* pNtlWorldSectorSplat;

	/*
	* Set the Default Pixel shader
	*/
	RwD3D9SetPixelShader(NULL);

	/* Set clipping */
	_rwD3D9EnableClippingIfNeeded(object, type);

	/* Get header */
	resEntryHeader = (RxD3D9ResEntryHeader *)(repEntry + 1);

	/*
	* Data shared between meshes
	*/
	if (resEntryHeader->indexBuffer != NULL)
	{
		RwD3D9SetIndices(resEntryHeader->indexBuffer);
	}

	/* Set the stream sources */
	_rwD3D9SetStreams(resEntryHeader->vertexStream,
		resEntryHeader->useOffsets);

	/*
	* Vertex Declaration*/
	
	RwD3D9SetVertexDeclaration(resEntryHeader->vertexDeclaration);

	/* check lighting */
	RwD3D9GetRenderState(D3DRS_LIGHTING, &lighting);

	if (lighting == FALSE)
	{
		if ((flags & rxGEOMETRY_PRELIT) == 0)
		{
			//D3D9DefaultRenderBlack(resEntryHeader,
			//	(flags & (rxGEOMETRY_TEXTURED | rxGEOMETRY_TEXTURED2)) != 0);

			return;//RWRETURNVOID();
		}
	}

	/* Get vertex alpha Blend state */
	vertexAlphaBlend = _rwD3D9RenderStateIsVertexAlphaEnable();

	/* force refresh*/
	lastRenderFlags = 0x80000000;

	/* Get the instanced data */
	instancedData = (RxD3D9InstanceData *)(resEntryHeader + 1);

	/* Get the number of meshes */
	numMeshes = resEntryHeader->numMeshes;
	while (numMeshes--)
	{
		const RpMaterial    *material;
		const RwRGBA        *matcolor;
		RwUInt32            currentRenderFlags = 0;

		//RWASSERT(instancedData->material != NULL);

		material = instancedData->material;

		matcolor = &(material->color);

		if ((0xFF != matcolor->alpha) ||
			instancedData->vertexAlpha)
		{
			if (vertexAlphaBlend == FALSE)
			{
				vertexAlphaBlend = TRUE;

				_rwD3D9RenderStateVertexAlphaEnable(TRUE);
			}
		}
		else
		{
			if (vertexAlphaBlend != FALSE)
			{
				vertexAlphaBlend = FALSE;

				_rwD3D9RenderStateVertexAlphaEnable(FALSE);
			}
		}
/*
		if (lighting)
		{
			RwD3D9SetSurfaceProperties(&(material->surfaceProps),
				matcolor,
				flags);
		}
		else
		{
			if (flags & rxGEOMETRY_MODULATE)
			{
				if (*((const RwUInt32 *)matcolor) != 0xffffffff)
				{
					RwUInt32 tFactor;

					currentRenderFlags |= RENDERFLAGS_HAS_TFACTOR;

					tFactor =
						((((RwUInt32)matcolor->alpha)<<24)|(((RwUInt32)matcolor->red)<<16)|(((RwUInt32)matcolor->green)<<8)|((RwUInt32)matcolor->blue));

					RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, tFactor);
				}
			}
		}

		if (material->texture != NULL &&
			(flags & (rxGEOMETRY_TEXTURED | rxGEOMETRY_TEXTURED2)) != 0)
		{
			currentRenderFlags |= RENDERFLAGS_HAS_TEXTURE;

			RwD3D9SetTexture(material->texture, 0);
		}
		else
		{
			RwD3D9SetTexture(NULL, 0);
		}
*/
		pNtlWorldSectorSplat = RpNtlWorldGetSectorSplat(material);
		RwD3D9SetTexture(pNtlWorldSectorSplat->pTextureBase, 0);

		//IDirect3DDevice9_SetTexture()
		//LPDIRECT3DBASETEXTURE9
		//DXCHECK(IDirect3DDevice9_SetTexture(_RwD3DDevice, stage,
		//	(LPDIRECT3DBASETEXTURE9)(rasExt->texture)));

		//RwRenderStateSet(rwRENDERSTATETEXTUREADDRESS, rwTEXTUREADDRESSWRAP);

		RwD3D9SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		RwD3D9SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

		RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
		RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

		RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
		RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		/*
		RwD3D9SetTexture(material->texture, 0);
		if (currentRenderFlags != lastRenderFlags)
		{
			if (currentRenderFlags & RENDERFLAGS_HAS_TEXTURE)
			{
				if ((lastRenderFlags & RENDERFLAGS_HAS_TEXTURE) == 0)
				{
					RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
					RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
					RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

					RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
					RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
					RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
				}

				if (currentRenderFlags & RENDERFLAGS_HAS_TFACTOR)
				{
					RwD3D9SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_MODULATE);
					RwD3D9SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
					RwD3D9SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TFACTOR);

					RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
					RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
					RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
				}
				else
				{
					if (lastRenderFlags == (RENDERFLAGS_HAS_TEXTURE | RENDERFLAGS_HAS_TFACTOR))
					{
						RwD3D9SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
						RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
					}
				}
			}
			else
			{
				if (currentRenderFlags & RENDERFLAGS_HAS_TFACTOR)
				{
					RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
					RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
					RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

					RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
					RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
					RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
				}
				else
				{
					RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG2);
					RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

					RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG2);
					RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
				}

				if (lastRenderFlags == (RENDERFLAGS_HAS_TEXTURE | RENDERFLAGS_HAS_TFACTOR))
				{
					RwD3D9SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
					RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
				}
			}

			lastRenderFlags = currentRenderFlags;
		}
*/
		
		// Vertex shader
		
		RwD3D9SetVertexShader(instancedData->vertexShader);

		
		// Render
		
		if (resEntryHeader->indexBuffer != NULL)
		{
			RwD3D9DrawIndexedPrimitive((D3DPRIMITIVETYPE)resEntryHeader->primType,
				instancedData->baseIndex,
				0, instancedData->numVertices,
				instancedData->startIndex, instancedData->numPrimitives);

			dGET_WORLD_PARAM()->WorldPolyCnt += instancedData->numPrimitives;
		}
		else
		{
			RwD3D9DrawPrimitive((D3DPRIMITIVETYPE)resEntryHeader->primType,
				instancedData->baseIndex,
				instancedData->numPrimitives);

			dGET_WORLD_PARAM()->WorldPolyCnt += instancedData->numPrimitives;
		}

		// Move onto the next instancedData 
		instancedData++;
	}


/*
	if (lastRenderFlags == (RENDERFLAGS_HAS_TEXTURE | RENDERFLAGS_HAS_TFACTOR))
	{
		RwD3D9SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
		RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
	}

	}*/
	//RWRETURNVOID();

	return;
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
			RxNodeDefinition	*pInstanceNode;

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
}

RwBool rpNtlWorldSectorPipeLineCreate(void)
{
	RxNodeDefinition*	pInstanceNode;
	RxPipelineNode*		pNode;

	/* create a new atomic pipeline */
	D3D9NtlWorldSectorAtomicPipeline = _rpNtlWorldSectorAtomicPipeline();

	if(!D3D9NtlWorldSectorAtomicPipeline)
		return FALSE;

	pInstanceNode = RxNodeDefinitionGetD3D9AtomicAllInOne();

	pNode = RxPipelineFindNodeByName(D3D9NtlWorldSectorAtomicPipeline, pInstanceNode->name, NULL, NULL);

	/* set Lighting callback - just turn it off for now to prevent hw lighting being set up */
	RxD3D9AllInOneSetLightingCallBack(pNode, NULL);

	/* node rendercallback */
	RxD3D9AllInOneSetRenderCallBack(pNode, _rpNtlWorldD3D9RenderCallback);

	return TRUE;
}

/******************************************************************************************************************************
 * NtlWorldGlobalDataConstructor :
 ******************************************************************************************************************************/
static void *NtlWorldGlobalDataConstructor(void *unused, RwInt32 offset __RWUNUSED__, RwInt32 size __RWUNUSED__)
{
	dNTL_WORLD_GLOBAL(WorldSectorCBRender) = RpNtlWorldSectorRender;
	dNTL_WORLD_GLOBAL(pSectorFreelist) = RwFreeListCreateAndPreallocateSpace(sizeof(sNtlWorldSector), dGET_WORLD_PARAM()->WorldSectorNum * dGET_WORLD_PARAM()->WorldSectorNum, 4, 1, NULL, rwMEMHINTDUR_EVENT);

	/* create a world sector pipeline 메모리 조심 */
	rpNtlWorldSectorPipeLineCreate();

    return unused;
}

/******************************************************************************************************************************
 * NtlWorldGlobalDataDestructor :
 ******************************************************************************************************************************/
static void *NtlWorldGlobalDataDestructor(void *unused, RwInt32 offset __RWUNUSED__, RwInt32 size __RWUNUSED__)
{
	RwFreeListDestroy(dNTL_WORLD_GLOBAL(pSectorFreelist));

	rpNtlWorldSectorPipeLineDelete();

    return unused;
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
		_RpNtlWorldSectorGeometryDelete(pWorldSector);

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

/******************************************************************************************************************************
* RpNtlWorldSectorGeometryCreate :
******************************************************************************************************************************/
RwBool RpNtlWorldSectorGeometryCreate(RpWorldSector *pWorldSector)
{
	dNTL_WORLD_LOCAL(pWorldSector, pNtlSector) = _RpNtlWorldSectorGeometryCreate(pWorldSector);

	return TRUE;
}

RwBool RpNtlWorldSectorGeometryDelete(RpWorldSector *pWorldSector)
{
	_RpNtlWorldSectorGeometryDelete(pWorldSector);

	return TRUE;
}

RwBool RpNtlWorldSectorGeometryCreateFromFile(RpWorldSector *pWorldSector, RwChar *pSectorFileName, FILE* pFile)
{
	_RpNtlWorldSectorGeometryLoadFromFile(pWorldSector, pSectorFileName, pFile);

	return TRUE;
}

RwBool RpNtlWorldSectorGeometryDeleteIntoFile(RpWorldSector *pWorldSector, RwChar *pSectorFileName, FILE* pFile)
{
	_RpNtlWorldSectorGeometrySaveIntoFile(pWorldSector, pSectorFileName, pFile);

	return TRUE;
}

/******************************************************************************************************************************
 * RpNtlWorldSectorRender :
 ******************************************************************************************************************************/
RpWorldSector *RpNtlWorldSectorRender(RpWorldSector *pRpWorldSector)
{
	sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(pRpWorldSector, pNtlSector);

	if(pNtlSector)
	{
		RpAtomic *pAtomic = dNTL_WORLD_LOCAL(pRpWorldSector, pNtlSector)->pAtomic;

		if(pAtomic)
		{
			RpAtomicRender(pAtomic);
		}
	}
	
	return pRpWorldSector;
}