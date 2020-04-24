#pragma once
//last update 30.11.2014
#include "Table.h"
#include "NtlVector.h"
#include "NtlDragonBall.h"


const DWORD		DBO_MAX_LENGTH_WORLD_NAME = 32;
const DWORD		DBO_MAX_LENGTH_WORLD_NAME_IN_UNICODE = 32;
const DWORD		DBO_MAX_LENGTH_WORLD_MOB_SPAWN_TABLE_NAME = 64;
const DWORD		DBO_MAX_LENGTH_WORLD_NPC_SPAWN_TABLE_NAME = 64;
const DWORD		DBO_MAX_LENGTH_WORLD_OBJ_SPAWN_TABLE_NAME = 64;
const DWORD		DBO_MAX_LENGTH_WORLD_RESOURCE_FOLDER_NAME = 64;
const DWORD		DBO_MAX_LENGTH_WORLD_RESOURCE_FLASH_NAME = 32;

enum eWORLD_PROPHITED
{
	eWORLD_PROPHITED_AIR
};

enum eWORLD_PROPHITED_FLAG
{
	eWORLD_PROPHITED_AIR_FLAG = 0x01 << eWORLD_PROPHITED_AIR
};

#pragma pack(push, 4)
struct sWORLD_TBLDAT : public sTBLDAT
{
public:

	char			szName[ DBO_MAX_LENGTH_WORLD_NAME + 1];
	WCHAR			wszName[ DBO_MAX_LENGTH_WORLD_NAME_IN_UNICODE + 1 ];

	bool			bDynamic;
	int				nCreateCount;

	DWORD			dwDynamicCreateCountShareGroup;

	BYTE			byDoorType;

	DWORD			dwDestroyTimeInMilliSec;

	WCHAR			wszMobSpawn_Table_Name[ DBO_MAX_LENGTH_WORLD_MOB_SPAWN_TABLE_NAME + 1 ];
	WCHAR			wszNpcSpawn_Table_Name[ DBO_MAX_LENGTH_WORLD_NPC_SPAWN_TABLE_NAME + 1 ];
	WCHAR			wszObjSpawn_Table_Name[ DBO_MAX_LENGTH_WORLD_OBJ_SPAWN_TABLE_NAME + 1 ];

	CNtlVector		vStart;//3
	CNtlVector		vEnd;//6

	CNtlVector		vStandardLoc;//9

	CNtlVector		vBattleStartLoc;//12
	CNtlVector		vBattleEndLoc;//15

	CNtlVector		vBattleStart2Loc;//18
	CNtlVector		vBattleEnd2Loc;//21

	CNtlVector		vOutSideBattleStartLoc;//24
	CNtlVector		vOutSideBattleEndLoc;//27
	CNtlVector		vSpectatorStartLoc;//30
	CNtlVector		vSpectatorEndLoc;//33

	CNtlVector		vDefaultLoc;//36
	CNtlVector		vDefaultDir;//39

	CNtlVector		vStart1Loc;//42
	CNtlVector		vStart1Dir;//45
	CNtlVector		vStart2Loc;//48
	CNtlVector		vStart2Dir;//51

	CNtlVector		vWaitingPoint1Loc;//54
	CNtlVector		vWaitingPoint1Dir;//57

	CNtlVector		vWaitingPoint2Loc;//60
	CNtlVector		vWaitingPoint2Dir;//63

	float			fSplitSize; //should be "50.0"
	bool			bNight_Able;
	BYTE			byStatic_Time;
	WORD			wFuncFlag;

	BYTE			byWorldRuleType;	// eGAMERULE_TYPE
	TBLIDX			worldRuleTbldx;		// 해당하는 룰 테이블 인덱스 ( 룰마다 테이블 틀림 : 랭크배틀/타임퀘스트/ 등등... )

	TBLIDX			outWorldTblidx;
	CNtlVector		outWorldLoc;
	CNtlVector		outWorldDir;

	WCHAR			wszResourceFolder[ DBO_MAX_LENGTH_WORLD_RESOURCE_FOLDER_NAME + 1];
	float			fBGMRestTime;
	DWORD			dwWorldResourceID;
	float			fFreeCamera_Height;

	WCHAR			wszEnterResourceFlash[ DBO_MAX_LENGTH_WORLD_RESOURCE_FLASH_NAME + 1];
	WCHAR			wszLeaveResourceFlash[ DBO_MAX_LENGTH_WORLD_RESOURCE_FLASH_NAME + 1];

	TBLIDX			wpsLinkIndex;

	//new
	BYTE			byStartPointRange;
	BYTE			abyDragonBallHaveRate[DBO_MAX_WORLD_DRAGONBALLDROP];
	BYTE			abyDragonBallDropRate[DBO_MAX_WORLD_DRAGONBALLDROP];
	//bool			bCanFly;

	DWORD			dwProhibition_Bit_Flag;


protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CWorldTable : public CTable
{
public:

	CWorldTable(void);
	virtual ~CWorldTable(void);

/*
public:

	BOOL				Check() { return TRUE; }*/

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *			FindData(TBLIDX tblidx); 
	
protected:
	WCHAR** GetSheetListInWChar() { return &(CWorldTable::m_pwszSheetList[0]); }
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
