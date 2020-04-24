////////////////////////////////////////////////////////////////////////////////
// File: SkillCustomizeGui.h
// Desc: SkillCustomize
//
// 2008.11.5 Peessi@hitel.net   
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __SKILL_CUSTOMIZE_GUI_H__
#define __SKILL_CUSTOMIZE_GUI_H__

#include "NtlPLGui.h"
#include "ceventhandler.h"

class CSkillCustomizeItemGroup;

class CSkillCustomizeGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	//! Enum
	enum TYPE { FIRST_CLASS, SECOND_CLASS_1, SECOND_CLASS_2, SOCIAL_ACTION, NUM_OF_CLASS };

	//! Constructor & Destructor
	CSkillCustomizeGui( const RwChar* pName );
	~CSkillCustomizeGui(VOID);

	//! Operation
	RwBool	Create(VOID);
	VOID	Destroy(VOID);

	VOID	Update( RwReal fElapsed );

	RwInt32 SwitchDialog( bool bOpen );

	VOID	UpdatePopoNotifySkillAlarm();	/// woosungs_test 20090730

	//! Event
	VOID	HandleEvents( RWS::CMsg& msg );

private:
	//! Implementation
	VOID	SetType( RwUInt8 byType );

	VOID	GenerateSkillItems(VOID);
	VOID	ClearSkillItems(VOID);

	VOID	SetInitSkill(VOID);
	VOID	SetSkillItem( RwInt32 nSlot );
	VOID	SetHTBItem( RwInt32 nSlot );
	VOID	SetActionSkillItem( RwInt32 nSlot );
	VOID	SetRPType( RwInt32 nSlot );
	VOID	SetSP(VOID);
	VOID	CheckUpgradebleSkill(VOID);
	VOID	CoolingEffectProc( RwUInt32 hSerial, RwBool bStart );	
	void	SelectEffectProc(RwUInt32 hSerial, bool bStart);
	
	//! Callback
	VOID	OnTabChanged( RwInt32 nIndex, RwInt32 nOldIndex );
	VOID	OnCloseButtonClicked( gui::CComponent* pComponent );
	VOID	OnScrollChanged( RwInt32 nNewOffset );
	VOID	OnHelpButtonClicked( gui::CComponent* pComponent );
	
	VOID	OnCaptureWheelMove( RwInt32 nFlag, RwInt16 sDelta, CPos& pos );
	VOID	OnCaptureMouseDown( const CKey& key );	

	gui::CSlot			m_slotTabChanged;
	gui::CSlot			m_slotCloseButtonClicked;
	gui::CSlot			m_slotScrollChanged;
	gui::CSlot			m_slotScrollSliderMoved;
	gui::CSlot			m_slotHelpButtonClicked;

	gui::CSlot			m_slotCaptureWheelMove;
	gui::CSlot			m_slotCaptureMouseDown;	

	//! Variables
	gui::CScrollBar*	m_pScrollBar;
	gui::CTabButton*	m_pTabBtn;
	gui::CButton*		m_pBtnClose;
	gui::CStaticBox*	m_pTitle;
	gui::CButton*		m_pBtnHelp;
	gui::CStaticBox*	m_pSP;
	gui::CDialog*		m_pSkillDlg;
	
	RwUInt8						m_byType;
	CSkillCustomizeItemGroup*	m_pSkillGroup[NUM_OF_CLASS];	
};

#endif