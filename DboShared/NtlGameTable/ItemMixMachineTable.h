//***********************************************************************************
//
//	File		:	ItemMixMachineTable.h
//
//	Begin		:	2009-04-24
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Chung Doo sup   ( john@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "Table.h"
#include "NtlItem.h"

const DWORD		DBO_MAX_COUNT_BUILT_IN_RECIPE = 3;


#pragma pack(push, 4)


struct sITEM_MIX_MACHINE_TBLDAT : public sTBLDAT
{	
	sITEM_MIX_MACHINE_TBLDAT()
	{
		bValidityAble = true;
	};
public:
	bool						bValidityAble;
	TBLIDX						name;
	BYTE						byMachineType;
	WORD						wFunctionBitFlag;
	BYTE						byMixZennyDiscountRate;
	TBLIDX						dynamicObjectTblidx;
	TBLIDX						aBuiltInRecipeTblidx[DBO_MAX_COUNT_BUILT_IN_RECIPE];

public:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CItemMixMachineTable : public CTable
{
public:

	CItemMixMachineTable(void);
	virtual ~CItemMixMachineTable(void);
	
	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *			FindData(TBLIDX tblidx); 
	
protected:
	WCHAR** GetSheetListInWChar() { return &(CItemMixMachineTable::m_pwszSheetList[0]); }
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