// PageMob.cpp : implementation file
//

#include "stdafx.h"
#include "ModelTool.h"
#include "PageMob.h"
#include "RightToolView.h"


// CPageMob dialog


IMPLEMENT_DYNAMIC(CPageMob, CPropertyPage)

CPageMob::CPageMob()
	: CPagePC(CPageMob::IDD)
{

}

CPageMob::~CPageMob()
{
 
}

BOOL CPageMob::OnInitDialog()
{
    USES_CONVERSION;

    CPropertyPage::OnInitDialog();

    m_treePC.SetRoot(L"MOB");
    m_treePC.SetParent(this);
    m_btSaveScript.EnableWindow(FALSE);

    m_pInstance = this;

    CString sWorkFolder = A2W(CModelToolApplication::GetInstance()->GetWorkDir());
    sWorkFolder += L"\\TreeMOB.xml";
    m_treePC.LoadTree((LPWSTR)(LPCWSTR)sWorkFolder);

    // Save할 폴더이름을 설정한다.
    m_sSaveFolderName = A2W(CModelToolApplication::GetInstance()->GetWorkDir());
    m_sSaveFolderName += L"\\Character\\";

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageMob::OnDestroy()
{
    // 창이 없어지기 전에 XML로 내용을 저장한다.
    USES_CONVERSION;

    CString sWorkFolder = A2W(CModelToolApplication::GetInstance()->GetWorkDir());
    sWorkFolder += L"\\TreeMOB.xml";
    m_treePC.SaveTree(L"MOB", (LPWSTR)(LPCWSTR)sWorkFolder);

    CPropertyPage::OnDestroy();    
}

BOOL CPageMob::OnSetActive()
{
    // 활성화 될때 RightView에 자신의 종류를 알려줘야 한다.
    // RightView는 공통으로 사용되고, LeftView의 종류에 따라서
    // 애니메이션 종류가 달라져야 하기 때문
    CRightToolView::GetInstance()->SetActiveLeftView(CRightToolView::LEFT_PAGE_MOB);

    // 선택된 아이템을 다시한번 선택해 준다. (오른쪽 툴뷰의 활성화를 위해)
    HTREEITEM hSelectedItem = m_treePC.GetSelectedItem();
    if(hSelectedItem)
    {
        m_treePC.SelectItem(m_treePC.GetRootItem());
        m_treePC.SelectItem(hSelectedItem);
    }

    return CPropertyPage::OnSetActive();
}