#include "stdafx.h"
#include "ScriptAlgoAction_ChangeSpsScene.h"
#include "WPSNodeAction_ChangeSpsScene.h"
#include "Npc.h"


CWpsAlgoAction_ChangeSpsScene::CWpsAlgoAction_ChangeSpsScene(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_CHANGE_SPS_SCENE, "SCRIPTCONTROL_ACTION_CHANGE_SPS_SCENE")
{
}


CWpsAlgoAction_ChangeSpsScene::~CWpsAlgoAction_ChangeSpsScene()
{
}


bool CWpsAlgoAction_ChangeSpsScene::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_ChangeSpsScene* pAction = dynamic_cast<CWPSNodeAction_ChangeSpsScene*>(pControlScriptNode);
	if (pAction)
	{
		m_npcTblidx = pAction->m_npcTblidx;
		m_dwPlayScript = pAction->m_dwPlayScript;
		m_dwPlayScene = pAction->m_dwPlayScene;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_ChangeSpsScene", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_ChangeSpsScene::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	bool bFromWorld = false;
	CNpc* pNpc = GetOwner()->FindNpc(m_npcTblidx);

	if (pNpc == NULL && GetOwner()->GetWorld())
	{
		bFromWorld = true;
		pNpc = GetOwner()->GetWorld()->FindNpc(m_npcTblidx);
	}

	if (pNpc == NULL || !pNpc->IsInitialized()) //did we already remove the npc? 
	{
		return m_status;
	}

	CBotAiController* pControll = pNpc->GetBotController();
	if (!pControll)
	{
		m_status = FAILED;
		return m_status;
	}

	if (pControll->GetCurrentStateID() == BOTCONTROL_IDLE)
	{
		if (pControll->IsEmptyNextControlState())
		{
			pNpc->SetPlayScript(m_dwPlayScript, m_dwPlayScene);
			pNpc->SetPlayScriptEnd(false);

			pNpc->AddScript(GetOwner());

			pNpc->GetSpsEventMap()->Clear();

			if (bFromWorld) //if we get NPC from world then add npc to script
			{
				GetOwner()->AddNpc(pNpc);
			}

			pControll->ChangeControlState_Idle();

			m_status = COMPLETED;
		}
	}


	return m_status;
}
