#pragma once
//created 3.12.2014
#include "Table.h"

#include "NtlMascot.h"


#pragma pack(push, 4)
struct sMASCOT_TBLDAT : public sTBLDAT
{

public:

	DWORD		Name;
	WCHAR		wszNameText[DBO_MAX_LENGTH_MASCOT_NAME_TEXT + 1];
	bool		bValidity_Able;
	BYTE		byModel_Type;
	char		szModel[NTL_MAX_LENGTH_MODEL_NAME + 1];
	BYTE		byRank;
	BYTE		bySlot_Num;
	WORD		wSP_Decrease_Rate;
	WORD		wMax_SP;

	

protected:
	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CMascotTable : public CTable
{
public:
	CMascotTable(void);
	virtual ~CMascotTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT* FindData(TBLIDX tblidx);

protected:
	WCHAR** GetSheetListInWChar() { return &(CMascotTable::m_pwszSheetList[0]); }
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