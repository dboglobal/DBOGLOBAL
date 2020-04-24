#pragma once
//created 2.12.2014
#include "Table.h"

#include "NtlDWC.h"

const BYTE	DBO_MAX_LENGTH_IMAGE_NAME = 0x40;


#pragma pack(push, 4)
struct sDWCMISSION_TBLDAT : public sTBLDAT
{

public:

	TBLIDX		tblNameIndex;
	char		szImageName[ DBO_MAX_LENGTH_IMAGE_NAME + 1 ];
	TBLIDX		tblContainScenarioIndex;
	BYTE		byCompleteMinNum;
	BYTE		byCompleteMaxNum;
	BYTE		byDifficulty;


	sDWCMISSION_REWARD		asReward[MAX_DWC_MISSION_COUNT_PER_SCENARIO];

	TBLIDX		clearObjectTextTblidx;
	TBLIDX		clearConditionTextTblidx;
	

protected:
	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CDwcMissionTable : public CTable
{
public:
	CDwcMissionTable(void);
	virtual ~CDwcMissionTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT* FindData(TBLIDX tblidx);

protected:
	WCHAR** GetSheetListInWChar() { return &(CDwcMissionTable::m_pwszSheetList[0]); }
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