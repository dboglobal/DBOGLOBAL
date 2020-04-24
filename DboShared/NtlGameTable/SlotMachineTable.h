#pragma once
//created 24.01.2014
#include "Table.h"
#include "NtlHlsMachine.h"


#pragma pack(push, 4)
struct sHLS_SLOT_MACHINE_TBLDAT : public sTBLDAT
{

public:

	DWORD		dwName;
	WCHAR		wszNameText[44 + 1];
	char		szFile_Name[41 + 1];
	WORD		byCoin;
	bool		bOnOff;
	BYTE		byType;

	WORD		wfirstWinCoin;
	TBLIDX		aItemTblidx[DBO_MAX_HLS_SLOT_MACHINES_MAX_ITEMS];
	BYTE		byStack[DBO_MAX_HLS_SLOT_MACHINES_MAX_ITEMS];
	WORD		wQuantity[DBO_MAX_HLS_SLOT_MACHINES_MAX_ITEMS];



protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CSlotMachineTable : public CTable
{
public:

	CSlotMachineTable(void);
	virtual ~CSlotMachineTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *			FindData(TBLIDX tblidx); 

protected:
	WCHAR** GetSheetListInWChar() { return &(CSlotMachineTable::m_pwszSheetList[0]); }
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