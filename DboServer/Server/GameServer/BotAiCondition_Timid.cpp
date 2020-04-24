#include "stdafx.h"
#include "BotAiCondition_Timid.h"
#include "ObjectManager.h"
#include "NtlRandom.h"
#include "BotAiAction_Timid.h"
#include "BotAiState.h"


CBotAiCondition_Timid::CBotAiCondition_Timid(CNpc* pBot)
	:CBotAiCondition(pBot, BOTCONTROL_CONDITION_TIMID, "BOTCONTROL_CONDITION_TIMID")
{
	m_dwTime = 0;
}

CBotAiCondition_Timid::~CBotAiCondition_Timid()
{
}


int CBotAiCondition_Timid::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	//Currently disabled because mob/npc could run to wall or places which are not possible to move
	/*

	if (GetBot()->IsActionStatusFlag(BOT_ACTIONSTATUS_TIMID_MOVE))
		return m_status;

	if (!GetBot()->IsMoveAble()) //not sure
		return m_status;

	if (GetBot()->GetMeAttackBot() == INVALID_HOBJECT)
		return m_status;

	CCharacter* pTarget = g_pObjectManager->GetChar(GetBot()->GetMeAttackBot());
	if (pTarget && pTarget->IsInitialized())
	{
		GetBot()->ResetMeAttackBot();

		CNtlVector vDestLoc;
		eTIMID_TYPE tType = GetBot()->GetTimidType();

		if (tType == TIMID_NORMAL)
		{
			CNtlVector rhs(GetBot()->GetCurLoc().operator-(pTarget->GetCurLoc()));
			rhs.y = 0.0f;
			if (!rhs.SafeNormalize())
				return m_status;

			vDestLoc.operator=(GetBot()->GetCurLoc().operator+(rhs.operator*(4.0f)));
		}
		else
		{
			if (tType != TIMID_SPAWN_AREA)
			{
				ERR_LOG(LOG_BOTAI, "invalid timid type[%u] objtype[%u] tblidx[%u]", tType, GetBot()->GetObjType(), GetBot()->GetTblidx());
				m_status = 3;
				return m_status;
			}

			CNtlVector v42;
			v42.x = RandomRangeF(0.0f, 1.0f);
			v42.z = RandomRangeF(0.0f, 1.0f);

			CNtlVector v43;
			v43.x = RandomRangeF(0.0f, 1.0f);
			v43.z = RandomRangeF(0.0f, 1.0f);

			CNtlVector v44(v42.operator-(v43));
			v44.y = 0.0f;

			if (!v44.SafeNormalize())
			{
				v44.x = 1.0f;
				v44.z = 0.0f;
			}

			v44.operator*=(RandomRangeF(0.0f, GetBot()->GetTimidRange()));

			vDestLoc.operator=(GetBot()->GetCurLoc().operator+(v44));
		}

		CBotAiState* pCurState = GetBot()->GetBotController()->GetCurrentState();
		if (pCurState)
		{
			CBotAiAction_Timid* pTimidAction = new CBotAiAction_Timid(GetBot(), vDestLoc);
			if (!pCurState->AddSubControlQueue(pTimidAction, true))
			{
				m_status = FAILED;
				return m_status;
			}

			GetBot()->SetActionStatusFlag(BOT_ACTIONSTATUS_TIMID_MOVE, true);
		}
	}

	*/
	return m_status;
}

