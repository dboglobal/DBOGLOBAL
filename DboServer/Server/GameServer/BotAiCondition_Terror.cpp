#include "stdafx.h"
#include "BotAiCondition_Terror.h"
#include "BotAiAction_Terror.h"


CBotAiCondition_Terror::CBotAiCondition_Terror(CNpc* pBot)
	:CBotAiCondition(pBot, BOTCONTROL_CONDITION_TERROR, "BOTCONTROL_CONDITION_TERROR")
{
}

CBotAiCondition_Terror::~CBotAiCondition_Terror()
{
}


int CBotAiCondition_Terror::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	//printf("CBotAiCondition_Terror::OnUpdate TO DO \n");

	if (!GetBot()->IsMoveAble())
		return m_status;

	if (!GetBot()->IsActionStatusFlag(BOT_ACTIONSTATUS_TERROR))
	{
		if (GetBot()->GetStateManager()->IsCharCondition(CHARCOND_TERROR))
		{
			CBotAiBase* pState = (CBotAiBase*)GetBot()->GetBotController()->GetCurrentState();
			if (pState)
			{
				if (pState->GetControlStateID() == BOTCONTROL_FIGHT)
				{
					CBotAiAction_Terror* pTerror = new CBotAiAction_Terror(GetBot());
					if (!pState->AddSubControlQueue(pTerror, true))
					{
						m_status = FAILED;
						return m_status;
					}

					GetBot()->SetActionStatusFlag(BOT_ACTIONSTATUS_TERROR, true);
				}
			}
		}
	}

	return m_status;
}

