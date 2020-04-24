/******************************************************************************
* File			: TimeNotifyGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 1. 2
* Abstract		: 
*****************************************************************************
* Desc			: 시간에 관련된 사항을 알린다
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

// trigger
#include "DboTSCoreDefine.h"

// dbo
#include "NumberGui.h"


class CTimeNotifyGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	enum eTimeNotifyType
	{
		TIME_TYPE_TMQ_QUEST_LIMIT_TIME,
		TIME_TYPE_RANK_BATTLE_LIMIT_TIME,
		TIME_TYPE_BUDOKAI_LIMIT_TIME,

		TIME_TYPE_INVALID
	};

	enum eDisplayType
	{
		DISPLAY_TYPE_NORMAL,
		DISPLAY_TYPE_WARNING,

		NUM_DISPLAY_TYPE
	};

	struct sTimeInfo
	{
		RwUInt8			byType;
		RwReal			fElapsed;
		
		sTS_KEY					sTSKey;				///< TS키
		RwUInt32				uiSortType;			///< 퀘스트 타입
	};

	CTimeNotifyGui(const RwChar* pName);
	virtual ~CTimeNotifyGui();

	RwBool		Create();
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);

protected:
	CTimeNotifyGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			SetDisplayType();
	VOID			LocateComponent();

	VOID			OnPaint();

protected:
	gui::CSlot			m_slotPaint;

	CNumberGui			m_MinuteNumber[NUM_DISPLAY_TYPE];
	CNumberGui			m_SecondNumber[NUM_DISPLAY_TYPE];

	CSurfaceGui			m_srfColon[NUM_DISPLAY_TYPE];

	sTimeInfo			m_TimeInfo;

	RwUInt8				m_byDisplayType;
};