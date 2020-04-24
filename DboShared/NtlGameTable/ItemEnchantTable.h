#pragma once
//created 2.12.2014
#include "Table.h"
#include "ItemTable.h"

class CNtlBitFlagManager;

const DWORD		DBO_MAX_LENGTH_ITEM_ENCHANT_NAME = 40;

#pragma pack(push, 4)
struct sITEM_ENCHANT_TBLDAT : public sTBLDAT
{

public:

	WCHAR		wszName[DBO_MAX_LENGTH_ITEM_ENCHANT_NAME + 1];
	TBLIDX		seTblidx;
	bool		bSeType;
	BYTE		byRvType;
	BYTE		byExclIdx;
	BYTE		byMinLevel;
	BYTE		byMaxLevel;
	BYTE		byFrequency;
	WORD		wEnchant_Value;
	BYTE		byKind;
	DWORD		dwEquip;
	BYTE		byGroupNo;
	WORD		wMaxValue;
	bool		bIsSuperior;
	bool		bIsExcellent;
	bool		bIsRare;
	bool		bIsLegendary;

protected:
	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CItemEnchantTable : public CTable
{
public:
	CItemEnchantTable(void);
	virtual ~CItemEnchantTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT*	FindData(TBLIDX tblidx);

	bool		GetGroupNoForRandomOption(BYTE byItemType, BYTE *pbyGroupNo);
	bool		FindOrVerifyRandomEnchantData(sITEM_TBLDAT *pItemTableData, BYTE byItemRank, int nRandomOptionCount, int nRandomOptionIndex, CNtlBitFlagManager *pFlag, float *pfRemainingItemWorth, sITEM_RANDOM_OPTION& sItemRandomOption);
	bool		ProcessRandomOption(sITEM_TBLDAT* itemtbl, BYTE byItemRank, bool bFirst, bool bLast, CNtlBitFlagManager* flagmgr, float* pfRemainingWorth, sITEM_RANDOM_OPTION &randomOption);
	bool		FindOrVerifyRandomSocketEnchantData(sITEM_TBLDAT *pItemTableData, BYTE byItemRank, int nRandomOptionCount, int nIndex, CNtlBitFlagManager *pFlag, float *pfRemainingItemWorth, sITEM_RANDOM_OPTION& sItemRandomOption, DWORD CostumeEquipPostion);
	bool		IsValidOption(BYTE byRank, int nMaxRandomOptionCount, sITEM_TBLDAT *pItemTableData, sITEM_RANDOM_OPTION_SET& rItemRandomOption);

	bool		GetVectorRandomOptionEnchantData(sITEM_TBLDAT *pItemTableData, BYTE byItemRank, std::vector<sITEM_ENCHANT_TBLDAT *> *pvecData);


protected:

	WCHAR** GetSheetListInWChar() { return &(CItemEnchantTable::m_pwszSheetList[0]); }
	void* AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage);
	bool DeallocNewTable(void* pvTable, WCHAR* pwszSheetName);
	bool AddTable(void * pvTable, bool bReload, bool bUpdate);
	bool SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData);

	bool IsProperItemOption(sITEM_ENCHANT_TBLDAT * pItemEnchantTableData, int optionValue, DWORD dwItemCategoryFlag, BYTE byItemLevel, float fRemainingItemWorth, CNtlBitFlagManager* pFlag);

public:

	virtual bool				LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate);

	virtual bool				SaveToBinary(CNtlSerializer& serializer);


private:
	static WCHAR* m_pwszSheetList[];

	typedef std::multimap<BYTE, BYTE> ENCHANTGROUP;
	typedef ENCHANTGROUP::iterator ENCHANTGROUP_IT;
	typedef ENCHANTGROUP::value_type ENCHANTGROUP_VAL;
	ENCHANTGROUP				m_mmapGroupNo;

	std::vector<sITEM_ENCHANT_TBLDAT *> m_vecByRank[ITEM_RANK_COUNT];

};