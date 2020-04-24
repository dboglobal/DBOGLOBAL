#include "stdafx.h"
#include "NtlRandom.h"
#include "ObjectManager.h"
#include "ActionPatternSystem.h"
#include "RangeCheck.h"
#include "TableContainerManager.h"
#include "GameServer.h"
#include "GameMain.h"
#include "NtlNavi.h"

#include "BusSystem.h" // HERE: #include "Npc.h" #include "NtlPacketGU.h"


CNpc::CNpc(eOBJTYPE eObjType/* = OBJTYPE_NPC*/)
	:CCharacter(eObjType)
{
	this->Initialize();
}

CNpc::~CNpc()
{
	if (!m_bIsDeleted)
		this->Destroy();
}



void CNpc::CopyToObjectInfo(sOBJECT_INFO *pObjectInfo, CHARACTERID playerCharID)
{
	pObjectInfo->objType = GetObjType();
	switch (GetObjType())
	{
		case OBJTYPE_NPC:
		{
			m_pStateManager->CopyTo(&pObjectInfo->npcState);

			pObjectInfo->npcBrief.actionpatternTblIdx = GetActionPattern()->GetTableIdx();
			pObjectInfo->npcBrief.bIsEnemyPc = m_eRelationPc == RELATION_TYPE_ENEMY;
			pObjectInfo->npcBrief.bySizeRate = m_bySizeRate;
			pObjectInfo->npcBrief.curLp = GetCurLP();
			pObjectInfo->npcBrief.fLastFlyingAccelSpeed = GetFlyAccelSpeed();
			pObjectInfo->npcBrief.fLastFlyingDashSpeed = GetFlyDashSpeed();
			pObjectInfo->npcBrief.fLastFlyingSpeed = GetFlySpeed();
			pObjectInfo->npcBrief.fLastRunningSpeed = GetRunSpeed();
			pObjectInfo->npcBrief.fLastWalkingSpeed = GetWalkingSpeed();
			pObjectInfo->npcBrief.fSkillAnimationSpeedModifier = GetSkillAnimationSpeedModifier();
			pObjectInfo->npcBrief.maxLp = GetMaxLP();
			pObjectInfo->npcBrief.nicknameTblidx = bot_profile.nicknameTblidx;
			pObjectInfo->npcBrief.tblidx = GetTblidx();
			pObjectInfo->npcBrief.wAttackSpeedRate = GetAttackSpeedRate();
			pObjectInfo->npcBrief.wCurEP = GetCurEP();
			pObjectInfo->npcBrief.wMaxEP = GetMaxEP();
		}
		break;

		case OBJTYPE_ITEM_PET:
		{

		}
		break;

		case OBJTYPE_DYNAMIC:
		{

		}
		break;

		default: ERR_LOG(LOG_USER, "Spawn Obj type %d not added ! TO-DO !!!", GetObjType()); break;
	}
}


void CNpc::Initialize()
{
	m_bStandAlone = true;
	m_bIsInRespawn = false;

	SetLinkPc(INVALID_CHARACTERID, INVALID_HOBJECT);

	m_eRelationPc = INVALID_RELATION_TYPE;
	m_pPathFinder = NULL;
	m_bPlayScriptEnd = false;
	m_dwAi_Bit_Flag = 0;

	bot_profile.bPartyLeader = false;
	bot_profile.sScriptData.Init();
	bot_profile.sBotSubData.Init();

	m_dwElapsedTimeSinceCheatingTargetSet = 0;
	m_bIsChasingCheatingTarget = false;
	m_bIsRetreat = false;
	SetTimid(TIMID_NORMAL, 0.0f);
	m_nActionStatusFlag = 0;

	m_dwChangeAttackTargetDelayTime = 5000;
	m_bIsChangeTargetWait = false;

	m_pEscortManager = NULL;
	m_pSpeechGroup = NULL;

	m_partyPtr = INVALID_NPCPARTY_PTR;
	m_dwLastHasNearbyPlayerQuadTime = 0;
	m_bCacheHasNearbyPlayerQuad = false;
}


void CNpc::Destroy()
{
	SAFE_DELETE(m_pPathFinder);
	SAFE_DELETE(m_pEscortManager);

	m_cControlStateManager.Destroy();
}


bool CNpc::Create(sCHARSTATE* sCharState)
{
	if (CCharacter::Create(sCharState))
	{
		m_pPathFinder = CreatePathFinder();
		if (!m_pPathFinder)
			return false;

		m_pEscortManager = CreateEscortManager();
		if (!m_pEscortManager)
			return false;

		if (!PrepareControlStateManager())
			return false;

		if (!PrepareHelpMeListManager())
			return false;

		return true;
	}

	return false;
}



bool CNpc::CreateDataAndSpawn(WORLDID worldId, sNPC_TBLDAT* npcTbldat, sSPAWN_TBLDAT* spawnTbldat, bool bSpawnOnServerStart/* = false*/, BYTE bySpawnFuncFlag/* = SPAWN_FUNC_FLAG_RESPAWN*/)
{
	SetWorldID(worldId);

	m_pTbldat = npcTbldat;

	sCHARSTATE sTempState;
	sTempState.sCharStateBase.bFightMode = false;
	sTempState.sCharStateBase.byStateID = CHARSTATE_STANDING;
	sTempState.sCharStateBase.dwConditionFlag = 0;
	sTempState.sCharStateBase.dwStateTime = 0;
	sTempState.sCharStateBase.eAirState = AIR_STATE_OFF;
	spawnTbldat->vSpawn_Dir.CopyTo(sTempState.sCharStateBase.vCurDir);
	spawnTbldat->vSpawn_Loc.CopyTo(sTempState.sCharStateBase.vCurLoc);
	sTempState.sCharStateBase.aspectState.sAspectStateBase.byAspectStateId = ASPECTSTATE_INVALID;

	bot_profile.nicknameTblidx = INVALID_TBLIDX;
	SetPartyID(spawnTbldat->dwParty_Index);
	SetTblidx(npcTbldat->tblidx);

	m_dwAi_Bit_Flag = npcTbldat->dwAi_Bit_Flag;

	SetLevel(npcTbldat->byLevel);
	SetEffectiveLevel(npcTbldat->byLevel);
	m_bySizeRate = 10;

	bot_profile.sBotSubData.Init();
	bot_profile.bPartyLeader = spawnTbldat->bParty_Leader;
	bot_profile.byMoveRange = spawnTbldat->byMove_Range;
	bot_profile.byMoveType = spawnTbldat->bySpawn_Move_Type;
	bot_profile.bySpawnFuncFlag = bySpawnFuncFlag; //eSPAWN_FUNC_FLAG
	bot_profile.bySpawnRange = spawnTbldat->bySpawn_Loc_Range;
	bot_profile.byWanderRange = spawnTbldat->byWander_Range;
	bot_profile.pathTblidx = spawnTbldat->path_Table_Index;
	bot_profile.sScriptData.byActualLevel = npcTbldat->byLevel;
	bot_profile.sScriptData.byCreatorType = INVALID_BYTE;
	bot_profile.sScriptData.byEffectiveLevel = 0;
	bot_profile.sScriptData.byMobGrade = npcTbldat->byGrade;
	bot_profile.sScriptData.dwBattleScriptScene = INVALID_DWORD;
	bot_profile.sScriptData.playScript = spawnTbldat->playScript;
	bot_profile.sScriptData.playScriptScene = spawnTbldat->playScriptScene;
	bot_profile.sScriptData.sBotAiIndex.tblidx = INVALID_TBLIDX;
	bot_profile.sScriptData.sBotAiIndex.bySubIndex = INVALID_BYTE;
	bot_profile.sScriptData.tblidxAiScript = spawnTbldat->aiScript;
	bot_profile.sScriptData.tblidxAiScriptScene = spawnTbldat->aiScriptScene;
	bot_profile.sScriptData.tblidxBattleScript = INVALID_TBLIDX;
	//bot_profile.sScriptData.wMobClass 

	spawnTbldat->vFollowDistance.CopyTo(bot_profile.vPartyLeaderDistance); 


	bot_profile.vSpawnDir.operator=(spawnTbldat->vSpawn_Dir);
	bot_profile.vSpawnLoc.operator=(spawnTbldat->vSpawn_Loc);

	bot_profile.wSpawnTime = spawnTbldat->wSpawn_Cool_Time;

	m_pAniTbldat = g_pTableContainer->GetModelToolCharDataTable()->FindData(npcTbldat->szModel);
	if (!m_pAniTbldat)
	{
		ERR_LOG(LOG_SYSTEM, "Mob can't find Animation Data[%s]", npcTbldat->szModel);
		return false;
	}

	m_fScale = npcTbldat->fScale;
	if (!RefreshObjectRadius())
	{
		ERR_LOG(LOG_SYSTEM, "couldnt refresh object radius. Scale %f NPC Tblidx %u", npcTbldat->fScale, npcTbldat->tblidx);
		return false;
	}

	if (!RefreshActionPatternData(spawnTbldat->actionPatternTblidx))
	{
		ERR_LOG(LOG_SYSTEM, "Couldnt refresh action pattern data. NPC tblidx %u action pattern tblidx %u", npcTbldat->tblidx, spawnTbldat->actionPatternTblidx);
		return false;
	}

	m_pSpeechGroup = g_pTableContainer->GetNpcSpeechTable()->FindSpeechGroup(npcTbldat->dwDialogGroup);

	if (Create(&sTempState))
	{
		LoadSkillTable(bot_profile.sBotSubData.tblidxOnlyOneSkillUse);

		GetCharAtt()->CalculateAll();
		Spawn(bSpawnOnServerStart);
		return true;
	}

	return false;
}


bool CNpc::CreateDataAndSpawn(sNPC_DATA& sData, sNPC_TBLDAT* npcTbldat)
{
	SetWorldID(sData.worldID);

	m_pTbldat = npcTbldat;

	sCHARSTATE sTempState;
	sTempState.sCharStateBase.bFightMode = false;
	sTempState.sCharStateBase.byStateID = CHARSTATE_STANDING;
	sTempState.sCharStateBase.dwConditionFlag = sData.qwCharConditionFlag;
	sTempState.sCharStateBase.dwStateTime = 0;
	sTempState.sCharStateBase.eAirState = AIR_STATE_OFF;
	sTempState.sCharStateBase.vCurDir = sData.vSpawnDir;
	sTempState.sCharStateBase.vCurLoc = sData.vSpawnLoc;
	sTempState.sCharStateBase.aspectState.sAspectStateBase.byAspectStateId = ASPECTSTATE_INVALID;

	bot_profile.nicknameTblidx = INVALID_TBLIDX;
	SetPartyID(sData.partyID);
	SetTblidx(npcTbldat->tblidx);

	m_dwAi_Bit_Flag = npcTbldat->dwAi_Bit_Flag;

	m_byImmortalMode = sData.byImmortalMode;

	SetEffectiveLevel(npcTbldat->byLevel);
	SetLevel(npcTbldat->byLevel);
	m_bySizeRate = 10;

	bot_profile.bPartyLeader = sData.bPartyLeader;
	bot_profile.byMoveRange = sData.byMoveRange;
	bot_profile.byMoveType = sData.byMoveType;
	bot_profile.bySpawnFuncFlag = sData.bySpawnFuncFlag;
	bot_profile.bySpawnRange = sData.bySpawnRange;
	bot_profile.byWanderRange = sData.byWanderRange;
	bot_profile.pathTblidx = sData.pathTblidx;

	bot_profile.sScriptData = sData.sScriptData;

	bot_profile.sBotSubData = sData.sBotSubData;

	bot_profile.vSpawnDir.operator=(sData.vSpawnDir);
	bot_profile.vSpawnLoc.operator=(sData.vSpawnLoc);

	bot_profile.wSpawnTime = sData.wSpawnTime;

	bot_profile.vPartyLeaderDistance = sData.vPartyLeaderDistance;

	m_pAniTbldat = g_pTableContainer->GetModelToolCharDataTable()->FindData(npcTbldat->szModel);
	if (!m_pAniTbldat)
	{
		ERR_LOG(LOG_SYSTEM, "Mob can't find Animation Data[%s]", npcTbldat->szModel);
		return false;
	}

	m_fScale = npcTbldat->fScale;
	if (!RefreshObjectRadius())
	{
		ERR_LOG(LOG_SYSTEM, "couldnt refresh object radius. Scale %f NPC Tblidx %u", npcTbldat->fScale, npcTbldat->tblidx);
		return false;
	}

	if (!RefreshActionPatternData(sData.actionpatternTblIdx))
	{
		ERR_LOG(LOG_SYSTEM, "Couldnt refresh action pattern data. NPC tblidx %u action pattern tblidx %u", npcTbldat->tblidx, sData.actionpatternTblIdx);
		return false;
	}

	m_pSpeechGroup = g_pTableContainer->GetNpcSpeechTable()->FindSpeechGroup(npcTbldat->dwDialogGroup);

	if (Create(&sTempState))
	{
		LoadSkillTable(bot_profile.sBotSubData.tblidxOnlyOneSkillUse);

		GetCharAtt()->CalculateAll();
		Spawn(false);
		return true;
	}

	return false;
}

bool CNpc::DoTransformation(TBLIDX tblidx, ACTIONPATTERNTBLIDX actionPatternTblidx, TBLIDX tblidxOnlyOneSkillUse)
{
	sNPC_TBLDAT* pNpcTbldat = (sNPC_TBLDAT*)g_pTableContainer->GetNpcTable()->FindData(tblidx);
	if (pNpcTbldat == NULL)
	{
		ERR_LOG(LOG_BOTAI, "Can not find monster table. Tblidx %u", tblidx);
		return false;
	}

	sMODELTOOL_CHAR_TBLDAT* pAniTbldat = g_pTableContainer->GetModelToolCharDataTable()->FindData(pNpcTbldat->szModel);
	if (!m_pAniTbldat)
	{
		ERR_LOG(LOG_SYSTEM, "Mob can't find Animation Data[%s]. Tblidx %u", pNpcTbldat->szModel, pNpcTbldat->tblidx);
		return false;
	}

	m_fScale = pNpcTbldat->fScale;
	if (!RefreshObjectRadius())
	{
		ERR_LOG(LOG_SYSTEM, "couldnt refresh object radius. Scale %f NPC Tblidx %u", pNpcTbldat->fScale, pNpcTbldat->tblidx);
		return false;
	}

	m_pAniTbldat = pAniTbldat;
	m_pTbldat = pNpcTbldat;

	SetTblidx(pNpcTbldat->tblidx);
	m_dwAi_Bit_Flag = pNpcTbldat->dwAi_Bit_Flag;
	SetLevel(pNpcTbldat->byLevel);
	SetEffectiveLevel(pNpcTbldat->byLevel);

	m_pSpeechGroup = g_pTableContainer->GetNpcSpeechTable()->FindSpeechGroup(pNpcTbldat->dwDialogGroup);

	CNtlPacket packet(sizeof(sGU_MONSTER_TRANSFORMED_NFY));
	sGU_MONSTER_TRANSFORMED_NFY * res = (sGU_MONSTER_TRANSFORMED_NFY *)packet.GetPacketData();
	res->wOpCode = GU_MONSTER_TRANSFORMED_NFY;
	res->hSubject = GetID();
	res->newTblidx = tblidx;
	packet.SetPacketLen(sizeof(sGU_MONSTER_TRANSFORMED_NFY));
	Broadcast(&packet);

	return true;
}


//--------------------------------------------------------------------------------------//
//		SPAWN NPC
//--------------------------------------------------------------------------------------//
void CNpc::Spawn(bool bSpawnOnServerStart)
{
	CGameServer* app = (CGameServer*)g_pApp;

	sNPC_TBLDAT* tbldat = GetTbldat();

	SetCurLP(GetCharAtt()->GetMaxLP());
	SetExp(0);
	SetZeni(0);
	SetCurEP(GetCharAtt()->GetMaxEP());

	m_vecFirstBattleLoc.operator=(GetEnterLoc());
	m_vecFirstBattleDir.operator=(GetEnterDir());

	if (!bSpawnOnServerStart)
	{
		sCHARSTATE sTempState;
		sTempState.sCharStateBase.bFightMode = false;
		sTempState.sCharStateBase.byStateID = CHARSTATE_SPAWNING;
		sTempState.sCharStateDetail.sCharStateSpawning.bSpawnDirection = false;
		sTempState.sCharStateDetail.sCharStateSpawning.bIsFaint = false;
		sTempState.sCharStateDetail.sCharStateSpawning.byTeleportType = TELEPORT_TYPE_DEFAULT;
		sTempState.sCharStateBase.dwConditionFlag = 0;
		sTempState.sCharStateBase.dwStateTime = 0;
		sTempState.sCharStateBase.eAirState = AIR_STATE_OFF;
		bot_profile.vSpawnLoc.CopyTo(sTempState.sCharStateBase.vCurLoc);
		bot_profile.vSpawnDir.CopyTo(sTempState.sCharStateBase.vCurDir);
		sTempState.sCharStateBase.aspectState.sAspectStateBase.byAspectStateId = ASPECTSTATE_INVALID;

		m_pStateManager->CopyFrom(&sTempState);

		SetMoveDirection(NTL_MOVE_NONE);
		SetMoveFlag(NTL_MOVE_FLAG_INVALID);
	}

	SetInitialized(true);
	m_bIsInRespawn = false;

	GetBotController()->ChangeControlState_Enter();
	app->GetGameMain()->GetWorldManager()->EnterObject(this, m_worldID, bSpawnOnServerStart);


	if (HasFunction(NPC_FUNC_FLAG_BUS))
	{
		g_pBusSystem->AddBus(this);
	}
}


void CNpc::OnEnterWorld(CWorld* pWorld)
{
	GetBotController()->ChangeControlState_Spawn();
	CSpawnObject::OnEnterWorld(pWorld);
	if (GetPartyID() != INVALID_PARTYID)
	{
		if (GetCurWorld())
		{
			if (!GetCurWorld()->GetNpcPartyManager()->JoinParty(this, GetPartyID(), IsPartyLeader()))
			{
				ERR_LOG(LOG_SYSTEM, "false == g_pNpcPartyManager->JoinParty(this, m_partyID, m_bPartyLeader). Bot tblidx[%u], partyid[%u] bPartyLeader[%u]", GetTblidx(), GetPartyID(), IsPartyLeader());
			}
		}
		else ERR_LOG(LOG_SYSTEM, "NULL == GetSectree(). Bot tblidx[%u], partyid[%u] bPartyLeader[%u]", GetTblidx(), GetPartyID(), IsPartyLeader());
	}
}

void CNpc::OnLeaveWorld(CWorld* pWorld)
{
	if (GetPartyPtr() != INVALID_NPCPARTY_PTR)
	{
		GetCurWorld()->GetNpcPartyManager()->LeaveParty(this);
	}

	CSpawnObject::OnLeaveWorld(pWorld);
}


void CNpc::LeaveGame()
{
	CGameServer* app = (CGameServer*)g_pApp;

	if (HasFunction(NPC_FUNC_FLAG_BUS))
	{
		g_pBusSystem->RemoveBus(this);
	}

	SetInitialized(false);
	SetPlayScriptEnd(true);

	if (GetBotController())
		GetBotController()->DestroyAllControlState();

	if (GetCurWorld()) //leave world if inside
		app->GetGameMain()->GetWorldManager()->LeaveObject(this);

	if (IsNPC()) //if npc we ALWAYS remove script when NPC leaves the game. It is very important. Otherwise the server might crash when class "BotAiAction_Leave" is used.
	{
		RemAllScript();	//remove all scripts
		SetTMQ(NULL);
	}


	ConsiderRespawn();
}


void CNpc::TickProcess(DWORD dwTickDiff, float fMultiple)
{
	if (!IsInitialized())
		return;

	CCharacter::TickProcess(dwTickDiff, fMultiple);
}


int CNpc::OnObjectMsg(CObjectMsg* pObjectMsg)
{
	int rc = CGameObject::OnObjectMsg(pObjectMsg);
	if (rc == 0)
	{
		rc = m_pStateManager->OnObjectMsg(pObjectMsg);
		if (rc == 0)
		{
			switch (pObjectMsg->GetID())
			{
				case OBJMSG_YOU_HIT:
				{
					rc = OnMsg_YouHit(pObjectMsg);
				}
				break;
				case OBJMSG_YOU_HIT_BY_SKILL:
				{
					rc = OnMsg_YouHitBySkill(pObjectMsg);
				}
				break;
				case OBJMSG_YOU_HIT_HTB:
				{
					rc = OnMsg_YouHitHTB(pObjectMsg);
				}
				break;
				case OBJMSG_YOU_HELP_ME:
				{
					rc = OnMsg_YouHelpMe(pObjectMsg);
				}
				break;
				case OBJMSG_YOU_KEEP_AGGRO:
				{
					rc = OnMsg_YouKeepAggro(pObjectMsg);
				}
				break;
				case OBJMSG_YOU_ALLIANCE_HELP_ME:
				{
					rc = OnMsg_AllianceYouHelpMe(pObjectMsg);
				}
				break;

				default: break;
			}
		}
	}

	return rc;
}


bool CNpc::CreateShenron(sNPC_TBLDAT* pNpcTbldat, CNtlVector& rLoc, sVECTOR2& rDir)
{
	m_pTbldat = pNpcTbldat;
	SetTblidx(pNpcTbldat->tblidx);

	sCHARSTATE sTempState;
	sTempState.sCharStateBase.bFightMode = false;
	sTempState.sCharStateBase.byStateID = CHARSTATE_SPAWNING;
	sTempState.sCharStateBase.dwConditionFlag = 0;
	sTempState.sCharStateBase.dwStateTime = 0;
	sTempState.sCharStateBase.eAirState = AIR_STATE_OFF;
	rLoc.CopyTo(sTempState.sCharStateBase.vCurLoc);
	sTempState.sCharStateBase.vCurDir.x = rDir.x;
	sTempState.sCharStateBase.vCurDir.y = 0.0f;
	sTempState.sCharStateBase.vCurDir.z = rDir.z;
	sTempState.sCharStateBase.aspectState.sAspectStateBase.byAspectStateId = ASPECTSTATE_INVALID;
	sTempState.sCharStateDetail.sCharStateSpawning.byTeleportType = TELEPORT_TYPE_DEFAULT;
	sTempState.sCharStateDetail.sCharStateSpawning.bSpawnDirection = true;
	sTempState.sCharStateDetail.sCharStateSpawning.bIsFaint = false;

	m_bySizeRate = 10;
	m_fScale = pNpcTbldat->fScale;

	bot_profile.bySpawnFuncFlag = 0;

	if (Create(&sTempState))
	{
		SetInitialized(true);
		GetBotController()->ChangeControlState_Enter();

		return true;
	}

	return false;
}


void CNpc::ConsiderRespawn()
{
	if (BIT_FLAG_TEST(GetSpawnFuncFlag(), SPAWN_FUNC_FLAG_RESPAWN)) //Check if can spawn
	{
		m_bIsInRespawn = true;
		g_pObjectManager->AddRespawn(this);
	}
	else //remove from game
	{
		CCharacter::LeaveGame();

		RemAllScript();	//remove all scripts

		g_pEventMgr->RemoveEvents(this);
		g_pObjectManager->DestroyCharacter(this);
	}
}


//--------------------------------------------------------------------------------------//
//		RECOVER EP AND LP
//--------------------------------------------------------------------------------------//
bool CNpc::Recover(DWORD dwTickDiff)
{
	CGameServer* app = (CGameServer*)g_pApp;

	if (CCharacter::Recover(dwTickDiff) == false || GetFightMode())
		return false;

	if (CannotMoveByBuff())
		return false;

	if (app->GetCurTickCount() <= m_dwLastAttackedTime + 6000 || app->GetCurTickCount() <= m_dwNextAttackTime)
		return false;

	AcquireLpEpEventID();
	UpdateCurLpEp((int)GetCharAtt()->GetLPSitdownRegen(), GetCharAtt()->GetEPSitdownRegen(), true, true);

	return true;
}


//--------------------------------------------------------------------------------------//
//		CREATE SKILL MANAGER
//--------------------------------------------------------------------------------------//
CSkillManager* CNpc::CreateSkillManager()
{
	CSkillManagerBot* pManager = new CSkillManagerBot;

	if (!pManager->Create(this, NTL_MAX_NPC_HAVE_SKILL))
	{
		ERR_LOG(LOG_SYSTEM, "StateManager Create Fail");
		SAFE_DELETE(pManager);
		return NULL;
	}

	return pManager;
}

//--------------------------------------------------------------------------------------//
//		CREATE BUFF MANAGER
//--------------------------------------------------------------------------------------//
CBuffManager* CNpc::CreateBuffManager()
{
	CBuffManagerBot* pManager = new CBuffManagerBot;
	
	if (!pManager->Create(this))
	{
		ERR_LOG(LOG_SYSTEM, "BuffManager Create Fail");
		return NULL;
	}

	return pManager;
}

//--------------------------------------------------------------------------------------//
//		CREATE CHAR ATTRIBUTE MANAGER
//--------------------------------------------------------------------------------------//
CCharacterAtt* CNpc::CreateCharAttManager()
{
	CCharacterAttBot* pManager = new CCharacterAttBot;

	if (!pManager->Create(this))
	{
		ERR_LOG(LOG_SYSTEM, "CCharacterAtt Create Fail");
		return NULL;
	}

	return pManager;
}

CStateManager * CNpc::CreateStateManager(eAIR_STATE eAirState)
{
	CBotStateManager* pManager = new CBotStateManager;

	if (!pManager->Create(this, eAirState))
	{
		ERR_LOG(LOG_SYSTEM, "StateManager Create Fail");
		return NULL;
	}

	return pManager;
}


//--------------------------------------------------------------------------------------//
//		GET MERCHANT TBLIDX
//--------------------------------------------------------------------------------------//
TBLIDX CNpc::GetMerchant(BYTE byMerchantTab)
{
	if (byMerchantTab >= NTL_MAX_MERCHANT_TAB_COUNT)
		return INVALID_TBLIDX;

	return GetTbldat()->amerchant_Tblidx[byMerchantTab];
}

CBotPathFinder* CNpc::CreatePathFinder()
{
	CGameServer* app = (CGameServer*)g_pApp;

	CBotPathFinder* pPath = new CBotPathFinder;
	if (!pPath->Create(this, app->m_config.m_bEnableNavigator))
	{
		delete pPath;
		return NULL;
	}

	return pPath;
}



//--------------------------------------------------------------------------------------//
//	
//--------------------------------------------------------------------------------------//
CNpcParty* CNpc::GetNpcParty()
{
	if (GetPartyID() == INVALID_PARTYID)
		return NULL;

	if (GetCurWorld())
		return GetCurWorld()->GetNpcPartyManager()->FindParty(GetPartyID(), GetPartyPtr());

	return NULL;
}

void CNpc::HelpParty(HOBJECT hProvoker)
{
	if (GetPartyID() != INVALID_PARTYID)
	{
		CNpcParty* pParty = GetNpcParty();
		if (pParty)
		{
			CObjMsg_YouHelpMe pMsg;
			pMsg.hSource = GetID();
			pMsg.hTarget = hProvoker;
			pMsg.dwAggroPoint = GetTbldat()->wBasic_Aggro_Point;
			pMsg.partyID = GetPartyID();

			for (CNpcParty::MEMBER_MAP::iterator it = pParty->Begin(); it != pParty->End(); it++)
			{
				if (it->second.handle != GetID())
				{
					CNpc* pMember = g_pObjectManager->GetNpc(it->second.handle);
					if (pMember->GetTargetListManager()->GetAggroCount() == 0)
						pMember->SendObjectMsg(&pMsg);
				}
			}

			pParty->AddHelpList(GetID(), pMsg.hTarget, pMsg.dwAggroPoint);
			m_cHelpMeListManager.AddHelpMeObj(pMsg.hTarget, GetID());
			m_cHelpMeListManager.SendBotCautionHelpMe(pMsg.hTarget, true);
		}
		else 
			ERR_LOG(LOG_BOTAI, "fail : Party is Null");
	}
}

void CNpc::HelpAlliance()
{
	if (GetAllianceID() == INVALID_ALLIANCEID)
	{
		ERR_LOG(LOG_SYSTEM, "Tblidx %u ObjType %u has invalid alliance id", GetTblidx(), GetObjType());
		//TODO: Find out why alliance id invalid. tblidx 6415128 objtype 2
	}
	else if(GetTargetHandle() != INVALID_HOBJECT)
	{
		CCharacter* pTarget = g_pObjectManager->GetChar(GetTargetHandle());
		if(pTarget && pTarget->IsInitialized())
		{
			if(pTarget->GetMeHaterManager()->GetContainerMeHater()->GetMeHaterCount() < 2)
			{
				CObjMsg_AllianceYouHelpMe pMsg;
				pMsg.hRequester = GetID();
				AllianceHelp(&pMsg, 1, this);

				m_cHelpMeListManager.AddHelpMeObj(GetTargetHandle(), GetID());
				m_cHelpMeListManager.SendBotCautionHelpMe(GetTargetHandle(), true);
			}
		}
	}
}

void CNpc::HelpDefend()
{
	if (GetAllianceID() == INVALID_ALLIANCEID)
	{
		ERR_LOG(LOG_SYSTEM, "Tblidx %u ObjType %u has invalid alliance id", GetTblidx(), GetObjType());
	}
	else
	{
		CObjMsg_AllianceYouHelpMe pMsg;
		pMsg.hRequester = GetID();
		AllianceBroadcast(&pMsg, false, this, 0);

		m_cHelpMeListManager.SendBotCautionHelpMe(GetTargetHandle(), true);
	}
}


void CNpc::AllianceBroadcast(CObjectMsg *pObjectMsg, bool bPost, CCharacterObject *pExcept, DWORD dwDelayTime)
{
	CWorldCell* pWorldCell = GetCurWorldCell();
	if (pWorldCell)
	{
		CWorldCell::QUADPAGE page = pWorldCell->GetCellQuadPage(GetCurLoc());

		int nInfiniteLoop = 0;

		for (int dir = CWorldCell::QUADDIR_SELF; dir < CWorldCell::QUADDIR_COUNT; dir++)
		{
			CWorldCell* pCellSibling = pWorldCell->GetQuadSibling(page, (CWorldCell::QUADDIR)dir);
			if (pCellSibling)
			{
				for (CCharacter* pChar = (CCharacter*)pCellSibling->GetObjectList()->GetFirst(GetObjType());
					pChar;
					pChar = (CCharacter*)pCellSibling->GetObjectList()->GetNext(pChar->GetWorldObjectLinker()))
				{
					++nInfiniteLoop;
					if (nInfiniteLoop > 1000)
					{
						ERR_LOG(LOG_SYSTEM, "INFINITE LOOP DETECTED");
					}

					if (pExcept != pChar
						&& pChar->GetTargetListManager()->GetAggroCount() == 0 //check if fighting
						&& GetAllianceID() == pChar->GetAllianceID() //check alliance id
						&& IsInRange(pChar, GetTbldat()->wSight_Range)) //check range
					{
						if (pChar->IsFainting() || pChar->GetCharStateID() == CHARSTATE_SPAWNING) //check state
							continue;

						if (bPost)
							pChar->PostObjectMsg(pObjectMsg, dwDelayTime);
						else
							pChar->SendObjectMsg(pObjectMsg);
					}
				}
			}
		}
	}
	else ERR_LOG(LOG_BOTAI, "pWorldCell == NULL");
}


void CNpc::AllianceHelp(CObjectMsg *pObjectMsg, const BYTE byHelpMaxCnt, CCharacterObject *pExcept)
{
	CWorldCell* pWorldCell = GetCurWorldCell();
	if (pWorldCell)
	{
		CRangeCheck rangeCheck(this, CRangeCheck::eSEVERAL);
		CWorldCell::QUADPAGE page = pWorldCell->GetCellQuadPage(GetCurLoc());

		int nInfiniteLoop = 0;


		for (int dir = CWorldCell::QUADDIR_SELF; dir < CWorldCell::QUADDIR_COUNT; dir++)
		{
			CWorldCell* pCellSibling = pWorldCell->GetQuadSibling(page, (CWorldCell::QUADDIR)dir);
			if (pCellSibling)
			{
				for (CCharacter* pChar = (CCharacter*)pCellSibling->GetObjectList()->GetFirst(GetObjType());
					pChar;
					pChar = (CCharacter*)pCellSibling->GetObjectList()->GetNext(pChar->GetWorldObjectLinker()))
				{
					++nInfiniteLoop;
					if (nInfiniteLoop > 1000)
					{
						ERR_LOG(LOG_SYSTEM, "INFINITE LOOP DETECTED");
					}

					if (pExcept != pChar
						&& pChar->GetTargetListManager()->GetAggroCount() == 0 //check if fighting
						&& GetAllianceID() == pChar->GetAllianceID() //check alliance id
						&& IsInRange(pChar, GetTbldat()->wSight_Range + 15.0f)) //check range
					{
						if (pChar->IsFainting() || pChar->IsSpawning()) //check state
							continue;

						rangeCheck.AddBot(pChar);
					}
				}
			}
		}

		if (!rangeCheck.IsEmptySeveral())
		{
			std::vector<CCharacter*> rBotList;
			rBotList.reserve(byHelpMaxCnt);

			rangeCheck.ResultSeveralBot(rBotList, byHelpMaxCnt);
			for (int i = 0; i < rBotList.size(); i++)
			{
				rBotList[i]->SendObjectMsg(pObjectMsg);
			}

			printf("CNpc::AllianceHelp Result Count %zu\n", rBotList.size());
		}
	}
	else ERR_LOG(LOG_BOTAI, "pWorldCell == NULL");
}



CNpc* CNpc::GetPartyLeader(HOBJECT hLeader, bool* bIsMePartyLeader)
{
	*bIsMePartyLeader = false;
	HOBJECT handle = hLeader;

	if (handle == INVALID_HOBJECT)
	{
		CNpcParty* pParty = GetNpcParty();
		if (!pParty)
			return NULL;

		handle = pParty->GetLeaderHandle();
	}

	if (handle == INVALID_HOBJECT)
		return NULL;
	else if (handle == GetID())
	{
		*bIsMePartyLeader = true;
		return NULL;
	}
	else
	{
		CNpc* pTarget = g_pObjectManager->GetNpc(handle);
		if (pTarget)
		{
			if (pTarget->IsSummonablePartyMember())
				return pTarget;
		}
	}

	return NULL;
}

bool CNpc::IsSummonablePartyMember()
{
	BYTE byState = GetStateManager()->GetCharStateID();

	return byState > CHARSTATE_DESPAWNING && byState != CHARSTATE_FAINTING && byState != CHARSTATE_LEAVING && byState != CHARSTATE_TELEPORTING;
}


//--------------------------------------------------------------------------------------//
//	
//--------------------------------------------------------------------------------------//
bool CNpc::IsTargetAttackState(CCharacter* pTarget)
{
	if (pTarget)
	{
		BYTE byStateID = pTarget->GetStateManager()->GetCharStateID();
		return byStateID != CHARSTATE_DIRECT_PLAY && byStateID != CHARSTATE_RIDEON;
	}

	return false;
}

//--------------------------------------------------------------------------------------//
//	
//--------------------------------------------------------------------------------------//
bool CNpc::CanSeeOnLineOfSight(CCharacter* pTarget)
{
	CGameServer* app = (CGameServer*)g_pApp;

	if (GetCurWorld())
	{
		if (app->m_config.m_bEnableNavigator == false)
			return true;

		if (GetNaviEngine()->IsPathDataLoaded(GetCurWorld()->GetTbldat()->dwWorldResourceID))
		{
			float fAgentRadius = Dbo_ConvertToAgentRadius(GetObjectRadius());

			sNAVI_POS src(GetCurLoc().x, GetCurLoc().y, GetCurLoc().z);
			sNAVI_POS dest(pTarget->GetCurLoc().x, pTarget->GetCurLoc().y, pTarget->GetCurLoc().z);

			eCOL_TEST_RESULT eResult = GetNaviEngine()->FastCanMoveNearestDest(GetCurWorld()->GetNaviInstanceHandle(), fAgentRadius, src, dest);

			switch (eResult)
			{
				case eCOL_TEST_RESULT_NO_COL: return true; break;

				default: return false; break;
			}
		}

		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------//
//	UN/SET ACTION STATUS FLAG
//--------------------------------------------------------------------------------------//
void CNpc::SetActionStatusFlag(eBOT_ACTIONSTATUS eActionStatus, bool bSet)
{
	if (bSet)
	{
		BIT_FLAG_SET(m_nActionStatusFlag, MAKE_BIT_FLAG(eActionStatus));
	}
	else
	{
		BIT_FLAG_UNSET(m_nActionStatusFlag, MAKE_BIT_FLAG(eActionStatus));
	}
}
//--------------------------------------------------------------------------------------//
//	CHECK ACTION STATUS FLAG
//--------------------------------------------------------------------------------------//
bool CNpc::IsActionStatusFlag(eBOT_ACTIONSTATUS eActionStatus)
{
	return BIT_FLAG_TEST(m_nActionStatusFlag, MAKE_BIT_FLAG(eActionStatus));
}

//--------------------------------------------------------------------------------------//
//	CHECK ACTION STATUS FLAG
//--------------------------------------------------------------------------------------//
CNtlVector& CNpc::GetBattleNestLoc()
{
	if (IsActionStatusFlag(BOT_ACTIONSTATUS_PLAYSCENE))
	{
		return GetFirstBattleLoc();
	}
	else
	{
		if (GetMoveType() <= SPAWN_MOVE_WANDER)
		{
			return GetEnterLoc();
		}
		else if (GetMoveType() > SPAWN_MOVE_WANDER && GetMoveType() <= SPAWN_MOVE_PATH_ONCE)
		{
			return GetFirstBattleLoc();
		}
	}

	return GetCurLoc();
}

float CNpc::GetAttackRange(CCharacter * pTarget)
{
	float fAttackRange = CCharacterObject::GetAttackRange();

	fAttackRange += CCharacter::GetAttackRange(pTarget);

	return fAttackRange;
}


//--------------------------------------------------------------------------------------//
//	SET TARGET ID
//--------------------------------------------------------------------------------------//
bool CNpc::SetTargetHandle(HOBJECT hHandle)
{
	HOBJECT hOldTarget = GetTargetHandle();
	if (CCharacterObject::SetTargetHandle(hHandle))
	{
		CCharacter* pTarget = g_pObjectManager->GetChar(hOldTarget);
		if (pTarget)
		{
			pTarget->GetBotBreakUp()->UnsetBreakUp(GetID());
		}

		return true;
	}

	return false;
}

void CNpc::UpdateNickName(TBLIDX nickNameIdx)
{
	bot_profile.nicknameTblidx = nickNameIdx;

	CNtlPacket packet(sizeof(sGU_CHANGED_CHAR_NICKNAME_NFY));
	sGU_CHANGED_CHAR_NICKNAME_NFY * res = (sGU_CHANGED_CHAR_NICKNAME_NFY *)packet.GetPacketData();
	res->wOpCode = GU_CHANGED_CHAR_NICKNAME_NFY;
	res->hSubject = GetID();
	res->nicknameTblidx = nickNameIdx;
	packet.SetPacketLen(sizeof(sGU_CHANGED_CHAR_NICKNAME_NFY));
	Broadcast(&packet);
}

bool CNpc::IsPerceiveEnergy(CCharacter * pTarget)
{
	if (pTarget)
	{
		switch (pTarget->GetCharStateID())
		{
			case CHARSTATE_DASH_PASSIVE:
			case CHARSTATE_FOCUSING:
			case CHARSTATE_CASTING:
			case CHARSTATE_SKILL_AFFECTING:
			case CHARSTATE_KEEPING_EFFECT:
			case CHARSTATE_CASTING_ITEM:
			case CHARSTATE_HTB:
			case CHARSTATE_CHARGING:
			case CHARSTATE_GUARD:
				return true;
				break;

			default: return false; break;
		}
	}

	return false;
}


//--------------------------------------------------------------------------------------//
//	
//--------------------------------------------------------------------------------------//
bool CNpc::RefreshActionPatternData(TBLIDX actionPatternTblidx)
{
	m_cActionPattern.Destroy();

	if (g_pActionPatternSystem->GetContainerActionPatternSet()->Add(this, actionPatternTblidx))
	{
		return m_cActionPattern.Create(this, g_pActionPatternSystem->GetContainerActionPatternSet()->Find(GetTblidx(), actionPatternTblidx));
	}

	ERR_LOG(LOG_BOTAI, "action pattern add fail. npc/mob tblidx[%u] pattern tblidx[%u]", GetTblidx(), actionPatternTblidx);
	return false;
}


//--------------------------------------------------------------------------------------//
//	
//--------------------------------------------------------------------------------------//
void CNpc::LoadSkillTable(TBLIDX tblidxOnlyOneSkillUse)
{
	CSkillManagerBot* pSkillManager = (CSkillManagerBot*)GetSkillManager();
	pSkillManager->RemoveSkillAll();

	sBOT_TBLDAT* pTbldat = (sBOT_TBLDAT*)CCharacter::GetTbldat();

	for (int i = 0; i < NTL_MAX_NPC_HAVE_SKILL; i++)
	{
		if (pTbldat->use_Skill_Tblidx[i] != INVALID_TBLIDX
			&& (tblidxOnlyOneSkillUse == INVALID_TBLIDX || tblidxOnlyOneSkillUse == pTbldat->use_Skill_Tblidx[i]))
		{
			CSkillBot* pSkill = new CSkillBot;

			sSKILL_TBLDAT* pSkillTbldat = (sSKILL_TBLDAT*)g_pTableContainer->GetSkillTable()->FindData(pTbldat->use_Skill_Tblidx[i]);
			if (pSkillTbldat)
			{
				if (pSkill->Create(pSkillTbldat, this, INVALID_BYTE))
				{
					if (tblidxOnlyOneSkillUse == INVALID_TBLIDX)
					{
						pSkillManager->AddSkill(i, this, pSkill, pTbldat->use_Skill_Tblidx[i], pTbldat->byUse_Skill_Basis[i], pTbldat->wUse_Skill_LP[i], pTbldat->wUse_Skill_Time[i]);
					}
					else
					{
						pSkillManager->AddSkill(i, this, pSkill, pTbldat->use_Skill_Tblidx[i], 5, pTbldat->wUse_Skill_LP[i], 5);
					}
				}
				else
					ERR_LOG(LOG_GENERAL, "fail : skill create fail. skill tblidx[%u]", pTbldat->use_Skill_Tblidx[i]);
			}
			else
				ERR_LOG(LOG_GENERAL, "fail : Can not find Skill tbldat. index[%u] skill tblidx[%u]", i, pTbldat->use_Skill_Tblidx[i]);
		}
	}
}


//--------------------------------------------------------------------------------------//
//	
//--------------------------------------------------------------------------------------//
HOBJECT CNpc::RemoveTargetAndPartyHelpList(HOBJECT hTarget)
{
	CNpcParty* pParty = GetNpcParty();
	if (pParty)
	{
		pParty->DeleteHelpList(hTarget, GetID());

		if (pParty->IsHelpTarget(hTarget))
		{
			m_cHelpMeListManager.SendBotCautionHelpMe(hTarget, false);
			return hTarget;
		}
	}
	
	GetTargetListManager()->RemoveAggro(hTarget);
	m_cHelpMeListManager.DeleteHelpMeObj(hTarget);
	if (hTarget == GetTargetHandle())
		ChangeTarget(INVALID_HOBJECT);

	return INVALID_HOBJECT;
}

//--------------------------------------------------------------------------------------//
//	
//--------------------------------------------------------------------------------------//
bool CNpc::IsAttacked(DWORD dwAttackTime)
{
	CGameServer* app = (CGameServer*)g_pApp;

	if (GetTickCount() <= dwAttackTime + this->m_dwLastAttackedTime)
		return m_bIsAttacked;

	return true;
}

void CNpc::SetAttacked(bool bAttacked, DWORD dwLastAttackedTime)
{
	m_bIsAttacked = bAttacked;
	m_dwLastAttackedTime = dwLastAttackedTime;
}


//--------------------------------------------------------------------------------------//
//	
//--------------------------------------------------------------------------------------//
bool CNpc::Faint(CCharacterObject* pkKiller, eFAINT_REASON byReason)
{
	if (CCharacterObject::Faint(pkKiller, byReason))
	{
		CCharacter::Faint(pkKiller, byReason);

		return true;
	}
	
	return false;
}


//--------------------------------------------------------------------------------------//
//	
//--------------------------------------------------------------------------------------//
float CNpc::GetTargetAndMeBodySizeRange(CCharacter* pTarget)
{
	if (pTarget)
	{
		return GetObjectRadius() + pTarget->GetObjectRadius();
	}

	return INVALID_FLOAT;
}

//--------------------------------------------------------------------------------------//
//	
//--------------------------------------------------------------------------------------//
bool CNpc::HasFunction(DWORD dwFuncFlag)
{
	return BIT_FLAG_TEST(dwFuncFlag, GetTbldat()->dwFunc_Bit_Flag);
}

bool CNpc::HasAttribute(DWORD dwFuncFlag)
{
	return BIT_FLAG_TEST(dwFuncFlag, GetTbldat()->dwNpcAttributeFlag);
}


bool CNpc::HasNearbyPlayer(bool bQuadPageCheck)
{
	if (bQuadPageCheck)
		return HasNearbyPlayerInQuadArea();

	return HasNearbyPlayerInArea();
}

bool CNpc::HasNearbyPlayerInQuadArea()
{
	CGameServer* app = (CGameServer*)g_pApp;

	if (app->GetCurTickCount() - m_dwLastHasNearbyPlayerQuadTime >= 1000)
	{
		m_bCacheHasNearbyPlayerQuad = false;
		m_dwLastHasNearbyPlayerQuadTime = app->GetCurTickCount();

		CWorldCell* pWorldCell = GetCurWorldCell();

		if (!pWorldCell)
			return m_bCacheHasNearbyPlayerQuad;

		CWorldCell::QUADPAGE page = pWorldCell->GetCellQuadPage(GetCurLoc());
		for (int dir = CWorldCell::QUADDIR_SELF; dir < CWorldCell::QUADDIR_COUNT; dir++)
		{
			CWorldCell* pSibling = pWorldCell->GetQuadSibling(page, (CWorldCell::QUADDIR)dir);
			if (pSibling)
			{
				if (pSibling->GetObjectList()->GetObjCount(OBJTYPE_PC))
				{
					m_bCacheHasNearbyPlayerQuad = true;
					break;
				}
			}
		}
	}

	return m_bCacheHasNearbyPlayerQuad;
}

bool CNpc::HasNearbyPlayerInArea()
{
	bool bRet = false;

	if (GetCurWorldCell())
		bRet = GetCurWorldCell()->GetNearbyPlayerCount() != 0;

	return bRet;
}




//--------------------------------------------------------------------------------------//
//	
//--------------------------------------------------------------------------------------//
bool CNpc::HasAIFunction(DWORD dwAIBitFlag)
{
	return BIT_FLAG_TEST(dwAIBitFlag, m_dwAi_Bit_Flag);
}

bool CNpc::IsChaseAble()
{
	if (HasAIFunction(AI_FUNC_FLAG_NOT_CHASE))
		return false;

	return CCharacter::IsChaseAble();
}
bool CNpc::IsMoveAble()
{
	if (HasAIFunction(AI_FUNC_FLAG_NOT_MOVE))
		return false;

	return CCharacter::IsMoveAble();
}
bool CNpc::IsFleeAble()
{
	if (HasAIFunction(AI_FUNC_FLAG_FLEE))
	{
		if (HasAIFunction(AI_FUNC_FLAG_NOT_MOVE))
			return false;

		return CCharacter::IsFleeAble();
	}
	
	return false;
}


bool CNpc::IsInSightRange(CCharacter* pChar)
{
	if (pChar->GetAirState() == AIR_STATE_ON)
	{
		if (IsInRange3(pChar, (float)GetTbldat()->wSight_Range))
			return true;
	}
	else
	{
		if (IsInRange(pChar, (float)GetTbldat()->wSight_Range))
			return true;
	}

	return false;
}


int CNpc::OnMsg_YouKeepAggro(CObjectMsg * pObjectMsg)
{
	CObjMsg_YouKeepAggro* pMsg = dynamic_cast<CObjMsg_YouKeepAggro*>(pObjectMsg);
	if (pMsg)
	{
		printf("OnMsg_YouKeepAggro \n");
		GetTargetListManager()->AddAggro(pMsg->hProvoker, pMsg->dwAggroPoint, false);
	}

	return NTL_SUCCESS;
}

int CNpc::OnMsg_YouHit(CObjectMsg * pObjectMsg)
{
	return NTL_SUCCESS; //TODO
}

int CNpc::OnMsg_YouHitBySkill(CObjectMsg * pObjectMsg)
{


	return NTL_SUCCESS;
}

int CNpc::OnMsg_YouHitHTB(CObjectMsg * pObjectMsg)
{
	return NTL_SUCCESS;
}

int CNpc::OnMsg_YouHelpMe(CObjectMsg * pObjectMsg)
{
	CObjMsg_YouHelpMe* pMsg = dynamic_cast<CObjMsg_YouHelpMe*>(pObjectMsg);
	if (pMsg)
	{
		GetTargetListManager()->AddAggro(pMsg->hTarget, pMsg->dwAggroPoint, true);

		CNpcParty* pParty = GetNpcParty();
		if (pParty)
			pParty->AddHelpList(GetID(), pMsg->hTarget, pMsg->dwAggroPoint);

		m_cHelpMeListManager.AddHelpMeObj(pMsg->hTarget, pMsg->hSource);
	}

	return NTL_SUCCESS;
}

int CNpc::OnMsg_AllianceYouHelpMe(CObjectMsg * pObjectMsg)
{
	CObjMsg_AllianceYouHelpMe* pMsg = dynamic_cast<CObjMsg_AllianceYouHelpMe*>(pObjectMsg);
	if (pMsg)
	{
		CCharacter* pRequester = g_pObjectManager->GetChar(pMsg->hRequester);
		if (pRequester)
		{
			if (pRequester->GetTargetListManager())
			{
				DWORD dwAggroPoint = GetTbldat()->wBasic_Aggro_Point + 1;

				GetTargetListManager()->AddAggro(pRequester->GetTargetHandle(), dwAggroPoint, true);
				m_cHelpMeListManager.AddHelpMeObj(pRequester->GetTargetHandle(), pRequester->GetID());
			}
			else
				ERR_LOG(LOG_BOTAI, "fail : NULL == pRequester->GetTargetListManager()");
		}
	}

	return NTL_SUCCESS;
}



CObjectController* CNpc::CreateObjectController()
{
	CBotAiController* pAiController = new CBotAiController;
	if (pAiController)
	{
		if (pAiController->Create(this))
			return pAiController;
		else
			ERR_LOG(LOG_BOTAI, "BotAiController Create fail. ObjType(%d) Handle(%d) Tblidx(%u) WorldID(%d) CurLoc(%f,%f,%f)", GetObjType(), GetID(), GetTblidx(), GetWorldTblidx(), GetCurLoc().x, GetCurLoc().y, GetCurLoc().z);
	}
	else
		ERR_LOG(LOG_BOTAI, "new CBotAiController failed");

	return NULL;
}


bool CNpc::PrepareControlStateManager()
{
	return m_cControlStateManager.Create(this);
}

bool CNpc::PrepareHelpMeListManager()
{
	return m_cHelpMeListManager.Create(this);
}

CEscortManager* CNpc::CreateEscortManager()
{
	CEscortManager* pEscortManager = new CEscortManager;
	if(pEscortManager->Create(this))
		return pEscortManager;

	ERR_LOG(LOG_BOTAI, "pEscortManager == NULL");
	return NULL;
}


void CNpc::SetPlayScript(TBLIDX playScript, TBLIDX playScriptScene)
{
	bot_profile.sScriptData.playScript = playScript;
	bot_profile.sScriptData.playScriptScene = playScriptScene;
}


bool CNpc::ConsiderNavLoc(CNtlVector& rDestLoc)
{
	if (m_pPathFinder->GetCurNavLoc(rDestLoc))
	{
		if (!m_pPathFinder->IsArrived())
			return m_pPathFinder->GetNextNavLoc(rDestLoc);
	}

	return false;
}

bool CNpc::ConsiderWanderLoc(CNtlVector& rDestLoc)
{
	if (GetWanderRange())
	{
		bool IsMePartyLeader = false;

		CNpcParty* pParty = GetNpcParty();
		if (!pParty || GetPartyLeader(INVALID_HOBJECT, &IsMePartyLeader) != false || IsMePartyLeader)
		{
			return ConsiderWanderLoc(bot_profile.vSpawnLoc, rDestLoc);
		}
		else
		{
			return ConsiderWanderLoc(m_vecFirstBattleLoc, rDestLoc);
		}
	}
	else
	{
		return true;
	}

	return false;
}


bool CNpc::AttackProgress(DWORD dwTickDiff, float fMultiple)
{
	if (CCharacterObject::AttackProgress(dwTickDiff, fMultiple))
	{
		CGameServer* app = (CGameServer*)g_pApp;

		CCharacter* pVictim = g_pObjectManager->GetChar(GetTargetHandle());
		if (!pVictim || !pVictim->IsInitialized())
			return false;

		AttackAction(pVictim, true);

		float fAnimationTime = (float)GetAniTbldat()->GetNpcAttackDurationTime(GetAttackType());
		float fAttackAnimationSpeed = (GetAttackSpeedRate() == 0) ? 1.0f : 1000.0f / (float)GetAttackSpeedRate();
		float fRequiredAttackAnimationTime = NTL_BATTLE_ATTACK_DELAY_TIME + (fAnimationTime / fAttackAnimationSpeed);

		m_dwNextAttackTime = app->GetCurTickCount() + (DWORD)fRequiredAttackAnimationTime;

	//	printf("fAnimationTime %f, fAttackAnimationSpeed %f, fRequiredAttackAnimationTime %f, GetAttackSpeedRate() %u\n", fAnimationTime, fAttackAnimationSpeed, fRequiredAttackAnimationTime, GetAttackSpeedRate());
		return true;
	}

	return false;
}


bool CNpc::OnAttackAction(CCharacter* pAttacker, int nDmg, BYTE byAttackResult,eFAINT_REASON eReason/* = FAINT_REASON_HIT*/)
{
	if (pAttacker)
	{
		CGameServer* app = (CGameServer*)g_pApp;

		GetTargetListManager()->SetAggroLastUpdateTime();
		SetAttacked(true, app->GetCurTickCount());

		return CCharacter::OnAttackAction(pAttacker, nDmg, byAttackResult, eReason);
	}

	return true;
}


bool CNpc::OnSkillAction(CCharacter* pAttacker, int nDmg, DWORD dwAggroPoint, BYTE byAttackResult, bool bWakeUpTarget)
{
	if (pAttacker)
	{
		CGameServer* app = (CGameServer*)g_pApp;

		GetTargetListManager()->SetAggroLastUpdateTime();
		SetAttacked(true, GetTickCount());

		return CCharacter::OnSkillAction(pAttacker, nDmg, dwAggroPoint, byAttackResult, bWakeUpTarget);
	}

	return true;
}


//--------------------------------------------------------------------------------------//
//	CHANGE AGGRO
//--------------------------------------------------------------------------------------//
void CNpc::ChangeAggro(HOBJECT hProvoker, BYTE byAggroChangeType, DWORD dwValue)
{
	if (IsActionStatusFlag(BOT_ACTIONSTATUS_DOING_RETURN))
		return;

	CCharacter::ChangeAggro(hProvoker, byAggroChangeType, dwValue);
}


bool CNpc::ConsiderWanderLoc(CNtlVector& rStandardLoc, CNtlVector& rDestLoc)
{
	CNtlVector vMove;

	rDestLoc.x = RandomRangeF(rStandardLoc.x - GetWanderRange(), rStandardLoc.x + GetWanderRange());
	rDestLoc.y = rStandardLoc.y;
	rDestLoc.z = RandomRangeF(rStandardLoc.z - GetWanderRange(), rStandardLoc.z + GetWanderRange());


	vMove = rDestLoc - GetCurLoc();

	if (vMove.Length() >= GetMoveRange())
	{
		vMove.y = 0;

		if (!vMove.SafeNormalize())
			return false;

		vMove *= GetMoveRange();
		vMove += GetCurLoc();

		rDestLoc = vMove;
	}

	//printf("ConsiderWanderLoc cur loc(%f %f %f) : result %f %f %f \n", GetCurLoc()->x, GetCurLoc()->y, GetCurLoc()->z, rDestLoc->x, rDestLoc->y, rDestLoc->z);

	return true;
}

bool CNpc::ConsiderPatrolLoc(CNtlVector& rDestLoc)
{
	if (GetMoveType() != 2)
		return false;

	if (m_pPathFinder->IsArrived())
	{
		if (!m_pPathFinder->GetNextWayPoint(rDestLoc))
			return false;
	}
	else if (!m_pPathFinder->GetCurWayPoint(rDestLoc))
	{
		return false;
	}

	m_pPathFinder->SetDestLoc(rDestLoc);

	if (m_pPathFinder->PathFind())
	{
		return m_pPathFinder->GetCurNavLoc(rDestLoc);
	}
	else
	{
		ERR_LOG(LOG_BOTAI, "PathFind() failed., GetWorldID() = %u, GetCurLoc() = (%f, %f, %f), rDestLoc = (%f, %f, %f)", GetWorldID(), GetCurLoc().x, GetCurLoc().y, GetCurLoc().z, rDestLoc.x, rDestLoc.y, rDestLoc.z);
	}

	return false;
}

bool CNpc::ConsiderFlee(CNtlVector& rDestLoc)
{
	CCharacter* pTarget = g_pObjectManager->GetChar(GetMeAttackBot());
	if (pTarget)
	{
		ResetMeAttackBot();
		if (Dbo_CheckProbability(60))
		{
			CNtlVector vFleeDir(GetCurLoc().operator-(pTarget->GetCurLoc()));
			vFleeDir.y = 0.0f;
			if (vFleeDir.SafeNormalize())
			{
				vFleeDir.operator*=(5.0f);
				rDestLoc.operator=(GetCurLoc().operator+(vFleeDir));
				return true;
			}
		}
	}

	return false;
}

bool CNpc::ConsiderTurnDir(CNtlVector& rDestDir)
{
	if (GetMoveType())
	{
		return false;
	}
	else
	{
		if (GetEnterDir().operator!=(GetCurDir()))
		{
			rDestDir.operator=(GetEnterDir());
			return true;
		}
	}

	return false;
}


HOBJECT CNpc::ConsiderAttackTarget(DWORD dwTickTime)
{
	if (m_pStateManager->IsCharCondition(CHARCOND_TAUNT) && GetTargetListManager()->GetTauntTarget() != INVALID_HOBJECT)
	{
		if (IsTargetAttackble(GetTargetListManager()->GetTauntTarget(), GetTbldat()->wSight_Range))
		{
			std::list<CNtlVector> rlistCollisionPos;
			if (IsReachable(GetTargetListManager()->GetTauntTarget(), rlistCollisionPos))
			{
				ResetCheatingTargetInfo();
				return GetTargetListManager()->GetTauntTarget();
			}

			if (rlistCollisionPos.size() > 0)
			{
				IncreaseChasingCheatingTargetInfo(dwTickTime, rlistCollisionPos);

				if (m_dwElapsedTimeSinceCheatingTargetSet < 10000)
				{
					return GetTargetListManager()->GetTauntTarget();
				}
			}
		}

		RemoveTargetAndPartyHelpList(GetTargetListManager()->GetTauntTarget());
		ResetCheatingTargetInfo();
		GetTargetListManager()->SetTauntTarget(INVALID_HOBJECT);
	}

	if (m_pStateManager->IsCharCondition(CHARCOND_CONFUSED))
		return GetTargetInConfusion();

	if (GetTargetListManager()->GetFixedTarget() != INVALID_HOBJECT)
	{
		if (IsTargetAttackble(GetTargetListManager()->GetFixedTarget(), GetTbldat()->wSight_Range))
		{
			std::list<CNtlVector> rlistCollisionPos;
			if (IsReachable(GetTargetListManager()->GetFixedTarget(), rlistCollisionPos))
			{
				ResetCheatingTargetInfo();
				return GetTargetListManager()->GetFixedTarget();
			}

			if (rlistCollisionPos.size() > 0)
			{
				IncreaseChasingCheatingTargetInfo(dwTickTime, rlistCollisionPos);

				if (m_dwElapsedTimeSinceCheatingTargetSet < 10000)
				{
					return GetTargetListManager()->GetFixedTarget();
				}
			}
		}
	}

	m_dwChangeAttackTargetDelayTime = UnsignedSafeIncrease<DWORD>(m_dwChangeAttackTargetDelayTime, dwTickTime);

	HOBJECT hTarget = INVALID_HOBJECT;
	WORD wSight_Range = GetTbldat()->wSight_Range;

	if (m_bIsChangeTargetWait && GetTargetHandle() != INVALID_HOBJECT)
	{
		if (m_dwChangeAttackTargetDelayTime < 5000)
		{
			if (IsTargetAttackble(GetTargetHandle(), wSight_Range))
			{
				std::list<CNtlVector> rlistCollisionPos;
				if (IsReachable(GetTargetHandle(), rlistCollisionPos))
				{
					ResetCheatingTargetInfo();
					return GetTargetHandle();
				}

				if (rlistCollisionPos.size() > 0)
				{
					IncreaseChasingCheatingTargetInfo(dwTickTime, rlistCollisionPos);

					if (m_dwElapsedTimeSinceCheatingTargetSet < 10000)
					{
						return GetTargetHandle();
					}
				}
			}
			
			RemoveTargetAndPartyHelpList(GetTargetHandle());
			ResetCheatingTargetInfo();
		}
		else
		{
			m_bIsChangeTargetWait = false;
		}
	}


	if (GetTargetListManager()->GetAggroCount() == 0)
	{
		return INVALID_HOBJECT;
	}

	hTarget = SelectTarget_AI_Type();
	if (hTarget == INVALID_HOBJECT)
	{
		ResetCheatingTargetInfo();
		return INVALID_HOBJECT;
	}

	if (!IsTargetAttackble(hTarget, wSight_Range))
	{
		HOBJECT hRemoveTarget = RemoveTargetAndPartyHelpList(hTarget);
		if (hRemoveTarget != INVALID_HOBJECT)
			return hRemoveTarget;

		ResetCheatingTargetInfo();
	}

	std::list<CNtlVector> rlistCollisionPos;
	if (!IsReachable(hTarget, rlistCollisionPos))
	{
		if (rlistCollisionPos.size() > 0)
		{
			IncreaseChasingCheatingTargetInfo(dwTickTime, rlistCollisionPos);

			if (m_dwElapsedTimeSinceCheatingTargetSet < 10000)
			{
				return hTarget;
			}
		}

		HOBJECT hRemoveTarget = RemoveTargetAndPartyHelpList(hTarget);
		if (hRemoveTarget != INVALID_HOBJECT)
			return hRemoveTarget;
	}

	ResetCheatingTargetInfo();

	if (GetTargetHandle() != hTarget)
	{
		m_dwChangeAttackTargetDelayTime = 0;
		m_bIsChangeTargetWait = true;
	}

	m_cHelpMeListManager.SendBotCautionHelpMe(hTarget, false);
	return hTarget;
}


//--------------------------------------------------------------------------------------//
//	
//--------------------------------------------------------------------------------------//
HOBJECT CNpc::SelectTarget_AI_Type()
{
	if (HasAIFunction(AI_FUNC_FLAG_FOCUS))
	{
		if (GetTargetHandle() == INVALID_HOBJECT)
		{
			return GetTargetListManager()->GetHighestAggroTarget();
		}
		else
		{
			return GetTargetHandle();
		}
	}
	else if (HasAIFunction(AI_FUNC_FLAG_MEAN))
	{
		return GetTargetListManager()->GetMeanTarget();
	}
	else if (HasAIFunction(AI_FUNC_FLAG_BRAVE))
	{
		return GetTargetListManager()->GetBraveTarget();
	}

	return GetTargetListManager()->GetHighestAggroTarget();
}


HOBJECT CNpc::ConsiderScanTarget(WORD wRange)
{
	CWorldCell* pWorldCell = GetCurWorldCell();
	if (!pWorldCell)
		return INVALID_HOBJECT;

	WORD wScanRange = wRange;
	if (wScanRange == INVALID_WORD)
		wScanRange = GetTbldat()->wScan_Range;

	CRangeCheck pRangeCheck(this, CRangeCheck::eSINGLENESS);

	int nLoopCount = 0;

	CWorldCell::QUADPAGE page = pWorldCell->GetCellQuadPage(GetCurLoc());
	for (int dir = CWorldCell::QUADDIR_SELF; dir <= CWorldCell::QUADDIR_VERTICAL; dir++)
	{
		CWorldCell* pWorldCellSibling = pWorldCell->GetQuadSibling(page, (CWorldCell::QUADDIR)dir);
		if (pWorldCellSibling)
		{
			CCharacter* pTarget = (CCharacter*)pWorldCellSibling->GetObjectList()->GetFirst(OBJTYPE_MOB);
			while (pTarget && pTarget->IsInitialized())
			{
				++nLoopCount;
				if (nLoopCount > 5000)
				{
					ERR_LOG(LOG_GENERAL, "INFINITE LOOP FOUND");

					if (pTarget)
					{
						ERR_LOG(LOG_GENERAL, "Target init? %i, pTarget->GetID(): %u, wScanRange: %u", pTarget->IsInitialized(), pTarget->GetID(), wScanRange);
					}

					break;
				}

				if (IsTargetAttackble(pTarget, wScanRange))
				{
					if (IsTargetAttackState(pTarget))
					{
						if (CanSeeOnLineOfSight(pTarget))
						{
							pRangeCheck.AddBot(pTarget);
						}
					}
				}
				break; // TEMP !! FIND OUT WHY INFINITE LOOP
				pTarget = (CCharacter*)pWorldCellSibling->GetObjectList()->GetNext(pTarget->GetWorldCellObjectLinker());
			}
		}
	}

	CCharacter* pResult = pRangeCheck.ResultSinglenessBot();
	if (pResult)
		return pResult->GetID();

	return INVALID_HOBJECT;
}


HOBJECT CNpc::ConsiderScanObject(TBLIDX tblidx, WORD wRange)
{
	CWorldCell* pWorldCell = GetCurWorldCell();
	if (!pWorldCell)
		return INVALID_HOBJECT;

	CRangeCheck pRangeCheck(this, CRangeCheck::eSINGLENESS);

	WORD wScanRange = wRange;
	if (wScanRange == INVALID_WORD)
		wScanRange = GetTbldat()->wScan_Range;

	int nLoopCount = 0;
	int nLoopCount2 = 0;

	CWorldCell::QUADPAGE page = pWorldCell->GetCellQuadPage(GetCurLoc());
	for (int dir = CWorldCell::QUADDIR_SELF; dir <= CWorldCell::QUADDIR_VERTICAL; dir++)
	{
		CWorldCell* pWorldCellSibling = pWorldCell->GetQuadSibling(page, (CWorldCell::QUADDIR)dir);
		if (pWorldCellSibling)
		{
			CCharacter* pTarget = (CCharacter*)pWorldCellSibling->GetObjectList()->GetFirst(OBJTYPE_MOB);
			while (pTarget && pTarget->IsInitialized())
			{
				++nLoopCount;
				if (nLoopCount > 5000)
				{
					ERR_LOG(LOG_GENERAL, "INFINITE LOOP FOUND");
				}

				if (pTarget->GetTblidx() == tblidx)
				{
					if (IsTargetAttackble(pTarget, wScanRange))
					{
						if (CanSeeOnLineOfSight(pTarget))
						{
							pRangeCheck.AddBot(pTarget);
						}
					}
				}
				pTarget = (CCharacter*)pWorldCellSibling->GetObjectList()->GetNext(pTarget->GetWorldCellObjectLinker());
			}

			pTarget = (CCharacter*)pWorldCellSibling->GetObjectList()->GetFirst(OBJTYPE_NPC);
			while (pTarget && pTarget->IsInitialized())
			{
				++nLoopCount2;
				if (nLoopCount2 > 5000)
				{
					ERR_LOG(LOG_GENERAL, "INFINITE LOOP FOUND");
				}

				if (pTarget->GetTblidx() == tblidx)
				{
					if (IsTargetAttackble(pTarget, wScanRange))
					{
						if (CanSeeOnLineOfSight(pTarget))
						{
							pRangeCheck.AddBot(pTarget);
						}
					}
				}
				pTarget = (CCharacter*)pWorldCellSibling->GetObjectList()->GetNext(pTarget->GetWorldCellObjectLinker());
			}
		}
	}

	CCharacter* pResult = pRangeCheck.ResultSinglenessBot();
	if (pResult)
		return pResult->GetID();

	return INVALID_HOBJECT;
}


HOBJECT CNpc::ConsiderScanTargetFixedPc(WORD wRange)
{
	CWorldCell* pWorldCell = GetCurWorldCell();
	if (!pWorldCell)
		return INVALID_HOBJECT;

	CRangeCheck pRangeCheck(this, CRangeCheck::eSINGLENESS);

	WORD wScanRange = wRange;
	if (wScanRange == INVALID_WORD)
		wScanRange = GetTbldat()->wScan_Range;

	int nLoopCount = 0;

	CWorldCell::QUADPAGE page = pWorldCell->GetCellQuadPage(GetCurLoc());
	for (int dir = CWorldCell::QUADDIR_SELF; dir <= CWorldCell::QUADDIR_VERTICAL; dir++)
	{
		CWorldCell* pWorldCellSibling = pWorldCell->GetQuadSibling(page, (CWorldCell::QUADDIR)dir);
		if (pWorldCellSibling)
		{
			CCharacter* pTarget = (CCharacter*)pWorldCellSibling->GetObjectList()->GetFirst(OBJTYPE_PC);
			while (pTarget && pTarget->IsInitialized())
			{
				++nLoopCount;
				if (nLoopCount > 5000)
				{
					ERR_LOG(LOG_GENERAL, "INFINITE LOOP FOUND");
				}

				if (GetTargetListManager()->GetFixedTarget() == INVALID_HOBJECT)
					return INVALID_HOBJECT;

				if (GetTargetListManager()->GetFixedTarget() == pTarget->GetID())
				{
					if (IsTargetAttackble(pTarget, wScanRange))
					{
						if (CanSeeOnLineOfSight(pTarget))
						{
							pRangeCheck.AddBot(pTarget);
						}
					}
				}
				pTarget = (CCharacter*)pWorldCellSibling->GetObjectList()->GetNext(pTarget->GetWorldCellObjectLinker());
			}
		}
	}

	CCharacter* pResult = pRangeCheck.ResultSinglenessBot();
	if (pResult)
		return pResult->GetID();

	return INVALID_HOBJECT;
}


HOBJECT CNpc::ConsiderScanTargetRingRange(WORD wRingRange)
{
	CWorldCell* pWorldCell = GetCurWorldCell();
	if (!pWorldCell)
		return INVALID_HOBJECT;

	CRangeCheck pRangeCheck(this, CRangeCheck::eSINGLENESS);

	int nLoopCount = 0;

	CWorldCell::QUADPAGE page = pWorldCell->GetCellQuadPage(GetCurLoc());
	for (int dir = CWorldCell::QUADDIR_SELF; dir <= CWorldCell::QUADDIR_VERTICAL; dir++)
	{
		CWorldCell* pWorldCellSibling = pWorldCell->GetQuadSibling(page, (CWorldCell::QUADDIR)dir);
		if (pWorldCellSibling)
		{
			CCharacter* pTarget = (CCharacter*)pWorldCellSibling->GetObjectList()->GetFirst(OBJTYPE_MOB);
			while (pTarget && pTarget->IsInitialized())
			{
				++nLoopCount;
				if (nLoopCount > 5000)
				{
					ERR_LOG(LOG_GENERAL, "INFINITE LOOP FOUND");
				}

				if (IsTargetAttackble(pTarget, GetTbldat()->wSight_Range))
				{
					if (!IsTargetAttackble(pTarget, wRingRange))
					{
						if (CanSeeOnLineOfSight(pTarget))
						{
							pRangeCheck.AddBot(pTarget);
						}
					}
				}
				pTarget = (CCharacter*)pWorldCellSibling->GetObjectList()->GetNext(pTarget->GetWorldCellObjectLinker());
			}
		}
	}

	CCharacter* pResult = pRangeCheck.ResultSinglenessBot();
	if (pResult)
		return pResult->GetID();

	return INVALID_HOBJECT;
}


bool CNpc::ConsiderAttackRange(float fBreakupPos)
{
	CCharacter* pTarget = g_pObjectManager->GetChar(GetTargetHandle());
	if (pTarget)
	{
		if (pTarget->GetAirState() == AIR_STATE_ON)
		{
			if (IsInRange3(pTarget, GetAttackRange(pTarget) * (fBreakupPos + 1.0f) ))
				return true;
		}
		else
		{
			if (IsInRange(pTarget, GetAttackRange(pTarget) * (fBreakupPos + 1.0f)))
				return true;
		}
	}

	return false;
}


bool CNpc::ConsiderTargetSightRange()
{
	CCharacter* pTarget = g_pObjectManager->GetChar(GetTargetHandle());
	if (pTarget)
	{
		return IsInSightRange(pTarget);
	}

	return false;
}

bool CNpc::ConsiderSpawnRange()
{
	return IsInRange(GetEnterLoc(), (float)GetSpawnRange());
}


bool CNpc::ConsiderRangedAttack()
{
	CCharacter* pTarget = g_pObjectManager->GetChar(GetTargetHandle());
	if (pTarget)
	{
		if (IsInRange(pTarget, 2.0f))
		{
			return Dbo_CheckProbability(5) != false;
		}
	}

	return false;
}

bool CNpc::ConsiderRange(float fSkillAttackRange, float fBreakupPos)
{
	CCharacter* pTarget = g_pObjectManager->GetChar(GetTargetHandle());
	if (pTarget)
	{
		return IsInRange(pTarget, fSkillAttackRange* (fBreakupPos + 1.0f));
	}

	return false;
}


bool CNpc::ConsiderRetreatLoc(CNtlVector& rDestLoc, float fOffsetMin, float fOffsetMax)
{
	if (g_pObjectManager->GetChar(GetTargetHandle()))
	{
		float fOffset = RandomRangeF(fOffsetMin, fOffsetMax);
		CNtlVector vDestDir(GetEnterLoc().operator-(GetCurLoc()));
		vDestDir.y = 0.0f;

		if (vDestDir.SafeNormalize())
		{
			rDestLoc.operator=(GetCurLoc().operator+(vDestDir.operator*(fOffset)));
			return true;
		}
	}

	rDestLoc.Reset();
	return false;
}
	
bool CNpc::ConsiderRetreatFleeLoc(CNtlVector& rDestLoc, float fOffsetMin, float fOffsetMax)
{
	CCharacter* pTarget = g_pObjectManager->GetChar(GetTargetHandle());
	if (pTarget && pTarget->IsInitialized())
	{
		float fOffset = RandomRangeF(fOffsetMin, fOffsetMax);
		CNtlVector vDestDir(GetCurLoc() - pTarget->GetCurLoc());
		vDestDir.y = 0.0f;

		if (vDestDir.SafeNormalize())
		{
			rDestLoc.operator=(GetCurLoc() + (vDestDir * fOffset));
			return true;
		}
	}

	rDestLoc.Reset();
	return false;
}


bool CNpc::GetRangedAttackLoc(CNtlVector& rDestLoc)
{
	CCharacter* pTarget = g_pObjectManager->GetChar(GetTargetHandle());
	if (pTarget && pTarget->IsInitialized())
	{
		CNtlVector rhs(GetCurLoc().operator-(pTarget->GetCurLoc()));
		rhs.y = 0.0f;
		if (rhs.SafeNormalize())
		{
			rhs.operator*=(GetTbldat()->fAttack_Range / 2.0f);

			rDestLoc.operator=(GetCurLoc().operator+(rhs));
			return true;
		}
	}
	
	rDestLoc.Reset();
	return false;
}


BYTE CNpc::ConsiderPathMovement(CCharacter* pTarget, int* pnPathFindingResult)
{
	if (pTarget)
	{
		if (pTarget->GetCurWorld())
		{
			CGameServer* app = (CGameServer*)g_pApp;

			if (app->m_config.m_bEnableNavigator)
			{
				CNtlVector vNewDir;
				CNtlVector vNewLoc;
				CNtlVector vNewDestLoc;
				float fActualSpeed = 0.0f;
				std::list<sVECTOR3> listConsumedDestLoc;

				PredictMovement(CONSIDERING_NEXT_MOVEMENT_INTERVAL_IN_MILLISECS, vNewDir, vNewLoc, vNewDestLoc, &listConsumedDestLoc, &fActualSpeed);

				if (GetPathFinder()->IsValidPos(vNewLoc))
				{
					GetPathFinder()->SetDestLoc(vNewLoc);
					if (pnPathFindingResult)
					{
						*pnPathFindingResult = GetPathFinder()->PathFind();
					}
				}
				else if (pnPathFindingResult)
				{
					*pnPathFindingResult = 0;
				}

				if (*pnPathFindingResult > 0)
				{
					if (GetPathFinder()->GetNaviPositionCount() > 2)
						return CHARSTATE_DESTMOVE;
					else
						return CHARSTATE_FOLLOWING;
				}

				GetPathFinder()->Reset();
				GetPathFinder()->SetDestLoc(GetCurLoc());
				if (pnPathFindingResult)
				{
					*pnPathFindingResult = GetPathFinder()->PathFind();
				}

				if (*pnPathFindingResult > 0)
				{
					if (GetPathFinder()->GetNaviPositionCount() > 2)
						return CHARSTATE_DESTMOVE;
					else
						return CHARSTATE_FOLLOWING;
				}
				else
				{
					GetPathFinder()->Reset();
				}
			}
			else
			{
				return CHARSTATE_FOLLOWING;
			}
		}
	}

	return CHARSTATE_STANDING;
}


void CNpc::SetTimid(eTIMID_TYPE eTimidType, float fTimidRange)
{
	this->m_eTimidType = eTimidType;
	this->m_fTimidRange = fTimidRange;
}




void CNpc::ResetCheatingTargetInfo()
{
	m_bIsChasingCheatingTarget = false;
	m_dwElapsedTimeSinceCheatingTargetSet = 0;
}

void CNpc::IncreaseChasingCheatingTargetInfo(DWORD dwTickDiff, std::list<CNtlVector>& rlistCollisionPos)
{
	m_bIsChasingCheatingTarget = true;
	m_dwElapsedTimeSinceCheatingTargetSet = UnsignedSafeIncrease<DWORD>(m_dwElapsedTimeSinceCheatingTargetSet, dwTickDiff);

	if (rlistCollisionPos.size() > 0)
	{
		m_vCheatingTargetCollisionLoc.operator=(*rlistCollisionPos.begin());
	}
	else
	{
		if (GetCurWorld())
		{
			ERR_LOG(LOG_SYSTEM, "No collision location has been specified., GetID() = %u, GetTbldat()->tblidx = %u, GetCurWorld()->GetID() = %u, GetCurLoc() = (%f, %f, %f)", GetID(), GetTbldat()->tblidx, GetCurWorld()->GetID(), GetCurLoc().x, GetCurLoc().y, GetCurLoc().z);
		}
		else
		{
			ERR_LOG(LOG_SYSTEM, "No collision location has been specified., GetID() = %u, GetTbldat()->tblidx = %u, GetCurLoc() = (%f, %f, %f)", GetID(), GetTbldat()->tblidx, GetCurLoc().x, GetCurLoc().y, GetCurLoc().z);
		}
		m_vCheatingTargetCollisionLoc.operator=(GetCurLoc());
	}
}

