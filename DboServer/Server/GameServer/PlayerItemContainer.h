#pragma once

#include "Item.h"

class CPlayer;
struct sAVATAR_ATTRIBUTE;


class CPlayerItemContainer
{

public:

	CPlayerItemContainer();
	~CPlayerItemContainer();

public:

	void						Init(CPlayer* pOwner);


public:

	void						AddItem(CItem* pItem);		// When receive new item or load from Database. Used inside "CItem::AddToCharacter"

	void						MoveItem(CItem* pItem, BYTE byCurPlace, BYTE byNewPlace, BYTE byOldPos, BYTE byNewPos);

	CItem*						GetItem(BYTE byPlace, BYTE byPos);

	void						RemoveItem(BYTE byPlace, HOBJECT hHandle);


//Inventory
public:

	void						InsertItem(HOBJECT hHandle, CItem* item);

	inline void					InsertActiveBag(BYTE byPos, CItem* item) { m_arr_ActiveBags[byPos] = item; }
	inline void					RemoveActiveBag(BYTE byPos) { m_arr_ActiveBags[byPos] = NULL; }

	CItem*						GetActiveBag(BYTE byPos) { return m_arr_ActiveBags[byPos]; }

	CItem*						GetItem(HOBJECT hHandle);
	CItem*						GetItemByIdx(TBLIDX itemTblidx);
	CItem*						GetItem(ITEMID itemId);

	CItem*						CheckStackItem(TBLIDX itemidx, BYTE byCount, BYTE byMaxStack, BYTE byRestrictState);

	void						DeleteAllItems();		// Used only for GM Command

	std::pair<BYTE, BYTE>		GetEmptyInventory();
	BYTE						CountBags();
	BYTE						CountEmptyInventory();

	bool						IsBagEmpty(BYTE byBagPos);

	bool						HasRequiredItem(TBLIDX itemTblidx, BYTE byCount, int nMaxLoopCount = ITEM_BULK_DELETE_COUNT);		//Check if has required items
	bool						RemoveRequiredItem(TBLIDX itemTblidx, BYTE byCount, BYTE& rbyUpdateCount, BYTE& rbyDeleteCount, sITEM_BASIC_DATA* apUpdate, sITEM_DELETE_DATA* apDelete);	//Remove required items(BEFORE RUN THIS FUNCTION MUST RUN FUNCTION "HasRequiredItem"

	TBLIDX						GetMissingDragonball(); //Used for dragonball hunt event. Return id of dragonball which dont have

	bool						IsInventoryReserved(BYTE byPlace, BYTE byPos);
	void						AddReservedInventory(BYTE byPlace, BYTE byPos);
	void						RemoveReservedInventory(BYTE byPlace, BYTE byPos);

	void						SortInventory(BYTE byInventoryType, HOBJECT hNpcHandle); //not wortking yet

	void						ProcessItemWithDuration(DWORD dwTickDiff);
	void						AddItemDuration(HOBJECT hHandle, CItem* pItem);
	bool						HasItemInDuration(HOBJECT hHandle);

//Equipment
public:

	void						SetItemBrief(BYTE byPos, sITEM_DATA* pItemData);
	void						UnsetItemBrief(BYTE byPos);
	void						CopyItemBrief(sITEM_BRIEF* pBrief);

	void						OnDecreaseEquipmentDurability(BYTE* pbyDur, BYTE& rbyApplyCount);

	void						CopyBaseItemAttributesTo(sAVATAR_ATTRIBUTE & avt);
	void						CopyItemAttributesTo(CCharacterAtt* pCharAtt);

	bool						WearGenderRequiredItem(BYTE byNewGender);

//Bank
public:

	void						InsertBankItem(HOBJECT hHandle, CItem* item);

	CItem*						GetBankItem(HOBJECT hHandle);

//Guild Bank
public:

	inline void					InsertGuildBankItem(HOBJECT hHandle, CItem* item) { m_map_GuildBankItems.insert(std::make_pair(hHandle, item)); }

	inline void					SetGuildBankZeni(DWORD dwZeni) { m_dwGuildZeni = dwZeni; }
	inline DWORD				GetGuildBankZeni() { return m_dwGuildZeni; }

	void						FreeGuildBank();

	inline void					SetUsingGuildBank(bool bFlag) { m_bIsUsingGuildBank = bFlag; }
	inline bool					IsUsingGuildBank() { return m_bIsUsingGuildBank; }

private:

	CPlayer*									m_pOwner;

private:

	std::unordered_map<HOBJECT, CItem*>			m_map_CharItems;							//store player items except bank
	CItem*										m_arr_ActiveBags[NTL_MAX_BAGSLOT_COUNT];	//store only bags which are on bag-slot <POS,OBJECT> (POS + 1 = Place inside Bag)

	std::vector<std::pair<BYTE, BYTE>>			m_vecReservedInventory;

	sITEM_BRIEF									m_sItemBrief[EQUIP_SLOT_TYPE_COUNT];

	DWORD										m_dwDurationUpdateTick;
	std::map<HOBJECT, CItem*>					m_mapItemsWithDuration;


	std::unordered_map<HOBJECT, CItem*>			m_map_BankItems;							//store player items from bank

	bool										m_bIsUsingGuildBank;
	DWORD										m_dwGuildZeni;
	std::unordered_map<HOBJECT, CItem*>			m_map_GuildBankItems;						//store player items from Guild bank

};