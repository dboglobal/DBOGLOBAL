#include "stdafx.h"
#include "BotAiCondition_Facing.h"
#include "BotAiAction_PatrolWait.h"


CBotAiCondition_Facing::CBotAiCondition_Facing(CNpc* pBot)
	:CBotAiCondition(pBot, BOTCONTROL_CONDITION_FACING, "BOTCONTROL_CONDITION_FACING")
{

}

CBotAiCondition_Facing::~CBotAiCondition_Facing()
{
}


int CBotAiCondition_Facing::OnObjectMsg(CObjectMsg* pObjMsg)
{
	if (pObjMsg->GetID() == OBJMSG_YOU_FACED
		&& !GetBot()->GetStateManager()->IsCharCondition(CHARCOND_CLICK_DISABLE)
		&& !GetBot()->IsActionStatusFlag(BOT_ACTIONSTATUS_FACING)
		&& GetParent()->GetControlStateID() == BOTCONTROL_ACTION_PATROL)
	{
		CObjMsg_YouFaced* pMsg = dynamic_cast<CObjMsg_YouFaced*>(pObjMsg);
		if (pMsg)
		{
			CBotAiAction_PatrolWait* pWait = new CBotAiAction_PatrolWait(GetBot(), pMsg->hSource);
			if (pWait)
			{
				if (GetParent()->AddSubControlQueue(pWait, true))
				{
					GetBot()->SetActionStatusFlag(BOT_ACTIONSTATUS_FACING, true);
				}
				else
				{
					SAFE_DELETE(pWait);
				}
			}
			else
			{
				ERR_LOG(LOG_BOTAI, "NULL == pPatrolWait");
			}
		}
		else
		{
			ERR_LOG(LOG_BOTAI, "dynamic_cast<CObjMsg_YouFaced*>(pObjMsg) failed");
		}
	}

	return m_status;
}