// ModelToolDoc.cpp : implementation of the CModelToolDoc class
//

#include "stdafx.h"
#include "ModelTool.h"

#include "ModelToolApplication.h"
#include "ModelToolDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CModelToolDoc

IMPLEMENT_DYNCREATE(CModelToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CModelToolDoc, CDocument)
    ON_COMMAND(ID_FILE_NEW, &CModelToolDoc::OnFileNew)
    ON_COMMAND(ID_FILE_SAVE, &CModelToolDoc::OnFileSave)
    ON_COMMAND(ID_FILE_SAVE_AS, &CModelToolDoc::OnFileSaveAs)
    ON_COMMAND(ID_VIEW_VIEWHIERARCHY, &CModelToolDoc::OnViewViewhierarchy)
    ON_COMMAND(ID_VIEW_VIEWWORLD, &CModelToolDoc::OnViewViewworld)
    ON_UPDATE_COMMAND_UI(ID_VIEW_VIEWWORLD, &CModelToolDoc::OnUpdateViewViewworld)
    ON_COMMAND(ID_VIEW_VIEWWIREFRAME, &CModelToolDoc::OnViewViewwireframe)
    ON_UPDATE_COMMAND_UI(ID_VIEW_VIEWHIERARCHY, &CModelToolDoc::OnUpdateViewViewhierarchy)
    ON_UPDATE_COMMAND_UI(ID_VIEW_VIEWWIREFRAME, &CModelToolDoc::OnUpdateViewViewwireframe)
END_MESSAGE_MAP()


// CModelToolDoc construction/destruction

CModelToolDoc::CModelToolDoc()
{
	// TODO: add one-time construction code here    

}

CModelToolDoc::~CModelToolDoc()
{
}

BOOL CModelToolDoc::OnNewDocument()
{
	//if (!CDocument::OnNewDocument())
	//	return FALSE;

	//// TODO: add reinitialization code here
	//// (SDI documents will reuse this document)

	return TRUE;
}




// CModelToolDoc serialization

void CModelToolDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CModelToolDoc diagnostics

#ifdef _DEBUG
void CModelToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CModelToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CModelToolDoc commands

BOOL CModelToolDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	//if (!CDocument::OnOpenDocument(lpszPathName))
	//	return FALSE;

	return TRUE;
}

void CModelToolDoc::OnFileNew()
{
    // TODO: Add your command handler code here
}

void CModelToolDoc::OnFileSave()
{
    // TODO: Add your command handler code here
}

void CModelToolDoc::OnFileSaveAs()
{
    // TODO: Add your command handler code here
}

void CModelToolDoc::OnViewViewhierarchy()
{
    // TODO: Add your command handler code here
    BOOL bViewHierarchy = CModelToolApplication::GetInstance()->GetHierarchyView();
    CModelToolApplication::GetInstance()->SetHierarchyView(!bViewHierarchy);
}

void CModelToolDoc::OnUpdateViewViewhierarchy(CCmdUI *pCmdUI)
{
    // TODO: Add your command update UI handler code here    
    BOOL bViewHierarchy = CModelToolApplication::GetInstance()->GetHierarchyView();
    pCmdUI->SetCheck(bViewHierarchy);
}

void CModelToolDoc::OnViewViewworld()
{
    // TODO: Add your command handler code here
    BOOL bViewWorld = CModelToolApplication::GetInstance()->GetWorldView();
    CModelToolApplication::GetInstance()->SetWorldView(!bViewWorld);
}

void CModelToolDoc::OnUpdateViewViewworld(CCmdUI *pCmdUI)
{
    // TODO: Add your command update UI handler code here
    BOOL bViewWorld = CModelToolApplication::GetInstance()->GetWorldView();
    pCmdUI->SetCheck(bViewWorld);    
}

void CModelToolDoc::OnViewViewwireframe()
{
    // TODO: Add your command handler code here
    BOOL bViewWireframe = CModelToolApplication::GetInstance()->GetWireView();
    CModelToolApplication::GetInstance()->SetWireView(!bViewWireframe);    
}

void CModelToolDoc::OnUpdateViewViewwireframe(CCmdUI *pCmdUI)
{
    // TODO: Add your command update UI handler code here
    BOOL bViewWireframe = CModelToolApplication::GetInstance()->GetWireView();
    pCmdUI->SetCheck(bViewWireframe);
}
