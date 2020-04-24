#pragma once

#include "explorer_information.h"

class CHTTPTransparent : public CExplorer_information
{
    DECLARE_DYNAMIC(CHTTPTransparent)

public:
    CHTTPTransparent(void);
    ~CHTTPTransparent(void);

protected:
    DECLARE_MESSAGE_MAP()

    HBITMAP    m_hBitMap;
public:
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
