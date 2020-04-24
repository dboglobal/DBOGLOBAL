// MainFrm.h : interface of the CMainFrame class
//
#pragma once

#pragma comment(lib, "ToolkitPro1321vc80.lib")

class CMainFrame : public CXTPFrameWnd
{

protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CXTPStatusBar            m_wndStatusBar;
	CXTPDockingPaneManager   m_paneManager;    
    CXTPReBar                m_wndReBar;
    CXTPControlEdit*         m_pEditCamraSense;             ///< ToolBar의 Camera Sensitive
    CXTPControlEdit*         m_pEditToonThickness;          ///< ToolBar의 Toon Thickness    
    CXTPControlPopupColor*   m_pCSToonColor;                ///< ToolBar의 Toon Color
    CXTPControlPopupColor*   m_pCSHeadSkinColor;            ///< ToolBar의 Head Skin Color
    CXTPControlPopupColor*   m_pCSBodySkinColor;            ///< ToolBar의 Body Skin Color
    COLORREF                 m_clrToonColor;                ///< 설정할 Toon Color
    COLORREF                 m_clrHeadSkinColor;            ///< 설정할 Head Skin Color
    COLORREF                 m_clrBodySkinColor;            ///< 설정할 Body Skin Color

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnCustomize();
	afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:

    afx_msg void OnPaneAnimationplay();
    afx_msg void OnPaneAnimationset();
    afx_msg void OnUpdatePaneAnimationplay(CCmdUI *pCmdUI);
    afx_msg void OnUpdatePaneAnimationset(CCmdUI *pCmdUI);
    afx_msg void OnPaneAttacktype();
    afx_msg void OnUpdatePaneAttacktype(CCmdUI *pCmdUI);
    afx_msg void OnPaneBboxedit();
    afx_msg void OnUpdatePaneBboxedit(CCmdUI *pCmdUI);
    afx_msg void OnPaneBoneedit();
    afx_msg void OnUpdatePaneBoneedit(CCmdUI *pCmdUI);
    afx_msg void OnPaneClumpedit();
    afx_msg void OnUpdatePaneClumpedit(CCmdUI *pCmdUI);
    afx_msg void OnPaneEventproperties();
    afx_msg void OnUpdatePaneEventproperties(CCmdUI *pCmdUI);
    afx_msg void OnPaneMaterialedit();
    afx_msg void OnUpdatePaneMaterialedit(CCmdUI *pCmdUI);
    afx_msg void OnPanePctree();
    afx_msg void OnUpdatePanePctree(CCmdUI *pCmdUI);
    afx_msg void OnPaneMobTree();
    afx_msg void OnUpdatePaneMobTree(CCmdUI* pCmdUI);
    afx_msg void OnPaneObjecttree();
    afx_msg void OnUpdatePaneObjecttree(CCmdUI *pCmdUI);
    afx_msg void OnPaneDummymodel();
    afx_msg void OnUpdatePaneDummymodel(CCmdUI *pCmdUI);
    afx_msg int OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl);
    afx_msg void OnCameraSense();
    afx_msg void OnUpdateSelectorBack(CCmdUI* pCmdUI);
    afx_msg void OnSelectorBack(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnUpdateBack(CCmdUI* pCmdUI);
    afx_msg void OnToonThickness();
    afx_msg void OnPaneItemtreee();
    afx_msg void OnUpdatePaneItemtreee(CCmdUI *pCmdUI);
    afx_msg void OnPaneEnviromentmap();
    afx_msg void OnPaneItemview();
    afx_msg void OnUpdatePaneItemview(CCmdUI *pCmdUI);
    afx_msg void OnPaneNpctree();
    afx_msg void OnUpdatePaneNpctree(CCmdUI *pCmdUI);    
    afx_msg void OnUpdateHeadSkinColor(CCmdUI* pCmdUI);
    afx_msg void OnUpdateBodySkinColor(CCmdUI* pCmdUI);
    afx_msg void OnSelectHeadSkinColor(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnSelectBodySkinColor(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnHeadSkinMore();
    afx_msg void OnBodySkinMore();
    void    SetVisibleNaviPane(BOOL bVisible);
public:
    afx_msg void OnPaneVehicleview();
public:
    afx_msg void OnUpdatePaneVehicleview(CCmdUI *pCmdUI);
};
