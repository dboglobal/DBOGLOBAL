	#pragma once
#include "afxcmn.h"


// CPaletteEffect dialog

class CPaletteEffect : public CDialog
{
	DECLARE_DYNAMIC(CPaletteEffect)

public:
	CPaletteEffect(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPaletteEffect();

// Dialog Data
	enum { IDD = IDD_EFFECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL	PreTranslateMessage(MSG* pMsg);
	virtual BOOL	OnInitDialog();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	void	InitializeWorldData();
	void	CreateLeafNode(const char *pName);
	void	UpdatePropertyData(CNtlPLEntity* pPLEntity);

	afx_msg void OnTvnSelchangedEffectTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

public:
	CTreeCtrl m_EffectTreeCtrl;
	HTREEITEM m_hRoot;
	HTREEITEM m_hRootWorld;

public:
	CString m_strScale;
	CString m_strRotateTab;
	CString m_strTranslateTab;
	CSliderCtrl m_SliderVisibleDist;
	CString m_strVisibleDist;
};
