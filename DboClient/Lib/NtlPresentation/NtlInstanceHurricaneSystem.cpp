//***********************************************************************************
//	File		:	NtlInstanceHurricaneSystem.cpp
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#include "precomp_ntlpresentation.h"
#include ".\ntlinstanceHurricanesystem.h"
#include "NtlPLRenderState.h"

#include ".\ntleffectsystemfreelist.h"

#include "NtlPLGlobal.h"
#include "NtlMath.h"

#define INTERPOLATE_COUNT 10

void* CNtlInstanceHurricaneSystem::operator new(size_t size)
{
	NTL_FUNCTION("CNtlInstanceHurricaneSystem::operator new");
    
	NTL_RETURN(CNtlEffectSystemFreeList::Alloc(CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_HURRICANE));
}

void CNtlInstanceHurricaneSystem::operator delete(void *pObj)
{
	CNtlEffectSystemFreeList::Free(CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_HURRICANE, pObj);
}

CNtlInstanceHurricaneSystem::CNtlInstanceHurricaneSystem(void)
{
	Init();
}

CNtlInstanceHurricaneSystem::~CNtlInstanceHurricaneSystem(void)
{
	Delete();
}


//------------------------------------------------------------------
//	FuncName	: Init
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceHurricaneSystem::Init()
{
	CNtlInstanceComponentSystem::Init();

	m_pResourceHurricaneSystem	= NULL;

	m_fDeltaAlpha		= 0.f;
	m_fHurricaneAngle	= 0.f;
	m_fDeltaAngle		= 0.f;

	m_nStartPoint		= 0;
	m_nCurrentPoint		= 0;    

    m_fTemp = 0.0f;

    m_uiMemoryUseSize = sizeof(CNtlInstanceHurricaneSystem);
}

//------------------------------------------------------------------
//	FuncName	: Delete
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceHurricaneSystem::Reset()
{
	Delete();
}

//------------------------------------------------------------------
//	FuncName	: Delete
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceHurricaneSystem::Delete()
{
    CNtlInstanceImVertexSystem::Delete();
}


//------------------------------------------------------------------
//	FuncName	: Create
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlInstanceHurricaneSystem::Create(CNtlResourceEffect* pResourceEffect, CNtlResourceComponentSystem* pResourceComponentSystem, const RwMatrix& matWorld)
{
	CNtlInstanceImVertexSystem::Create(pResourceEffect, pResourceComponentSystem, matWorld);
    
	m_pResourceHurricaneSystem = (CNtlResourceHurricaneSystem*) pResourceComponentSystem;

	m_nVertexCount = m_pResourceHurricaneSystem->m_EmitterStandard.maxPolygonCount;	
    m_pVertices = NTL_NEW RwIm3DVertex[m_nVertexCount];
	NTL_ASSERTE(m_pVertices);
    if(!m_pVertices) return FALSE;
    m_uiMemoryUseSize += sizeof(RwIm3DVertex) * m_nVertexCount;
    ZeroMemory(m_pVertices, sizeof(RwIm3DVertex) * m_nVertexCount);

	m_pCurrentTexture = m_pStandardTexture = CreateTexture(m_pResourceHurricaneSystem->m_strTextureName);

	BuildEmitterStandard(&m_pResourceHurricaneSystem->m_EmitterStandard);

    // 첫 위치는 원점으로 설정한다.
    //RwIm3DVertexSetPos(&m_pVertices[m_nCurrentPoint * 2 + 0], 0.0f, 0.0f, 0.0f);
    //RwIm3DVertexSetPos(&m_pVertices[m_nCurrentPoint * 2 + 1], 0.0f, 0.0f, 0.0f);

    //RwIm3DVertexSetU(&m_pVertices[m_nCurrentPoint * 2 + 0], 0.0f);
    //RwIm3DVertexSetV(&m_pVertices[m_nCurrentPoint * 2 + 0], 0.f);

    //RwIm3DVertexSetU(&m_pVertices[m_nCurrentPoint * 2 + 1], 0.0f);
    //RwIm3DVertexSetV(&m_pVertices[m_nCurrentPoint * 2 + 1], 1.f);

    //RwIm3DVertexSetRGBA(&m_pVertices[m_nCurrentPoint * 2 + 0], m_pResourceHurricaneSystem->m_EmitterStandard.prtColor.red,
    //    m_pResourceHurricaneSystem->m_EmitterStandard.prtColor.green,
    //    m_pResourceHurricaneSystem->m_EmitterStandard.prtColor.blue,
    //    m_pResourceHurricaneSystem->m_EmitterStandard.prtColor.alpha);

    //RwIm3DVertexSetRGBA(&m_pVertices[m_nCurrentPoint * 2 + 1], m_pResourceHurricaneSystem->m_EmitterStandard.prtColor.red,
    //    m_pResourceHurricaneSystem->m_EmitterStandard.prtColor.green,
    //    m_pResourceHurricaneSystem->m_EmitterStandard.prtColor.blue,
    //    m_pResourceHurricaneSystem->m_EmitterStandard.prtColor.alpha);

    //++m_nCurrentPoint;

	return TRUE;
}


//------------------------------------------------------------------
//	FuncName	: SetVertexColor
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceHurricaneSystem::SetVertexColor(RwRGBA& color)
{
    // NOTE: 알파값은 바꾸질 않는다. (허리케인의 특성상 버텍스들의 알파값이 전부다르기 때문)
	RwInt32 nAlpha;
	for(RwInt32 i = 0; i < m_nVertexCount; ++i)
	{
		nAlpha = m_pVertices[i].color >> 24;
		RwIm3DVertexSetRGBA(&m_pVertices[i], color.red,  color.green, color.blue, nAlpha);
	}

    m_sColor = color;
}

void CNtlInstanceHurricaneSystem::SetAlpha( RwUInt8 byValue ) 
{
    // NOTE: 허리케인에서 SetVertexColor 메소드는 알파값을 변경하지 않는다.
    // 알파값을 변경할때는 이 메소드를 사용한다.

    m_sColor.alpha = byValue;

    for(int i = 0; i < m_nVertexCount; ++i)
    {
        RwIm3DVertexSetRGBA(&m_pVertices[i], m_sColor.red, m_sColor.green, m_sColor.blue, m_sColor.alpha);
    }
}

//------------------------------------------------------------------
//	FuncName	: BuildEmitterStandard
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceHurricaneSystem::BuildEmitterStandard(SNtlHurricaneEmitterStandard* pEmitterStandard)
{
	if (!m_pResourceHurricaneSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE))
	{
		m_vImVertexSize = pEmitterStandard->prtSize;
	}

	if (!m_pResourceHurricaneSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))
	{
		SetVertexColor(m_pResourceHurricaneSystem->m_EmitterStandard.prtColor);
	}
}

//------------------------------------------------------------------
//	FuncName	: Update
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlInstanceHurricaneSystem::Update(RwReal fElapsedTime)
{
    CNtlInstanceImVertexSystem::Update(fElapsedTime);

	m_fLifeTime += fElapsedTime;
	if (m_bStart)
	{
		if (m_pResourceHurricaneSystem->m_fStartTime < m_fLifeTime)
		{
			m_bStart	= FALSE;
			m_bReady	= TRUE;
			m_bUpdate	= TRUE;

			// 스타트가 되면 라이프 타임을 초기화 한다.
			m_fLifeTime = fElapsedTime;
		}
		else
		{
			return TRUE;
		}
	}

	if (m_bUpdate && m_pResourceHurricaneSystem->m_bEternity == FALSE && m_fLifeTime >= m_pResourceHurricaneSystem->m_fLifeTime)
	{
		Stop();
	}

	if (m_bReady)
	{
		RwReal fDeltaTime = m_fLifeTime / m_pResourceHurricaneSystem->m_fLifeTime;
		if (m_pResourceHurricaneSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE))
		{
			UpdateSize(fDeltaTime);
		}
		if (m_pResourceHurricaneSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))
		{
			UpdateColor(fDeltaTime);
		}
		if (m_pResourceHurricaneSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTITEXTURES))
		{
			UpdateMultiTexture(fElapsedTime);
		}

        if(m_bUpdate)
        {
            UpdateVertices(fElapsedTime, m_fLifeTime);
        }
        
	}
	if (m_bUpdate == FALSE)
	{
        if(m_pResourceHurricaneSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTFINISH))
        {
            m_bReady = UpdateFinish(fElapsedTime);
        }
        else
        {
            m_bReady = FALSE;
        }        
	}

	return m_bReady;
}

//------------------------------------------------------------------
//	FuncName	: Render
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlInstanceHurricaneSystem::Render()
{
	if (!m_bReady || !m_bVisible) return TRUE;

    RwD3D9SetTexture(m_pCurrentTexture, 0);

	BeginEffectHurricaneSystem(m_pResourceHurricaneSystem->m_bAlphaBlend, m_pResourceHurricaneSystem->m_nSrcBlend, m_pResourceHurricaneSystem->m_nDestBlend);

    //RwRenderStateSet(rwRENDERSTATECULLMODE,					(void *)rwCULLMODECULLFRONT);

	if (m_nCurrentPoint - m_nStartPoint > 1)
	{
		if(RwIm3DTransform(&m_pVertices[m_nStartPoint * 2], (m_nCurrentPoint - m_nStartPoint) * 2, &m_matRender, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA|rwIM3D_VERTEXUV))
		{
			RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);
			RwIm3DEnd();
		}
	}

    //RwRenderStateSet(rwRENDERSTATECULLMODE,					(void *)rwCULLMODECULLBACK);

    //if (m_nCurrentPoint - m_nStartPoint > 3)
    //{
    //    if(RwIm3DTransform(&m_pVertices[m_nStartPoint * 2], (m_nCurrentPoint - m_nStartPoint) * 2, &m_matRender, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA|rwIM3D_VERTEXUV))
    //    {
    //        RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);
    //        RwIm3DEnd();
    //    }
    //}

	EndEffectHurricaneSystem(m_pResourceHurricaneSystem->m_bAlphaBlend);

	return TRUE;
}


//------------------------------------------------------------------
//	FuncName	: UpdateVertices
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceHurricaneSystem::UpdateVertices(RwReal fElapsedTime, RwReal fLifeTime)
{
	if (!m_pResourceHurricaneSystem->m_EmitterStandard.bEternity)
	{
        UpdateVanish(fElapsedTime);
	}

	if (m_nCurrentPoint > (m_pResourceHurricaneSystem->m_EmitterStandard.maxPolygonCount / 2)-1) return;

	// ===============================================================================
	// 폴리곤 조각 각도 계산
	// ===============================================================================
	RwReal fFramePerSecond = (RwReal)m_pResourceHurricaneSystem->m_EmitterStandard.revolutionCount * 360.f;
	m_fDeltaAngle += (fElapsedTime * fFramePerSecond);

	RwMatrix matRotate;    
	RwV3d p1, p2;

    RwMatrixRotate(&matRotate, &VEC_Z, m_fDeltaAngle, rwCOMBINEREPLACE);
    RwV3d vUp = *RwMatrixGetUp(&matRotate);    

    //p1.y = m_vImVertexSize.x * 0.5f;
    //p2.y = -m_vImVertexSize.x * 0.5f;    
    p1.y = 0.0f;
    p1.x = m_pResourceHurricaneSystem->m_EmitterStandard.radius;
    p1.z = m_fLifeTime * m_pResourceHurricaneSystem->m_EmitterStandard.fSpeed;

    p2.x = m_pResourceHurricaneSystem->m_EmitterStandard.radius;    
    p2.z = p1.z;

    RwV3dTransformPoint(&p1, &p1, &matRotate);
    //RwV3dTransformPoint(&p2, &p2, &matRotate);

    m_vPoint.push_back(p1);    

    if(m_vPoint.size() < 2)
    {
        m_vPrev = p1;
        return;
    }

    RwV3d vDir = p1 - m_vPrev;
    RwV3dNormalize(&vDir, &vDir);
    RwV3d vRight;
    RwV3dCrossProduct(&vRight, &vDir, &vUp);
    RwV3dNormalize(&vRight, &vRight);

    RwV3d vPoint[2];
    vPoint[0] = m_vPrev + (vRight * m_vImVertexSize.x * 0.5f);
    vPoint[1] = m_vPrev - (vRight * m_vImVertexSize.x * 0.5f);

    for(int i = 0; i < 2; ++i)
    {
        RwIm3DVertexSetPos(&m_pVertices[m_nCurrentPoint * 2 + i], vPoint[i].x, vPoint[i].y, vPoint[i].z);        

        RwIm3DVertexSetU(&m_pVertices[m_nCurrentPoint * 2 + i], fLifeTime);
        RwIm3DVertexSetV(&m_pVertices[m_nCurrentPoint * 2 + i], (RwReal)i);

        RwIm3DVertexSetRGBA(&m_pVertices[m_nCurrentPoint * 2 + i], m_pResourceHurricaneSystem->m_EmitterStandard.prtColor.red,
            m_pResourceHurricaneSystem->m_EmitterStandard.prtColor.green,
            m_pResourceHurricaneSystem->m_EmitterStandard.prtColor.blue,
            m_pResourceHurricaneSystem->m_EmitterStandard.prtColor.alpha);
    }

    ++m_nCurrentPoint;
    m_vPrev = p1;

    //RwIm3DVertexSetPos(&m_pVertices[m_nCurrentPoint * 2 + 0], p1.x, p1.y, p1.z);
    //RwIm3DVertexSetPos(&m_pVertices[m_nCurrentPoint * 2 + 1], p2.x, p2.y, p2.z);

    //RwIm3DVertexSetU(&m_pVertices[m_nCurrentPoint * 2 + 0], fLifeTime);
    //RwIm3DVertexSetV(&m_pVertices[m_nCurrentPoint * 2 + 0], 0.f);

    //RwIm3DVertexSetU(&m_pVertices[m_nCurrentPoint * 2 + 1], fLifeTime);
    //RwIm3DVertexSetV(&m_pVertices[m_nCurrentPoint * 2 + 1], 1.f);

    //RwIm3DVertexSetRGBA(&m_pVertices[m_nCurrentPoint * 2 + 0], m_pResourceHurricaneSystem->m_EmitterStandard.prtColor.red,
    //    m_pResourceHurricaneSystem->m_EmitterStandard.prtColor.green,
    //    m_pResourceHurricaneSystem->m_EmitterStandard.prtColor.blue,
    //    m_pResourceHurricaneSystem->m_EmitterStandard.prtColor.alpha);

    //RwIm3DVertexSetRGBA(&m_pVertices[m_nCurrentPoint * 2 + 1], m_pResourceHurricaneSystem->m_EmitterStandard.prtColor.red,
    //    m_pResourceHurricaneSystem->m_EmitterStandard.prtColor.green,
    //    m_pResourceHurricaneSystem->m_EmitterStandard.prtColor.blue,
    //    m_pResourceHurricaneSystem->m_EmitterStandard.prtColor.alpha);

    //++m_nCurrentPoint;

	// 각도로 계산한다.
	//while(m_fDeltaAngle >=m_pResourceHurricaneSystem->m_EmitterStandard.revolutionAngle)
	//{
	//	m_fDeltaAngle -= m_pResourceHurricaneSystem->m_EmitterStandard.revolutionAngle;
	//	m_fHurricaneAngle += m_pResourceHurricaneSystem->m_EmitterStandard.revolutionAngle;

	//	RwMatrixRotate(&matRotate, &VEC_Z, m_fHurricaneAngle, rwCOMBINEREPLACE);

	//	//RwV3d vEnd = { 0, 0, 0 };
	//	//vEnd.z = m_fLifeTime * m_pResourceHurricaneSystem->m_EmitterStandard.fSpeed;

	//	//p1 = *RwMatrixGetUp(&matRotate);
	//	//p2 = *RwMatrixGetUp(&matRotate);

	//	//RwV3dScale(&p1, &p1, m_pResourceHurricaneSystem->m_EmitterStandard.radius * m_vImVertexSize.x);
	//	//RwV3dScale(&p2, &p2, m_pResourceHurricaneSystem->m_EmitterStandard.radius * m_vImVertexSize.x + m_pResourceHurricaneSystem->m_EmitterStandard.radiusGap * m_vImVertexSize.x);

 // //      p1 += vEnd;
 // //      p2 += vEnd;

 //       p1.y = m_vImVertexSize.x * 0.5f;
 //       p2.y = -m_vImVertexSize.x * 0.5f;
 //       p1.x = m_pResourceHurricaneSystem->m_EmitterStandard.radius;
 //       p2.x = m_pResourceHurricaneSystem->m_EmitterStandard.radius;
 //       p1.z = m_fLifeTime * m_pResourceHurricaneSystem->m_EmitterStandard.fSpeed;
 //       p2.z = p1.z;

 //       RwV3dTransformPoint(&p1, &p1, &matRotate);
 //       RwV3dTransformPoint(&p2, &p2, &matRotate);

 //       RwIm3DVertexSetPos(&m_pVertices[m_nCurrentPoint * 2 + 0], p1.x, p1.y, p1.z);
 //       RwIm3DVertexSetPos(&m_pVertices[m_nCurrentPoint * 2 + 1], p2.x, p2.y, p2.z);

 //       RwIm3DVertexSetU(&m_pVertices[m_nCurrentPoint * 2 + 0], fLifeTime);
 //       RwIm3DVertexSetV(&m_pVertices[m_nCurrentPoint * 2 + 0], 0.f);

 //       RwIm3DVertexSetU(&m_pVertices[m_nCurrentPoint * 2 + 1], fLifeTime);
 //       RwIm3DVertexSetV(&m_pVertices[m_nCurrentPoint * 2 + 1], 1.f);

 //       RwIm3DVertexSetRGBA(&m_pVertices[m_nCurrentPoint * 2 + 0], m_pResourceHurricaneSystem->m_EmitterStandard.prtColor.red,
 //                                                                  m_pResourceHurricaneSystem->m_EmitterStandard.prtColor.green,
 //                                                                  m_pResourceHurricaneSystem->m_EmitterStandard.prtColor.blue,
 //                                                                  m_pResourceHurricaneSystem->m_EmitterStandard.prtColor.alpha);
 //       RwIm3DVertexSetRGBA(&m_pVertices[m_nCurrentPoint * 2 + 1], m_pResourceHurricaneSystem->m_EmitterStandard.prtColor.red,
 //                                                                  m_pResourceHurricaneSystem->m_EmitterStandard.prtColor.green,
 //                                                                  m_pResourceHurricaneSystem->m_EmitterStandard.prtColor.blue,
 //                                                                  m_pResourceHurricaneSystem->m_EmitterStandard.prtColor.alpha);

 //       if (++m_nCurrentPoint > (m_nVertexCount / 2)-1)
 //       {
 //           break;
 //       }  
	//}
}
void CNtlInstanceHurricaneSystem::UpdateVanish( RwReal fElapsedTime ) 
{
    RwReal fManus = m_pResourceHurricaneSystem->m_EmitterStandard.lifeTime / 255.f;
    m_fDeltaAlpha += fElapsedTime;

    // ===============================================================================
    // 알파 마이너스 하기
    // ===============================================================================
    if (m_fDeltaAlpha > fManus)
    {
        RwInt32 nAlpha;
        for (RwInt32 i = m_nStartPoint; i < m_nCurrentPoint; ++ i)
        {
            nAlpha			= m_pVertices[i * 2 + 0].color >> 24;
            nAlpha			-= 5;
            if (nAlpha < 0)
            {
                nAlpha = 0;
                ++ m_nStartPoint;
            }
            m_pVertices[i * 2 + 0].color = (m_pVertices[i * 2 + 0].color & 0x00ffffff) | (nAlpha << 24);
            m_pVertices[i * 2 + 1].color = (m_pVertices[i * 2 + 1].color & 0x00ffffff) | (nAlpha << 24);
        }

        m_fDeltaAlpha -= fManus;
    }
}

RwBool CNtlInstanceHurricaneSystem::UpdateFinish( RwReal fElapsedTime ) 
{
    if(m_pResourceHurricaneSystem->m_EmitterPrtFinish.m_nFinishTypeFlag & FINISH_TYPE_ALPHA)
    {
        UpdateFinishAlpha(m_pResourceHurricaneSystem->m_EmitterStandard.prtColor.alpha, fElapsedTime, m_pResourceHurricaneSystem->m_EmitterPrtFinish.m_fFinishTime);
    }

    if(m_pResourceHurricaneSystem->m_EmitterPrtFinish.m_nFinishTypeFlag & FINISH_TYPE_SIZE)
    {
        UpdateFinishSize(m_pResourceHurricaneSystem->m_EmitterStandard.prtSize, fElapsedTime, m_pResourceHurricaneSystem->m_EmitterPrtFinish.m_fFinishTime);
    }

    if(m_pResourceHurricaneSystem->m_EmitterPrtFinish.m_nFinishTypeFlag & FINISH_TYPE_FOLLOW)
    {
        RwInt32 nMinusCnt = (RwInt32)(((RwReal)m_nCurrentPoint * fElapsedTime / m_pResourceHurricaneSystem->m_EmitterPrtFinish.m_fFinishTime) + 0.5f);
        if(nMinusCnt < 1)
        {
            nMinusCnt = 1;
        }

        if(m_nCurrentPoint - (m_nStartPoint + nMinusCnt) > 2)
        {
            m_nStartPoint += nMinusCnt;
        }        
    }

    m_fFinishTime += fElapsedTime;
    if(m_fFinishTime >= m_pResourceHurricaneSystem->m_EmitterPrtFinish.m_fFinishTime)
    {
        return FALSE;
    }

    return TRUE;
}

