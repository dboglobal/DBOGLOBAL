// MPPConfigSetting.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MrPoPo.h"
#include "MPPConfigSetting.h"
#include "MPPDef.h"
#include "MPPLogView.h"
#include "NtlLuaState.h"

#define IDC_PROPERTY_GRID               99999

// CMPPConfigSetting

IMPLEMENT_DYNCREATE(CMPPConfigSetting, CXTResizeFormView)

CMPPConfigSetting::CMPPConfigSetting()
	: CXTResizeFormView(CMPPConfigSetting::IDD)
{

}

CMPPConfigSetting::~CMPPConfigSetting()
{
}

void CMPPConfigSetting::DoDataExchange(CDataExchange* pDX)
{
    CXTResizeFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_BTN_SAVEXML, m_btnSaveXML);
    DDX_Control(pDX, IDC_BTN_SAVEENC, m_btnSaveEnc);
    DDX_Control(pDX, IDC_ED_LUA, m_edLuaDir);
    DDX_Control(pDX, IDC_ED_OUT, m_edOutDir);
}

BEGIN_MESSAGE_MAP(CMPPConfigSetting, CXTResizeFormView)
    ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, OnGridNotify)
    ON_BN_CLICKED(IDC_BTN_SAVEXML, &CMPPConfigSetting::OnBnClickedBtnSavexml)
    ON_BN_CLICKED(IDC_BTN_SAVEENC, &CMPPConfigSetting::OnBnClickedBtnSaveenc)
    ON_BN_CLICKED(IDC_BTN_LUA_COMPILE, &CMPPConfigSetting::OnBnClickedBtnLuaCompile)
    ON_EN_CHANGE(IDC_ED_LUA, &CMPPConfigSetting::OnEnChangeEdLua)
END_MESSAGE_MAP()


// CMPPConfigSetting 진단입니다.

#ifdef _DEBUG
void CMPPConfigSetting::AssertValid() const
{
	CXTResizeFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMPPConfigSetting::Dump(CDumpContext& dc) const
{
	CXTResizeFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMPPConfigSetting 메시지 처리기입니다.

void CMPPConfigSetting::OnInitialUpdate()
{
    CXTResizeFormView::OnInitialUpdate();

    CRect rc;
    GetDlgItem(IDC_STATIC)->GetWindowRect(&rc);
    ScreenToClient(&rc);
    if(!m_property.Create(rc, this, IDC_PROPERTY_GRID))
        return;

    m_property.SetVariableItemsHeight(TRUE);        
    m_property.ShowToolBar(TRUE);
    m_property.ShowHelp(FALSE);
    //m_property.HighlightChangedItems(TRUE);
    m_property.SetTheme(xtpGridThemeWhidbey);

    m_pCatConfig = m_property.AddCategory(L"DBO Config Options");
    m_pFileLoad = (CCustomItemFileBox*)m_pCatConfig->AddChildItem(new CCustomItemFileBox(L"Load Config File", FILTER_CONFIG));
    m_pOpIp     = m_pCatConfig->AddChildItem(new CXTPPropertyGridItem(L"Server IP"));
    m_pOpPort   = m_pCatConfig->AddChildItem(new CXTPPropertyGridItem(L"Server Port"));
    m_pLocalDsp = (CCustomItemFileBox*)m_pCatConfig->AddChildItem(new CCustomItemFileBox(L"Local Dsp", FILTER_DST));
    m_pLocalSyncDsp = (CCustomItemFileBox*)m_pCatConfig->AddChildItem(new CCustomItemFileBox(L"Local Sync Dsp", FILTER_DST));
    m_pBugTrapIp = m_pCatConfig->AddChildItem(new CXTPPropertyGridItem(L"BugTrap Server IP"));
    m_pBugTrapPort = m_pCatConfig->AddChildItem(new CXTPPropertyGridItem(L"BugTrap Server Port"));

    m_pFileLoad->SetID(ID_FILE_LOAD);
    m_pOpIp->SetID(ID_OP_IP);
    m_pOpPort->SetID(ID_OP_PORT);
    m_pLocalDsp->SetID(ID_LOCAL_DSP);
    m_pLocalSyncDsp->SetID(ID_LOCAL_SYNC_DSP);
    m_pBugTrapIp->SetID(ID_BUGTRAP_IP);
    m_pBugTrapPort->SetID(ID_BUGTRAP_PORT);
    
    m_pCatConfig->Expand();    

    m_edLuaDir.Initialize(this, BES_XT_CHOOSEDIR);
    m_edOutDir.Initialize(this, BES_XT_CHOOSEDIR);

    SetResize(IDC_PROPERTY_GRID, SZ_TOP_LEFT, SZ_MIDDLE_RIGHT);       
    SetResize(IDC_BTN_SAVEXML, SZ_MIDDLE_CENTER, SZ_MIDDLE_CENTER);
    SetResize(IDC_BTN_SAVEENC, SZ_MIDDLE_CENTER, SZ_MIDDLE_CENTER);
    SetResize(IDC_ST_LUA, SZ_MIDDLE_LEFT, SZ_BOTTOM_RIGHT);
    SetResize(IDC_ST_LUA_DIR, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
    SetResize(IDC_ST_OUT_DIR, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
    SetResize(IDC_ED_LUA, SZ_MIDDLE_LEFT, SZ_MIDDLE_RIGHT);
    SetResize(IDC_ED_OUT, SZ_MIDDLE_LEFT, SZ_MIDDLE_RIGHT);
    SetResize(IDC_BTN_LUA_COMPILE, SZ_BOTTOM_CENTER, SZ_BOTTOM_CENTER);
}

afx_msg LRESULT CMPPConfigSetting::OnGridNotify(WPARAM wParam, LPARAM lParam)
{
    USES_CONVERSION;

    if(wParam == XTP_PGN_ITEMVALUE_CHANGED)
    {
        CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam;

        switch(pItem->GetID())
        {
        case ID_FILE_LOAD:          
            if(CNtlScriptEncrypter::LoadConfigOption(&m_configData, W2A(pItem->GetValue())))
            {
                UpdateConfigData();                
                CString strLog;
                strLog = L"Success Config File Loading - ";
                strLog += pItem->GetValue();
                AddLogView(strLog);
            }
            else
            {
                MessageBox(L"Config File Load Fail!", NULL, MB_ICONERROR);   
            }
            break;
        case ID_OP_IP:   
            m_configData.strAddr = W2A(pItem->GetValue());
            break;
        case ID_OP_PORT:
            m_configData.dwPort = atoi(W2A(pItem->GetValue()));
            break;
        case ID_LOCAL_DSP:
            m_configData.strLocalDsp = W2A(pItem->GetValue());
            break;
        case ID_LOCAL_SYNC_DSP:
            m_configData.strLocalSyncDsp = W2A(GetFileNameFromPath(pItem->GetValue()));
            break;
        case ID_BUGTRAP_IP:
            m_configData.strBugTrapServerIP = W2A(GetFileNameFromPath(pItem->GetValue()));
            break;
        case ID_BUGTRAP_PORT:
            m_configData.dwBugTrapServerPort = atoi(W2A(pItem->GetValue()));
            break;
        }
    }

    return 0;
}

void CMPPConfigSetting::UpdateConfigData() 
{
    WCHAR buf[64] = {0,};
    
    m_pOpIp->SetValue(m_configData.strAddr);
    swprintf_s(buf, L"%d", m_configData.dwPort);
    m_pOpPort->SetValue(buf);

    m_pLocalDsp->SetValue(m_configData.strLocalDsp);
    m_pLocalSyncDsp->SetValue(m_configData.strLocalSyncDsp);    
    m_pBugTrapIp->SetValue(m_configData.strBugTrapServerIP);
    swprintf_s(buf, L"%d", m_configData.dwBugTrapServerPort);
    m_pBugTrapPort->SetValue(buf);    
}

void CMPPConfigSetting::OnBnClickedBtnSavexml()
{
    USES_CONVERSION;

    CFileDialog dlg(FALSE, L"xml", L"ConfigOptions.xml");    
    dlg.DoModal();

    if(CNtlScriptEncrypter::SaveConfigOption(&m_configData, W2A(dlg.m_pOFN->lpstrFile)))
    {
        CString strLog;
        strLog = L"Success Config File Save - ";
        strLog += dlg.m_pOFN->lpstrFile;
        AddLogView(strLog);        
    }
    else
    {
        MessageBox(L"Config File Save Fail!", NULL, MB_ICONERROR);   
    }
}

void CMPPConfigSetting::OnBnClickedBtnSaveenc()
{
    USES_CONVERSION;

    CFileDialog dlg(FALSE, L"edf", L"ConfigOptions.edf");    
    dlg.DoModal();

    if(CNtlScriptEncrypter::SaveConfigOption(&m_configData, W2A(dlg.m_pOFN->lpstrFile), TRUE))
    {
        CString strLog;
        strLog = L"Success Config File Save - ";
        strLog += dlg.m_pOFN->lpstrFile;
        AddLogView(strLog);        
    }
    else
    {
        MessageBox(L"Config File Save Fail!", NULL, MB_ICONERROR);   
    }
}

/**
 * 루아파일을 컴파일한다.
 */
void CMPPConfigSetting::OnBnClickedBtnLuaCompile()
{
    //USES_CONVERSION;

    //// Script 폴더안에 있는 모든 Lua 파일들을 컴파일한다.
    //CString strLuaPath, strOutPath;
    //m_edLuaDir.GetWindowText(strLuaPath);    
    //m_edOutDir.GetWindowText(strOutPath);

    //std::vector<std::wstring> vecLuaFiles;
    //LoadLuaFiles();

    //CString strLog;
    //for each(std::wstring strLuaFile in vecLuaFiles)
    //{
    //    strLuaFile = strLuaPath + L"\\" + strLuaFile.c_str();
    //    if(CNtlLuaState::CompileLua(W2A(strLuaFile.c_str()), W2A(strOutPath)))
    //    {
    //        strLog = L"Success Compile Lua File - ";
    //        strLog += strLuaFile.c_str();
    //    }
    //    else
    //    {
    //        strLog = L"Fail Lua Compile Lua File - ";
    //        strLog += strLuaFile.c_str();
    //    }

    //    AddLogView(strLog);
    //}
}

void CMPPConfigSetting::LoadLuaFiles()
{
    //vecFiles.clear();
    //vecFiles.reserve(64);
    //strPath += "\\*.lua";

    //WIN32_FIND_DATA FindFileData;
    //HANDLE hFile = FindFirstFile(strPath, &FindFileData);
    //if(hFile == INVALID_HANDLE_VALUE) 
    //    return;

    //do
    //{
    //    vecFiles.push_back(FindFileData.cFileName);
    //}
    //while(FindNextFile(hFile, &FindFileData));
}

void CMPPConfigSetting::OnEnChangeEdLua()
{
    CString strLuaPath;
    m_edLuaDir.GetWindowText(strLuaPath);
    m_edOutDir.SetWindowText(strLuaPath);
}
