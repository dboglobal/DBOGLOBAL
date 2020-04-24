//***********************************************************************************
//	File		:	NtlInstanceImVertexSystem.cpp
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	¨Ï 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#include "precomp_ntlpresentation.h"
#include ".\ntlinstanceImVertexsystem.h"

#include ".\ntleffectsystemfreelist.h"

#include "NtlPLGlobal.h"

#include "NtlPLRenderState.h"

CNtlInstanceImVertexSystem::CNtlInstanceImVertexSystem(void)
{
	Init();
}

CNtlInstanceImVertexSystem::~CNtlInstanceImVertexSystem(void)
{
	Delete();
}


//------------------------------------------------------------------
//	FuncName	: Init
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceImVertexSystem::Init()
{
	CNtlInstanceComponentSystem::Init();

	m_pResourceImVertexSystem	= NULL;

	m_nVertexCount			= 0;
	m_pVertices				= NULL;

	m_vImVertexSize.x = m_vImVertexSize.y = m_vImVertexSize.z = 1.f;
    m_sColor.red = m_sColor.green = m_sColor.blue = m_sColor.alpha = 255;
}

//------------------------------------------------------------------
//	FuncName	: Delete
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceImVertexSystem::Reset()
{
	Delete();
}

//------------------------------------------------------------------
//	FuncName	: Delete
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceImVertexSystem::Delete()
{
    NTL_ARRAY_DELETE(m_pVertices);

	CNtlInstanceComponentSystem::Delete();
}
//------------------------------------------------------------------
//	FuncName	: Create
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlInstanceImVertexSystem::Create(CNtlResourceEffect* pResourceEffect, CNtlResourceComponentSystem* pResourceComponentSystem, const RwMatrix& matWorld)
{
	CNtlInstanceComponentSystem::Create(pResourceEffect, pResourceComponentSystem, matWorld);
    
	m_pResourceImVertexSystem = (CNtlResourceImVertexSystem*) pResourceComponentSystem;

	return TRUE;
}

//------------------------------------------------------------------
//	FuncName	: SetVertexColor
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceImVertexSystem::SetVertexColor(RwRGBA& color)
{
	for(RwInt32 i = 0; i < m_nVertexCount; ++i)
	{
		RwIm3DVertexSetRGBA(&m_pVertices[i], color.red,  color.green, color.blue, color.alpha);
	}

    m_sColor = color;
}

//------------------------------------------------------------------
//	FuncName	: SetWorldMatrix
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceImVertexSystem::SetWorldMatrix(const RwMatrix& matWorld)
{
	CNtlInstanceComponentSystem::SetWorldMatrix(matWorld);

	RwV3d vPos = *RwMatrixGetPos(&m_matRender);
	RwMatrixGetPos(&m_matRender)->x = RwMatrixGetPos(&m_matRender)->y = RwMatrixGetPos(&m_matRender)->z = 0.f;

	RwMatrix matOldRender = m_matRender;
	RwMatrixMultiply(&m_matRender, &matOldRender, &m_matRotate);
	
	matOldRender = m_matRender;
	RwMatrixMultiply(&m_matRender, &m_matScale, &matOldRender);

	RwMatrixTranslate(&m_matRender, &vPos, rwCOMBINEPOSTCONCAT);

}

//------------------------------------------------------------------
//	FuncName	: UpdateColor
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceImVertexSystem::UpdateColor(RwReal fDeltaTime)
{
	RwRGBA color;
	if (m_pResourceImVertexSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTICOLOR))
	{
		RebuildMultiColor(color, m_nRandomSeed, fDeltaTime, &m_pResourceImVertexSystem->m_EmitterPrtCol, &m_pResourceImVertexSystem->m_EmitterPrtMultiCol);
	}
	else
	{
		RebuildColor(color, m_nRandomSeed, fDeltaTime, &m_pResourceImVertexSystem->m_EmitterPrtCol);
	}
	SetVertexColor(color);
}


//------------------------------------------------------------------
//	FuncName	: UpdateSize
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceImVertexSystem::UpdateSize(RwReal fDeltaTime)
{
	if (m_pResourceImVertexSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTISIZE))
	{
		RebuildMultiSize(m_vImVertexSize, m_nRandomSeed, fDeltaTime, &m_pResourceImVertexSystem->m_EmitterPrtSize, &m_pResourceImVertexSystem->m_EmitterPrtMultiSize);
	}
	else
	{
		RebuildSize(m_vImVertexSize, m_nRandomSeed, fDeltaTime, &m_pResourceImVertexSystem->m_EmitterPrtSize);
	}
}

//------------------------------------------------------------------
//	FuncName	: BeginRenderState
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceImVertexSystem::BeginRenderState(CNtlResourceComponentSystem* pResourceSystem)
{
	RwD3D9SetTexture(m_pCurrentTexture, 0);

	BeginEffectImVertexSystemState(pResourceSystem->m_bAlphaBlend, pResourceSystem->m_nSrcBlend, pResourceSystem->m_nDestBlend);
}

//------------------------------------------------------------------
//	FuncName	: BeginRenderState
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceImVertexSystem::EndRenderState(CNtlResourceComponentSystem* pResourceSystem)
{
	EndEffectImVertexSystemState(pResourceSystem->m_bAlphaBlend);
}

void CNtlInstanceImVertexSystem::SetAlpha( RwUInt8 byValue )
{
    m_sColor.alpha = byValue;
    
    SetVertexColor(m_sColor);
}

RwUInt8 CNtlInstanceImVertexSystem::UpdateFinishAlpha( RwUInt8 uiOriginAlpha, RwReal fElapsedTime, RwReal fFinishTime ) 
{
    if(m_sColor.alpha > 0)
    {
        RwUInt8 uiDeltaAlpha = CNtlInstanceComponentSystem::UpdateFinishAlpha(uiOriginAlpha, fElapsedTime, fFinishTime);

        if((RwInt32)m_sColor.alpha - (RwInt32)uiDeltaAlpha < 0)
        {
            SetAlpha(0);
        }
        else
        {
            SetAlpha(m_sColor.alpha - uiDeltaAlpha);            
        }        

        return uiDeltaAlpha;
    }    

    return 0;    
}

RwBool CNtlInstanceImVertexSystem::Update( RwReal fElapsedTime ) 
{
    CNtlInstanceComponentSystem::Update(fElapsedTime);

    return TRUE;
}