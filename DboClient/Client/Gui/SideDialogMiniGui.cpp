#include "precomp_dboclient.h"
#include "SideDialogMiniGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"
#include "SideDialogManager.h"
#include "DialogPriority.h"

CSideDialogMiniGui::CSideDialogMiniGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_uiSideDialogType(SDIALOG_UNKNOWN)
{

}

CSideDialogMiniGui::~CSideDialogMiniGui()
{

}

RwBool CSideDialogMiniGui::Create()
{
	NTL_FUNCTION( "CSideDialogMiniGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\SideDialogMini.srf", "gui\\SideDialogMini.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_SIDE_DIALOG);

	m_pNormalizeButton = (gui::CButton*)GetComponent( "btnNormalize" );
	m_slotNormalizeButton = m_pNormalizeButton->SigClicked().Connect(this, &CSideDialogMiniGui::OnClicked_NormalizeButton);

	m_pDialogName = (gui::CStaticBox*)GetComponent( "stbDialogName" );

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CSideDialogMiniGui::Destroy()
{
	NTL_FUNCTION("CSideDialogMiniGui::Destroy");

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CSideDialogMiniGui::Active(RwUInt32 uiSideDialogType, const WCHAR* pwcDialogName)
{
	m_uiSideDialogType = uiSideDialogType;
	m_pDialogName->SetText(pwcDialogName);

	Show(true);
}

VOID CSideDialogMiniGui::Inactive()
{
	m_uiSideDialogType = SDIALOG_UNKNOWN;
	m_pDialogName->Clear();

	Show(false);
}

RwUInt32 CSideDialogMiniGui::GetSideDialogType()
{
	return m_uiSideDialogType;
}

VOID CSideDialogMiniGui::OnClicked_NormalizeButton(gui::CComponent* pComponent)
{
	NTL_ASSERT(m_uiSideDialogType != SDIALOG_UNKNOWN, "unknown Side Dialog type");

	if( GetSideDialogManager()->OpenDialog(m_uiSideDialogType) )
		Show(false);
}