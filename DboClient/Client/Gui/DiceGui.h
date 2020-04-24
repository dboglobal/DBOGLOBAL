/******************************************************************************
* File			: DiceGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 5. 26
* Abstract		: 
*****************************************************************************
* Desc			: 
*****************************************************************************/

#pragma once

// presetation
#include "NtlPLGui.h"

// dbo
#include "SlotGui.h"
#include "DiceManager.h"

struct sITEM_PROFILE;

struct sWaitDiceDisappear
{
	RwReal		fElapsedDisappear;	///< 주사위가 사라지기까지의 시간
	RwBool		bWaitDisappear;		///< 주사위가 사라지기를 기다리는지 여부

	sWaitDiceDisappear()
	:fElapsedDisappear(0.f), bWaitDisappear(false)
	{
	}
};

class CDiceGui : public CNtlPLGui
{
public:
	CDiceGui(const RwChar* pName, RwUInt8 byWorkID)
	:CNtlPLGui(pName)
	,m_byWorkID(byWorkID)
	,m_byResultNum(0)
	,m_bGenerateResultEvent(false)
	,m_byFunctionFlag(dDICE_FUNCTION_NONE)
	{}

	virtual ~CDiceGui() {}

	virtual RwBool	Create(RwUInt8 byFunctionFlag) = 0;
	virtual VOID	Destroy() = 0;

	virtual RwBool	ReadyDice() = 0;

	RwUInt8			GetWorkID() { return m_byWorkID; }

	virtual VOID	SetResultNum(RwUInt8 byNum) { m_byResultNum = byNum; }
	virtual VOID	Switch(bool bOpen) = 0;

protected:
	CDiceGui() {}

protected:
	RwUInt8		m_byWorkID;
	RwUInt8		m_byResultNum;
	RwBool		m_bGenerateResultEvent;
	RwUInt8		m_byFunctionFlag;
};


class CActionDiceGui : public CDiceGui
{
public:
	CActionDiceGui(const RwChar* pName, RwUInt8 byWorkID)
	:CDiceGui(pName, byWorkID) {}
	virtual ~CActionDiceGui();

	RwBool		Create(RwUInt8 byFunctionFlag);
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();

	VOID		Switch(bool bOpen);

	virtual RwBool	ReadyDice();
	virtual VOID	SetResultNum(RwUInt8 byNum);

	VOID			SetExplain(const WCHAR* pwcText);
	VOID			SetExplain2(const WCHAR* pwcText);

protected:
	CActionDiceGui() {}
	CActionDiceGui(const RwChar* pName) {}

	VOID			CheckInfoWindow();

	VOID			OnClicked_CloseButton(gui::CComponent* pComponent);
	VOID			OnMovieEnd(gui::CComponent* pComponent);
	VOID			OnFSCallback(const char* pcString, const char* pcString2);

protected:	
	gui::CSlot			m_slotCloseButton;
	gui::CSlot			m_slotFSCallback;
	gui::CSlot			m_slotMovieEnd;
	
	gui::CButton*		m_pExitButton;			///< 창닫기 버튼
	gui::CFlash*		m_pflashDice;			///< 주사위 플래쉬

	gui::CStaticBox*	m_pActionDiceExplain;	///< 액션 주사위 설명
	gui::CStaticBox*	m_pActionDiceExplain2;	///< 액션 주사위 설명2
	gui::CStaticBox*	m_pResultNum;			///< 주사위를 굴린 숫자
};


class CItemDiceGui : public CDiceGui
{
public:	
	CItemDiceGui(const RwChar* pName, RwUInt8 byWorkID);
	virtual ~CItemDiceGui();

	RwBool		Create(RwUInt8 byFunctionFlag);
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();

	VOID		Switch(bool bOpen);

	virtual RwBool	ReadyDice();
	virtual VOID	SetResultNum(RwUInt8 byNum);

	VOID			SetExplain(const WCHAR* pwcText);
	
	VOID			SetItemData(void* pItemData);

protected:
	CItemDiceGui() {}
	CItemDiceGui(const RwChar* pName) {}

	VOID			CheckInfoWindow();

	VOID			OnClicked_CloseButton(gui::CComponent* pComponent);
	VOID			OnMovieEnd(gui::CComponent* pComponent);
	VOID			OnFSCallback(const char* pcString, const char* pcString2);

	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID			OnMouseLeave(gui::CComponent* pComponent);
	VOID			OnPaint();

protected:	
	gui::CSlot			m_slotCloseButton;
	gui::CSlot			m_slotFSCallback;
	gui::CSlot			m_slotMovieEnd;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotPaint;

	gui::CButton*		m_pExitButton;			///< 창닫기 버튼
	gui::CFlash*		m_pflashDice;			///< 주사위 플래쉬

	gui::CStaticBox*	m_pAutoDiceExplain;		///< 자동 주사위 설명
	gui::CStaticBox*	m_pResultNum;			///< 주사위를 굴린 숫자

	gui::CProgressBar*	m_pTimeGauge;			///< 주사위 굴리기 시간 제한

	CSurfaceGui			m_srfItemSlot;			///< 아이템 슬롯 배경
	CSurfaceGui			m_srfTimeGaugeBack;		///< 주사위 굴리기 시간 제한 배경

	CRegularSlotGui			m_slot;					///< 아이템 슬롯

	RwReal				m_fElapsed;				///< 주사위를 굴려야 할 제한 시간
	
	sWaitDiceDisappear	m_WaitDiceDisappear;	///< 주사위가 사라질 때까지 필요로 하는 정보

	sITEM_PROFILE*		m_pItemProfile;	
};