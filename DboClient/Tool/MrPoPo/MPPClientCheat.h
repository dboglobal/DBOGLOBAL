#pragma once
#include "afxwin.h"

/**
 * \ingroup MrPoPo
 * \brief 클라이언트의 치트키들을 실행시키는 클래스
 *
 * \date 2008-09-04
 * \author agebreak
 */
class CMPPClientCheat : public CXTResizeFormView
{
	DECLARE_DYNCREATE(CMPPClientCheat)

protected:
	CMPPClientCheat();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMPPClientCheat();

public:
	enum { IDD = IDD_MPPCLIENTCHEAT };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    virtual void OnInitialUpdate();
    afx_msg void OnBnClickedBtElapsed();
    afx_msg void OnBnClickedBtDumpConsole();
    afx_msg void OnBnClickedBtDumpGui();
    afx_msg void OnBnClickedBtDumpFile();
    afx_msg void OnBnClickedDumpTarget();
    afx_msg void OnBnClickedDumpReg();
    afx_msg void OnBnClickedDumpUnreg();
    afx_msg void OnBnClickedBtLowspecChar();

public:
    CEdit m_edElapsed;
    CComboBox m_cbDump;
public:
    afx_msg void OnBnClickedBtLowspecEffect();
public:
    afx_msg void OnBnClickedBtLowspecMesh();
public:
    afx_msg void OnBnClickedBtLowspecDecal();
};


