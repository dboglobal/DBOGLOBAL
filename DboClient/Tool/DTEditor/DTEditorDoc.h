// DTEditorDoc.h : interface of the CDTEditorDoc class
//


#pragma once

#define MAX_DIR_PATH	2048

class CNtlDTContainer;

VOID DTEditorExceptionHandler( PEXCEPTION_POINTERS pExceptionInfo );


class CDTEditorDoc : public CDocument
{
protected: // create from serialization only
	CDTEditorDoc();
	DECLARE_DYNCREATE(CDTEditorDoc)

// Attributes
public:

	TCHAR m_szWorkDir[MAX_DIR_PATH];
	CString m_strWorkFull;
	CString m_strWorkFileName;
	CString m_strWorkFileTitle;
	CString m_strWorkPath;

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

	void LoadPrj(CString& strFileName);
	void SavePrj(CString& strFileName);

	void ImportPrj(CString& strFileName, CNtlDTContainer *pDTContainer);
	void OnImportFile(void);

// Implementation
public:
	virtual ~CDTEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileNew();
public:
	afx_msg void OnFileSave();
public:
	afx_msg void OnFileSaveAs();
public:
	afx_msg void OnFileOpen();
public:
//	afx_msg void OnEditCopy();
};
