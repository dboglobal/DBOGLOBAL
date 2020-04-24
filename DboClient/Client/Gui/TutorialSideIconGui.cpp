#include "precomp_dboclient.h"
#include "TutorialSideIconGui.h"

// core
#include "NtlDebug.h"

// sound
#include "GUISoundDefine.h"

// presentation
#include "NtlPLGuiManager.h"

// dbo
#include "DisplayStringManager.h"
#include "DboEventGenerator.h"
#include "AlarmManager.h"

//////////////////////////////////////////////////////////////////////////
// CTutorialSideIconGui
//////////////////////////////////////////////////////////////////////////

CTutorialSideIconGui::CTutorialSideIconGui(const RwChar* pName)
:CSideIconBase(pName)
{
}

CTutorialSideIconGui::~CTutorialSideIconGui()
{

}

RwBool CTutorialSideIconGui::Create()
{
	NTL_FUNCTION( "CTutorialSideIconGui::Create" );

	if(!CNtlPLGui::Create("gui\\Tutorial.rsr", "gui\\TutorialSideIcon.srf", "gui\\TutorialSideIcon.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_pIconButton = (gui::CButton*)GetComponent("btnIcon");
	m_slotIconButton = m_pIconButton->SigClicked().Connect(this, &CTutorialSideIconGui::OnIconButtonClicked);

	Show(false);	

	NTL_RETURN(TRUE);
}

VOID CTutorialSideIconGui::Destroy()
{
	NTL_FUNCTION("CTutorialSideIconGui::Destroy");

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

void CTutorialSideIconGui::OnSideViewClosed()
{
}

void CTutorialSideIconGui::OnIconButtonClicked(gui::CComponent* pComponent)
{	
	// 튜토리얼을 종료하시겠습니까?
	//CDboEventGenerator::MsgBoxShow(GetDisplayStringManager()->GetString("DST_TUTORIAL_ASK_END"), MBW_TUTORIAL_ASK_END, MBTF_OK | MBTF_CANCEL);
	GetAlarmManager()->AlarmMessage( "DST_TUTORIAL_ASK_END" );
}

//////////////////////////////////////////////////////////////////////////
// CTutorialSideViewGui
//////////////////////////////////////////////////////////////////////////

CTutorialSideViewGui::CTutorialSideViewGui(const RwChar* pName)
:CSideViewBase(pName)
{
}

CTutorialSideViewGui::~CTutorialSideViewGui()
{

}

RwBool CTutorialSideViewGui::Create()
{
	NTL_FUNCTION( "CTutorialSideViewGui::Create" );

	if(!CNtlPLGui::Create("", "", "gui\\TutorialSideView.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	CRectangle rect;

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CTutorialSideViewGui::Destroy()
{
	NTL_FUNCTION("CTutorialSideViewGui::Destroy");

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CTutorialSideViewGui::OnPressESC()
{

}


void CTutorialSideViewGui::OnSideViewOpen(const void* pData)
{
	Show(true);	
}

VOID CTutorialSideViewGui::OnSideViewClose()
{

}

VOID CTutorialSideViewGui::OnSideViewLocate(const CRectangle& rectSideIcon)
{

}