#pragma once
//created 2.12.2014
#include "Table.h"

#include "NtlDWC.h"

#pragma pack(push, 4)
struct sDWC_TBLDAT : public sTBLDAT
{

public:

	TBLIDX		tblNameIndex;
	BYTE		byLevel_Min;
	BYTE		byLevel_Max;
	WORD		wAdmission_Bit_Flag;
	BYTE		byAdmission_Num_Min;
	BYTE		byAdmission_Num_Max;
	TBLIDX		aConditionTblidx[MAX_DWC_ADMISSION_CONDITION_COUNT];
	TBLIDX		aMissionTblidx[MAX_DWC_MISSION_COUNT_PER_SCENARIO];
	TBLIDX		prologueCinematicTblidx;
	TBLIDX		prologueTblidx;
	TBLIDX		worldTblidx;
	
protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CDwcTable : public CTable
{
public:
	CDwcTable(void);
	virtual ~CDwcTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT* FindData(TBLIDX tblidx);

protected:
	WCHAR** GetSheetListInWChar() { return &(CDwcTable::m_pwszSheetList[0]); }
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