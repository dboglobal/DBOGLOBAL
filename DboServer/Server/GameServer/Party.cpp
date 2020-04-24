#include "stdafx.h"
#include "NtlPacketGT.h"
#include "NtlPacketGU.h"
#include "CPlayer.h" //include party.h
#include "PartyMatching.h"
#include "GameServer.h"
#include "UltimateDungeon.h"
#include "BattleDungeon.h"
#include "ScriptAlgoObject.h"
#include "RankBattle.h"
#include "TableContainerManager.h"
#include "ServerConfigTable.h"
#include "ItemManager.h"
#include "ItemDrop.h"
#include "DiceManager.h"



CPartyManager::CPartyManager()
{
	CGameServer* app = (CGameServer*)g_pApp;

	m_PartyHandler = 1 + (app->GetGsChannel() * NTL_MAX_PARTY_COUNT);	// CH 0 = party id 1 - 2000, CH1 = 1001 - 3000, CH2 = 2001 - 4000 ...

	m_map_Party.clear();
}

CPartyManager::~CPartyManager()
{
	for (std::map<PARTYID, CParty*>::iterator it = m_map_Party.begin(); it != m_map_Party.end(); it++)
	{
		CParty* pParty = it->second;
		SAFE_DELETE(pParty);
	}
	m_map_Party.clear();
}

void CPartyManager::TickProcess(DWORD dwTickDiff)
{
	for (std::map<PARTYID, CParty*>::iterator it = m_map_Party.begin(); it != m_map_Party.end(); it++)
	{
		CParty* pParty = it->second;
		if (pParty)
			pParty->TickProcess(dwTickDiff);
	}
}

//--------------------------------------------------------------------------------------//
//		CREATE PARTY AND ADD CREATOR TO PARTY
//--------------------------------------------------------------------------------------//
CParty*	CPartyManager::CreateParty(CPlayer* player, WCHAR* name, bool bSendCreatePartyPacket/* = false*/)
{
	CParty* party = new CParty;

	if (GetPartyCount() + 1 == NTL_MAX_PARTY_COUNT)
		return NULL;

	PARTYID partyid = CreatePartyID();

	party->SetPartyLeaderID(player->GetID());
	party->SetPartyName(name);
	party->SetPartyID(partyid);

	m_map_Party.insert(std::make_pair(partyid, party));

	party->SetMemberInfo(player, 0); //add player to party info
	party->SetMemberCount(true);

	CGameServer* app = (CGameServer*)g_pApp;

	//send to chat server
	CNtlPacket chatPacket(sizeof(sGT_PARTY_CREATED));
	sGT_PARTY_CREATED * chatRes = (sGT_PARTY_CREATED *)chatPacket.GetPacketData();
	chatRes->wOpCode = GT_PARTY_CREATED;
	chatRes->leaderCharId = player->GetCharID();
	chatRes->partyId = partyid;
	chatPacket.SetPacketLen( sizeof(sGT_PARTY_CREATED) );
	app->SendTo( app->GetChatServerSession(), &chatPacket); //Send to chat server

	if (bSendCreatePartyPacket)
	{
		CNtlPacket packet(sizeof(sGU_PARTY_CREATE_RES));
		sGU_PARTY_CREATE_RES * res = (sGU_PARTY_CREATE_RES *)packet.GetPacketData();
		res->wOpCode = GU_PARTY_CREATE_RES;
		res->wResultCode = 500;
		res->partyID = partyid;
		wcscpy_s(res->wszPartyName, NTL_MAX_SIZE_PARTY_NAME + 1, name);
		packet.SetPacketLen(sizeof(sGU_PARTY_CREATE_RES));
		g_pApp->Send(player->GetClientSessionID(), &packet);
	}

	player->SetParty(party);
	player->SetPartyID(partyid);

	return party;
}

//--------------------------------------------------------------------------------------//
//		RETURN PARTY
//--------------------------------------------------------------------------------------//
CParty*	CPartyManager::GetParty(PARTYID partyid)
{
	std::map<PARTYID, CParty*>::iterator it = m_map_Party.find(partyid);

	if (m_map_Party.end() == it)
	{
		NTL_PRINT(PRINT_APP,"CPartyManager::GetParty(PARTYID partyid) PARTY NOT FOUND %d", partyid);
		return NULL;
	}

	return it->second;
}

//--------------------------------------------------------------------------------------//
//		DISBAND PARTY
//--------------------------------------------------------------------------------------//
void	CPartyManager::DisbandParty(CParty* party)
{
	CGameServer* app = (CGameServer*)g_pApp;

	//send to chat server
	CNtlPacket chatPacket(sizeof(sGT_PARTY_DISBANDED));
	sGT_PARTY_DISBANDED * chatRes = (sGT_PARTY_DISBANDED *)chatPacket.GetPacketData();
	chatRes->wOpCode = GT_PARTY_DISBANDED;
	chatRes->partyId = party->GetPartyID();
	chatPacket.SetPacketLen(sizeof(sGT_PARTY_DISBANDED));
	app->SendTo(app->GetChatServerSession(), &chatPacket); //Send to chat server

	//if delete party while members still inside
	if(party->GetPartyMemberCount() > 0)
	{
		//send to all party members
		CNtlPacket packet(sizeof(sGU_PARTY_DISBANDED_NFY));
		sGU_PARTY_DISBANDED_NFY * res = (sGU_PARTY_DISBANDED_NFY *)packet.GetPacketData();
		res->wOpCode = GU_PARTY_DISBANDED_NFY;
		packet.SetPacketLen( sizeof(sGU_PARTY_DISBANDED_NFY));
		party->SendMsgToParty(&packet);

		//remove party from player
		for(int i = 0; i < party->GetPartyMemberCount(); i++)
		{
			CPlayer* player = g_pObjectManager->GetPC(party->GetMemberInfo(i).hHandle);
			if(player && player->IsInitialized())
			{
				if (player->GetRankBattleRoomTblidx() != INVALID_TBLIDX && player->GetRankBattleRoomTblidx() == party->GetRankBattleRoomTblidx())
					player->LeaveRankBattleQueue();

				player->SetPartyID(INVALID_PARTYID);
				player->SetParty(NULL);
			}
		}
	}

	if (party->GetRankBattleRoomTblidx() != INVALID_TBLIDX && party->GetRankBattleRoomId() == INVALID_ROOMID) // if party is in rank battle queue and not inside battle already
		g_pRankbattleManager->LeaveRoom(party->GetPartyID(), party->GetRankBattleRoomTblidx());

	//remove from party search if registered
	if (party->IsInPartySearch())
	{
		g_pPartyMatching->Unregister(NULL, party->GetPartyID());
	}

	//decrease total party count
	DeletePartyID(party->GetPartyID());

	m_map_Party.erase(party->GetPartyID());

	SAFE_DELETE(party);
}


//--------------------------------------------------------------------------------------//
//		remove mark from parties which have marked object X.
//--------------------------------------------------------------------------------------//
void CPartyManager::CleanMark(std::set<PARTYID>* pSetMarkedByEnemyParty, HOBJECT hTarget)
{
	for (std::set<PARTYID>::iterator it = pSetMarkedByEnemyParty->begin(); it != pSetMarkedByEnemyParty->end(); it++)
	{
		CParty* pParty = GetParty(*it);
		if (pParty)
		{
			pParty->RemoveTargetMark(hTarget);
		}
	}

	pSetMarkedByEnemyParty->clear();
}


//--------------------------------------------------------------------------------------//
//		CREATE PARTY-ID
//--------------------------------------------------------------------------------------//
PARTYID CPartyManager::CreatePartyID()
{
	PARTYID partyID = INVALID_HOBJECT;

	if (m_vPartyHandlerVec.size() > 0)
	{
		partyID = m_vPartyHandlerVec.front();
		m_vPartyHandlerVec.erase(m_vPartyHandlerVec.begin());
	}
	else
		partyID = m_PartyHandler++;

	return partyID;
}

//--------------------------------------------------------------------------------------//
//		REMOVE PARTY ID
//--------------------------------------------------------------------------------------//
void CPartyManager::DeletePartyID(PARTYID partyID)
{
	m_vPartyHandlerVec.push_back(partyID);
}

void CPartyManager::BudokaiCreateParty(CPlayer * pPlayer, WCHAR * teamName, CHARACTERID leaderCharId)
{
	//search if party already exist
	for (std::map<PARTYID, CParty*>::iterator it = m_map_Party.begin(); it != m_map_Party.end(); it++)
	{
		CParty* pParty = it->second;
		if (pParty)
		{
			//compare party names
			if (wcscmp(pParty->GetPartyName(), teamName) == 0)
			{
				//add player to party
				if (pParty->AddPartyMember(pPlayer))
				{
					//check if player is team leader
					if (leaderCharId == pPlayer->GetCharID())
					{
						//change party leader
						pParty->UpdatePartyLeader(pPlayer->GetID());
					}
				}
				else
				{
					ERR_LOG(LOG_GENERAL, "Could not add player to budokai party. Party member count %u", pParty->GetPartyMemberCount());
				}

				return;
			}
		}
	}

	CParty* pParty = CreateParty(pPlayer, teamName);
	if (pParty == NULL)
	{
		ERR_LOG(LOG_GENERAL, "Could create party for budokai");
	}
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////											PARTY											/////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CParty::CParty()
{
	m_byMemberInfoCount = 0;
	m_byItemLootingMethod = NTL_PARTY_ITEM_LOOTING_IN_ORDER;
	m_byZennyLootingMethod = NTL_PARTY_ZENNY_LOOTING_SHARED;
	m_eDiff = PARTY_DUNGEON_STATE_INVALID;
	m_byItemRank = ITEM_RANK_SUPERIOR;

	for (BYTE i = 0; i < NTL_MAX_SHARETARGET_COUNT; i++)
		m_sharetargetInfo[i].hTarget = INVALID_HOBJECT;

	m_rankBattleInfo.roomId = INVALID_ROOMID;
	m_rankBattleInfo.rankBattleTblidx = INVALID_TBLIDX;
	m_rankBattleInfo.hObject = INVALID_HOBJECT;
	m_rankBattleInfo.byMemberCount = 0;
	for (BYTE i = 0; i < NTL_MAX_MEMBER_IN_PARTY; i++)
	{
		m_memberInfo[i].hHandle = INVALID_HOBJECT;
		m_rankBattleInfo.asMember[i].hPc = INVALID_HOBJECT;
	}

	bIsInPartySearch = false;

	ResetPartyDirectPlay();

	m_diceProgress.Init();
	m_byLastItemOrderPartyIndex = 0;
}

CParty::~CParty()
{
	for (int i = 0; i < NTL_MAX_SHARETARGET_COUNT; i++) //unsert mark at targets
	{
		if (m_sharetargetInfo[i].hTarget != INVALID_HOBJECT)
		{
			CCharacter* pChar = g_pObjectManager->GetChar(m_sharetargetInfo[i].hTarget);
			if (pChar && pChar->IsInitialized())
			{
				pChar->RemoveMarkedByParty(GetPartyID());
			}
		}
	}

	m_mapRewardSelectState.clear();

	//unlock items from inventory
	for (std::map<BYTE, sPARTY_ITEM>::iterator it = m_mapPartyInventory.begin(); it != m_mapPartyInventory.end(); it++)
	{
		CItemDrop* pDrop = g_pItemManager->FindDrop(it->second.hHandle);
		if (pDrop && pDrop->IsInitialized() && pDrop->GetLocked())
		{
			pDrop->SetLocked(false);
		}
	}
	//clear inventory map
	m_mapPartyInventory.clear();
}


void CParty::TickProcess(DWORD dwTickDiff)
{
	if (m_diceProgress.bInventoryDiceInProgress)
	{
		std::map<BYTE, sPARTY_ITEM>::iterator it = m_mapPartyInventory.find(m_diceProgress.byInventorySlotDiceInProgress);
		if (it != m_mapPartyInventory.end())
		{
			sPARTY_ITEM& partyItem = it->second;

			partyItem.dwDiceTimeLeft = UnsignedSafeDecrease<DWORD>(partyItem.dwDiceTimeLeft, dwTickDiff);

			if (partyItem.dwDiceTimeLeft == 0)
			{
				DecideDice(partyItem);
			}
		}
		else
		{
			m_diceProgress.Init();
		}
	}
	else
	{
		std::map<BYTE, sPARTY_ITEM>::iterator it = m_mapPartyInventory.begin();
		if (it != m_mapPartyInventory.end())
		{
			sPARTY_ITEM& partyItem = it->second;

			CItemDrop* pDrop = g_pItemManager->FindDrop(partyItem.hHandle);
			if (pDrop && pDrop->IsInitialized() && pDrop->GetCurWorld())
			{
				sITEM_TBLDAT* pItemTbldat = (sITEM_TBLDAT*)g_pTableContainer->GetItemTable()->FindData(pDrop->GetTblidx());
				if (pItemTbldat)
				{
					m_diceProgress.bInventoryDiceInProgress = true;
					m_diceProgress.byInventorySlotDiceInProgress = it->first;
					m_diceProgress.hInventoryItemHandleDiceInProgress = partyItem.hHandle;

					BYTE byItemTypeGroup = GetItemTypeGroup(pItemTbldat->byItem_Type);

					for (int i = 0; i < m_byMemberInfoCount; i++)
					{
						CPlayer* player = g_pObjectManager->GetPC(m_memberInfo[i].hHandle);
						if (player && player->IsInitialized() && player->IsInRange(pDrop, DBO_VALID_DISTANCE_TO_GET_ITEM_IN_PARTY))
						{
							player->SetItemDice(true);

							//increase counter players received dice
							++m_diceProgress.byReceivedDiceCount;

							//send nfy that dice begin
							CNtlPacket packetEnd(sizeof(sGU_ITEM_PICK_DICE_NFY));
							sGU_ITEM_PICK_DICE_NFY * res = (sGU_ITEM_PICK_DICE_NFY *)packetEnd.GetPacketData();
							res->wOpCode = GU_ITEM_PICK_DICE_NFY;
							res->hItemHandle = partyItem.hHandle;
							res->bEquiped = TRUE;

							if (GetItemLootingMethod() == NTL_PARTY_ITEM_LOOTING_DICE_BY_EQUIPED && byItemTypeGroup != ITEM_TYPE_GROUP_ETC)
							{
								if (Dbo_CheckClass(player->GetClass(), pItemTbldat->dwNeed_Class_Bit_Flag) == false) //check class
									res->bEquiped = FALSE;
								else if (BIT_FLAG_TEST(MAKE_BIT_FLAG(player->GetGender()), pItemTbldat->dwNeed_Gender_Bit_Flag) == false) //check gender
									res->bEquiped = FALSE;
								else if (pItemTbldat->byRace_Special != player->GetRace() && pItemTbldat->byRace_Special != INVALID_BYTE) //check race
									res->bEquiped = FALSE;
							}

							res->itemInfo.objType = OBJTYPE_DROPITEM;
							res->itemInfo.itemBrief.byBattleAttribute = partyItem.itemInfo.byBattleAttribute;
							res->itemInfo.itemBrief.byGrade = partyItem.itemInfo.byGrade;
							res->itemInfo.itemBrief.byRank = partyItem.itemInfo.byRank;
							res->itemInfo.itemBrief.tblidx = partyItem.itemInfo.tblidx;
							res->itemInfo.itemState.bIsNew = false;
							res->itemInfo.itemState.bNeedToIdentify = partyItem.itemInfo.bNeedToIdentify;
							res->itemInfo.itemState.dwDropTime = 0;// if this is 20000 then the time left to dice will be 40seconds(40000ms) because drop time is 60seconds(60000ms) //Lets set it to 0 
							res->itemInfo.itemState.hOwner = pDrop->GetOwnerID();
							res->itemInfo.itemState.partyID = pDrop->GetPartyID();
							pDrop->GetCurLoc().CopyTo(res->itemInfo.itemState.vCurLoc);
							packetEnd.SetPacketLen(sizeof(sGU_ITEM_PICK_DICE_NFY));
							player->SendPacket(&packetEnd);
						}
					}

					//printf("dice begin \n");
				}
				else
				{
					ERR_LOG(LOG_GENERAL, "Couldnt find item tblidx: %u", pDrop->GetTblidx());
					m_mapPartyInventory.erase(it);
				}
			}
			else
			{
				//printf("m_mapPartyInventory.erase(it); \n");
				m_mapPartyInventory.erase(it);
			}
		}
	}
}


//--------------------------------------------------------------------------------------//
//		UPDATE PARTY MEMBER COUNTER
//--------------------------------------------------------------------------------------//
void CParty::SetMemberCount(bool bIsPlus)
{
	bIsPlus == true ? m_byMemberInfoCount = UnsignedSafeIncrease<BYTE>(m_byMemberInfoCount, 1) : m_byMemberInfoCount = UnsignedSafeDecrease<BYTE>(m_byMemberInfoCount, 1);
}

//--------------------------------------------------------------------------------------//
//		ADD PARTY MEMBER
//--------------------------------------------------------------------------------------//
bool CParty::AddPartyMember(CPlayer* player)
{
	if (m_byMemberInfoCount == NTL_MAX_MEMBER_IN_PARTY)
		return false;

	player->SetParty(this);
	player->SetPartyID(this->GetPartyID());


	CGameServer * app = (CGameServer*)g_pApp;

	//send party info member
	CNtlPacket pPartyInfo(sizeof(sGU_PARTY_INFO));
	sGU_PARTY_INFO * rPartyInfo = (sGU_PARTY_INFO *)pPartyInfo.GetPacketData();
	rPartyInfo->wOpCode = GU_PARTY_INFO;
	wcscpy_s(rPartyInfo->awchPartyName, NTL_MAX_SIZE_PARTY_NAME + 1, m_awchPartyName);
	rPartyInfo->byItemLootingMethod = m_byItemLootingMethod;
	rPartyInfo->byMemberInfoCount = m_byMemberInfoCount;
	rPartyInfo->byZennyLootingMethod = m_byZennyLootingMethod;
	rPartyInfo->byItemRank = m_byItemRank;
	rPartyInfo->eDiff = m_eDiff;
	rPartyInfo->hLeader = m_hLeader;
	memcpy(rPartyInfo->memberInfo, &m_memberInfo, m_byMemberInfoCount * sizeof(sPARTY_MEMBER_INFO));
	rPartyInfo->partyID = m_partyID;
	memcpy(rPartyInfo->sharetargetInfo, m_sharetargetInfo, sizeof(rPartyInfo->sharetargetInfo));
	pPartyInfo.SetPacketLen( sizeof(sGU_PARTY_INFO) );
	g_pApp->Send(player->GetClientSessionID(), &pPartyInfo );
	

	//set party member info
	SetMemberInfo(player, m_byMemberInfoCount);


	//send to chat server 
	CNtlPacket chatPacket(sizeof(sGT_PARTY_MEMBER_JOINED));
	sGT_PARTY_MEMBER_JOINED * chatRes = (sGT_PARTY_MEMBER_JOINED *)chatPacket.GetPacketData();
	chatRes->wOpCode = GT_PARTY_MEMBER_JOINED;
	chatRes->memberCharId = player->GetCharID();
	chatRes->partyId = m_partyID;
	chatPacket.SetPacketLen(sizeof(sGT_PARTY_MEMBER_JOINED));
	app->SendTo(app->GetChatServerSession(), &chatPacket);


	//send nfy to party members that new player joined
	CNtlPacket packet(sizeof(sGU_PARTY_MEMBER_JOINED_NFY));
	sGU_PARTY_MEMBER_JOINED_NFY * res = (sGU_PARTY_MEMBER_JOINED_NFY *)packet.GetPacketData();
	res->wOpCode = GU_PARTY_MEMBER_JOINED_NFY;
	memcpy(&res->memberInfo, &m_memberInfo[m_byMemberInfoCount], sizeof(sPARTY_MEMBER_INFO));
	packet.SetPacketLen( sizeof(sGU_PARTY_MEMBER_JOINED_NFY) );
	this->SendMsgToPartyExceptOne(&packet, player->GetID());


	//send buff info to party & get buff info from party
	GetPartyBuffInfo(player);


	//increase member counter
	SetMemberCount(true);

	return true;
}

//--------------------------------------------------------------------------------------//
//		MEMBER LEAVE PARTY
//--------------------------------------------------------------------------------------//
void CParty::LeaveParty(CPlayer* player)
{
	CGameServer * app = (CGameServer*) NtlSfxGetApp();

	player->CancelDice();

	player->SetPartyID(INVALID_PARTYID);
	player->SetParty(NULL);

	//if player leave party in dungeon then he will be teleported out
	if (player->GetCurWorld() && (player->GetUD() || player->GetCCBD()))
	{
		CWorld* pWorld = player->GetCurWorld();
		player->StartTeleport(pWorld->GetTbldat()->outWorldLoc, pWorld->GetTbldat()->outWorldDir, pWorld->GetTbldat()->outWorldTblidx, TELEPORT_TYPE_DUNGEON);
	}

	if(m_byMemberInfoCount == 1) //delete party if last member leaves
	{
		g_pPartyManager->DisbandParty(this);
		return;
	}

	//SET MEMBER INFO
	for (int e = 0; e < m_byMemberInfoCount; e++)
	{
		if (player->GetID() == m_memberInfo[e].hHandle)
		{
			SetMemberInfo(NULL, e); //set member info to null

			for (int ee = e; ee < m_byMemberInfoCount; ee++)
			{
				if(ee == NTL_MAX_MEMBER_IN_PARTY)
					break;

				//check if next member slot is a member. if true then set that member on the leaver slot
				if (m_memberInfo[ee + 1].hHandle != INVALID_HOBJECT)
				{
					memcpy(&m_memberInfo[ee], &m_memberInfo[ee + 1], sizeof(sPARTY_MEMBER_INFO));
				}
				else
				{
					if(ee+1 <= NTL_MAX_MEMBER_IN_PARTY)
					{
						memset(&m_memberInfo[ee + 1], NULL, sizeof(sPARTY_MEMBER_INFO));
						m_memberInfo[ee + 1].hHandle = INVALID_HOBJECT;
					}
				}
			}

			break;
		}
	}

	//SET BATTLERANK INFO
	for (int e = 0; e < m_rankBattleInfo.byMemberCount; e++)
	{
		if (player->GetID() == m_memberInfo[e].hHandle)
		{
			m_rankBattleInfo.asMember[e].hPc = INVALID_HOBJECT;

			for (int ee = e; ee < m_rankBattleInfo.byMemberCount; ee++)
			{
				if (ee == NTL_MAX_MEMBER_IN_PARTY)
					break;

				//check if next member slot is a member. if true then set that member on the leaver slot
				if (m_memberInfo[ee + 1].hHandle != INVALID_HOBJECT)
				{
					m_rankBattleInfo.asMember[ee].hPc = m_rankBattleInfo.asMember[ee + 1].hPc;
				}
				else
				{
					if (ee + 1 <= NTL_MAX_MEMBER_IN_PARTY)
					{
						m_rankBattleInfo.asMember[ee + 1].hPc = INVALID_HOBJECT;
					}
				}
			}

			player->LeaveRankBattleQueue();
			break;
		}
	}
	m_rankBattleInfo.byMemberCount = UnsignedSafeDecrease<BYTE>(m_rankBattleInfo.byMemberCount, 1);

	if (GetRankBattleRoomTblidx() != INVALID_TBLIDX && GetRankBattleRoomId() == INVALID_ROOMID && GetMemberCanEnterRankbattleQueueCount(1, 100) < 2) // if party is in rank battle queue and not inside battle already and not enough players can enter rank battle
		g_pRankbattleManager->LeaveRoom(GetPartyID(), GetRankBattleRoomTblidx());


	//decrease member count
	SetMemberCount(false);

	RemoveMembersBuffs(player);

	//SEND TO CHAT SERVER
	CNtlPacket chatpacket(sizeof(sGT_PARTY_MEMBER_LEFT));
	sGT_PARTY_MEMBER_LEFT * chatres = (sGT_PARTY_MEMBER_LEFT *)chatpacket.GetPacketData();
	chatres->wOpCode = GT_PARTY_MEMBER_LEFT;
	chatres->memberCharId = player->GetCharID();
	chatres->partyId = m_partyID;
	chatpacket.SetPacketLen(sizeof(sGT_PARTY_MEMBER_LEFT));
	app->SendTo(app->GetChatServerSession(), &chatpacket);

	//send nfy to party that member left
	CNtlPacket packet(sizeof(sGU_PARTY_MEMBER_LEFT_NFY));
	sGU_PARTY_MEMBER_LEFT_NFY * res = (sGU_PARTY_MEMBER_LEFT_NFY *)packet.GetPacketData();
	res->wOpCode = GU_PARTY_MEMBER_LEFT_NFY;
	res->hMember = player->GetID();
	packet.SetPacketLen(sizeof(sGU_PARTY_MEMBER_LEFT_NFY));
	SendMsgToParty(&packet);

	if (player->GetID() == this->GetPartyLeaderID()) //if party leader leave party then update leader
	{
		UpdatePartyLeader(m_memberInfo[0].hHandle);
	}
}


//--------------------------------------------------------------------------------------//
//		
//--------------------------------------------------------------------------------------//
void CParty::SetMemberInfo(CPlayer* pPlayer, int nMemberCount)
{
	if (pPlayer)
	{
		wcscpy_s(m_memberInfo[nMemberCount].awchMemberName, NTL_MAX_SIZE_CHAR_NAME + 1, pPlayer->GetCharName());
		m_memberInfo[nMemberCount].byClass = pPlayer->GetClass();
		m_memberInfo[nMemberCount].byLevel = pPlayer->GetLevel();
		m_memberInfo[nMemberCount].byRace = pPlayer->GetRace();
		m_memberInfo[nMemberCount].curLp = pPlayer->GetCurLP();
		m_memberInfo[nMemberCount].dwZenny = pPlayer->GetZeni();
		m_memberInfo[nMemberCount].hHandle = pPlayer->GetID();
		m_memberInfo[nMemberCount].maxLp = pPlayer->GetMaxLP();
		pPlayer->GetCurLoc().CopyTo(m_memberInfo[nMemberCount].vCurLoc);
		m_memberInfo[nMemberCount].wCurEP = pPlayer->GetCurEP();
		m_memberInfo[nMemberCount].wMaxEP = pPlayer->GetMaxEP();
		m_memberInfo[nMemberCount].worldId = pPlayer->GetWorldID();
		m_memberInfo[nMemberCount].worldTblidx = pPlayer->GetWorldTblidx();
	}
	else
	{
		memset(&m_memberInfo[nMemberCount], NULL, sizeof(sPARTY_MEMBER_INFO));
		m_memberInfo[nMemberCount].hHandle = INVALID_HOBJECT;
	}
}

bool CParty::IsMemberInsideGuild()
{
	for (int i = 0; i < m_byMemberInfoCount; i++)
	{
		CPlayer* player = g_pObjectManager->GetPC(m_memberInfo[i].hHandle);
		if (player && player->IsInitialized())
		{
			if (player->GetGuildID() != 0)
				return true;
		}
		else return true;
	}

	return false;
}


//--------------------------------------------------------------------------------------//
//		UPDATE PARTY LEADER
//--------------------------------------------------------------------------------------//
void	CParty::UpdatePartyLeader(HOBJECT newLeader)
{
	CGameServer* app = (CGameServer*)g_pApp;

	CNtlPacket pNewLeader(sizeof(sGU_PARTY_LEADER_CHANGED_NFY));
	sGU_PARTY_LEADER_CHANGED_NFY * rNewLeader = (sGU_PARTY_LEADER_CHANGED_NFY *)pNewLeader.GetPacketData();
	rNewLeader->wOpCode = GU_PARTY_LEADER_CHANGED_NFY;
	rNewLeader->hNewLeader = newLeader;
	pNewLeader.SetPacketLen( sizeof(sGU_PARTY_LEADER_CHANGED_NFY) );
	this->SendMsgToPartyExceptOne(&pNewLeader, m_hLeader);

	//SEND TO CHAT SERVER
	CNtlPacket chatpacket(sizeof(sGT_PARTY_LEADER_CHANGED));
	sGT_PARTY_LEADER_CHANGED * chatres = (sGT_PARTY_LEADER_CHANGED *)chatpacket.GetPacketData();
	chatres->wOpCode = GT_PARTY_LEADER_CHANGED;
	chatres->partyId = m_partyID;
	chatres->newLeaderCharId = newLeader;
	chatpacket.SetPacketLen(sizeof(sGT_PARTY_LEADER_CHANGED));
	app->SendTo(app->GetChatServerSession(), &chatpacket); //Send to chat server

	m_hLeader = newLeader;
}

//--------------------------------------------------------------------------------------//
//		KICK PARTY MEMBER
//--------------------------------------------------------------------------------------//
void CParty::KickPartyMember(CPlayer* kickedplayer)
{
	CGameServer* app = (CGameServer*)g_pApp;

	kickedplayer->CancelDice();

	//SEND TO CHAT SERVER
	CNtlPacket chatpacket(sizeof(sGT_PARTY_MEMBER_LEFT));
	sGT_PARTY_MEMBER_LEFT * chatres = (sGT_PARTY_MEMBER_LEFT *)chatpacket.GetPacketData();
	chatres->wOpCode = GT_PARTY_MEMBER_LEFT;
	chatres->memberCharId = kickedplayer->GetCharID();
	chatres->partyId = m_partyID;
	chatpacket.SetPacketLen( sizeof(sGT_PARTY_MEMBER_LEFT) );
	app->SendTo(app->GetChatServerSession(), &chatpacket); //Send to chat server

	//Send nfy to party that player has been kicked
	CNtlPacket pKick(sizeof(sGU_PARTY_MEMBER_KICKED_OUT_NFY));
	sGU_PARTY_MEMBER_KICKED_OUT_NFY * rKick = (sGU_PARTY_MEMBER_KICKED_OUT_NFY *)pKick.GetPacketData();
	rKick->wOpCode = GU_PARTY_MEMBER_KICKED_OUT_NFY;
	rKick->hMember = kickedplayer->GetID();
	pKick.SetPacketLen( sizeof(sGU_PARTY_MEMBER_KICKED_OUT_NFY) );
	this->SendMsgToPartyExceptOne(&pKick, m_hLeader);


	//set party member info
	for (int e = 0; e < m_byMemberInfoCount; e++)
	{
		if (kickedplayer->GetID() == m_memberInfo[e].hHandle)
		{
			SetMemberInfo(NULL, e);

			for (int ee = e; ee < m_byMemberInfoCount; ee++)
			{
				if(ee == NTL_MAX_MEMBER_IN_PARTY)
					break;

				if (m_memberInfo[ee + 1].hHandle != INVALID_HOBJECT)
				{
					memcpy(&m_memberInfo[ee], &m_memberInfo[ee + 1], sizeof(sPARTY_MEMBER_INFO));
				}
				else
				{
					if(ee+1 <= NTL_MAX_MEMBER_IN_PARTY)
					{
						memset(&m_memberInfo[ee + 1], NULL, sizeof(sPARTY_MEMBER_INFO));
						m_memberInfo[ee + 1].hHandle = INVALID_HOBJECT;
					}
				}
			}

			break;
		}
	}

	//SET BATTLERANK INFO
	for (int e = 0; e < m_rankBattleInfo.byMemberCount; e++)
	{
		if (kickedplayer->GetID() == m_memberInfo[e].hHandle)
		{
			m_rankBattleInfo.asMember[e].hPc = INVALID_HOBJECT;

			for (int ee = e; ee < m_rankBattleInfo.byMemberCount; ee++)
			{
				if (ee == NTL_MAX_MEMBER_IN_PARTY)
					break;

				//check if next member slot is a member. if true then set that member on the leaver slot
				if (m_memberInfo[ee + 1].hHandle != INVALID_HOBJECT)
				{
					m_rankBattleInfo.asMember[ee].hPc = m_rankBattleInfo.asMember[ee + 1].hPc;
				}
				else
				{
					if (ee + 1 <= NTL_MAX_MEMBER_IN_PARTY)
					{
						m_rankBattleInfo.asMember[ee + 1].hPc = INVALID_HOBJECT;
					}
				}
			}

			kickedplayer->LeaveRankBattleQueue();
			break;
		}
	}
	m_rankBattleInfo.byMemberCount = UnsignedSafeDecrease<BYTE>(m_rankBattleInfo.byMemberCount, 1);

	if (GetRankBattleRoomTblidx() != INVALID_TBLIDX && GetRankBattleRoomId() == INVALID_ROOMID && GetMemberCanEnterRankbattleQueueCount(1, 100) < 2) // if party is in rank battle queue and not inside battle already and not enough players can enter rank battle
		g_pRankbattleManager->LeaveRoom(GetPartyID(), GetRankBattleRoomTblidx());

	//unset party data for kicked player
	kickedplayer->SetPartyID(INVALID_PARTYID);
	kickedplayer->SetParty(NULL);

	//decrease member count
	SetMemberCount(false);

	RemoveMembersBuffs(kickedplayer);

	//if player get kicked in dungeon then he will be teleported out
	if (kickedplayer->GetUD() && kickedplayer->GetUD()->GetWorld())
	{
		CWorld* pWorld = kickedplayer->GetUD()->GetWorld();
		kickedplayer->StartTeleport(pWorld->GetTbldat()->outWorldLoc, pWorld->GetTbldat()->outWorldDir, pWorld->GetTbldat()->outWorldTblidx, TELEPORT_TYPE_DUNGEON);
	}
}

//--------------------------------------------------------------------------------------//
//		SHARE TARGET
//--------------------------------------------------------------------------------------//
void CParty::ShareTarget(CCharacter* pTarget, BYTE bySlot)
{
	CNtlPacket packet(sizeof(sGU_PARTY_SHARETARGET_NFY));
	sGU_PARTY_SHARETARGET_NFY * res = (sGU_PARTY_SHARETARGET_NFY *)packet.GetPacketData();
	res->wOpCode = GU_PARTY_SHARETARGET_NFY;

	for (int i = 0; i < NTL_MAX_SHARETARGET_COUNT; i++)
	{
		if (i == bySlot)
		{
			if (m_sharetargetInfo[i].hTarget == pTarget->GetID()) //remove mark
			{
				res->sharetargetInfo[i].hTarget = INVALID_HOBJECT;
				m_sharetargetInfo[i].hTarget = INVALID_HOBJECT;

				pTarget->RemoveMarkedByParty(GetPartyID());
			}
			else //update mark
			{
				res->sharetargetInfo[i].hTarget = pTarget->GetID();
				m_sharetargetInfo[i].hTarget = pTarget->GetID();

				pTarget->SetMarkedByParty(GetPartyID());
			}
		}
		else
		{
			if (m_sharetargetInfo[i].hTarget == pTarget->GetID()) //if target already marked on other slot then remove from other slot
			{
				res->sharetargetInfo[i].hTarget = INVALID_HOBJECT;
				m_sharetargetInfo[i].hTarget = INVALID_HOBJECT;
			}
			else
			{
				res->sharetargetInfo[i].hTarget = m_sharetargetInfo[i].hTarget;
			}
		}
	}

	packet.SetPacketLen( sizeof(sGU_PARTY_SHARETARGET_NFY) );
	this->SendMsgToParty(&packet);
}

//--------------------------------------------------------------------------------------//
//		REMOVE SHARED TARGET
//--------------------------------------------------------------------------------------//
void CParty::RemoveTargetMark(HOBJECT hTarget)
{
	CNtlPacket packet(sizeof(sGU_PARTY_SHARETARGET_NFY));
	sGU_PARTY_SHARETARGET_NFY * res = (sGU_PARTY_SHARETARGET_NFY *)packet.GetPacketData();
	res->wOpCode = GU_PARTY_SHARETARGET_NFY;

	for (int i = 0; i < NTL_MAX_SHARETARGET_COUNT; i++)
	{
		if (m_sharetargetInfo[i].hTarget == hTarget) //if target already marked on other slot then remove from other slot
		{
			res->sharetargetInfo[i].hTarget = INVALID_HOBJECT;
			m_sharetargetInfo[i].hTarget = INVALID_HOBJECT;
		}
		else
		{
			res->sharetargetInfo[i].hTarget = m_sharetargetInfo[i].hTarget;
		}
	}

	packet.SetPacketLen(sizeof(sGU_PARTY_SHARETARGET_NFY));
	this->SendMsgToParty(&packet);
}


//--------------------------------------------------------------------------------------//
//		SET ITEM LOOTING METHOD
//--------------------------------------------------------------------------------------//
void	CParty::SetItemLootingMethod(BYTE byMethod)
{
	this->m_byItemLootingMethod = byMethod;

	CNtlPacket packet(sizeof(sGU_PARTY_ITEM_LOOTING_METHOD_CHANGED_NFY));
	sGU_PARTY_ITEM_LOOTING_METHOD_CHANGED_NFY * res = (sGU_PARTY_ITEM_LOOTING_METHOD_CHANGED_NFY *)packet.GetPacketData();
	res->wOpCode = GU_PARTY_ITEM_LOOTING_METHOD_CHANGED_NFY;
	res->byNewLootingMethod = byMethod;
	packet.SetPacketLen( sizeof(sGU_PARTY_ITEM_LOOTING_METHOD_CHANGED_NFY) );
	this->SendMsgToPartyExceptOne(&packet, m_hLeader);
}

//--------------------------------------------------------------------------------------//
//		SET ZENI LOOTING METHOD
//--------------------------------------------------------------------------------------//
void	CParty::SetZeniLootingMethod(BYTE byMethod)
{
	this->m_byZennyLootingMethod = byMethod;

	CNtlPacket packet(sizeof(sGU_PARTY_ZENNY_LOOTING_METHOD_CHANGED_NFY));
	sGU_PARTY_ZENNY_LOOTING_METHOD_CHANGED_NFY * res = (sGU_PARTY_ZENNY_LOOTING_METHOD_CHANGED_NFY *)packet.GetPacketData();
	res->wOpCode = GU_PARTY_ZENNY_LOOTING_METHOD_CHANGED_NFY;
	res->byNewLootingMethod = byMethod;
	packet.SetPacketLen( sizeof(sGU_PARTY_ZENNY_LOOTING_METHOD_CHANGED_NFY) );
	this->SendMsgToPartyExceptOne(&packet, m_hLeader);
}
//--------------------------------------------------------------------------------------//
//		SET ITEM LOOTING RANK
//--------------------------------------------------------------------------------------//
void	CParty::SetItemLootingMethodRank(BYTE byRank)
{
	this->m_byItemRank = byRank;

	CNtlPacket packet(sizeof(sGU_PARTY_CHANGE_ITEM_LOOTING_RANK_METHOD_NFY));
	sGU_PARTY_CHANGE_ITEM_LOOTING_RANK_METHOD_NFY * res = (sGU_PARTY_CHANGE_ITEM_LOOTING_RANK_METHOD_NFY *)packet.GetPacketData();
	res->wOpCode = GU_PARTY_CHANGE_ITEM_LOOTING_RANK_METHOD_NFY;
	res->byRank = byRank;
	packet.SetPacketLen( sizeof(sGU_PARTY_CHANGE_ITEM_LOOTING_RANK_METHOD_NFY) );
	this->SendMsgToPartyExceptOne(&packet, m_hLeader);
}
//--------------------------------------------------------------------------------------//
//		SET DUNGEON DIFFICULTY
//--------------------------------------------------------------------------------------//
void	CParty::SetDungeonDiff(ePARTY_DUNGEON_STATE eDiff)
{
	this->m_eDiff = eDiff;

	CNtlPacket packet(sizeof(sGU_PARTY_DUNGEON_DIFF_NFY));
	sGU_PARTY_DUNGEON_DIFF_NFY * res = (sGU_PARTY_DUNGEON_DIFF_NFY *)packet.GetPacketData();
	res->wOpCode = GU_PARTY_DUNGEON_DIFF_NFY;
	res->eDiff = eDiff;
	packet.SetPacketLen( sizeof(sGU_PARTY_DUNGEON_DIFF_NFY) );
	this->SendMsgToPartyExceptOne(&packet, m_hLeader);
}

//--------------------------------------------------------------------------------------//
//		INIT DUNGEON
//--------------------------------------------------------------------------------------//
void	CParty::InitDungeon()
{
	this->m_eDiff = PARTY_DUNGEON_STATE_INVALID;

	CNtlPacket packet(sizeof(sGU_PARTY_DUNGEON_INIT_NFY));
	sGU_PARTY_DUNGEON_INIT_NFY * res = (sGU_PARTY_DUNGEON_INIT_NFY *)packet.GetPacketData();
	res->wOpCode = GU_PARTY_DUNGEON_INIT_NFY;
	packet.SetPacketLen( sizeof(sGU_PARTY_DUNGEON_INIT_NFY) );
	this->SendMsgToPartyExceptOne(&packet, m_hLeader);
}

//--------------------------------------------------------------------------------------//
//		SET PARTY NAME
//--------------------------------------------------------------------------------------//
void CParty::SetPartyName(WCHAR* name)
{
	NTL_SAFE_WCSCPY(m_awchPartyName, name);
}
//--------------------------------------------------------------------------------------//
//		GET PARTY SLOT FROM MEMBER
//--------------------------------------------------------------------------------------//
BYTE CParty::GetPartyMemberSlot(HOBJECT hMember)
{
	for (int i = 0; i < m_byMemberInfoCount; i++)
	{
		if (m_memberInfo[i].hHandle == hMember)
			return i;
	}
	
	return INVALID_BYTE;
}
//--------------------------------------------------------------------------------------//
//		CHECK IF IS PARTY MEMBER
//--------------------------------------------------------------------------------------//
bool CParty::IsPartyMember(HOBJECT hTarget)
{
	for (int i = 0; i < m_byMemberInfoCount; i++)
	{
		if (m_memberInfo[i].hHandle == hTarget)
			return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------//
//		SET MEMBER LP
//--------------------------------------------------------------------------------------//
void	CParty::SetMemberLP(CPlayer* player)
{
	for (int i = 0; i < m_byMemberInfoCount; i++)
	{
		if (m_memberInfo[i].hHandle == player->GetID())
		{
			m_memberInfo[i].curLp = player->GetCurLP();
			m_memberInfo[i].maxLp = player->GetMaxLP();

			CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_LP));
			sGU_UPDATE_CHAR_LP * res = (sGU_UPDATE_CHAR_LP *)packet.GetPacketData();
			res->wOpCode = GU_UPDATE_CHAR_LP;
			res->handle = player->GetID();
			res->dwLpEpEventId = player->GetLpEpEventID();
			res->curLp = m_memberInfo[i].curLp;
			res->maxLp = m_memberInfo[i].maxLp;
			packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_LP));
			SendMsgToPartyExceptOne(&packet, player->GetID());

			break;
		}
	}



}

//--------------------------------------------------------------------------------------//
//		SET MEMBER EP
//--------------------------------------------------------------------------------------//
void CParty::SetMemberEP(CPlayer* player)
{
	for (int i = 0; i < m_byMemberInfoCount; i++)
	{
		if (m_memberInfo[i].hHandle == player->GetID())
		{
			m_memberInfo[i].wCurEP = player->GetCurEP();
			m_memberInfo[i].wMaxEP = player->GetMaxEP();

			CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_EP));
			sGU_UPDATE_CHAR_EP * res = (sGU_UPDATE_CHAR_EP *)packet.GetPacketData();
			res->wOpCode = GU_UPDATE_CHAR_EP;
			res->handle = player->GetID();
			res->dwLpEpEventId = player->GetLpEpEventID();
			res->wCurEP = m_memberInfo[i].wCurEP;
			res->wMaxEP = m_memberInfo[i].wMaxEP;
			packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_EP));
			SendMsgToPartyExceptOne(&packet, player->GetID());

			break;
		}
	}
}

//--------------------------------------------------------------------------------------//
//		SET MEMBER LP EP
//--------------------------------------------------------------------------------------//
void CParty::SetMemberLpEp(CPlayer* player)
{
	for (int i = 0; i < m_byMemberInfoCount; i++)
	{
		if (m_memberInfo[i].hHandle == player->GetID())
		{
			m_memberInfo[i].curLp = player->GetCurLP();
			m_memberInfo[i].maxLp = player->GetMaxLP();
			m_memberInfo[i].wCurEP = player->GetCurEP();
			m_memberInfo[i].wMaxEP = player->GetMaxEP();

			CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_LP_EP));
			sGU_UPDATE_CHAR_LP_EP * res = (sGU_UPDATE_CHAR_LP_EP *)packet.GetPacketData();
			res->wOpCode = GU_UPDATE_CHAR_LP_EP;
			res->handle = player->GetID();
			res->dwLpEpEventId = player->GetLpEpEventID();
			res->curLp = m_memberInfo[i].curLp;
			res->maxLp = m_memberInfo[i].maxLp;
			res->wCurEP = m_memberInfo[i].wCurEP;
			res->wMaxEP = m_memberInfo[i].wMaxEP;
			packet.SetPacketLen(sizeof(sGU_UPDATE_CHAR_LP_EP));
			SendMsgToPartyExceptOne(&packet, player->GetID());

			break;
		}
	}
}

//--------------------------------------------------------------------------------------//
//		UPDATE PARTY MEMBER LOCATION
//--------------------------------------------------------------------------------------//
void	CParty::UpdateMemberLocation(CPlayer* player)
{
	for (int i = 0; i < m_byMemberInfoCount; i++)
	{
		if (m_memberInfo[i].hHandle == player->GetID())
		{
			player->GetCurLoc().CopyTo(m_memberInfo[i].vCurLoc);
			m_memberInfo[i].worldId = player->GetWorldID();
			m_memberInfo[i].worldTblidx = player->GetWorldTblidx();

			break;
		}
	}

	CNtlPacket packet(sizeof(sGU_PARTY_MEMBER_LOCATION_NFY));
	sGU_PARTY_MEMBER_LOCATION_NFY * res = (sGU_PARTY_MEMBER_LOCATION_NFY *)packet.GetPacketData();
	res->wOpCode = GU_PARTY_MEMBER_LOCATION_NFY;
	res->hMember = player->GetID();
	NtlLocationCompress(&res->vCompressLoc, player->GetCurLoc().x, player->GetCurLoc().y, player->GetCurLoc().z);
	res->worldId = player->GetWorldID();
	res->worldTblidx = player->GetWorldTblidx();
	SendMsgToPartyExceptOne(&packet, player->GetID());
}


//--------------------------------------------------------------------------------------//
//		UPDATE PARTY MEMBER LEVEL
//--------------------------------------------------------------------------------------//
void	CParty::UpdateMemberLevel(CPlayer* player)
{
	for (int i = 0; i < m_byMemberInfoCount; i++)
	{
		if (m_memberInfo[i].hHandle == player->GetID())
		{
			m_memberInfo[i].byLevel = player->GetLevel();
			m_memberInfo[i].maxLp = player->GetMaxLP();
			m_memberInfo[i].wMaxEP = player->GetMaxEP();

			break;
		}
	}

	CNtlPacket packet(sizeof(sGU_PARTY_MEMBER_LEVELED_UP_NFY));
	sGU_PARTY_MEMBER_LEVELED_UP_NFY * res = (sGU_PARTY_MEMBER_LEVELED_UP_NFY *)packet.GetPacketData();
	res->wOpCode = GU_PARTY_MEMBER_LEVELED_UP_NFY;
	res->hMember = player->GetID();
	res->byLevel = player->GetLevel();
	res->maxLp = player->GetMaxLP();
	res->wMaxEP = player->GetMaxEP();
	packet.SetPacketLen(sizeof(sGU_PARTY_MEMBER_LEVELED_UP_NFY));
	this->SendMsgToPartyExceptOne(&packet, player->GetID());
}

//--------------------------------------------------------------------------------------//
//		UPDATE PARTY MEMBER CLASS
//--------------------------------------------------------------------------------------//
void	CParty::UpdateMemberClass(CPlayer* player)
{
	for (int i = 0; i < m_byMemberInfoCount; i++)
	{
		if (m_memberInfo[i].hHandle == player->GetID())
		{
			m_memberInfo[i].byClass = player->GetClass();

			break;
		}
	}

	CNtlPacket packet(sizeof(sGU_PARTY_MEMBER_CLASS_CHANGED_NFY));
	sGU_PARTY_MEMBER_CLASS_CHANGED_NFY * res = (sGU_PARTY_MEMBER_CLASS_CHANGED_NFY *)packet.GetPacketData();
	res->wOpCode = GU_PARTY_MEMBER_CLASS_CHANGED_NFY;
	res->hMember = player->GetID();
	res->byNewClass = player->GetClass();
	packet.SetPacketLen(sizeof(sGU_PARTY_MEMBER_CLASS_CHANGED_NFY));
	this->SendMsgToPartyExceptOne(&packet, player->GetID());
}


//--------------------------------------------------------------------------------------//
//		send buff from party members to player
//--------------------------------------------------------------------------------------//
void	CParty::GetPartyBuffInfo(CPlayer* player)
{
	for (int i = 0; i < m_byMemberInfoCount; i++)
	{
		if (m_memberInfo[i].hHandle != player->GetID())
		{
			CPlayer* member = g_pObjectManager->GetPC(m_memberInfo[i].hHandle);
			if(member)
			{
				member->GetBuffManager()->SendBuffInfo(player); //send member buff to player
				player->GetBuffManager()->SendBuffInfo(member); //send player buff to member
			}
		}
	}
}

//--------------------------------------------------------------------------------------//
//		SEND MSG(PACKET) TO ALL PARTY MEMBERS
//--------------------------------------------------------------------------------------//
void	CParty::SendMsgToParty(CNtlPacket * pPacket)
{
	for (int i = 0; i < m_byMemberInfoCount; i++)
	{
		CCharacter* player = g_pObjectManager->GetChar(m_memberInfo[i].hHandle);
		if (player && player->IsInitialized())
			player->SendPacket(pPacket);
	}
}

//--------------------------------------------------------------------------------------//
//		SEND MSG(PACKET) TO ALL PARTY MEMBERS EXCEPT ONE MEMBER
//--------------------------------------------------------------------------------------//
void	CParty::SendMsgToPartyExceptOne(CNtlPacket * pPacket, HOBJECT hHandle)
{
	for (int i = 0; i < m_byMemberInfoCount; i++)
	{
		if (m_memberInfo[i].hHandle != hHandle)
		{
			CCharacter* player = g_pObjectManager->GetChar(m_memberInfo[i].hHandle);
			if (player)
				g_pApp->Send(player->GetClientSessionID(), pPacket);
		}
	}
}


//--------------------------------------------------------------------------------------//
//		BROADCAST PACKET TO PARTY MEMBERS WITHIN RANGE
//--------------------------------------------------------------------------------------//
void CParty::BroadcastWithinRange(CNtlPacket * pPacket, CPlayer* pSender, float fRange)
{
	for (int i = 0; i < m_byMemberInfoCount; i++)
	{
		CCharacter* player = g_pObjectManager->GetChar(m_memberInfo[i].hHandle);
		if (player && player->IsInitialized())
		{
			if (player->IsInRange(pSender, fRange))
				g_pApp->Send(player->GetClientSessionID(), pPacket);
		}
	}
}

void CParty::AddEventIdToAll(DWORD dwEventID)
{
	for (int i = 0; i < m_byMemberInfoCount; i++)
	{
		CPlayer* pPlayer = g_pObjectManager->GetPC(m_memberInfo[i].hHandle);
		if (pPlayer)
		{
			pPlayer->GetQuests()->GetEventMap()->OnEventReceived(dwEventID);
		}
	}
}

void CParty::AddEventIdToWithinRange(DWORD dwEventID, CPlayer * pSender, float fRange)
{
	for (int i = 0; i < m_byMemberInfoCount; i++)
	{
		CPlayer* pPlayer = g_pObjectManager->GetPC(m_memberInfo[i].hHandle);
		if (pPlayer && pPlayer->IsInitialized())
		{
			if (pPlayer->IsInRange(pSender, fRange))
				pPlayer->GetQuests()->GetEventMap()->OnEventReceived(dwEventID);
		}
	}
}

//--------------------------------------------------------------------------------------//
//		UPDATE AGGRO POINTS FROM SUBJECT
//--------------------------------------------------------------------------------------//
void	CParty::UpdatePartyAggro(HOBJECT hSubject, CNtlPacket * pPacket)
{
	for (int i = 0; i < m_byMemberInfoCount; i++)
	{
		CCharacter* player = g_pObjectManager->GetChar(m_memberInfo[i].hHandle);
		if (player)
		{
			if (player->GetTargetHandle() == hSubject)
				player->SendPacket(pPacket);
		}
	}
}

//--------------------------------------------------------------------------------------//
//		TELEPORT PARTY TO MEMBER
//--------------------------------------------------------------------------------------//
void	CParty::StartPartyTeleport(CPlayer* player)
{
	for (int i = 0; i < m_byMemberInfoCount; i++)
	{
		CPlayer* target = g_pObjectManager->GetPC(m_memberInfo[i].hHandle);
		if (target && target->IsInitialized())
		{
			if (player->GetDragonballScramble() != target->GetDragonballScramble())
				continue;

			if (target->GetID() != player->GetID() && target->GetCurWorld() && target->GetCurWorld()->GetTbldat()->bDynamic == false && target->GetDragonballScrambleBallFlag() == 0)
			{
				target->StartTeleportProposal(player, 15, TELEPORT_TYPE_PARTY_POPO, TELEPORT_TYPE_PARTY_POPO, player->GetWorldTblidx(), player->GetWorldID(), player->GetCurLoc(), target->GetCurDir());

			}
		}
	}
}


void CParty::EnterDungeon(CPlayer* pPartyLeader, CUltimateDungeon* pDungeon, CWorld* pWorld, CScriptAlgoObject* pScript)
{
	for (int i = 0; i < m_byMemberInfoCount; i++)
	{
		CPlayer* pMember = g_pObjectManager->GetPC(m_memberInfo[i].hHandle);
		if (pMember && pMember->IsInitialized())
		{
			if (pPartyLeader->IsInRange(pMember, 100.f))
			{
				if (pMember->GetLevel() < pDungeon->GetTbldat()->byMinLevel || pMember->GetLevel() > pDungeon->GetTbldat()->byMaxLevel)
					continue;
				else if (pMember->GetRankBattleRoomTblidx() != INVALID_TBLIDX)
					continue;
				else if (pMember->GetDragonballScramble())
					continue;

				pDungeon->AddPlayer(pMember->GetID()); //add party member to dungeon set

				pScript->AddPlayer(pMember->GetID()); //add player to wps script
				pMember->AddScript(pScript); //add wps script to player

				pMember->StartTeleport(pWorld->GetTbldat()->vStart1Loc, pWorld->GetTbldat()->vStart1Dir, pWorld->GetID(), TELEPORT_TYPE_DUNGEON);

				pMember->SetUD(pDungeon);
			}
		}
	}
}


void CParty::EnterBattleDungeon(CPlayer* pPartyLeader, CBattleDungeon* pDungeon, CWorld* pWorld, CScriptAlgoObject* pScript)
{
	TBLIDX directPlayIdx = g_pTableContainer->GetServerConfigTable()->GetServerConfigData()->sBattleDungeonData.directPlay_EnterWorld;

	for (int i = 0; i < m_byMemberInfoCount; i++)
	{
		CPlayer* pMember = g_pObjectManager->GetPC(m_memberInfo[i].hHandle);
		if (pMember && pMember->IsInitialized())
		{
			if (pPartyLeader->IsInRange(pMember, NTL_MAX_RADIUS_OF_VISIBLE_AREA))
			{
				if (pMember->GetRankBattleRoomTblidx() != INVALID_TBLIDX)
					continue;
				if (pMember->GetDragonballScramble())
					continue;

				pDungeon->AddPlayer(pMember->GetID()); //add party member to dungeon set

				pScript->AddPlayer(pMember->GetID()); //add player to wps script
				pMember->AddScript(pScript); //add wps script to player

				pMember->StartTeleport(pWorld->GetTbldat()->vStart1Loc, pWorld->GetTbldat()->vStart1Dir, pWorld->GetID(), TELEPORT_TYPE_WORLD_MOVE, directPlayIdx, true);

				pMember->SetCCBD(pDungeon);
			}
		}
	}
}


void CParty::ShareZeniDropInParty(CPlayer* pPlayer, DWORD dwZeni)
{
	BYTE byReceiverCount = 0;
	CPlayer* ppMember[NTL_MAX_MEMBER_IN_PARTY];

	ppMember[byReceiverCount++] = pPlayer;

	for (int i = 0; i < m_byMemberInfoCount; i++)
	{
		CPlayer* pMember = g_pObjectManager->GetPC(m_memberInfo[i].hHandle);
		if (pMember && pMember->IsInitialized() && pMember->GetID() != pPlayer->GetID())
		{
			if (pMember->IsInRange(pPlayer, DBO_VALID_DISTANCE_TO_GET_ZENNY_IN_PARTY))
			{
				ppMember[byReceiverCount++] = pMember;
			}
		}
	}

	CGameServer* app = (CGameServer*)g_pApp;

	DWORD dwBonusZeni = dwZeni * app->GetZeniPartyBonusRate() / 100;
	DWORD dwZeniPerMember = dwZeni / byReceiverCount;

	CNtlPacket packet(sizeof(sGU_PARTY_MEMBER_GAINED_ZENNY_NFY));
	sGU_PARTY_MEMBER_GAINED_ZENNY_NFY * res = (sGU_PARTY_MEMBER_GAINED_ZENNY_NFY *)packet.GetPacketData();
	res->wOpCode = GU_PARTY_MEMBER_GAINED_ZENNY_NFY;
	res->bIsShared = true;
	res->dwBonusZenny = dwBonusZeni;
	res->dwOriginalZenny = dwZeni;
	res->dwAcquisitionZenny = dwZeniPerMember;
	res->dwZenny = res->dwAcquisitionZenny + dwBonusZeni;
	res->hMember = pPlayer->GetID();
	packet.SetPacketLen(sizeof(sGU_PARTY_MEMBER_GAINED_ZENNY_NFY));

	for (BYTE i = 0; i < byReceiverCount; i++)
	{
		if (ppMember[i]->CanReceiveZeni(res->dwZenny))
		{
			ERR_LOG(LOG_USER, "Player: %u receive %u zeni from party share", ppMember[i]->GetCharID(), res->dwZenny);

			ppMember[i]->UpdateZeni(ZENNY_CHANGE_TYPE_PICK, res->dwZenny, true, true);
			ppMember[i]->SendPacket(&packet);
		}
	}
}

bool CParty::IsSomeoneInDynamic(WORLDID destWorldId)
{
	for (int i = 0; i < m_byMemberInfoCount; i++)
	{
		CPlayer* pMember = g_pObjectManager->GetPC(m_memberInfo[i].hHandle);
		if (pMember && pMember->IsInitialized())
		{
			if (pMember->GetCurWorld() == NULL)
				return true;

			if (pMember->GetCurWorld()->GetTbldat()->bDynamic == true && (destWorldId != INVALID_WORLDID && pMember->GetCurWorld()->GetID() != destWorldId)) //required for UD. So players can re-join.
				return true;
		}
	}

	return false;
}

bool CParty::IsEveryoneInLeaderRange(CPlayer* pLeader, float fRange)
{
	for (int i = 0; i < m_byMemberInfoCount; i++)
	{
		CPlayer* pMember = g_pObjectManager->GetPC(m_memberInfo[i].hHandle);
		if (pMember && pMember->IsInitialized())
		{
			if (pMember->GetID() == pLeader->GetID())
				continue;

			if (pLeader->IsInRange(pMember, fRange) == false)
				return false;
		}
		else
		{
			return false;
		}
	}

	return true;
}

CPlayer * CParty::GetPlayer(BYTE bySlot)
{
	CPlayer* pPlayer = g_pObjectManager->GetPC(m_memberInfo[bySlot].hHandle);
	if (pPlayer && pPlayer->IsInitialized())
	{
		return pPlayer;
	}

	return NULL;
}

bool CParty::RegisterRankBattle(HOBJECT hObject, TBLIDX roomTblidx)
{
	CGameServer* app = (CGameServer*)g_pApp;

	m_rankBattleInfo.roomId = INVALID_ROOMID;
	m_rankBattleInfo.hObject = hObject;
	m_rankBattleInfo.rankBattleTblidx = roomTblidx;
	m_rankBattleInfo.byMemberCount = 0;

	for (int i = 0; i < m_byMemberInfoCount; i++)
	{
		CPlayer* pPlayer = g_pObjectManager->GetPC(m_memberInfo[i].hHandle);
		if (pPlayer && pPlayer->IsInitialized())
		{
			if (pPlayer->GetCurWorld() && pPlayer->GetCurWorld()->GetTbldat()->bDynamic)
				continue;
			if (pPlayer->GetDragonballScramble() && app->GetGsChannel() == 0)
				continue;

			m_rankBattleInfo.asMember[m_rankBattleInfo.byMemberCount++].hPc = m_memberInfo[i].hHandle;

			pPlayer->SetRankBattleRoomTblidx(roomTblidx); //set room tblidx
			pPlayer->SetRankBattleRegisterObject(hObject); //set object handle where player registered. User to teleport player out of battle

			CNtlPacket packetNfy(sizeof(sGU_RANKBATTLE_JOIN_NFY));
			sGU_RANKBATTLE_JOIN_NFY * resNfy = (sGU_RANKBATTLE_JOIN_NFY *)packetNfy.GetPacketData();
			resNfy->wOpCode = GU_RANKBATTLE_JOIN_NFY;
			resNfy->rankBattleTblidx = roomTblidx;
			packetNfy.SetPacketLen(sizeof(sGU_RANKBATTLE_JOIN_NFY));
			pPlayer->SendPacket(&packetNfy);

		}
	}

	return m_rankBattleInfo.byMemberCount > 0;
}

void CParty::LeaveRankBattleQueue()
{
	m_rankBattleInfo.roomId = INVALID_ROOMID;
	m_rankBattleInfo.hObject = INVALID_HOBJECT;
	m_rankBattleInfo.rankBattleTblidx = INVALID_TBLIDX;

	for (int i = 0; i < m_rankBattleInfo.byMemberCount; i++)
	{
		CPlayer* pPlayer = g_pObjectManager->GetPC(m_rankBattleInfo.asMember[i].hPc);
		if (pPlayer && pPlayer->IsInitialized())
		{
			pPlayer->LeaveRankBattleQueue();
		}

		m_rankBattleInfo.asMember[i].hPc = INVALID_HOBJECT;
	}

	m_rankBattleInfo.byMemberCount = 0;
}

void CParty::PlayerCancelRankTeleport(HOBJECT hId)
{
	if (m_rankBattleInfo.byMemberCount == 1)
	{
		ResetRankBattleInfo();

		return;
	}

	//SET BATTLERANK INFO
	for (int e = 0; e < m_rankBattleInfo.byMemberCount; e++)
	{
		if (hId == m_memberInfo[e].hHandle)
		{
			m_rankBattleInfo.asMember[e].hPc = INVALID_HOBJECT;

			for (int ee = e; ee < m_rankBattleInfo.byMemberCount; ee++)
			{
				if (ee == NTL_MAX_MEMBER_IN_PARTY)
					break;

				//check if next member slot is a member. if true then set that member on the leaver slot
				if (m_memberInfo[ee + 1].hHandle != INVALID_HOBJECT)
				{
					m_rankBattleInfo.asMember[ee].hPc = m_rankBattleInfo.asMember[ee + 1].hPc;
				}
				else
				{
					if (ee + 1 <= NTL_MAX_MEMBER_IN_PARTY)
					{
						m_rankBattleInfo.asMember[ee + 1].hPc = INVALID_HOBJECT;
					}
				}
			}

			break;
		}
	}

	m_rankBattleInfo.byMemberCount = UnsignedSafeDecrease<BYTE>(m_rankBattleInfo.byMemberCount, 1);
}


CPlayer * CParty::GetRankBattlePlayer(BYTE bySlot)
{
	CPlayer* pPlayer = g_pObjectManager->GetPC(m_rankBattleInfo.asMember[bySlot].hPc);
	if (pPlayer && pPlayer->IsInitialized())
	{
		return pPlayer;
	}

	return NULL;
}

void CParty::ResetRankBattleInfo()
{
	m_rankBattleInfo.roomId = INVALID_ROOMID;
	m_rankBattleInfo.rankBattleTblidx = INVALID_TBLIDX;
	m_rankBattleInfo.hObject = INVALID_HOBJECT;
	m_rankBattleInfo.byMemberCount = 0;
	memset(m_rankBattleInfo.asMember, NULL, sizeof(m_rankBattleInfo.asMember));
}

int CParty::GetPartyLevel()
{
	int nlv = 0;

	for (BYTE i = 0; i < m_byMemberInfoCount; i++)
	{
		nlv += (int)m_memberInfo[i].byLevel;
	}

	return nlv;
}

BYTE CParty::GetMemberCanEnterRankbattleQueueCount(BYTE byMinLevel, BYTE byMaxLevel)
{
	CGameServer* app = (CGameServer*)g_pApp;
	BYTE byCount = 0;

	for (int i = 0; i < m_byMemberInfoCount; i++)
	{
		CPlayer* pPlayer = g_pObjectManager->GetPC(m_memberInfo[i].hHandle);
		if (pPlayer && pPlayer->IsInitialized())
		{
			if (pPlayer->GetCurWorld() == NULL)
				continue;
			if (pPlayer->GetCurWorld()->GetTbldat()->bDynamic)
				continue;

			if (pPlayer->GetRankBattleRoomTblidx() != INVALID_TBLIDX)
				continue;
			if (pPlayer->GetRankBattleRoomId() != INVALID_ROOMID)
				continue;
			if (pPlayer->GetLevel() < byMinLevel)
				continue;
			if (pPlayer->GetLevel() > byMaxLevel)
				continue;
			if (pPlayer->GetDragonballScramble() && app->GetGsChannel() == 0)
				continue;
			
			++byCount;
		}
	}

	return byCount;
}


void CParty::CancelDirectPlay(HOBJECT hPc)
{
	for (int i = 0; i < m_byMemberInfoCount; i++)
	{
		if (m_memberInfo[i].hHandle == hPc)
		{
			if (m_bSkipDirectPlay[i]) //if already canceled then return
				return;

			++m_bySkipDirectPlayCount;
			m_bSkipDirectPlay[i] = true;

			CNtlPacket packet(sizeof(sGU_CHAR_DIRECT_PLAY_CANCEL_NFY));
			sGU_CHAR_DIRECT_PLAY_CANCEL_NFY * res = (sGU_CHAR_DIRECT_PLAY_CANCEL_NFY*)packet.GetPacketData();
			res->wOpCode = GU_CHAR_DIRECT_PLAY_CANCEL_NFY;
			res->hPc = hPc;
			SendMsgToParty(&packet);

			break;
		}
	}
}


void CParty::ResetPartyDirectPlay()
{
	m_bySkipDirectPlayCount = 0;
	memset(m_bSkipDirectPlay, false, sizeof(m_bSkipDirectPlay));
}

bool CParty::IsCancelDirectPlay(BYTE byPlayerCount)
{
	if (m_bySkipDirectPlayCount >= byPlayerCount)
	{
		ResetPartyDirectPlay();
		return true;
	}

	return false;
}

bool CParty::MemberHasScript(DWORD scriptId, eSSD_SCRIPT_TYPE scripType)
{
	for (int i = 0; i < m_byMemberInfoCount; i++)
	{
		CPlayer* pPlayer = g_pObjectManager->GetPC(m_memberInfo[i].hHandle);
		if (pPlayer && pPlayer->IsInitialized())
		{
			CScriptAlgoObject* pScript = pPlayer->GetScript(scriptId);
			if (pScript && pScript->GetScriptType() == scripType)
				return true;
		}
	}

	return false;
}


void CParty::ShareQuest(CHARACTERID hActor, QUESTID qId)
{
	for (int i = 0; i < m_byMemberInfoCount; i++)
	{
		if (m_memberInfo[i].hHandle == hActor)
			continue;

		CPlayer* pPlayer = g_pObjectManager->GetPC(m_memberInfo[i].hHandle);
		if (pPlayer && pPlayer->IsInitialized())
		{
			if (pPlayer->GetQuests()->CheckQuestCounter(qId) && pPlayer->GetQuests()->GetQuestProgress(qId) == NULL)
			{
				CNtlPacket packet(sizeof(sGU_QUEST_SHARE_NFY));
				sGU_QUEST_SHARE_NFY * res = (sGU_QUEST_SHARE_NFY *)packet.GetPacketData();
				res->wOpCode = GU_QUEST_SHARE_NFY;
				res->wResultCode = 0; //RESULT_SUCCESS
				res->hActor = hActor;
				res->tId = qId;
				packet.SetPacketLen(sizeof(sGU_QUEST_SHARE_NFY));
				pPlayer->SendPacket(&packet);
			}
		}
	}
}

void CParty::RemoveMembersBuffs(CPlayer * pPlayer)
{
	if (pPlayer->GetCurWorld() == NULL)
		return;

	HOBJECT hHandle = pPlayer->GetID();

	for (int i = 0; i < m_byMemberInfoCount; i++)
	{
		CPlayer* pMember = g_pObjectManager->GetPC(m_memberInfo[i].hHandle);
		if (pMember && pMember->IsInitialized() && pMember->GetID() != hHandle && pMember->GetCurWorld() && pMember->GetCurWorld()->GetTbldat()->bDynamic)
		{
			pMember->GetBuffManager()->RemoveBlessBuffsFromCaster(hHandle);
		}
	}
}


void CParty::InitPartySelect(bool bLastStage, TBLIDX rewardItemIdx)
{
	m_rewardItemIdx = rewardItemIdx;
	m_bLastStage = bLastStage;

	for (int i = 0; i < m_byMemberInfoCount; i++)
	{
		CPlayer* pPlayer = g_pObjectManager->GetPC(m_memberInfo[i].hHandle);
		if (pPlayer && pPlayer->IsInitialized() && pPlayer->GetCCBD())
		{
			CNtlPacket packet(sizeof(sGU_PARTY_SELECT_STATE_INIT_NFY));
			sGU_PARTY_SELECT_STATE_INIT_NFY * res = (sGU_PARTY_SELECT_STATE_INIT_NFY*)packet.GetPacketData();
			res->wOpCode = GU_PARTY_SELECT_STATE_INIT_NFY;
			res->bySelectState = NTL_PARTY_SELECT_TYPE_CCBD_REWARD;
			res->dwParam = rewardItemIdx;
			res->bLastStage = bLastStage;
			packet.SetPacketLen(sizeof(sGU_PARTY_SELECT_STATE_INIT_NFY));
			pPlayer->SendPacket(&packet);
		}
	}
}

void CParty::PartyMemberSelect(HOBJECT hPlayer, BYTE bySelectState)
{
	m_mapRewardSelectState[hPlayer] = bySelectState;

	CNtlPacket packet(sizeof(sGU_PARTY_SELECT_STATE_NFY));
	sGU_PARTY_SELECT_STATE_NFY * res = (sGU_PARTY_SELECT_STATE_NFY*)packet.GetPacketData();
	res->wOpCode = GU_PARTY_SELECT_STATE_NFY;
	res->hTarget = hPlayer;
	res->bySelectState = bySelectState;
	packet.SetPacketLen(sizeof(sGU_PARTY_SELECT_STATE_NFY));
	SendMsgToPartyExceptOne(&packet, hPlayer);
}

void CParty::DecidePartySelect()
{
	CNtlPacket packet(sizeof(sGU_BATTLE_DUNGEON_REWARD_FINISH_NFY));
	sGU_BATTLE_DUNGEON_REWARD_FINISH_NFY* res = (sGU_BATTLE_DUNGEON_REWARD_FINISH_NFY *)packet.GetPacketData();
	res->wOpCode = GU_BATTLE_DUNGEON_REWARD_FINISH_NFY;
	packet.SetPacketLen(sizeof(sGU_BATTLE_DUNGEON_REWARD_FINISH_NFY));

	for (int i = 0; i < m_byMemberInfoCount; i++)
	{
		CPlayer* pPlayer = g_pObjectManager->GetPC(m_memberInfo[i].hHandle);
		if (pPlayer && pPlayer->IsInitialized() && pPlayer->GetCCBD())
		{
			pPlayer->SendPacket(&packet);

			std::map<HOBJECT, BYTE>::iterator it = m_mapRewardSelectState.find(pPlayer->GetID());
			BYTE bySelectState = NTL_PARTY_SELECT_TYPE_CCBD_REWARD;

			if(it != m_mapRewardSelectState.end())
				bySelectState = it->second;

			if (m_bLastStage || bySelectState == NTL_PARTY_SELECT_TYPE_CCBD_REWARD)
			{
				CWorld* pWorld = pPlayer->GetCurWorld();
				if (pWorld)
				{
					CNtlVector destLoc(pWorld->GetTbldat()->outWorldLoc);
					WORLDID destWorldID = pWorld->GetTbldat()->outWorldTblidx;

					g_pItemManager->CreateItem(pPlayer, m_rewardItemIdx, 1);
					pPlayer->StartTeleport(destLoc, pPlayer->GetCurDir(), destWorldID, TELEPORT_TYPE_WORLD_MOVE);
				}
			}
			else
			{
				CNtlVector destLoc(g_pTableContainer->GetServerConfigTable()->GetServerConfigData()->sBattleDungeonData.sEnterLoc_NormalStage.sLoc);
				CNtlVector destDir(g_pTableContainer->GetServerConfigTable()->GetServerConfigData()->sBattleDungeonData.sEnterLoc_NormalStage.sDir);

				pPlayer->StartTeleport(destLoc, destDir, pPlayer->GetWorldID(), TELEPORT_TYPE_DEFAULT, g_pTableContainer->GetServerConfigTable()->GetServerConfigData()->sBattleDungeonData.directPlay_StageChange, true);
			}
		}
	}

	m_mapRewardSelectState.clear();
}

bool CParty::CreatePartyInventoryItem(CItemDrop * pDrop)
{
	if (m_mapPartyInventory.size() >= 3)
		return false;

	BYTE byInventoryPos = INVALID_BYTE;

	//Get free inventory pos
	for (BYTE i = 0; i < 3; i++)
	{
		if (m_mapPartyInventory.find(i) == m_mapPartyInventory.end())
		{
			byInventoryPos = i;
			break;
		}
	}

	if (byInventoryPos == INVALID_BYTE)
		return false;

	//lock the drop
	pDrop->SetLocked(true);

	sPARTY_ITEM partyItem;
	partyItem.hHandle = pDrop->GetID();
	partyItem.dwDiceTimeLeft = DBO_MAX_TIME_PARTY_DICE_ITEM;
	partyItem.itemInfo.tblidx = pDrop->GetTblidx();
	partyItem.itemInfo.byStackcount = 1;
	partyItem.itemInfo.byRank = pDrop->GetRank();
	partyItem.itemInfo.byGrade = pDrop->GetGrade();
	partyItem.itemInfo.byBattleAttribute = pDrop->GetBattleAttribute();
	partyItem.itemInfo.bNeedToIdentify = pDrop->NeedToIdentify();
	memcpy(&partyItem.itemInfo.sOptionSet, pDrop->GetOptionSet(), sizeof(sITEM_OPTION_SET));

	m_mapPartyInventory.insert(std::make_pair(byInventoryPos, partyItem));

	return true;
}

bool CParty::ShareItemDropInOrder(CItemDrop * pDrop)
{
	for (int i = 0; i < NTL_MAX_MEMBER_IN_PARTY; i++) //must not be "m_byMemberInfoCount" otherwise issues when member(s) out of range
	{
		if (m_byLastItemOrderPartyIndex == NTL_MAX_MEMBER_IN_PARTY)
			m_byLastItemOrderPartyIndex = 0;

		if (GetMemberInfo(m_byLastItemOrderPartyIndex).hHandle != INVALID_HOBJECT)
		{
			CPlayer* pPlayer = g_pObjectManager->GetPC(GetMemberInfo(m_byLastItemOrderPartyIndex++).hHandle);
			if (pPlayer && pPlayer->IsInitialized())
			{
				if (pPlayer->IsInRange(pDrop, DBO_VALID_DISTANCE_TO_GET_ITEM_IN_PARTY))
				{
					if (g_pItemManager->CreateItem(pPlayer, pDrop))
						return true;
				}
			}
		}
		else
		{
			m_byLastItemOrderPartyIndex = 0;
		}
	}

	return false;
}

void CParty::RollDice(HOBJECT hId, BYTE byDice)
{
	if (m_diceProgress.bInventoryDiceInProgress == false)
		return;

	for (BYTE i = 0; i < m_diceProgress.byReceivedDiceCount; i++)
	{
		if (m_diceProgress.ahPlayerHandle[i] == hId)
			return;

		if (m_diceProgress.ahPlayerHandle[i] == 0)
		{
			m_diceProgress.ahPlayerHandle[i] = hId;
			m_diceProgress.abyDice[i] = byDice;

			if (byDice == DBO_ITEM_DICE_DICE)
			{
				m_diceProgress.abyDiceResult[i] = m_diceProgress.dice.RollDice();

				if (m_diceProgress.abyDiceResult[i] > m_diceProgress.byHighestDice)
				{
					m_diceProgress.byHighestDice = m_diceProgress.abyDiceResult[i];
					m_diceProgress.hGainCharHandle = hId;
				}
			}
			else if (byDice == DBO_ITEM_DICE_RESERVE)
			{
				m_diceProgress.vecReservedPlayerHandle.push_back(hId);
				++m_diceProgress.byReservedCount;
			}

			//increase counter
			++m_diceProgress.byPlayerCount;

			//if everyone diced, then end the dice
			if (m_diceProgress.byPlayerCount == m_diceProgress.byReceivedDiceCount)
			{
				std::map<BYTE, sPARTY_ITEM>::iterator it = m_mapPartyInventory.find(m_diceProgress.byInventorySlotDiceInProgress);
				if (it != m_mapPartyInventory.end())
				{
					sPARTY_ITEM& partyItem = it->second;
					
					DecideDice(partyItem);
				}
			}

			break;
		}
	}
}


void CParty::LeaveDice(HOBJECT hId)
{
	if (m_diceProgress.bInventoryDiceInProgress == false)
		return;

	bool bParticipated = false;

	//check if player participated in the dice
	for (BYTE i = 0; i < m_diceProgress.byPlayerCount; i++)
	{
		if (m_diceProgress.ahPlayerHandle[i] == hId)
		{
			bParticipated = true;

			m_diceProgress.ahPlayerHandle[i] = 0;
			m_diceProgress.abyDiceResult[i] = 0;

			if (m_diceProgress.abyDice[i] == DBO_ITEM_DICE_DICE)
			{
				//check if player which has highest dice leaves
				if (m_diceProgress.hGainCharHandle == hId)
				{
					m_diceProgress.byHighestDice = 0; //set highest dice to 0

					//get new highest dice
					for (BYTE ii = 0; ii < m_diceProgress.byReceivedDiceCount; ii++)
					{
						if (m_diceProgress.abyDiceResult[ii] > m_diceProgress.byHighestDice)
						{
							m_diceProgress.byHighestDice = m_diceProgress.abyDiceResult[ii];
							m_diceProgress.hGainCharHandle = m_diceProgress.ahPlayerHandle[ii];
						}
					}
				}
			}
			else if (m_diceProgress.abyDice[i] == DBO_ITEM_DICE_RESERVE)
			{
				for (std::vector<HOBJECT>::iterator iter = m_diceProgress.vecReservedPlayerHandle.begin(); iter != m_diceProgress.vecReservedPlayerHandle.end(); iter++)
				{
					if (*iter == hId)
					{
						--m_diceProgress.byReservedCount;
						m_diceProgress.vecReservedPlayerHandle.erase(iter);
						break;
					}
				}
			}

			break;
		}
	}

	if (bParticipated == false)
	{
		//increase dice counter
		++m_diceProgress.byPlayerCount;

		//if everyone diced, then end the dice
		if (m_diceProgress.byPlayerCount == m_diceProgress.byReceivedDiceCount)
		{
			std::map<BYTE, sPARTY_ITEM>::iterator it = m_mapPartyInventory.find(m_diceProgress.byInventorySlotDiceInProgress);
			if (it != m_mapPartyInventory.end())
			{
				sPARTY_ITEM& partyItem = it->second;

				DecideDice(partyItem);
			}
		}
	}
}


void CParty::DecideDice(sPARTY_ITEM & partyItem)
{
	//printf("DecideDice \n");
	CItemDrop* pDrop = g_pItemManager->FindDrop(partyItem.hHandle);
	if (pDrop && pDrop->IsInitialized() && pDrop->GetCurWorld())
	{
		//printf("item drop found \n");
		bool bDestroyItem = true;
		bool bItemCreated = false;

		if (m_diceProgress.hGainCharHandle != INVALID_HOBJECT)
		{
			CPlayer* player = g_pObjectManager->GetPC(m_diceProgress.hGainCharHandle);
			if (player && player->IsInitialized())
			{
				if (g_pItemManager->CreateItem(player, pDrop) == true)
				{
					bItemCreated = true;
				}
				else
				{
					player->GetClientSession()->SendResultcode(818); // GAME_ITEM_INVEN_FULL

				}
			}
		}

		if (bItemCreated == false && m_diceProgress.byReservedCount > 0)
		{
			CPlayer* player = g_pObjectManager->GetPC(m_diceProgress.vecReservedPlayerHandle[RandomRange(0, m_diceProgress.byReservedCount - 1)]);
			if (player && player->IsInitialized())
			{
				if (g_pItemManager->CreateItem(player, pDrop) == true)
				{
					bItemCreated = true;
				}
				else
				{
					player->GetClientSession()->SendResultcode(818); // GAME_ITEM_INVEN_FULL
				}
			}
		}


		CNtlPacket packet(sizeof(sGU_ITEM_DICE_RESULT_NFY));
		sGU_ITEM_DICE_RESULT_NFY * res = (sGU_ITEM_DICE_RESULT_NFY*)packet.GetPacketData();
		res->wOpCode = GU_ITEM_DICE_RESULT_NFY;
		res->hItemHandle = partyItem.hHandle;
		res->bCancelDice = (bItemCreated == true) ? false:true;
		res->hGainCharHandle = m_diceProgress.hGainCharHandle;

		for (BYTE i = 0; i < m_diceProgress.byReceivedDiceCount; i++)
		{
			if (m_diceProgress.ahPlayerHandle[i] != 0)
			{
				res->hCharHandle[res->byDiceCount] = m_diceProgress.ahPlayerHandle[i];
				res->byDice[res->byDiceCount] = m_diceProgress.abyDice[i];
				res->byDiceNum[res->byDiceCount] = m_diceProgress.abyDiceResult[i];

				++res->byDiceCount;
			}
		}

		packet.SetPacketLen(sizeof(sGU_ITEM_DICE_RESULT_NFY));

		for (BYTE i = 0; i < res->byDiceCount; i++)
		{
			CPlayer* player = g_pObjectManager->GetPC(res->hCharHandle[i]);
			if (player && player->IsInitialized())
			{
				player->SetItemDice(false);
				player->SendPacket(&packet);
			}
		}

		if (bDestroyItem)
		{
			pDrop->SetLocked(false); //unlock item
			g_pItemManager->DestroyItemDropOverTime(pDrop);
		}
	}

	//remove item from party inventory
	m_mapPartyInventory.erase(m_diceProgress.byInventorySlotDiceInProgress);

	//init dice in progress
	m_diceProgress.Init();
}


