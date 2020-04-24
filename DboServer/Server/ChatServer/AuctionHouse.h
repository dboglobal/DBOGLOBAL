#ifndef __SERVER_AUCTIONHOUSE_MANAGER__
#define __SERVER_AUCTIONHOUSE_MANAGER__


#include "NtlSingleton.h"
#include "NtlPacketGT.h"
#include "EventableObject.h"

#include <boost/unordered_map.hpp>

class CAutionhouse : public EventableObject, public CNtlSingleton<CAutionhouse>
{

public:

	CAutionhouse();
	virtual ~CAutionhouse();

	static const DWORD				UPDATE_AUCTIONHOUSE_TICK = 60000;

private:

	void							Init();

public:

	void							Update();

public:

	void							AddItem(sTENKAICHIDAISIJYOU_DATA& rData);

	void							DelItem(ITEMID nItem);

	void							LoadAuctionHouseData(CPlayer* pPlayer, sGT_TENKAICHIDAISIJYOU_LIST_REQ* req);

	DWORD							ReturnPrice(ITEMID nItem);

	sTENKAICHIDAISIJYOU_DATA*		GetItem(ITEMID nitem);

private:

	typedef boost::unordered_map<ITEMID, sTENKAICHIDAISIJYOU_DATA*>	TMapAuctionHouse;
	TMapAuctionHouse	m_mapAuctionhouse;

};

#define GetAuctionHouseManager()		CAutionhouse::GetInstance()
#define g_pAH							GetAuctionHouseManager()

#endif