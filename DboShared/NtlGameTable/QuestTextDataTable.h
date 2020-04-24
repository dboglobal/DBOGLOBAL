#pragma once

#include <vector>
#include "Table.h"

// Quest Text ¸ðÀ½
#pragma pack(push, 4)
struct sQUEST_TEXT_DATA_TBLDAT : public sTBLDAT
{
public:
	std::wstring wstrText;

protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CQuestTextDataTable : public CTable
{
public:

	CQuestTextDataTable(void);
	virtual ~CQuestTextDataTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *			FindData(TBLIDX tblidx); 

	// Function created to use the Tool
	// Speed is slow!
	void				FindDataRange( TBLIDX tblBegin, TBLIDX tblEnd, std::vector<sQUEST_TEXT_DATA_TBLDAT*>& vecRangeList );

	bool				AddTable(void * pvTable, bool bReload, bool bUpdate);
	
protected:
	WCHAR** GetSheetListInWChar() { return &(CQuestTextDataTable::m_pwszSheetList[0]); }
	void* AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage);
	bool DeallocNewTable(void* pvTable, WCHAR* pwszSheetName);
	bool SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData);


public:

	virtual bool				LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate);

	virtual bool				SaveToBinary(CNtlSerializer& serializer);


private:
	static WCHAR* m_pwszSheetList[];
};
