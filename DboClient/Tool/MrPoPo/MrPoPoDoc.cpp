// MrPoPoDoc.cpp : implementation of the CMrPoPoDoc class
//

#include "stdafx.h"
#include "MrPoPo.h"

#include "MrPoPoDoc.h"
#include "MPPDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMrPoPoDoc

IMPLEMENT_DYNCREATE(CMrPoPoDoc, CDocument)

BEGIN_MESSAGE_MAP(CMrPoPoDoc, CDocument)
END_MESSAGE_MAP()


// CMrPoPoDoc construction/destruction

CMrPoPoDoc::CMrPoPoDoc()
{
	// TODO: add one-time construction code here

}

CMrPoPoDoc::~CMrPoPoDoc()
{
}

BOOL CMrPoPoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CMrPoPoDoc serialization

void CMrPoPoDoc::Serialize(CArchive& ar)
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


// CMrPoPoDoc diagnostics

#ifdef _DEBUG
void CMrPoPoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMrPoPoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMrPoPoDoc commands

void CMrPoPoDoc::SetTitle(LPCTSTR lpszTitle)
{

    CDocument::SetTitle(MPP_TITLE_VER);
}
