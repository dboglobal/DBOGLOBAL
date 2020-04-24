#ifndef __DRAGONBALL_SCRAMBLE_EVENT_SYSTEM__
#define __DRAGONBALL_SCRAMBLE_EVENT_SYSTEM__

#include "NtlSingleton.h"
#include "NtlDragonBall.h"


class CPlayer;
class CNtlPacket;

class CDragonballScramble : public CNtlSingleton<CDragonballScramble>
{
	struct sBALLS
	{
		bool		bOnGround;
		HOBJECT		hOwner;
		sVECTOR3	vLoc;
		HOBJECT		hHandle;
		TBLIDX		tblidx;
	};

	enum eSTATE
	{
		eREFRESH,	//dragon ball turn shiny and can collect them
		eSTART,
		eCOOLTIME,	//dragonball turned into stone message
		eEND,
	};

public:

	CDragonballScramble();
	virtual ~CDragonballScramble();

private:

	void				Init();

	void				Destroy();

public:

	void				Create();

public:

	void				TickProcess(DWORD dwTick);

	void				StartEvent(bool bStartByCommand = true);

	void				EndEvent(bool bForce = false);
	void				EndEventNfy();

public:

	void				RequestJoin(CPlayer* pPlayer);

	void				RequestBallLoc(CPlayer* pPlayer, bool bEnable);

	void				SpawnBall(CPlayer* pPlayer, bool bFaint = false, bool bLogout = false);

	void				PickUpBall(CPlayer* pPlayer, HOBJECT hHandle, TBLIDX tblidx);
			
private:

	void				BroadCast(CNtlPacket* pPacket);

	void				SendDragonballLocation(CPlayer* pPlayer = NULL);
	

public:

	bool				IsOn() { return m_bOnOff; }

	void				LoadEvent(CPlayer* pChar);

	void				RefreshEvent(CPlayer* pChar);


private:

	bool								m_bOnOff;
		
	DBOTIME								m_timeStart;

	DBOTIME								m_timeEnd;

	DWORD								m_dwNextUpdateTick;

	std::map<CHARACTERID, HOBJECT>		m_mapPlayers;

	sBALLS								m_arrBalls[NTL_ITEM_MAX_DRAGONBALL];
};

#define GetDragonballScrambleEvent()		CDragonballScramble::GetInstance()
#define g_pDragonballScramble				GetDragonballScrambleEvent()

#endif