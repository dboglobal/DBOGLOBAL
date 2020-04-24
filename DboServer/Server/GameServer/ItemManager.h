#ifndef __DBOG_SERVER_ITEM_MANAGER__
#define __DBOG_SERVER_ITEM_MANAGER__

#include "NtlSingleton.h"
#include "NtlSharedType.h"
#include "NtlItem.h"


class CItem;
class CMonster;
class CPlayer;
class CItemDrop;
struct sITEM_TBLDAT;

class CItemManager : public CNtlSingleton<CItemManager>
{

public:

	CItemManager();
	virtual ~CItemManager();

public:

	void					TickProcess(DWORD dwTickDiff, float fMultiple);

	void					AddItem(CItem* item);
	void					RemoveItem(HOBJECT hId);

	void					DestroyItem(CItem* item);
	void					DestroyItemDropOverTime(CItemDrop* item);

	CItem*					CreateFromDB(HOBJECT hHandle, sITEM_DATA& rData, CPlayer* ch); // only used when player login
	bool					CreateItemDrop(CMonster* pkMob, CPlayer* pkKiller, std::vector<CItemDrop*> & vec_item);
	CItemDrop*				CreateSingleDrop(float fRate, TBLIDX itemTblidx);
	void					CreateItemDrop(TBLIDX dropItem_ProbabilityTblidx, std::vector<CItemDrop*> & vec_item);

	CItem*					CreateItem(CPlayer* pPlayer, sITEM_DATA* pItemData, bool bSendPacket = true); //only create the item without database etc
	bool					CreateItem(CPlayer* ch, TBLIDX uiIdx, BYTE byCount, BYTE byDestPlace = 255, BYTE byDestPos = 255, bool bEnchantAble = false);
	bool					CreateItem(CPlayer* ch, CItemDrop* pDrop);
	bool					CreateQuestRewardItem(CPlayer* ch, TBLIDX uiIdx, BYTE byCount);
	bool					CreateShenronRewardItem(CPlayer* ch, TBLIDX uiIdx, BYTE byCount);

	CItem*					Find(HOBJECT hHandle);
	CItemDrop*				FindDrop(HOBJECT hHandle);

	CItem*					SwitchOwner(CPlayer* owner, CPlayer* receiver, HOBJECT hItem);

protected:

	std::map<HOBJECT, CItem*>		m_map_pkItemByID;
	std::set<CItem*>				m_set_pkItemForDelayedSave;

	std::map<HOBJECT, CItemDrop*>		m_map_pkItemDrop;

private:

	std::map<CItemDrop*, DWORD>		m_mapItemDropDelayedDestroy; //DWORD = time in milliseconds
	std::map<CItem*, DWORD>			m_mapItemDelayedDestroy; //DWORD = time in milliseconds

};

#define GetItemManager()		CItemManager::GetInstance()
#define g_pItemManager			GetItemManager()


#endif