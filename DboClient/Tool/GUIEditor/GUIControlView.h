#pragma once



// CGUIControlView 폼 뷰입니다.

class CGUIControlView : public CFormView
{
	DECLARE_DYNCREATE(CGUIControlView)

protected:
	CGUIControlView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CGUIControlView();

public:
	enum { IDD = IDD_FORMVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};

extern CGUIControlView* g_pControlView;

