#ifndef __PLAYER_CACHE_H__
#define __PLAYER_CACHE_H__

#include "NtlSingleton.h"
#include <set>
#include <unordered_map>

#include "CallBack.h"

#include "NtlCharacter.h"
#include "NtlTeleport.h"
#include "NtlHlsItem.h"
#include "NtlMail.h"
#include "NtlRankBattle.h"
#include "NtlMascot.h"
#include "NtlEventDef.h"


class CAccountCache
{
public:

	CAccountCache(ACCOUNTID accountID);
	virtual ~CAccountCache();

private:

	void					Init();
	void					Destroy();

public:

	void					SetSession(HSESSION hSession) { m_hServerSession = hSession; } //set server session to which we send packet
	HSESSION				GetSession() { return m_hServerSession; }

public:

	void					OnLoadAccountInfo(QueryResultVector & results);

	void					OnLoadEventReward(QueryResultVector & results, HOBJECT hHandle, CHARACTERID charId);

	void					SendShortcutLoadRes(CHARACTERID charId);
	void					SendCashshopLoadRes(CHARACTERID charId, HOBJECT hHandle);

public:

	bool					RemoveCashItem(QWORD qwProductId);
	bool					CashItemDecreaseCount(QWORD qwProductId, bool& rbDelete);
	DWORD					GetCash() { return m_dwCash; }
	void					SetCash(DWORD dwCash) { m_dwCash = dwCash; }

	DWORD					GetEventCoin() { return m_dwEventCoins; }
	void					SetEventCoin(DWORD dwCoins) { m_dwEventCoins = dwCoins; }

	DWORD					GetWaguCoin() { return m_dwWaguCoins; }
	void					SetWaguCoin(DWORD dwCoins) { m_dwWaguCoins = dwCoins; }

	BYTE					GetGmLevel() { return m_byGameMasterLevel; }

	void					InsertCashItem(sCASHITEM_BRIEF* pBrief);

	void					AddShortcut(WORD wActionID, WORD wKey);
	void					DeleteShortcut(WORD wActionID);
	bool					UpdateShortcut(WORD wActionID, WORD wNewkey);

	void					AddBankItem(sITEM_DATA & rData);
	void					EraseBankItem(ITEMID itemId);
	void					InsertBankItem(sITEM_DATA* pData);
	sITEM_DATA*				GetBankItem(ITEMID itemId, BYTE byPlace, BYTE byPos);
	sITEM_DATA*				GetBankItem(TBLIDX tblidx);
	void					SendBankItemLoad(CHARACTERID charId);
	bool					IsBankLoaded() { return m_bIsBankLoaded; }
	void					SetBankLoaded(bool bFlag) { m_bIsBankLoaded = bFlag; }

	bool					HasEventReward(TBLIDX rewardTblidx, CHARACTERID charId);
	inline void				EraseEventReward(TBLIDX rewardTblidx) { m_mapEventReward.erase(rewardTblidx); }
	inline void				ClearEventReward() { m_mapEventReward.clear(); }

private:

	HSESSION								m_hServerSession;

	ACCOUNTID								m_accountID;

	BYTE									m_byGameMasterLevel;

	bool									m_bIsBankLoaded;

	DWORD									m_dwCash; //mallpoints
	BYTE									m_byPremiumSlots;
	DWORD									m_dwEventCoins;
	DWORD									m_dwWaguCoins;

	std::set<CHARACTERID>					m_setCharId;
	std::map<QWORD, sCASHITEM_BRIEF*>		m_mapCashItems; //first = qwProductId
	std::map<WORD, WORD>					m_mapShortcutInfo; //first = wActionID, second = wKey

	std::map<TBLIDX, sEVENT_PC_NAME>		m_mapEventReward;

	std::map<ITEMID, sITEM_DATA*>			m_mapBank;
};


class CPlayerCache
{

public:

	CPlayerCache(ACCOUNTID accountID, CAccountCache* pAccountCache);
	virtual ~CPlayerCache();


private:

	void						Init();
	void						Destroy();

public:

	void						SetSession(HSESSION hSession) { m_hServerSession = hSession; } //set server session to which we send packet
	HSESSION					GetSession() { return m_hServerSession; }

public:

	void						SendPcDataLoadRes();
	void						SendSkillLoadRes();
	void						SendBuffLoadRes();
	void						SendHtbSkillLoadRes();
	void						SendItemLoadRes();
	void						SendQuestItemLoadRes();
	void						SendQuestCompleteLoadRes();
	void						SendQuestProgressLoadRes();
	void						SendQuickSlotLoadRes();
	void						SendItemRecipeLoadRes();
	void						SendMascotLoadRes();
	void						SendItemCoolTimeLoadRes();
	void						SendPortalLoadRes();

	void						SendBankItemLoad();

public:

	void						OnLoadPcData(QueryResultVector & results);
	void						OnLoadPcData2(QueryResultVector & results);

	void						OnLoadBank(QueryResultVector & results, CAccountCache* pAccountCache, bool bLoadSharedBankFromCache, HOBJECT hObject, HOBJECT hNpcHandle);

public:

	ACCOUNTID					GetAccountID() { return m_accountID; }
	CHARACTERID					GetCharID() { return m_sPcData.charId; }

	void						SetTutorialFlag(bool bFlag) { m_bTutorialFlag = bFlag; }

	void						SetIsMailAway(bool bFlag) { m_sPcData.bIsMailAway = bFlag; }

	void						SetGuild(GUILDID guildId, WCHAR* wszGuildName);
	
	DWORD						GetSkillPoints() { return m_sPcData.dwSpPoint; }
	void						SetSkillPoints(DWORD dwPoints);

	DWORD						GetZeni() { return m_sPcData.dwMoney; }
	void						SetZeni(DWORD dwZeni);
	DWORD						GetMudusaPoints() { return m_sPcData.dwMudosaPoint; }
	void						SetMudusaPoints(DWORD dwPoints);

	DWORD						GetReputation() { return m_sPcData.dwReputation; }

	void						SetJoinID(JOINID joinid) { m_sPcData.wJoinID = joinid; }
	JOINID						GetJoinID() { return m_sPcData.wJoinID; }

	void						StoreRunTimeData(DWORD dwExp, TBLIDX worldIdx, WORLDID worldId, sVECTOR3& rLoc, sVECTOR3& rDir);
	void						SavePcData(const sPC_DATA& rPcData);
	void						SaveServerChangeInfo(const sDBO_SERVER_CHANGE_INFO& rInfo);

	bool						IsChannelChanged();

	void						UpdateWarFog(BYTE* pbyWarFogFlag);
	void						UpdateSelectedTitle(TBLIDX titleIdx);
	void						UpdateTitle(BYTE* pbyTitleIndexFlag);
	void						RegisterRecipe(TBLIDX recipeIdx, BYTE byRecipeType);

	void						RegisterMascot(BYTE byIndex, TBLIDX mascotTblidx, BYTE byRank, DWORD dwVP);
	void						DeleteMascot(BYTE byIndex);
	void						MascotSkillAdd(BYTE byMascotIndex, BYTE bySlot, TBLIDX skillTblidx);
	sMASCOT_DATA_EX*			GetMascotData(BYTE byIndex);
	void						EraseMascot(BYTE byIndex);

	void						AddPortal(PORTALID portalID);

	void						UpdateBind(WORLDID worldId, TBLIDX objectTblidx, BYTE byType, sVECTOR3& vLoc, sVECTOR3& vDir);

	void						SetChildAdult(bool bIsAdult);
	void						SetClass(BYTE byClass);
	BYTE						GetClass() { return m_sPcData.byClass; }
	void						SetGender(BYTE byGender);
	BYTE						GetGender() { return m_sPcData.byGender; }
	void						SetLevel(BYTE byLevel);
	BYTE						GetLevel() { return m_sPcData.byLevel; }
	void						SetExp(DWORD dwExp);
	void						SetInvisibleCostume(bool bFlag);
	void						SetCharName(WCHAR* wchName);
	WCHAR*						GetCharName() { return m_sPcData.awchName; }
	void						SetHoipoiMix(BYTE byLevel, DWORD dwExp);
	BYTE						GetRace() { return m_sPcData.byRace; }

	GUILDID						GetGuildID() { return m_sPcData.guildId; }

	void						UpdateQuickSlot(BYTE bySlot, BYTE byType, TBLIDX tblidx, ITEMID itemid);
	void						DeleteQuickSlot(BYTE bySlot);

	void						ClearItemCoolTime() { m_mapItemCoolTime.clear(); }
	void						UpdateItemCoolTime(sDBO_ITEM_COOL_TIME& rItemCoolTime);

	bool						UpdateSkillTime(sSKILL_DATA& rData);
	void						UpdateSkillData(sSKILL_DATA& rData);
	void						UpdateSkillData(BYTE bySkillIndex, TBLIDX skillIdx);
	void						AddSkillData(BYTE bySkillIndex, TBLIDX skillIdx, BYTE byRpBonusType, bool bRpAuto);
	void						InitSkills();
	bool						DeleteSkill(BYTE byIndex);
	sSKILL_DATA*				GetSkillData(BYTE byIndex);

	bool						UpdateHtbSkillTime(sHTB_SKILL_DATA& rData);
	void						AddHtbSkillData(TBLIDX skillIdx);
	void						AddHtbSkillData(TBLIDX skillIdx, DWORD dwSkillTime);

	bool						UpdateBuffTime(sBUFF_DATA& rData);
	bool						AddBuffData(sBUFF_DATA& rData);
	bool						DelBuffData(BYTE byBuffIndex);

	void						UpdateIdentifyItem(ITEMID itemId);
	void						UpdateItemDur(ITEMID itemId, BYTE byDur);
	bool						UpdateEquipItemDur(BYTE byPos, BYTE byDur);

	void						AddQuestItem(TBLIDX tblidx, BYTE byPos, BYTE byStack);
	bool						UpdateQuestItem(BYTE byPos, BYTE byCount);
	bool						CreateQuestItem(TBLIDX tblidx, BYTE byPos, BYTE byCount);
	bool						DeleteQuestItem(BYTE byPos);
	bool						MoveQuestItem(BYTE byPos, BYTE byDestPos);
	bool						SwitchQuestItem(BYTE byPos, BYTE byDestPos);

	bool						DeleteQuestProgressData(QUESTID qId);
	bool						CompleteQuest(QUESTID qId);
	void						StoreQuestProgress(sPROGRESS_QUEST_INFO& rQuest);

	void						AddQuickTeleport(BYTE bySlot, sQUICK_TELEPORT_INFO& rInfo);
	void						UpdateQuickTeleport(BYTE bySlot, sQUICK_TELEPORT_INFO& rInfo);
	bool						DeleteQuickTeleport(BYTE bySlot);
	bool						IsQuickTeleportLoaded() { return m_bIsQuickTeleportLoaded; }
	void						SetQuickTeleportLoaded(bool bFlag) { m_bIsQuickTeleportLoaded = bFlag; }
	void						FillQuickTeleportData(sQUICK_TELEPORT_INFO* pInfo, BYTE & rbyCount);

	bool						IsBankLoaded() { return m_bIsBankLoaded; }
	void						SetBankLoaded(bool bFlag) { m_bIsBankLoaded = bFlag; }
	

	bool						RemoveItem(ITEMID itemId);
	bool						SwitchItem(ITEMID itemId, CPlayerCache* pNewOwner, BYTE byNewPlace, BYTE byNewPos);
	void						InsertItem(sITEM_DATA* pItemData);
	void						EraseItem(ITEMID itemId);
	void						AddItem(sITEM_DATA& rItemData);
	void						AddItem(sSHOP_BUY_INVEN & rData, ITEMID itemId);
	bool						DecreaseItemCount(ITEMID itemId, BYTE byCount, bool& rbDelete);
	void						RenewItemDuration(DBOTIME endUseTime, ITEMID itemId);
	void						SetItemBattleAttribute(BYTE byBattleAttribute, ITEMID itemId);
	sITEM_DATA*					GetItemData(ITEMID itemId);
	sITEM_DATA*					GetItemData(ITEMID itemId, BYTE byPlace, BYTE byPos);

	void						AddBankItem(sITEM_DATA & rData);
	void						EraseBankItem(ITEMID itemId);
	void						InsertBankItem(sITEM_DATA* pData);
	sITEM_DATA*					GetBankItem(ITEMID itemId, BYTE byPlace, BYTE byPos);
	sITEM_DATA*					GetBankItem(TBLIDX itemTblidx);

	void						SetRankBattleScoreInfo(sRANKBATTLE_SCORE_INFO& rNewInfo);

	bool						HasMail(MAILID mailid);
	void						SetMailBrief(BYTE byMailCount, BYTE byUnReadManager, BYTE byUnReadNormal);
	void						ScheduleReloadMailsResult(QueryResultVector & results, HOBJECT hHandle, bool bSchedule);
	void						StartMailResult(QueryResultVector & results, HOBJECT hHandle, HOBJECT hObject);
	void						LoadMailResult(QueryResultVector & results, HOBJECT hHandle, HOBJECT hObject);
	void						AddMailData(sMAIL_DATA& rMailData);
	sMAIL_DATA*					GetMailData(MAILID mailId);
	bool						DeleteMail(MAILID mailId);
	void						DecreaseMailUnreadNormal();
	void						DecreaseMailUnreadManager();

	bool						CanInitSkills();

private:

	CAccountCache*							m_pAccountCache;

	HSESSION								m_hServerSession;
	bool									m_bIsQuickTeleportLoaded;
	bool									m_bIsBankLoaded;

	ACCOUNTID								m_accountID;

	sPC_DATA								m_sPcData;
	sDBO_SERVER_CHANGE_INFO					m_serverChangeInfo;
	bool									m_bTutorialFlag;
	sCHAR_WAR_FOG_FLAG						m_sWarFogInfo;
	sMAIL_NEW_BREIF							m_sMailBrief;
	sRANKBATTLE_SCORE_INFO					m_sRankBattleScore;
	BYTE									m_abyTitleIndexFlag[NTL_MAX_CHAR_TITLE_COUNT_IN_FLAG];

	std::map<ITEMID, sITEM_DATA*>			m_mapItems;
	std::map<ITEMID, sITEM_DATA*>			m_mapBank;

	std::map<BYTE, sSKILL_DATA*>			m_mapSkills; //first = byIndex

	std::map<BYTE, sBUFF_DATA*>				m_mapBuffs;

	std::map<TBLIDX, sHTB_SKILL_DATA*>		m_mapHtbSkills;

	std::map<BYTE, sQUESTITEM_DATA*>		m_mapQuestItems; //first = byPos

	sCOMPLETE_QUEST_INFO					m_sQuestCompleteData;

	std::map<QUESTID, sPROGRESS_QUEST_INFO*>	m_mapQuestProgressInfo; //first = quest id

	std::map<TBLIDX, BYTE>					m_mapRecipes; //first = recipeTblidx, second = byRecipeType

	std::map<BYTE, sQUICK_TELEPORT_INFO*>	m_mapQuickTeleport; //first = bySlotNum

	std::map<BYTE, sMASCOT_DATA_EX*>		m_mapMascots; //first = index

	std::map<BYTE, sDBO_ITEM_COOL_TIME>		m_mapItemCoolTime; //first = byItemCoolTimeGroupIndex

	std::map<TBLIDX, sEVENT_REWARD_INFO*>	m_mapEventRewardInfo; //first = eventTblidx

	std::map<BYTE, sQUICK_SLOT_DATA*>		m_mapQuickSlotInfo; //first = bySlot

	std::set<PORTALID>						m_setPortalPoints;

	std::map<MAILID, sMAIL_DATA*>			m_mapMail; 

	DBOTIME									m_tNextSkillReset;

};



class CPlayerCacheManager : public CNtlSingleton<CPlayerCacheManager>
{

public:

	CPlayerCacheManager();
	virtual ~CPlayerCacheManager();


private:

	void						Init();
	void						Destroy();


public:

	CAccountCache*				GetAccount(ACCOUNTID accountID);
	CPlayerCache*				GetCharacter(CHARACTERID charID);

	void						InsertAccount(ACCOUNTID accountID, CAccountCache* pAccountCache);
	void						InsertCharacter(CHARACTERID charID, CPlayerCache* pPlayerCache);

	void						EraseCharacter(CHARACTERID charID);


private:

	std::unordered_map<ACCOUNTID, CAccountCache*>	m_mapAccounts;
	std::unordered_map<CHARACTERID, CPlayerCache*>	m_mapCharacters;

};

#define GetPlayerCache()	CPlayerCacheManager::GetInstance()
#define g_pPlayerCache		GetPlayerCache()


#endif