// PackEditorDlg.h : header file
//

#pragma once

#include "NotifyDlg.h"
#include "afxcmn.h"
#include "afxwin.h"

#include "NtlPLResourcePack.h"


#define EXPORT_TIMER_ID		20

#define PACK_USER_MASTER_ALL		0
#define PACK_USER_MASTER_PACK		1
#define PACK_USER_MASTER_UNPACK		2


// CPackEditorDlg dialog
class CPackEditorDlg : public CDialog
{
// Construction
public:

	HANDLE			m_hThread;
	unsigned int	m_uiThreaID;

	static BYTE		m_byPackUserLevel;
	static BOOL		m_bThreadExport;
	static INT		m_nResult;
	static DWORD	m_dwPackTime;

	static BOOL		m_bUnPack;
	static BYTE		m_byPackType;

	CNotifyDlg		m_dlgNotify;

	void			CreateThread(void);
	void			DeleteThread(void);

	static unsigned int __stdcall PackExport(void* pParam);

	void			EnableAll(void);
	void			DisableAll(void);

	void			UpdateNotifyWindow(void);

	int				MakePackNotify(unsigned int uiNotifyString ,unsigned int uiPersent);

	CPackEditorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PACKEDITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

    void    ExportPack(EFilePackType eType);
public:
	afx_msg void OnBnClickedBtnTextureExport();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnObjectExport();
	afx_msg void OnBnClickedBtnTerrainExport();
	afx_msg void OnBnClickedBtnGuiExport();
	afx_msg void OnBnClickedBtnFlashExport();
	afx_msg void OnBnClickedBtnSoundExport();
    afx_msg void OnBnClickedBtnLanguageExport();
    afx_msg void OnBnClickedBtnScriptExport();
    afx_msg void OnBnClickedBtnTableExport();
	afx_msg void OnBnClickedBtnPropertyExport();

	afx_msg void OnBnClickedBtnUnpack();

public:
	CProgressCtrl m_ctrlPackPersent;
	CStatic m_ctrlPackNotify;
};
