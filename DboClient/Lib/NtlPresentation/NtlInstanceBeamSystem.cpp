//***********************************************************************************
//	File		:	NtlInstanceBeamSystem.cpp
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#include "precomp_ntlpresentation.h"
#include ".\ntlinstancebeamsystem.h"

#include ".\ntleffectsystemfreelist.h"

#include "NtlPLGlobal.h"
#include "NtlMath.h"


RwImVertexIndex CNtlInstanceBeamSystem::indices[INDEX_COUNT] = { 0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11 };

void* CNtlInstanceBeamSystem::operator new(size_t size)
{
	NTL_FUNCTION("CNtlInstanceBeamSystem::operator new");
    
	NTL_RETURN(CNtlEffectSystemFreeList::Alloc(CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_BEAM));
}

void CNtlInstanceBeamSystem::operator delete(void *pObj)
{
	CNtlEffectSystemFreeList::Free(CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_BEAM, pObj);
}

CNtlInstanceBeamSystem::CNtlInstanceBeamSystem(void)
{
	Init();
}

CNtlInstanceBeamSystem::~CNtlInstanceBeamSystem(void)
{
	Delete();
}


//------------------------------------------------------------------
//	FuncName	: Init
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceBeamSystem::Init()
{
	CNtlInstanceComponentSystem::Init();

	m_pResourceBeamSystem	= NULL;

	m_pHeadSystem	= NULL;

	m_fStopTime = 0.f;
	m_vStartPoint.x = m_vStartPoint.y = m_vStartPoint.z = 0.f;
	m_vEndPoint.x = m_vEndPoint.y = m_vEndPoint.z = 0.f;    
}

//------------------------------------------------------------------
//	FuncName	: Delete
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceBeamSystem::Reset()
{
	Delete();
}

//------------------------------------------------------------------
//	FuncName	: Delete
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceBeamSystem::Delete()
{
	CNtlInstanceImVertexSystem::Delete();
}

//------------------------------------------------------------------
//	FuncName	: Create
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlInstanceBeamSystem::Create(CNtlResourceEffect* pResourceEffect, CNtlResourceComponentSystem* pResourceComponentSystem, const RwMatrix& matWorld)
{
	CNtlInstanceImVertexSystem::Create(pResourceEffect, pResourceComponentSystem, matWorld);    
    
	m_pResourceBeamSystem = (CNtlResourceBeamSystem*) pResourceComponentSystem;

	m_nVertexCount = VERTEX_COUNT;    
    m_pVertices = NTL_NEW RwIm3DVertex[m_nVertexCount];
    m_uiMemoryUseSize += sizeof(RwIm3DVertex) * m_nVertexCount;
	NTL_ASSERTE(m_pVertices);
    if(!m_pVertices) return FALSE;
    ZeroMemory(m_pVertices, sizeof(RwIm3DVertex) * m_nVertexCount);

	m_pCurrentTexture = m_pStandardTexture = CreateTexture(m_pResourceBeamSystem->m_strTextureName);

	BuildEmitterStandard(&m_pResourceBeamSystem->m_EmitterStandard);

	return TRUE;
}


//------------------------------------------------------------------
//	FuncName	: Stop
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceBeamSystem::Stop()
{
	m_fStopTime	= m_fLifeTime;
	m_fFadeOutTime = 0.f;
	m_bUpdate = FALSE;
}

//------------------------------------------------------------------
//	FuncName	: BuildEmitterStandard
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceBeamSystem::BuildEmitterStandard(SNtlBeamEmitterStandard* pEmitterStandard)
{
	SetVertexUv();

	if (m_pResourceBeamSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE))
	{
		m_vImVertexSize = m_pResourceBeamSystem->m_EmitterPrtSize.prtStartSize;        
	}
	else
	{
		m_vImVertexSize = pEmitterStandard->prtSize;
	}

    m_v3dFinishSize = m_vImVertexSize;

	if (m_pResourceBeamSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))
	{
		RwRGBA color = {	(RwUInt8)m_pResourceBeamSystem->m_EmitterPrtCol.prtStartCol.red, 
							(RwUInt8)m_pResourceBeamSystem->m_EmitterPrtCol.prtStartCol.green, 
							(RwUInt8)m_pResourceBeamSystem->m_EmitterPrtCol.prtStartCol.blue, 
							(RwUInt8)m_pResourceBeamSystem->m_EmitterPrtCol.prtStartCol.alpha };
		SetVertexColor(color);
	}
	else
	{
		SetVertexColor(pEmitterStandard->prtColor);
	}
}


//------------------------------------------------------------------
//	FuncName	: SetVertexUv
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceBeamSystem::SetVertexUv()
{
	RwIm3DVertexSetU(&m_pVertices[0], 0.f);
	RwIm3DVertexSetV(&m_pVertices[0], 0.5f);

	RwIm3DVertexSetU(&m_pVertices[1], 0.f);
	RwIm3DVertexSetV(&m_pVertices[1], 0.f);

	RwIm3DVertexSetU(&m_pVertices[2], 0.5f);
	RwIm3DVertexSetV(&m_pVertices[2], 0.f);

	RwIm3DVertexSetU(&m_pVertices[3], 0.5f);
	RwIm3DVertexSetV(&m_pVertices[3], 0.5f);

	RwIm3DVertexSetU(&m_pVertices[4], 0.5f);
	RwIm3DVertexSetV(&m_pVertices[4], 1.0f);

	RwIm3DVertexSetU(&m_pVertices[5], 0.5f);
	RwIm3DVertexSetV(&m_pVertices[5], 0.f);

	RwIm3DVertexSetU(&m_pVertices[6], 1.f);
	RwIm3DVertexSetV(&m_pVertices[6], 0.f);

	RwIm3DVertexSetU(&m_pVertices[7], 1.f);
	RwIm3DVertexSetV(&m_pVertices[7], 1.f);

	RwIm3DVertexSetU(&m_pVertices[8], 0.f);
	RwIm3DVertexSetV(&m_pVertices[8], 1.f);

	RwIm3DVertexSetU(&m_pVertices[9], 0.f);
	RwIm3DVertexSetV(&m_pVertices[9], 0.5f);

	RwIm3DVertexSetU(&m_pVertices[10], 0.5f);
	RwIm3DVertexSetV(&m_pVertices[10], 0.5f);

	RwIm3DVertexSetU(&m_pVertices[11], 0.5f);
	RwIm3DVertexSetV(&m_pVertices[11], 1.f);
}

//------------------------------------------------------------------
//	FuncName	: Update
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlInstanceBeamSystem::Update(RwReal fElapsedTime)
{
    CNtlInstanceImVertexSystem::Update(fElapsedTime);

	m_fLifeTime += fElapsedTime;
	if (m_bStart)
	{
		if (m_pResourceBeamSystem->m_fStartTime < m_fLifeTime)
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

	if (m_bUpdate && m_pResourceBeamSystem->m_bEternity == FALSE && m_fLifeTime >= m_pResourceBeamSystem->m_fLifeTime)
	{
		Stop();
	}

	//if (m_bReady && m_bUpdate)
    if (m_bReady)
	{
		if (m_pResourceBeamSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTITEXTURES))
		{
			UpdateMultiTexture(fElapsedTime);
		}

        if(m_bUpdate)
        {
		    m_vEndPoint.z = m_fLifeTime * m_pResourceBeamSystem->m_EmitterStandard.fSpeed;
        }

		if (m_pHeadSystem != NULL)
		{
			RwMatrix matWorld;
			RwMatrixTranslate(&matWorld, &m_vEndPoint, rwCOMBINEREPLACE);

			m_pHeadSystem->SetAttachMatrix(matWorld);
		}

		UpdateVertices();
	}

	if (!m_bUpdate)
	{
		m_fFadeOutTime += fElapsedTime;
		if (m_pResourceBeamSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTFADEOUT))
		{
			if (m_fFadeOutTime < m_pResourceBeamSystem->m_EmitterPrtFadeOut.time)
			{
				RwReal fDeltaTime = m_fFadeOutTime / m_pResourceBeamSystem->m_EmitterPrtFadeOut.time;
				if (m_pResourceBeamSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE))
				{
					UpdateSize(fDeltaTime);
				}
				if (m_pResourceBeamSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))
				{
					UpdateColor(fDeltaTime);
				}

				if (m_pResourceBeamSystem->m_EmitterPrtFadeOut.follow)
				{
					m_vStartPoint.z = m_fStopTime * (m_fFadeOutTime * m_pResourceBeamSystem->m_EmitterStandard.fSpeed);
					if (m_pFadeOutSystem != NULL)
					{
						RwMatrix matWorld;
						RwMatrixTranslate(&matWorld, &m_vStartPoint, rwCOMBINEREPLACE);

						m_pFadeOutSystem->SetAttachMatrix(matWorld);
					}
				}
				UpdateVertices();
			}
			else
			{
				if (m_pFadeOutSystem != NULL)
				{
					m_pFadeOutSystem->Stop();
				}

                m_bReady = FALSE;
			}
		}

        if(m_pResourceBeamSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTFINISH))
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
RwBool CNtlInstanceBeamSystem::Render()
{
	if (!m_bReady || !m_bVisible) return TRUE;

	if(RwIm3DTransform(m_pVertices, VERTEX_COUNT, NULL, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA|rwIM3D_VERTEXUV))
	{
		BeginRenderState(m_pResourceBeamSystem);
	
		RwIm3DRenderIndexedPrimitive(rwPRIMTYPETRILIST, indices, INDEX_COUNT);
		RwIm3DEnd();

		EndRenderState(m_pResourceBeamSystem);
	}

	return TRUE;
}

//------------------------------------------------------------------
//	FuncName	: UpdateVertices
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceBeamSystem::UpdateVertices()
{
	RwV3d vWorldStartPoint, vWorldEndPoint;

	RwV3dTransformPoint(&vWorldStartPoint, &m_vStartPoint, &m_matRender);
	RwV3dTransformPoint(&vWorldEndPoint, &m_vEndPoint, &m_matRender);

	RwFrame *pFrame = RwCameraGetFrame(CNtlPLGlobal::m_RwCamera);
    RwMatrix *pMatrix = RwFrameGetMatrix(pFrame);
		
	RwV3d vPos		= *RwMatrixGetPos(pMatrix);
	RwV3d vFront	= *RwMatrixGetAt(pMatrix);

	RwV3d vBeamDir, vCameraDir, vUp, vRight, vBeamUp;
	RwV3dSubMacro(&vBeamDir, &vWorldEndPoint, &vWorldStartPoint);
	RwV3dSubMacro(&vCameraDir, &vPos, &vWorldStartPoint);

	RwV3dCrossProduct(&vBeamUp, &vCameraDir, &vBeamDir);     
	RwV3dNormalize(&vBeamUp, &vBeamUp);
	RwV3dCrossProduct(&vUp, &vBeamUp, &vFront);
	RwV3dCrossProduct(&vRight, &vUp, &vFront);

	RwV3d vRenderSize;
	vRenderSize.x = RwV3dLength(RwMatrixGetRight(&m_matRender)) * m_vImVertexSize.x;
	vRenderSize.y = RwV3dLength(RwMatrixGetUp(&m_matRender)) * m_vImVertexSize.y;
	vRenderSize.z = RwV3dLength(RwMatrixGetAt(&m_matRender)) * m_vImVertexSize.z;


    m_pVertices[0].objVertex = vWorldStartPoint + vRight * vRenderSize.x + vUp * vRenderSize.x;
    m_pVertices[1].objVertex = vWorldStartPoint + vRight * vRenderSize.x - vUp * vRenderSize.x;
	m_pVertices[2].objVertex = vWorldStartPoint - vRight * vRenderSize.x - vUp * vRenderSize.x;
    m_pVertices[3].objVertex = vWorldStartPoint - vRight * vRenderSize.x + vUp * vRenderSize.x;
    m_pVertices[4].objVertex = vWorldStartPoint + vRight * vRenderSize.y;
	
    m_pVertices[5].objVertex = vWorldEndPoint + vRight * vRenderSize.y;
	m_pVertices[6].objVertex = vWorldEndPoint - vRight * vRenderSize.y;
	m_pVertices[7].objVertex = vWorldStartPoint - vRight * vRenderSize.y;
    m_pVertices[8].objVertex = vWorldEndPoint + vRight * vRenderSize.z + vUp * vRenderSize.z;
    m_pVertices[9].objVertex = vWorldEndPoint + vRight * vRenderSize.z - vUp * vRenderSize.z;
    m_pVertices[10].objVertex = vWorldEndPoint - vRight * vRenderSize.z - vUp * vRenderSize.z;	
    m_pVertices[11].objVertex = vWorldEndPoint - vRight * vRenderSize.z + vUp * vRenderSize.z;    
}

RwBool CNtlInstanceBeamSystem::UpdateFinish( RwReal fElapsedTime ) 
{
    if(m_pResourceBeamSystem->m_EmitterPrtFinish.m_nFinishTypeFlag & FINISH_TYPE_ALPHA)
    {
        UpdateFinishAlpha(m_pResourceBeamSystem->m_EmitterStandard.prtColor.alpha, fElapsedTime, m_pResourceBeamSystem->m_EmitterPrtFinish.m_fFinishTime);
    }
    if(m_pResourceBeamSystem->m_EmitterPrtFinish.m_nFinishTypeFlag & FINISH_TYPE_SIZE)
    {
        m_vImVertexSize -= m_v3dFinishSize * (fElapsedTime / m_pResourceBeamSystem->m_EmitterPrtFinish.m_fFinishTime);
    }
    if(m_pResourceBeamSystem->m_EmitterPrtFinish.m_nFinishTypeFlag & FINISH_TYPE_FOLLOW)
    {
        // Beam의 Follow는 FadeOut Action을 사용한다. (기존과의 호환성을 위해)
    }

    m_fFinishTime += fElapsedTime;

    if(m_fFinishTime >= m_pResourceBeamSystem->m_EmitterPrtFinish.m_fFinishTime)
    {
        return FALSE;
    }

    return TRUE;
}


