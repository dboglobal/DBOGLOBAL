#ifndef __GUI_STATICBOX_ITEM_H__
#define __GUI_STATICBOX_ITEM_H__

#include "gui_define.h"
#include "gui_unibuffer.h"

START_GUI

class CStaticBox_Item
{
public:

	CUniBuffer		*m_pBuffer;	

public:
	
	CStaticBox_Item(const WCHAR *pString, const CGuiFont *pFont)
	{
		m_pBuffer = NTL_NEW CUniBuffer(0);
		m_pBuffer->SetSizeCalcEnable();
		m_pBuffer->SetFont(pFont); 		
		m_pBuffer->SetText(pString);		
	}

	~CStaticBox_Item()
	{
		NTL_DELETE( m_pBuffer );
	}
};

END_GUI

#endif
