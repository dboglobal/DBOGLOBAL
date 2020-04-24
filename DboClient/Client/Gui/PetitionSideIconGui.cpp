#include "precomp_dboclient.h"
#include "PetitionSideIconGui.h"

// core
#include "NtlDebug.h"

// sound
#include "GUISoundDefine.h"

// presentation
#include "NtlPLGuiManager.h"

// Cleint
#include "DisplayStringManager.h"
#include "PetitionManager.h"
#include "DboGlobal.h"

//////////////////////////////////////////////////////////////////////////
// CPetitionSideIconGui
//////////////////////////////////////////////////////////////////////////

CPetitionSideIconGui::CPetitionSideIconGui(const RwChar* pName)
:CSideIconBase(pName)
{
}

CPetitionSideIconGui::~CPetitionSideIconGui()
{

}

RwBool CPetitionSideIconGui::Create()
{
	NTL_FUNCTION( "CPetitionSideIconGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\PetitionSideIcon.srf", "gui\\PetitionSideIcon.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	// 일반 아이콘 버튼
	m_pIconButton = (gui::CButton*)GetComponent("btnIcon");
	m_slotIconButton = m_pIconButton->SigClicked().Connect(this, &CPetitionSideIconGui::OnIconButtonClicked);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CPetitionSideIconGui::Destroy()
{
	NTL_FUNCTION("CPetitionSideIconGui::Destroy");

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID  CPetitionSideIconGui::Update(RwReal fElapsed)
{
	if( IsShow() == FALSE )
		return;
}

void CPetitionSideIconGui::OnSideViewClosed()
{
}

void CPetitionSideIconGui::OnIconButtonClicked(gui::CComponent* pComponent)
{
	CSideIconGui::GetInstance()->OpenSideView(this, SIDEVIEW_PETITION, NULL);
}

VOID CPetitionSideIconGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CPetitionSideIconGui::HandleEvents");

	NTL_RETURNVOID();
}


//////////////////////////////////////////////////////////////////////////
// CPetitionSideViewGui
//////////////////////////////////////////////////////////////////////////

CPetitionSideViewGui::CPetitionSideViewGui(const RwChar* pName)
:CSideViewBase(pName)
{
}

CPetitionSideViewGui::~CPetitionSideViewGui()
{

}

RwBool CPetitionSideViewGui::Create()
{
	NTL_FUNCTION( "CPetitionSideViewGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\PetitionSideView.srf", "gui\\PetitionSideView.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	CRectangle rect;

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	// 닫기 버튼
	m_pExitButton = (gui::CButton*)GetComponent("btnExit");
	m_slotExitButton = m_pExitButton->SigClicked().Connect(this, &CPetitionSideViewGui::OnExitClicked);

	m_pMessage = (gui::CStaticBox*)GetComponent("stbMessage");
	m_pMessage->SetText( GetDisplayStringManager()->GetString("DST_PETITION_SIDEVIEW_MESSAGE") );

	// 배경
	m_BackPanel.SetType(CWindowby3::WT_HORIZONTAL);
	m_BackPanel.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("PetitionSideView.srf", "srfBackUp"));
	m_BackPanel.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("PetitionSideView.srf", "srfBackCenter"));
	m_BackPanel.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("PetitionSideView.srf", "srfBackDown"));
	m_BackPanel.SetSize( m_pThis->GetWidth(), m_pThis->GetHeight() );

	// 취소 버튼
	m_pCancelButton = (gui::CButton*)GetComponent("btnCancel");
	m_pCancelButton->SetTextUpColor(RGB(0, 255, 255));
	m_pCancelButton->SetText( GetDisplayStringManager()->GetString("DST_PETITION_CANCEL_PETITION") );
	m_slotCancelButton = m_pCancelButton->SigClicked().Connect(this, &CPetitionSideViewGui::OnCancelClicked);

	// sig	
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CPetitionSideViewGui::OnPaint );

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CPetitionSideViewGui::Destroy()
{
	NTL_FUNCTION("CPetitionSideViewGui::Destroy");

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

void CPetitionSideViewGui::OnPressESC()
{

}

void CPetitionSideViewGui::OnSideViewOpen(const void* pData)
{
	Logic_PlayGUISound(GSD_SYSTEM_WINDOW_ALREAT);
	Show(true);
}

void CPetitionSideViewGui::OnSideViewClose()
{
	Show(false);
}

void CPetitionSideViewGui::OnSideViewLocate(const CRectangle& rectSideIcon)
{
	CRectangle rtScreen;

	rtScreen.SetRectWH(rectSideIcon.left - m_BackPanel.GetWidth() + rectSideIcon.GetWidth(),
					   rectSideIcon.top - m_BackPanel.GetHeight(),
					   m_BackPanel.GetWidth(),
					   m_BackPanel.GetHeight() );

	m_pThis->SetPosition(rtScreen);
	m_BackPanel.SetRect(rtScreen);
}

VOID CPetitionSideViewGui::OnExitClicked(gui::CComponent* pComponent)
{
	Show(false);
}

VOID CPetitionSideViewGui::OnCancelClicked(gui::CComponent* pComponent)
{
	PETITIONID petitionID = GetPetitionManager()->GetPetitionID();
	GetDboGlobal()->GetChatPacketGenerator()->SendPetitionUserCancelReq(petitionID);
}

VOID CPetitionSideViewGui::OnPaint()
{
	m_BackPanel.Render();
}

VOID CPetitionSideViewGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CPetitionSideViewGui::HandleEvents");

	NTL_RETURNVOID();
}