#include "stdafx.h"
#include "BuffLinkedObject.h"
#include "NtlResultCode.h"
#include "NtlPacketGU.h"
#include "calcs.h"
#include "char.h"
#include "TableContainerManager.h"
#include "SkillTable.h"
#include "ItemTable.h"
#include "UseItemTable.h"
#include "CommonConfigTable.h"
#include "NtlRandom.h"


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CBuffManager::CBuffManager()
{
	Init();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CBuffManager::~CBuffManager()
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CBuffManager::Create(CCharacter *pOwnerRef)
{
	if (pOwnerRef == NULL)
	{
		ERR_LOG(LOG_USER, "pOwnerRef is NULL");
		return false;
	}

	m_pOwnerRef = pOwnerRef;

	if (m_blessBuffList.Create(NTL_MAX_BLESS_BUFF_CHARACTER_HAS, 0))
	{
		if (m_curseBuffList.Create(NTL_MAX_CURSE_BUFF_CHARACTER_HAS, 0))
		{
			if (m_subBuffList.Create(NTL_MAX_SUB_BUFF_CHARACTER_HAS, 0))
			{
				return true;
			}
		}
	}

	Destroy();
	return false;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CBuffManager::Destroy()
{
	m_pOwnerRef = NULL;
	memset(m_apBuff, NULL, sizeof(m_apBuff));

	//CLEAR BLESS BUFF
	for (unsigned int pos = m_blessBuffList.Begin();
		m_blessBuffList.End() != pos;
		pos = m_blessBuffList.Next(pos))
	{
		CBuff* pBuff = m_blessBuffList.GetAt(pos);
		if (pBuff)
		{
			SAFE_DELETE(pBuff);
			m_blessBuffList.GetAt(pos) = NULL;
		}
	}
	m_blessBuffList.Clear();

	//CLEAR CURSE BUFF
	for (unsigned int pos = m_curseBuffList.Begin();
		m_curseBuffList.End() != pos;
		pos = m_curseBuffList.Next(pos))
	{
		CBuff* pBuff = m_curseBuffList.GetAt(pos);
		if (pBuff)
		{
			SAFE_DELETE(pBuff);
			m_curseBuffList.GetAt(pos) = NULL;
		}
	}
	m_curseBuffList.Clear();

	//CLEAR SUB BUFF
	for (unsigned int pos = m_subBuffList.Begin();
		m_subBuffList.End() != pos;
		pos = m_subBuffList.Next(pos))
	{
		CBuff* pBuff = m_subBuffList.GetAt(pos);
		if (pBuff)
		{
			SAFE_DELETE(pBuff);
			m_subBuffList.GetAt(pos) = NULL;
		}
	}
	m_subBuffList.Clear();


	while (!m_listBuffOrderedByRegisteredTime.IsEmpty())
	{
		CBuffLinkedObject* pBuff = (CBuffLinkedObject*)m_listBuffOrderedByRegisteredTime.PopFront();
		SAFE_DELETE(pBuff);
	}

}



//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CBuffManager::Init()
{
	m_pOwnerRef = NULL;
	memset(m_apBuff, NULL, sizeof(m_apBuff));
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CBuffManager::TickProcess(DWORD dwTickDiff)
{
	if (m_pOwnerRef == NULL)
		return;

	BYTE byStateID = m_pOwnerRef->GetCharStateID();
	if (byStateID == CHARSTATE_DESPAWNING || byStateID == CHARSTATE_TELEPORTING)
		return;

	for (int i = 0; i < DBO_MAX_BUFF_CHARACTER_HAS; i++)
	{
		CBuff* pBuff = m_apBuff[i];
		if (pBuff)
		{
			pBuff->TickProcess(dwTickDiff);
		}
	}
}


void CBuffManager::LeaveGame()
{
	for (CBuffLinkedObject* pBuffLink = (CBuffLinkedObject*)m_listBuffOrderedByRegisteredTime.GetFirst();
		pBuffLink;
		pBuffLink = (CBuffLinkedObject*)pBuffLink->GetNext())
	{
		if (CBuff* pBuff = pBuffLink->GetBuffRef())
		{
			if (pBuff->GetSourceType() == DBO_OBJECT_SOURCE_SKILL)
			{
				if (sSKILL_TBLDAT* pSkill = (sSKILL_TBLDAT*)g_pTableContainer->GetSkillTable()->FindData(pBuff->GetSourceTblidx()))
				{
					if (pSkill->bKeep_Effect)
					{
						//remove CHARSTATE_KEEPING_EFFECT state from caster if no objects anymore left
						if (pBuff->GetCasterHandle() != m_pOwnerRef->GetID())
						{
							CCharacter* pCaster = g_pObjectManager->GetChar(pBuff->GetCasterHandle());
							if (pCaster && pCaster->GetCharStateID() == CHARSTATE_KEEPING_EFFECT)
								pCaster->EraseKeepEffectTarget(m_pOwnerRef->GetID());
						}
					}
				}
			}
		}
	}
}

bool CBuffManager::RegisterBuff(DWORD& rdwKeepTime, eSYSTEM_EFFECT_CODE * effectCode, sDBO_BUFF_PARAMETER * paBuffParameter, HOBJECT hCaster, eBUFF_TYPE buffType, sSKILL_TBLDAT * pSkillTbldat, BYTE* prBuffIndex)
{
	if (m_pOwnerRef->IsFainting())
		return false;

	switch (buffType)
	{
	case BUFF_TYPE_BLESS:
	{
		if (m_blessBuffList.GetSize() >= NTL_MAX_BLESS_BUFF_CHARACTER_HAS)
			return false;
	}
	break;
	case BUFF_TYPE_CURSE:
	{
		if (m_curseBuffList.GetSize() >= NTL_MAX_CURSE_BUFF_CHARACTER_HAS)
			return false;
	}
	break;

	default: ERR_LOG(LOG_GENERAL, "buffType %u not allowed here !!!", buffType); return false; break;
	}

	TBLIDX basicSkillTblidx = g_pTableContainer->GetSkillTable()->FindBasicSkillTblidx(pSkillTbldat->tblidx);

	//anti cc spam
	if (Dbo_IsCcReduction(effectCode[0]) || Dbo_IsCcReduction(effectCode[1]))
	{
		rdwKeepTime -= DWORD((float)rdwKeepTime * m_pOwnerRef->GetCrowdControlReduction(hCaster, basicSkillTblidx) / 100.0f);
	}

	if (pSkillTbldat->byBuff_Group == INVALID_BYTE) // (de)buffs with invalid buff_group can be stacked
		return true;

	//check if overwrite buff when same tblidx or group
	CBuff* pOldBuff = FindBuff(pSkillTbldat->byBuff_Group, DBO_OBJECT_SOURCE_SKILL);

	//printf("dwFunction_Bit_Flag %u, byClass_Type %u, bySkill_Type %u, bySkill_Active_Type %u, byBuff_Group %u, bySlot_Index %u, bySkill_Group %u \n", pSkillTbldat->dwFunction_Bit_Flag, pSkillTbldat->byClass_Type,
	//	pSkillTbldat->bySkill_Type, pSkillTbldat->bySkill_Active_Type, pSkillTbldat->byBuff_Group, pSkillTbldat->bySlot_Index, pSkillTbldat->bySkill_Group);

	if (pOldBuff)
	{
		if (pOldBuff->GetBuffType() != buffType) //return true if the old buff type is different from the new buff type to avoid curse buff remove bless buff and vice versa
			return true;

		bool bOverwrite = BIT_FLAG_TEST(SKILL_FUNCTION_FLAG_OVERWRITE_IF_SAME_GROUP, pSkillTbldat->dwFunction_Bit_Flag);
		TBLIDX basicBuffSkillTblidx = g_pTableContainer->GetSkillTable()->FindBasicSkillTblidx(pOldBuff->GetSourceTblidx());

		if ((bOverwrite && pOldBuff->GetBuffGroup() == pSkillTbldat->byBuff_Group) || basicSkillTblidx == basicBuffSkillTblidx)
		{
			m_pOwnerRef->GetBuffManager()->RemoveBuff(pOldBuff->GetBuffIndex(), pOldBuff->GetBuffType(), CBuff::BUFF_REMOVAL_REASON_REPLACED);
		}
		else
		{
			if (buffType == BUFF_TYPE_CURSE)
			{
				if (pOldBuff->GetBuffGroup() == pSkillTbldat->byBuff_Group) //required to renew current stun
				{
					m_pOwnerRef->GetBuffManager()->RemoveBuff(pOldBuff->GetBuffIndex(), pOldBuff->GetBuffType(), CBuff::BUFF_REMOVAL_REASON_REPLACED);
					return true;
				}
			}

			return false;
		}
	}

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	: Register a new Item buff
//		Return	: true/false
//-----------------------------------------------------------------------------------
bool CBuffManager::RegisterSubBuff(sBUFF_INFO* pBuffInfo, eSYSTEM_EFFECT_CODE* effectCode, HOBJECT hCaster, BYTE byBuffGroup, WORD & rwResultCode, BYTE* abyEffectType)
{
	if (m_subBuffList.GetSize() < NTL_MAX_SUB_BUFF_CHARACTER_HAS)
	{
		if (FindSubBuff(byBuffGroup, pBuffInfo->bySourceType))
		{
			rwResultCode = GAME_SKILL_ALREADY_EXIST_SAME_KIND_BUFF;
			return false;
		}

		rwResultCode = GAME_SUCCESS;
		return true;
	}

	rwResultCode = GAME_SKILL_SUB_BUFF_IS_FULL;
	return false;
}

void CBuffManager::EnableDisableBuff(BYTE byBuffIndex, eBUFF_TYPE buffType)
{
	CBuff* pBuffTmp = NULL;

	switch (buffType)
	{
		case BUFF_TYPE_BLESS:
		{
			for (unsigned int pos = m_blessBuffList.Begin(); pos != m_blessBuffList.End(); pos = m_blessBuffList.Next(pos))
			{
				CBuff* pBuff = m_blessBuffList.GetAt(pos);
				if (pBuff && pBuff->GetBuffIndex() == byBuffIndex)
				{
					pBuffTmp = pBuff;
					break;
				}
			}
		}
		break;
		case BUFF_TYPE_CURSE:
		{
			for (unsigned int pos = m_curseBuffList.Begin(); pos != m_curseBuffList.End(); pos = m_curseBuffList.Next(pos))
			{
				CBuff* pBuff = m_curseBuffList.GetAt(pos);
				if (pBuff && pBuff->GetBuffIndex() == byBuffIndex)
				{
					pBuffTmp = pBuff;
					break;
				}
			}
		}
		break;
		case BUFF_TYPE_SUB:
		{
			for (unsigned int pos = m_subBuffList.Begin(); pos != m_subBuffList.End(); pos = m_subBuffList.Next(pos))
			{
				CBuff* pBuff = m_subBuffList.GetAt(pos);
				if (pBuff && pBuff->GetBuffIndex() == byBuffIndex)
				{
					pBuffTmp = pBuff;
					break;
				}
			}
		}
		break;

		default: ERR_LOG(LOG_GENERAL, "buffType % not allowed here \n", buffType);
	}

	if (pBuffTmp)
	{
		pBuffTmp->ActivateBuff(pBuffTmp->GetBuffInfo()->bActive == false);
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	: 
//		Return	: 
//-----------------------------------------------------------------------------------
void CBuffManager::RemoveBuff(BYTE byBuffIndex, eBUFF_TYPE buffType, CBuff::BUFF_REMOVAL_REASON nReason)
{
	switch (buffType)
	{
		case BUFF_TYPE_BLESS:
		{
			for (unsigned int pos = m_blessBuffList.Begin(); pos != m_blessBuffList.End(); pos = m_blessBuffList.Next(pos))
			{
				CBuff* pBuff = m_blessBuffList.GetAt(pos);
				if (pBuff && pBuff->GetBuffIndex() == byBuffIndex)
				{
					RemoveBuff_Internal(nReason, &m_blessBuffList, pos);
					return;
				}
			}
		}
		break;
		case BUFF_TYPE_CURSE:
		{
			for (unsigned int pos = m_curseBuffList.Begin(); pos != m_curseBuffList.End(); pos = m_curseBuffList.Next(pos))
			{
				CBuff* pBuff = m_curseBuffList.GetAt(pos);
				if (pBuff && pBuff->GetBuffIndex() == byBuffIndex)
				{
					RemoveBuff_Internal(nReason, &m_curseBuffList, pos);
					return;
				}
			}
		}
		break;
		case BUFF_TYPE_SUB:
		{
			for (unsigned int pos = m_subBuffList.Begin(); pos != m_subBuffList.End(); pos = m_subBuffList.Next(pos))
			{
				CBuff* pBuff = m_subBuffList.GetAt(pos);
				if (pBuff && pBuff->GetBuffIndex() == byBuffIndex)
				{
					RemoveBuff_Internal(nReason, &m_subBuffList, pos);
					return;
				}
			}
		}
		break;

		default: ERR_LOG(LOG_GENERAL, "buffType % not allowed here \n", buffType);
	}
}

void CBuffManager::RemoveBuffTS(BYTE byBuffIndex, eBUFF_TYPE buffType, CBuff::BUFF_REMOVAL_REASON nReason)
{
	switch (buffType)
	{
		case BUFF_TYPE_BLESS:
		{
			for (unsigned int pos = m_blessBuffList.Begin(); pos != m_blessBuffList.End(); pos = m_blessBuffList.Next(pos))
			{
				CBuff* pBuff = m_blessBuffList.GetAt(pos);
				if (pBuff && pBuff->GetBuffIndex() == byBuffIndex)
				{
					pBuff->GetBuffInfo()->dwTimeRemaining = 0;
					return;
				}
			}
		}
		break;
		case BUFF_TYPE_CURSE:
		{
			for (unsigned int pos = m_curseBuffList.Begin(); pos != m_curseBuffList.End(); pos = m_curseBuffList.Next(pos))
			{
				CBuff* pBuff = m_curseBuffList.GetAt(pos);
				if (pBuff && pBuff->GetBuffIndex() == byBuffIndex)
				{
					pBuff->GetBuffInfo()->dwTimeRemaining = 0;
					return;
				}
			}
		}
		break;
		case BUFF_TYPE_SUB:
		{
			for (unsigned int pos = m_subBuffList.Begin(); pos != m_subBuffList.End(); pos = m_subBuffList.Next(pos))
			{
				CBuff* pBuff = m_subBuffList.GetAt(pos);
				if (pBuff && pBuff->GetBuffIndex() == byBuffIndex)
				{
					pBuff->GetBuffInfo()->dwTimeRemaining = 0;
					return;
				}
			}
		}
		break;

		default: ERR_LOG(LOG_GENERAL, "buffType % not allowed here \n", buffType);
	}
}

void CBuffManager::EndBuff(TBLIDX sourceTblidx, CBuff::BUFF_REMOVAL_REASON nReason)
{
	for (int i = 0; i < DBO_MAX_BUFF_CHARACTER_HAS; i++)
	{
		CBuff* pBuff = m_apBuff[i];
		if (pBuff)
		{
			if (pBuff->GetSourceTblidx() == sourceTblidx)
			{
				RemoveBuff(pBuff->GetBuffIndex(), pBuff->GetBuffType(), nReason);
				break;
			}
		}
	}
}

void CBuffManager::EndBuffTS(TBLIDX sourceTblidx, CBuff::BUFF_REMOVAL_REASON nReason)
{
	for (int i = 0; i < DBO_MAX_BUFF_CHARACTER_HAS; i++)
	{
		CBuff* pBuff = m_apBuff[i];
		if (pBuff)
		{
			if (pBuff->GetSourceTblidx() == sourceTblidx)
			{
				RemoveBuffTS(pBuff->GetBuffIndex(), pBuff->GetBuffType(), nReason);
				break;
			}
		}
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	: sets the buff time to 0
//		Return	: 
//-----------------------------------------------------------------------------------
void CBuffManager::EndBuff(eSYSTEM_EFFECT_CODE effectcode)
{
	for (int i = 0; i < DBO_MAX_BUFF_CHARACTER_HAS; i++)
	{
		CBuff* pBuff = m_apBuff[i];
		if (pBuff)
		{
			if (pBuff->GetSystemEffectCode(0) == effectcode || pBuff->GetSystemEffectCode(1) == effectcode)
			{
				RemoveBuff(pBuff->GetBuffIndex(), pBuff->GetBuffType(), CBuff::BUFF_REMOVAL_REASON_BY_ITSELF);
				break;
			}
		}
	}
}

void CBuffManager::EndBuffs(eSYSTEM_EFFECT_CODE effectcode)
{
	for (int i = 0; i < DBO_MAX_BUFF_CHARACTER_HAS; i++)
	{
		CBuff* pBuff = m_apBuff[i];
		if (pBuff)
		{
			if (pBuff->GetSystemEffectCode(0) == effectcode || pBuff->GetSystemEffectCode(1) == effectcode)
			{
				RemoveBuff(pBuff->GetBuffIndex(), pBuff->GetBuffType(), CBuff::BUFF_REMOVAL_REASON_BY_ITSELF);
			}
		}
	}
}

void CBuffManager::EndBuff(BYTE byBuffIndex, CBuff::BUFF_REMOVAL_REASON nReason)
{
	for (int i = 0; i < DBO_MAX_BUFF_CHARACTER_HAS; i++)
	{
		CBuff* pBuff = m_apBuff[i];
		if (pBuff)
		{
			if (pBuff->GetBuffIndex() == byBuffIndex)
			{
				RemoveBuff(pBuff->GetBuffIndex(), pBuff->GetBuffType(), nReason);
				break;
			}
		}
	}
}

void CBuffManager::EndBuffTS(BYTE byBuffIndex, eBUFF_TYPE buffType)
{
	for (int i = 0; i < DBO_MAX_BUFF_CHARACTER_HAS; i++)
	{
		CBuff* pBuff = m_apBuff[i];
		if (pBuff)
		{
			if (pBuff->GetBuffIndex() == byBuffIndex && pBuff->GetBuffType() == buffType)
			{
				pBuff->GetBuffInfo()->dwTimeRemaining = 0;
			}
		}
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CBuffManager::EndBlessBuff(eSYSTEM_EFFECT_CODE effectcode)
{
	for (unsigned int pos = m_blessBuffList.Begin(); pos != m_blessBuffList.End(); pos = m_blessBuffList.Next(pos))
	{
		CBuff* pBuff = m_blessBuffList.GetAt(pos);
		if (pBuff)
		{
			if (pBuff->GetSystemEffectCode(0) == effectcode || pBuff->GetSystemEffectCode(1) == effectcode)
			{
				RemoveBuff(pBuff->GetBuffIndex(), pBuff->GetBuffType(), CBuff::BUFF_REMOVAL_REASON_BY_ITSELF);
				break;
			}
		}
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	: thread safe
//		Return	:
//-----------------------------------------------------------------------------------
void CBuffManager::EndBlessBuffTS(eSYSTEM_EFFECT_CODE effectcode)
{
	for (unsigned int pos = m_blessBuffList.Begin(); pos != m_blessBuffList.End(); pos = m_blessBuffList.Next(pos))
	{
		CBuff* pBuff = m_blessBuffList.GetAt(pos);
		if (pBuff)
		{
			if (pBuff->GetSystemEffectCode(0) == effectcode || pBuff->GetSystemEffectCode(1) == effectcode)
			{
				pBuff->GetBuffInfo()->dwTimeRemaining = 1;
				break;
			}
		}
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	: end sub buff <thread safe>
//		Return	:
//-----------------------------------------------------------------------------------
void CBuffManager::EndSubBuff(eSYSTEM_EFFECT_CODE effectcode, eSYSTEM_EFFECT_CODE effectcode2)
{
	for (unsigned int pos = m_subBuffList.Begin(); pos != m_subBuffList.End(); pos = m_subBuffList.Next(pos))
	{
		CBuff* pBuff = m_subBuffList.GetAt(pos);
		if (pBuff)
		{
			if (pBuff->GetSystemEffectCode(0) == effectcode && pBuff->GetSystemEffectCode(1) == effectcode2)
			{
				pBuff->GetBuffInfo()->dwTimeRemaining = 1;
				pBuff->GetBuffInfo()->aBuffParameter[0].buffParameter.dwRemainValue = 0;
				break;
			}
		}
	}
}

void CBuffManager::RemoveBlessBuff(TBLIDX skillTblidx)
{
	for (unsigned int pos = m_blessBuffList.Begin(); pos != m_blessBuffList.End(); pos = m_blessBuffList.Next(pos))
	{
		CBuff* pBuff = m_blessBuffList.GetAt(pos);
		if (pBuff)
		{
			if (pBuff->GetSourceTblidx() == skillTblidx)
			{
				RemoveBuff(pBuff->GetBuffIndex(), pBuff->GetBuffType(), CBuff::BUFF_REMOVAL_REASON_BY_ITSELF);
				break;
			}
		}
	}
}

void CBuffManager::RemoveRandomBlessBuff()
{
	for (unsigned int pos = m_blessBuffList.Begin(); pos != m_blessBuffList.End(); pos = m_blessBuffList.Next(pos))
	{
		CBuff* pBuff = m_blessBuffList.GetAt(pos);
		if (pBuff && pBuff->IsActive())
		{
			RemoveBuff(pBuff->GetBuffIndex(), pBuff->GetBuffType(), CBuff::BUFF_REMOVAL_REASON_BY_ITSELF);
			break;
		}
	}
}


void CBuffManager::EndCurseBuff(eSYSTEM_EFFECT_CODE effectcode)
{
	for (unsigned int pos = m_curseBuffList.Begin(); pos != m_curseBuffList.End(); pos = m_curseBuffList.Next(pos))
	{
		CBuff* pBuff = m_curseBuffList.GetAt(pos);
		if (pBuff)
		{
			if (pBuff->GetSystemEffectCode(0) == effectcode || pBuff->GetSystemEffectCode(1) == effectcode)
			{
				RemoveBuff(pBuff->GetBuffIndex(), pBuff->GetBuffType(), CBuff::BUFF_REMOVAL_REASON_BY_ITSELF);
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------------
//		Purpose	: thread safe
//		Return	:
//-----------------------------------------------------------------------------------
void CBuffManager::EndCurseBuffTS(eSYSTEM_EFFECT_CODE effectcode)
{
	for (unsigned int pos = m_curseBuffList.Begin(); pos != m_curseBuffList.End(); pos = m_curseBuffList.Next(pos))
	{
		CBuff* pBuff = m_curseBuffList.GetAt(pos);
		if (pBuff)
		{
			if (pBuff->GetSystemEffectCode(0) == effectcode || pBuff->GetSystemEffectCode(1) == effectcode)
			{
				pBuff->GetBuffInfo()->dwTimeRemaining = 1;
				break;
			}
		}
	}
}

bool CBuffManager::CheckAndApplyOtherStun(BYTE byEndState, WORD wEffectCodeEnd)
{
	for (CBuffLinkedObject* pBuffLink = (CBuffLinkedObject*)m_listBuffOrderedByRegisteredTime.GetFirst();
		pBuffLink;
		pBuffLink = (CBuffLinkedObject*)pBuffLink->GetNext())
	{
		if (CBuff* pBuff = pBuffLink->GetBuffRef())
		{
			if (pBuffLink->IsBuffRemoved() == false)
			{
				if (pBuff->GetBuffInfo()->dwTimeRemaining > 0)
				{
					if (byEndState == CHARSTATE_STUNNED)
					{
						if(Dbo_IsSystemEffectForStun(wEffectCodeEnd) && (Dbo_IsSystemEffectForStun(pBuff->GetSystemEffectCode(0)) || Dbo_IsSystemEffectForStun(pBuff->GetSystemEffectCode(1))))
							continue;
					}
					else if (byEndState == CHARSTATE_PARALYZED)
					{
						if(Dbo_IsSystemEffectForStun(wEffectCodeEnd) && (Dbo_IsSystemEffectForStun(pBuff->GetSystemEffectCode(0)) || Dbo_IsSystemEffectForStun(pBuff->GetSystemEffectCode(1))))
							continue;
					}

					if (Dbo_IsSystemEffectForBodyCurse(pBuff->GetSystemEffectCode(0)) || Dbo_IsSystemEffectForBodyCurse(pBuff->GetSystemEffectCode(1)))
					{
						pBuff->OnEffectActive();

						return true;
					}
				}
			}
		}
	}

	return false;
}

void CBuffManager::OnLoginApplyBuffs()
{
	for (CBuffLinkedObject* pBuffLink = (CBuffLinkedObject*)m_listBuffOrderedByRegisteredTime.GetFirst();
		pBuffLink;
		pBuffLink = (CBuffLinkedObject*)pBuffLink->GetNext())
	{
		if (CBuff * pBuff = pBuffLink->GetBuffRef())
		{
			if (pBuffLink->IsBuffRemoved() == false)
			{
				if (pBuff->GetBuffInfo()->dwTimeRemaining > 0)
				{
					pBuff->OnEffectActive(false);
				}
			}
		}
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CBuffManager::LoadBuff(CBuff* pBuff)
{
	if (pBuff->IsSubBuff())
		return AddBuff_Internal(pBuff, m_subBuffList, NTL_MAX_SUB_BUFF_CHARACTER_HAS, true, false);
	else if (pBuff->IsBlessBuff())
		return AddBuff_Internal(pBuff, m_blessBuffList, NTL_MAX_BLESS_BUFF_CHARACTER_HAS, true, false);
	else if (pBuff->IsCurseBuff())
		return AddBuff_Internal(pBuff, m_curseBuffList, NTL_MAX_CURSE_BUFF_CHARACTER_HAS, true, false);

	return false;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CBuffManager::AddBuff(CBuff* pBuff, bool bNeedToDisplayMessage)
{
	if (pBuff->IsSubBuff())
		return AddBuff_Internal(pBuff, m_subBuffList, NTL_MAX_SUB_BUFF_CHARACTER_HAS, false, bNeedToDisplayMessage);
	else if (pBuff->IsBlessBuff())
		return AddBuff_Internal(pBuff, m_blessBuffList, NTL_MAX_BLESS_BUFF_CHARACTER_HAS, false, bNeedToDisplayMessage);
	else if (pBuff->IsCurseBuff())
		return AddBuff_Internal(pBuff, m_curseBuffList, NTL_MAX_CURSE_BUFF_CHARACTER_HAS, false, bNeedToDisplayMessage);

	return false;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CBuff* CBuffManager::FindBuff(BYTE byBuffIndex)
{
	if (byBuffIndex >= DBO_MAX_BUFF_CHARACTER_HAS)
		return NULL;

	return m_apBuff[byBuffIndex];
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CBuff* CBuffManager::FindBuff(BYTE byBuffGroup, BYTE bySourceType)
{
	for (int i = 0; i < DBO_MAX_BUFF_CHARACTER_HAS; i++)
	{
		CBuff* pBuff = m_apBuff[i];
		if (pBuff)
		{
			if (pBuff->GetBuffGroup() == byBuffGroup && pBuff->GetSourceType() == bySourceType)
				return pBuff;
		}
	}

	return NULL;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CBuff* CBuffManager::FindBuff(TBLIDX sourceTblidx, BYTE bySourceType)
{
	for (int i = 0; i < DBO_MAX_BUFF_CHARACTER_HAS; i++)
	{
		CBuff* pBuff = m_apBuff[i];
		if (pBuff)
		{
			if (pBuff->GetSourceTblidx() == sourceTblidx && pBuff->GetSourceType() == bySourceType)
				return pBuff;
		}
	}

	return NULL;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CBuff* CBuffManager::FindBuff(eSYSTEM_EFFECT_CODE effectcode, eSYSTEM_EFFECT_CODE effectcode2/* = INVALID_SYSTEM_EFFECT_CODE*/)
{
	for (int i = 0; i < DBO_MAX_BUFF_CHARACTER_HAS; i++)
	{
		CBuff* pBuff = m_apBuff[i];
		if (pBuff)
		{
			if (pBuff->GetSystemEffectCode(0) == effectcode && pBuff->GetSystemEffectCode(1) == effectcode2)
				return pBuff;
		}
	}

	return NULL;
}

CBuff * CBuffManager::FindAnyBuff(eSYSTEM_EFFECT_CODE effectcode)
{
	for (int i = 0; i < DBO_MAX_BUFF_CHARACTER_HAS; i++)
	{
		CBuff* pBuff = m_apBuff[i];
		if (pBuff)
		{
			if (pBuff->GetSystemEffectCode(0) == effectcode || pBuff->GetSystemEffectCode(1) == effectcode)
				return pBuff;
		}
	}

	return NULL;
}

CBuff * CBuffManager::FindSubBuff(BYTE byBuffGroup, BYTE bySourceType)
{
	unsigned int pos = m_subBuffList.Begin();
	while (pos != m_subBuffList.End())
	{
		CBuff* pBuff = m_subBuffList.GetAt(pos);
		if (pBuff)
		{
			if (pBuff->GetBuffGroup() == byBuffGroup && pBuff->GetSourceType() == bySourceType)
				return pBuff;
		}

		pos = m_subBuffList.Next(pos);
	}

	return nullptr;
}

CBuff * CBuffManager::FindCurse(eSYSTEM_EFFECT_CODE effectcode, DWORD dwLeastDuration)
{
	unsigned int pos = m_curseBuffList.Begin();
	while (pos != m_curseBuffList.End())
	{
		CBuff* pBuff = m_curseBuffList.GetAt(pos);
		if (pBuff && pBuff->GetBuffInfo()->dwTimeRemaining >= dwLeastDuration)
		{
			if (pBuff->GetSystemEffectCode(0) == effectcode || pBuff->GetSystemEffectCode(1) == effectcode)
				return pBuff;
		}

		pos = m_curseBuffList.Next(pos);
	}

	return NULL;
}

bool CBuffManager::HasAirSkillBuff()
{
	for (int i = 0; i < DBO_MAX_BUFF_CHARACTER_HAS; i++)
	{
		CBuff* pBuff = m_apBuff[i];
		if (pBuff)
		{
			if (pBuff->GetSystemEffectCode(0) == ACTIVE_AIR_MOVE || pBuff->GetSystemEffectCode(0) == ACTIVE_AIR_MOVE_DASH_ACCEL)
				return true;
		}
	}

	return false;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CBuffManager::RemoveAllBuff()
{
	unsigned int pos = m_curseBuffList.Begin();
	while (pos != m_curseBuffList.End())
	{
		CBuff* pBuff = m_curseBuffList.GetAt(pos);
		if (pBuff)
			pos = RemoveBuff_Internal(CBuff::BUFF_REMOVAL_REASON_ETC, &m_curseBuffList, pos);
		else
			pos = m_curseBuffList.Next(pos);
	}

	pos = m_blessBuffList.Begin();
	while (pos != m_blessBuffList.End())
	{
		CBuff* pBuff = m_blessBuffList.GetAt(pos);
		if (pBuff)
			pos = RemoveBuff_Internal(CBuff::BUFF_REMOVAL_REASON_ETC, &m_blessBuffList, pos);
		else
			pos = m_blessBuffList.Next(pos);
	}

	pos = m_subBuffList.Begin();
	while (pos != m_subBuffList.End())
	{
		CBuff* pBuff = m_subBuffList.GetAt(pos);
		if (pBuff && pBuff->RemoveSubBuffOnDeath())
		{
			pos = RemoveBuff_Internal(CBuff::BUFF_REMOVAL_REASON_ETC, &m_subBuffList, pos);
		}
		else
			pos = m_subBuffList.Next(pos);
	}
}

void CBuffManager::RemoveAllBuffExceptApplySelf()
{
	unsigned int pos = m_curseBuffList.Begin();
	while (pos != m_curseBuffList.End())
	{
		CBuff* pBuff = m_curseBuffList.GetAt(pos);
		if (pBuff)
		{
			pos = RemoveBuff_Internal(CBuff::BUFF_REMOVAL_REASON_ETC, &m_curseBuffList, pos);
		}
		else
			pos = m_curseBuffList.Next(pos);
	}

	pos = m_blessBuffList.Begin();
	while (pos != m_blessBuffList.End())
	{
		CBuff* pBuff = m_blessBuffList.GetAt(pos);
		if (pBuff)
		{
			sSKILL_TBLDAT* pSkillTbldat = (sSKILL_TBLDAT*)g_pTableContainer->GetSkillTable()->FindData(pBuff->GetBuffInfo()->sourceTblidx);

			if(pSkillTbldat->byApply_Target == DBO_SKILL_APPLY_TARGET_SELF ||  pBuff->GetCasterHandle() == m_pOwnerRef->GetID()) //dont need to remove buffs which we register ourselves
				pos = m_blessBuffList.Next(pos);
			else
				pos = RemoveBuff_Internal(CBuff::BUFF_REMOVAL_REASON_ETC, &m_blessBuffList, pos);
		}
		else
			pos = m_blessBuffList.Next(pos);
	}

	pos = m_subBuffList.Begin();
	while (pos != m_subBuffList.End())
	{
		CBuff* pBuff = m_subBuffList.GetAt(pos);
		if (pBuff && pBuff->RemoveSubBuffOnDeath())
		{
			pos = RemoveBuff_Internal(CBuff::BUFF_REMOVAL_REASON_ETC, &m_subBuffList, pos);
		}
		else
			pos = m_subBuffList.Next(pos);
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CBuffManager::RemoveAllCurseBuff()
{
	unsigned int pos = m_curseBuffList.Begin();
	while (pos != m_curseBuffList.End())
	{
		CBuff* pBuff = m_curseBuffList.GetAt(pos);
		if (pBuff)
			pos = RemoveBuff_Internal(CBuff::BUFF_REMOVAL_REASON_ETC, &m_curseBuffList, pos);
		else
			pos = m_curseBuffList.Next(pos);
	}
}

void CBuffManager::RemoveAllStun()
{
	unsigned int pos = m_curseBuffList.Begin();
	while (pos != m_curseBuffList.End())
	{
		CBuff* pBuff = m_curseBuffList.GetAt(pos);
		if (pBuff)
		{
			if (Dbo_IsSystemEffectForBodyCurse(pBuff->GetSystemEffectCode(0)) || Dbo_IsSystemEffectForBodyCurse(pBuff->GetSystemEffectCode(1)))
			{
				pos = RemoveBuff_Internal(CBuff::BUFF_REMOVAL_REASON_ETC, &m_curseBuffList, pos);
				continue;
			}
		}
		
		pos = m_curseBuffList.Next(pos);
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CBuffManager::RemoveOldestCurseBuff()
{
	for (CBuffLinkedObject* pBuffLink = (CBuffLinkedObject*)m_listBuffOrderedByRegisteredTime.GetFirst();
		pBuffLink;
		pBuffLink = (CBuffLinkedObject*)pBuffLink->GetNext())
	{
		CBuff* pBuff = pBuffLink->GetBuffRef();
		if (pBuff->IsCurseBuff())
		{
			for (unsigned int pos = m_curseBuffList.Begin();
				m_curseBuffList.End() != pos;
				pos = m_curseBuffList.Next(pos))
			{
				CBuff* pOldest = m_curseBuffList.GetAt(pos);
				if (pOldest)
				{
					if (pBuff->GetBuffIndex() == pOldest->GetBuffIndex())
					{
						RemoveBuff_Internal(CBuff::BUFF_REMOVAL_REASON_BY_ITSELF, &m_curseBuffList, pos);
						return true;
					}
				}
			}
		}
	}

	return false;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CBuffManager::RemoveRandomCurseBuff(bool bDontRemoveBodyCurse/* = false*/)
{
	if (m_curseBuffList.GetSize() == 0)
		return false;

	std::vector<unsigned int> vecTemp;
	
	for (unsigned int pos = m_curseBuffList.Begin();
		m_curseBuffList.End() != pos;
		pos = m_curseBuffList.Next(pos))
	{
		CBuff* pBuff = m_curseBuffList.GetAt(pos);
		if (pBuff)
		{
			if (bDontRemoveBodyCurse == false ||
				(Dbo_IsSystemEffectForBodyCurse(pBuff->GetSystemEffectCode(0)) == false && Dbo_IsSystemEffectForBodyCurse(pBuff->GetSystemEffectCode(1)) == false 
					&& pBuff->GetBuffInfo()->aBuffParameter[0].byBuffParameterType == DBO_BUFF_PARAMETER_TYPE_DEFAULT && pBuff->GetBuffInfo()->aBuffParameter[1].byBuffParameterType == DBO_BUFF_PARAMETER_TYPE_DEFAULT))
			{
				vecTemp.push_back(pos);
			}
		}
	}

	if (vecTemp.size() == 0)
		return false;

	unsigned int rand = vecTemp[RandomRangeU(0, (DWORD)vecTemp.size() - 1)];

	RemoveBuff_Internal(CBuff::BUFF_REMOVAL_REASON_BY_ITSELF, &m_curseBuffList, rand);

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CBuffManager::CopyBuffAttributesTo(CCharacterAtt* pCharAtt)
{
	sAVATAR_ATTRIBUTE& rAvatarAttribute = pCharAtt->GetAvatarAttribute();

	for (CBuffLinkedObject* pBuffLink = (CBuffLinkedObject*)m_listBuffOrderedByRegisteredTime.GetFirst();
		pBuffLink;
		pBuffLink = (CBuffLinkedObject*)pBuffLink->GetNext())
	{
		CBuff* pBuff = pBuffLink->GetBuffRef();
		if (pBuff)
		{
			sBUFF_INFO* buffinfo = pBuff->GetBuffInfo();

			if (buffinfo->dwTimeRemaining > 0 && buffinfo->bActive)
			{
				BYTE byEffectType[NTL_MAX_EFFECT_IN_ITEM];
				byEffectType[0] = INVALID_BYTE;
				byEffectType[1] = INVALID_BYTE;

				if (buffinfo->bySourceType == DBO_OBJECT_SOURCE_SKILL)
				{
					sSKILL_TBLDAT* skillinfo = (sSKILL_TBLDAT *)g_pTableContainer->GetSkillTable()->FindData(buffinfo->sourceTblidx);
					if (skillinfo)
					{
						byEffectType[0] = skillinfo->bySkill_Effect_Type[0];
						byEffectType[1] = skillinfo->bySkill_Effect_Type[1];
					}
					else
						ERR_LOG(LOG_SYSTEM, "Could not find skill from buff. buffinfo->sourceTblidx %u", buffinfo->sourceTblidx);
				}
				else
				{
					sITEM_TBLDAT* item = (sITEM_TBLDAT*)g_pTableContainer->GetItemTable()->FindData(buffinfo->sourceTblidx);
					if (item)
					{
						sUSE_ITEM_TBLDAT* useitem = (sUSE_ITEM_TBLDAT*)g_pTableContainer->GetUseItemTable()->FindData(item->Use_Item_Tblidx);
						if (useitem)
						{
							byEffectType[0] = useitem->abySystem_Effect_Type[0];
							byEffectType[1] = useitem->abySystem_Effect_Type[1];
						}
						else
							ERR_LOG(LOG_SYSTEM, "Could not find useitem. item->Use_Item_Tblidx %u", item->Use_Item_Tblidx);
					}
					else
						ERR_LOG(LOG_SYSTEM, "Could not find item. buffinfo->sourcetblidx %u", buffinfo->sourceTblidx);
				}

				
				for (int j = 0; j < NTL_MAX_EFFECT_IN_SKILL; j++)
				{
					if (byEffectType[j] != INVALID_BYTE)
					{
						eSYSTEM_EFFECT_CODE effectcode = pBuff->GetSystemEffectCode(j);
						if (effectcode != INVALID_SYSTEM_EFFECT_CODE)
						{
							if (effectcode == ACTIVE_RABIES)
							{
								sCOMMONCONFIG_VALUE_DATA* commonConfig = g_pTableContainer->GetCommonConfigTable()->FindCommonConfig(buffinfo->aBuffParameter[j].buffParameter.commonConfigTblidx);
								if (commonConfig)
								{
									pCharAtt->CalculateAttackSpeedRate((float)commonConfig->adwValue[0], SYSTEM_EFFECT_APPLY_TYPE_PERCENT, false);
									rAvatarAttribute.wPhysicalOffence += WORD((float)rAvatarAttribute.wPhysicalOffence * ((float)commonConfig->adwValue[1]) / 100.f);
									rAvatarAttribute.fRunSpeed += rAvatarAttribute.fRunSpeed * ((float)commonConfig->adwValue[2]) / 100.f;
									rAvatarAttribute.fMindCurseImmunity += 100.f;
									break;
								}
							}
							else if (effectcode == ACTIVE_DRUNK)
							{
								sCOMMONCONFIG_VALUE_DATA* commonConfig = g_pTableContainer->GetCommonConfigTable()->FindCommonConfig(buffinfo->aBuffParameter[j].buffParameter.commonConfigTblidx);
								if (commonConfig)
								{
									pCharAtt->CalculateAttackSpeedRate((float)commonConfig->adwValue[0], SYSTEM_EFFECT_APPLY_TYPE_PERCENT, true);
									rAvatarAttribute.fSkillAnimationSpeedModifier -= rAvatarAttribute.fSkillAnimationSpeedModifier * ((float)commonConfig->adwValue[1]) / 100.f;
									rAvatarAttribute.wDodgeRate += WORD((float)rAvatarAttribute.wDodgeRate * ((float)commonConfig->adwValue[2]) / 100.f);

									break;
								}
							}
							else if (effectcode == ACTIVE_EXCITATION_MALE)
							{
								sCOMMONCONFIG_VALUE_DATA* commonConfig = g_pTableContainer->GetCommonConfigTable()->FindCommonConfig(buffinfo->aBuffParameter[j].buffParameter.commonConfigTblidx);
								if (commonConfig)
								{
									pCharAtt->CalculateAttackSpeedRate((float)commonConfig->adwValue[0], SYSTEM_EFFECT_APPLY_TYPE_PERCENT, false);
									rAvatarAttribute.fRunSpeed += rAvatarAttribute.fRunSpeed * ((float)commonConfig->adwValue[1]) / 100;
									break;
								}
							}
							else if (effectcode == ACTIVE_EXCITATION_FEMALE)
							{
								sCOMMONCONFIG_VALUE_DATA* commonConfig = g_pTableContainer->GetCommonConfigTable()->FindCommonConfig(buffinfo->aBuffParameter[j].buffParameter.commonConfigTblidx);
								if (commonConfig)
								{
									rAvatarAttribute.wPhysicalDefence += WORD((float)rAvatarAttribute.wPhysicalDefence * ((float)commonConfig->adwValue[0]) / 100.f);
									rAvatarAttribute.wEnergyDefence += WORD((float)rAvatarAttribute.wEnergyDefence * ((float)commonConfig->adwValue[0]) / 100.f);
									break;
								}
							}
							else
							{
								Dbo_SetAvatarAttributeValue(pCharAtt, effectcode, buffinfo->aBuffParameter[j].buffParameter.fParameter, byEffectType[j]);
							}
						}
					}
				}
			}
		}
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CBuffManager::SendBuffInfo(CCharacter* pPlayer)
{
	CNtlPacket packet(sizeof(sGU_BUFF_REFRESH_ALL));
	sGU_BUFF_REFRESH_ALL * res = (sGU_BUFF_REFRESH_ALL *)packet.GetPacketData();
	res->wOpCode = GU_BUFF_REFRESH_ALL;
	res->hHandle = m_pOwnerRef->GetID();

	for (CBuffLinkedObject* pBuffLink = (CBuffLinkedObject*)m_listBuffOrderedByRegisteredTime.GetFirst();
		pBuffLink;
		pBuffLink = (CBuffLinkedObject*)pBuffLink->GetNext())
	{
		CBuff* pBuff = pBuffLink->GetBuffRef();
		if (pBuff)
		{
			sBUFF_INFO* pBuffInfo = pBuff->GetBuffInfo();

			memcpy(&res->aBuffInfo[res->byCount], pBuffInfo, sizeof(sBUFF_INFO));

			res->byCount++;
		}
	}

	if (res->byCount > 0)
	{
		pPlayer->SendPacket(&packet);
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CBuffManager::AddBuff_Internal(CBuff* pBuff, CNtlLinkArray_Static<CBuff*> & rlistBuff, int nMaxBuffCount, bool bIsLoadedBuff, bool bNeedToDisplayMessage)
{
	if (nMaxBuffCount <= (int)rlistBuff.GetSize())
		return false;

	BYTE byBuffIndex = pBuff->GetBuffIndex();
	BYTE finalBuffIndex = INVALID_BYTE;

	if(byBuffIndex == INVALID_BYTE)
	{
		for (int j = 0; j < DBO_MAX_BUFF_CHARACTER_HAS; j++)
		{
			if (!m_apBuff[j])
			{
				finalBuffIndex = j;
				break;
			}
		}
	}
	else
	{
		if (byBuffIndex >= DBO_MAX_BUFF_CHARACTER_HAS)
			return false;

		if (m_apBuff[byBuffIndex])
			return false;

		finalBuffIndex = byBuffIndex;
	}

	if (finalBuffIndex == INVALID_BYTE)
		return false;
	else
	{
		m_apBuff[finalBuffIndex] = pBuff;

		if (rlistBuff.End() == rlistBuff.Insert(pBuff))
		{
			m_apBuff[finalBuffIndex] = NULL;
			return false;
		}

		if (byBuffIndex == INVALID_BYTE)
			pBuff->SetBuffIndex(finalBuffIndex);
		
		CBuffLinkedObject* pBuffLinker = new CBuffLinkedObject(pBuff);
		if (pBuffLinker)
		{
			m_listBuffOrderedByRegisteredTime.Append(pBuffLinker);
		}
		else
		{
			ERR_LOG(LOG_SYSTEM, "new CBuffLinkedObject(pBuff) failed.");
		}

		return pBuff->OnRegistered(bIsLoadedBuff, bNeedToDisplayMessage);
	}

	return false;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
unsigned int CBuffManager::RemoveBuff_Internal(CBuff::BUFF_REMOVAL_REASON nReason, CNtlLinkArray_Static<CBuff *> *plistBuff, unsigned int iterBuff)
{
	CBuff* pBuff = plistBuff->GetAt(iterBuff);
	if (pBuff)
	{
		BYTE byBuffIndex = pBuff->GetBuffIndex();

		for (CBuffLinkedObject* pBuffLink = (CBuffLinkedObject*)m_listBuffOrderedByRegisteredTime.GetFirst();
			pBuffLink;
			pBuffLink = (CBuffLinkedObject*)pBuffLink->GetNext())
		{
			if (pBuff == pBuffLink->GetBuffRef())
			{
				pBuffLink->SetBuffRemoved();
				m_listBuffOrderedByRegisteredTime.Remove(pBuffLink);
				SAFE_DELETE(pBuffLink);
				break;
			}
		}

		int nPos = plistBuff->Remove(iterBuff);

		if (byBuffIndex < DBO_MAX_BUFF_CHARACTER_HAS)
			m_apBuff[byBuffIndex] = NULL;
		else
			ERR_LOG(LOG_SYSTEM, "buffIndex has a wrong value., buffIndex = %u", byBuffIndex);

		pBuff->OnRemoved(nReason);

		SAFE_DELETE(pBuff);

		return nPos;
	}

	return plistBuff->End();
}
