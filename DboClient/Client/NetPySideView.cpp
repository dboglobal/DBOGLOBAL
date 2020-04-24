#include "precomp_dboclient.h"
#include "NetPySideView.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// sl
#include "NtlSLLogic.h"

// dbo
#include "DisplayStringManager.h"
#include "DialogDefine.h"


#define NOTIFY_SHOWTIME_MAX     5.0f                        ///< Notify 정보를 보여줄 시간 (초)


CNetPySideViewGui::CNetPySideViewGui(const RwChar* pName)
: CSideViewBase(pName)
{
    ZeroMemory(&m_UpdateNetPyInfo, sizeof(m_UpdateNetPyInfo));
    m_dwUpdateNetPyPoint = 0;
    m_fRemainTime = 0.0f;
    //m_type = E_NETPY_INFO;
    m_type = E_NETPY_NOTIFY;
    m_fNotifyShowTime = 0.0f;
    m_bIsPCBang = FALSE;
}

CNetPySideViewGui::~CNetPySideViewGui(void)
{
}

RwBool CNetPySideViewGui::Create() 
{
    if(!CNtlPLGui::Create("", "gui\\NetPySideView.srf", "gui\\NetPySideView.frm"))
        return FALSE;

    CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());
    
    m_pThis = (gui::CDialog*)GetComponent("dlgMain");

    // 배경
    m_BackPanel.SetType(CWindowby3::WT_HORIZONTAL);    
    m_BackPanel.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("NetPySideView.srf", "srfDialogBackUp"));
    m_BackPanel.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("NetPySideView.srf", "srfDialogBackCenter"));
    m_BackPanel.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("NetPySideView.srf", "srfDialogBackDown"));	    

    // sig	
    m_slotPaint			= m_pThis->SigPaint().Connect( this, &CNetPySideViewGui::OnPaint );
    m_slotMove			= m_pThis->SigMove().Connect(this, &CNetPySideViewGui::OnMove);
    m_slotResize		= m_pThis->SigMove().Connect(this, &CNetPySideViewGui::OnResize);

    //////////////////////////////////////////////////////////////////////////
    // 현재 NetPy 정보
    CRectangle rect;
    rect.SetRectWH(20, 10, 250, 14);
    // 다음 NetPy 획득까지 남은 시간
    m_pstbRemainTimeTitle = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
    m_pstbRemainTimeTitle->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);	        
    m_pstbRemainTimeTitle->SetTextColor(INFOCOLOR_13);
    m_pstbRemainTimeTitle->SetText(GetDisplayStringManager()->GetString("DST_NETPY_REMAIN_TIME"));
    

    rect.SetRectWH(20, 26, 250, 14);
    m_pstbRemainTimeData = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
    m_pstbRemainTimeData->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);	        
    m_pstbRemainTimeData->SetTextColor(RGB(255, 255, 255));    
    

    // 누적 획득 넷피
    rect.SetRectWH(20, 50, 250, 14);
    m_pstbGetTitle = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
    m_pstbGetTitle->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);	        
    m_pstbGetTitle->SetTextColor(INFOCOLOR_13);
    m_pstbGetTitle->SetText(GetDisplayStringManager()->GetString("DST_NETPY_GET_NETPY"));
    

    rect.SetRectWH(20, 66, 250, 14);
    m_pstbGetData = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
    m_pstbGetData->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);	        
    m_pstbGetData->SetTextColor(RGB(255, 255, 255));    
    

    // 현재 넷피
    rect.SetRectWH(20, 90, 250, 14);
    m_pstbCurrentTitle = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
    m_pstbCurrentTitle->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);	        
    m_pstbCurrentTitle->SetTextColor(INFOCOLOR_13);
    m_pstbCurrentTitle->SetText(GetDisplayStringManager()->GetString("DST_NETPY_CURRENT_NETPY"));
    

    rect.SetRectWH(20, 106, 250, 14);
    m_pstbCurrentData = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
    m_pstbCurrentData->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);	            
    m_pstbCurrentData->SetTextColor(RGB(255, 255, 255));
    
    //////////////////////////////////////////////////////////////////////////
    // NetpY 획득 정보

    rect.SetRectWH(20, 10, 250, 14);
    m_pstbGetPoint = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
    m_pstbGetPoint->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);	        
    m_pstbGetPoint->SetTextColor(INFOCOLOR_13);

    LinkMsg(g_EventUpdateNetPy);    
    LinkMsg(g_EventNetMarbleMemberShipNfy);

    GetNtlGuiManager()->AddUpdateFunc(this);

    Show(false);

    return TRUE;
}

VOID CNetPySideViewGui::Destroy() 
{
    GetNtlGuiManager()->RemoveUpdateFunc(this);

    UnLinkMsg(g_EventUpdateNetPy);    
    UnLinkMsg(g_EventNetMarbleMemberShipNfy);

    CNtlPLGui::DestroyComponents();
    CNtlPLGui::Destroy(); 
}
VOID CNetPySideViewGui::OnPressESC() 
{

}
VOID CNetPySideViewGui::OnSideViewOpen( const void* pData ) 
{
    eNetPySideViewType type = (eNetPySideViewType)(RwInt32)pData;    
    SetState(type);

    Show(true);
}

VOID CNetPySideViewGui::OnSideViewClose() 
{
    Show(false);
}

VOID CNetPySideViewGui::OnSideViewLocate( const CRectangle& rectSideIcon ) 
{
    RwInt32 iHeight = m_bIsPCBang? NETPY_SIDEVIEW_HEIGHT_PCBANG : NETPY_SIDEVIEW_HEIGHT_NORMAL;
    m_pThis->SetHeight(iHeight);
    LocateComponent();
    m_pThis->SetPosition(rectSideIcon.left - m_pThis->GetWidth() + rectSideIcon.GetWidth(), rectSideIcon.top - iHeight);
}

VOID CNetPySideViewGui::HandleEvents( RWS::CMsg &msg ) 
{
    if(msg.Id == g_EventUpdateNetPy)
    {
        SDboEventUpdateNetPy* pData = (SDboEventUpdateNetPy*)msg.pData;        
        memcpy_s(&m_UpdateNetPyInfo, sizeof(SDboEventUpdateNetPy), pData, sizeof(SDboEventUpdateNetPy));        
        m_fRemainTime = (RwReal)m_UpdateNetPyInfo.timeNextGainTime; // 단위 초
        
        // NetPy 획득이면
        if(pData->netPy > Logic_GetNetPy())
        {
            m_dwUpdateNetPyPoint = pData->netPy - Logic_GetNetPy();
            SetState(E_NETPY_NOTIFY);
            Show(true);
        }
    }
    else if(msg.Id == g_EventNetMarbleMemberShipNfy)
    {
        SetState(E_NETPY_INFO);
        m_bIsPCBang = TRUE;
    }
}

VOID CNetPySideViewGui::LocateComponent() 
{
    m_BackPanel.SetRect( m_pThis->GetScreenRect() );
}

VOID CNetPySideViewGui::OnMove( RwInt32 iOldX, RwInt32 iOldY ) 
{
    LocateComponent();
}

VOID CNetPySideViewGui::OnResize( RwInt32 iOldW, RwInt32 iOldH ) 
{
    LocateComponent();
}

VOID CNetPySideViewGui::OnPaint() 
{
    m_BackPanel.Render();
}

VOID CNetPySideViewGui::SetState( eNetPySideViewType type ) 
{
    m_pstbRemainTimeTitle->Show(false);
    m_pstbRemainTimeData->Show(false);
    m_pstbGetTitle->Show(false);
    m_pstbGetData->Show(false);
    m_pstbCurrentTitle->Show(false);
    m_pstbCurrentData->Show(false);
    m_pstbGetPoint->Show(false);

    switch(type)
    {
    case E_NETPY_INFO:
        OnShowInfo();
        break;
    case E_NETPY_NOTIFY:
        OnShowNotify();
        break;
    }

    m_type = type;
}

VOID CNetPySideViewGui::OnShowInfo() 
{
    m_pstbCurrentTitle->Show(true);
    m_pstbCurrentData->Show(true);    

    if(m_bIsPCBang)
    {
        m_pstbRemainTimeTitle->Show(true);
        m_pstbRemainTimeData->Show(true);
        m_pstbGetTitle->Show(true);
        m_pstbGetData->Show(true);        

        m_pThis->SetHeight(NETPY_SIDEVIEW_HEIGHT_PCBANG);            

        // 남은 시간
        WCHAR wcRemainTime[128] = {0,};
        if(m_UpdateNetPyInfo.timeNextGainTime > 60) // 분단위
        {
            swprintf_s(wcRemainTime, L"%d %s", m_UpdateNetPyInfo.timeNextGainTime / 60, GetDisplayStringManager()->GetString("DST_TIME_MINUTE"));
        }
        else // 초 단위
        {
            swprintf_s(wcRemainTime, L"%d %s", m_UpdateNetPyInfo.timeNextGainTime, GetDisplayStringManager()->GetString("DST_TIME_SECOND"));
        }
        m_pstbRemainTimeData->SetText(wcRemainTime);

        // 획득 넷피
        WCHAR wcAccumNetPy[128] = {0,};
        swprintf_s(wcAccumNetPy, L"%d %s", m_UpdateNetPyInfo.dwAccumlationNetPy, GetDisplayStringManager()->GetString("DST_NETPY_POINT"));
        m_pstbGetData->SetText(wcAccumNetPy);

        m_pstbCurrentTitle->SetPosition(20, 90);
        m_pstbCurrentData->SetPosition(20, 106);
    }
    else
    {
        m_pThis->SetHeight(NETPY_SIDEVIEW_HEIGHT_NORMAL);
        m_pstbCurrentTitle->SetPosition(20, 10);
        m_pstbCurrentData->SetPosition(20, 26);
    }

    LocateComponent();

    // 총 넷피
    WCHAR wcTotalNetPy[128] = {0,};
    swprintf_s(wcTotalNetPy, L"%d %s", Logic_GetNetPy(), GetDisplayStringManager()->GetString("DST_NETPY_POINT"));
    m_pstbCurrentData->SetText(wcTotalNetPy);
}

VOID CNetPySideViewGui::OnShowNotify() 
{
    m_pstbGetPoint->Show(true);

    m_pThis->SetHeight(NETPY_SIDEVIEW_HEIGHT_NFY);

    LocateComponent();
    
    // 메시지
    WCHAR wcNotify[128] = {0,};
    swprintf_s(wcNotify, GetDisplayStringManager()->GetString("DST_NETPY_GET_NOTIFY"), m_dwUpdateNetPyPoint);
    m_pstbGetPoint->SetText(wcNotify);
}

VOID CNetPySideViewGui::Update( RwReal fElapsed ) 
{
    // 남은 시간 계산
    if(m_UpdateNetPyInfo.timeNextGainTime > 0 && m_fRemainTime > 0.0f)
    {
        m_fRemainTime -= fElapsed;
        m_fRemainTime = max(0.0f, m_fRemainTime);

        m_UpdateNetPyInfo.timeNextGainTime = (DWORD)m_fRemainTime;

        if(m_pstbCurrentData->IsVisible())  // Info 정보 표시중이면 갱신한다.
        {
            OnShowInfo();
        }        
    }    

    if(m_pstbGetPoint->IsVisible())
    {
        m_fNotifyShowTime += fElapsed;

        if(m_fNotifyShowTime > NOTIFY_SHOWTIME_MAX)
        {
            Show(FALSE);
            m_fNotifyShowTime = 0.0f;
        }
    }
}