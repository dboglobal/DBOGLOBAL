#include "precomp_ntlpresentation.h"
#include "NtlPLEmblemMaker.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLCameraRT.h"
#include "NtlPLPalette.h"



CNtlPLEmblemMaker* CNtlPLEmblemMaker::m_pInstance = NULL;

CNtlPLEmblemMaker::CNtlPLEmblemMaker()
{
}

CNtlPLEmblemMaker::~CNtlPLEmblemMaker()
{	
}

VOID CNtlPLEmblemMaker::CreateInstance()
{
	NTL_FUNCTION("CNtlPLEmblemMaker::CreateInstance");
	NTL_ASSERT(m_pInstance == NULL, "Already created instance");

	m_pInstance = NTL_NEW CNtlPLEmblemMaker;


	RwIm2DVertexSetScreenZ(&m_pInstance->m_2dVertices[0], 0.0f);
	RwIm2DVertexSetRecipCameraZ(&m_pInstance->m_2dVertices[0], 1.0f);
	RwIm2DVertexSetU (&m_pInstance->m_2dVertices[0], 0.f, 0);
	RwIm2DVertexSetV (&m_pInstance->m_2dVertices[0], 0.f, 0);
	
	RwIm2DVertexSetScreenZ(&m_pInstance->m_2dVertices[1], 0.0f);
	RwIm2DVertexSetRecipCameraZ(&m_pInstance->m_2dVertices[1], 1.0f);
	RwIm2DVertexSetU (&m_pInstance->m_2dVertices[1], 0.f, 0);
	RwIm2DVertexSetV (&m_pInstance->m_2dVertices[1], 1.f, 0);
	
	RwIm2DVertexSetScreenZ(&m_pInstance->m_2dVertices[2], 0.0f);
	RwIm2DVertexSetRecipCameraZ(&m_pInstance->m_2dVertices[2], 1.0f);
	RwIm2DVertexSetU (&m_pInstance->m_2dVertices[2], 1.f, 0);
	RwIm2DVertexSetV (&m_pInstance->m_2dVertices[2], 0.f, 0);
	
	RwIm2DVertexSetScreenZ(&m_pInstance->m_2dVertices[3], 0.0f);
	RwIm2DVertexSetRecipCameraZ(&m_pInstance->m_2dVertices[3], 1.0f);
	RwIm2DVertexSetU (&m_pInstance->m_2dVertices[3], 1.f, 0);
	RwIm2DVertexSetV (&m_pInstance->m_2dVertices[3], 1.f, 0);

	m_pInstance->m_pStuffList = NTL_NEW CNtlPLEmblemStuffList;
	m_pInstance->m_pStuffList->Create();

	NTL_RETURNVOID();
}

VOID CNtlPLEmblemMaker::DestoryInstance()
{
	NTL_FUNCTION("CNtlPLEmblemMaker::~DestoryInstance");

	if( m_pInstance->m_pStuffList )
	{
		m_pInstance->m_pStuffList->Destroy();
		NTL_DELETE(m_pInstance->m_pStuffList);
	}

	ITER_RT_CAMERA it_RTCamera = m_pInstance->m_mapRTCamera.begin();
	for( ; it_RTCamera != m_pInstance->m_mapRTCamera.end() ; ++it_RTCamera )
	{
		CNtlPLCameraRT* pRTCamera = it_RTCamera->second;
		pRTCamera->Destroy();
		NTL_DELETE(pRTCamera);
	}
	m_pInstance->m_mapRTCamera.clear();

	NTL_ASSERT(m_pInstance->m_mapTexture.size() == 0, "exist emblem texture");

	NTL_DELETE(m_pInstance);
	
	NTL_RETURNVOID();
}

VOID CNtlPLEmblemMaker::DestoyEmblem(const char* pcTextureName)
{
	if( !pcTextureName )
		return;

	ITER_MAP_TEXTURE it = m_mapTexture.find(pcTextureName);
	if( it != m_mapTexture.end() )
	{
		RwBool bDestroyEmblemData = false;
		sEmblemData* pEmblemData = it->second;	

		if( pEmblemData->pTexture->refCount == 1 )
		{
			bDestroyEmblemData = true;
			m_mapTexture.erase(it);
		}

		RwTextureDestroy(pEmblemData->pTexture);
		
		if(bDestroyEmblemData)
			NTL_DELETE(pEmblemData);
	}
}

VOID CNtlPLEmblemMaker::DestoyEmblem(RwTexture* pTexture)
{
	if( !pTexture )
		return;

	ITER_MAP_TEXTURE it = m_mapTexture.begin();
	for( ; it != m_mapTexture.end() ; ++it )
	{
		RwBool bDestroyEmblemData = false;
		sEmblemData* pEmblemData = it->second;	

		if( pEmblemData->pTexture == pTexture )
		{
			if( pTexture->refCount == 1 )
			{
				bDestroyEmblemData = true;
				m_mapTexture.erase(it);
			}

			RwTextureDestroy(pEmblemData->pTexture);

			if(bDestroyEmblemData)
				NTL_DELETE(pEmblemData);
			return;
		}
	}
}

RwTexture* CNtlPLEmblemMaker::CreateEmblem(const sEmblemFactor* pEmblemFactor,
										   RwUInt8 byEmblemSize /* = dEMBLEM_TEXTURE_DEFAULT_SIZE */,
										   const RwChar* pcBaseTextureName /* = NULL */)
{
	char acEmblemName[256];

	if( !pEmblemFactor )
	{
		DBO_FAIL("Emblem factor is null");
		return NULL;
	}

	if( pEmblemFactor->byTypeA == dPLEMBLEM_INVALID_FACTOR &&
		pEmblemFactor->byTypeB == dPLEMBLEM_INVALID_FACTOR &&
		pEmblemFactor->byTypeC == dPLEMBLEM_INVALID_FACTOR )
		return NULL;

	if( dMAX_EMBLEM_TEXTURE_SIZE < byEmblemSize )
	{
		NTL_ASSERTFAIL("Emblem size is bigger than " << dMAX_EMBLEM_TEXTURE_SIZE);
		return NULL;
	}


	MakeEmblemTextureName(acEmblemName, 256, pEmblemFactor, byEmblemSize, pcBaseTextureName);	
	if( strlen(acEmblemName) == 0 )
		return NULL;


	// 기존에 존재하는 텍스처이다
	ITER_MAP_TEXTURE it = m_mapTexture.find(acEmblemName);
	if( it != m_mapTexture.end() )
	{
		sEmblemData* pEmblemData = it->second;
		RwTextureAddRef(pEmblemData->pTexture);
		return pEmblemData->pTexture;
	}

	// 텍스처를 새로 제작한다
	return MakeEmblemImmidately(pEmblemFactor, acEmblemName, byEmblemSize, pcBaseTextureName);
}

RwTexture* CNtlPLEmblemMaker::MakeEmblemImmidately(const sEmblemFactor* pEmblemFactor,
												   const char* pcEmblemName,
												   RwUInt8 byEmblemSize,
												   const RwChar* pcBaseTextureName /*= NULL */)
{
	if( !pEmblemFactor )
	{
		DBO_FAIL("Emblem factor is null");
		return NULL;
	}

	if( !pcEmblemName || strlen(pcEmblemName) == 0 )
	{
		DBO_FAIL("Emblem name is empty");
		return NULL;
	}

	char acEmblemFactor[64];
	RwTexture* pTextureTypeA	= NULL;
	RwTexture* pTextureTypeB	= NULL;
	RwTexture* pTextureTypeC	= NULL;
	RwTexture* pBaseTexture		= NULL;
	RwTexture* pNewEmblemTexture= NULL;
	
	RwImageSetPath(".\\texture\\gui\\emblem\\");

	// 재료 텍스처를 읽어온다
	if( pEmblemFactor->byTypeA != dPLEMBLEM_INVALID_FACTOR )
	{
		MakeEmblemResourceName(EMBLEMTYPE_A, pEmblemFactor->byTypeA, acEmblemFactor, 64, byEmblemSize);
		pTextureTypeA = RwTextureRead(acEmblemFactor, NULL);
		if(!pTextureTypeA)
		{
			char acBuffer[128];
			sprintf_s(acBuffer, 128, "Not exist texture of name : %s", acEmblemFactor);
			NtlLogFilePrint(acBuffer);
			return NULL;
		}
	}

	if( pEmblemFactor->byTypeB != dPLEMBLEM_INVALID_FACTOR )
	{
		MakeEmblemResourceName(EMBLEMTYPE_B, pEmblemFactor->byTypeB, acEmblemFactor, 64, byEmblemSize);
		pTextureTypeB = RwTextureRead(acEmblemFactor, NULL);
		if(!pTextureTypeB)
		{
			char acBuffer[128];
			sprintf_s(acBuffer, 128, "Not exist texture of name : %s", acEmblemFactor);
			NtlLogFilePrint(acBuffer);

			if( pTextureTypeA )
				RwTextureDestroy(pTextureTypeA);

			return NULL;
		}
	}

	if( pEmblemFactor->byTypeC != dPLEMBLEM_INVALID_FACTOR )
	{
		MakeEmblemResourceName(EMBLEMTYPE_C, pEmblemFactor->byTypeC, acEmblemFactor, 64, byEmblemSize);
		pTextureTypeC = RwTextureRead(acEmblemFactor, NULL);
		if(!pTextureTypeC)
		{
			char acBuffer[128];
			sprintf_s(acBuffer, 128, "Not exist texture of name : %s", acEmblemFactor);
			NtlLogFilePrint(acBuffer);

			if( pTextureTypeA)
				RwTextureDestroy(pTextureTypeA);

			if( pTextureTypeB )
				RwTextureDestroy(pTextureTypeB);

			return NULL;
		}
	}

	if( pcBaseTextureName && strlen(pcBaseTextureName) > 0 )
	{
		pBaseTexture = RwTextureRead(pcBaseTextureName, NULL);
		if(!pBaseTexture)
		{
			// Base Texture가 없어도 엠블렘을 만든다
			char acBuffer[128];
			sprintf_s(acBuffer, 128, "Not exist texture of name : %s", pcBaseTextureName);
			NtlLogFilePrint(acBuffer);
		}
	}

	CNtlPLCameraRT* pRTCamera = GetRTCamera(byEmblemSize);
	if( !pRTCamera )
	{
		if( pTextureTypeA )
			RwTextureDestroy(pTextureTypeA);

		if( pTextureTypeB )
			RwTextureDestroy(pTextureTypeB);

		if( pTextureTypeC )
			RwTextureDestroy(pTextureTypeC);

		if( pBaseTexture )
			RwTextureDestroy(pBaseTexture);

		NTL_ASSERTFAIL("Can not create render target camera of size : " << byEmblemSize);
		return NULL;
	}


	// Render to texture
	if( pRTCamera->CameraBeginUpdate() )
	{

	// 엠블렘 사이즈 결정
#define d2DPIXEL_OFFSET		(-.5f)

		RwIm2DVertexSetScreenX(&m_pInstance->m_2dVertices[0], 0.f + d2DPIXEL_OFFSET);
		RwIm2DVertexSetScreenY(&m_pInstance->m_2dVertices[0], 0.f + d2DPIXEL_OFFSET);

		RwIm2DVertexSetScreenX(&m_pInstance->m_2dVertices[1], 0.f + d2DPIXEL_OFFSET);
		RwIm2DVertexSetScreenY(&m_pInstance->m_2dVertices[1], (RwReal)byEmblemSize + d2DPIXEL_OFFSET);

		RwIm2DVertexSetScreenX(&m_pInstance->m_2dVertices[2], (RwReal)byEmblemSize + d2DPIXEL_OFFSET);
		RwIm2DVertexSetScreenY(&m_pInstance->m_2dVertices[2], 0.f + d2DPIXEL_OFFSET);

		RwIm2DVertexSetScreenX(&m_pInstance->m_2dVertices[3], (RwReal)byEmblemSize + d2DPIXEL_OFFSET);
		RwIm2DVertexSetScreenY(&m_pInstance->m_2dVertices[3], (RwReal)byEmblemSize + d2DPIXEL_OFFSET);



		COLORREF color;

		if( pBaseTexture )
		{			
			RecoatEmblem(pBaseTexture, 255, 255, 255);
		}

		if( pEmblemFactor->byTypeC != dPLEMBLEM_INVALID_FACTOR )
		{
			color = GetNtlPLPalette()->GetColor(PALETTETYPE_EMBLEM, pEmblemFactor->byTypeCColor);
			RecoatEmblem(pTextureTypeC, GetRValue(color), GetGValue(color), GetBValue(color));
		}

		if( pEmblemFactor->byTypeB != dPLEMBLEM_INVALID_FACTOR )
		{
			color = GetNtlPLPalette()->GetColor(PALETTETYPE_EMBLEM, pEmblemFactor->byTypeBColor);
			RecoatEmblem(pTextureTypeB, GetRValue(color), GetGValue(color), GetBValue(color));
		}

		if( pEmblemFactor->byTypeA != dPLEMBLEM_INVALID_FACTOR )
		{
			color = GetNtlPLPalette()->GetColor(PALETTETYPE_EMBLEM, pEmblemFactor->byTypeAColor);
			RecoatEmblem(pTextureTypeA, GetRValue(color), GetGValue(color), GetBValue(color));
		}

		pRTCamera->CameraEndUpdate();
		
		pNewEmblemTexture = CopyToTexture( pRTCamera->GetTexture() );
		RwTextureSetName(pNewEmblemTexture, pcEmblemName);

		// 새로 제작된 엠블렘을 저장한다
		sEmblemData* pNewEmblemData = NTL_NEW sEmblemData;
		pNewEmblemData->pTexture	= pNewEmblemTexture;
		pNewEmblemData->factor		= *pEmblemFactor;

		m_mapTexture[pcEmblemName] = pNewEmblemData;
	}	

	if( pTextureTypeA )
		RwTextureDestroy(pTextureTypeA);

	if( pTextureTypeB )
		RwTextureDestroy(pTextureTypeB);

	if( pTextureTypeC )
		RwTextureDestroy(pTextureTypeC);

	if( pBaseTexture )
		RwTextureDestroy(pBaseTexture);	

	return pNewEmblemTexture;
}

VOID CNtlPLEmblemMaker::RecoatEmblem(RwTexture* pTexture, RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue)
{
	if( !pTexture )
		return;

	for(RwInt32 i = 0; i < 4; i++)
	{
		RwIm2DVertexSetIntRGBA(&m_2dVertices[i], byRed, byGreen, byBlue, 255);
	}

	RwD3D9SetTexture(pTexture, 0);

	RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_2dVertices, 4);
}

VOID CNtlPLEmblemMaker::MakeEmblemResourceName(eEmblemType eType, RwUInt8 byValue,
											   char* pcOutBuffer, RwInt32 iBufferLength,
											   RwUInt8 byEmblemSize /* = dEMBLEM_TEXTURE_DEFAULT_SIZE */)
{
	if( !pcOutBuffer )
	{
		DBO_FAIL("name buffer is null");
		return;
	}

	switch(eType)
	{
		case EMBLEMTYPE_A:
		{
			sprintf_s(pcOutBuffer, iBufferLength, "EmblemA_%03d_%d.png", byValue, byEmblemSize);
		}
		break;
		case EMBLEMTYPE_B:
		{
			sprintf_s(pcOutBuffer, iBufferLength, "EmblemB_%03d_%d.png", byValue, byEmblemSize);
		}
		break;
		case EMBLEMTYPE_C:
		{
			sprintf_s(pcOutBuffer, iBufferLength, "EmblemC_%03d_%d.png", byValue, byEmblemSize);
		}
		break;
		default:
		{
			NTL_ASSERT(false, "Invalid emblem type : " << eType);
		}
		break;
	}
}

VOID CNtlPLEmblemMaker::MakeEmblemTextureName(char* pcOutBuffer, RwInt32 iBufferLength, const sEmblemFactor* pEmblemFactor,
											  RwUInt8 byEmblemSize, const RwChar* pcBaseTextureName)
{
	if( !pcOutBuffer )
	{
		DBO_FAIL("name buffer is null");
		return;
	}

	if( !pEmblemFactor )
	{
		DBO_FAIL("Emblem factor is null");
		return;
	}

	if( pcBaseTextureName && strlen(pcBaseTextureName) > 0 )
	{
		sprintf_s(pcOutBuffer, iBufferLength, "A%03dAC%03dB%03dBC%03dC%03dCC%03d%d",
												pEmblemFactor->byTypeA, pEmblemFactor->byTypeAColor,
												pEmblemFactor->byTypeB, pEmblemFactor->byTypeBColor,
												pEmblemFactor->byTypeC, pEmblemFactor->byTypeCColor,
												byEmblemSize);
	}
	else
		sprintf_s(pcOutBuffer, iBufferLength, "A%03dAC%03dB%03dBC%03dC%03dCC%03d%d",
												pEmblemFactor->byTypeA, pEmblemFactor->byTypeAColor,
												pEmblemFactor->byTypeB, pEmblemFactor->byTypeBColor,
												pEmblemFactor->byTypeC, pEmblemFactor->byTypeCColor,
												byEmblemSize);
}

RwTexture* CNtlPLEmblemMaker::CopyToTexture(RwTexture* pTexture)
{
	if( !pTexture )
	{
		DBO_FAIL("texture is null");
		return NULL;
	}

	RwTexture* pNewTexture = NULL;
	RwRaster* pRaster = RwTextureGetRaster(pTexture);

	if( pRaster )
	{
		RwImage* pImage = RwImageCreate( RwRasterGetWidth(pRaster), RwRasterGetHeight(pRaster), RwRasterGetDepth(pRaster) );

		RwImageAllocatePixels(pImage);
		RwImageSetFromRaster(pImage, pRaster);


		RwRaster* pNewRaster = RwRasterCreate( RwRasterGetWidth(pRaster), RwRasterGetHeight(pRaster), RwRasterGetDepth(pRaster), rwRASTERTYPETEXTURE | rwRASTERFORMAT8888 );

		RwRasterSetFromImage(pNewRaster, pImage);
		pNewTexture = RwTextureCreate(pNewRaster);
		RwTextureSetFilterMode(pNewTexture, RwTextureGetFilterMode(pTexture));
		RwTextureSetAddressingU(pNewTexture, RwTextureGetAddressingU(pTexture));
		RwTextureSetAddressingV(pNewTexture, RwTextureGetAddressingV(pTexture));

		RwImageDestroy(pImage);
	}
	
	return pNewTexture;
}

const MAP_STUFF* CNtlPLEmblemMaker::GetStuffList(eEmblemType eType)
{
	return m_pStuffList->GetStuffList(eType);
}

CNtlPLCameraRT* CNtlPLEmblemMaker::GetRTCamera(RwUInt8 bySize)
{
	ITER_RT_CAMERA it_RTCamera = m_mapRTCamera.find(bySize);
	if( it_RTCamera != m_mapRTCamera.end() )
		return it_RTCamera->second;

	CNtlPLCameraRT* pRTCamera = NTL_NEW CNtlPLCameraRT;
	pRTCamera->Create(bySize, bySize);

	m_mapRTCamera[bySize] = pRTCamera;

	return pRTCamera;
}