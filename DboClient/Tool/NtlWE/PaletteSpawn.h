#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "SpawnManager.h"



// CPaletteSpawn 대화 상자입니다.


class CPaletteSpawnBase;
class CPaletteSpawnPoint;
class CPaletteSpawnParty;
class CPaletteSpawnPath;
class CPaletteSpawnSearch;


class CPaletteSpawn : public CDialog
{
	DECLARE_DYNAMIC(CPaletteSpawn)

public:
	CPaletteSpawn(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPaletteSpawn();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SPAWN_PALETTE };

public:
	CPaletteSpawnSearch*	m_pPaletteSpawnNavigator;
	CPaletteSpawnPath*		m_pPaletteSpawnPath;
	CPaletteSpawnBase*		m_pPaletteSpawnBase;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	VOID InitializeWorldData();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedMacroSetup();
public:
	afx_msg void OnBnClickedSpawnPaletteSearch();
public:
	CButton m_NPC;
	CButton m_MOB;
	//CButton m_PARTY;
	//CButton m_POINT;
public:
	afx_msg void OnBnClickedNpc();
public:
	afx_msg void OnBnClickedMob();
	/*
public:
	afx_msg void OnBnClickedPoint();
public:
	afx_msg void OnBnClickedParty();
	*/

public:
	HTREEITEM	GetItemByName(char* szItemName, HTREEITEM hItem);
	VOID		CreateTree(eSPAWN_TYPE _eSpawnType);
	VOID		Refresh();

public:
	CTreeCtrl m_SpawnTree;
public:
	afx_msg void OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult);


public:
	CString m_MacroIdx;
	CString m_CoolTime;
	CString m_MoveType;
	CString m_WanderRange;
	CString m_MoveRange;
	CString m_DelayTime;
	CComboBox m_CurMacroIdx;

	RwBool m_PathDialogOpend;
	RwBool m_NavigatorDialogOpend;

public:
	afx_msg void OnCbnSelchangeMacrotype();
public:
	afx_msg void OnBnClickedSpawnSave();
public:
	afx_msg void OnBnClickedSpawnLoad();
public:
	afx_msg void OnBnClickedSpawnExport();
public:
	//CButton m_PathListView;
public:
	//afx_msg void OnBnClickedPathlistview();
public:
	afx_msg void OnBnClickedPathOpen();
public:
	afx_msg void OnBnClickedSpawnMacroModify();
	CButton m_cbObjectSnap;
	afx_msg void OnBnClickedCheckObjectSnap();
	afx_msg void OnBnClickedCheckCircle();
	afx_msg void OnBnClickedCheckCircleScan();
	CButton m_BtnShow_Range;
	CButton m_BtnShow_Range_Scan;
};
