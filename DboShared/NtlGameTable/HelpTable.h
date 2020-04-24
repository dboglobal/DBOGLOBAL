#pragma once

#include "Table.h"

const DWORD		DBO_MAX_LENGTH_HELP_TITLE_TEXT = 32;
const DWORD		DBO_MAX_LENGTH_HELP_HINT_TEXT = 255;
const DWORD		DBO_MAX_LENGTH_HELP_HTML_NAME = 32;

#pragma pack(push, 4)
struct sHELP_TBLDAT : public sTBLDAT
{
public:
	BYTE		byCategory;		
	DWORD		dwHelpTitle;
	DWORD		dwPopoHint;
	WCHAR		wszHelpHTMLName[DBO_MAX_LENGTH_HELP_HTML_NAME + 1];
	BYTE		byConditionCheck;

protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CHelpTable : public CTable
{
public:
	CHelpTable(void);
	virtual ~CHelpTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT* FindData(TBLIDX tblidx);

protected:
	WCHAR** GetSheetListInWChar() { return &(CHelpTable::m_pwszSheetList[0]); }
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