#include "precomp_dboclient.h"
#include "DboVirtualServer.h"

// shared
#include "NtlClientNet.h"
#include "NtlCharacter.h"
#include "NtlPacketCommon.h"
#include "NtlPacketUA.h"
#include "NtlPacketUC.h"
#include "NtlPacketUG.h"
#include "NtlPacketUT.h"
#include "NtlPacketAU.h"
#include "NtlPacketCU.h"
#include "NtlPacketGU.h"
#include "NtlResultCode.h"
#include "PCTable.h"
#include "SkillTable.h"
#include "NtlBattle.h"
#include "HTBSetTable.h"
#include "TableContainer.h"


// core
#include "NtlDebug.h"
#include "NtlMath.h"


// simulation
#include "NtlSLDef.h"
#include "NtlSLGlobal.h"
#include "NtlEventQueue.h"
#include "NtlSobActor.h"
#include "NtlSobAvatar.h"
#include "NtlSobItem.h"
#include "NtlSobPet.h"
#include "NtlSobManager.h"
#include "NtlInventory.h"
#include "NtlSobSkill.h"
#include "NtlSobSkillAttr.h"
#include "NtlSLLuaGlue.h"
#include "NtlSobProxy.h"
#include "NtlSLLogic.h"
#include "NtlBehaviorProj.h"
#include "NtlSLApi.h"
#include "NtlSLEventFunc.h"


// dbo
#include "DboEvent.h"

#ifndef NET_SERVER_SYNC

#define VIRTUAL_MAX_LP		1000
#define VIRTUAL_MAX_EP		1000
#define VIRTUAL_MAX_RP		1000

#define VIRTUAL_CURR_LP		1000
#define VIRTUAL_CURR_EP		1000
#define VIRTUAL_CURR_RP		1000

#define VIRTUAL_DAMAGE		20

#define VIRTUAL_PHISHIC_DEFENCE		10
#define VIRTUAL_PHISHIC_OFFENCE		10

#define VIRTUAL_ENERGY_DEFENCE		10
#define VIRTUAL_ENERGY_OFFENCE		10

#define VIRTUAL_ATTACK_RATE			50
#define VIRTUAL_DODGE_RATE			50
#define VIRTUAL_BLOCK_RATE			50
#define VIRTUAL_CRITIAL_RATE		50

// #define VIRTUAL_AVATAR_POSX			4863.0f  // thread load test
// #define VIRTUAL_AVATAR_POSZ			4339.0f  // thread load test

#define VIRTUAL_AVATAR_POSX			4583.0f
#define VIRTUAL_AVATAR_POSZ			4070.0f

// #define VIRTUAL_AVATAR_POSX			5473.0f  // 물
// #define VIRTUAL_AVATAR_POSZ			4902.0f

// #define VIRTUAL_AVATAR_POSX			5999.0f  // 카린탑 마을
// #define VIRTUAL_AVATAR_POSZ			253.0f


#define VIRTUAL_AVATAR_LEVEL		1
#define VIRTUAL_AVATAR_SPEED		8.0f
#define VIRTUAL_AVATAR_LEVEL		1
#define VIRTUAL_AVATAR_CURR_EXP		50

#define VIRTUAL_SKILL_TYPE_NONE		0
#define VIRTUAL_SKILL_TYPE_CASTING	1
#define VIRTUAL_SKILL_TYPE_ACTION	2



CDboVirtualServer::CDboVirtualServer()
{
	m_uiSerialId = 1;

	m_uiAvatarSerialId = INVALID_SERIAL_ID;
	m_byLevel = 1;
	m_bySelCharId = 0;

//	m_uiMobSerial[VIRTUAL_MONSTER_SPAWN_NUM];
//	m_uiMobMoveCurrTime;

	m_bSitDown				= FALSE;
	m_uiCurrTime			= 0;
	m_uiAttackerSerialId	= INVALID_SERIAL_ID;
	m_uiDefenderSerialId	= INVALID_SERIAL_ID;
	m_iCurrentHp			= VIRTUAL_MAX_LP;
	m_uiTargetSerialId		= INVALID_SERIAL_ID;

	m_bySkillActiveType		= VIRTUAL_SKILL_TYPE_NONE;
	m_uiSkillTblId = 0;
}

CDboVirtualServer::~CDboVirtualServer()
{
	ListPacket::iterator it;
	for(it = m_listPacket.begin(); it != m_listPacket.end(); ++it)
	{
		void *pData = (*it);
		NTL_ARRAY_DELETE(pData);
	}
	m_listPacket.clear(); 

	ListPcData::iterator the;
	for(the = m_listPcData.begin(); the != m_listPcData.end(); ++the)
	{
		SVirtualPcData *pPcData = (*the);
		NTL_DELETE(pPcData);
	}
	m_listPcData.clear(); 
}

RwUInt32 CDboVirtualServer::AcquireSerialId(void)
{
	if(m_uiSerialId++)
	{
		if(m_uiSerialId == INVALID_SERIAL_ID)
			m_uiSerialId = 0;
	}

	return m_uiSerialId;
}


void CDboVirtualServer::AddPcData(RwUInt8 byCharId, WCHAR *pName, RwUInt8 byRace, RwUInt8 byClass, RwUInt8 byGender, 
									RwUInt8 byFace, RwUInt8 byHair, RwUInt8 byHairColor, RwUInt8 bySkinColor, RwUInt8 byBlood)
{
	SVirtualPcData *pPcData = NTL_NEW SVirtualPcData;
	memset(pPcData, 0, sizeof(SVirtualPcData));

	pPcData->byCharId = byCharId;
	wcscpy_s(pPcData->wchName, NTL_MAX_SIZE_CHAR_NAME, pName);
	pPcData->byRace = byRace;
	pPcData->byClass = byClass;
	pPcData->byGender = byGender;
	pPcData->byFace = byFace;
	pPcData->byHair = byHair;
	pPcData->byHairColor = byHairColor;
	pPcData->bySkinColor = bySkinColor;
	pPcData->byBlood = byBlood;

	m_listPcData.push_back(pPcData);
}

void CDboVirtualServer::RemovePcData(RwUInt8 byCharId)
{
	ListPcData::iterator it;
	for(it = m_listPcData.begin(); it != m_listPcData.end(); ++it)
	{
		SVirtualPcData *pPcData = (*it);
		if(pPcData->byCharId == byCharId)
		{
			NTL_DELETE(pPcData);
			m_listPcData.erase(it); 
			return;
		}
	}
}

SVirtualPcData* CDboVirtualServer::FindPcData(RwUInt8 byCharId)
{
	ListPcData::iterator it;
	for(it = m_listPcData.begin(); it != m_listPcData.end(); ++it)
	{
		SVirtualPcData *pPcData = (*it);
		if(pPcData->byCharId == byCharId)
			return pPcData;
	}

	return NULL;
}

void CDboVirtualServer::AddAttackBegin(RwUInt32 uiSerialId)
{
	SVirtualBattleData *pBattleData = NTL_NEW SVirtualBattleData;

	pBattleData->uiSerialId			= uiSerialId;
	pBattleData->uiTargetSerialId	= INVALID_SERIAL_ID;
	pBattleData->bAttackMode		= TRUE;
	pBattleData->dwCurrTime			= timeGetTime();

	m_listAttackBegin.push_back(pBattleData);
}

void CDboVirtualServer::RemoveAttackBegin(RwUInt32 uiSerialId)
{
	SVirtualBattleData *pBattleData;
	ListAttackBegin::iterator it;

	for(it = m_listAttackBegin.begin(); it != m_listAttackBegin.end(); it++)
	{
		pBattleData = (*it);
		if(pBattleData->uiSerialId == uiSerialId)
		{
			NTL_DELETE(pBattleData);
			m_listAttackBegin.erase(it);
			return;
		}
	}
}


void CDboVirtualServer::RecvCharTargetSelect(void *pPacket)
{
	sUG_CHAR_TARGET_SELECT *pCharTargetSelect = (sUG_CHAR_TARGET_SELECT*)pPacket;
	m_uiTargetSerialId = pCharTargetSelect->hTarget;
}


void CDboVirtualServer::RecvCharAttackBegin(void *pPacket)
{
	sUG_CHAR_ATTACK_BEGIN *pAttackBegin = (sUG_CHAR_ATTACK_BEGIN*)pPacket;
	if(pAttackBegin->byType == 0)
	{
		SERIAL_HANDLE hSerialId = GetNtlSLGlobal()->GetSobAvatar()->GetSerialID();
		AddAttackBegin(hSerialId);
	}
	else if(pAttackBegin->byType == 1)
	{
		CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
		CNtlPetBuffer *pPetBuffer = pSobAvatar->GetPetBuffer();
		AddAttackBegin(pPetBuffer->GetSerialId(0));
	}
}

void CDboVirtualServer::RecvCharAttackEnd(void *pPacket)
{
	sUG_CHAR_ATTACK_END *pAttackEnd = (sUG_CHAR_ATTACK_END*)pPacket;
	if(pAttackEnd->byType == 0)
	{
		SERIAL_HANDLE hSerialId = GetNtlSLGlobal()->GetSobAvatar()->GetSerialID();
		RemoveAttackBegin(hSerialId);
	}
	else if(pAttackEnd->byType == 1)
	{
		CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
		CNtlPetBuffer *pPetBuffer = pSobAvatar->GetPetBuffer();
		RemoveAttackBegin(pPetBuffer->GetSerialId(0));
	}
}

void CDboVirtualServer::RecvServerCommand(void *pPacket)
{
	sUG_SERVER_COMMAND *pServerCmd = (sUG_SERVER_COMMAND*)pPacket;

	char chBuffer[1024];
	::WideCharToMultiByte(GetACP(), 0, pServerCmd->awchCommand, -1, chBuffer, 1024, NULL, NULL);

	CNtlTokenizer lexer(chBuffer);

	if(!lexer.IsSuccess())
		return;

	enum ECmdParseState
	{
		SERVER_CMD_NONE,
		SERVER_CMD_KEY,
		SERVER_CMD_END,
	};

	ECmdParseState eState = SERVER_CMD_KEY;
	RwInt32 iOldLine = 0;
	RwInt32 iLine;
	
	while(1)
	{
		std::string strToken = lexer.PeekNextToken(NULL, &iLine);

		if(strToken == "") 
			break;
	
		switch (eState)
		{
		case SERVER_CMD_KEY:
			if(strToken == "@setspeed")
			{
				lexer.PopToPeek();
				strToken = lexer.PeekNextToken(NULL, &iLine);
				RwReal fSpeed = (RwReal)atof(strToken.c_str());
				SendUpdateCharSpeed(fSpeed);
				return;
			}
			else if(strToken == "@addmob")
			{
				lexer.PopToPeek();
				strToken = lexer.PeekNextToken(NULL, &iLine);
				RwUInt32 uiMobId = (RwUInt32)atoi(strToken.c_str());
				lexer.PopToPeek();
				strToken = lexer.PeekNextToken(NULL, &iLine);
				RwReal fDist = (RwReal)atof(strToken.c_str());
				lexer.PopToPeek();

				SendMonsterCreate(uiMobId, fDist);
				
				return;
			}
			else if(strToken == "@addmobg")
			{
				lexer.PopToPeek();
				strToken = lexer.PeekNextToken(NULL, &iLine);
				RwUInt32 iNum = (RwUInt32)atoi(strToken.c_str());

				SendMonsterGroupCreate(iNum);
				
				return;
			}
			else if(strToken == "@createitem")
			{
				lexer.PopToPeek();
				strToken = lexer.PeekNextToken(NULL, &iLine);
				RwUInt32 uiTblId = (RwUInt32)atof(strToken.c_str());
				SendAddItem(uiTblId);
				return;
			}
			else if(strToken == "@learnskill")
			{
				lexer.PopToPeek();
				strToken = lexer.PeekNextToken(NULL, &iLine);
				RwUInt32 uiTblId = (RwUInt32)atof(strToken.c_str());
				SendCharLearnSkillRes(uiTblId);
				return;
			}
			else if(strToken == "@learnhtb")
			{
				lexer.PopToPeek();
				strToken = lexer.PeekNextToken(NULL, &iLine);
				RwUInt32 uiTblId = (RwUInt32)atof(strToken.c_str());
				SendCharLearnHTBRes(uiTblId);
				return;
			}
			else if(strToken == "@refreshlp")
			{
				m_iCurrentHp = VIRTUAL_MAX_LP;
				return; 
			}
			else if(strToken == "@setscale")
			{
				lexer.PopToPeek();
				strToken = lexer.PeekNextToken(NULL, &iLine);
				RwReal fScale = (RwReal)atof(strToken.c_str());
				CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(m_uiTargetSerialId);
				if(pSobObj)
				{
					CNtlSobProxy *pSobProxy = pSobObj->GetSobProxy();
					pSobProxy->SetScale(fScale);
				}
				
				return;
			}
			else if(strToken == "@is")
			{
				lexer.PopToPeek();
				strToken = lexer.PeekNextToken(NULL, &iLine);
				CNtlBehaviorProjSteal::m_ffIncSpeed = (RwReal)atof(strToken.c_str());
			}
			else if(strToken == "@iw")
			{
				lexer.PopToPeek();
				strToken = lexer.PeekNextToken(NULL, &iLine);
				CNtlBehaviorProjSteal::m_fWaitCheckTime = (RwReal)atof(strToken.c_str());
			}
			else if(strToken == "@compilelua")
			{
				SLLua_Setup();
				return;
			}
			
			break;
		}

		lexer.PopToPeek();
	}
}

void CDboVirtualServer::SendLoginCreateUserRes(void *pPacket)
{
	sUA_LOGIN_CREATEUSER_REQ *pRecv = (sUA_LOGIN_CREATEUSER_REQ*)pPacket;

	sAU_LOGIN_CREATEUSER_RES sPacket;
	memset(&sPacket, 0, sizeof(sAU_LOGIN_CREATEUSER_RES));

	sPacket.wOpCode = AU_LOGIN_CREATEUSER_RES;
	sPacket.wResultCode = AUTH_SUCCESS;
	memcpy(sPacket.awchUserId, pRecv->awchUserId, NTL_MAX_SIZE_USERID);

	SendEvent(sizeof(sPacket), &sPacket);
}

void CDboVirtualServer::SendLoginRes(void *pPacket)
{
	sUA_LOGIN_REQ *pRecv = (sUA_LOGIN_REQ*)pPacket;

	sAU_LOGIN_RES sPacket;
	memset(&sPacket, 0, sizeof(sAU_LOGIN_RES));

	sPacket.wOpCode = AU_LOGIN_RES;
	sPacket.wResultCode = AUTH_SUCCESS;
	memcpy(sPacket.awchUserId, pRecv->awchUserId, NTL_MAX_SIZE_USERID);
	strcpy_s((char*)sPacket.abyAuthKey, NTL_MAX_SIZE_AUTH_KEY, "Dbo");
	sPacket.accountId = 1;
	sPacket.byServerInfoCount = 1;

	strcpy_s(sPacket.aServerInfo[0].szCharacterServerIP, NTL_MAX_LENGTH_OF_IP, "10.0.0.132");
	sPacket.aServerInfo[0].wCharacterServerPortForClient = 0;
	sPacket.aServerInfo[0].dwLoad = 0;
	
	sPacket.lastServerFarmId = 0;

	SendEvent(sizeof(sPacket), &sPacket);
}

void CDboVirtualServer::SendCharLoginRes(void *pPacket)
{
	sUC_LOGIN_REQ *pRecv = (sUC_LOGIN_REQ*)pPacket;

	sCU_LOGIN_RES sPacket;
	memset(&sPacket, 0, sizeof(sCU_LOGIN_RES));

	sPacket.wOpCode = CU_LOGIN_RES;
	sPacket.wResultCode = CHARACTER_SUCCESS;
	
	SendEvent(sizeof(sPacket), &sPacket);
}

void CDboVirtualServer::SendServerListRes(void *pPacket)
{
	// farm info
	sCU_SERVER_FARM_INFO sPackeFarmInfo;
	sPackeFarmInfo.wOpCode = CU_SERVER_FARM_INFO;
	sPackeFarmInfo.serverFarmInfo.serverFarmId = 0;
	wcscpy_s(sPackeFarmInfo.serverFarmInfo.wszGameServerFarmName, NTL_MAX_SIZE_SERVER_FARM_NAME_UNICODE, L"바보서버");

	SendEvent(sizeof(sPackeFarmInfo), &sPackeFarmInfo);

	// server list res
	sCU_CHARACTER_SERVERLIST_RES sPackeListRes;
	sPackeListRes.wOpCode = CU_CHARACTER_SERVERLIST_RES;
	sPackeListRes.wResultCode = CHARACTER_SUCCESS;

	SendEvent(sizeof(sPackeListRes), &sPackeListRes);
}

void CDboVirtualServer::SendOneServerRes(void *pPacket)
{
	// farm info
	sCU_SERVER_FARM_INFO sPackeFarmInfo;
	sPackeFarmInfo.wOpCode = CU_SERVER_FARM_INFO;
	sPackeFarmInfo.serverFarmInfo.serverFarmId = 0;
	wcscpy_s(sPackeFarmInfo.serverFarmInfo.wszGameServerFarmName, NTL_MAX_SIZE_SERVER_FARM_NAME_UNICODE, L"바보서버");

	SendEvent(sizeof(sPackeFarmInfo), &sPackeFarmInfo);

	// server list res
	sCU_CHARACTER_SERVERLIST_ONE_RES sPackeOneRes;
	sPackeOneRes.wOpCode = CU_CHARACTER_SERVERLIST_ONE_RES;
	sPackeOneRes.wResultCode = CHARACTER_SUCCESS;

	SendEvent(sizeof(sPackeOneRes), &sPackeOneRes);

	// channel 정보.
	sCU_SERVER_CHANNEL_INFO sPacketChannel;
	sPackeOneRes.wOpCode = CU_SERVER_CHANNEL_INFO;
	sPacketChannel.byCount = 1;
	sPacketChannel.serverChannelInfo[0].serverFarmId = 0;
	sPacketChannel.serverChannelInfo[0].byServerChannelIndex = 0;

	SendEvent(sizeof(sPacketChannel), &sPacketChannel);
}

void CDboVirtualServer::SendCharLoadRes(void *pPacket)
{
	sUC_CHARACTER_LOAD_REQ *pRecv = (sUC_CHARACTER_LOAD_REQ*)pPacket;
	
	// character info
	sCU_CHARACTER_INFO sPacketCharInfo;
	memset(&sPacketCharInfo, 0, sizeof(sCU_CHARACTER_INFO));

	sPacketCharInfo.wOpCode = CU_CHARACTER_INFO;
	sPacketCharInfo.byCount = 0;

	SendEvent(sizeof(sPacketCharInfo), &sPacketCharInfo);
	
	// character info
	sCU_CHARACTER_LOAD_RES sPacketLoadRes;
	memset(&sPacketLoadRes, 0, sizeof(sCU_CHARACTER_LOAD_RES));
	sPacketLoadRes.wOpCode = CU_CHARACTER_LOAD_RES;

	SendEvent(sizeof(sPacketLoadRes), &sPacketLoadRes);

	// 
}

void CDboVirtualServer::SendCharAddRes(void *pPacket)
{
	static RwUInt32 uiCharId = 0;
	uiCharId++;

	sUC_CHARACTER_ADD_REQ *pRecv = (sUC_CHARACTER_ADD_REQ*)pPacket;

	AddPcData(uiCharId, pRecv->awchCharName, pRecv->byRace, pRecv->byClass, pRecv->byGender, pRecv->byFace, 
				pRecv->byHair, pRecv->byHairColor, pRecv->bySkinColor, pRecv->byBlood);
		
	sCU_CHARACTER_ADD_RES sPacket;
	memset(&sPacket, 0, sizeof(sCU_CHARACTER_ADD_RES));

	sPacket.wOpCode = CU_CHARACTER_ADD_RES;
	sPacket.wResultCode = CHARACTER_SUCCESS;
	sPacket.sPcDataSummary.charId = uiCharId;
	memcpy(sPacket.sPcDataSummary.awchName, pRecv->awchCharName, sizeof(wchar_t)* NTL_MAX_SIZE_CHAR_NAME);
	sPacket.sPcDataSummary.byRace = pRecv->byRace;
	sPacket.sPcDataSummary.byClass = pRecv->byClass;
	sPacket.sPcDataSummary.byGender = pRecv->byGender;
	sPacket.sPcDataSummary.byFace = pRecv->byFace;
	sPacket.sPcDataSummary.byHair = pRecv->byHair;
	sPacket.sPcDataSummary.byHairColor = pRecv->byHairColor;
	sPacket.sPcDataSummary.bySkinColor = pRecv->bySkinColor;
	sPacket.sPcDataSummary.byLevel = 1;
	sPacket.sPcDataSummary.worldId = 1;
	sPacket.sPcDataSummary.fPositionX = 0.0f;
	sPacket.sPcDataSummary.fPositionY = 0.0f;
	sPacket.sPcDataSummary.fPositionZ = 0.0f;
	sPacket.sPcDataSummary.dwMoney = 100;
	sPacket.sPcDataSummary.dwMoneyBank = 100;

	for(RwInt32 i = 0; i < EQUIP_SLOT_TYPE_COUNT; i++)
	{
		sPacket.sPcDataSummary.sItem[i].tblidx = INVALID_TBLIDX;
	}


	SendEvent(sizeof(sPacket), &sPacket);
}

void CDboVirtualServer::SendCharDelRes(void *pPacket)
{
	sUC_CHARACTER_DEL_REQ *pRecv = (sUC_CHARACTER_DEL_REQ*)pPacket;

	RemovePcData(pRecv->charId);
	
	sCU_CHARACTER_DEL_RES sPacket;
	memset(&sPacket, 0, sizeof(sCU_CHARACTER_DEL_RES));

	sPacket.wOpCode = CU_CHARACTER_DEL_RES;
	sPacket.wResultCode = CHARACTER_SUCCESS;
	sPacket.charId = pRecv->charId;

	SendEvent(sizeof(sPacket), &sPacket);
}

void CDboVirtualServer::SendCharSelectRes(void *pPacket)
{
	sUC_CHARACTER_SELECT_REQ *pRecv = (sUC_CHARACTER_SELECT_REQ*)pPacket;

	m_bySelCharId = pRecv->charId;

	sCU_CHARACTER_SELECT_RES sPacket;
	memset(&sPacket, 0, sizeof(sCU_CHARACTER_SELECT_RES));

	sPacket.wOpCode = CU_CHARACTER_SELECT_RES;
	sPacket.wResultCode = CHARACTER_SUCCESS;
	sPacket.charId = pRecv->charId; 
	strcpy_s((char*)sPacket.abyAuthKey, NTL_MAX_SIZE_AUTH_KEY, "Dbo");
	strcpy_s(sPacket.szGameServerIP, sizeof(sPacket.szGameServerIP), "10.0.0.132");
	sPacket.wGameServerPortForClient = 10000;

	SendEvent(sizeof(sPacket), &sPacket);
}

void CDboVirtualServer::SendGameEnterRes(void *pPacket)
{
	sUG_GAME_ENTER_REQ *pRecv = (sUG_GAME_ENTER_REQ*)pPacket;

	sGU_GAME_ENTER_RES sPacket;
	memset(&sPacket, 0, sizeof(sGU_GAME_ENTER_RES));

	sPacket.wOpCode = GU_GAME_ENTER_RES;
	sPacket.wResultCode = GAME_SUCCESS;

	SendEvent(sizeof(sPacket), &sPacket);
}

void CDboVirtualServer::SendAvatarCharInfo(void)
{
	m_uiAvatarSerialId = AcquireSerialId();

	sGU_AVATAR_CHAR_INFO sPacket;
	memset(&sPacket, 0, sizeof(sGU_AVATAR_CHAR_INFO));

	SVirtualPcData *pPcData = FindPcData(m_bySelCharId);
	CPCTable *pPcTable = API_GetTableContainer()->GetPcTable();
	sPC_TBLDAT *pTblData = (sPC_TBLDAT*)pPcTable->GetPcTbldat(pPcData->byRace, pPcData->byClass, pPcData->byGender);   

	sPacket.wOpCode = GU_AVATAR_CHAR_INFO;
	sPacket.handle = m_uiAvatarSerialId;
	sPacket.sPcProfile.tblidx = pTblData->tblidx;
	memcpy(sPacket.sPcProfile.awchName, pPcData->wchName, sizeof(WCHAR)*NTL_MAX_SIZE_CHAR_NAME); 
	sPacket.sPcProfile.sPcShape.byFace = pPcData->byFace; 
	sPacket.sPcProfile.sPcShape.byHair = pPcData->byHair; 
	sPacket.sPcProfile.sPcShape.byHairColor = pPcData->byHairColor; 
	sPacket.sPcProfile.sPcShape.bySkinColor	= pPcData->bySkinColor;

	sPacket.sPcProfile.avatarAttribute.byBaseStr = 1;
	sPacket.sPcProfile.avatarAttribute.byLastStr = 1;
	sPacket.sPcProfile.avatarAttribute.byBaseCon = 1;
	sPacket.sPcProfile.avatarAttribute.byLastCon = 1;
	sPacket.sPcProfile.avatarAttribute.byBaseFoc = 1;
	sPacket.sPcProfile.avatarAttribute.byLastFoc = 1;
	sPacket.sPcProfile.avatarAttribute.byBaseDex = 1;
	sPacket.sPcProfile.avatarAttribute.byLastDex = 1;
	sPacket.sPcProfile.avatarAttribute.byBaseSol = 1;
	sPacket.sPcProfile.avatarAttribute.byLastSol = 1;
	sPacket.sPcProfile.avatarAttribute.byBaseEng = 1;
	sPacket.sPcProfile.avatarAttribute.byLastEng = 1;

	sPacket.sPcProfile.wCurLP = VIRTUAL_CURR_LP;
	sPacket.sPcProfile.avatarAttribute.wBaseMaxLP = VIRTUAL_MAX_LP;
	sPacket.sPcProfile.avatarAttribute.wLastMaxLP = VIRTUAL_MAX_LP;
	sPacket.sPcProfile.wCurEP = VIRTUAL_CURR_EP;
	sPacket.sPcProfile.avatarAttribute.wBaseMaxEP = VIRTUAL_MAX_EP;
	sPacket.sPcProfile.avatarAttribute.wLastMaxEP = VIRTUAL_MAX_EP;
	sPacket.sPcProfile.wCurRP = VIRTUAL_CURR_RP;
	sPacket.sPcProfile.avatarAttribute.wBaseMaxRP = VIRTUAL_MAX_RP;
	sPacket.sPcProfile.avatarAttribute.wLastMaxRP = VIRTUAL_MAX_RP;

	sPacket.sPcProfile.avatarAttribute.wBasePhysicalOffence = VIRTUAL_PHISHIC_OFFENCE;
	sPacket.sPcProfile.avatarAttribute.wLastPhysicalOffence = VIRTUAL_PHISHIC_OFFENCE;
	sPacket.sPcProfile.avatarAttribute.wBasePhysicalDefence = VIRTUAL_PHISHIC_DEFENCE;
	sPacket.sPcProfile.avatarAttribute.wLastPhysicalDefence = VIRTUAL_PHISHIC_DEFENCE;
	sPacket.sPcProfile.avatarAttribute.wBaseEnergyOffence = VIRTUAL_ENERGY_OFFENCE;
	sPacket.sPcProfile.avatarAttribute.wLastEnergyOffence = VIRTUAL_ENERGY_OFFENCE;
	sPacket.sPcProfile.avatarAttribute.wBaseEnergyDefence = VIRTUAL_ENERGY_DEFENCE;
	sPacket.sPcProfile.avatarAttribute.wLastEnergyDefence = VIRTUAL_ENERGY_DEFENCE;
		
	sPacket.sPcProfile.avatarAttribute.wBaseAttackRate = VIRTUAL_ATTACK_RATE;
	sPacket.sPcProfile.avatarAttribute.wLastAttackRate = VIRTUAL_ATTACK_RATE;
	sPacket.sPcProfile.avatarAttribute.wBaseDodgeRate = VIRTUAL_DODGE_RATE;
	sPacket.sPcProfile.avatarAttribute.wLastDodgeRate = VIRTUAL_DODGE_RATE;
	sPacket.sPcProfile.avatarAttribute.wBaseBlockRate = VIRTUAL_BLOCK_RATE;
	sPacket.sPcProfile.avatarAttribute.wLastBlockRate = VIRTUAL_BLOCK_RATE;
	sPacket.sPcProfile.avatarAttribute.wLastPhysicalCriticalRate = VIRTUAL_CRITIAL_RATE;
	sPacket.sPcProfile.avatarAttribute.wLastEnergyCriticalRate = VIRTUAL_CRITIAL_RATE;
		
	sPacket.sPcProfile.avatarAttribute.fLastRunSpeed = VIRTUAL_AVATAR_SPEED;
	sPacket.sPcProfile.byLevel = VIRTUAL_AVATAR_LEVEL;
	sPacket.sPcProfile.dwCurExp = VIRTUAL_AVATAR_CURR_EXP;

	sPacket.sCharState.sCharStateBase.vCurLoc.x = VIRTUAL_AVATAR_POSX;
	sPacket.sCharState.sCharStateBase.vCurLoc.y = 0.0f;
	sPacket.sCharState.sCharStateBase.vCurLoc.z = VIRTUAL_AVATAR_POSZ;

	sPacket.sCharState.sCharStateBase.vCurDir.x = 0.0f;  
	sPacket.sCharState.sCharStateBase.vCurDir.y = 0.0f;  
	sPacket.sCharState.sCharStateBase.vCurDir.z = 1.0f;  

	sPacket.wCharStateSize = sizeof(sCHARSTATE_BASE);

	SendEvent(sizeof(sPacket), &sPacket);
}

void CDboVirtualServer::SendAvatarItemInfo(void)
{
	sGU_AVATAR_ITEM_INFO sPacket;
	memset(&sPacket, 0, sizeof(sGU_AVATAR_ITEM_INFO));

	sPacket.wOpCode = GU_AVATAR_ITEM_INFO;
	sPacket.byItemCount = 0;

	sPacket.aItemProfile[0].handle = AcquireSerialId();
	sPacket.aItemProfile[0].tblidx = 9901;
	sPacket.aItemProfile[0].byPlace = CONTAINER_TYPE_BAGSLOT;
	sPacket.aItemProfile[0].byPos = 0;
	sPacket.aItemProfile[0].byStackcount = 0;
	sPacket.aItemProfile[0].byRank = 1;
	sPacket.aItemProfile[0].byCurDur = 10;

	sPacket.aItemProfile[1].handle = AcquireSerialId();
	sPacket.aItemProfile[1].tblidx = 3001;
	sPacket.aItemProfile[1].byPlace = CONTAINER_TYPE_BAG1;
	sPacket.aItemProfile[1].byPos = 0;
	sPacket.aItemProfile[1].byStackcount = 0;
	sPacket.aItemProfile[1].byRank = 1;
	sPacket.aItemProfile[1].byCurDur = 10;

	sPacket.aItemProfile[2].handle = AcquireSerialId();
	sPacket.aItemProfile[2].tblidx = 8001;
	sPacket.aItemProfile[2].byPlace = CONTAINER_TYPE_BAG1;
	sPacket.aItemProfile[2].byPos = 1;
	sPacket.aItemProfile[2].byStackcount = 20;
	sPacket.aItemProfile[2].byRank = 1;
	sPacket.aItemProfile[2].byCurDur = 10;

	sPacket.aItemProfile[3].handle = AcquireSerialId();
	sPacket.aItemProfile[3].tblidx = 9901;
	sPacket.aItemProfile[3].byPlace = CONTAINER_TYPE_BAG1;
	sPacket.aItemProfile[3].byPos = 2;
	sPacket.aItemProfile[3].byStackcount = 0;
	sPacket.aItemProfile[3].byRank = 1;
	sPacket.aItemProfile[3].byCurDur = 10;

	RwInt32 iDataLen = sizeof(sNTLPACKETHEADER);
	iDataLen += sizeof(BYTE);
	iDataLen += sizeof(sITEM_PROFILE)*sPacket.byItemCount;

	SendEvent(iDataLen, &sPacket);
}

void CDboVirtualServer::SendAvatarSkillInfo(void)
{
	sGU_AVATAR_SKILL_INFO sPacket;
	memset(&sPacket, 0, sizeof(sGU_AVATAR_SKILL_INFO));


	sPacket.wOpCode = GU_AVATAR_SKILL_INFO;
	sPacket.bySkillCount = 3;

	sPacket.aSkillInfo[0].tblidx = 1;
	sPacket.aSkillInfo[0].bySlotId = 0;
	sPacket.aSkillInfo[0].nExp = 0;
	sPacket.aSkillInfo[0].dwTimeRemaining = 0;

	sPacket.aSkillInfo[1].tblidx = 2;
	sPacket.aSkillInfo[1].bySlotId = 1;
	sPacket.aSkillInfo[1].nExp = 0;
	sPacket.aSkillInfo[1].dwTimeRemaining = 0;

	sPacket.aSkillInfo[2].tblidx = 3;
	sPacket.aSkillInfo[2].bySlotId = 2;
	sPacket.aSkillInfo[2].nExp = 0;
	sPacket.aSkillInfo[2].dwTimeRemaining = 0;

	sPacket.aSkillInfo[3].tblidx = 10;
	sPacket.aSkillInfo[3].bySlotId = 3;
	sPacket.aSkillInfo[3].nExp = 0;
	sPacket.aSkillInfo[3].dwTimeRemaining = 0;

	sPacket.aSkillInfo[4].tblidx = 20;
	sPacket.aSkillInfo[4].bySlotId = 4;
	sPacket.aSkillInfo[4].nExp = 0;
	sPacket.aSkillInfo[4].dwTimeRemaining = 0;

	sPacket.aSkillInfo[5].tblidx = 30;
	sPacket.aSkillInfo[5].bySlotId = 5;
	sPacket.aSkillInfo[5].nExp = 0;
	sPacket.aSkillInfo[5].dwTimeRemaining = 0;

	RwInt32 iDataLen = sizeof(sNTLPACKETHEADER);
	iDataLen += sizeof(BYTE);
	iDataLen += sizeof(sITEM_PROFILE)*sPacket.bySkillCount;

	SendEvent(iDataLen, &sPacket);
}

void CDboVirtualServer::SendAvatarInfoEnd(void)
{
	sGU_AVATAR_INFO_END sPacket;
	memset(&sPacket, 0, sizeof(sGU_AVATAR_INFO_END));

	sPacket.wOpCode = GU_AVATAR_INFO_END;
	
	SendEvent(sizeof(sPacket), &sPacket);
}

void CDboVirtualServer::SendAvatarWorldInfo(void)
{
	sGU_AVATAR_WORLD_INFO sPacket;
	memset(&sPacket, 0, sizeof(sGU_AVATAR_WORLD_INFO));

	sPacket.wOpCode = GU_AVATAR_WORLD_INFO;
	sPacket.worldInfo.tblidx = 1;
	sPacket.worldInfo.worldID = 1;
	sPacket.worldInfo.hTriggerObjectOffset = 100000;

	sPacket.worldInfo.sRuleInfo.byRuleType = GAMERULE_NORMAL;

	sPacket.vCurLoc.x = VIRTUAL_AVATAR_POSX + (RwReal)(rand()%5);
	sPacket.vCurLoc.y = 0.0f;
	sPacket.vCurLoc.z = VIRTUAL_AVATAR_POSZ + (RwReal)(rand()%5);

	sPacket.vCurDir.x = 0.0f;  
	sPacket.vCurDir.y = 0.0f;  
	sPacket.vCurDir.z = 1.0f;  
	
	SendEvent(sizeof(sPacket), &sPacket);
}

void CDboVirtualServer::SendPlayerCreate(void)
{
	sGU_OBJECT_CREATE sPacket;

	memset(&sPacket, 0, sizeof(sGU_OBJECT_CREATE));

	sPacket.wOpCode = GU_OBJECT_CREATE;
	sPacket.handle = AcquireSerialId();
	sPacket.sObjectInfo.objType = OBJTYPE_PC;
	sPacket.sObjectInfo.pcBrief.byLevel = 1;
	sPacket.sObjectInfo.pcBrief.tblidx = 1;
	wcscpy_s(sPacket.sObjectInfo.pcBrief.awchName, NTL_MAX_SIZE_CHAR_NAME, L"하하하");
	sPacket.sObjectInfo.pcBrief.wCurLP = 100;
	sPacket.sObjectInfo.pcBrief.wMaxLP = 100;
	sPacket.sObjectInfo.pcBrief.fSpeed = 7.0f;
	sPacket.sObjectInfo.pcBrief.sPcShape.byFace = 1;
	sPacket.sObjectInfo.pcBrief.sPcShape.byHair = 1;
	sPacket.sObjectInfo.pcBrief.sPcShape.byHairColor = 1;
	sPacket.sObjectInfo.pcBrief.sPcShape.bySkinColor = 1;

	sPacket.sObjectInfo.pcState.sCharStateBase.vCurLoc.x = 4387.0f + (RwReal)(rand()%50);
	sPacket.sObjectInfo.pcState.sCharStateBase.vCurLoc.y = 0.0f;
	sPacket.sObjectInfo.pcState.sCharStateBase.vCurLoc.z = 4613.0f + (RwReal)(rand()%50);

	sPacket.sObjectInfo.pcState.sCharStateBase.vCurDir.x = 0.0f;  
	sPacket.sObjectInfo.pcState.sCharStateBase.vCurDir.y = 0.0f;  
	sPacket.sObjectInfo.pcState.sCharStateBase.vCurDir.z = 1.0f;  

	SendEvent(sizeof(sPacket), &sPacket);
}

void CDboVirtualServer::SendMonsterCreate(void)
{
	sGU_OBJECT_CREATE sPacket;

	memset(&sPacket, 0, sizeof(sGU_OBJECT_CREATE));

	sPacket.wOpCode = GU_OBJECT_CREATE;
	sPacket.handle = AcquireSerialId();
	sPacket.sObjectInfo.objType = OBJTYPE_MOB;
	sPacket.sObjectInfo.mobBrief.tblidx = 4311111;
	sPacket.sObjectInfo.mobBrief.wCurLP = VIRTUAL_MAX_LP;
	sPacket.sObjectInfo.mobBrief.wMaxLP = VIRTUAL_MAX_LP;
	sPacket.sObjectInfo.mobBrief.fLastWalkingSpeed = 3.0f;
	sPacket.sObjectInfo.mobBrief.fLastRunningSpeed = 7.0f;

	sPacket.sObjectInfo.mobState.sCharStateBase.byStateID = CHARSTATE_SPAWNING;
	sPacket.sObjectInfo.mobState.sCharStateBase.bFightMode = FALSE;
	
	sPacket.sObjectInfo.mobState.sCharStateBase.vCurLoc.x = VIRTUAL_AVATAR_POSX + (RwReal)(rand()%5);
	sPacket.sObjectInfo.mobState.sCharStateBase.vCurLoc.y = 0.0f;
	sPacket.sObjectInfo.mobState.sCharStateBase.vCurLoc.z = VIRTUAL_AVATAR_POSZ + (RwReal)(rand()%5);

	sPacket.sObjectInfo.mobState.sCharStateBase.vCurDir.x = 0.0f;  
	sPacket.sObjectInfo.mobState.sCharStateBase.vCurDir.y = 0.0f;  
	sPacket.sObjectInfo.mobState.sCharStateBase.vCurDir.z = 1.0f;  

	sPacket.sObjectInfo.mobState.sCharStateDetail.sCharStateDestMove.vDestLoc.x = sPacket.sObjectInfo.mobState.sCharStateBase.vCurLoc.x + 100;
	sPacket.sObjectInfo.mobState.sCharStateDetail.sCharStateDestMove.vDestLoc.y = sPacket.sObjectInfo.mobState.sCharStateBase.vCurLoc.y;
	sPacket.sObjectInfo.mobState.sCharStateDetail.sCharStateDestMove.vDestLoc.z = sPacket.sObjectInfo.mobState.sCharStateBase.vCurLoc.z;

	SendEvent(sizeof(sPacket), &sPacket);
}

void CDboVirtualServer::SendMonsterGroupCreate(RwUInt32 iNum)
{
	RwUInt32 uiCurrTime = GetTickCount();

	RwV3d vLoc, vDir;

	if(iNum <= 0)
		return;

	RwUInt32 arrTblId[10] = {1411101, 1511101, 2612110, 1711101, 1811101, 2131106, 2131115, 2611105, 3611115, 4031108};

	for(RwInt32 i = 0; i < iNum; i++)
	{
		sGU_OBJECT_CREATE sPacket;

		memset(&sPacket, 0, sizeof(sGU_OBJECT_CREATE));

		sPacket.wOpCode = GU_OBJECT_CREATE;
		sPacket.handle = AcquireSerialId();
		sPacket.sObjectInfo.objType = OBJTYPE_MOB;
		sPacket.sObjectInfo.mobBrief.tblidx = arrTblId[i%10];
		sPacket.sObjectInfo.mobBrief.wCurLP = VIRTUAL_MAX_LP;
		sPacket.sObjectInfo.mobBrief.wMaxLP = VIRTUAL_MAX_LP;
		sPacket.sObjectInfo.mobBrief.fLastWalkingSpeed = 3.0f;
		sPacket.sObjectInfo.mobBrief.fLastRunningSpeed = 7.0f;

		sPacket.sObjectInfo.mobState.sCharStateBase.byStateID = CHARSTATE_SPAWNING;
		sPacket.sObjectInfo.mobState.sCharStateBase.bFightMode = FALSE;
		
		sPacket.sObjectInfo.mobState.sCharStateBase.vCurLoc.x = VIRTUAL_AVATAR_POSX + (RwReal)(rand()%5);
		sPacket.sObjectInfo.mobState.sCharStateBase.vCurLoc.y = 0.0f;
		sPacket.sObjectInfo.mobState.sCharStateBase.vCurLoc.z = VIRTUAL_AVATAR_POSZ + (RwReal)(rand()%5);

		sPacket.sObjectInfo.mobState.sCharStateBase.vCurDir.x = 0.0f;  
		sPacket.sObjectInfo.mobState.sCharStateBase.vCurDir.y = 0.0f;  
		sPacket.sObjectInfo.mobState.sCharStateBase.vCurDir.z = 1.0f;  

		sPacket.sObjectInfo.mobState.sCharStateDetail.sCharStateDestMove.vDestLoc.x = sPacket.sObjectInfo.mobState.sCharStateBase.vCurLoc.x + 100;
		sPacket.sObjectInfo.mobState.sCharStateDetail.sCharStateDestMove.vDestLoc.y = sPacket.sObjectInfo.mobState.sCharStateBase.vCurLoc.y;
		sPacket.sObjectInfo.mobState.sCharStateDetail.sCharStateDestMove.vDestLoc.z = sPacket.sObjectInfo.mobState.sCharStateBase.vCurLoc.z;

		// SendEvent(sizeof(sPacket), &sPacket);


		CNtlMath::MathRwV3dAssign(&vLoc, 
			sPacket.sObjectInfo.mobState.sCharStateBase.vCurLoc.x,
			sPacket.sObjectInfo.mobState.sCharStateBase.vCurLoc.y,
			sPacket.sObjectInfo.mobState.sCharStateBase.vCurLoc.z);
		CNtlMath::MathRwV3dAssign(&vDir, 
			sPacket.sObjectInfo.mobState.sCharStateBase.vCurDir.x,
			sPacket.sObjectInfo.mobState.sCharStateBase.vCurDir.y,
			sPacket.sObjectInfo.mobState.sCharStateBase.vCurDir.z);

		sMOB_BRIEF *pMobBrief = &(sPacket.sObjectInfo.mobBrief);
		sCHARSTATE *pCharState = &(sPacket.sObjectInfo.mobState);

		CNtlSLEventGenerator::SobMonsterCreate(SLCLASS_MONSTER, sPacket.handle, vLoc, vDir, pMobBrief, pCharState);
	}
	
	RwUInt32 uiDeltaTime = GetTickCount() - uiCurrTime;
	NtlLogFilePrintArgs("character create time (%u) \n", uiDeltaTime);
}

void CDboVirtualServer::SendMonsterCreate(RwUInt32 uiTblId, RwReal fDist)
{
	sGU_OBJECT_CREATE sPacket;

	memset(&sPacket, 0, sizeof(sGU_OBJECT_CREATE));

	sPacket.wOpCode = GU_OBJECT_CREATE;
	sPacket.handle = AcquireSerialId();
	sPacket.sObjectInfo.objType = OBJTYPE_MOB;
	sPacket.sObjectInfo.mobBrief.tblidx = uiTblId;
	sPacket.sObjectInfo.mobBrief.wCurLP = VIRTUAL_MAX_LP;
	sPacket.sObjectInfo.mobBrief.wMaxLP = VIRTUAL_MAX_LP;
	sPacket.sObjectInfo.mobBrief.fLastWalkingSpeed = 3.0f;
	sPacket.sObjectInfo.mobBrief.fLastRunningSpeed = 7.0f;

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	RwV3d vLoc = pSobAvatar->GetPosition();
	RwV3d vDir = pSobAvatar->GetDirection();
	
	sPacket.sObjectInfo.mobState.sCharStateBase.byStateID = CHARSTATE_SPAWNING;
	sPacket.sObjectInfo.mobState.sCharStateBase.bFightMode = FALSE;

	sPacket.sObjectInfo.mobState.sCharStateBase.vCurLoc.x = vLoc.x + vDir.x*fDist;
	sPacket.sObjectInfo.mobState.sCharStateBase.vCurLoc.y = vLoc.y + vDir.y*fDist;
	sPacket.sObjectInfo.mobState.sCharStateBase.vCurLoc.z = vLoc.z + vDir.z*fDist;

	sPacket.sObjectInfo.mobState.sCharStateBase.vCurDir.x = 0.0f;  
	sPacket.sObjectInfo.mobState.sCharStateBase.vCurDir.y = 0.0f;  
	sPacket.sObjectInfo.mobState.sCharStateBase.vCurDir.z = 1.0f;  

	if(sPacket.sObjectInfo.mobState.sCharStateBase.byStateID == CHARSTATE_DESTMOVE)
	{
		sPacket.sObjectInfo.mobState.sCharStateDetail.sCharStateDestMove.vDestLoc.x = sPacket.sObjectInfo.mobState.sCharStateBase.vCurLoc.x + 100;
		sPacket.sObjectInfo.mobState.sCharStateDetail.sCharStateDestMove.vDestLoc.y = sPacket.sObjectInfo.mobState.sCharStateBase.vCurLoc.y;
		sPacket.sObjectInfo.mobState.sCharStateDetail.sCharStateDestMove.vDestLoc.z = sPacket.sObjectInfo.mobState.sCharStateBase.vCurLoc.z;
	}

	SendEvent(sizeof(sPacket), &sPacket);
}

void CDboVirtualServer::SendEnterWorldComplete(void)
{
	sGU_ENTER_WORLD_COMPLETE sPacket;
	memset(&sPacket, 0, sizeof(sGU_ENTER_WORLD_COMPLETE));

	sPacket.wOpCode = GU_ENTER_WORLD_COMPLETE;

	SendEvent(sizeof(sPacket), &sPacket);
}

void CDboVirtualServer::SendItemMoveRes(void *pPacket)
{
	sUG_ITEM_MOVE_REQ *pRecv = (sUG_ITEM_MOVE_REQ*)pPacket;

	sGU_ITEM_MOVE_RES sPacket;
	memset(&sPacket, 0, sizeof(sGU_ITEM_MOVE_RES));

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar(); 
	CNtlInventory *pInventory = pSobAvatar->GetInventory();
	
	RwUInt32 hSrcItem = INVALID_SERIAL_ID;
	RwUInt32 hDestItem = INVALID_SERIAL_ID;

	if(pRecv->bySrcPlace == CONTAINER_TYPE_BAGSLOT)
	{
		hSrcItem = pInventory->GetBagItem(pRecv->bySrcPos); 
	}
	else if(pRecv->bySrcPlace == CONTAINER_TYPE_BAG1)
	{
		SERIAL_HANDLE hSerial = pInventory->GetBagItem(0);
		CNtlSobItem *pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(hSerial) ); 
		hSrcItem = pSobItem->GetChildSerial(pRecv->bySrcPos);
	}
	else if(pRecv->bySrcPlace == CONTAINER_TYPE_BAG2)
	{
		SERIAL_HANDLE hSerial = pInventory->GetBagItem(1);
		CNtlSobItem *pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(hSerial) ); 
		hSrcItem = pSobItem->GetChildSerial(pRecv->bySrcPos);
	}
	else if(pRecv->bySrcPlace == CONTAINER_TYPE_BAG3)
	{
		SERIAL_HANDLE hSerial = pInventory->GetBagItem(2);
		CNtlSobItem *pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(hSerial) ); 
		hSrcItem = pSobItem->GetChildSerial(pRecv->bySrcPos);
	}
	else if(pRecv->bySrcPlace == CONTAINER_TYPE_BAG4)
	{
		SERIAL_HANDLE hSerial = pInventory->GetBagItem(3);
		CNtlSobItem *pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(hSerial) ); 
		hSrcItem = pSobItem->GetChildSerial(pRecv->bySrcPos);
	}
	else if(pRecv->bySrcPlace == CONTAINER_TYPE_BAG5)
	{
		SERIAL_HANDLE hSerial = pInventory->GetBagItem(4);
		CNtlSobItem *pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(hSerial) ); 
		hSrcItem = pSobItem->GetChildSerial(pRecv->bySrcPos);
	}
	else if(pRecv->bySrcPlace == CONTAINER_TYPE_EQUIP)
	{
		hSrcItem = pInventory->GetEquipItem(pRecv->bySrcPos);
	}
	else if(pRecv->bySrcPlace == CONTAINER_TYPE_SCOUT)
	{
	}

	if(pRecv->byDestPlace == CONTAINER_TYPE_BAGSLOT)
	{
		hDestItem = pInventory->GetBagItem(pRecv->byDestPos); 
	}
	else if(pRecv->byDestPlace == CONTAINER_TYPE_BAG1)
	{
		SERIAL_HANDLE hSerial = pInventory->GetBagItem(0);
		CNtlSobItem *pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(hSerial) ); 
		hDestItem = pSobItem->GetChildSerial(pRecv->byDestPos);
	}
	else if(pRecv->byDestPlace == CONTAINER_TYPE_BAG2)
	{
		SERIAL_HANDLE hSerial = pInventory->GetBagItem(1);
		CNtlSobItem *pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(hSerial) ); 
		hDestItem = pSobItem->GetChildSerial(pRecv->byDestPos);
	}
	else if(pRecv->byDestPlace == CONTAINER_TYPE_BAG3)
	{
		SERIAL_HANDLE hSerial = pInventory->GetBagItem(2);
		CNtlSobItem *pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(hSerial) ); 
		hDestItem = pSobItem->GetChildSerial(pRecv->byDestPos);
	}
	else if(pRecv->byDestPlace == CONTAINER_TYPE_BAG4)
	{
		SERIAL_HANDLE hSerial = pInventory->GetBagItem(3);
		CNtlSobItem *pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(hSerial) ); 
		hDestItem = pSobItem->GetChildSerial(pRecv->byDestPos);
	}
	else if(pRecv->byDestPlace == CONTAINER_TYPE_BAG5)
	{
		SERIAL_HANDLE hSerial = pInventory->GetBagItem(4);
		CNtlSobItem *pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(hSerial) ); 
		hDestItem = pSobItem->GetChildSerial(pRecv->byDestPos);
	}
	else if(pRecv->byDestPlace == CONTAINER_TYPE_EQUIP)
	{
		hDestItem = pInventory->GetEquipItem(pRecv->byDestPos);
	}
	else if(pRecv->byDestPlace == CONTAINER_TYPE_SCOUT)
	{
	}

	sPacket.wOpCode = GU_ITEM_MOVE_RES;
	sPacket.wResultCode = GAME_SUCCESS;
	sPacket.hSrcItem = hSrcItem;
	sPacket.bySrcPlace = pRecv->bySrcPlace;
	sPacket.bySrcPos = pRecv->bySrcPos; 
	sPacket.hDestItem = hDestItem;
	sPacket.byDestPlace = pRecv->byDestPlace; 
	sPacket.byDestPos = pRecv->byDestPos; 

	SendEvent(sizeof(sPacket), &sPacket);
}


void CDboVirtualServer::SendCharUpdateFaintingState(void)
{
	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(m_uiTargetSerialId);
	RwV3d vLoc = pSobObj->GetPosition();
	RwV3d vDir = pSobObj->GetDirection();

	sGU_UPDATE_CHAR_STATE sPacket;
	memset(&sPacket, 0, sizeof(sGU_UPDATE_CHAR_STATE));

	sPacket.wOpCode = GU_UPDATE_CHAR_STATE;
	sPacket.handle = m_uiTargetSerialId;
	sPacket.sCharState.sCharStateBase.byStateID = CHARSTATE_FAINTING;
	sPacket.sCharState.sCharStateBase.vCurLoc.x = vLoc.x;
	sPacket.sCharState.sCharStateBase.vCurLoc.y = vLoc.y;
	sPacket.sCharState.sCharStateBase.vCurLoc.z = vLoc.z;
	sPacket.sCharState.sCharStateBase.vCurDir.x = vDir.x;
	sPacket.sCharState.sCharStateBase.vCurDir.y = vDir.y;
	sPacket.sCharState.sCharStateBase.vCurDir.z = vDir.z;

	SendEvent(sizeof(sPacket), &sPacket);
}


void CDboVirtualServer::SendCharUpdateHTBState(void)
{
	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	RwV3d vLoc = pSobAvatar->GetPosition();
	RwV3d vDir = pSobAvatar->GetDirection();

	sGU_UPDATE_CHAR_STATE sPacket;
	memset(&sPacket, 0, sizeof(sGU_UPDATE_CHAR_STATE));

	sPacket.wOpCode = GU_UPDATE_CHAR_STATE;
	sPacket.handle = pSobAvatar->GetSerialID();
	sPacket.sCharState.sCharStateBase.byStateID = CHARSTATE_HTB;
	sPacket.sCharState.sCharStateBase.vCurLoc.x = vLoc.x;
	sPacket.sCharState.sCharStateBase.vCurLoc.y = vLoc.y;
	sPacket.sCharState.sCharStateBase.vCurLoc.z = vLoc.z;
	sPacket.sCharState.sCharStateBase.vCurDir.x = vDir.x;
	sPacket.sCharState.sCharStateBase.vCurDir.y = vDir.y;
	sPacket.sCharState.sCharStateBase.vCurDir.z = vDir.z;

	CHTBSetTable *pHTBSetTbl = API_GetTableContainer()->GetHTBSetTable();
	sHTB_SET_TBLDAT *pHTBSetTblData = reinterpret_cast<sHTB_SET_TBLDAT*>( pHTBSetTbl->FindData(m_uiSkillTblId) );
	
	sPacket.sCharState.sCharStateDetail.sCharStateHTB.hTarget = m_uiTargetSerialId;
	sPacket.sCharState.sCharStateDetail.sCharStateHTB.byStepCount = pHTBSetTblData->bySetCount;
	sPacket.sCharState.sCharStateDetail.sCharStateHTB.byCurStep = 0;
	sPacket.sCharState.sCharStateDetail.sCharStateHTB.byResultCount = 0;
	sPacket.sCharState.sCharStateDetail.sCharStateHTB.HTBId = pHTBSetTblData->tblidx;

	RwInt8 byResultCount = 0;
	for(RwInt32 i = 0; i < pHTBSetTblData->bySetCount; ++i)
	{
		if(pHTBSetTblData->aHTBAction[i].skillTblidx != INVALID_TBLIDX)
		{
			sPacket.sCharState.sCharStateDetail.sCharStateHTB.aHTBSkillResult[byResultCount].byStep = i;
			sPacket.sCharState.sCharStateDetail.sCharStateHTB.aHTBSkillResult[byResultCount].sSkillResult.byAttackResult = BATTLE_ATTACK_RESULT_HIT;
			sPacket.sCharState.sCharStateDetail.sCharStateHTB.aHTBSkillResult[byResultCount].sSkillResult.effectResult1.fResultValue = 100;
			sPacket.sCharState.sCharStateDetail.sCharStateHTB.aHTBSkillResult[byResultCount].sSkillResult.effectResult2.fResultValue = 100;
			sPacket.sCharState.sCharStateDetail.sCharStateHTB.aHTBSkillResult[byResultCount].sSkillResult.vShift.x = 0.0f;
			sPacket.sCharState.sCharStateDetail.sCharStateHTB.aHTBSkillResult[byResultCount].sSkillResult.vShift.y = 0.0f;
			sPacket.sCharState.sCharStateDetail.sCharStateHTB.aHTBSkillResult[byResultCount].sSkillResult.vShift.z = 0.0f;

			sPacket.sCharState.sCharStateDetail.sCharStateHTB.byResultCount++;
			byResultCount++;
		}
	}
		
	SendEvent(sizeof(sPacket), &sPacket);
}

void CDboVirtualServer::SendCharUpdateHTBSendbagState(void)
{
	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(m_uiTargetSerialId);
	RwV3d vLoc = pSobObj->GetPosition();
	RwV3d vDir = pSobObj->GetDirection();

	sGU_UPDATE_CHAR_STATE sPacket;
	memset(&sPacket, 0, sizeof(sGU_UPDATE_CHAR_STATE));

	sPacket.wOpCode = GU_UPDATE_CHAR_STATE;
	sPacket.handle = pSobObj->GetSerialID();
	sPacket.sCharState.sCharStateBase.byStateID = CHARSTATE_SANDBAG;
	sPacket.sCharState.sCharStateBase.vCurLoc.x = vLoc.x;
	sPacket.sCharState.sCharStateBase.vCurLoc.y = vLoc.y;
	sPacket.sCharState.sCharStateBase.vCurLoc.z = vLoc.z;
	sPacket.sCharState.sCharStateBase.vCurDir.x = vDir.x;
	sPacket.sCharState.sCharStateBase.vCurDir.y = vDir.y;
	sPacket.sCharState.sCharStateBase.vCurDir.z = vDir.z;

	SendEvent(sizeof(sPacket), &sPacket);
}


void CDboVirtualServer::SendCharToggleFighting(void *pPacket)
{
	//sUG_CHAR_TOGG_FIGHTING *pFightingMode = (sUG_CHAR_TOGG_FIGHTING*)pPacket;

	//SVirtualBattleData *pBattleData;
	//ListAttackBegin::iterator it;
	//for(it = m_listAttackBegin.begin(); it != m_listAttackBegin.end(); it++)
	//{
	//	pBattleData = (*it);
	//	if(pBattleData->uiSerialId == pFightingMode->handle)
	//	{
	//		if(pFightingMode->bFightMode)
	//			pBattleData->bAttackMode = TRUE;
	//		else
	//			pBattleData->bAttackMode = FALSE;
	//	}
	//}

	//sGU_CHAR_FIGHTMODE sPacket;
	//memset(&sPacket, 0, sizeof(sGU_CHAR_FIGHTMODE));

	//sPacket.wOpCode = GU_CHAR_FIGHTMODE;
	//sPacket.handle = pFightingMode->handle;
	//if(pFightingMode->bFightMode)
	//	sPacket.bFightMode = true;
	//else
	//	sPacket.bFightMode = false;

	//SendEvent(sizeof(sPacket), &sPacket);
}

void CDboVirtualServer::SendCharActionAttack(RwUInt32 uiSerialId)
{
	static RwUInt8 byChainAttack = 0;
	RwBool bDamageApply = TRUE;

	CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(uiSerialId) );
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>( pActor->GetBehaviorData() );
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
	CNtlSob *pTarSobObj = GetNtlSobManager()->GetSobObject(pCtrlStuff->hTargetSerialId);
	if(!pTarSobObj)
		return;

	RwUInt32 uiLp = Logic_GetLp(pTarSobObj);
	if(uiLp <= 0)
		return;

	if(!pTarSobObj->CanHurted())
		return;
	
	RwV3d vAttLoc = pActor->GetPosition();
	RwV3d vTarLoc = pTarSobObj->GetPosition();
	RwV3d vAttDir;
	RwV3dSubMacro(&vAttDir, &vTarLoc, &vAttLoc);
	RwV3dNormalize(&vAttDir, &vAttDir);

	sGU_CHAR_ACTION_ATTACK sPacket;
	memset(&sPacket, 0, sizeof(sGU_CHAR_ACTION_ATTACK));

	sPacket.wOpCode = GU_CHAR_ACTION_ATTACK;
	sPacket.hSubject = pActor->GetSerialID(); 
	sPacket.hTarget = pCtrlStuff->hTargetSerialId;
	sPacket.bChainAttack = TRUE;

	if( !(pActor->GetClassID() == SLCLASS_PLAYER || pActor->GetClassID() == SLCLASS_AVATAR) )
		sPacket.bChainAttack = FALSE;
	
	if(sPacket.bChainAttack)
		sPacket.byAttackSequence = byChainAttack%6+NTL_BATTLE_CHAIN_ATTACK_START;
	else
		sPacket.byAttackSequence = rand()%2;

	sPacket.wAttackResultValue = VIRTUAL_DAMAGE;
	
	sPacket.vShift.x = vAttDir.x;
	sPacket.vShift.y = vAttDir.y;
	sPacket.vShift.z = vAttDir.z;
	
	if(sPacket.byAttackSequence == 6)
	{
		if(rand()%2)
			sPacket.byAttackResult = BATTLE_ATTACK_RESULT_KNOCKDOWN;
		else
			sPacket.byAttackResult = BATTLE_ATTACK_RESULT_SLIDING;
	}
	else
	{
		RwInt32 iRandValue = rand()%5;
		if(iRandValue <= 2)
			sPacket.byAttackResult = BATTLE_ATTACK_RESULT_HIT;
		else if(iRandValue == 3)
		{
			bDamageApply = FALSE;
			sPacket.byAttackResult = BATTLE_ATTACK_RESULT_DODGE;
		}
		else
		{
			bDamageApply = FALSE;
			sPacket.byAttackResult = BATTLE_ATTACK_RESULT_BLOCK;
		}
	}

	SendEvent(sizeof(sPacket), &sPacket);

	byChainAttack++;

	// update LP
	if(bDamageApply)
	{
		m_iCurrentHp -= VIRTUAL_DAMAGE;
		if(m_iCurrentHp <= 0)
			m_iCurrentHp = 0;

		SendCharUpdateLp(m_iCurrentHp);
	}

	// fainting 검사.
	if(m_iCurrentHp <= 0)
	{
		SendCharUpdateFaintingState();
	}
}


void CDboVirtualServer::SendCharSitDown(void)
{
	sGU_CHAR_SITDOWN sPacket;
	memset(&sPacket, 0, sizeof(sGU_CHAR_SITDOWN));

	sPacket.wOpCode = GU_CHAR_SITDOWN;
	sPacket.handle =  GetNtlSLGlobal()->GetSobAvatar()->GetSerialID();

	SendEvent(sizeof(sPacket), &sPacket);
}

void CDboVirtualServer::SendCharStandUp(void)
{
	sGU_CHAR_STANDUP sPacket;
	memset(&sPacket, 0, sizeof(sGU_CHAR_STANDUP));

	sPacket.wOpCode = GU_CHAR_STANDUP;
	sPacket.handle =  GetNtlSLGlobal()->GetSobAvatar()->GetSerialID();

	SendEvent(sizeof(sPacket), &sPacket);
}

void CDboVirtualServer::SendAddItem(RwUInt32 uiTblId)
{
	sGU_ITEM_CREATE sPacket;
	memset(&sPacket, 0, sizeof(sGU_ITEM_CREATE));

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlInventory *pInventory = pSobAvatar->GetInventory();
	SERIAL_HANDLE hItemSerial = pInventory->GetBagItem(0);
	CNtlSobItem *pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(hItemSerial) );
	RwUInt8 bySlotIdx = 0;
	
	for(RwInt32 i = 0; i < pSobItem->GetChildNum(); ++i)
	{
		if(!pSobItem->IsValidChild(i))
		{
			bySlotIdx = i;
			break;
		}
	}

	sPacket.wOpCode = GU_ITEM_CREATE;
	sPacket.handle = AcquireSerialId();
	sPacket.sItemData.itemNo = uiTblId;
	sPacket.sItemData.byPlace = CONTAINER_TYPE_BAG1;
	sPacket.sItemData.byPosition = bySlotIdx;
	sPacket.sItemData.byRank = 1;
	sPacket.sItemData.byStackcount = 0;
	sPacket.sItemData.byCurrentDurability = 100;
		
	SendEvent(sizeof(sPacket), &sPacket);

}

RwBool CDboVirtualServer::IsSkillCastingExist(void)
{
	CSkillTable *pSkillTbl = API_GetTableContainer()->GetSkillTable();
	sSKILL_TBLDAT *pSkillTblData = reinterpret_cast<sSKILL_TBLDAT*>( pSkillTbl->FindData(m_uiSkillTblId) );
	if(pSkillTblData->byCasting_Time == 0)
		return FALSE;

	return TRUE;
}

void CDboVirtualServer::SendCharSkillRes(void *pPacket)
{
	// rect data 처리.
	sUG_CHAR_SKILL_REQ *pCharSkillReq = (sUG_CHAR_SKILL_REQ*)pPacket;
	
	m_bySkillActiveType = VIRTUAL_SKILL_TYPE_CASTING;
	m_uiTargetSerialId = pCharSkillReq->hTarget;
	m_uiSkillTime = timeGetTime();

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();

	CNtlSkillContainer *pSkillContainer = pSobAvatar->GetSkillContainer();
	sSKILL_TBLDAT *pSkillTblData = pSkillContainer->GetSkillFromTable(pCharSkillReq->bySlotIndex);
	m_uiSkillTblId = pSkillTblData->tblidx;
	m_uiSkillReqSerialId = pSobAvatar->GetSerialID();
	
	/*
	CNtlSobPet *pSobPet = reinterpret_cast<CNtlSobPet*>(pSobAvatar->m_pSobPet);
	CNtlPetSkillContainer *pPetSkillContainer = pSobPet->GetSkillContainer();
	sSKILL_TBLDAT *pSkillTblData = pPetSkillContainer->GetSkillFromTable(pCharSkillReq->bySlotIndex);
	m_uiSkillReqSerialId = pSobPet->GetSerialID();
	m_uiSkillTblId = pSkillTblData->tblidx;
	*/

	// send
	sGU_CHAR_SKILL_RES sPacket;
	memset(&sPacket, 0, sizeof(sGU_CHAR_SKILL_RES));

	sPacket.wOpCode = GU_CHAR_SKILL_RES;
	sPacket.wResultCode = GAME_SUCCESS;

	SendEvent(sizeof(sPacket), &sPacket);
}

void CDboVirtualServer::SendCharHTBSkillRes(void *pPacket)
{
	// rect data 처리.
	sUG_HTB_START_REQ *pCharHTBSkillReq = (sUG_HTB_START_REQ*)pPacket;

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlSkillContainer *pSkillContainer = pSobAvatar->GetSkillContainer();
	sHTB_SET_TBLDAT *pHTBSetTblData = pSkillContainer->GetHTBSkillFromTable(pCharHTBSkillReq->bySkillSlot);

	m_uiTargetSerialId = pCharHTBSkillReq->hTarget;
	m_uiSkillTblId = pHTBSetTblData->tblidx;
	
	// send
	sGU_HTB_START_RES sPacket;
	memset(&sPacket, 0, sizeof(sGU_HTB_START_RES));

	sPacket.wOpCode = GU_HTB_START_RES;
	sPacket.wResultCode = GAME_SUCCESS;
	sPacket.bySkillSlot = pCharHTBSkillReq->bySkillSlot;

	SendEvent(sizeof(sPacket), &sPacket);
}

void CDboVirtualServer::SendCharSkillCasting(void)
{
	sGU_CHAR_SKILL_USE sPacket;
	memset(&sPacket, 0, sizeof(sGU_CHAR_SKILL_USE));

	sPacket.wOpCode = GU_CHAR_SKILL_USE;
	sPacket.handle = m_uiSkillReqSerialId;
	sPacket.hTarget = m_uiTargetSerialId;
	sPacket.skillId = m_uiSkillTblId;
		
	SendEvent(sizeof(sPacket), &sPacket);
}

void CDboVirtualServer::SendCharSkillAction(void)
{
	sGU_CHAR_ACTION_SKILL sPacket;
	memset(&sPacket, 0, sizeof(sGU_CHAR_ACTION_SKILL));

	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(m_uiSkillReqSerialId); 
	CNtlSob *pTarSobObj = GetNtlSobManager()->GetSobObject(m_uiTargetSerialId);
	RwV3d vAttLoc = pSobObj->GetPosition();
	RwV3d vTarLoc = pTarSobObj->GetPosition();
	RwV3d vAttDir;
	RwV3dSubMacro(&vAttDir, &vTarLoc, &vAttLoc);
	RwV3dNormalize(&vAttDir, &vAttDir);

	sPacket.wOpCode = GU_CHAR_ACTION_SKILL;
	sPacket.handle = pSobObj->GetSerialID();
	sPacket.wResultCode = GAME_SUCCESS;
	sPacket.hAppointedTarget = m_uiTargetSerialId;
	sPacket.skillId = m_uiSkillTblId;
	sPacket.bySkillResultCount = 1;

	sPacket.aSkillResult[0].hTarget = m_uiTargetSerialId;
	sPacket.aSkillResult[0].byAttackResult = BATTLE_ATTACK_RESULT_HIT;
	sPacket.aSkillResult[0].effectResult1.fResultValue = 100;
	sPacket.aSkillResult[0].effectResult2.fResultValue = 100;
	sPacket.aSkillResult[0].vShift.x = vAttDir.x;
	sPacket.aSkillResult[0].vShift.y = vAttDir.y;
	sPacket.aSkillResult[0].vShift.z = vAttDir.z;

	sPacket.aSkillResult[1].hTarget = m_uiTargetSerialId+1;
	sPacket.aSkillResult[1].byAttackResult = BATTLE_ATTACK_RESULT_HIT;
	sPacket.aSkillResult[1].effectResult1.fResultValue = 100;
	sPacket.aSkillResult[1].effectResult2.fResultValue = 100;
	sPacket.aSkillResult[1].vShift.x = vAttDir.x;
	sPacket.aSkillResult[1].vShift.y = vAttDir.y;
	sPacket.aSkillResult[1].vShift.z = vAttDir.z;

	SendEvent(sizeof(sPacket), &sPacket);
}

void CDboVirtualServer::SendCharLearnSkillRes(RwUInt32 uiTblId)
{
	sGU_SKILL_LEARN_RES sPacket;
	memset(&sPacket, 0, sizeof(sGU_SKILL_LEARN_RES));

	sPacket.wOpCode = GU_SKILL_LEARN_RES;
	sPacket.wResultCode = GAME_SUCCESS;

	CNtlSobAvatar *pSobAvater = GetNtlSLGlobal()->GetSobAvatar();
	CNtlSkillContainer *pSkillContainer = pSobAvater->GetSkillContainer();
	RwInt32 iSkillCount = pSkillContainer->GetSkillCount();
	for(RwInt32 i = 0; i < iSkillCount; ++i)
	{
		if(!pSkillContainer->GetSkill(i))
		{
			sPacket.bySlot = i;
			break;
		}
	}

	sPacket.skillId = uiTblId;

	SendEvent(sizeof(sPacket), &sPacket);
}

void CDboVirtualServer::SendCharLearnHTBRes(RwUInt32 uiTblId)
{
	sGU_HTB_LEARN_RES sPacket;
	memset(&sPacket, 0, sizeof(sGU_HTB_LEARN_RES));

	sPacket.wOpCode = GU_HTB_LEARN_RES;
	sPacket.wResultCode = GAME_SUCCESS;
	sPacket.skillId = uiTblId;

	SendEvent(sizeof(sPacket), &sPacket);
}

void CDboVirtualServer::SendCharUpdateLp(RwUInt16 wLp)
{
	sGU_UPDATE_CHAR_LP sPacket;
	memset(&sPacket, 0, sizeof(sGU_UPDATE_CHAR_LP));

	sPacket.wOpCode = GU_UPDATE_CHAR_LP;
	sPacket.handle = m_uiTargetSerialId;
	sPacket.wCurLP = wLp;
	sPacket.wMaxLP = VIRTUAL_MAX_LP;

	SendEvent(sizeof(sPacket), &sPacket);
}

void CDboVirtualServer::SendUpdateCharSpeed(RwReal fSpeed)
{
	sGU_UPDATE_CHAR_SPEED sPacket;
	memset(&sPacket, 0, sizeof(sGU_UPDATE_CHAR_SPEED));

	sPacket.wOpCode = GU_UPDATE_CHAR_SPEED;
	sPacket.handle = m_uiAvatarSerialId;
	sPacket.fLastWalkingSpeed = fSpeed;
	sPacket.fLastRunningSpeed = fSpeed;

	SendEvent(sizeof(sPacket), &sPacket);
}

void CDboVirtualServer::SendCharTestBuffRegistered(void)
{
	sGU_BUFF_REGISTERED sPacket;

	memset(&sPacket, 0, sizeof(sGU_BUFF_REGISTERED));

	sPacket.wOpCode = GU_BUFF_REGISTERED;
	sPacket.hHandle = m_uiAvatarSerialId;
	sPacket.tblidx = 1;
	sPacket.dwTimeRemaining = 100;
	
	SendEvent(sizeof(sPacket), &sPacket);
}

void CDboVirtualServer::SendEvent(RwUInt32 iDataLen, void *pData)
{
	NTL_ASSERT(iDataLen < MAX_VIRTUAL_SERVER_PACKET_SIZE, "Virtual server packet buffer not enough");

	RWS::CMsg msg;
	msg.Id = g_EventVirtualServerPacket;

	SDboEventVirtualServerPacket data;
	data.iDataLen = iDataLen;
	memcpy(data.chPacket, pData, iDataLen); 
		
	PostMsg(msg, reinterpret_cast<void*>(&data), sizeof(SDboEventVirtualServerPacket));
}

CDboVirtualServer* CDboVirtualServer::GetInstance(void)
{
	static CDboVirtualServer virtualserver;
	return &virtualserver;
}


void CDboVirtualServer::Update(RwReal fElapsed)
{
	// packet을 pop한다.
	PopPacket();

	// 공격 처리
	SVirtualBattleData *pBattleData;
	ListAttackBegin::iterator it;
	for(it = m_listAttackBegin.begin(); it != m_listAttackBegin.end(); it++)
	{
		pBattleData = (*it);
		if(timeGetTime() - pBattleData->dwCurrTime >= VIRTUAL_MONSTER_ATTACK_UPDATE_TICK)
		{
			SendCharActionAttack(pBattleData->uiSerialId);
			pBattleData->dwCurrTime = timeGetTime();
		}
	}

	// 이동 처리
	if(timeGetTime() - m_uiMobMoveCurrTime >= VIRTUAL_MONSTER_MOVE_UPDATE_TICK)
	{
//		SendMobMove();
		m_uiMobMoveCurrTime = timeGetTime();
	}

	// skill 처리. 
	if(m_bySkillActiveType == VIRTUAL_SKILL_TYPE_CASTING)
	{
		CSkillTable *pSkillTbl = API_GetTableContainer()->GetSkillTable();
		sSKILL_TBLDAT *pSkillTblData = reinterpret_cast<sSKILL_TBLDAT*>( pSkillTbl->FindData(m_uiSkillTblId) );
		
		if(timeGetTime() - m_uiSkillTime >= (RwUInt32)(pSkillTblData->byCasting_Time*1000))
		{
			SendCharSkillAction();
			m_bySkillActiveType = VIRTUAL_SKILL_TYPE_NONE;
		}
	}
}

void CDboVirtualServer::PacketProc(void *pPacket)
{
	sNTLPACKETHEADER *pHeader = (sNTLPACKETHEADER*)pPacket;
	RwUInt16 wOpCode = pHeader->wOpCode;

	switch(wOpCode)
	{
	case UA_LOGIN_CREATEUSER_REQ:
		SendLoginCreateUserRes(pPacket);
		break;
	case UA_LOGIN_REQ:
		SendLoginRes(pPacket);
		break;
	case UC_LOGIN_REQ:
		SendCharLoginRes(pPacket);
		break;
	case UC_CHARACTER_SERVERLIST_REQ:
		SendServerListRes(pPacket);
		break;
	case UC_CHARACTER_SERVERLIST_ONE_REQ:
		SendOneServerRes(pPacket);
		break;
	case UC_CHARACTER_LOAD_REQ:
		SendCharLoadRes(pPacket);
		break;
	case UC_CHARACTER_ADD_REQ:
		SendCharAddRes(pPacket);
		break;
	case UC_CHARACTER_DEL_REQ:
		SendCharDelRes(pPacket);
		break;
	case UC_CHARACTER_SELECT_REQ:
		SendCharSelectRes(pPacket);
		break;
	case UG_GAME_ENTER_REQ:
		SendGameEnterRes(pPacket);
		SendAvatarCharInfo();
		SendAvatarItemInfo();
//		SendAvatarSkillInfo();
		SendAvatarInfoEnd();
		break;
	case UG_ENTER_WORLD:
		{
			SendAvatarWorldInfo();

			RwInt32 i;

			for(i = 0; i < VIRTUAL_PLAYER_SPAWN_NUM; i++)
				SendPlayerCreate();

//			for(i = 0; i < VIRTUAL_MONSTER_SPAWN_NUM; i++)
//				SendMonsterCreate();

			SendEnterWorldComplete();

//			SendCharTestBuffRegistered();
		}
		break;
	case UG_CHAR_MOVE: case UG_CHAR_DEST_MOVE:
		break;
	case UG_ITEM_MOVE_REQ:
		SendItemMoveRes(pPacket);
		break;
	case UG_CHAR_TOGG_FIGHTING:
		SendCharToggleFighting(pPacket);
		break;
	case UG_CHAR_ATTACK_BEGIN:
		RecvCharAttackBegin(pPacket);
		break;
	case UG_CHAR_ATTACK_END:
		RecvCharAttackEnd(pPacket);
		break;
	case UG_CHAR_TOGG_SITDOWN:
		m_bSitDown = !m_bSitDown;
		if(m_bSitDown)
			SendCharSitDown();
		else
			SendCharStandUp();
		break;
	case UG_CHAR_TARGET_SELECT:
		RecvCharTargetSelect(pPacket);
		break;
	case UG_CHAR_SKILL_REQ:
		SendCharSkillRes(pPacket);
		if(!IsSkillCastingExist())
		{
			SendCharSkillAction();
			m_bySkillActiveType = VIRTUAL_SKILL_TYPE_NONE;
		}
		else
			SendCharSkillCasting();
		break;
	case UG_HTB_START_REQ:
		SendCharHTBSkillRes(pPacket);
		SendCharUpdateHTBState();
		SendCharUpdateHTBSendbagState();
		break;
	case UG_SERVER_COMMAND:
		RecvServerCommand(pPacket);
		break;
	}
}

void CDboVirtualServer::PopPacket(void)
{
	ListPacket::iterator it;
	for(it = m_listPacket.begin(); it != m_listPacket.end(); ++it)
	{
		void *pData = (*it);
		PacketProc(pData);
		NTL_ARRAY_DELETE(pData);
	}

	m_listPacket.clear();
}

bool CDboVirtualServer::SendPacket(RwInt32 iDataLen, void *pData)
{
	void *pPacket = NTL_NEW char [iDataLen];
	memcpy(pPacket, pData, iDataLen);
	m_listPacket.push_back(pPacket); 
	
	return true;
}

bool DboVirtualNetSenderFunc(RwInt32 iDataLen, void *pData)
{
	return CDboVirtualServer::GetInstance()->SendPacket(iDataLen, pData);
}


#endif