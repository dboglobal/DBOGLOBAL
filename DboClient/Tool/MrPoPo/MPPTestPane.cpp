// MPPTestPane.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MrPoPo.h"
#include "MPPTestPane.h"
#include "CNtlMrPoPoMsg.h"
#include "MPPClientEvent.h"

/// 관중의상태
enum ETBCrowdStepType
{
    E_STEP_BASIC,
    E_STEP_MATCH_READY,    
    E_STEP_STAGE_READY,
    E_STEP_STAGE_RUN,
    E_STEP_STAGE_FINISH,
    E_STEP_MATCH_FINISH,
    E_STEP_AWARD,
    E_STEP_BATTLE_END,
    E_STEP_HTB,
    E_STEP_KNOCKDOWN,
    E_STEP_KO,

    E_STEP_COUNT,
    E_STEP_NONE = 0xff,
};

// CMPPTestPane

IMPLEMENT_DYNCREATE(CMPPTestPane, CXTResizeFormView)

CMPPTestPane::CMPPTestPane()
	: CXTResizeFormView(CMPPTestPane::IDD)
{

}

CMPPTestPane::~CMPPTestPane()
{
}

void CMPPTestPane::DoDataExchange(CDataExchange* pDX)
{
    CXTResizeFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CB_CROWD, m_cbCrowdTest);
    DDX_Control(pDX, IDC_ED_OBSERVER_INDEX, m_edObserverIndex);
    DDX_Control(pDX, IDC_ED_OBSERVER_LUA, m_edObserverLua);
    DDX_Control(pDX, IDC_ED_TEST1, m_edTest1);
    DDX_Control(pDX, IDC_ED_TEST2, m_edTest2);
    DDX_Control(pDX, IDC_ED_TEST3, m_edTest3);
    DDX_Control(pDX, IDC_TENKAICHI, m_cmbTenkaichi);
    DDX_Control(pDX, IDC_COLORCHANGE, m_cbColorChange);
    DDX_Control(pDX, IDC_STATECHANGE, m_cbStateChange);
    DDX_Control(pDX, IDC_TRANSFORM, m_cbTransform);
    DDX_Control(pDX, IDC_STUNTYPE, m_cbStunType);
}

BEGIN_MESSAGE_MAP(CMPPTestPane, CXTResizeFormView)
    ON_BN_CLICKED(IDC_BTN_CROWD_CHANGE, &CMPPTestPane::OnBnClickedBtnCrowdChange)    
    ON_BN_CLICKED(IDC_BT_OBSERVER_INDEX, &CMPPTestPane::OnBnClickedBtObserverIndex)
    ON_BN_CLICKED(IDC_BT_OBSERVER_LUA, &CMPPTestPane::OnBnClickedBtObserverLua)
    ON_BN_CLICKED(IDC_BT_TEST1, &CMPPTestPane::OnBnClickedBtTest1)
    ON_BN_CLICKED(IDC_BT_TEST2, &CMPPTestPane::OnBnClickedBtTest2)
    ON_BN_CLICKED(IDC_BT_TEST3, &CMPPTestPane::OnBnClickedBtTest3)
    ON_BN_CLICKED(IDC_BT_FREECAMERA, &CMPPTestPane::OnBnClickedBtFreecamera)
    ON_BN_CLICKED(IDC_BT_RENDER_SCOUTER, &CMPPTestPane::OnBnClickedBtRenderScouter)
    ON_CBN_SELCHANGE(IDC_TENKAICHI, &CMPPTestPane::OnCbnSelchangeTenkaichi)
    ON_CBN_SELCHANGE(IDC_COLORCHANGE, &CMPPTestPane::OnCbnSelchangeColorchange)
    ON_CBN_SELCHANGE(IDC_STATECHANGE, &CMPPTestPane::OnCbnSelchangeStatechange)
    ON_CBN_SELCHANGE(IDC_TRANSFORM, &CMPPTestPane::OnCbnSelchangeTransform)
    ON_CBN_SELCHANGE(IDC_STUNTYPE, &CMPPTestPane::OnCbnSelchangeStuntype)
    ON_BN_CLICKED(IDC_BT_TARGET_MARKING, &CMPPTestPane::OnBnClickedBtTargetMarking)
    ON_BN_CLICKED(IDC_BT_PUSHING, &CMPPTestPane::OnBnClickedBtPushing)
END_MESSAGE_MAP()


// CMPPTestPane 진단입니다.

#ifdef _DEBUG
void CMPPTestPane::AssertValid() const
{
	CXTResizeFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMPPTestPane::Dump(CDumpContext& dc) const
{
	CXTResizeFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMPPTestPane 메시지 처리기입니다.
void CMPPTestPane::OnInitialUpdate()
{
    CXTResizeFormView::OnInitialUpdate();

    m_cbCrowdTest.InsertString(E_STEP_BASIC, L"BASIC");
    m_cbCrowdTest.InsertString(E_STEP_MATCH_READY, L"MATCH READY");
    m_cbCrowdTest.InsertString(E_STEP_STAGE_READY, L"STAGE READY");
    m_cbCrowdTest.InsertString(E_STEP_STAGE_RUN, L"STAGE RUN");
    m_cbCrowdTest.InsertString(E_STEP_STAGE_FINISH, L"STAGE FINISH");
    m_cbCrowdTest.InsertString(E_STEP_MATCH_FINISH, L"MATCH FINISH");
    m_cbCrowdTest.InsertString(E_STEP_AWARD, L"AWARD");
    m_cbCrowdTest.InsertString(E_STEP_BATTLE_END, L"BATTLE END");
    m_cbCrowdTest.InsertString(E_STEP_HTB, L"HTB");
    m_cbCrowdTest.InsertString(E_STEP_KNOCKDOWN, L"KNOCKDOWN");
    m_cbCrowdTest.InsertString(E_STEP_KO, L"KO");
    m_cbCrowdTest.SetCurSel(0);

    // 천하제일 무도회 칭호 이펙트    
    m_cmbTenkaichi.AddString(L"NONE");
    m_cmbTenkaichi.AddString(L"1");    
    m_cmbTenkaichi.AddString(L"2");
    m_cmbTenkaichi.AddString(L"3");

    // 컬러 체인지 이벤트
    m_cbColorChange.AddString(L"START");
    m_cbColorChange.AddString(L"END");
    m_cbColorChange.AddString(L"ANIM");

    // 스테이트 체인지 
    m_cbStateChange.AddString(L"IDLE");
    m_cbStateChange.AddString(L"SPIN ATTACK");
    m_cbStateChange.SetItemData(0, 3);
    m_cbStateChange.SetItemData(1, 51);

    // 변신 이벤트    
    m_cbTransform.AddString(L"Super Saiyan");
    m_cbTransform.AddString(L"Pure Majin");
    m_cbTransform.AddString(L"Great Namek");    
    m_cbTransform.AddString(L"KaiouKen");
    m_cbTransform.AddString(L"NONE");

    // 스턴 타입
    m_cbStunType.AddString(L"GENERAL");
    m_cbStunType.AddString(L"FROZEN");
    m_cbStunType.AddString(L"TIED");
}


void CMPPTestPane::OnBnClickedBtnCrowdChange()
{
    int nIndex = m_cbCrowdTest.GetCurSel();
    
    GetMPPClientEvent()->SendEvent(MSG_MRPOPO_TEST_CROWD, nIndex);
}

void CMPPTestPane::OnBnClickedBtObserverIndex()
{
    CString strIndex;
    m_edObserverIndex.GetWindowText(strIndex);

    GetMPPClientEvent()->SendEvent(MSG_MRPOPO_OBSERVER_STATIC, _wtoi(strIndex));
}

void CMPPTestPane::OnBnClickedBtObserverLua() 
{
    CString str;
    m_edObserverLua.GetWindowText(str);

    GetMPPClientEvent()->SendEvent(MSG_MRPOPO_OBSERVER_LUA, _wtoi(str));
}

void CMPPTestPane::OnBnClickedBtTest1()
{
    CString str;
    m_edTest1.GetWindowText(str);
    
    GetMPPClientEvent()->SendEvent(MSG_MRPOPO_TEST1, Float2DWORD(str));
}

void CMPPTestPane::OnBnClickedBtTest2()
{
    CString str;
    m_edTest2.GetWindowText(str);

    GetMPPClientEvent()->SendEvent(MSG_MRPOPO_TEST2, Float2DWORD(str));
}

void CMPPTestPane::OnBnClickedBtTest3()
{
    CString str;
    m_edTest3.GetWindowText(str);

    GetMPPClientEvent()->SendEvent(MSG_MRPOPO_TEST3,Float2DWORD(str));
}

void CMPPTestPane::OnBnClickedBtFreecamera()
{
    static BOOL bFreeCamera = FALSE;    
    bFreeCamera = !bFreeCamera;

    GetMPPClientEvent()->SendEvent(MSG_MRPOPO_FREECAMERA, bFreeCamera);
}
void CMPPTestPane::OnBnClickedBtRenderScouter()
{
    static BOOL bRender = TRUE;
    bRender = !bRender;

    GetMPPClientEvent()->SendEvent(MSG_MRPOPO_SCOUTER_RENDER, bRender);
}


void CMPPTestPane::OnCbnSelchangeTenkaichi()
{
    GetMPPClientEvent()->SendEvent(MSG_MRPOPO_TENKAICHI_MARK, m_cmbTenkaichi.GetCurSel());
}

void CMPPTestPane::OnCbnSelchangeColorchange()
{
    GetMPPClientEvent()->SendEvent(MSG_MRPOPO_TEST_CHANGECOLOR, m_cbColorChange.GetCurSel());    
}

void CMPPTestPane::OnCbnSelchangeStatechange()
{
    GetMPPClientEvent()->SendEvent(MSG_MRPOPO_TRANSLATE_STATE, (DWORD)m_cbStateChange.GetItemData(m_cbStateChange.GetCurSel()));
}

void CMPPTestPane::OnCbnSelchangeTransform()
{
    CString strText;
    m_cbTransform.GetWindowText(strText);
    if(strText == L"NONE")
    {
        GetMPPClientEvent()->SendEvent(MSG_MRPOPO_TRANSFORM, 0xFF);
    }
    else
    {
        GetMPPClientEvent()->SendEvent(MSG_MRPOPO_TRANSFORM, m_cbTransform.GetCurSel());
    }
}

void CMPPTestPane::OnCbnSelchangeStuntype()
{
    GetMPPClientEvent()->SendEvent(MSG_MRPOPO_STUN, m_cbStunType.GetCurSel());
}

void CMPPTestPane::OnBnClickedBtTargetMarking()
{
    static BOOL bMark = FALSE;
    bMark = !bMark;

    GetMPPClientEvent()->SendEvent(MSG_MRPOPO_TARGET_MARKING, bMark);
    
}

void CMPPTestPane::OnBnClickedBtPushing()
{
    GetMPPClientEvent()->SendEvent(MSG_MRPOPO_PUSHING, NULL);   
}
