#include "precomp_dboclient.h"
#include "CharCreateGui.h"

// core
#include "NtlDebug.h"

// sound 
#include "GUISoundDefine.h"
// share
#include "NtlCharacter.h"

// presentation
#include "NtlPLDef.h"
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// dbo
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DboGlobal.h"
#include "DisplayStringManager.h"
#include "CharStageState.h"
#include "CharMakePartGui.h"
#include "ClassExplainGui.h"
#include "RaceExplainGui.h"

namespace
{
	#define	dEDGE_ADJUST			4
};


CCharCreateGuiGui::CCharCreateGuiGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_byRotationState(ER_STOP)
{
}

CCharCreateGuiGui::~CCharCreateGuiGui()
{

}

RwBool CCharCreateGuiGui::Create()
{
	NTL_FUNCTION( "CCharCreateGuiGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\CharCreate.srf", "gui\\CharCreate.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	CRectangle rect;

	// rotate left button
	m_pLeftRotButton = (gui::CButton*)GetComponent("LeftRotButton");
	m_pLeftRotButton->SetClickSound( GSD_SYSTEM_CHARACTER_ROTATE );
	m_slotPressLeftButton		= m_pLeftRotButton->SigPressed().Connect( this, &CCharCreateGuiGui::OnPress_LeftTurnButton );
	m_slotReleaseLeftButton		= m_pLeftRotButton->SigReleased().Connect( this, &CCharCreateGuiGui::OnRelease_TurnButton );

	// rotate right button
	m_pRightRotButton = (gui::CButton*)GetComponent("RightRotButton");
	m_pRightRotButton->SetClickSound( GSD_SYSTEM_CHARACTER_ROTATE );
	m_slotPressRightButton		= m_pRightRotButton->SigPressed().Connect( this, &CCharCreateGuiGui::OnPress_RightTurnButton );
	m_slotReleaseRightButton	= m_pRightRotButton->SigReleased().Connect( this, &CCharCreateGuiGui::OnRelease_TurnButton );

	// return button
	m_pReturnButton = (gui::CButton*)GetComponent("btnReturn");
	m_pReturnButton->SetTextFocusColor(INFOCOLOR_LOBBY_FOC);
	m_pReturnButton->SetTextDownColor(INFOCOLOR_LOBBY_DOWN);
	m_pReturnButton->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_RETURN"));
	m_slotReturnButton = m_pReturnButton->SigClicked().Connect( this, &CCharCreateGuiGui::OnClick_ReturnButton );

	// create button
	m_pMakeButton = (gui::CButton*)GetComponent("btnCreateChar");
	m_pMakeButton->SetTextFocusColor(INFOCOLOR_LOBBY_FOC);
	m_pMakeButton->SetTextDownColor(INFOCOLOR_LOBBY_DOWN);
	m_pMakeButton->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_MAKE"));
	m_slotMakeButton = m_pMakeButton->SigClicked().Connect( this, &CCharCreateGuiGui::OnClick_MakeButton );

	// race Description Gui
	m_pRaceExplainGui = NTL_NEW CRaceExplainGui("RaceExplain");
	m_pRaceExplainGui->Create();
	m_pRaceExplainGui->SetPosition(dEDGE_ADJUST, 53);

	// Class Description Gui
	m_pClassExplainGui = NTL_NEW CClassExplainGui("ClassExplain");
	m_pClassExplainGui->Create();
	m_pClassExplainGui->SetPosition(dEDGE_ADJUST, 364);

	// The class that selects each part of the character
	m_pCharMakePartGui = NTL_NEW CCharMakePartGui("CharMakePart");
	m_pCharMakePartGui->Create(this);

	// Name Back surface
	m_srfNameBack.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate.srf", "srfNameBack" ));

	// 'name'
	rect.SetRectWH(415, 692, 60, 14);
	m_pNameStatic = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
	m_pNameStatic->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pNameStatic->SetText(GetDisplayStringManager()->GetString("DST_STATUS_NAME"));
	m_pNameStatic->Enable(false);

	// 이름 입력란
	m_pNameInput = (gui::CInputBox*)GetComponent( "NameInput" );
	m_pNameInput->SetMaxLength(NTL_MAX_SIZE_CHAR_NAME);
	m_pNameInput->SetCaretSize(dINPUTBOX_CARET_WIDTH, dINPUTBOX_CARET_HEIGHT);
	m_pNameInput->Clear();
	
	LocateComponent(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());

	// default value
	m_tMouseRol.bPressLeft	= FALSE;
	m_tMouseRol.iOldX		= 0;	

	// sig	
	m_slotMouseDown		= m_pThis->SigMouseDown().Connect(this, &CCharCreateGuiGui::OnMouseDown );
	m_slotMouseUp		= m_pThis->SigMouseUp().Connect(this, &CCharCreateGuiGui::OnMouseUp);
	m_slotMouseMove		= m_pThis->SigMouseMove().Connect(this, &CCharCreateGuiGui::OnMouseMove );	
	m_slotCaptureWheelMove	= GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect( this, &CCharCreateGuiGui::OnCaptureWheelMove );
	m_slotCaptureMouseUp	= GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseUp().Connect( this, &CCharCreateGuiGui::OnCaptureMouseUp );
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CCharCreateGuiGui::OnPaint );

	LinkMsg(g_EventCharStageStateEnter);
	LinkMsg(g_EventCharStageStateExit);
	LinkMsg(g_EventResize);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CCharCreateGuiGui::Destroy()
{
	NTL_FUNCTION("CCharCreateGuiGui::Destroy");

	if( m_pRaceExplainGui )
	{
		m_pRaceExplainGui->Destroy();
		NTL_DELETE(m_pRaceExplainGui);
	}

	if( m_pClassExplainGui )
	{
		m_pClassExplainGui->Destroy();
		NTL_DELETE(m_pClassExplainGui);
	}

	if( m_pCharMakePartGui )
	{
		m_pCharMakePartGui->Destroy();
		NTL_DELETE(m_pCharMakePartGui);
	}

	UnLinkMsg(g_EventCharStageStateEnter);
	UnLinkMsg(g_EventCharStageStateExit);
	UnLinkMsg(g_EventResize);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

const WCHAR* CCharCreateGuiGui::GetInputText()
{
	return m_pNameInput->GetText();
}

VOID CCharCreateGuiGui::SwitchDialog(bool bShow)
{
	m_pRaceExplainGui->Show(bShow);
	m_pClassExplainGui->Show(bShow);
	m_pCharMakePartGui->Show(bShow);

	Show(bShow);
}

VOID CCharCreateGuiGui::EnableButtons(bool bEnable)
{
	m_pLeftRotButton	->ClickEnable(bEnable);
	m_pRightRotButton	->ClickEnable(bEnable);
	m_pReturnButton		->ClickEnable(bEnable);
	m_pMakeButton		->ClickEnable(bEnable);
}

VOID CCharCreateGuiGui::LocateComponent(RwInt32 iWidth, RwInt32 iHeight)
{
	RwInt32 iXPos, iYPos;

	m_pThis->SetPosition(CRectangle(0, 0, iWidth, iHeight));

	m_pCharMakePartGui->SetPosition(iWidth - m_pCharMakePartGui->GetWidth() - dEDGE_ADJUST, 46);

	RwInt32 iButtonCenterX = iWidth/2;
	m_pLeftRotButton->SetPosition(iButtonCenterX - 206, iHeight - 194);
	m_pRightRotButton->SetPosition(iButtonCenterX + 206 - m_pRightRotButton->GetWidth(), iHeight - 194);

	iXPos = iButtonCenterX - m_srfNameBack.GetWidth()/2;
	iYPos = iHeight - 84;
	m_srfNameBack.SetPosition(iXPos, iYPos);

	m_pNameStatic	->SetPosition(iXPos + 5, iYPos + 22);
	m_pNameInput	->SetPosition(iXPos + 75, iYPos + 24);
	m_pReturnButton	->SetPosition(iWidth - 185, iHeight - 75);
	m_pMakeButton	->SetPosition(iXPos + 215, iYPos + 17);
}

VOID CCharCreateGuiGui::OnPress_LeftTurnButton(gui::CComponent* pComponent)
{
	if( GetCharStageState()->GetCurrentState() != CHAR_STATE_MAKE_IDLE )
		return;

	m_byRotationState = ER_TURN_LEFT;
	CDboEventGenerator::LobbyEvent(LMT_ROTATION_LEFT, 100.0f);
}

VOID CCharCreateGuiGui::OnPress_RightTurnButton(gui::CComponent* pComponent)
{
	if( GetCharStageState()->GetCurrentState() != CHAR_STATE_MAKE_IDLE )
		return;

	m_byRotationState = ER_TURN_RIGHT;
	CDboEventGenerator::LobbyEvent(LMT_ROTATION_RIGHT, 100.0f);
}

VOID CCharCreateGuiGui::OnRelease_TurnButton(gui::CComponent* pComponent)
{
	if( GetCharStageState()->GetCurrentState() != CHAR_STATE_MAKE_IDLE )
		return;

	m_byRotationState = ER_STOP;
	CDboEventGenerator::LobbyEvent(LMT_ROTATION_STOP);
}

VOID CCharCreateGuiGui::OnClick_MakeButton(gui::CComponent* pComponent)
{
	if( GetCharStageState()->GetCurrentState() != CHAR_STATE_MAKE_IDLE )
		return;

	GetAlarmManager()->AlarmMessage("DST_LOBBY_ADD_CHARACTER");
}

VOID CCharCreateGuiGui::OnClick_ReturnButton(gui::CComponent* pComponent)
{
	if( GetCharStageState()->GetCurrentState() != CHAR_STATE_MAKE_IDLE )
		return;

	GetCharStageState()->ChangeState(CHAR_STATE_MAKE_EXIT);
}

VOID CCharCreateGuiGui::OnMouseDown(const CKey& key)
{
	if( key.m_nID != UD_LEFT_BUTTON )
		return;

	m_tMouseRol.bPressLeft	= TRUE;
	m_tMouseRol.iOldX		= (RwInt32)key.m_fX;
}

VOID CCharCreateGuiGui::OnMouseUp(const CKey& key)
{
	if( key.m_nID != UD_LEFT_BUTTON )
		return;

	m_tMouseRol.bPressLeft	= FALSE;
}

VOID CCharCreateGuiGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	if( !m_tMouseRol.bPressLeft )
		return;

	if( m_tMouseRol.iOldX == nX )
		return;

	RwReal fDeltaAngle = (RwReal)(nX - m_tMouseRol.iOldX) * 1.5f;
	CDboEventGenerator::LobbyEvent(LMT_ROTATION, fDeltaAngle);

	m_tMouseRol.iOldX = nX;
}

VOID CCharCreateGuiGui::OnCaptureWheelMove(RwInt32 nFlag, RwInt16 sDelta, CPos& pos)
{
	if( !IsShow() )
		return;

	if(sDelta < 0)
		CDboEventGenerator::LobbyEvent(LMT_ZOOM_OUT);
	else
		CDboEventGenerator::LobbyEvent(LMT_ZOOM_IN);
}

VOID CCharCreateGuiGui::OnCaptureMouseUp(const CKey& key)
{
	if( key.m_nID != UD_LEFT_BUTTON )
		return;

	m_tMouseRol.bPressLeft = FALSE;
}

VOID CCharCreateGuiGui::OnPaint()
{
	m_srfNameBack.Render();
}

VOID CCharCreateGuiGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CCharCreateGuiGui::HandleEvents");

	if( msg.Id == g_EventCharStageStateEnter )
	{
		SDboEventCharStageStateEnter* pEvent = reinterpret_cast<SDboEventCharStageStateEnter*>( msg.pData );

		if( pEvent->byState == CHAR_STATE_MAKE_IDLE )
			EnableButtons(true);
		else
			EnableButtons(false);

		if( pEvent->byState == CHAR_STATE_MAKE_ENTER )
		{
			m_pNameInput->Clear();
			SwitchDialog(true);
		}
	}
	else if( msg.Id == g_EventCharStageStateExit )
	{
		SDboEventCharStageStateExit* pEvent = reinterpret_cast<SDboEventCharStageStateExit*>( msg.pData );
		if( pEvent->byState == CHAR_STATE_MAKE_EXIT )
		{
			m_byRotationState = ER_STOP;
			CDboEventGenerator::LobbyEvent(LMT_ROTATION_FOREWARD);

			if( GetCharStageState()->GetReservateState() == CHAR_STATE_RETURN_LOGIN ) // this is kind of useless.. To do
			{
				CDboEventGenerator::LobbyEvent(LMT_DESTORY_LOBBY_AVATAR);
				CDboEventGenerator::LobbyEvent(LMT_DESTORY_LOBBY_WORLD);
			}			

			SwitchDialog(false);
		}
	}
	else if(msg.Id == g_EventResize)
	{
		SNtlPLEventResize* pEvent = reinterpret_cast<SNtlPLEventResize*>( msg.pData );		
		LocateComponent(pEvent->iWidht, pEvent->iHeight);	
	}

	NTL_RETURNVOID();
}