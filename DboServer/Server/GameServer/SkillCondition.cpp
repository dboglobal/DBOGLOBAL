#include "stdafx.h"
#include "SkillCondition.h"
#include "Monster.h"
#include "SpellAreaChecker.h"
#include "ObjectManager.h"



CSkillCondition::CSkillCondition()
{
	m_pBot = NULL;
	m_pSkill = NULL;
	m_Skill_Tblidx = INVALID_TBLIDX;
	m_byUse_Skill_Basis = INVALID_BYTE;
	m_wUse_Skill_LP = INVALID_WORD;
	m_wUse_Skill_Time = INVALID_WORD;
	m_dwTime = 0;
	m_bySkillConditionIdx = INVALID_BYTE;

	m_bCanUse = true;
}

CSkillCondition::~CSkillCondition()
{
	Destroy();
}


void CSkillCondition::Destroy()
{
	this->m_pBot = NULL;
	this->m_pSkill = NULL;
	this->m_Skill_Tblidx = INVALID_TBLIDX;
	this->m_byUse_Skill_Basis = INVALID_BYTE;
	this->m_wUse_Skill_LP = INVALID_WORD;
	this->m_wUse_Skill_Time = INVALID_WORD;
	this->m_dwTime = 0;
	m_bCanUse = false;
}


CSkillBot* CSkillCondition::OnUpdate(DWORD dwTickTime)
{
	m_dwTime = UnsignedSafeIncrease<DWORD>(m_dwTime, dwTickTime + 1000);

	if (m_dwTime >= m_wUse_Skill_Time)
	{
		m_dwTime = 0;
		if (GetBot()->GetCurEP() >= m_pSkill->GetOriginalTableData()->wRequire_EP)
		{
			if (m_pSkill->GetCoolTimeRemaining() == 0)
				return m_pSkill;
		}
	}

	return NULL;
}


bool CSkillCondition::GetTarget(HOBJECT & hTarget, sSKILL_TARGET_LIST & rTargetList)
{
	switch (m_pSkill->GetOriginalTableData()->byAppoint_Target)
	{
		case DBO_SKILL_APPOINT_TARGET_SELF:
		{
			hTarget = GetBot()->GetID();
			SkillAppointTargetSelf(rTargetList);
		}
		break;
		case DBO_SKILL_APPOINT_TARGET_TARGET:
		{
			SkillAppointTargetTarget(hTarget, rTargetList);
		}
		break;
		default: 
		{
			ERR_LOG(LOG_USER, "fail : switch( m_pSkill->GetOriginalTableData()->byAppoint_Target(%u) )", m_pSkill->GetOriginalTableData()->byAppoint_Target);
			return false;
		}
		break;
	}

	return true;
}


void CSkillCondition::SkillAppointTargetSelf(sSKILL_TARGET_LIST & rTargetList)
{
	switch (m_pSkill->GetOriginalTableData()->byApply_Target)
	{
		case DBO_SKILL_APPLY_TARGET_SELF:
		{
			AppointTargetSelf_ApplyTargetSelf(rTargetList);
		}
		break;
		case DBO_SKILL_APPLY_TARGET_ENEMY:
		{
			AppointTargetSelf_ApplyTargetEnemy(rTargetList);
		}
		break;

		case DBO_SKILL_APPLY_TARGET_ALLIANCE:
		case DBO_SKILL_APPLY_TARGET_PARTY:
		case DBO_SKILL_APPLY_TARGET_MOB_PARTY:
		{
			AppointTargetSelf_ApplyTargetParty(rTargetList);
		}
		break;

		case DBO_SKILL_APPLY_TARGET_ANY: break;

		case DBO_SKILL_APPLY_TARGET_SUMMON:
		{
			ERR_LOG(LOG_SYSTEM, "DBO_SKILL_APPLY_TARGET_SUMMON value is used by in NPC Server.");
		}
		break;

		case DBO_SKILL_APPLY_TARGET_ANY_NPC:
		{
			ERR_LOG(LOG_SYSTEM, "DBO_SKILL_APPLY_TARGET_ANY_NPC value is used by in NPC Server.");
		}
		break;

		case DBO_SKILL_APPLY_TARGET_ANY_MOB:
		{
			ERR_LOG(LOG_SYSTEM, "DBO_SKILL_APPLY_TARGET_ANY_MOB value is used by in NPC Server.");
		}
		break;

		case DBO_SKILL_APPLY_TARGET_ANY_ALLIANCE:
		{
			ERR_LOG(LOG_SYSTEM, "DBO_SKILL_APPLY_TARGET_ANY_ALLIANCE value is used by in NPC Server.");
		}
		break;

		default:
		{
			ERR_LOG(LOG_SYSTEM, "fail : switch( m_pSkill->GetOriginalTableData()->byApply_Target(%u) )", m_pSkill->GetOriginalTableData()->byApply_Target);
		}
		break;
	}
}


void CSkillCondition::SkillAppointTargetTarget(HOBJECT & hTarget, sSKILL_TARGET_LIST & rTargetList)
{
	switch (m_pSkill->GetOriginalTableData()->byApply_Target)
	{
	case DBO_SKILL_APPLY_TARGET_SELF:
	{
		AppointTargetTarget_ApplyTargetSelf(hTarget, rTargetList);
	}
	break;
	case DBO_SKILL_APPLY_TARGET_ENEMY:
	{
		AppointTargetTarget_ApplyTargetEnemy(hTarget, rTargetList);
	}
	break;

	case DBO_SKILL_APPLY_TARGET_ALLIANCE:
	case DBO_SKILL_APPLY_TARGET_PARTY:
	case DBO_SKILL_APPLY_TARGET_MOB_PARTY:
	{
		AppointTargetTarget_ApplyTargetParty(hTarget, rTargetList);
	}
	break;

	case DBO_SKILL_APPLY_TARGET_ANY: break;

	case DBO_SKILL_APPLY_TARGET_SUMMON:
	{
		ERR_LOG(LOG_SYSTEM, "DBO_SKILL_APPLY_TARGET_SUMMON value is used by in NPC Server.");
	}
	break;

	case DBO_SKILL_APPLY_TARGET_ANY_NPC:
	{
		ERR_LOG(LOG_SYSTEM, "DBO_SKILL_APPLY_TARGET_ANY_NPC value is used by in NPC Server.");
	}
	break;

	case DBO_SKILL_APPLY_TARGET_ANY_MOB:
	{
		ERR_LOG(LOG_SYSTEM, "DBO_SKILL_APPLY_TARGET_ANY_MOB value is used by in NPC Server.");
	}
	break;

	case DBO_SKILL_APPLY_TARGET_ANY_ALLIANCE:
	{
		ERR_LOG(LOG_SYSTEM, "DBO_SKILL_APPLY_TARGET_ANY_ALLIANCE value is used by in NPC Server.");
	}
	break;

	default:
	{
		ERR_LOG(LOG_SYSTEM, "fail : switch( m_pSkill->GetOriginalTableData()->byApply_Target(%u) )", m_pSkill->GetOriginalTableData()->byApply_Target);
	}
	break;
	}
}


void CSkillCondition::AppointTargetSelf_ApplyTargetSelf(sSKILL_TARGET_LIST & rTargetList)
{
	if (IsApplyNotMe())
	{
		ERR_LOG(LOG_BOTAI, "fail : true == bIsApplyNotMe()");
		rTargetList.Init();
	}
	else
	{
		rTargetList.AddTarget(GetBot()->GetID());
	}
}


void CSkillCondition::AppointTargetSelf_ApplyTargetEnemy(sSKILL_TARGET_LIST & rTargetList)
{
	rTargetList.AddTarget(GetBot()->GetTargetHandle());

	if (GetApplyRangeType())
	{
		if (GetBot()->GetObjType() == OBJTYPE_MOB)
			GetTarget_ApplyRange_PCandNPC(GetBot(), rTargetList);
		else
			GetTarget_ApplyRange_Bot(GetBot(), rTargetList);
	}
}


void CSkillCondition::AppointTargetSelf_ApplyTargetParty(sSKILL_TARGET_LIST & rTargetList)
{
	if (GetApplyRangeType())
	{
		GetTarget_ApplyRange_Party(GetBot(), rTargetList, GetTargetMaxCount());
	}
	else
	{
		GetTarget_ApplyRange_Party(GetBot(), rTargetList, 1);
	}
}


void CSkillCondition::AppointTargetTarget_ApplyTargetSelf(HOBJECT & hTarget, sSKILL_TARGET_LIST & rTargetList)
{
	hTarget = GetBot()->GetID();
	if (IsApplyNotMe())
	{
		ERR_LOG(LOG_BOTAI, "fail : true == bIsApplyNotMe()");
		rTargetList.Init();
	}
	else
	{
		rTargetList.AddTarget(hTarget);
	}
}


void CSkillCondition::AppointTargetTarget_ApplyTargetEnemy(HOBJECT & hTarget, sSKILL_TARGET_LIST & rTargetList)
{
	hTarget = GetBot()->GetTargetHandle();
	rTargetList.AddTarget(hTarget);

	if (GetApplyRangeType())
	{
		if (GetBot()->GetObjType() == OBJTYPE_MOB)
			GetTarget_ApplyRange_PCandNPC(GetBot(), rTargetList);
		else
			GetTarget_ApplyRange_Bot(GetBot(), rTargetList);
	}
}


void CSkillCondition::AppointTargetTarget_ApplyTargetParty(HOBJECT & hTarget, sSKILL_TARGET_LIST & rTargetList)
{
	hTarget = GetBot()->GetID();

	if (GetApplyRangeType())
	{
		GetTarget_ApplyRange_Party(GetBot(), rTargetList, GetTargetMaxCount());
	}
	else
	{
		GetTarget_ApplyRange_Party(GetBot(), rTargetList, 1);
	}
}


HOBJECT CSkillCondition::IsObjectInApplyRingRange()
{
	if (GetBot()->GetObjType() == OBJTYPE_NPC)
		return (GetBot())->ConsiderScanTargetRingRange(m_wUse_Skill_LP);
	else if (GetBot()->GetObjType() == OBJTYPE_MOB)
		return ((CMonster*)GetBot())->ConsiderScanTargetRingRange(m_wUse_Skill_LP);

	return INVALID_HOBJECT;
}


void CSkillCondition::GetTargetApplyRange_Cell(eOBJTYPE byObjType, CWorldCell *pCell, CSpellAreaChecker& cSpellAreaChecker, sSKILL_TARGET_LIST& rTargetList, BYTE byMaxTargetCount)
{
	for (CSpawnObject* pObject = pCell->GetObjectList()->GetFirst(byObjType);
		rTargetList.byTargetCount < (BYTE)byMaxTargetCount && pObject;
		pObject = pCell->GetObjectList()->GetNext(pObject->GetWorldCellObjectLinker()))
	{
		if (cSpellAreaChecker.IsObjectInApplyRange(pObject, NULL))
		{
			if (!rTargetList.IsExist(pObject->GetID()))
			{
				rTargetList.AddTarget(pObject->GetID());
			}
		}
	}
}


void CSkillCondition::GetTargetApplyRange_Cell(CWorldCell *pCell, CSpellAreaChecker& cSpellAreaChecker, sSKILL_TARGET_LIST& rTargetList, BYTE byMaxTargetCount)
{
	for (CNpc* pObject = (CNpc*)pCell->GetObjectList()->GetFirst(OBJTYPE_NPC);
		rTargetList.byTargetCount < (BYTE)byMaxTargetCount && pObject;
		pObject = (CNpc*)pCell->GetObjectList()->GetNext(pObject->GetWorldCellObjectLinker()))
	{
		if (cSpellAreaChecker.IsObjectInApplyRange(pObject, NULL))
		{
			if (!rTargetList.IsExist(pObject->GetID()))
			{
				if (pObject->HasFunction(NPC_FUNC_FLAG_SCAN_BY_MOB))
					rTargetList.AddTarget(pObject->GetID());
			}
		}
	}
}


void CSkillCondition::GetTarget_ApplyRange_PCandNPC(CCharacter *pAppointTarget, sSKILL_TARGET_LIST& rTargetList)
{
	CSpellAreaChecker rSpellAreaChecker;
	rSpellAreaChecker.Create();

	rSpellAreaChecker.PrepareForSelection(GetBot(), pAppointTarget, GetApplyRangeType(), GetApplyAreaSize1(), GetApplyAreaSize2());

	if (GetBot())
	{
		if (GetBot()->GetTargetListManager())
		{
			GetBot()->GetTargetListManager()->GetTargetApplyRange(rSpellAreaChecker, rTargetList, GetTargetMaxCount());

			CWorldCell* pWorldCell = GetBot()->GetCurWorldCell();
			if (pWorldCell)
			{
				CWorldCell::QUADPAGE page = pWorldCell->GetCellQuadPage(GetBot()->GetCurLoc());

				for (int dir = CWorldCell::QUADDIR_SELF; dir < CWorldCell::QUADDIR_COUNT; dir++)
				{
					CWorldCell* pCellSibling = pWorldCell->GetQuadSibling(page, (CWorldCell::QUADDIR)dir);
					if (pCellSibling)
					{
						GetTargetApplyRange_Cell(OBJTYPE_PC, pCellSibling, rSpellAreaChecker, rTargetList, GetTargetMaxCount());

						if (GetTargetMaxCount() <= rTargetList.byTargetCount)
							return;

						GetTargetApplyRange_Cell(pCellSibling, rSpellAreaChecker, rTargetList, GetTargetMaxCount());

						if (GetTargetMaxCount() <= rTargetList.byTargetCount)
							return;

						GetTargetApplyRange_Cell(OBJTYPE_SUMMON_PET, pCellSibling, rSpellAreaChecker, rTargetList, GetTargetMaxCount());

						if (GetTargetMaxCount() <= rTargetList.byTargetCount)
							return;
					}
				}
			}
		}
		else
			ERR_LOG(LOG_BOTAI, "fail : NULL == pTargetListManager");
	}
	else
		ERR_LOG(LOG_BOTAI, "fail : NULL == m_pChar");
}


void CSkillCondition::GetTarget_ApplyRange_Bot(CCharacter *pAppointTarget, sSKILL_TARGET_LIST& rTargetList)
{
	CSpellAreaChecker rSpellAreaChecker;
	rSpellAreaChecker.Create();

	rSpellAreaChecker.PrepareForSelection(GetBot(), pAppointTarget, GetApplyRangeType(), GetApplyAreaSize1(), GetApplyAreaSize2());

	if (GetBot())
	{
		if (GetBot()->GetTargetListManager())
		{
			GetBot()->GetTargetListManager()->GetTargetApplyRange(rSpellAreaChecker, rTargetList, GetTargetMaxCount());

			CWorldCell* pWorldCell = GetBot()->GetCurWorldCell();
			if (pWorldCell)
			{
				CWorldCell::QUADPAGE page = pWorldCell->GetCellQuadPage(GetBot()->GetCurLoc());

				for (int dir = CWorldCell::QUADDIR_SELF; dir < CWorldCell::QUADDIR_COUNT; dir++)
				{
					CWorldCell* pCellSibling = pWorldCell->GetQuadSibling(page, (CWorldCell::QUADDIR)dir);
					if (pCellSibling)
					{
						GetTargetApplyRange_Cell(OBJTYPE_MOB, pCellSibling, rSpellAreaChecker, rTargetList, GetTargetMaxCount());

						if (GetTargetMaxCount() <= rTargetList.byTargetCount)
							return;
					}
				}
			}
		}
		else
			ERR_LOG(LOG_BOTAI, "fail : NULL == pTargetListManager");
	}
	else
		ERR_LOG(LOG_BOTAI, "fail : NULL == m_pChar");
}


void CSkillCondition::GetTarget_ApplyRange_Party(CCharacter *pAppointTarget, sSKILL_TARGET_LIST& rTargetList, BYTE byMaxTargetCount)
{
	CSpellAreaChecker rSpellAreaChecker;
	rSpellAreaChecker.Create();

	rSpellAreaChecker.PrepareForSelection(GetBot(), pAppointTarget, GetApplyRangeType(), GetApplyAreaSize1(), GetApplyAreaSize2());

	std::map<HOBJECT, HOBJECT> mapCandidate;

	if (GetBot()->GetObjType() != OBJTYPE_MOB && GetBot()->GetObjType() != OBJTYPE_NPC)
	{
		if (GetBot()->GetObjType() == OBJTYPE_SUMMON_PET)
		{
			CWorldCell* pCell = GetBot()->GetCurWorldCell();
			if (pCell)
			{
				for (int dir = 0; dir < CWorldCell::MAX_CELLDIR; dir++)
				{
					CWorldCell* pSibling = pCell->GetSibling((CWorldCell::CELLDIR)dir);
					if (pSibling)
					{
						CSpawnObject* pSpawnObject = NULL;

						for (pSpawnObject = pSibling->GetObjectList()->GetFirst(OBJTYPE_PC);
							pSpawnObject;
							pSpawnObject = pSibling->GetObjectList()->GetNext(pSpawnObject->GetWorldCellObjectLinker()))
						{
							mapCandidate.insert(std::make_pair(pSpawnObject->GetID(), pSpawnObject->GetID()));
						}

						for (pSpawnObject = pSibling->GetObjectList()->GetFirst(OBJTYPE_NPC);
							pSpawnObject;
							pSpawnObject = pSibling->GetObjectList()->GetNext(pSpawnObject->GetWorldCellObjectLinker()))
						{
							mapCandidate.insert(std::make_pair(pSpawnObject->GetID(), pSpawnObject->GetID()));
						}

						for (pSpawnObject = pSibling->GetObjectList()->GetFirst(OBJTYPE_SUMMON_PET);
							pSpawnObject;
							pSpawnObject = pSibling->GetObjectList()->GetNext(pSpawnObject->GetWorldCellObjectLinker()))
						{
							mapCandidate.insert(std::make_pair(pSpawnObject->GetID(), pSpawnObject->GetID()));
						}
					}
				}
			}
		}
	}
	else
	{
		CNpcParty* pParty = GetBot()->GetNpcParty();
		if (!pParty)
		{
			rTargetList.AddTarget(GetBot()->GetID());
			return;
		}

		
		for (CNpcParty::MEMBER_MAP::iterator it = pParty->Begin(); it != pParty->End(); it++)
		{
			mapCandidate.insert(std::make_pair(it->first, it->first));
		}
	}

	float pfSquaredLength = 0.0;
	std::set<HOBJECT> mapSelectedBot;

	for (std::map<HOBJECT, HOBJECT>::iterator it = mapCandidate.begin(); it != mapCandidate.end(); it++)
	{
		CCharacter* pObject = g_pObjectManager->GetChar(it->first);
		if (pObject)
		{
			if (rSpellAreaChecker.IsObjectInApplyRange(pObject, &pfSquaredLength))
			{
				if (GetBot()->GetID() != pObject->GetID() || IsApplyNotMe() != true)
				{
					mapSelectedBot.insert(it->second);
				}
			}
		}
		else
			ERR_LOG(LOG_GENERAL, "NULL == pBot");
	}

	for (std::set<HOBJECT>::iterator it = mapSelectedBot.begin(); it != mapSelectedBot.end(); it++)
	{
		if (rTargetList.byTargetCount < byMaxTargetCount)
		{
			rTargetList.AddTarget(*it);
		}
		else
			break;
	}
}


void CSkillCondition::GetTarget_ApplyRange_Party_LPLow(CCharacter *pAppointTarget, sSKILL_TARGET_LIST& rTargetList, BYTE byMaxTargetCount)
{
	CSpellAreaChecker rSpellAreaChecker;
	rSpellAreaChecker.Create();

	rSpellAreaChecker.PrepareForSelection(GetBot(), pAppointTarget, GetApplyRangeType(), GetApplyAreaSize1(), GetApplyAreaSize2());

	std::map<int, HOBJECT> mapCandidate;

	if (GetBot()->GetObjType() == OBJTYPE_MOB && GetBot()->GetObjType() == OBJTYPE_NPC)
	{
		CNpcParty* pParty = GetBot()->GetNpcParty();
		if (pParty)
		{
			for (CNpcParty::MEMBER_MAP::iterator it = pParty->Begin(); it != pParty->End(); it++)
			{
				CNpc* pObject = g_pObjectManager->GetNpc(it->first);
				if (pObject)
				{
					if (rSpellAreaChecker.IsObjectInApplyRange(pObject, NULL))
					{
						if (pObject->ConsiderLPLow(m_wUse_Skill_LP))
						{
							if (GetBot()->GetID() != pObject->GetID() || IsApplyNotMe() != true)
							{
								mapCandidate.insert(std::make_pair(pObject->GetCurLP(), it->first));
							}
						}
					}
				}
			}

			for (std::map<int, HOBJECT>::iterator it = mapCandidate.begin(); it != mapCandidate.end(); it++)
			{
				if (rTargetList.byTargetCount < byMaxTargetCount)
				{
					rTargetList.AddTarget(it->second);
				}
				else
					break;
			}
		}
	}
}

