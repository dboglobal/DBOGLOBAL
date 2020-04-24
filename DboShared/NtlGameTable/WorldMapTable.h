#pragma once

#include "Table.h"
#include "NtlVector.h"

const DWORD		DBO_MAX_LENGTH_WORLD_MAP_NAME = 32;
const DWORD		DBO_WORLD_MAP_TABLE_COUNT_WORLD_WARFOG	  = 8;

#pragma pack(push, 4)
struct sWORLD_MAP_TBLDAT : public sTBLDAT
{
public:

	TBLIDX			World_Tblidx;
	TBLIDX			Zone_Tblidx;
	TBLIDX			Worldmap_Name;
	WCHAR			wszNameText[ DBO_MAX_LENGTH_WORLD_MAP_NAME + 1 ];
	bool			bValidityAble;
	BYTE			byMapType;
	CNtlVector		vStandardLoc;
	float			fWorldmapScale;

	DWORD			dwLinkMapIdx;//new
	DWORD			dwComboBoxType;//new

	WORD			wWarfog[DBO_WORLD_MAP_TABLE_COUNT_WORLD_WARFOG];

	BYTE			byRecomm_Min_Level; //new
	BYTE			byRecomm_Max_Level; //new

	CNtlVector		vUiModify;//new

public:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CWorldMapTable : public CTable
{
public:
	CWorldMapTable(void);
	virtual ~CWorldMapTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT* FindData(TBLIDX tblidx);

protected:
	WCHAR** GetSheetListInWChar() { return &(CWorldMapTable::m_pwszSheetList[0]); }
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