#include "stdafx.h"
#include "TqsAlgoAction_Information.h"
#include "TQSNodeAction_Information.h"
#include "CPlayer.h"
#include "NtlPacketGU.h"


CTqsAlgoAction_Information::CTqsAlgoAction_Information(CTqsAlgoObject* pObject) :
	CTqsAlgoAction_Base(pObject, TQS_ALGOCONTROLID_ACTION_INFORMATION, "TQS_ALGOCONTROLID_ACTION_INFORMATION")
{

}


CTqsAlgoAction_Information::~CTqsAlgoAction_Information()
{
}


bool CTqsAlgoAction_Information::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CTQSNodeAction_Information* pNode = dynamic_cast<CTQSNodeAction_Information*>(pControlScriptNode);
	if (pNode)
	{
		m_eInfoType = pNode->m_eInfoType;
		return true;
	}
	return false;
}

int CTqsAlgoAction_Information::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CNtlPacket packet(sizeof(sGU_TIMEQUEST_INFORMATION_NFY));
	sGU_TIMEQUEST_INFORMATION_NFY* res = (sGU_TIMEQUEST_INFORMATION_NFY *)packet.GetPacketData();
	res->wOpCode = GU_TIMEQUEST_INFORMATION_NFY;
	res->byInfoType = m_eInfoType;

	GetOwner()->Broadcast(&packet);

	m_status = COMPLETED;
	return m_status;
}
