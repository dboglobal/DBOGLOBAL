// ModelToolDoc.h : interface of the CModelToolDoc class
//


#pragma once


class CModelToolDoc : public CDocument
{
protected: // create from serialization only
	CModelToolDoc();
	DECLARE_DYNCREATE(CModelToolDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CModelToolDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
public:
    afx_msg void OnFileNew();
public:
    afx_msg void OnFileSave();
public:
    afx_msg void OnFileSaveAs();
public:
    afx_msg void OnViewViewhierarchy();
public:
    afx_msg void OnViewViewworld();
public:
    afx_msg void OnUpdateViewViewworld(CCmdUI *pCmdUI);
public:
    afx_msg void OnViewViewwireframe();
public:
    afx_msg void OnUpdateViewViewhierarchy(CCmdUI *pCmdUI);
public:
    afx_msg void OnUpdateViewViewwireframe(CCmdUI *pCmdUI);
};


