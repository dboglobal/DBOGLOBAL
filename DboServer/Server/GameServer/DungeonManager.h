#ifndef __SERVER_DUNGEON_MANAGER__
#define __SERVER_DUNGEON_MANAGER__


#include "NtlSingleton.h"
#include "TimeQuest.h"
#include "TimeLeapDungeon.h"
#include "UltimateDungeon.h"
#include "BattleDungeon.h"

class CPlayer;

class CDungeonManager : public CNtlSingleton<CDungeonManager>
{

public:

	CDungeonManager();
	virtual ~CDungeonManager();

public:

	void								TickProcess(DWORD dwTickDiff);


	//TMQ
public:

	WORD								CreateTimeQuest(CPlayer* player, TBLIDX id, BYTE byDifficulty, BYTE byMode);
	CTimeQuest*							FindTimeQuest(PARTYID partyid);

private:

	std::map<PARTYID, CTimeQuest*>		m_mapTimeQuest;


	//TLQ
public:

	CTimeLeapDungeon*					CreateTimeLeapDungeon(CPlayer* pOwner, TBLIDX dungeonTblidx);
	CTimeLeapDungeon*					FindTimeLeapDungeon(CHARACTERID charID);
	void								DestroyTimeLeapDungeon(CHARACTERID charID, CTimeLeapDungeon* pDungeon);


private:

	std::map<CHARACTERID, CTimeLeapDungeon*>	m_mapTimeLeapDungeon;


	//ULTIMATE DUNGEON
public:

	CUltimateDungeon*						CreateUltimateDungeon(CPlayer* pOwner, GROUPID groupid, BYTE byDifficulty);
	CUltimateDungeon*						FindUltimateDungeon(PARTYID partyID);
	void									DestroyUltimateDungeon(CUltimateDungeon* pDungeon);


private:

	std::map<PARTYID, CUltimateDungeon*>	m_mapUltimateDungeon;


	//BATTLE DUNGEON
public:

	CBattleDungeon*							CreateBattleDungeon(CPlayer* pOwner, WORD & rwResultcode, BYTE byBeginStage);
	CBattleDungeon*							FindBattleDungeon(PARTYID partyID);
	void									DestroyBattleDungeon(CBattleDungeon* pDungeon);


private:

	std::map<PARTYID, CBattleDungeon*>		m_mapBattleDungeon;


};

#define GetDungeonManager()		CDungeonManager::GetInstance()
#define g_pDungeonManager		GetDungeonManager()

#endif