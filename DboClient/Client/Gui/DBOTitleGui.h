/*****************************************************************************
 *
 * File			: DBOTitle.h
 * Author		: Hong Sungbock
 * Copyright	: (주)NTL
 * Date			: 2009. 4. 2
 * Abstract		: 
 *****************************************************************************
 * Desc         : DBO Title screen when starting game
 *
 *****************************************************************************/

#pragma once

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "SurfaceGui.h"

class CDBOTitleGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CDBOTitleGui(const RwChar *pName);
	~CDBOTitleGui();

	RwBool		Create();
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();
	
protected:
	CDBOTitleGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			SwitchEnableButtons(bool bEnable);	///< Exit 버튼을 제외한 버튼의 이용가능 설정

	VOID			LocateComponent(RwInt32 iWidth, RwInt32 iHeight);

	VOID			OnClicked_CinemaButton(gui::CComponent* pComponent);
	VOID			OnClicked_ContractButton(gui::CComponent* pComponent);
	VOID			OnClicked_ConnectGameServerButton(gui::CComponent* pComponent);
	VOID			OnClicked_ExitButton(gui::CComponent* pComponent);

	VOID			OnPaint();

	int				CallBackMoviePlayStop();

	VOID			LogInStageEnterEventHandler(RWS::CMsg &msg);
	VOID			LogInStageExitEventHandler(RWS::CMsg &msg);
	VOID			LogInStageTimeOutEventHandler(RWS::CMsg &msg);
	
protected:
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotCinemaButton;
	gui::CSlot			m_slotContractButton;
	gui::CSlot			m_slotConnectGameServerButton;
	gui::CSlot			m_slotExitButton;

	gui::CButton*		m_pCinemaButton;			///< 동영상 재생 버튼
	gui::CButton*		m_pContractButton;			///< 약관 보기 버튼
	gui::CButton*		m_pConnectGameServerButton;	///< 게임 서버 접속 버튼
	gui::CButton*		m_pExitButton;				///< 나가기 버튼

	gui::CStaticBox*	m_pVersion;					///< Dbo 버전 표시

	gui::CFlash*		m_pFlashBackground;

	CSurfaceGui			m_srfTitle;					///< 제목 서페이스
};