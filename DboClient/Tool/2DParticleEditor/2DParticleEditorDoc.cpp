// 2DParticleEditorDoc.cpp : implementation of the CMy2DParticleEditorDoc class
//

#include "stdafx.h"
#include "2DParticleEditor.h"
#include "2DParticleEditorDoc.h"
#include "MainFrm.h"

#include "gui_particle_doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMy2DParticleEditorDoc* g_Doc = NULL;
// CMy2DParticleEditorDoc

IMPLEMENT_DYNCREATE(CMy2DParticleEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CMy2DParticleEditorDoc, CDocument)
END_MESSAGE_MAP()


// CMy2DParticleEditorDoc construction/destruction

CMy2DParticleEditorDoc::CMy2DParticleEditorDoc()
{
	// TODO: add one-time construction code here
	g_Doc = this;
}

CMy2DParticleEditorDoc::~CMy2DParticleEditorDoc()
{
}

BOOL CMy2DParticleEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CMy2DParticleEditorDoc serialization

void CMy2DParticleEditorDoc::Serialize(CArchive& ar)
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


// CMy2DParticleEditorDoc diagnostics

#ifdef _DEBUG
void CMy2DParticleEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMy2DParticleEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


void CMy2DParticleEditorDoc::LoadAttr( LPCTSTR lpszPathName )
{
	// TODO:  Add specialized writing code here.
	gui::GetParticleDoc()->Load( lpszPathName );
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	CAttributePane* pAttrPane = pFrm->GetAttributePane();
	if( pAttrPane )
	{
		pAttrPane->LoadTree( gui::GetParticleDoc() );
	}	
}
