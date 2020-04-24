/******************************************************************************
* File			: OptionBase.h
* Author		: Cho Hae sung
* Copyright		: (주)NTL
* Date			: 2007. 10. 15 
* Abstract		: 
*****************************************************************************
* Desc			: Option Base
*****************************************************************************/

#ifndef _OPTIONBASE_H_
#define _OPTIONBASE_H_

#pragma once

class COptionWindowGui;

/**
* \ingroup Client
* OptionWindow 각 Tab들의 Base
*/
class COptionBase
{
protected:
	typedef std::vector<LPCWSTR>			STRVEC;
	typedef std::vector<gui::CComponent*>	COMPONENTVEC;
	typedef std::vector<COMPONENTVEC>		COMPONENTDVEC;

	struct SOnOff
	{
		RwBool			bValue;
		gui::CButton*	pOnComponent;
		gui::CButton*	pOffComponent;
		gui::CSlot		slotOnComponent;
		gui::CSlot		slotOffComponent;
	};

	typedef std::map<gui::CComponent*, SOnOff*>	ONOFFMAP;

	COptionWindowGui*	m_pOptionWindow;
	COMPONENTDVEC		m_vecComponent;
	STRVEC				m_vecOptionName;
	STRVEC				m_vecToolTipName;
	ONOFFMAP			m_mapOnOff;
	RwInt32				m_iNumOption;

	gui::CScrollBar*	m_pScrollBar;
	RwInt32				m_iNumVisible;

public:
	COptionBase();
	virtual ~COptionBase();

	virtual RwBool		Create(COptionWindowGui* pOptionWindow);
	virtual void		Destroy();

	virtual void		Show(RwBool bShow);

	virtual void		Show();
	virtual void		Hide();
	virtual void		Update();

	virtual void		OnInit()	= 0; // 컨트롤 생성시 설정값
	virtual void		OnReset()	= 0; // 컨트롤을 기본값으로 변경. 시스템 값은 변경 되지 않는다.
	virtual void		OnOk()		= 0; // 컨트롤로 지정된 값으로 시스템 값 변경
	virtual void		OnCancel()	= 0; // 현재의 시스템값으로  컨트롤 값 다시 설정
	virtual void		OnHandleEvents(RWS::CMsg &pMsg)	= 0;

	gui::CComponent*	GetComponent(const std::string& str);
	void				SetComponentPos(RwInt32 iPos, gui::CComponent* pCompnent);
	void				SetScrollBarRange(RwInt32 iMin, RwInt32 iMax)				{ m_pScrollBar->SetRange(iMin, iMax); }
	void				SetScrollBarValue(RwInt32 iValue)							{ m_pScrollBar->SetValue(iValue, false); }
	RwInt32				GetScrollBarValue()											{ return m_pScrollBar->GetValue(); }

	void				SetComponentGroup(RwInt32 nIndex, LPCWSTR pName, ...);
	void				SetComponentGroup(RwInt32 nIndex, LPCWSTR pName, LPCWSTR pToolTip, ...);
	void				SetComponentOnOff(SOnOff* pOnOff, gui::CButton* pOnComponent, gui::CButton* pOffComponent, RwBool bOn = true);
	void				SetOnOff(SOnOff* pOnOff, RwBool bOn);

	void				OnOnOffToggled(gui::CComponent* pComponent, bool bToggled);
	void				OnListToggled(BOOL bToggled, gui::CComponent* pComponent);
};

#else

/**
* \brief 옵션에 콤보박스 항목
*/
struct sOptionCombo
{
	sOptionCombo(gui::CComponent* pParentGui, CRectangle& rectTitle, CRectangle& rectCombo, 
		RwInt32 nMarginX = 0, RwInt32 nMarginY = 0, 
		RwInt32 nButtonWidth = 22, RwInt32 nButtonHeight = 17, 
		RwInt32 nListHeight = 71, RwInt32 nVisibleItem = 3, 
		RwInt32 nSliderWidth = 13, RwInt32 nSliderHeight = 39, 
		RwInt32 nFontHeight = 90)
	{
		// StaticBox
		pSttTitle = NTL_NEW gui::CStaticBox( &rectTitle, pParentGui, GetNtlGuiManager()->GetSurfaceManager() );
		pSttTitle->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "OptionWnd.srf", "srfStaticBGMiddle" );
		
		// ComboBox
		pCbbOption = NTL_NEW gui::CComboBox( &rectCombo, pParentGui, GetNtlGuiManager()->GetSurfaceManager(),
			nMarginX, nMarginY,
			nButtonWidth, nButtonHeight,
			nListHei
			nVisibleItem );
		pCbbOption->SetTextFont( DEFAULT_FONT, nFontHeight, 0 );
		pCbbOption->SetListFont( DEFAULT_FONT, nFontHeight, 0 );
		pCbbOption->SetTextStyle( COMP_TEXT_CENTER );
		pCbbOption->SetListTextStyle( COMP_TEXT_CENTER );
		pCbbOption->AddStaticBoxSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "OptionWnd.srf", ""srfcbbStatic"" ) );
		pCbbOption->AddLIstBoxSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "OptionWnd.srf", "srfCbbCompareList3" ) );
		pCbbOption->AddListBoxMaskSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "OptionWnd.srf", "srfcbbListboxMask" ) );
		pCbbOption->AddSurfaceButtonUp( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "OptionWnd.srf", "srfcbbButtonUp" ) );
		pCbbOption->AddSurfaceButtonFocus( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "OptionWnd.srf", "srfcbbButtonFocus" ) );
		pCbbOption->AddSurfaceButtonDown( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "OptionWnd.srf", "srfcbbButtonDown" ) );
		pCbbOption->AddSurfaceButtonDisabled( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "OptionWnd.srf", "srfcbbButtonUp" ) );
		pCbbOption->GetListBox()->GetScrollBar()->AddSurfaceSlider( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "OptionWnd.srf", "srfcbbScrollSlider5" ) );
		pCbbOption->GetListBox()->GetScrollBar()->AddSurfaceLayout( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "OptionWnd.srf", "srfScrollLayout" ) );
		pCbbOption->GetListBox()->GetScrollBar()->SetSliderSize( nSliderWidth, nSliderHeight );
	}
	~sOptionCombo()
	{
		NTL_DELETE( pSttTitle );
		NTL_DELETE( pCbbOption );
	}

	VOID			SetTitle(const WCHAR* pwcText);
	
	RwUInt32		GetIdx();
	VOID			SetIdx(RwUInt32 uiIndex);

	VOID			AddItem(const WCHAR* pwcText);
	VOID			DelItem(RwUInt32 uiIndex);

	RwUInt32		GetItemCount();

	gui::CStaticBox*	pSttTitle;
	gui::CComboBox*		pCbbOption;
	gui::CSlot			slotToggleList;
	gui::CSlot			slotClickButton;
	gui::CSlot			slotSelectItem;
};

/**
* \ingroup client
*/
class COptionBase
{
public:
	COptionBase();
	virtual COptionBase();

	virtual RwBool	Create(COptionWindowGui* pOptionWindow);
	virtual void	Destroy();

	virtual void	Show(RwBool bShow = TRUE);

	virtual void	Update();

	virtual void	OnCreate()	= 0;
	virtual void	OnInit()	= 0;
	virtual void	OnReset()	= 0;
	virtual void	OnApply()	= 0;
	virtual void	OnCancel()	= 0;
};

#endif//OPTIONREFACTOR