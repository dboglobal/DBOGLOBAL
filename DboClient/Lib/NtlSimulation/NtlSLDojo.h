/******************************************************************************
* File			: NtlDojo.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2009. 4. 16
* Abstract		: 
*****************************************************************************
* Desc			: 도장 정보
*****************************************************************************/

#pragma once

// Share
#include "NtlSharedType.h"
#include "NtlSharedDef.h"
#include "NtlDojo.h"

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLEmblemMaker.h"

// simulation
#include "NtlSLDef.h"

struct sDBO_DOJO_BRIEF;
struct sSCRAMBLE_SEAL;


enum eSCRAMBLE_TEAM
{
	SCRAMBLE_TEAM_OFFENCE,
	SCRAMBLE_TEAM_DEFENCE,

	NUM_SCRAMBLE_TEAM,

	INVALID_SCRAMBLE_TEAM
};

struct sSCRAMBLE_TEAM
{
	GUILDID				guildID;
	RwUInt8				byPlayerCount;
	RwUInt8				bySealCount;
	RwUInt32			uiScore;
	RwUInt32			uiTotalPoint;
	WCHAR				awcGuildName[NTL_MAX_SIZE_GUILD_NAME + 1];
	sEmblemFactor		tEmblemFactor;
};

struct sSCRAMBLE_SEAL_INFO
{
	eTOBJECT_STATE_TYPE_C	eState;
	RwV2d					v2Pos;
};

typedef std::map<RwUInt32, sSCRAMBLE_SEAL_INFO*>			MAP_SCRAMBLE_SEAL;

struct sSCRAMBLE_INFO
{
	eDBO_DOJO_STATUS	eDojoState;
	eSCRAMBLE_TEAM		eAvatarTeam;
	RwUInt32			uiScrambleDojoTableIndex;			///< 도장 쟁탈전 대상인 도장의 테이블 인덱스
	RwUInt8				byTeleportProposalIndex;
	MAP_SCRAMBLE_SEAL	mapScrambleSeal;
	RwReal				fLeftTime;							///< 현재 상태에서 다음 상태까지 남은 시간
};

struct sDOJO_INFO
{
	// 기본 정보(서버와 항상 동기화 되어 있다)
	RwUInt32			uiDojoTableIndex;
	GUILDID				guildID;
	RwUInt8				byLevel;
	sDBO_GUILD_MARK		tGuildMark;
};


typedef std::map<RwUInt32, sDOJO_INFO*>			MAP_DOJO;

class CNtlDojo : public RWS::CEventHandler
{
public:
	CNtlDojo();
	virtual ~CNtlDojo();

	virtual RwBool		Create(void);
	void				Update(RwReal fElapsed);
	void				Destroy();

	sDOJO_INFO*			CreateDojo(sDBO_DOJO_BRIEF* pDBO_DOJO_BRIEF);
	void				ClearDojo(RwUInt32 uiDojoTableIndex);
	void				ClearScrambleInfo();

	sDOJO_INFO*			GetDojoInfo(RwUInt32 uiDojoTableIndex);
	sDOJO_INFO*			GetDojoInfo_fromGuildID(GUILDID guildID);
	sSCRAMBLE_TEAM*		GetTeam(eSCRAMBLE_TEAM eTeam);
	sSCRAMBLE_INFO*		GetScramble();

	virtual void		HandleEvents(RWS::CMsg &msg);

protected:
	MAP_DOJO			m_mapDojo;

	sSCRAMBLE_TEAM*		m_pScrambleTeam[NUM_SCRAMBLE_TEAM];
	sSCRAMBLE_INFO		m_ScrambleInfo;
};