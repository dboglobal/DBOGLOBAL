// DTEditorView.cpp : implementation of the CDTEditorView class
//

#include "stdafx.h"
#include "DTEditor.h"
#include "MainFrm.h"

#include "DTEditorDoc.h"
#include "DTEditorView.h"

// shared
#include "NtlMovement.h"

// core
#include "NtlMath.h"

// simulation
#include "precomp_ntlsimulation.h"
#include "NtlDTProp.h"
#include "NtlCameraManager.h"
#include "NtlSLEventFunc.h"
#include "NtlSLApi.h"
#include "NtlSLPacketGenerator.h"
#include "NtlSobProxy.h"
#include "NtlSLLogic.h"

// DTEdit.h
#include "DTEditorGlobal.h"
#include "DirectApp.h"
#include "PointAttrDlg.h"
#include "DirectionPane.h"
#include "DirectionPropRefreeView.h"
#include "DirectionPropSound.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDTEditorView

IMPLEMENT_DYNCREATE(CDTEditorView, CView)

BEGIN_MESSAGE_MAP(CDTEditorView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()

// CDTEditorView construction/destruction

CDTEditorView::CDTEditorView()
{
	// TODO: add construction code here

	m_bLButtonMoveFlag			= FALSE;
	m_bRButtonMoveFlag			= FALSE;
	m_bChangeHeadingFlag		= FALSE;

	m_fMoveMarkTime				= 0.0f;
	m_pMoveMark					= NULL;
	
	m_eRotFlag					= CAMERA_ROTATE_FLAG_NONE;

	m_iMouseX					= 0;
	m_iMouseY					= 0;
}

CDTEditorView::~CDTEditorView()
{
}

void CDTEditorView::Picking(CPoint point)
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

	FLOAT fFocusDistance;
	int nSelIdx = GetFocusControlPointIndex(point, fFocusDistance);
	
	// picking 검출.
	SWorldPickInfo sPickInfo;
	sPickInfo.pPLEntity = NULL;
	sPickInfo.bTerrainExclusion		= FALSE;
	sPickInfo.bCharacterExclusion	= FALSE;
	RwBool bPick = GetSceneManager()->PickWorld(point.x, point.y, sPickInfo, 300.0f, 100.0f);

	if(bPick)
	{
		ENtlPLEntityType ePLType = sPickInfo.pPLEntity->GetClassType();
		SERIAL_HANDLE hPickHandle = sPickInfo.pPLEntity->GetSerialID();

		if(hPickHandle == INVALID_SERIAL_ID)
		{
			if(GetSelectTarget() != INVALID_SERIAL_ID)
			{
				CNtlSLEventGenerator::SobTargetSelectRelease(GetSelectTarget());
				SetSelectTarget(INVALID_SERIAL_ID);

				API_GetSLPacketGenerator()->SendCharTargetSelect(GetSelectTarget());
			}

			if(nSelIdx >= 0)
			{
				SetActiveSelectControlPoint(nSelIdx, nSelIdx+1);
				CControlPane *pControlPane = pMainFrame->GetControlPane();
				pControlPane->PointClick(nSelIdx, nSelIdx+1);
			}
		}
		else
		{
			if(GetSelectTarget() != INVALID_SERIAL_ID)
			{
				CNtlSLEventGenerator::SobTargetSelectRelease(GetSelectTarget());
				SetSelectTarget(INVALID_SERIAL_ID);

				API_GetSLPacketGenerator()->SendCharTargetSelect(GetSelectTarget());
			}

			if(nSelIdx < 0 || fFocusDistance > sPickInfo.fMinDist)
			{
				SetSelectTarget(hPickHandle);

				CNtlSLEventGenerator::SobTargetSelect(GetSelectTarget());

				CActorPane *pActorPane = pMainFrame->GetActorPane();
				pActorPane->SetActiveActor(GetSelectTarget());

				API_GetSLPacketGenerator()->SendCharTargetSelect(GetSelectTarget());
			}
			else
			{
				SetActiveSelectControlPoint(nSelIdx, nSelIdx+1);
				CControlPane *pControlPane = pMainFrame->GetControlPane();
				pControlPane->PointClick(nSelIdx, nSelIdx+1);
			}
		}
	}
	else
	{
		if(GetSelectTarget() != INVALID_SERIAL_ID)
			CNtlSLEventGenerator::SobTargetSelectRelease(GetSelectTarget());

		SetSelectTarget(INVALID_SERIAL_ID);
				
		CActorPane *pActorPane = pMainFrame->GetActorPane();
		pActorPane->SetActiveActor(GetSelectTarget());

		API_GetSLPacketGenerator()->SendCharTargetSelect(GetSelectTarget());

		// 선택한 control point
		if(nSelIdx >= 0)
		{
			SetActiveSelectControlPoint(nSelIdx, nSelIdx+1);
			CControlPane *pControlPane = pMainFrame->GetControlPane();
			pControlPane->PointClick(nSelIdx, nSelIdx+1);
		}
	}
}


INT CDTEditorView::GetFocusControlPointIndex(CPoint point, FLOAT& fDistance)
{
	CNtlDTSpline *pDTSpline = reinterpret_cast<CNtlDTSpline*>( GetActiveDTProp() );
	if(pDTSpline == NULL)
		return -1;

	RwV3d vSplineEditPos = GetSplineEditPosition();

	const SSplineProp *pSplineProp = pDTSpline->GetSplineProp();
	
	for(INT i = 0; i < pSplineProp->iCtrlNum; i++)
	{
		RwSphere Sphere;
		Sphere.center = pDTSpline->GetSplineControlPointWorldPosition(i);
		Sphere.radius = (RwReal)(GetSplinePointBoxSize()*1.5);

		RwV3dAdd(&Sphere.center, &Sphere.center, &vSplineEditPos);

		RwBool bResult = CNtlMath::LineSphereCollisionTestPixelRay(CNtlPLGlobal::m_RwCamera, &Sphere, point.x, point.y, 300.0f, fDistance);
		if(bResult)
			return i;
	}	

	return -1;
}

BOOL CDTEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CDTEditorView drawing

void CDTEditorView::OnDraw(CDC* /*pDC*/)
{
	CDTEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CDTEditorView printing

BOOL CDTEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDTEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDTEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CDTEditorView diagnostics

#ifdef _DEBUG
void CDTEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CDTEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDTEditorDoc* CDTEditorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDTEditorDoc)));
	return (CDTEditorDoc*)m_pDocument;
}
#endif //_DEBUG




// CDTEditorView message handlers

void CDTEditorView::OnMouseMove(UINT nFlags, CPoint point)
{
	ViewerModeMouseMove(nFlags, point);

	if(GetEditModeType() == EDIT_MODE_FREE_CAMERA)
		SplineModeMouseMove(nFlags, point);

	m_ptPrev = point;
	

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnMouseMove(nFlags, point);
}

BOOL CDTEditorView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if( !(nFlags & MK_CONTROL) && !(nFlags & MK_SHIFT) )
	{
		//120.f (Wheel
		RwReal fDelta = -zDelta * NTL_ZOOM_INOUT_SENSITIVITY;
		GetNtlGameCameraManager()->ZoomInOut(fDelta);
	}
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CDTEditorView::OnLButtonDblClk(UINT nFlags, CPoint point)
{

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnLButtonDblClk(nFlags, point);
}


void CDTEditorView::OnLButtonUp(UINT nFlags, CPoint point)
{
	ViewerModeMouseLBtnUp(nFlags, point);
	
	if(GetEditModeType() == EDIT_MODE_THIRD_CAMERA && nFlags & MK_CONTROL)
		SplineModeMouseLBtnUp(point);

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnLButtonUp(nFlags, point);
}

void CDTEditorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	ViewerModeMouseLBtnDown(point);

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnLButtonDown(nFlags, point);
}

void CDTEditorView::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnMButtonUp(nFlags, point);
}

void CDTEditorView::OnRButtonDown(UINT nFlags, CPoint point)
{
	ViewerModeMouseRBtnDown(point);

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnRButtonDown(nFlags, point);
}


void CDTEditorView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ViewerModeMouseRBtnUp(point);

	if(GetEditModeType() == EDIT_MODE_THIRD_CAMERA)
	{
		if(nFlags & MK_CONTROL)
			SplineModeMouseRBtnUp(point);
	}
			
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnRButtonUp(nFlags, point);
}

void CDTEditorView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	CDirectApplication *pApp = (CDirectApplication*)CNtlApplication::GetInstance();
	if(pApp)
	{
		pApp->OnResize(cx, cy, TRUE);
	}
}


void CDTEditorView::Update(RwReal fElapsed)
{
	CalcCameraRotateToCharBack();
	UpdateCameraRotateToCharBack(fElapsed);
	GetNtlGameCameraManager()->Update(fElapsed);

	// for test
	CMainFrame* pMainFrame = ((CMainFrame*)AfxGetMainWnd());
	if( pMainFrame )
	{
		CDirectionPane* pDirectoinPane = pMainFrame->GetDirectionPane();
		if( pDirectoinPane && pDirectoinPane->GetSafeHwnd() )
		{
			pDirectoinPane->Update(fElapsed);
		}
	}

	// update move mark
//	UpdateMoveMark(fElapsed);
}

void CDTEditorView::CalcCameraRotateSensitivity(int iPosX, int iPosY, int iMouseDeltaX, int iMouseDeltaY)
{
	RwReal fDeltaYaw = ((RwReal)iMouseDeltaX * NTL_MOUSE_SENSITIVITY_SCALE);
	RwReal fDeltaPitch = ((RwReal)iMouseDeltaY * NTL_MOUSE_SENSITIVITY_SCALE);
	
	GetNtlGameCameraManager()->AddYaw(fDeltaYaw);
	GetNtlGameCameraManager()->AddPitch(-fDeltaPitch);
}



void CDTEditorView::ViewerModeMouseLBtnDown(CPoint point)
{
	m_iOldLButtonMouseX = point.x;
	m_iOldLButtonMouseY = point.y;

	CalcCameraRotateToCharBack();

	m_dwLBtnDownTime = timeGetTime();
}

void CDTEditorView::ViewerModeMouseLBtnUp(UINT nFlags, CPoint point)
{
	m_bLButtonMoveFlag = FALSE;

	// mouse click 
	if(GetEditModeType() == EDIT_MODE_THIRD_CAMERA) 
	{
		if(timeGetTime() - m_dwLBtnDownTime <= 300)
		{
			// picking 검출.
			SWorldPickInfo sPickInfo;
			sPickInfo.bTerrainExclusion		= FALSE;
			sPickInfo.bCharacterExclusion	= FALSE;
			RwBool bPick = GetSceneManager()->PickWorld(point.x, point.y, sPickInfo, 300.0f, 300.0f);

			if(bPick)
			{
				ENtlPLEntityType ePLType = sPickInfo.pPLEntity->GetClassType();

				if(ePLType == PLENTITY_WORLD || 
					(ePLType == PLENTITY_OBJECT && sPickInfo.pPLEntity->GetSerialID() == INVALID_SERIAL_ID) )
				{
					CDirectApplication *pApp = (CDirectApplication*)CNtlApplication::GetInstance();
					RwUInt32 uiMoveFlags = pApp->GetMoveFlags();
					if(uiMoveFlags == 0)
					{
						// 사운드 출력.
						CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar(); 
						RwV3d vPos = pSobAvatar->GetPosition(); 

						CNtlSLEventGenerator::TerrainClick(sPickInfo.vPickPos);

						if(pSobAvatar->CanMove())
						{
							//Mark를 찍을 때마다 생성을 한다. 퍼포먼스의 하락이 있을것으로 예상
							CreateMoveMark(&sPickInfo.vPickPos);
						}
					}

					CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

					if( pMainFrame->GetDirectionRefreePane() )
						pMainFrame->GetDirectionRefreePane()->SetPickInfo(&sPickInfo);

					pMainFrame->GetDirectionSoundPane()->SetPickInfo(&sPickInfo);
				}
			}
		}
	}
	else
	{
		Picking(point);
	}
}

void CDTEditorView::SplineModeMouseLBtnUp(CPoint point)
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

	CPoint ptDelta;
	ptDelta.x = m_iOldLButtonMouseX - point.x;
	ptDelta.y = m_iOldLButtonMouseY - point.y;

	if(timeGetTime() - m_dwLBtnDownTime < 300 && abs(ptDelta.x) < 10 && abs(ptDelta.x) < 10)
	{
		Picking(point);
	}
	else
	{
		if(GetSelectTarget() != INVALID_SERIAL_ID)
			CNtlSLEventGenerator::SobTargetSelectRelease(GetSelectTarget());

		SetSelectTarget(INVALID_SERIAL_ID);

		API_GetSLPacketGenerator()->SendCharTargetSelect(GetSelectTarget());
		// pMainFrame->ActiveActorPane(FALSE);
	}
}

void CDTEditorView::ViewerModeMouseRBtnDown(CPoint point)
{
	m_iOldRButtonMouseX = point.x;
	m_iOldRButtonMouseY = point.y;

	CalcCameraRotateToCharBack();

	m_dwRBtnDownTime = timeGetTime();
}


void CDTEditorView::ViewerModeMouseRBtnUp(CPoint point)
{
	RwInt32 iDeltaMouseX = abs(point.x - m_iOldRButtonMouseX);
	RwInt32 iDeltaMouseY = abs(point.y - m_iOldRButtonMouseY);

	m_bRButtonMoveFlag = FALSE;
	m_bChangeHeadingFlag = FALSE;
}

void CDTEditorView::SplineModeMouseRBtnUp(CPoint point)
{
	CPoint ptScreen = point;
	ClientToScreen(&ptScreen);

	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_POPUP));

	CMenu* pPopup = menu.GetSubMenu(1);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;

	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptScreen.x, ptScreen.y,	pWndPopupOwner);
}


void CDTEditorView::ViewerModeMouseMove(UINT nFlags, CPoint point)
{
	m_iMouseX = point.x;
	m_iMouseY = point.y;

	if( nFlags & MK_LBUTTON)
		ViewerModeMouseMoveLBtn(point);
	else if( nFlags & MK_RBUTTON)
		ViewerModeMouseMoveRBtn(point);
	else
		ViewerModeMouseMoveNotBtn(point);
}

void CDTEditorView::SplineModeMouseMove(UINT nFlags, CPoint point)
{
	CNtlDTProp *pDTProp = GetActiveDTProp(); 
	if(pDTProp)
	{
		CNtlDTSpline *pDTSpline = (CNtlDTSpline*)pDTProp;

		RwV3d vDelta;
		CNtlMath::MathRwV3dAssign(&vDelta, 0.0f, 0.0f, 0.0f);

		if(nFlags & MK_LBUTTON)
		{
			if(nFlags & MK_CONTROL)
			{
				RwV3d vPos, vDir;

				RwV3dScale(&vDelta, &vDelta, 0.1f);
				const RwV3d *pDir = GetNtlGameCameraManager()->GetCameraDir();
				CNtlMath::MathRwV3dAssign(&vPos, 0.0f, 0.0f, 0.0f);
				CNtlMath::MathRwV3dAssign(&vDir, pDir->x, 0.0f, pDir->z);
				RwV3dNormalize(&vDir, &vDir);
				RwMatrix mat;
				CNtlMath::MathRwMatrixAssign(&mat, &vPos, &vDir);

				vDelta.x = (point.x-m_ptPrev.x)*mat.right.x+(m_ptPrev.y-point.y)*mat.right.z ;
				vDelta.z = (point.x-m_ptPrev.x)*mat.at.x+(m_ptPrev.y-point.y)*mat.at.z;
			}

			if(nFlags & MK_SHIFT)
			{
				vDelta.y = (RwReal)(m_ptPrev.y-point.y);
			}

			if(nFlags & MK_CONTROL || nFlags & MK_SHIFT)
			{
				vDelta.x *= -0.01f;
				vDelta.y *= 0.01f;
				vDelta.z *= 0.01f;

				int nSelSx, nSelEx;
				GetActiveSelectControlPoint(nSelSx, nSelEx);

				pDTSpline->AddControlPointPosition(nSelSx, nSelEx, vDelta);
			}
		}

		// focus control point
		FLOAT fDistance;
		int nFocusIdx = GetFocusControlPointIndex(point, fDistance);
		SetFocusControlPoint(nFocusIdx);
	}

	m_ptPrev = point;
}


void CDTEditorView::ViewerModeMouseMoveLBtn(CPoint point)
{
	RwInt32 iDeltaX = (m_iOldLButtonMouseX - point.x);
	RwInt32 iDeltaY = (m_iOldLButtonMouseY - point.y);

	if(GetEditModeType() != EDIT_MODE_FREE_CAMERA)
		CalcCameraRotateSensitivity(point.x, point.y, iDeltaX, iDeltaY);

	m_iOldLButtonMouseX = point.x;
	m_iOldLButtonMouseY = point.y;

	m_bLButtonMoveFlag = TRUE;
}

void CDTEditorView::ViewerModeMouseMoveRBtn(CPoint point)
{
	m_dwRBtnDownTime = timeGetTime();

	RwInt32 iDeltaX = ((RwInt32)m_iOldRButtonMouseX - point.x);
	RwInt32 iDeltaY = ((RwInt32)m_iOldRButtonMouseY - point.y);

	CDirectApplication *pApp = (CDirectApplication*)CNtlApplication::GetInstance();
	RwUInt32 uiMoveFlags = pApp->GetMoveFlags();

	if( uiMoveFlags == NTL_MOVE_F || uiMoveFlags == NTL_MOVE_B)
	{
		if(!m_bChangeHeadingFlag)
		{
			m_bChangeHeadingFlag = TRUE;
		}

		RwBool bSuccess = TRUE;
		CNtlSLEventGenerator::InputChagneHeading(FALSE, (float)iDeltaX *NTL_MOUSE_SENSITIVITY_SCALE, bSuccess);
	}
	else
	{
		if(uiMoveFlags == NTL_MOVE_NONE)
		{
			if(GetEditModeType() != EDIT_MODE_FREE_CAMERA)
			{
				RwBool bSuccess = TRUE;
				CNtlSLEventGenerator::InputChagneHeading(TRUE, (float)iDeltaX *NTL_MOUSE_SENSITIVITY_SCALE, bSuccess);

				if(bSuccess && !m_bChangeHeadingFlag)
				{
					// 카메라와 틀어진 target 방향을 맞춘다.
					RwReal fYaw = GetNtlGameCameraManager()->GetYaw();
					CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
					pSobAvatar->SetAngleY(fYaw);

					// flag setting
					m_bChangeHeadingFlag = TRUE;
				}
			}
		}
		else
		{
			if(m_bChangeHeadingFlag)
			{
			}

			m_bChangeHeadingFlag = FALSE;
		}
	}

	CalcCameraRotateSensitivity(point.x, point.y, iDeltaX, iDeltaY);

	m_bRButtonMoveFlag = TRUE;

	m_iOldRButtonMouseX = point.x;
	m_iOldRButtonMouseY = point.y;
}

void CDTEditorView::ViewerModeMouseMoveNotBtn(CPoint point)
{
	
}


void CDTEditorView::CalcCameraRotateToCharBack(void)
{
	CDirectApplication *pApp = (CDirectApplication*)CNtlApplication::GetInstance();
	RwUInt32 uiMoveFlags = pApp->GetMoveFlags();

	if(uiMoveFlags == NTL_MOVE_NONE || m_bLButtonMoveFlag || m_bRButtonMoveFlag)
	{
		m_eRotFlag = CAMERA_ROTATE_FLAG_NONE;
	}
	else
	{
		if(m_eRotFlag == CAMERA_ROTATE_FLAG_NONE)
		{
			m_eRotFlag = CAMERA_ROTATE_FLAG_ADJUST;
		}
		else if(m_eRotFlag == CAMERA_ROTATE_FLAG_ACCORD)
		{
			CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
			RwV3d vDir = pSobAvatar->GetDirection();
			RwReal fAngleY = CNtlMath::LineToAngleY(&vDir);

			GetNtlGameCameraManager()->SetYaw(fAngleY);
		}
	}
}


void CDTEditorView::UpdateCameraRotateToCharBack(RwReal fElapsed)
{
	if(m_eRotFlag != CAMERA_ROTATE_FLAG_ADJUST)
		return;

	RwReal fDeltaAngleY = 180.0f * fElapsed;

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	RwV3d vDir = pSobAvatar->GetDirection();
	RwReal fAngleY = CNtlMath::LineToAngleY(&vDir);
	RwReal fCovAngleY = GetNtlGameCameraManager()->GetYaw();

	if(fAngleY < 0.0f)
		fAngleY += 360.0f;
	if(fCovAngleY >= 360.0f)
		fCovAngleY -= 360.0f;

	//  회전은 시계 반대 방향 회전이다.
	//  카메라 angle 에서 character angle 뺀다.
	RwReal fSubAngleY = fCovAngleY - fAngleY;

	if(fSubAngleY > 0.0f)
		if(fSubAngleY > 180.0f) // 각도를 증가시킨다.
			fSubAngleY = 360.0f - fSubAngleY;
		else // 각도를 감소시킨다.
			fSubAngleY = -fSubAngleY; //(시계 반대 방향이므로 빼줘야 한다.)
	else
		if(fSubAngleY > -180.0f) // 각도를 증가시킨다.
			fSubAngleY = fabs(fSubAngleY); 
		else // 각도를 감소시킨다.
			fSubAngleY = -(360.0f - fabs(fSubAngleY)); // 시계 반대 방향이므로 빼줘야 한다.

	if(fabs(fSubAngleY) < fDeltaAngleY)
		m_eRotFlag = CAMERA_ROTATE_FLAG_ACCORD;
	else
	{
		if(fSubAngleY > 0.0f)
			GetNtlGameCameraManager()->AddYaw(fDeltaAngleY);
		else
			GetNtlGameCameraManager()->AddYaw(-fDeltaAngleY);
	}
}


void CDTEditorView::CreateMoveMark(const RwV3d *pPos)
{
	return;

//	DeleteMoveMark();

	if(m_pMoveMark == NULL)
		m_pMoveMark = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, "GME_EFF_003");

	if(m_pMoveMark)
		m_pMoveMark->SetPosition(pPos);
}

void CDTEditorView::DeleteMoveMark(void)
{
	if(m_pMoveMark)
	{
		GetSceneManager()->DeleteEntity(m_pMoveMark);
		m_pMoveMark = NULL;
		m_fMoveMarkTime = 0.0f;
	}
}


void CDTEditorView::UpdateMoveMark(RwReal fElapsed)
{
	// move mark를 잠깐 띄웠다가 없앤다.
	if(m_pMoveMark)
	{
		m_fMoveMarkTime += fElapsed;
		if(m_fMoveMarkTime > 3.0f)
			DeleteMoveMark();
	}
}


void CDTEditorView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CInputHandler::GetInstance()->KeyDownHandler(nChar,  nRepCnt);

	if(nChar == VK_SPACE)
	{
		/*
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CControlPane *pControlPane = pMainFrame->GetControlPane();
		CPointAttrDlg *pAttrDlg = pControlPane->GetPointAttrDlg();

		pAttrDlg->OnBnClickedPointattrAddPointPos();
		*/

		SERIAL_HANDLE hTargetSerialId = GetSelectTarget();

		CNtlSobActor *pSobActor = Logic_FindNearEnemyFromAvatarActor();
		if(pSobActor)
		{
			if(pSobActor->GetSerialID() != hTargetSerialId)
			{
				if(hTargetSerialId != INVALID_SERIAL_ID)
					CNtlSLEventGenerator::SobTargetSelectRelease(hTargetSerialId);
			}

			hTargetSerialId = pSobActor->GetSerialID();
			SetSelectTarget(hTargetSerialId);
			CNtlSLEventGenerator::SobTargetSelect(hTargetSerialId);
		}
	}
	else if(nChar == 'c' || nChar == 'C')
	{
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CControlPane *pControlPane = pMainFrame->GetControlPane();
		CPointAttrDlg *pAttrDlg = pControlPane->GetPointAttrDlg();

		pAttrDlg->OnBnClickedPointattrPointPos();
	}
	
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CDTEditorView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CInputHandler::GetInstance()->KeyUpHandler(nChar,  nRepCnt);

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}
