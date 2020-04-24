#include "stdafx.h"
#include "TqsAlgoAction_Direction_Indicate.h"
#include "TQSNodeAction_DirectionIndicate.h"
#include "CPlayer.h"
#include "NtlPacketGU.h"


CTqsAlgoAction_Direction_Indicate::CTqsAlgoAction_Direction_Indicate(CTqsAlgoObject* pObject) :
	CTqsAlgoAction_Base(pObject, TQS_ALGOCONTROLID_ACTION_DIRECTION_INDICATE, "TQS_ALGOCONTROLID_ACTION_DIRECTION_INDICATE")
{
}


CTqsAlgoAction_Direction_Indicate::~CTqsAlgoAction_Direction_Indicate()
{
}


bool CTqsAlgoAction_Direction_Indicate::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CTQSNodeAction_DirectionIndicate* pAction = dynamic_cast<CTQSNodeAction_DirectionIndicate*>(pControlScriptNode);
	if (pAction)
	{
		m_bIndicate = pAction->m_bIndicate;
		m_eIndicateType = pAction->m_eIndicateType;
		m_bAutoHide = pAction->m_bAutoHide;
		m_tblidx = pAction->m_tblidx;
		m_vLoc = pAction->m_vLoc;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CTQSNodeAction_DirectionIndicate", pControlScriptNode);
	return false;
}

int CTqsAlgoAction_Direction_Indicate::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CNtlPacket packet(sizeof(sGU_DIRECTION_INDICATE_NFY));
	sGU_DIRECTION_INDICATE_NFY * res = (sGU_DIRECTION_INDICATE_NFY*)packet.GetPacketData();
	res->wOpCode = GU_DIRECTION_INDICATE_NFY;
	res->bIndicate = m_bIndicate;
	res->bAutoHide = m_bAutoHide;
	res->vLoc.x = m_vLoc.x;
	res->vLoc.y = m_vLoc.y;
	res->vLoc.z = m_vLoc.z;
	GetOwner()->Broadcast(&packet);

	m_status = COMPLETED;
	return m_status;
}
