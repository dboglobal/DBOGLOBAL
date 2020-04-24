////////////////////////////////////////////////////////////////////////////////
// Name: class gui:CComboBox
// Desc: 일반적인 ComboBox. StaticBox, Button, ListBox의 연계로 이루어짐.
//
//			 
// 2006.03.13 Peessi@hitel.net   
//
// To Be Desired: 
//	1. 인터페이스 간결화. 쓰는 기능만 남겨놓고 삭제.
//	2. ScrollBar크기 및 Slider도 변하도록.
////////////////////////////////////////////////////////////////////////////////

#ifndef __GUI_COMBOBOX_H__
#define __GUI_COMBOBOX_H__

START_GUI

#define COMBOBOX_DEFAULT_BUTTON_WSIZE	16
#define COMBOBOX_DEFAULT_VISIBLEITEM	5
#define COMBOBOX_DEFAULT_LIST_HSIZE		100

class CComboBox_Generic;

class CComboBox : public CComponent
{
public:
//! Constructor:
	CComboBox( CComponent *pParent,CSurfaceManager *pSurfaceManager );
	// rtRect는 ListBox를 제외한 크기.
	CComboBox( const CRectangle &rtRect, CComponent *pParent, CSurfaceManager *pSurfaceManager,
			   INT nMarginX = 0, INT nMarginY = 0, INT nButtonWidth = COMBOBOX_DEFAULT_BUTTON_WSIZE, INT nButtonHeight = COMBOBOX_DEFAULT_BUTTON_WSIZE,
			   INT nListHeight = COMBOBOX_DEFAULT_LIST_HSIZE, INT nMaxVisibleItems = COMBOBOX_DEFAULT_VISIBLEITEM );
//! Destructor:
	virtual ~CComboBox();

//! Attributes:
	std::wstring	GetSelectedItemText(VOID) const;
	DWORD			GetSelectedItemData(VOID) const;
	INT				GetSelectedItemIndex(VOID) const;
	DWORD			GetItemData( INT nIndex ) const;
	std::wstring	GetItemText( INT nIndex ) const;
	bool			IsSelected( INT nIndex ) const;
	
	std::list<CSurface>* GetSurface(VOID) const;
	std::list<CSurface>* GetStaticBoxSurface(VOID) const;
	std::list<CSurface>* GetListBoxSurface(VOID) const;
	std::list<CSurface>* GetSurfaceUp(VOID) const;
	std::list<CSurface>* GetSurfaceDown(VOID) const;
	std::list<CSurface>* GetSurfaceFocus(VOID) const;
	std::list<CSurface>* GetSurfaceDisable(VOID) const;
	
	CButton*		GetButton(VOID) const;
	CListBox*		GetListBox(VOID) const;
	CStaticBox*		GetStaticBox(VOID) const;

	VOID			ToggleList(VOID);

//! Operations:
	VOID SetListFont( CHAR* pFaceName = DEFAULT_FONT, INT nHeight = DEFAULT_FONT_SIZE, INT nAttributes = 0 );
	VOID SetTextFont( CHAR* pFaceName = DEFAULT_FONT, INT nHeight = DEFAULT_FONT_SIZE, INT nAttributes = 0 );

	VOID SetText( const CHAR* text );
	VOID SetText( const WCHAR* text );
	VOID ClearText(VOID);

	VOID SetTextColor( COLORREF color );
	VOID SetEffectColor( COLORREF color );
	VOID SetEffectMode( INT nMode );
	VOID SetEffectValue( INT nValue );
	VOID SetTextStyle( DWORD dwStyle );

	INT AddItem( const CHAR* text, DWORD dwData = 0 );
	INT AddItem( const WCHAR* text, DWORD dwData = 0 );
	INT InsertItem( const CHAR* text, INT nIndex = -1, DWORD dwData = 0 );
	INT InsertItem( const WCHAR* text, INT nIndex = -1, DWORD dwData = 0 );
	VOID RemoveItem( INT nIndex );
	VOID ChangeItem( const CHAR* text, INT nIndex, DWORD dwData );
	VOID ChangeItem( const WCHAR* text, INT nIndex, DWORD dwData );
	VOID ClearAllItems(VOID);

	VOID SelectItem( INT nIndex, bool bSelect = true );
	VOID SetItemHeight( INT nHeight );
	VOID SetMaxVisibleItems( INT nCount );
	VOID SetTopItem( INT nIndex );

	VOID SetSelectedMaskColor( BYTE uRed, BYTE uGreen, BYTE uBlue, BYTE uAlpha );
	VOID SetSelectedTextColor( COLORREF color );
	VOID SetListTextColor( COLORREF color );
	VOID SetListEffectColor( COLORREF color );
	VOID SetListTextStyle( DWORD dwStyle );
	VOID SetListEffectMode( INT nMode );
	VOID SetListEffectValue( INT nValue );
	
	VOID AddSurface( CSurface surface );
	VOID AddStaticBoxSurface( CSurface surface );
	VOID AddLIstBoxSurface( CSurface surface );
	VOID AddListBoxMaskSurface( CSurface& surface );
	
	VOID AddSurfaceButtonUp( CSurface Surface );
	VOID AddSurfaceButtonDown( CSurface Surface );
	VOID AddSurfaceButtonFocus( CSurface Surface );
	VOID AddSurfaceButtonDisabled( CSurface Surface );
		
//! Signals:
	CSignal_v1<INT>& SigSelected(VOID);
	CSignal_v2<BOOL,CComponent*>& SigListToggled(VOID);

private:
//! Implementation:
	VOID CalcSubRect( CRectangle* pStaticRect, CRectangle* pButtonRect, CRectangle* pListRect,
					  const CRectangle& rtCombo, INT nMarginX, INT nMarginY,
					  INT nButtonWidth, INT nButtonHeight, INT nListHeight );

//! CallBack:
	VOID OnSetOptions( const CComponentOptions& options );

	CSlot m_slotSetOption;

protected:
//! Variables:
	CComboBox_Generic* m_pImpl;
	
	CListBox*	m_pListBox;
	CStaticBox*	m_pStaticBox;
	CButton*	m_pButton;

	bool		m_bStaticBoxDisabled;

	CComboBox( CComboBox& copy ) : CComponent( NULL, NULL ) { return; }
	friend class CComboBox_Generic;
};

END_GUI

#endif//__GUI_COMBOBOX_H__