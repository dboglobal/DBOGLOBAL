#include "stdafx.h"
#include "WpsScriptAlgoAction_CCBD_reward.h"
#include "WpsNodeAction_CCBD_reward.h"
#include "CPlayer.h"
#include "NtlPacketGU.h"
#include "ItemManager.h"
#include "NtlPacketGT.h"
#include "GameServer.h"


CWpsScriptAlgoAction_CCBD_reward::CWpsScriptAlgoAction_CCBD_reward(CWpsAlgoObject* pObject) :
	CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_CCBD_REWARD, "SCRIPTCONTROL_ACTION_CCBD_REWARD")
{
}


CWpsScriptAlgoAction_CCBD_reward::~CWpsScriptAlgoAction_CCBD_reward()
{
}


bool CWpsScriptAlgoAction_CCBD_reward::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWpsNodeAction_CCBD_reward* pAction = dynamic_cast<CWpsNodeAction_CCBD_reward*>(pControlScriptNode);
	if (pAction)
	{
		m_itemTblidx = pAction->m_itemTblidx;
		m_bLastStage = pAction->m_bLastStage;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWpsNodeAction_CCBD_reward", pControlScriptNode);
	return false;
}

int CWpsScriptAlgoAction_CCBD_reward::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CGameServer* app = (CGameServer*)g_pApp;

	CPlayer* pPlayer = NULL;

	//if (m_bLastStage)
	//{
	//	CNtlPacket pChat(sizeof(sGT_BROADCASTING_SYSTEM_NFY));
	//	sGT_BROADCASTING_SYSTEM_NFY * rChat = (sGT_BROADCASTING_SYSTEM_NFY *)pChat.GetPacketData();
	//	rChat->wOpCode = GT_BROADCASTING_SYSTEM_NFY;
	//	rChat->byMsgType = DBO_BROADCASTING_MSG_TYPE_CCBD;
	//	rChat->sData.sCCBD.byStage = 150;

	//	pPlayer = GetOwner()->GetPlayersFirst();
	//	while (pPlayer)
	//	{
	//		if (pPlayer->GetWorldID() == GetOwner()->GetWorld()->GetID())
	//		{
	//			wcscpy_s(rChat->sData.sCCBD.awszMember[rChat->sData.sCCBD.byMemberCount++], NTL_MAX_SIZE_CHAR_NAME + 1, pPlayer->GetCharName());
	//		}

	//		pPlayer = GetOwner()->GetPlayersNext();
	//	}

	//	pChat.SetPacketLen(sizeof(sGT_BROADCASTING_SYSTEM_NFY));
	//	app->SendTo(app->GetChatServerSession(), &pChat);
	//}


	pPlayer = GetOwner()->GetPlayersFirst();
	while (pPlayer)
	{
		if (pPlayer->GetWorldID() == GetOwner()->GetWorld()->GetID())
		{
			if (pPlayer->GetParty())
			{
				pPlayer->GetParty()->InitPartySelect(m_bLastStage, m_itemTblidx);
				break;
			}
		}
		else
		{
			ERR_LOG(LOG_GENERAL, "User is registered for ccbd but has different world id (%u != %u) !!!", pPlayer->GetWorldID(), GetOwner()->GetWorld()->GetID());
		}

		pPlayer = GetOwner()->GetPlayersNext();
	}


	m_status = COMPLETED;
	return m_status;
}
