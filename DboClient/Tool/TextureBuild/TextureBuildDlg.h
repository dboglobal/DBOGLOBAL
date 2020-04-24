// TextureBuildDlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"

#include "GlobalDef.h"
#include "FileTreeCtrl.h"

// CTextureBuildDlg 대화 상자
class CTextureBuildDlg : public CDialog
{
// 생성입니다.
public:
	CTextureBuildDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TEXTUREBUILD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
	
// 구현입니다.
protected:
	HICON			m_hIcon;
	CFileTreeCtrl	m_treeFile;
	CProgressCtrl	m_proAllItems;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	afx_msg void OnNMClickTreeFile(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickTreeFile(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void On0Setuplevel0();
	afx_msg void On0Setuplevel1();
	afx_msg void On0Setuplevel2();
	afx_msg void On0Setuplevel3();
	afx_msg void On0Export();

	CFileTreeCtrl* GetTreeFile() { return &m_treeFile; }
	CProgressCtrl* GetProgressCtrl() { return &m_proAllItems; }
	afx_msg void On0Texturenum();
};
