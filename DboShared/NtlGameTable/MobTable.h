#pragma once
//updated 1.12.2014
#include "Table.h"
#include "NPCTable.h"


// MOB
#pragma pack(push, 4)
struct sMOB_TBLDAT : public sBOT_TBLDAT
{
public:
	DWORD			dwMobGroup;		
	WORD			wMob_Kind;
	DWORD			dwDrop_Zenny;
	float			fDrop_Zenny_Rate;
	DWORD			dwExp; 
	BYTE			byMob_Type;
	TBLIDX			drop_Item_Tblidx;  // this is wrong.. 7x CD

	bool			bSize;
	WORD			wTMQPoint;
	
	TBLIDX			dropQuestTblidx;

	TBLIDX			idxBigBag1;
	BYTE			byDropRate1;
	BYTE			byTryCount1;
	TBLIDX			idxBigBag2;
	BYTE			byDropRate2;
	BYTE			byTryCount2;
	TBLIDX			idxBigBag3;
	BYTE			byDropRate3;
	BYTE			byTryCount3;


	DWORD			dwUnknown;
	BYTE			byUnknown;
	BYTE			byUnknown2;
	bool			bShow_Name; 
	/*
	bool			bShow_Name; 

	DWORD			dwUnknown; 
	BYTE			byProperty; 

	BYTE			byKnockDownBlockRate; 
	BYTE			byHtbBlockRate; 
	*/

	WORD			wSightAngle; 

	DWORD			dwImmunity_Bit_Flag; 

	bool			bIsDragonBallDrop;

	WORD			wMonsterClass; 
	WORD			wUseRace; 

	float			fRewardExpRate;
	float			fRewardZennyRate; 

	DWORD			dwFormulaOffset; 

	float			fSettingRate_LP;
	float			fSettingRate_LPRegen; 
	float			fSettingRate_PhyOffence; 
	float			fSettingRate_EngOffence; 
	float			fSettingRate_PhyDefence;
	float			fSettingRate_EngDefence; 
	float			fSettingRate_AttackRate; 
	float			fSettingRate_DodgeRate; 
	float			fSettingPhyOffenceRate; 
	float			fSettingEngOffenceRate; 
	float			fSettingRate_Defence_Role; 

public:
	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CMobTable : public CTable
{

public:

	typedef std::map<DWORD, TBLIDX> MOB_TABLE;
	typedef MOB_TABLE::iterator MOB_TABLEIT;
	typedef MOB_TABLE::value_type MOB_TABLEVAL;

public:

	CMobTable(void);

public:
	virtual ~CMobTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:

	MOB_TABLEIT	MobBegin() { return m_mapMobTableList.begin(); }
	MOB_TABLEIT	MobEnd() { return m_mapMobTableList.end(); }

	sTBLDAT *			FindData(TBLIDX tblidx); 
	TBLIDX				FindTblidxByGroup(DWORD dwMobGroup); 

protected:

	WCHAR** GetSheetListInWChar() { return &(CMobTable::m_pwszSheetList[0]); }
	void* AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage);
	bool DeallocNewTable(void* pvTable, WCHAR* pwszSheetName);
	bool AddTable(void * pvTable, bool bReload, bool bUpdate);
	bool SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData);

public:

	virtual bool				LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate);

	virtual bool				SaveToBinary(CNtlSerializer& serializer);


private:

	static WCHAR* m_pwszSheetList[];
	MOB_TABLE			m_mapMobTableList;

};
