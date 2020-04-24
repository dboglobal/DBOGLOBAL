#include "precomp_ntlpresentation.h"
#include "NtlPLSun.h"
#include "NtlEffectDefine.h"
#include "NtlPLResourceManager.h"
#include "NtlPLEntityFreeList.h"
#include "NtlPLVisualManager.h"
#include "NtlPLRenderState.h"
#include "NtlDebug.h"
#include "NtlMath.h"
#include "NtlPostEffectCamera.h"
#include "NtlDNController.h"
#include "ntlworldcommon.h"

/// 안전하게 싱글톤 객체를 가져오는 매크로 함수
#define GetSafeInstance(class) if(class::GetInstance()) class::GetInstance()


void* CNtlPLSun::operator new(size_t size)
{
	NTL_FUNCTION(__FUNCTION__);

	NTL_RETURN(CNtlPLEntityFreeList::Alloc(PLENTITY_SUN));
}

void CNtlPLSun::operator delete(void *pObj)
{
	CNtlPLEntityFreeList::Free(PLENTITY_SUN, pObj);	
}

CNtlPLSun::CNtlPLSun(void)
{
	m_pIm3dBuffer	= NULL;
	m_vPos			= ZeroAxis;
	m_fScale		= 1.0f;
	m_pTexture		= NULL;
	m_bFading		= FALSE;
	m_fFadeTime		= 0.0f;
	m_fFadingTime	= 0.0f;
	m_pLensFlare	= NULL;
	m_bVisible		= TRUE;
	
	m_color.red = m_color.green = m_color.blue = m_color.alpha = 255;

	RwMatrixSetIdentity(&m_matWorld);

	SetLayer(PLENTITY_LAYER_SKY);
	SetMinimapLayer(NTL_PLEFLAG_MINIMAP_LAYER_ONE);
	SetClassType(PLENTITY_SUN);
	// 디폴트 값을 Not Visible로 세팅한다. SceneManager에서 랜더링하는게 아니라, CNtlPLSky에서 따로 렌더링 하기 때문이다.
	// 2007.08.06 by agebreak
	SetFlags(NTL_PLEFLAG_NOT_ADD_WORLD | NTL_PLEFLAG_NOT_PROPERTY_USED  | NTL_PLEFLAG_ATTACH | NTL_PLEFLAG_NOT_VISIBLE);	
}

CNtlPLSun::~CNtlPLSun(void)
{
	Destroy();
}

void CNtlPLSun::Destroy( void ) 
{
	if(m_pLensFlare)
	{
		GetSafeInstance(CNtlPostEffectCamera)->DetachLensFlare(m_pLensFlare);
		NTL_DELETE(m_pLensFlare);		
	}

	if(m_pTexture)
	{
		GetNtlResourceManager()->UnLoadTexture(m_pTexture);
		m_pTexture = NULL;
	}

	NTL_ARRAY_DELETE(m_pIm3dBuffer);
}

RwBool CNtlPLSun::SetProperty( const CNtlPLProperty *pData ) 
{
	return TRUE;
}

RwBool CNtlPLSun::Create( const SPLEntityCreateParam * pParam /*= NULL */ ) 
{
	// Vertex Buffer를 생성한다.
	CreateVertexBuffer();

	if(pParam)
	{
		SPLSunCreateParam* pSunParam = (SPLSunCreateParam*)pParam;
		if(pSunParam->bAttachLensFlare)
		{
			// LensFlare를 생성한다.
			m_pLensFlare = NTL_NEW CNtlPLLensFlare();
			m_pLensFlare->Create();
			
            GetSafeInstance(CNtlPostEffectCamera)->AttachLensFlare(m_pLensFlare);
		}

		if(!pSunParam->strTexture.empty())
		{
			m_pTexture = GetNtlResourceManager()->LoadTexture(pSunParam->strTexture.c_str(), PATH_EFFECT_TEXTURE);
			DBO_ASSERT(m_pTexture, "Texture load failed.");
		}
	}

	return TRUE;
}


void CNtlPLSun::CreateVertexBuffer() 
{
	m_pIm3dBuffer = NTL_NEW RwIm3DVertex[6];
	RwV3d vert[4];
	vert[0].x = 0.5f;
	vert[0].y = 0.5f;
	vert[1].x = -0.5f;
	vert[1].y = 0.5f;
	vert[2].x = 0.5f;
	vert[2].y = -0.5f;
	vert[3].x = -0.5f;
	vert[3].y = -0.5f;
	for(int i = 0; i < 4; ++i)
	{
		vert[i].z = 0.0f;
	}

	m_pIm3dBuffer[0].objVertex = vert[0];
	m_pIm3dBuffer[1].objVertex = vert[2];
	m_pIm3dBuffer[2].objVertex = vert[1];
	m_pIm3dBuffer[3].objVertex = vert[1];
	m_pIm3dBuffer[4].objVertex = vert[2];
	m_pIm3dBuffer[5].objVertex = vert[3];

	m_pIm3dBuffer[0].u = 0.0f;
	m_pIm3dBuffer[0].v = 0.0f;
	m_pIm3dBuffer[1].u = 0.0f;
	m_pIm3dBuffer[1].v = 1.0f;
	m_pIm3dBuffer[2].u = 1.0f;
	m_pIm3dBuffer[2].v = 0.0f;
	m_pIm3dBuffer[3].u = 1.0f;
	m_pIm3dBuffer[3].v = 0.0f;
	m_pIm3dBuffer[4].u = 0.0f;
	m_pIm3dBuffer[4].v = 1.0f;
	m_pIm3dBuffer[5].u = 1.0f;
	m_pIm3dBuffer[5].v = 1.0f;

	for(int i = 0; i < 6; ++i)
	{
		RwIm3DVertexSetRGBA(&m_pIm3dBuffer[i], 255, 255, 255, 255);
	}
}

RwBool CNtlPLSun::Update( RwReal fElapsed ) 
{
	if(m_bFading)
	{
		UpdateFadeColor(fElapsed);
	}

	if(GetDnController()->IsDNEffectApplied())
	{
		SetColor(m_color);
	}

	if(m_pLensFlare)
	{
		// Sun의 실제 위치는 카메라 + 절대좌표 값이다. 그 값에 맞게 LensFlare의 광원위치도 설정한다.
		RwMatrix matLensFlare = m_matWorld;
		RwV3d vPosCamera = *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));	
		*RwMatrixGetPos(&matLensFlare) = vPosCamera + m_vPos;
		m_pLensFlare->SetMatrix(matLensFlare);

		m_pLensFlare->Update(fElapsed);
	}

	return TRUE;
}

RwBool CNtlPLSun::Render( void ) 
{
	// 카메라의 Far Plane 값을 변경한다. (태양이 멀리 있어도 찍히게)
	RwReal fOldFarPlane = RwCameraGetFarClipPlane(CNtlPLGlobal::m_RwCamera);
/*
	RwCameraEndUpdate(CNtlPLGlobal::m_RwCamera);
	// 10000 미터로 Far Plane을 세팅한다.
	RwCameraSetFarClipPlane(CNtlPLGlobal::m_RwCamera, 5000.0f);
	RwCameraBeginUpdate(CNtlPLGlobal::m_RwCamera);
*/
	// 빌보드와 위치값을 적용한다.
	RwMatrix matRender = *RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera));	
	
	RwV3d vPosCamera = *RwMatrixGetPos(&matRender);				
	*RwMatrixGetPos(&matRender) = vPosCamera + m_vPos;		

	RwV3d vScale;
	vScale.x = vScale.y = vScale.z = m_fScale;
	RwMatrixScale(&matRender, &vScale, rwCOMBINEPRECONCAT);

	//BeginSunState();	

	if(m_pTexture)
	{
		RwD3D9SetTexture(m_pTexture, 0);
	}

	if(RwIm3DTransform(m_pIm3dBuffer, 6, &matRender, rwIM3D_VERTEXUV | rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA))
	{
		RwIm3DRenderPrimitive(rwPRIMTYPETRILIST);
		RwIm3DEnd();
	}

	
	//EndSunState();
/*
	RwCameraEndUpdate(CNtlPLGlobal::m_RwCamera);
	RwCameraSetFarClipPlane(CNtlPLGlobal::m_RwCamera, fOldFarPlane);
	RwCameraBeginUpdate(CNtlPLGlobal::m_RwCamera);
*/
	// LensFlare의 Render는 PostEffectCamera에서 한다.	
	
	return TRUE;
}

void CNtlPLSun::SetColor( RwRGBA color ) 
{
	m_color = color;
	
	RwRGBAReal colorReal;
	RwRGBARealFromRwRGBA(&colorReal, &color);

	if(GetDnController()->IsDNEffectApplied())	// D&N이 켜져있으면 알파값으로 영향을 받는다
	{
		RwReal TFactor = static_cast<RwReal>(dGET_WORLD_PARAM()->ClrDayAndNight & 0x000000ff) / 255.0f;
		TFactor = 1.0f - TFactor * CNtlPLGlobal::m_fLensFlareForDNRatio;
		if(TFactor < 0.0f) TFactor = 0.0f;			
		colorReal.alpha *= TFactor;
	}

	for(int i = 0; i < 6; ++i)
	{
		RwIm3DVertexSetRGBA(&m_pIm3dBuffer[i], color.red, color.green, color.blue, (RwUInt8)(colorReal.alpha * 255.0f));		
	}
}

void CNtlPLSun::SetPosition( const RwV3d* pPos ) 
{
	m_vPos = *pPos;	
	*RwMatrixGetPos(&m_matWorld) = m_vPos;
}

void CNtlPLSun::SetMatrix( RwMatrix& matWorld ) 
{
	m_matWorld = matWorld;
	SetPosition(RwMatrixGetPos(&m_matWorld));	
}

void CNtlPLSun::SetScale( RwReal fScale ) 
{
	m_fScale = fScale;
}

void CNtlPLSun::SetTexture( RwTexture* pTexture ) 
{
	m_pTexture = pTexture;
}

RwBool CNtlPLSun::GetVisibility()
{
	return (m_color.alpha && m_bVisible);
}

void CNtlPLSun::SetFadeEffect( RwBool bFadeIn, RwReal fTime /*= 1.0f*/ ) 
{
	RwRGBA colorFade = m_color;

	if(bFadeIn)
	{
		colorFade.alpha = 255;
	}
	else
	{
		colorFade.alpha = 0;		
	}

	SetFadeEffect(colorFade, fTime);
}

void CNtlPLSun::SetFadeEffect( RwRGBA colorFade, RwReal fTime /*= 1.0f*/ ) 
{
	m_OriginColor = m_color;
	m_FadeColor = colorFade;
	m_fFadeTime = fTime;
	m_fFadingTime = 0.0f;
	m_bFading = TRUE;
}

void CNtlPLSun::UpdateFadeColor( RwReal fElapsedTime ) 
{
	m_fFadingTime += fElapsedTime;

	RwReal fDeltaTime = m_fFadingTime / m_fFadeTime;
	if(fDeltaTime > 1.0f)
	{
		m_bFading = FALSE;
		fDeltaTime = 1.0f;
	}

	RwRGBA color;
	color = CNtlMath::Interpolation(m_OriginColor, m_FadeColor, fDeltaTime);
	SetColor(color);

	// 렌즈 플레어 Fade
	if(m_pLensFlare)
	{
		RwReal fAlpha = color.alpha / 255.0f;
		m_pLensFlare->SetMaxIntensity(fAlpha);
	}
}

void CNtlPLSun::ReFreshLensFlareData() 
{
	if(m_pLensFlare)
	{
		GetSafeInstance(CNtlPostEffectCamera)->DetachLensFlare(m_pLensFlare);
		NTL_DELETE(m_pLensFlare);		
	}

	// LensFlare를 생성한다.
	m_pLensFlare = NTL_NEW CNtlPLLensFlare();
	m_pLensFlare->Create();
	m_pLensFlare->SetVisible(m_bVisible);

	GetSafeInstance(CNtlPostEffectCamera)->AttachLensFlare(m_pLensFlare);
}


void CNtlPLSun::SetVisibility(RwBool bVisible)
{
	m_bVisible = bVisible;

	if (m_pLensFlare)
	{
		m_pLensFlare->SetVisible(m_bVisible);
	}
}