#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "NtlMtXml.h"

// CPaletteOrnament 대화 상자입니다.


#define dNTL_ORNAMENT_OBJ_NUM 13


class CNtlPLEntity;


class CPaletteOrnament : public CDialog
{
	DECLARE_DYNAMIC(CPaletteOrnament)

public:
	CPaletteOrnament(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPaletteOrnament();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ORNAMENT };

public:
	void InitializeWorldData();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()

public:
	HTREEITEM m_hRoot;
	HTREEITEM m_hCat0TreeItem[dNTL_ORNAMENT_OBJ_NUM];
	CTreeCtrl m_TreeOrnament;
	CString m_strTapMovementEdit;

public:
	VOID		RenderPreview();
	VOID		UpdatePreview();
	void		CreateLeafNode(const char *pName, HTREEITEM hTreeItem);
	HTREEITEM	GetItemByName(char* szItemName, HTREEITEM hItem);
	VOID		RefreshScaleInfo(CNtlPLEntity* pEntity);

public:
	afx_msg void OnBnClickedDoodadSrandPb();
	afx_msg void OnBnClickedDoodadRrandPb();
	afx_msg void OnBnClickedDoodadTrandPb();
	afx_msg void OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void	InsertItemRecursive(SItemNode* pItemNode, HTREEITEM hParentItem);
	
public:
	CString m_strRotTabValue;
	CString m_StrScaleValue;
	CString m_CurDoodadScaleX;
	CString m_CurDoodadScaleY;
	CString m_CurDoodadScaleZ;
};
