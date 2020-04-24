// GUIEditorDoc.cpp : CGUIEditorDoc 클래스의 구현
//

#include "stdafx.h"
#include "GUIEditor.h"

#include "GUIEditorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CGUIEditorDoc* g_pDocument = NULL;


// CGUIEditorDoc

IMPLEMENT_DYNCREATE(CGUIEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CGUIEditorDoc, CDocument)
END_MESSAGE_MAP()


// CGUIEditorDoc 생성/소멸

CGUIEditorDoc::CGUIEditorDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	g_pDocument = this;
}

CGUIEditorDoc::~CGUIEditorDoc()
{
}

BOOL CGUIEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 다시 초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CGUIEditorDoc serialization

void CGUIEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}


// CGUIEditorDoc 진단

#ifdef _DEBUG
void CGUIEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGUIEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGUIEditorDoc 명령
