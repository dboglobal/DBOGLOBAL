/******************************************************************************
* File			: TMQStatusGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2007. 10. 29
* Abstract		: 
*****************************************************************************
* Desc			: TMQ 진행 상태창
*****************************************************************************/

#pragma once

#include <list>

// core
#include "ceventhandler.h"

// share
#include "NtlTimeQuest.h"

// presentation
#include "NtlPLGui.h"

// simulation
#include "NtlSLDef.h"
#include "NtlSLEvent.h"

// dbo
#include "SurfaceGui.h"
#include "SlotGui.h"
#include "NumberGui.h"

class CLittleQuestContainer;

class CTMQStatusGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	struct sTimeBonus
	{
		RwReal		fElapsed;
		RwBool		bShow;
		CSurfaceGui	srfTimeBonus;
		CNumberGui	TimeBonusNum;
	};

	CTMQStatusGui(const RwChar* pName);
	virtual ~CTMQStatusGui();

	static VOID		CreateInstance();
	static VOID		DestroyInstance();

	RwBool		Create();
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);

protected:
	CTMQStatusGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			SetTMQPoint(RwInt32 iPoint);
	VOID			ShowBonusTime(RwReal fBonusTime);
	VOID			CheckInfoWindow();

	VOID			OnClicked_MinimizeButton(gui::CComponent* pComponent);
	VOID			OnClicked_SenarioButton(gui::CComponent* pComponent);
	VOID			OnClicked_InfoButton(gui::CComponent* pComponent);
	VOID			OnClicked_NextButton(gui::CComponent* pComponent);

	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID			OnMouseLeave(gui::CComponent* pComponent);
	VOID			OnPaint();
	VOID			OnPostPaint();

protected:		
	static CTMQStatusGui*	m_pInstance;

	gui::CSlot			m_slotMinimizeButton;
	gui::CSlot			m_slotSenarioButton;
	gui::CSlot			m_slotStageButton;
	gui::CSlot			m_slotNextButton;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotPostPaint;

	gui::CStaticBox*	m_pDialogName;					///< 다이얼로그의 이름 스태틱
	gui::CStaticBox*	m_pInfo;						///< '정보'
	gui::CStaticBox*	m_pPoint;						///< '포인트'
	gui::CStaticBox*	m_pCurPoint;					///< 현재 포인트
	gui::CStaticBox*	m_pQuest;						///< '퀘스트'
	gui::CStaticBox*	m_pQuestTitle;					///< 퀘스트 제목
	gui::CStaticBox*	m_pQuestAim_1;					///< 퀘스트 목표 1
	gui::CStaticBox*	m_pQuestAim_2;					///< 퀘스트 목표 2
	gui::CStaticBox*	m_pQuestAim_3;					///< 퀘스트 목표 3
	gui::CStaticBox*	m_pQuestAim_4;					///< 퀘스트 목표 4
	
	gui::CButton*		m_pMinimizeButton;				///< 최소화 버튼
	gui::CButton*		m_pStageButton;					///< 스테이지 정보
	gui::CButton*		m_pSenarioButton;				///< 시나리오 버튼
	gui::CButton*		m_pNextButton;					///< 다음 퀘스트 버튼

	gui::CFlickerEffect	m_SenarioButtonFlicker;
	gui::CFlickerEffect	m_StageButtonFlicker;

	CSurfaceGui			m_srfGaugeBack;					///< TMQ 포인트 백서페이스
	CSurfaceGui			m_srfTMQContourLine_1;			///< 구분선
	CSurfaceGui			m_srfTMQContourLine_2;			///< 구분선
	CSurfaceGui			m_srfTMQContourLine_3;			///< 구분선
	CSurfaceGui			m_srfCouponSlot;				///< 쿠폰 슬롯

	gui::CProgressBar*	m_pTMQGauge;					///< TMQ 게이지

	CLittleQuestContainer* m_pTMQQuestContainer;
	CRegularSlotGui			m_CouponSlot;					///< 쿠폰 슬롯
};