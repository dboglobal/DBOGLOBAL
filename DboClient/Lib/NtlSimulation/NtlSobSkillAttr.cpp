#include "precomp_ntlsimulation.h"
#include "NtlSobSkillAttr.h"
#include "NtlDebug.h"

// shared
#include "TableContainer.h"

// simualtion
#include "NtlSLEvent.h"
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"

DEFINITION_MEMORY_POOL(CNtlSobSkillAttr)

void CNtlSobSkillAttr::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventSobCreate)
	{
		CNtlEventSobSkillCreate *pSkillCreate = reinterpret_cast<CNtlEventSobSkillCreate*>(pMsg.pData);
		
		CSkillTable *pSkillTable = API_GetTableContainer()->GetSkillTable();
		sSKILL_TBLDAT* pSkillTbl = reinterpret_cast<sSKILL_TBLDAT*>( pSkillTable->FindData( pSkillCreate->uiSkillTblId ) );
		NTL_ASSERT(pSkillTbl, "Skill table is null (" << pSkillCreate->uiSkillTblId << ")");

		SetSkillTbl( pSkillTbl );
		
		m_uiExp = pSkillCreate->iExp;
		m_uiMaxExp = GetSkillTbl()->wNext_Skill_Train_Exp;
		m_byRPBonusType = pSkillCreate->byRPBonusType;
		m_bRPBonusAutoMode = pSkillCreate->bRPBonusAutoMode;
	}

	else if(pMsg.Id == g_EventSobSkillUpgrade)
	{
		SNtlEventSobSkillUpgrade *pSkillUpgrade = reinterpret_cast<SNtlEventSobSkillUpgrade*>(pMsg.pData);

		CSkillTable *pSkillTable = API_GetTableContainer()->GetSkillTable();
		sSKILL_TBLDAT* pSkillTbl = reinterpret_cast<sSKILL_TBLDAT*>( pSkillTable->FindData(pSkillUpgrade->uiUpgradeSkillTblId) );
		NTL_ASSERT(pSkillTbl, "Skill table is null (" << pSkillUpgrade->uiUpgradeSkillTblId << ")");

		SetSkillTbl( pSkillTbl );
		RefreshPassiveEffect();

		m_uiExp = 0;
		m_uiMaxExp = GetSkillTbl()->wNext_Skill_Train_Exp;
	}

	else if(pMsg.Id == g_EventRpBonusSetupRes)
	{
		SNtlEventRpBonusSetupRes* pData = reinterpret_cast<SNtlEventRpBonusSetupRes*>(pMsg.pData);
		NTL_ASSERT( pData->skillId == GetSkillTbl()->tblidx, "CNtlSobSkillAttr::HandleEvents : RpBonus Event does not match tblidx" );

		m_byRPBonusType = pData->byRpBonusType;
		m_bRPBonusAutoMode = pData->bRpBonusAutoMode;
	}	
}

RwBool CNtlSobSkillAttr::IsMasterSkill(void)
{
	return ( GetSkillTbl()->dwNextSkillTblidx == INVALID_TBLIDX ) ? TRUE : FALSE;		
}

void CNtlSobSkillAttr::UpdatePassiveEffect( sDBO_UPDATED_SKILL_DATA* pPassiveEffectData )
{
	switch( pPassiveEffectData->bySkillPassiveEffect )
	{
	case DBO_SKILL_PASSIVE_EFFECT_CASTING_TIME_DOWN:
		m_stUpdatedData.fCasting_Time = pPassiveEffectData->fCasting_Time;		
		m_SkillTbl.fCasting_Time = pPassiveEffectData->fCasting_Time;
		m_stUpdatedData.m_flagApply |= sNtlSLUpdatedData::CASTING_TIME_DOWN;
		break;
	case DBO_SKILL_PASSIVE_EFFECT_COOL_TIME_DOWN:
		m_stUpdatedData.wCool_Time = pPassiveEffectData->wCool_Time;
		m_SkillTbl.wCool_Time = pPassiveEffectData->wCool_Time;
		m_stUpdatedData.m_flagApply |= sNtlSLUpdatedData::COOL_TIME_DOWN;
		break;
	case DBO_SKILL_PASSIVE_EFFECT_SYSTEM_EFFECT_VALUE_UP:
		{
			for( RwInt32 i = 0 ; i < NTL_MAX_EFFECT_IN_SKILL ; ++i )
			{
				m_stUpdatedData.aSkill_Effect_Value[i] = pPassiveEffectData->aSkill_Effect_Value[i];
				m_SkillTbl.aSkill_Effect_Value[i] = pPassiveEffectData->aSkill_Effect_Value[i];				
			}
			m_stUpdatedData.m_flagApply |= sNtlSLUpdatedData::SYSTEM_EFFECT_VALUE_UP;
		}
		break;
	case DBO_SKILL_PASSIVE_EFFECT_KEEP_TIME_UP:
		m_stUpdatedData.wKeep_Time = pPassiveEffectData->wKeep_Time;
		m_SkillTbl.wKeep_Time = pPassiveEffectData->wKeep_Time;
		m_stUpdatedData.m_flagApply |= sNtlSLUpdatedData::KEEP_TIME_UP;
		break;
	case DBO_SKILL_PASSIVE_EFFECT_REQUIRED_EP_DOWN:
		m_stUpdatedData.wRequire_EP = pPassiveEffectData->wRequire_EP;
		m_SkillTbl.wRequire_EP = pPassiveEffectData->wRequire_EP;
		m_stUpdatedData.m_flagApply |= sNtlSLUpdatedData::REQUIRED_EP_DOWN;
		break;
	case DBO_SKILL_PASSIVE_EFFECT_APPLY_RANGE_TYPE:
		m_stUpdatedData.byApply_Range = pPassiveEffectData->byApply_Range;
		m_SkillTbl.byApply_Range = pPassiveEffectData->byApply_Range;
		m_stUpdatedData.m_flagApply |= sNtlSLUpdatedData::APPLY_RANGE_TYPE;
		break;
	case DBO_SKILL_PASSIVE_EFFECT_APPLY_AREA_SIZE_UP:
		m_stUpdatedData.byApply_Area_Size_1 = pPassiveEffectData->byApply_Area_Size_1;
		m_stUpdatedData.byApply_Area_Size_2 = pPassiveEffectData->byApply_Area_Size_2;
		m_SkillTbl.byApply_Area_Size_1 = pPassiveEffectData->byApply_Area_Size_1;
		m_SkillTbl.byApply_Area_Size_2 = pPassiveEffectData->byApply_Area_Size_2;
		m_stUpdatedData.m_flagApply |= sNtlSLUpdatedData::APPLY_AREA_SIZE_UP;
		break;
	case DBO_SKILL_PASSIVE_EFFECT_USE_RANGE_MAX_UP:
		m_stUpdatedData.byUse_Range_Max = pPassiveEffectData->byUse_Range_Max;
		m_SkillTbl.byUse_Range_Max = pPassiveEffectData->byUse_Range_Max;
		m_SkillTbl.fUse_Range_Max = (RwReal)pPassiveEffectData->byUse_Range_Max;
		m_stUpdatedData.m_flagApply |= sNtlSLUpdatedData::USE_RANGE_MAX_UP;
		break;
	}		
}

void CNtlSobSkillAttr::RefreshPassiveEffect(void)
{
	if( m_stUpdatedData.m_flagApply & sNtlSLUpdatedData::CASTING_TIME_DOWN )
		m_SkillTbl.fCasting_Time = m_stUpdatedData.fCasting_Time;

	if( m_stUpdatedData.m_flagApply & sNtlSLUpdatedData::COOL_TIME_DOWN )
		m_SkillTbl.wCool_Time = m_stUpdatedData.wCool_Time;

	if( m_stUpdatedData.m_flagApply & sNtlSLUpdatedData::KEEP_TIME_UP )
		m_SkillTbl.wKeep_Time = m_stUpdatedData.wKeep_Time;

	if( m_stUpdatedData.m_flagApply & sNtlSLUpdatedData::REQUIRED_EP_DOWN )
		m_SkillTbl.wRequire_EP = m_stUpdatedData.wRequire_EP;

	if( m_stUpdatedData.m_flagApply & sNtlSLUpdatedData::APPLY_RANGE_TYPE )
		m_SkillTbl.byApply_Range = m_stUpdatedData.byApply_Range;

	if( m_stUpdatedData.m_flagApply & sNtlSLUpdatedData::APPLY_AREA_SIZE_UP )
	{
		m_SkillTbl.byApply_Area_Size_1 = m_stUpdatedData.byApply_Area_Size_1;
		m_SkillTbl.byApply_Area_Size_2 = m_stUpdatedData.byApply_Area_Size_2;
	}

	if( m_stUpdatedData.m_flagApply & sNtlSLUpdatedData::USE_RANGE_MAX_UP )
	{
		m_SkillTbl.byUse_Range_Max = m_stUpdatedData.byUse_Range_Max;
		m_SkillTbl.fUse_Range_Max = (RwReal)m_stUpdatedData.byUse_Range_Max;
	}	

	if( m_stUpdatedData.m_flagApply & sNtlSLUpdatedData::SYSTEM_EFFECT_VALUE_UP )
	{
		for( RwInt32 i = 0 ; i < NTL_MAX_EFFECT_IN_SKILL ; ++i )
			m_SkillTbl.aSkill_Effect_Value[i] = m_stUpdatedData.aSkill_Effect_Value[i];	
	}
}