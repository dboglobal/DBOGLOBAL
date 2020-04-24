// TextureConverterView.h : iCTextureConverterView 클래스의 인터페이스
//


#pragma once


class CTextureConverterView : public CScrollView
{
protected: // serialization에서만 만들어집니다.
	CTextureConverterView();
	DECLARE_DYNCREATE(CTextureConverterView)

// 특성
public:
	CTextureConverterDoc* GetDocument() const;

// 작업
public:

// 재정의
	public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현
public:
	virtual ~CTextureConverterView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 메시지 맵 함수를 생성했습니다.
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // TextureConverterView.cpp의 디버그 버전
inline CTextureConverterDoc* CTextureConverterView::GetDocument() const
   { return reinterpret_cast<CTextureConverterDoc*>(m_pDocument); }
#endif

