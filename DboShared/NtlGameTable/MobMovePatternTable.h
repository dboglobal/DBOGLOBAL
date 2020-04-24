#pragma once

#include "Table.h"


const DWORD		DBO_MAX_COUNT_MOVE_PATTERN = 64;

#pragma pack(push, 4)
struct sMOVE_PATTERN_TBLDAT : public sTBLDAT
{
public:
	BYTE		abyPattern[DBO_MAX_COUNT_MOVE_PATTERN];		

public:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CMobMovePatternTable : public CTable
{
public:
	CMobMovePatternTable(void);
	virtual ~CMobMovePatternTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT* FindData(TBLIDX tblidx);

protected:
	WCHAR** GetSheetListInWChar() { return &(CMobMovePatternTable::m_pwszSheetList[0]); }
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