#include "precomp_dboclient.h"
#include "DeveloperPreviewGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// cleint
#include "DialogManager.h"


CDeveloperPreviewGui::CDeveloperPreviewGui(const RwChar* pName)
:CNtlPLGui(pName)
{

}

CDeveloperPreviewGui::~CDeveloperPreviewGui()
{	

}

RwBool CDeveloperPreviewGui::Create()
{
	NTL_FUNCTION( "CDeveloperPreviewGui::Create" );

	if(!CNtlPLGui::Create("gui\\DeveloperPreview.rsr", "gui\\DeveloperPreview.srf", "gui\\DeveloperPreview.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_FLASH_PROLOG);

	NTL_RETURN(TRUE);
}

VOID CDeveloperPreviewGui::Destroy()
{
	NTL_FUNCTION("CDeveloperPreviewGui::Destroy");

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CDeveloperPreviewGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CDeveloperPreviewGui::HandleEvents");

	NTL_RETURNVOID();
}