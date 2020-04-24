#include "precomp_dboclient.h"
#include "GroupChatGui.h"

// core
#include "NtlDebug.h"


CGroupChatGui::CGroupChatGui(const RwChar* pName)
:CNtlPLGui( pName )
{

}

CGroupChatGui::~CGroupChatGui()
{

}

RwBool CGroupChatGui::Create()
{
	NTL_FUNCTION( "CGroupChatGui::Create" );
/*
	if(!CNtlPLGui::Create("", "", "gui\\Scouter.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	Show(false);*/

	NTL_RETURN(TRUE);
}

VOID CGroupChatGui::Destroy()
{
	NTL_FUNCTION("CGroupChatGui::Destroy");

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

RwInt32 CGroupChatGui::SwitchDialog(bool bOpen)
{
	Show(bOpen);
	return 1;
}

VOID CGroupChatGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION( "CGroupChatGui::HandleEvents" );

	NTL_RETURNVOID();
}