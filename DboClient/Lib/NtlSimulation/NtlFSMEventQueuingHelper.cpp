#include "precomp_ntlsimulation.h"
#include "NtlFSMEventQueuingHelper.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlFSMAgent.h"

void AgentQueuingTerrainClick(CNtlFSMAgent *pAgent, RwV3d vPos)
{
	SNtlEventTerrainClick sTerrainClick;
	RwV3dAssignMacro(&sTerrainClick.vPickPos, &vPos);

	RWS::CMsg msg;
	msg.Id = g_EventTerrainClick;
	msg.pData = reinterpret_cast<void*>(&sTerrainClick);

	pAgent->AddEvent(msg);
}

void AgentQueuingKeyboardMove(CNtlFSMAgent *pAgent, RwUInt8 byMoveFlags)
{
	SNtlEventKeyboardMove sKeyboardMove;
	sKeyboardMove.byMoveFlags = byMoveFlags;
	sKeyboardMove.bAvatarAngleChange = FALSE;

	RWS::CMsg msg;
	msg.Id = g_EventKeyboardMove;
	msg.pData = reinterpret_cast<void*>(&sKeyboardMove);

	pAgent->AddEvent(msg);
}
