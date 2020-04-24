#include "precomp_ntlsimulation.h"
#include "NtlSLCCBD.h"

// core
#include "NtlDebug.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLEventFunc.h"
#include "NtlSobAvatar.h"
#include "NtlSLLogic.h"


CNtlCCBD::CNtlCCBD()
{
}

CNtlCCBD::~CNtlCCBD()
{
}

RwBool CNtlCCBD::Create(void)
{
	NTL_FUNCTION("CNtlCCBD::Create");

	SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
	if (!pAvatarInfo)
		NTL_RETURN(FALSE);

	Clear();

	NTL_RETURN(TRUE);
}

void CNtlCCBD::Destroy()
{
	NTL_FUNCTION("CNtlCCBD::Destroy");

	Clear();

	NTL_RETURNVOID();
}

void CNtlCCBD::Update(RwReal fElapsed)
{
}

void CNtlCCBD::HandleEvents(RWS::CMsg &msg)
{
	NTL_FUNCTION("CNtlCCBD::HandleEvents");

	if (msg.Id == g_EventBattleDungeonStateUpdate)
	{
		SNtlEventBattleDungeonStateUpdate* pEvent = reinterpret_cast<SNtlEventBattleDungeonStateUpdate*>(msg.pData);

		m_byStage = pEvent->byStage;
	}
	else if (msg.Id == g_EventChangeWorldConceptState)
	{
		SNtlEventWorldConceptState* pEvent = reinterpret_cast<SNtlEventWorldConceptState*>(msg.pData);

		if (pEvent->eWorldConcept != WORLD_PLAY_CCBD)
			NTL_RETURNVOID();

		if (pEvent->uiState != WORLD_STATE_EXIT)
			NTL_RETURNVOID();

		Clear();
	}

	NTL_RETURNVOID();
}

void CNtlCCBD::Clear()
{
	m_byStage = 0;
}
