#pragma once

#include "Table.h"

const DWORD		DBO_MAX_LENGTH_RESOURCE = 32;
const DWORD		DBO_MAX_LENGTH_NOTE = 32;

#pragma pack(push, 4)
struct sGUIDE_HINT_TBLDAT : public sTBLDAT
{
public:
	BYTE			byType;
	BYTE			byWidthPosition;
	BYTE			byHeightPosition;
	WORD			wWidthSize;
	WORD			wHeightSize;
	char			szResource[ DBO_MAX_LENGTH_RESOURCE + 1];
	char			szNote[ DBO_MAX_LENGTH_NOTE + 1];
	bool			bAutoShow;

protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CGuideHintTable : public CTable
{
public:
	CGuideHintTable(void);
	virtual ~CGuideHintTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT* FindData(TBLIDX tblidx);

protected:
	WCHAR** GetSheetListInWChar() { return &(CGuideHintTable::m_pwszSheetList[0]); }
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