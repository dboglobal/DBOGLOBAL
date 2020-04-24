/*****************************************************************************
*
* File			: NtlSLTMQ.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 
* Update		: 10. 29
* Abstract		: Simulation time machine class
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_MACHINE_H__
#define __NTL_MACHINE_H__

// core
#include "NtlDebug.h"
#include "ceventhandler.h"

// share
#include "NtlTimeQuest.h"

// simulation
#include "NtlSLCommunityAgent.h"

struct sTIMEQUEST_TBLDAT;

#define dTMQTYPE_NONE		MAX_TIMEQUEST_TYPE

#define dINVALID_STAGE		-1

class CNtlTMQ : public NtlSLCommunityAgent, public RWS::CEventHandler
{
public:
	CNtlTMQ();
	virtual ~CNtlTMQ();

	struct sWaitInfo
	{
		sTIMEQUEST_TBLDAT*			pTMQTable;	///< TMQ 테이블		
		RwReal						fRemainTime;///< TMQ 추첨/진입까지 남은 시간
		RwBool						bTeleport;	///< TMQ 추첨이 되어 1회 텔레포트 할 수 있다
		SERIAL_HANDLE				hNPCHandle;	///< TMQ 신청을 한 NPC의 핸들
	};

	struct sTMQRunning
	{
		sTIMEQUEST_TBLDAT*		pTMQTable;			///< TMQ 테이블
		eTIMEQUEST_DIFFICULTY	byDifficultyFlag;	///< 난이도 플래그( eTIMEQUEST_DIFFICULTY_FLAG )
		eTIMEQUEST_MODE			eTimeQuestMode;		///< eTIMEQUEST_MODE

		RwUInt8				byStage;
		RwReal				fLimitedTime;		///< 남은 시간
		RwBool				bUpdateTime;		///< 남은 시간 업데이트 여부

		RwReal				fLastDayRecord;		///< 지금까지의 TMQ 최단기록
		RwUInt32			uiClearTime;
		RwUInt32			uiBonusTime;
		RwBool				bDayRecord;
		RwBool				bBestRecord;

		// avooo's comment: 진행에 관한 보다 자세한 정보는 SAvatarInfo의 sWORLD_INFO 로 통해서..
	};

	virtual	RwBool			Create(void);
	VOID					Destroy(void);
	VOID					Update(RwReal fElapsed);

	VOID					ClearBoardInfo();
	VOID					ClearRunningInfo();

	VOID					SetNPCHandle(SERIAL_HANDLE hNPCHandle);

	// TMQ 예약 정보
	sTIMEQUEST_TBLDAT*		GetTMQTable_for_RegRoom();
	RwReal					GetRemainTime_in_Room();
	RwBool					CanTMQTeleport();
	SERIAL_HANDLE			GetNPCHandle();

	// TMQ 진행 정보
	VOID					SetStage(RwUInt8 byStage) { m_RunningInfo.byStage = byStage; }

	sTIMEQUEST_TBLDAT*		GetTMQTable();
	eTIMEQUEST_MODE			GetTMQMode();
	eTIMEQUEST_DIFFICULTY	GetDifficult();
	RwReal					GetRemainTime_in_TMQ();
	RwUInt8					GetStageNum();
	RwReal					GetLastDayRecord();
	RwUInt32				GetClearTime();
	RwUInt32				GetBonusTime();
	RwBool					IsDayRecord();
	RwBool					IsBestRecord();

	VOID					HandleEvents(RWS::CMsg &pMsg);

protected:
	RwUInt8					GetNearDifficult(TBLIDX tmqTblidx);

protected:
	sWaitInfo		m_WaitInfo;			///< 등록후 대기 정보
	sTMQRunning		m_RunningInfo;		///< TMQ 진행 정보
};

inline VOID CNtlTMQ::SetNPCHandle(SERIAL_HANDLE hNPCHandle)
{
	m_WaitInfo.hNPCHandle = hNPCHandle;
}

inline sTIMEQUEST_TBLDAT* CNtlTMQ::GetTMQTable_for_RegRoom()
{
	return m_WaitInfo.pTMQTable;
}

inline RwReal CNtlTMQ::GetRemainTime_in_Room()
{
	return m_WaitInfo.fRemainTime;
}

inline RwBool CNtlTMQ::CanTMQTeleport()
{
	return m_WaitInfo.bTeleport;
}

inline SERIAL_HANDLE CNtlTMQ::GetNPCHandle()
{
	return m_WaitInfo.hNPCHandle;
}

inline sTIMEQUEST_TBLDAT* CNtlTMQ::GetTMQTable()
{
	return m_RunningInfo.pTMQTable;
}

inline eTIMEQUEST_MODE CNtlTMQ::GetTMQMode()
{
	return m_RunningInfo.eTimeQuestMode;
}

inline eTIMEQUEST_DIFFICULTY CNtlTMQ::GetDifficult()
{
	return m_RunningInfo.byDifficultyFlag;
}

inline RwReal CNtlTMQ::GetRemainTime_in_TMQ()
{
	return m_RunningInfo.fLimitedTime;
}

inline RwUInt8 CNtlTMQ::GetStageNum()
{
	return m_RunningInfo.byStage;
}

inline RwReal CNtlTMQ::GetLastDayRecord()
{
	return m_RunningInfo.fLastDayRecord;
}

inline RwUInt32 CNtlTMQ::GetClearTime()
{
	return m_RunningInfo.uiClearTime;
}

inline RwUInt32 CNtlTMQ::GetBonusTime()
{
	return m_RunningInfo.uiBonusTime;
}

inline RwBool CNtlTMQ::IsDayRecord()
{
	return m_RunningInfo.bDayRecord;
}

inline RwBool CNtlTMQ::IsBestRecord()
{
	return m_RunningInfo.bBestRecord;
}

#endif