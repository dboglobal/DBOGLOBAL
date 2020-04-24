#include "StdAfx.h"
#include "HTTPTransparent.h"
#include "Resource.h"

IMPLEMENT_DYNAMIC(CHTTPTransparent, CWnd)

BEGIN_MESSAGE_MAP(CHTTPTransparent, CWnd)    
    ON_WM_CTLCOLOR()        
    ON_WM_ERASEBKGND()
    ON_WM_CREATE()
END_MESSAGE_MAP()


CHTTPTransparent::CHTTPTransparent(void)
{
}

CHTTPTransparent::~CHTTPTransparent(void)
{

}

HBRUSH CHTTPTransparent::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    //HBRUSH hbr = CExplorer_information::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  여기서 DC의 특성을 변경합니다.

    // TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.

    pDC->SetBkMode(TRANSPARENT);    
    return (HBRUSH)GetStockObject(NULL_BRUSH);
}

BOOL CHTTPTransparent::OnEraseBkgnd(CDC* pDC)
{
    if (!m_hBitMap)
        return FALSE;

    CRect   rect;
    GetClientRect(rect);

    CDC dc;
    dc.CreateCompatibleDC(pDC);

    HBITMAP pbmpOldBmp = (HBITMAP)::SelectObject(dc.m_hDC, m_hBitMap);

    pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &dc, 0, 0,SRCCOPY);

    ::SelectObject(dc.m_hDC, m_hBitMap);

    return FALSE;

    return CExplorer_information::OnEraseBkgnd(pDC);
}

int CHTTPTransparent::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CExplorer_information::OnCreate(lpCreateStruct) == -1)
        return -1;

    HINSTANCE  hInstResource = NULL;
    // Find correct resource handle
    hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_HTTP_BACK), RT_BITMAP);    
    // Load bitmap In
    m_hBitMap = (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(IDB_HTTP_BACK),
                                    IMAGE_BITMAP, 0, 0, 0);

    return 0;
}
