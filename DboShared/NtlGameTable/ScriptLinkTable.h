#pragma once

#include "Table.h"

const DWORD		DBO_MAX_LENGTH_FILE = 64;

#pragma pack(push, 4)
struct sSCRIPT_LINK_TBLDAT : public sTBLDAT
{
public:

	WCHAR			wszValue[ DBO_MAX_LENGTH_FILE + 1];
	BYTE			byType;
	BYTE			byAction;

protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CScriptLinkTable : public CTable
{
public:
	CScriptLinkTable(void);
	virtual ~CScriptLinkTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT* FindData(TBLIDX tblidx);

protected:
	WCHAR** GetSheetListInWChar() { return &(CScriptLinkTable::m_pwszSheetList[0]); }
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