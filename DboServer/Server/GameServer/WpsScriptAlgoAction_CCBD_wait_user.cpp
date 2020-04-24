#include "stdafx.h"
#include "WpsScriptAlgoAction_CCBD_wait_user.h"
#include "CPlayer.h"


CWpsScriptAlgoAction_CCBD_wait_user::CWpsScriptAlgoAction_CCBD_wait_user(CWpsAlgoObject* pObject) :
	CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_CCBD_WAIT_USER, "SCRIPTCONTROL_ACTION_CCBD_WAIT_USER")
{
}


CWpsScriptAlgoAction_CCBD_wait_user::~CWpsScriptAlgoAction_CCBD_wait_user()
{
}

int CWpsScriptAlgoAction_CCBD_wait_user::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	bool bFlag = true;

	CPlayer* pPlayer = GetOwner()->GetPlayersFirst();
	while (pPlayer)
	{
		if (pPlayer->GetWorldID() != GetOwner()->GetWorld()->GetID() || pPlayer->GetCharStateID() == CHARSTATE_DIRECT_PLAY || pPlayer->GetCharStateID() == CHARSTATE_SPAWNING || pPlayer->GetCharStateID() == CHARSTATE_DESPAWNING || pPlayer->GetCharStateID() == CHARSTATE_TELEPORTING)
		{
			bFlag = false;
			break;
		}

		pPlayer = GetOwner()->GetPlayersNext();
	}

	if (bFlag)
		m_status = COMPLETED;


	return m_status;
}
