#pragma once

#include "Table.h"

const int QUEST_ITEM_DROP_MAX_COUNT = 5;

#pragma pack(push, 4)
struct sQUEST_DROP_TBLDAT : public sTBLDAT
{
	sQUEST_DROP_TBLDAT()
	{
		memset( aQuestItemTblidx, 0xFF, sizeof(TBLIDX) * QUEST_ITEM_DROP_MAX_COUNT );
		memset( aDropRate, 0xFF, sizeof(float) * QUEST_ITEM_DROP_MAX_COUNT);
	}

public:
	TBLIDX		aQuestItemTblidx[QUEST_ITEM_DROP_MAX_COUNT];
	float		aDropRate[QUEST_ITEM_DROP_MAX_COUNT];

protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CQuestDropTable : public CTable
{
public:

	CQuestDropTable(void);
	virtual ~CQuestDropTable(void);

/*
public:

	BOOL				Check() { return TRUE; }*/

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *			FindData(TBLIDX tblidx); 
	
protected:
	WCHAR** GetSheetListInWChar() { return &(CQuestDropTable::m_pwszSheetList[0]); }
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
