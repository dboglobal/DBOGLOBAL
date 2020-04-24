//***********************************************************************************
//	File		:	VenusEventEnemy.cpp
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	¨Ï 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#include "StdAfx.h"
#include ".\venuseventenemy.h"

#include "NtlEffectDefine.h"
#include "NtlEffectAPI.h"

#include "NtlPLResourceManager.h"
#include "VenusPlayerEventManager.h"

CVenusEventEnemy::CVenusEventEnemy(void)
{
	m_pClumpResource			= NULL;
	m_pIdleAnimationResource	= NULL;
	m_pDamageAnimationResource	= NULL;

	m_pCurrentAnimation			= NULL;

	m_fCurrentTime				= 0.f;
}

CVenusEventEnemy::~CVenusEventEnemy(void)
{
}

//------------------------------------------------------------------
//	FuncName	: Delete
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusEventEnemy::Delete()
{
	m_strClumpName.clear();
	m_strIdleAnimationName.clear();
	m_strDamageAnimationName.clear();

	m_strEffectName.clear();

	DeleteResource();
}

//------------------------------------------------------------------
//	FuncName	: Delete
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusEventEnemy::DeleteResource()
{
	if (m_pClumpResource != NULL)
	{
		CNtlPLResourceManager::GetInstance()->UnLoad(m_pClumpResource);
		m_pClumpResource = NULL;
	}
	if (m_pIdleAnimationResource != NULL)
	{
		CNtlPLResourceManager::GetInstance()->UnLoad(m_pIdleAnimationResource);
		m_pIdleAnimationResource = NULL;
	}
	if (m_pDamageAnimationResource != NULL)
	{
		CNtlPLResourceManager::GetInstance()->UnLoad(m_pDamageAnimationResource);
		m_pDamageAnimationResource = NULL;
	}
	m_pCurrentAnimation = NULL;
}

//------------------------------------------------------------------
//	FuncName	: CreateResource
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusEventEnemy::CreateResource()
{
	DeleteResource();

	if (!m_strClumpName.empty())
	{
		const RwChar* strPathName = CVenusPlayerEventManager::GetInstance().m_ClumpFileManager.GetPathName(m_strClumpName.c_str());
		if (strPathName != NULL)
		{
			m_pClumpResource = CNtlPLResourceManager::GetInstance()->LoadClump(strPathName, CVenusPlayerEventManager::GetInstance().m_strClumpTexturePath.c_str());
			RwV3d vPosition = { 0, 0, 10 };
			SetPosition(vPosition);
		}
	}

	if (m_pClumpResource != NULL && !m_strIdleAnimationName.empty())
	{
		const RwChar* strPathName = CVenusPlayerEventManager::GetInstance().m_AnimationFileManager.GetPathName(m_strIdleAnimationName.c_str());
		m_pIdleAnimationResource = CNtlPLResourceManager::GetInstance()->LoadAnimation(strPathName);
		m_pCurrentAnimation = m_pIdleAnimationResource->GetAnimation();
	}
	if (m_pClumpResource != NULL && m_pIdleAnimationResource != NULL && !m_strDamageAnimationName.empty())
	{
		const RwChar* strPathName = CVenusPlayerEventManager::GetInstance().m_AnimationFileManager.GetPathName(m_strDamageAnimationName.c_str());
		m_pDamageAnimationResource = CNtlPLResourceManager::GetInstance()->LoadAnimation(strPathName);
	}
	if (m_pCurrentAnimation != NULL)
	{
		API_SetAnimation(m_pClumpResource->GetClump(), m_pCurrentAnimation);
	}
}

//------------------------------------------------------------------
//	FuncName	: SetDamage
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusEventEnemy::SetDamage()
{
	if (m_pDamageAnimationResource != NULL)
	{
		m_pCurrentAnimation = m_pDamageAnimationResource->GetAnimation();
		API_SetAnimation(m_pClumpResource->GetClump(), m_pCurrentAnimation);


		m_fCurrentTime = 0.f;
	}
	return TRUE;
}

//------------------------------------------------------------------
//	FuncName	: SetPosition
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusEventEnemy::SetPosition(RwV3d& vPosition)
{
	if (m_pClumpResource != NULL)
	{
		RwMatrix matWorld;
		RwMatrixRotate(&matWorld, &VEC_Y, 180, rwCOMBINEREPLACE);
		RwMatrixTranslate(&matWorld, &vPosition, rwCOMBINEPOSTCONCAT);

		RwFrameTransform(RpClumpGetFrame(m_pClumpResource->GetClump()), &matWorld, rwCOMBINEREPLACE);
	}
	return TRUE;
}

//------------------------------------------------------------------
//	FuncName	: Update
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusEventEnemy::Update(RwReal fElapsedTime)
{
	// Animation
	if (m_pCurrentAnimation != NULL)
	{
		RpHAnimHierarchy *hierarchy = NULL;
		RpClumpForAllAtomics(m_pClumpResource->GetClump(), API_GetSkinHierarchy, (void *)&hierarchy);
		RpHAnimHierarchyAddAnimTime(hierarchy, fElapsedTime);

		RpHAnimHierarchyUpdateMatrices(hierarchy);

		// Å×½ºÆ®
		if (m_pDamageAnimationResource && (m_pDamageAnimationResource->GetAnimation() == m_pCurrentAnimation))
		{
			if (m_fCurrentTime > hierarchy->currentAnim->currentTime)
			{
				m_pCurrentAnimation = m_pIdleAnimationResource->GetAnimation();
				API_SetAnimation(m_pClumpResource->GetClump(), m_pCurrentAnimation);
			}
		}
		m_fCurrentTime = hierarchy->currentAnim->currentTime;
	}
	return TRUE;
}

//------------------------------------------------------------------
//	FuncName	: Render
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusEventEnemy::Render()
{
	if (m_pClumpResource && m_pClumpResource->GetClump() != NULL)
	{
		RpClumpRender(m_pClumpResource->GetClump());
	}
	return TRUE;
}

//------------------------------------------------------------------
//	FuncName	: Save
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusEventEnemy::Save(FILE* pFile)
{
	RwInt32 length = (RwInt32)m_strClumpName.size() + 1;
	fwrite(&length,					sizeof(RwInt32),							1,			pFile);
	fwrite(m_strClumpName.c_str(),	sizeof(RwChar),								length,		pFile);

	length = (RwInt32)m_strIdleAnimationName.size() + 1;
	fwrite(&length,					sizeof(RwInt32),							1,			pFile);
	fwrite(m_strIdleAnimationName.c_str(),	sizeof(RwChar),							length,		pFile);

	length = (RwInt32)m_strDamageAnimationName.size() + 1;
	fwrite(&length,					sizeof(RwInt32),							1,			pFile);
	fwrite(m_strDamageAnimationName.c_str(),	sizeof(RwChar),							length,		pFile);

	length = (RwInt32)m_strEffectName.size() + 1;
	fwrite(&length,					sizeof(RwInt32),							1,			pFile);
	fwrite(m_strEffectName.c_str(),	sizeof(RwChar),							length,		pFile);

	return TRUE;
}


//------------------------------------------------------------------
//	FuncName	: Load
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusEventEnemy::Load(FILE* pFile)
{
	RwChar strName[MAX_PATH];
	RwInt32 length;

	fread(&length,					sizeof(RwInt32),							1,			pFile);
	memset(strName, 0, MAX_PATH);
	fread(strName,					sizeof(RwChar),								length,		pFile);

	m_strClumpName = strName;

	fread(&length,					sizeof(RwInt32),							1,			pFile);
	memset(strName, 0, MAX_PATH);
	fread(strName,					sizeof(RwChar),								length,		pFile);

	m_strIdleAnimationName = strName;

	fread(&length,					sizeof(RwInt32),							1,			pFile);
	memset(strName, 0, MAX_PATH);
	fread(strName,					sizeof(RwChar),								length,		pFile);

	m_strDamageAnimationName = strName;

	fread(&length,					sizeof(RwInt32),							1,			pFile);
	memset(strName, 0, MAX_PATH);
	fread(strName,					sizeof(RwChar),								length,		pFile);

	m_strEffectName = strName;
	return TRUE;
}
