#include "stdafx.h"
#include "BotAiAction_DirectTurning.h"
#include "SPSNodeAction_DirectTurning.h"


CBotAiAction_DirectTurning::CBotAiAction_DirectTurning(CNpc* pBot)
	: CBotAiAction(pBot, BOTCONTROL_ACTION_DIRECT_TURNING, "BOTCONTROL_ACTION_DIRECT_TURNING")
{
	m_directionTblidx = INVALID_TBLIDX;
}


CBotAiAction_DirectTurning::~CBotAiAction_DirectTurning()
{
}


bool CBotAiAction_DirectTurning::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CSPSNodeAction_DirectTurning* pAction = dynamic_cast<CSPSNodeAction_DirectTurning*>(pControlScriptNode);
	if (pAction)
	{
		m_directionTblidx = pAction->m_directionTblidx;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CSPSNodeAction_DirectTurning", pControlScriptNode);
	return false;
}

void CBotAiAction_DirectTurning::OnEnter()
{
	//CPacketSender::Send_DirectTurning(v7, pBot, v13->m_directionTblidx);
}

void CBotAiAction_DirectTurning::OnExit()
{
}


int CBotAiAction_DirectTurning::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	return m_status;
}

int CBotAiAction_DirectTurning::OnObjectMsg(CObjectMsg* pObjMsg)
{
	if (pObjMsg->GetID() == OBJMSG_CHAR_TURNING_END)
	{
		CObjMsg_CharTurningEnd* pMsg = dynamic_cast<CObjMsg_CharTurningEnd*>(pObjMsg);
		if (pMsg)
		{
			if (m_directionTblidx == pMsg->directTblidx)
				m_status = COMPLETED;
		}
	}

	return m_status;
}