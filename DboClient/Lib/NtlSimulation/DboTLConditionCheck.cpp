#include "precomp_ntlsimulation.h"
#include "DboTLConditionCheck.h"

// core
#include "NtlDebug.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLLogic.h"
#include "DboTLConditionCheckState.h"


CDboTLConditionCheck::CDboTLConditionCheck()
{
	ZeroMemory(m_acMouseInput, dMAX_SIZE_MOUSE_INPUT_TYPE);
	ZeroMemory(m_acKeyboardInput, dMAX_SIZE_KEYBOARD_INPUT_TYPE);
	ZeroMemory(m_acAttack, dMAX_SIZE_ATTACK_INPUT_TYPE);


	CDboConditionCheckBase* pConditionCheck;

	pConditionCheck = NTL_NEW CDboConditionCheck_MouseDrag;
	m_listConditoinCheck.push_back(pConditionCheck);

	pConditionCheck = NTL_NEW CDboConditionCheck_CheckPoint;
	m_listConditoinCheck.push_back(pConditionCheck);

	pConditionCheck = NTL_NEW CDboConditionCheck_Mob_Target;
	m_listConditoinCheck.push_back(pConditionCheck);

	pConditionCheck = NTL_NEW CDboConditionCheck_Mob_NormalAttack;
	m_listConditoinCheck.push_back(pConditionCheck);

	pConditionCheck = NTL_NEW CDboConditionCheck_Mob_SkillAttack;
	m_listConditoinCheck.push_back(pConditionCheck);
}

CDboTLConditionCheck::~CDboTLConditionCheck()
{
    Destroy();
}

RwBool CDboTLConditionCheck::Create(void)
{
	NTL_FUNCTION("CDboTLConditionCheck::Create");
    
	NTL_RETURN(TRUE);
}

void CDboTLConditionCheck::Destroy(void)
{
	NTL_FUNCTION("CDboTLConditionCheck::Destroy");

	LIST_CONDITIONCHECK_ITER it = m_listConditoinCheck.begin();
	for( ; it != m_listConditoinCheck.end() ; ++it )
	{
		CDboConditionCheckBase* pBase = *it;
		NTL_DELETE(pBase);
	}
	m_listConditoinCheck.clear();

	NTL_RETURNVOID();
}

VOID CDboTLConditionCheck::Update(RwReal fElapsed)
{
	LIST_CONDITIONCHECK_ITER it = m_listConditoinCheck.begin();
	for( ; it != m_listConditoinCheck.end() ; ++it )
		(*it)->Update(fElapsed);
}

RwBool CDboTLConditionCheck::CanMouseInput(ETL_MOUSE_INPUT_TYPE byType)
{
	return !IsExistFlag(m_acMouseInput, byType, ETL_MOUSE_INPUT_TYPE_ALL);
}

RwBool CDboTLConditionCheck::CanKeybaordInput(ETL_KEYBOARD_INPUT_TYPE byType)
{
	return !IsExistFlag(m_acKeyboardInput, byType, ETL_KEYBOARD_INPUT_TYPE_ALL);
}

RwBool CDboTLConditionCheck::CanAttack(ETL_ATTACK_TYPE byType)
{
	return !IsExistFlag(m_acAttack, byType, ETL_ATTACK_TYPE_ALL);
}

VOID CDboTLConditionCheck::SetFlags(RwChar* pcFlags, RwUInt32 uiIndex, RwUInt32 uiMaxIndex)
{
	if( uiIndex == uiMaxIndex )
	{
		for( RwUInt8 i = 0 ; i < uiMaxIndex ; ++i )
			Logic_SetFlags((RwChar*)pcFlags, i, uiMaxIndex);
	}
	else
	{
		Logic_SetFlags((RwChar*)pcFlags, uiIndex, uiMaxIndex);
	}
}

VOID CDboTLConditionCheck::UnsetFlags(RwChar* pcFlags, RwUInt32 uiIndex, RwUInt32 uiMaxIndex, RwUInt8 bySize)
{
	if( uiIndex == uiMaxIndex )
	{
		for( RwUInt8 i = 0 ; i < bySize ; ++i )
			pcFlags[i] = 0;
	}
	else
	{
		Logic_UnsetFlags((RwChar*)pcFlags, uiIndex, uiMaxIndex);
	}
}

RwBool CDboTLConditionCheck::IsExistFlag(RwChar* pcFlags, RwUInt32 uiIndex, RwUInt32 uiMaxIndex)
{
	if( uiIndex == uiMaxIndex )
	{
		for( RwUInt8 i = 0 ; i < uiMaxIndex ; ++i )
		{
			if( Logic_IsExistFlag((RwChar*)pcFlags, uiIndex, uiMaxIndex) == false )
				return false;
		}

		return true;
	}

	return Logic_IsExistFlag((RwChar*)pcFlags, uiIndex, uiMaxIndex);
}

VOID CDboTLConditionCheck::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CDboTLConditionCheck::HandleEvents");

	if( pMsg.Id == g_EventTLMouseInputLockUnlock )
	{
		SNtlEventTLMouseInputLockUnlock* pEvent = reinterpret_cast<SNtlEventTLMouseInputLockUnlock*>( pMsg.pData );

		if( pEvent->bLock )
			SetFlags(m_acMouseInput, pEvent->uiMouseInputType, ETL_MOUSE_INPUT_TYPE_ALL);
		else
			UnsetFlags(m_acMouseInput, pEvent->uiMouseInputType, ETL_MOUSE_INPUT_TYPE_ALL, dMAX_SIZE_MOUSE_INPUT_TYPE);

		CNtlSLEventGenerator::TLNotifyLockUnlock(TLLOCK_MOUSE, pEvent->uiMouseInputType, pEvent->bLock);
	}
	else if( pMsg.Id == g_EventTLKeyboardInputLockUnlock )
	{
		SNtlEventTLKeyboardInputLockUnlock* pEvent = reinterpret_cast<SNtlEventTLKeyboardInputLockUnlock*>( pMsg.pData );

		if( pEvent->bLock )
			SetFlags(m_acKeyboardInput, pEvent->uiKeyboardInputType, ETL_KEYBOARD_INPUT_TYPE_ALL);
		else
			UnsetFlags(m_acKeyboardInput, pEvent->uiKeyboardInputType, ETL_KEYBOARD_INPUT_TYPE_ALL, dMAX_SIZE_KEYBOARD_INPUT_TYPE);

		CNtlSLEventGenerator::TLNotifyLockUnlock(TLLOCK_KEYBOARD, pEvent->uiKeyboardInputType, pEvent->bLock);
	}
	else if( pMsg.Id == g_EventTLAttackLockUnlock )
	{
		SNtlEventTLAttackLockUnlock* pEvent = reinterpret_cast<SNtlEventTLAttackLockUnlock*>( pMsg.pData );

		if( pEvent->bLock )
			SetFlags(m_acAttack, pEvent->uiAttackType, ETL_ATTACK_TYPE_ALL);
		else
			UnsetFlags(m_acAttack, pEvent->uiAttackType, ETL_ATTACK_TYPE_ALL, dMAX_SIZE_ATTACK_INPUT_TYPE);

		CNtlSLEventGenerator::TLNotifyLockUnlock(TLLOCK_ATTACK, pEvent->uiAttackType, pEvent->bLock);
	}

	LIST_CONDITIONCHECK_ITER it = m_listConditoinCheck.begin();
	for( ; it != m_listConditoinCheck.end() ; ++it )
		(*it)->HandleEvents(pMsg);

	NTL_RETURNVOID();
}