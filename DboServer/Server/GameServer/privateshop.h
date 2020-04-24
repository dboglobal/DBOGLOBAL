#ifndef __DBOG_PRIVATESHOP__
#define __DBOG_PRIVATESHOP__


class CPlayer;
class CNtlPacket;
class CItem;

#include "NtlItem.h"

class CPrivateShop
{
public:

	CPrivateShop();
	virtual ~CPrivateShop();

	sPACKET_PRIVATESHOP_SHOP_DATA*	GetShopData() { return &shop_data; }
	
public: //shop owner

	void						SetOwner(CPlayer* ch) { m_pkPC = ch; }
	CPlayer*					GetOwner() { return m_pkPC; }

	void						AddItem(BYTE invPlace, BYTE invPos, BYTE shopPos);
	void						DelItem(BYTE bySlot);
	sPRIVATESHOP_ITEM_DATA*		GetItem(BYTE bySlot);

	void						UpdatePrice(BYTE bySlot, DWORD dwNewPrice);

	void						OpenShop(bool bIsOwnerEmpty, BYTE byNoticeSize, WCHAR* wcNotice, WCHAR* wcPrivateShopName);
	void						CloseShop();

	bool						IsShopEditAble();

public:

	void						BroadcastGuests(CNtlPacket* pPacket, CPlayer* pException = NULL);

public: // visitors

	void						EnterShop(CPlayer* visitor);
	void						LeaveShop(CPlayer* visitor, bool bIsLogout = false);
	void						SelectItem(BYTE byPos, BYTE byNewItemState, CPlayer* visitor);
	void						BuyItem(BYTE* byPos, CPlayer* visitor);

private:

	typedef std::set<CPlayer*> GuestSetType;
	GuestSetType				m_set_guest;

	typedef std::map<BYTE, sPRIVATESHOP_ITEM_DATA*>	ItemsMapType;
	ItemsMapType				m_map_items;

	typedef std::map<sPRIVATESHOP_ITEM_DATA*, HOBJECT>	SelectedItemsMapType;
	SelectedItemsMapType		m_map_selectedItems;

	CPlayer*					m_pkPC; //owner

private:
	sPACKET_PRIVATESHOP_SHOP_DATA	shop_data;
};



class CShopManager : public CNtlSingleton<CShopManager>
{
public:
	typedef std::map<HOBJECT, CPrivateShop *> TPrivateShopMap;

public:
	CShopManager();
	virtual ~CShopManager();

public:

	CPrivateShop*	CreatePrivateShop(CPlayer* ch);
	CPrivateShop*	GetPrivateShop(HOBJECT hOwner);
	bool			DestroyPrivateShop(CPlayer* ch);

private:
	TPrivateShopMap	m_map_pkShop;
};
#define GetPrivateShopManager()			CShopManager::GetInstance()
#define g_pPShopManager					GetPrivateShopManager()

#endif