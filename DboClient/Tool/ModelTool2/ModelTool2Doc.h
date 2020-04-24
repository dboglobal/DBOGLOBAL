// ModelTool2Doc.h : interface of the CModelTool2Doc class
//


#pragma once

#include "ConvertingDlg.h"


class CModelTool2Doc : public CDocument
{
protected: // create from serialization only
	CModelTool2Doc();
	DECLARE_DYNCREATE(CModelTool2Doc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CModelTool2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnViewViewbbox();
    afx_msg void OnUpdateViewViewbbox(CCmdUI *pCmdUI);
    afx_msg void OnViewViewworld();
    afx_msg void OnUpdateViewViewworld(CCmdUI *pCmdUI);
    afx_msg void OnViewViewwireframe();
    afx_msg void OnUpdateViewViewwireframe(CCmdUI *pCmdUI);
    afx_msg void OnViewViewheirarchy();
    afx_msg void OnUpdateViewViewheirarchy(CCmdUI *pCmdUI);
    virtual void SetTitle(LPCTSTR lpszTitle);
    afx_msg void OnCameraLeftview();
    afx_msg void OnCameraFrontview();
    afx_msg void OnCameraTopview();
    afx_msg void OnCameraBottomview();
    afx_msg void OnCameraRearview();
    afx_msg void OnCameraRightview();
    afx_msg void OnViewViewtoonedge();
//    afx_msg void OnUpdateViewViewtoonedge(CCmdUI *pCmdUI);
    afx_msg void OnToolbarCamraicon();
    afx_msg void OnToolbarThicknessicon();
    afx_msg void OnOptionHiteffect();
    afx_msg void OnUpdateOptionHiteffect(CCmdUI *pCmdUI);
    afx_msg void OnOptionCollisionmesh();
    afx_msg void OnUpdateOptionCollisionmesh(CCmdUI *pCmdUI);
    afx_msg void OnItemgradeeffectNone();
    afx_msg void OnItemgradeeffect1();
    afx_msg void OnItemgradeeffect2();
    afx_msg void OnItemgradeeffect3();
    afx_msg void OnItemgradeeffect4();
    afx_msg void OnOptionGlowfilter();
    afx_msg void OnUpdateOptionGlowfilter(CCmdUI *pCmdUI);
    afx_msg void OnOptionBlurfilter();
    afx_msg void OnUpdateOptionBlurfilter(CCmdUI *pCmdUI);
    afx_msg void OnOptionFramefix();
    afx_msg void OnUpdateOptionFramefix(CCmdUI *pCmdUI);
	afx_msg void OnFileConvertbinary();
    afx_msg void OnFileConvertencryptbinary();
    afx_msg void OnCameraFreecamera();
    afx_msg void OnUpdateCameraFreecamera(CCmdUI *pCmdUI);
    afx_msg void OnOptionCameradistancealpha();
    afx_msg void OnUpdateOptionCameradistancealpha(CCmdUI *pCmdUI);
    afx_msg void OnFileUpdatepropertydata();
    afx_msg void OnOptionCamerashakedisable();
    afx_msg void OnUpdateOptionCamerashakedisable(CCmdUI *pCmdUI);

protected:
	CConvertingDlg		m_dlgConvert;
};
