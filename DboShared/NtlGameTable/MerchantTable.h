#pragma once
//last update 1.12.2014
#include "Table.h"
#include "NtlItem.h"

const DWORD		DBO_MAX_LENGTH_MERCHANT_NAME_TEXT = 16;

#pragma pack(push, 4)
struct sMERCHANT_TBLDAT : public sTBLDAT
{
public:
	WCHAR			wszNameText[DBO_MAX_LENGTH_MERCHANT_NAME_TEXT + 1];
	BYTE			bySell_Type;
	TBLIDX			Tab_Name;
	DWORD			dwNeedMileage;
	TBLIDX			aitem_Tblidx[NTL_MAX_MERCHANT_COUNT];

	TBLIDX			aNeedItemTblidx[NTL_MAX_MERCHANT_COUNT];//new
	BYTE			abyNeedItemStack[NTL_MAX_MERCHANT_COUNT];//new
	DWORD			adwNeedZenny[NTL_MAX_MERCHANT_COUNT];//new

public:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CMerchantTable : public CTable
{
public:
	CMerchantTable(void);
	virtual ~CMerchantTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT* FindData(TBLIDX tblidx);
	static TBLIDX FindMerchantItem(sMERCHANT_TBLDAT* psTbldat, BYTE byIndex);

protected:
	WCHAR** GetSheetListInWChar() { return &(CMerchantTable::m_pwszSheetList[0]); }
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