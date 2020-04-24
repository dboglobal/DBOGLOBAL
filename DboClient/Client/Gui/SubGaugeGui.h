////////////////////////////////////////////////////////////////////////////////
// File: SubGaugeGui.h
// Desc: RP Gauge, Cast Gauge
//
// 2006.9.9 Peessi@hitel.net   
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __SUB_GAUGE_GUI_H__
#define __SUB_GAUGE_GUI_H__

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "NtlSLDef.h"
#include "NtlSoundDefines.h"
#include "SurfaceGui.h"

class CNtlSobActor;

class CCastGauge
{
public:
	CCastGauge( gui::CComponent* pParent, CNtlSobActor* pActor );
	~CCastGauge(void);

	VOID	Update( RwReal fElapsedTime );
		
private:
	VOID	PositionUpdate(VOID);

	gui::CProgressBar*	m_pCastGauge;
	gui::CPanel*		m_pCastBack;

	CNtlSobActor*		m_pActor;
};

class CSubGaugeGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	// typedef
	typedef std::map<SERIAL_HANDLE,CCastGauge*>				MAP_CASTGAUGE;
	typedef std::map<SERIAL_HANDLE,CCastGauge*>::iterator	MAP_CASTGAUGE_ITER;
	
	enum STATE_GUARDGAUGE { STATE_NONE, STATE_START, STATE_RUN };

	//! Constructor & Destructor
	CSubGaugeGui( const RwChar* pName );
	~CSubGaugeGui(VOID);

	//! Operatrion
	RwBool	Create(VOID);
	VOID	Destroy(VOID);

	VOID	Update( RwReal fElapsed );

	//! Event
	VOID	HandleEvents( RWS::CMsg& msg );
	
private:
	//! Implementation
	VOID	CheckRPGaugeState( RwInt32 nCurrentRPBall );
	VOID	PositionUpdate(VOID);
	VOID	GuardPositionUpdate(VOID);	

	VOID	SetGuardValue( RwInt32 nValue,RwInt32 nMaxValue );
	VOID	SetGuardMark( const CHAR* pSurfaceName );
	VOID	SetGuardState( STATE_GUARDGAUGE eType );
	VOID	GuardGaugeUpdate( RwReal fElapsed );

	//! CallBack
	VOID	OnPaintGuard(VOID);
	VOID	OnMoveGaurd( RwInt32 nX, RwInt32 nY );

	gui::CSlot	m_slotOnPaintGuard;
	gui::CSlot	m_slotOnMoveGuard;
				
	//! Variables
	//! RpGauge
	gui::CProgressBar*	m_pRPGauge;
	gui::CPanel*		m_pRPBack;

	RwBool				m_bRPShow;
	RwBool				m_bRPBallMax;

	//! GuardGauge
	CSurfaceGui			m_surGuardGauge;
	gui::CPanel*		m_ppnlGuardMark;
	
	RwReal				m_fGuardCoolTime;
	RwReal				m_fMaxGuardCoolTime;
	
	RwBool				m_bGuardShow;

	SOUND_HANDLE		m_hSound;
	//! CastGauge
	MAP_CASTGAUGE		m_mapCastGauge;

public:
	static	STATE_GUARDGAUGE	m_eGuardGaugeState;
};

#endif