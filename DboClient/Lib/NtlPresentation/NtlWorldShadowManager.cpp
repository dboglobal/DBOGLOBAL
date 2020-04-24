////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "precomp_ntlpresentation.h"
#include "NtlWorldShadowManager.h"
#include "NtlWorldFieldManager.h"
#include "NtlWorldShadow.h"
#include "NtlWorldSector.h"
#include "NtlMath.h"
#include "NtlPLObject.h"
#include "ntlworlddi.h"
#include "NtlPLResourceManager.h"
#include "NtlPLRenderState.h"
#include "ntlworldinterface.h"
#include "NtlPLResourceManager.h"
#include "NtlAtomic.h"
#include "ntlworldcommon.h"
#include "ntlworldmaterialplugin.h"
#include "NtlPLSceneManager.h"

#include "NtlWorldBrush.h"
#include "NtlWorldBrushCircle.h"
#include "NtlWorldBrushRectangle.h"
#include "NtlWorldBrushFrustum.h"
#include "NtlWorldBrushLine.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


CNtlWorldShadowManager::CNtlWorldShadowManager(void)
{
	m_pCam						= NULL;
	m_pIPCam					= NULL;
	m_pR2S						= NULL;
	m_pShadowCameraRaster		= NULL;
	m_pShadowCameraRasterAA		= NULL;
	m_pShadowCameraRasterBlur	= NULL;

	// texture brush
	char szOldDir[NTL_MAX_DIR_PATH];
	char szNewDir[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, szOldDir);
	::strcpy_s(szNewDir, "Tool\\SABER\\Texture\\UI\\");
	m_pPSTexBrush = CNtlPLResourceManager::GetInstance()->LoadTexture("PSTexBrush", szNewDir);
	DBO_ASSERT(m_pPSTexBrush, "Texture load failed.");

	:: SetCurrentDirectory(szOldDir);

	m_BrushRadius	= 16.0f;
	m_Opacity		= 0.01f;
}

void CNtlWorldShadowManager::Destroy()
{
	if (m_pPSTexBrush)
	{
		RwTextureDestroy(m_pPSTexBrush);
		m_pPSTexBrush = NULL;
	}
}

CNtlWorldShadowManager::~CNtlWorldShadowManager(void)
{
	DeleteShadowObj();
}

CNtlWorldShadowManager* CNtlWorldShadowManager::GetInst(void)
{
	static CNtlWorldShadowManager NtlWorldShadowManager;
	return &NtlWorldShadowManager;
}

void CNtlWorldShadowManager::DeleteShadowCamera(RwCamera* pShadowCamera)
{
	if(pShadowCamera)
	{
		RwRaster*	pRaster;
		RwFrame*	pFrame;

		pFrame = RwCameraGetFrame(pShadowCamera);
		if(pFrame)
		{
			RwCameraSetFrame(pShadowCamera, NULL);
			RwFrameDestroy(pFrame);
		}

		pRaster = RwCameraGetZRaster(pShadowCamera);
		if(pRaster)
		{
			RwCameraSetZRaster(pShadowCamera, NULL);
			RwRasterDestroy(pRaster);
		}

		pRaster = RwCameraGetRaster(pShadowCamera);
		if (pRaster)
		{
			RwCameraSetRaster(pShadowCamera, NULL);
		}

		RwCameraDestroy(pShadowCamera);
	}
}

void CNtlWorldShadowManager::CreateUITex()
{
	RwReal recipCameraZ = 1.0f / RwCameraGetNearClipPlane(CNtlPLGlobal::m_RwCamera);
	RwInt32 nStartX	= 0;
	RwInt32 nStartY = 128;
	RwInt32 width	= 512;
	RwInt32 height	= 512;
	RwInt32 Alpha	= 160;


	RwIm2DVertexSetScreenX(&m_UITex[0], nStartX-0.5f);
	RwIm2DVertexSetScreenY(&m_UITex[0], nStartY-0.5f);
	RwIm2DVertexSetScreenZ(&m_UITex[0], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_UITex[0], recipCameraZ);
	RwIm2DVertexSetU(&m_UITex[0], 0, recipCameraZ);
	RwIm2DVertexSetV(&m_UITex[0], 0, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_UITex[0], 255, 255, 255, Alpha); 

	RwIm2DVertexSetScreenX(&m_UITex[1], nStartX-0.5f);
	RwIm2DVertexSetScreenY(&m_UITex[1], nStartY+height-0.5f);
	RwIm2DVertexSetScreenZ(&m_UITex[1], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_UITex[1], recipCameraZ);
	RwIm2DVertexSetU(&m_UITex[1], 0, recipCameraZ);
	RwIm2DVertexSetV(&m_UITex[1], 1, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_UITex[1], 255, 255, 255, Alpha); 

	RwIm2DVertexSetScreenX(&m_UITex[2], nStartX+width-0.5f);
	RwIm2DVertexSetScreenY(&m_UITex[2], nStartY-0.5f);
	RwIm2DVertexSetScreenZ(&m_UITex[2], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_UITex[2], recipCameraZ);
	RwIm2DVertexSetU(&m_UITex[2], 1, recipCameraZ);
	RwIm2DVertexSetV(&m_UITex[2], 0, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_UITex[2], 255, 255, 255, Alpha); 

	RwIm2DVertexSetScreenX(&m_UITex[3], nStartX+width-0.5f);
	RwIm2DVertexSetScreenY(&m_UITex[3], nStartY+height-0.5f);
	RwIm2DVertexSetScreenZ(&m_UITex[3], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_UITex[3], recipCameraZ);
	RwIm2DVertexSetU(&m_UITex[3], 1, recipCameraZ);
	RwIm2DVertexSetV(&m_UITex[3], 1, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_UITex[3], 255, 255, 255, Alpha);
}

RwCamera* CNtlWorldShadowManager::CreateShadowCameraIP(RwInt32 _TexSize)
{
	// create a parallel projection camera for shadows under directional lighting. 
	// The raster should be created externally using ShadowRasterCreate(). 

	m_pIPCam = RwCameraCreate();
	if(m_pIPCam)
	{
		RwCameraSetFrame(m_pIPCam, RwFrameCreate());

		if(RwCameraGetFrame(m_pIPCam))
		{
			RwRaster* zRaster;
			zRaster = RwRasterCreate(_TexSize, _TexSize, 0, rwRASTERTYPEZBUFFER);

			if(zRaster)
			{
				RwCameraSetZRaster(m_pIPCam, zRaster);
				RwCameraSetProjection(m_pIPCam, rwPARALLEL);

				return m_pIPCam;
			}
		}
	}

	DeleteShadowCamera(m_pIPCam);

	return NULL;
}

RwCamera* CNtlWorldShadowManager::CreateShadowCamera(RwInt32 _TexSize)
{
	// create a parallel projection camera for shadows under directional lighting. 
	// The raster should be created externally using ShadowRasterCreate(). 

	m_pCam = RwCameraCreate();
	if(m_pCam)
	{
		RwCameraSetFrame(m_pCam, RwFrameCreate());

		if(RwCameraGetFrame(m_pCam))
		{
			RwRaster* zRaster;
			zRaster = RwRasterCreate(_TexSize, _TexSize, 0, rwRASTERTYPEZBUFFER);

			if(zRaster)
			{
				RwCameraSetZRaster(m_pCam, zRaster);
				RwCameraSetProjection(m_pCam, rwPARALLEL);

				return m_pCam;
			}
		}
	}

	DeleteShadowCamera(m_pCam);

	return NULL;
}

void CNtlWorldShadowManager::SetShadowCameraDir(RwMatrix& _MatLight)
{
	RwFrame *pCamFrame = RwCameraGetFrame(m_pCam);

	// Set the shadow camera frame to look along the direction of the light.
	RwMatrixCopy(RwFrameGetMatrix(pCamFrame), &_MatLight);
	RwFrameUpdateObjects(pCamFrame);
}

void CNtlWorldShadowManager::SetShadowCameraFrustum(RwReal _Radius)
{
	RwV2d	vw;

	//  Set the frustum (a cube) to enclose an object of the given radius. 
	//  The frustum may be centered on the object using 

	RwCameraSetFarClipPlane(m_pCam, 4.0f * _Radius);
	RwCameraSetNearClipPlane(m_pCam, 0.001f * _Radius);

	vw.x = _Radius;
	vw.y = _Radius;
	RwCameraSetViewWindow(m_pCam, &vw);
}

void CNtlWorldShadowManager::SetShadowCameraCenter(RwV3d& Center)
{
	RwFrame*	pCamFrame	= RwCameraGetFrame(m_pCam);
	RwMatrix*	pCamMatrix	= RwFrameGetMatrix(pCamFrame);
	RwV3d       Translation;

	// Set the center of the shadow camera frustum volume.
	// -0.5f could be changed 'cos the frustum is orthogonal.
	// It depends on deciding how big the frustum could include objects but if the cam. was far from the target,
	// shadow texture detail will be more lower.
	RwV3dNegate(&Translation, RwMatrixGetPos(pCamMatrix));
	RwV3dAdd(&Translation, &Translation, &Center);
	RwV3dIncrementScaled(&Translation, RwMatrixGetAt(pCamMatrix), -0.5f * RwCameraGetFarClipPlane(m_pCam));

	RwFrameTranslate(pCamFrame, &Translation, rwCOMBINEPOSTCONCAT);
}

void CNtlWorldShadowManager::GetNeighborSecMap(RwV3d& _Pos, CNtlWorldFieldManager* _pNWFManager, CNtlWorldSector* _pNeighborSecMap[9])
{
	RwV3d TmpPos;
	::ZeroMemory(_pNeighborSecMap, sizeof(CNtlWorldSector*) * 9);

	// C
	_pNeighborSecMap[0] = &_pNWFManager->m_pSectors[_pNWFManager->GetSectorIdx(_Pos)];

	// N
	CNtlMath::MathRwV3dAssign(&TmpPos, _Pos.x, _Pos.y, _Pos.z + dGET_WORLD_PARAM()->WorldSectorSize);
	if(_pNWFManager->GetSectorIdx(TmpPos) != -1)	_pNeighborSecMap[1] = &_pNWFManager->m_pSectors[_pNWFManager->GetSectorIdx(TmpPos)];

	// NE
	CNtlMath::MathRwV3dAssign(&TmpPos, _Pos.x - dGET_WORLD_PARAM()->WorldSectorSize, _Pos.y, _Pos.z + dGET_WORLD_PARAM()->WorldSectorSize);
	if(_pNWFManager->GetSectorIdx(TmpPos) != -1)	_pNeighborSecMap[2] = &_pNWFManager->m_pSectors[_pNWFManager->GetSectorIdx(TmpPos)];

	// E
	CNtlMath::MathRwV3dAssign(&TmpPos, _Pos.x - dGET_WORLD_PARAM()->WorldSectorSize, _Pos.y, _Pos.z);
	if(_pNWFManager->GetSectorIdx(TmpPos) != -1)	_pNeighborSecMap[3] = &_pNWFManager->m_pSectors[_pNWFManager->GetSectorIdx(TmpPos)];

	// SE
	CNtlMath::MathRwV3dAssign(&TmpPos, _Pos.x - dGET_WORLD_PARAM()->WorldSectorSize, _Pos.y, _Pos.z - dGET_WORLD_PARAM()->WorldSectorSize);
	if(_pNWFManager->GetSectorIdx(TmpPos) != -1)	_pNeighborSecMap[4] = &_pNWFManager->m_pSectors[_pNWFManager->GetSectorIdx(TmpPos)];

	// S
	CNtlMath::MathRwV3dAssign(&TmpPos, _Pos.x, _Pos.y, _Pos.z - dGET_WORLD_PARAM()->WorldSectorSize);
	if(_pNWFManager->GetSectorIdx(TmpPos) != -1)	_pNeighborSecMap[5] = &_pNWFManager->m_pSectors[_pNWFManager->GetSectorIdx(TmpPos)];

	// SW
	CNtlMath::MathRwV3dAssign(&TmpPos, _Pos.x + dGET_WORLD_PARAM()->WorldSectorSize, _Pos.y, _Pos.z - dGET_WORLD_PARAM()->WorldSectorSize);
	if(_pNWFManager->GetSectorIdx(TmpPos) != -1)	_pNeighborSecMap[6] = &_pNWFManager->m_pSectors[_pNWFManager->GetSectorIdx(TmpPos)];

	// W
	CNtlMath::MathRwV3dAssign(&TmpPos, _Pos.x + dGET_WORLD_PARAM()->WorldSectorSize, _Pos.y, _Pos.z);
	if(_pNWFManager->GetSectorIdx(TmpPos) != -1)	_pNeighborSecMap[7] = &_pNWFManager->m_pSectors[_pNWFManager->GetSectorIdx(TmpPos)];

	// NW
	CNtlMath::MathRwV3dAssign(&TmpPos, _Pos.x + dGET_WORLD_PARAM()->WorldSectorSize, _Pos.y, _Pos.z + dGET_WORLD_PARAM()->WorldSectorSize);
	if(_pNWFManager->GetSectorIdx(TmpPos) != -1)	_pNeighborSecMap[8] = &_pNWFManager->m_pSectors[_pNWFManager->GetSectorIdx(TmpPos)];
}

// shadow light call back related
vector<RwUInt32>	g_vecAtomicFlagsBK;
RwUInt32			g_vecAtomicFlagsBKCnt;
vector<RwRGBAReal>	g_vecLightColorBK;
RwUInt32			g_vecLightColorBKCnt;

static void
ShadowPropRenderCallback(RwResEntry *repEntry, void *object, RwUInt8 type, RwUInt32 flags)
{
	RxD3D9ResEntryHeader*	pResEntryHeader;
	RxD3D9InstanceData*		pInstancedData;
	RwInt32					numMeshes;

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
		// RenderState & TextureState Setting ( WorldShadowTextrueMatrix )
		BeginShadowPropState();
		do
		{
			// Vertex shader
			RwD3D9SetVertexShader(pInstancedData->vertexShader);

			sRpNtlWorldSectorSplat	*pNtlWorldSectorSplat	= RpNtlWorldGetSectorSplat(pInstancedData->material);
			CNtlWorldSector			*pNtlWorldSector		= pNtlWorldSectorSplat->pNtlWorldSector;

			RwD3D9SetTexture(pNtlWorldSector->m_pNtlWorldShadow->m_pTexShadow, 0);
			RwD3D9SetTransform(D3DTS_TEXTURE0, &pNtlWorldSector->m_pNtlWorldShadow->m_matTex);

			RwD3D9SetStreamSource(0, (IDirect3DVertexBuffer9*)pResEntryHeader->vertexStream->vertexBuffer, 0, pResEntryHeader->vertexStream->stride);
			RwD3D9SetIndices((IDirect3DIndexBuffer9*)pResEntryHeader->indexBuffer);
			RwD3D9DrawIndexedPrimitive((D3DPRIMITIVETYPE)pResEntryHeader->primType,
				pInstancedData->baseIndex,
				0,
				pInstancedData->numVertices,
				pInstancedData->startIndex,
				pInstancedData->numPrimitives);

			// RenderState & TextureState Setting
		} while(++pInstancedData, --numMeshes); // Move onto the next pInstancedData
		EndShadowPropState();
	}
}

static void
WhiteRenderCallback(RwResEntry *repEntry, void *object, RwUInt8 type, RwUInt32 flags)
{
	RxD3D9ResEntryHeader*	pResEntryHeader;
	RxD3D9InstanceData*		pInstancedData;
	RwInt32					numMeshes;

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
		// RenderState & TextureState Setting ( WorldShadowTextrueMatrix )
		RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, 0xffffffff);

		RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG2);
		RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
		RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

		RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG2);
		RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
		RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

		do
		{
			// Vertex shader
			RwD3D9SetVertexShader(pInstancedData->vertexShader);

			sRpNtlWorldSectorSplat	*pNtlWorldSectorSplat	= RpNtlWorldGetSectorSplat(pInstancedData->material);
			CNtlWorldSector			*pNtlWorldSector		= pNtlWorldSectorSplat->pNtlWorldSector;

			RwD3D9SetTexture(pNtlWorldSector->m_pNtlWorldShadow->m_pTexShadow, 0);
			RwD3D9SetTransform(D3DTS_TEXTURE0, &pNtlWorldSector->m_pNtlWorldShadow->m_matTex);

			RwD3D9SetStreamSource(0, (IDirect3DVertexBuffer9*)pResEntryHeader->vertexStream->vertexBuffer, 0, pResEntryHeader->vertexStream->stride);
			RwD3D9SetIndices((IDirect3DIndexBuffer9*)pResEntryHeader->indexBuffer);
			RwD3D9DrawIndexedPrimitive((D3DPRIMITIVETYPE)pResEntryHeader->primType,
				pInstancedData->baseIndex,
				0,
				pInstancedData->numVertices,
				pInstancedData->startIndex,
				pInstancedData->numPrimitives);

			// RenderState & TextureState Setting
		} while(++pInstancedData, --numMeshes); // Move onto the next pInstancedData

		RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
		RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);

		RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
		RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	}
}

static RpAtomic *
RemoveAtomicFlags(RpAtomic *atomic, void *data __RWUNUSED__)
{
	RpGeometry *geometry;
	RwUInt32 flags;

	geometry	= RpAtomicGetGeometry(atomic);
	flags		= RpGeometryGetFlags(geometry);

	g_vecAtomicFlagsBK.push_back(flags);
	flags &= ~(*(static_cast<RwUInt32*>(data))/*rxGEOMETRY_LIGHT | rxGEOMETRY_MODULATE | rxGEOMETRY_PRELIT*/);
	RpGeometrySetFlags(geometry, flags);

	return atomic;
}

static RpAtomic *
RestoreAtomicFlags(RpAtomic *atomic, void *data __RWUNUSED__)
{
	RpGeometry *geometry;

	geometry = RpAtomicGetGeometry(atomic);
	RpGeometrySetFlags(geometry, g_vecAtomicFlagsBK[g_vecAtomicFlagsBKCnt++]);

	return atomic;
}

static RpLight* 
WhiteLightCallback(RpLight *light, void *data __RWUNUSED__)
{
	RwRGBAReal color;
	color = *RpLightGetColor(light);

	g_vecLightColorBK.push_back(color);

	color.red	= 1.0f;
	color.green	= 1.0f;
	color.blue	= 1.0f;
	color.alpha	= 1.0f;

	RpLightSetColor(light, &color);

	return light;
}

static RpLight* 
RestoreLightCallback(RpLight *light, void *data __RWUNUSED__)
{
	RpLightSetColor(light, &g_vecLightColorBK.at(g_vecLightColorBKCnt++));

	return light;
}

#include "NtlPLMaterialExtWrapper.h"
void CNtlWorldShadowManager::RenderAtR2S(RwV3d& _Pos, sNTL_WORLD_SHADOW_PARAM& _NtlWorldShadowParam, CNtlWorldFieldManager* _pNWFManager, CNtlPLObject* _pDoodadPSObj/* = NULL*/)
{
	CNtlWorldSector*	pNeighborSecMap[9];
	RwInt32				DoodadsNum;
	CNtlPLObject*		pNPObj;


	GetNeighborSecMap(_Pos, _pNWFManager, pNeighborSecMap);

	RwRGBA		bgColor = {255, 255, 255, 0};
	RwUInt32	removeFlags = (rxGEOMETRY_LIGHT | rxGEOMETRY_MODULATE | rxGEOMETRY_PRELIT);
	if (RwCameraClear(m_pCam, &bgColor, rwCAMERACLEARZ | rwCAMERACLEARIMAGE))
	{
		if(RwCameraBeginUpdate(m_pCam))
		{
			/*if (pNeighborSecMap[0])
			{
			if (pNeighborSecMap[0]->m_pAtomic)
			{
			pNeighborSecMap[0]->m_pNtlWorldShadow->Update();
			RxD3D9AllInOneSetLightingCallBack(g_pNodeWorld, NULL);
			RxD3D9AllInOneSetRenderCallBack(g_pNodeWorld, WhiteRenderCallback);
			RpAtomicRender(pNeighborSecMap[0]->m_pAtomic);
			}
			}*/

			for(RwInt32 i = 0; i < 9; ++i)
			{
				if(pNeighborSecMap[i])
				{
					DoodadsNum = pNeighborSecMap[i]->m_vecNtlPLEntityOrnament.size();
					for(int j = 0; j < DoodadsNum; ++j)
					{
						pNPObj = static_cast<CNtlPLObject*>(pNeighborSecMap[i]->m_vecNtlPLEntityOrnament[j]);

						g_vecAtomicFlagsBK.clear();
						g_vecAtomicFlagsBKCnt = 0;

						if(pNPObj)
						{
							if(pNPObj != _pDoodadPSObj)
							{
								RpClumpForAllAtomics(pNPObj->GetClump(), RemoveAtomicFlags, &removeFlags);
								{
									RwUInt32 iCullFlags = pNPObj->GetCullFlags();

									pNPObj->SetCullFlags(NULL);
									pNPObj->Render();
									pNPObj->SetCullFlags(iCullFlags);
								}
								RpClumpForAllAtomics(pNPObj->GetClump(), RestoreAtomicFlags, NULL);
							}
							/*else
							{
							RpClumpForAllAtomics(pNPObj->GetClump(), RemoveAtomicFlags, NULL);
							{
							pNPObj->Render();
							}
							RpClumpForAllAtomics(pNPObj->GetClump(), RestoreAtomicFlags, NULL);
							}
							*/
						}
					}
				}
				else
				{
					continue;
				}
			}

			RwCameraEndUpdate(m_pCam);
		}
	}
}

RwBool CNtlWorldShadowManager::RenderRasterFade(RwCamera *camera,
												RwRaster *raster,
												//RwReal shadowStrength,
												RwReal fadeDist,
												RwReal objectRadius,
												RwBool enableFading,
												RwRGBA& Color)
{
	RwUInt32 bottomCoef, topCoef;
	RwReal fbottomCoef, ftopCoef;
	RwReal shadowStrength = 0.4f;

	if(shadowStrength < 0.0f)
	{
		shadowStrength = -shadowStrength;
	}

	bottomCoef = RwFastRealToUInt32((1.0f - shadowStrength) * 255.0f);

	if(enableFading)
	{
		RwInt32 fadeCoef = (RwInt32)(255.0f - 96.0f * (fadeDist / objectRadius));

		if(fadeCoef < 0)
		{
			fadeCoef = 0;
		}

		bottomCoef += (RwUInt32)fadeCoef;

		if(bottomCoef > 255)
		{
			bottomCoef = 255;
		}

		topCoef = ((255 + bottomCoef) / 2);
		topCoef += (RwUInt32)fadeCoef;

		if(topCoef > 255)
		{
			topCoef = 255;
		}
	}
	else
	{
		topCoef = bottomCoef;
	}

	ftopCoef	= static_cast<RwReal>(topCoef) / 255.0f;
	fbottomCoef	= static_cast<RwReal>(bottomCoef) / 255.0f;

	RwCameraSetRaster(camera, raster);

	if(RwCameraBeginUpdate(camera))
	{
		RwIm2DVertex        vx[4];
		RwReal              crw, crh;
		RwRaster           *camRas;
		RwReal              recipZ;

		camRas	= RwCameraGetRaster(camera);
		crw		= (RwReal) RwRasterGetWidth(camRas);
		crh		= (RwReal) RwRasterGetHeight(camRas);
		recipZ	= 1.0f / RwCameraGetNearClipPlane(camera);

		// the alpha value should 255, D3DTSS_TCI_CAMERASPACEPOSITION can get white color for a key color
		RwIm2DVertexSetScreenX(&vx[0], 0.0f);
		RwIm2DVertexSetScreenY(&vx[0], 0.0f);
		RwIm2DVertexSetScreenZ(&vx[0], RwIm2DGetNearScreenZ());
		RwIm2DVertexSetRecipCameraZ(&vx[0], recipZ);
		RwIm2DVertexSetIntRGBA(&vx[0], Color.red, Color.green, Color.blue, 255);

		RwIm2DVertexSetScreenX(&vx[1], 0.0f);
		RwIm2DVertexSetScreenY(&vx[1], crh);
		RwIm2DVertexSetScreenZ(&vx[1], RwIm2DGetNearScreenZ());
		RwIm2DVertexSetRecipCameraZ(&vx[1], recipZ);
		RwIm2DVertexSetIntRGBA(&vx[1], Color.red, Color.green, Color.blue, 255);

		RwIm2DVertexSetScreenX(&vx[2], crw);
		RwIm2DVertexSetScreenY(&vx[2], 0.0f);
		RwIm2DVertexSetScreenZ(&vx[2], RwIm2DGetNearScreenZ());
		RwIm2DVertexSetRecipCameraZ(&vx[2], recipZ);
		RwIm2DVertexSetIntRGBA(&vx[2], Color.red, Color.green, Color.blue, 255);

		RwIm2DVertexSetScreenX(&vx[3], crw);
		RwIm2DVertexSetScreenY(&vx[3], crh);
		RwIm2DVertexSetScreenZ(&vx[3], RwIm2DGetNearScreenZ());
		RwIm2DVertexSetRecipCameraZ(&vx[3], recipZ);
		RwIm2DVertexSetIntRGBA(&vx[3], Color.red, Color.green, Color.blue, 255);


		RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
		RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void*)NULL);
		RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)FALSE);
		RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);
		RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTION, (void*)rwALPHATESTFUNCTIONALWAYS);
		RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDINVDESTCOLOR);
		RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDONE);

		RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, vx, 4);

		RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
		RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);
		RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTION, (void*)rwALPHATESTFUNCTIONGREATER);
		RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
		RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);

		RwCameraEndUpdate(camera);
	}

	RwCameraSetRaster(camera, NULL);

	return TRUE;
}

RwBool CNtlWorldShadowManager::RenderIm2DRenderQuad(RwReal x1, RwReal y1, RwReal x2, RwReal y2, RwReal z, RwReal recipCamZ, RwReal uvOffset)
{
	RwIm2DVertex	vx[4];

	//  Render an opaque white 2D quad at the given coordinates and
	//  spanning a whole texture.

	RwIm2DVertexSetScreenX(&vx[0], x1);
	RwIm2DVertexSetScreenY(&vx[0], y1);
	RwIm2DVertexSetScreenZ(&vx[0], z);
	RwIm2DVertexSetIntRGBA(&vx[0], 255, 255, 255, 255);
	RwIm2DVertexSetRecipCameraZ(&vx[0], recipCamZ);
	RwIm2DVertexSetU(&vx[0], uvOffset, recipCamZ);
	RwIm2DVertexSetV(&vx[0], uvOffset, recipCamZ);

	RwIm2DVertexSetScreenX(&vx[1], x1);
	RwIm2DVertexSetScreenY(&vx[1], y2);
	RwIm2DVertexSetScreenZ(&vx[1], z);
	RwIm2DVertexSetIntRGBA(&vx[1], 255, 255, 255, 255);
	RwIm2DVertexSetRecipCameraZ(&vx[1], recipCamZ);
	RwIm2DVertexSetU(&vx[1], uvOffset, recipCamZ);
	RwIm2DVertexSetV(&vx[1], 1.0f + uvOffset, recipCamZ);

	RwIm2DVertexSetScreenX(&vx[2], x2);
	RwIm2DVertexSetScreenY(&vx[2], y1);
	RwIm2DVertexSetScreenZ(&vx[2], z);
	RwIm2DVertexSetIntRGBA(&vx[2], 255, 255, 255, 255);
	RwIm2DVertexSetRecipCameraZ(&vx[2], recipCamZ);
	RwIm2DVertexSetU(&vx[2], 1.0f + uvOffset, recipCamZ);
	RwIm2DVertexSetV(&vx[2], uvOffset, recipCamZ);

	RwIm2DVertexSetScreenX(&vx[3], x2);
	RwIm2DVertexSetScreenY(&vx[3], y2);
	RwIm2DVertexSetScreenZ(&vx[3], z);
	RwIm2DVertexSetIntRGBA(&vx[3], 255, 255, 255, 255);
	RwIm2DVertexSetRecipCameraZ(&vx[3], recipCamZ);
	RwIm2DVertexSetU(&vx[3], 1.0f + uvOffset, recipCamZ);
	RwIm2DVertexSetV(&vx[3], 1.0f + uvOffset, recipCamZ);

	RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, vx, 4);

	return TRUE;
}

RwBool CNtlWorldShadowManager::CreateAA(RwRaster* pDstRaster, RwRaster* pSrcRaster, RwCamera* pCam)
{
	RwReal	size;
	RwReal  uvOffset;
	RwReal  recipCamZ;

	// This resamples the pSrcRaster into the pDstRaster which should
	// be half the size in both dimensions, using the pSrcRaster as a
	// texture and the pDstRaster as a rendering target. Bilinear filtering
	// achieves an anti-alias effect by averaging four pixels down into one.
	// A 'dummy' camera is required for the rendering which should
	// have a z-raster of the same size as the destination raster.

	size		= (RwReal) RwRasterGetWidth(pDstRaster);
	uvOffset	= dTEXEL_OFFSET / size;
	recipCamZ	= 1.0f / RwCameraGetFarClipPlane(pCam);

	RwCameraSetRaster(pCam, pDstRaster);
	if(RwCameraBeginUpdate(pCam))
	{
		RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*) rwBLENDONE);
		RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*) rwBLENDZERO);
		RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*) FALSE);
		RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*) FALSE);
		RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*) rwFILTERLINEAR);
		RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void *) pSrcRaster);

		RenderIm2DRenderQuad(0.0f, 0.0f, size, size, RwIm2DGetFarScreenZ(), recipCamZ, uvOffset);

		RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*) TRUE);
		RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*) TRUE);
		RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*) rwBLENDSRCALPHA);
		RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*) rwBLENDINVSRCALPHA);

		RwCameraEndUpdate(pCam);
	}

	RwCameraSetRaster(pCam, NULL);

	return TRUE;
}

RwBool CNtlWorldShadowManager::CreateShadowRasterBlur(RwRaster * shadowRaster, RwRaster * tempRaster, RwCamera * camera, RwUInt32 numPass)
{
	RwUInt32	iPass;
	RwReal      size;
	RwReal      uvStep;
	RwReal      recipCamZ;

	/*
	*  Blur the image in shadowRaster using tempRaster as a temporary
	*  buffer (both should be the same size and created with 
	*  ShadowRasterCreate). A 2D immediate mode rendering method is used
	*  which requires a 'dummy' camera to work with. This should have an
	*  appropriately sized Z-raster.
	*
	*  The technique used is one that will work on PS2 where methods of
	*  blurring the alpha channel are limited, but will also work on other
	*  platforms where color channels are used.
	*
	*  It works by effectively blitting one raster into another but with
	*  half a pixel offset in U and V so that bilinear filtering causes
	*  each pixel to be an average of four from the source. This is
	*  repeated with the opposite UV offset to increase the blur and
	*  leave the texture with no net displacement. The overall effect is
	*  equivalent to applying a 3x3 convolution matrix of
	*
	*      1 2 1
	*      2 4 2
	*      1 2 1
	*/

	size = (RwReal) RwRasterGetWidth(shadowRaster);
	uvStep = 1.0f / size;
	recipCamZ = 1.0f / RwCameraGetFarClipPlane(camera);

	for(iPass = 0; iPass < numPass; iPass++)
	{
		RwCameraSetRaster(camera, tempRaster);
		if(RwCameraBeginUpdate(camera))
		{
			RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDONE);
			RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDZERO);
			RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)FALSE);
			RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);
			RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERLINEAR);
			RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void*)shadowRaster);

			RenderIm2DRenderQuad(0.0f, 0.0f, size, size, RwIm2DGetFarScreenZ(), recipCamZ, (dTEXEL_OFFSET + 0.5f) * uvStep);

			RwCameraEndUpdate(camera);
		}

		RwCameraSetRaster(camera, shadowRaster);
		if(RwCameraBeginUpdate(camera))
		{
			RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void*) tempRaster);

			RenderIm2DRenderQuad(0.0f, 0.0f, size, size, RwIm2DGetFarScreenZ(), recipCamZ, (dTEXEL_OFFSET - 0.5f) * uvStep);

			RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
			RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);
			RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*) rwBLENDSRCALPHA);
			RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*) rwBLENDINVSRCALPHA);

			RwCameraEndUpdate(camera);
		}
	}

	RwCameraSetRaster(camera, NULL);

	return TRUE;
}

RwBool CNtlWorldShadowManager::IsCollidedInTex()
{
	POINT SPos, EPos;
	SPos.x = dGET_MOUSE()->GetCusPos().x;
	SPos.y = dGET_MOUSE()->GetCusPos().y;
	EPos.x = dGET_MOUSE()->GetCusPos().x + static_cast<LONG>(m_BrushRadius / 2.0f);
	EPos.y = dGET_MOUSE()->GetCusPos().y + static_cast<LONG>(m_BrushRadius / 2.0f);

	if(SPos.x >= 0 && SPos.x <= 512 && EPos.x >= 0 && EPos.x <= 512 &&
		SPos.y >= 128 && SPos.y <= (512 + 128) && EPos.y >= 128 && EPos.y <= (512 + 128))
	{
		return TRUE;
	}

	return FALSE;
}

void CNtlWorldShadowManager::UpdateSectorPSTex(std::vector<CNtlPLEntity*>& vecSectors, CNtlWorldFieldManager* _pNWFManager)
{
	if(vecSectors.size() != 1 || !CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_PSTEX))
	{
 		return;
	}
	
	if(dOKEY(DIK_LBRACKET))
	{
		m_BrushRadius--;
		CLAMP(m_BrushRadius, 0, 30);
	}

	if(dOKEY(DIK_RBRACKET))
	{
		m_BrushRadius++;
		CLAMP(m_BrushRadius, 0, 30);
	}

	CNtlPLObject*		pNtlPLObj		= static_cast<CNtlPLObject*>(vecSectors[0]);
	RwV3d				vPosSectorMid	= pNtlPLObj->GetPosition();
	RwInt32				iCurSecIdx		= _pNWFManager->GetSectorIdx(vPosSectorMid);
	CNtlWorldSector*	pNWSec			= &_pNWFManager->m_pSectors[iCurSecIdx];
	CNtlWorldShadow*	pNtlWorldShadow	= pNWSec->m_pNtlWorldShadow;
	RwTexture*			pTex2Edit		= pNtlWorldShadow->m_pTexShadow;
	RwInt32				iTexSize		= RwRasterGetWidth(pTex2Edit->raster);
	
	if (!pTex2Edit)
	{
		return;
	}

	POINT				SPos, EPos, MPos, RPos;

	// Brush를 정확하게 표현할 방법이 없다. 현재는 텍셀 4개를 지우고 나중에 UI를 다시 구성하자.
	RwReal fRadius = 3.0f;
	if (IsCollidedInTex())
	{
		fRadius = m_BrushRadius;
		SPos.x	= dGET_MOUSE()->GetCusPos().x - static_cast<LONG>(m_BrushRadius);
		SPos.y	= dGET_MOUSE()->GetCusPos().y - static_cast<LONG>(m_BrushRadius) - 128;
		EPos.x	= dGET_MOUSE()->GetCusPos().x + static_cast<LONG>(m_BrushRadius);
		EPos.y	= dGET_MOUSE()->GetCusPos().y + static_cast<LONG>(m_BrushRadius);
		MPos.x	= SPos.x + (LONG)m_BrushRadius;
		MPos.y	= SPos.y + (LONG)m_BrushRadius;		
	}
	else
	{
		RwMatrix    matLight;
		RwV3d       vScaling;
		RwV3d		vTranslation;
		RwMatrixInvert(&matLight, &pNtlWorldShadow->m_matLight);

		vScaling.x = vScaling.y = -0.5f / pNtlWorldShadow->m_CamViewWindowX;
		vScaling.z = 1.0f / (pNtlWorldShadow->m_CamViewWindowX + pNtlWorldShadow->m_CamViewWindowX);
		RwMatrixScale(&matLight, &vScaling, rwCOMBINEPOSTCONCAT);

		vTranslation.x = vTranslation.y = 0.5f; vTranslation.z = 0.0f;
		RwMatrixTranslate(&matLight, &vTranslation, rwCOMBINEPOSTCONCAT);

		RwV3d vTemp;
		RwV3dTransformPoint(&vTemp, &dGET_COLLISION_INFO()->RayIntersectionPt4Terrain, &matLight);

		MPos.x = static_cast<RwInt32>(vTemp.x * 512);
		MPos.y = static_cast<RwInt32>(vTemp.y * 512);
		SPos.x = static_cast<RwInt32>(MPos.x - fRadius);
		SPos.y = static_cast<RwInt32>(MPos.y - fRadius);
		EPos.x = static_cast<RwInt32>(MPos.x + fRadius);
		EPos.y = static_cast<RwInt32>(MPos.y + fRadius);
	}

	if(dGET_MOUSE()->GetLBOnce())
	{	
		vector<POINT> vecPointBuffer;
		for (LONG i = SPos.y; i <= EPos.y; ++i)
		{
			if (i <= 0 || i > 512)
			{
				continue;
			}

			for(LONG j = SPos.x; j <= EPos.x; ++j)
			{
				if (j <= 0 || j > 512)
				{
					continue;
				}

				float Distance = sqrtf((float)((j - MPos.x) * (j - MPos.x)) + (float)((i - MPos.y) * (i - MPos.y)));
				if(Distance > fRadius)
				{
					continue;
				}

				RPos.x = (LONG)( ((float)j * (float)iTexSize) / 512.0f);
				RPos.y = (LONG)( ((float)i * (float)iTexSize) / 512.0f);

				RwBool bSame = FALSE;
				for(unsigned int k = 0; k < vecPointBuffer.size(); ++k)
				{
					if(vecPointBuffer[k].x == RPos.x && vecPointBuffer[k].y == RPos.y)
					{
						bSame = TRUE;
						break;
					}
				}

				if(!bSame)
				{
					vecPointBuffer.push_back(RPos);
				}
			}
		}
		for(unsigned i = 0; i < vecPointBuffer.size(); ++i)
		{
			GetNtlResourceManager()->SetColor(pTex2Edit, m_Opacity, vecPointBuffer[i]);
		}

		vecPointBuffer.clear();
	}

	// update cursor
	POINT CurPos;
	CurPos.x = dGET_MOUSE()->GetCusPos().x;
	CurPos.y = dGET_MOUSE()->GetCusPos().y;

	RwReal recipCameraZ = 1.0f / RwCameraGetNearClipPlane(CNtlPLGlobal::m_RwCamera);

	RwIm2DVertexSetScreenX(&m_PSTexBrush[0], CurPos.x - m_BrushRadius - 0.5f);
	RwIm2DVertexSetScreenY(&m_PSTexBrush[0], CurPos.y - m_BrushRadius - 0.5f);
	RwIm2DVertexSetScreenZ(&m_PSTexBrush[0], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_PSTexBrush[0], recipCameraZ);
	RwIm2DVertexSetU(&m_PSTexBrush[0], 0, recipCameraZ);
	RwIm2DVertexSetV(&m_PSTexBrush[0], 0, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_PSTexBrush[0], 255, 255, 255, 222);

	RwIm2DVertexSetScreenX(&m_PSTexBrush[1], CurPos.x - m_BrushRadius - 0.5f);
	RwIm2DVertexSetScreenY(&m_PSTexBrush[1], CurPos.y + m_BrushRadius - 0.5f);
	RwIm2DVertexSetScreenZ(&m_PSTexBrush[1], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_PSTexBrush[1], recipCameraZ);
	RwIm2DVertexSetU(&m_PSTexBrush[1], 0, recipCameraZ);
	RwIm2DVertexSetV(&m_PSTexBrush[1], 1, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_PSTexBrush[1], 255, 255, 255, 222);

	RwIm2DVertexSetScreenX(&m_PSTexBrush[2], CurPos.x + m_BrushRadius - 0.5f);
	RwIm2DVertexSetScreenY(&m_PSTexBrush[2], CurPos.y - m_BrushRadius - 0.5f);
	RwIm2DVertexSetScreenZ(&m_PSTexBrush[2], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_PSTexBrush[2], recipCameraZ);
	RwIm2DVertexSetU(&m_PSTexBrush[2], 1, recipCameraZ);
	RwIm2DVertexSetV(&m_PSTexBrush[2], 0, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_PSTexBrush[2], 255, 255, 255, 222);

	RwIm2DVertexSetScreenX(&m_PSTexBrush[3], CurPos.x + m_BrushRadius - 0.5f);
	RwIm2DVertexSetScreenY(&m_PSTexBrush[3], CurPos.y + m_BrushRadius - 0.5f);
	RwIm2DVertexSetScreenZ(&m_PSTexBrush[3], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&m_PSTexBrush[3], recipCameraZ);
	RwIm2DVertexSetU(&m_PSTexBrush[3], 1, recipCameraZ);
	RwIm2DVertexSetV(&m_PSTexBrush[3], 1, recipCameraZ);
	RwIm2DVertexSetIntRGBA(&m_PSTexBrush[3], 255, 255, 255, 222);

	CreateUITex();
}
/*
VOID CNtlWorldShadowManager::CreateDoodadSPS(CNtlPLObject* _pNPObj, RwV3d& _Pos, sNTL_WORLD_SHADOW_PARAM& _NtlWorldShadowParam, CNtlWorldFieldManager* _pNWFManager)
{
ENTITY_ATOMIC_VEC*		_pEAV	= _pNPObj->GetAtomicList();
ENTITY_ATOMIC_VEC_ITER	_The	= _pEAV->begin();
while(_The != _pEAV->end())
{
RpAtomic*	pCurAtom	= (*_The)->pData;
RwInt32		TexSize		= static_cast<RwInt32>(RpNtlAtomicGetMultiTextureSize(pCurAtom));

if(!(RpNtlAtomicGetFlag(pCurAtom) & NTL_SHADOW_MAP))
{
_The++;
continue;
}

// prepare stuff
RwInt32 CamTexSize		= TexSize << 1;
RwInt32 IPCamTexSize	= TexSize;
CNtlWorldSector* pNWSec = &_pNWFManager->m_pSectors[_pNWFManager->GetSectorIdx(_Pos)];

// create camera textures
CreateShadowCamera(CamTexSize);
CreateShadowCameraIP(IPCamTexSize);

SetShadowCameraDir(pNWSec->m_pNtlWorldShadow->m_matLight);

// extend a frustum to include the atomic properly
SetShadowCameraFrustum(1.1f * pNWSec->m_pAtomic->boundingSphere.radius);

// position the shadow camera to the proper position
SetShadowCameraCenter(pNWSec->m_pAtomic->boundingSphere.center);

// add the shadow camera to the world
RpWorldAddCamera(CNtlPLGlobal::m_pRpWorld, m_pCam);

// main raster	
m_pShadowCameraRaster = RwRasterCreate(CamTexSize, CamTexSize, 0, rwRASTERTYPECAMERATEXTURE);
RwCameraSetRaster(m_pCam, m_pShadowCameraRaster);

// create a render2surface; camera textures
m_pR2S = RwTextureCreate(NULL);
RwTextureSetRaster(m_pR2S, m_pShadowCameraRaster);
RwTextureSetFilterMode(m_pR2S, rwFILTERLINEAR);
RwTextureSetAddressing(m_pR2S, rwTEXTUREADDRESSCLAMP);

// render all atomics at the sector including neighbors
RenderAtR2S(_Pos, _NtlWorldShadowParam, _pNWFManager, static_cast<CNtlPLObject*>(RpNtlAtomicGetData(pCurAtom)));

// image processing step
RwRaster* ShadowRenderRaster;

// 1. create rasters for processed images; anti-alias
m_pShadowCameraRasterAA = RwRasterCreate(IPCamTexSize, IPCamTexSize, 0, rwRASTERTYPECAMERATEXTURE);
CreateAA(m_pShadowCameraRasterAA, m_pShadowCameraRaster, m_pIPCam);
ShadowRenderRaster = m_pShadowCameraRasterAA;

// 2. blur
if(_NtlWorldShadowParam._BlurPass)
{
m_pShadowCameraRasterBlur = RwRasterCreate(IPCamTexSize, IPCamTexSize, 0, rwRASTERTYPECAMERATEXTURE);
CreateShadowRasterBlur(ShadowRenderRaster, m_pShadowCameraRasterBlur, m_pIPCam, _NtlWorldShadowParam._BlurPass);
}

// 3. fade effect
RenderRasterFade(m_pIPCam, ShadowRenderRaster, pNWSec->m_pAtomic->boundingSphere.radius, pNWSec->m_pAtomic->boundingSphere.radius, _NtlWorldShadowParam._Fade, _NtlWorldShadowParam._RGBA);
RwTextureSetRaster(m_pR2S, ShadowRenderRaster);

// clone sector's shadow maps from camera textures
RwTexture* pTex = CNtlPLResourceManager::GetInstance()->CreateTexture(Helper_AtomicName(pCurAtom), TexSize, TexSize, dNTL_SPLAT_ALPHA_SIZE, rwRASTERFORMAT8888);
RwTextureSetAddressing(pTex, rwTEXTUREADDRESSCLAMP);
RwTextureSetFilterMode(pTex, rwFILTERLINEAR);
CNtlPLResourceManager::GetInstance()->CopyRaster(m_pR2S, pTex);
RpNtlAtomicSetMultiTexture(pCurAtom, pTex);

#ifdef dNTL_WORLD_TOOL_MODE

pNWSec->m_pNtlWorldShadow->UpdateSParam4Doodads(_NtlWorldShadowParam);

#endif
// delete all shadow objects
DeleteShadowObj();

_The++;
}
}
*/
VOID CNtlWorldShadowManager::CreateDoodadSPS(CNtlPLObject* _pNPObj, RwV3d& _Pos, sNTL_WORLD_SHADOW_PARAM& _NtlWorldShadowParam, CNtlWorldFieldManager* _pNWFManager)
{
	ENTITY_ATOMIC_VEC*		_pEAV	= _pNPObj->GetAtomicList();
	ENTITY_ATOMIC_VEC_ITER	_The	= _pEAV->begin();
	while(_The != _pEAV->end())
	{
		RpAtomic*			pCurAtom	= (*_The)->pData;
		RwInt32				TexSize		= static_cast<RwInt32>(RpNtlAtomicGetUserDatU16(pCurAtom));
		CNtlWorldShadow*	pPSUser		= static_cast<CNtlWorldShadow*>(RpNtlAtomicGetUserDat(pCurAtom));
		RwReal				FScaleUser	= static_cast<RwReal>(RpNtlAtomicGetUserDatReal(pCurAtom));

		if(!(RpNtlAtomicGetFlag(pCurAtom) & NTL_SHADOW_MAP))
		{
			_The++;
			continue;
		}

		// destroy the user data and recreate if exists
		if(pPSUser)
		{
			NTL_DELETE(pPSUser);
			RpNtlAtomicSetUserDat(pCurAtom, NULL);
		}

		pPSUser = NTL_NEW CNtlWorldShadow;

		// prepare stuff
		RwInt32 CamTexSize		= TexSize << 1;
		RwInt32 IPCamTexSize	= TexSize;
		CNtlWorldSector* pNWSec = &_pNWFManager->m_pSectors[_pNWFManager->GetSectorIdx(_Pos)];

		// create camera textures
		CreateShadowCamera(CamTexSize);
		CreateShadowCameraIP(IPCamTexSize);

		// create light matrices
		pPSUser->Create(_NtlWorldShadowParam._LightDir);

		SetShadowCameraDir(pPSUser->m_matLight);

		// extend a frustum to include the atomic properly
		DBO_TRACE(FScaleUser > 0.0f, "mest be positive.");

		SetShadowCameraFrustum(1.1f * pCurAtom->worldBoundingSphere.radius * FScaleUser);

		// position the shadow camera to the proper position
		SetShadowCameraCenter(pCurAtom->worldBoundingSphere.center);

		// add the shadow camera to the world
		RpWorldAddCamera(CNtlPLGlobal::m_pRpWorld, m_pCam);

		// main raster	
		m_pShadowCameraRaster = RwRasterCreate(CamTexSize, CamTexSize, 0, rwRASTERTYPECAMERATEXTURE);
		RwCameraSetRaster(m_pCam, m_pShadowCameraRaster);

		// create a render2surface; camera textures
		m_pR2S = RwTextureCreate(NULL);
		DBO_ASSERT(m_pR2S, "Texture create failed.");

		RwTextureSetRaster(m_pR2S, m_pShadowCameraRaster);
		RwTextureSetFilterMode(m_pR2S, rwFILTERLINEAR);
		RwTextureSetAddressing(m_pR2S, rwTEXTUREADDRESSCLAMP);

		// render all atomics at the sector including neighbors
		RenderAtR2S(_Pos, _NtlWorldShadowParam, _pNWFManager, static_cast<CNtlPLObject*>(RpNtlAtomicGetData(pCurAtom)));

		// image processing step
		RwRaster* ShadowRenderRaster;

		// 1. create rasters for processed images; anti-alias
		m_pShadowCameraRasterAA = RwRasterCreate(IPCamTexSize, IPCamTexSize, 0, rwRASTERTYPECAMERATEXTURE);
		CreateAA(m_pShadowCameraRasterAA, m_pShadowCameraRaster, m_pIPCam);
		ShadowRenderRaster = m_pShadowCameraRasterAA;

		// 2. blur
		if(_NtlWorldShadowParam._BlurPass)
		{
			m_pShadowCameraRasterBlur = RwRasterCreate(IPCamTexSize, IPCamTexSize, 0, rwRASTERTYPECAMERATEXTURE);
			CreateShadowRasterBlur(ShadowRenderRaster, m_pShadowCameraRasterBlur, m_pIPCam, _NtlWorldShadowParam._BlurPass);
		}

		// 3. fade effect
		RenderRasterFade(m_pIPCam, ShadowRenderRaster, pCurAtom->worldBoundingSphere.radius, pCurAtom->worldBoundingSphere.radius, _NtlWorldShadowParam._Fade, _NtlWorldShadowParam._RGBA);
		RwTextureSetRaster(m_pR2S, ShadowRenderRaster);

		// clone sector's shadow maps from camera textures
		pPSUser->m_pTexShadow = CNtlPLResourceManager::GetInstance()->CreateTexture(Helper_AtomicName(pCurAtom), TexSize, TexSize, dNTL_SPLAT_ALPHA_SIZE, rwRASTERFORMAT8888);
		RwTextureSetAddressing(pPSUser->m_pTexShadow, rwTEXTUREADDRESSCLAMP);
		RwTextureSetFilterMode(pPSUser->m_pTexShadow, rwFILTERLINEAR);
		CNtlPLResourceManager::GetInstance()->CopyRaster(m_pR2S, pPSUser->m_pTexShadow);

		// camera view window x; radius
		pPSUser->m_CamViewWindowX = RwCameraGetViewWindow(m_pCam)->x;

		// light matrix
		RwMatrixCopy(&pPSUser->m_matLight, RwFrameGetMatrix(RwCameraGetFrame(m_pCam)));

		RpNtlAtomicSetUserDat(pCurAtom, pPSUser);

#ifdef dNTL_WORLD_TOOL_MODE

		pNWSec->m_pNtlWorldShadow->UpdateSParam4Doodads(_NtlWorldShadowParam);

#endif
		// delete all shadow objects
		DeleteShadowObj();

		_The++;
	}
}

RwBool CNtlWorldShadowManager::CreateDoodadShadow(RwV3d& _Pos, sNTL_WORLD_SHADOW_PARAM& _NtlWorldShadowParam, CNtlWorldFieldManager* _pNWFManager)
{
	CNtlWorldSector*	pNeighborSecMap[9];
	CNtlPLObject*		pNPObj;

	// get the doodads which's got shadow info.
	GetNeighborSecMap(_Pos, _pNWFManager, pNeighborSecMap);

	if(pNeighborSecMap[0])
	{
		m_vecShadowDoodads.clear();

		for(RwUInt32 j = 0; j < pNeighborSecMap[0]->m_vecNtlPLEntityOrnament.size(); ++j)
		{
			pNPObj = static_cast<CNtlPLObject*>(pNeighborSecMap[0]->m_vecNtlPLEntityOrnament[j]);
			if(pNPObj->GetEnableGenShadowMap())
			{
				m_vecShadowDoodads.push_back(pNPObj);
			}
		}
	}
	else
	{
		return TRUE;
	}

	// create shadow for each doodad which's got a shoadow info
	for(RwUInt32 i = 0; i < m_vecShadowDoodads.size(); ++i)
	{
		RwV3d Pos = m_vecShadowDoodads[i]->GetPosition();
		CreateDoodadSPS(m_vecShadowDoodads[i], Pos, _NtlWorldShadowParam, _pNWFManager);
	}

	return TRUE;
}

RwBool CNtlWorldShadowManager::DeleteDoodadShadow(RwV3d& _Pos, sNTL_WORLD_SHADOW_PARAM& _NtlWorldShadowParam, CNtlWorldFieldManager* _pNWFManager)
{
	RwInt32				SectorIdx	= _pNWFManager->GetSectorIdx(_Pos);
	CNtlWorldSector*	pCurSector	= &_pNWFManager->m_pSectors[SectorIdx];

	for(RwInt32 i = 0; i < static_cast<RwInt32>(pCurSector->m_vecNtlPLEntityOrnament.size()); ++i)
	{
		(static_cast<CNtlPLObject*>(pCurSector->m_vecNtlPLEntityOrnament[i]))->DeleteDoodadShadow();
	}

#ifdef dNTL_WORLD_TOOL_MODE

	NTL_DELETE(pCurSector->m_pNtlWorldShadow->m_pNtlWorldShadowParam4Doodads);

#endif

	return TRUE;
}

RwBool CNtlWorldShadowManager::Create(RwV3d& _Pos, sNTL_WORLD_SHADOW_PARAM& _NtlWorldShadowParam, CNtlWorldFieldManager* _pNWFManager)
{
	RwInt32 CamTexSize		= _NtlWorldShadowParam._TexSize << 1;
	RwInt32 IPCamTexSize	= _NtlWorldShadowParam._TexSize;
	CNtlWorldSector* pNWSec = &_pNWFManager->m_pSectors[_pNWFManager->GetSectorIdx(_Pos)];

	// create camera textures
	CreateShadowCamera(CamTexSize);
	CreateShadowCameraIP(IPCamTexSize);

	// create a shadow light matrix
	pNWSec->m_pNtlWorldShadow->Create(_NtlWorldShadowParam._LightDir);

	// update the shadow camera to make the same direction of the light
	SetShadowCameraDir(pNWSec->m_pNtlWorldShadow->m_matLight);

	// extend a frustum to include the atomic properly
	SetShadowCameraFrustum(1.1f * pNWSec->m_pAtomic->boundingSphere.radius * 1.0f);

	// position the shadow camera to the proper position
	SetShadowCameraCenter(pNWSec->m_pAtomic->boundingSphere.center);

	// add the shadow camera to the world
	RpWorldAddCamera(CNtlPLGlobal::m_pRpWorld, m_pCam);

	// main raster	
	m_pShadowCameraRaster = RwRasterCreate(CamTexSize, CamTexSize, 0, rwRASTERTYPECAMERATEXTURE);
	RwCameraSetRaster(m_pCam, m_pShadowCameraRaster);

	// create a render2surface; camera textures
	m_pR2S = RwTextureCreate(NULL);
	DBO_ASSERT(m_pR2S, "Texture create failed.");

	RwTextureSetRaster(m_pR2S, m_pShadowCameraRaster);
	RwTextureSetFilterMode(m_pR2S, rwFILTERLINEAR);
	RwTextureSetAddressing(m_pR2S, rwTEXTUREADDRESSCLAMP);

	// render all atomics at the sector including neighbors
	RenderAtR2S(_Pos, _NtlWorldShadowParam, _pNWFManager);

	// image processing step
	RwRaster* ShadowRenderRaster;

	// 1. create rasters for processed images; anti-alias
	m_pShadowCameraRasterAA = RwRasterCreate(IPCamTexSize, IPCamTexSize, 0, rwRASTERTYPECAMERATEXTURE);
	CreateAA(m_pShadowCameraRasterAA, m_pShadowCameraRaster, m_pIPCam);
	ShadowRenderRaster = m_pShadowCameraRasterAA;

	// 2. blur
	if(_NtlWorldShadowParam._BlurPass)
	{
		m_pShadowCameraRasterBlur = RwRasterCreate(IPCamTexSize, IPCamTexSize, 0, rwRASTERTYPECAMERATEXTURE);
		CreateShadowRasterBlur(ShadowRenderRaster, m_pShadowCameraRasterBlur, m_pIPCam, _NtlWorldShadowParam._BlurPass);
	}

	// 3. fade effect
	RenderRasterFade(m_pIPCam, ShadowRenderRaster, pNWSec->m_pAtomic->boundingSphere.radius * 1.0f, pNWSec->m_pAtomic->boundingSphere.radius * 1.0f, _NtlWorldShadowParam._Fade, _NtlWorldShadowParam._RGBA);
	RwTextureSetRaster(m_pR2S, ShadowRenderRaster);

	// clone sector's shadow maps from camera textures
	if(pNWSec->m_pNtlWorldShadow->m_pTexShadow)
	{
		RwTextureDestroy(pNWSec->m_pNtlWorldShadow->m_pTexShadow);
		pNWSec->m_pNtlWorldShadow->m_pTexShadow = NULL;
	}
	pNWSec->m_pNtlWorldShadow->m_pTexShadow = CNtlPLResourceManager::GetInstance()->CreateTexture(	"ShadowMap",
		IPCamTexSize,
		IPCamTexSize,
		32,
		rwRASTERFORMAT8888);

	RwTextureSetFilterMode(pNWSec->m_pNtlWorldShadow->m_pTexShadow, rwFILTERLINEAR);
	RwTextureSetAddressing(pNWSec->m_pNtlWorldShadow->m_pTexShadow, rwTEXTUREADDRESSCLAMP);

	CNtlPLResourceManager::GetInstance()->CopyRaster(m_pR2S, pNWSec->m_pNtlWorldShadow->m_pTexShadow);

	// camera view window x; radius
	pNWSec->m_pNtlWorldShadow->m_CamViewWindowX = RwCameraGetViewWindow(m_pCam)->x;

	// light matrix
	RwMatrixCopy(&pNWSec->m_pNtlWorldShadow->m_matLight, RwFrameGetMatrix(RwCameraGetFrame(m_pCam)));

#ifdef dNTL_WORLD_TOOL_MODE

	pNWSec->m_pNtlWorldShadow->UpdateSParam(_NtlWorldShadowParam);

#endif

	// delete all shadow objects
	DeleteShadowObj();

	return TRUE;
}

RwBool CNtlWorldShadowManager::Delete(RwV3d& _Pos, sNTL_WORLD_SHADOW_PARAM& _NtlWorldShadowParam, CNtlWorldFieldManager* _pNWFManager)
{
	CNtlWorldSector* pNWSec = &_pNWFManager->m_pSectors[_pNWFManager->GetSectorIdx(_Pos)];

	pNWSec->DeleteShadow();

	return TRUE;
}

RwBool CNtlWorldShadowManager::DeleteShadowObj()
{
	if(m_pCam)
	{
		RpWorldRemoveCamera(CNtlPLGlobal::m_pRpWorld, m_pCam);
		DeleteShadowCamera(m_pCam);
		m_pCam = NULL;
	}

	if(m_pIPCam)
	{
		DeleteShadowCamera(m_pIPCam);
		m_pIPCam = NULL;
	}

	if(m_pShadowCameraRaster)
	{
		RwRasterDestroy(m_pShadowCameraRaster);
		m_pShadowCameraRaster = NULL;
	}

	if(m_pShadowCameraRasterAA)
	{
		RwRasterDestroy(m_pShadowCameraRasterAA);
		m_pShadowCameraRasterAA = NULL;
	}

	if(m_pShadowCameraRasterBlur)
	{
		RwRasterDestroy(m_pShadowCameraRasterBlur);
		m_pShadowCameraRasterBlur = NULL;
	}

	if(m_pR2S)
	{
		RwTextureSetRaster(m_pR2S, NULL);
		RwTextureDestroy(m_pR2S);
		m_pR2S = NULL;
	}

	return TRUE;
}

void CNtlWorldShadowManager::RenderTexPanel(vector<CNtlPLEntity*>& vecSectors, CNtlWorldFieldManager* _pNWFManager)
{
	CNtlPLObject*	pNtlPLObj;
	RwV3d			PosSectorMid;
	RwInt32			CurSecIdx;

	pNtlPLObj		= static_cast<CNtlPLObject*>(vecSectors[0]);
	PosSectorMid	= pNtlPLObj->GetPosition();
	CurSecIdx		= _pNWFManager->GetSectorIdx(PosSectorMid);

	UpdateAlphaState(TRUE);

	RwD3D9SetTexture(_pNWFManager->m_pSectors[CurSecIdx].m_pNtlWorldShadow->m_pTexShadow, 0);
	RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_UITex, 4);

	RwD3D9SetTexture(m_pPSTexBrush, 0);
	RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_PSTexBrush, 4);

	UpdateAlphaState(FALSE);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RwBool CNtlWorldShadowManager::CreateShadowProp(RwV3d& _Pos, sNTL_WORLD_SHADOW_PARAM& _NtlWorldShadowParam, CNtlWorldFieldManager* _pNWFManager)
{
 	CNtlWorldSector* pNWSec = &_pNWFManager->m_pSectors[_pNWFManager->GetSectorIdx(_Pos)];
 
	if (!pNWSec->m_pNtlWorldShadow->m_pTexShadow)
	{
		return FALSE;
	}

	RwInt32				CamTexSize		= dGET_WORLD_PARAM()->WorldSectorSize;
	CNtlWorldShadow*	pWorldShadow	= pNWSec->m_pNtlWorldShadow;

	CreateShadowCamera(CamTexSize);

	RwFrame*	pfrmCam = RwCameraGetFrame(m_pCam);
	RwMatrix*	pmatCam = RwFrameGetMatrix(pfrmCam);
	CNtlMath::MathRwV3dAssign(RwMatrixGetAt(pmatCam), 0.0f, -1.0f, 0.0f);
	CNtlMath::MathRwV3dAssign(RwMatrixGetRight(pmatCam), 1.0f, 0.0f, 0.0f);
	CNtlMath::MathRwV3dAssign(RwMatrixGetUp(pmatCam), 0.0f, 0.0f, 1.0f);
	RwFrameUpdateObjects(pfrmCam);

	SetShadowCameraFrustum(static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize / 2));
	SetShadowCameraCenter(pNWSec->m_pAtomic->worldBoundingSphere.center);

	RpWorldAddCamera(CNtlPLGlobal::m_pRpWorld, m_pCam);
 
	m_pShadowCameraRaster = RwRasterCreate(CamTexSize, CamTexSize, 0, rwRASTERTYPECAMERATEXTURE);
	RwCameraSetRaster(m_pCam, m_pShadowCameraRaster);

	m_pR2S = RwTextureCreate(NULL);
	DBO_ASSERT(m_pR2S, "Texture create failed.");

	RwTextureSetRaster(m_pR2S, m_pShadowCameraRaster);
	RwTextureSetFilterMode(m_pR2S, rwFILTERLINEAR);
	RwTextureSetAddressing(m_pR2S, rwTEXTUREADDRESSCLAMP);

	RwCamera* pCameraBK = CNtlPLGlobal::m_RwCamera;
	CNtlPLGlobal::m_RwCamera = m_pCam;

	RwRGBA bgColor = {255, 255, 255, 0};
	if (RwCameraClear(m_pCam, &bgColor, rwCAMERACLEARZ | rwCAMERACLEARIMAGE))
	{
		if (RwCameraBeginUpdate(m_pCam))
		{	
			// Shadow Map Render : Start
			if (pNWSec->m_pAtomic)
			{
				pNWSec->m_pNtlWorldShadow->Update();
				RxD3D9AllInOneSetLightingCallBack(g_pNodeWorld, NULL);
				RxD3D9AllInOneSetRenderCallBack(g_pNodeWorld, ShadowPropRenderCallback);
				RpAtomicRender(pNWSec->m_pAtomic);
			}
			// ShadowMapEnd

			// Self Shadow Object : Start
			CNtlWorldSector*	pNeighborSecMap[9];
			RwUInt32			removeFlags = rxGEOMETRY_PRELIT | rxGEOMETRY_MODULATE | rxGEOMETRY_TEXTURED | rxGEOMETRY_TEXTURED2;
			GetNeighborSecMap(_Pos, _pNWFManager, pNeighborSecMap);

			for (int i = 0; i < 1; ++i)
			{
				if (pNeighborSecMap[i])
				{
					g_vecLightColorBK.clear();
					g_vecLightColorBKCnt = 0;

					// All Light In Sector -> Color White
					RpWorldSectorForAllLights(pNeighborSecMap[i]->m_pWorldSector, WhiteLightCallback, NULL);
					{
						for(int j = 0; j < (int)pNeighborSecMap[i]->m_vecNtlPLEntityOrnament.size(); ++j)
						{
							CNtlPLObject* pNPObj = static_cast<CNtlPLObject*>(pNeighborSecMap[i]->m_vecNtlPLEntityOrnament[j]);
							if (pNPObj->GetEnableGenShadowMap())
							{
								g_vecAtomicFlagsBK.clear();
								g_vecAtomicFlagsBKCnt = 0;

								// Geometry Flag Set : rxGEOMETRY_PRELIT | rxGEOMETRY_TEXTURED
								RpClumpForAllAtomics(pNPObj->GetClump(), RemoveAtomicFlags, &removeFlags);
								{
									RwUInt32 iCullFlags = pNPObj->GetCullFlags();

									pNPObj->SetCullFlags(NULL);
									pNPObj->Render();
									pNPObj->SetCullFlags(iCullFlags);
								}
								// Geometry Flag Restore
								RpClumpForAllAtomics(pNPObj->GetClump(), RestoreAtomicFlags, NULL);
							}
						}
					}
					// Restore Light Color
					RpWorldSectorForAllLights(pNeighborSecMap[i]->m_pWorldSector, RestoreLightCallback, NULL);
				}
			}
			// Self Shadow Object : End

			RwCameraEndUpdate(m_pCam);
		}
	}

	CNtlPLGlobal::m_RwCamera = pCameraBK;

	//m_pR2S -> SetShadow Property
	RwV3d vTilePos;
	vTilePos.x = pNWSec->DatumPoint.x + static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize / 2) - static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize / 2);
	vTilePos.y = pNWSec->DatumPoint.y;
	vTilePos.z = pNWSec->DatumPoint.z + static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize / 2) - static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize / 2);

	RwInt32 iIdx		= 0;
	RwV3d	vTempPos	= vTilePos;
	for (RwReal z = vTilePos.z; z > pNWSec->DatumPoint.z - static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize / 2); z -= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize))
	{
		vTempPos.z = z;
		for (RwReal x = vTilePos.x; x > pNWSec->DatumPoint.x - static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize / 2); x -= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize))
		{
			vTempPos.x = x;
			RwBool bIsShadow = CheckShadowFilter(m_pR2S, iIdx++, dGET_WORLD_PARAM()->WorldSectorTileSize, _NtlWorldShadowParam._RGBA, 0.75f);
			SetShadowProp(vTempPos, bIsShadow, _pNWFManager);
		}
	}

#ifdef dNTL_WORLD_TOOL_MODE

	pNWSec->m_pNtlWorldShadow->UpdateSParam(_NtlWorldShadowParam);

#endif

	DeleteShadowObj();
 	return TRUE;
}

RwBool CNtlWorldShadowManager::DeleteShadowProp(RwV3d& _Pos, sNTL_WORLD_SHADOW_PARAM& _NtlWorldShadowParam, CNtlWorldFieldManager* _pNWFManager)
{
	CNtlWorldSector* pNWSec = &_pNWFManager->m_pSectors[_pNWFManager->GetSectorIdx(_Pos)];

	RwV3d vTilePos;
	vTilePos.x = pNWSec->DatumPoint.x + static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize / 2) - static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize / 2);
	vTilePos.y = pNWSec->DatumPoint.y;
	vTilePos.z = pNWSec->DatumPoint.z + static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize / 2) - static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize / 2);

	RwV3d	vTempPos	= vTilePos;
	for (RwReal z = vTilePos.z; z > pNWSec->DatumPoint.z - static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize / 2); z -= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize))
	{
		vTempPos.z = z;
		for (RwReal x = vTilePos.x; x > pNWSec->DatumPoint.x - static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize / 2); x -= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize))
		{
			vTempPos.x = x;
			SetShadowProp(vTempPos, FALSE, _pNWFManager);
		}
	}

	return TRUE;
}

RwBool CNtlWorldShadowManager::UpdateParam(RwV3d& _Pos, sNTL_WORLD_SHADOW_PARAM& _NtlWorldShadowParam, CNtlWorldFieldManager* _pNWFManager)
{
	CNtlWorldSector* pNWSec = &_pNWFManager->m_pSectors[_pNWFManager->GetSectorIdx(_Pos)];

	if (!pNWSec->m_pNtlWorldShadow->m_pTexShadow)
	{
		return FALSE;
	}

#ifdef dNTL_WORLD_TOOL_MODE

	pNWSec->m_pNtlWorldShadow->UpdateSParam(_NtlWorldShadowParam);

#endif

	return TRUE;
} 


RwBool CNtlWorldShadowManager::CheckShadowFilter(RwTexture* pTexture, RwInt32 iIdx, RwInt32 iSize, RwRGBA rgbaShadow, RwReal fShadowPer)
{
	static RwInt32 iCase = 0;

	if(!pTexture)
		return FALSE;

	RwInt32 iRwTexW = RwRasterGetWidth(pTexture->raster);
	RwInt32 iRwTexH = RwRasterGetHeight(pTexture->raster);

	RwInt32 iNum = iRwTexW / iSize;

	RwInt32 iPosY = (iIdx / iNum) * iSize;
	RwInt32 iPosX = (iIdx % iNum) * iSize;
	RwInt32 iCheck = 0;

	DWORD*	pDest;
	DWORD	texel;
	DWORD*	pBits = (DWORD*)RwRasterLock(pTexture->raster, 0, rwRASTERLOCKREAD); 
	{
		for (RwInt32 y = iPosY; y < iPosY + iSize; ++y)
		{
			for (RwInt32 x = iPosX; x < iPosX + iSize; ++x)
			{
				pDest = pBits + iRwTexW * y + x;
				texel = (*pDest);

				RwUInt8 A = (RwUInt8)((texel & 0xff000000) >> 24);
				RwUInt8 R = (RwUInt8)((texel & 0x00ff0000) >> 16);
				RwUInt8 G = (RwUInt8)((texel & 0x0000ff00) >> 8);
				RwUInt8 B = (RwUInt8)((texel & 0x000000ff) >> 0);

				RwReal fGrayOri = (static_cast<RwReal>(rgbaShadow.red) * 0.299f) + (static_cast<RwReal>(rgbaShadow.green) * 0.587f) + (static_cast<RwReal>(rgbaShadow.blue) * 0.114f);
				RwReal fGrayTex = (static_cast<RwReal>(R) * 0.299f) + (static_cast<RwReal>(G) * 0.587f) + (static_cast<RwReal>(B) * 0.114f);
				if (fGrayOri + ((255.0f - fGrayOri) * 0.5f) >= fGrayTex)
				{
					++iCheck;
				}
			}
		}	
	}
	RwRasterUnlock(pTexture->raster);

	// 위에서 그림자가 아닌 영역을 체크했으므로, 여기서는 그림자 아닌 영역 기준으로 바꾸서 체크한다.
	if (static_cast<RwReal>(iCheck) / static_cast<RwReal>(iSize * iSize) >= fShadowPer)
	{
		return TRUE;
	}
	return FALSE;
}

void CNtlWorldShadowManager::SetShadowProp(RwV3d& vTilePos, RwBool bIsShadow, CNtlWorldFieldManager* _pNWFManager)
{
	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(_pNWFManager->GetFields());
	RwInt32					iFieldIdx	= _pNWFManager->GetFieldIdx(vTilePos);

	RwV3d	SPos		= pFields[iFieldIdx].GetSPos();
	RwInt32	XMoveCnt	= static_cast<RwInt32>(abs(vTilePos.x - SPos.x)) / dGET_WORLD_PARAM()->WorldSectorTileSize;
	RwInt32	ZMoveCnt	= static_cast<RwInt32>(abs(vTilePos.z - SPos.z)) / dGET_WORLD_PARAM()->WorldSectorTileSize;
	RwInt32 PosBitFlag	= XMoveCnt + (ZMoveCnt * (dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldFieldSectorNum));

	if (bIsShadow)
	{
		pFields[iFieldIdx].GetProp()->_pBitProp[PosBitFlag] |= 0x00008000; /*15 Normal Prop*/
	}
	else
	{
		pFields[iFieldIdx].GetProp()->_pBitProp[PosBitFlag] &= ~0x00008000; /*15 Normal Prop*/
	}
}
