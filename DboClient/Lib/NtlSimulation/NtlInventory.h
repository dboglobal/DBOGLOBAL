/*****************************************************************************
 *
 * File			: NtlInventory.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2006. 4. 24	
 * Abstract		: Simulation Inventory
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_INVENTORY_H__
#define __NTL_INVENTORY_H__

// core
#include "ceventhandler.h"

//shared
#include "NtlItem.h"

//simulation
#include "NtlSLDef.h"
#include "NtlSLEvent.h"

class CNtlSobItem;
class CNtlSobQuestItem;

///////////////////////////////////////////////////////////////////////////////////////////////
// player 사용

class CNtlInventoryBase : public RWS::CEventHandler
{
private:

	RwUInt32 m_uiTblId[NTL_MAX_EQUIP_ITEM_SLOT];	/** inventory item table id data */

public:

	CNtlInventoryBase();
	virtual ~CNtlInventoryBase();

	virtual void HandleEvents(RWS::CMsg &pMsg);

	void SetEquipTableId(RwUInt8 bySlotIdx, RwUInt32 uiTblId);
	RwUInt32 GetEquipTableId(RwUInt8 bySlotIdx);
	RwBool IsValidEquipTable(RwUInt8 bySlotIdx);
};

///////////////////////////////////////////////////////////////////////////////////////////////
// avatar 사용

typedef struct _stCOOLTIMEDATA
{
	RwUInt32	dwFlag;
	RwReal		fCoolTime;
	RwReal		fCurrentTime;		

}stCOOLTIMEDATA;

class CNtlInventory : public CNtlInventoryBase
{
private:

	SERIAL_HANDLE m_hEquipItem[NTL_MAX_EQUIP_ITEM_SLOT];	/** inventory item serial data */
	SERIAL_HANDLE m_hBagItem[NTL_MAX_BAGSLOT_COUNT];		/** inventory item serial data */
	stCOOLTIMEDATA	m_stCoolTimeGroup[ITEM_COOL_TIME_GROUP_COUNT];	// 아이템 사용 그룹별 쿨타임 관리
	
private:

	CNtlSobItem* CreateItem(void *pParentSobItem, RwInt32 iItemPfIdx);
	CNtlSobItem* CreateItem(void *pParentSobItem, SNtlEventSobItemAdd *pSobItemAdd);

	void CreateEventHandler(RWS::CMsg &pMsg);
	void AddEventHandler(RWS::CMsg &pMsg);
	void DeleteEventHandler(RWS::CMsg &pMsg);
	void ItemMoveEventHandler(RWS::CMsg &pMsg);
	void ItemStackMoveEventHandler(RWS::CMsg &msg);
	void ItemUpdateEventHandler(RWS::CMsg &msg);
	void ItemDurDownEventHandler(RWS::CMsg &msg);
	void ItemDurUpdateEventHandler(RWS::CMsg &msg);
	void ItemRestrictStateUpdateEventHandler(RWS::CMsg &msg);
	void ItemSocketInsertBeadEventHandler(RWS::CMsg &msg);
	void ItemSocketDestroyBeadEventHandler(RWS::CMsg &msg);
	void ItemUseActionEventHandler(RWS::CMsg &msg);
	void ItemUpgradeEventHandler(RWS::CMsg &msg);
	void ItemChangeOptionSetEventHandler(RWS::CMsg &msg);
	void ItemChangeBattleAttributeEventHandler(RWS::CMsg &msg);
	void EquippedItemUpgradeAllEventHandler(RWS::CMsg &msg);
	
public:

	CNtlInventory();
	~CNtlInventory();

	RwBool Create(void);
	void Destroy(void);
	void HandleEvents(RWS::CMsg &pMsg);

	void CoolTimeUpdate(RwReal fElapsed);

	// equip item
	void SetEquipItem(RwInt32 iIdx, SERIAL_HANDLE hSerial);

	SERIAL_HANDLE GetEquipItem(RwInt32 iIdx);

	RwInt32 FindEquipSlot(SERIAL_HANDLE hSerial);

	RwBool IsVaildEquipItemIndex(RwInt32 iIdx);

	RwBool IsValidEquipItem(RwInt32 iIdx);

	// bag item
	void SetBagItem(RwInt32 iIdx, SERIAL_HANDLE hSerial);

	SERIAL_HANDLE GetBagItem(RwInt32 iIdx);

	RwInt32 FindBagSlot(SERIAL_HANDLE hSerial);

	RwBool IsValidBagItemIndex(RwInt32 iIdx);

	RwBool IsValidBagItem(RwInt32 iIdx);

	// scouter
	void SetScouterChildItem(RwInt32 iIdx, SERIAL_HANDLE hSerial);

	SERIAL_HANDLE GetScouterItem(VOID);

	// item 전체에 대하여.
	void InvalidItem(SERIAL_HANDLE hItemSerial);

	// cooltime
	void SetItemCooltimeInfo( RwUInt32 uiFlag, RwReal fCoolTime );
	void UnsetItemCooltimeInfo( RwUInt32 uiFlag );	
	stCOOLTIMEDATA* GetFirstItemCooltimeInfo( RwUInt32 uiFlag );
	stCOOLTIMEDATA* GetCooltimeInfo(VOID) { return m_stCoolTimeGroup; }
};

///////////////////////////////////////////////////////////////////////////////////////////////
// avatar quest inventroy 

#define MAX_QUEST_INVENTORY_SLOT		32

class CNtlQuestInventory : public RWS::CEventHandler
{
private:

	SERIAL_HANDLE m_hSlotItem[MAX_QUEST_INVENTORY_SLOT];	

public:

	CNtlQuestInventory();
	~CNtlQuestInventory();

	RwBool Create(void);
	void Destroy(void);
	void HandleEvents(RWS::CMsg &pMsg);

public:

	void CreateEventHandler(RWS::CMsg &pMsg);
	void AddEventHandler(RWS::CMsg &pMsg);
	void DeleteEventHandler(RWS::CMsg &pMsg);
	void UpdateEventHandler(RWS::CMsg &pMsg);
	void MoveEventHandler(RWS::CMsg &pMsg);

	SERIAL_HANDLE	  GetQuestItemSerial(RwUInt8 bySlotIdx);
	CNtlSobQuestItem* GetQuestItemFromIdx(RwUInt8 bySlotIdx);
	CNtlSobQuestItem* GetQuestItemFromTable(RwUInt32 uiQuestItemTblIdx);
};

#endif