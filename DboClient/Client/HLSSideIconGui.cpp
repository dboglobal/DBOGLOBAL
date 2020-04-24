#include "precomp_dboclient.h"
#include "HLSSideIconGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// dbo
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DboPacketGenerator.h"
#include "DialogManager.h"
#include "DboGlobal.h"


CHLSSideIconGui::CHLSSideIconGui(const RwChar* pName)
	: CSideIconBase(pName)
	, m_pBtnHLS(NULL)
{
}

CHLSSideIconGui::~CHLSSideIconGui(void)
{
}

RwBool CHLSSideIconGui::Create()
{
	NTL_FUNCTION(__FUNCTION__);

	if (!CNtlPLGui::Create("gui\\HLS.rsr", "gui\\HLS.srf", "gui\\HLSSideIcon.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pBtnHLS = (gui::CButton*)GetComponent("btnHLSSideIcon");
	m_slotHLSBtn = m_pBtnHLS->SigClicked().Connect(this, &CHLSSideIconGui::OnIconButtonClicked);
	m_slotHLSMouseEnter = m_pBtnHLS->SigMouseEnter().Connect(this, &CHLSSideIconGui::OnMouseEnter);
	m_slotHLSMouseLeave = m_pBtnHLS->SigMouseLeave().Connect(this, &CHLSSideIconGui::OnMouseLeave);

	Show(true);

	NTL_RETURN(TRUE);
}

VOID CHLSSideIconGui::Destroy()
{
	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
	Show(false);
}

VOID CHLSSideIconGui::HandleEvents(RWS::CMsg &msg)
{
}

VOID CHLSSideIconGui::OnIconButtonClicked(gui::CComponent* pComponent)
{
	if (GetDialogManager()->IsOpenDialog(DIALOG_HLSHOP) == FALSE)
		GetDboGlobal()->GetGamePacketGenerator()->SendCashItemHLShopStartReq();
}

VOID CHLSSideIconGui::OnSideViewClosed()
{
}

void CHLSSideIconGui::OnMouseEnter(gui::CComponent* pComponent)
{
	CSideIconGui::GetInstance()->OpenSideView(this, SIDEVIEW_HLS, NULL);
}

void CHLSSideIconGui::OnMouseLeave(gui::CComponent* pComponent)
{
	CSideIconGui::GetInstance()->CloseSideView(SIDEVIEW_HLS);
}

void CHLSSideIconGui::Show(bool bShow)
{
	__super::Show(bShow);
}