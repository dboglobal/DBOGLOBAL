#pragma once

#include "Table.h"
#include "NtlVector.h"
#include "NtlItem.h"

#pragma pack(push, 4)
struct sSET_ITEM_TBLDAT : public sTBLDAT
{
	sSET_ITEM_TBLDAT()
	{
		bValidity_Able = true;
	};

public:
	bool			bValidity_Able;
	TBLIDX			semiSetOption;
	TBLIDX			fullSetOption;
	TBLIDX			aItemTblidx[NTL_MAX_SET_ITEM_COUNT];

public:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CSetItemTable : public CTable
{
public:
	CSetItemTable(void);
	virtual ~CSetItemTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT* FindData(TBLIDX tblidx);

protected:
	WCHAR** GetSheetListInWChar() { return &(CSetItemTable::m_pwszSheetList[0]); }
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