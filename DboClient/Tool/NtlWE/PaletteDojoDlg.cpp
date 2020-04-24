// PaletteDojo.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"

#include "NtlWorldPaletteDefine.h"

#include "PaletteDojoDlg.h"

#include "NtlWorldEntityPreview.h"

// CPaletteDojoDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPaletteDojoDlg, CDialog)

CPaletteDojoDlg::CPaletteDojoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPaletteDojoDlg::IDD, pParent)
	, m_strTapMovementEdit(_T(""))
	, m_strRotTabValue(_T(""))
	, m_StrScaleValue(_T(""))
	, m_CurDoodadScaleX(_T("0.00"))
	, m_CurDoodadScaleY(_T("0.00"))
	, m_CurDoodadScaleZ(_T("0.00"))
{

}

CPaletteDojoDlg::~CPaletteDojoDlg()
{
}

void CPaletteDojoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE, m_TreeOrnament);
	DDX_Control(pDX, IDC_LIST_LEVEL, m_listLevel);
	DDX_Text(pDX, IDC_TAP_MOVEMENT_EDIT, m_strTapMovementEdit);
	DDX_Text(pDX, IDC_R_TAP_VALUE, m_strRotTabValue);
	DDX_Text(pDX, IDC_S_TAP_VALUE, m_StrScaleValue);
	DDX_Text(pDX, IDC_DOODAD_SCALE_X, m_CurDoodadScaleX);
	DDX_Text(pDX, IDC_DOODAD_SCALE_Y, m_CurDoodadScaleY);
	DDX_Text(pDX, IDC_DOODAD_SCALE_Z, m_CurDoodadScaleZ);
}


BEGIN_MESSAGE_MAP(CPaletteDojoDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CPaletteDojoDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPaletteDojoDlg::OnBnClickedCancel)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, OnTvnSelchangedTree)
	ON_LBN_SELCHANGE(IDC_LIST_LEVEL, &CPaletteDojoDlg::OnLbnSelchangeListLevel)
END_MESSAGE_MAP()


// CPaletteDojoDlg 메시지 처리기입니다.

BOOL CPaletteDojoDlg::PreTranslateMessage(MSG* pMsg)
{
	// apply scaling only for one doodad
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if(	pMsg->hwnd == GetDlgItem(IDC_DOODAD_SCALE_X)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_DOODAD_SCALE_Y)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_DOODAD_SCALE_Z)->m_hWnd)
		{
			UpdateData(TRUE);

			if (dGETHDL()->GetCurDojo())
			{
				RwV3d vScale;
				vScale.x = static_cast<RwReal>(::atof(m_CurDoodadScaleX.GetBuffer(0)));
				vScale.y = static_cast<RwReal>(::atof(m_CurDoodadScaleY.GetBuffer(0)));
				vScale.z = static_cast<RwReal>(::atof(m_CurDoodadScaleZ.GetBuffer(0)));
				dGETHDL()->GetCurDojo()->SetScaleVector(&vScale);
			}
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
			if (dGETHDL()->GetCurDojo())
			{
				dGETHDL()->GetCurDojo()->SetScale(dGETHDL()->m_ScalingValue);
			}			
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

	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
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

void CPaletteDojoDlg::InitializeWorldData()
{
	CNtlMTXml xmlLoad;
	SItemNode sItemNode;
	if(xmlLoad.LoadTreeXML(L"Tool/ModelTool/TreeObject.XML", &sItemNode))
	{
		m_TreeOrnament.DeleteAllItems();
		InsertItemRecursive(&sItemNode, NULL);
	}

	m_hRoot = m_TreeOrnament.GetRootItem();

	m_TreeOrnament.Expand(m_hRoot, TVE_EXPAND);

	m_listLevel.ResetContent();

	m_listLevel.AddString("Level 1 Dojo");
	m_listLevel.AddString("Level 2 Dojo");
	m_listLevel.AddString("Level 3 Dojo");
	m_listLevel.AddString("Level 4 Dojo");
	m_listLevel.AddString("Level 5 Dojo");
	m_listLevel.AddString("Level 6 Dojo");
	m_listLevel.AddString("Level 7 Dojo");

	m_strTapMovementEdit.Format("%.2f", dGETHDL()->m_fObjMovementSensitivity);
	m_strRotTabValue.Format("%.2f", dGETHDL()->m_RotationTabValue);
	m_StrScaleValue.Format("%.2f", dGETHDL()->m_ScalingValue);

	UpdatePropertyData(NULL);

	UpdateData(FALSE);
}

LRESULT CPaletteDojoDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
void CPaletteDojoDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//OnOK();
}

void CPaletteDojoDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//OnCancel();
}

void CPaletteDojoDlg::InsertItemRecursive(SItemNode* pItemNode, HTREEITEM hParentItem) 
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

void CPaletteDojoDlg::OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW	pNMTreeView	= reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM		hTreeItem	= pNMTreeView->itemNew.hItem;
	CString			StrObjName	= m_TreeOrnament.GetItemText(hTreeItem);

	if (m_hRoot == hTreeItem)
		return;

	if (m_TreeOrnament.ItemHasChildren(hTreeItem))
		return;

	if (dGET_BRUSH_ENTITY())
	{
		GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
		dGET_BRUSH_ENTITY() = NULL;
	}

	SPLDojoCreateParam createParam;
	createParam.pPos		= &dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;
	
	createParam.iDojoID		= -1;
	createParam.fFadeInTime	= 1.0f;
	createParam.fFadeOutTime= 1.0f;
	
	memset(createParam.acObjectName, 0, sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN * dPL_DOJO_MAX_LEVEL);
	memset(createParam.acEffectNameUp, 0, sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN * dPL_DOJO_MAX_LEVEL);
	memset(createParam.acEffectNameDown, 0, sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN * dPL_DOJO_MAX_LEVEL);

	for (int i = 0; i < dPL_DOJO_MAX_LEVEL; ++i)
	{
		sprintf_s(createParam.acObjectName[i], dPL_DOJO_RES_NAME_LEN, "%s", StrObjName.GetBuffer());
	}

	dGET_BRUSH_ENTITY() = static_cast<CNtlPLEntity*>(GetSceneManager()->CreateEntity(PLENTITY_DOJO, NULL, &createParam));
	DBO_ASSERT(dGET_BRUSH_ENTITY(), "Entity create failed.");

	CNtlPLDojo* pDojo = static_cast<CNtlPLDojo*>(dGET_BRUSH_ENTITY());
	if (pDojo->GetDojoObject())
	{
		dGETHDL()->m_pPreview->SetCameraAtObj(pDojo->GetDojoObject());
	}

	UpdatePropertyData(dGET_BRUSH_ENTITY());

	*pResult = 0;
}

VOID CPaletteDojoDlg::UpdatePreview()
{
	RECT	rt;
	POINT	pt;

	GetWindowRect(&rt);
	GetCursorPos(&pt);

	if(pt.x > rt.left && pt.x < rt.right && pt.y > rt.top && pt.y < rt.bottom)
	{
		if(dGET_BRUSH_ENTITY())
		{
			CNtlPLDojo* pDojo = static_cast<CNtlPLDojo*>(dGET_BRUSH_ENTITY());
			if (pDojo->GetDojoObject())
			{
				RwV3d OldPos = pDojo->GetDojoObject()->GetPosition();

				RwV3d CurPos = {0.0f, 0.0f, 0.0f};
				pDojo->GetDojoObject()->SetPosition(&CurPos);

				dGETHDL()->m_pPreview->Update(pDojo->GetDojoObject());

				pDojo->GetDojoObject()->SetPosition(&OldPos);
			}			
		}
	}
}

VOID CPaletteDojoDlg::RenderPreview()
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
void CPaletteDojoDlg::OnLbnSelchangeListLevel()
{
	if (m_listLevel.GetCurSel() != -1 && m_pPLDojoCur)
	{
		if (strlen(m_pPLDojoCur->GetResObjectName(m_listLevel.GetCurSel())))
		{
			m_pPLDojoCur->SetLevel(m_listLevel.GetCurSel());
		}
		else
		{
			AfxMessageBox("Invalid ResourceName.");
			m_listLevel.SetCurSel(-1);
		}
	}
}


void CPaletteDojoDlg::UpdatePropertyData(const CNtlPLEntity* pEntity)
{
	if (!pEntity || pEntity->GetClassType() != PLENTITY_DOJO)
	{
		m_pPLDojoCur = NULL;
		m_listLevel.SetCurSel(-1);
		GetDlgItem(IDC_LIST_LEVEL)->EnableWindow(FALSE);
	}
	else
	{
		m_pPLDojoCur = (CNtlPLDojo*)pEntity;
		m_listLevel.SetCurSel(m_pPLDojoCur->GetLevel());
		GetDlgItem(IDC_LIST_LEVEL)->EnableWindow(TRUE);

		const RwV3d* pScale = m_pPLDojoCur->GetScaleVector();

		m_CurDoodadScaleX.Format("%.2f", pScale->x);
		m_CurDoodadScaleY.Format("%.2f", pScale->y);
		m_CurDoodadScaleZ.Format("%.2f", pScale->z);
	}
	UpdateData(FALSE);	
}

SPLDojoCreateParam*	CPaletteDojoDlg::GetCreateParam(CNtlPLEntity* pEntity)
{
	if (pEntity && pEntity->GetClassType() == PLENTITY_DOJO)
	{
		CNtlPLDojo* pPLDojo = (CNtlPLDojo*)pEntity;

		m_sDojoCreateParam.pPos			= &dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;
		m_sDojoCreateParam._pUserData	= NULL;

		m_sDojoCreateParam.iDojoID		= pPLDojo->GetDojoID();
		m_sDojoCreateParam.fFadeInTime	= pPLDojo->GetFadeInTime();
		m_sDojoCreateParam.fFadeOutTime	= pPLDojo->GetFadeOutTime();

		for (int i = 0; i < dPL_DOJO_MAX_LEVEL; ++i)
		{
			memcpy(m_sDojoCreateParam.acObjectName[i], pPLDojo->GetResObjectName(i), sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN);
			memcpy(m_sDojoCreateParam.acEffectNameUp[i], pPLDojo->GetResEffectNameUp(i), sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN);
			memcpy(m_sDojoCreateParam.acEffectNameDown[i], pPLDojo->GetResEffectNameDown(i), sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN);
		}
		
		return &m_sDojoCreateParam;
	}
	return NULL;
}