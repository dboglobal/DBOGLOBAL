#include "stdafx.h"
#include "ScriptAlgoAction_ExecAtOnce.h"


CWpsAlgoAction_ExecAtOnce::CWpsAlgoAction_ExecAtOnce(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_EXEC_AT_ONCE, "SCRIPTCONTROL_ACTION_EXEC_AT_ONCE")
{
}


CWpsAlgoAction_ExecAtOnce::~CWpsAlgoAction_ExecAtOnce()
{
}


int CWpsAlgoAction_ExecAtOnce::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	// Update all sub actions at once
	// function only used inside ccbd to spawn/reove mobs

	CControlStateComposite* pLinkObject = (CControlStateComposite*)m_subControlQueue.GetFirst();
	CControlStateComposite* pNextState = NULL;
	if (pLinkObject)
	{
		while (pLinkObject)
		{
			pNextState = (CControlStateComposite*)pLinkObject->GetNext();

			pLinkObject->Start();
			pLinkObject->OnUpdate(dwTickDiff, fMultiple);


			pLinkObject->Stop();
			m_subControlQueue.Remove(pLinkObject);
			SAFE_DELETE(pLinkObject);

			pLinkObject = pNextState;
		}
	}

	m_status = COMPLETED;
	return m_status;
}
