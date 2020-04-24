#include "stdafx.h"
#include "BotAiAction_PlayScene.h"
#include "PlayScriptControlFactoy.h"
#include "PlayScriptManager.h"


CBotAiAction_PlayScene::CBotAiAction_PlayScene(CNpc* pBot, DWORD dwScriptNumber, DWORD dwSceneNumber)
	:CBotAiAction(pBot, BOTCONTROL_ACTION_SCENE_PLAY, "BOTCONTROL_ACTION_SCENE_PLAY")
{
	m_dwScriptNumber = dwScriptNumber;
	m_dwSceneNumber = dwSceneNumber;
}


CBotAiAction_PlayScene::~CBotAiAction_PlayScene()
{
}


void CBotAiAction_PlayScene::OnEnter()
{
	//printf("CBotAiAction_PlayScene::OnEnter m_dwScriptNumber %u m_dwSceneNumber %u\n", m_dwScriptNumber, m_dwSceneNumber);
	if (GetBot()->IsPlayScriptEnd())
	{
		m_status = COMPLETED;
	}
	else if (CreateVariableMap())
	{
		CPlayScriptControlFactoy pFactory(GetBot());
		if (g_pPlayScriptManager->AttachScript(this, &pFactory, m_dwScriptNumber, m_dwSceneNumber))
		{
			GetBot()->SetActionStatusFlag(BOT_ACTIONSTATUS_PLAYSCENE, true);
		}
		else
		{
			m_status = SYSTEMERROR;
		}
	}
	else
	{
		ERR_LOG(LOG_SCRIPT, "false == CreateVariableMap()");
		m_status = SYSTEMERROR;
	}
}

void CBotAiAction_PlayScene::OnContinue()
{
}

void CBotAiAction_PlayScene::OnPause()
{
}

void CBotAiAction_PlayScene::OnExit()
{
	GetBot()->SetActionStatusFlag(BOT_ACTIONSTATUS_PLAYSCENE, false);
}


int CBotAiAction_PlayScene::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (UpdateSubControlQueue(dwTickDiff, fMultiple) == COMPLETED)
	{
		GetBot()->GetSpsEventMap()->Clear();

		if (GetBot()->IsPlayScriptEnd())
		{
			m_status = COMPLETED;
		}
		else
		{
			CPlayScriptControlFactoy pFactory(GetBot());
			if (!g_pPlayScriptManager->AttachScript(this, &pFactory, m_dwScriptNumber, m_dwSceneNumber))
			{
				ERR_LOG(LOG_SCRIPT, "Failed to re-attach the script. id %u scene %u", m_dwScriptNumber, m_dwSceneNumber);
				m_status = SYSTEMERROR;
			}
		}
	}

	return m_status;
}