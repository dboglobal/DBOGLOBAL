// MaterialPane.cpp : implementation file
//

#include "stdafx.h"
#include "ModelTool2.h"
#include "MaterialPane.h"

CMaterialPane* CMaterialPane::m_pInstance = NULL;

// CMaterialPane

IMPLEMENT_DYNCREATE(CMaterialPane, CXTResizeFormView)

CMaterialPane::CMaterialPane()
	: CXTResizeFormView(CMaterialPane::IDD)
{
    m_pInstance = this;
    m_pMTClump = NULL;
}

CMaterialPane::~CMaterialPane()
{
}

void CMaterialPane::DoDataExchange(CDataExchange* pDX)
{
    CXTResizeFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_MATERIAL, m_listBoxMaterial);
    DDX_Control(pDX, IDC_EDIT_NAME, m_editTextureName);
    DDX_Control(pDX, IDC_EDIT_SIZE, m_editTextureSize);
    DDX_Control(pDX, IDC_BT_MATERIAL_ADD, m_btTextureAdd);
    DDX_Control(pDX, IDC_BT_MATERIAL_DEL, m_btTextureDelete);
}

BEGIN_MESSAGE_MAP(CMaterialPane, CXTResizeFormView)
    ON_LBN_SELCHANGE(IDC_LIST_MATERIAL, &CMaterialPane::OnLbnSelchangeListMaterial)
    ON_BN_CLICKED(IDC_BT_MATERIAL_ADD, &CMaterialPane::OnBnClickedBtMaterialAdd)
    ON_BN_CLICKED(IDC_BT_MATERIAL_DEL, &CMaterialPane::OnBnClickedBtMaterialDel)
END_MESSAGE_MAP()


// CMaterialPane diagnostics

#ifdef _DEBUG
void CMaterialPane::AssertValid() const
{
	CXTResizeFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMaterialPane::Dump(CDumpContext& dc) const
{
	CXTResizeFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMaterialPane message handlers

void CMaterialPane::OnInitialUpdate()
{
    CXTResizeFormView::OnInitialUpdate();

    SetResize(IDC_FRAME_MATERIAL, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
    SetResize(IDC_LIST_MATERIAL, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
    SetResize(IDC_STATIC_MULTI_TEXTURE, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
    SetResize(IDC_ST_NAME, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
    SetResize(IDC_ST_SIZE, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
    SetResize(IDC_EDIT_NAME, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
    SetResize(IDC_EDIT_SIZE, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
    SetResize(IDC_BT_MATERIAL_ADD, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
    SetResize(IDC_BT_MATERIAL_DEL, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

    SetClump(NULL);
}

void CMaterialPane::SetClump(CMTClump* pMTClump)
{
    if(!pMTClump)
    {
        m_pMTClump = NULL;        
        SetEnable(FALSE);
    }
    else
    {
        m_pMTClump = pMTClump;
        OnDataUpdate();
        SetEnable(TRUE);
    }
}

void CMaterialPane::SetEnable(BOOL bEnable)
{
    if(bEnable == FALSE)
    {
        m_listBoxMaterial.ResetContent();
        m_editTextureName.SetWindowText("");
        m_editTextureSize.SetWindowText("");
    }

    m_listBoxMaterial.EnableWindow(bEnable);
    m_editTextureName.EnableWindow(bEnable);
    m_editTextureSize.EnableWindow(bEnable);
    m_btTextureAdd.EnableWindow(bEnable);
    m_btTextureDelete.EnableWindow(bEnable);
}

void CMaterialPane::OnDataUpdate()
{
    USES_CONVERSION;

    if(!m_pMTClump)
        return;

    m_listBoxMaterial.ResetContent();

    std::vector<char*> vMaterialNames = m_pMTClump->GetMaterialNames();
    for(UINT i = 0; i < vMaterialNames.size(); ++i)
    {
        m_listBoxMaterial.AddString((vMaterialNames[i]));
    }

}
void CMaterialPane::OnLbnSelchangeListMaterial()
{
    USES_CONVERSION;
    if(!m_pMTClump)
        return;

    // Material 정보를 가져와서 리스트 박스에 세팅한다. (멀티 텍스쳐)
    int nIndex = m_listBoxMaterial.GetCurSel();
    char szMaterialName[256] = {0,};
    m_listBoxMaterial.GetText(nIndex, szMaterialName);
    OnDisplayMaterialInfo(szMaterialName);
}

void CMaterialPane::OnBnClickedBtMaterialAdd()
{
    USES_CONVERSION;
    if(!m_pMTClump)
        return;

    // 멀티텍스처 추가 기능
    char szOpenFilter[] = "Texture File (*.dds;*.png)|*.dds; *.png||";    
    CFileDialog dlgTextureFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, szOpenFilter);
    if(dlgTextureFile.DoModal() == IDOK)
    {
		CString strFileName = dlgTextureFile.GetFileName();
        CString strFilePath = dlgTextureFile.GetPathName();        
        strFilePath.Replace(strFileName, "");
        strFileName.MakeUpper();
        strFileName.Replace(".DDS", "");
        //strFileName.Replace(".PNG", "");
        
        
		// ListBox에서 Material 이름을 가져온다.
        int nIndex = m_listBoxMaterial.GetCurSel();
        char szMaterialName[256] = {0,};
        m_listBoxMaterial.GetText(nIndex, szMaterialName);

        RwBool bReturn = m_pMTClump->SetMultiTexture((szMaterialName), strFileName.GetBuffer(), strFilePath.GetBuffer());
        if(bReturn == (RwBool)TRUE)
        {
            OnDisplayMaterialInfo(szMaterialName);
        }
        else
        {
            m_editTextureName.SetWindowText("");
            m_editTextureSize.SetWindowText("");
            MessageBox("Set MultiTexture Fail");
        }
    }
}

void CMaterialPane::OnBnClickedBtMaterialDel()
{
    USES_CONVERSION;

    if(!m_pMTClump)
        return;

    // 기존에 있던 멀티 텍스처를 제거한다.

    
    // ListBox에서 Material 이름을 가져온다.
    int nIndex = m_listBoxMaterial.GetCurSel();
    char szMaterialName[256] = {0,};
    m_listBoxMaterial.GetText(nIndex, szMaterialName);

    RwBool bReturn = m_pMTClump->SetMultiTexture((szMaterialName), NULL, NULL);
    if(bReturn == (RwBool)TRUE)
    {
        // 화면 정보를 갱신한다.
        m_editTextureName.SetWindowText("");
        m_editTextureSize.SetWindowText("");
    }
    else
    {
        MessageBox("Delete MultiTexture Fail");
    }
    
}

void CMaterialPane::OnDisplayMaterialInfo(char* szMaterialName)
{
    USES_CONVERSION;

    if(!m_pMTClump || !szMaterialName)
        return;

    RpMaterial* pMaterial = m_pMTClump->GetMaterialByName((szMaterialName));    
    if(pMaterial)
    {
        // 텍스쳐 설정
        RwTexture* pTexture = NtlMatExtGetMultiTexture(pMaterial);
        if(pTexture)
        {
            // 이름 설정
            std::string strTextureName = RwTextureGetName(pTexture);
            RwRaster* pRaster = RwTextureGetRaster(pTexture);
            RwInt32 nWidth = RwRasterGetWidth(pRaster);
            RwInt32 nHeight = RwRasterGetHeight(pRaster);

            m_editTextureName.SetWindowText((strTextureName.c_str()));
            CString strTextureSize;
            strTextureSize.Format("WIDTH:%d, HEIGHT:%d", nWidth, nHeight);
            m_editTextureSize.SetWindowText(strTextureSize);
        }
        else
        {
            // MultiTexture가 없을때
            m_editTextureName.SetWindowText("");
            m_editTextureSize.SetWindowText("");
        }
    }
}
