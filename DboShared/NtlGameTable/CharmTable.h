#pragma once

#include "Table.h"

#pragma pack(push, 4)
struct sCHARM_TBLDAT : public sTBLDAT
{
public:
	WORD				wDrop_Rate;
	WORD				wEXP;
	WORD				wRP_Sharing;
	WORD				wCool_Time;
	WORD				wKeep_Time;
	DWORD				dwKeep_Time_In_Millisecs;
	DWORD				dwNeed_Zenny;
	BYTE				byDice_Min;
	BYTE				byDice_Max;
	BYTE				byCharm_Type_Bit_Flag;

protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CCharmTable : public CTable
{
public:
	CCharmTable(void);
public:
	virtual ~CCharmTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *			FindData(TBLIDX tblidx); 

protected:
	WCHAR** GetSheetListInWChar() { return &(CCharmTable::m_pwszSheetList[0]); }
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