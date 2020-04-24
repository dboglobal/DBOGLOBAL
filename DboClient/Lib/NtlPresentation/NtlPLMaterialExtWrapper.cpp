#include "precomp_ntlpresentation.h"
#include "NtlDebug.h"
#include "NtlPLMaterialExtWrapper.h"
#include "NtlAtomic.h"
#include "NtlPLWorldEntity.h"
#include "NtlWorldFieldManager.h"
#include "NtlPLSceneManager.h"
#include "NtlWorldShadow.h"
#include "NtlPLEntityRenderHelpers.h"
#include "ntlworldinterface.h"

static RxPipeline   *g_sNtlMatExtAtomicPipe = NULL;

#define RENDERFLAGS_HAS_TEXTURE  1
#define RENDERFLAGS_HAS_TFACTOR  2

// Black Render
static void D3D9DefaultRenderBlack(RxD3D9ResEntryHeader *resEntryHeader,RpAtomic *pAtomic, RwBool hasTextureCoordinates)
{
    RxD3D9InstanceData *instancedData;
    RwUInt32    ditherEnable;
    RwUInt32    shadeMode;
    RwBool      useAlphaTexture;
    RwInt32     numMeshes;
	
	RwD3D9GetRenderState(D3DRS_DITHERENABLE, &ditherEnable);
    RwD3D9GetRenderState(D3DRS_SHADEMODE, &shadeMode);

    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)FALSE);
    RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, 0xff000000);
    RwD3D9SetRenderState(D3DRS_DITHERENABLE, FALSE);
    RwD3D9SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);

    /* Always black color (only alpha channel may change) */
    RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG2);
    RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

    RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG2);
    RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

    RwD3D9SetTexture(NULL, 0);

    
    useAlphaTexture = FALSE;

    instancedData = (RxD3D9InstanceData *)(resEntryHeader + 1);
	
	numMeshes = resEntryHeader->numMeshes;
    while (numMeshes--)
    {
		NTL_ASSERTE(instancedData->material != NULL);
		
		if (hasTextureCoordinates &&
            instancedData->material->texture &&
			((RpNtlAtomicGetFlag(pAtomic) & NTL_ALPHA) ||
			 (RpNtlAtomicGetFlag(pAtomic) & NTL_ALPHATEST)) )
            //_rwD3D9TextureHasAlpha(instancedData->material->texture))
        {
            RwD3D9SetTexture(instancedData->material->texture, 0);

            if (useAlphaTexture == FALSE)
            {
                useAlphaTexture = TRUE;
				RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
            }
        }
        else
        {
            if (useAlphaTexture)
            {
                useAlphaTexture = FALSE;

                RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG2);

                RwD3D9SetTexture(NULL, 0);
            }
        }

        RwD3D9SetVertexShader(instancedData->vertexShader);

        if (resEntryHeader->indexBuffer != NULL)
        {
            RwD3D9DrawIndexedPrimitive((D3DPRIMITIVETYPE)resEntryHeader->primType,
                                       instancedData->baseIndex,
                                       0, instancedData->numVertices,
                                       instancedData->startIndex, instancedData->numPrimitives);
        }
        else
        {
            RwD3D9DrawPrimitive((D3DPRIMITIVETYPE)resEntryHeader->primType,
                                instancedData->baseIndex,
                                instancedData->numPrimitives);
        }

        instancedData++;
    }

    RwD3D9SetRenderState(D3DRS_DITHERENABLE, ditherEnable);
    RwD3D9SetRenderState(D3DRS_SHADEMODE, shadeMode);

    return;
}

static void NtlMatExtRenderCallback( RwResEntry *repEntry, void *object,RwUInt8 type, RwUInt32 flags)
{
	RxD3D9ResEntryHeader    *resEntryHeader;
	RxD3D9InstanceData      *instancedData;
	RwInt32                 numMeshes;
	RwBool                  lighting;
	RwBool                  vertexAlphaBlend;
	RwUInt32                lastRenderFlags;
	RpAtomic*				pCurAtomic = static_cast<RpAtomic*>(object);

	RwD3D9SetPixelShader(NULL);

	_rwD3D9EnableClippingIfNeeded(object, type);

	resEntryHeader = (RxD3D9ResEntryHeader *)(repEntry + 1);

	if (resEntryHeader->indexBuffer != NULL)
	{
		RwD3D9SetIndices(resEntryHeader->indexBuffer);
	}

	_rwD3D9SetStreams(resEntryHeader->vertexStream, resEntryHeader->useOffsets);

	RwD3D9SetVertexDeclaration(resEntryHeader->vertexDeclaration);

	RwD3D9GetRenderState(D3DRS_LIGHTING, &lighting);

	if (lighting == FALSE)
	{
		if ((flags & rxGEOMETRY_PRELIT) == 0)
		{
			D3D9DefaultRenderBlack(resEntryHeader, (RpAtomic *)object, (flags & (rxGEOMETRY_TEXTURED | rxGEOMETRY_TEXTURED2)) != 0);
			return;
		}
	}

	/* Get vertex alpha Blend state */
	RwRenderStateGet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)&vertexAlphaBlend);

	/* force refresh */
	lastRenderFlags = 0x80000000;

	/* Get the instanced data */
	instancedData = (RxD3D9InstanceData *)(resEntryHeader + 1);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CNtlWorldShadow* _pUserDat = static_cast<CNtlWorldShadow*>(RpNtlAtomicGetUserDat(pCurAtomic));

	// dogi gym. emblem mark attach
	RwTexture* pEmblemMark = NULL;
	if(RpNtlAtomicGetFlag(pCurAtomic) & NTL_EMBLEM_MARK)
	{
		pEmblemMark = static_cast<RwTexture*>(RpNtlAtomicGetUserDat(pCurAtomic));
	}
	// only if the pCurAtomic's got a texture for shadow
	else if(_pUserDat && _pUserDat->m_pTexShadow)
	{
		if(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_PROJECTION_SHADOW))
		{
			_pUserDat->Update();
			RwD3D9SetTexture(_pUserDat->m_pTexShadow, 1);
			RwD3D9SetTransform(D3DTS_TEXTURE1, &_pUserDat->m_matTex);
			RwD3D9SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			RwD3D9SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);	
			RwD3D9SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
			RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
			RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
			RwD3D9SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION);
			RwD3D9SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/* Get the number of meshes */
	numMeshes = resEntryHeader->numMeshes;
	while (numMeshes--)
	{
		const RpMaterial    *material;
		const RwRGBA        *matcolor;
		RwUInt32            currentRenderFlags = 0;

		NTL_ASSERTE(instancedData->material != NULL);

		material = instancedData->material;

		matcolor = &(material->color);

		if ((0xFF != matcolor->alpha) ||
			instancedData->vertexAlpha)
		{
			if (vertexAlphaBlend == FALSE)
			{
				vertexAlphaBlend = TRUE;
				RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)TRUE);
			}
		}
		else
		{
			if (vertexAlphaBlend != FALSE)
			{
				vertexAlphaBlend = FALSE;
				RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)FALSE);
			}
		}

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

					tFactor = ((((RwUInt32)matcolor->alpha)<<24)|(((RwUInt32)matcolor->red)<<16)|(((RwUInt32)matcolor->green)<<8)|((RwUInt32)matcolor->blue));

					RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, tFactor);
				}
			}
		}

		// Cz : WorldEditor에서 Shadow Property를 추출할 때 rxGEOMETRY_TEXTURED, rxGEOMETRY_TEXTURED2 Flag를
		// 제거 하기 때문에 추출 시 Texture가 NULL로 설정되어 Radeon에서 ShadowMap이 Render되지 않는다.
		// WorldEditor에서는 이부분을 교체하여 사용한다.
#ifdef dNTL_WORLD_TOOL_MODE
		if (material->texture != NULL && (flags & (rxGEOMETRY_TEXTURED | rxGEOMETRY_TEXTURED2)) != 0)
		{
			currentRenderFlags |= RENDERFLAGS_HAS_TEXTURE;

			if(pEmblemMark)
				RwD3D9SetTexture(pEmblemMark, 0);
			else
				RwD3D9SetTexture(material->texture, 0);
			
			RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
			RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
		}
		else
		{
			RwD3D9SetTexture(NULL, 0);

			RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
			RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
			RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
		}

		lastRenderFlags = currentRenderFlags;
#else
		if(material->texture != NULL && (flags & (rxGEOMETRY_TEXTURED | rxGEOMETRY_TEXTURED2)) != 0)
		{
			currentRenderFlags |= RENDERFLAGS_HAS_TEXTURE;

			if(pEmblemMark)
				RwD3D9SetTexture(pEmblemMark, 0);
			else
				RwD3D9SetTexture(material->texture, 0);
		}
		else
		{
			RwD3D9SetTexture(NULL, 0);
		}


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
		RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
		RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

		RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
		RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
#endif

		/*
		* Vertex shader
		*/
		RwD3D9SetVertexShader(instancedData->vertexShader);

		/*
		* Render
		*/
		if (resEntryHeader->indexBuffer != NULL)
		{
			RwD3D9DrawIndexedPrimitive((D3DPRIMITIVETYPE)resEntryHeader->primType,
				instancedData->baseIndex,
				0, instancedData->numVertices,
				instancedData->startIndex, instancedData->numPrimitives);
		}
		else
		{
			RwD3D9DrawPrimitive((D3DPRIMITIVETYPE)resEntryHeader->primType,
				instancedData->baseIndex,
				instancedData->numPrimitives);
		}

		// restore
		if(_pUserDat && _pUserDat->m_pTexShadow)
		{
			if(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_PROJECTION_SHADOW))
			{
				D3DXMATRIXA16 mIdentity;
				D3DXMatrixIdentity(&mIdentity);
				RwD3D9SetTransform(D3DTS_TEXTURE1, &mIdentity);
				RwD3D9SetTexture(NULL, 1);

				RwD3D9SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
				RwD3D9SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
				RwD3D9SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
				RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
				RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
				RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

				RwD3D9SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
				RwD3D9SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);

			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		// 선택했을때 밝게 처리 되는 부분
		if(RpNtlMaterialExtGetFlag(material) & rpNTL_MATERIAL_ADD_COLOR)
		{
			D3DCOLOR    matColor;
			const RwRGBA *pAddColor = NULL;
			RwBlendFunction srcBlend, destBlend;
			RwBool  zWriteEnable;

			pAddColor = RpNtlMaterialExtGetAddColor(material);
			RwD3D9SetTexture(material->texture, 0);
			matColor = (((RwUInt32)pAddColor->alpha) << 24) |
				(((RwUInt32)pAddColor->red) << 16) |
				(((RwUInt32)pAddColor->green) << 8) |
				pAddColor->blue;

			RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, matColor );

			RwRenderStateGet(rwRENDERSTATESRCBLEND, (void *)&srcBlend);
			RwRenderStateGet(rwRENDERSTATEDESTBLEND, (void *)&destBlend);

			RwD3D9GetRenderState(D3DRS_ZWRITEENABLE, (void *)&zWriteEnable);
			RwD3D9SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)TRUE);

			//Add
			RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDONE);
			RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDONE);

			RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE2X);
			RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
			RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

			RwD3D9SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
			RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);

			if (resEntryHeader->indexBuffer != NULL)
			{
				RwD3D9DrawIndexedPrimitive((D3DPRIMITIVETYPE)resEntryHeader->primType,
					instancedData->baseIndex,
					0, instancedData->numVertices,
					instancedData->startIndex, instancedData->numPrimitives);
			}
			else
			{
				RwD3D9DrawPrimitive((D3DPRIMITIVETYPE)resEntryHeader->primType,

					instancedData->baseIndex,
					instancedData->numPrimitives);
			}

			RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)FALSE);
			RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)srcBlend);
			RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)destBlend);

			RwD3D9SetRenderState(D3DRS_ZWRITEENABLE, zWriteEnable);
		}
		instancedData++;
	}

	RwD3D9SetTexture(NULL, 0);
	RwD3D9SetTexture(NULL, 1);
	RwD3D9SetTransform(D3DTS_TEXTURE0, &CNtlPLGlobal::m_mD3D9IdentityMatrix);
	RwD3D9SetTransform(D3DTS_TEXTURE1, &CNtlPLGlobal::m_mD3D9IdentityMatrix);

	RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	if (lastRenderFlags == (RENDERFLAGS_HAS_TEXTURE | RENDERFLAGS_HAS_TFACTOR))
	{
		RwD3D9SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
		RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
	}
}

static RxPipeline *AtomicNtlMatExtPipelineCreate(void)
{
    RxPipeline  *pipe;

    pipe = RxPipelineCreate();
    if (pipe)
    {
        RxLockedPipe    *lpipe;

        pipe->pluginId = rwID_NTL_MATERIAL_EXT;
        lpipe = RxPipelineLock(pipe);
        if (NULL != lpipe)
        {
            RxNodeDefinition    *instanceNode;

            /*
             * Get the instance node definition
             */
            instanceNode = RxNodeDefinitionGetD3D9AtomicAllInOne();

            /*
             * Add the node to the pipeline
             */
            lpipe = RxLockedPipeAddFragment(lpipe, NULL, instanceNode, NULL);

            /*
             * Unlock the pipeline
             */
            lpipe = RxLockedPipeUnlock(lpipe);

			return pipe;
        }

        RxPipelineDestroy(pipe);
    }
	return NULL;
}

RwBool NtlMatExtPipelineCreate(void)
{
	RxNodeDefinition    *instanceNode;
    RxPipelineNode      *node;
	
	g_sNtlMatExtAtomicPipe = AtomicNtlMatExtPipelineCreate();
    NTL_ASSERTE(NULL != g_sNtlMatExtAtomicPipe);
	
	instanceNode = RxNodeDefinitionGetD3D9AtomicAllInOne();
    NTL_ASSERTE(NULL != instanceNode);

    node = RxPipelineFindNodeByName(g_sNtlMatExtAtomicPipe, instanceNode->name, NULL, NULL);
    NTL_ASSERTE(NULL != node);
	
	RxD3D9AllInOneSetRenderCallBack(node, NtlMatExtRenderCallback);
	
	return TRUE;
}

RwBool NtlMatExtPipelineDestroy(void)
{
	if(g_sNtlMatExtAtomicPipe)
	{
		RxPipelineDestroy(g_sNtlMatExtAtomicPipe);
		g_sNtlMatExtAtomicPipe = NULL;
	}

	return TRUE;
}

static void *NtlMatExtOpen(void *instance, RwInt32 offset __RWUNUSED__, RwInt32 size __RWUNUSED__)
{
    NTL_ASSERTE( instance );
    
	if(FALSE == NtlMatExtPipelineCreate())
	{
		instance = NULL;
		return instance;
    }

	return instance;
}

static void *NtlMatExtClose(void *instance, RwInt32 offset __RWUNUSED__, RwInt32 size __RWUNUSED__)
{
    NTL_ASSERTE( instance );
    
	NtlMatExtPipelineDestroy();
	
	return instance;
}


void NtlMatExtSetPipeline(RpAtomic *pAtomic)
{
	NTL_ASSERTE( pAtomic );
	NTL_ASSERTE( g_sNtlMatExtAtomicPipe );
	pAtomic->pipeline = g_sNtlMatExtAtomicPipe;
}

RwBool NtlMatExtPluginAttach(void)
{
	if (RwEngineRegisterPlugin(0, (RwUInt32) MAKECHUNKID(rwVENDORID_CRITERIONTK, rwID_NTL_MATERIAL_EXT), 
		NtlMatExtOpen, NtlMatExtClose) < 0)
    {
        return FALSE;
    }

	return RpNtlMatFxPluginAttach();
}

RwBool NtlMatExtSetMultiTexture(RpMaterial *pMaterial, RwTexture *pTexture)
{
	
	return RpNtlMaterialExtSetTexture(pMaterial, pTexture);
}

RwTexture *NtlMatExtGetMultiTexture(RpMaterial *pMaterial)
{
	return RpNtlMaterialExtGetTexture(pMaterial);
}


void NtlMatExtSetFlag(RpMaterial *pMaterial, RwUInt32 uiFlag)	
{
	RpNtlMaterialExtSetFlag(pMaterial, uiFlag);
}

RwUInt32 NtlMatExtGetFlag(RpMaterial *pMaterial)						
{	
	return RpNtlMaterialExtGetFlag(pMaterial);
}


