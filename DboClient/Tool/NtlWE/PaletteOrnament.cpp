// PaletteOrnament.cpp : 구현 파일입니다.
//

#include "stdafx.h"

#include "RtLTMap.h"

#include "NtlWE.h"
#include "PaletteOrnament.h"
#include "NtlWorldPaletteDefine.h"
#include "NtlPLPropertyContainer.h"
#include "NtlPLObjectProperty.h"
#include "NtlPLObject.h"
#include "NtlPLEntity.h"
#include "NtlMTXml.h"
#include "NtlWorldEntityPreview.h"

// CPaletteOrnament 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPaletteOrnament, CDialog)
CPaletteOrnament::CPaletteOrnament(CWnd* pParent /*=NULL*/)
	: CDialog(CPaletteOrnament::IDD, pParent)
	, m_strTapMovementEdit(_T(""))
	, m_strRotTabValue(_T(""))
	, m_StrScaleValue(_T(""))
	, m_CurDoodadScaleX(_T("0.00"))
	, m_CurDoodadScaleY(_T("0.00"))
	, m_CurDoodadScaleZ(_T("0.00"))
{
}

CPaletteOrnament::~CPaletteOrnament()
{
}

void CPaletteOrnament::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE, m_TreeOrnament);
	DDX_Text(pDX, IDC_TAP_MOVEMENT_EDIT, m_strTapMovementEdit);
	DDX_Text(pDX, IDC_R_TAP_VALUE, m_strRotTabValue);
	DDX_Text(pDX, IDC_S_TAP_VALUE, m_StrScaleValue);
	DDX_Text(pDX, IDC_DOODAD_SCALE_X, m_CurDoodadScaleX);
	DDX_Text(pDX, IDC_DOODAD_SCALE_Y, m_CurDoodadScaleY);
	DDX_Text(pDX, IDC_DOODAD_SCALE_Z, m_CurDoodadScaleZ);
}

BEGIN_MESSAGE_MAP(CPaletteOrnament, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, OnTvnSelchangedTree)
	ON_WM_ACTIVATE()
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()

BOOL CPaletteOrnament::PreTranslateMessage(MSG* pMsg)
{
	// apply scaling only for one doodad
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if(	pMsg->hwnd == GetDlgItem(IDC_DOODAD_SCALE_X)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_DOODAD_SCALE_Y)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_DOODAD_SCALE_Z)->m_hWnd)
		{
			UpdateData(TRUE);
			
			dGETHDL()->SetScaleCurDoodad(static_cast<RwReal>(::atof(m_CurDoodadScaleX.GetBuffer(0))),
																		static_cast<RwReal>(::atof(m_CurDoodadScaleY.GetBuffer(0))),
																		static_cast<RwReal>(::atof(m_CurDoodadScaleZ.GetBuffer(0))));

		}
	}

	// Object transform info.
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if(pMsg->hwnd == GetDlgItem(IDC_TAP_MOVEMENT_EDIT)->m_hWnd)
		{
			UpdateData(TRUE);
		
			dGETHDL()->m_fObjMovementSensitivity = static_cast<RwReal>(::atof(m_strTapMovementEdit.GetBuffer(0)));
		}
	}

	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if(pMsg->hwnd == GetDlgItem(IDC_S_TAP_VALUE)->m_hWnd)
		{
			UpdateData(TRUE);

			dGETHDL()->m_ScalingValue = static_cast<RwReal>(::atof(m_StrScaleValue.GetBuffer(0)));
			dGETHDL()->SetScaling();
		}
	}

	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if(pMsg->hwnd == GetDlgItem(IDC_R_TAP_VALUE)->m_hWnd)
		{
			UpdateData(TRUE);

			dGETHDL()->m_RotationTabValue = static_cast<RwReal>(::atof(m_strRotTabValue.GetBuffer(0)));
		}
	}

	// windows messages
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case 'R':
			{
				//dGETHDL()->m_pPreview->m_Rot.x += 3.3f;
			}
			break;

		case 'T':
			{
				//dGETHDL()->m_pPreview->m_Rot.x -= 3.3f;
			}
			break;

		case VK_ADD:
			{
				RwCamera*	pCam		= dGETHDL()->m_pPreview->GetCamera();
				RwFrame*	pCamFrame	= RwCameraGetFrame(pCam);
				RwMatrix*	pM			= RwFrameGetMatrix(pCamFrame);
				RwV3d		pos			= *RwMatrixGetPos(pM);
				RwV3d		invPos;

				RwV3dNegate(&invPos, &pos);
				RwFrameTranslate(pCamFrame, &invPos, rwCOMBINEPOSTCONCAT);

				RwV3dIncrementScaled(&pos, RwMatrixGetAt(pM), 1.0f);
				RwFrameTranslate(pCamFrame, &pos, rwCOMBINEPOSTCONCAT);
			}
			break;

		case VK_SUBTRACT:
			{
				RwCamera*	pCam		= dGETHDL()->m_pPreview->GetCamera();
				RwFrame*	pCamFrame	= RwCameraGetFrame(pCam);
				RwMatrix*	pM			= RwFrameGetMatrix(pCamFrame);
				RwV3d		pos			= *RwMatrixGetPos(pM);
				RwV3d		invPos;

				RwV3dNegate(&invPos, &pos);
				RwFrameTranslate(pCamFrame, &invPos, rwCOMBINEPOSTCONCAT);

				RwV3dIncrementScaled(&pos, RwMatrixGetAt(pM), -1.0f);
				RwFrameTranslate(pCamFrame, &pos, rwCOMBINEPOSTCONCAT);
			}
			break;

		case VK_ESCAPE:
			{
				return true;
				break;
			}
		case VK_RETURN:
			{
				return true;
				break;
			}
		default:
			break;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CPaletteOrnament::CreateLeafNode(const char *pName, HTREEITEM hTreeItem)
{
	TVINSERTSTRUCT TI;
	TI.hParent				= hTreeItem;
	TI.hInsertAfter			= TVI_LAST;
	TI.item.mask			= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	TI.item.iImage			= 0;
	TI.item.iSelectedImage	= 1;
	TI.item.pszText			= const_cast<LPSTR>(pName);
	m_TreeOrnament.InsertItem(&TI);
}

HTREEITEM CPaletteOrnament::GetItemByName(char* szItemName, HTREEITEM hItem)
{
	while (hItem)
    {
		CString sItemText = m_TreeOrnament.GetItemText(hItem);
		
		if(sItemText == szItemName)
			return hItem;

        if (m_TreeOrnament.ItemHasChildren(hItem))
        {
            HTREEITEM hResultItem = GetItemByName(szItemName, m_TreeOrnament.GetChildItem(hItem));
			if(hResultItem != NULL)
				return hResultItem;
		} 

        hItem = m_TreeOrnament.GetNextSiblingItem(hItem);
	}
	return NULL;
}

void CPaletteOrnament::InitializeWorldData()
{
	USES_CONVERSION;
	
	CNtlMTXml xmlLoad;
	SItemNode sItemNode;
    if(xmlLoad.LoadTreeXML(L"Tool/ModelTool/TreeObject.XML", &sItemNode))
	{
		m_TreeOrnament.DeleteAllItems();
		InsertItemRecursive(&sItemNode, NULL);
	}
	
	m_hRoot = m_TreeOrnament.GetRootItem();
	/*
	// create root tree item
	TVINSERTSTRUCT TI;
	TI.hParent				= TVI_ROOT;
	TI.hInsertAfter			= TVI_LAST;
	TI.item.mask			= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	TI.item.iImage			= 0;
	TI.item.iSelectedImage	= 1;
	TI.item.pszText			= "ornament map objects";
	m_hRoot					= m_TreeOrnament.InsertItem(&TI);

	// create first category tree item
	char Indices[dNTL_ORNAMENT_OBJ_NUM][MAX_PATH] = {	"Construction_Shop",
														"Construction_Building",
														"Construction_Road",
														"Construction_Wall",
														"Landscape_Game",
														"Landscape_Life",
														"Landscape_Sight",
														"Matter_Tool",
														"Matter_Carry",
														"Nature_Tree",
														"Nature_Grass",
														"Nature_Rock",
														"Nature_Animal"};

	// create first category
	for(int i = 0; i < dNTL_ORNAMENT_OBJ_NUM; ++i)
	{
		TI.hParent				= m_hRoot;
		TI.hInsertAfter			= TVI_LAST;
		TI.item.mask			= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		TI.item.iImage			= 0;
		TI.item.iSelectedImage	= 1;
		TI.item.pszText			= Indices[i];
		m_hCat0TreeItem[i]		= m_TreeOrnament.InsertItem(&TI);
	}

	// create leaf node
	CNtlPLObjectProperty *pPLObjProp;
	CNtlPLPropertyContainer::MapProp::iterator it;
	CNtlPLPropertyContainer::MapProp& Props = CNtlPLPropertyContainer::GetInstance()->Gets(PLENTITY_OBJECT);
	
	for(it = Props.begin(); it != Props.end(); it++)
	{
		pPLObjProp	= reinterpret_cast<CNtlPLObjectProperty *>((*it).second);

		switch(pPLObjProp->GetCategory())
		{
		case E_OBJECT_CATEGORY_CONSTRUCTION_SHOP:		CreateLeafNode(pPLObjProp->GetName(), m_hCat0TreeItem[0]);	break;
		case E_OBJECT_CATEGORY_CONSTRUCTION_BUILDING:	CreateLeafNode(pPLObjProp->GetName(), m_hCat0TreeItem[1]);	break;
		case E_OBJECT_CATEGORY_CONSTRUCTION_WALL:		CreateLeafNode(pPLObjProp->GetName(), m_hCat0TreeItem[2]);	break;
		case E_OBJECT_CATEGORY_CONSTRUCTION_ROAD:		CreateLeafNode(pPLObjProp->GetName(), m_hCat0TreeItem[3]);	break;
		case E_OBJECT_CATEGORY_LANDSCAPE_GAME:			CreateLeafNode(pPLObjProp->GetName(), m_hCat0TreeItem[4]);	break;
		case E_OBJECT_CATEGORY_LANDSCAPE_LIFE:			CreateLeafNode(pPLObjProp->GetName(), m_hCat0TreeItem[5]);	break;
		case E_OBJECT_CATEGORY_LANDSCAPE_SIGHT:			CreateLeafNode(pPLObjProp->GetName(), m_hCat0TreeItem[6]);	break;
		case E_OBJECT_CATEGORY_MATTER_TOOL:				CreateLeafNode(pPLObjProp->GetName(), m_hCat0TreeItem[7]);	break;
		case E_OBJECT_CATEGORY_MATTER_CARRY:			CreateLeafNode(pPLObjProp->GetName(), m_hCat0TreeItem[8]);	break;
		case E_OBJECT_CATEGORY_NATURE_TREE:				CreateLeafNode(pPLObjProp->GetName(), m_hCat0TreeItem[9]);	break;
		case E_OBJECT_CATEGORY_NATURE_GRASS:			CreateLeafNode(pPLObjProp->GetName(), m_hCat0TreeItem[10]);	break;
		case E_OBJECT_CATEGORY_NATURE_ROCK:				CreateLeafNode(pPLObjProp->GetName(), m_hCat0TreeItem[11]);	break;
		case E_OBJECT_CATEGORY_NATURE_ANIMAL:			CreateLeafNode(pPLObjProp->GetName(), m_hCat0TreeItem[12]);	break;
		}
	}
	*/

	m_TreeOrnament.Expand(m_hRoot, TVE_EXPAND);

	dGET_MAPOBJ_MODE() = eOEM_IDLE;

	// object transform info.
	m_strTapMovementEdit.Format("%.2f", dGETHDL()->m_fObjMovementSensitivity);
	m_strRotTabValue.Format("%.2f", dGETHDL()->m_RotationTabValue);
	m_StrScaleValue.Format("%.2f", dGETHDL()->m_ScalingValue);

	UpdateData(FALSE);
}

void CPaletteOrnament::OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView	= reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM hTreeItem			= pNMTreeView->itemNew.hItem;
	CString StrObjName			= m_TreeOrnament.GetItemText(hTreeItem);

	RwV3d Pos = {-999.0f, -999.0f, -999.0f};
	SPLEntityCreateParam PLEntityCreateParam;
	PLEntityCreateParam.pPos = &Pos;

	if(m_hRoot == hTreeItem)
		return;

	if(m_TreeOrnament.ItemHasChildren(hTreeItem))
		return;

	//for(int i = 0; i < dNTL_ORNAMENT_OBJ_NUM; ++i)
	//	if(hTreeItem == m_hCat0TreeItem[i])
	//		return;

	// update display brush entity
	if(dGET_BRUSH_ENTITY())
	{
		GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
		dGET_BRUSH_ENTITY() = NULL;
	}

	dGET_BRUSH_ENTITY() = static_cast<CNtlPLEntity*>(GetSceneManager()->CreateEntity(PLENTITY_OBJECT, StrObjName.GetBuffer(0), &PLEntityCreateParam));
	DBO_ASSERT(dGET_BRUSH_ENTITY(), "Entity create failed.");

	(static_cast<CNtlPLObject*>(dGET_BRUSH_ENTITY()))->SetFadeEnable(FALSE);

	RwV3d vDefP	= {0.0f, 0.0f, 0.0f};
	RwV3d vDefS	= {1.0f, 1.0f, 1.0f};

	RwV3d vDist;
	vDist.x = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.x;
	vDist.y = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.y;
	vDist.z = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.z;

	static_cast<CNtlPLObject*>(dGET_BRUSH_ENTITY())->SetScale(&vDefS);
	static_cast<CNtlPLObject*>(dGET_BRUSH_ENTITY())->SetRotate(0.0f, 0.0f, 0.0f);
	static_cast<CNtlPLObject*>(dGET_BRUSH_ENTITY())->SetPosition(&vDist);

	RwChar Name[128];
	strcpy_s(Name, 128, const_cast<RwChar*>(static_cast<CNtlPLObject*>(dGET_BRUSH_ENTITY())->GetName()));
	if ((Name[3] == 'n' || Name[3] == 'N') &&
		Name[4] == '_' &&
		(Name[5] == 't' || Name[5] == 'T') &&
		(Name[6] == 'r' || Name[6] == 'R') &&
		(Name[7] == 'e' || Name[7] == 'E') &&
		(Name[8] == 'e' || Name[8] == 'E') &&
		Name[9] == '_')
	{
		static_cast<CNtlPLObject*>(dGET_BRUSH_ENTITY())->SetFlags(static_cast<CNtlPLObject*>(dGET_BRUSH_ENTITY())->GetFlags() | NTL_PLEFLAG_SHADOW_PROP);
	}

	if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	{
		static_cast<CNtlPLObject*>(dGET_BRUSH_ENTITY())->SetLightMapObjectFlags(rtLTMAPOBJECTLIGHTMAP);
		static_cast<CNtlPLObject*>(dGET_BRUSH_ENTITY())->SetLightMapMaterialFlags(rtLTMAPMATERIALLIGHTMAP);

		static_cast<CNtlPLObject*>(dGET_BRUSH_ENTITY())->CheckLightmapFlags();
	}

	dGET_CUR_PALETTE() = ePM_ORNAMENT;
	dGET_MAPOBJ_MODE() = eOEM_PLACE;

	if(dGET_BRUSH_ENTITY())
	{
		dGETHDL()->m_pPreview->SetCameraAtObj(dGET_BRUSH_ENTITY());
	}

	*pResult = 0;
}

void CPaletteOrnament::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
}
LRESULT CPaletteOrnament::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		{
			dGET_CUR_PALETTE() = ePM_DEFAULT;
			dGETHDL()->DeleteAllPaletteDummies();
			break;
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CPaletteOrnament::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	/*
	CRect wndRect, trayRect; 

	int leftTaskbar = 0, rightTaskbar = 0, topTaskbar = 0, bottomTaskbar = 0; 

	GetWindowRect(&wndRect); 

	int m_nXOffset = 22;
	int m_nYOffset = 22;

	// Screen resolution 

	int screenWidth = GetSystemMetrics(SM_CXSCREEN); 

	int screenHeight = GetSystemMetrics(SM_CYSCREEN); 



	// Find the taskbar 

	//CWnd* pWnd = FindWindow(_T("Shell_TrayWnd"), _T("")); 
	CWnd* pWnd = FindWindow(NULL, _T("SAVER 1.00")); 

	if(!pWnd)
		return;

	pWnd->GetWindowRect(&trayRect); 



	int wndWidth = wndRect.right - wndRect.left; 

	int wndHeight = wndRect.bottom - wndRect.top; 



	if(trayRect.top <= 0 && trayRect.left <= 0 && trayRect.right >= screenWidth) { 

		// top taskbar 

		topTaskbar = trayRect.bottom - trayRect.top; 

	} 

	else if(trayRect.top > 0 && trayRect.left <= 0) { 

		// bottom taskbar 

		bottomTaskbar = trayRect.bottom - trayRect.top; 

	} 

	else if(trayRect.top <= 0 && trayRect.left > 0) { 

		// right taskbar 

		rightTaskbar = trayRect.right - trayRect.left; 

	} 

	else { 

		// left taskbar 

		leftTaskbar = trayRect.right - trayRect.left; 

	} 



	// Snap to screen border 

	// Left border 

	if(lpwndpos->x >= -m_nXOffset + leftTaskbar && lpwndpos->x <= leftTaskbar + m_nXOffset) { 

		lpwndpos->x = leftTaskbar; 

	} 



	// Top border 

	if(lpwndpos->y >= -m_nYOffset && lpwndpos->y <= topTaskbar + m_nYOffset) { 

		lpwndpos->y = topTaskbar; 

	} 



	// Right border 

	if(lpwndpos->x + wndWidth <= screenWidth - rightTaskbar + m_nXOffset && 

		lpwndpos->x + wndWidth >= screenWidth - rightTaskbar - m_nXOffset) { 

			lpwndpos->x = screenWidth - rightTaskbar - wndWidth; 

	} 



	// Bottom border 

	if( lpwndpos->y + wndHeight <= screenHeight - bottomTaskbar + m_nYOffset 

		&& lpwndpos->y + wndHeight >= screenHeight - bottomTaskbar - m_nYOffset) { 

			lpwndpos->y = screenHeight - bottomTaskbar - wndHeight; 

	} */

	CDialog::OnWindowPosChanging(lpwndpos);
}

VOID CPaletteOrnament::UpdatePreview()
{
	RECT	rt;
	POINT	pt;

	GetWindowRect(&rt);
	GetCursorPos(&pt);

	if(pt.x > rt.left && pt.x < rt.right && pt.y > rt.top && pt.y < rt.bottom)
	{
		if(dGET_BRUSH_ENTITY())
		{
			RwV3d OldPos = dGET_BRUSH_ENTITY()->GetPosition();

			RwV3d CurPos = {0.0f, 0.0f, 0.0f};
			dGET_BRUSH_ENTITY()->SetPosition(&CurPos);

			dGETHDL()->m_pPreview->Update(dGET_BRUSH_ENTITY());

			dGET_BRUSH_ENTITY()->SetPosition(&OldPos);
		}
	}
}

VOID CPaletteOrnament::RenderPreview()
{
	RECT	rt;
	POINT	pt;

	GetWindowRect(&rt);
	GetCursorPos(&pt);

	if(pt.x > rt.left && pt.x < rt.right && pt.y > rt.top && pt.y < rt.bottom)
	{
		if(dGET_BRUSH_ENTITY())
		{
			dGETHDL()->m_pPreview->Render();
		}
	}
}

VOID CPaletteOrnament::RefreshScaleInfo(CNtlPLEntity* pEntity)
{
	const RwV3d* pScale = static_cast<CNtlPLObject*>(pEntity)->GetScale();

	m_CurDoodadScaleX.Format("%.2f", pScale->x);
	m_CurDoodadScaleY.Format("%.2f", pScale->y);
	m_CurDoodadScaleZ.Format("%.2f", pScale->z);

	UpdateData(FALSE);
}

void CPaletteOrnament::InsertItemRecursive( SItemNode* pItemNode, HTREEITEM hParentItem ) 
{
	// 자신을 추가한다.
	TV_INSERTSTRUCT sTvStruct;
	sTvStruct.hParent = hParentItem;
	sTvStruct.hInsertAfter = TVI_SORT;
	sTvStruct.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	sTvStruct.item.pszText = (LPSTR)(LPCSTR)pItemNode->strNodeName;

	if(pItemNode->eNodeType == NODE_SCIRPT)
	{
		// 스크립트라면 자신을 추가하고 끝낸다.
		sTvStruct.item.iImage = 2;
		sTvStruct.item.iSelectedImage = 2;
		m_TreeOrnament.InsertItem(&sTvStruct);
	}
	else	// 폴더라면 자식들을 재귀로 돈다.
	{
		sTvStruct.item.iImage = 0;
		sTvStruct.item.iSelectedImage = 0;
		HTREEITEM hItem = m_TreeOrnament.InsertItem(&sTvStruct);

		for each(SItemNode* pChildNode in pItemNode->vecChildList)
		{
			if(pChildNode)
			{
				InsertItemRecursive(pChildNode, hItem);
			}
		}
	}	
}