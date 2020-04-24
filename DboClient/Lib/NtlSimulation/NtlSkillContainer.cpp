#include "precomp_ntlsimulation.h"
#include "NtlSkillContainer.h"

// shared
#include "SkillTable.h"
#include "HTBSetTable.h"
#include "ActionTable.h"
#include "SystemEffectTable.h"
#include "NPCTable.h"
#include "TableContainer.h"

// core
#include "NtlDebug.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLGlobal.h"
#include "NtlSobManager.h"
#include "NtlSobSkill.h"
#include "NtlSobSkillAttr.h"
#include "NtlSobSkillIcon.h"
#include "NtlSobHTBSkill.h"
#include "NtlSobHTBSkillAttr.h"
#include "NtlSobHTBSkillIcon.h"
#include "NtlSobActionSkill.h"
#include "NtlSobActionSkillAttr.h"
#include "NtlSobActionSkillIcon.h"
#include "NtlSobAvatar.h"
#include "NtlSobManager.h"
#include "NtlSLApi.h"


CNtlSkillContainer::CNtlSkillContainer()
{
	RwInt32 i;

	for(i = 0; i < NTL_MAX_PC_HAVE_SKILL; ++i)
	{
		m_hSerial[i] = INVALID_SERIAL_ID;
	}
	for( i = 0 ; i < NTL_HTB_MAX_PC_HAVE_HTB_SKILL ; ++i )
	{
		m_hHTBSerial[i] = INVALID_SERIAL_ID;
	}
	for( i = 0 ; i < NTL_MAX_PC_HAVE_ACTION_SKILL ; ++i )
	{
		m_hActionSerial[i] = INVALID_SERIAL_ID;
	}
}

CNtlSkillContainer::~CNtlSkillContainer()
{
}

RwBool CNtlSkillContainer::Create(void)
{
	NTL_FUNCTION("CNtlSkillContainer::Create");

	NTL_RETURN(TRUE);
	
}

void CNtlSkillContainer::Destroy(void)
{
	NTL_FUNCTION("CNtlSkillContainer::Destroy");

	for(RwInt32 i = 0; i < NTL_MAX_PC_HAVE_SKILL; ++i)
	{
		if(m_hSerial[i] != INVALID_SERIAL_ID)
		{
			CNtlSLEventGenerator::SobDelete(m_hSerial[i]);
			m_hSerial[i] = INVALID_SERIAL_ID;
		}			
	}
	for(RwInt32 i = 0 ; i <	NTL_HTB_MAX_PC_HAVE_HTB_SKILL ; ++i )
	{
		if(m_hHTBSerial[i] != INVALID_SERIAL_ID)
		{
			CNtlSLEventGenerator::SobDelete(m_hHTBSerial[i]);
			m_hHTBSerial[i] = INVALID_SERIAL_ID;
		}
	}

	for(RwInt32 i = 0 ; i < NTL_MAX_PC_HAVE_ACTION_SKILL ; ++i )
	{
		if(m_hActionSerial[i] != INVALID_SERIAL_ID)
		{
			CNtlSLEventGenerator::SobDelete(m_hActionSerial[i]);
			m_hActionSerial[i] = INVALID_SERIAL_ID;
		}
	}

	NTL_RETURNVOID();
}

void CNtlSkillContainer::SetSkillSerial(RwInt32 iSlotIdx, SERIAL_HANDLE hSerial)
{
	NTL_ASSERT(iSlotIdx >= 0 && iSlotIdx < NTL_MAX_PC_HAVE_SKILL, "CNtlSkillContainer::SetSkillSerial => slot id is invalid");

	m_hSerial[iSlotIdx] = hSerial;
}

void CNtlSkillContainer::SetHTBSkillSerial(RwInt32 iSlotIdx, SERIAL_HANDLE hSerial)
{
	NTL_ASSERT(iSlotIdx >= 0 && iSlotIdx < NTL_HTB_MAX_PC_HAVE_HTB_SKILL, "CNtlSkillContainer::SetHTBSkillSerial => slot id is invalid");

	m_hHTBSerial[iSlotIdx] = hSerial;
}

void CNtlSkillContainer::SetActionSkillSerial(RwInt32 iSlotIdx, SERIAL_HANDLE hSerial)
{
	NTL_ASSERT(iSlotIdx >= 0 && iSlotIdx < NTL_MAX_PC_HAVE_ACTION_SKILL, "CNtlSkillContainer::SetActionSkillSerial => slot id is invalid");

	m_hActionSerial[iSlotIdx] = hSerial;
}

RwInt32 CNtlSkillContainer::GetSkillCount(void)
{
	return NTL_MAX_PC_HAVE_SKILL;
}

CNtlSobSkill* CNtlSkillContainer::GetSkill(RwInt32 iSlotIdx)
{
	if(iSlotIdx < 0 && iSlotIdx >= NTL_MAX_PC_HAVE_SKILL)
		return NULL;

	return reinterpret_cast<CNtlSobSkill*>( GetNtlSobManager()->GetSobObject(m_hSerial[iSlotIdx]) );
}

CNtlSobSkill* CNtlSkillContainer::GetSkillFromTableId(RwUInt32 uiTblId)
{
	CNtlSob *pSobObj;
	CNtlSobSkillAttr *pSobSkillAttr;
	for(RwInt32 i = 0; i < NTL_MAX_PC_HAVE_SKILL; i++)
	{
		if(m_hSerial[i] == INVALID_SERIAL_ID)
			continue;

		pSobObj = GetNtlSobManager()->GetSobObject(m_hSerial[i]);
		pSobSkillAttr = reinterpret_cast<CNtlSobSkillAttr*>( pSobObj->GetSobAttr() );
		sSKILL_TBLDAT *pSkillTblData = pSobSkillAttr->GetSkillTbl();
		if(pSkillTblData->tblidx == uiTblId)
			return reinterpret_cast<CNtlSobSkill*>( pSobObj );
	}

	return NULL;
}

CNtlSobSkill* CNtlSkillContainer::GetSameKindSkillFromTableId(RwUInt32 uiTblId)
{
	CNtlSob *pSobObj;
	CNtlSobSkillAttr *pSobSkillAttr;
	for(RwInt32 i = 0; i < NTL_MAX_PC_HAVE_SKILL; i++)
	{
		if(m_hSerial[i] == INVALID_SERIAL_ID)
			continue;

		pSobObj = GetNtlSobManager()->GetSobObject(m_hSerial[i]);
		pSobSkillAttr = reinterpret_cast<CNtlSobSkillAttr*>( pSobObj->GetSobAttr() );
		sSKILL_TBLDAT *pSkillTblData = pSobSkillAttr->GetSkillTbl();
		if(pSkillTblData->tblidx / 10 == uiTblId / 10 )
			return reinterpret_cast<CNtlSobSkill*>( pSobObj );
	}

	return NULL;
}

sSKILL_TBLDAT* CNtlSkillContainer::GetSkillFromTable(RwInt32 iSlotIdx)
{
	CNtlSobSkill *pSobSkill = GetSkill(iSlotIdx);
	if(pSobSkill == NULL)
		return NULL;

	CNtlSobSkillAttr *pSobSkillAttr = reinterpret_cast<CNtlSobSkillAttr*>( pSobSkill->GetSobAttr() );
	return pSobSkillAttr->GetSkillTbl();
}

RwInt32 CNtlSkillContainer::GetSkillSlotIdx( SERIAL_HANDLE hSerial )
{
	for( RwInt32 i = 0 ; i < NTL_MAX_PC_HAVE_SKILL ; ++i )
	{
		if( hSerial == m_hSerial[i] )
			return i;
	}

	return -1;
}

RwBool CNtlSkillContainer::SkillExists( TBLIDX minIdx, TBLIDX maxIdx )
{
	if( minIdx == INVALID_TBLIDX || maxIdx == INVALID_TBLIDX )
		return FALSE;

	CNtlSob* pSobObj;
	CNtlSobSkillAttr* pSobSkillAttr;
	for( RwInt32 i = 0 ; i < NTL_MAX_PC_HAVE_SKILL ; ++i )
	{
		if( m_hSerial[i] == INVALID_SERIAL_ID )
			continue;

		pSobObj = GetNtlSobManager()->GetSobObject( m_hSerial[i] );
		pSobSkillAttr = reinterpret_cast<CNtlSobSkillAttr*>( pSobObj->GetSobAttr() );
		sSKILL_TBLDAT *pSkillTblData = pSobSkillAttr->GetSkillTbl();
		
		if( pSkillTblData->tblidx >= minIdx && pSkillTblData->tblidx <= maxIdx )
			return TRUE;	
	}

	return FALSE;
}

RwInt32 CNtlSkillContainer::GetHTBSkillCount(void)
{
	return NTL_HTB_MAX_PC_HAVE_HTB_SKILL;
}

CNtlSobHTBSkill* CNtlSkillContainer::GetHTBSkill(RwInt32 iSlotIdx)
{
	if(iSlotIdx < 0 && iSlotIdx >= NTL_HTB_MAX_PC_HAVE_HTB_SKILL)
		return NULL;

	return reinterpret_cast<CNtlSobHTBSkill*>( GetNtlSobManager()->GetSobObject(m_hHTBSerial[iSlotIdx]) );
}

CNtlSobHTBSkill* CNtlSkillContainer::GetHTBSkillFromTableId(RwUInt32 uiTblId)
{
	CNtlSob *pSobObj;
	CNtlSobHTBSkillAttr *pSobHTBSkillAttr;
	for(RwInt32 i = 0; i < NTL_HTB_MAX_PC_HAVE_HTB_SKILL; i++)
	{
		if(m_hHTBSerial[i] == INVALID_SERIAL_ID)
			continue;

		pSobObj = GetNtlSobManager()->GetSobObject(m_hHTBSerial[i]);
		pSobHTBSkillAttr = reinterpret_cast<CNtlSobHTBSkillAttr*>( pSobObj->GetSobAttr() );
		sHTB_SET_TBLDAT *pHTBSkillTblData = pSobHTBSkillAttr->GetHTBSkillTbl();
		if(pHTBSkillTblData->tblidx == uiTblId)
			return reinterpret_cast<CNtlSobHTBSkill*>( pSobObj );
	}

	return NULL;
}

sHTB_SET_TBLDAT* CNtlSkillContainer::GetHTBSkillFromTable(RwInt32 iSlotIdx)
{
	CNtlSobHTBSkill *pSobHTBSkill = GetHTBSkill(iSlotIdx);
	if(pSobHTBSkill == NULL)
		return NULL;

	CNtlSobHTBSkillAttr *pSobHTBSkillAttr = reinterpret_cast<CNtlSobHTBSkillAttr*>( pSobHTBSkill->GetSobAttr() );
	return pSobHTBSkillAttr->GetHTBSkillTbl();
}

RwInt32 CNtlSkillContainer::GetHTBSkillSlotIDx( SERIAL_HANDLE hSerial )
{
	for( RwInt32 i = 0 ; i < NTL_HTB_MAX_PC_HAVE_HTB_SKILL ; ++i )
	{
		if( hSerial == m_hHTBSerial[i] )
			return i;
	}

	return -1;
}

RwInt32 CNtlSkillContainer::GetActionSkillCount(void)
{
	return NTL_MAX_PC_HAVE_ACTION_SKILL;
}

CNtlSobActionSkill* CNtlSkillContainer::GetActionSkill(RwInt32 iSlotIdx)
{
	if(iSlotIdx < 0 || iSlotIdx >= NTL_MAX_PC_HAVE_ACTION_SKILL)
		return NULL;

	return reinterpret_cast<CNtlSobActionSkill*>( GetNtlSobManager()->GetSobObject(m_hActionSerial[iSlotIdx]) );
}

CNtlSobActionSkill* CNtlSkillContainer::GetActionSkillFromTableId(RwUInt32 uiTblId)
{
	CNtlSob *pSobObj;
	CNtlSobActionSkillAttr *pSobActionSkillAttr;
	for(RwInt32 i = 0; i < NTL_MAX_PC_HAVE_ACTION_SKILL; i++)
	{
		if(m_hActionSerial[i] == INVALID_SERIAL_ID)
			continue;

		pSobObj = GetNtlSobManager()->GetSobObject(m_hActionSerial[i]);
		pSobActionSkillAttr = reinterpret_cast<CNtlSobActionSkillAttr*>( pSobObj->GetSobAttr() );
		sACTION_TBLDAT *pActionSkillTblData = pSobActionSkillAttr->GetActionSkillTbl();
		if(pActionSkillTblData->tblidx == uiTblId)
			return reinterpret_cast<CNtlSobActionSkill*>( pSobObj );
	}

	return NULL;
}

sACTION_TBLDAT* CNtlSkillContainer::GetActionSkillFromTable(RwInt32 iSlotIdx)
{
	CNtlSobActionSkill *pSobActionSkill = GetActionSkill(iSlotIdx);
	if(pSobActionSkill == NULL)
		return NULL;

	CNtlSobActionSkillAttr *pSobActionSkillAttr = reinterpret_cast<CNtlSobActionSkillAttr*>( pSobActionSkill->GetSobAttr() );
	return pSobActionSkillAttr->GetActionSkillTbl();
}

RwInt32 CNtlSkillContainer::GetActionSkillSlotIDx( SERIAL_HANDLE hSerial )
{
	for( RwInt32 i = 0 ; i < NTL_MAX_PC_HAVE_ACTION_SKILL ; ++i )
	{
		if( hSerial == m_hActionSerial[i] )
			return i;
	}

	return -1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


CNtlAvatarSkillContainer::CNtlAvatarSkillContainer()
{
	m_bDashPossible = FALSE;
	m_bChargingPossible = FALSE;
	m_bBlockingPossible = FALSE;
	m_bAirPossible = false;
}

void CNtlAvatarSkillContainer::CreateEventHandler(RWS::CMsg &pMsg)
{
	SAvatarInfo *pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();

	CNtlSobSkill *pSobSkill;
	for(RwInt32 i = 0; i < pAvatarInfo->bySkillCount; i++)
	{
		pSobSkill = reinterpret_cast<CNtlSobSkill*>( CNtlSLEventGenerator::SobSkillCreate( pSobAvatar->GetSerialID(),
																						SLCLASS_SKILL, INVALID_SERIAL_ID, 
																						pAvatarInfo->sSkillPf[i].tblidx,
																						pAvatarInfo->sSkillPf[i].skillIndex,
																						pAvatarInfo->sSkillPf[i].byRpBonusType,
																						pAvatarInfo->sSkillPf[i].bIsRpBonusAuto,
																						pAvatarInfo->sSkillPf[i].nExp,
																						pAvatarInfo->sSkillPf[i].dwTimeRemaining) );

		SetSkillSerial(pAvatarInfo->sSkillPf[i].skillIndex, pSobSkill->GetSerialID());

		// passsive skill detecting
		PassiveSkillDetecting(pAvatarInfo->sSkillPf[i].tblidx);
	}
	// HTB Create
	CNtlSobHTBSkill *pSobHTBSkill;
	for(RwInt32 i = 0; i < pAvatarInfo->byHTBCount; i++)
	{
		pSobHTBSkill = reinterpret_cast<CNtlSobHTBSkill*>( CNtlSLEventGenerator::SobHTBSkillCreate(pSobAvatar->GetSerialID(),
																								 SLCLASS_HTBSKILL, INVALID_SERIAL_ID,
																								 pAvatarInfo->sHTBPf[i].skillId,
																								 pAvatarInfo->sHTBPf[i].bySlotId,
																								 pAvatarInfo->sHTBPf[i].dwTimeRemaining) );
		SetHTBSkillSerial(pAvatarInfo->sHTBPf[i].bySlotId, pSobHTBSkill->GetSerialID());
	}

	// sActionSkill Create
	CNtlSobActionSkill *pSobActionSkill;
	CActionTable* pActionTable = API_GetTableContainer()->GetActionTable();

	CActionTable::TABLEIT it;	
    RwUInt8 i = 0;
	for(i = 0 , it = pActionTable->Begin() ; it != pActionTable->End() ; ++it, ++i )
	{
		sACTION_TBLDAT* pActionTableData = reinterpret_cast<sACTION_TBLDAT*>( (*it).second );
		pSobActionSkill = reinterpret_cast<CNtlSobActionSkill*>( CNtlSLEventGenerator::SobActionSkillCreate(SLCLASS_ACTIONSKILL, INVALID_SERIAL_ID,
																											pActionTableData->tblidx, i) );	
		pSobActionSkill->SetOwnerID(pSobAvatar->GetSerialID());
		SetActionSkillSerial(i, pSobActionSkill->GetSerialID());		
	}	

	LIST_PASSIVE_SKILL_EFFECT* listPassiveSkillEffect = GetNtlSLGlobal()->GetPassiveSkillEffectInfo();
	LIST_PASSIVE_SKILL_EFFECT_ITER iter;
	for( iter = listPassiveSkillEffect->begin() ; iter != listPassiveSkillEffect->end() ; ++iter )
	{
		SPassiveSkillEffect* pPassiveEffectData = &(*iter);
		
		CNtlSobSkillAttr* pSobSkillAttr = reinterpret_cast<CNtlSobSkillAttr*>( GetSkill( pPassiveEffectData->bySlotidx )->GetSobAttr() );
		pSobSkillAttr->UpdatePassiveEffect( &pPassiveEffectData->sUpdatedSkillData );
	}
	listPassiveSkillEffect->clear();
}

void CNtlAvatarSkillContainer::AddEventHandler(RWS::CMsg &pMsg)
{
	SNtlEventSobSkillAdd *pSkillAdd = reinterpret_cast<SNtlEventSobSkillAdd*>(pMsg.pData);

	NTL_ASSERT(pSkillAdd->bySkillSlotIdx >= 0 && pSkillAdd->bySkillSlotIdx < NTL_MAX_PC_HAVE_SKILL,
		"CNtlAvatarSkillContainer::AddEventHandler => slot id( " << pSkillAdd->bySkillSlotIdx << " ) is range out");

	NTL_ASSERT(m_hSerial[pSkillAdd->bySkillSlotIdx] == INVALID_SERIAL_ID,
		"CNtlAvatarSkillContainer::AddEventHandler => slot id( " << pSkillAdd->bySkillSlotIdx << " ) is valid skill exist");

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlSobSkill *pSobSkill = reinterpret_cast<CNtlSobSkill*>( CNtlSLEventGenerator::SobSkillCreate(pSobAvatar->GetSerialID(),
																									SLCLASS_SKILL, INVALID_SERIAL_ID, 
																									pSkillAdd->uiSkillTblId,
																									pSkillAdd->bySkillSlotIdx,
																									DBO_RP_BONUS_TYPE_INVALID,
																									FALSE, 0) );
	
	SetSkillSerial(pSkillAdd->bySkillSlotIdx, pSobSkill->GetSerialID());

	// passive skill detecting
	PassiveSkillDetecting(pSkillAdd->uiSkillTblId);
}

void CNtlAvatarSkillContainer::UpgradeEventHandler(RWS::CMsg &pMsg)
{
	SNtlEventSobSkillUpgrade *pSkillUpgrade = reinterpret_cast<SNtlEventSobSkillUpgrade*>(pMsg.pData);

	NTL_ASSERT(pSkillUpgrade->bySkillSlotIdx >= 0 && pSkillUpgrade->bySkillSlotIdx < NTL_MAX_PC_HAVE_SKILL,
		"CNtlAvatarSkillContainer::UpgradeEventHandler => slot id( " << pSkillUpgrade->bySkillSlotIdx << " ) is range out");

	NTL_ASSERT(m_hSerial[pSkillUpgrade->bySkillSlotIdx] != INVALID_SERIAL_ID,
		"CNtlAvatarSkillContainer::UpgradeEventHandler => slot id( " << pSkillUpgrade->bySkillSlotIdx << " ) is not exist");

	CNtlSobSkill *pSobSkill = reinterpret_cast<CNtlSobSkill*>( GetNtlSobManager()->GetSobObject(m_hSerial[pSkillUpgrade->bySkillSlotIdx]) );

	NTL_ASSERT(pSobSkill, "CNtlAvatarSkillContainer::UpgradeEventHandler => skill object not exist");
	pSobSkill->HandleEvents(pMsg);
}

void CNtlAvatarSkillContainer::SkillAction(RWS::CMsg &pMsg)
{
	SNtlEventSobSkillAction *pSkillAction = reinterpret_cast<SNtlEventSobSkillAction*>( pMsg.pData );
	CNtlSobSkill *pSobSkill = GetSkillFromTableId(pSkillAction->uiSkillTblId);
	if(pSobSkill == NULL)
	{
		NTL_ASSERTFAIL( "Skill TBLID " << pSkillAction->uiSkillTblId << " is Invalid" );
		return;
	}

	// RpBonusType에 따른 쿨타임 감소효과가 추가되었기 때문에 스킬아이콘 쿨타임은 밑의 함수를 이용해야 한다.
	CNtlSobSkillIcon *pSobSkillIcon = reinterpret_cast<CNtlSobSkillIcon*>( pSobSkill->GetIcon() );
	pSobSkillIcon->SetSkillIconState( CNtlSobSkillIcon::ICON_STATE_COOLING, pSkillAction->byRpBonusType, TRUE );
}

void CNtlAvatarSkillContainer::AddHTBEventHandler(RWS::CMsg& msg)
{
	SNtlEventSobHTBSkillAdd *pHTBSkillAdd = reinterpret_cast<SNtlEventSobHTBSkillAdd*>(msg.pData);

	NTL_ASSERT(pHTBSkillAdd->byHTBSkillSlotIdx >= 0 && pHTBSkillAdd->byHTBSkillSlotIdx < NTL_HTB_MAX_PC_HAVE_HTB_SKILL,
		"CNtlAvatarSkillContainer::AddEventHandler => slot id( " << pHTBSkillAdd->byHTBSkillSlotIdx << " ) is range out");

	NTL_ASSERT(m_hHTBSerial[pHTBSkillAdd->byHTBSkillSlotIdx] == INVALID_SERIAL_ID,
		"CNtlAvatarSkillContainer::AddEventHandler => slot id( " << pHTBSkillAdd->byHTBSkillSlotIdx << " ) is valid skill exist");

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlSobHTBSkill *pSobHTBSkill = reinterpret_cast<CNtlSobHTBSkill*>( CNtlSLEventGenerator::SobHTBSkillCreate(pSobAvatar->GetSerialID(),
																												SLCLASS_HTBSKILL, INVALID_SERIAL_ID, 
 																												pHTBSkillAdd->uiHTBSkillTblId,
																												pHTBSkillAdd->byHTBSkillSlotIdx) );	
	
	SetHTBSkillSerial(pHTBSkillAdd->byHTBSkillSlotIdx, pSobHTBSkill->GetSerialID());
}

void CNtlAvatarSkillContainer::HTBSkillAction(RWS::CMsg& msg)
{
	SNtlEventSobHTBSkillAction* pHTBSkillAction = reinterpret_cast<SNtlEventSobHTBSkillAction*>(msg.pData);
	sCHARSTATE_HTB *pHTB = &pHTBSkillAction->pCharState->sCharStateDetail.sCharStateHTB;

	CNtlSobHTBSkill* pSobHTBSkill = GetHTBSkillFromTableId( pHTB->HTBId );
	if(pSobHTBSkill == NULL)
		return;

	CNtlSobIcon* pSobIcon = pSobHTBSkill->GetIcon();
	pSobIcon->SetIconState(CNtlSobIcon::ICON_STATE_COOLING);	
}

void CNtlAvatarSkillContainer::SocialActionCooling(RWS::CMsg& msg)
{
	SNtlEventSobSocialActionCooling* pSocialActionCooling = reinterpret_cast<SNtlEventSobSocialActionCooling*>(msg.pData);
	CNtlSobActionSkill* pSelectedActionSkill = GetActionSkillFromTableId( pSocialActionCooling->uiTblIdx );
	if( pSelectedActionSkill == NULL )
	{
		NTL_ASSERTFAIL( "SocialAction TBLID " << pSocialActionCooling->uiTblIdx << " is Invalid" );
		return;
	}

	for( RwInt32 i = 0 ; i < NTL_MAX_PC_HAVE_ACTION_SKILL ; ++i )
	{
		if( m_hActionSerial[i] == INVALID_SERIAL_ID )
			continue;

		CNtlSobActionSkill* pActionSkill = reinterpret_cast<CNtlSobActionSkill*>( GetNtlSobManager()->GetSobObject( m_hActionSerial[i] ) );
		if( pActionSkill == NULL )
		{
			NTL_ASSERTFAIL( "ActionSkill Slot " << i << " has invalid serial" );
			return;
		}

		pActionSkill->GetIcon()->SetIconState( CNtlSobIcon::ICON_STATE_COOLING );
	}	
}

void CNtlAvatarSkillContainer::RPBonusSetupHandler(RWS::CMsg& msg)
{
	SNtlEventRpBonusSetupRes* pData = reinterpret_cast<SNtlEventRpBonusSetupRes*>( msg.pData );

	CNtlSobSkill* pSobSkill = NULL;
	pSobSkill = reinterpret_cast<CNtlSobSkill*>( GetNtlSobManager()->GetSobObject( m_hSerial[pData->bySlotIndex] ) );
	pSobSkill->GetSobAttr()->HandleEvents( msg );	
}

void CNtlAvatarSkillContainer::SkillInitHandler( RWS::CMsg& msg )
{
	RwInt32 i = 0;

	// Initialize skill information. Do not initialize a guy whose NeedSp is 0.
	for( i = 0 ; i < NTL_MAX_PC_HAVE_SKILL ; ++i )
	{
		if( m_hSerial[i] == INVALID_SERIAL_ID )
			continue;

		//CNtlSobSkillAttr* pSobSkillAttr = reinterpret_cast<CNtlSobSkillAttr*>( GetSkill( i )->GetSobAttr() );
		//if( pSobSkillAttr->GetSkillTbl()->wRequireSP > 0 && pSobSkillAttr->GetSkillTbl()->wRequireSP != INVALID_WORD )
		{
			CNtlSLEventGenerator::SobDelete( m_hSerial[i] );
			m_hSerial[i] = INVALID_SERIAL_ID;
		}		
	}

	// by daneos
	// we do not init HTB skills

	//for( i = 0 ; i < NTL_HTB_MAX_PC_HAVE_HTB_SKILL ; ++i )
	//{
	//	if( m_hHTBSerial[i] == INVALID_SERIAL_ID )
	//		continue;

	//	//CNtlSobHTBSkillAttr* pSobHTBSkillAttr = reinterpret_cast<CNtlSobHTBSkillAttr*>( GetSkill( i )->GetSobAttr() );
	//	//if( pSobHTBSkillAttr->GetHTBSkillTbl()->wRequireSP > 0 && pSobHTBSkillAttr->GetHTBSkillTbl()->wRequireSP != INVALID_WORD )
	//	{
	//		CNtlSLEventGenerator::SobDelete( m_hHTBSerial[i] );
	//		m_hHTBSerial[i] = INVALID_SERIAL_ID;
	//	}				
	//}
}

void CNtlAvatarSkillContainer::PassiveSkillEffectHandler( RWS::CMsg& msg )
{
	SNtlEventSetSkillPassiveEffect* pPassiveEffect = reinterpret_cast<SNtlEventSetSkillPassiveEffect*>( msg.pData );

	CNtlSobSkillAttr* pSobSkillAttr = reinterpret_cast<CNtlSobSkillAttr*>( GetSkill( pPassiveEffect->bySkillSlotIdx )->GetSobAttr() );
	pSobSkillAttr->UpdatePassiveEffect( pPassiveEffect->pPassiveEffectData );
}

void CNtlAvatarSkillContainer::PassiveSkillDetecting(RwUInt32 uiTblId)
{
	CSkillTable *pSkillTbl = API_GetTableContainer()->GetSkillTable();
	CSystemEffectTable *pEffectTbl = API_GetTableContainer()->GetSystemEffectTable();

	sSKILL_TBLDAT *pSkillTblData = reinterpret_cast<sSKILL_TBLDAT*>( pSkillTbl->FindData(uiTblId) );
	if (!pSkillTblData)
		return;

	for(RwInt32 i = 0; i < NTL_MAX_EFFECT_IN_SKILL; ++i)
	{
		if(pSkillTblData->skill_Effect[i] == INVALID_TBLIDX)
			continue;

		sSYSTEM_EFFECT_TBLDAT *pEffectTblData = reinterpret_cast<sSYSTEM_EFFECT_TBLDAT*>( pEffectTbl->FindData(pSkillTblData->skill_Effect[i]) );
		if(pEffectTblData->effectCode == PASSIVE_DASH)
		{
			m_dwDashReqLP = pSkillTblData->dwRequire_LP;
			m_wDashReqEP = pSkillTblData->wRequire_EP;
			m_wDashReqRP = (RwUInt16)pSkillTblData->byRequire_RP_Ball;
			m_bDashPossible = TRUE;
		}
		else if(pEffectTblData->effectCode == PASSIVE_CHARGE)
		{
			m_bChargingPossible = TRUE;
		}
		else if(pEffectTblData->effectCode == PASSIVE_BLOCK_MODE)
		{
			m_bBlockingPossible = TRUE;
		}
		else if (pEffectTblData->effectCode == PASSIVE_AIR_MASTERY)
		{
			if (m_bAirPossible == false)
			{
				m_bAirPossible = true;

				CNtlSLEventGenerator::EnableAir(true);
			}
			m_bAirPossible = true;
		}
	}
}

void CNtlAvatarSkillContainer::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventSobCreate)
	{
		CreateEventHandler(pMsg);
	}
	else if(pMsg.Id == g_EventSobSkillAdd)
	{
		AddEventHandler(pMsg);
	}
	else if(pMsg.Id == g_EventSobSkillUpgrade)
	{
		UpgradeEventHandler(pMsg);
	}
	else if(pMsg.Id == g_EventSobSkillAction)
	{
		SkillAction(pMsg);
	}
	else if(pMsg.Id == g_EventSobHTBSkillAdd)
	{
		AddHTBEventHandler(pMsg);
	}
	else if(pMsg.Id == g_EventSobHTBSkillAction)
	{
		HTBSkillAction(pMsg);
	}
	else if(pMsg.Id == g_EventSobSocialActionCooling)
	{
		SocialActionCooling(pMsg);
	}
	else if(pMsg.Id == g_EventRpBonusSetupRes)
	{
		RPBonusSetupHandler(pMsg);
	}
	else if( pMsg.Id == g_EventSkillInit )
	{
		SkillInitHandler( pMsg );
	}	
	else if( pMsg.Id == g_EventSetSkillPassiveEffect )
	{
		PassiveSkillEffectHandler( pMsg );
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNtlPetSkillContainer::CNtlPetSkillContainer()
{
	for(RwInt32 i = 0; i < NTL_MAX_NPC_HAVE_SKILL; ++i)
	{
		m_hSerial[i]	= INVALID_SERIAL_ID;
		m_byUseBasis[i]	= INVALID_BYTE;
		m_wUseLp[i] = INVALID_WORD;
		m_wUseTime[i] = INVALID_WORD;
	}
}

CNtlPetSkillContainer::~CNtlPetSkillContainer()
{
}

RwBool CNtlPetSkillContainer::Create(void)
{
	return TRUE;
}

void CNtlPetSkillContainer::Destroy(void)
{
	NTL_FUNCTION("CNtlPetSkillContainer::Destroy");

	for(RwInt32 i = 0; i < NTL_MAX_NPC_HAVE_SKILL; ++i)
	{
		if(m_hSerial[i] != INVALID_SERIAL_ID)
			CNtlSLEventGenerator::SobDelete(m_hSerial[i]);
	}
	
	NTL_RETURNVOID();
}

void CNtlPetSkillContainer::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventSobCreate)
	{
		SNtlEventSobPetCreate *pPetCreate = reinterpret_cast<SNtlEventSobPetCreate*>( pMsg.pData );

		// Skill의 AI 조건을 읽기 위해 NPC 테이블을 읽어놓는다.
		sNPC_TBLDAT *pNpcTblData;
		CNPCTable *pNpcTable = API_GetTableContainer()->GetNpcTable();

		if(pPetCreate->bOwnerAvatar)
			pNpcTblData = reinterpret_cast<sNPC_TBLDAT*>( pNpcTable->FindData(pPetCreate->uPetBrief.pPetProfile->npcTblidx) );
		else
			pNpcTblData = reinterpret_cast<sNPC_TBLDAT*>( pNpcTable->FindData(pPetCreate->uPetBrief.pPetBrief->npcTblidx) );

		SSummonPetSkillInfo* pSummonPetSkillInfo = GetNtlSLGlobal()->GetAvatarSummonPetSkillInfo();
		CNtlSob *pSobObj = reinterpret_cast<CNtlSob*>( pPetCreate->pSobObj );
		CNtlSobSkill *pSobSkill;

		for(RwInt32 i = 0; i< pSummonPetSkillInfo->bySkillCount; ++i )
		{
			pSobSkill = reinterpret_cast<CNtlSobSkill*>( CNtlSLEventGenerator::SobSkillCreate(
				pSobObj->GetSerialID(),
				SLCLASS_SKILL, INVALID_SERIAL_ID, 
				pSummonPetSkillInfo->aSkillInfo[i].tblidx,		// Table Index
				pSummonPetSkillInfo->aSkillInfo[i].skillIndex,	// Slot Index
				DBO_RP_BONUS_TYPE_INVALID, 
				FALSE, 0) );

			SetSkillSerial(pSummonPetSkillInfo->aSkillInfo[i].skillIndex, pSobSkill->GetSerialID());
			
			// Table Index와 매칭되는 AI 데이타를 읽어서 Skill에 세팅한다.
			for(RwInt32 j = 0; j < NTL_MAX_NPC_HAVE_SKILL; ++j )
			{
				if( pSummonPetSkillInfo->aSkillInfo[i].tblidx == pNpcTblData->use_Skill_Tblidx[j] )
				{
					// AI 세팅
					SetSkillUseBasis( pSummonPetSkillInfo->aSkillInfo[i].skillIndex, pNpcTblData->byUse_Skill_Basis[j] );
					SetSkillUseLp( pSummonPetSkillInfo->aSkillInfo[i].skillIndex, pNpcTblData->wUse_Skill_LP[j] );
					SetSkillUseTime( pSummonPetSkillInfo->aSkillInfo[i].skillIndex, pNpcTblData->wUse_Skill_Time[j] );
				}
			}
		}
	}
	else if(pMsg.Id == g_EventSobSkillAction)
	{
		SNtlEventSobSkillAction *pSkillAction = reinterpret_cast<SNtlEventSobSkillAction*>( pMsg.pData );
		CNtlSobSkill *pSobSkill = GetSkillFromTableId(pSkillAction->uiSkillTblId);
		if(pSobSkill == NULL)
			return;

		CNtlSobSkillIcon *pSobSkillIcon = reinterpret_cast<CNtlSobSkillIcon*>( pSobSkill->GetIcon() );
		// Skill Icon 의 쿨링 타임
		pSobSkillIcon->SetSkillIconState( CNtlSobSkillIcon::ICON_STATE_COOLING, INVALID_BYTE, TRUE );
	}
}

void CNtlPetSkillContainer::SetSkillSerial(RwInt32 iSlotIdx, SERIAL_HANDLE hSerial)
{
	NTL_ASSERT(iSlotIdx >= 0 && iSlotIdx < NTL_MAX_NPC_HAVE_SKILL, "CNtlPetSkillContainer::SetSkillSerial => slot id is invalid");

	m_hSerial[iSlotIdx] = hSerial;
}

void CNtlPetSkillContainer::SetSkillUseBasis(RwInt32 iSlotIdx, RwUInt8 byUseBasis)
{
	NTL_ASSERT(iSlotIdx >= 0 && iSlotIdx < NTL_MAX_NPC_HAVE_SKILL, "CNtlPetSkillContainer::SetSkillUseBasis => slot id is invalid");

	m_byUseBasis[iSlotIdx] = byUseBasis;
}

void CNtlPetSkillContainer::SetSkillUseLp( RwInt32 iSlotIdx, RwUInt16 wUseLp ) 
{
	NTL_ASSERT(iSlotIdx >= 0 && iSlotIdx < NTL_MAX_NPC_HAVE_SKILL, "CNtlPetSkillContainer::SetSkillUseLp => slot id is invalid");

	m_wUseLp[iSlotIdx] = wUseLp;
}

void CNtlPetSkillContainer::SetSkillUseTime(RwInt32 iSlotIdx, RwUInt16 wUseTime)
{
	NTL_ASSERT(iSlotIdx >= 0 && iSlotIdx < NTL_MAX_NPC_HAVE_SKILL, "CNtlPetSkillContainer::SetSkillUseTime => slot id is invalid");

	m_wUseTime[iSlotIdx] = wUseTime;
}

RwInt32 CNtlPetSkillContainer::GetSkillCount(void)
{
	return NTL_MAX_NPC_HAVE_SKILL;
}

CNtlSobSkill* CNtlPetSkillContainer::GetSkill(RwInt32 iSlotIdx)
{
	if(iSlotIdx < 0 && iSlotIdx >= NTL_MAX_NPC_HAVE_SKILL)
		return NULL;

	return reinterpret_cast<CNtlSobSkill*>( GetNtlSobManager()->GetSobObject(m_hSerial[iSlotIdx]) );
}

CNtlSobSkill* CNtlPetSkillContainer::GetSkillFromTableId(RwUInt32 uiTblId)
{
	CNtlSob *pSobObj;
	CNtlSobSkillAttr *pSobSkillAttr;
	for(RwInt32 i = 0; i < NTL_MAX_NPC_HAVE_SKILL; i++)
	{
		if(m_hSerial[i] == INVALID_SERIAL_ID)
			continue;

		pSobObj = GetNtlSobManager()->GetSobObject(m_hSerial[i]);
		pSobSkillAttr = reinterpret_cast<CNtlSobSkillAttr*>( pSobObj->GetSobAttr() );
		sSKILL_TBLDAT *pSkillTblData = pSobSkillAttr->GetSkillTbl();
		if(pSkillTblData->tblidx == uiTblId)
			return reinterpret_cast<CNtlSobSkill*>( pSobObj );
	}

	return NULL;
}

sSKILL_TBLDAT* CNtlPetSkillContainer::GetSkillFromTable(RwInt32 iSlotIdx)
{
	CNtlSobSkill *pSobSkill = GetSkill(iSlotIdx);
	if(pSobSkill == NULL)
		return NULL;

	CNtlSobSkillAttr *pSobSkillAttr = reinterpret_cast<CNtlSobSkillAttr*>( pSobSkill->GetSobAttr() );
	return pSobSkillAttr->GetSkillTbl();
}

RwInt32 CNtlPetSkillContainer::GetSkillSlotIdx( SERIAL_HANDLE hSerial )
{
	for( RwInt32 i = 0 ; i < NTL_MAX_NPC_HAVE_SKILL ; ++i )
	{
		if( hSerial == m_hSerial[i] )
			return i;
	}

	return -1;
}

RwUInt8 CNtlPetSkillContainer::GetSkillUseBasis(RwInt32 iSlotIdx)
{
	if(iSlotIdx < 0 && iSlotIdx >= NTL_MAX_NPC_HAVE_SKILL)
		return 0;

	return m_byUseBasis[iSlotIdx];
}

RwUInt16 CNtlPetSkillContainer::GetSkillUseLp(RwInt32 iSlotIdx)
{
	if(iSlotIdx < 0 && iSlotIdx >= NTL_MAX_NPC_HAVE_SKILL)
		return 0;

	return m_wUseLp[iSlotIdx];
}

RwUInt16 CNtlPetSkillContainer::GetSkillUseTime(RwInt32 iSlotIdx)
{
	if(iSlotIdx < 0 && iSlotIdx >= NTL_MAX_NPC_HAVE_SKILL)
		return 0;

	return m_wUseTime[iSlotIdx];
}