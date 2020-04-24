#ifndef __GUI_OUTPUT_ITEM_H__
#define __GUI_OUTPUT_ITEM_H__

#include "gui_define.h"
#include "gui_unibuffer.h"

START_GUI

class COutputBox_Item
{
public:
	
	CUniBuffer* m_pBuffer;
	COLORREF	m_textcolor;
	COLORREF	m_effectcolor;
	INT			m_nEffectMode;
	INT			m_nEffectValue;

public:

	COutputBox_Item(const CGuiFont* pFont)
	{
		m_pBuffer = NTL_NEW CUniBuffer(1024);
		m_pBuffer->SetFont( pFont );
		m_textcolor = 0;
		m_effectcolor = 0;
		m_nEffectMode = 0;
		m_nEffectValue = 0;
	}

	COutputBox_Item( const WCHAR* pString, COLORREF textcolor, COLORREF effectcolor, INT nEffectMode, INT nEffectValue, const CGuiFont* pFont, BOOL bSizeCalc = FALSE )
	{
		m_pBuffer = NTL_NEW CUniBuffer(0);
		if( bSizeCalc )
			m_pBuffer->SetSizeCalcEnable();
		m_pBuffer->SetFont(pFont); 
		m_pBuffer->SetText(pString); 
		m_textcolor = textcolor;
		m_effectcolor = effectcolor;
		m_nEffectMode = nEffectMode;
		m_nEffectValue = nEffectValue;
	}

	COutputBox_Item( COutputBox_Item &item )
	{
		m_pBuffer = NTL_NEW CUniBuffer( (*item.m_pBuffer) );
		m_textcolor = item.m_textcolor;
		m_effectcolor = item.m_effectcolor;
		m_nEffectMode = item.m_nEffectMode;
		m_nEffectValue = item.m_nEffectValue;
	}

	COutputBox_Item( const WCHAR* pString, WORD* pSize, INT* pCPtoXSize,
		COLORREF textcolor, COLORREF effectcolor, INT nEffectMode, INT nEffectValue,
		const CGuiFont* pFont )
	{
		m_pBuffer = NTL_NEW CUniBuffer(0);
		m_pBuffer->SetFont(pFont); 
		m_pBuffer->SetText(pString); 
		m_pBuffer->SetSizeCopy( pSize, pCPtoXSize, lstrlen( pString ) );
		m_textcolor = textcolor;
		m_effectcolor = effectcolor;
		m_nEffectMode = nEffectMode;
		m_nEffectValue = nEffectValue;
	}

	VOID SetOutputBox_Item( const WCHAR* pString, WORD* pSize, INT* pCPtoXSize,
		COLORREF textcolor, COLORREF effectcolor, INT nEffectMode, INT nEffectValue,
		const CGuiFont* pFont )
	{
		m_pBuffer->SetFont(pFont); 
		m_pBuffer->SetText(pString); 
		m_pBuffer->SetSizeCopy( pSize, pCPtoXSize, lstrlen( pString ) );
		m_textcolor = textcolor;
		m_effectcolor = effectcolor;
		m_nEffectMode = nEffectMode;
		m_nEffectValue = nEffectValue;
	}

	VOID Clear()
	{
		m_pBuffer->Clear();
		m_textcolor = 0;
		m_effectcolor = 0;
		m_nEffectMode = 0;
		m_nEffectValue = 0;
	}

	~COutputBox_Item()
	{
		NTL_DELETE( m_pBuffer );
	}
};

END_GUI

#endif
