#ifndef __GUI_SETUPGUI_H__
#define __GUI_SETUPGUI_H__

#include "gui_define.h"

START_GUI

class CGUIManager;

//: Setup GUI Class
class CSetupGUI
{
//! Operations:
public:
	
	static BOOL Init( HWND hWnd, INT nWidth, INT nHeight, CGUIManager* pGuiManager,
					  CHAR* pFontScr, CHAR* pHtmlMetaTag, CHAR* pFlashFontScr ); 

	static BOOL Init( HWND hWnd, INT nWidth, INT nHeight, CGUIManager* pGuiManager,
					  CHAR* pFontScr, CHAR* pHtmlMetaTag, CHAR* pFlashFontScr, CHAR* pParticleScr ); 
					  
	static VOID Deinit(VOID);
	static VOID	Resize( INT nWidth, INT nHeight, BOOL bFullScreen = FALSE );

//! Variable:
	static CGUIManager* m_pGuiManager;	
};

END_GUI

#endif
