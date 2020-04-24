#include "precomp_dboclient.h"
#include "HLSSideView.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// sl
#include "NtlSLLogic.h"

// dbo
#include "DisplayStringManager.h"
#include "DialogDefine.h"



CHLSSideViewGui::CHLSSideViewGui(const RwChar* pName)
	: CSideViewBase(pName)
{
}

CHLSSideViewGui::~CHLSSideViewGui(void)
{
}

RwBool CHLSSideViewGui::Create()
{
	if (!CNtlPLGui::Create("", "gui\\HLSSideView.srf", "gui\\HLSSideView.frm"))
		return FALSE;

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	// background
	m_BackPanel.SetType(CWindowby3::WT_HORIZONTAL);
	m_BackPanel.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLSSideView.srf", "srfDialogBackUp"));
	m_BackPanel.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLSSideView.srf", "srfDialogBackCenter"));
	m_BackPanel.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HLSSideView.srf", "srfDialogBackDown"));

	// sig	
	m_slotPaint = m_pThis->SigPaint().Connect(this, &CHLSSideViewGui::OnPaint);
	m_slotMove = m_pThis->SigMove().Connect(this, &CHLSSideViewGui::OnMove);
	m_slotResize = m_pThis->SigMove().Connect(this, &CHLSSideViewGui::OnResize);

	CRectangle rect;
	rect.SetRect(0, 17, 200, 31);

	m_pstbTitle = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER);
	m_pstbTitle->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pstbTitle->SetTextColor(INFOCOLOR_0);
	m_pstbTitle->SetText(GetDisplayStringManager()->GetString("DST_HLS_TITLE"));

	Show(false);

	m_pThis->SetHeight(60);

	return TRUE;
}

VOID CHLSSideViewGui::Destroy()
{
	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}
VOID CHLSSideViewGui::OnPressESC()
{

}
VOID CHLSSideViewGui::OnSideViewOpen(const void* pData)
{
	Show(true);
}

VOID CHLSSideViewGui::OnSideViewClose()
{
	Show(false);
}

VOID CHLSSideViewGui::OnSideViewLocate(const CRectangle& rectSideIcon)
{
	m_pThis->SetPosition(rectSideIcon.left - m_pThis->GetWidth() + rectSideIcon.GetWidth(), rectSideIcon.top - m_pThis->GetHeight());
}

VOID CHLSSideViewGui::HandleEvents(RWS::CMsg &msg)
{
}

VOID CHLSSideViewGui::LocateComponent()
{
	m_BackPanel.SetRect(m_pThis->GetScreenRect());
}

VOID CHLSSideViewGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	LocateComponent();
}

VOID CHLSSideViewGui::OnResize(RwInt32 iOldW, RwInt32 iOldH)
{
	LocateComponent();
}

VOID CHLSSideViewGui::OnPaint()
{
	m_BackPanel.Render();
}