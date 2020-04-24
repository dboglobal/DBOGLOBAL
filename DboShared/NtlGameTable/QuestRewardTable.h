#pragma once
//last update 1.12.2014
#include "Table.h"
#include "NtlQuest.h"


#pragma pack(push, 4)
struct sQUEST_REWARD_TBLDAT : public sTBLDAT
{
	sQUEST_REWARD_TBLDAT()
	{
		memset( arsDefRwd, 0xff, sizeof( arsDefRwd ) );
		memset( arsSelRwd, 0xff, sizeof( arsSelRwd ) );
	}

public:
	DWORD						dwDef_Reward_EXP;
	DWORD						dwDef_Reward_Zeny;
	sQUEST_REWARD_DATASET		arsDefRwd[QUEST_REWARD_DEF_MAX_CNT];
	sQUEST_REWARD_DATASET		arsSelRwd[QUEST_REWARD_SEL_MAX_CNT];

protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)



class CQuestRewardTable : public CTable
{
public:
	CQuestRewardTable(void);
	virtual ~CQuestRewardTable(void);

public:
	bool 						Create(DWORD dwCodePage);
	void 						Destroy();

protected:
	void 						Init();

public:
	sTBLDAT *					FindData(TBLIDX tblidx); 
	

protected:
	WCHAR**						GetSheetListInWChar() { return &(CQuestRewardTable::m_pwszSheetList[0]); }
	void*						AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage);
	bool 						DeallocNewTable(void* pvTable, WCHAR* pwszSheetName);
	bool 						AddTable(void * pvTable, bool bReload, bool bUpdate);
	bool 						SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData);


public:
	virtual bool				LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate);
	virtual bool				SaveToBinary(CNtlSerializer& serializer);


private:
	static WCHAR* m_pwszSheetList[];
};

