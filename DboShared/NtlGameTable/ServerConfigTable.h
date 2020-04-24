#pragma once
//created 5.5.2015
#include "Table.h"
#include "NtlVector.h"


#define SERVERCONFIG_MAX_TBLDAT_VALUE_COUNT	10
const DWORD ENTER_BOSS_STATE_LOC_COUNT = 30;
const DWORD ITEM_UPGRADE_SPECIAL_SOURCE_COUNT = 4;
const DWORD MASCOT_LUMINOSITY_DROP_COUNT = 5;

#pragma pack(push, 4)

struct sENTERLOCATION_BOSS_STAGE
{
	sVECTOR3	sLoc;
	sVECTOR3	sDir;
};


struct sSERVERCONFIG_BATTLEDUNGEON
{
	TBLIDX	battleDungeonWorld;
	DWORD	directPlay_EnterWorld;
	DWORD	directPlay_StageChange;
	float	fDirectPlay_StageChange_PlayTime;
	TBLIDX	stageTriggerObjectTblidx;
	TBLIDX	mailSenderTblidx;
	TBLIDX	mailTextTblidx;
	sENTERLOCATION_BOSS_STAGE	sEnterLoc_NormalStage;
	sENTERLOCATION_BOSS_STAGE	aEnterLoc_BossStage[ENTER_BOSS_STATE_LOC_COUNT];
	bool	bLpRegen;
	bool	bEpRegen;
	bool	bItemDrop;
	bool	bItemUse;
	bool	bExpAdd;
};


struct sSERVERCONFIG_DWC
{
	DWORD	dwEnterProposalWaitTime;
	TBLIDX	enterCharDirectionTblidx;
	TBLIDX	enterFailDirectionTblidx;
	DWORD	dwEnterDirectionPlayTime;
	TBLIDX	mailSenderTblidx;
	TBLIDX	mailTextTblidx;
	TBLIDX	failFlashTblidx;
	DWORD	dwFailStateWaitTime;
};

struct sSERVERCONFIG_DATA
{
	BYTE	byDragonballScrambleCoolTime;
	BYTE	byDailyLimitInitialTime;
	BYTE	byDailyLimitTMQ;
	BYTE	byDailyLimitDWC;
	BYTE	byDailyLimitSKDungeon;
	BYTE	byItemUpgradeSpecialSource[ITEM_UPGRADE_SPECIAL_SOURCE_COUNT];
	sSERVERCONFIG_BATTLEDUNGEON	sBattleDungeonData;
	sSERVERCONFIG_DWC	sDWCData;
};

struct sMASCOT_LUMINOSITY_DROP_DATA
{
	BYTE	byCount;
	DWORD	dwRate;
};

struct sSERVERCONFIG_TBLDAT : public sTBLDAT
{

public:

	std::wstring				wstrName;
	std::wstring				wstrValue[SERVERCONFIG_MAX_TBLDAT_VALUE_COUNT];

protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CServerConfigTable : public CTable
{
public:

	CServerConfigTable(void);
	virtual ~CServerConfigTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();


public:
	sTBLDAT*						FindData(TBLIDX tblidx); 
	sSERVERCONFIG_DATA*				GetServerConfigData() { return &m_sServerConfigData; }
	sMASCOT_LUMINOSITY_DROP_DATA*	GetMascotLumDrop(BYTE b) { if (b >= MASCOT_LUMINOSITY_DROP_COUNT) return NULL; else return &m_aMascotLuminosityDropData[b]; }

protected:

	void			Init( void );

	WCHAR**			GetSheetListInWChar() { return &(CServerConfigTable::m_pwszSheetList[0]); }
	void*			AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage);
	bool			DeallocNewTable(void* pvTable, WCHAR* pwszSheetName);
	bool			AddTable(void * pvTable, bool bReload, bool bUpdate);
	bool			SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData);

	bool			ReadByte( BYTE & rDest, std::wstring & wstrValue, BYTE byMaxValue);

	bool			ReadDWORD( DWORD & rDest, std::wstring & wstrValue, DWORD dwMaxValue);

	bool			ReadTBLIDX( TBLIDX & rDest, std::wstring & wstrValue, TBLIDX dwMaxTblidx);

	bool			ReadFLOAT(float & rDest, std::wstring & wstrValue, float fMaxValue);

	bool			ReadSTR( WCHAR * pDest, DWORD dwDestLength, std::wstring & wstrSrc);

	bool			ReadBOOL( bool & rDest, std::wstring & wstrValue);

	bool			GetBinaryText(std::wstring & wstrValue, CNtlSerializer& serializer);


public:

	virtual bool				LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate);

	virtual bool				SaveToBinary(CNtlSerializer& serializer);



private:
	static WCHAR* m_pwszSheetList[];

	sSERVERCONFIG_DATA				m_sServerConfigData;
	sMASCOT_LUMINOSITY_DROP_DATA	m_aMascotLuminosityDropData[MASCOT_LUMINOSITY_DROP_COUNT];
};