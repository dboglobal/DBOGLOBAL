#ifndef __AI_DBOG_BOT_STATE_TELEPORTING_H__
#define __AI_DBOG_BOT_STATE_TELEPORTING_H__

#include "BotAiState.h"

class CBotAiState_Teleporting : public CBotAiState
{

public:

	enum eTELEPORTPROCESS
	{
		DESPAWN = 0x0,
		TELEPORT = 0x1,
		WORLD_ENTER_COMPLETE = 0x2,
		SPAWN = 0x3,
		TELEPORT_END = 0x4,
	};

	CBotAiState_Teleporting(CNpc* pBot, WORLDID worldId, TBLIDX worldTblidx, CNtlVector& vTeleportLoc, CNtlVector& vTeleportDir);
	virtual	~CBotAiState_Teleporting();

public:

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	WORLDID m_worldId;
	TBLIDX m_worldTblidx;
	CNtlVector m_vTeleportLoc;
	CNtlVector m_vTeleportDir;
	CBotAiState_Teleporting::eTELEPORTPROCESS m_eTeleportProcess;
	DWORD m_dwCharConditionFlag;
	DWORD m_dwTime;

};

#endif