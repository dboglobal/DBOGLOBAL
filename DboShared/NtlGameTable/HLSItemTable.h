#pragma once
//last update 20.08.2015
#include "Table.h"
#include "NtlItem.h"

#include "NtlHlsItem.h"


#pragma pack(push, 4)
struct sHLS_ITEM_TBLDAT : public sTBLDAT
{

public:

	TBLIDX			itemTblidx;
	BYTE			byCategory;
	bool			bOnSale;
	DWORD			dwCash;
	BYTE			byStackCount;
	WORD			wDisplayBitFlag;


public:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CHLSItemTable : public CTable
{
public:

	CHLSItemTable(void);
	virtual ~CHLSItemTable(void);
	
	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *			FindData(TBLIDX tblidx); 
	
protected:
	WCHAR** GetSheetListInWChar() { return &(CHLSItemTable::m_pwszSheetList[0]); }
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