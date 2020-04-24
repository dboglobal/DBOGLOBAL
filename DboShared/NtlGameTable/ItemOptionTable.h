#pragma once

#include "Table.h"
#include "NtlItem.h"

const DWORD		DBO_MAX_LENGTH_ITEM_OPTION_NAME = 30;
const DWORD		DBO_MAX_LENGTH_ITEM_OPTION_NOTE = 128;
const DWORD		INVALID_SYSTEMEFFECTVALUE_DB	= 3;

#pragma pack(push, 4)
struct sITEM_OPTION_TBLDAT : public sTBLDAT
{

public:
	WCHAR			wszOption_Name[ DBO_MAX_LENGTH_ITEM_OPTION_NAME + 1 ];
	bool			bValidity_Able;
	BYTE			byOption_Rank;
	BYTE			byItem_Group;
	BYTE			byMaxQuality;
	BYTE			byQuality;
	BYTE			byQualityIndex;
	DWORD			dwCost;
	BYTE			byLevel;
	TBLIDX			system_Effect[NTL_MAX_SYSTEM_EFFECT_COUNT];
	bool			bAppliedInPercent[NTL_MAX_SYSTEM_EFFECT_COUNT];
	int				nValue[NTL_MAX_SYSTEM_EFFECT_COUNT];

	TBLIDX			activeEffect;
	float			fActiveRate;
	char			szNote[ DBO_MAX_LENGTH_ITEM_OPTION_NOTE + 1 ];
	BYTE			byScouterInfo[NTL_MAX_SYSTEM_EFFECT_COUNT];   // cf) Defined by eSCOUTER_INFO of NtlItem.h 

public:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CItemOptionTable : public CTable
{
public:

	CItemOptionTable(void);
	virtual ~CItemOptionTable(void);
	
	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *					FindData(TBLIDX tblidx); 

protected:
	WCHAR** GetSheetListInWChar() { return &(CItemOptionTable::m_pwszSheetList[0]); }
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