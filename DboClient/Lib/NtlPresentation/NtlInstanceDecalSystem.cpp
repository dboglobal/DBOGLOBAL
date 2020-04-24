//***********************************************************************************
//	File		:	NtlInstanceDecalSystem.cpp
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#include "precomp_ntlpresentation.h"
#include "NtlInstanceDecalSystem.h"

#include ".\ntleffectsystemfreelist.h"

#include "NtlPLGlobal.h"
#include "NtlPLRenderState.h"
#include "NtlMath.h"
#include "NtlInstanceEffect.h"

void* CNtlInstanceDecalSystem::operator new(size_t size)
{
	NTL_FUNCTION("CNtlInstanceDecalSystem::operator new");
    
	NTL_RETURN(CNtlEffectSystemFreeList::Alloc(CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_DECAL));
}

void CNtlInstanceDecalSystem::operator delete(void *pObj)
{
	CNtlEffectSystemFreeList::Free(CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_DECAL, pObj);
}

CNtlInstanceDecalSystem::CNtlInstanceDecalSystem(void)
{
	Init();
}

CNtlInstanceDecalSystem::~CNtlInstanceDecalSystem(void)
{
	Delete();
}


//------------------------------------------------------------------
//	FuncName	: Init
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceDecalSystem::Init()
{
	CNtlInstanceComponentSystem::Init();

	m_pResourceDecalSystem	= NULL;
	m_bUpdateVertex	= TRUE;
	m_pDecalSystem = NULL;
}

//------------------------------------------------------------------
//	FuncName	: Delete
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceDecalSystem::Reset()
{
	Delete();
}

//------------------------------------------------------------------
//	FuncName	: Delete
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceDecalSystem::Delete()
{
	if(m_pDecalSystem)
	{
		m_pDecalSystem->Destroy();
		NTL_DELETE(m_pDecalSystem);
	}

	CNtlInstanceComponentSystem::Delete();
}



//------------------------------------------------------------------
//	FuncName	: Create
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlInstanceDecalSystem::Create(CNtlResourceEffect* pResourceEffect, CNtlResourceComponentSystem* pResourceComponentSystem, const RwMatrix& matWorld)
{
	CNtlInstanceComponentSystem::Create(pResourceEffect, pResourceComponentSystem, matWorld);

	m_pResourceDecalSystem = (CNtlResourceDecalSystem*) pResourceComponentSystem;

	// Decal Create an object.
	std::string strTexPath;
	if(!m_pResourceDecalSystem->m_strTextureName.empty())
	{
		const RwChar* szPath = CNtlEffectSystemFreeList::m_TextureFileManager.GetPathName(m_pResourceDecalSystem->m_strTextureName.c_str());
		if(szPath)
		{
			strTexPath = szPath;
			int nPos = strTexPath.rfind("\\");
			strTexPath.replace(nPos + 1, strTexPath.length() - nPos - 1, "\0");
		}
		else
		{
			strTexPath = PATH_EFFECT_TEXTURE;
		}
	}

	SPLDecalCreateParam param;
	param.pTexName = m_pResourceDecalSystem->m_strTextureName.c_str();
	param.pTexPath = strTexPath.c_str();
	param.fScale = m_pResourceDecalSystem->m_EmitterStandard.fScale;	
	param.vSize = m_pResourceDecalSystem->m_EmitterStandard.vSize;
	param.fVisibleSquaredDist = pow(m_pResourceDecalSystem->m_EmitterStandard.fVisibleDistance, 2);
	param.rwWrapType = m_pResourceDecalSystem->m_EmitterStandard.eWrapType;	
    param.fYOffset = m_pResourceDecalSystem->m_EmitterStandard.fYOffset;
	param.eDecalType = DECAL_EFFECT;	

	m_pDecalSystem = NTL_NEW CNtlPLDecal();
	m_pDecalSystem->Create(&param);	
	
	RwRGBA color = m_pResourceDecalSystem->m_EmitterStandard.color;		
	m_pDecalSystem->SetColor(color.red, color.blue, color.alpha);
	m_pDecalSystem->SetAlpha(color.alpha);		
	m_pDecalSystem->SetAlphaBlend(m_pResourceDecalSystem->m_bAlphaBlend, m_pResourceDecalSystem->m_nSrcBlend, m_pResourceDecalSystem->m_nDestBlend);

	BuildEmitterStandard(&m_pResourceDecalSystem->m_EmitterStandard);

	return TRUE;
}


//------------------------------------------------------------------
//	FuncName	: BuildEmitterStandard
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceDecalSystem::BuildEmitterStandard(SNtlDecalEmitterStandard* pEmitterStandard)
{
	if(!m_pDecalSystem)
		return;
	
	// Rotate Action
	if(m_pResourceDecalSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRT2DROTATE))
	{
		// Rotate 각도는 1초에 걸쳐 회전하는 각도로 설정한다. (Life Time과는 관계 없다)
		m_pDecalSystem->Set2DRoateAction(m_pResourceDecalSystem->m_EmitterPrtRotate.prtEnd2DRotate);
	}

	// color Action
	if(m_pResourceDecalSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))
	{
		RwRGBA colStart, colEnd;
		colStart.red	 = (RwUInt8)m_pResourceDecalSystem->m_EmitterPrtCol.prtStartCol.red;
		colStart.green	 = (RwUInt8)m_pResourceDecalSystem->m_EmitterPrtCol.prtStartCol.green;
		colStart.blue	 = (RwUInt8)m_pResourceDecalSystem->m_EmitterPrtCol.prtStartCol.blue;
		colStart.alpha	 = (RwUInt8)m_pResourceDecalSystem->m_EmitterPrtCol.prtStartCol.alpha;
		colEnd.red		 = (RwUInt8)m_pResourceDecalSystem->m_EmitterPrtCol.prtEndCol.red;
		colEnd.green	 = (RwUInt8)m_pResourceDecalSystem->m_EmitterPrtCol.prtEndCol.green;
		colEnd.blue		 = (RwUInt8)m_pResourceDecalSystem->m_EmitterPrtCol.prtEndCol.blue;
		colEnd.alpha	 = (RwUInt8)m_pResourceDecalSystem->m_EmitterPrtCol.prtEndCol.alpha;

		m_pDecalSystem->SetColorAction(colStart, colEnd, m_pResourceDecalSystem->m_fLifeTime);
	}

	// Multi Color Action
	if(m_pResourceDecalSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTICOLOR))
	{
		m_pDecalSystem->SetMultiColorAction(m_pResourceDecalSystem->m_EmitterPrtMultiCol);
	}

	// Size Action
	if (m_pResourceDecalSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE))
	{
		m_pDecalSystem->SetSizeAction(m_pResourceDecalSystem->m_EmitterPrtSize.prtStartSize.x, m_pResourceDecalSystem->m_EmitterPrtSize.prtEndSize.x, m_pResourceDecalSystem->m_fLifeTime);
	}

	if(m_pResourceDecalSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTISIZE))
	{
		m_pDecalSystem->SetMultiSizeAction(m_pResourceDecalSystem->m_EmitterPrtMultiSize);
	}

	// Multi Texture Action
	if(m_pResourceDecalSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTITEXTURES))
	{
		m_pDecalSystem->SetMultiTextureAction(m_pResourceDecalSystem->m_EmitterMultiTextures);
	}

	if(m_pResourceDecalSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGUVANIM))
	{
		m_pDecalSystem->SetUVAnimAction(m_pResourceDecalSystem->m_EmitterUVAnim);
	}
}

//------------------------------------------------------------------
//	FuncName	: Update
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlInstanceDecalSystem::Update(RwReal fElapsedTime)
{
    m_uiMemoryUseSize = sizeof(CNtlInstanceDecalSystem) + m_pDecalSystem->GetMemoryUseSize();

	m_fLifeTime += fElapsedTime;
	if (m_bStart)
	{
		if (m_pResourceDecalSystem->m_fStartTime < m_fLifeTime)
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

	if (m_bUpdate && m_pResourceDecalSystem->m_bEternity == FALSE && m_fLifeTime >= m_pResourceDecalSystem->m_fLifeTime)
	{
		Stop();
	}

    if (m_bReady && m_pDecalSystem && CNtlInstanceEffect::GetRenderDecalSystem())
	{
		//if (m_pResourceDecalSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTITEXTURES))
		//{
		//	UpdateMultiTexture(fElapsedTime);
		//}

        if(m_bUpdateVertex)
        {
            m_pDecalSystem->SetUpdateVertex(TRUE);
            m_bUpdateVertex = FALSE;
        }

		
		m_pDecalSystem->Update(fElapsedTime);
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
RwBool CNtlInstanceDecalSystem::Render()
{
    if(!CNtlInstanceEffect::GetRenderDecalSystem())
    {
        return TRUE;
    }

	if (!m_bReady || !m_bVisible) return TRUE;
	
	if(m_pDecalSystem)
	{
		m_pDecalSystem->Render();
	}

	return TRUE;
}

//------------------------------------------------------------------
//	FuncName	: SetWorldMatrix
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceDecalSystem::SetWorldMatrix(const RwMatrix& matWorld)
{
	CNtlInstanceComponentSystem::SetWorldMatrix(matWorld);

	if(m_pDecalSystem)
	{
		m_pDecalSystem->SetMatrix((RwMatrix)m_matRender);
	}
	
	m_bUpdateVertex	= TRUE;
}

RwInt32 CNtlInstanceDecalSystem::GetPolyCount() 
{
	if(m_pDecalSystem)
	{
		return m_pDecalSystem->GetVerexCount() / 3;
	}
	else
	{
		return 0;
	}
}

void CNtlInstanceDecalSystem::SetAlpha( RwUInt8 byValue ) 
{
    if(m_pDecalSystem)
    {
        m_pDecalSystem->SetAlpha(byValue);
    }
}

void CNtlInstanceDecalSystem::SetScale( RwReal fScale ) 
{
    if(m_pDecalSystem)
    {
        m_pDecalSystem->SetScaleSize(fScale);
    }

    m_bUpdateVertex = TRUE;
}