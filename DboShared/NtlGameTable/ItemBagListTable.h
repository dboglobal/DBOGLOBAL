#pragma once
//created 2.12.2014
#include "Table.h"

const DWORD		DBO_MAX_LENGTH_ITEM_BAG_LIST_NAME = 40;
const BYTE		DBO_MAX_ITEM_BAG_LIST = 20;

#pragma pack(push, 4)
struct sITEM_BAG_LIST_TBLDAT : public sTBLDAT
{

public:

	WCHAR		wszName[DBO_MAX_LENGTH_ITEM_BAG_LIST_NAME + 1];
	BYTE		byLevel;
	bool		bEnchant_Able;

	TBLIDX		aItem[DBO_MAX_ITEM_BAG_LIST];
	DWORD		adwProb[DBO_MAX_ITEM_BAG_LIST];
	DWORD		dwItemCount;
	DWORD		dwTotalProb;


protected:
	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CItemBagListTable : public CTable
{
public:
	CItemBagListTable(void);
	virtual ~CItemBagListTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT* FindData(TBLIDX tblidx);

protected:
	WCHAR** GetSheetListInWChar() { return &(CItemBagListTable::m_pwszSheetList[0]); }
	void* AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage);
	bool DeallocNewTable(void* pvTable, WCHAR* pwszSheetName);
	bool AddTable(void * pvTable, bool bReload, bool bUpdate);
	bool SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData);

public:

	virtual bool				LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate);

	virtual bool				SaveToBinary(CNtlSerializer& serializer);


private:
	static WCHAR* m_pwszSheetList[];
};