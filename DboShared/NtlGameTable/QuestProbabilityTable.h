#pragma once

#include "Table.h"

const int NTL_QUEST_PROBABILITY_MAX_NAME_LENGTH = 64;
const int NTL_QUEST_PROBABILITY_MAX_NOTE_LENGTH = 255;
const int NTL_QUEST_PROBABILITY_MAX_COUNT = 50;


#pragma pack(push, 4)
enum ePROBABILITY_USE_TYPE
{
  PROBABILITY_USE_TYPE_TS = 0,
  PROBABILITY_USE_TYPE_TS_DROP,
  PROBABILITY_USE_TYPE_TMQ_DAYRECORD,
  PROBABILITY_USE_TYPE_GAMBLE_SHOP,
  PROBABILITY_USE_TYPE_GAMBLE_ITEM,
  PROBABILITY_USE_TYPE_SUB_REWARD,

  PROBABILITY_USE_TYPE_COUNT,
  INVALID_PROBABILITY_USE_TYPE = 0xFF,
};
struct sQUEST_PROBABILITY_DATA
{
	BYTE	byType;			// eREWARD_TYPE
	TBLIDX	tblidx;			// 테이블 인덱스 INVALID_
	DWORD	dwMinValue;		// 최소 값
	DWORD	dwMaxValue;		// 최대 값
	DWORD	dwRate;
};
#pragma pack(pop)

#pragma pack(push, 4)
struct sQUEST_PROBABILITY_TBLDAT : public sTBLDAT
{
	sQUEST_PROBABILITY_TBLDAT()
	{
		memset( asProbabilityData, 0xFF, sizeof(sQUEST_PROBABILITY_DATA) * NTL_QUEST_PROBABILITY_MAX_COUNT );
	}

public:
	WCHAR					wszName[NTL_QUEST_PROBABILITY_MAX_NAME_LENGTH + 1];
	WCHAR					wszNote[NTL_QUEST_PROBABILITY_MAX_NOTE_LENGTH + 1];

	ePROBABILITY_USE_TYPE	eUseType; //new
	BYTE					byProbabilityType;	// ePROBABILITY_TYPE

	bool					bAllowBlank; //new
	BYTE					byCount; //new

	sQUEST_PROBABILITY_DATA	asProbabilityData[NTL_QUEST_PROBABILITY_MAX_COUNT];

protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CQuestProbabilityTable :
	public CTable
{
public:
	CQuestProbabilityTable(void);
	virtual ~CQuestProbabilityTable(void);


	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *			FindData(TBLIDX tblidx); 

protected:
	WCHAR** GetSheetListInWChar() { return &(CQuestProbabilityTable::m_pwszSheetList[0]); }
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
