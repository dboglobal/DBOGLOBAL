#pragma once
//created 08.02.2015
#include "Table.h"
#include "NtlItem.h"

#pragma pack(push, 4)
struct sITEM_DISASSEMBLE_TBLDAT : public sTBLDAT
{

public:

	TBLIDX		ItemTblidxResult[ITEM_DISASSEMBLE_MAX_RESULT];

	BYTE		ByMat2Rate;
	BYTE		ByMat3Rate;


protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CItemDisassembleTable : public CTable
{
public:

	CItemDisassembleTable(void);
	virtual ~CItemDisassembleTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *			FindData(TBLIDX tblidx); 

protected:
	WCHAR** GetSheetListInWChar() { return &(CItemDisassembleTable::m_pwszSheetList[0]); }
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