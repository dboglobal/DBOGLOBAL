#include "stdafx.h"
#include "WpsScriptAlgoAction_CCBD_register_direct_play.h"
#include "WpsNodeAction_CCBD_register_direct_play.h"


CWpsScriptAlgoAction_CCBD_register_direct_play::CWpsScriptAlgoAction_CCBD_register_direct_play(CWpsAlgoObject* pObject) :
	CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_CCBD_REGISTER_DIRECT_PLAY, "SCRIPTCONTROL_ACTION_CCBD_REGISTER_DIRECT_PLAY")
{
}


CWpsScriptAlgoAction_CCBD_register_direct_play::~CWpsScriptAlgoAction_CCBD_register_direct_play()
{
}


bool CWpsScriptAlgoAction_CCBD_register_direct_play::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWpsNodeAction_CCBD_register_direct_play* pAction = dynamic_cast<CWpsNodeAction_CCBD_register_direct_play*>(pControlScriptNode);
	if (pAction)
	{
		m_directIndex = pAction->m_directIndex;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWpsNodeAction_CCBD_register_direct_play", pControlScriptNode);
	return false;
}

int CWpsScriptAlgoAction_CCBD_register_direct_play::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	//printf("CWpsScriptAlgoAction_CCBD_register_direct_play: state %u \n", m_status);
	m_status = COMPLETED;
	return m_status;
}
