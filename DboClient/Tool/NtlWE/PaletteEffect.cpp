// PaletteEffect.cpp : implementation file
//

#include "stdafx.h"
#include "NtlWE.h"
#include "PaletteEffect.h"
#include "NtlPLObjectProperty.h"
#include "NtlWorldPaletteDefine.h"
#include "NtlPLObject.h"


// CPaletteEffect dialog

IMPLEMENT_DYNAMIC(CPaletteEffect, CDialog)

CPaletteEffect::CPaletteEffect(CWnd* pParent /*=NULL*/)
	: CDialog(CPaletteEffect::IDD, pParent)
	, m_strScale(_T(""))
	, m_strRotateTab(_T(""))
	, m_strTranslateTab(_T(""))
	, m_strVisibleDist(_T(""))
{

}

CPaletteEffect::~CPaletteEffect()	
{
}

void CPaletteEffect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EFFECT_TREE, m_EffectTreeCtrl);
	DDX_Text(pDX, IDC_S_TAP_VALUE_EFFECT, m_strScale);
	DDX_Text(pDX, IDC_R_TAP_VALUE_EFFECT, m_strRotateTab);
	DDX_Text(pDX, IDC_TAP_MOVEMENT_EDIT_EFFECT, m_strTranslateTab);
	DDX_Control(pDX, IDC_SLIDER_VISIBLE_DIST, m_SliderVisibleDist);
	DDX_Text(pDX, IDC_EDIT_VISIBLE_DIST, m_strVisibleDist);
}


BEGIN_MESSAGE_MAP(CPaletteEffect, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_EFFECT_TREE, &CPaletteEffect::OnTvnSelchangedEffectTree)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CPaletteEffect message handlers

void CPaletteEffect::CreateLeafNode(const char *pName)
{
	std::string PrefixSrc = pName;
	std::string PrefixObj = "obj_";
	std::string PrefixWrd = "wrd_";

	if(!PrefixSrc.compare(0, 4, PrefixObj))
	{
		TVINSERTSTRUCT TI;
		TI.hParent				= m_hRoot;
		TI.hInsertAfter			= TVI_LAST;
		TI.item.mask			= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		TI.item.iImage			= 0;
		TI.item.iSelectedImage	= 1;
		TI.item.pszText			= const_cast<LPSTR>(pName);
		m_EffectTreeCtrl.InsertItem(&TI);
	}
	else if(!PrefixSrc.compare(0, 4, PrefixWrd))
	{
		TVINSERTSTRUCT TI;
		TI.hParent				= m_hRootWorld;
		TI.hInsertAfter			= TVI_LAST;
		TI.item.mask			= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		TI.item.iImage			= 0;
		TI.item.iSelectedImage	= 1;
		TI.item.pszText			= const_cast<LPSTR>(pName);
		m_EffectTreeCtrl.InsertItem(&TI);
	}
	else
	{
		return;
	}
}

void CPaletteEffect::InitializeWorldData()
{
	m_EffectTreeCtrl.DeleteAllItems();

	m_hRoot = m_EffectTreeCtrl.GetRootItem();
	
	// create root tree item
	TVINSERTSTRUCT TI;
	TI.hParent				= TVI_ROOT;
	TI.hInsertAfter			= TVI_LAST;
	TI.item.mask			= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	TI.item.iImage			= 0;
	TI.item.iSelectedImage	= 1;
	TI.item.pszText			= "Object";
	m_hRoot					= m_EffectTreeCtrl.InsertItem(&TI);

	TI.hParent				= TVI_ROOT;
	TI.hInsertAfter			= TVI_LAST;
	TI.item.mask			= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	TI.item.iImage			= 0;
	TI.item.iSelectedImage	= 1;
	TI.item.pszText			= "World";
	m_hRootWorld			= m_EffectTreeCtrl.InsertItem(&TI);

	// create leaf node
	CNtlPLEntity*								pCheckEntity = NULL;
	CNtlResourceEffect*							pPLProp;
	CNtlPLPropertyContainer::MapProp::iterator	it;
	CNtlPLPropertyContainer::MapProp&			Props = CNtlPLPropertyContainer::GetInstance()->Gets(PLENTITY_EFFECT);

	const RwV3d Pos = {-999.0f, -999.0f, -999.0f};
	SPLEntityCreateParam PLEntityCreateParam;
	PLEntityCreateParam.pPos = &Pos;

	for(it = Props.begin(); it != Props.end(); it++)
	{
		pPLProp		= reinterpret_cast<CNtlResourceEffect*>((*it).second);
		if (!pPLProp->IsAutoDelete())
		{
			CreateLeafNode(pPLProp->GetName());
		}
	}
	m_EffectTreeCtrl.Expand(m_hRoot, TVE_EXPAND);

	// object transform info.
	m_strTranslateTab.Format("%.2f", dGETHDL()->m_EffectTransform[2]);
	m_strRotateTab.Format("%.2f", dGETHDL()->m_EffectTransform[1]);
	m_strScale.Format("%.2f", dGETHDL()->m_EffectTransform[0]);

	m_SliderVisibleDist.SetRange(0, 512);

	UpdatePropertyData(NULL);

	UpdateData(FALSE);
}

BOOL CPaletteEffect::PreTranslateMessage(MSG* pMsg)
{
	// Object transform info.
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if(pMsg->hwnd == GetDlgItem(IDC_TAP_MOVEMENT_EDIT_EFFECT)->m_hWnd)
		{
			UpdateData(TRUE);

			dGETHDL()->m_EffectTransform[2] = static_cast<RwReal>(::atof(m_strTranslateTab.GetBuffer(0)));
		}
	}

	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if(pMsg->hwnd == GetDlgItem(IDC_S_TAP_VALUE_EFFECT)->m_hWnd)
		{
			UpdateData(TRUE);

			dGETHDL()->m_EffectTransform[0] = static_cast<RwReal>(::atof(m_strScale.GetBuffer(0)));
			dGETHDL()->SetScaling(1);
		}
	}

	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if(pMsg->hwnd == GetDlgItem(IDC_R_TAP_VALUE_EFFECT)->m_hWnd)
		{
			UpdateData(TRUE);

			dGETHDL()->m_EffectTransform[1] = static_cast<RwReal>(::atof(m_strRotateTab.GetBuffer(0)));
		}
	}
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if(pMsg->hwnd == GetDlgItem(IDC_EDIT_VISIBLE_DIST)->m_hWnd)
		{
			UpdateData(TRUE);
			CNtlInstanceEffect* pEffect = dGETHDL()->GetCurEffect();
			if (pEffect)
			{
				RwReal fVisibleDist = static_cast<RwReal>(::atof(m_strVisibleDist.GetBuffer(0)));
				CLAMP(fVisibleDist, 0, 512);
				pEffect->SetVisibleCullingDistance(fVisibleDist);

				UpdatePropertyData(pEffect);
			}
		}
	}

	// windows messages
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

BOOL CPaletteEffect::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CPaletteEffect::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		{
			dGET_CUR_PALETTE() = ePM_DEFAULT;
			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.m_pEventHandler->SetEffectsDummy(FALSE);
			dGETHDL()->DeleteAllPaletteDummies();
			break;
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CPaletteEffect::OnTvnSelchangedEffectTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView	= reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM hTreeItem			= pNMTreeView->itemNew.hItem;
	CString StrObjName			= m_EffectTreeCtrl.GetItemText(hTreeItem);

	RwV3d Pos = {-999.0f, -999.0f, -999.0f};
	SPLEntityCreateParam PLEntityCreateParam;
	PLEntityCreateParam.pPos = &Pos;

	if(m_hRoot == hTreeItem)
		return;

	if(m_EffectTreeCtrl.ItemHasChildren(hTreeItem))
		return;

	// update display brush entity
	if(dGET_BRUSH_ENTITY())
	{
		GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
		dGET_BRUSH_ENTITY() = NULL;
	}

	dGET_BRUSH_ENTITY() = static_cast<CNtlPLEntity*>(GetSceneManager()->CreateEntity(PLENTITY_EFFECT, StrObjName.GetBuffer(0), &PLEntityCreateParam));
	DBO_ASSERT(dGET_BRUSH_ENTITY(), "Entity create failed.");

	RwV3d vDefP	= {0.0f, 0.0f, 0.0f};

	RwV3d vDist;
	vDist.x = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.x;
	vDist.y = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.y;
	vDist.z = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.z;

	dGET_BRUSH_ENTITY()->SetScale(1.0f);
	dGET_BRUSH_ENTITY()->SetRotate(0.0f, 0.0f, 0.0f);
	dGET_BRUSH_ENTITY()->SetPosition(&vDist);

	UpdatePropertyData(dGET_BRUSH_ENTITY());

	dGET_CUR_PALETTE() = ePM_EFFECT;

	*pResult = 0;
}

void CPaletteEffect::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar->GetSafeHwnd() == m_SliderVisibleDist.GetSafeHwnd())
	{
		CNtlInstanceEffect* pEffect = dGETHDL()->GetCurEffect();
		if (pEffect)
		{
			pEffect->SetVisibleCullingDistance(static_cast<RwReal>(m_SliderVisibleDist.GetPos()));
			UpdatePropertyData(pEffect);
		}
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPaletteEffect::UpdatePropertyData(CNtlPLEntity* pPLEntity)
{
// 	if (!pPLEntity || pPLEntity->GetClassType() != PLENTITY_EFFECT)
// 	{
// 		GetDlgItem(IDC_EDIT_VISIBLE_DIST)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_SLIDER_VISIBLE_DIST)->EnableWindow(FALSE);
// 
// 		return;
// 	}
// 
// 	GetDlgItem(IDC_EDIT_VISIBLE_DIST)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_SLIDER_VISIBLE_DIST)->EnableWindow(TRUE);
// 
// 	m_strVisibleDist.Format("%.2f", pPLEntity->GetVisibleCullingDistance());
// 	m_SliderVisibleDist.SetPos(static_cast<RwInt32>(pPLEntity->GetVisibleCullingDistance()));
// 
// 	UpdateData(FALSE);
}