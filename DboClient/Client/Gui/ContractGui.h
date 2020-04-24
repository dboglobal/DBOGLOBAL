/*****************************************************************************
 * File			: ContractGui.h
 * Author		: Hong Sungbock
 * Copyright	: (주)NTL
 * Date			: 2009. 2. 26
 * Abstract		: DBO contract gui.
 *****************************************************************************
 * Desc         : Definition of when to see the terms GUI
 *
 *				  1. When you see it automatically
 *					- The first time you access the account from the local PC
 *					- Renewal of Terms and Conditions
 *					- When DBO's expansion pack comes up, it is the first time you access the account on your local PC
 *
 *				  2. When the "Terms" button is pressed on the DBO Title screen
 *
 *				  3. If you see the terms automatically, do not "agree" to end the game
 *****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

// cleint
#include "SurfaceGui.h"

class CContractGui : public CNtlPLGui, public RWS::CEventHandler
{
public:

	CContractGui(const RwChar *pName);
	~CContractGui();

	RwBool		Create();
	VOID		Destroy();

protected:

	CContractGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			LocateComponent(RwInt32 iWidth, RwInt32 iHeight);

	VOID			OnClicked_AgreementButton(gui::CComponent* pComponent);
	VOID			OnClicked_OKButton(gui::CComponent* pComponent);
	VOID			OnClicked_CancelButton(gui::CComponent* pComponent);
		
	VOID			OnScrollChanged(RwInt32 iOffset);
	VOID			OnCaptureWheelMove(RwInt32 iFlag, RwInt16 sDelta, CPos& pos);
	VOID			OnPaint();

protected:
	gui::CSlot			m_slotAgreementButton;
	gui::CSlot			m_slotOKButton;
	gui::CSlot			m_slotCancelButton;	
	gui::CSlot			m_slotServerScrollChanged;
	gui::CSlot			m_slotServerScrollSliderMoved;
	gui::CSlot			m_slotCaptureWheelMove;
	gui::CSlot			m_slotPaint;

	gui::CHtmlBox*		m_pContractHtml;

	gui::CStaticBox*	m_pTitleText;

	gui::CStaticBox*	m_pPleaseRead;

	gui::CButton*		m_pAgreementButton;
	gui::CButton*		m_pOKButton;
	gui::CButton*		m_pCancelButton;

	CSurfaceGui			m_srfBackground;					///< 검은 배경

private:

	bool				m_bContinueLogin;
};