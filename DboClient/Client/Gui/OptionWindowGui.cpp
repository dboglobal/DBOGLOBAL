#include "precomp_dboclient.h"

// core
#include "NtlDebug.h"
#include "CEventHandler.h"

#include "NtlSoundManager.h"
#include "NtlSysEvent.h"

// presentation
#include "NtlPLGui.h"
#include "NtlPLGuiManager.h"

// framework
#include "NtlApplication.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLLogic.h"
#include "NtlSLApi.h"
#include "NtlStorageMTContainer.h"
#include "NtlStorageManager.h"
#include "NtlSobManager.h"
#include "NtlCameraManager.h"

// Presentation
#include "NtlPostEffectCamera.h"

// dbo
#include "DialogManager.h"
#include "DisplayStringManager.h"
#include "DboLogic.h"
#include "DboEventGenerator.h"
#include "MoviePlayer.h"

// Option
#include "OptionWindowGui.h"
#include "OptionBase.h"
#include "OptionControlTab.h"
#include "OptionChattingTab.h"
#include "OptionGraphicTab.h"


////////////////////////////////////////////////////////////////////////////////
// Class : COptionWindowGui
////////////////////////////////////////////////////////////////////////////////

COptionWindowGui::COptionWindowGui(const RwChar* pName) : CNtlPLGui(pName), m_ePresentOption(OPTION_NONE),
m_pTitle(NULL), m_pTabButton(NULL), m_pCloseButton(NULL), m_pResetButton(NULL), m_pOkButton(NULL), m_pCancelButton(NULL),
m_pScrollBar(NULL)
{
	 memset(m_apOptionTab, NULL, sizeof(void*) * OPTION_NUM);
	 memset(m_apVirtualComponent, NULL, sizeof(void*) * dOPTION_VIRTUAL_NUM);
	 memset(m_apOptionButton, NULL, sizeof(void*) * OPTION_NUM);

	 m_eLastType = SYSTEM;
	 m_eLastTab = SYSTEM_GRAPHIC;
}

COptionWindowGui::~COptionWindowGui()
{
}

RwBool COptionWindowGui::Create()
{
	NTL_FUNCTION("COptionWindowGui::Create");

	// Component Setting
	if (!CNtlPLGui::Create("", "gui\\OptionWnd.srf", "gui\\OptionWnd.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_pThis				= (gui::CDialog*)GetComponent("dlgMain");
	m_pTitle			= (gui::CStaticBox*)GetComponent("stbTitle");
	m_pTabButton		= (gui::CTabButton*)GetComponent("tabOption");
	m_pCloseButton		= (gui::CButton*)GetComponent("btnClose");

	m_pResetButton		= (gui::CButton*)GetComponent("btnReset");
	m_pOkButton			= (gui::CButton*)GetComponent("btnOk");
	m_pCancelButton		= (gui::CButton*)GetComponent("btnCancel");

	m_pScrollBar		= (gui::CScrollBar*)GetComponent("scbScroll");

	m_pHideComponent		= GetComponent("dlgOptionHide");

	m_apVirtualComponent[0] = GetComponent("dlgOption1");
	m_apVirtualComponent[1] = GetComponent("dlgOption2");
	m_apVirtualComponent[2] = GetComponent("dlgOption3");
	m_apVirtualComponent[3] = GetComponent("dlgOption4");
	m_apVirtualComponent[4] = GetComponent("dlgOption5");
	m_apVirtualComponent[5] = GetComponent("dlgOption6");
	m_apVirtualComponent[6] = GetComponent("dlgOption7");
	m_apVirtualComponent[7] = GetComponent("dlgOption8");
	m_apVirtualComponent[8] = GetComponent("dlgOption9");

	m_apOptionButton[SYSTEM_GRAPHIC]	= m_apOptionButton[GAME_CONTROL]	= (gui::CButton*)GetComponent("btnOption1");
	m_apOptionButton[SYSTEM_SOUND]		= m_apOptionButton[GAME_INFO]		= (gui::CButton*)GetComponent("btnOption2");
	m_apOptionButton[GAME_ETC]		= (gui::CButton*)GetComponent("btnOption3");
	m_apOptionButton[GAME_CHATTING] = (gui::CButton*)GetComponent("btnOption4");

	// Dialog Priority
	m_pThis->SetPriority(dDIALOGPRIORITY_DEFAULT);

	// Component Signal Connect
	m_slotTabChanged				= m_pTabButton->SigSelectChanged().Connect(this, &COptionWindowGui::OnTabChanged);
	m_slotCloseButtonClicked		= m_pCloseButton->SigClicked().Connect(this, &COptionWindowGui::OnCloseButtonClicked);

	m_slotCaptureMouseDown				= GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect(this, &COptionWindowGui::OnCaptureMouseDown);

	m_slotResetButtonClicked		= m_pResetButton->SigClicked().Connect(this, &COptionWindowGui::OnResetButtonClicked);
	m_slotOKButtonClicked			= m_pOkButton->SigClicked().Connect(this, &COptionWindowGui::OnOkButtonClicked);
	m_slotCancelButtonClicked		= m_pCancelButton->SigClicked().Connect(this, &COptionWindowGui::OnCancelButtonClicked);

	m_slotScrollBarChanged			= m_pScrollBar->SigValueChanged().Connect(this, &COptionWindowGui::OnScrollBarChanged);
	m_slotScrollBarSliderMoved		= m_pScrollBar->SigSliderMoved().Connect(this, &COptionWindowGui::OnScrollBarChanged);

	m_slotOptionButtonClicked[0]	= m_apOptionButton[0]->SigToggled().Connect(this, &COptionWindowGui::OnOptionButtonToggled1);
	m_slotOptionButtonClicked[1]	= m_apOptionButton[1]->SigToggled().Connect(this, &COptionWindowGui::OnOptionButtonToggled2);
	m_slotOptionButtonClicked[2]	= m_apOptionButton[4]->SigToggled().Connect(this, &COptionWindowGui::OnOptionButtonToggled3);
	m_slotOptionButtonClicked[3]	= m_apOptionButton[5]->SigToggled().Connect(this, &COptionWindowGui::OnOptionButtonToggled4);

	// Component Value Setting
	m_pTitle->SetText(GetDisplayStringManager()->GetString("DST_OPTION_TITLE"));
	m_pTitle->SetPosition(DBOGUI_DIALOG_TITLE_X, DBOGUI_DIALOG_TITLE_Y);
	m_pResetButton->SetText(GetDisplayStringManager()->GetString("DST_OPTION_RESET"));
	m_pOkButton->SetText(GetDisplayStringManager()->GetString("DST_OPTION_OK"));
	m_pCancelButton->SetText(GetDisplayStringManager()->GetString("DST_OPTION_CANCEL"));
	m_pCancelButton->SetClickSound("System\\SYSTEM_CANCEL_LT.wav");

	m_pTabButton->AddTab(std::string());
	m_pTabButton->AddTab(std::string());
	m_pTabButton->ChangeTabText(0, std::wstring(GetDisplayStringManager()->GetString("DST_OPTION_TAB_SYSTEM")));
	m_pTabButton->ChangeTabText(1, std::wstring(GetDisplayStringManager()->GetString("DST_OPTION_TAB_GAME")));

	// OptionTab Setting
	m_apOptionTab[SYSTEM_GRAPHIC]	= NTL_NEW COptionGraphic;
	m_apOptionTab[SYSTEM_SOUND]	= NTL_NEW COptionSound;
	m_apOptionTab[GAME_CONTROL]	= NTL_NEW COptionControl;
	m_apOptionTab[GAME_INFO]		= NTL_NEW COptionInfo;
	m_apOptionTab[GAME_ETC]		= NTL_NEW COptionETC;
	m_apOptionTab[GAME_CHATTING]	= NTL_NEW COptionChatting;

	for (int i = 0; i < OPTION_NUM; ++i)
	{
		m_apOptionTab[i]->Create(this);
		m_apOptionTab[i]->Show(false);
	}

	LinkMsg( g_EventActionMapClientNotify, 0 );
	LinkMsg( g_EventToggleFullScreen, 0 );

	InitDialog();
	Show(false);
	NTL_RETURN(TRUE);
}

void COptionWindowGui::InitDialog()
{
	for (int i = 0; i < OPTION_NUM; ++i)
	{
		m_apOptionTab[i]->OnInit();
	}

	m_pTabButton->SelectTab(SYSTEM);
	m_apOptionButton[GAME_ETC]->Show(false);
	m_apOptionButton[GAME_CHATTING]->Show(false);
	SwitchOptionTab(SYSTEM_GRAPHIC);
	ButtonTextSystem();
	
}

void COptionWindowGui::Destroy()
{
	NTL_FUNCTION("COptionWindowGui::Destroy");

	UnLinkMsg( g_EventActionMapClientNotify );
	UnLinkMsg( g_EventToggleFullScreen );

	for (int i = 0; i < OPTION_NUM; ++i)
	{
		m_apOptionTab[i]->Destroy();
		NTL_DELETE(m_apOptionTab[i]);
	}

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
	NTL_RETURNVOID();
}

RwInt32	COptionWindowGui::SwitchDialog(bool bOpen)
{
	Show(bOpen);

	if (bOpen)
	{
		InitDialog();
	}
	else
	{
		for (int i = 0; i < OPTION_NUM; ++i)
		{
			m_apOptionTab[i]->OnCancel();
		}

		GetDialogManager()->CloseDialog( DIALOG_OPTIONWND );
	}

	return TRUE;
}

RwInt32	COptionWindowGui::SwitchOptionTab(const COptionWindowGui::eOPTION_CATEGORY& eID)
{
    if(eID >= OPTION_NUM || eID < 0)
        return FALSE;
	
	if (eID != m_ePresentOption)
	{	
		if (m_ePresentOption > OPTION_NONE)
		{
			m_apOptionButton[m_ePresentOption]->Enable(true);
			if (m_apOptionButton[m_ePresentOption]->IsDown())
			{
				m_apOptionButton[m_ePresentOption]->SetDown(false);
			}
			m_apOptionTab[m_ePresentOption]->Show(false);
			m_apOptionTab[m_ePresentOption]->OnCancel();
		}
		m_apOptionButton[eID]->Enable(false);
		m_apOptionTab[eID]->Show(true);
		m_apOptionTab[eID]->OnInit();
		m_eLastTab = m_ePresentOption = eID;
	}
	return TRUE;
}

void COptionWindowGui::SelectOptionTab( const eOPTION_CATEGORY eID )
{
	if( eID != m_ePresentOption )
	{
		if( m_ePresentOption != OPTION_NONE )
		{
			if( m_ePresentOption <= SYSTEM_SOUND && eID >= GAME_CONTROL )
			{
				m_pTabButton->SelectTab( GAME , TRUE );
				SwitchOptionTab( eID );
			}
			else if( m_ePresentOption >= GAME_CONTROL && eID <= SYSTEM_SOUND )
			{
				m_pTabButton->SelectTab( SYSTEM, TRUE );
				SwitchOptionTab( eID );
			}
			else
			{
				SwitchOptionTab( eID );
			}
		}
	}
}

VOID COptionWindowGui::HandleEvents(RWS::CMsg &msg)
{
	NTL_FUNCTION("COptionWindowGui::HandleEnvets");

	if( msg.Id == g_EventActionMapClientNotify )
		m_apOptionTab[GAME_CONTROL]->OnHandleEvents(msg);
	else if( msg.Id == g_EventToggleFullScreen )
		m_apOptionTab[SYSTEM_GRAPHIC]->OnHandleEvents(msg);

	NTL_RETURNVOID();
}

void COptionWindowGui::OnTabChanged(RwInt32 nIndex, RwInt32 nOldIndex)
{
	switch (nIndex)
	{
		case SYSTEM:
		{
			ButtonTextSystem();
			SwitchOptionTab(SYSTEM_GRAPHIC);
			m_apOptionButton[SYSTEM_GRAPHIC]->Show( true );
			m_apOptionButton[SYSTEM_SOUND]->Show( true );
			m_apOptionButton[GAME_ETC]->Show(false);
			m_apOptionButton[GAME_CHATTING]->Show(false);
		}
		break;
		case GAME:
		{
			ButtonTextGame();
			SwitchOptionTab(GAME_CONTROL);
			m_apOptionButton[GAME_CONTROL]->Show(true);
			m_apOptionButton[GAME_INFO]->Show(true);
			m_apOptionButton[GAME_ETC]->Show(true);
			m_apOptionButton[GAME_CHATTING]->Show(true);
		}
		break;
	}

	//m_eLastType = static_cast<eOPTION_TYPE>(nIndex);
}

void COptionWindowGui::ButtonTextSystem()
{
	m_apOptionButton[SYSTEM_GRAPHIC]->SetText(GetDisplayStringManager()->GetString("DST_OPTION_TAB_SYSTEM_GRAPHIC"));
	m_apOptionButton[SYSTEM_GRAPHIC]->SetToolTip(GetDisplayStringManager()->GetString("DST_OPTION_TOOLTIP_TAB_GRAPHIC"));
	/*m_apOptionButton[SYSTEM_EFFECT]->SetText(GetDisplayStringManager()->GetString(DST_OPTION_TAB_SYSTEM_EFFECT));
	m_apOptionButton[SYSTEM_EFFECT]->SetToolTip(GetDisplayStringManager()->GetString(DST_OPTION_TOOLTIP_TAB_EFFECT));*/
	m_apOptionButton[SYSTEM_SOUND]->SetText(GetDisplayStringManager()->GetString("DST_OPTION_TAB_SYSTEM_SOUND"));
	m_apOptionButton[SYSTEM_SOUND]->SetToolTip(GetDisplayStringManager()->GetString("DST_OPTION_TOOLTIP_TAB_SOUND"));
}

void COptionWindowGui::ButtonTextGame()
{
	m_apOptionButton[GAME_CONTROL]->SetText(GetDisplayStringManager()->GetString("DST_OPTION_TAB_GAME_CONTROL"));
	m_apOptionButton[GAME_CONTROL]->SetToolTip(GetDisplayStringManager()->GetString("DST_OPTION_TOOLTIP_TAB_CONTROL"));
	m_apOptionButton[GAME_INFO]->SetText(GetDisplayStringManager()->GetString("DST_OPTION_TAB_GAME_INFO"));
	m_apOptionButton[GAME_INFO]->SetToolTip(GetDisplayStringManager()->GetString("DST_OPTION_TOOLTIP_TAB_INFO"));
	m_apOptionButton[GAME_ETC]->SetText(GetDisplayStringManager()->GetString("DST_OPTION_TAB_GAME_ETC"));
	m_apOptionButton[GAME_ETC]->SetToolTip(GetDisplayStringManager()->GetString("DST_OPTION_TOOLTIP_TAB_ETC"));
	m_apOptionButton[GAME_CHATTING]->SetText(GetDisplayStringManager()->GetString("DST_OPTION_TAB_GAME_CHATTING"));
	m_apOptionButton[GAME_CHATTING]->SetToolTip(GetDisplayStringManager()->GetString( "DST_OPTION_TOOLTIP_TAB_CHATTING"));
}

void COptionWindowGui::SetComponentPos(RwInt32 iPos, gui::CComponent* pComponent)
{
	DBO_ASSERT(pComponent, "COptionWindowGui::SetComponentPos Param(pComponent) is NULL");
	if (iPos >= 0)
	{
		pComponent->SetParent(m_apVirtualComponent[iPos], true);
	}
	else
	{
		pComponent->SetParent(m_pHideComponent, true);
	}
}

VOID COptionWindowGui::OnOptionButtonToggled1(gui::CComponent* pComponent, bool bToggled)
{	
	SwitchOptionTab((COptionWindowGui::eOPTION_CATEGORY)(m_pTabButton->GetSelectedButtonIndex() * 2));
}

VOID COptionWindowGui::OnOptionButtonToggled2(gui::CComponent* pComponent, bool bToggled)
{
	SwitchOptionTab((COptionWindowGui::eOPTION_CATEGORY)(m_pTabButton->GetSelectedButtonIndex() * 2 + 1));
}

VOID COptionWindowGui::OnOptionButtonToggled3(gui::CComponent* pComponent, bool bToggled)
{
	SwitchOptionTab((COptionWindowGui::eOPTION_CATEGORY)(m_pTabButton->GetSelectedButtonIndex() * 2 + 2));
}

VOID COptionWindowGui::OnOptionButtonToggled4( gui::CComponent* pComponent, bool bToggled )
{
	SwitchOptionTab((COptionWindowGui::eOPTION_CATEGORY)(m_pTabButton->GetSelectedButtonIndex() * 2 + 3));
}

void COptionWindowGui::OptionReset()
{
	//for (int i = 0; i < OPTION_NUM; ++i)
	//{
	//	m_apOptionTab[i]->OnReset();
	//}

	if( m_ePresentOption == OPTION_NONE )
		return;

	m_apOptionTab[m_ePresentOption]->OnReset();
}

void COptionWindowGui::OptionOk()
{
	/*for (int i = 0; i < OPTION_NUM; ++i)
	{
		m_apOptionTab[i]->OnOk();
	}*/

	if( m_ePresentOption == OPTION_NONE )
		return;

	m_apOptionTab[m_ePresentOption]->OnOk();

	Logic_SaveGameOption();
	Logic_SaveSystemOption();
}

void COptionWindowGui::OptionCancel()
{
	GetDialogManager()->CloseDialog(DIALOG_OPTIONWND);
}

void COptionWindowGui::OnResetButtonClicked(gui::CComponent* pComponent)
{
	OptionReset();
}

void COptionWindowGui::OnOkButtonClicked(gui::CComponent* pComponent)
{
	OptionOk();
}

void COptionWindowGui::OnCancelButtonClicked(gui::CComponent* pComponent)
{
	OptionCancel();
}

void COptionWindowGui::OnScrollBarChanged(RwInt32 iParam)
{
	m_apOptionTab[m_ePresentOption]->Update();
}

void COptionWindowGui::OnCloseButtonClicked(gui::CComponent* pComponent)
{
	if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_OPTION_WND_CLOSE_BTN ) )
		return;

	for (int i = 0; i < OPTION_NUM; ++i)
	{
		m_apOptionTab[i]->OnCancel();
	}

	GetDialogManager()->CloseDialog(DIALOG_OPTIONWND);
}

void COptionWindowGui::OnCaptureMouseDown(const CKey& key)
{
	CAPTURE_MOUSEDOWN_RAISE(DIALOG_OPTIONWND, key.m_fX, key.m_fY);
}



////////////////////////////////////////////////////////////////////////////////
// Class : COptionSound
////////////////////////////////////////////////////////////////////////////////
COptionSound::COptionSound() : m_pSoundMain(NULL), m_pSoundBGM(NULL), m_pSoundSE(NULL), m_pSoundENV(NULL)
{
}

COptionSound::~COptionSound()
{
}

RwBool COptionSound::Create(COptionWindowGui* pOptionWindow)
{
	NTL_FUNCTION("COptionSound::Create");
	COptionBase::Create(pOptionWindow);	

	m_iNumVisible = dOPTION_VIRTUAL_NUM_CONTROL;

	// Component
	m_pSoundMain	= (gui::CScrollBar*)GetComponent("scbSoundMain");
	SetComponentGroup(0, GetDisplayStringManager()->GetString("DST_OPTION_SOUND_MAIN"),
		GetDisplayStringManager()->GetString("DST_OPTION_TOOLTIP_MAINVOLUME"),
		GetComponent("stbMiddle1"), m_pSoundMain, NULL);

	m_pSoundBGM		= (gui::CScrollBar*)GetComponent("scbSoundBGM");
	SetComponentGroup(1, GetDisplayStringManager()->GetString("DST_OPTION_SOUND_BGM"),
		GetDisplayStringManager()->GetString("DST_OPTION_TOOLTIP_BGMVOLUME"),
		GetComponent("stbMiddle2"), m_pSoundBGM, NULL);

	m_pSoundSE		= (gui::CScrollBar*)GetComponent("scbSoundSE");
	SetComponentGroup(2, GetDisplayStringManager()->GetString("DST_OPTION_SOUND_SE"),
		GetDisplayStringManager()->GetString("DST_OPTION_TOOLTIP_EFFECTVOLUME"),
		GetComponent("stbMiddle3"), m_pSoundSE, NULL);

	m_pSoundENV		= (gui::CScrollBar*)GetComponent("scbSoundENV");
	SetComponentGroup(3, GetDisplayStringManager()->GetString("DST_OPTION_SOUND_ENV"),
		GetDisplayStringManager()->GetString("DST_OPTION_TOOLTIP_ENVVOLUME"),
		GetComponent("stbMiddle4"), m_pSoundENV, NULL );

	m_slotMainSliderMoved	= m_pSoundMain->SigSliderMoved().Connect(this, &COptionSound::OnMainVolumeChanged);
	m_slotMainValueChanged	= m_pSoundMain->SigValueChanged().Connect(this, &COptionSound::OnMainVolumeChanged);

	m_slotBGMSliderMoved	= m_pSoundBGM->SigSliderMoved().Connect(this, &COptionSound::OnBGMVolumeChanged);
	m_slotBGMValueChanged	= m_pSoundBGM->SigValueChanged().Connect(this, &COptionSound::OnBGMVolumeChanged);

	m_slotSESliderMoved		= m_pSoundSE->SigSliderMoved().Connect(this, &COptionSound::OnSEVolumeChanged);
	m_slotSEValueChanged	= m_pSoundSE->SigValueChanged().Connect(this, &COptionSound::OnSEVolumeChanged);

	m_slotENVSliderMoved	= m_pSoundENV->SigSliderMoved().Connect(this, &COptionSound::OnENVVolumeChanged);
	m_slotENVValueChanged	= m_pSoundENV->SigValueChanged().Connect(this, &COptionSound::OnENVVolumeChanged);

	NTL_RETURN(TRUE);
}

void COptionSound::Destroy()
{
	NTL_FUNCTION("COptionSound::Destroy");
	NTL_RETURNVOID();
}

void COptionSound::OnInit()
{
	SetComponentVolume();
}

void COptionSound::OnReset()
{
	SetComponentVolume(TRUE);
}

void COptionSound::OnOk()
{
	SetMainVolume(m_pSoundMain->GetValue());
	SetBGMVolume(m_pSoundBGM->GetValue());
	SetSEVolume(m_pSoundSE->GetValue());
	SetENVVolume(m_pSoundENV->GetValue());
}

void COptionSound::OnCancel()
{
	SetMainVolume(m_iVolumeMain);
	SetBGMVolume(m_iVolumeBGM);
	SetSEVolume(m_iVolumeSE);
	SetENVVolume(m_iVolumeENV);
}

void COptionSound::SetComponentVolume(RwBool bDefault /* = FASLE */)
{
	RwReal fMin = 0.0f, fMax = 1.0f;

	fMin *= 100.0f;
	fMax *= 100.0f;

	m_pSoundMain->SetRange((int)fMin, (int)fMax);
	m_pSoundBGM->SetRange((int)fMin, (int)fMax);
	m_pSoundSE->SetRange((int)fMin, (int)fMax);
	m_pSoundENV->SetRange((int)fMin, (int)fMax);

	if( bDefault )
	{
		m_iVolumeMain	= (RwInt32)(GetNtlStorageMTContainer()->GetDefaultFloat( dSTORAGE_SOUND_MAIN_VOLUME ) * 100.0f);
		m_iVolumeBGM	= (RwInt32)(GetNtlStorageMTContainer()->GetDefaultFloat( dSTORAGE_SOUND_BACK_VOLUME ) * 200.0f);
		m_iVolumeSE	= (RwInt32)(GetNtlStorageMTContainer()->GetDefaultFloat( dSTORAGE_SOUND_EFFECT_VOLUME ) * 100.0f);
		m_iVolumeENV	= (RwInt32)(GetNtlStorageMTContainer()->GetDefaultFloat( dSTORAGE_SOUND_ENV_VOLUME ) * 100.0f);
	}
	else
	{
		m_iVolumeMain	= GetMainVolume();
		m_iVolumeBGM	= GetBGMVolume();
		m_iVolumeSE		= GetSEVolume();
		m_iVolumeENV	= GetENVVolume();
	}

	m_pSoundMain->SetValue(m_iVolumeMain);
	m_pSoundBGM->SetValue(m_iVolumeBGM);
	m_pSoundSE->SetValue(m_iVolumeSE);
	m_pSoundENV->SetValue(m_iVolumeENV);
}

void COptionSound::OnMainVolumeChanged(RwInt32 iParam)
{
	GetSoundManager()->SetMasterVolume((RwReal)iParam * 0.01f);
	GetMoviePlayer()->SetVolume((RwReal)iParam * 0.01f);
}

void COptionSound::OnBGMVolumeChanged(RwInt32 iParam)
{
	GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_BGM, (RwReal)iParam * 0.005f);
	GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_FLASH_MUSIC, (RwReal)iParam * 0.005f);
	GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_WEATHER_MUSIC, (RwReal)iParam * 0.005f);
	GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_AMBIENT_MUSIC, (RwReal)iParam * 0.005f);
	GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_JINGLE_MUSIC, (RwReal)iParam * 0.005f);
}

void COptionSound::OnSEVolumeChanged(RwInt32 iParam)
{
	GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_AVATAR_VOICE_SOUND, (RwReal)iParam * 0.01f);
	GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_AVATAR_EFFECT_SOUND, (RwReal)iParam * 0.01f);
	GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_VOICE_SOUND, (RwReal)iParam * 0.01f);
	GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_EFFECT_SOUND, (RwReal)iParam * 0.01f);
	GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_UI_SOUND, (RwReal)iParam * 0.01f);
}

void COptionSound::OnENVVolumeChanged( RwInt32 iParam ) 
{
	GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_OBJECT_MUSIC, (RwReal)iParam * 0.01f);
}

void COptionSound::SetMainVolume(RwInt32 iVolume)
{
	//OnMainVolumeChanged(iVolume);
	m_iVolumeMain = iVolume;
	GetSoundManager()->SetMasterVolume((RwReal)m_iVolumeMain * 0.01f);
	GetNtlStorageManager()->SetData( dSTORAGE_SOUND_MAIN_VOLUME, (RwReal)m_iVolumeMain * 0.01f );
	GetMoviePlayer()->SetVolume((RwReal)m_iVolumeMain * 0.01f);
}

RwInt32 COptionSound::GetMainVolume()
{
	return (RwInt32)(GetNtlStorageManager()->GetFloatData( dSTORAGE_SOUND_MAIN_VOLUME ) * 100.0f);
}

void COptionSound::SetBGMVolume(RwInt32 iVolume)
{
	//OnBGMVolumeChanged(iVolume);
	m_iVolumeBGM = iVolume;
	GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_BGM, (RwReal)m_iVolumeBGM * 0.005f);
	GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_FLASH_MUSIC, (RwReal)m_iVolumeBGM * 0.005f);
	GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_WEATHER_MUSIC, (RwReal)m_iVolumeBGM * 0.005f);
	GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_AMBIENT_MUSIC, (RwReal)m_iVolumeBGM * 0.005f);
	GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_JINGLE_MUSIC, (RwReal)m_iVolumeBGM * 0.005f);
	GetNtlStorageManager()->SetData( dSTORAGE_SOUND_BACK_VOLUME, (RwReal)m_iVolumeBGM * 0.005f );
}

RwInt32 COptionSound::GetBGMVolume()
{
	return (RwInt32)(GetNtlStorageManager()->GetFloatData( dSTORAGE_SOUND_BACK_VOLUME ) * 200.0f );
}

void COptionSound::SetSEVolume(RwInt32 iVolume)
{
	m_iVolumeSE = iVolume; 
	GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_AVATAR_VOICE_SOUND, (RwReal)m_iVolumeSE * 0.01f);
	GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_AVATAR_EFFECT_SOUND, (RwReal)m_iVolumeSE * 0.01f);
	GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_VOICE_SOUND, (RwReal)m_iVolumeSE * 0.01f);
	GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_EFFECT_SOUND, (RwReal)m_iVolumeSE * 0.01f);
	GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_UI_SOUND, (RwReal)m_iVolumeSE * 0.01f);
	GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_JINGLE_MUSIC, (RwReal)m_iVolumeSE * 0.01f);
	GetNtlStorageManager()->SetData( dSTORAGE_SOUND_EFFECT_VOLUME, (RwReal)iVolume * 0.01f);
}

RwInt32 COptionSound::GetSEVolume()
{
	return (RwInt32)(GetNtlStorageManager()->GetFloatData( dSTORAGE_SOUND_EFFECT_VOLUME ) * 100.0f );
}

void COptionSound::SetENVVolume( RwInt32 iVolume ) 
{
	m_iVolumeENV = iVolume;
	GetSoundManager()->SetGroupVolume(CHANNEL_GROUP_OBJECT_MUSIC, (RwReal)m_iVolumeENV * 0.01f);
	GetNtlStorageManager()->SetData( dSTORAGE_SOUND_ENV_VOLUME, (RwReal)iVolume * 0.01f);
}

RwInt32 COptionSound::GetENVVolume() 
{
	return (RwInt32)(GetNtlStorageManager()->GetFloatData( dSTORAGE_SOUND_ENV_VOLUME ) * 100.0f );
}

void COptionSound::OnHandleEvents( RWS::CMsg &pMsg ) 
{

}
////////////////////////////////////////////////////////////////////////////////
// Class : COptionInfo
////////////////////////////////////////////////////////////////////////////////
COptionInfo::COptionInfo() : m_pGuildOn(NULL), m_pGuildOff(NULL), m_pDojoOn(NULL), m_pDojoOff(NULL),
m_pScouterOn(NULL), m_pScouterOff(NULL)
{
}

COptionInfo::~COptionInfo()
{
}

RwBool COptionInfo::Create(COptionWindowGui* pOptionWindow)
{
	NTL_FUNCTION("COptionInfo::Create");
	COptionBase::Create(pOptionWindow);	

	//// Component
	//m_pGuildOn	= (gui::CButton*)GetComponent("btnInfoGuildOn");
	//m_pGuildOff	= (gui::CButton*)GetComponent("btnInfoGuildOff");
	//SetComponentGroup(0, GetDisplayStringManager()->GetString(DST_OPTION_INFO_GUILDEMBLEM),
	//	GetDisplayStringManager()->GetString(DST_OPTION_TOOLTIP_GUILD_EMBLEM),
	//	GetComponent("stbMiddle1"), m_pGuildOn, m_pGuildOff, NULL);
	//SetComponentOnOff(&m_onoffGuild, m_pGuildOn, m_pGuildOff);

	//m_pDojoOn	= (gui::CButton*)GetComponent("btnInfoDojoOn");
	//m_pDojoOff	= (gui::CButton*)GetComponent("btnInfoDojoOff");
	//SetComponentGroup(1, GetDisplayStringManager()->GetString(DST_OPTION_INFO_DOJOMARK),
	//	GetDisplayStringManager()->GetString(DST_OPTION_TOOLTIP_DOJO_MARK),
	//	GetComponent("stbMiddle2"), m_pDojoOn, m_pDojoOff, NULL);
	//SetComponentOnOff(&m_onoffDojo, m_pDojoOn, m_pDojoOff);

	m_pScouterOn = (gui::CButton*)GetComponent("btnInfoScouterOn");
	m_pScouterOff= (gui::CButton*)GetComponent("btnInfoScouterOff");
	SetComponentGroup(0, GetDisplayStringManager()->GetString("DST_OPTION_INFO_SCOUTERMODEL"),
		GetDisplayStringManager()->GetString("DST_OPTION_TOOLTIP_SCOUTER"),
		GetComponent("stbMiddle3"), m_pScouterOn, m_pScouterOff, NULL);
	SetComponentOnOff(&m_onoffScouter, m_pScouterOn, m_pScouterOff);

	NTL_RETURN(TRUE);
}

void COptionInfo::Destroy()
{
	NTL_FUNCTION("COptionInfo::Destroy");
	NTL_RETURNVOID();
}

void COptionInfo::OnInit()
{
	/*SetOnOff(&m_onoffGuild, GetGuildEmblemOnOff());
	SetOnOff(&m_onoffDojo, GetDojoMarkOnOff());*/
	SetOnOff(&m_onoffScouter, GetScouterModelOnOff());
}

void COptionInfo::OnReset()
{
	/*SetOnOff(&m_onoffGuild, GAMEOPT_DEFAULT_INFO_GUILDEMBLEM);
	SetOnOff(&m_onoffDojo, GAMEOPT_DEFAULT_INFO_DOJOMARK);*/
	SetOnOff(&m_onoffScouter, GetNtlStorageMTContainer()->GetDefaultBool( dSTORAGE_GAMEINFO_SCOUTER_VISIBLE ));
}

void COptionInfo::OnOk()
{
	//SetGuildEmblemOnOff(m_onoffGuild.bValue);
	//SetDojoMarkOnOff(m_onoffDojo.bValue);
	SetScouterModelOnOff(m_onoffScouter.bValue);
}

void COptionInfo::OnCancel()
{
}

void COptionInfo::SetGuildEmblemOnOff(RwBool bOnOff)
{
	GetNtlStorageManager()->SetData( dSTORAGE_GAMEINFO_GUILD_EMBLEM, B2b(bOnOff) );
}

RwBool COptionInfo::GetGuildEmblemOnOff()
{
	return GetNtlStorageManager()->GetBoolData( dSTORAGE_GAMEINFO_GUILD_EMBLEM );
}

void COptionInfo::SetDojoMarkOnOff(RwBool bOnOff)
{
	GetNtlStorageManager()->SetData( dSTORAGE_GAMEINFO_DOJO_MARK, B2b(bOnOff) );
}

RwBool COptionInfo::GetDojoMarkOnOff()
{
	return GetNtlStorageManager()->GetBoolData( dSTORAGE_GAMEINFO_DOJO_MARK );
}

void COptionInfo::SetScouterModelOnOff(RwBool bOnOff)
{
	GetNtlStorageManager()->SetData( dSTORAGE_GAMEINFO_SCOUTER_VISIBLE, B2b(bOnOff) );
	CNtlPLGlobal::m_bRenderScouter = bOnOff;
}

RwBool COptionInfo::GetScouterModelOnOff()
{
	return GetNtlStorageManager()->GetBoolData( dSTORAGE_GAMEINFO_SCOUTER_VISIBLE );
}

void COptionInfo::OnHandleEvents( RWS::CMsg &pMsg ) 
{

}


////////////////////////////////////////////////////////////////////////////////
// Class : COptionETC
////////////////////////////////////////////////////////////////////////////////
COptionETC::COptionETC() : m_pFriendOn(NULL), m_pFriendOff(NULL), m_pGuildOn(NULL), m_pGuildOff(NULL),
m_pQuestOn(NULL), m_pQuestOff(NULL), m_pTradeOn(NULL), m_pTradeOff(NULL)
{
}

COptionETC::~COptionETC()
{
}

RwBool COptionETC::Create(COptionWindowGui* pOptionWindow)
{
	NTL_FUNCTION("COptionETC::Create");
	COptionBase::Create(pOptionWindow);	

	// Component
	/*m_pFriendOn	= (gui::CButton*)GetComponent("btnETCFriendOn");
	m_pFriendOff= (gui::CButton*)GetComponent("btnETCFriendOff");
	SetComponentGroup(0, GetDisplayStringManager()->GetString(DST_OPTION_ETC_FRIENDCONNECT),
		GetDisplayStringManager()->GetString(DST_OPTION_TOOLTIP_FRIEND_CONNECT),
		GetComponent("stbMiddle1"), m_pFriendOn, m_pFriendOff, NULL);
	SetComponentOnOff(&m_onoffFriend, m_pFriendOn, m_pFriendOff);

	m_pGuildOn	= (gui::CButton*)GetComponent("btnETCGuildOn");
	m_pGuildOff	= (gui::CButton*)GetComponent("btnETCGuildOff");
	SetComponentGroup(1, GetDisplayStringManager()->GetString(DST_OPTION_ETC_GUILDCONNECT),
		GetDisplayStringManager()->GetString(DST_OPTION_TOOLTIP_MEMBER_CONNECT),
		GetComponent("stbMiddle2"), m_pGuildOn, m_pGuildOff, NULL);
	SetComponentOnOff(&m_onoffGuild, m_pGuildOn, m_pGuildOff);

	m_pQuestOn	= (gui::CButton*)GetComponent("btnETCQuestOn");
	m_pQuestOff	= (gui::CButton*)GetComponent("btnETCQuestOff");
	SetComponentGroup(2, GetDisplayStringManager()->GetString(DST_OPTION_ETC_QUESTEFFECT),
		GetDisplayStringManager()->GetString(DST_OPTION_TOOLTIP_QUEST_TEXT),
		GetComponent("stbMiddle3"), m_pQuestOn, m_pQuestOff, NULL);
	SetComponentOnOff(&m_onoffQuest, m_pQuestOn, m_pQuestOff);

	m_pTradeOn	= (gui::CButton*)GetComponent("btnETCTradeOn");
	m_pTradeOff	= (gui::CButton*)GetComponent("btnETCTradeOff");
	SetComponentGroup(3, GetDisplayStringManager()->GetString(DST_OPTION_ETC_USERTRADE),
		GetDisplayStringManager()->GetString(DST_OPTION_TOOLTIP_TRADE),
		GetComponent("stbMiddle4"), m_pTradeOn, m_pTradeOff, NULL);
	SetComponentOnOff(&m_onoffTrade, m_pTradeOn, m_pTradeOff);*/

	m_pWindowMoveOn = (gui::CButton*)GetComponent("btnETCWindowMoveOn");
	m_pWindowMoveOff = (gui::CButton*)GetComponent("btnETCWindowMoveOff");
	SetComponentGroup(0, GetDisplayStringManager()->GetString("DST_OPTION_ETC_WINDOWMOVE"),
		GetDisplayStringManager()->GetString("DST_OPTION_TOOLTIP_WINDOW_MOVEABLE"),
		GetComponent("stbMiddle1"), m_pWindowMoveOn, m_pWindowMoveOff, NULL);
	SetComponentOnOff(&m_onoffWindowMove, m_pWindowMoveOn, m_pWindowMoveOff);

	m_pWindowMoveDefault = (gui::CButton*)GetComponent("btnETCWindowMoveDefault");
	SetComponentGroup(1, L"", L"", GetComponent("stbMiddle7"), m_pWindowMoveDefault, NULL );
	m_pWindowMoveDefault->SetText( GetDisplayStringManager()->GetString( "DST_OPTION_ETC_WINDOWMOVE_DEFAULT" ) );
	m_pWindowMoveDefault->SetToolTip( GetDisplayStringManager()->GetString( "DST_OPTION_TOOLTIP_WINDOW_DEFAULT" ));

	m_pScbMouseDash = (gui::CScrollBar*)GetComponent("scbMouseDash");
	SetComponentGroup(2, GetDisplayStringManager()->GetString("DST_OPTION_ETC_MOUSEDASH"),
		GetDisplayStringManager()->GetString("DST_OPTION_TOOLTIP_MOUSE_DASH"),
		GetComponent("stbMiddle3"), m_pScbMouseDash, NULL );
	m_pScbMouseDash->SetRange( 0, 100 );

	m_pScbMouseFOV = (gui::CScrollBar*)GetComponent("scbFOV");
	SetComponentGroup(3, GetDisplayStringManager()->GetString("DST_OPTION_ETC_FOV"),
		GetDisplayStringManager()->GetString("DST_OPTION_TOOLTIP_FOV"),
		GetComponent("stbMiddle4"), m_pScbMouseFOV, NULL);
	m_pScbMouseFOV->SetRange(0, NTL_CAMERA_FOV_MAX_BONUS);

	m_pScbMouseViewPoint = (gui::CScrollBar*)GetComponent("scbViewPoint");
	SetComponentGroup(4, GetDisplayStringManager()->GetString("DST_OPTION_ETC_VIEW_POINT"),
		GetDisplayStringManager()->GetString("DST_OPTION_TOOLTIP_VIEW_POINT"),
		GetComponent("stbMiddle5"), m_pScbMouseViewPoint, NULL);
	m_pScbMouseViewPoint->SetRange(0, 20);

	m_pExQuickSlot1On = (gui::CButton*)GetComponent("btnETCExQuickSlot1On");
	m_pExQuickSlot1Off = (gui::CButton*)GetComponent("btnETCExQuickSlot1Off");
	SetComponentGroup(5, GetDisplayStringManager()->GetString( "DST_OPTION_ETC_EX_QUICKSLOT1" ),
		GetDisplayStringManager()->GetString("DST_OPTION_TOOLTIP_EX_QUICKSLOT1" ),
		GetComponent("stbMiddle6"), m_pExQuickSlot1On, m_pExQuickSlot1Off, NULL );
	SetComponentOnOff(&m_onoffExQuickslot1, m_pExQuickSlot1On, m_pExQuickSlot1Off );

	m_pExQuickSlot2On = (gui::CButton*)GetComponent("btnETCExQuickSlot2On");
	m_pExQuickSlot2Off = (gui::CButton*)GetComponent("btnETCExQuickSlot2Off");
	SetComponentGroup(6, GetDisplayStringManager()->GetString( "DST_OPTION_ETC_EX_QUICKSLOT2" ),
		GetDisplayStringManager()->GetString("DST_OPTION_TOOLTIP_EX_QUICKSLOT2" ),
		GetComponent("stbMiddle7"), m_pExQuickSlot2On, m_pExQuickSlot2Off, NULL );
	SetComponentOnOff(&m_onoffExQuickslot2, m_pExQuickSlot2On, m_pExQuickSlot2Off );

	m_pExQuickSlotLockOn = (gui::CButton*)GetComponent("btnETCExQuickSlotLockOn");
	m_pExQuickSlotLockOff = (gui::CButton*)GetComponent("btnETCExQuickSlotLockOff");
	SetComponentGroup(7, GetDisplayStringManager()->GetString( "DST_OPTION_ETC_EX_QUICKSLOT_LOCK" ),
		GetDisplayStringManager()->GetString("DST_OPTION_TOOLTIP_QUICKSLOT_LOCK" ),
		GetComponent("stbMiddle8"), m_pExQuickSlotLockOn, m_pExQuickSlotLockOff, NULL );
	SetComponentOnOff(&m_onoffExQuickslotLock, m_pExQuickSlotLockOn, m_pExQuickSlotLockOff );

	m_slotClickedWindowMoveDefault = m_pWindowMoveDefault->SigClicked().Connect( this, &COptionETC::OnClickedBtnWindowMoveDefault );
	m_slotDashSliderMoved = m_pScbMouseDash->SigSliderMoved().Connect( this, &COptionETC::OnMouseDashChanged );
	m_slotDashValueChanged = m_pScbMouseDash->SigValueChanged().Connect( this, &COptionETC::OnMouseDashChanged );

	m_slotExQuickSlot1OffToggled = m_pExQuickSlot1Off->SigToggled().Connect( this, &COptionETC::OnToggledBtnExQuickSlot1 );
	m_slotExQuickSlot2OnToggled = m_pExQuickSlot2On->SigToggled().Connect( this, &COptionETC::OnToggledBtnExQuickSlot2 );
	
	NTL_RETURN(TRUE);
}

void COptionETC::Destroy()
{
	NTL_FUNCTION("COptionETC::Destroy");
	NTL_RETURNVOID();
}

void COptionETC::OnInit()
{
	SetOnOff(&m_onoffWindowMove, GetWindowMoveOnOff() );
	m_pWindowMoveDefault->ClickEnable( GetWindowMoveOnOff() );
	m_pScbMouseDash->SetValue(GetMouseDash());
	m_pScbMouseFOV->SetValue(GetMouseFOV());
	m_pScbMouseViewPoint->SetValue(GetMouseViewPoint());
	SetOnOff(&m_onoffExQuickslot1, GetExQuickSlot1OnOff());
	SetOnOff(&m_onoffExQuickslot2, GetExQuickSlot2OnOff());
	SetOnOff(&m_onoffExQuickslotLock, GetQuickSlotLockOnOff());
}

void COptionETC::OnReset()
{
	SetOnOff(&m_onoffWindowMove, GetNtlStorageMTContainer()->GetDefaultBool( dSTORAGE_ETC_MOVEABLEWINDOW ) );
	m_pScbMouseDash->SetValue(GetNtlStorageMTContainer()->GetDefaultFloat(dSTORAGE_ETC_MOUSE_DASH));
	m_pScbMouseFOV->SetValue(GetNtlStorageMTContainer()->GetDefaultFloat(dSTORAGE_ETC_MOUSE_FOV));
	m_pScbMouseViewPoint->SetValue(GetNtlStorageMTContainer()->GetDefaultFloat(dSTORAGE_ETC_MOUSE_VIEW_POINT));
	SetOnOff(&m_onoffExQuickslot1, GetNtlStorageMTContainer()->GetDefaultBool( dSTORAGE_ETC_EX_QUICKSLOT1 ) );
	SetOnOff(&m_onoffExQuickslot2, GetNtlStorageMTContainer()->GetDefaultBool( dSTORAGE_ETC_EX_QUICKSLOT2 ) );
	SetOnOff(&m_onoffExQuickslotLock, GetNtlStorageMTContainer()->GetDefaultBool( dSTORAGE_ETC_QUICKSLOT_LOCK ));
}

void COptionETC::OnOk()
{
	SetWindowMoveOnOff(m_onoffWindowMove.bValue);
	SetMouseDash( m_pScbMouseDash->GetValue() );
	SetMouseFOV(m_pScbMouseFOV->GetValue());
	SetMouseViewPoint(m_pScbMouseViewPoint->GetValue());
	SetExQuickSlot1OnOff( m_onoffExQuickslot1.bValue );
	SetExQuickSlot2OnOff( m_onoffExQuickslot2.bValue );
	SetQuickSlotLockOnOff( m_onoffExQuickslotLock.bValue );

	CNtlSLEventGenerator::QuickSlotGuiMode( m_onoffExQuickslot1.bValue, m_onoffExQuickslot2.bValue );
	CNtlSLEventGenerator::QuickSlotLockMode( m_onoffExQuickslotLock.bValue );
}

void COptionETC::OnCancel()
{
}

void COptionETC::OnMouseDashChanged( RwInt32 iParam )
{

}

void COptionETC::SetFriendConnectOnOff(RwBool bOnOff)
{
	GetNtlStorageManager()->SetData( dSTORAGE_ETC_FRIEND_NOTIFY, B2b(bOnOff) );
}

RwBool COptionETC::GetFriendConnectOnOff()
{
	return GetNtlStorageManager()->GetBoolData( dSTORAGE_ETC_FRIEND_NOTIFY );
}

void COptionETC::SetGuildMemberConnectOnOff(RwBool bOnOff)
{
	GetNtlStorageManager()->SetData( dSTORAGE_ETC_GUILD_MEMBER_NOTIFY, B2b(bOnOff) );
}

RwBool COptionETC::GetGuildMemberConnectOnOff()
{
	return GetNtlStorageManager()->GetBoolData( dSTORAGE_ETC_GUILD_MEMBER_NOTIFY );
}

void COptionETC::SetQuestTextEffectOnOff(RwBool bOnOff)
{
	GetNtlStorageManager()->SetData( dSTORAGE_ETC_QUESTTEXT_EFFECT, B2b(bOnOff) );
}

RwBool COptionETC::GetQuestTextEffectOnOff()
{
	return GetNtlStorageManager()->GetBoolData( dSTORAGE_ETC_QUESTTEXT_EFFECT );
}

void COptionETC::SetUserTradeOnOff(RwBool bOnOff)
{
	GetNtlStorageManager()->SetData( dSTORAGE_ETC_USER_TRADE, B2b(bOnOff) );
}

RwBool COptionETC::GetUserTradeOnOff()
{
	return GetNtlStorageManager()->GetBoolData( dSTORAGE_ETC_USER_TRADE );
}

void COptionETC::SetWindowMoveOnOff( RwBool bOnOff )
{
	GetNtlStorageManager()->SetData( dSTORAGE_ETC_MOVEABLEWINDOW, B2b(bOnOff) );
	m_pWindowMoveDefault->ClickEnable( bOnOff );

	if( bOnOff )
		CNtlSLEventGenerator::DialogEvent(TRM_DIALOG_DIALOG_MOVABLE);
	else
		CNtlSLEventGenerator::DialogEvent(TRM_DIALOG_DIALOG_DISMOVABLE);		
}

RwBool COptionETC::GetWindowMoveOnOff()
{
	return GetNtlStorageManager()->GetBoolData( dSTORAGE_ETC_MOVEABLEWINDOW );
}

void COptionETC::OnHandleEvents( RWS::CMsg &pMsg ) 
{
}

void COptionETC::OnClickedBtnWindowMoveDefault( gui::CComponent* pComponent )
{
	CNtlSLEventGenerator::DialogEvent(TRM_DIALOG_DIALOG_MOVABLE_DEFAULT_POSITION);
}

void COptionETC::OnToggledBtnExQuickSlot1( gui::CComponent* pComponent, bool bToggled )
{
	if( bToggled )
	{
		SetOnOff( &m_onoffExQuickslot2, FALSE );
	}
}

void COptionETC::OnToggledBtnExQuickSlot2( gui::CComponent* pComponent, bool bToggled )
{
	if( bToggled )
	{
		SetOnOff( &m_onoffExQuickslot1, TRUE );
	}
}

void COptionETC::SetMouseDash( RwInt32 iDashInterval )
{
	m_iMouseDash = iDashInterval;
	API_SetDoubleClickInterval( m_iMouseDash * 0.01f );
	GetNtlStorageManager()->SetData( dSTORAGE_ETC_MOUSE_DASH, (RwReal)m_iMouseDash * 0.01f );
}

RwInt32 COptionETC::GetMouseDash()
{
	return (RwInt32)(100.0f * GetNtlStorageManager()->GetFloatData( dSTORAGE_ETC_MOUSE_DASH ) );
}

void COptionETC::SetMouseFOV(float fValue)
{
	float fFov = NTL_CAMERA_DEFAULT_FOV + fValue;

	GetNtlGameCameraManager()->SetFov(fFov);

	GetNtlStorageManager()->SetData(dSTORAGE_ETC_MOUSE_FOV, fValue);
}

float COptionETC::GetMouseFOV()
{
	return GetNtlStorageManager()->GetFloatData(dSTORAGE_ETC_MOUSE_FOV);
}

void COptionETC::SetMouseViewPoint(float fValue)
{
	GetNtlGameCameraManager()->SetHeightRatioBonus(fValue);

	GetNtlStorageManager()->SetData(dSTORAGE_ETC_MOUSE_VIEW_POINT, fValue);
}

float COptionETC::GetMouseViewPoint()
{
	return GetNtlStorageManager()->GetFloatData(dSTORAGE_ETC_MOUSE_VIEW_POINT);
}

void COptionETC::SetExQuickSlot1OnOff( RwBool bOnOff )
{
	GetNtlStorageManager()->SetData( dSTORAGE_ETC_EX_QUICKSLOT1, B2b(bOnOff) );
	/*CNtlSLEventGenerator::QuickSlotGuiMode( eQUICKSLOT_GUI_MODE_EXPAND_1, bOnOff );		*/
}

RwBool COptionETC::GetExQuickSlot1OnOff()
{
	return GetNtlStorageManager()->GetBoolData( dSTORAGE_ETC_EX_QUICKSLOT1 );
}

void COptionETC::SetExQuickSlot2OnOff( RwBool bOnOff )
{
	GetNtlStorageManager()->SetData( dSTORAGE_ETC_EX_QUICKSLOT2, B2b(bOnOff) );
}

RwBool COptionETC::GetExQuickSlot2OnOff()
{
	return GetNtlStorageManager()->GetBoolData( dSTORAGE_ETC_EX_QUICKSLOT2 );
}

void COptionETC::SetQuickSlotLockOnOff( RwBool bOnOff )
{
	GetNtlStorageManager()->SetData( dSTORAGE_ETC_QUICKSLOT_LOCK, B2b(bOnOff) );
}

RwBool COptionETC::GetQuickSlotLockOnOff()
{
	return GetNtlStorageManager()->GetBoolData( dSTORAGE_ETC_QUICKSLOT_LOCK );
}

