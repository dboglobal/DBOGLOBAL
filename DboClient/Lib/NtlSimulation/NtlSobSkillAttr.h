/*****************************************************************************
 *
 * File			: NtlSobSkillAttr.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2006. 4. 24	
 * Abstract		: Simulation object skill attribute base class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_SOBSKILL_ATTR_H__
#define __NTL_SOBSKILL_ATTR_H__

#include "NtlSobAttr.h"
#include "SkillTable.h"

struct sNtlSLUpdatedData
{
	enum APPLYTYPE { CASTING_TIME_DOWN = 0x01, COOL_TIME_DOWN = 0x02, SYSTEM_EFFECT_VALUE_UP = 0x04, KEEP_TIME_UP = 0x08,
					 REQUIRED_EP_DOWN = 0x10, APPLY_RANGE_TYPE = 0x20, APPLY_AREA_SIZE_UP = 0x40, USE_RANGE_MAX_UP = 0x80 };

	sNtlSLUpdatedData(VOID)
	{
		fCasting_Time = 0.0f;
		wCool_Time = 0;
		wKeep_Time = 0;
		wRequire_EP = 0;
		byApply_Range = 0;
		byApply_Area_Size_1 = 0;
		byApply_Area_Size_2 = 0;
		byUse_Range_Max = 0;

		for( RwInt32 i = 0 ; i < NTL_MAX_EFFECT_IN_SKILL ; ++i )
			aSkill_Effect_Value[i] = 0.0f;

		m_flagApply = 0;
	}

	// DBO_SKILL_PASSIVE_EFFECT_CASTING_TIME_DOWN
	float		fCasting_Time;

	// DBO_SKILL_PASSIVE_EFFECT_COOL_TIME_DOWN
	WORD		wCool_Time;

	// DBO_SKILL_PASSIVE_EFFECT_SYSTEM_EFFECT_VALUE_UP
	double		aSkill_Effect_Value[NTL_MAX_EFFECT_IN_SKILL];

	// DBO_SKILL_PASSIVE_EFFECT_KEEP_TIME_UP
	WORD		wKeep_Time;

	// DBO_SKILL_PASSIVE_EFFECT_REQUIRED_EP_DOWN
	WORD		wRequire_EP;

	// DBO_SKILL_PASSIVE_EFFECT_APPLY_RANGE_TYPE
	BYTE		byApply_Range;

	// DBO_SKILL_PASSIVE_EFFECT_APPLY_AREA_SIZE_UP
	BYTE	byApply_Area_Size_1;
	BYTE	byApply_Area_Size_2;
	
	// DBO_SKILL_PASSIVE_EFFECT_USE_RANGE_MAX_UP
	BYTE		byUse_Range_Max;

	// Apply Flag
	RwUInt32	m_flagApply;
};

class CNtlSobSkillAttr : public CNtlSobAttr
{
	DECLEAR_MEMORY_POOL(CNtlSobSkillAttr, NTL_DEFAULT_MEMORY_POOL)
	
private:
	sSKILL_TBLDAT  m_SkillTbl;
	sSKILL_TBLDAT* m_pBaseSkillTbl;		/** skill table data pointer */	
	sNtlSLUpdatedData	m_stUpdatedData;

public:
	RwUInt32 m_uiExp;
	RwUInt32 m_uiMaxExp;
	RwUInt8	 m_byRPBonusType;
	RwBool	 m_bRPBonusAutoMode;	
			
public:

	CNtlSobSkillAttr() 
	{ 
		m_pBaseSkillTbl = NULL;
		m_uiExp = 0;
		m_uiMaxExp = 0;
		m_byRPBonusType = 0xFF;
		m_bRPBonusAutoMode = FALSE;
	}

	virtual ~CNtlSobSkillAttr() {}

	virtual RwBool Create(void) { SetClassID(SLCLASS_SKILL); return TRUE; }

	virtual void Destroy(void) {}

	virtual void HandleEvents(RWS::CMsg &pMsg);

public:

	void SetSkillTbl(const sSKILL_TBLDAT *pSkillTbl);
	sSKILL_TBLDAT* GetSkillTbl(void);
	sSKILL_TBLDAT* GetBaseSkillTbl(void) const;

	RwBool	IsMasterSkill(void);

	void UpdatePassiveEffect( sDBO_UPDATED_SKILL_DATA* pPassiveEffectData );
	void RefreshPassiveEffect(void);
};

inline void CNtlSobSkillAttr::SetSkillTbl(const sSKILL_TBLDAT *pSkillTbl)
{
	m_pBaseSkillTbl = const_cast<sSKILL_TBLDAT*>(pSkillTbl);
	m_SkillTbl = *pSkillTbl;
}

inline sSKILL_TBLDAT* CNtlSobSkillAttr::GetSkillTbl(void)
{
	return &m_SkillTbl;
}

inline sSKILL_TBLDAT* CNtlSobSkillAttr::GetBaseSkillTbl(void) const
{
	return m_pBaseSkillTbl;
}


#endif