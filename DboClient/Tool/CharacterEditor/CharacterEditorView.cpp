// CharacterEditorView.cpp : CCharacterEditorView 클래스의 구현
//

#include "stdafx.h"
#include "DboCharacterApplication.h"
#include "CharacterEditor.h"

#include "CharacterEditorDoc.h"
#include "CharacterEditorView.h"
#include ".\charactereditorview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCharacterEditorView

IMPLEMENT_DYNCREATE(CCharacterEditorView, CView)

BEGIN_MESSAGE_MAP(CCharacterEditorView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CCharacterEditorView 생성/소멸

CCharacterEditorView::CCharacterEditorView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CCharacterEditorView::~CCharacterEditorView()
{
}

BOOL CCharacterEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	// Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CCharacterEditorView 그리기

void CCharacterEditorView::OnDraw(CDC* /*pDC*/)
{
	CCharacterEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CCharacterEditorView 인쇄

BOOL CCharacterEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CCharacterEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CCharacterEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CCharacterEditorView 진단

#ifdef _DEBUG
void CCharacterEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CCharacterEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCharacterEditorDoc* CCharacterEditorView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCharacterEditorDoc)));
	return (CCharacterEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CCharacterEditorView 메시지 처리기

void CCharacterEditorView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	RECT rc;
	GetClientRect(&rc);
	CDboCharacterApplication::GetInstance()->Resize(rc.right-rc.left, rc.bottom - rc.top, TRUE);
}

BOOL CCharacterEditorView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	theApp.m_DboApplication.WndProc(pMsg->hwnd, pMsg->message, pMsg->wParam, pMsg->lParam);
	return CView::PreTranslateMessage(pMsg);
}
