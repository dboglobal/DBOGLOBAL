/******************************************************************************
* File			: LittleQuestContainer.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2007. 2. 20
* Abstract		: 
*****************************************************************************
* Desc			: 특정 타입의 인스턴스 퀘스트 컨테이너
*****************************************************************************/

#pragma once

#include <list>

// core
#include "ceventhandler.h"

// share
#include "NtlTimeQuest.h"

// simulation
#include "NtlSLDef.h"
#include "NtlSLEvent.h"

// dbo
#include "SurfaceGui.h"

class CLittleQuestContainer : public RWS::CEventHandler
{
#define dMAX_QUEST_AIM			4	///< 퀘스트당 최대 목표 갯수

	struct sQuestData
	{
		sTS_KEY					sTSKey;				///< TS키
		RwUInt32				uiSortType;			///< 퀘스트 타입
		RwUInt32				uiQuestTitle;		///< 퀘스트 제목
		eSTOC_EVT_DATA_TYPE		eEvtInfoType;		///< 서버 이벤트 정보 - 데이터 타입
		uSTOC_EVT_DATA			uEvtInfoData;		///< 서버 이벤트 정보 - 데이터
		RwUInt32				uiTimeLimit;		///< 타임 제한 정보
	};

public:
	CLittleQuestContainer();
	virtual ~CLittleQuestContainer();

	typedef std::list<sQuestData*>				LIST_QUEST;
	typedef std::list<sQuestData*>::iterator	LIST_QUEST_ITER;

	RwBool		Create(RwUInt32 uiQuestType, gui::CStaticBox* pQuestTitle, gui::CStaticBox* pAim_1, gui::CStaticBox* pAim_2,
					   gui::CStaticBox* pAim_3, gui::CStaticBox* pAim_4);
	VOID		Destroy();

	VOID		NextQuest();

protected:
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			InitQuestData();
	VOID			SetIndicator(sQuestData* pQuestData);
	sQuestData*		FindData(NTL_TS_T_ID tID);
	sQuestData*		NextData(NTL_TS_T_ID tID_current);
	VOID			DelData(NTL_TS_T_ID& tID);

protected:		
	gui::CStaticBox*	m_pQuestTitle;				///< 퀘스트 제목
	gui::CStaticBox*	m_pQuestAim[dMAX_QUEST_AIM];///< 퀘스트 목표

	RwUInt32			m_uiQuestType;
	NTL_TS_T_ID			m_tID;						///< 현재 보여지고 있는 퀘스트 ID
	LIST_QUEST			m_listQuestData;
};