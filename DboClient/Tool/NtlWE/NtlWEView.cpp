// NtlWEView.cpp : CNtlWEView 클래스의 구현
//

#include "stdafx.h"
#include "NtlWE.h"
#include "NtlWEDoc.h"
#include "NtlWEView.h"
#include "NtlWEFrm.h"
#include "ntlworldcommon.h"
#include "ntlplworldentity.h"
#include "PaletteTerrain.h"
#include "FieldUIManager.h"
#include "MergeUIManager.h"
#include "ntlworldinterface.h"
#include "NtlWorldBrushTile.h"
#include "NtlWorldFieldManager.h"
#include "NtlWorldBrush.h"
#include "NtlWorldMouse.h"
#include "SpawnNestDlg.h"
#include "SpawnManager.h"
#include "SpawnPopupDlg.h"
#include "SpawnGroupBasedPropDlg.h"
#include "SpawnManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNtlWEView

IMPLEMENT_DYNCREATE(CNtlWEView, CView)

BEGIN_MESSAGE_MAP(CNtlWEView, CView)
	// 표준 인쇄 명령입니다.
	//ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

// CNtlWEView 생성/소멸

CNtlWEView::CNtlWEView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CNtlWEView::~CNtlWEView()
{
}

BOOL CNtlWEView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	// Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CNtlWEView 그리기

void CNtlWEView::OnDraw(CDC* /*pDC*/)
{
	CNtlWEDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	//dGETAPP().OnIdle();
}


// CNtlWEView 인쇄

BOOL CNtlWEView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	//return DoPreparePrinting(pInfo);
	return TRUE;
}

void CNtlWEView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CNtlWEView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CNtlWEView 진단

#ifdef _DEBUG
void CNtlWEView::AssertValid() const
{
	CView::AssertValid();
}

void CNtlWEView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CNtlWEDoc* CNtlWEView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNtlWEDoc)));
	return (CNtlWEDoc*)m_pDocument;
}
#endif //_DEBUG


// CNtlWEView 메시지 처리기

void CNtlWEView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	RECT rc;
	GetClientRect(&rc);
	CNtlWEApplication::GetInstance()->Resize(rc.right-rc.left, rc.bottom - rc.top, TRUE, FALSE);
}

void CNtlWEView::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	dGET_MOUSE()->SetBehavior(MB_LBDN, point.x, point.y, true);

	// update terrain palette modaless in app. level
	if(dGET_CUR_PALETTE() == ePM_TERRAIN ||
	   dGET_CUR_PALETTE() == ePM_TEXTURE ||
	   dGET_CUR_PALETTE() == ePM_DIFFUSE)
	{
		if(dKEY(DIK_LCONTROL) && dGET_MOUSE()->GetLBOnce())
		{
			dGET_CUR_PALETTE() = ePM_TEXTURE;

			CPaletteTerrain *pPTDlg = (CPaletteTerrain*)(((CNtlWEApp*)(AfxGetApp()))->m_pFrm->m_pTerrainPalette);
			pPTDlg->UpdateFieldTextureInfo();
		}
	}

	dGETAPP().GetNtlPLWorldEntity()->OnBtnDN((RwReal)point.x, (RwReal)point.y);

	CView::OnLButtonDown(nFlags, point);
}

void CNtlWEView::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	dGET_MOUSE()->SetBehavior(MB_LBDN, point.x, point.y, false);

	dGETAPP().GetNtlPLWorldEntity()->OnBtnUP((RwReal)point.x, (RwReal)point.y);

	CView::OnLButtonUp(nFlags, point);
}

void CNtlWEView::OnMouseMove(UINT nFlags, CPoint point)
{
	if(dGETAPP().GetNtlPLWorldEntity())
	{
		dGET_MOUSE()->SetBehavior(MB_MOVE, point.x, point.y);

		dGETAPP().GetNtlPLWorldEntity()->OnBtnMV((RwReal)point.x, (RwReal)point.y);
	}

	CView::OnMouseMove(nFlags, point);
}

BOOL CNtlWEView::OnEraseBkgnd(CDC* pDC)
{
	//dGETAPP().OnIdle();

	return CView::OnEraseBkgnd(pDC);
}

void CNtlWEView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	/*
	if(nChar == 't' || nChar == 'T')
	{
		((CNtlWEFrm*)(::AfxGetMainWnd()))->OnPaletteTerrain();
	}
	else if(nChar == 'o' || nChar == 'O')
	{
		((CNtlWEFrm*)(::AfxGetMainWnd()))->OnPaletteOrnament();
	}*/

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CNtlWEView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	dGET_MOUSE()->SetWheelMovement(zDelta);

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CNtlWEView::OnRButtonDown(UINT nFlags, CPoint point)
{
	dGET_MOUSE()->SetBehavior(MB_RBDN, point.x, point.y, true);

	CView::OnRButtonDown(nFlags, point);
}

void CNtlWEView::OnRButtonUp(UINT nFlags, CPoint point)
{
	dGET_MOUSE()->SetBehavior(MB_RBDN, point.x, point.y, false);

	CView::OnRButtonUp(nFlags, point);
}

RwBool CNtlWEView::IsNMapPropEditingEnable()
{
	if(dGET_CUR_PALETTE() != ePM_NM_PROP)
	{
		return FALSE;
	}

	if(dGETHDL()->m_pBrushSelector)
	{
		CNtlWorldBrushTile<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWorldBrushTile<CNtlWorldFieldManager>*>(dGETHDL()->m_pBrushSelector->GetController());
		if(pCurBrush->GetTileNum())
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CNtlWEView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	if(!dGETHDL())
		return;

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	if(dGET_CUR_PALETTE() == ePM_SPAWN)
	{
		if (dKEY(DIK_LCONTROL) && (GetSpawnInstance()->GetOneSpawnSelected() || GetSpawnInstance()->GetSpawnSelectedAtLeastOnce()))
		{
			CSpawnPopupDlg Dlg;
			Dlg.DoModal();
		}
	}
	else if (dGET_CUR_PALETTE() == ePM_ORNAMENT)
	{
		if (dKEY(DIK_LCONTROL) && dGETHDL()->GetCapturedDoodadsNum() > 0)
		{
			CMenu Menu, *pSubMenu; 

			Menu.LoadMenu(IDR_MENU_OBJECT_POPUP);

			pSubMenu = Menu.GetSubMenu(0);

			for (UINT i = 0; i < pSubMenu->GetMenuItemCount(); i++) 
			{ 
				CCmdUI state; 

				state.m_nIndexMax = pSubMenu->GetMenuItemCount(); 

				state.m_nIndex = i; 

				state.m_nID = pSubMenu->GetMenuItemID(i); 

				state.m_pMenu = pSubMenu; 

				state.DoUpdate(this, TRUE); 
			} 

			dGETDOC()->SetFieldPropMode(EFIELDPROP_MODE_OBJECT);
			pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
		}
	}

	if(IsNMapPropEditingEnable() && dKEY(DIK_LCONTROL))
	{
		CMenu Menu, *pSubMenu; 

		Menu.LoadMenu(IDR_PROP);

		pSubMenu = Menu.GetSubMenu(0);

		for (UINT i = 0; i < pSubMenu->GetMenuItemCount(); i++) 
		{ 
			CCmdUI state; 

			state.m_nIndexMax = pSubMenu->GetMenuItemCount(); 

			state.m_nIndex = i; 

			state.m_nID = pSubMenu->GetMenuItemID(i); 

			state.m_pMenu = pSubMenu; 

			state.DoUpdate(this, TRUE); 
		} 

		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}

	if(!dGETHDL()->m_pFieldUIManager && !dGETHDL()->m_pMergeUIManager)
		return;

	if(dGET_CUR_PALETTE() != ePM_FIELDUI)
		return;

	if(dGETHDL()->m_pFieldUIManager->IsWorking() && dGETHDL()->m_pFieldUIManager->IsCollided(pt.x, pt.y))
	{
		CMenu Menu, *pSubMenu; 

		Menu.LoadMenu(IDR_FIELD_POP);

		pSubMenu = Menu.GetSubMenu(0);

		for (UINT i = 0; i < pSubMenu->GetMenuItemCount(); i++) 
		{ 
			CCmdUI state; 

			state.m_nIndexMax = pSubMenu->GetMenuItemCount(); 

			state.m_nIndex = i; 

			state.m_nID = pSubMenu->GetMenuItemID(i); 

			state.m_pMenu = pSubMenu; 

			state.DoUpdate(this, TRUE); 
		} 
		
		dGETDOC()->SetFieldPropMode(EFIELDPROP_MODE_OUTDOOR);
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
	else if(dGETHDL()->m_pMergeUIManager->IsWorking() && dGETHDL()->m_pMergeUIManager->IsCollided(pt.x, pt.y))
	{
		CMenu Menu, *pSubMenu; 

		Menu.LoadMenu(IDR_MERGE_POP);

		pSubMenu = Menu.GetSubMenu(0);

		for (UINT i = 0; i < pSubMenu->GetMenuItemCount(); i++)
		{ 
			CCmdUI state;

			state.m_nIndexMax = pSubMenu->GetMenuItemCount(); 

			state.m_nIndex = i; 

			state.m_nID = pSubMenu->GetMenuItemID(i); 

			state.m_pMenu = pSubMenu; 

			state.DoUpdate(this, TRUE); 
		} 

		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}	
}

BOOL CNtlWEView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_PSTEX) && dGET_CUR_PALETTE() == ePM_SHADOW)
	{
		SetCursor(NULL);
	}
	else
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
	}

	return TRUE;
	//return CView::OnSetCursor(pWnd, nHitTest, message);
}

void CNtlWEView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	dGET_MOUSE()->SetBehavior(MB_LBDC, point.x, point.y, true);

	CView::OnLButtonDblClk(nFlags, point);
}