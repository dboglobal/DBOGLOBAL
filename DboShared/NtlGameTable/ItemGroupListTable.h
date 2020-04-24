#pragma once
//created 3.12.2014
#include "Table.h"
#include "NtlWorld.h"
#include "NtlCharacter.h"
#include <deque>

const BYTE	DBO_MAX_LENGTH_ITEM_GROUP_LIST_NAME = 40;
const BYTE	DBO_MAX_ITEM_GROUP_LIST = 20;

#pragma pack(push, 4)
struct sITEM_GROUP_LIST_TBLDAT : public sTBLDAT
{

public:

	WCHAR		wszName[DBO_MAX_LENGTH_ITEM_GROUP_LIST_NAME + 1];
	BYTE		byLevel;
	BYTE		byTry_Count;
	TBLIDX		mob_Index;
	DWORD		dwMob_Type;
	DWORD		dwWorld_Rule_Type;
	DWORD		dwInterval;
	DWORD		dwSuperior;
	DWORD		dwExcellent;
	DWORD		dwRare;
	DWORD		dwLegendary;
	DWORD		dwNo_Drop;
	DWORD		dwZenny;

	TBLIDX		aItemBag[DBO_MAX_ITEM_GROUP_LIST];
	DWORD		adwProb[DBO_MAX_ITEM_GROUP_LIST];

	DWORD		dwItemBagCount;
	DWORD		dwTotalProb;

	

protected:
	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CItemGroupListTable : public CTable
{
public:
	CItemGroupListTable(void);
	virtual ~CItemGroupListTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT* FindData(TBLIDX tblidx);

	void AccumulateItemGroupListByWorldRuleType(BYTE byMobLevel, BYTE byWorldRuleType, BYTE byMaxItemLevel, std::deque<sITEM_GROUP_LIST_TBLDAT*> & rdequeItemGroupList);
	void AccumulateItemGroupListByMobType(BYTE byMobLevel, BYTE byMobType, BYTE byMaxItemLevel, std::deque<sITEM_GROUP_LIST_TBLDAT*> &rdequeItemGroupList);
	void AccumulateItemGroupListByMobTblidx(BYTE byMobLevel, TBLIDX mobTblidx, std::deque<sITEM_GROUP_LIST_TBLDAT*> &rdequeItemGroupList);

protected:
	WCHAR** GetSheetListInWChar() { return &(CItemGroupListTable::m_pwszSheetList[0]); }
	void* AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage);
	bool DeallocNewTable(void* pvTable, WCHAR* pwszSheetName);
	bool AddTable(void * pvTable, bool bReload, bool bUpdate);
	bool SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData);

	void AddItemGroupListByWorldRuleType(DWORD dwWorldRuleType, BYTE byMaxLevel, sITEM_GROUP_LIST_TBLDAT* ItemGroupList);
	void AddItemGroupListByMobType(DWORD dwMobType, BYTE byMaxLevel, sITEM_GROUP_LIST_TBLDAT* ItemGroupList);
	void AddItemGroupListByMobTblidx(TBLIDX mobTblidx, sITEM_GROUP_LIST_TBLDAT* ItemGroupList);

public:

	virtual bool				LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate);

	virtual bool				SaveToBinary(CNtlSerializer& serializer);


private:
	static WCHAR* m_pwszSheetList[];


	typedef std::multimap<BYTE, sITEM_GROUP_LIST_TBLDAT *> itemWorldRuleGroup;
	typedef std::multimap<BYTE, sITEM_GROUP_LIST_TBLDAT *> itemMobTypeGroup;
	typedef std::multimap<TBLIDX, sITEM_GROUP_LIST_TBLDAT *> itemMobTblidxGroup;

	itemWorldRuleGroup	m_mmapItemGroupByWorldRuleType[GAMERULE_TYPE_COUNT];
	itemMobTypeGroup	m_mmapItemGroupByMobType[MOB_TYPE_COUNT];
	itemMobTblidxGroup	m_mmapItemGroupByMobTblidx;
};