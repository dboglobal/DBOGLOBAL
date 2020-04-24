#pragma once

// core
#include "ceventhandler.h"
#include "NtlSLEvent.h"

// presentation
#include "NtlPLGui.h"

// dbo
#include "SideIconGui.h"

/**
 * \ingroup Client
 * \brief HLS icon
 *
 * \date 12-sep-2018
 * \author daneos
 */
class CHLSSideIconGui : public CSideIconBase, public RWS::CEventHandler
{
public:
	CHLSSideIconGui(const RwChar* pName);
	virtual ~CHLSSideIconGui(void);

	RwBool			Create();
	VOID			Destroy();

	virtual VOID	OnIconButtonClicked(gui::CComponent* pComponent);
	virtual VOID	OnSideViewClosed();
	virtual void	Show(bool bShow);

protected:
	virtual VOID	HandleEvents(RWS::CMsg &msg);
	void            OnMouseEnter(gui::CComponent* pComponent);
	void            OnMouseLeave(gui::CComponent* pComponent);

protected:
	gui::CSlot      m_slotHLSBtn;
	gui::CSlot      m_slotHLSMouseEnter;
	gui::CSlot      m_slotHLSMouseLeave;
	gui::CButton*   m_pBtnHLS;
};
