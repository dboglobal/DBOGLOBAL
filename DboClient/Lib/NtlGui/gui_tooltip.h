////////////////////////////////////////////////////////////////////////////////
// Name: class gui::CToolTip
// Desc: ToolTip. CDynamicOutline, CStaticBox로 이루어져 있음. 
//
//			 
// 2006.03.6 Peessi@hitel.net   
//
// To Be Desired: 
//	1. Popup 위치 변경. Component외부 영역으로 설정되어야 함.
//  2. Render Top 관련 설정. Renderer의 코드부터 수정해야함. 
//  3. GUIManager에서 화면크기를 가져오도록.
////////////////////////////////////////////////////////////////////////////////

#ifndef __GUI_TOOLTIP_H__
#define __GUI_TOOLTIP_H__

START_GUI

class CToolTip_Generic;

class CToolTip : public CComponent
{
public:
//! Constructors:
	CToolTip( CComponent* pParent, CSurfaceManager* pSurfaceManager, DWORD dwStyle = 0 );
	CToolTip( const CRectangle& rtPos, CComponent* pParent, CSurfaceManager* pSurfaceManager, DWORD dwStyle = 0 );

//! Destructor:
	virtual ~CToolTip(VOID);
	
public:
//! Attributes:
	BOOL IsOwnerComponent( CComponent* pComp );

//! Operations:

	// Override 
	VOID Show( bool bShow /* = true */ );								// Tooltip은 일반 Component와 로직이 다르다.
	
	// ToolTip Interface
	VOID SetMargin( INT nMarginX, INT nMarginY );						// ToolTip LT에서 StaticBoxLT 까지의 거리
	VOID SetMouseMargin( INT nMouseMarginX, INT nMouseMarginY );		// 마우스포인터에서 ToolTip LT까지의 거리

	// Static Box Interface
	VOID CreateFontStd( CHAR* pFaceName, INT nHeight, INT nAttributes );
	VOID SetTextStyle( DWORD dwStyle );
	VOID SetTextColor( COLORREF color );
	VOID SetBkColor( COLORREF color );
	VOID SetBkMode( INT nBkMode );
	VOID SetEffectMode( INT nMode );
	VOID SetEffectColor( COLORREF color );
	VOID SetEffectValue( INT nValue );
	VOID SetMaxSize( INT nCX, INT nCY );
	VOID SetMinSize( INT nCX, INT nCY );
		
	// DynamicOutline Interface
	VOID AddSurfaces( CSurface& surCenter, CSurface& surLTC, CSurface& surRTC, CSurface& surLBC, CSurface& surRBC,
					  CSurface& surLL, CSurface& surTL, CSurface& surRL, CSurface& surBL );
	VOID SetTextPosition(VOID);											// Dynamic Outline Surface가 다 입력된 후에 실행.

	VOID ShowToolTip( const CHAR* text, CComponent* pOwner, DWORD dwAlign = 0 );
	VOID ShowToolTip( const WCHAR* text,  CComponent* pOwner, DWORD dwAlign = 0 );
	VOID HideToolTip( CComponent* pOwner );
	
	VOID UpdateMousePt(VOID);

	VOID CheckOwnerCompHide(VOID);
		
//! Signals:

private:
//! Callbacks:
	VOID OnSetOption( const CComponentOptions& options );
	
	CSlot m_SlotSetOptions;
	
protected:
//! Variables:
	CStaticBox* m_pStaticBox;

	CToolTip_Generic* m_pImpl;

//!etc:
	CToolTip( const CToolTip& copy );	// prevent construction by copy construction
	friend class CToolTip_Generic;
};

END_GUI

#endif//__GUI_TOOLTIP_H__