#pragma once


#include "afxcmn.h"
#include "PathHandler.h"


// CPaletteSpawnPath 대화 상자입니다.

class CPaletteSpawnPath : public CDialog
{
	DECLARE_DYNAMIC(CPaletteSpawnPath)

public:
	CPaletteSpawnPath(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPaletteSpawnPath();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SPAWN_PATH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL	PreTranslateMessage(MSG* pMsg);

public:
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnBnClickedExport();
	afx_msg void OnBnClickedPortal();

public:
	CString m_StrPathIndex;
	CString m_StrPathRadius;
	CString m_StrPathMemo;
	CListCtrl m_LcPathList;
	CListCtrl m_LcPointList;

public:
	RwInt32 GetCurUID()		{ UpdateData(TRUE); return static_cast<RwInt32>(atoi(m_StrPathIndex.GetBuffer(0))); }
	RwReal GetCurRadius()	{ UpdateData(TRUE); return static_cast<RwReal>(atof(m_StrPathRadius.GetBuffer(0))); }
	RwChar* GetMemo()		{ UpdateData(TRUE); return static_cast<RwChar*>(m_StrPathMemo.GetBuffer(0)); }
	RwInt32 GetCurPathListIndex();
	void	CreatePathList(CPathHandler* _PathHandler, RwInt32 SelIdx);
	void	CreatePathPoint(CPathList* _pPathList, RwInt32 SelIdx);
	void	SelectPathList(RwInt32 iIndex);
	VOID	RefreshPathList();
	VOID	RefreshPathPoint();
	VOID	ClearEditBox();
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnLvnItemchangedPathlist(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnLvnItemchangedPathpointlist(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnLvnColumnclickPathlist(NMHDR *pNMHDR, LRESULT *pResult);
};
