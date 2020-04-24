#pragma once

#include "Table.h"

const DWORD		DBO_MAX_LENGTH_SLANG = 32;

#pragma pack(push, 4)
struct sCHAT_FILTER_TBLDAT : public sTBLDAT
{
public:
	WCHAR			wszSlangText[ DBO_MAX_LENGTH_SLANG + 1];
	TBLIDX			filteringTextIndex;

protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CChattingFilterTable : public CTable
{
public:
	CChattingFilterTable(void);
	virtual ~CChattingFilterTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT* FindData(TBLIDX tblidx);

protected:
	WCHAR** GetSheetListInWChar() { return &(CChattingFilterTable::m_pwszSheetList[0]); }
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