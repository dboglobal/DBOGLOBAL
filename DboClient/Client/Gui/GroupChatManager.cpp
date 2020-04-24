#include "precomp_dboclient.h"
#include "GroupChatManager.h"

// core
#include "NtlDebug.h"

// presetation
#include "NtlPLEvent.h"
#include "NtlPLGuiManager.h"

// cleint
#include "DboLogic.h"
#include "ChatDefine.h"

CGroupChatManager::CGroupChatManager(const RwChar* pName)
:CNtlPLGui(pName)
{
}

CGroupChatManager::~CGroupChatManager()
{

}

RwBool CGroupChatManager::Create()
{
	NTL_FUNCTION( "CGroupChatManager::Create" );

	m_pThis			= NTL_NEW gui::CDialog(CRectangle(0, 0, 0, 0), GetNtlGuiManager()->GetGuiManager(), GetNtlGuiManager()->GetSurfaceManager());

	LinkMsg(g_EventResize);

	NTL_RETURN(TRUE);
}

VOID CGroupChatManager::Destroy()
{
	NTL_FUNCTION("CGroupChatManager::Destroy");

	UnLinkMsg(g_EventResize);

	NTL_RETURNVOID();
}

VOID CGroupChatManager::AddText(RwUInt8 byChatType, const WCHAR* pwcText)
{
	
}

RwInt32 CGroupChatManager::SwitchDialog( bool bOpen )
{
	Show(bOpen);
	return 1;
}

VOID CGroupChatManager::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CGroupChatManager::HandleEvents");

	if( msg.Id == g_EventResize )
	{
		Logic_LocateDialog_in_CleintRect(this);
	}

	NTL_RETURNVOID();
}