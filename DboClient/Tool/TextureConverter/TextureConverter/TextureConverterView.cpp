// TextureConverterView.cpp : CTextureConverterView 클래스의 구현
//

#include "stdafx.h"
#include "TextureConverter.h"

#include "TextureConverterDoc.h"
#include "TextureConverterView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTextureConverterView

IMPLEMENT_DYNCREATE(CTextureConverterView, CScrollView)

BEGIN_MESSAGE_MAP(CTextureConverterView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CTextureConverterView 생성/소멸

CTextureConverterView::CTextureConverterView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CTextureConverterView::~CTextureConverterView()
{
}

BOOL CTextureConverterView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	// Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CTextureConverterView 그리기

void CTextureConverterView::OnDraw(CDC* pDC)
{
	CTextureConverterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.

	for( int nIndex = 0; nIndex < GetDocument()->GetFileCount(); ++nIndex )
	{
		pDC->TextOut( 0, nIndex * 16, *(GetDocument()->GetFileName( nIndex )) );
	}
}

void CTextureConverterView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CTextureConverterView 인쇄

BOOL CTextureConverterView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CTextureConverterView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CTextureConverterView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CTextureConverterView 진단

#ifdef _DEBUG
void CTextureConverterView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CTextureConverterView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CTextureConverterDoc* CTextureConverterView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTextureConverterDoc)));
	return (CTextureConverterDoc*)m_pDocument;
}
#endif //_DEBUG


// CTextureConverterView 메시지 처리기
