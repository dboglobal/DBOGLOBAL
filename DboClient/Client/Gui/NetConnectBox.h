///******************************************************************************
//* File			: NetConnectBox.h
//* Author		: Hong SungBock
//* Copyright		: (주)NTL
//* Date			: 2007. 1. 4
//* Update		: 2007. 10. 15
//* Abstract		: network connect 전용 box
//*****************************************************************************
//* Desc			: CNetConnectBox
//*****************************************************************************/
//
//#pragma once
//
//// core
//#include "ceventhandler.h"
//
//// gui
//#include "gui_button.h"
//
//// presentation
//#include "NtlPLGui.h"
//
//// dbo
//#include "DboDef.h"
//#include "Windowby3.h"
//
//class CNetConnectBox : public CNtlPLGui , public RWS::CEventHandler
//{
//public:
//	CNetConnectBox(const RwChar* pName);
//	virtual ~CNetConnectBox();
//
//	RwBool		Create();
//	VOID		Destroy();
//	VOID		Update(RwReal fElapsed);
//
//protected:
//	CNetConnectBox() {}
//	VOID			HandleEvents( RWS::CMsg &msg );	
//	
//	VOID			CalcBoxSize();				///< 주어진 메세지에 맞는 박스를 만든다
//	VOID			SetText();
//	VOID			ProcessCancel();
//	VOID			SwitchDialog(bool bSwitch);
//
//	VOID			OnClickedOkButton(gui::CComponent* pComponent);
//	VOID			OnClickedCancelButton(gui::CComponent* pComponent);	
//	VOID			OnPaint();	
//
//protected:
//
//	gui::CSlot			m_slotPaint;
//	gui::CSlot			m_slotOk;
//	gui::CSlot			m_slotCencle;
//
//	CWindowby3			m_Backboard;			///< 배경 
//	CWindowby3			m_Panel;				///< 글자 배경 틀
//
//	gui::CButton*		m_pOkBtn;
//	gui::CButton*		m_pCancelBtn;
//
//	gui::CStaticBox*	m_pMessage;				///< 메세지 내용
//
//	std::wstring		m_wstrString;			///< 원본 메세지
//	RwUInt8				m_byMsgBoxType;			///< 메세지 박스 모양 타입
//	RwUInt32			m_uiMsgProcessType;		///< 메세지 처리 타입
//	RwReal				m_fRemainTime;			///< 업데이트 타임
//	sMsgBoxData			m_ExData;				///< 메세지 확장 데이타
//
//	RwBool				m_bRemainTime;			///< 메세지 박스를 보여주는 제한 시간이 있는지 여부
//	const WCHAR*		m_pwcText;				///< 마지막에 보여주었던 스트링 테이블 인덱스
//};