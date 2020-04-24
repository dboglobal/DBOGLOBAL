#pragma once
//created 13.10.2017
#include "Table.h"
#include <vector>


#pragma pack(1)
struct sHLS_SLOT_MACHINE_ITEM_TBLDAT : public sTBLDAT
{

public:

	WCHAR		wszNameText[44 + 1];
	bool		bActive;
	TBLIDX		slotMachineTblidx;
	TBLIDX		cashItemTblidx;
	BYTE		byStackCount;
	BYTE		byPercent;



protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack()

class CHlsSlotMachineItemTable : public CTable
{
public:

	CHlsSlotMachineItemTable(void);
	virtual ~CHlsSlotMachineItemTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *			FindData(TBLIDX tblidx);

protected:
	WCHAR** GetSheetListInWChar() { return &(CHlsSlotMachineItemTable::m_pwszSheetList[0]); }
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