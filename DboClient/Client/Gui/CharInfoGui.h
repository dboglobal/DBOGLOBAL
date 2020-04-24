/******************************************************************************
* File			: CharInfoGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2007. 10. 17
* Abstract		:
*****************************************************************************
* Desc			: 로비에서 현재 선택된 캐릭터의 정보를 보여준다
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

// simulation
#include "Inputhandler.h"

// dbo
#include "Windowby3.h"

class CCharacterInfoGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CCharacterInfoGui(const RwChar* pName);
	virtual ~CCharacterInfoGui();

	RwBool		Create();
	VOID		Destroy();

	VOID		SetChar(RwUInt8 byIndex);
	VOID		SetPosition(RwInt32 iXPos, RwInt32 iYPos);

protected:
	CCharacterInfoGui() {}
	virtual VOID	HandleEvents(RWS::CMsg &msg);

	VOID			SetReservatedDelInfo(RwUInt8 byCharIndex);

	VOID			OnPaint();

protected:
	gui::CSlot			m_slotPaint;

	CWindowby3			m_Background;		///< 배경

	CSurfaceGui			m_srtInfoBack;		///< 캐릭터 정보 배경

	gui::CStaticBox*	m_pTitleStatic;		///<
	gui::CStaticBox*	m_pNameStatic;		///< '이름'
	gui::CStaticBox*	m_pLevelStatic;		///< '레벨'
	gui::CStaticBox*	m_pClassStatic;		///< '클래스'
	gui::CStaticBox*	m_pZennyStatic;		///< '소유제니'
	gui::CStaticBox*	m_pLastLocationStatic;///< '최종위치'
	gui::CStaticBox*	m_pDeleteDayStatic;	///< '삭제 예정일'
	gui::CStaticBox*	m_pName;			///< 캐릭터 이름
	gui::CStaticBox*	m_pLevel;			///< 캐릭터 레벨
	gui::CStaticBox*	m_pClass;			///< 캐릭터 클래스
	gui::CStaticBox*	m_pZenny;			///< 캐릭터 소유 제니
	gui::CStaticBox*	m_pLastLocation;	///< 캐릭터 최종 위치

	gui::CStaticBox*	m_pDeleteDay;		///< 캐릭터 삭제 예정일
	gui::CStaticBox*	m_pDeleteDayExplain;///< 캐릭터 삭제 예정일 상세 설명
};