#pragma once

#include "GuiLineTree.h"

class CWindowList;

class CGUI_test : public CNtlPLGui, public RWS::CEventHandler
{
protected:
	
	gui::CDialog*		m_pDigMain;
	gui::CDialog*		m_pDigStatic;
	gui::CDialog*		m_pDigEdit;

	gui::CStaticBox*	m_pStbTitle;

	gui::CStaticBox*	m_pStbStatic;
	gui::CStaticBox*	m_pStbEdit;

	gui::CButton*		m_pBtnStatic;
	gui::CButton*		m_pBtnEdit;
	gui::CButton*		m_pBtnClose;

	gui::CButton*		m_pBtnDlgStatic;
	gui::CButton*		m_pBtnDlgEdit;

	gui::CInputBox*		m_pInput;

	gui::CScrollBar*	m_pScroll;
	CGUI_test*			m_pGuiTest;


	gui::CSlot          m_slotScroll;
	gui::CSlot          m_slotScrollMove;

	gui::CSlot			m_slotClickedBtnClose;
	gui::CSlot			m_slotClickedBtnStatic;
	gui::CSlot			m_slotClickedBtnEdit;

	gui::CSlot			m_slotInput;

	bool				m_bButCheck;
	bool				m_bdlgEdit;
	bool				m_bdlgStatic;
public:
	CGUI_test( const RwChar* pName );
	virtual ~CGUI_test();

	RwBool	Create( );
	void	Destroy( );

	// event
	void	HandleEvents( RWS::CMsg& msg );
	RwInt32	SwitchDialog(bool bOpen);
	// btn
	void	OnclickedBtnStatic( gui::CComponent* pComponent );
	void	OnclickedBtnEdit( gui::CComponent* pComponent );
	void	OnClickedBtnClose( gui::CComponent* pComponent );

	void	OnClickedBtnDlgEdit( gui::CComponent* pComponent );
	void	OnClickedBtnDlgStatic( gui::CComponent* pComponent );

	VOID	OnScrollChanged( RwInt32 nNewOffset ) ;

	void	OnEditInput();
};