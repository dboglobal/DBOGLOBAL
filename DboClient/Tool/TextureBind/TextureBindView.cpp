// TextureBindView.cpp : CTextureBindView 클래스의 구현
//

#include "stdafx.h"
#include "TextureBind.h"

#include "TextureBindDoc.h"
#include "TextureBindView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTextureBindView

IMPLEMENT_DYNCREATE(CTextureBindView, CView)

BEGIN_MESSAGE_MAP(CTextureBindView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CTextureBindView 생성/소멸

CTextureBindView::CTextureBindView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CTextureBindView::~CTextureBindView()
{
}

BOOL CTextureBindView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CTextureBindView 그리기

void CTextureBindView::OnDraw(CDC* /*pDC*/)
{
	CTextureBindDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CTextureBindView 인쇄

BOOL CTextureBindView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CTextureBindView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CTextureBindView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CTextureBindView 진단

#ifdef _DEBUG
void CTextureBindView::AssertValid() const
{
	CView::AssertValid();
}

void CTextureBindView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTextureBindDoc* CTextureBindView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTextureBindDoc)));
	return (CTextureBindDoc*)m_pDocument;
}
#endif //_DEBUG


// CTextureBindView 메시지 처리기
