// LauncherDlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "BitmapDlg.h"
#include "ImageTextButton.h"
#include "OutputBox.h"
#include "HTTPTransparent.h"

// CLauncherDlg 대화 상자
class CLauncherDlg : public CBitmapDlg
{
// 생성입니다.
public:
	CLauncherDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_LAUNCHER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    
	DECLARE_MESSAGE_MAP()
public:
	int PatchManagerCallBack(unsigned int uiEventID, unsigned int pParam);
    virtual BOOL DestroyWindow();
    afx_msg void OnClose();
    afx_msg void OnBnClickedButtonGameStart();
    afx_msg void OnBnClickedButtonCancel();

protected:
	void AttachBackSlash( CString& strString );

public:
    COutputBox m_StaticFileDown;	
    COutputBox m_StaticInfoOutput;    
    COutputBox m_StaticLog;
    COutputBox m_StaticFIlePatch;

    int m_nIDProgress1;
    int m_nIDProgress2;
    int m_nDownValue;
    int m_nPatchValue;

	CImageTextButton m_ButtonGameStart;
	CImageTextButton m_ButtonCancel;

    CString m_strPatchVer;                          ///< 현재까지 패치된 클라이언트 버전
    CString m_strLauncherVer;                       ///< 런처 버전
	
	CHTTPTransparent m_ExplorerInformation;
};
