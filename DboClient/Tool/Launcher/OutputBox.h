#if !defined(AFX_OUTPUTBOX_H__E6E22F26_54FD_4361_A548_C773EE24EC39__INCLUDED_)
#define AFX_OUTPUTBOX_H__E6E22F26_54FD_4361_A548_C773EE24EC39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OutputBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COutputBox window

class COutputBox : public CStatic
{
// Construction
public:
	COutputBox();
    virtual ~COutputBox();

    void EraseBack(CDialog* pDlg);      // 글씨영역을 지운다.

public:
    bool        m_bIsLog;

	
protected:
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTPUTBOX_H__E6E22F26_54FD_4361_A548_C773EE24EC39__INCLUDED_)
