#include "stdafx.h"
#include "WpsScriptAlgoAction_CCBD_Leave.h"


CWpsScriptAlgoAction_CCBD_Leave::CWpsScriptAlgoAction_CCBD_Leave(CWpsAlgoObject* pObject) :
	CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_CCBD, "SCRIPTCONTROL_ACTION_CCBD")
{
}


CWpsScriptAlgoAction_CCBD_Leave::~CWpsScriptAlgoAction_CCBD_Leave()
{
}

void CWpsScriptAlgoAction_CCBD_Leave::OnEnter()
{
}

int CWpsScriptAlgoAction_CCBD_Leave::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	// HERE WE END THE CCBD. CCBD SCRIPT WILL BE ERASED BY ITSELF ONCE ALL PLAYERS TELEPORTED OUT OF CCBD
	//Important: never update the m_status of this function otherwise it will break.

	//printf("CWpsScriptAlgoAction_CCBD_Leave: state %u \n", m_status);

	return m_status;
}
