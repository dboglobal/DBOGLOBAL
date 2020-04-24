#include "precomp_dboclient.h"
#include "TutorialDialogFocusGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLEventFunc.h"

// dbo
#include "DboGlobal.h"
#include "DialogManager.h"
#include "DialogPriority.h"
#include "QuickSlotGui.h"

#define dTUTORIAL_FLASH_FOLDER		"\\tutorial\\"


CTutorialDialogFocus::CTutorialDialogFocus(const RwChar* pName)
:CNtlPLGui(pName)
,m_uiFocusDialog(DIALOG_UNKNOWN)
{

}

CTutorialDialogFocus::~CTutorialDialogFocus()
{

}

RwBool CTutorialDialogFocus::Create()
{
	NTL_FUNCTION( "CTutorialDialogFocus::Create" );

	if(!CNtlPLGui::Create("", "", "gui\\TutorialDialogFocus.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_TUTORIAL_FOCUS);

	m_pflashFocus = (gui::CFlash*)GetComponent("flaFocus");

	GetNtlGuiManager()->AddUpdateFunc( this );

	m_slotResize		= m_pThis->SigResize().Connect( this, &CTutorialDialogFocus::OnResize );

	LinkMsg(g_EventTLUIIndicator_ShowHide);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CTutorialDialogFocus::Destroy()
{
	NTL_FUNCTION("CTutorialDialogFocus::Destroy");

	GetNtlGuiManager()->RemoveUpdateFunc( this );

	FinishFlash();

	UnLinkMsg(g_EventTLUIIndicator_ShowHide);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CTutorialDialogFocus::Update(RwReal fElapsed)
{
	if( IsShow() == false )
		return;

	m_pflashFocus->Update(fElapsed);
}
RwBool CTutorialDialogFocus::LoadFlash(const char* acFileName, gui::CFlash* pFlash)
{
	std::string strFullName = dTUTORIAL_FLASH_FOLDER;
	strFullName += acFileName;

	if( pFlash->Load(strFullName.c_str()) == false )
	{
		DBO_FAIL("Not eixst file : " << acFileName << ". You can continue the game.");
		FinishFlash();
		return FALSE;
	}
	
	pFlash->PlayMovie(TRUE);

	return TRUE;
}

VOID CTutorialDialogFocus::LocateComponent()
{
	RwInt32 DialogType = ConvertIdicateTypetoDialog( m_uiFocusDialog );
	CNtlPLGui* pGui = GetDialogManager()->GetDialog( DialogType );
	CPos posDialogOffset;

	if( !pGui )
		return;

	if( DialogType == DIALOG_QUICKSLOT )
	{
		CQuickSlotGui* pQuickSlotGui = reinterpret_cast<CQuickSlotGui*>( pGui );
		posDialogOffset = pQuickSlotGui->GetInitialRowPosOffset();				
	}

	CRectangle rtPosition = pGui->GetPosition();
	CRectangle rtFrameResolution, rtRect;
	
	rtFrameResolution = m_pflashFocus->GetFrameResolution();
	rtRect.left		= rtPosition.left + (RwInt32)m_v2FocusOffset.x + posDialogOffset.x;
	rtRect.top		= rtPosition.top + (RwInt32)m_v2FocusOffset.y + posDialogOffset.y;
	rtRect.right	= rtRect.left + rtFrameResolution.GetWidth();
	rtRect.bottom	= rtRect.top + rtFrameResolution.GetHeight();
	m_pflashFocus->SetPosition(rtRect);
}

VOID  CTutorialDialogFocus::FinishFlash()
{
	m_pflashFocus->Unload();
}

RwUInt32 CTutorialDialogFocus::ConvertIdicateTypetoDialog(RwUInt32 uiIndicateType)
{
	switch(uiIndicateType)
	{
		case ETL_UI_INDICATOR_TYPE_LP_WND:					return DIALOG_HP;
		case ETL_UI_INDICATOR_TYPE_MINIMAP:					return DIALOG_MINIMAP;
		case ETL_UI_INDICATOR_TYPE_CHAT:					return DIALOG_CHAT;
		case ETL_UI_INDICATOR_TYPE_QUICK_SLOT:				return DIALOG_QUICKSLOT;
		case ETL_UI_INDICATOR_TYPE_QUICK_SLOT_1:			return DIALOG_QUICKSLOT;
		case ETL_UI_INDICATOR_TYPE_CAPSULE_SLOT:			return DIALOG_BAGSLOT;
		case ETL_UI_INDICATOR_TYPE_MAIN_MENU:				return DIALOG_MAINMENUBUTTON;
		case ETL_UI_INDICATOR_TYPE_MAIN_MENU_SKILL:			return DIALOG_MAINMENUBUTTON;
		case ETL_UI_INDICATOR_TYPE_MAIN_MENU_STATUS:		return DIALOG_MAINMENUBUTTON;
		case ETL_UI_INDICATOR_TYPE_STATUS_SCOUTER_SLOT:		return DIALOG_STATUS;
		case ETL_UI_INDICATOR_TYPE_CAPSULE_KIT_SCOUTER_ITEM:return DIALOG_CAPSULE_1;
		case ETL_UI_INDICATOR_TYPE_SKILL_ICON:				return DIALOG_SKILL;
		case ETL_UI_INDICATOR_TYPE_SKILL_CLOSE_BTN:			return DIALOG_SKILL;
	}

	return DIALOG_UNKNOWN;
}

VOID CTutorialDialogFocus::Display(RwUInt8 byType, RwChar* pcFlashName, const RwV2d* pFocusOffset)
{	
	m_uiFocusDialog		= byType;
	m_v2FocusOffset.x	= pFocusOffset->x;
	m_v2FocusOffset.y	= pFocusOffset->y;

	LoadFlash(pcFlashName, m_pflashFocus);

	LocateComponent();	
}

VOID CTutorialDialogFocus::OnResize( RwInt32 iOldW, RwInt32 iOldH )
{
	LocateComponent();
}

RwInt32 CTutorialDialogFocus::SwitchDialog(bool bOpen)
{
	if(bOpen)
	{
		Show(true);
	}
	else
	{
		Show(false);
		FinishFlash();
	}

	return 1;
}

VOID CTutorialDialogFocus::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CTutorialDialogFocus::HandleEvents");

	if( msg.Id == g_EventTLUIIndicator_ShowHide )
	{
		SNtlEventTLUIIndicator_ShowHide* pEvent = reinterpret_cast<SNtlEventTLUIIndicator_ShowHide*>( msg.pData );

		if( pEvent->bShow )
		{
			Display((RwUInt8)pEvent->uiTLIndicatorType, pEvent->pcFlashName, &pEvent->vFocusOffPos);
			GetDialogManager()->OpenDialog(DIALOG_TUTORIAL_DIALOG_FOCUS);
		}
		else
		{
			GetDialogManager()->CloseDialog(DIALOG_TUTORIAL_DIALOG_FOCUS);
		}
	}

	NTL_RETURNVOID();
}