// VenusView.cpp : CVenusView 클래스의 구현
//

#include "stdafx.h"
#include "Venus.h"

#include "VenusDoc.h"
#include "VenusView.h"
#include "VenusFrame.h"

#include ".\venusconfig.h"

#include ".\manipulator.h"
#include "VenusGrid.h"

#include ".\venuscamera.h"
#include ".\venusview.h"

#include ".\venuspropertycontainer.h"
#include ".\venusvisualmanager.h"

#include ".\stringmanager.h"

#include "VenusFramework.h"

#include "StatusBarDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVenusView

IMPLEMENT_DYNCREATE(CVenusView, CView)

BEGIN_MESSAGE_MAP(CVenusView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_WM_SIZE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(POPUP_INIT_CAMERA, OnInitCamera)
	ON_COMMAND(POPUP_FILLMODE_POINT, OnFillmodePoint)
	ON_UPDATE_COMMAND_UI(POPUP_FILLMODE_POINT, OnUpdateFillmodePoint)
	ON_COMMAND(POPUP_FILLMODE_WIREFRAME, OnFillmodeWireframe)
	ON_UPDATE_COMMAND_UI(POPUP_FILLMODE_WIREFRAME, OnUpdateFillmodeWireframe)
	ON_COMMAND(POPUP_FILLMODE_SOLID, OnFillmodeSolid)
	ON_UPDATE_COMMAND_UI(POPUP_FILLMODE_SOLID, OnUpdateFillmodeSolid)
    ON_COMMAND(ID_CAMERA_FRONTCAMERA, &CVenusView::OnCameraFrontcamera)
    ON_COMMAND(ID_CAMERA_LEFTCAMERA, &CVenusView::OnCameraLeftcamera)
    ON_COMMAND(ID_CAMERA_RIGHTCAMERA, &CVenusView::OnCameraRightcamera)
    ON_COMMAND(ID_CAMERAVIEWPOSITION_BACKCAMERA, &CVenusView::OnCameraviewpositionBackcamera)
    ON_COMMAND(ID_CAMERAVIEWPOSITION_UPCAMERA, &CVenusView::OnCameraviewpositionUpcamera)
    ON_COMMAND(ID_CAMERAVIEWPOSITION_DOWNCAMERA, &CVenusView::OnCameraviewpositionDowncamera)
    ON_COMMAND(ID_VIEW_LOWSPECEFFECT, &CVenusView::OnViewLowspeceffect)
    ON_UPDATE_COMMAND_UI(ID_VIEW_LOWSPECEFFECT, &CVenusView::OnUpdateViewLowspeceffect)
END_MESSAGE_MAP()

// CVenusView 생성/소멸

CVenusView* g_pMainView = NULL;

CVenusView::CVenusView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	g_pMainView = this;

}

CVenusView::~CVenusView()
{
	g_pMainView = NULL;
}

BOOL CVenusView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	// Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CVenusView 그리기

void CVenusView::OnDraw(CDC* /*pDC*/)
{
	CVenusDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CVenusView 인쇄

BOOL CVenusView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CVenusView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CVenusView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CVenusView 진단

#ifdef _DEBUG
void CVenusView::AssertValid() const
{
	CView::AssertValid();
}

void CVenusView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CVenusDoc* CVenusView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVenusDoc)));
	return (CVenusDoc*)m_pDocument;
}
#endif //_DEBUG


// CVenusView 메시지 처리기

void CVenusView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	RECT rt;
	GetClientRect(&rt);
	CVenusCamera::GetInstance().SetCameraSize((RwRect*)&rt);
}

void CVenusView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CVenusCamera::GetInstance().MouseMessages(WM_RBUTTONDOWN, point);
	SetCapture();

	CView::OnRButtonDown(nFlags, point);
}

void CVenusView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CVenusCamera::GetInstance().MouseMessages(WM_RBUTTONUP, point);
	ReleaseCapture();

	CView::OnRButtonUp(nFlags, point);
}

void CVenusView::OnMouseMove(UINT nFlags, CPoint point)
{
	CVenusCamera::GetInstance().MouseMessages(WM_MOUSEMOVE, point);

    // 이펙트를 선택하고, 화살표를 클릭했을대, (이펙트의 이동,회전,크기변환등)
	if (CManipulator::GetInstance().m_nArrowType != CManipulator::AXIS_TYPE_NONE)
	{
		if (CVenusConfig::GetInstance().m_nCurrentLayer == EDIT_LAYER_PATH)
		{
			RwV3d vResult;
			if (CManipulator::GetInstance().IntersectionMove(&vResult, point))
			{
				CVenusPropertyContainer::GetInstance().AddSelectControlPoint(&vResult);
			}
		}
		else
		{
			RwMatrix matResult;
			if (CManipulator::GetInstance().IntersectionMove(&matResult, point))
			{
				CVenusVisualManager::GetInstance().EditInstanceEffect(&matResult);
			}
		}
	}

	if (CVenusFramework::GetInstance().GetSelectMode())
	{
		CVenusFramework::GetInstance().SetSelectRectangle(m_PointButtonDown, point);
	}

	if (CVenusGrid::GetInstance().GetIntersection())
	{
		RwV3d vMove;
		if (CVenusGrid::GetInstance().IntersectionMove(&vMove, point, &CVenusCamera::GetInstance().GetTargetPosition()))
		{
			CVenusCamera::GetInstance().UpdateViewMatrix(&vMove);
		}
	}

	CView::OnMouseMove(nFlags, point);
}

void CVenusView::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CVenusCamera::GetInstance().MouseMessages(WM_MBUTTONDOWN, point);

	CVenusGrid::GetInstance().IntersectionStart(point, &CVenusCamera::GetInstance().GetTargetPosition());

	SetCapture();

	CView::OnMButtonDown(nFlags, point);
}

void CVenusView::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CVenusGrid::GetInstance().IntersectionEnd();

	CVenusCamera::GetInstance().MouseMessages(WM_MBUTTONUP, point);
	ReleaseCapture();

	CView::OnMButtonUp(nFlags, point);
}

BOOL CVenusView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CVenusCamera::GetInstance().MouseMessages(WM_MOUSEWHEEL, pt, zDelta, nFlags);

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CVenusView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_PointButtonDown = point;
	if (CVenusConfig::GetInstance().m_nCurrentLayer != EDIT_LAYER_PATH)
	{
		if (!CVenusVisualManager::GetInstance().m_svSelectInstanceEffect.empty())
		{
			CManipulator::GetInstance().IntersectionStart(point);
		}
	}
	else
	{
		if (!CVenusPropertyContainer::GetInstance().m_svSelectSplineControlPoint.empty())
		{
			CManipulator::GetInstance().IntersectionStart(point);
		}
	}

	if (CManipulator::GetInstance().m_nArrowType == CManipulator::AXIS_TYPE_NONE)
	{
		CVenusFramework::GetInstance().SetSelectRectangle(m_PointButtonDown, point);
		CVenusFramework::GetInstance().SetSelectMode(TRUE);
	}

	SetCapture();

	CView::OnLButtonDown(nFlags, point);
}

void CVenusView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CManipulator::GetInstance().IntersectionEnd();

	if (CVenusFramework::GetInstance().GetSelectMode())
	{
		if (CVenusConfig::GetInstance().m_nCurrentLayer != EDIT_LAYER_PATH)
		{
			if (m_PointButtonDown.x != point.x || m_PointButtonDown.y != point.y)
			{
				CVenusVisualManager::GetInstance().IntersectionEffect(m_PointButtonDown, point, (nFlags & MK_SHIFT), (nFlags & MK_CONTROL));
			}
            else
            {
                CVenusVisualManager::GetInstance().IntersectionEffect(point, (nFlags & MK_SHIFT), (nFlags & MK_CONTROL));
            }
		}
		else
		{
			CVenusPropertyContainer::GetInstance().IntersectionSplineControlPoint(point, (nFlags & MK_SHIFT), (nFlags & MK_CONTROL));
			if (m_PointButtonDown.x != point.x || m_PointButtonDown.y != point.y)
			{
				CVenusPropertyContainer::GetInstance().IntersectionSplineControlPoint(m_PointButtonDown, point, (nFlags & MK_SHIFT), (nFlags & MK_CONTROL));
			}
		}

		CVenusFramework::GetInstance().SetSelectMode(FALSE);
	}

	ReleaseCapture();
	CView::OnLButtonUp(nFlags, point);
}

void CVenusView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch(nChar)
	{
	case '1':
		{
			//// Scale 테스트용
			//CVenusVisualManager::svdef_InstanceEffect::iterator it;
			//for (it = CVenusVisualManager::GetInstance().m_svSelectInstanceEffect.begin(); it != CVenusVisualManager::GetInstance().m_svSelectInstanceEffect.end(); ++ it)
			//{
			//	(*it)->SetScale(2.0f);			
            //}

            //CVenusCamera::GetInstance().SetProjection(rwPARALLEL);
            CVenusCamera::GetInstance().SetStaticView(E_STATIC_VIEW_FRONT);
		}
		break;
	case '2':
		{
            //CVenusCamera::GetInstance().SetProjection(rwPERSPECTIVE);
            CVenusCamera::GetInstance().SetStaticView(E_STATIC_VIEW_BACK);
        }
		break;
	case VK_INSERT:
	{
		
	}
	break;
	case VK_DELETE:
		{
			if (!CVenusVisualManager::GetInstance().m_svSelectInstanceEffect.empty())
			{
				if (AfxMessageBox(CStringManager::GetInstance().GetString(STRING_DELETE), MB_OKCANCEL) == IDOK)
				{
					CVenusVisualManager::svdef_InstanceEffect::iterator it;
					for (it = CVenusVisualManager::GetInstance().m_svSelectInstanceEffect.begin(); it != CVenusVisualManager::GetInstance().m_svSelectInstanceEffect.end(); ++ it)
					{
						CVenusVisualManager::GetInstance().DeleteEntity(*it);
					}
					CVenusVisualManager::GetInstance().m_svSelectInstanceEffect.clear();
				}

				CEditDialog::GetInstance().Reset();
			}
		}
		break;
	case VK_SPACE:
		{
			CVenusConfig::GetInstance().m_bTimePause = !CVenusConfig::GetInstance().m_bTimePause;
			CStatusBarDialog::GetInstance().UpdateTimeButton();
		}
		break;
	case VK_HOME:
		{
			OnInitCamera();
		}
		break;
	case VK_SHIFT:
		{
		}
		break;
	case VK_CONTROL:
		{
		}
		break;
    case 'W':
        CVenusCamera::GetInstance().SetStaticView(E_STATIC_VIEW_FRONT);
        break;
    case 'A':
        CVenusCamera::GetInstance().SetStaticView(E_STATIC_VIEW_LEFT);
        break;
    case 'S':
        CVenusCamera::GetInstance().SetStaticView(E_STATIC_VIEW_BACK);        
        break;
    case 'D':
        CVenusCamera::GetInstance().SetStaticView(E_STATIC_VIEW_RIGHT);        
        break;
    case 'Q':
        CVenusCamera::GetInstance().SetStaticView(E_STATIC_VIEW_UP);
        break;
    case 'E':
        CVenusCamera::GetInstance().SetStaticView(E_STATIC_VIEW_DOWN);
        break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CVenusView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch(nChar)
	{
		case VK_SHIFT:
		{
		}
		break;
		case VK_CONTROL:
		{
		}
		break;
	}
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CVenusView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	if (point.x == -1 && point.y == -1)
	{
		//keystroke invocation
		CRect rect;
		GetClientRect(rect);
		ClientToScreen(rect);

		point = rect.TopLeft();
		point.Offset(5, 5);
	}

	CPoint screenPoint = point;
	ScreenToClient(&screenPoint);
	if (screenPoint.x < 50 && screenPoint.y < 50)
	{
		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_POPUP));

		CMenu* pPopup = menu.GetSubMenu(2);
		ASSERT(pPopup != NULL);
		CWnd* pWndPopupOwner = this;

		while (pWndPopupOwner->GetStyle() & WS_CHILD)
			pWndPopupOwner = pWndPopupOwner->GetParent();

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
			pWndPopupOwner);
	}
}

void CVenusView::OnInitCamera()
{
	CVenusCamera::GetInstance().SetViewDistance(10.0f);
    RwV3d vPosition = {0.0f, 0.0f, 0.0f};
	CVenusCamera::GetInstance().SetTargetPosition(vPosition);
	CVenusCamera::GetInstance().InitViewMatrix();
}

void CVenusView::OnFillmodePoint()
{
	CVenusConfig::GetInstance().m_D3DFillMode = D3DFILL_POINT;
}

void CVenusView::OnUpdateFillmodePoint(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(CVenusConfig::GetInstance().m_D3DFillMode == D3DFILL_POINT);
}

void CVenusView::OnFillmodeWireframe()
{
	CVenusConfig::GetInstance().m_D3DFillMode = D3DFILL_WIREFRAME;
}

void CVenusView::OnUpdateFillmodeWireframe(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(CVenusConfig::GetInstance().m_D3DFillMode == D3DFILL_WIREFRAME);
}

void CVenusView::OnFillmodeSolid()
{
	CVenusConfig::GetInstance().m_D3DFillMode = D3DFILL_SOLID;
}

void CVenusView::OnUpdateFillmodeSolid(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(CVenusConfig::GetInstance().m_D3DFillMode == D3DFILL_SOLID);
}

void CVenusView::OnCameraFrontcamera()
{
    CVenusCamera::GetInstance().SetStaticView(E_STATIC_VIEW_FRONT);
}

void CVenusView::OnCameraLeftcamera()
{
    CVenusCamera::GetInstance().SetStaticView(E_STATIC_VIEW_LEFT);
}

void CVenusView::OnCameraRightcamera()
{
    CVenusCamera::GetInstance().SetStaticView(E_STATIC_VIEW_RIGHT);
}

void CVenusView::OnCameraviewpositionBackcamera()
{
    CVenusCamera::GetInstance().SetStaticView(E_STATIC_VIEW_BACK);
}

void CVenusView::OnCameraviewpositionUpcamera()
{
    CVenusCamera::GetInstance().SetStaticView(E_STATIC_VIEW_UP);
}

void CVenusView::OnCameraviewpositionDowncamera()
{
    CVenusCamera::GetInstance().SetStaticView(E_STATIC_VIEW_DOWN);
}

void CVenusView::OnViewLowspeceffect()
{
    RwBool bEnable = CNtlInstanceEffect::GetLowSpecEnable();
    CNtlInstanceEffect::SetLowSpecEnable(!bEnable);
}

void CVenusView::OnUpdateViewLowspeceffect(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(CNtlInstanceEffect::GetLowSpecEnable());   
}
