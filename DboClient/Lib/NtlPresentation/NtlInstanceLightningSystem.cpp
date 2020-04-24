//***********************************************************************************
//	File		:	NtlInstanceLightningSystem.cpp
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#include "precomp_ntlpresentation.h"
#include ".\ntlinstancelightningsystem.h"
#include ".\ntleffectsystemfreelist.h"
#include "NtlPLGlobal.h"
#include "NtlMath.h"
#include "NtlPLHelpers.h"
#include "NtlInstanceEffect.h"

void* CNtlInstanceLightningSystem::operator new(size_t size)
{
	NTL_FUNCTION("CNtlInstanceLightningSystem::operator new");
    
	NTL_RETURN(CNtlEffectSystemFreeList::Alloc(CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_LIGHTNING));
}

void CNtlInstanceLightningSystem::operator delete(void *pObj)
{
	CNtlEffectSystemFreeList::Free(CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_LIGHTNING, pObj);
}

CNtlInstanceLightningSystem::CNtlInstanceLightningSystem(void)
{
	Init();
}

CNtlInstanceLightningSystem::~CNtlInstanceLightningSystem(void)
{
	Delete();
}


//------------------------------------------------------------------
//	FuncName	: Init
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceLightningSystem::Init()
{
	CNtlInstanceComponentSystem::Init();

	m_pResourceLightningSystem	= NULL;

	RwMatrixSetIdentity(&m_matLightning);

	m_nLevelCount	= 0;
	m_pLocation		= NULL;

	m_fUpdateTime	= 0.f;
	m_vStartPoint.x = m_vStartPoint.y = m_vStartPoint.z = 0.f;
	m_vEndPoint.x = m_vEndPoint.y = m_vEndPoint.z = 0.f;

    m_uiMemoryUseSize = sizeof(CNtlInstanceLightningSystem);
}

//------------------------------------------------------------------
//	FuncName	: Delete
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceLightningSystem::Reset()
{
	Delete();
}

//------------------------------------------------------------------
//	FuncName	: Delete
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceLightningSystem::Delete()
{
    NTL_ARRAY_DELETE(m_pLocation);

	CNtlInstanceImVertexSystem::Delete();
}


//------------------------------------------------------------------
//	FuncName	: Create
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlInstanceLightningSystem::Create(CNtlResourceEffect* pResourceEffect, CNtlResourceComponentSystem* pResourceComponentSystem, const RwMatrix& matWorld)
{
	CNtlInstanceImVertexSystem::Create(pResourceEffect, pResourceComponentSystem, matWorld);
    
	m_pResourceLightningSystem = (CNtlResourceLightningSystem*) pResourceComponentSystem;

	m_nLevelCount = 11;
	m_nVertexCount = (m_nLevelCount) * 2;	
    m_pVertices = NTL_NEW RwIm3DVertex[m_nVertexCount];
	NTL_ASSERTE(m_pVertices);
    if(!m_pVertices) return FALSE;
    ZeroMemory(m_pVertices, sizeof(RwIm3DVertex) * m_nVertexCount);
    m_uiMemoryUseSize += sizeof(RwIm3DVertex) * m_nVertexCount;

    m_pLocation = NTL_NEW RwV3d[m_nLevelCount];
	NTL_ASSERTE(m_pLocation);
    if(!m_pLocation) return FALSE;	
    ZeroMemory(m_pLocation, sizeof(RwV3d) * m_nLevelCount);
    m_uiMemoryUseSize += sizeof(RwV3d) * m_nLevelCount;

	m_pCurrentTexture = m_pStandardTexture = CreateTexture(m_pResourceLightningSystem->m_strTextureName);

	BuildEmitterStandard(&m_pResourceLightningSystem->m_EmitterStandard);

    // Start, End Point 설정
    m_vStartPoint = m_pResourceLightningSystem->m_vPosition;
    m_vEndPoint   = m_pResourceLightningSystem->m_vRotate; // 사용하지 않는 Rotate값을 EndPoint 값으로 사용한다.
    m_vFinishPoint = m_vStartPoint;
    
    UpdateVertices();

       
	return TRUE;
}


//------------------------------------------------------------------
//	FuncName	: BuildEmitterStandard
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceLightningSystem::BuildEmitterStandard(SNtlLightningEmitterStandard* pEmitterStandard)
{
	if (!m_pResourceLightningSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE))
	{
		m_vImVertexSize = pEmitterStandard->prtSize;
	}
    else 
    {
        m_vImVertexSize = m_pResourceLightningSystem->m_EmitterPrtSize.prtStartSize;
    }

	if (!m_pResourceLightningSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))
	{
		SetVertexColor(m_pResourceLightningSystem->m_EmitterStandard.prtColor);
	}
}


//------------------------------------------------------------------
//	FuncName	: Update
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlInstanceLightningSystem::Update(RwReal fElapsedTime)
{
    CNtlInstanceImVertexSystem::Update(fElapsedTime);

    if(!m_pResourceLightningSystem)
        return FALSE;

	m_fLifeTime += fElapsedTime;
	m_fUpdateTime += fElapsedTime;
	if (m_bStart)
	{
		if (m_pResourceLightningSystem->m_fStartTime < m_fLifeTime)
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

	if (m_bUpdate && m_pResourceLightningSystem->m_bEternity == FALSE && m_fLifeTime >= m_pResourceLightningSystem->m_fLifeTime)
	{
		Stop();
	}

	if (m_bReady)
	{
		RwReal fDeltaTime = m_fLifeTime / m_pResourceLightningSystem->m_fLifeTime;
		if (m_pResourceLightningSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE))
		{
			UpdateSize(fDeltaTime);
		}
		if (m_pResourceLightningSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))
		{
			UpdateColor(fDeltaTime);
		}
		if (m_pResourceLightningSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTITEXTURES))
		{
			UpdateMultiTexture(fElapsedTime);
		}

        
        RwReal fUpdateTime = m_pResourceLightningSystem->m_EmitterStandard.fUpdateTime;
        if(CNtlInstanceEffect::GetLowSpecEnable())
        {
            fUpdateTime *= 1.0f / CNtlInstanceEffect::GetLowSpecRatio();
        }

		if (m_fUpdateTime > m_pResourceLightningSystem->m_EmitterStandard.fUpdateTime)
		{
			UpdateVertices();
			m_fUpdateTime = 0.f;
		}
	}
	if (m_bUpdate == FALSE)
	{
        if(m_pResourceLightningSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTFINISH))
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
RwBool CNtlInstanceLightningSystem::Render()
{
	if (!m_bReady || !m_bVisible) return TRUE;

	BeginRenderState(m_pResourceLightningSystem);

	if(RwIm3DTransform(m_pVertices, m_nVertexCount, (RwMatrix *)&m_matWorld, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA|rwIM3D_VERTEXUV))
	{
		RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);        
		RwIm3DEnd();
	}

	EndRenderState(m_pResourceLightningSystem);

	return TRUE;
}

//------------------------------------------------------------------
//	FuncName	: GetLineWidth
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwReal CNtlInstanceLightningSystem::GetLineWidth(RwReal fRatio)
{
	if (fRatio < 0.5f)
		return m_vImVertexSize.z * (m_vImVertexSize.x + (1.0f - m_vImVertexSize.x) * fRatio);
	else
		return m_vImVertexSize.z * (m_vImVertexSize.y + (1.0f - m_vImVertexSize.y) * (1.0f - fRatio));
}

//------------------------------------------------------------------
//	FuncName	: UpdateVertices
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceLightningSystem::UpdateVertices()
{
	CalculatePoints(m_vStartPoint, m_vEndPoint, m_pResourceLightningSystem->m_EmitterStandard.fAmp, m_pResourceLightningSystem->m_EmitterStandard.fRough);

	RwV3d vDist;
	RwV3dSubMacro(&vDist, &m_vEndPoint, &m_vStartPoint);

	RwReal fLength = RwV3dLength(&vDist) * 3;

	RwV3d vRight = *RwMatrixGetRight(&m_matLightning);

	for(RwInt32 i = 0; i < m_nLevelCount; ++i)
	{
		RwReal fRatio = (RwReal)i/(RwReal)m_nLevelCount;
        m_pVertices[i * 2 + 0].objVertex = m_pLocation[i] + (vRight * GetLineWidth(fRatio));
		m_pVertices[i*2+0].u = 0;
		m_pVertices[i*2+0].v = fLength * fRatio;
		
        m_pVertices[i * 2 + 1].objVertex = m_pLocation[i] - (vRight * GetLineWidth(fRatio));
		m_pVertices[i*2+1].u = 1;
		m_pVertices[i*2+1].v = m_pVertices[i*2+0].v;
	}

	//m_vEndPoint = m_pVertices[(m_nLevelCount-1)*2+1].objVertex;
}


//------------------------------------------------------------------
//	FuncName	: CalculateMatrix
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceLightningSystem::CalculateMatrix(RwMatrix& matWorld, const RwV3d& vStart, const RwV3d& vEnd)
{
	static RwV3d vAxisZ, vAxisX, vAxisY;

	const RwV3d* pViewPosition = RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));

	RwMatrixSetIdentity(&matWorld);

    vAxisZ = vEnd - vStart;
	RwV3dNormalize(&vAxisZ, &vAxisZ);
	
    vAxisY = *pViewPosition - vStart;

	RwV3dCrossProduct(&vAxisX, &vAxisY, &vAxisZ);
	RwV3dNormalize(&vAxisX, &vAxisX);

	RwV3dCrossProduct(&vAxisY, &vAxisX, &vAxisZ);

    *RwMatrixGetRight(&matWorld) = vAxisX;
	*RwMatrixGetAt(&matWorld)	= vAxisZ;
	*RwMatrixGetUp(&matWorld)	= vAxisY;   

}

void CNtlInstanceLightningSystem::CalculatePoints(const RwV3d& vStart, const RwV3d& vEnd, RwReal fAmp, RwReal fRough)
{
	if(m_nLevelCount <= 1)
		return;

	RwInt32 nSize = m_nLevelCount - 1;
	RwInt32 nRectSize = nSize;
	RwReal fDH = fAmp;
	RwReal r = pow(2.0f, -fRough);
	RwInt32 i, ni, mi;

    for(int k = 0; k < nSize + 1; ++k)
    {
        ZeroMemory(&m_pLocation[k], sizeof(m_pLocation[k]));        
    }

	while(nRectSize > 0)
	{
		for (i = 0; i < nSize; i += nRectSize)
		{
			ni = i + nRectSize;
			mi = (i+nRectSize/2);
			RwReal fRand = (((RwReal)rand()-(RwReal)rand())/(2L*RAND_MAX));            
			m_pLocation[mi].x = (m_pLocation[i].x + m_pLocation[ni].x) * 0.5f + fDH * fRand;            
			m_pLocation[mi].y = 0;//(m_pLocation[i].y + m_pLocation[ni].y) * 0.5f + fDH * fRand;
		}

		nRectSize /= 2;
		fDH *= r;
	}
    

	RwV3d vDist;
	RwV3dSubMacro(&vDist, &vEnd, &vStart);
	RwReal fLength = RwV3dLength(&vDist);
	RwReal fDelta, fOffset;

	fDelta = fLength / (nSize*2);

	CalculateMatrix(m_matLightning, vStart, vEnd);
    *RwMatrixGetPos(&m_matLightning) = vStart; 

	for (i = 0; i < nSize+1; ++i)
	{
		fOffset = ((RwReal)i / nSize); 
		RwReal fRand = (((RwReal)rand()-(RwReal)rand())/(2L*RAND_MAX));
		m_pLocation[i].z = fLength * fOffset + fDelta * fRand;

		RwV3dTransformPoint(&m_pLocation[i], &m_pLocation[i], &m_matLightning);        
	}

	m_pLocation[0] = vStart;    
}

RwBool CNtlInstanceLightningSystem::UpdateFinish( RwReal fElapsedTime ) 
{
    if(m_pResourceLightningSystem->m_EmitterPrtFinish.m_nFinishTypeFlag & FINISH_TYPE_ALPHA)
    {
        UpdateFinishAlpha(m_pResourceLightningSystem->m_EmitterStandard.prtColor.alpha, fElapsedTime, m_pResourceLightningSystem->m_EmitterPrtFinish.m_fFinishTime);
    }

    if(m_pResourceLightningSystem->m_EmitterPrtFinish.m_nFinishTypeFlag & FINISH_TYPE_SIZE)
    {
        m_vImVertexSize -= m_pResourceLightningSystem->m_EmitterStandard.prtSize * fElapsedTime / m_pResourceLightningSystem->m_EmitterPrtFinish.m_fFinishTime;
        UpdateVertices();
    }

    if(m_pResourceLightningSystem->m_EmitterPrtFinish.m_nFinishTypeFlag & FINISH_TYPE_FOLLOW)
    {
        // 방향 벡터를 구해서. 그 방향으로 보간될 길이만큼 StartPoint를 옮긴다.

        RwV3d vDir = m_vEndPoint - m_vFinishPoint;
        RwReal vLength = RwV3dLength(&vDir) * fElapsedTime / m_pResourceLightningSystem->m_EmitterPrtFinish.m_fFinishTime; 
        RwV3dNormalize(&vDir, &vDir);

        m_vStartPoint += vDir * vLength;
    }

    m_fFinishTime += fElapsedTime;

    if(m_fFinishTime >= m_pResourceLightningSystem->m_EmitterPrtFinish.m_fFinishTime)
    {
        return FALSE;
    }

    return TRUE;
}