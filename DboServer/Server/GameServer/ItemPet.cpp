#include "stdafx.h"
#include "ItemPet.h"
#include "CPlayer.h"
#include "GameServer.h"
#include "NtlRandom.h"
#include "item.h"
#include "TableContainerManager.h"
#include "NtlPacketGU.h"
#include "NtlPacketGQ.h"
#include "NtlResultCode.h"



CItemPet::CItemPet()
{
	Initialize();
}

CItemPet::~CItemPet()
{
	Destroy();
}


void CItemPet::Initialize()
{
	pet_tbldat = NULL;
	mascotGradeTbldat = NULL;
	m_pOwner = NULL;
	m_npcTblidx = INVALID_TBLIDX;
	m_bySourceType = DBO_OBJECT_SOURCE_ITEM;

	memset(m_dwSkillCooldown, NULL, sizeof(m_dwSkillCooldown));

	m_bCanLoot = false;
	m_bCanRemoteSell = false;
}

void CItemPet::Destroy()
{
	if (g_pEventMgr->HasEvent(this, EVENT_MASCOT_VP_REGEN))
		g_pEventMgr->RemoveEvents(this, EVENT_MASCOT_VP_REGEN);
}


void CItemPet::TickProcess(DWORD dwTickDiff)
{
	for (int i = 0; i < DBO_MASCOT_MAX_SKILL_COUNT; i++)
	{
		m_dwSkillCooldown[i] = UnsignedSafeDecrease<DWORD>(m_dwSkillCooldown[i], dwTickDiff);
	}
}


void CItemPet::Summon()
{
	g_pEventMgr->AddEvent(this, &CItemPet::OnEvent_VpRegen, EVENT_MASCOT_VP_REGEN, 10000, 0xFFFFFFFF, 0);
}


void CItemPet::UnSummon()
{
	g_pEventMgr->RemoveEvents(this, EVENT_MASCOT_VP_REGEN);
}


void CItemPet::SetSkillCooldown(BYTE bySkillindex, DWORD dwTime)
{
	m_dwSkillCooldown[bySkillindex] = dwTime;
}


void CItemPet::ResetSkillCooldown(BYTE bySkillindex)
{
	m_dwSkillCooldown[bySkillindex] = 0;
}


bool CItemPet::IsSkillOnCooldown(BYTE bySkillindex)
{
	return m_dwSkillCooldown[bySkillindex] > 0;
}


void CItemPet::OnEvent_VpRegen()
{
	UpdateVP(1);
}



void CItemPet::SetPetData(sMASCOT_STATUS_TBLDAT* pettable, CPlayer* owner, sMASCOT_DATA_EX* mascotData)
{
	pet_tbldat = pettable;

	memcpy(&m_mascotData, mascotData, sizeof(sMASCOT_DATA_EX));

	mascotGradeTbldat = (sMASCOT_GRADE_TBLDAT*)g_pTableContainer->GetMascotGradeTable()->FindData(pettable->byRank);

	m_pOwner = owner;
}

void CItemPet::UpdateExp(DWORD dwPlusExp)
{
	DWORD dwGainExp = m_mascotData.dwCurExp + dwPlusExp;
	if (dwGainExp > mascotGradeTbldat->dwNeedExp)
		dwGainExp = mascotGradeTbldat->dwNeedExp;

	m_mascotData.dwCurExp = dwGainExp;

	CNtlPacket packet(sizeof(sGU_MASCOT_CHANGE_EXP));
	sGU_MASCOT_CHANGE_EXP * res = (sGU_MASCOT_CHANGE_EXP*)packet.GetPacketData();
	res->wOpCode = GU_MASCOT_CHANGE_EXP;
	res->dwCurExp = dwGainExp;
	packet.SetPacketLen(sizeof(sGU_MASCOT_CHANGE_EXP));
	g_pApp->Send(m_pOwner->GetClientSessionID(), &packet);
}

void CItemPet::LearnRandomSkill(BYTE bySlot)
{
	CGameServer* app = (CGameServer*)g_pApp;
	WORD resultcode = GAME_SUCCESS;

	if (bySlot < DBO_MASCOT_MAX_SKILL_COUNT)
	{
		TBLIDX skillTblidx = INVALID_TBLIDX;
		std::vector<sSKILL_TBLDAT*> vSkills;

		if (g_pTableContainer->GetSkillTable()->GetMascotSkillsVector().size() == 0)
		{
			ERR_LOG(LOG_SYSTEM, "ERROR. NO MASCOT SKILLS FOUND");
			return;
		}

		if (m_mascotData.skillTblidx[bySlot] != INVALID_TBLIDX)
			resultcode = GAME_SKILL_NO_EMPTY_SKILL_SLOT;
		else if (bySlot > m_mascotData.byItemRank) 
			resultcode = GAME_SKILL_NO_EMPTY_SKILL_SLOT;

		//remove already learned skills from vector
		for (std::vector<sSKILL_TBLDAT*>::iterator it = g_pTableContainer->GetSkillTable()->GetMascotSkillsVectorBegin(); it != g_pTableContainer->GetSkillTable()->GetMascotSkillsVectorEnd(); it++)
		{
			sSKILL_TBLDAT* skill = *it;
			if (skill)
			{
				bool bFound = false;

				for (int i = 0; i < DBO_MASCOT_MAX_SKILL_COUNT; i++)
				{
					if (m_mascotData.skillTblidx[i] == INVALID_TBLIDX)
						continue;

					if (g_pTableContainer->GetSkillTable()->FindBasicSkillTblidx(m_mascotData.skillTblidx[i]) == skill->tblidx)
					{
						bFound = true;
						continue;
					}
				}

				if (bFound == false)
					vSkills.push_back(skill);
			}
		}

	//	printf("LearnRandomSkill: g_pTableContainer->GetSkillTable()->GetMascotSkillsVector().size() %u \n", g_pTableContainer->GetSkillTable()->GetMascotSkillsVector().size());
	//	printf("LearnRandomSkill: vSkills.size() = %u \n", vSkills.size());

		if (resultcode == GAME_SUCCESS)
		{
			if (vSkills.size() > 0)
			{
				skillTblidx = vSkills[RandomRange(0, (int)vSkills.size() - 1)]->tblidx; //get random skill from vector
				m_mascotData.skillTblidx[bySlot] = skillTblidx;

				CNtlPacket packetQry(sizeof(sGQ_MASCOT_SKILL_ADD_REQ));
				sGQ_MASCOT_SKILL_ADD_REQ * resQry = (sGQ_MASCOT_SKILL_ADD_REQ*)packetQry.GetPacketData();
				resQry->wOpCode = GQ_MASCOT_SKILL_ADD_REQ;
				resQry->charId = m_pOwner->GetCharID();
				resQry->slotId = bySlot;
				resQry->skillTblidx = skillTblidx;
				resQry->byMascotIndex = m_mascotData.byIndex;
				packetQry.SetPacketLen(sizeof(sGQ_MASCOT_SKILL_ADD_REQ));
				app->SendTo(app->GetQueryServerSession(), &packetQry);
			}
			else resultcode = GAME_SKILL_NOT_FOUND;
		}


		CNtlPacket packet(sizeof(sGU_MASCOT_SKILL_ADD_RES));
		sGU_MASCOT_SKILL_ADD_RES * res = (sGU_MASCOT_SKILL_ADD_RES*)packet.GetPacketData();
		res->wOpCode = GU_MASCOT_SKILL_ADD_RES;
		res->bySlot = bySlot;
		res->wResultCode = resultcode;
		res->skillTblidx = skillTblidx;
		res->byMascotIndex = m_mascotData.byIndex;
		packet.SetPacketLen(sizeof(sGU_MASCOT_SKILL_ADD_RES));
		app->Send(m_pOwner->GetClientSessionID(), &packet);
	}
	else ERR_LOG(LOG_SYSTEM, "Player %u hacker. skill slot %u does not exist \n", m_pOwner->GetCharID(), bySlot);
}

void CItemPet::UpdateSkill(BYTE bySlot, BYTE byItemPlace, BYTE byItemPos)
{
	WORD resultcode = GAME_SUCCESS;

	if (bySlot < DBO_MASCOT_MAX_SKILL_COUNT)
	{
		if (IsInvenContainer(byItemPlace))
		{
			CItem* item = m_pOwner->GetPlayerItemContainer()->GetItem(byItemPlace, byItemPos);
			if (item && item->IsLocked() == false)
			{
				TBLIDX skillTblidx = INVALID_TBLIDX;

				if (m_mascotData.skillTblidx[bySlot] == INVALID_TBLIDX)
					resultcode = GAME_SKILL_NOT_EXISTING_SKILL;
				else
				{
					std::vector<sSKILL_TBLDAT*> vSkills;

					if (g_pTableContainer->GetSkillTable()->GetMascotSkillsVector().size() == 0)
					{
						ERR_LOG(LOG_SYSTEM, "ERROR. NO MASCOT SKILLS FOUND");
						resultcode = GAME_SKILL_NOT_FOUND;
					}
					else
					{
						//remove already learned skills from vector
						for (std::vector<sSKILL_TBLDAT*>::iterator it = g_pTableContainer->GetSkillTable()->GetMascotSkillsVectorBegin(); it != g_pTableContainer->GetSkillTable()->GetMascotSkillsVectorEnd(); it++)
						{
							sSKILL_TBLDAT* skill = *it;
							if (skill)
							{
								bool bFound = false;

								for (int i = 0; i < DBO_MASCOT_MAX_SKILL_COUNT; i++)
								{
									if (m_mascotData.skillTblidx[i] == INVALID_TBLIDX)
										continue;

									if (g_pTableContainer->GetSkillTable()->FindBasicSkillTblidx(m_mascotData.skillTblidx[i]) == skill->tblidx)
									{
										bFound = true;
										continue;
									}
								}

								if (bFound == false)
									vSkills.push_back(skill);
							}
						}

						//	printf("UpdateSkill: g_pTableContainer->GetSkillTable()->GetMascotSkillsVector().size() %u \n", g_pTableContainer->GetSkillTable()->GetMascotSkillsVector().size());
						//	printf("UpdateSkill: vSkills.size() = %u \n", vSkills.size());

						if (resultcode == GAME_SUCCESS)
						{
							if (vSkills.size() > 0)
							{
								skillTblidx = vSkills[RandomRange(0, (int)vSkills.size() - 1)]->tblidx; //get random skill from vector
							}
							else resultcode = GAME_SKILL_NOT_FOUND;
						}
					}
				}

				if (resultcode == GAME_SUCCESS)
				{
					CGameServer* app = (CGameServer*)g_pApp;

					CNtlPacket packetQry(sizeof(sGQ_MASCOT_SKILL_UPDATE_REQ));
					sGQ_MASCOT_SKILL_UPDATE_REQ * resQry = (sGQ_MASCOT_SKILL_UPDATE_REQ*)packetQry.GetPacketData();
					resQry->wOpCode = GQ_MASCOT_SKILL_UPDATE_REQ;
					resQry->charId = m_pOwner->GetCharID();
					resQry->slotId = bySlot;
					resQry->skillTblidx = skillTblidx;
					resQry->byMascotIndex = m_mascotData.byIndex;
					resQry->byItemCount = item->GetCount() - 1;
					resQry->itemId = item->GetItemID();
					packetQry.SetPacketLen(sizeof(sGQ_MASCOT_SKILL_UPDATE_REQ));
					app->SendTo(app->GetQueryServerSession(), &packetQry);

					item->SetCount(item->GetCount() - 1, false, false);
					m_mascotData.skillTblidx[bySlot] = skillTblidx;
					ResetSkillCooldown(bySlot); //reset old skill cooldown
				}

				CNtlPacket packet(sizeof(sGU_MASCOT_SKILL_UPDATE_RES));
				sGU_MASCOT_SKILL_UPDATE_RES * res = (sGU_MASCOT_SKILL_UPDATE_RES*)packet.GetPacketData();
				res->wOpCode = GU_MASCOT_SKILL_UPDATE_RES;
				res->bySlot = bySlot;
				res->wResultCode = resultcode;
				res->skillTblidx = skillTblidx;
				res->byMascotIndex = m_mascotData.byIndex;
				packet.SetPacketLen(sizeof(sGU_MASCOT_SKILL_UPDATE_RES));
				g_pApp->Send(m_pOwner->GetClientSessionID(), &packet);
			}
			else ERR_LOG(LOG_SYSTEM, "Player %u hacker. Item not found \n", m_pOwner->GetCharID());
		}
	}
	else ERR_LOG(LOG_SYSTEM, "Player %u hacker. skill slot %u does not exist \n", m_pOwner->GetCharID(), bySlot);
}

void CItemPet::UpgradeSkill(BYTE bySlot, BYTE byItemPlace, BYTE byItemPos)
{
	CGameServer* app = (CGameServer*)g_pApp;
	WORD resultcode = GAME_SUCCESS;

	if (bySlot < DBO_MASCOT_MAX_SKILL_COUNT)
	{
		if (IsInvenContainer(byItemPlace))
		{
			CItem* item = m_pOwner->GetPlayerItemContainer()->GetItem(byItemPlace, byItemPos);
			if (item && item->IsLocked() == false)
			{
				TBLIDX skillTblidx = INVALID_TBLIDX;

				if (m_mascotData.skillTblidx[bySlot] == INVALID_TBLIDX)
					resultcode = GAME_SKILL_NOT_EXISTING_SKILL;
				else
				{
					sSKILL_TBLDAT* pSkillTbldat = (sSKILL_TBLDAT*)g_pTableContainer->GetSkillTable()->FindData(m_mascotData.skillTblidx[bySlot]);
					if (pSkillTbldat)
					{
						skillTblidx = pSkillTbldat->dwNextSkillTblidx;
						if (skillTblidx == INVALID_TBLIDX)
							resultcode = GAME_SKILL_TOO_HIGH_LEVEL_TO_TRAIN;
					}
					else resultcode = GAME_SKILL_TOO_HIGH_LEVEL_TO_TRAIN;
				}

				//	printf("UpgradeSkill res %u %u %u \n", resultcode, skillTblidx, m_mascotData.skillTblidx[bySlot]);

				if (resultcode == GAME_SUCCESS)
				{
					CNtlPacket packetQry(sizeof(sGQ_MASCOT_SKILL_UPGRADE_REQ));
					sGQ_MASCOT_SKILL_UPGRADE_REQ * resQry = (sGQ_MASCOT_SKILL_UPGRADE_REQ*)packetQry.GetPacketData();
					resQry->wOpCode = GQ_MASCOT_SKILL_UPGRADE_REQ;
					resQry->charId = m_pOwner->GetCharID();
					resQry->slotId = bySlot;
					resQry->skillTblidx = skillTblidx;
					resQry->byMascotIndex = m_mascotData.byIndex;
					resQry->byItemPlace = byItemPlace;
					resQry->byItemPos = byItemPos;
					resQry->itemID = item->GetItemID();
					packetQry.SetPacketLen(sizeof(sGQ_MASCOT_SKILL_UPGRADE_REQ));
					app->SendTo(app->GetQueryServerSession(), &packetQry);

					item->SetCount(item->GetCount() - 1, false, false);
					m_mascotData.skillTblidx[bySlot] = skillTblidx;
				}

				CNtlPacket packet(sizeof(sGU_MASCOT_SKILL_UPGRADE_RES));
				sGU_MASCOT_SKILL_UPGRADE_RES * res = (sGU_MASCOT_SKILL_UPGRADE_RES*)packet.GetPacketData();
				res->wOpCode = GU_MASCOT_SKILL_UPGRADE_RES;
				res->bySlot = bySlot;
				res->wResultCode = resultcode;
				res->skillTblidx = skillTblidx;
				res->byMascotIndex = m_mascotData.byIndex;
				packet.SetPacketLen(sizeof(sGU_MASCOT_SKILL_UPGRADE_RES));
				g_pApp->Send(m_pOwner->GetClientSessionID(), &packet);
			}
			else ERR_LOG(LOG_SYSTEM, "Player %u hacker. Item not found \n", m_pOwner->GetCharID());
		}
	}
	else ERR_LOG(LOG_SYSTEM, "Player %u hacker. skill slot %u does not exist \n", m_pOwner->GetCharID(), bySlot);
}

TBLIDX CItemPet::GetDashEpReduceSkill()
{
	for (int i = 0; i < DBO_MASCOT_MAX_SKILL_COUNT; i++)
	{
		if (m_mascotData.skillTblidx[i] >= 314061 && m_mascotData.skillTblidx[i] <= 314063)
			return m_mascotData.skillTblidx[i];
	}

	return INVALID_TBLIDX;
}

void CItemPet::LevelUp()
{
	pet_tbldat = (sMASCOT_STATUS_TBLDAT*)g_pTableContainer->GetMascotStatusTable()->FindData(pet_tbldat->nextMascotTblidx);
	mascotGradeTbldat = (sMASCOT_GRADE_TBLDAT*)g_pTableContainer->GetMascotGradeTable()->FindData(pet_tbldat->byRank);

	m_mascotData.byItemRank = pet_tbldat->byRank;
	m_mascotData.dwCurExp = 0;
	m_mascotData.itemTblidx = pet_tbldat->tblidx;
	m_mascotData.tblidx = pet_tbldat->tblidx;
	m_mascotData.dwMaxVP += RandomRange(pet_tbldat->wVpUpMin, pet_tbldat->wVpUpMax);
}


WORD CItemPet::UpdateVP(DWORD dwVP)
{
	if (IsVpFull())
		return MASCOT_SP_IS_FULL;

	DWORD dwNewVP = m_mascotData.dwCurVP + dwVP;

	if (dwNewVP > m_mascotData.dwMaxVP)
		dwNewVP = m_mascotData.dwMaxVP;

	m_mascotData.dwCurVP = dwNewVP;

	CNtlPacket packet(sizeof(sGU_MASCOT_CHANGE_VP));
	sGU_MASCOT_CHANGE_VP * res = (sGU_MASCOT_CHANGE_VP*)packet.GetPacketData();
	res->wOpCode = GU_MASCOT_CHANGE_VP;
	res->dwCurVP = m_mascotData.dwCurVP;
	packet.SetPacketLen(sizeof(sGU_MASCOT_CHANGE_VP));
	g_pApp->Send(m_pOwner->GetClientSessionID(), &packet);

	return GAME_SUCCESS;
}

bool CItemPet::ConsiderVPLow(float fLowVPFactor)
{
	return (float)GetCurVP() <= (float)GetMaxVP() * fLowVPFactor / 100.0f;
}
