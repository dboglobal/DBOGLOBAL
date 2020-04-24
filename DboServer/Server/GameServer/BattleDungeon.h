#pragma once

#include "DungeonTable.h"

class CPlayer;
class CWorld;
class CWpsAlgoObject;

class CBattleDungeon
{

public:

	CBattleDungeon(PARTYID partyId);
	virtual ~CBattleDungeon();


public:

	bool						Create(CPlayer* pPlayer, BYTE byStartStage);

	void						Destroy();

public:

	PARTYID						GetPartyID() { return m_partyId; }

	CWorld*						GetWorld() { return m_pWorld; }

public:

	void						AddPlayer(HOBJECT hPlayer);

	void						JoinDungeon(CPlayer* pPlayer);

	void						LeaveDungeon(HOBJECT hPlayer);

private:

	PARTYID						m_partyId;

	CWorld*						m_pWorld;

	std::set<HOBJECT>			m_setPlayers;
};

