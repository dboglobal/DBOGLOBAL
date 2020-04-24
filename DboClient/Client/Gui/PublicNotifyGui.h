////////////////////////////////////////////////////////////////////////////////
// File: PublicNotifyGui.h
// Desc: 별 기능이 없기 때문에 일단 Public과 Action Notify를 혼용.
//
// 2007.02.14 Peessi@hitel.net   
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __PUBLIC_NOTIFY_GUI_H__
#define __PUBLIC_NOTIFY_GUI_H__

#include "ceventhandler.h"
#include "NtlPLGui.h"

class CPublicNotifyGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	//! Enum 
	enum STATE { NONE, FADE_IN, SHOW, FADE_OUT };

	//! Constructor & Desturctor
	CPublicNotifyGui(VOID);
	CPublicNotifyGui( const RwChar* pName );
	virtual ~CPublicNotifyGui(VOID);

	//! Operation
	VOID	Init(VOID);
	RwBool	Create(VOID);
	VOID	Destroy(VOID);
	VOID	Update( RwReal fElapsed );

	//! Event
	VOID	HandleEvents( RWS::CMsg& msg );

private:
	//! Implementation
	VOID	PublicUpdateProc( RwReal fElapsed );

	VOID	PublicUpdate_None(VOID);
	VOID	PublicUpdate_FadeIn(VOID);
	VOID	PublicUpdate_Show(VOID);
	VOID	PublicUpdate_FadeOut(VOID);
	VOID	SetPublicUpdateState( STATE eState );	

	VOID	PositionAlign( RwInt32 nScreenWidth, RwInt32 nScreenHeight );

	//! Variables
	gui::CStaticBox*	m_pstbPublicNotifyText;
	
	RwReal		m_fPublicCurrentTime;

	RwBool		m_bPublicNotify;

	RwInt32		m_ePublicState;

	VOID	(CPublicNotifyGui::*StatePublicUpdate)();
};

#endif//__PUBLIC_NOTIFY_GUI_H__

