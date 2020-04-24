#pragma once
//created 3.12.2014
#include "Table.h"
#include "NtlQuest.h"


#pragma pack(push, 4)

struct sQUEST_REWARD_SELECT_TBLDAT : public sTBLDAT
{

public:

	BYTE		bySelect_Type;

	sQUEST_REWARD_DATASET		aRewardSet[DBO_MAX_COUNT_OF_QUEST_REWARD_SELECT];

	

protected:
	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CQuestRewardSelectTable : public CTable
{
public:
	CQuestRewardSelectTable(void);
	virtual ~CQuestRewardSelectTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT* FindData(TBLIDX tblidx);

protected:
	WCHAR** GetSheetListInWChar() { return &(CQuestRewardSelectTable::m_pwszSheetList[0]); }
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