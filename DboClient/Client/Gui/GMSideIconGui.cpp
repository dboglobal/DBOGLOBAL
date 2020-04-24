#include "precomp_dboclient.h"
#include "GMSideIconGui.h"

// core
#include "NtlDebug.h"

// sound
#include "GUISoundDefine.h"

// presentation
#include "NtlPLGuiManager.h"

// dbo
#include "DisplayStringManager.h"
#include "DboEventGenerator.h"
#include "DialogManager.h"
#include "DboGlobal.h"


//////////////////////////////////////////////////////////////////////////
// CGMSideIconGui
//////////////////////////////////////////////////////////////////////////

CGMSideIconGui::CGMSideIconGui(const RwChar* pName)
:CSideIconBase(pName)
{
}

CGMSideIconGui::~CGMSideIconGui()
{

}

RwBool CGMSideIconGui::Create()
{
	NTL_FUNCTION( "CGMSideIconGui::Create" );

	if(!CNtlPLGui::Create("gui\\Observer.rsr", "gui\\GMSideIcon.srf", "gui\\GMSideIcon.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_pIconButton = (gui::CButton*)GetComponent("btnIcon");
	if (m_pIconButton)
	{
		m_slotIconButton = m_pIconButton->SigClicked().Connect(this, &CGMSideIconGui::OnIconButtonClicked);
	}
	else
	{
		DBO_WARNING_MESSAGE("CGMSideIconGui::Create m_pIconButton == NULL");
		NTL_RETURN(FALSE);
	}

	Show(false);	

	NTL_RETURN(TRUE);
}

VOID CGMSideIconGui::Destroy()
{
	NTL_FUNCTION("CGMSideIconGui::Destroy");

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

void CGMSideIconGui::OnSideViewClosed()
{
}

void CGMSideIconGui::OnIconButtonClicked(gui::CComponent* pComponent)
{
	// ¿­°í ´Ý±â
	if( GetDialogManager()->GetDialog(DIALOG_GM_QUICKSLOT) )
	{
		GetDialogManager()->CloseGMDialog();
		CDboEventGenerator::ObserverMenu(OBSERVER_END_OBSERVER);		

		GetDialogManager()->OpenDialog(DIALOG_QUICKSLOT);
		GetDialogManager()->OpenDialog(DIALOG_BAGSLOT);
		GetDialogManager()->OpenDialog(DIALOG_EXP);
	}
	else
	{	
		GetDialogManager()->CloseDialog(DIALOG_QUICKSLOT);
		GetDialogManager()->CloseDialog(DIALOG_BAGSLOT);
		GetDialogManager()->CloseDialog(DIALOG_EXP);

		CDboEventGenerator::ObserverMenu(OBSERVER_START_OBSERVER);
		GetDialogManager()->OpenGMDialog();
		GetDialogManager()->LocationDialogs(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());
	}
}

//////////////////////////////////////////////////////////////////////////
// CGMSideViewGui
//////////////////////////////////////////////////////////////////////////

CGMSideViewGui::CGMSideViewGui(const RwChar* pName)
:CSideViewBase(pName)
{
}

CGMSideViewGui::~CGMSideViewGui()
{

}

RwBool CGMSideViewGui::Create()
{
	NTL_FUNCTION( "CGMSideViewGui::Create" );

	if(!CNtlPLGui::Create("", "", "gui\\GMSideView.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	CRectangle rect;

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CGMSideViewGui::Destroy()
{
	NTL_FUNCTION("CGMSideViewGui::Destroy");

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CGMSideViewGui::OnPressESC()
{

}


void CGMSideViewGui::OnSideViewOpen(const void* pData)
{
	Show(true);	
}

VOID CGMSideViewGui::OnSideViewClose()
{

}

VOID CGMSideViewGui::OnSideViewLocate(const CRectangle& rectSideIcon)
{

}