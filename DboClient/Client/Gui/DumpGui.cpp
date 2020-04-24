#include "precomp_dboclient.h"

// core
#include "DumpGui.h"
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// dbo
#include "DboGlobal.h"


CDumpGui::CDumpGui()
{
}

CDumpGui::CDumpGui(const RwChar *pName)
:CNtlPLGui(pName)
{
}

CDumpGui::~CDumpGui()
{
}

RwBool CDumpGui::Create(void)
{
	NTL_FUNCTION("CDumpGui::Create");

	if(!CNtlPLGui::Create("", "gui\\Dump.srf", "gui\\Dump.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_poutDisplay = (gui::COutputBox*)GetComponent("outDisplay");
		
	NTL_RETURN(TRUE);
}

void CDumpGui::Destroy(void)
{
	NTL_FUNCTION("CDumpGui::Destroy");
	
	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}


gui::CDialog* CDumpGui::GetDialog(void)
{
	return m_pThis;
}

gui::COutputBox* CDumpGui::GetOutputBox(void)
{
	return m_poutDisplay;
}

//////////////////////////////////////////////////////////////////////////////////
// signal function
