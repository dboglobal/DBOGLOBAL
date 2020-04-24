#pragma once
//last update 1.12.2014
#include "Table.h"
#include "NtlVector.h"
#include "NtlCharacter.h"

#pragma pack(push, 4)
struct sNEWBIE_TBLDAT : public sTBLDAT
{
public:

	BYTE			byRace;
	BYTE			byClass;
	WORLDID			world_Id;
	TBLIDX			tutorialWorld;
	CNtlVector      vSpawn_Loc;
	CNtlVector      vSpawn_Dir;
	CNtlVector      vBind_Loc;
	CNtlVector      vBind_Dir;
	TBLIDX			aitem_Tblidx[NTL_MAX_NEWBIE_ITEM];
	BYTE			abyPos[NTL_MAX_NEWBIE_ITEM];
	BYTE			abyStack_Quantity[NTL_MAX_NEWBIE_ITEM];
	TBLIDX			mapNameTblidx;
	TBLIDX			aSkillTblidx[NTL_MAX_NEWBIE_SKILL];
	sNEWBIE_QUICKSLOT_DATA	asQuickData[NTL_MAX_NEWBIE_QUICKSLOT_COUNT];
	PORTALID		defaultPortalId[3];
	TBLIDX			qItemTblidx1;
	BYTE			byQPosition1;
	BYTE			byQStackQuantity1;

	WORD			wMixLevelData; //Always 1

protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CNewbieTable :
	public CTable
{
public:
	CNewbieTable(void);
public:
	virtual ~CNewbieTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *					FindData(TBLIDX tblidx); 

	sTBLDAT *					GetNewbieTbldat(BYTE byRace, BYTE byClass);

	bool						SetNewbieTbldat(BYTE byRace, BYTE byClass, sTBLDAT * pTbldat);

protected:
	WCHAR** GetSheetListInWChar() { return &(CNewbieTable::m_pwszSheetList[0]); }
	void* AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage);
	bool DeallocNewTable(void* pvTable, WCHAR* pwszSheetName);
	bool AddTable(void * pvTable, bool bReload, bool bUpdate);
	bool SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData);

public:

	virtual bool				LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate);

	virtual bool				SaveToBinary(CNtlSerializer& serializer);


protected:

	sTBLDAT *					m_aNewbieTbldat[RACE_COUNT][PC_CLASS_COUNT];

private:
	static WCHAR* m_pwszSheetList[];
};
