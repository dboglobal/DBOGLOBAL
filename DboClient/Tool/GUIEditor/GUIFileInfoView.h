#pragma once

// CGUIFileInfoView form view
#include "resource.h"

class CGUIFileInfoView : public CFormView
{
	DECLARE_DYNCREATE(CGUIFileInfoView)

protected:
	CGUIFileInfoView();           // protected constructor used by dynamic creation
	virtual ~CGUIFileInfoView();

public:
	enum { IDD = IDD_FILEINFOVIEW };
	
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	void SetRsrText( CString& strFilename );
	void SetSrfText( CString& strFilename );
	void SetFrmText( CString& strFilename );
	void SetWorkDirText( CString& strPath );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);

protected:

	CString m_strRsrFilename;
	CString m_strSrfFilename;
	CString m_strFrmFilename;
	CString m_strWorkDir;
};

extern CGUIFileInfoView* g_pFileInfoView;

