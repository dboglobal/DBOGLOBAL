#include "stdafx.h"
#include "ItemManager.h"
#include "QueryServer.h"


CItemManager::CItemManager()
{
	Init();
}

CItemManager::~CItemManager()
{

}


void CItemManager::Init()
{
	m_lastItemID = 0;

	smart_ptr<QueryResult> result = GetCharDB.Query("SELECT MAX(id) FROM items");
	if (result)
	{
		Field* f = result->Fetch();

		m_lastItemID = f[0].GetUInt64();
	}

	ERR_LOG(LOG_GENERAL, "Last ITEMID %I64u", m_lastItemID);
}


ITEMID CItemManager::CreateItem(sITEM_DATA& rItemData)
{
	GetCharDB.Execute("INSERT INTO items (id,tblidx,owner_id,place,pos,count,`rank`,durability,grade,NeedToIdentify,BattleAttribute,Maker,OptionTblidx,OptionTblidx2,OptionRandomId,OptionRandomVal,OptionRandomId2,OptionRandomVal2,OptionRandomId3,OptionRandomVal3,OptionRandomId4,OptionRandomVal4,OptionRandomId5,OptionRandomVal5,OptionRandomId6,OptionRandomVal6,OptionRandomId7,OptionRandomVal7,OptionRandomId8,OptionRandomVal8,UseStartTime,UseEndTime,RestrictState,DurationType)"
		"VALUES(%I64u, %u,%u,%u,%u,%u,%u,%u,%u,%u,%u,'%ls',%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u)",
		++m_lastItemID, rItemData.itemNo, rItemData.charId, rItemData.byPlace, rItemData.byPosition, rItemData.byStackcount, rItemData.byRank, rItemData.byCurrentDurability, rItemData.byGrade, rItemData.bNeedToIdentify, rItemData.byBattleAttribute, rItemData.awchMaker,
		rItemData.sOptionSet.aOptionTblidx[0], rItemData.sOptionSet.aOptionTblidx[1],
		rItemData.sOptionSet.aRandomOption[0].wOptionIndex, rItemData.sOptionSet.aRandomOption[0].optionValue,
		rItemData.sOptionSet.aRandomOption[1].wOptionIndex, rItemData.sOptionSet.aRandomOption[1].optionValue,
		rItemData.sOptionSet.aRandomOption[2].wOptionIndex, rItemData.sOptionSet.aRandomOption[2].optionValue,
		rItemData.sOptionSet.aRandomOption[3].wOptionIndex, rItemData.sOptionSet.aRandomOption[3].optionValue,
		rItemData.sOptionSet.aRandomOption[4].wOptionIndex, rItemData.sOptionSet.aRandomOption[4].optionValue,
		rItemData.sOptionSet.aRandomOption[5].wOptionIndex, rItemData.sOptionSet.aRandomOption[5].optionValue,
		rItemData.sOptionSet.aRandomOption[6].wOptionIndex, rItemData.sOptionSet.aRandomOption[6].optionValue,
		rItemData.sOptionSet.aRandomOption[7].wOptionIndex, rItemData.sOptionSet.aRandomOption[7].optionValue,
		rItemData.nUseStartTime, rItemData.nUseEndTime, rItemData.byRestrictState, rItemData.byDurationType);

	return m_lastItemID;
}

ITEMID CItemManager::CreateItem(sSHOP_BUY_INVEN & rData, CHARACTERID charID)
{
	GetCharDB.Execute("INSERT INTO items (id,tblidx,owner_id,place,pos,count,`rank`,durability,grade,Maker,OptionTblidx,OptionTblidx2,OptionRandomId,OptionRandomVal,OptionRandomId2,OptionRandomVal2,OptionRandomId3,OptionRandomVal3,OptionRandomId4,OptionRandomVal4,OptionRandomId5,OptionRandomVal5,OptionRandomId6,OptionRandomVal6,OptionRandomId7,OptionRandomVal7,OptionRandomId8,OptionRandomVal8,UseStartTime,UseEndTime,RestrictState,DurationType)"
		"VALUES(%I64u, %u,%u,%u,%u,%u,%u,%u,%u,'%ls',%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u)",
		++m_lastItemID, rData.tblItem, charID, rData.byPlace, rData.byPos, rData.byStack, rData.byRank, rData.byCurrentDurability, rData.byGrade, rData.awchMaker,
		rData.sOptionSet.aOptionTblidx[0], rData.sOptionSet.aOptionTblidx[1],
		rData.sOptionSet.aRandomOption[0].wOptionIndex, rData.sOptionSet.aRandomOption[0].optionValue,
		rData.sOptionSet.aRandomOption[1].wOptionIndex, rData.sOptionSet.aRandomOption[1].optionValue,
		rData.sOptionSet.aRandomOption[2].wOptionIndex, rData.sOptionSet.aRandomOption[2].optionValue,
		rData.sOptionSet.aRandomOption[3].wOptionIndex, rData.sOptionSet.aRandomOption[3].optionValue,
		rData.sOptionSet.aRandomOption[4].wOptionIndex, rData.sOptionSet.aRandomOption[4].optionValue,
		rData.sOptionSet.aRandomOption[5].wOptionIndex, rData.sOptionSet.aRandomOption[5].optionValue,
		rData.sOptionSet.aRandomOption[6].wOptionIndex, rData.sOptionSet.aRandomOption[6].optionValue,
		rData.sOptionSet.aRandomOption[7].wOptionIndex, rData.sOptionSet.aRandomOption[7].optionValue,
		rData.nUseStartTime, rData.nUseEndTime, rData.byRestrictState, rData.byDurationType);

	return m_lastItemID;
}


ITEMID CItemManager::CreateBank(CHARACTERID charID, TBLIDX itemTblidx, BYTE byPlace, BYTE byPos, BYTE byRank, BYTE byDurType, DBOTIME nUseStartTime, DBOTIME nUseEndTime)
{
	GetCharDB.Execute("INSERT INTO items (id,tblidx,owner_id,place,pos,`rank`,UseStartTime,UseEndTime,DurationType) VALUES (%I64u, %u,%u,%u,%u,%u,%u,%u,%u)", ++m_lastItemID, itemTblidx, charID, byPlace, byPos, byRank, nUseStartTime, nUseEndTime, byDurType);

	return m_lastItemID;
}

ITEMID CItemManager::CreateGuildItem(sITEM_DATA & rItemData, GUILDID guildId)
{
	GetCharDB.Execute("INSERT INTO items (id,tblidx,place,pos,count,`rank`,durability,grade,NeedToIdentify,BattleAttribute,Maker,OptionTblidx,OptionTblidx2,OptionRandomId,OptionRandomVal,OptionRandomId2,OptionRandomVal2,OptionRandomId3,OptionRandomVal3,OptionRandomId4,OptionRandomVal4,OptionRandomId5,OptionRandomVal5,OptionRandomId6,OptionRandomVal6,OptionRandomId7,OptionRandomVal7,OptionRandomId8,OptionRandomVal8,UseStartTime,UseEndTime,RestrictState,DurationType,GuildID)"
		"VALUES(%I64u, %u,%u,%u,%u,%u,%u,%u,%u,%u,'%ls',%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u)",
		++m_lastItemID, rItemData.itemNo, rItemData.byPlace, rItemData.byPosition, rItemData.byStackcount, rItemData.byRank, rItemData.byCurrentDurability, rItemData.byGrade, rItemData.bNeedToIdentify, rItemData.byBattleAttribute, rItemData.awchMaker,
		rItemData.sOptionSet.aOptionTblidx[0], rItemData.sOptionSet.aOptionTblidx[1],
		rItemData.sOptionSet.aRandomOption[0].wOptionIndex, rItemData.sOptionSet.aRandomOption[0].optionValue,
		rItemData.sOptionSet.aRandomOption[1].wOptionIndex, rItemData.sOptionSet.aRandomOption[1].optionValue,
		rItemData.sOptionSet.aRandomOption[2].wOptionIndex, rItemData.sOptionSet.aRandomOption[2].optionValue,
		rItemData.sOptionSet.aRandomOption[3].wOptionIndex, rItemData.sOptionSet.aRandomOption[3].optionValue,
		rItemData.sOptionSet.aRandomOption[4].wOptionIndex, rItemData.sOptionSet.aRandomOption[4].optionValue,
		rItemData.sOptionSet.aRandomOption[5].wOptionIndex, rItemData.sOptionSet.aRandomOption[5].optionValue,
		rItemData.sOptionSet.aRandomOption[6].wOptionIndex, rItemData.sOptionSet.aRandomOption[6].optionValue,
		rItemData.sOptionSet.aRandomOption[7].wOptionIndex, rItemData.sOptionSet.aRandomOption[7].optionValue,
		rItemData.nUseStartTime, rItemData.nUseEndTime, rItemData.byRestrictState, rItemData.byDurationType, guildId);

	return m_lastItemID;
}

ITEMID CItemManager::CreateSharedBankItem(sITEM_DATA & rItemData, ACCOUNTID accountId)
{
	GetCharDB.Execute("INSERT INTO items (id,tblidx,place,pos,count,`rank`,durability,grade,NeedToIdentify,BattleAttribute,Maker,OptionTblidx,OptionTblidx2,OptionRandomId,OptionRandomVal,OptionRandomId2,OptionRandomVal2,OptionRandomId3,OptionRandomVal3,OptionRandomId4,OptionRandomVal4,OptionRandomId5,OptionRandomVal5,OptionRandomId6,OptionRandomVal6,OptionRandomId7,OptionRandomVal7,OptionRandomId8,OptionRandomVal8,UseStartTime,UseEndTime,RestrictState,DurationType,AccountID)"
		"VALUES(%I64u, %u,%u,%u,%u,%u,%u,%u,%u,%u,'%ls',%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u)",
		++m_lastItemID, rItemData.itemNo, rItemData.byPlace, rItemData.byPosition, rItemData.byStackcount, rItemData.byRank, rItemData.byCurrentDurability, rItemData.byGrade, rItemData.bNeedToIdentify, rItemData.byBattleAttribute, rItemData.awchMaker,
		rItemData.sOptionSet.aOptionTblidx[0], rItemData.sOptionSet.aOptionTblidx[1],
		rItemData.sOptionSet.aRandomOption[0].wOptionIndex, rItemData.sOptionSet.aRandomOption[0].optionValue,
		rItemData.sOptionSet.aRandomOption[1].wOptionIndex, rItemData.sOptionSet.aRandomOption[1].optionValue,
		rItemData.sOptionSet.aRandomOption[2].wOptionIndex, rItemData.sOptionSet.aRandomOption[2].optionValue,
		rItemData.sOptionSet.aRandomOption[3].wOptionIndex, rItemData.sOptionSet.aRandomOption[3].optionValue,
		rItemData.sOptionSet.aRandomOption[4].wOptionIndex, rItemData.sOptionSet.aRandomOption[4].optionValue,
		rItemData.sOptionSet.aRandomOption[5].wOptionIndex, rItemData.sOptionSet.aRandomOption[5].optionValue,
		rItemData.sOptionSet.aRandomOption[6].wOptionIndex, rItemData.sOptionSet.aRandomOption[6].optionValue,
		rItemData.sOptionSet.aRandomOption[7].wOptionIndex, rItemData.sOptionSet.aRandomOption[7].optionValue,
		rItemData.nUseStartTime, rItemData.nUseEndTime, rItemData.byRestrictState, rItemData.byDurationType, accountId);

	return m_lastItemID;
}
