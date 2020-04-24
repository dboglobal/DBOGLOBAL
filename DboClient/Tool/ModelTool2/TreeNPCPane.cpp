// TreeNPCPane.cpp : implementation file
//

#include "stdafx.h"
#include "ModelTool2.h"
#include "TreeNPCPane.h"


CTreeNPCPane* CTreeNPCPane::m_pInstance  = NULL;

// CTreeNPCPane

IMPLEMENT_DYNCREATE(CTreeNPCPane, CTreePCPane)

CTreeNPCPane::CTreeNPCPane()
{
    m_pInstance = this;
    
    m_pImageList = NULL;
    m_pCharacter = NULL;

    m_strRootName = "NPC";
}

CTreeNPCPane::~CTreeNPCPane()
{
}

BEGIN_MESSAGE_MAP(CTreeNPCPane, CTreePCPane)
    ON_WM_SETFOCUS()
END_MESSAGE_MAP()


// CTreeNPCPane diagnostics

#ifdef _DEBUG
void CTreeNPCPane::AssertValid() const
{
	CTreePCPane::AssertValid();
}

#ifndef _WIN32_WCE
void CTreeNPCPane::Dump(CDumpContext& dc) const
{
	CTreePCPane::Dump(dc);
}


#endif
#endif //_DEBUG


// CTreeNPCPane message handlers

void CTreeNPCPane::InitClass() 
{
    USES_CONVERSION;

    CString sWorkFolder = (CModelToolApplication::GetInstance()->GetWorkDir());
    m_sSaveFolderName = sWorkFolder;
    m_sSaveFolderName += MT_PATH_CHARACTER;

    m_strTreeFileName = sWorkFolder;
    m_strTreeFileName += "\\Tool\\ModelTool\\TreeNPC.xml";

    m_strPropertyListFileName = m_sSaveFolderName;
    m_strPropertyListFileName += "NPCPropertyList.xml";
}

void CTreeNPCPane::OnSetFocus(CWnd* pOldWnd)
{
    CTreePCPane::OnSetFocus(pOldWnd);

    CModelToolApplication::GetInstance()->SetAppMode(MT_MODE_NPC);
}
