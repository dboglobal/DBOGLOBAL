#include "gui_precomp.h"
#include "gui_setupgui.h"
#include "gui_componentmanager.h"
#include "gui_componenttypes.h"
#include "gui_inputbox_generic.h"
#include "gui_flash_generic.h"
#include "gui_particle_doc.h"

START_GUI

CGUIManager* CSetupGUI::m_pGuiManager = NULL;

BOOL CSetupGUI::Init( HWND hWnd, INT nWidth, INT nHeight, CGUIManager* pGuiManager, 
					  CHAR* pFontScr, CHAR* pHtmlMetaTag, CHAR* pFlashFontScr )
{
	CComponentManager::m_stlCompType["frame"]			= NTL_NEW CComponentType_Frame;
	CComponentManager::m_stlCompType["panel"]			= NTL_NEW CComponentType_Panel;
	CComponentManager::m_stlCompType["anipanel"]		= NTL_NEW CComponentType_AniPanel;
	CComponentManager::m_stlCompType["button"]			= NTL_NEW CComponentType_Button;
	CComponentManager::m_stlCompType["scrollbar"]		= NTL_NEW CComponentType_ScrollBar;
	CComponentManager::m_stlCompType["sliderbar"]		= NTL_NEW CComponentType_SliderBar;
	CComponentManager::m_stlCompType["inputbox"]		= NTL_NEW CComponentType_InputBox;
	CComponentManager::m_stlCompType["outputbox"]		= NTL_NEW CComponentType_OutputBox;
	CComponentManager::m_stlCompType["hypertextbox"]	= NTL_NEW CComponentType_HyperTextBox;
	CComponentManager::m_stlCompType["staticbox"]		= NTL_NEW CComponentType_StaticBox;
	CComponentManager::m_stlCompType["dialog"]			= NTL_NEW CComponentType_Dialog;
	CComponentManager::m_stlCompType["progressbar"]		= NTL_NEW CComponentType_ProgressBar;
	CComponentManager::m_stlCompType["listbox"]			= NTL_NEW CComponentType_ListBox;
	CComponentManager::m_stlCompType["balloonbox"]		= NTL_NEW CComponentType_Balloon;
	CComponentManager::m_stlCompType["tooltipbox"]		= NTL_NEW CComponentType_ToolTip;
	CComponentManager::m_stlCompType["combobox"]		= NTL_NEW CComponentType_ComboBox;
	CComponentManager::m_stlCompType["tabbutton"]		= NTL_NEW CComponentType_TabButton;
	CComponentManager::m_stlCompType["mdstaticbox"]		= NTL_NEW CComponentType_MDStaticBox;
	CComponentManager::m_stlCompType["htmlbox"]			= NTL_NEW CComponentType_HtmlBox;
	CComponentManager::m_stlCompType["flash"]			= NTL_NEW CComponentType_Flash;
	CComponentManager::m_stlCompType["particlebox"]		= NTL_NEW CComponentType_ParticleBox;

	CInputBox_Generic::StaticOnCreateDevice( hWnd ); 
	
	// Flash
	if( !CFlash_Generic::CreateFlashManager( hWnd ) )
		return FALSE;
	if( !CFlash_Generic::ParseFlashFont( pFlashFontScr ) )
		return FALSE;
			
	m_pGuiManager = pGuiManager;
	
	Resize( nWidth, nHeight );

	// Script
	GetGuiFontManager()->Initialize( pFontScr );
	GetMetaTagContainer()->SetHtmlFromFile( pHtmlMetaTag );
	
	return TRUE;
}

BOOL CSetupGUI::Init( HWND hWnd, INT nWidth, INT nHeight, CGUIManager* pGuiManager,
					  CHAR* pFontScr, CHAR* pHtmlMetaTag, CHAR* pFlashFontScr, CHAR* pParticleScr )
{
	CComponentManager::m_stlCompType["frame"]			= NTL_NEW CComponentType_Frame;
	CComponentManager::m_stlCompType["panel"]			= NTL_NEW CComponentType_Panel;
	CComponentManager::m_stlCompType["anipanel"]		= NTL_NEW CComponentType_AniPanel;
	CComponentManager::m_stlCompType["button"]			= NTL_NEW CComponentType_Button;
	CComponentManager::m_stlCompType["scrollbar"]		= NTL_NEW CComponentType_ScrollBar;
	CComponentManager::m_stlCompType["sliderbar"]		= NTL_NEW CComponentType_SliderBar;
	CComponentManager::m_stlCompType["inputbox"]		= NTL_NEW CComponentType_InputBox;
	CComponentManager::m_stlCompType["outputbox"]		= NTL_NEW CComponentType_OutputBox;
	CComponentManager::m_stlCompType["hypertextbox"]	= NTL_NEW CComponentType_HyperTextBox;
	CComponentManager::m_stlCompType["staticbox"]		= NTL_NEW CComponentType_StaticBox;
	CComponentManager::m_stlCompType["dialog"]			= NTL_NEW CComponentType_Dialog;
	CComponentManager::m_stlCompType["progressbar"]		= NTL_NEW CComponentType_ProgressBar;
	CComponentManager::m_stlCompType["listbox"]			= NTL_NEW CComponentType_ListBox;
	CComponentManager::m_stlCompType["balloonbox"]		= NTL_NEW CComponentType_Balloon;
	CComponentManager::m_stlCompType["tooltipbox"]		= NTL_NEW CComponentType_ToolTip;
	CComponentManager::m_stlCompType["combobox"]		= NTL_NEW CComponentType_ComboBox;
	CComponentManager::m_stlCompType["tabbutton"]		= NTL_NEW CComponentType_TabButton;
	CComponentManager::m_stlCompType["mdstaticbox"]		= NTL_NEW CComponentType_MDStaticBox;
	CComponentManager::m_stlCompType["htmlbox"]			= NTL_NEW CComponentType_HtmlBox;
	CComponentManager::m_stlCompType["flash"]			= NTL_NEW CComponentType_Flash;
	CComponentManager::m_stlCompType["particlebox"]		= NTL_NEW CComponentType_ParticleBox;

	CInputBox_Generic::StaticOnCreateDevice( hWnd ); 

	// Flash
	if( !CFlash_Generic::CreateFlashManager( hWnd ) )
		return FALSE;
	if( !CFlash_Generic::ParseFlashFont( pFlashFontScr ) )
		return FALSE;

	m_pGuiManager = pGuiManager;
	
	Resize( nWidth, nHeight );

	// Script
	if (GetGuiFontManager()->Initialize(pFontScr) == false)
		return FALSE;

	GetMetaTagContainer()->SetHtmlFromFile( pHtmlMetaTag );	
	GetParticleDoc()->Load( pParticleScr );

	return TRUE;
}

VOID CSetupGUI::Deinit(VOID)
{
	CComponentType* pCompType;
	CComponentManager::stlCOMPTYPE_MAP::iterator it;

	for( it = CComponentManager::m_stlCompType.begin() ; it != CComponentManager::m_stlCompType.end() ; ++it )
	{
		pCompType = (CComponentType*)(*it).second;

		NTL_DELETE( pCompType );
	}

	CComponentManager::m_stlCompType.clear();

	CFlash_Generic::DeleteFlashManager();
}

VOID CSetupGUI::Resize( INT nWidth, INT nHeight, BOOL bFullScreen /* = FALSE  */ )
{
	m_pGuiManager->SetSize( nWidth, nHeight );		

	// Flash 
	CFlash_Generic::SetScreenSize( nWidth, nHeight );
}
END_GUI

