#pragma once

#include "DungeonTable.h"

class CPlayer;
class CWorld;
class CWpsAlgoObject;

class CTimeLeapDungeon
{

public:
	CTimeLeapDungeon(CPlayer* pPlayer, sDUNGEON_TBLDAT* pDungeonTbldat);
	virtual ~CTimeLeapDungeon();


public:

	bool						Create();

	void						Destroy();

public:

	CPlayer*					GetOwner() { return m_pOwner; }

	sDUNGEON_TBLDAT*			GetTbldat() { return m_pDungeonTbldat; }

	CWorld*						GetWorld() { return m_pWorld; }

private:

	CPlayer*					m_pOwner;

	sDUNGEON_TBLDAT*			m_pDungeonTbldat;

	CWorld*						m_pWorld;

};

