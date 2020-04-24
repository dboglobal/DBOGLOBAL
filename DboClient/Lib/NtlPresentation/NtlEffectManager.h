//***********************************************************************************
//	File		:	NtlEffectManager.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	¨Ï 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once

#include ".\ntlresourceeffect.h"
#include ".\ntlinstanceeffect.h"

#include <map>
#include <vector>
#include <string>

#include "rpworld.h"


#include "NtlDebug.h"

#include "NtlHandleManager.h"

#include "NtlResourceEffect.h"

class CNtlEffectManager
{
public:
	// resource
	typedef std::vector<CNtlResourceEffect*>			svdef_ResourceEffect;
	svdef_ResourceEffect								m_svResourceEffect;

	typedef std::map<HEFFECT, CNtlResourceEffect*>		smdef_ResourceEffect;
	smdef_ResourceEffect								m_smResourceEffect;

	// Instance
	CNtlHandleManager<HEFFECT, EFFECT_INVALID_HANDLE>	m_InstanceHandle;

	typedef std::map<HEFFECT, CNtlInstanceEffect*>		smdef_InstanceEffect;
	smdef_InstanceEffect								m_smInstanceEffect;

public:
	CNtlEffectManager(void);
	virtual ~CNtlEffectManager(void);

	virtual RwBool	Create();
	virtual RwBool	Delete();

	CNtlResourceComponentSystem*	CreateResourceSystem(RwInt8 nType, RwChar* strName);

	CNtlResourceEffect* GetResourceEffect(HEFFECT hResourceEffect);

	virtual HEFFECT		CreateManagerEffect(HEFFECT hResourceEffect, const RwMatrix& matWorld);
	virtual RwBool		DeleteManagerEffect(HEFFECT& hEffect);
	virtual RwBool		StopManagerEffect(HEFFECT& hEffect);

	virtual RwBool		Update(RwReal fElapsedTime);
	virtual RwBool		Render();

	
public:
	void				InsertResourceEffect(CNtlResourceEffect* pResourceEffect);
};

//------------------------------------------------------------------
//	FuncName	: InsertResourceSystem
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
inline void CNtlEffectManager::InsertResourceEffect(CNtlResourceEffect* pResourceEffect)
{
	m_svResourceEffect.push_back(pResourceEffect);
	m_smResourceEffect.insert(smdef_ResourceEffect::value_type(pResourceEffect->GetId(), pResourceEffect));
}
