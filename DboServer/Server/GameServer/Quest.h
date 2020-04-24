#pragma once

#include "QuestProgress.h"
#include "ControlScriptEventMap.h"

class CPlayer;
class CTriggerObject;
struct sOBJTRIGGER;
class QueryBuffer;

class CQuest
{

public:

	CQuest();
	virtual ~CQuest();

	void									Create(CPlayer* player);

	void									UpdateQuestEvt(TBLIDX SourceTblidx);

	void									Update(DWORD dwTickDiff);

public:

	BYTE									GetFreeSlotID();
	void									RemoveSlotID(BYTE bySlot);

	CControlScriptEventMap*	GetEventMap()	{ return &m_cEventMap; }

public:

	void									SaveQuestProgress();

////////////////////////////////////////////////////////////////////////////////////

//QUEST IN PROGRESS

public:

	BYTE									CountQuestsInProgress() { return (BYTE)m_pMapQuestInProgress.size(); }

public:

	CQuestProgress*							StartQuest(QUESTID tId, NTL_TS_TC_ID tcId);
	void									EraseQuest(QUESTID qId);

	void									StartQuest(sPROGRESS_QUEST_INFO* questinfo);


	sPROGRESS_QUEST_INFO*					GetQuestProgressInfo(QUESTID tId);

	CQuestProgress*							GetQuestProgress(QUESTID tId);


	WORD									ProgressTsEntity(CNtlTSEntity* pEntity, NTL_TS_T_ID tId, CQuestProgress* pQuestProgress, sOBJTRIGGER* pTrigger = NULL);

	bool									CanStoreQuestInDatabase(QUESTID tId);

	bool									IsDungeonQuestID(QUESTID tId);

	void									PlayerDied();

	void									WpsFailed(DWORD dwWpsIndex);

	void									LeaveWorld(bool bLogout = false, bool bLeaveTMQ = false);

	bool									CheckQuestCounter(QUESTID tId);

	void									UpdateQuestCounter(bool bIncrease, QUESTID tId);


private:

	std::map<QUESTID, CQuestProgress*>		m_pMapQuestInProgress;

	BYTE									m_byLastSlot;
	std::vector<BYTE>						m_vSlotVec;

	std::vector<QUESTID>					m_vErasedDungeonQuestIDs;

	BYTE									m_byGeneralQuestsCount;

	BYTE									m_bySpecialQuestsCount;


//COMPLETE QUEST
private:

	sCOMPLETE_QUEST_INFO					m_stCQInfo;

public:

	void									SetQuestCompleteInfo(const sCOMPLETE_QUEST_INFO& stCQInfo);
	bool									HasBeenClearQuest( QUESTID tId );
	void									SetClearQuest( QUESTID tId );
	void									UnsetClearQuest(QUESTID tId);


//QUEST INVENTORY
private:

	typedef std::set<sQUESTITEM_DATA*>		TQuestSet;

	TQuestSet								setQuestItems;

public:

	BYTE									QuestInventoryCount() { return (BYTE)setQuestItems.size(); }

	void									AddQuestItemData(sQUESTITEM_DATA* data);
	void									AddQuestItemData(TBLIDX itemid, BYTE count, BYTE pos);
	void									RemQuestItemData(BYTE byPos);
	sQUESTITEM_DATA*						GetQuestItemData(BYTE byPos);
	sQUESTITEM_DATA*						GetQuestItemData(TBLIDX ItemTblidx);

	BYTE									GetFreeQuestInvPos();
	BYTE									ReturnItemPos(TBLIDX ItemTblidx);

	//Create,Delete quest item
	bool									CreateQuestItem(TBLIDX ItemTblidx, BYTE byCount);
	void									DeleteQuestItem(BYTE byPos);
	void									DeleteQuestItem(TBLIDX ItemTblidx, int nCount);
	void									MoveQuestItem(BYTE byPos, BYTE byNewPos);


	WORD									UpdateQuestObjectItemEvt(QUESTID qId, TBLIDX ItemTblidx, BYTE byCount);


// RANDOM TRIGGER SYSTEM FUNCTIONS

	bool									CheckQuestItem(eQUEST_ITEM_CHECK_TYPE checktype, int haveCnt, int needCnt);
	void									UnequipAndDeleteQuestItem();


private:

	CPlayer*	m_pkOwner;

	CControlScriptEventMap		m_cEventMap;

};