#include "precomp_dboclient.h"
#include "CommuTargetGui.h"

// PL
#include "NtlPLGuiManager.h"

// SL


// DBO
#include "DboGlobal.h"
#include "DisplayStringManager.h"

CCommuTargetGui::CCommuTargetGui( void ) 
{

}
CCommuTargetGui::CCommuTargetGui( const RwChar* pName ) 
: CNtlPLGui(pName)
{
    Init();
}

CCommuTargetGui::~CCommuTargetGui( void ) 
{

}

VOID CCommuTargetGui::Init() 
{    
    ZeroMemory(&m_currTarget, sizeof(m_currTarget));
}

RwBool CCommuTargetGui::Create() 
{
    if(!CNtlPLGui::Create("", "gui\\CommuTarget.srf", "gui\\CommuTarget.frm"))
        return FALSE;
    
    CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

    m_pThis         = (gui::CDialog*)GetComponent("dlgMain");
    m_pSttName      = (gui::CStaticBox*)GetComponent("sttName");
    m_pSttType      = (gui::CStaticBox*)GetComponent("sttType");

    LinkMsg(g_EventCommuTargetSelect);
    LinkMsg(g_EventCommuTargetRelease);
    LinkMsg(g_EventSobTargetSelect);
	LinkMsg(g_EventChangeCharName);

    Show(FALSE);

    return TRUE;
}

VOID CCommuTargetGui::Destroy() 
{
    UnLinkMsg(g_EventCommuTargetSelect);
    UnLinkMsg(g_EventCommuTargetRelease);
    UnLinkMsg(g_EventSobTargetSelect);
	UnLinkMsg(g_EventChangeCharName);
    
    CNtlPLGui::DestroyComponents();
    CNtlPLGui::Destroy();
}

RwInt32 CCommuTargetGui::SwitchDialog( bool bOpen ) 
{
    Show(bOpen);

    return TRUE;
}

VOID CCommuTargetGui::HandleEvents( RWS::CMsg &pMsg ) 
{
    if(pMsg.Id == g_EventCommuTargetSelect)
    {
        OnEventTargetSelect((SNtlEventCommuTargetSelect*)pMsg.pData);
    }
    else if(pMsg.Id == g_EventCommuTargetRelease)
    {
        OnEventTargetRelease();
    }
    else if(pMsg.Id == g_EventSobTargetSelect)      // When you select a general target
    {
        OnEventTargetRelease();
    }
	else if (pMsg.Id == g_EventChangeCharName)
	{
		SNtlEventChangeCharName* pEvent = reinterpret_cast<SNtlEventChangeCharName*>(pMsg.pData);

		if (IsShow())
		{
			if (m_pSttName->IsVisible() && wcscmp(m_pSttName->GetText().c_str(), pEvent->wchOldCharName) == 0)
			{
				m_pSttName->SetText(pEvent->wchCharName);
			}
		}
	}
}

VOID CCommuTargetGui::OnEventTargetSelect( SNtlEventCommuTargetSelect* data ) 
{
    if(data->uiSerialID == INVALID_SERIAL_ID)
    {
        OnEventTargetRelease();
        return;
    }

    m_pSttName->SetText(data->wchName);    
    switch(data->eTargetType)       // 타입에 따른 텍스트 설정
    {
    case COMMU_TARGET_FRIEND:
        m_pSttType->SetText(GetDisplayStringManager()->GetString("DST_COMMU_TARGET_FRIEND"));
        break;
    case COMMU_TARGET_GUILD:
        m_pSttType->SetText(GetDisplayStringManager()->GetString("DST_COMMU_TARGET_GUILD"));
        break;
    case COMMU_TARGET_PARTY:
        m_pSttType->SetText(GetDisplayStringManager()->GetString("DST_COMMU_TARGET_PARTY"));
        break;
    }
    
    SwitchDialog(true);

    m_currTarget = *data;
    GetDboGlobal()->SetCommunityTarget(data->uiCharID);
}

VOID CCommuTargetGui::OnEventTargetRelease() 
{
    m_pSttName->Clear();
    m_pSttType->Clear();

    SwitchDialog(false);

    ZeroMemory(&m_currTarget, sizeof(m_currTarget));
    GetDboGlobal()->SetCommunityTarget(INVALID_SERIAL_ID);
}
