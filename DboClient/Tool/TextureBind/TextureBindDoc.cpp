// TextureBindDoc.cpp : CTextureBindDoc 클래스의 구현
//

#include "stdafx.h"
#include "TextureBind.h"

#include "TextureBindDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTextureBindDoc

IMPLEMENT_DYNCREATE(CTextureBindDoc, CDocument)

BEGIN_MESSAGE_MAP(CTextureBindDoc, CDocument)
END_MESSAGE_MAP()


// CTextureBindDoc 생성/소멸

CTextureBindDoc::CTextureBindDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CTextureBindDoc::~CTextureBindDoc()
{
}

BOOL CTextureBindDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CTextureBindDoc serialization

void CTextureBindDoc::Serialize(CArchive& ar)
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


// CTextureBindDoc 진단

#ifdef _DEBUG
void CTextureBindDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTextureBindDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTextureBindDoc 명령
