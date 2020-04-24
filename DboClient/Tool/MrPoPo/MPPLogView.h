#pragma once


// CMPPLogView 뷰입니다.

class CMPPLogView : public CRichEditView
{
	DECLARE_DYNCREATE(CMPPLogView)

protected:
	CMPPLogView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMPPLogView();

    static CMPPLogView* m_pInstance;

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
    static CMPPLogView* GetInstance() {return m_pInstance;}

    void AddLog(const CString& strLog);    
    void SetOwnerPane(CXTPDockingPane* pOwnerPane) {m_pOwnerPane = pOwnerPane;}

protected:
	DECLARE_MESSAGE_MAP()

protected:
    CXTPDockingPane*        m_pOwnerPane;
};

static void AddLogView(const CString& strLog)
{
    CMPPLogView::GetInstance()->AddLog(strLog);    
}