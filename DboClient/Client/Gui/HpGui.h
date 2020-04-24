/*****************************************************************************
 *
 * File			: HpGui.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 9. 20	
 * Abstract		: DBO HP/MP/CP gui.
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __HP_GUI_H__
#define __HP_GUI_H__


#include "GUISoundDefine.h"
#include "NumberGui.h"
#include "InfoWndManager.h"
//#include "GaugeAnimation.h"

class CBuffDispGui;

class CHpGui : public CNtlPLGui, public RWS::CEventHandler
{
private:
	enum TYPE_RPCHARGE { TYPE_NONE, TYPE_INCREASE, TYPE_DECREASE, TYPE_DECREASE_END };
	enum TYPE_BATTLEATTR { TYPE_WEAPON, TYPE_ARMOR, TYPECOUNT };
	enum TYPE_AIR_COLOR { TYPE_DISABLE, TYPE_BLUE, TYPE_YELLOW, TYPE_RED };
	
	gui::CProgressBar*	m_ppgbHp;
	gui::CProgressBar*	m_ppgbEp;
	gui::CProgressBar*	m_ppgbRp;
	gui::CProgressBar*	m_ppgbRpBall;
	gui::CStaticBox*	m_sttLevel;
	gui::CStaticBox*	m_sttHp;
	gui::CStaticBox*	m_sttMp;	
	gui::CPanel*		m_ppnlRpBack;
	gui::CPanel*		m_ppnlRpBallBack;
	gui::CButton*		m_pbtnAvatar;

	gui::CPanel*		m_ppnlAggro;
	gui::CStaticBox*	m_psttTargetAggroPoint;

	gui::CPanel*		m_ppnlBattleCombating;
	gui::CPanel*		m_ppnlBattleCombatingToolTip;

	gui::CPanel*		m_ppnlAirPoint;
	gui::CStaticBox*	m_psttAirHeight;
	gui::CStaticBox*	m_psttAirPoint;

	gui::CPanel*		m_pPn_HpScouterBg;
	gui::CButton*		m_pBtn_HpScouterOption;

	
	gui::CButton*		m_pbtnPost;
	
	gui::CStaticBox *m_psttNickName;

	CFilteredSurfaceGui m_surCharacter;
	gui::CTexture		m_texCharacter;

	// Gauge Animation Data
	//CGaugeAnimation		m_LPGaugeAnimation;
	//CGaugeAnimation		m_EPGagueAnimation;
	
	// Power // disable by daneos
	/*gui::CPanel*		m_ppnlPowerText;
	CNumberGui			m_numPower;*/

	// Emergency Effect
	gui::CFlickerEffect	m_feEmergency;
	SOUND_HANDLE		m_hSound;

	// MailSystem Effect
	gui::CFlickerEffect	m_feNewMail;
	gui::CFlickerEffect	m_feMailFull;

	// Battle combating effect
	gui::CFlickerEffect	m_feBattleCombat;

	
	// RP Gauge
	RwReal	m_fCurrentRPChargingTime;
	RwReal	m_fRPChargingTime;
	RwReal	m_fDestValue;
	RwReal	m_fCurrentValue;
	RwInt32	m_nMaxValue;	

	TYPE_RPCHARGE m_eRPChargeType;	

	// Buff
	CBuffDispGui*	m_pBuff;

	bool			m_bIsWorldAirPossible; // check if current world allows flying

	TYPE_AIR_COLOR	m_eAirColor;
	CSurfaceGui		m_surMidAir;
	CSurfaceGui		m_surRoundAir;
	
public:

	CHpGui();
	CHpGui(const RwChar *pName);
	~CHpGui();

	VOID	Init(VOID);
	RwBool	Create(void);
	void	Destroy(void);

	void Update( RwReal fElapsed );
	void UpdateBeforeCamera(RwReal fElapsed);

	RwInt32 SwitchDialog(bool bOpen);

	RwBool IsMouseOnAvatar(VOID);	

	gui::CProgressBar* GetRpGauge(VOID) { return m_ppgbRp; }

private:
	virtual void HandleEvents(RWS::CMsg &pMsg);
//! Implement
	VOID	SetRPValue( RwInt32 nValue, RwInt32 nMaxValue );
	VOID	SetMaxRPBall( RwInt32 nMaxRPBall );

	void	UpdateAir();
	void	CheckAir();
	void	EnableAir(bool bFlag);
	void	SetAP(int nAP, int nMaxAP);
	void	CalculateAirHeight();

	VOID	SetEffectRunable(gui::CFlickerEffect& flickerEffect, RwBool bRun);

	gui::CSlot	m_slotPaint;
	gui::CSlot	m_slotPaintLast;
	gui::CSlot	m_slotMove;
	gui::CSlot	m_sloTracking;
	gui::CSlot	m_slogPaintPost;
	gui::CSlot	m_slotClickPost;
//	gui::CSlot	m_slotPowerPaint;
//	gui::CSlot	m_slotPowerMove;
	gui::CSlot	m_slotAvatarFocused;
	gui::CSlot	m_slotAvatarClick;
	
	gui::CSlot	m_slotPaintRPBall;
	gui::CSlot	m_slotMoveRPBall;
			
	VOID	OnPaint(VOID);
	VOID	OnPaintLast(VOID);
	VOID	OnMove(RwInt32 nX, RwInt32 nY);	
	VOID	OnResize(RwInt32 nX, RwInt32 nY);
	VOID	OnPaintPost();
	VOID	OnClickPost( gui::CComponent* pComponent );		
	//VOID	OnPowerPaint(VOID);
	//VOID	OnPowerMove( RwInt32 nOldX, RwInt32 nOldY );
	VOID	OnAvatarFocused( gui::CComponent* pComponent, bool bFocused );
	VOID	OnAvatarClick( gui::CComponent* pComponent );	

};

#endif