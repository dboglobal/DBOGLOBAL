#ifndef __INC_DBOG_PARTY_H__
#define __INC_DBOG_PARTY_H__

#include "NtlSingleton.h"
#include "EventableObject.h"
#include "NtlPartyDungeon.h"
#include "NtlRankBattle.h"
#include "DiceManager.h"
#include "NtlItem.h"


class CPlayer;
class CNtlPacket;
class CCharacter;
class CUltimateDungeon;
class CBattleDungeon;
class CWorld;
class CScriptAlgoObject;
enum eSSD_SCRIPT_TYPE;
class CItemDrop;

class CParty : public EventableObject
{

public:

	CParty();
	virtual ~CParty();

public:

	void		TickProcess(DWORD dwTickDiff);

public:

	bool		AddPartyMember(CPlayer* player);
	void		LeaveParty(CPlayer* player);

	void		SetMemberInfo(CPlayer* pPlayer, int nMemberCount);

	bool		IsMemberInsideGuild();

	HOBJECT		GetPartyLeaderID()	{ return m_hLeader; }
	void		SetPartyLeaderID(HOBJECT Leader)	{ m_hLeader = Leader; }
	void		UpdatePartyLeader(HOBJECT newLeader);

	void		KickPartyMember(CPlayer* kickedplayer);

	void		ShareTarget(CCharacter* pTarget, BYTE bySlot);
	void		RemoveTargetMark(HOBJECT hTarget);

	void		SetItemLootingMethod(BYTE byMethod);
	void		SetZeniLootingMethod(BYTE byMethod);
	void		SetItemLootingMethodRank(BYTE byRank);
	
	BYTE		GetItemLootingMethod() { return m_byItemLootingMethod; }
	BYTE		GetZeniLootingMethod() { return m_byZennyLootingMethod; }
	BYTE		GetItemLootingMethodRank() { return m_byItemRank; }

	PARTYID		GetPartyID()			{ return m_partyID; }
	void		SetPartyID(PARTYID id)	{ m_partyID = id; }
	void		SetPartyName(WCHAR* name);
	WCHAR*		GetPartyName()	{ return m_awchPartyName; }

	BYTE		GetPartyMemberCount()	{ return m_byMemberInfoCount; }

	sPARTY_MEMBER_INFO&	GetMemberInfo(BYTE slot)	{ return m_memberInfo[slot]; }

	BYTE		GetPartyMemberSlot(HOBJECT hMember);

	void		SetMemberCount(bool bIsPlus);

	bool		IsPartyMember(HOBJECT hTarget);

	void		SetMemberLP(CPlayer* player);
	void		SetMemberEP(CPlayer* player);
	void		SetMemberLpEp(CPlayer* player);

	void		UpdateMemberLocation(CPlayer* player);
	void		UpdateMemberLevel(CPlayer* player);
	void		UpdateMemberClass(CPlayer* player);

	void		GetPartyBuffInfo(CPlayer* player);

	//send msg(packet)
	void		SendMsgToParty(CNtlPacket * pPacket);
	void		SendMsgToPartyExceptOne(CNtlPacket * pPacket, HOBJECT hHandle);

	void		BroadcastWithinRange(CNtlPacket * pPacket, CPlayer* pSender, float fRange);

	void		AddEventIdToAll(DWORD dwEventID);
	void		AddEventIdToWithinRange(DWORD dwEventID, CPlayer* pSender, float fRange);

	//party aggro
	void		UpdatePartyAggro(HOBJECT hSubject, CNtlPacket * pPacket);

	//party tp
	void		StartPartyTeleport(CPlayer* player);


	void		EnterDungeon(CPlayer* pPartyLeader, CUltimateDungeon* pDungeon, CWorld* pWorld, CScriptAlgoObject* pScript);
	void		EnterBattleDungeon(CPlayer* pPartyLeader, CBattleDungeon* pDungeon, CWorld* pWorld, CScriptAlgoObject* pScript);

	void		ShareZeniDropInParty(CPlayer* pPlayer, DWORD dwZeni);

	bool		IsSomeoneInDynamic(WORLDID destWorldId);

	bool		IsEveryoneInLeaderRange(CPlayer* pLeader, float fRange);

	CPlayer*	GetPlayer(BYTE bySlot);

	//Rank battle
	bool		RegisterRankBattle(HOBJECT hObject, TBLIDX roomTblidx);
	void		SetRankBattleRoomID(ROOMID roomId) { m_rankBattleInfo.roomId = roomId; }
	ROOMID		GetRankBattleRoomId() { return m_rankBattleInfo.roomId; }
	TBLIDX		GetRankBattleRoomTblidx() { return m_rankBattleInfo.rankBattleTblidx; }
	void		LeaveRankBattleQueue();
	void		PlayerCancelRankTeleport(HOBJECT hId);
	BYTE		GetRankBattleMemberCount() { return m_rankBattleInfo.byMemberCount; }
	CPlayer*	GetRankBattlePlayer(BYTE bySlot);
	void		ResetRankBattleInfo();

	int			GetPartyLevel();
	BYTE		GetMemberCanEnterRankbattleQueueCount(BYTE byMinLevel, BYTE byMaxLevel);


	void		CancelDirectPlay(HOBJECT hPc);
	void		ResetPartyDirectPlay();
	bool		IsCancelDirectPlay(BYTE byPlayerCount);
	
	bool		MemberHasScript(DWORD scriptId, eSSD_SCRIPT_TYPE scripType);

	void		ShareQuest(CHARACTERID hActor, QUESTID qId);

	template <class Func> void ForEachOnlineMember(Func & f);

private:

	void		RemoveMembersBuffs(CPlayer* pPlayer); // remove all buffs from party members which were given from pPlayer

private:

	WCHAR						m_awchPartyName[NTL_MAX_SIZE_PARTY_NAME + 1];
	PARTYID						m_partyID;

	HOBJECT						m_hLeader;

	BYTE						m_byItemLootingMethod;
	BYTE						m_byZennyLootingMethod;
	BYTE						m_byItemRank;

	sSHARETARGET_INFO			m_sharetargetInfo[NTL_MAX_SHARETARGET_COUNT];

	BYTE						m_byMemberInfoCount;
	sPARTY_MEMBER_INFO			m_memberInfo[NTL_MAX_MEMBER_IN_PARTY];

	ePARTY_DUNGEON_STATE		m_eDiff;

	sRANKBATTLE_ROOMINFO_PARTY	m_rankBattleInfo;

	BYTE						m_bySkipDirectPlayCount;
	bool						m_bSkipDirectPlay[NTL_MAX_MEMBER_IN_PARTY];

public:

	void		SetDungeonDiff(ePARTY_DUNGEON_STATE eDiff);
	BYTE		GetDungeonDiff()	{ return (BYTE)m_eDiff; }

	void		InitDungeon();


private:
	bool			bIsInPartySearch;

public:
	void		SetPartySearch(bool b)	{ bIsInPartySearch = b; }
	BYTE		IsInPartySearch()	{ return bIsInPartySearch; }


	//party select (used for ccbd)
public:

	void							InitPartySelect(bool bLastStage, TBLIDX rewardItemIdx);
	void							PartyMemberSelect(HOBJECT hPlayer, BYTE bySelectState);
	void							DecidePartySelect(); //return true when not last stage and decide next stage

private:

	TBLIDX							m_rewardItemIdx;
	bool							m_bLastStage;
	std::map<HOBJECT, BYTE>			m_mapRewardSelectState;

//party inventory
private:

	struct sPARTY_ITEM
	{
		DWORD						dwDiceTimeLeft;
		HOBJECT						hHandle;
		sITEM_PROFILE				itemInfo;
	};

	std::map<BYTE, sPARTY_ITEM>		m_mapPartyInventory;


	struct sDICE_PROGRESS
	{
		void Init()
		{
			bInventoryDiceInProgress = false;
			byInventorySlotDiceInProgress = INVALID_BYTE;
			byReceivedDiceCount = 0;
			byPlayerCount = 0;
			memset(ahPlayerHandle, NULL, sizeof(ahPlayerHandle));
			memset(abyDiceResult, NULL, sizeof(abyDiceResult));
			hGainCharHandle = INVALID_HOBJECT;
			byHighestDice = 0;
			byReservedCount = 0;
			vecReservedPlayerHandle.clear();
			dice.Init();
		}

		//dice manager
		CDiceManager dice;

		bool		bInventoryDiceInProgress;
		BYTE		byInventorySlotDiceInProgress;
		HOBJECT		hInventoryItemHandleDiceInProgress;

		BYTE		byReceivedDiceCount; //amount players received the dice
		BYTE		byPlayerCount; //amount players participated in the dice
		HOBJECT		ahPlayerHandle[NTL_MAX_MEMBER_IN_PARTY];
		BYTE		abyDiceResult[NTL_MAX_MEMBER_IN_PARTY];
		BYTE		abyDice[NTL_MAX_MEMBER_IN_PARTY];
		HOBJECT		hGainCharHandle;
		BYTE		byHighestDice;
		BYTE		byReservedCount;
		std::vector<HOBJECT>	vecReservedPlayerHandle;

	}m_diceProgress;

	BYTE							m_byLastItemOrderPartyIndex;

public:

	bool							CreatePartyInventoryItem(CItemDrop* pDrop);
	bool							ShareItemDropInOrder(CItemDrop* pDrop);

	void							RollDice(HOBJECT hId, BYTE byDice);
	void							LeaveDice(HOBJECT hId);

	void							DecideDice(sPARTY_ITEM& partyItem);

	HOBJECT							GetDiceItemHandle() { return m_diceProgress.hInventoryItemHandleDiceInProgress; }

};

#include "ObjectManager.h" //temp

template <class Func> void CParty::ForEachOnlineMember(Func & f)
{
	for(int i = 0; i < m_byMemberInfoCount; i++)
	{
		CPlayer* player = g_pObjectManager->GetPC(m_memberInfo[i].hHandle);
		if(player)
			f(player);
	}
}





// PARTY MANAGER
class CPartyManager : public CNtlSingleton<CPartyManager>
{

public:
	CPartyManager();
	virtual ~CPartyManager();

private:

	PARTYID						m_PartyHandler;
	std::vector<PARTYID>		m_vPartyHandlerVec;

	std::map<PARTYID, CParty*>	m_map_Party;


public:

	void		TickProcess(DWORD dwTickDiff);

public:

	CParty*		CreateParty(CPlayer* pLeader, WCHAR* name, bool bSendCreatePartyPacket = false);
	CParty*		GetParty(PARTYID partyid);
	void		DisbandParty(CParty* party);

	int			GetPartyCount() { return (int)m_map_Party.size(); }

	void		CleanMark(std::set<PARTYID>* pSetMarkedByEnemyParty, HOBJECT hTarget);

	PARTYID		CreatePartyID();
	void		DeletePartyID(PARTYID partyID);

	void		BudokaiCreateParty(CPlayer* pPlayer, WCHAR* teamName, CHARACTERID leaderCharId);

};

#define GetPartyManager()		CPartyManager::GetInstance()
#define g_pPartyManager			GetPartyManager()


#endif