/******************************************************************************
* File			: PartyMemberGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Abstract		: 2006. 10. 13
*****************************************************************************
* Desc			: CPartyMemberBuffGui : 파티원의 버프를 보여준다.
*
*				  CPartyMemberGui : 파티원의 정보를 보여준다.
*									CPartyMemberBuffGui객체를 두개를
*									가지고서 위아래 2층으로 구성된 버프를
*									보여준다.
*****************************************************************************/

#pragma once

#include <list>

// core
#include "ceventhandler.h"

// shared
#include "NtlParty.h"

// presetation
#include "NtlPLGui.h"

// simulation
#include "NtlSLDef.h"

// dbo
#include "SurfaceGui.h"

class CBuffDispGui;

class CPartyMemberBuffGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CPartyMemberBuffGui();
	virtual ~CPartyMemberBuffGui();

	RwBool		Create(SERIAL_HANDLE hHandle);
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();

	VOID		Show(bool bShow);
	VOID		SetPosition(RwInt32 iXPos, RwInt32 iYPos);

protected:
	VOID		HandleEvents(RWS::CMsg &pMsg);	

	VOID		OnPaint();

	VOID		OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID		OnMouseLeave(gui::CComponent* pComponent);

protected:
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotPaint;

	SERIAL_HANDLE		m_hOwnerHandle;

	CBuffDispGui*		m_pBuff;
};

class CPartyMemberGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CPartyMemberGui();
	virtual ~CPartyMemberGui();

	RwBool		Create(SERIAL_HANDLE hSerial, WCHAR* pcText, RwUInt8 byClass);
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();

	SERIAL_HANDLE GetHandle();						///< 맴버의 핸들 반환

	VOID		SetLP(RwInt32 iValue, RwInt32 iMaxValue);	///< LP 설정
	VOID		SetEP(RwInt32 iValue, RwInt32 iMaxValue);	///< EP 설정
	VOID		SetLevel(RwInt8 byLevel);			///< Level 설정
	VOID		SetClass(RwUInt8 byClass);			///< Class 설정
	VOID		SetLeader(RwBool bLeader);			///< 파티 리더라면 설정
	VOID		SetOtherWorld(RwBool bOtherWorld);
	VOID		SetCurEmergencyPulse(RwReal fPulse);
	VOID		SetEmergencyState(RwBool bState);

	RwReal		GetCurEmergencyPulse();
	RwBool		GetEmergencyState();

	VOID		ShowBuff(bool bShow);

protected:
	virtual VOID HandleEvents( RWS::CMsg &msg );

	VOID		OnPostPaint();
	
	VOID		OnMouseDown(const CKey& key);
	VOID		OnMouseUp(const CKey& key);
	VOID		OnMove(RwInt32 nX, RwInt32 nY);
	VOID		OnMouseEnter(gui::CComponent* pComponent);
	VOID		OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID		OnMouseLeave(gui::CComponent* pComponent);

protected:
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseEnter;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotPostPaint;

	gui::CPanel*		m_pTributarySimbol;			///< 종족과 직업 그림

	gui::CStaticBox*	m_pNameStatic;				///<  맴버 이름

	gui::CProgressBar*	m_pLPGauge;					///< LP 게이지
	/*
	gui::CStaticBox*	m_pLPStatic;				///< LP 스태틱
	gui::CStaticBox*	m_pLPSlachStatic;			///< 슬래쉬 스태틱
	gui::CStaticBox*	m_pMaxLPStatic;				///< MaxLP 스태틱
*/
	gui::CProgressBar*	m_pEPGauge;					///< EP 게이지
	/*
	gui::CStaticBox*	m_pEPStatic;				///< EP 스태틱
	gui::CStaticBox*	m_pEPSlachStatic;			///< 슬래쉬 스태틱
	gui::CStaticBox*	m_pMaxEPStatic;				///< MAxEP 스태틱
*/
	gui::CStaticBox*	m_pLevel;					///< Level

	CSurfaceGui			m_srfOtherWorldPanel;		///< 다른 월드에 있는 파티원의 패널
	CSurfaceGui			m_srfDeathPanel;			///< 사망시 패널

	CSurfaceGui			m_srfLeaderMark;			///< 리더 마크
	CSurfaceGui			m_srfDeathMark;				///< 사망 마크
	CSurfaceGui			m_srfTooFarMark;			///< 멀리 떨어진 파티원

	gui::CFlickerEffect	m_feEmergency;				///< LP 경고 표시등

	SERIAL_HANDLE		m_hSerial;					///< 맴버의 시리얼
	RwUInt8				m_byClass;					///< 클래스
	RwInt8				m_byMouse;					///< 맴버창이 눌렸는지 여부

	RwUInt8				m_byDisplayFlag;

	CPartyMemberBuffGui*	m_pBuff;				///< 버프윈도우
};