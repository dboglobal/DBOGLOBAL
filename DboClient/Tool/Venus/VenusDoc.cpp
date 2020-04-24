// VenusDoc.cpp : CVenusDoc 클래스의 구현
//

#include "stdafx.h"
#include "Venus.h"

#include "VenusDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVenusDoc

IMPLEMENT_DYNCREATE(CVenusDoc, CDocument)

BEGIN_MESSAGE_MAP(CVenusDoc, CDocument)
END_MESSAGE_MAP()


// CVenusDoc 생성/소멸

CVenusDoc::CVenusDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CVenusDoc::~CVenusDoc()
{
}

BOOL CVenusDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 다시 초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CVenusDoc serialization

void CVenusDoc::Serialize(CArchive& ar)
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


// CVenusDoc 진단

#ifdef _DEBUG
void CVenusDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CVenusDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CVenusDoc 명령
