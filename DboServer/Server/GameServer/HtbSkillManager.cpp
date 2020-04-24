#include "stdafx.h"
#include "CPlayer.h" //include htbskillmanager.h -> htbskill.h
#include "TableContainerManager.h"
#include "NtlResultCode.h"
#include "GameServer.h"
#include "NtlPacketGQ.h"



CHtbSkillManager::CHtbSkillManager()
{
}


CHtbSkillManager::~CHtbSkillManager()
{
}



bool CHtbSkillManager::Create(CPlayer * pOwnerRef)
{
	m_pPlayerRef = pOwnerRef;

	if (m_htbList.Create(NTL_HTB_MAX_PC_HAVE_HTB_SKILL, 0))
	{
		m_ppHtbSkill = new CHtbSkill*[NTL_HTB_MAX_PC_HAVE_HTB_SKILL];

		for (BYTE i = 0; i < NTL_HTB_MAX_PC_HAVE_HTB_SKILL; i++)
			m_ppHtbSkill[i] = NULL;

		return  true;
	}

	Destroy();

	return true;
}


void CHtbSkillManager::Destroy()
{
}


void CHtbSkillManager::Init()
{
	m_pPlayerRef = NULL;
}


void CHtbSkillManager::TickProcess(DWORD dwTickDiff)
{
	for (unsigned int pos = m_htbList.Begin();
		pos != m_htbList.End();
		pos = m_htbList.Next(pos))
	{
		CHtbSkill* pHtb = m_htbList.GetAt(pos);
		if (pHtb)
			pHtb->TickProcess(dwTickDiff);
	}
}


void CHtbSkillManager::LearnHtbSkill(TBLIDX skillTblidx, WORD & rwResultCode)
{
	rwResultCode = GAME_FAIL;

	if (NTL_HTB_MAX_PC_HAVE_HTB_SKILL > m_htbList.GetSize())
	{
		sHTB_SET_TBLDAT* pHtbData = (sHTB_SET_TBLDAT*)g_pTableContainer->GetHTBSetTable()->FindData(skillTblidx);
		if (pHtbData)
		{
			if (CanLearnHtbSkill(pHtbData->tblidx, rwResultCode)) //check if current skill already learned
			{
				if (m_pPlayerRef->GetLevel() >= pHtbData->byRequire_Train_Level) //check required lvl
				{
					if (Dbo_CheckClass(m_pPlayerRef->GetClass(), pHtbData->dwPC_Class_Bit_Flag) == true) //check class
					{
						rwResultCode = GAME_SUCCESS;

						CHtbSkill* pSkill = new CHtbSkill;

						if (pSkill->Create(pHtbData, m_pPlayerRef, INVALID_BYTE, 0))
						{
							AddHtbSkill(pSkill);
						}
						else
							ERR_LOG(LOG_GENERAL, "fail : htb skill create fail. Player[%u] htb skill tblidx[%u]", m_pPlayerRef->GetCharID(), pHtbData->tblidx);

						pSkill->OnLearnHtbSkill();
					}
					else rwResultCode = GAME_SKILL_TRIED_TO_LEARN_WRONG_CLASS_SKILL;
				}
				else rwResultCode = GAME_SKILL_TOO_HIGH_LEVEL_TO_TRAIN;
			}
			else rwResultCode = GAME_SKILL_ALREADY_MASTERED_SKILL;
		}
		else rwResultCode = GAME_SKILL_NOT_EXISTING_SKILL;
	}
	else rwResultCode = GAME_SKILL_NO_EMPTY_SKILL_SLOT;
}


bool CHtbSkillManager::AddHtbSkill(CHtbSkill * pHtbSkill)
{
	BYTE bySkillIndex = pHtbSkill->GetSkillIndex();
	BYTE finalSkillIndex = INVALID_BYTE;

	if (bySkillIndex == 255)
	{
		for (BYTE j = 0; j < NTL_HTB_MAX_PC_HAVE_HTB_SKILL && j != INVALID_BYTE; j++)
		{
			if (!m_ppHtbSkill[j])
			{
				finalSkillIndex = j;
				break;
			}
		}
	}
	else
	{
		if (NTL_HTB_MAX_PC_HAVE_HTB_SKILL <= bySkillIndex)
			return false;

		if (m_ppHtbSkill[bySkillIndex])
			return false;

		finalSkillIndex = bySkillIndex;
	}

	if (finalSkillIndex == INVALID_BYTE)
		return false;
	else
	{
		m_ppHtbSkill[finalSkillIndex] = pHtbSkill;

		if (m_htbList.Insert(pHtbSkill) == m_htbList.End())
		{
			m_ppHtbSkill[finalSkillIndex] = NULL;

			return false;
		}
		else
		{
			if (bySkillIndex == INVALID_BYTE)
				pHtbSkill->SetSkillIndex(finalSkillIndex);

			return true;
		}
	}

	return false;
}

CHtbSkill * CHtbSkillManager::GetHtbSkill(BYTE bySlot)
{
	return m_ppHtbSkill[bySlot];
}


CHtbSkill* CHtbSkillManager::FindHtbSkill(TBLIDX tblidx)
{
	for (unsigned int pos = m_htbList.Begin();
		pos != m_htbList.End();
		pos = m_htbList.Next(pos))
	{
		CHtbSkill* pHtb = m_htbList.GetAt(pos);
		if (pHtb && pHtb->GetHtbID() == tblidx)
		{
			return pHtb;
		}
	}

	return NULL;
}


bool CHtbSkillManager::CanLearnHtbSkill(TBLIDX tblidx, WORD & rwResultCode)
{
	if (m_htbList.GetSize() == NTL_HTB_MAX_PC_HAVE_HTB_SKILL) //already learned max amount of htb skills
	{
		rwResultCode = 618; //GAME_SKILL_NO_EMPTY_SKILL_SLOT
		return false;
	}

	if (FindHtbSkill(tblidx)) //already has htb skill
	{
		rwResultCode = 620; //GAME_SKILL_ALREADY_MASTERED_SKILL
		return false;
	}

	return true;
}


void CHtbSkillManager::LearnHtb(TBLIDX skillTblidx, WORD & rwResultCode)
{
}


int CHtbSkillManager::CopyToInfo(sHTB_SKILL_INFO * pHtbInfo)
{
	int nSize = 0;

	for (unsigned int pos = m_htbList.Begin();
		pos != m_htbList.End();
		pos = m_htbList.Next(pos))
	{
		CHtbSkill* pHtb = (CHtbSkill*)m_htbList.GetAt(pos);
		if (pHtb)
		{
			{
				nSize += pHtb->CopyToInfo(&pHtbInfo[pos]);
			}
		}
	}

	return nSize;
}


int CHtbSkillManager::CopyToData(sHTB_SKILL_DATA * pHtbData)
{
	int nSize = 0;

	for (unsigned int pos = m_htbList.Begin();
		pos != m_htbList.End();
		pos = m_htbList.Next(pos))
	{
		CHtbSkill* pHtb = (CHtbSkill*)m_htbList.GetAt(pos);
		if (pHtb)
		{
			{
				nSize += pHtb->CopyToData(pHtbData);
				++pHtbData;
			}
		}
	}

	return nSize;
}


void CHtbSkillManager::_SaveHtbCooldowns()
{
	CGameServer* app = (CGameServer*)g_pApp;

	CNtlPacket packet(sizeof(sGQ_SAVE_HTB_DATA_REQ));
	sGQ_SAVE_HTB_DATA_REQ* res = (sGQ_SAVE_HTB_DATA_REQ*)packet.GetPacketData();
	res->wOpCode = GQ_SAVE_HTB_DATA_REQ;
	res->charId = m_pPlayerRef->GetCharID();
	res->handle = m_pPlayerRef->GetID();
	res->bySkillCount = 0;

	for (unsigned int pos = m_htbList.Begin();
		pos != m_htbList.End();
		pos = m_htbList.Next(pos))
	{
		CHtbSkill* pHtb = (CHtbSkill*)m_htbList.GetAt(pos);
		if (pHtb)
		{
			res->asHTBSkill[res->bySkillCount].dwSkillTime = pHtb->GetCoolTimeRemaining();
			res->asHTBSkill[res->bySkillCount++].skillId = pHtb->GetHtbID();
		}
	}

	if (res->bySkillCount > 0)
	{
		packet.SetPacketLen(sizeof(sGQ_SAVE_HTB_DATA_REQ));
		app->SendTo(app->GetQueryServerSession(), &packet);
	}
}



