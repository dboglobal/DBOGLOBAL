/******************************************************************************
* File			: ScrambleBalloonHelper.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2009. 6. 18
* Abstract		: 
*****************************************************************************
* Desc			: 도장전 말풍선 관리자의 생성, 소멸을 주관한다
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// simulation
#include "NtlSLDef.h"


class ScrambleBalloonHelper : public RWS::CEventHandler
{
	enum eBATTLE_BALLOON_STATE
	{
		BATTLE_BALLOON_STATE_NOT_CREATE,
		BATTLE_BALLOON_STATE_CREATED,
		BATTLE_BALLOON_STATE_FAIL_CREATE,
	};

	struct sBALLOON_INFO
	{
		eBATTLE_BALLOON_STATE	eBattleBalloonState;
		RwUInt8					byTickCount;
	};

	typedef std::map<SERIAL_HANDLE, sBALLOON_INFO>		MAP_PLAYER_BALLOON;	

public:
	ScrambleBalloonHelper();
	virtual ~ScrambleBalloonHelper();

	RwBool		Create();
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();

protected:
	virtual VOID	HandleEvents( RWS::CMsg &msg );
	VOID			ClearBalloon();

protected:
	MAP_PLAYER_BALLOON		m_mapPlayerBalloon;
};