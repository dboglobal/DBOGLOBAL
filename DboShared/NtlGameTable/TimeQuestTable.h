#pragma once
//last update 1.12.2014
#include "table.h"

#include "NtlTimeQuest.h"

#include "NtlVector.h"
#include "NtlSharedDef.h"

const DWORD		DBO_MAX_LENGTH_PROLOGUE_DIRECTION = 32;
const DWORD		DBO_MAX_LENGTH_BGM_STRING = 32;
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
#pragma pack(push, 4)
struct sTIMEQUEST_DATASET
{
	TBLIDX			nameTblidx; // Quest title text index
	TBLIDX			questStringTblidx; // Quest Description Text Index
	TBLIDX			worldTblidx; // World Index
	TBLIDX			scriptTblidx; // Script Index

	BYTE			byMinMemberCount; // Minimum number of people
	BYTE			byMaxMemberCount; // Maximum number of people
	BYTE			byMinMemberLevel; // Minimum level
	BYTE			byMaxMemberLevel; // Maximum level

	DWORD			dwLimitTime; // Timeout
	DWORD			dwNeedZenny; // Admission fee
	TBLIDX			needItemTblidx; // Required Item Index

	BYTE			byNeedLimitCount;	// need item count
	BYTE			byWorldCount;		// world count

	TBLIDX			dayRecordRewardTblidx;
	TBLIDX			bestRecordRewardTblidx;
	
	
};
#pragma pack(pop)

#pragma pack(push, 4)
struct sTIMEQUEST_TBLDAT : public sTBLDAT
{
public:

	sTIMEQUEST_TBLDAT( void ) { }

public:

	BYTE			byTimeQuestType; // 타임퀘스트 타입 ( eTIMEQUEST_TYPE )
	BYTE			byDifficultyFlag; // 난이도 플래그( eTIMEQUEST_DIFFICULTY_FLAG )
	DWORD			dwStartTime; // 시작시간 ( 하루 00:00 을 기준으로 시작되는 분단위를 넣는다 )

	TBLIDX			startCharacterDirection;
	TBLIDX			startObjectIndex;
	TBLIDX			startTriggerId;

	BYTE			abyStartTriggerDirectionState[10]; //new

	TBLIDX			arriveCharacterDirection;
	TBLIDX			arriveObjectIndex;
	TBLIDX			arriveTriggerId;

	TBLIDX			leaveCharacterDirection;	
	TBLIDX			leaveObjectIndex;	
	TBLIDX			leaveTriggerId;

	TBLIDX			dayRecordMailTblidx;
	TBLIDX			bestRecordMailTblidx;

	BYTE			byResetTime;				// 리셋 시키는 시간 0~23 사용

	WCHAR			wszPrologueDirection[ DBO_MAX_LENGTH_PROLOGUE_DIRECTION + 1 ];
	sTIMEQUEST_DATASET sTimeQuestDataset[ MAX_TIMEQUEST_DIFFICULTY ];
	TBLIDX			openCine;
	TBLIDX			Note;
	WCHAR			wszStageBgm1[ DBO_MAX_LENGTH_BGM_STRING + 1 ];
	WCHAR			wszStageBgm2[ DBO_MAX_LENGTH_BGM_STRING + 1 ];
	WCHAR			wszLastBgm[ DBO_MAX_LENGTH_BGM_STRING + 1 ];
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
class CTimeQuestTable : public CTable
{
public:

	CTimeQuestTable( void );

	virtual ~CTimeQuestTable( void );

public:

	virtual bool			LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate);

	virtual bool			SaveToBinary(CNtlSerializer& serializer);

public:

	bool					Create( DWORD dwCodePage );

	void					Destroy( void );

public:

	sTBLDAT*				FindData(TBLIDX tblidx); 

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