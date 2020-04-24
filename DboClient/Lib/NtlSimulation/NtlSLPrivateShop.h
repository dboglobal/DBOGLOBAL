#ifndef __NTL_PRIVATE_SHOP_H__
#define __NTL_PRIVATE_SHOP_H__

// core
#include "ceventhandler.h"
#include "NtlItem.h"

class CNtlPrivateShop : public RWS::CEventHandler
{
public:
	CNtlPrivateShop();
	virtual ~CNtlPrivateShop();

	virtual	RwBool	Create();
	void			Destroy();

	virtual	void	HandleEvents(RWS::CMsg &pMsg);

	void			HandleEvents_PrivateShopState(RWS::CMsg &pMsg);
	void			HandleEvents_PrivateShopStateVisitor(RWS::CMsg &pMsg);
	void			HandleEvents_PrivateShopItem(RWS::CMsg &pMsg);
	void			HandleEvents_PrivateShopItemDataInfo(RWS::CMsg &pMsg);
	void			HandleEvents_PrivateShopItemSelect(RWS::CMsg &pMsg);
	void			HandleEvents_PrivateShopItemBuying(RWS::CMsg &pMsg);
	void			HandleEvents_PrivateShopItemState(RWS::CMsg &pMsg);

	ePRIVATESHOP_STATE		GetPrivateShopState()				{ return m_ePrivateShopState; }
	sPRIVATESHOP_SHOP_DATA*	GetPrivateShopData()				{ return &m_PrivateShopData; }
	sPRIVATESHOP_ITEM_DATA*	GetPrivateShopItemData(RwUInt8 byPos);
	RwUInt8					GetPrivateShopEmptyPos();

	void					SetPrivateShopState(ePRIVATESHOP_STATE ePrivateShopState, sPRIVATESHOP_SHOP_DATA* pPrivateShopData);
	void					SetPrivateShopItemData(RwUInt8 byPos, sPRIVATESHOP_ITEM_DATA* pPrivateShopItemData);

	void					ClearPrivateShopItem(RwBool bCashOnly = FALSE);

protected:
	typedef void (CNtlPrivateShop::*FuncEventHander)(RWS::CMsg &pMsg);
	typedef std::map<const RWS::CRegisteredMsgs*, FuncEventHander> MapFuncEventHandler;
	MapFuncEventHandler m_mapEventFunc;

	sPRIVATESHOP_SHOP_DATA	m_PrivateShopData;
	sPRIVATESHOP_ITEM_DATA	m_aPrivateShopItemData[NTL_MAX_PRIVATESHOP_INVENTORY];
	ePRIVATESHOP_STATE		m_ePrivateShopState;
};

#endif