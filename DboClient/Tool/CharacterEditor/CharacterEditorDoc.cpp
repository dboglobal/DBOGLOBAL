// CharacterEditorDoc.cpp : CCharacterEditorDoc 클래스의 구현
//

#include "stdafx.h"
#include "CharacterEditor.h"

#include "CharacterEditorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCharacterEditorDoc

IMPLEMENT_DYNCREATE(CCharacterEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CCharacterEditorDoc, CDocument)
END_MESSAGE_MAP()


// CCharacterEditorDoc 생성/소멸

CCharacterEditorDoc::CCharacterEditorDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CCharacterEditorDoc::~CCharacterEditorDoc()
{
}

BOOL CCharacterEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 다시 초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CCharacterEditorDoc serialization

void CCharacterEditorDoc::Serialize(CArchive& ar)
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


// CCharacterEditorDoc 진단

#ifdef _DEBUG
void CCharacterEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCharacterEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CCharacterEditorDoc 명령
