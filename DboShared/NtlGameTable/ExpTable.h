#pragma once
//last update 30.11.2014
#include "Table.h"


#pragma pack(push, 4)

struct sRANKBATTLE_POINT_TBLDAT
{
	WORD		wStageWin;
	WORD		wStageDraw;
	WORD		wStageLose;
	WORD		wWin;
	WORD		wPerfectWin;
};

struct sEXP_TBLDAT : public sTBLDAT
{

public:
	DWORD		dwExp;
	DWORD		dwNeed_Exp;

	sRANKBATTLE_POINT_TBLDAT sIndividualRankPoint;
	sRANKBATTLE_POINT_TBLDAT sTeamRankPoint;

	WORD		wNormal_Race; // Normal_Race
	WORD		wSuperRace; // Super_Race
	DWORD		dwMobExp; // Mob_Exp
	DWORD		dwPhyDefenceRef; // Phy_Defence_Ref
	DWORD		dwEngDefenceRef; // Eng_Defence_Ref
	DWORD		dwMobZenny; // Mob_Zenny


	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CExpTable : public CTable
{
public:
	CExpTable(void);
	virtual ~CExpTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT* FindData(TBLIDX tblidx);

protected:
	WCHAR** GetSheetListInWChar() { return &(CExpTable::m_pwszSheetList[0]); }
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