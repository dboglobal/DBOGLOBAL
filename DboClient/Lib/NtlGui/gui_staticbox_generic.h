#ifndef __GUI_STATICBOX_GENERIC_H__
#define __GUI_STATICBOX_GENERIC_H__

#include "gui_staticbox.h"
#include "gui_outputbox_item.h"
#include "gui_staticbox_item.h"

START_GUI

class CFont;

class CStaticBox_Generic
{
public:

	typedef std::list<CSurface>						LIST_SURFACE;
	typedef std::list<CSurface>::iterator			LIST_SURFACE_IT;
	typedef std::list<CStaticBox_Item*>				LIST_ITEM;
	typedef std::list<CStaticBox_Item*>::iterator	LIST_ITEM_IT;


	CStaticBox_Generic(CStaticBox *pSelf, DWORD dwStyle, BOOL bDynamic);
	~CStaticBox_Generic(VOID);

	CStaticBox*		m_pStaticBox;

	LIST_SURFACE	m_stlSurface;
	CSurface		m_TextSurface;

	CGuiFont*		m_pFont;
	DWORD			m_dwStyle;
	COLORREF		m_Color;
	INT				m_nBkMode;
	COLORREF		m_BkColor;	
	INT				m_nEffectMode;
	COLORREF		m_effectColor;
	INT				m_nEffectValue;
	std::string		m_strFont;
	INT				m_nFontH;
	INT				m_nFontAttribute;
	BOOL			m_bDynamic;			// Dynamically adjust window size.
	INT				m_nMaxCX;			// Dynamic시 최대 폭
	INT				m_nMaxCY;			// Dynamic시 최대 높이.
	INT				m_nMinCX;			// Dynamic시 최소 폭.
	INT				m_nMinCY;			// Dynamic시 최소 높이.
	INT				m_nTextureWidth;
	INT				m_nTextureHeight;
	INT				m_nMarginX;
	INT				m_nMarginY;
	INT				m_nLineSpace;		// Line.
	
	BYTE			m_ucAlpha;				// Alpha저장값.
	BYTE			m_ucTexturePosOffset;	// Shadow, Outline등으로 인해 발생되는 택스쳐 여분의 사이즈. 
	BYTE			m_ucTextureSizeOffset;

	LIST_ITEM		m_listItem;
	CUniBuffer		*m_pBuffer;

private:

	VOID AdjustRectToString(VOID);			
	VOID StringDetect(VOID);
	INT	 GetStartPosX( CUniBuffer* pBuffer );
	INT	 GetStartPosY( INT nTotLine, INT nCurrLine );
	VOID ItemClear(VOID);
	
	INT	 CalcCurrentHeight( INT nTotalLine );
	INT	 GetTextRegionWidth(VOID);
	INT	 GetTextRegionHeight(VOID);	

	inline VOID SetTextTexturePos( CRectangle& rect );

public:

	//-------------------------------------------------------------------------
	//: font Create
	VOID CreateFont( CHAR* pFaceName, INT nHeight, INT nAttributes = 0 );
	VOID CreateFont(VOID);

	VOID Clear(VOID);

	VOID SetText( const CHAR* text );		
	VOID SetText( const WCHAR* text );
	VOID AddText( const CHAR* text );		
	VOID AddText( const WCHAR* text );

	VOID SetMargin( INT nMarginX, INT nMarginY );	// Font생성후 Margin조정시. 생성전에는 자동으로 입력된다.
	VOID SetEffectMode( INT nEffectMode, BOOL bSurfaceChange = TRUE );
	VOID SetEffectValue( INT nEffectValue, BOOL bSurfaceChange = TRUE );

	VOID DrawItem(VOID);

	VOID Render(VOID);

	VOID SetTextSurface( CRectangle rcScreen );

	INT  GetFitWidthToString(VOID);					// 텍스트 자체의 길이 + Margin의 값으로 리사이즈.
	INT  GetFitHeightToString(VOID);

	VOID Redraw(VOID);

// callback function
private:

	VOID OnSetOptions( const CComponentOptions& options );
	VOID OnScrollChange( INT nValue );
	VOID OnMove( INT nOldX, INT nOldY );
	VOID OnResize( INT nOldCX, INT nOldCY );
	VOID OnSetAlpha( BYTE ucAlpha );
	VOID OnSetColor( BYTE ucRed, BYTE ucGreen, BYTE ucBlue );

	CSlot m_SlotSetOptions;
	CSlot m_SlotMove;
	CSlot m_SlotResize;
	CSlot m_SlotSetAlpha;
	CSlot m_SlotSetColor;
};

END_GUI

#endif
