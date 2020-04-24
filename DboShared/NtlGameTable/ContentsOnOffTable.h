#pragma once
//created 08.02.2015
#include "Table.h"


#pragma pack(push, 4)


struct sCONTENTSONOFF_DATA
{
  bool bWestCityPortal;
  bool bZone2130;
  bool bZone3150;
  bool bZone5155;
  bool bZone5660;
  bool bZone6170;
  unsigned int tblUDTrigger[10];
  unsigned int tblTMQNpc[7];
  bool bTMPUse;
  bool bFreeBattleUse;
  bool bRankBattleUse;
  bool bDojoUse;
  bool bMascotUse;
  bool bHoipoiMixUse;
  unsigned int tblDungeon001World;
  unsigned int tblDungeon001Trigger;
  unsigned int tblDungeon002Npc;
  unsigned int tblDungeon003Trigger;
  unsigned int tblDungeon004World;
  unsigned int tblDungeon004Trigger;
  unsigned int tblDungeon006Trigger;
  unsigned int tblDungeon007Trigger;
  unsigned int tblWZone1Npc;
  unsigned int tblBDungeon001Trigger;
	unsigned int tblMainWorld02Npc;
  bool bDWCUse;
  bool bDBScrambleUse;
  DWORD dwCCBDUse;
  DWORD dwQuestTypeBitFlag;
  bool bContentsConditionUse;
  bool bCharDataHistoryUse;
  DWORD dwUDHelp[10];
  DWORD dwTMQHelp[10];
  DWORD dwTMPHelp[10];
  DWORD dwFreeBattleHelp[10];
  DWORD dwRankBattleHelp[10];
  DWORD dwDojoHelp[10];
  DWORD dwMascotHelp[10];
  DWORD dwDWCHelp[10];
  DWORD dwDBScrambleHelp[10];
  DWORD dwAchievementHelp[10];
  DWORD dwCCBDHelp[10];
  bool bDFCUse;
  unsigned int tblUDNPC[10];
  unsigned int tblBDungeon001NPC;

  unsigned int tblSkdNPC;
  unsigned int tblSkd1Trigger;
  unsigned int tblBDungeon003NPC;
  unsigned int tblBDungeon003Trigger;
  unsigned int tblBDungeon002NPC;
  unsigned int tblBDungeon002Trigger;
  bool bUseItemUpgrade;
  unsigned int tblBDungeon004NPC;
  unsigned int tblBDungeon004Trigger;

  unsigned int tblUseScouter;
  bool bUseItemDisassemble;

};


struct sCONTENTSONOFF_TBLDAT : public sTBLDAT
{

public:

	WCHAR		wszNameText[32 + 1];
	DWORD		adwDataValue[10];


protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CContentsOnOffTable : public CTable
{
public:

	CContentsOnOffTable(void);
	virtual ~CContentsOnOffTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *			FindData(TBLIDX tblidx); 

protected:
	WCHAR** GetSheetListInWChar() { return &(CContentsOnOffTable::m_pwszSheetList[0]); }
	void* AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage);
	bool DeallocNewTable(void* pvTable, WCHAR* pwszSheetName);
	bool AddTable(void * pvTable, bool bReload, bool bUpdate);
	bool SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData);


public:

	virtual bool				LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate);

	virtual bool				SaveToBinary(CNtlSerializer& serializer);



private:
	static WCHAR* m_pwszSheetList[];

	sCONTENTSONOFF_DATA				m_sContentsOnOffData;
};