#pragma once
//created 11.12.2014
#include "Table.h"
#include "NtlItem.h"

#pragma pack(push, 4)
struct sITEM_UPGRADE_RATE_NEW_TBLDAT : public sTBLDAT
{

public:

	BYTE		byItem_Type;
	BYTE		byGrade;

	float		fAdditional_Ability;

	float		fUpgrade_Destroy_Rate;
	float		fUpgrade_Success_Basic_Value;
	float		fUpgrade_Success_Stone_Value;

	float		fUpgrade_RateStone_Value1;
	float		fUpgrade_RateStone_Value2;

protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CItemUpgradeRateNewTable : public CTable
{
public:

	CItemUpgradeRateNewTable(void);
	virtual ~CItemUpgradeRateNewTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *			FindData(TBLIDX tblidx); 

	sTBLDAT *			GetUpgradeRateNewTbldat(BYTE byItem_Type, BYTE byGrade);
	bool				SetUpgradeRateNewTbldat(BYTE byItem_Type, BYTE byGrade, sTBLDAT * pTbldat);

protected:
	WCHAR** GetSheetListInWChar() { return &(CItemUpgradeRateNewTable::m_pwszSheetList[0]); }
	void* AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage);
	bool DeallocNewTable(void* pvTable, WCHAR* pwszSheetName);
	bool AddTable(void * pvTable, bool bReload, bool bUpdate);
	bool SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData);


public:

	virtual bool				LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate);

	virtual bool				SaveToBinary(CNtlSerializer& serializer);

protected:

	sTBLDAT *					m_aUpgradeRateNewTbldat[2][NTL_ITEM_MAX_GRADE];

private:
	static WCHAR* m_pwszSheetList[];
};