//***********************************************************************************
//
//	File		:	DojoTable.h
//
//	Begin		:	2008-12-29
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Chung,DooSup ( mailto:john@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once


#include "table.h"
#include "NtlVector.h"

const DWORD	    DOJO_MAX_UPGRADE_OBJECT_COUNT	= 4;
const DWORD	    DOJO_MAX_REWARD_TYPE_COUNT	= 5;

struct sDOJO_REWARD
{
	DWORD	dwGetPoint;
	BYTE	byGetRock;
};
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
#pragma pack(push, 4)
struct sDOJO_TBLDAT : public sTBLDAT
{
public:

	sDOJO_TBLDAT( void ) { }

public:

	TBLIDX			zoneTblidx; // WorldTable Index
	TBLIDX			objectTblidx[DOJO_MAX_UPGRADE_OBJECT_COUNT];
	TBLIDX			mapName;		// town name textalltblidx
	BYTE			byReceiveHour;	// 신청대기시작시간
	BYTE			byReceiveMinute; // 신청대기시작분
	BYTE			byRepeatType;	// 신청대기 상태 반복 방식 eDBO_DOJO_REPEAT_TYPE
	BYTE			byRepeatTime;	// 신청대기상태의 시작이 반복되는 일정 시간 -> 반복타입이 시간일경우만 사용
	WORD			wWeekBitFlag;   // 신청대기 시작 요일 -> 월수금 일주일에 일정한날 eDBO_DOJO_WEEK_BITFLAG
	BYTE			byReceiveDuration; // 시작시간에서 신청가능한 시간 분 예 30분
	BYTE			byRejectDuration;  // 시작시간 기준 거부가능한 시간 분 예 20분
	BYTE			byStandbyDuration; // 쟁탈전 준비상태의 시작부터 종료되는 기간 분 예 20분
	BYTE			byInitialDuration; // 쟁탈전 준비상태 종료 시점부터 도장 초기화 상태가 종료되는 기간 예 7분
	BYTE			byReadyDuration;   // 초기화상태 종료 시간 예3분
	BYTE			byBattleDuration;  // 쟁탈전 시작후 완료까지의 최대 시간
	DWORD			dwReceivePoint;
	DWORD			dwReceiveZenny;
	TBLIDX			controllerTblidx;
	DWORD			dwBattlePointGoal;
	DWORD			dwBattlePointGet;
	DWORD			dwBattlePointCharge;
	DWORD			dwChargePointGoal;
	DWORD			dwChargeTime;
	DWORD			dwChageTimePoint;
	TBLIDX			rockTblidx;
	sDOJO_REWARD	asRawrd[DOJO_MAX_REWARD_TYPE_COUNT];


protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
class CDojoTable : public CTable
{
public:

	CDojoTable( void );

	virtual ~CDojoTable( void );

public:

	virtual bool			LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate);

	virtual bool			SaveToBinary(CNtlSerializer& serializer);

public:

	bool					Create( DWORD dwCodePage );

	void					Destroy( void );

public:

	sTBLDAT*				FindData(TBLIDX tblidx); 

	sTBLDAT*				GetDojoData(); 

	bool					IsWorldDojo( TBLIDX worldIdx );

	TBLIDX					GetTblidxByWorldIdx(TBLIDX worldIdx );

	sDOJO_TBLDAT*			GetDojoByMapName(TBLIDX mapname);

protected:

	void					Init( void );

	WCHAR**					GetSheetListInWChar( void ) { return &m_pwszSheetList[0]; }

	void*					AllocNewTable( WCHAR* pwszSheetName, DWORD dwCodePage );

	bool					DeallocNewTable( void* pvTable, WCHAR* pwszSheetName );

	bool					AddTable( void* pvTable, bool bReload, bool bUpdate );

	bool					SetTableData( void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData );

	
protected:

	static WCHAR*			m_pwszSheetList[];

};
