#pragma once
//***********************************************************************************
//
//	File		:	SpawnTable.h
//
//	Begin		:	2006-03-27
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Doo Sup, Chung   ( john@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#include "Table.h"
#include "NtlVector.h"

#pragma pack(push, 4)
struct sSPAWN_TBLDAT : public sTBLDAT
{
public:

	sSPAWN_TBLDAT()
		: spawnGroupId ( INVALID_SPAWNGROUPID )
		, byWander_Range ( INVALID_BYTE )
	{
	}

	TBLIDX			mob_Tblidx;

	CNtlVector		vSpawn_Loc;
	CNtlVector		vSpawn_Dir;

	BYTE			bySpawn_Loc_Range;
	BYTE			bySpawn_Quantity;
	WORD			wSpawn_Cool_Time;
	BYTE			bySpawn_Move_Type;

	BYTE			byWander_Range;
	BYTE			byMove_Range;

	BYTE			actionPatternTblidx;

	TBLIDX			path_Table_Index;
	CNtlVector		vFollowDistance;

	TBLIDX			playScript;
	TBLIDX			playScriptScene;
	
	TBLIDX			aiScript;
	TBLIDX			aiScriptScene;

	DWORD			dwParty_Index;
	bool			bParty_Leader;

	SPAWNGROUPID	spawnGroupId;

protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CSpawnTable :	public CTable
{
public:
	CSpawnTable(void);

	virtual ~CSpawnTable(void);

public:

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *			FindData(TBLIDX tblidx); 

protected:
	WCHAR** GetSheetListInWChar() { return &(CSpawnTable::m_pwszSheetList[0]); }
	void* AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage);
	bool DeallocNewTable(void* pvTable, WCHAR* pwszSheetName);
	bool AddTable(void * pvTable, bool bReload, bool bUpdate);
	bool SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData);

public:

	virtual bool				LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate);

	virtual bool				SaveToBinary(CNtlSerializer& serializer);


public:

	sSPAWN_TBLDAT *				GetSpawnGroupFirst(SPAWNGROUPID spawnGroupId);

	sSPAWN_TBLDAT *				GetSpawnGroupNext(SPAWNGROUPID spawnGroupId);

	DWORD						GetSpawnGroupCount(SPAWNGROUPID spawnGroupId) { return (DWORD) m_spawnGroup.count( spawnGroupId); }


private:

	static WCHAR* m_pwszSheetList[];


private:

	typedef std::multimap<SPAWNGROUPID, sSPAWN_TBLDAT*> SPAWNGROUP;
	typedef SPAWNGROUP::iterator SPAWNGROUP_IT;
	typedef SPAWNGROUP::value_type SPAWNGROUP_VAL;

	SPAWNGROUP					m_spawnGroup;

	SPAWNGROUP_IT				m_spawnGroupItBeg;

	SPAWNGROUP_IT				m_spawnGroupItEnd;
	
};
