#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CSaberTestDlg 대화 상자입니다.

class CSaberTestDlg : public CDialog
{
	DECLARE_DYNAMIC(CSaberTestDlg)

public:
	CSaberTestDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSaberTestDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SABER_TEST_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	void	InitializeWorldData();

protected:
	//void	TerrainTextrueCombine(RwUInt32 iSectorIdx);
	
public:	
	CString			m_strTest1;
	CString			m_strTest2;

	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

