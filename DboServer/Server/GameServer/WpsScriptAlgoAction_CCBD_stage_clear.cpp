#include "stdafx.h"
#include "WpsScriptAlgoAction_CCBD_stage_clear.h"
#include "CPlayer.h"
#include "NtlPacketGU.h"


CWpsScriptAlgoAction_CCBD_stage_clear::CWpsScriptAlgoAction_CCBD_stage_clear(CWpsAlgoObject* pObject) :
	CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_CCBD_STAGE_CLEAR, "SCRIPTCONTROL_ACTION_CCBD_STAGE_CLEAR")
{
}


CWpsScriptAlgoAction_CCBD_stage_clear::~CWpsScriptAlgoAction_CCBD_stage_clear()
{
}

int CWpsScriptAlgoAction_CCBD_stage_clear::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
//	NTL_PRINT(PRINT_APP, "CWpsScriptAlgoAction_CCBD_stage_clear\n");

	CNtlPacket packet(sizeof(sGU_BATTLE_DUNGEON_STAGE_CLEAR_NFY));
	sGU_BATTLE_DUNGEON_STAGE_CLEAR_NFY* res = (sGU_BATTLE_DUNGEON_STAGE_CLEAR_NFY *)packet.GetPacketData();
	res->wOpCode = GU_BATTLE_DUNGEON_STAGE_CLEAR_NFY;
	GetOwner()->Broadcast(&packet);

	if (GetOwner()->GetCCBDTimeLimit())
	{
		GetOwner()->SetCCBDTimeLimit(false);

		CNtlPacket packet2(sizeof(sGU_BATTLE_DUNGEON_LIMIT_TIME_END_NFY));
		sGU_BATTLE_DUNGEON_LIMIT_TIME_END_NFY* res2 = (sGU_BATTLE_DUNGEON_LIMIT_TIME_END_NFY *)packet2.GetPacketData();
		res2->wOpCode = GU_BATTLE_DUNGEON_LIMIT_TIME_END_NFY;
		GetOwner()->Broadcast(&packet2);
	}

	m_status = COMPLETED;
	return m_status;
}
