/*****************************************************************************
 *
 * File			: ExpGui.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 11. 030	
 * Abstract		: DBO exp  gui.
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __EXPSLOT_GUI_H__
#define __EXPSLOT_GUI_H__

#include "NtlPLGui.h"
#include "ceventhandler.h"

#include "SurfaceGui.h"
#include "NumberGui.h"

class CExpEffect
{
public:
	CExpEffect(VOID);
	~CExpEffect(VOID);

	VOID	Create(VOID);
	VOID	Destroy(VOID);

	VOID	StartProc( RwInt32 nScreenXPos, RwInt32 nScreenYPos, RwUInt32 nValue, RwReal fDelayTime, RwUInt32 uiAcquisitionExp = 0, RwUInt32 uiBonusExp = 0 );
	VOID	EndProc(VOID);
	
	RwBool	Update( RwReal fElapsed );	
	VOID	Render(VOID);

	VOID	SetInitXPos( RwInt32 nScreenXPos );	// DestPos도 같이 수정됨.
	VOID	SetInitYPos( RwInt32 nScreenYPos );

	RwBool	IsEffectShow(VOID) { return m_bShowEffect; }

private:
	RwInt32		m_nInitYPos;
	RwInt32		m_nDestYPos;
	RwInt32		m_nNumXPos;
	RwInt32		m_nSurXPos;
	
	RwBool		m_bShowEffect;
	CSurfaceGui	m_surEXP;
	CNumberGui	m_numEXP;
	RwReal		m_fCurrentTime;		

	RwInt32		m_nInitWidth;
	RwInt32		m_nInitHeight;
	RwInt32		m_nDestWidth;
	RwInt32		m_nDestHeight;

	RwBool		m_bSound;

    // PC방용 추가 EXP 관련 정보
    RwUInt32    m_uiAcquisitionExp;
    RwUInt32    m_uiBonusExp;
};

class CExpGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
//! Contructor & Destructor
	CExpGui();
	CExpGui(const RwChar *pName);
	~CExpGui();

//! Opration
	RwBool	Create(VOID);
	VOID	Destroy(VOID);

	VOID	Update( RwReal fElapsed );

	RwInt32 SwitchDialog(bool bOpen);

private:
//! Implement
	VOID	SetExpInfoWnd(VOID);


//! Event
	VOID	HandleEvents( RWS::CMsg &pMsg );

//! Callback
	VOID	OnMouseEnter( gui::CComponent* pComponent );
	VOID	OnMouseLeave( gui::CComponent* pComponent );
	VOID	OnPaint(VOID);

	gui::CSlot	m_slotMouseEnter;
	gui::CSlot	m_slotMouseLeave;
	gui::CSlot	m_slotPaint;

//! Variable
	gui::CProgressBar*	m_ppgbExp;		

	std::list<CExpEffect*>	m_listExpEffect;
};

#endif