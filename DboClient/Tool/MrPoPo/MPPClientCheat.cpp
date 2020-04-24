// MPPClientCheat.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MrPoPo.h"
#include "MPPClientCheat.h"
#include "CNtlMrPoPoMsg.h"
#include "MPPClientEvent.h"

// CMPPClientCheat

IMPLEMENT_DYNCREATE(CMPPClientCheat, CXTResizeFormView)

CMPPClientCheat::CMPPClientCheat()
	: CXTResizeFormView(CMPPClientCheat::IDD)
{

}

CMPPClientCheat::~CMPPClientCheat()
{
}

void CMPPClientCheat::DoDataExchange(CDataExchange* pDX)
{
    CXTResizeFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_ED_ELAPSED, m_edElapsed);
    DDX_Control(pDX, IDC_CB_DUMP, m_cbDump);
}

BEGIN_MESSAGE_MAP(CMPPClientCheat, CXTResizeFormView)
    ON_BN_CLICKED(IDC_BT_ELAPSED, &CMPPClientCheat::OnBnClickedBtElapsed)
    ON_BN_CLICKED(IDC_BT_DUMP_CONSOLE, &CMPPClientCheat::OnBnClickedBtDumpConsole)
    ON_BN_CLICKED(IDC_BT_DUMP_GUI, &CMPPClientCheat::OnBnClickedBtDumpGui)
    ON_BN_CLICKED(IDC_BT_DUMP_FILE, &CMPPClientCheat::OnBnClickedBtDumpFile)
    ON_BN_CLICKED(IDC_DUMP_TARGET, &CMPPClientCheat::OnBnClickedDumpTarget)
    ON_BN_CLICKED(IDC_DUMP_REG, &CMPPClientCheat::OnBnClickedDumpReg)
    ON_BN_CLICKED(IDC_DUMP_UNREG, &CMPPClientCheat::OnBnClickedDumpUnreg)
    ON_BN_CLICKED(IDC_BT_LOWSPEC_CHAR, &CMPPClientCheat::OnBnClickedBtLowspecChar)
    ON_BN_CLICKED(IDC_BT_LOWSPEC_EFFECT, &CMPPClientCheat::OnBnClickedBtLowspecEffect)
    ON_BN_CLICKED(IDC_BT_LOWSPEC_MESH, &CMPPClientCheat::OnBnClickedBtLowspecMesh)
    ON_BN_CLICKED(IDC_BT_LOWSPEC_DECAL, &CMPPClientCheat::OnBnClickedBtLowspecDecal)
END_MESSAGE_MAP()


// CMPPClientCheat 진단입니다.

#ifdef _DEBUG
void CMPPClientCheat::AssertValid() const
{
	CXTResizeFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMPPClientCheat::Dump(CDumpContext& dc) const
{
	CXTResizeFormView::Dump(dc);
}
#endif
#endif //_DEBUG

void CMPPClientCheat::OnInitialUpdate()
{
    CXTResizeFormView::OnInitialUpdate();

    m_cbDump.AddString(L"SELF");
    m_cbDump.AddString(L"TARGET");
    m_cbDump.SetItemData(0, MPP_DUMP_SELF);
    m_cbDump.SetItemData(1, MPP_DUMP_TARGET);
}

// CMPPClientCheat 메시지 처리기입니다.

void CMPPClientCheat::OnBnClickedBtElapsed()
{
    CString str;
    m_edElapsed.GetWindowText(str);        

    GetMPPClientEvent()->SendEvent(MSG_MRPOPO_ELAPSEDTIME_WEIGHT, Float2DWORD(str));
}

void CMPPClientCheat::OnBnClickedBtDumpConsole()
{
    static BOOL bDumpConsole = FALSE;
    bDumpConsole = !bDumpConsole;
    
    GetMPPClientEvent()->SendEvent(MSG_MRPOPO_DUMP_TO_CONSOLE, bDumpConsole);
}

void CMPPClientCheat::OnBnClickedBtDumpGui()
{
    static BOOL bDumpGui = FALSE;
    bDumpGui = !bDumpGui;

    GetMPPClientEvent()->SendEvent(MSG_MRPOPO_DUMP_TO_GUI, bDumpGui);
}

void CMPPClientCheat::OnBnClickedBtDumpFile()
{
    static BOOL bDumpFile = FALSE;
    bDumpFile = !bDumpFile;

    GetMPPClientEvent()->SendEvent(MSG_MRPOPO_DUMP_TO_FILE, bDumpFile);
}

void CMPPClientCheat::OnBnClickedDumpTarget()
{
    int nIndex = (int)m_cbDump.GetItemData(m_cbDump.GetCurSel());

    GetMPPClientEvent()->SendEvent(MSG_MRPOPO_DUMP, nIndex);
}

void CMPPClientCheat::OnBnClickedDumpReg()
{
    GetMPPClientEvent()->SendEvent(MSG_MRPOPO_DUMP_REG, NULL);
}

void CMPPClientCheat::OnBnClickedDumpUnreg()
{
    GetMPPClientEvent()->SendEvent(MSG_MRPOPO_DUMP_UNREG, NULL);
}

void CMPPClientCheat::OnBnClickedBtLowspecChar()
{
    static BOOL bLowSpec = FALSE;
    bLowSpec = !bLowSpec;

    GetMPPClientEvent()->SendEvent(MSG_MRPOPO_LOWSPEC_CHAR, bLowSpec);
}

void CMPPClientCheat::OnBnClickedBtLowspecEffect()
{
    static BOOL bLowSpec = FALSE;
    bLowSpec = !bLowSpec;

    GetMPPClientEvent()->SendEvent(MSG_MRPOPO_LOWSPEC_EFFECT, bLowSpec);
}

void CMPPClientCheat::OnBnClickedBtLowspecMesh()
{
    static BOOL bRenderMesh = TRUE;
    bRenderMesh = !bRenderMesh;

    GetMPPClientEvent()->SendEvent(MSG_MRPOPO_RENDER_MESHSYSTEM, bRenderMesh);
}

void CMPPClientCheat::OnBnClickedBtLowspecDecal()
{
    static BOOL bRenderDecal = TRUE;
    bRenderDecal = !bRenderDecal;

    GetMPPClientEvent()->SendEvent(MSG_MRPOPO_RENDER_DECALSYSTEM, bRenderDecal);
}
