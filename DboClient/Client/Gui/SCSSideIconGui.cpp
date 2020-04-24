#include "precomp_dboclient.h"

//core
#include "NtlDebug.h"
#include "ceventhandler.h"

//gui
#include "gui_define.h"

//presentation
#include "NtlPLGui.h"
#include "NtlPLGuiManager.h"

// framework
#include "NtlApplication.h"


// table
#include "HelpTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"

// dbo
#include "DboGlobal.h"
#include "DboDef.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DialogManager.h"
#include "SCSSideIconGui.h"

CSCSSideIconGui::CSCSSideIconGui(const RwChar* pName)
:CSideIconBase(pName)
{

}

CSCSSideIconGui::~CSCSSideIconGui()
{

}

RwBool CSCSSideIconGui::Create( )
{
	NTL_FUNCTION( "CGMSideIconGui::Create" );

	if(!CNtlPLGui::Create("gui\\Observer.rsr", "gui\\GMSideIcon.srf", "gui\\GMSideIcon.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_pIconButton = (gui::CButton*)GetComponent("btnIcon");
	m_slotIconButton = m_pIconButton->SigClicked().Connect(this, &CSCSSideIconGui::OnIconButtonClicked);

	Show(false);	

	NTL_RETURN(TRUE);
}

void CSCSSideIconGui::Destroy( )
{
	NTL_FUNCTION("CGMSideIconGui::Destroy");

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CSCSSideIconGui::OnSideViewClosed( )
{

}

VOID CSCSSideIconGui::OnIconButtonClicked(gui::CComponent* pComponent)
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
// CSCSSideViewGui
//////////////////////////////////////////////////////////////////////////

CSCSSideViewGui::CSCSSideViewGui(const RwChar* pName)
:CSideViewBase(pName)
{
}

CSCSSideViewGui::~CSCSSideViewGui()
{

}

RwBool CSCSSideViewGui::Create()
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

VOID CSCSSideViewGui::Destroy()
{
	NTL_FUNCTION("CGMSideViewGui::Destroy");

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CSCSSideViewGui::OnPressESC()
{

}


void CSCSSideViewGui::OnSideViewOpen(const void* pData)
{
	Show(true);	
}

VOID CSCSSideViewGui::OnSideViewClose()
{

}

VOID CSCSSideViewGui::OnSideViewLocate(const CRectangle& rectSideIcon)
{

}