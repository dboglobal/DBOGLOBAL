#pragma once

#include "DungeonTable.h"

class CPlayer;
class CWorld;
class CWpsAlgoObject;

class CUltimateDungeon
{

public:

	CUltimateDungeon(PARTYID partyId, sDUNGEON_TBLDAT* pDungeonTbldat);
	virtual ~CUltimateDungeon();


public:

	bool						Create(CPlayer* pPlayer);

	void						Destroy();

public:

	PARTYID						GetPartyID() { return m_partyId; }

	sDUNGEON_TBLDAT*			GetTbldat() { return m_pDungeonTbldat; }

	CWorld*						GetWorld() { return m_pWorld; }

public:

	void						AddPlayer(HOBJECT hPlayer);

	void						JoinDungeon(CPlayer* pPlayer);

	void						LeaveDungeon(HOBJECT hPlayer);

private:

	PARTYID						m_partyId;

	sDUNGEON_TBLDAT*			m_pDungeonTbldat;

	CWorld*						m_pWorld;

	std::set<HOBJECT>			m_setPlayers;

};

