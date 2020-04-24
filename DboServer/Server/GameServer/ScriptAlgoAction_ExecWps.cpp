#include "stdafx.h"
#include "ScriptAlgoAction_ExecWps.h"
#include "WPSNodeAction_ExecWps.h"
#include "ScriptAlgoObjectManager.h"
#include "CPlayer.h"


CWpsAlgoAction_ExecWps::CWpsAlgoAction_ExecWps(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_EXEC_WPS, "SCRIPTCONTROL_ACTION_EXEC_WPS")
{
}


CWpsAlgoAction_ExecWps::~CWpsAlgoAction_ExecWps()
{
}


bool CWpsAlgoAction_ExecWps::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_ExecWps* pAction = dynamic_cast<CWPSNodeAction_ExecWps*>(pControlScriptNode);
	if (pAction)
	{
		m_wpsTblidx = pAction->m_wpsTblidx;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_ExecWps", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_ExecWps::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (m_wpsTblidx != INVALID_TBLIDX)
	{
		bool rbStart = false;
		CWpsAlgoObject* pObj = g_pScriptAlgoManager->CreateWpsObject(m_wpsTblidx, rbStart);
		if (pObj)
		{
			pObj->SetWorld(GetOwner()->GetWorld());

			if (pObj->GetExecuterType() == WORLDPLAY_EXECUTER_STATIC_WORLD || pObj->GetExecuterType() == WORLDPLAY_EXECUTER_DYNAMIC_WORLD)
				pObj->GetWorld()->AddScript(pObj);

			CPlayer* pPlayer = GetOwner()->GetPlayersFirst();
			while (pPlayer)
			{
				pObj->AddPlayer(pPlayer->GetID());
				pPlayer->AddScript(pObj);

				pPlayer = GetOwner()->GetPlayersNext();
			}

			if (rbStart)
				pObj->Start();

			m_status = COMPLETED;
		}
		else m_status = SYSTEMERROR;
	}
	else m_status = SYSTEMERROR;

	return m_status;
}
