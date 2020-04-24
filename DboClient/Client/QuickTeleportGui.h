/*****************************************************************************
 *
 * File			: QuickTeleportGui.h
 * Author		: Daneos
 * Copyright	: DBOG
 * Date			: 3/11/2019
 * Abstract		:
 *****************************************************************************
 * Desc         :
 *
 *****************************************************************************/

 //#ifndef __QUICKTELEPORT_GUI_H__
 //#define __QUICKTELEPORT_GUI_H__
#pragma once

#include "ceventhandler.h"
#include "SurfaceGui.h"

#include "NtlTeleport.h"


class CQuickTeleportGui : public CNtlPLGui, public RWS::CEventHandler
{
	struct sBUTTONS
	{
		gui::CButton*			btnPanel;
		gui::CPanel*			pnlSlotIcon;
		gui::CStaticBox*		stbPosition;
		gui::CStaticBox*		stbDate;
		gui::CStaticBox*		stbEmpty;
		gui::CStaticBox*		stbDeadPosition;
		gui::CStaticBox*		stbDeadDate;

		gui::CSlot				m_slotPanelBtnClick;

		bool					m_bEmpty;
		sQUICK_TELEPORT_INFO	teleportInfo;
	};

public:

	CQuickTeleportGui(const RwChar *pName);
	~CQuickTeleportGui();

	void			Init();
	RwBool			Create();
	void			Destroy();

	int				SwitchDialog(bool bOpen);

protected:

	virtual void	HandleEvents(RWS::CMsg &pMsg);
	void			HandleEventsSubMsgBox(RWS::CMsg& msg);

protected:



protected:

	//!Callback

	void				OnPaint();
	void				OnMove(RwInt32 nXPos, RwInt32 nYPos);

	void				OnClickedBtnClose(gui::CComponent* pComponent);
	void				OnClickedBtnSave(gui::CComponent* pComponent);
	void				OnClickedBtnMove(gui::CComponent* pComponent);
	void				OnClickedBtnDelete(gui::CComponent* pComponent);

	void				OnClickedBtnPanel(gui::CComponent* pComponent);

	void				OnScrollChanged(int nNewOffset);
	void				OnCaptureWheelMove(RwInt32 nFlag, RwInt16 sDelta, CPos& pos);

protected:

	//!Variables
	gui::CStaticBox*	m_pstbTitle;

	gui::CDialog*		m_pdlgClippingRect;
	gui::CDialog*		m_pdlgContentsScrolled;

	gui::CButton*		m_pbtnClose;
	gui::CButton*		m_pbtnSave;
	gui::CButton*		m_pbtnMove;
	gui::CButton*		m_pbtnDelete;

	gui::CScrollBar*	m_pScrollBar;

	sBUTTONS			m_pButtons[NTL_QUICK_PORTAL_MAX_COUNT];

	// !Slots
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotMove;

	gui::CSlot          m_slotCloseBtnClick;
	gui::CSlot          m_slotSaveBtnClick;
	gui::CSlot          m_slotMoveBtnClick;
	gui::CSlot          m_slotDeleteBtnClick;

	gui::CSlot			m_slotScrollChanged;
	gui::CSlot			m_slotScrollSliderMoved;
	gui::CSlot			m_slotMouseWheel;


	//
	int					m_nScrollPos;

private:

	void				SetItemHandle(HOBJECT hHandle);

private:

	HOBJECT				m_hItemHandle;
	BYTE				m_bySelectedSlot;

	bool				m_bLoaded;
};

//#endif