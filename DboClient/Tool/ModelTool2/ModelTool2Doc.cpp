// ModelTool2Doc.cpp : implementation of the CModelTool2Doc class
//

#include "stdafx.h"
#include "ModelTool2.h"
#include "ModelToolApplication.h"
#include "ModelTool2Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CModelTool2Doc

IMPLEMENT_DYNCREATE(CModelTool2Doc, CDocument)

BEGIN_MESSAGE_MAP(CModelTool2Doc, CDocument)
    ON_COMMAND(ID_VIEW_VIEWBBOX, &CModelTool2Doc::OnViewViewbbox)
    ON_UPDATE_COMMAND_UI(ID_VIEW_VIEWBBOX, &CModelTool2Doc::OnUpdateViewViewbbox)
    ON_COMMAND(ID_VIEW_VIEWWORLD, &CModelTool2Doc::OnViewViewworld)
    ON_UPDATE_COMMAND_UI(ID_VIEW_VIEWWORLD, &CModelTool2Doc::OnUpdateViewViewworld)
    ON_COMMAND(ID_VIEW_VIEWWIREFRAME, &CModelTool2Doc::OnViewViewwireframe)
    ON_UPDATE_COMMAND_UI(ID_VIEW_VIEWWIREFRAME, &CModelTool2Doc::OnUpdateViewViewwireframe)
    ON_COMMAND(ID_VIEW_VIEWHEIRARCHY, &CModelTool2Doc::OnViewViewheirarchy)
    ON_UPDATE_COMMAND_UI(ID_VIEW_VIEWHEIRARCHY, &CModelTool2Doc::OnUpdateViewViewheirarchy)
    ON_COMMAND(ID_CAMERA_LEFTVIEW, &CModelTool2Doc::OnCameraLeftview)
    ON_COMMAND(ID_CAMERA_FRONTVIEW, &CModelTool2Doc::OnCameraFrontview)
    ON_COMMAND(ID_CAMERA_TOPVIEW, &CModelTool2Doc::OnCameraTopview)
    ON_COMMAND(ID_CAMERA_BOTTOMVIEW, &CModelTool2Doc::OnCameraBottomview)
    ON_COMMAND(ID_CAMERA_REARVIEW, &CModelTool2Doc::OnCameraRearview)
    ON_COMMAND(ID_CAMERA_RIGHTVIEW, &CModelTool2Doc::OnCameraRightview)
    ON_COMMAND(ID_VIEW_VIEWTOONEDGE, &CModelTool2Doc::OnViewViewtoonedge)
//    ON_UPDATE_COMMAND_UI(ID_VIEW_VIEWTOONEDGE, &CModelTool2Doc::OnUpdateViewViewtoonedge)
ON_COMMAND(ID_TOOLBAR_CAMRAICON, &CModelTool2Doc::OnToolbarCamraicon)
ON_COMMAND(ID_TOOLBAR_THICKNESSICON, &CModelTool2Doc::OnToolbarThicknessicon)
ON_COMMAND(ID_OPTION_HITEFFECT, &CModelTool2Doc::OnOptionHiteffect)
ON_UPDATE_COMMAND_UI(ID_OPTION_HITEFFECT, &CModelTool2Doc::OnUpdateOptionHiteffect)
ON_COMMAND(ID_OPTION_COLLISIONMESH, &CModelTool2Doc::OnOptionCollisionmesh)
ON_UPDATE_COMMAND_UI(ID_OPTION_COLLISIONMESH, &CModelTool2Doc::OnUpdateOptionCollisionmesh)
ON_COMMAND(ID_ITEMGRADEEFFECT_NONE, &CModelTool2Doc::OnItemgradeeffectNone)
ON_COMMAND(ID_ITEMGRADEEFFECT_1, &CModelTool2Doc::OnItemgradeeffect1)
ON_COMMAND(ID_ITEMGRADEEFFECT_2, &CModelTool2Doc::OnItemgradeeffect2)
ON_COMMAND(ID_ITEMGRADEEFFECT_3, &CModelTool2Doc::OnItemgradeeffect3)
ON_COMMAND(ID_ITEMGRADEEFFECT_4, &CModelTool2Doc::OnItemgradeeffect4)
ON_COMMAND(ID_OPTION_GLOWFILTER, &CModelTool2Doc::OnOptionGlowfilter)
ON_UPDATE_COMMAND_UI(ID_OPTION_GLOWFILTER, &CModelTool2Doc::OnUpdateOptionGlowfilter)
ON_COMMAND(ID_OPTION_BLURFILTER, &CModelTool2Doc::OnOptionBlurfilter)
ON_UPDATE_COMMAND_UI(ID_OPTION_BLURFILTER, &CModelTool2Doc::OnUpdateOptionBlurfilter)
ON_COMMAND(ID_OPTION_FRAMEFIX, &CModelTool2Doc::OnOptionFramefix)
ON_UPDATE_COMMAND_UI(ID_OPTION_FRAMEFIX, &CModelTool2Doc::OnUpdateOptionFramefix)
ON_COMMAND(ID_FILE_CONVERTBINARY, &CModelTool2Doc::OnFileConvertbinary)
ON_COMMAND(ID_FILE_CONVERTENCRYPTBINARY, &CModelTool2Doc::OnFileConvertencryptbinary)
ON_COMMAND(ID_CAMERA_FREECAMERA, &CModelTool2Doc::OnCameraFreecamera)
ON_UPDATE_COMMAND_UI(ID_CAMERA_FREECAMERA, &CModelTool2Doc::OnUpdateCameraFreecamera)
ON_COMMAND(ID_OPTION_CAMERADISTANCEALPHA, &CModelTool2Doc::OnOptionCameradistancealpha)
ON_UPDATE_COMMAND_UI(ID_OPTION_CAMERADISTANCEALPHA, &CModelTool2Doc::OnUpdateOptionCameradistancealpha)
ON_COMMAND(ID_FILE_UPDATEPROPERTYDATA, &CModelTool2Doc::OnFileUpdatepropertydata)
ON_COMMAND(ID_OPTION_CAMERASHAKEDISABLE, &CModelTool2Doc::OnOptionCamerashakedisable)
ON_UPDATE_COMMAND_UI(ID_OPTION_CAMERASHAKEDISABLE, &CModelTool2Doc::OnUpdateOptionCamerashakedisable)
END_MESSAGE_MAP()


// CModelTool2Doc construction/destruction

CModelTool2Doc::CModelTool2Doc()
{
	// TODO: add one-time construction code here

}

CModelTool2Doc::~CModelTool2Doc()
{
}

BOOL CModelTool2Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CModelTool2Doc serialization

void CModelTool2Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CModelTool2Doc diagnostics

#ifdef _DEBUG
void CModelTool2Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CModelTool2Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CModelTool2Doc commands

void CModelTool2Doc::OnViewViewbbox()
{
    BOOL bViewBBox = CModelToolApplication::GetInstance()->GetBBoxView();
    CModelToolApplication::GetInstance()->SetBBoxView(!bViewBBox);
}

void CModelTool2Doc::OnUpdateViewViewbbox(CCmdUI *pCmdUI)
{
    BOOL bViewBBox = CModelToolApplication::GetInstance()->GetBBoxView();
    pCmdUI->SetCheck(bViewBBox);
}

void CModelTool2Doc::OnViewViewworld()
{
    BOOL bView = CModelToolApplication::GetInstance()->GetWorldView();
    CModelToolApplication::GetInstance()->SetWorldView(!bView);
}

void CModelTool2Doc::OnUpdateViewViewworld(CCmdUI *pCmdUI)
{
    BOOL bView = CModelToolApplication::GetInstance()->GetWorldView();
    pCmdUI->SetCheck(bView);
}

void CModelTool2Doc::OnViewViewwireframe()
{
    BOOL bView = CModelToolApplication::GetInstance()->GetWireView();
    CModelToolApplication::GetInstance()->SetWireView(!bView);
}

void CModelTool2Doc::OnUpdateViewViewwireframe(CCmdUI *pCmdUI)
{
    BOOL bView = CModelToolApplication::GetInstance()->GetWireView();
    pCmdUI->SetCheck(bView);
}

void CModelTool2Doc::OnViewViewheirarchy()
{
    BOOL bView = CModelToolApplication::GetInstance()->GetHierarchyView();
    CModelToolApplication::GetInstance()->SetHierarchyView(!bView);
}

void CModelTool2Doc::OnUpdateViewViewheirarchy(CCmdUI *pCmdUI)
{
    BOOL bView = CModelToolApplication::GetInstance()->GetHierarchyView();
    pCmdUI->SetCheck(bView);
}

void CModelTool2Doc::OnViewViewtoonedge()
{
    BOOL bView = CModelToolApplication::GetInstance()->GetToonView();
    CModelToolApplication::GetInstance()->SetToonView(!bView);
}

//void CModelTool2Doc::OnUpdateViewViewtoonedge(CCmdUI *pCmdUI)
//{
//    BOOL bView = CModelToolApplication::GetInstance()->GetToonView();
//    pCmdUI->SetCheck(bView);
//}

void CModelTool2Doc::SetTitle(LPCTSTR lpszTitle)
{

    CDocument::SetTitle(MT_WINDOW_NAME);
}

void CModelTool2Doc::OnCameraLeftview()
{
	if (CModelToolApplication::GetInstance()->GetCamera() == NULL)
		return;

    CNtlGameCamera* pCamera = (CNtlGameCamera*)CModelToolApplication::GetInstance()->GetCamera();
    pCamera->SetLeftView();
}

void CModelTool2Doc::OnCameraFrontview()
{
	if (CModelToolApplication::GetInstance()->GetCamera() == NULL)
		return;

    CNtlGameCamera* pCamera = (CNtlGameCamera*)CModelToolApplication::GetInstance()->GetCamera();
    pCamera->SetFrontView();
}

void CModelTool2Doc::OnCameraTopview()
{
	if (CModelToolApplication::GetInstance()->GetCamera() == NULL)
		return;

    CNtlGameCamera* pCamera = (CNtlGameCamera*)CModelToolApplication::GetInstance()->GetCamera();
    pCamera->SetTopView();
}

void CModelTool2Doc::OnCameraBottomview()
{
	if (CModelToolApplication::GetInstance()->GetCamera() == NULL)
		return;

    CNtlGameCamera* pCamera = (CNtlGameCamera*)CModelToolApplication::GetInstance()->GetCamera();
    pCamera->SetBottomView();
}

void CModelTool2Doc::OnCameraRearview()
{
	if (CModelToolApplication::GetInstance()->GetCamera() == NULL)
		return;

    CNtlGameCamera* pCamera = (CNtlGameCamera*)CModelToolApplication::GetInstance()->GetCamera();
    pCamera->SetRearView();
}

void CModelTool2Doc::OnCameraRightview()
{
	if (CModelToolApplication::GetInstance()->GetCamera() == NULL)
		return;

    CNtlGameCamera* pCamera = (CNtlGameCamera*)CModelToolApplication::GetInstance()->GetCamera();
    pCamera->SetRightView();
}

void CModelTool2Doc::OnToolbarCamraicon()
{
    // TODO: Add your command handler code here
}

void CModelTool2Doc::OnToolbarThicknessicon()
{
    // TODO: Add your command handler code here
}

void CModelTool2Doc::OnOptionHiteffect()
{
    BOOL bView = CModelToolApplication::GetInstance()->GetHitEffect();
    GetSafeInstance(CModelToolApplication)->SetHitEffect(!bView);
}

void CModelTool2Doc::OnUpdateOptionHiteffect(CCmdUI *pCmdUI)
{
    BOOL bView = CModelToolApplication::GetInstance()->GetHitEffect();
    pCmdUI->SetCheck(bView);
}

void CModelTool2Doc::OnOptionCollisionmesh()
{
    BOOL bView = CModelToolApplication::GetInstance()->GetObjectCollisionMesh();
    GetSafeInstance(CModelToolApplication)->SetObjectCollisionMesh(!bView);
}

void CModelTool2Doc::OnUpdateOptionCollisionmesh(CCmdUI *pCmdUI)
{
    BOOL bView = CModelToolApplication::GetInstance()->GetObjectCollisionMesh();
    pCmdUI->SetCheck(bView);
}

void CModelTool2Doc::OnItemgradeeffectNone()
{
    CModelToolApplication::GetInstance()->SetItemGrade(ITEM_GRADE_NONE);    
}

void CModelTool2Doc::OnItemgradeeffect1()
{
    CModelToolApplication::GetInstance()->SetItemGrade(ITEM_GRADE_1);    
}

void CModelTool2Doc::OnItemgradeeffect2()
{
    CModelToolApplication::GetInstance()->SetItemGrade(ITEM_GRADE_2);
}

void CModelTool2Doc::OnItemgradeeffect3()
{
    CModelToolApplication::GetInstance()->SetItemGrade(ITEM_GRADE_3);
}

void CModelTool2Doc::OnItemgradeeffect4()
{
    CModelToolApplication::GetInstance()->SetItemGrade(ITEM_GRADE_4);    
}

void CModelTool2Doc::OnOptionGlowfilter()
{
    RwBool bEnable = CModelToolApplication::GetInstance()->GetEnableGlowFilter();
    CModelToolApplication::GetInstance()->SetEnableGlowFilter(!bEnable);
}

void CModelTool2Doc::OnUpdateOptionGlowfilter(CCmdUI *pCmdUI)
{
    RwBool bEnable = CModelToolApplication::GetInstance()->GetEnableGlowFilter();    
    pCmdUI->SetCheck(bEnable);
}

void CModelTool2Doc::OnOptionBlurfilter()
{
    RwBool bEnable = CModelToolApplication::GetInstance()->GetEnableBlurFilter();
    CModelToolApplication::GetInstance()->SetEnableBlurFilter(!bEnable);
}

void CModelTool2Doc::OnUpdateOptionBlurfilter(CCmdUI *pCmdUI)
{
    RwBool bEnable = CModelToolApplication::GetInstance()->GetEnableBlurFilter();
    pCmdUI->SetCheck(bEnable);
}

void CModelTool2Doc::OnOptionFramefix()
{
    RwBool bEnable = CModelToolApplication::GetInstance()->GetEnableFrameFix();
    CModelToolApplication::GetInstance()->SetEnableFrameFix(!bEnable);
}

void CModelTool2Doc::OnUpdateOptionFramefix(CCmdUI *pCmdUI)
{
    RwBool bEnable = CModelToolApplication::GetInstance()->GetEnableFrameFix();
    pCmdUI->SetCheck(bEnable);
}

void CModelTool2Doc::OnFileConvertbinary()
{
    m_dlgConvert.m_eType = CConvertingDlg::CONVERT_BIN;
	m_dlgConvert.DoModal();	
}

void CModelTool2Doc::OnFileConvertencryptbinary()
{
    m_dlgConvert.m_eType = CConvertingDlg::CONVERT_ENC_BIN;
	m_dlgConvert.DoModal();
}

void CModelTool2Doc::OnCameraFreecamera()
{
	if (CModelToolApplication::GetInstance()->GetCamera() == NULL)
		return;

    RwBool bEnable = CModelToolApplication::GetInstance()->GetCamera()->GetEnableFreeCamera();
    CModelToolApplication::GetInstance()->GetCamera()->SetEnableFreeCamera(!bEnable);
}

void CModelTool2Doc::OnUpdateCameraFreecamera(CCmdUI *pCmdUI)
{
	if (CModelToolApplication::GetInstance()->GetCamera() == NULL)
		return;

    RwBool bEnable = CModelToolApplication::GetInstance()->GetCamera()->GetEnableFreeCamera();
    pCmdUI->SetCheck(bEnable);
}

void CModelTool2Doc::OnOptionCameradistancealpha()
{
    RwBool bEnable =  CModelToolApplication::GetInstance()->GetEnableCameraDistAlpha();
    CModelToolApplication::GetInstance()->SetEnableCameraDistAlpha(!bEnable);
}

void CModelTool2Doc::OnUpdateOptionCameradistancealpha(CCmdUI *pCmdUI)
{
    RwBool bEnable = CModelToolApplication::GetInstance()->GetEnableCameraDistAlpha();
    pCmdUI->SetCheck(bEnable);
}

void CModelTool2Doc::OnFileUpdatepropertydata()
{
    m_dlgConvert.m_eType = CConvertingDlg::CONVERT_UPDATE_PROP;
    m_dlgConvert.DoModal();
}

void CModelTool2Doc::OnOptionCamerashakedisable()
{
    RwBool bEnable = CModelToolApplication::GetInstance()->GetCameraShakeDisable();
    CModelToolApplication::GetInstance()->SetCameraShakeDisable(!bEnable);
}

void CModelTool2Doc::OnUpdateOptionCamerashakedisable(CCmdUI *pCmdUI)
{
    RwBool bEnable = CModelToolApplication::GetInstance()->GetCameraShakeDisable();
    pCmdUI->SetCheck(bEnable);
}
