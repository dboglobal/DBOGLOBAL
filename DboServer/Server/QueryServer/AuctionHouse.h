#ifndef __SERVER_AUCTIONHOUSE_MANAGER__
#define __SERVER_AUCTIONHOUSE_MANAGER__


#include "NtlSingleton.h"
#include <unordered_map>
#include "NtlItem.h"


class CAutionhouse : public CNtlSingleton<CAutionhouse>
{

public:

	CAutionhouse();
	virtual ~CAutionhouse();

private:

	void			Init();

	void			Destroy();

public:

	ITEMID						AcquireID() { return ++ullHighestAuctionID; }
			
	void						InsertItem(sTENKAICHIDAISIJYOU_DATA* pData);
	void						EraseItem(ITEMID nItem);
	sTENKAICHIDAISIJYOU_DATA*	GetItem(ITEMID nItem);
	
	auto						GetBegin() { return m_mapAuctionhouse.begin(); }
	auto						GetEnd() { return m_mapAuctionhouse.end(); }
	int							GetCount() { return (int)m_mapAuctionhouse.size(); }

private:

	ITEMID						ullHighestAuctionID;

	std::unordered_map<ITEMID, sTENKAICHIDAISIJYOU_DATA*>			m_mapAuctionhouse;

};

#define GetAuctionHouse()		CAutionhouse::GetInstance()
#define g_pAH					GetAuctionHouse()

#endif