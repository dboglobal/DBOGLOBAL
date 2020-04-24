#ifndef __ITEM_MANAGER_H__
#define __ITEM_MANAGER_H__

#include "NtlSingleton.h"
#include "ItemTable.h"

class CItemManager : public CNtlSingleton<CItemManager>
{

public:

	CItemManager();
	virtual ~CItemManager();


private:

	void						Init();

public:

	ITEMID						IncLastItemID() { return ++m_lastItemID; }

	ITEMID						CreateItem(sITEM_DATA& rItemData);

	ITEMID						CreateItem(sSHOP_BUY_INVEN& rData, CHARACTERID charID);

	ITEMID						CreateBank(CHARACTERID charID, TBLIDX itemTblidx, BYTE byPlace, BYTE byPos, BYTE byRank, BYTE byDurType, DBOTIME nUseStartTime, DBOTIME nUseEndTime);

	ITEMID						CreateGuildItem(sITEM_DATA& rItemData, GUILDID guildId);

	ITEMID						CreateSharedBankItem(sITEM_DATA& rItemData, ACCOUNTID accountId);

private:

	ITEMID						m_lastItemID;

};

#define GetItemManager()		CItemManager::GetInstance()
#define g_pItemManager			GetItemManager()

#endif