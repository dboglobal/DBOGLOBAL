////////////////////////////////////////////////////////////////////////////////
// File: TerritoryNotifyGui.h
// Desc: 
//
// 2007.02.14 Peessi@hitel.net   
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __TERRITORY_NOTIFY_GUI_H__
#define __TERRITORY_NOTIFY_GUI_H__

#include "ceventhandler.h"
#include "NtlPLGui.h"

class CTerritoryNotifyGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	//! Enum
	enum STATE { NONE, FADE_IN, SHOW, FADE_OUT };

	//! Constructor & Desturctor
	CTerritoryNotifyGui(VOID);
	CTerritoryNotifyGui( const RwChar* pName );
	virtual ~CTerritoryNotifyGui(VOID);

	//! Operation
	VOID	Init(VOID);
	RwBool	Create(VOID);
	VOID	Destroy(VOID);
	VOID	Update( RwReal fElapsed );
	
	//! Event
	VOID	HandleEvents( RWS::CMsg& msg );

private:
	//! Implements
	VOID	Update_None(VOID);
	VOID	Update_FadeIn(VOID);
	VOID	Update_Show(VOID);
	VOID	Update_FadeOut(VOID);

	VOID	SetUpdateState( STATE eState );

	VOID	PositionAlign( RwInt32 nScreenWidth, RwInt32 nScreenHeight );

	//! Variables
	gui::CMDStaticBox* m_pmdbNotifyText;

	RwReal		m_fCurrentTime;
	RwInt32		m_eState;
	RwUInt32	m_uiNextMapIndex;

	std::wstring m_strOldZoneName;
	std::wstring m_strOldSectorName;

	VOID	(CTerritoryNotifyGui::*StateUpdate)();
};

#endif//__TERRITORY_NOTIFY_GUI_H__

