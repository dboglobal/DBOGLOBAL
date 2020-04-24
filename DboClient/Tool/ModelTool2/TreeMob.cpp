// TreeMob.cpp : implementation file
//

#include "stdafx.h"
#include "ModelTool2.h"
#include "TreeMob.h"


CTreeMobPane* CTreeMobPane::m_pInstance = NULL;

// CTreeMobPane

IMPLEMENT_DYNCREATE(CTreeMobPane, CTreePCPane)

CTreeMobPane::CTreeMobPane()
{
    m_pInstance = this;

    m_pImageList = NULL;
    m_pCharacter = NULL;

    m_strRootName = "MOB";
}

CTreeMobPane::~CTreeMobPane()
{
}

BEGIN_MESSAGE_MAP(CTreeMobPane, CTreePCPane)
    ON_WM_SETFOCUS()
END_MESSAGE_MAP()


// CTreeMobPane diagnostics

#ifdef _DEBUG
void CTreeMobPane::AssertValid() const
{
	CTreePCPane::AssertValid();
}

#ifndef _WIN32_WCE
void CTreeMobPane::Dump(CDumpContext& dc) const
{
	CTreePCPane::Dump(dc);
}
#endif
#endif //_DEBUG


// CTreeMobPane message handlers

void CTreeMobPane::InitClass()
{
    USES_CONVERSION;

    CString sWorkFolder = (CModelToolApplication::GetInstance()->GetWorkDir());
    m_sSaveFolderName = sWorkFolder;
    m_sSaveFolderName += MT_PATH_CHARACTER;

    m_strTreeFileName = sWorkFolder;
    m_strTreeFileName += "\\Tool\\ModelTool\\TreeMob.xml";

    m_strPropertyListFileName = m_sSaveFolderName;
    m_strPropertyListFileName += "MobPropertyList.xml";
}

void CTreeMobPane::OnSetFocus(CWnd* pOldWnd)
{
    CTreePCPane::OnSetFocus(pOldWnd);

    CModelToolApplication::GetInstance()->SetAppMode(MT_MODE_MOB);
    //if(m_pCharacter && m_pCharacter != CModelToolApplication::GetInstance()->GetEditChar())
    //{
    //    CModelToolApplication::GetInstance()->SetEditChar(m_pCharacter);
    //    m_pCharacter->SetVisible(TRUE);
    //}
}
