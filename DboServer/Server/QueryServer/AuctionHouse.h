#ifndef __SERVER_AUCTIONHOUSE_MANAGER__
#define __SERVER_AUCTIONHOUSE_MANAGER__


#include "NtlSingleton.h"
#include <boost/unordered_map.hpp>
#include "NtlItem.h"


class CAutionhouse : public CNtlSingleton<CAutionhouse>
{

public:

	CAutionhouse();
	virtual ~CAutionhouse();

	typedef boost::unordered_map<ITEMID, sTENKAICHIDAISIJYOU_DATA*>	TMapAuctionHouse;

private:

	void			Init();

	void			Destroy();

public:

	ITEMID						AcquireID() { return ++ullHighestAuctionID; }
			
	void						InsertItem(sTENKAICHIDAISIJYOU_DATA* pData);
	void						EraseItem(ITEMID nItem);
	sTENKAICHIDAISIJYOU_DATA*	GetItem(ITEMID nItem);
	
	TMapAuctionHouse::iterator	GetBegin() { return m_mapAuctionhouse.begin(); }
	TMapAuctionHouse::iterator	GetEnd() { return m_mapAuctionhouse.end(); }
	int							GetCount() { return (int)m_mapAuctionhouse.size(); }

private:

	ITEMID						ullHighestAuctionID;

	TMapAuctionHouse			m_mapAuctionhouse;

};

#define GetAuctionHouse()		CAutionhouse::GetInstance()
#define g_pAH					GetAuctionHouse()

#endif