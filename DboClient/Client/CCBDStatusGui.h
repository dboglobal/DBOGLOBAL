#pragma once

#include "ceventhandler.h"
#include "NumberGui.h"

class CCCBDStatusGui : public CNtlPLGui, public RWS::CEventHandler
{

protected:

	static CCCBDStatusGui*	m_pInstance;

protected:

	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotPaint;

	gui::CStaticBox*	m_pstbDialogName;

	gui::CButton*		m_pbtnMinimize;
	gui::CSlot			m_slotClickedBtnMinimize;

	gui::CStaticBox*	m_pstbFloor;

	gui::CStaticBox*	m_pstbAimStatic;

	gui::CStaticBox*	m_pstbAim;

	gui::CStaticBox*	m_pstbLeftTimeStatic;

	CNumberGui			m_LeftTimeMinuteNumber;	
	CNumberGui			m_LeftTimeSecondNumber;
	CSurfaceGui			m_srfRemainSemiColon;

	gui::CPanel*		m_ppnlDisableItem;
	gui::CPanel*		m_ppnlDisableChargeLPEP;
	gui::CPanel*		m_ppnlDisableGetEXP;

private:

	bool				m_bCountDown;
	bool				m_bHasCountDown;
	float				m_fRemainTime;

public:

	CCCBDStatusGui(const RwChar* pName);
	~CCCBDStatusGui();

	static VOID		CreateInstance();
	static VOID		DestroyInstance();

	//! Operation
	void						Init();
	virtual RwBool				Create();
	virtual void				Destroy();

	RwInt32						SwitchDialog(bool bOpen);

	virtual void				HandleEvents(RWS::CMsg& msg);

	void						Update(float fElapsed);

protected:

	void						OnPaint();
	void						OnMove(RwInt32 iOldX, RwInt32 iOldY);

	void						OnClickedBtnMinimize(gui::CComponent* pComponent);

};