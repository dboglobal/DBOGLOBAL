// PageClump.cpp : implementation file
//

#include "stdafx.h"
#include "ModelTool.h"
#include "ModelToolApplication.h"
#include "NtlText.h"
#include "RightToolView.h"

#include "PageClump.h"

// CPageClump dialog

CPageClump* CPageClump::m_pInstance = NULL;

IMPLEMENT_DYNAMIC(CPageClump, CPropertyPage)

CPageClump::CPageClump()
	: CPropertyPage(CPageClump::IDD)
	, strEditClumpName(_T(""))
{
	m_pInstance = this;
    m_btColor.SetColor(0, 0, 255);    
}

CPageClump::~CPageClump()
{
}

void CPageClump::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_CLUMP_NAME, strEditClumpName);
    DDX_Control(pDX, IDC_LIST_ATOMIC, m_listBoxAtomic);    
    DDX_Control(pDX, IDC_BUTTON3, m_btColor);    
    DDX_Control(pDX, IDC_LIST_MATERIAL, m_listBoxMaterial);
    DDX_Control(pDX, IDC_EDIT1, m_editTextureName);
    DDX_Control(pDX, IDC_EDIT2, m_editTextureSize);
    DDX_Control(pDX, IDC_BT_MATERIAL_ADD, m_btTextureAdd);
    DDX_Control(pDX, IDC_BT_MATERIAL_DEL, m_btTextureDelete);        
    DDX_Control(pDX, IDC_CHECK_ALPHA, m_cbAlphaTest);
    DDX_Control(pDX, IDC_CK_2SIDE, m_cb2Side);
    DDX_Control(pDX, IDC_CK_2SIDE_EDGE, m_cb2SideEdge);
}


BEGIN_MESSAGE_MAP(CPageClump, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, &CPageClump::OnBnClickedButton1)
	ON_LBN_SELCHANGE(IDC_LIST_ATOMIC, &CPageClump::OnLbnSelchangeListAtomic)
    ON_BN_CLICKED(IDC_BT_DELETE, &CPageClump::OnBnClickedBtDelete)
    ON_BN_CLICKED(IDC_BT_CLUMP_SAVE, &CPageClump::OnBnClickedBtClumpSave)    
    ON_BN_CLICKED(IDC_BT_MATERIAL_ADD, &CPageClump::OnBnClickedBtMaterialAdd)
    ON_BN_CLICKED(IDC_BT_MATERIAL_DEL, &CPageClump::OnBnClickedBtMaterialDel)
    ON_BN_CLICKED(IDC_BUTTON3, &CPageClump::OnBnClickedButton3)
    ON_LBN_SELCHANGE(IDC_LIST_MATERIAL, &CPageClump::OnLbnSelchangeListMaterial)    
    ON_BN_CLICKED(IDC_CHECK_ALPHA, &CPageClump::OnBnClickedCheckAlpha)
    ON_BN_CLICKED(IDC_CK_2SIDE, &CPageClump::OnBnClickedCk2side)
    ON_BN_CLICKED(IDC_CK_2SIDE_EDGE, &CPageClump::OnBnClickedCk2sideEdge)
END_MESSAGE_MAP()


// CPageClump message handlers

void CPageClump::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	// Clump 파일을 로딩한다.
	USES_CONVERSION;

	WCHAR szOpenFilter[] = L"Clump File (*.dff)|*.dff||";
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szOpenFilter);
	if(fileDlg.DoModal() == IDOK)
	{
		m_strLoadFilePath = fileDlg.GetPathName();	
        m_strLoadFileName = fileDlg.GetFileName();
		CString strFileExt = fileDlg.GetFileExt();

		if(strFileExt.MakeUpper() == "DFF")		// clump 파일
		{
			RwBool retBool = CModelToolApplication::GetInstance()->LoadClump(W2A(m_strLoadFilePath));
			if(retBool == (RwBool)TRUE)
			{
				strEditClumpName = m_strLoadFileName;
				UpdateData(FALSE);
			}
		}
	}		
}

void CPageClump::OnLbnSelchangeListAtomic()
{
	// TODO: Add your control notification handler code here
	// ListBox의 Atomic을 선택햇을때 바운딩박스를 그려준다.
	USES_CONVERSION;

	CMTCharacter* pCharacter = CModelToolApplication::GetInstance()->GetCharcter();
	if(!pCharacter)
		return;

	pCharacter->ClearBB();

	int nCount = m_listBoxAtomic.GetSelCount();
	if(nCount > 0)
	{
		int* arrIndex = new int[nCount];
		m_listBoxAtomic.GetSelItems(nCount, arrIndex);
		for(int i = 0; i < nCount; i++)
		{
            WCHAR szAtomicName[128] = {0,};			

			m_listBoxAtomic.GetText(arrIndex[i], szAtomicName);
			pCharacter->CreateBB((RwChar*)(W2A(szAtomicName)));
		}	

		delete[] arrIndex;
	}
}

void CPageClump::OnBnClickedBtDelete()
{
    // TODO: Add your control notification handler code here
    USES_CONVERSION;

    // 선택되지 않은 Atomic을 골라서 삭제한다.
    int nCount = m_listBoxAtomic.GetSelCount();
    if(nCount > 0)
    {
        CMTCharacter* pCharacter = CModelToolApplication::GetInstance()->GetCharcter();
        if(!pCharacter)
            return;

        int* arrIndex = new int[nCount];
        m_listBoxAtomic.GetSelItems(nCount, arrIndex);
        
        BOOL bExist = FALSE;
        for(int i = 0; i < m_listBoxAtomic.GetCount(); ++i)
        {
            bExist = FALSE;
            for(int j = 0; j < nCount; ++j)
            {
                if(i == arrIndex[j])
                {
                    bExist = TRUE;
                    break;
                }
            }

            if(bExist == FALSE) // 리스트 박스에 존재하지 않으면 삭제한다.
            {
                WCHAR szAtomicName[128] = {0,};
                m_listBoxAtomic.GetText(i, szAtomicName);
                RwBool retBool = pCharacter->RemoveAtomic(W2A(szAtomicName));
                if(retBool == FALSE)
                {
                    MessageBox(L"Atomic Remove Error!", szAtomicName);
                }                
            }
        }

        pCharacter->UpdateClumpInfo();

        delete[] arrIndex;
    }
}

void CPageClump::OnBnClickedBtClumpSave()
{
    // TODO: Add your control notification handler code here
    USES_CONVERSION;

    // 선택한 Atomic들만 Save한다.

    WCHAR szOpenFilter[] = L"Clump File (*.dff)|*.dff||";
    CFileDialog fileDlg(FALSE, L"dff", m_strLoadFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szOpenFilter);
    
    if(fileDlg.DoModal() == IDOK)
    {
        CString strFileName = fileDlg.GetPathName();	        
        
        CMTCharacter* pCharacter = CModelToolApplication::GetInstance()->GetCharcter();
        if(pCharacter)
        {
            OnBnClickedBtDelete();

            RwBool retBool = pCharacter->SaveClump(W2A(strFileName));
            if(retBool == (RwBool)FALSE)
            {
                MessageBox(L"Clump File Save Fail");
            }
            
            retBool = CModelToolApplication::GetInstance()->LoadClump(W2A(m_strLoadFilePath));
            if(retBool == (RwBool)TRUE)
            {
                strEditClumpName = m_strLoadFileName;
                UpdateData(FALSE);
            }
        }        
   } 
}

void CPageClump::OnBnClickedBtMaterialAdd()
{
    // TODO: Add your control notification handler code here
    USES_CONVERSION;

    // 멀티텍스처 추가 기능
    WCHAR szOpenFilter[] = L"Texture File (*.dds;*.png)|*.dds; *.png||";
    CFileDialog dlgTextureFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, szOpenFilter);
    if(dlgTextureFile.DoModal() == IDOK)
    {
        CString strFileName = dlgTextureFile.GetFileName();
        CString strFilePath = dlgTextureFile.GetPathName();        
        strFilePath.Replace(strFileName, L"");
        strFileName.Replace(L".dds", L"");
            
        CMTCharacter* pCharacter = CModelToolApplication::GetInstance()->GetCharcter();
        if(pCharacter)
        {
            // ListBox에서 Material 이름을 가져온다.
            int nIndex = m_listBoxMaterial.GetCurSel();
            WCHAR szMaterialName[256] = {0,};
            m_listBoxMaterial.GetText(nIndex, szMaterialName);

            RwBool bReturn = pCharacter->SetMultiTexture(W2A(szMaterialName), W2A(strFileName), W2A(strFilePath));
            if(bReturn == (RwBool)TRUE)
            {
                // 화면 정보를 갱신한다.
                pCharacter->DisplayMaterialAttribute(W2A(szMaterialName));
            }
            else
            {
                MessageBox(L"Set MultiTexture Fail");
            }
        }
    }
}

void CPageClump::OnBnClickedBtMaterialDel()
{
    // TODO: Add your control notification handler code here
    USES_CONVERSION;

    // 기존에 있던 멀티 텍스처를 제거한다.

    CMTCharacter* pCharacter = CModelToolApplication::GetInstance()->GetCharcter();
    if(pCharacter)
    {
        // ListBox에서 Material 이름을 가져온다.
        int nIndex = m_listBoxMaterial.GetCurSel();
        WCHAR szMaterialName[256] = {0,};
        m_listBoxMaterial.GetText(nIndex, szMaterialName);

        RwBool bReturn = pCharacter->SetMultiTexture(W2A(szMaterialName), NULL, NULL);
        if(bReturn == (RwBool)TRUE)
        {
            // 화면 정보를 갱신한다.
            pCharacter->DisplayMaterialAttribute(W2A(szMaterialName));
        }
        else
        {
            MessageBox(L"Set MultiTexture Fail");
        }
    }
}

void CPageClump::OnBnClickedButton3()
{
    // TODO: Add your control notification handler code here
    USES_CONVERSION;

    CColorDialog dlgColor;
    if(dlgColor.DoModal() == IDOK)
    {
        COLORREF color = dlgColor.GetColor();

        m_btColor.SetColor(GetRValue(color), GetGValue(color), GetBValue(color));
        Invalidate(FALSE);

        // Material에 Color를 적용한다.
        int nIndex = m_listBoxMaterial.GetCurSel();
        if(nIndex < 0)  // Material이 선택되지 않았을때
            return;
        
        CMTCharacter* pCharacter = CModelToolApplication::GetInstance()->GetCharcter();
        if(!pCharacter) // Character가 Load 되지 않았을때
            return;
        
        WCHAR strMaterialName[256] = {0,};
        m_listBoxMaterial.GetText(nIndex, strMaterialName);
        pCharacter->SetMaterialColor(W2A(strMaterialName), GetRValue(color), GetGValue(color), GetBValue(color));
    }    
}

void CPageClump::OnLbnSelchangeListMaterial()
{
    // TODO: Add your control notification handler code here
    USES_CONVERSION;

    // Material (Texture) 정보를 가져와서 리스트 박스에 세팅한다.
    CMTCharacter* pCharacter = CModelToolApplication::GetInstance()->GetCharcter();
    if(pCharacter)
    {
        int nIndex = m_listBoxMaterial.GetCurSel();
        WCHAR strMaterialName[256] = {0,};
        m_listBoxMaterial.GetText(nIndex, strMaterialName);
        pCharacter->DisplayMaterialAttribute(W2A(strMaterialName));
    }
}

void CPageClump::SetMultiTextureInfo(RwChar* chTextureName, RwInt32 iWidth, RwInt32 iHeight)
{
    USES_CONVERSION;

    if(chTextureName == NULL)
    {
        // MultiTexture 정보가 없을때, Add 버튼만 활성화 시킨다.
        m_btTextureAdd.EnableWindow(TRUE);
        m_btTextureDelete.EnableWindow(FALSE);      
        m_editTextureName.SetWindowText(L"");
        m_editTextureSize.SetWindowText(L"");

    }
    else
    {
        // MultiTexture 정보가 있을때, Delete 버튼만 활성화 시킨다.
        m_btTextureAdd.EnableWindow(FALSE);
        m_btTextureDelete.EnableWindow(TRUE);

        m_editTextureName.SetWindowText(A2W(chTextureName));

        RwChar temp[32] = {0,};
        RsSprintf(temp, RWSTRING("%d X %d"), iWidth, iHeight);
        m_editTextureSize.SetWindowText(A2W(temp));
    }
}
BOOL CPageClump::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    // TODO:  Add extra initialization here
    m_btTextureAdd.EnableWindow(FALSE);
    m_btTextureDelete.EnableWindow(FALSE);

    

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

     

void CPageClump::OnBnClickedCheckAlpha()
{
    USES_CONVERSION;

    CMTCharacter* pCharacter = CModelToolApplication::GetInstance()->GetCharcter();
    if(!pCharacter)
        return;

    int nCount = m_listBoxAtomic.GetSelCount();
    if(nCount > 0)
    {
        int* arrIndex = new int[nCount];
        m_listBoxAtomic.GetSelItems(nCount, arrIndex);
        for(int i = 0; i < nCount; i++)
        {
            WCHAR szAtomicName[128] = {0,};			

            m_listBoxAtomic.GetText(arrIndex[i], szAtomicName); 

            BOOL bFlag = m_cbAlphaTest.GetCheck();
            pCharacter->SetAlphaTest(W2A(szAtomicName), bFlag);
        }	

        delete[] arrIndex;
    }
}

BOOL CPageClump::OnSetActive()
{
    CMTCharacter* pCharacter = CModelToolApplication::GetInstance()->GetCharcter();
    if(pCharacter)
    {
        pCharacter->SetVisible(TRUE);
        CModelToolApplication::GetInstance()->SetEditChar(pCharacter);
    }

    return CPropertyPage::OnSetActive();
}

BOOL CPageClump::OnKillActive()
{
    CMTCharacter* pCharacter = CModelToolApplication::GetInstance()->GetCharcter();
    if(pCharacter)
    {
        pCharacter->SetVisible(FALSE);
    }

    return CPropertyPage::OnKillActive();
}

void CPageClump::OnBnClickedCk2side()
{
    USES_CONVERSION;

    CMTCharacter* pCharacter = CModelToolApplication::GetInstance()->GetCharcter();
    if(!pCharacter)
        return;

    int nCount = m_listBoxAtomic.GetSelCount();
    if(nCount > 0)
    {
        int* arrIndex = new int[nCount];
        m_listBoxAtomic.GetSelItems(nCount, arrIndex);
        for(int i = 0; i < nCount; ++i)
        {
            CString strAtomicName;
            m_listBoxAtomic.GetText(arrIndex[i], strAtomicName);
            if(m_cb2Side.GetCheck())
            {
                pCharacter->Set2Side(W2A(strAtomicName), TRUE);
                m_cb2SideEdge.SetCheck(BST_UNCHECKED);
            }
            else
            {
                pCharacter->Set2Side(W2A(strAtomicName), FALSE);
            }
        }
    }
}

void CPageClump::OnBnClickedCk2sideEdge()
{
    USES_CONVERSION;

    CMTCharacter* pCharacter = CModelToolApplication::GetInstance()->GetCharcter();
    if(!pCharacter)
        return;

    int nCount = m_listBoxAtomic.GetSelCount();
    if(nCount > 0)
    {
        int* arrIndex = new int[nCount];
        m_listBoxAtomic.GetSelItems(nCount, arrIndex);
        for(int i = 0; i < nCount; ++i)
        {
            CString strAtomicName;
            m_listBoxAtomic.GetText(arrIndex[i], strAtomicName);
            if(m_cb2SideEdge.GetCheck())
            {
                pCharacter->Set2SideEdge(W2A(strAtomicName), TRUE);
                m_cb2Side.SetCheck(BST_UNCHECKED);
            }
            else
            {
                pCharacter->Set2SideEdge(W2A(strAtomicName), FALSE);
            }
        }
    }
}
