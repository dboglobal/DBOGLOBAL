////////////////////////////////////////////////////////////////////////////////
// Name: class gui:CTabButton
// Desc: Tab형식의 버튼 관리. 임의의 수의 Button을 포함, 관리함.
//
//
// 2006.03.28 Peessi@hitel.net   
// 2007.04.04 Peessi@hitel.net 
//		- 동적 크기변환 갯수 변환 가능.
// 2008.03.03 Peessi@hitel.net
//		- 탭버튼 최대폭 이상 늘어나지 않음. 비선택사이즈 * 갯수가 탭버튼 전체폭 초과시 Assert
//      - 탭버튼 사이즈 부족으로 미표시인 경우 자동으로 툴팁을 띄운다. 
//
// Use : 1. SetTabSurface를 사용, Surface를 등록하기 전에 Add한 경우 꼭 Refresh할 것.
////////////////////////////////////////////////////////////////////////////////

#ifndef __GUI_TABBUTTON_H__
#define __GUI_TABBUTTON_H__

START_GUI

class CTabButton_Generic;

class CTabButton : public CComponent
{
public:
//! Constructors:
	CTabButton( CComponent* pParent, CSurfaceManager* pSurfaceManager );
	CTabButton( const CRectangle& rtRect, CComponent* pParent, CSurfaceManager* pSurfaceManager );
//! Destructor:
	virtual ~CTabButton(VOID);

public:
//! Attributes:
	INT		GetButtonCount(VOID) const;
	INT		GetSelectedButtonIndex(VOID) const;

	VOID	SetTabSurface( CSurface& left, CSurface& center, CSurface& right, 
						   CSurface& selectedleft, CSurface& selectedcenter, CSurface& selectedright,
						   CSurface& focusleft, CSurface& focuscenter, CSurface& focusright, CSurface& line );
						   
	////////////////////////////////////////////////////////////////////////////
	// 이 단락의 속성은 Tab을 Add하기 전에 세팅한다. Tab이 있으면 Clear후.
	// 그 이외의 경우는 적용 후 ApplyAttribute()함수를 호출한다. 
	VOID	SetTextColor( BYTE ucRed, BYTE ucGreen, BYTE ucBlue );
	VOID	SetTextSelectedColor( BYTE ucRed, BYTE ucGreen, BYTE ucBlue );
	VOID	SetTextFocusedColor( BYTE ucRed, BYTE ucGreen, BYTE ucBlue );
	VOID	SetTextDisabledColor( BYTE ucRed, BYTE ucGreen, BYTE ucBlue );
	VOID	SetTextAlpha( BYTE ucAlpha );
	VOID	SetTextSelectedAlpha( BYTE ucAlpha );
	VOID	SetTextFocusedAlpha( BYTE ucAlpha );
	VOID	SetTextDisabledAlpha( BYTE ucAlpha );
	VOID	SetFont( CHAR* pFaceName, INT nHeight, INT nAttributes );
	VOID	SetTextAlign( DWORD dwAlign );
	VOID	SetTabTextMargin( INT nMarginX, INT nMarginY );
	////////////////////////////////////////////////////////////////////////////
	// ApplyAttribute 호출시 이전의 탭선택은 유지된다.
	VOID	ApplyAttribute(VOID);

	VOID	SetUnselTabWidth( INT nWidth, BOOL bRedraw = TRUE );
	VOID	SetOrient( BOOL bLeft, BOOL bRedraw = TRUE );
		
	INT		AddTab( std::string& strTabText, BOOL bRedraw = TRUE );
	INT		AddTab( std::wstring& wstrTabText, BOOL bRedraw = TRUE );
	VOID	ClearTab(VOID);

	VOID	SetClickSound( const CHAR* szClickSoundFilename );
	VOID	SetFocusSound( const CHAR* szFocusSoundFIlename );
		
//! Operations:
	VOID	SelectTab( INT nIndex, BOOL bDoCallBack = FALSE );		
	
	VOID	EnableTab( INT nIndex, BOOL bEnable = TRUE );
	VOID	ChangeTabText( INT nIndex, std::string& strText );
	VOID	ChangeTabText( INT nIndex, std::wstring& wstrText );
		
//! Signals:
	CSignal_v1<INT>& SigSelected(VOID);
	CSignal_v2<INT,INT>& SigSelectChanged(VOID);

private:
				
protected:
//! Variables:
	CTabButton_Generic* m_pImpl;

private:
//! Etc:
	CTabButton( const CTabButton& copy ); // prevent construction by copy construction
	friend class CTabButton_Generic;
};

END_GUI

#endif//__GUI_TABBUTTON_H__
