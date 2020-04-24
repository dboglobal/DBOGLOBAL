#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

// dbo
#include "Windowby3.h"
#include "SideIconGui.h"
#include "DBOEvent.h"

/**
 * \ingroup Client
 * \brief HLS Side View
 *
 * \date 12-sep-2018
 * \author daneos
 */
class CHLSSideViewGui : public CSideViewBase, public RWS::CEventHandler
{

public:
	CHLSSideViewGui(const RwChar* pName);
	virtual ~CHLSSideViewGui(void);

	RwBool		Create();
	VOID		Destroy();

	virtual VOID	OnPressESC();
	virtual VOID	OnSideViewOpen(const void* pData);
	virtual VOID	OnSideViewClose();
	virtual VOID	OnSideViewLocate(const CRectangle& rectSideIcon);

protected:

	virtual VOID	HandleEvents(RWS::CMsg &msg);
	VOID			LocateComponent();
	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID			OnResize(RwInt32 iOldW, RwInt32 iOldH);
	VOID			OnPaint();

protected:

	CWindowby3			m_BackPanel;		///< background

	gui::CStaticBox*    m_pstbTitle;

	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotResize;
};
