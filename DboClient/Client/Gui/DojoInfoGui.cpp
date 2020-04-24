#include "precomp_dboclient.h"
#include "DojoInfoGui.h"

// shared
#include "NPCTable.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlWorldConcept.h"
#include "NtlSLEventFunc.h"
#include "NtlCameraController.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"

// cleint
#include "DboEventGenerator.h"
#include "DialogManager.h"
#include "DisplayStringManager.h"
#include "DboGlobal.h"


CDojoInfoGui::CDojoInfoGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_eTabPage(TAB_PAGE_INFO)
,m_hNPCSerial(INVALID_SERIAL_ID)
{

}

CDojoInfoGui::~CDojoInfoGui()
{
	Destroy();
}

RwBool CDojoInfoGui::Create()
{
	NTL_FUNCTION( "CDojoInfoGui::Create" );

	if(!CNtlPLGui::Create("gui\\DojoAccount.rsr", "gui\\DojoInfo.srf", "gui\\DojoInfo.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis				= (gui::CDialog*)GetComponent("dlgMain");

	m_pTabButton		= (gui::CTabButton*)GetComponent( "TabButton" );	

	m_pHelpButton		= (gui::CButton*)GetComponent( "btnHelp" );
	m_pExitButton		= (gui::CButton*)GetComponent( "btnExit" );

	m_pDialogName		= (gui::CStaticBox*)GetComponent( "stbTitle" );

	// 백라인
	m_srf3BackLine.SetType(CWindowby3::WT_HORIZONTAL);
	m_srf3BackLine.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfBackLineTop" ) );
	m_srf3BackLine.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfBackLineCenter" ) );
	m_srf3BackLine.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfBackLineBottom" ) );
	m_srf3BackLine.SetSize(303, 414);
	m_srf3BackLine.SetPositionfromParent(9, 50);

	// 정보 페이지
	m_pTabPage[TAB_PAGE_INFO] = NTL_NEW CDojoInfo_Info;
	if( !m_pTabPage[TAB_PAGE_INFO]->Create(this) )
		NTL_RETURN(FALSE);

	// 전투 페이지
	m_pTabPage[TAB_PAGE_BATTLE] = NTL_NEW CDojoInfo_Battle;
	if( !m_pTabPage[TAB_PAGE_BATTLE]->Create(this) )
		NTL_RETURN(FALSE);


	// 스트링	
	m_pDialogName->SetText(GetDisplayStringManager()->GetString("DST_DOJO_INFO"));

	std::wstring wstrText = GetDisplayStringManager()->GetString("DST_DOJO_INFO_INFO");
	m_pTabButton->AddTab(wstrText);

	wstrText = GetDisplayStringManager()->GetString("DST_DOJO_INFO_BATTLE");
	m_pTabButton->AddTab(wstrText);

	m_pTabButton->SelectTab(0);	

	// default value
	m_pDialogName->SetPosition(DBOGUI_DIALOG_TITLE_X, DBOGUI_DIALOG_TITLE_Y);

	// sig
	m_slotTabChanged				= m_pTabButton->SigSelectChanged().Connect( this, &CDojoInfoGui::OnTabChanged );
	m_slotHelpButton				= m_pHelpButton->SigClicked().Connect(this, &CDojoInfoGui::OnClicked_HelpButton);
	m_slotExitButton				= m_pExitButton->SigClicked().Connect(this, &CDojoInfoGui::OnClicked_ExitButton);

	m_slotMouseDown					= m_pThis->SigMouseDown().Connect( this, &CDojoInfoGui::OnMouseDown );
	m_slotMouseUp					= m_pThis->SigMouseUp().Connect( this, &CDojoInfoGui::OnMouseUp );	
	m_slotMove						= m_pThis->SigMove().Connect( this, &CDojoInfoGui::OnMove );	
	m_slotMouseMove					= m_pThis->SigMouseMove().Connect( this, &CDojoInfoGui::OnMouseMove );
	m_slotMouseLeave				= m_pThis->SigMouseLeave().Connect( this, &CDojoInfoGui::OnMouseLeave );
	m_slotPaint						= m_pThis->SigPaint().Connect( this, &CDojoInfoGui::OnPaint );
	m_slotCaptureMouseDown			= GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CDojoInfoGui::OnCaptureMouseDown );

	// OnMove를 통해 위치가 결정되는 컴포넌트를 위해서
	OnMove(100, 100);

	LinkMsg(g_EventNPCDialogOpen);
	LinkMsg(g_EventCharObjDelete);
	LinkMsg(g_EventDialog);
	LinkMsg(g_EventDojoNotify);
	LinkMsg(g_EventPostDialog);
	LinkMsg(g_EventNotifyGuild);
	LinkMsg(g_EventScrambleDefaultInfoNotify);	

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CDojoInfoGui::Destroy()
{
	NTL_FUNCTION("CDojoInfoGui::Destroy");

	for( RwUInt8 i = 0 ; i < NUM_TAB_PAGE ; ++i )
	{
		if( m_pTabPage[i] )
		{
			m_pTabPage[i]->Destroy();
			NTL_DELETE(m_pTabPage[i]);
		}
	}

	UnLinkMsg(g_EventNPCDialogOpen);
	UnLinkMsg(g_EventCharObjDelete);
	UnLinkMsg(g_EventDialog);
	UnLinkMsg(g_EventDojoNotify);
	UnLinkMsg(g_EventPostDialog);
	UnLinkMsg(g_EventNotifyGuild);
	UnLinkMsg(g_EventScrambleDefaultInfoNotify);	

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CDojoInfoGui::SetPageType(RwInt32 iPage)
{
	if( iPage < 0 || iPage >= NUM_TAB_PAGE )
		return;

	m_pTabButton->SelectTab(iPage);
	m_eTabPage = (eTabPage)iPage;

	for( RwUInt8 i = 0 ; i < NUM_TAB_PAGE ; ++i )
	{
		if( i == iPage )
			m_pTabPage[i]->ActiveTab(true);			
		else
			m_pTabPage[i]->ActiveTab(false);
	}	
}

VOID CDojoInfoGui::OnClicked_HelpButton(gui::CComponent* pComponent)
{
	CDboEventGenerator::OpenHelpContent( DIALOG_DOJO_INFO );
}

VOID CDojoInfoGui::OnClicked_ExitButton(gui::CComponent* pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_DOJO_INFO);
}

VOID CDojoInfoGui::OnTabChanged(RwInt32 nIndex, RwInt32 nOldIndex)
{
	if( nIndex < 0 || nIndex >= NUM_TAB_PAGE )
		return;

	m_eTabPage = (eTabPage)nIndex;

	m_pTabPage[nOldIndex]->ActiveTab(false);
	m_pTabPage[nIndex]->ActiveTab(true);
}

VOID CDojoInfoGui::OnMouseDown(const CKey& key)
{
	m_pTabPage[m_eTabPage]->OnMouseDown(key);
}

VOID CDojoInfoGui::OnMouseUp(const CKey& key)
{
	m_pTabPage[m_eTabPage]->OnMouseUp(key);
}

VOID CDojoInfoGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	m_srf3BackLine.SetPositionbyParent(rtScreen.left, rtScreen.top);

	for( RwUInt8 i = 0 ; i < NUM_TAB_PAGE ; ++i )
		m_pTabPage[i]->OnMove(iOldX, iOldY);

	MoveLinkedPLGui(rtScreen.left - iOldX, rtScreen.top - iOldY);
}

VOID CDojoInfoGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	m_pTabPage[m_eTabPage]->OnMouseMove(nFlags, nX, nY);
}

VOID CDojoInfoGui::OnMouseLeave(gui::CComponent* pComponent)
{
	m_pTabPage[m_eTabPage]->OnMouseLeave(pComponent);
}

VOID CDojoInfoGui::OnPaint()
{
	m_srf3BackLine.Render();

	m_pTabPage[m_eTabPage]->OnPaint();
}

VOID CDojoInfoGui::OnCaptureMouseDown(const CKey& key)
{
	CAPTURE_MOUSEDOWN_RAISE(DIALOG_DOJO_INFO, key.m_fX, key.m_fY);
}

RwInt32 CDojoInfoGui::SwitchDialog(bool bOpen)
{
	if( bOpen )
	{
		SERIAL_HANDLE hTarget = Logic_GetAvatarTargetHandle();
		sNPC_TBLDAT* pNPC_TBLDAT = Logic_GetNPCDataFromSob(hTarget);
		if( !pNPC_TBLDAT )
		{
			DBO_WARNING_MESSAGE("Not exist NPC TABLE of handle : " << hTarget );
			Logic_CancelNpcFacing();
			return -1;
		}

		GetDboGlobal()->GetGamePacketGenerator()->SendDojo_NPCInfoReq(hTarget);

		for(RwUInt8 i = 0 ; i < NUM_TAB_PAGE ; ++i )
			m_pTabPage[i]->SwitchDialog(true, pNPC_TBLDAT->contentsTblidx);
	}
	else
	{
		m_hNPCSerial = INVALID_SERIAL_ID;

		Logic_CancelNpcFacing();

		for(RwUInt8 i = 0 ; i < NUM_TAB_PAGE ; ++i )
			m_pTabPage[i]->SwitchDialog(false);
	}

	Show(bOpen);
	return 1;
}

VOID CDojoInfoGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CDojoInfoGui::HandleEvents");

	if( msg.Id == g_EventNPCDialogOpen )
	{
		SDboNpcDialogOpen* pEvent = reinterpret_cast<SDboNpcDialogOpen*>( msg.pData );

		if( pEvent->eDialog != DIALOG_DOJO_INFO )
			return;

		// 같은 NPC
		if( m_hNPCSerial == pEvent->hSerialId )
			return;

		// 다른 NPC와 대화를 하는 경우이다
		if( m_hNPCSerial != INVALID_SERIAL_ID )
		{
			GetDialogManager()->CloseDialog(DIALOG_DOJO_INFO);
			return;
		}

		m_hNPCSerial = pEvent->hSerialId;

		GetDialogManager()->OpenDialog(DIALOG_DOJO_INFO, m_hNPCSerial);
	}
	else if( msg.Id == g_EventCharObjDelete )
	{		
		RwUInt32* pDeleteSerial = reinterpret_cast<RwUInt32*>( msg.pData );

		if( m_hNPCSerial == *pDeleteSerial )
		{
			// NPC 서버가 다운되거나 하여 갑자기 NPC가 사라지는 경우
			GetDialogManager()->CloseDialog(DIALOG_DOJO_INFO);
		}
	}
	else if( msg.Id == g_EventDialog )
	{		
		SDboEventDialog* pEvent = (SDboEventDialog*)msg.pData;

		if( pEvent->iDestDialog == DIALOG_DOJO_INFO )
		{
			if( pEvent->iType == DIALOGEVENT_NPC_BYEBYE ||
				pEvent->iType == DIALOGEVENT_OPEN_FAIL_NPC_DIALOG )
			{
				GetDialogManager()->CloseDialog(DIALOG_DOJO_INFO);
			}
		}
	}
	else if( msg.Id == g_EventDojoNotify ||
			 msg.Id == g_EventPostDialog ||
			 msg.Id == g_EventNotifyGuild ||
			 msg.Id == g_EventScrambleDefaultInfoNotify )
	{
		for(RwUInt8 i = 0 ; i < NUM_TAB_PAGE ; ++i )
			m_pTabPage[i]->HandleEvents(msg);
	}

	NTL_RETURNVOID();
}