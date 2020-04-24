#include "stdafx.h"
#include "Player.h"
#include "CharServer.h"
#include "ClientSession.h"
#include "NtlPacketCU.h"
#include "NtlPacketCQ.h"
#include "Utils.h"
#include "NtlAdmin.h"

CPlayer::CPlayer(ACCOUNTID accountId, CClientSession * pSession)
{
	m_accountId = accountId;
	m_pSession = pSession;
	m_hSession = pSession->GetHandle();

	m_bIsQueue = false;
	m_bIsServerChanged = false;
	m_serverFarmID = 0;
	m_serverChannelID = 0;
	m_characterSelectId = 0;
	SetGM(false);

	m_byPremiumSlotCount = 0;

	m_byCharCount = 0;
	memset(m_asDelData, INVALID_TBLIDX, sizeof(m_asDelData));
	memset(m_sPcData, NULL, sizeof(m_sPcData));

	m_bIsStartCharLoading = true;
	m_bIsCharLoaded = false;

	m_dwDeleteTime = 60000 * 10; //delete after 10 minutes
}


CPlayer::~CPlayer()
{
	//printf("del acc %u \n", GetAccountID());
}



void CPlayer::LoadCharactersFromDB()
{
	//load characters async
	AsyncQuery * q = new AsyncQuery(new SQLClassCallbackP1<CPlayer, ACCOUNTID>(this, &CPlayer::LoadFromDBProc, GetAccountID()));
	q->AddQuery("SELECT CharID,CharName,Level,Race,Class,Gender,Face,Adult,Hair,HairColor,SkinColor,CurLocX,CurLocY,CurLocZ,WorldID,WorldTable,MapInfoIndex,Money,MoneyBank,TutorialFlag,NameChange,GuildID,DelCharTime,Title,InvisibleCostume,SuperiorEffectType FROM characters WHERE AccountID=%u AND SrvFarmID=%u LIMIT 8", GetAccountID(), GetServerFarmID());
	GetCharDB.QueueAsyncQuery(q);
}

void CPlayer::LoadFromDBProc(QueryResultVector & results, ACCOUNTID accountID)
{
	CCharServer* app = (CCharServer*)g_pApp;

	if (results.size() > 0)
	{
		if (g_PlrMgr->GetPlayer(accountID) == NULL || GetSession() == NULL)
		{
			return;
		}

		QueryResult *result = results[0].result;
		if (result)
		{
			do
			{
				Field *f = result->Fetch();

				CHARACTERID charId = f[0].GetUInt32();

				QWORD dwDelTime = f[22].GetUInt64();
				QWORD dwCurTime = (QWORD)time(0);

				if (dwDelTime > 0 && dwDelTime > dwCurTime)
				{
					m_asDelData[m_byCharCount].charId = charId;
					m_asDelData[m_byCharCount].dwRemainTick = DWORD(dwDelTime - dwCurTime) * 1000;
				}

				if (dwDelTime > 0 && dwDelTime <= dwCurTime) //delete time period over ! delete character.
				{
					//del character data async
					CNtlPacket pQry(sizeof(sCQ_CHARACTER_DEL_REQ));
					sCQ_CHARACTER_DEL_REQ * qRes = (sCQ_CHARACTER_DEL_REQ *)pQry.GetPacketData();
					qRes->wOpCode = CQ_CHARACTER_DEL_REQ;
					qRes->accountID = accountID;
					qRes->charID = charId;
					pQry.SetPacketLen(sizeof(sCQ_CHARACTER_DEL_REQ));
					app->SendTo(app->GetQueryServerSession(), &pQry);

					ERR_LOG(LOG_USER, "Account %u deleted character %u(%s)", accountID, charId, f[1].GetString());

					continue;
				}

				m_sPcData[m_byCharCount].charId = charId;

				wcscpy_s(m_sPcData[m_byCharCount].awchName, NTL_MAX_SIZE_CHAR_NAME + 1, s2ws(f[1].GetString()).c_str());

				m_sPcData[m_byCharCount].byLevel = f[2].GetBYTE();
				m_sPcData[m_byCharCount].byRace = f[3].GetBYTE();
				m_sPcData[m_byCharCount].byClass = f[4].GetBYTE();
				m_sPcData[m_byCharCount].byGender = f[5].GetBYTE();
				m_sPcData[m_byCharCount].byFace = f[6].GetBYTE();
				m_sPcData[m_byCharCount].bIsAdult = f[7].GetBool();
				m_sPcData[m_byCharCount].byHair = f[8].GetBYTE();
				m_sPcData[m_byCharCount].byHairColor = f[9].GetBYTE();
				m_sPcData[m_byCharCount].bySkinColor = f[10].GetBYTE();
				m_sPcData[m_byCharCount].fPositionX = f[11].GetFloat();
				m_sPcData[m_byCharCount].fPositionY = f[12].GetFloat();
				m_sPcData[m_byCharCount].fPositionZ = f[13].GetFloat();
				m_sPcData[m_byCharCount].worldId = f[14].GetUInt32();
				m_sPcData[m_byCharCount].worldTblidx = f[15].GetUInt32();
				m_sPcData[m_byCharCount].dwMapInfoIndex = f[16].GetDWORD();
				m_sPcData[m_byCharCount].dwMoney = f[17].GetDWORD();
				m_sPcData[m_byCharCount].dwMoneyBank = f[18].GetDWORD();
				m_sPcData[m_byCharCount].bTutorialFlag = f[19].GetBool();
				m_sPcData[m_byCharCount].bNeedNameChange = f[20].GetBool();
				m_sPcData[m_byCharCount].charTitle = f[23].GetUInt32();
				m_sPcData[m_byCharCount].bInvisibleCostume = f[24].GetBool();
				m_sPcData[m_byCharCount].bySuperiorEffectType = f[25].GetBYTE();

				//set all items invalid to avoid bug
				memset(m_sPcData[m_byCharCount].sItem, INVALID_TBLIDX, sizeof(m_sPcData[m_byCharCount].sItem));

				m_sPcData[m_byCharCount].sDogi.guildId = f[21].GetUInt32();

				//guild mark/dogi
				m_sPcData[m_byCharCount].sDogi.byDojoColor = INVALID_BYTE;
				m_sPcData[m_byCharCount].sDogi.byGuildColor = INVALID_BYTE;
				m_sPcData[m_byCharCount].sDogi.byType = INVALID_BYTE;
				m_sPcData[m_byCharCount].sMark.byMarkInColor = INVALID_BYTE;
				m_sPcData[m_byCharCount].sMark.byMarkInLine = INVALID_BYTE;
				m_sPcData[m_byCharCount].sMark.byMarkMain = INVALID_BYTE;
				m_sPcData[m_byCharCount].sMark.byMarkMainColor = INVALID_BYTE;
				m_sPcData[m_byCharCount].sMark.byMarkOutColor = INVALID_BYTE;
				m_sPcData[m_byCharCount].sMark.byMarkOutLine = INVALID_BYTE;

				m_byCharCount++;
			} while (result->NextRow());
		}

		//load items & guild info async
		AsyncQuery * q = new AsyncQuery(new SQLClassCallbackP1<CPlayer, ACCOUNTID>(this, &CPlayer::LoadFromDBProcSecond, accountID));

		for (BYTE i = 0; i < m_byCharCount; i++)
		{
			q->AddQuery("SELECT tblidx,pos,`rank`,grade,BattleAttribute FROM items WHERE owner_id=%u AND place=6", m_sPcData[i].charId);
		}

		for (BYTE i = 0; i < m_byCharCount; i++)
		{
			if (m_sPcData[i].sDogi.guildId > 0)
				q->AddQuery("SELECT MarkInColor,MarkInLine,MarkMain,MarkMainColor,MarkOutColor,MarkOutLine,DojoColor,GuildColor,DogiType FROM guilds WHERE GuildID=%u LIMIT 1", m_sPcData[i].sDogi.guildId);
		}

		GetCharDB.QueueAsyncQuery(q);
	}
}


void CPlayer::LoadFromDBProcSecond(QueryResultVector & results, ACCOUNTID accountID)
{
	if (g_PlrMgr->GetPlayer(accountID) == NULL || GetSession() == NULL)
	{
		return;
	}

	if (m_byCharCount > NTL_MAX_COUNT_USER_CHAR_SLOT)
	{
		ERR_LOG(LOG_USER, "Error: User %u has more than 8 chars loaded. Count %u", GetAccountID(), m_byCharCount);
		m_byCharCount = 0;
		m_bIsCharLoaded = false;

		return;
	}

	CNtlPacket packet(sizeof(sCU_CHARACTER_INFO));
	sCU_CHARACTER_INFO * res = (sCU_CHARACTER_INFO *)packet.GetPacketData();
	res->wOpCode = CU_CHARACTER_INFO;

	ERR_LOG(LOG_SYSTEM, "Acc %u load chars. m_byCharCount = %u", GetAccountID(), m_byCharCount);

	if (results.size() > 0)
	{
		for (BYTE i = 0; i < m_byCharCount; i++)
		{
			if (results[i].result)
			{
				do
				{
					Field *f = results[i].result->Fetch();
					if (f)
					{
						BYTE byPos = f[1].GetBYTE();

						if (byPos >= EQUIP_SLOT_TYPE_COUNT)
						{
							ERR_LOG(LOG_USER, "ERROR: User %u has item on wrong pos.. Place = 7, Pos = %u", GetAccountID(), byPos);
						}
						else
						{
							m_sPcData[i].sItem[byPos].tblidx = f[0].GetUInt32();
							m_sPcData[i].sItem[byPos].byBattleAttribute = f[4].GetBYTE();
							m_sPcData[i].sItem[byPos].byGrade = f[3].GetBYTE();
							m_sPcData[i].sItem[byPos].byRank = f[2].GetBYTE();
						}
					}

				} while (results[i].result->NextRow());
			}
		}

		BYTE byGuildLoadSlot = m_byCharCount;
		for (BYTE i = m_byCharCount; i < m_byCharCount * 2; i++)
		{
			BYTE bySlot = i - m_byCharCount;
			if (m_sPcData[bySlot].sDogi.guildId > 0)
			{
				//	ERR_LOG(LOG_SYSTEM, "Char %u Load guild %u. i = %u, bySlot = %u, byGuildLoadSlot = %u", m_sPcData[bySlot].charId, m_sPcData[bySlot].sDogi.guildId, i, bySlot, byGuildLoadSlot);
				QueryResult *qrGuildInfo = results[byGuildLoadSlot].result;	//guild info
				if (qrGuildInfo)
				{
					Field *g = qrGuildInfo->Fetch();
					if (g)
					{
						m_sPcData[bySlot].sMark.byMarkInColor = g[0].GetBYTE();
						m_sPcData[bySlot].sMark.byMarkInLine = g[1].GetBYTE();
						m_sPcData[bySlot].sMark.byMarkMain = g[2].GetBYTE();
						m_sPcData[bySlot].sMark.byMarkMainColor = g[3].GetBYTE();
						m_sPcData[bySlot].sMark.byMarkOutColor = g[4].GetBYTE();
						m_sPcData[bySlot].sMark.byMarkOutLine = g[5].GetBYTE();
						m_sPcData[bySlot].sDogi.byDojoColor = g[6].GetBYTE();
						m_sPcData[bySlot].sDogi.byGuildColor = g[7].GetBYTE();
						m_sPcData[bySlot].sDogi.byType = g[8].GetBYTE();

						++byGuildLoadSlot;
					}
				}
			}
		}
	}

	for (BYTE i = 0; i < m_byCharCount; i++)
	{
		res->asDelData[i] = m_asDelData[i];
		res->sPcData[i] = m_sPcData[i];
	}


	res->byCount = m_byCharCount;
	packet.SetPacketLen(sizeof(sCU_CHARACTER_INFO));
	g_pApp->Send(GetSessionHandle(), &packet);


	//load acc data & finish
	AsyncQuery * q2 = new AsyncQuery(new SQLClassCallbackP1<CPlayer, ACCOUNTID>(this, &CPlayer::LoadFromDBProcEnd, GetAccountID()));
	q2->AddQuery("SELECT PremiumSlots,isGm FROM accounts WHERE AccountID = %u LIMIT 1", GetAccountID());
	GetAccDB.QueueAsyncQuery(q2);
}


void CPlayer::LoadFromDBProcEnd(QueryResultVector & results, ACCOUNTID accountID)
{
	if (g_PlrMgr->GetPlayer(accountID) == NULL || GetSession() == NULL)
	{
		return;
	}

	QueryResult *res = results[0].result;
	if (res)
	{
		Field *f = res->Fetch();

		m_byPremiumSlotCount = f[0].GetBYTE();
		SetGM(f[1].GetBYTE() > ADMIN_LEVEL_EARLY_ACCESS);

		CNtlPacket packet(sizeof(sCU_CHARACTER_LOAD_RES));
		sCU_CHARACTER_LOAD_RES * pres = (sCU_CHARACTER_LOAD_RES *)packet.GetPacketData();
		pres->wOpCode = CU_CHARACTER_LOAD_RES;
		pres->wResultCode = 200; // CHARACTER_SUCCESS;
		pres->byPremiumSlotCount = m_byPremiumSlotCount;
		pres->bySlotBasic = CREATE_CHAR_BASIC_SLOT;
		pres->serverFarmId = GetServerFarmID();
		packet.SetPacketLen(sizeof(sCU_CHARACTER_LOAD_RES));
		g_pApp->Send(GetSessionHandle(), &packet);

		m_bIsCharLoaded = true;
	}

	DisableLoadCharInfo(true);

	ERR_LOG(LOG_USER, "Account: %u Finish loading characters", GetAccountID());
}


void CPlayer::SetCharDeleted(CHARACTERID charId, time_t DelTime)
{
	for (BYTE i = 0; i < m_byCharCount; i++)
	{
		if (m_sPcData[i].charId == charId)
		{
			m_asDelData[i].charId = charId;
			m_asDelData[i].dwRemainTick = DWORD(DelTime - time(NULL)) * 1000;

			break;
		}
	}
}


void CPlayer::UnsetCharDeleted(CHARACTERID charId)
{
	for (BYTE i = 0; i < m_byCharCount; i++)
	{
		if (m_asDelData[i].charId == charId)
		{
			m_asDelData[i].charId = INVALID_TBLIDX;
			m_asDelData[i].dwRemainTick = INVALID_DWORD;

			break;
		}
	}
}


void CPlayer::SetNewCharName(CHARACTERID charId, WCHAR * wchCharName)
{
	for (BYTE i = 0; i < m_byCharCount; i++)
	{
		if (m_sPcData[i].charId == charId)
		{
			wcscpy_s(m_sPcData[i].awchName, NTL_MAX_SIZE_CHAR_NAME + 1, wchCharName);

			break;
		}
	}
}

void CPlayer::LoadFromCache()
{
	CNtlPacket packet(sizeof(sCU_CHARACTER_INFO));
	sCU_CHARACTER_INFO * res = (sCU_CHARACTER_INFO *)packet.GetPacketData();
	res->wOpCode = CU_CHARACTER_INFO;

	for (BYTE i = 0; i < m_byCharCount; i++)
	{
		res->asDelData[i] = m_asDelData[i];
		res->sPcData[i] = m_sPcData[i];
	}

	res->byCount = m_byCharCount;
	packet.SetPacketLen(sizeof(sCU_CHARACTER_INFO));
	g_pApp->Send(GetSessionHandle(), &packet);

	CNtlPacket packet2(sizeof(sCU_CHARACTER_LOAD_RES));
	sCU_CHARACTER_LOAD_RES * pres2 = (sCU_CHARACTER_LOAD_RES *)packet2.GetPacketData();
	pres2->wOpCode = CU_CHARACTER_LOAD_RES;
	pres2->wResultCode = 200; // CHARACTER_SUCCESS;
	pres2->byPremiumSlotCount = m_byPremiumSlotCount;
	pres2->bySlotBasic = CREATE_CHAR_BASIC_SLOT;
	pres2->serverFarmId = GetServerFarmID();
	packet2.SetPacketLen(sizeof(sCU_CHARACTER_LOAD_RES));
	g_pApp->Send(GetSessionHandle(), &packet2);
}

bool CPlayer::HasCharacter(CHARACTERID charId)
{
	if (charId == 0 || charId == INVALID_CHARACTERID)
		return false;

	for (BYTE i = 0; i < m_byCharCount; i++)
	{
		if (m_asDelData[i].charId == charId)
			return false;

		if (m_sPcData[i].charId == charId)
			return true;
	}

	return false;
}

bool CPlayer::HasDelPendingCharacter(CHARACTERID charId)
{
	if (charId == 0 || charId == INVALID_CHARACTERID)
		return false;

	for (BYTE i = 0; i < m_byCharCount; i++)
	{
		if (m_asDelData[i].charId == charId)
			return true;
	}

	return false;
}

void CPlayer::AddCharacter(sPC_SUMMARY * pSummary)
{
	memcpy(&m_sPcData[m_byCharCount], pSummary, sizeof(sPC_SUMMARY));

	++m_byCharCount;
}


bool CPlayer::DecreaseDeleteTime(DWORD dwTickCount)
{
	m_dwDeleteTime = UnsignedSafeDecrease<DWORD>(m_dwDeleteTime, dwTickCount);

	return m_dwDeleteTime == 0;
}

