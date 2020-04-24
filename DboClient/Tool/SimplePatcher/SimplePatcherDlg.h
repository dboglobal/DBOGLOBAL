// SimplePatcherDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "resource.h"


class CLocalConfig;
class CPatchHistory;
class CPatcher;


class CSimplePatcherDlg : public CDialog
{
public:
	enum { IDD = IDD_SIMPLEPATCHER_DIALOG };

	enum { ePROGRESS_RANGE = 10000 };

protected:
	HICON			m_hIcon;
	CProgressCtrl	m_ctrProgress;

	CLocalConfig*	m_pLocalConfig;
	CPatchHistory*	m_pPatchHistory;
	CPatcher*		m_pPatcher;

	int				m_nPatchTotalCnt;
	int				m_nPatchCurCnt;

public:
	CSimplePatcherDlg(CWnd* pParent = NULL);	// standard constructor

public:
	void PatchBegin( int nPatchCnt );
	void PatchUpdate( void );
	void PatchEnd( void );

protected:
	void UpdateProgress( void );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL DestroyWindow();

protected:
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedSimplepatcherPatchStart();
	virtual void OnCancel();
};
