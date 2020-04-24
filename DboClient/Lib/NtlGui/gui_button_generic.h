#ifndef __GUI_BUTTON_GENERIC_H__
#define __GUI_BUTTON_GENERIC_H__

#include "gui_button.h"
#include "gui_texture.h"


START_GUI

class CButton_Generic
{
public:

	CButton_Generic(CButton *pSelf, bool bToggleMode);
	~CButton_Generic();

	CButton *m_pButton;

	VOID OnSetOptions(const CComponentOptions & options);
	VOID OnKeyDown(CComponent*, CInputDevice*, const CKey&);
	VOID OnKeyUp(CComponent*, CInputDevice*, const CKey&);
	VOID OnMouseLeave( CComponent* pComponent );
	VOID OnMouseDown(const CKey&);
	VOID OnMouseUp(const CKey&);
	VOID OnMouseMove( INT nKey, INT nX, INT nY );
	VOID OnPaint(VOID);
	VOID OnSetColor( BYTE byRed, BYTE byGreen, BYTE byBlue );
	VOID OnSetAlpha( BYTE ucAlpha );
		
	std::list<CSurface> m_stlSurfaceUp;
	std::list<CSurface> m_stlSurfaceDown;
	std::list<CSurface> m_stlSurfaceFocus;
	std::list<CSurface> m_stlSurfaceDisabled;
	std::list<CSurface>* m_pRenderSurface;
	CSurface m_surMask;

	bool m_bToggled;
	bool m_bToggleMode;
	BOOL m_bMouseEnter;
	BOOL m_bClickEnable;							// Component 자체의 Enable이 아닌 button만의 Enable 모든 메세지는 허용하나 단지 클릭만 되지 않는다. 
	BOOL m_bEnableMouseCapture;						// MouseDown시 MouseCapture를 할지 여부.
		
	COLORREF m_colorTextUpColor;					// 각 텍스트 색.
	COLORREF m_colorTextDownColor;
	COLORREF m_colorTextFocusColor;
	COLORREF m_colorTextDisableColor;
	BYTE	 m_ucTextUpAlpha;						// 각 텍스트 알파.
	BYTE	 m_ucTextDownAlpha;
	BYTE	 m_ucTextFocusAlpha;
	BYTE	 m_ucTextDisableAlpha;

	INT		m_nTextDownCoordDiffX;					// 버튼다운시 텍스트 변화.
	INT		m_nTextDownCoordDiffY;

	INT		m_nTextCoordX;							// 버튼 내 Text위치
	INT		m_nTextCoordY;							

	std::string	m_strFocusSound;					
	std::string m_strClickSound;				
	std::string m_strDisableSound;	

	CSignal_v2<CComponent*,bool> m_SigToggled;
	CSignal_v1<CComponent*> m_SigClicked;
	CSignal_v1<CComponent*> m_SigPressed;
	CSignal_v1<CComponent*> m_SigReleased;
	CSignal_v2<CComponent*,bool> m_SigFocused;
	
	CSlot m_SlotSetOptions;
	CSlot m_SlotKeyDown;
	CSlot m_SlotKeyUp;
	CSlot m_SlotMouseLeave;
	CSlot m_SlotMouseDown;
	CSlot m_SlotMouseUp;
	CSlot m_SlotMouseMove;
	CSlot m_SlotPaint;
	CSlot m_slotSetAlpha;
	CSlot m_slotSetColor;
	
public:

	RwImage *m_pMaskImage;

	// mask 그림에서 유효하지 않은 영역이가?
	bool PtInMask(int nX, int nY);
	BOOL PtInValidRegion( INT nX, INT nY );
	VOID CreateMaskImage(const char *pFileName);
	VOID SetRenderSurface(VOID);
	VOID SetMouseEnter( BOOL bMouseEnter );
};  

END_GUI

#endif
