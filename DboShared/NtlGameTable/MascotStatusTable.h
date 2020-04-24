#pragma once
//created 3.12.2014
#include "Table.h"

#include "NtlMascot.h"


#pragma pack(push, 4)
struct sMASCOT_STATUS_TBLDAT : public sTBLDAT
{

public:

	DWORD		Name;
	WCHAR		wszNameText[DBO_MAX_LENGTH_MASCOT_NAME_TEXT + 1];
	bool		bValidity_Able;

	char		szModel[NTL_MAX_LENGTH_MODEL_NAME + 1];
	BYTE		byRank; //always 1-4. Min 1 and Max 4
	BYTE		bySlot_Num; //always 1-4. Min 1 and Max 4

	WORD		wVpUpMin; //always 50
	WORD		wVpUpMax; //always 100

	WORD		trash1; //CD CD

	WORD		wSkillGrade1; //always 100,80,60,50
	WORD		wSkillGrade2; //always 0,20,39,45
	WORD		wSkillGrade3; //always 0,0,1,5

	WORD		wVpRegen; //CD CD

	WORD		wSkillGradeMax; //always 1

	TBLIDX		nextMascotTblidx;//next mascot tblidx. But if rank = 4 then next tblidx is -1
	TBLIDX		sealItemIndex;//always 6006001

protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CMascotStatusTable : public CTable
{
public:

	CMascotStatusTable(void);
	virtual ~CMascotStatusTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *			FindData(TBLIDX tblidx); 

protected:
	WCHAR** GetSheetListInWChar() { return &(CMascotStatusTable::m_pwszSheetList[0]); }
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