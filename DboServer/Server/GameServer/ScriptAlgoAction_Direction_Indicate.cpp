#include "stdafx.h"
#include "ScriptAlgoAction_Direction_Indicate.h"
#include "WPSNodeAction_Direction_Indicate.h"
#include "CPlayer.h"
#include "NtlPacketGU.h"


CWpsAlgoAction_Direction_Indicate::CWpsAlgoAction_Direction_Indicate(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_DIRECTION_INDICATE, "SCRIPTCONTROL_ACTION_DIRECTION_INDICATE")
{
}


CWpsAlgoAction_Direction_Indicate::~CWpsAlgoAction_Direction_Indicate()
{
}


bool CWpsAlgoAction_Direction_Indicate::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_Direction_Indicate* pAction = dynamic_cast<CWPSNodeAction_Direction_Indicate*>(pControlScriptNode);
	if (pAction)
	{
		m_bIndicate = pAction->m_bIndicate;
		m_eIndicateType = pAction->m_eIndicateType;
		m_bAutoHide = pAction->m_bAutoHide;
		m_tblidx = pAction->m_tblidx;
		m_vLoc = pAction->m_vLoc;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_Direction_Indicate", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_Direction_Indicate::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CNtlPacket packet(sizeof(sGU_DIRECTION_INDICATE_NFY));
	sGU_DIRECTION_INDICATE_NFY * res = (sGU_DIRECTION_INDICATE_NFY*)packet.GetPacketData();
	res->wOpCode = GU_DIRECTION_INDICATE_NFY;
	res->bIndicate = m_bIndicate;
	res->bAutoHide = m_bAutoHide;
	res->vLoc.x = m_vLoc.x;
	res->vLoc.y = m_vLoc.y;
	res->vLoc.z = m_vLoc.z;
	GetOwner()->GetWorld()->Broadcast(&packet);

	m_status = COMPLETED;
	return m_status;
}
