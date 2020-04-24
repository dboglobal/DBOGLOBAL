#include "stdafx.h"
#include "DynamicFieldSystemEvent.h"
#include "TableContainerManager.h"
#include "SystemEffectTable.h"
#include "UseItemTable.h"
#include "ItemManager.h"
#include "ItemDrop.h"
#include "Monster.h"
#include "NtlRandom.h"
#include "NtlPacketGU.h"
#include "NtlPacketGQ.h"
#include "ScriptAlgoObjectManager.h"
#include "GameServer.h"
#include "GameMain.h"
#include "WpsAlgoObject.h"


CDynamicFieldSystemEvent::CDynamicFieldSystemEvent()
{
	Init();
}


CDynamicFieldSystemEvent::~CDynamicFieldSystemEvent()
{
	Destroy();
}


void CDynamicFieldSystemEvent::Init()
{
	m_nAddCount = 0;
	m_dwCurCount = 0;
	m_byBossCount = 0;
	m_dwMaxCount = 20000;
	m_wpsEndTime = INVALID_DBOTIME;
	m_dwNextUpdate = GetTickCount();
}


void CDynamicFieldSystemEvent::Destroy()
{
}


void CDynamicFieldSystemEvent::Create()
{
	CGameServer* app = (CGameServer*)g_pApp;

	CNtlPacket packet(sizeof(sGQ_DYNAMIC_FIELD_SYSTEM_COUNTING_REQ));
	sGQ_DYNAMIC_FIELD_SYSTEM_COUNTING_REQ* res = (sGQ_DYNAMIC_FIELD_SYSTEM_COUNTING_REQ*)packet.GetPacketData();
	res->wOpCode = GQ_DYNAMIC_FIELD_SYSTEM_COUNTING_REQ;
	res->byChannelId = app->GetGsChannel();
	res->ServerFarmId = app->GetGsServerId();
	res->nAddCount = 0;
	res->dwSettingCount = m_dwMaxCount;
	res->tblidx = 80002;
	app->SendTo(app->GetQueryServerSession(), &packet);
}


void CDynamicFieldSystemEvent::TickProcess(DWORD dwTickDiff)
{
	CGameServer* app = (CGameServer*)g_pApp;

	DWORD dwCurTick = GetTickCount();

	if (m_wpsEndTime > dwCurTick) //If boss spawned then return and wait until event finished.
		return;

	if (m_dwNextUpdate < dwCurTick + dwTickDiff)
	{
		if (m_wpsEndTime > 0 && m_wpsEndTime < dwCurTick) //If boss spawn event ended then reset everything..
		{
			ERR_LOG(LOG_GENERAL, "Reset DynamicFieldSystem Event");
			m_nAddCount = 0;
			m_dwCurCount = 0;
			m_byBossCount = 0;
			m_wpsEndTime = INVALID_DBOTIME;

			CNtlPacket packet2(sizeof(sGU_DYNAMIC_FIELD_COUNT_NFY));
			sGU_DYNAMIC_FIELD_COUNT_NFY* res2 = (sGU_DYNAMIC_FIELD_COUNT_NFY*)packet2.GetPacketData();
			res2->wOpCode = GU_DYNAMIC_FIELD_COUNT_NFY;
			res2->byBossCount = 0;
			res2->dwMaxCount = m_dwMaxCount;
			res2->dwCurCount = m_dwCurCount;
			res2->nEventTime = 0;
			g_pObjectManager->SendPacketToAll(&packet2);
		}
		else
		{
			if (m_dwCurCount >= m_dwMaxCount && m_wpsEndTime == INVALID_DBOTIME) //start wps script
			{
				ERR_LOG(LOG_GENERAL, "Start DynamicFieldSystem Event");

				m_wpsEndTime = dwCurTick + (DBO_DYNAMIC_FIELD_EVENT_TIME * 1000);

				CWorld* pMainWorld = app->GetGameMain()->GetWorldManager()->GetDefaultWorld();

				bool bTemp;
				CWpsAlgoObject* pWps = g_pScriptAlgoManager->CreateWpsObject(80002, bTemp);
				if (pWps)
				{
					pWps->SetWorld(pMainWorld);
					pMainWorld->AddScript(pWps);

					pWps->Start();
				}

				m_byBossCount = 12;

				CNtlPacket packet(sizeof(sGU_DYNAMIC_FIELD_COUNT_NFY));
				sGU_DYNAMIC_FIELD_COUNT_NFY* res = (sGU_DYNAMIC_FIELD_COUNT_NFY*)packet.GetPacketData();
				res->wOpCode = GU_DYNAMIC_FIELD_COUNT_NFY;
				res->byBossCount = m_byBossCount;
				res->dwMaxCount = m_dwMaxCount;
				res->dwCurCount = m_dwMaxCount;
				res->nEventTime = 1;
				g_pObjectManager->SendPacketToAll(&packet);
			}
		}

		m_dwNextUpdate = dwCurTick + 600000; //update again in 10 minutes
	}
}


void CDynamicFieldSystemEvent::LoadDynamicField(CCharacter * pPlayer)
{
	CNtlPacket packet(sizeof(sGU_DYNAMIC_FIELD_COUNT_NFY));
	sGU_DYNAMIC_FIELD_COUNT_NFY* res = (sGU_DYNAMIC_FIELD_COUNT_NFY*)packet.GetPacketData();
	res->wOpCode = GU_DYNAMIC_FIELD_COUNT_NFY;
	res->byBossCount = m_byBossCount;
	res->dwMaxCount = m_dwMaxCount;
	res->dwCurCount = m_dwCurCount;
	res->nEventTime = m_byBossCount == 0 ? 0 : 1;
	pPlayer->SendPacket(&packet);
}

void CDynamicFieldSystemEvent::UpdateDynamicFieldCount(DWORD dwCount)
{
	m_dwCurCount = dwCount;

	if (m_dwCurCount >= m_dwMaxCount)
		m_nAddCount = 0;

	CNtlPacket packet(sizeof(sGU_DYNAMIC_FIELD_COUNT_NFY));
	sGU_DYNAMIC_FIELD_COUNT_NFY* res = (sGU_DYNAMIC_FIELD_COUNT_NFY*)packet.GetPacketData();
	res->wOpCode = GU_DYNAMIC_FIELD_COUNT_NFY;
	res->byBossCount = m_byBossCount;
	res->dwMaxCount = m_dwMaxCount;
	res->dwCurCount = m_dwCurCount;
	res->nEventTime = m_byBossCount == 0 ? 0 : 1;
	g_pObjectManager->SendPacketToAll(&packet);
}


void CDynamicFieldSystemEvent::Update(CMonster* pMob, CCharacter* pPlayer)
{
	if (pMob->GetLevel() < 40) //dont do the following if monster below lv 40
		return;

	if (m_wpsEndTime > GetTickCount())
		return;

	if (pPlayer->GetCurWorld() == NULL)
		return;
	if (pPlayer->GetCurWorld()->GetRuleType() != GAMERULE_NORMAL)
		return;

	CGameServer* app = (CGameServer*)g_pApp;

	DYNAMIC_FIELD_SYSTEM_MAP::iterator it = g_pTableContainer->GetDynamicFieldSystemTable()->DynamicFieldBegin();

	for (; it != g_pTableContainer->GetDynamicFieldSystemTable()->DynamicFieldEnd(); it++)
	{
		sDYNAMIC_FIELD_SYSTEM_TBLDAT* pField = it->second;

		switch (pField->byAction)
		{
			case EVENT_SYSTEM_ACTION_ITEM: //drop item
			{
				if (pField->tIndex == pMob->GetTblidx())
				{
					CItemDrop* pDrop = g_pItemManager->CreateSingleDrop(pField->fRate, pField->aIndex);
					if (pDrop)
					{
						sVECTOR3 pos;
						pos.x = pMob->GetCurLoc().x + RandomRangeF(-2.0f, 2.0f);
						pos.y = pMob->GetCurLoc().y;
						pos.z = pMob->GetCurLoc().z + RandomRangeF(-2.0f, 2.0f);

						pDrop->SetOwnership(pPlayer->GetID(), pPlayer->GetPartyID());
						pDrop->StartDestroyEvent();
						pDrop->AddToGround(pMob->GetWorldID(), pos);
					}
				}
			}
			break;
			case EVENT_SYSTEM_ACTION_MOB: //summon mob
			{
				if (pMob->GetLevel() >= pField->adwSetting[0] && pMob->GetLevel() <= pField->adwSetting[1])
				{
					if (Dbo_CheckProbabilityF(pField->fRate)) //check percent
					{
						sMOB_TBLDAT* pTbldat = (sMOB_TBLDAT*)g_pTableContainer->GetMobTable()->FindData(pField->aIndex);
						if (pTbldat)
						{
							sMOB_DATA data;
							InitMobData(data);

							data.worldID = pMob->GetWorldID();
							data.worldtblidx = pMob->GetWorldTblidx();

							data.tblidx = pTbldat->tblidx;

							pMob->GetCurLoc().CopyTo(data.vCurLoc);
							pMob->GetCurLoc().CopyTo(data.vSpawnLoc);
							pMob->GetCurDir().CopyTo(data.vCurDir);
							pMob->GetCurDir().CopyTo(data.vSpawnDir);

							data.actionpatternTblIdx = 1;

							CMonster* pNewMob = (CMonster*)g_pObjectManager->CreateCharacter(OBJTYPE_MOB);
							if (pNewMob)
							{
								if (pNewMob->CreateDataAndSpawn(data, pTbldat))
								{
									pNewMob->SetStandAlone(true);
								}
							}
						}
						else
						{
							ERR_LOG(LOG_SCRIPT, "Could not find MOB-TBLDAT. Tblidx %u", pField->aIndex);
						}
					}
				}
			}
			break;
			case EVENT_SYSTEM_ACTION_BUFF: //give buff
			{
				if (pField->bOnOff && pField->tIndex == pMob->GetTblidx())
				{
					if (Dbo_CheckProbabilityF(pField->fRate))
					{
						sUSE_ITEM_TBLDAT* pUseItemTbldat = (sUSE_ITEM_TBLDAT*)g_pTableContainer->GetUseItemTable()->FindData(pField->aIndex);
						if (pUseItemTbldat)
						{
							sBUFF_INFO buffInfo;
							buffInfo.buffIndex = INVALID_BYTE;
							buffInfo.sourceTblidx = pField->aIndex;
							buffInfo.dwTimeRemaining = pUseItemTbldat->dwKeepTimeInMilliSecs;
							buffInfo.dwInitialDuration = pUseItemTbldat->dwKeepTimeInMilliSecs;
							buffInfo.bySourceType = DBO_OBJECT_SOURCE_ITEM;

							eSYSTEM_EFFECT_CODE effectCode[NTL_MAX_EFFECT_IN_ITEM];
							effectCode[0] = g_pTableContainer->GetSystemEffectTable()->GetEffectCodeWithTblidx(pUseItemTbldat->aSystem_Effect[0]);
							effectCode[1] = g_pTableContainer->GetSystemEffectTable()->GetEffectCodeWithTblidx(pUseItemTbldat->aSystem_Effect[1]);

							for (int x = 0; x < NTL_MAX_EFFECT_IN_ITEM; x++)
							{
								switch (effectCode[x])
								{
									case ACTIVE_HEAL_OVER_TIME:
									{
										buffInfo.aBuffParameter[x].byBuffParameterType = DBO_BUFF_PARAMETER_TYPE_HOT;
										buffInfo.aBuffParameter[x].buffParameter.dwRemainTime = pUseItemTbldat->dwKeepTimeInMilliSecs;
										buffInfo.aBuffParameter[x].buffParameter.fParameter = (float)pUseItemTbldat->aSystem_Effect_Value[x];
									}
									break;
									case ACTIVE_EP_OVER_TIME:
									{
										buffInfo.aBuffParameter[x].byBuffParameterType = DBO_BUFF_PARAMETER_TYPE_HOT;
										buffInfo.aBuffParameter[x].buffParameter.dwRemainTime = pUseItemTbldat->dwKeepTimeInMilliSecs;
										buffInfo.aBuffParameter[x].buffParameter.fParameter = (float)pUseItemTbldat->aSystem_Effect_Value[x];
									}
									break;
									default: break;
								}
							}
							WORD wTemp;
							pPlayer->GetBuffManager()->RegisterSubBuff(&buffInfo, effectCode, pPlayer->GetID(), pUseItemTbldat->byBuff_Group, wTemp, pUseItemTbldat->abySystem_Effect_Type);
						}
					}
				}
			}
			break;
			case EVENT_SYSTEM_ACTION_INVEN_INSERT: //add item to inventory
			{

			}
			break;
			case EVENT_SYSTEM_ACTION_COUNTING://mob kill counter. Increase counter at event aIndex
			{
				if (pMob->GetTblidx() == pField->tIndex && m_dwCurCount < m_dwMaxCount)
				{
					++m_nAddCount;

					if (m_nAddCount >= 100) //update every 100 kills
					{
						CNtlPacket packet(sizeof(sGQ_DYNAMIC_FIELD_SYSTEM_COUNTING_REQ));
						sGQ_DYNAMIC_FIELD_SYSTEM_COUNTING_REQ* res = (sGQ_DYNAMIC_FIELD_SYSTEM_COUNTING_REQ*)packet.GetPacketData();
						res->wOpCode = GQ_DYNAMIC_FIELD_SYSTEM_COUNTING_REQ;
						res->byChannelId = app->GetGsChannel();
						res->ServerFarmId = app->GetGsServerId();
						res->nAddCount = m_nAddCount;
						res->dwSettingCount = m_dwMaxCount;
						res->tblidx = 80002;
						app->SendTo(app->GetQueryServerSession(), &packet);

						m_nAddCount = 0;
					}
				}
			}
			break;
			case EVENT_SYSTEM_ACTION_WPS:
			{

			}
			break;
		}
	}

}

void CDynamicFieldSystemEvent::GetBosses(CCharacter * pPlayer)
{
	if (m_wpsEndTime > GetTickCount()) //check if event in progress
	{
		CNtlPacket packet(sizeof(sGU_DYNAMIC_FIELD_SYSTEM_BOSS_POSITION_RES));
		sGU_DYNAMIC_FIELD_SYSTEM_BOSS_POSITION_RES * res = (sGU_DYNAMIC_FIELD_SYSTEM_BOSS_POSITION_RES *)packet.GetPacketData();
		res->wOpCode = GU_DYNAMIC_FIELD_SYSTEM_BOSS_POSITION_RES;
		res->byBossCount = 0;

		CWpsAlgoObject* pWps = g_pScriptAlgoManager->GetWpsObject(80002);
		if (pWps && !pWps->IsPaused())
		{
			CWpsAlgoObject::SPAWNGROUP_IT it = pWps->GetMobSpawnBegin();
			for (; it != pWps->GetMobSpawnEnd(); it++)
			{
				CMonster* pMob = g_pObjectManager->GetMob(it->second);
				if (pMob && pMob->IsInitialized())
				{
					res->aDynamicFieldBossInfo[res->byBossCount].handle = pMob->GetID();
					pMob->GetCurLoc().CopyTo(res->aDynamicFieldBossInfo[res->byBossCount++].vBossPos);
				}
			}

		}

		m_byBossCount = res->byBossCount;

		pPlayer->SendPacket(&packet);
	}
}


