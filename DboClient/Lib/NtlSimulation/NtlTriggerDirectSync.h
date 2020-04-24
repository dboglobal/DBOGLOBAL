/*****************************************************************************
 *
 * File			: NtlTriggerDirectSync.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 8. 23	
 * Abstract		: Simulation trigger direct sync manager class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_TRIGGER_DIRECT_SYNC_H__
#define __NTL_TRIGGER_DIRECT_SYNC_H__

#include "ceventhandler.h"
#include "NtlSLDef.h"
#include "DboTSCore.h"

typedef RwUInt32	TC_HANDLE;				// trigger community handle
#define INVALID_TC_HANDLE	0xffffffff

enum ETriggerDirectType
{
	ETD_None,
	ETD_QuestProposal,
	ETD_QuestUserSelect,
	ETD_QuestReward,
	ETD_QuestNarration,
	ETD_QuestNarrationUserSelect,
	ETD_Camera
};

class CNtlTCUnit
{
	DECLEAR_MEMORY_POOL(CNtlTCUnit, NTL_SMALL_MEMORY_POOL);

private:

	TC_HANDLE m_hHandle;

public:

	CNtlTCUnit();
	~CNtlTCUnit();

	void SetHandle(const TC_HANDLE hHandle);
	TC_HANDLE GetHandle(void) const;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
// tirgger community unit register

class CNtlTCUnitReg
{
	DECLEAR_MEMORY_POOL(CNtlTCUnitReg, NTL_SMALL_MEMORY_POOL);

private:

#define MAX_TC_UNIT_DATA	512

	RwInt32 m_iRef;
	struct sTS_KEY m_sKey;
	ETriggerDirectType m_eTDType;
	RwChar m_chData[MAX_TC_UNIT_DATA];

public:

	CNtlTCUnitReg();
	~CNtlTCUnitReg();

	void SetTSKey(const sTS_KEY& key);
	const sTS_KEY& GetTSKey(void) const;

	void SetTriggerDirectType(ETriggerDirectType eTDType);
	ETriggerDirectType GetTriggerDirectType(void) const;

	void SetData(const void *pData, RwInt32 iSize);
	const RwChar* GetData(void);

	void IncRef(void);
	void DecRef(void);
	RwBool IsValid(void);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
// tirgger community manager

class CNtlTriggerSyncManager : public RWS::CEventHandler
{
private:

	static TC_HANDLE m_uiHandle;
	static CNtlTriggerSyncManager *m_pInstance;

	typedef std::map<TC_HANDLE, CNtlTCUnitReg*>  MapUnitRegister;

	MapUnitRegister m_mapUnitReg;
	
private:

	RwUInt32 AcquireHandle(void);

	CNtlTCUnit* CreateUnit(void);
	CNtlTCUnit* CreateUnit(TC_HANDLE hHandle);
	void ReleaseUnit(CNtlTCUnit* pUnit, void *pData);

	CNtlTCUnitReg* CreateUnitReg(const sTS_KEY& key, ETriggerDirectType eTDType, const void *pData, RwInt32 iSize);
	void ReleaseUnitReg(CNtlTCUnitReg *pUnitReg); 

	void RegisterUnit(CNtlTCUnit *pTCUnit, CNtlTCUnitReg *pTCUnitReg);

public:

	CNtlTriggerSyncManager();
	~CNtlTriggerSyncManager();

	static CNtlTriggerSyncManager* GetInstace(void);

	RwBool Create(void);
	void Destroy(void);
	void Reset(void);
	void HandleEvents(RWS::CMsg &pMsg);

/**
* event send
*/

public:

	void ResponseQuestTriggerSystem(CNtlTCUnitReg *pTCUnitReg, void *pData);

	void ResponseQuestProposalDialog(const sTS_KEY& key, void *pData);
	void ResponseQuestUserSelectDialog(const sTS_KEY& key, void *pData);
	void ResponseQuestRewardDialog(const sTS_KEY& key, void *pData);
	void ResponseQuestNarrationDialog(const sTS_KEY& key, void *pData);
	void ResponseQuestNarrationUserSelectDialog(const sTS_KEY& key, void *pData);
	void ResponseCameraDirect(const sTS_KEY& key, void *pData);
	
/**
* event handler
*/

public:

	void QuestProposalDialogEventHandler(RWS::CMsg &pMsg);
	void QuestUserSelectDialogEventHandler(RWS::CMsg &pMsg);
	void QuestRewardDialogEventHandler(RWS::CMsg &pMsg);
	void QuestNarrationDialogEventHandler(RWS::CMsg &pMsg);
	void QuestNarrationUserSelectDialogEventHandler(RWS::CMsg &pMsg);

	void QuestDirectEchoEventHandler(RWS::CMsg &pMsg);
};

static CNtlTriggerSyncManager* GetNtlTriggerSyncManager(void)
{
	return CNtlTriggerSyncManager::GetInstace();
}

#endif
