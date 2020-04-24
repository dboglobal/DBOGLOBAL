//***********************************************************************************
//	File		:	NtlInstancePostEffectSystem.cpp
//	Desc		:	
//	Begin		:	2007. 1.17
//	Copyright	:	ⓒ 2007 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************


#include "precomp_ntlpresentation.h"
#include "NtlInstancePostEffectSystem.h"

#include ".\ntleffectsystemfreelist.h"

#include "NtlPostEffectCamera.h"
#include "NtlPLRenderState.h"
#include "NtlPLApi.h"

void* CNtlInstancePostEffectSystem::operator new(size_t size)
{
	NTL_FUNCTION("CNtlInstanceEffect::operator new");
	NTL_RETURN(CNtlEffectSystemFreeList::Alloc(CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_POST_EFFECT));
}

void CNtlInstancePostEffectSystem::operator delete(void *pObj)
{
	CNtlEffectSystemFreeList::Free(CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_POST_EFFECT, pObj);
}

CNtlInstancePostEffectSystem::CNtlInstancePostEffectSystem(void)
{
	Init();
}

CNtlInstancePostEffectSystem::~CNtlInstancePostEffectSystem(void)
{
	Delete();
}


//------------------------------------------------------------------
//	FuncName	: Init
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstancePostEffectSystem::Init()
{
	CNtlInstanceComponentSystem::Init();

	m_pResourcePostEffectSystem	= NULL;
	m_pStandardTexture			= NULL;
    m_nVertexCount              = 0;
    m_nIndexCount               = 0;
    m_pComicVertices            = NULL;    
    m_pComicHalfIndex           = NULL;
    m_nCurLoopCount             = 0;        
    m_bHalfIndexType            = FALSE;
    m_nComicState               = COMIC_STATE_FADE_OUT;
    m_fComicAlpha               = 0.0f;
    m_bCenterFix                = FALSE;
    m_bUpdateCenter             = TRUE;
    m_uiMemoryUseSize           = sizeof(CNtlInstancePostEffectSystem);
}

//------------------------------------------------------------------
//	FuncName	: Delete
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstancePostEffectSystem::Reset()
{
	Delete();
}

//------------------------------------------------------------------
//	FuncName	: Delete
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstancePostEffectSystem::Delete()
{
    NTL_ARRAY_DELETE(m_pComicVertices);
    NTL_ARRAY_DELETE(m_pComicHalfIndex);

    // 삭제시 Camera 클래스에서 등록을 해제한다.
    if(CNtlPostEffectCamera::GetInstance())
    {
        CNtlPostEffectCamera::GetInstance()->DetachPostEffectSystem(this);
    }    

	CNtlInstanceComponentSystem::Delete();
}


//------------------------------------------------------------------
//	FuncName	: Create
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlInstancePostEffectSystem::Create(CNtlResourceEffect* pResourceEffect, CNtlResourceComponentSystem* pResourceComponentSystem, const RwMatrix& matWorld)
{
	CNtlInstanceComponentSystem::Create(pResourceEffect, pResourceComponentSystem, matWorld);

	m_pResourcePostEffectSystem = (CNtlResourcePostEffectSystem*) pResourceComponentSystem;

	BuildEmitterStandard(&m_pResourcePostEffectSystem->m_EmitterStandard);

	if (!m_pResourcePostEffectSystem->m_strTextureName.empty())
	{
		m_pCurrentTexture = m_pStandardTexture = CreateTexture(m_pResourcePostEffectSystem->m_strTextureName);
	}

    SetComicVertex();
    SetComicIndex();

    // Center Fix   (모델툴에서 Post Effect Event로 사용하면, 이벤트에서 다시 재정의 된다)
    if(pResourceComponentSystem->m_nEmitterDataFlag & NTLrpPRTSTDEMITTERDATAFLAGPRTCENTERFIX)
    {
        m_bCenterFix = TRUE;
    }
    else
    {
        m_bCenterFix = FALSE;
    }


	return TRUE;
}

void CNtlInstancePostEffectSystem::BuildEmitterStandard(SNtlPostEffectEmitterStandard* pEmitterStandard)
{
    m_nVertexCount = pEmitterStandard->nVertexCount;
    m_pComicVertices = NTL_NEW RwIm2DVertex[m_nVertexCount + 2];
    ZeroMemory(m_pComicVertices, sizeof(RwIm2DVertex) * (m_nVertexCount + 2));
    m_uiMemoryUseSize += sizeof(RwIm2DVertex) * (m_nVertexCount + 2);
    
    // 중심점 좌표 초기화 
    m_v2dCenterPos.x = CNtlPostEffectCamera::GetInstance()->GetScreenSize().x * 0.5f;
    m_v2dCenterPos.y = CNtlPostEffectCamera::GetInstance()->GetScreenSize().y * 0.5f;
}

void CNtlInstancePostEffectSystem::SetComicVertex() 
{
    RwRGBA color;
    RwRGBAFromRwRGBAReal(&color, &m_pResourcePostEffectSystem->m_EmitterPrtCol.prtStartCol);

    RwReal recipCameraZ = 1.0f / RwCameraGetNearClipPlane(CNtlPostEffectCamera::GetInstance()->GetCamera());

    RwIm2DVertexSetScreenZ(&m_pComicVertices[0], RwIm2DGetNearScreenZ());
    RwIm2DVertexSetRecipCameraZ(&m_pComicVertices[0], recipCameraZ);
    RwIm2DVertexSetIntRGBA(&m_pComicVertices[0], color.red, color.green, color.blue, color.alpha);
    
    
    RwIm2DVertexSetU(&m_pComicVertices[0], 1.0f, recipCameraZ);
    RwIm2DVertexSetV(&m_pComicVertices[0], 1.0f, recipCameraZ);

    for (RwInt32 i = 1; i < m_nVertexCount + 2; ++i)
    {
        RwIm2DVertexSetScreenZ(&m_pComicVertices[i], RwIm2DGetNearScreenZ());
        RwIm2DVertexSetRecipCameraZ(&m_pComicVertices[i], recipCameraZ);
        RwIm2DVertexSetIntRGBA(&m_pComicVertices[i], color.red, color.green, color.blue, color.alpha);

        if (i % 2 == 0)
        {
            RwIm2DVertexSetU(&m_pComicVertices[i], 0.f, recipCameraZ);
            RwIm2DVertexSetV(&m_pComicVertices[i], 1.f, recipCameraZ);
        }
        else
        {
            RwIm2DVertexSetU(&m_pComicVertices[i], 0.f, recipCameraZ);
            RwIm2DVertexSetV(&m_pComicVertices[i], 0.f, recipCameraZ);
        }
    }
}

void CNtlInstancePostEffectSystem::SetComicIndex() 
{
    if(m_pResourcePostEffectSystem->m_EmitterStandard.bDrawFullScreen)
    {
        // Full Screen의 경우 Index가 아닌 Fan으로 그린다
    }
    else
    {
        m_nIndexCount = m_nVertexCount / 2 * 3;
        m_pComicHalfIndex = NTL_NEW RwImVertexIndex[m_nIndexCount];
        ZeroMemory(m_pComicHalfIndex, sizeof(RwImVertexIndex) * m_nIndexCount);
        m_uiMemoryUseSize += sizeof(RwImVertexIndex) * m_nIndexCount;

        int nCount = 1;
        for(int i = 0; i < m_nIndexCount; i += 3)
        {
            m_pComicHalfIndex[i] = 0;
            m_pComicHalfIndex[i + 1] = nCount++;
            m_pComicHalfIndex[i + 2] = nCount++;
        }
    }    
}

//------------------------------------------------------------------
//	FuncName	: Update
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlInstancePostEffectSystem::Update(RwReal fElapsedTime)
{
    if(!m_pResourcePostEffectSystem)
        return FALSE;

	m_fLifeTime += fElapsedTime;
	if (m_bStart)
	{
		if (m_pResourcePostEffectSystem->m_fStartTime < m_fLifeTime)
		{
			m_bStart	= FALSE;
			m_bReady	= TRUE;
			m_bUpdate	= TRUE;

			// 스타트가 되면 라이프 타임을 초기화 한다.
			m_fLifeTime = fElapsedTime;

            RebuildPostEffectVertex();

            // Camera 객체 등록한다.
            CNtlPostEffectCamera::GetInstance()->AttachPostEffectSystem(this);
		}
		else
		{
			return TRUE;
		}
	}

	if (m_bUpdate && m_pResourcePostEffectSystem->m_bEternity == FALSE && m_fLifeTime >= m_pResourcePostEffectSystem->m_fLifeTime)
	{
		Stop();
	}

	if (m_bReady)
	{
        UpdateVertex(fElapsedTime);

        // Multi Texture Action
        if(m_pResourcePostEffectSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTITEXTURES))
        {
            UpdateMultiTexture(fElapsedTime);
        }
	}

	if (m_bUpdate == FALSE)
	{
		m_bReady = FALSE;
	}

	return m_bReady;
}

//------------------------------------------------------------------
//	FuncName	: Render
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlInstancePostEffectSystem::Render()
{
    // Visual Manager에서 호출하는 Render는 사용하지 않는다.
    // 대신 Camera에서 RenderPostEffect를 호출한다.
	if (!m_bReady) return TRUE;

	return TRUE;
}

void CNtlInstancePostEffectSystem::RebuildPostEffectVertex() 
{
    if(!m_pResourcePostEffectSystem)
        return;

    RwV2d v2dScreenSize = CNtlPostEffectCamera::GetInstance()->GetScreenSize();

    RwReal fRadius = 0.0f;
    if(m_pResourcePostEffectSystem->m_EmitterStandard.bDrawFullScreen)
    {
        // Full Screen인경우에는 반지름을 중심점과 화면의 4귀퉁이들과의 거리를 구해서 가장 긴 거리로 설정한다.
        RwV2d v2dEndPoint[4];
        v2dEndPoint[0].x = v2dEndPoint[0].y = 0.0f;
        v2dEndPoint[1].x = 0.0f;
        v2dEndPoint[1].y = v2dScreenSize.y;
        v2dEndPoint[2].x = v2dScreenSize.x;
        v2dEndPoint[2].y = 0.0f;
        v2dEndPoint[3] = v2dScreenSize;
        
        RwV2d vSub;
        for(int i = 0; i < 4; ++i)
        {
            RwV2dSub(&vSub, &v2dEndPoint[i], &m_v2dCenterPos);
            RwReal fLength = RwV2dLength(&vSub);

            if(fLength > fRadius)
            {
                fRadius = fLength;
            }
        }

        // 화면 전체를 채우기 위해서 약간의 Offset값을 더 넣어준다.
        fRadius += 100.0f;
    }
    else
    {
        fRadius = m_pResourcePostEffectSystem->m_EmitterStandard.fRadius;
    }

    RwIm2DVertexSetScreenX(&m_pComicVertices[0], m_v2dCenterPos.x);
    RwIm2DVertexSetScreenY(&m_pComicVertices[0], m_v2dCenterPos.y);

    RwReal constant = (2.0f * rwPI) / m_nVertexCount;

    for(RwInt32 i = 1; i < m_nVertexCount + 1; ++i)
    {
        RwReal rw = 0.0f, rh = 0.0f;

        //if(!m_pResourcePostEffectSystem->m_EmitterStandard.bDrawFullScreen)
        //{
            rw = (constant * (((rand() % 80) - 40) / 100.f));
            rh = (constant * (((rand() % 80) - 40) / 100.f));
        //}

        RwIm2DVertexSetScreenX(&m_pComicVertices[i], (RwReal)RwCos((i-1) * -constant + rw) * fRadius + m_v2dCenterPos.x);
        RwIm2DVertexSetScreenY(&m_pComicVertices[i], (RwReal)RwSin((i-1) * -constant + rh) * fRadius + m_v2dCenterPos.y);
    }

    RwIm2DVertexSetScreenX(&m_pComicVertices[m_nVertexCount+1], m_pComicVertices[1].x);
    RwIm2DVertexSetScreenY(&m_pComicVertices[m_nVertexCount+1], m_pComicVertices[1].y);
}

void CNtlInstancePostEffectSystem::UpdateVertex( RwReal fElapsedTime ) 
{
    if (m_nComicState == COMIC_STATE_FADE_OUT)
    {
        m_fComicAlpha += (fElapsedTime * 255.f) / (m_pResourcePostEffectSystem->m_fLifeTime / 2.f) * m_pResourcePostEffectSystem->m_EmitterStandard.nLoopCount;
        if (m_fComicAlpha >= 255.f)
        {
            m_fComicAlpha = 255.f;
            m_nComicState = COMIC_STATE_FADE_IN;
        }
    }
    else
    {
        m_fComicAlpha -= (fElapsedTime * 255.f) / (m_pResourcePostEffectSystem->m_fLifeTime / 2.f) * m_pResourcePostEffectSystem->m_EmitterStandard.nLoopCount;
        if (m_fComicAlpha <= 0.f)
        {
            m_fComicAlpha = 0.f; 
            ++ m_nCurLoopCount;
            if (m_nCurLoopCount >= m_pResourcePostEffectSystem->m_EmitterStandard.nLoopCount && !m_pResourcePostEffectSystem->m_bEternity)
            {
                m_nComicState = COMIC_STATE_IDLE;
            }
            else
            {
                m_nComicState = COMIC_STATE_FADE_OUT;
                RebuildPostEffectVertex();
                if (!m_pResourcePostEffectSystem->m_EmitterStandard.bDrawFullScreen)
                {
                    m_bHalfIndexType = rand() % 2;
                }
            }
        }
    }

    RwRGBA color;
    color.red = color.green = color.blue = 255;

    // Update Color
    if(m_pResourcePostEffectSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))    
    {
        color = UpdateColor(m_fLifeTime / m_pResourcePostEffectSystem->m_fLifeTime);
    }

    for (RwInt32 i = 0; i < m_nVertexCount + 2; ++i)
    {
        RwIm2DVertexSetIntRGBA(&m_pComicVertices[i], color.red, color.green, color.blue, (RwInt32)m_fComicAlpha); 
    }
}

RwBool CNtlInstancePostEffectSystem::RenderPostEffect() 
{
    if(!m_pResourcePostEffectSystem->m_bShow)
        return TRUE;

    if(m_bReady == FALSE)
        return TRUE;
    
    BeginEffectPostSystemState(m_pResourcePostEffectSystem->m_nSrcBlend, m_pResourcePostEffectSystem->m_nDestBlend);

    RwD3D9SetTexture(m_pCurrentTexture, 0);

    if(m_pResourcePostEffectSystem->m_EmitterStandard.bDrawFullScreen)
    {
        // Full Screen의 경우 Fan으로 그리면 된다.
        RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, m_pComicVertices, m_nVertexCount + 2);        
    }
    else
    {
        RwIm2DRenderIndexedPrimitive(rwPRIMTYPETRILIST, m_pComicVertices, m_nVertexCount + 2, m_pComicHalfIndex, m_nIndexCount);
    }

    EndEffectPostSystemState();

    return TRUE;
}

void CNtlInstancePostEffectSystem::SetWorldMatrix( const RwMatrix& matWorld ) 
{
    CNtlInstanceComponentSystem::SetWorldMatrix(matWorld);
    RwV3d vPosition = *RwMatrixGetPos(&m_matRender);

    // 중심점 위치 고정 기능
    if(!m_bCenterFix || m_bUpdateCenter)
    {
        m_v2dCenterPos = API_PL_Calc3DPosTo2D(&vPosition, (RwInt32)CNtlPostEffectCamera::GetInstance()->GetScreenSize().x, (RwInt32)CNtlPostEffectCamera::GetInstance()->GetScreenSize().y);        
        m_bUpdateCenter = FALSE;
    }       
    
    RebuildPostEffectVertex();
}

RwRGBA CNtlInstancePostEffectSystem::UpdateColor( RwReal fDeltaTime ) 
{
    RwRGBA color;
    if(m_pResourcePostEffectSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTICOLOR))
    {
        RebuildMultiColor(color, 0, fDeltaTime, &m_pResourcePostEffectSystem->m_EmitterPrtCol, &m_pResourcePostEffectSystem->m_EmitterPrtMultiCol);
    }
    else
    {
        RebuildColor(color, 0, fDeltaTime, &m_pResourcePostEffectSystem->m_EmitterPrtCol);
    }

    return color;
}

void CNtlInstancePostEffectSystem::SetCenterFixEnable( RwBool bEnable ) 
{
    m_bCenterFix = bEnable;
    m_bUpdateCenter = TRUE;
}