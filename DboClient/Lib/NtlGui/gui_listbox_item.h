#ifndef __GUI_LISTBOX_ITEM_H__
#define __GUI_LISTBOX_ITEM_H__

#include "gui_define.h"
#include "gui_unibuffer.h"

START_GUI

class CListBox_Item
{
public:
	CListBox_Item() : m_bSelected(false) {}

	CListBox_Item( const WCHAR* pString, const CGuiFont* pFont, DWORD dwData = 0, bool bSelected = false )
	{
		m_pBuf = NTL_NEW CUniBuffer( 0 );
		m_pBuf->SetSizeCalcEnable();
		m_pBuf->SetFont( pFont );
		m_pBuf->SetText( pString );
		m_dwData	= dwData;
		m_bSelected = bSelected;
	}

	~CListBox_Item()
	{
		NTL_DELETE(m_pBuf);
	}

	bool operator< ( const CListBox_Item &item ) const
	{
		std::wstring str1, str2;
		str1 = m_pBuf->GetBuffer();
		str2 = item.m_pBuf->GetBuffer();

		return str1 < str2;
	}

	bool operator== ( const CListBox_Item &item ) const
	{
		std::wstring str1, str2;
		str1 = m_pBuf->GetBuffer();
		str2 = item.m_pBuf->GetBuffer();

		return str1 == str2;
	}

	CUniBuffer* m_pBuf;
	DWORD		m_dwData;		
	bool		m_bSelected;	
};

END_GUI

#endif
