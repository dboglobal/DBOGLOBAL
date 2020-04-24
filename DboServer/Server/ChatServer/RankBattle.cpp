#include "stdafx.h"
#include "RankBattle.h"
#include "NtlPacketTU.h"
#include "ChatServer.h"


CRankBattle::CRankBattle()
{
	Init();
}

CRankBattle::~CRankBattle()
{
	Destroy();
}


void CRankBattle::Init()
{
}


void CRankBattle::Destroy()
{
}


void CRankBattle::LoadRankBattleList(HSESSION hSession, DWORD dwPage, BYTE byCompareDay)
{
	SQLCallbackBase* pCallBack = new SQLClassCallbackP3<CRankBattle, HSESSION, DWORD, BYTE>(this, &CRankBattle::OnLoadRankBattleList, hSession, dwPage, byCompareDay);
	AsyncQuery * q = new AsyncQuery(pCallBack);
	q->AddQuery("SELECT CharID, Points, StraightWin FROM rank_battle ORDER BY Points DESC LIMIT 10 OFFSET %u", dwPage * 10); //page * 10 = 1 * 10 = result 11-20 or if page is 2 then 2 * 10 = result 21-30
	GetCharDB.QueueAsyncQuery(q);
}

void CRankBattle::OnLoadRankBattleList(QueryResultVector & results, HSESSION hSession, DWORD dwPage, BYTE byCompareDay)
{
	BYTE byListNum = 0;
	sRANKBATTLE_RANK_INFO rankInfo[DBO_RANKBATTLE_MAX_RANK_BOARD_RECORD_COUNT];
	WCHAR wchCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
	WCHAR wchGuildName[NTL_MAX_SIZE_GUILD_NAME + 1];

	sVARIABLE_DATA sData;
	sData.Init(1023);


	if (results[0].result)
	{
		do
		{
			Field* f = results[0].result->Fetch();

			smart_ptr<QueryResult> pPlayer = GetCharDB.Query("SELECT CharName, Level, Class, GuildName FROM characters WHERE CharID=%u LIMIT 1", f[0].GetUInt32());
			if (pPlayer)
			{
				Field* i = pPlayer->Fetch();
				if (i)
				{
					rankInfo[byListNum].nPoint = f[1].GetINT();
					rankInfo[byListNum].wStraightWinCount = f[2].GetWORD();

					wcscpy_s(wchCharName, NTL_MAX_SIZE_CHAR_NAME + 1, s2ws(i[0].GetString()).c_str());
					wcscpy_s(wchGuildName, NTL_MAX_SIZE_GUILD_NAME + 1, s2ws(i[3].GetString()).c_str());


					rankInfo[byListNum].wCharName = sData.Write(sizeof(wchCharName), wchCharName);
					rankInfo[byListNum].byLevel = i[1].GetBYTE();
					rankInfo[byListNum].byClass = i[2].GetBYTE();			
					rankInfo[byListNum].wGuildName = sData.Write(sizeof(wchGuildName), wchGuildName);

					rankInfo[byListNum++].dwCompareRank = 0; //dunno
				}
			}


		} while (results[0].result->NextRow());
	}

	WORD wRankInfo = sData.Write(byListNum * sizeof(sRANKBATTLE_RANK_INFO), rankInfo);
	WORD wPacketSize = sData.GetPacketSize(sizeof(sTU_RANKBATTLE_RANK_LIST_RES));

	CNtlPacket packet(wPacketSize);
	sTU_RANKBATTLE_RANK_LIST_RES * res = (sTU_RANKBATTLE_RANK_LIST_RES *)packet.GetPacketData();
	res->wOpCode = TU_RANKBATTLE_RANK_LIST_RES;
	res->wResultCode = CHAT_SUCCESS;
	res->dwPage = dwPage;
	res->byCompareDay = byCompareDay;
	res->byRankInfoCount = byListNum;
	res->wRankInfo = wRankInfo;
	sData.CopyTo(&res->sData, sData.GetDataSize());
	g_pApp->Send(hSession, &packet);
}


void CRankBattle::LoadRankBattleListFindName(HSESSION hSession, WCHAR * wchCharName, DWORD dwPage, BYTE byCompareDay)
{
	//char* target = Ntl_WC2MB(wchCharName);
	//std::string escapeTarget = GetCharDB.EscapeString(target);
	//Ntl_CleanUpHeapString(target);

	//smart_ptr<QueryResult> pPlayer = GetCharDB.Query("SELECT CharID FROM characters WHERE CharName=\"%s\" LIMIT 1", escapeTarget.c_str());
	//if (pPlayer)
	//{
	//	Field* a = pPlayer->Fetch();

	////	smart_ptr<QueryResult> pPlayerRank = GetCharDB.Query("SELECT Points FROM rank_battle WHERE CharID=%u LIMIT 1", a[0].GetUInt32());
	////	if (pPlayerRank)
	////	{
	////		Field* b = pPlayerRank->Fetch();


	////		SQLCallbackBase* pCallBack = new SQLClassCallbackP4<CRankBattle, HSESSION, DWORD, BYTE, WCHAR *>(this, &CRankBattle::OnLoadRankBattleListFindName, hSession, dwPage, byCompareDay, wchCharName);
	////		AsyncQuery * q = new AsyncQuery(pCallBack);
	////		q->AddQuery("SELECT CharID, Points, StraightWin FROM rank_battle WHERE Points <= %u ORDER BY Points DESC LIMIT 10 OFFSET %u", b[0].GetUInt32(), dwPage * 10); //page * 10 = 1 * 10 = result 11-20 or if page is 2 then 2 * 10 = result 21-30
	////		GetCharDB.QueueAsyncQuery(q);

	//		return;
	////	}
	//}

	CNtlPacket packet(sizeof(sTU_RANKBATTLE_RANK_FIND_CHARACTER_RES));
	sTU_RANKBATTLE_RANK_FIND_CHARACTER_RES * res = (sTU_RANKBATTLE_RANK_FIND_CHARACTER_RES *)packet.GetPacketData();
	res->wOpCode = TU_RANKBATTLE_RANK_FIND_CHARACTER_RES;
	res->wResultCode = COMMUNITY_RANK_BATTLE_LIST_NOT_FOUND;
	g_pApp->Send(hSession, &packet);
}


void CRankBattle::OnLoadRankBattleListFindName(QueryResultVector & results, HSESSION hSession, DWORD dwPage, BYTE byCompareDay, WCHAR * wchName)
{
	BYTE byListNum = 0;
	sRANKBATTLE_RANK_INFO rankInfo[DBO_RANKBATTLE_MAX_RANK_BOARD_RECORD_COUNT];
	WCHAR wchCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
	WCHAR wchGuildName[NTL_MAX_SIZE_GUILD_NAME + 1];

	sVARIABLE_DATA sData;
	sData.Init(1023);


	if (results[0].result)
	{
		do
		{
			Field* f = results[0].result->Fetch();

			smart_ptr<QueryResult> pPlayer = GetCharDB.Query("SELECT CharName, Level, Class, GuildName FROM characters WHERE CharID=%u LIMIT 1", f[0].GetUInt32());
			if (pPlayer)
			{
				Field* i = pPlayer->Fetch();
				if (i)
				{
					rankInfo[byListNum].nPoint = f[1].GetINT();
					rankInfo[byListNum].wStraightWinCount = f[2].GetWORD();

					wcscpy_s(wchCharName, NTL_MAX_SIZE_CHAR_NAME + 1, s2ws(i[0].GetString()).c_str());
					wcscpy_s(wchGuildName, NTL_MAX_SIZE_GUILD_NAME + 1, s2ws(i[3].GetString()).c_str());


					rankInfo[byListNum].wCharName = sData.Write(sizeof(wchCharName), wchCharName);
					rankInfo[byListNum].byLevel = i[1].GetBYTE();
					rankInfo[byListNum].byClass = i[2].GetBYTE();
					rankInfo[byListNum].wGuildName = sData.Write(sizeof(wchGuildName), wchGuildName);

					rankInfo[byListNum++].dwCompareRank = 0; //dunno
				}
			}


		} while (results[0].result->NextRow());
	}

	WORD wRankInfo = sData.Write(byListNum * sizeof(sRANKBATTLE_RANK_INFO), rankInfo);
	WORD wPacketSize = sData.GetPacketSize(sizeof(sTU_RANKBATTLE_RANK_LIST_RES));

	CNtlPacket packet(wPacketSize);
	sTU_RANKBATTLE_RANK_FIND_CHARACTER_RES * res = (sTU_RANKBATTLE_RANK_FIND_CHARACTER_RES *)packet.GetPacketData();
	res->wOpCode = TU_RANKBATTLE_RANK_FIND_CHARACTER_RES;
	res->wResultCode = CHAT_SUCCESS;
	res->dwPage = dwPage;
	res->byCompareDay = byCompareDay;
	NTL_SAFE_WCSCPY(res->wszCharName, wchName);
	res->byRankInfoCount = byListNum;
	res->wRankInfo = wRankInfo;
	sData.CopyTo(&res->sData, sData.GetDataSize());
	g_pApp->Send(hSession, &packet);
}
