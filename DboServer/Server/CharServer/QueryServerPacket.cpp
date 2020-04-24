#include "stdafx.h"
#include "QueryServerSession.h"
#include "CharServer.h"
#include "NtlPacketCU.h"
#include "PlayerManager.h"
#include "Player.h"




//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void CQueryServerSession::OnInvalid(CNtlPacket * pPacket)
{
	UNREFERENCED_PARAMETER(pPacket);
}

////--------------------------------------------------------------------------------------//
////		CREATE CHARACTER
////--------------------------------------------------------------------------------------//
void CQueryServerSession::RecvCreateCharRes(CNtlPacket * pPacket)
{
	sQC_CHARACTER_ADD_RES * req = (sQC_CHARACTER_ADD_RES*)pPacket->GetPacketData();

	CPlayer* pPlayer = g_PlrMgr->GetPlayer(req->accountId);
	if (pPlayer && pPlayer->GetSession())
	{
		if (req->wResultCode == 200) // CHARACTER_SUCCESS
			pPlayer->AddCharacter(&req->sPcDataSummary);

		CNtlPacket packet(sizeof(sCU_CHARACTER_ADD_RES));
		sCU_CHARACTER_ADD_RES * res = (sCU_CHARACTER_ADD_RES *)packet.GetPacketData();
		res->wOpCode = CU_CHARACTER_ADD_RES;
		res->wResultCode = req->wResultCode;
		memcpy(&res->sPcDataSummary, &req->sPcDataSummary, sizeof(sPC_SUMMARY));
		packet.SetPacketLen(sizeof(sCU_CHARACTER_ADD_RES));
		g_pApp->Send(pPlayer->GetSessionHandle(), &packet);
	}
}

void CQueryServerSession::RecvCharacterLoadRes(CNtlPacket * pPacket)
{
	sQC_CHARACTER_LOAD_RES * req = (sQC_CHARACTER_LOAD_RES*)pPacket->GetPacketData();

	CPlayer* pPlayer = g_PlrMgr->GetPlayer(req->accountId);
	if (pPlayer && pPlayer->GetSession())
	{
		pPlayer->GetSession()->LoadServerChannelInfo();

		if (pPlayer->IsCharLoadRequestPossible())
		{
			pPlayer->DisableLoadCharInfo(true);
			pPlayer->LoadCharactersFromDB();
		}
		else if(pPlayer->IsCharsLoaded())
		{
			pPlayer->LoadFromCache();
		}
	}
}
