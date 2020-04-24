// WorldEditorDoc.cpp : CWorldEditorDoc 클래스의 구현
//

#include "stdafx.h"
#include "WorldEditor.h"

#include "WorldEditorDoc.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif


// CWorldEditorDoc

IMPLEMENT_DYNCREATE(CWorldEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CWorldEditorDoc, CDocument)
	ON_COMMAND(ID_FILE_SAVE, CWorldEditorDoc::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, CWorldEditorDoc::OnFileSaveAs)
END_MESSAGE_MAP()


// CWorldEditorDoc 생성/소멸

CWorldEditorDoc::CWorldEditorDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CWorldEditorDoc::~CWorldEditorDoc()
{
}

BOOL CWorldEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 다시 초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}


void CWorldEditorDoc::OnFileSave()
{
	theApp.OnFileSave();
}

void CWorldEditorDoc::OnFileSaveAs()
{
	theApp.OnFileSaveAs();
}


// CWorldEditorDoc serialization

void CWorldEditorDoc::Serialize(CArchive& ar)
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


// CWorldEditorDoc 진단

#ifdef _DEBUG
void CWorldEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWorldEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CWorldEditorDoc 명령
