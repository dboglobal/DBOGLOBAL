#ifndef __INC_DBOG_GAME_ITEM_H__
#define __INC_DBOG_GAME_ITEM_H__

#include "ItemTable.h"

struct sUSE_ITEM_TBLDAT;
class CPlayer;

class CItem
{

public:

	CItem();
	virtual ~CItem();

	inline void					SetID(HOBJECT id)	{ hHandle = id; }
	inline HOBJECT				GetID()				{ return hHandle; }

	inline void					SetItemID(ITEMID id)	{ item_data.itemId = id; }
	inline ITEMID				GetItemID()			{ return item_data.itemId; }

	void						SetTbldat(sITEM_TBLDAT* table);
	sITEM_TBLDAT*				GetTbldat()	{ return m_pProto; }

	inline TBLIDX				GetTblidx() const { return m_pProto->tblidx; }

	inline void					SetOwner(CPlayer* ch) { m_pOwner = ch; }
	inline CPlayer*				GetOwner()		{ return m_pOwner; }

	bool						TradeItem(CPlayer* newowner);

	bool						SetCount(BYTE byCount, bool bIsNew = false, bool bQuery = false, bool bSendPacket = true); //bIsNew = true when increase stack
	inline BYTE					GetCount() { return item_data.byStackcount; }
	inline void					SetStackCount(BYTE byCount) { item_data.byStackcount = byCount; }

	CItem*						RemoveFromCharacter();
	bool						AddToCharacter(CPlayer* pPlayer);

	inline BYTE					GetRank() { return item_data.byRank; }
	inline void					SetRank(BYTE rank) { item_data.byRank = rank; }

	inline void					SetGrade(BYTE grade) { item_data.byGrade = grade; }
	inline BYTE					GetGrade() { return item_data.byGrade; }

	void						UpdateDurability(BYTE byDur);
	void						SetDurability(BYTE byDur) { item_data.byCurrentDurability = byDur; }
	inline BYTE					GetDurability() { return item_data.byCurrentDurability; }

	inline BYTE					GetBattleAttribute() { return item_data.byBattleAttribute; }
	inline void					SetBattleAttribute(BYTE byAttr) { item_data.byBattleAttribute = byAttr; }

	inline BYTE					GetRestrictType() { return m_pProto->byRestrictType; }

	inline BYTE					GetRestrictState() { return item_data.byRestrictState; }
	void						SetRestrictState(BYTE byState) { item_data.byRestrictState = byState; }

	inline void					IdentifiyItem(bool b) { item_data.bNeedToIdentify = b; }
	inline bool					NeedToIdentify() { return item_data.bNeedToIdentify; }

	void						SetUseEndTime(DBOTIME nTime) { item_data.nUseEndTime = nTime; }
	inline DBOTIME				GetUseEndTime() { return item_data.nUseEndTime; }

	void						SetUseStartTime(DBOTIME nTime) { item_data.nUseStartTime = nTime; }
	inline DBOTIME				GetUseStartTime() { return item_data.nUseStartTime; }

	bool						IsExpired();

	inline WCHAR*				GetMaker() { return item_data.awchMaker; }

	inline BYTE					GetPlace() { return item_data.byPlace; }
	inline BYTE					GetPos() { return item_data.byPosition; }

	inline bool					IsBag() const { return m_pProto->byItem_Type == ITEM_TYPE_BAG; }
	inline BYTE					GetBagSize() const { return m_pProto->byBag_Size; }

	inline void					SetPlace(BYTE byPlace) { item_data.byPlace = byPlace; }
	inline void					SetPos(BYTE byPos) { item_data.byPosition = byPos; }

	inline void					SetEquipped(bool bFlag)	{ m_bEquipped = bFlag; }
	inline bool					IsEquipped() const	{ return m_bEquipped; }

	inline void					SetTrading(bool bFlag)	{ m_bTrading = bFlag; }
	inline bool					IsTrading() const	{ return m_bTrading; }

	inline void					SetLocked(bool bFlag)	{ m_bIsLocked = bFlag; }
	bool						IsLocked(bool bCheckEquipped = true);

	void						SetItemData(sSHOP_BUY_INVEN* pData, ITEMID itemId);
	inline void					SetItemData(sITEM_DATA* itemdata) { memcpy(&item_data, itemdata, sizeof(sITEM_DATA)); }
	inline sITEM_DATA&			GetItemData() { return item_data; }

	void						GenerateOptionSet(bool bEnchantAble = false);
	static void					GenerateOptionSet(bool bEnchantAble, sITEM_TBLDAT* pTbldat, sITEM_DATA* pItemData);
	inline sITEM_OPTION_SET&	GetOptionSet() { return item_data.sOptionSet; }
	inline bool					CanHaveOption() { return m_pProto->bIsCanHaveOption; }
	inline TBLIDX				GetOptionTblidx() { return m_pProto->Item_Option_Tblidx; }
	void						SetOptionSet(sITEM_OPTION_SET& rSet);

	static bool					ChangeOption(WORD wOptionIndex, sITEM_TBLDAT* pTbldat, BYTE byRank, sITEM_OPTION_SET* sOptionSet);

	inline BYTE					GetEquipType() { return m_pProto->byEquip_Type; }

	inline void					SetDurationtype(BYTE byType) { item_data.byDurationType = byType; }
	inline BYTE					GetDurationtype() { return item_data.byDurationType; }

	bool						CanTrade();
	bool						CanSell();
	bool						CanAuctionhouse();
	bool						CanMail();
	bool						CanPrivateshop();
	bool						CanDelete();
	bool						CanWarehouse(bool bIsSharedBank);
	bool						CanDojowarehouse();
	bool						CanGuildWarehouse();

public:

	void						UseItem(BYTE byKeyPlace, BYTE byKeyPos, BYTE byApplyTargetCount, HOBJECT* ahApplyTarget, sUSE_ITEM_TBLDAT* pUseItemTbldat, HOBJECT hTarget);
	void						UseItemCheckTarget(HOBJECT hAppointTargetHandle, BYTE byApplyTargetCount, HOBJECT* ahApplyTarget);
	void						Cast(); //called after finish casting

	void						CancelCasting();

	WORD						InsertSocketBead(CItem* pBeadItem);
	void						DestroySocketBead(bool bTimeOut);
	void						SetSocketBead(BYTE byRestrictState, BYTE byDurationType, sITEM_RANDOM_OPTION* pRandomOption, DBOTIME nUseStartTime, DBOTIME nUseEndTime);

	bool						HasScouterEffect(int nEffectCode);

private:

	HOBJECT						hHandle;

	sITEM_DATA					item_data;		// Item data
	sITEM_TBLDAT* 				m_pProto;		// ITEM TBLDAT
	CPlayer*					m_pOwner;		// Item Owner

	bool						m_bEquipped;
	bool						m_bTrading;
	bool						m_bIsLocked;	// true = cant move, update, del..

	//item usage
	BYTE						m_byKeyPlace;
	BYTE						m_byKeyPos;
	BYTE						m_byApplyTargetCount;
	HOBJECT						m_ahApplyTarget[32];
	sUSE_ITEM_TBLDAT *			m_pUseItemTbldat;

};

#endif
