/*****************************************************************************
*
* File			: PetStatusBarGui.h
* Author		: agebreak
* Copyright	: (주)NTL
* Date			: 2007. 8. 16	
* Abstract		: DBO Pet HP/MP gui.
*****************************************************************************
* Desc         : Summon Pet의 StatusBar GUI를 표시하는 클래스
*
*****************************************************************************/

#pragma once

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "SurfaceGui.h"
#include "NumberGui.h"

//sound
#include "GUISoundDefine.h"

class CBuffDispGui;

class CPetStatusBarGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CPetStatusBarGui();
	CPetStatusBarGui(const RwChar* pName);
	~CPetStatusBarGui();

	virtual void	Init();
	virtual RwBool	Create();
	virtual void	Destroy();
	virtual void	Update(RwReal fElapsed);
	virtual void	UpdateBeforeCamera(RwReal fElapsed);
	virtual RwInt32	SwitchDialog(bool bOpen);
    virtual void    HandleEvents(RWS::CMsg &pMsg);

	static CPetStatusBarGui* GetInstance();
	static void				 DeleteInstance();

	void			SetPetID(RwUInt32 uiSerialId);			///< 소환한 Pet의 ID를 설정한다.
	RwUInt32		GetPetID();

protected:
	VOID	OnMove( RwInt32 iOldX, RwInt32 iOldY );
	VOID	OnPaint(VOID);
	VOID	OnPaintLast(VOID);
   /* VOID    OnPaintPower(VOID);
    VOID    OnMovePower( RwInt32 nOldX, RwInt32 nOldY );*/
    VOID	OnClickInfoBtn(gui::CComponent* pComponent);
    VOID	OnCaptureMouseDown(const CKey& key);
	VOID	OnCaptureMouseUp(const CKey& key);
    
	void	UpdatePetInfo();						///< Pet의 정보를 UI에 업데이트한다.
	//void	UpdatePetPowerValue();					///< Pet의 전투력을 UI에 업데이트한다.
    void    ToggleInfo();                           ///< 상세 정보창을 토글한다	

protected:
	static CPetStatusBarGui* m_pInstance;			///< 싱글톤 객체
    
	gui::CProgressBar*	m_ppgbHp;
	gui::CProgressBar*	m_ppgbEp;		
	gui::CStaticBox*	m_sttHp;
	gui::CStaticBox*	m_sttMp;	
	gui::CButton*		m_pbtnPetInfo;	
	gui::CStaticBox*    m_psttNickName;

	// disable by daneos
    /*gui::CPanel*		m_ppnlPowerText;
    CNumberGui			m_numPower;*/

    gui::CPanel*        m_pPnlInfoBack;
    gui::CStaticBox*    m_pSttInfoLPLabel;
    gui::CStaticBox*    m_pSttInfoEPLabel;    
    gui::CStaticBox*    m_pSttInfoHRLabel;
    gui::CStaticBox*    m_pSttInfoDRLabel;
    gui::CStaticBox*    m_pSttInfoPOLabel;
    gui::CStaticBox*    m_pSttInfoPDLabel;    
    gui::CStaticBox*    m_pSttInfoEDLabel;
    gui::CStaticBox*    m_pSttInfoLP;
    gui::CStaticBox*    m_pSttInfoEP;    
    gui::CStaticBox*    m_pSttInfoHR;           // 공격 성공율
    gui::CStaticBox*    m_pSttInfoDR;           // 회피율
    gui::CStaticBox*    m_pSttInfoPO;           // 공격력
    gui::CStaticBox*    m_pSttInfoPD;           // 타격 방어력    
    gui::CStaticBox*    m_pSttInfoED;           // 기공 방어력

	/*CFilteredSurfaceGui m_surCharacter;
	gui::CTexture		m_texCharacter;*/
	CSurfaceGui			m_surCharacter;			///< Pet Illust -by Kell

	gui::CSlot			m_slotMove;
	gui::CSlot	        m_slotPaint;	
    gui::CSlot          m_slotPaintLast;
    gui::CSlot          m_slotInfoBtnClick;
    gui::CSlot          m_slotCaptureMouseDown;
	gui::CSlot			m_slotCaptureMouseUp;
    /*gui::CSlot          m_slotPowerPaint;
    gui::CSlot          m_slotPowerMove;*/

	// Emergency Effect
	gui::CFlickerEffect	m_feEmergency;

	// RP Gauge
	RwReal	m_fDestValue;
	RwReal	m_fCurrentValue;
	RwInt32	m_nMaxValue;

	// Buff
	CBuffDispGui*	m_pBuff;

	RwBool			m_bDelete;
	RwUInt32		m_uiPetID;
    bool            m_bShowInfo;                ///< 정보 윈도우의 표시 여부
};