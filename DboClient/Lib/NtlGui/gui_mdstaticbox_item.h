#ifndef __GUI_MDSTATICBOX_ITEM_H__
#define __GUI_MDSTATICBOX_ITEM_H__

#include "gui_define.h"

START_GUI

class CMDStaticBox_Item
{
public:
//! Variables
	std::wstring m_wstrText;		// Item의 Text, 실제 Data
	std::string	 m_strName;			// Item의 ID

	INT			m_nPosX;
	INT			m_nPosY;			// Item의 Left, Top
	INT			m_nCX;
	INT			m_nCY;				// Item의 Width, Height
		
	COLORREF	m_colorText;		
	DWORD 		m_dwStyle;

	COLORREF	m_colorEffect;
	INT			m_nEffectMode;
	INT			m_nEffectValue;

	BOOL		m_bAttachPrevLine;	// 상위 라인 끝에서 시작.

	DWORD		m_dwFontIndex;		// Item의 폰트 인덱스.

	std::list<CMDStaticBox_Item*> m_listChildItem;	// 자식 Item

//! Constructor & Destructor
	CMDStaticBox_Item()
	{
		m_wstrText = L"";		
		m_nPosX = m_nPosY = m_nCX = m_nCY = 0;
		m_colorText = DEFAULT_TEXTCOLOR;
		m_dwStyle = COMP_TEXT_LEFT;
		m_bAttachPrevLine = FALSE;
		m_dwFontIndex = 0;
		m_colorEffect = DEFAULT_TEXTEFFECTCOLOR;
		m_nEffectMode = DEFAULT_TEXTEFFECTMODE;
		m_nEffectValue = 0;
	}
	
	CMDStaticBox_Item( const WCHAR* pString, const CHAR* pName, INT nX, INT nY, INT nCX, INT nCY, 
					   DWORD dwFontIndex, COLORREF colorText, COLORREF colorEffect, INT nEffectMode, INT nEffectValue,
					   DWORD dwStyle, BOOL bAttachPrevLine = FALSE )
	{
		m_wstrText = pString;
		m_strName = pName;

		m_nPosX = nX;
		m_nPosY = nY;
		m_nCX = nCX;
		m_nCY = nCY;
		
		m_colorText = colorText;
		m_colorEffect = colorEffect;
		m_nEffectMode = nEffectMode;
		m_nEffectValue = nEffectValue;
		m_dwStyle = dwStyle;
		m_bAttachPrevLine = bAttachPrevLine;
		m_dwFontIndex = dwFontIndex;
	}

	~CMDStaticBox_Item()
	{
		ClearSubItem();
	}

//! Operation:
	VOID SetPos( INT nX, INT nY )
	{
		m_nPosX = nX;
		m_nPosY = nY;
	}

	VOID SetSize( INT nCX, INT nCY )
	{
		m_nCX = nCX;
		m_nCY = nCY;
	}

	VOID SetValue( const WCHAR* pString, DWORD dwFontIndex, COLORREF colorText, COLORREF colorEffect, 
				   INT nEffectMode, INT nEffectValue, DWORD dwStyle, BOOL bAttachPrevLine = FALSE )
	{
		m_wstrText = pString;

		m_colorText = colorText;
		m_colorEffect = colorEffect;
		m_nEffectMode = nEffectMode;
		m_nEffectValue = nEffectValue;
		m_dwStyle = dwStyle;
		m_bAttachPrevLine = bAttachPrevLine;
		m_dwFontIndex = dwFontIndex;
	}

	VOID AddItem( CMDStaticBox_Item* pItem )
	{
		m_listChildItem.push_back( pItem );
	}

	VOID ClearSubItem(VOID)
	{
		std::list<CMDStaticBox_Item*>::iterator it;

		for( it = m_listChildItem.begin() ; it != m_listChildItem.end() ; ++it )
		{
			NTL_DELETE( *it );
		}

		m_listChildItem.clear();
	}

};

END_GUI

#endif
