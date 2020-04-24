#ifndef __GUI_MDSTATICBOX_GENERIC_H__
#define __GUI_MDSTATICBOX_GENERIC_H__

#include "gui_mdstaticbox.h"
#include "gui_mdstaticbox_item.h"

START_GUI

class CMDStaticBox_Generic
{
public:
//! Construction & Destruction
	CMDStaticBox_Generic( CMDStaticBox *pSelf );
	~CMDStaticBox_Generic(VOID);

//! Attributes
	BOOL IsFontAvailable( DWORD dwIndex );

//! Operation
	VOID SetItem( const WCHAR* szText, const CHAR* szName, DWORD dwFontIndex, DWORD dwStyle, 
				  COLORREF colorText, COLORREF colorEffect, INT nEffectMode, INT nEffectValue,
				  INT nX, BOOL bAttachPrevLine );
	VOID SetItemXY( const WCHAR* szText, const CHAR* szName, DWORD dwFontIndex, INT nX, INT nY, DWORD dwStyle,
					COLORREF colorText, COLORREF colorEffect, INT nEffectMode, INT nEffectValue );
	VOID DeleteItem( const CHAR* szName );
	VOID ClearItem(VOID);
	VOID DrawText(VOID);
	VOID SetBlankLine( INT nCY = -1 );
	
	INT  AddFont( const CHAR* pFontName, INT nHeight, INT nAttributes );
	VOID AddSurface( CSurface surface );
private:
//! Implementation
	VOID CalcMaxWidthHeight( INT nCX, INT nCY, INT nY, BOOL bXY );
	VOID FixMaxWidthHeight( CMDStaticBox_Item* pItem );
	VOID DetectLineFeed( CMDStaticBox_Item* pItem, CMDStaticBox_Item* pPrevItem, BOOL bXY );
	std::list<CMDStaticBox_Item*>::iterator FindItem( std::string& strName, std::list<CMDStaticBox_Item*>& stdList );
	CMDStaticBox_Item* FindLastItem( std::list<CMDStaticBox_Item*>& stdList );
	VOID CreateTexture(VOID);
	VOID RecalcPosition(VOID);
	VOID DrawItem( CMDStaticBox_Item* pItem, CMDStaticBox_Item* pPrevItem );
	VOID DrawXYItem( CMDStaticBox_Item* pItem, CMDStaticBox_Item* pPrevChildItem );

	VOID SetTextTexturePos( CRectangle& rect );
		
//! Callback
	VOID OnSetOptions( const CComponentOptions& options );
	VOID OnMove( INT nX, INT nY );
	VOID OnPaint(VOID);
	VOID OnSetAlpha( RwUInt8 ucAlpha );
	VOID OnSetColor( RwUInt8 ucRed, RwUInt8 ucGreen, RwUInt8 ucBlue );

	CSlot m_SlotSetOptions;
	CSlot m_SlotMove;
	CSlot m_SlotPaint;
	CSlot m_SlotSetAlpha;
	CSlot m_SlotSetColor;

public:
//! Variables
	DWORD			m_dwStyle;		// Default Value, left or center or right
	COLORREF		m_colorText;    // Default Value, text color
	COLORREF		m_colorBK;		// Default Value, background text color
	INT				m_nBkMode;		// Default Value, transparent text
	COLORREF		m_colorEffect;	// Default Value
	INT				m_nEffectMode;	// Default Value
	INT				m_nEffectValue;	// Default Value

	INT				m_nMinCX;
	INT				m_nMinCY;
	INT				m_nMarginX;
	INT				m_nMarginY;
	INT				m_nInterval;
	DWORD			m_dwDefaultFont;

	INT				m_nCursorX;		// ´ÙÀ½¿¡ ÂïÈú ¼ö ÀÖ´Â ÁÂÇ¥.
	INT				m_nCursorY;		
	INT				m_nMaxWidth;
	INT				m_nMaxHeight;
	INT				m_nTotalItemCY;	

	INT				m_nTextureWidth;
	INT				m_nTextureHeight;

	//INT			m_nBlankIndexCount;
	INT				m_nLineCount;
	
	CSurface		m_TextSurface;
	
	std::list<CSurface>				m_listSurface;
	std::vector<CGuiFont*>			m_vecFont;
	
	CUniBuffer		m_uniBuffer;

private:
	CMDStaticBox*	m_pMDStaticBox;
	std::list<CMDStaticBox_Item*> m_listItem;
	std::list<CMDStaticBox_Item*> m_listXYItem;
};

END_GUI

#endif
