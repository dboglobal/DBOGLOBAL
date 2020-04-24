/*****************************************************************************
 *
 * File			: DboVirtualServer.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 9. 30	
 * Abstract		: DBO virtual server
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __DBO_VIRTUAL_SERVER_H__
#define __DBO_VIRTUAL_SERVER_H__

#include "NtlSharedDef.h"

// core
#include "NtlTokenizer.h"

// simulation
#include "NtlNetSender.h"

// dbo
#include "DboDef.h"


#ifndef NET_SERVER_SYNC

#define VIRTUAL_PLAYER_SPAWN_NUM			0

#define VIRTUAL_MONSTER_SPAWN_NUM			1
#define VIRTUAL_MONSTER_MOVE_UPDATE_TICK	300000
#define VIRTUAL_MONSTER_ATTACK_UPDATE_TICK	2000

typedef struct _SVirtualPcData
{
	RwUInt8 byCharId;
	WCHAR wchName[NTL_MAX_SIZE_CHAR_NAME + 1];
	RwUInt8 byRace;
	RwUInt8 byClass;
	RwUInt8 byGender;
	RwUInt8 byFace;
	RwUInt8 byHair;
	RwUInt8 byHairColor;
	RwUInt8 bySkinColor;
	RwUInt8 byBlood;
}SVirtualPcData;


typedef struct _SVirtualBattleData
{
	RwUInt32	uiSerialId;
	RwUInt32	uiTargetSerialId;
	RwBool		bAttackMode;
	DWORD		dwCurrTime;
}SVirtualBattleData;

class CDboVirtualServer
{
private:

	RwUInt32	m_uiSerialId;

	RwUInt32	m_uiAvatarSerialId;
	RwUInt8		m_byLevel;
	RwUInt8		m_bySelCharId;

	RwUInt32	m_uiMobSerial[VIRTUAL_MONSTER_SPAWN_NUM];
	RwUInt32	m_uiMobMoveCurrTime;
	
	RwBool		m_bSitDown;
	RwUInt32	m_uiCurrTime;
	RwUInt32	m_uiAttackerSerialId;
	RwUInt32	m_uiDefenderSerialId;
	RwInt32		m_iCurrentHp;

	RwUInt32	m_uiTargetSerialId;
	RwUInt32	m_uiSkillReqSerialId;
	RwUInt8		m_bySkillActiveType;
	RwUInt32	m_uiSkillTblId;
	RwUInt32	m_uiSkillTime;

	typedef std::list<SVirtualBattleData*> ListAttackBegin;
	ListAttackBegin m_listAttackBegin;

private:

	typedef std::list<SVirtualPcData*> ListPcData;
	typedef std::list<void*> ListPacket;

	ListPcData m_listPcData;
	ListPacket m_listPacket;

	void AddPcData(RwUInt8 byCharId, WCHAR *pName, RwUInt8 byRace, RwUInt8 byClass, RwUInt8 byGender, 
					RwUInt8 byFace, RwUInt8 byHair, RwUInt8 byHairColor, RwUInt8 bySkinColor, RwUInt8 byBlood);
	void RemovePcData(RwUInt8 byCharId);
	SVirtualPcData* FindPcData(RwUInt8 byCharId);

	void AddAttackBegin(RwUInt32 uiSerialId);
	void RemoveAttackBegin(RwUInt32 uiSerialId);

	RwUInt32 AcquireSerialId(void);
	
	void PacketProc(void *pPacket);
	void PopPacket(void);
	void SendEvent(RwUInt32 iDataLen, void *pData);

private:

	RwBool IsSkillCastingExist(void);

	/**
	/* server command parsing                                                                     
	*/

	/**
	* recv packet process
	*/
	void RecvCharTargetSelect(void *pPacket);
	void RecvCharAttackBegin(void *pPacket);
	void RecvCharAttackEnd(void *pPacket);
	void RecvServerCommand(void *pPacket);

	/**
	* send packet process
	*/
	void SendLoginCreateUserRes(void *pPacket);
	void SendLoginRes(void *pPacket);
	void SendCharLoginRes(void *pPacket);
	void SendServerListRes(void *pPacket);
	void SendOneServerRes(void *pPacket);
	void SendCharLoadRes(void *pPacket);
	void SendCharAddRes(void *pPacket);
	void SendCharDelRes(void *pPacket);
	void SendCharSelectRes(void *pPacket);
	void SendGameEnterRes(void *pPacket);

	void SendAvatarCharInfo(void);
	void SendAvatarItemInfo(void);
	void SendAvatarSkillInfo(void);
	void SendAvatarInfoEnd(void);
	void SendAvatarWorldInfo(void);
	void SendPlayerCreate(void);
	void SendMonsterCreate(void);
	void SendMonsterCreate(RwUInt32 uiTblId, RwReal fDist);
	void SendMonsterGroupCreate(RwUInt32 iNum);
	void SendEnterWorldComplete(void);

	void SendItemMoveRes(void *pPacket);

	// update state
	void SendCharUpdateFaintingState(void);
	void SendCharUpdateHTBState(void);
	void SendCharUpdateHTBSendbagState(void);

	// attack
	void SendCharToggleFighting(void *pPacket);
	void SendCharActionAttack(RwUInt32 uiSerialId);

	// sit down and stand up
	void SendCharSitDown(void);
	void SendCharStandUp(void);

	// item
	void SendAddItem(RwUInt32 uiTblId);

	// skill
	void SendCharSkillRes(void *pPacket);
	void SendCharSkillCasting(void);
	void SendCharSkillAction(void);
	void SendCharLearnSkillRes(RwUInt32 uiTblId);
	void SendCharLearnHTBRes(RwUInt32 uiTblId);
	void SendCharHTBSkillRes(void *pPacket);

	// update
	void SendCharUpdateLp(RwUInt16 wLp);

	// server command
	void SendUpdateCharSpeed(RwReal fSpeed);

	// buff
	void SendCharTestBuffRegistered(void);
	
public:

	CDboVirtualServer();
	~CDboVirtualServer();

	static CDboVirtualServer* GetInstance(void);

	void Update(RwReal fElapsed);

	bool SendPacket(RwInt32 iDataLen, void *pData);
};

bool DboVirtualNetSenderFunc(RwInt32 iDataLen, void *pData);

#endif


#endif
