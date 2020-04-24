// NaviToolView.cpp : CNaviToolView 클래스의 구현
//

#include "stdafx.h"
#include "NaviTool.h"

#include "NaviToolDoc.h"
#include "NaviToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNaviToolView

IMPLEMENT_DYNCREATE(CNaviToolView, CView)

BEGIN_MESSAGE_MAP(CNaviToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CNaviToolView 생성/소멸

CNaviToolView::CNaviToolView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CNaviToolView::~CNaviToolView()
{
}

BOOL CNaviToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CNaviToolView 그리기

void CNaviToolView::OnDraw(CDC* /*pDC*/)
{
	CNaviToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CNaviToolView 인쇄

BOOL CNaviToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CNaviToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CNaviToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CNaviToolView 진단

#ifdef _DEBUG
void CNaviToolView::AssertValid() const
{
	CView::AssertValid();
}

void CNaviToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CNaviToolDoc* CNaviToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNaviToolDoc)));
	return (CNaviToolDoc*)m_pDocument;
}
#endif //_DEBUG
BOOL CNaviToolView::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	return CView::OnWndMsg(message, wParam, lParam, pResult);
}
