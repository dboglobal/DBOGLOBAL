#include "precomp_ntlpresentation.h"
#include "NtlPLLensFlare.h"
#include "NtlPLResourceManager.h"
#include "NtlEffectDefine.h"
#include "NtlPLApi.h"
#include "NtlPLRenderState.h"
#include "NtlPostEffectCamera.h"
#include "NtlPLPicking.h"
#include "NtlPLWorldEntity.h"
#include "NtlPLVisualManager.h"
#include "NtlXMlDoc.h"
#include "NtlDNController.h"

CNtlPLLensFlare::CNtlPLLensFlare(void)
{
	m_fIntensity = m_fFadeIntensity = 1.0f;
	m_fIntensityBorder = 300.0f;
	m_bPrevPickingResult = FALSE;
	m_fPickingTickTime = 0.0f;

	m_eFadeStatus = FADE_STATUS_OUT;

	RwMatrixSetIdentity(&m_matWorld);	
	m_bRender = FALSE;

	SetFlags(NTL_PLEFLAG_NOT_ADD_WORLD | NTL_PLEFLAG_NOT_PROPERTY_USED  | NTL_PLEFLAG_ATTACH);
}

CNtlPLLensFlare::~CNtlPLLensFlare(void)
{
	Destroy();
}

RwBool CNtlPLLensFlare::Create( const SPLEntityCreateParam * pParam /*= NULL */ ) 
{
	// Add Spots
	AddSpots();


	// LensFlare 설정값을 설정하기 위한 테스트 함수
	//AddSpotForTest(".\\Tool\\VenusData\\LensFlare.xml");

	return TRUE;
}

void CNtlPLLensFlare::Destroy( void ) 
{
	for each(RwTexture* pTex in m_vecTex)
	{
		GetNtlResourceManager()->UnLoadTexture(pTex);
	}

	m_vecTex.clear();
	m_vecSpots.clear();	
}

RwBool CNtlPLLensFlare::Update( RwReal fElapsed ) 
{
	UpdateVertices(fElapsed);

	return TRUE;
}

RwBool CNtlPLLensFlare::Render( void ) 
{
	if(!m_bRender)
		return TRUE;

	BeginLensFlareState();

	for(UINT i = 0; i < m_vecSpots.size(); ++i)
	{
		RwD3D9SetTexture(m_vecSpots[i].m_pTexture, 0);
		RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_vecSpots[i].m_pIm2dBuffer, 4);		
	}

	EndLensFlareState();

	return TRUE;
}

void CNtlPLLensFlare::SetMatrix( RwMatrix& matWorld ) 
{
	m_matWorld = matWorld;
}

void CNtlPLLensFlare::AddSpots() 
{
	// Texture를 로딩한다.
	RwTexture* tex1 = GetNtlResourceManager()->LoadTexture("LensFlare_01", PATH_EFFECT_TEXTURE);
	RwTexture* tex2 = GetNtlResourceManager()->LoadTexture("LensFlare_02", PATH_EFFECT_TEXTURE);
	RwTexture* tex3 = GetNtlResourceManager()->LoadTexture("LensFlare_03", PATH_EFFECT_TEXTURE);
	RwTexture* tex4 = GetNtlResourceManager()->LoadTexture("LensFlare_04", PATH_EFFECT_TEXTURE);

	DBO_ASSERT(tex1, "Texture load failed.");
	DBO_ASSERT(tex2, "Texture load failed.");
	DBO_ASSERT(tex3, "Texture load failed.");
	DBO_ASSERT(tex4, "Texture load failed.");

	// 아래 값들은 그래픽팀에서 테스트하여 지정한 값이다. 
	AddSpot(SNtlPLLensFlareSpot(tex2, 0.30f,  0.7f,  1.0f, 1.0f,  1.0f, 0.3f));
	AddSpot(SNtlPLLensFlareSpot(tex1, 0.20f,  0.5f,  1.0f, 1.0f,  1.0f, 0.2f));
	AddSpot(SNtlPLLensFlareSpot(tex4, 0.06f,  0.4f,  0.6f, 1.0f,  0.9f, 0.1f));
	AddSpot(SNtlPLLensFlareSpot(tex3, 0.04f,  0.3f,  1.0f, 0.6f,  0.5f, 0.3f));
	AddSpot(SNtlPLLensFlareSpot(tex4, 0.03f,  0.15f, 0.4f, 0.6f,  1.0f, 0.1f));
	AddSpot(SNtlPLLensFlareSpot(tex4, 0.06f, -0.1f,  0.6f, 1.0f,  0.9f, 0.1f));
	AddSpot(SNtlPLLensFlareSpot(tex3, 0.15f, -0.3f,  1.0f, 0.7f,  0.0f, 0.13f));
	AddSpot(SNtlPLLensFlareSpot(tex3, 0.35f, -0.7f,  1.0f, 0.35f, 0.0f, 0.16f));
	AddSpot(SNtlPLLensFlareSpot(tex4, 0.25f, -1.0f,  1.0f, 0.7f,  0.5f, 0.1f));
	AddSpot(SNtlPLLensFlareSpot(tex4, 0.60f, -2.5f,  1.0f, 0.8f,  0.2f, 0.05f));

	m_vecTex.push_back(tex1);
	m_vecTex.push_back(tex2);
	m_vecTex.push_back(tex3);		
	m_vecTex.push_back(tex4);
}

void CNtlPLLensFlare::AddSpotForTest( const std::string& strXMLFileName ) 
{
	CNtlXMLDoc xmlDoc;
	xmlDoc.Create();

	if(!xmlDoc.Load((char*)strXMLFileName.c_str()))
		return;

	IXMLDOMNodeList* pSpotNodeList = xmlDoc.SelectNodeList("SPOT_TABLE/SPOT");
	if(pSpotNodeList)
	{
		long listLen = 0;
		pSpotNodeList->get_length(&listLen);
		for(long i = 0; i < listLen; ++i)
		{
			IXMLDOMNode* pSpotNode = NULL;
			pSpotNodeList->get_item(i, &pSpotNode);
			if(!pSpotNode)
				continue;

			char szTexture[128] = {0,};
			char szSize[32] = {0,};
			char szPos[32] = {0,};
			char szRed[32] = {0,};
			char szGreen[32] = {0,};
			char szBlue[32] = {0,};
			char szAlpha[32] = {0,};

			xmlDoc.GetTextWithAttributeName(pSpotNode, "TEXTURE", szTexture, 128);
			xmlDoc.GetTextWithAttributeName(pSpotNode, "SIZE", szSize, 32);
			xmlDoc.GetTextWithAttributeName(pSpotNode, "POS", szPos, 32);
			xmlDoc.GetTextWithAttributeName(pSpotNode, "RED", szRed, 32);
			xmlDoc.GetTextWithAttributeName(pSpotNode, "GREEN", szGreen, 32);
			xmlDoc.GetTextWithAttributeName(pSpotNode, "BLUE", szBlue, 32);
			xmlDoc.GetTextWithAttributeName(pSpotNode, "ALPHA", szAlpha, 32);

			RwTexture* pTex = GetNtlResourceManager()->LoadTexture(szTexture, PATH_EFFECT_TEXTURE);
			DBO_ASSERT(pTex, "Texture load failed.");

			m_vecTex.push_back(pTex);

			SNtlPLLensFlareSpot spot;
			spot.m_pTexture = pTex;
			spot.m_fSize = (RwReal)atof(szSize);
			spot.m_fLinePos = (RwReal)atof(szPos);
			spot.m_color.red = (RwReal)atof(szRed);
			spot.m_color.green = (RwReal)atof(szGreen);
			spot.m_color.blue = (RwReal)atof(szBlue);
			spot.m_color.alpha = (RwReal)atof(szAlpha);			

			AddSpot(spot);
		}
	}

}

void CNtlPLLensFlare::AddSpot( const SNtlPLLensFlareSpot& spot ) 
{
	m_vecSpots.push_back(spot);
}

void CNtlPLLensFlare::UpdateVertices(RwReal fElapsedTime)
{
	if(CNtlPLGlobal::m_RwCamera == NULL)
		return;

	RwInt32 nScreenWidth = CNtlPLGlobal::m_RwCamera->frameBuffer->width;
	RwInt32 nScreenHeight = CNtlPLGlobal::m_RwCamera->frameBuffer->height;

	// 광원의 2D 위치	
	RwV2d posLight = API_PL_Calc3DPosTo2D(RwMatrixGetPos(&m_matWorld), nScreenWidth, nScreenHeight, TRUE);
	if(posLight.x >= 9999.0f && posLight.y >= 9999.0f)
	{
		m_bRender = FALSE;
		return;
	}

	// 강도 계산
	RwReal fRealIntensity;
	RwReal iAwayX = (posLight.x < 0) ? -posLight.x : 
		(posLight.x > nScreenWidth) ? posLight.x - nScreenWidth : 0;
	RwReal iAwayY = (posLight.y < 0) ? -posLight.y : 
		(posLight.y > nScreenHeight) ? posLight.y - nScreenHeight : 0;

	RwReal fAway = (iAwayX > iAwayY) ? iAwayX : iAwayY;

	if (fAway > m_fIntensityBorder)
	{
		fRealIntensity = 0.0f;
	}
	else
	{
		fRealIntensity = 1.0f - (fAway / m_fIntensityBorder);
		fRealIntensity *= m_fIntensity;
	}

	if(fRealIntensity > 0.0f)
	{
		// Picking 테스트를 한다.
		m_fPickingTickTime += fElapsedTime;
		m_bPrevPickingResult = CalcPicking(posLight);
		if(m_bPrevPickingResult)
		{
			if(m_eFadeStatus == FADE_STATUS_IN)
			{
				m_fFadeIntensity = fRealIntensity;				
				m_eFadeStatus = FADE_STATUS_ING;
				m_bRender = TRUE;
			}
			else if(m_eFadeStatus == FADE_STATUS_ING)
			{
				m_fFadeIntensity -= fElapsedTime * LENSFLARE_FADE_VELOCITY;

				if(m_fFadeIntensity < 0.0f)
				{
					m_eFadeStatus = FADE_STATUS_OUT;
					m_fFadeIntensity = 0.0f;
					m_bRender = FALSE;
					return;
				}
				else
				{
					fRealIntensity = m_fFadeIntensity;
					m_bRender = TRUE;
				}
			}
			else
			{
				m_bRender = FALSE;
				return;
			}			
		}
		else
		{
			if(m_eFadeStatus == FADE_STATUS_OUT)
			{
				m_fFadeIntensity = fRealIntensity = 0.0f;
				m_eFadeStatus = FADE_STATUS_ING;				
			}
			else if(m_eFadeStatus == FADE_STATUS_ING)
			{
				m_fFadeIntensity += fElapsedTime * LENSFLARE_FADE_VELOCITY;

				if(m_fFadeIntensity > fRealIntensity)
				{
					m_fFadeIntensity = fRealIntensity;
					m_eFadeStatus = FADE_STATUS_IN;
				}

				fRealIntensity = m_fFadeIntensity;
			}

			m_bRender = TRUE;
		}
	}
	else
	{
		// 강도가 0보다 작으면 화면에 안나오기 때문에, 계산도 렌더링도 필요없다
		m_bRender = FALSE;
		return;
	}

	// Vertex 위치를 계산하여 설정한다.

	RwInt32 nCenterOfScreenX = nScreenWidth / 2;
	RwInt32 nCenterOfScreenY = nScreenHeight / 2;
	RwInt32	nDistanceX		 = (RwInt32)(nCenterOfScreenX - posLight.x);
	RwInt32 nDistanceY		 = (RwInt32)(nCenterOfScreenY - posLight.y);

	for(UINT i = 0; i < m_vecSpots.size(); ++i)
	{
		// spot의 위치를 계산한다.
		RwInt32 nSpotCenterPosX = (RwInt32)(nCenterOfScreenX - (RwReal)nDistanceX * m_vecSpots[i].m_fLinePos);
		RwInt32 nSpotCenterPosY = (RwInt32)(nCenterOfScreenY - (RwReal)nDistanceY * m_vecSpots[i].m_fLinePos);
		RwInt32 nSpotSizeHalf = (RwInt32)(nScreenWidth * m_vecSpots[i].m_fSize * 0.5f);		

		// 색상 결정
		RwRGBAReal color = m_vecSpots[i].m_color;
		color.alpha *= fRealIntensity;		

		if(GetDnController()->IsDNEffectApplied())	// D&N이 켜져있으면 LensFlare에도 영향을 준다
		{
			RwReal TFactor = static_cast<RwReal>(dGET_WORLD_PARAM()->ClrDayAndNight & 0x000000ff) / 255.0f;
			TFactor = 1.0f - TFactor * CNtlPLGlobal::m_fLensFlareForDNRatio;
			if(TFactor < 0.0f) TFactor = 0.0f;				
			color.alpha *= TFactor;
		}

		if(color.alpha > 1.0f) color.alpha = 1.0f;
		if(color.alpha < 0.0f) color.alpha = 0.0f;


		if(!m_vecSpots[i].m_pIm2dBuffer)
		{
			m_vecSpots[i].m_pIm2dBuffer = NTL_NEW RwIm2DVertex[4];
		}

		SetRhwVertex(m_vecSpots[i].m_pIm2dBuffer, (RwReal)(nSpotCenterPosX - nSpotSizeHalf), (RwReal)(nSpotCenterPosY - nSpotSizeHalf), nSpotSizeHalf * 2, nSpotSizeHalf * 2, color);		
	}
}

void CNtlPLLensFlare::SetRhwVertex(RwIm2DVertex* pVertex, RwReal nStartX, RwReal nStartY, RwInt32 width, RwInt32 height, RwRGBAReal color)
{
	if (CNtlPLGlobal::m_RwCamera == NULL) return;
	RwReal recipCameraZ = 1.0f / RwCameraGetNearClipPlane(CNtlPLGlobal::m_RwCamera);

	RwRGBA nColor;
	RwRGBAFromRwRGBAReal(&nColor, &color);

	RwIm2DVertexSetScreenX(&pVertex[0], nStartX);
	RwIm2DVertexSetScreenY(&pVertex[0], nStartY);
	RwIm2DVertexSetScreenZ(&pVertex[0], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&pVertex[0], recipCameraZ);
	RwIm2DVertexSetU(&pVertex[0], 0.0f, recipCameraZ);
	RwIm2DVertexSetV(&pVertex[0], 0.0f, recipCameraZ);		
	RwIm2DVertexSetIntRGBA(&pVertex[0], nColor.red, nColor.green, nColor.blue, nColor.alpha);

	RwIm2DVertexSetScreenX(&pVertex[1], nStartX);
	RwIm2DVertexSetScreenY(&pVertex[1], nStartY+height);
	RwIm2DVertexSetScreenZ(&pVertex[1], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&pVertex[1], recipCameraZ);
	RwIm2DVertexSetU(&pVertex[1], 0.0f, recipCameraZ);
	RwIm2DVertexSetV(&pVertex[1], 1.0f, recipCameraZ);	
	RwIm2DVertexSetIntRGBA(&pVertex[1], nColor.red, nColor.green, nColor.blue, nColor.alpha);

	RwIm2DVertexSetScreenX(&pVertex[2], nStartX+width);
	RwIm2DVertexSetScreenY(&pVertex[2], nStartY);
	RwIm2DVertexSetScreenZ(&pVertex[2], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&pVertex[2], recipCameraZ);
	RwIm2DVertexSetU(&pVertex[2], 1.0f, recipCameraZ);
	RwIm2DVertexSetV(&pVertex[2], 0.0f, recipCameraZ);	
	RwIm2DVertexSetIntRGBA(&pVertex[2], nColor.red, nColor.green, nColor.blue, nColor.alpha);

	RwIm2DVertexSetScreenX(&pVertex[3], nStartX+width);
	RwIm2DVertexSetScreenY(&pVertex[3], nStartY+height);
	RwIm2DVertexSetScreenZ(&pVertex[3], RwIm2DGetNearScreenZ());
	RwIm2DVertexSetRecipCameraZ(&pVertex[3], recipCameraZ);
	RwIm2DVertexSetU(&pVertex[3], 1.0f, recipCameraZ);
	RwIm2DVertexSetV(&pVertex[3], 1.0f, recipCameraZ);	
	RwIm2DVertexSetIntRGBA(&pVertex[3], nColor.red, nColor.green, nColor.blue, nColor.alpha);
}

RwBool CNtlPLLensFlare::SetProperty( const CNtlPLProperty *pData ) 
{
	return TRUE;
}

RwBool CNtlPLLensFlare::CalcPicking( const RwV2d& posLight ) 
{
	if(m_fPickingTickTime < LENSFLARE_PICK_TICK)
	{
		return m_bPrevPickingResult;
	}
	else
	{
		m_fPickingTickTime = 0.0f;
	}

	// 지형을 피킹한다.	
	CNtlPLWorldEntity* pWorldEntity = reinterpret_cast<CNtlPLVisualManager*>(GetSceneManager())->GetWorld();
	if(!pWorldEntity)
	{
		return TRUE;
	}

	RwV3d IntersectionPt;
	if(pWorldEntity->PickTerrain(static_cast<RwInt32>(posLight.x), static_cast<RwInt32>(posLight.y), IntersectionPt))
	{
		return TRUE;
	}

	// 오브젝트를 피킹한다.		
	SWorldPickInfo pickInfo;
	ZeroMemory(&pickInfo, sizeof(pickInfo));
	pickInfo.fMinDist = RwRealMAXVAL;	

	RwLine line;
	line.end = *RwMatrixGetPos(&m_matWorld);
	line.start = *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));

	return Collision_WorldIntersectionObject(line, &pickInfo);
}
