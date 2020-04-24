// PaletteShoreLine.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "PaletteShoreLine.h"
#include "NtlPLShoreLine.h"
#include "NtlPLShoreLineProp.h"
#include "NtlPLWorldEntity.h"
#include "NtlWorldPaletteDefine.h"


// CPaletteShoreLine 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPaletteShoreLine, CDialog)

CPaletteShoreLine::CPaletteShoreLine(CWnd* pParent /*=NULL*/)
	: CDialog(CPaletteShoreLine::IDD, pParent)
{
	m_hSelect = NULL;
}

CPaletteShoreLine::~CPaletteShoreLine()
{
}

void CPaletteShoreLine::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCRIPT_TREE, m_TreeCtrl);
}


BEGIN_MESSAGE_MAP(CPaletteShoreLine, CDialog)
	//ON_BN_CLICKED(IDC_AUTO_GEN_SHORELINE, &CPaletteShoreLine::OnBnClickedAutoGenShoreline)
	ON_NOTIFY(TVN_SELCHANGED, IDC_SCRIPT_TREE, &CPaletteShoreLine::OnTvnSelchangedScriptTree)
END_MESSAGE_MAP()


BOOL CPaletteShoreLine::PreTranslateMessage(MSG* pMsg)
{
	// prevent keys; escape, return
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

void CPaletteShoreLine::InitializeWorldData()
{
	if(!dGETAPP().GetNtlPLWorldEntity() || !dGETAPP().GetNtlPLWorldEntity()->GetWorldReady())
	{
		return;
	}

	m_TreeCtrl.DeleteAllItems();

	m_hSelect = NULL;

	TVINSERTSTRUCT			TI;
	CNtlPLShoreLineProp*	pProp = reinterpret_cast<CNtlPLShoreLineProp*>(CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_SHORELINE, NTL_PLEN_SHORELINE));

	// create root nodes
	if(pProp->m_vecUVSingleSLC.size())
	{
		TI.hParent				= TVI_ROOT;
		TI.hInsertAfter			= TVI_LAST;
		TI.item.mask			= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		TI.item.iImage			= 0;
		TI.item.iSelectedImage	= 1;
		TI.item.pszText			= "UV Single";
		m_hRoot[0] = m_TreeCtrl.InsertItem(&TI);
	}

	if(pProp->m_vecUVDualSLC.size())
	{
		TI.hParent				= TVI_ROOT;
		TI.hInsertAfter			= TVI_LAST;
		TI.item.mask			= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		TI.item.iImage			= 0;
		TI.item.iSelectedImage	= 1;
		TI.item.pszText			= "UV Dual";
		m_hRoot[1] = m_TreeCtrl.InsertItem(&TI);
	}

	if(pProp->m_vecSequenceSLC.size())
	{
		TI.hParent				= TVI_ROOT;
		TI.hInsertAfter			= TVI_LAST;
		TI.item.mask			= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		TI.item.iImage			= 0;
		TI.item.iSelectedImage	= 1;
		TI.item.pszText			= "Sequence";
		m_hRoot[2] = m_TreeCtrl.InsertItem(&TI);
	}

	// create leaf nodes
	for(RwUInt32 i = 0; i < pProp->m_vecUVSingleSLC.size(); ++i)
	{
		CUVSingleShoreLineContainer* pUVSingleSLC = pProp->m_vecUVSingleSLC[i];

		TI.hParent				= m_hRoot[0];
		TI.hInsertAfter			= TVI_LAST;
		TI.item.mask			= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		TI.item.iImage			= 0;
		TI.item.iSelectedImage	= 1;
		TI.item.pszText			= pUVSingleSLC->m_KeyName;
		m_TreeCtrl.InsertItem(&TI);
	}

	for(RwUInt32 i = 0; i < pProp->m_vecUVDualSLC.size(); ++i)
	{
		CUVDualShoreLineContainer* pUVDualSLC = pProp->m_vecUVDualSLC[i];

		TI.hParent				= m_hRoot[1];
		TI.hInsertAfter			= TVI_LAST;
		TI.item.mask			= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		TI.item.iImage			= 0;
		TI.item.iSelectedImage	= 1;
		TI.item.pszText			= pUVDualSLC->m_KeyName;
		m_TreeCtrl.InsertItem(&TI);
	}

	for(RwUInt32 i = 0; i < pProp->m_vecSequenceSLC.size(); ++i)
	{
		CSequenceShoreLineContainer* pSequenceSLC = pProp->m_vecSequenceSLC[i];

		TI.hParent				= m_hRoot[2];
		TI.hInsertAfter			= TVI_LAST;
		TI.item.mask			= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		TI.item.iImage			= 0;
		TI.item.iSelectedImage	= 1;
		TI.item.pszText			= pSequenceSLC->m_KeyName;
		m_TreeCtrl.InsertItem(&TI);
	}

	UpdateData(FALSE);
}

// CPaletteShoreLine 메시지 처리기입니다.
/*
void CPaletteShoreLine::OnBnClickedAutoGenShoreline()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
*/
BOOL CPaletteShoreLine::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CPaletteShoreLine::OnTvnSelchangedScriptTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView	= reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	m_hSelect					= pNMTreeView->itemNew.hItem;
	CString StrObjName			= m_TreeCtrl.GetItemText(m_hSelect);

	if(m_hRoot[0] == m_hSelect || m_hRoot[1] == m_hSelect || m_hRoot[2] == m_hSelect)
	{
		return;
	}

	if(m_TreeCtrl.ItemHasChildren(m_hSelect))
	{
		return;
	}

	HTREEITEM				hParentItem = m_TreeCtrl.GetParentItem(m_hSelect);
	CString					StrObjPName	= m_TreeCtrl.GetItemText(hParentItem);
	CNtlPLShoreLineProp*	pProp		= static_cast<CNtlPLShoreLineProp*>(CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_SHORELINE, NTL_PLEN_SHORELINE));
	string					strName		= StrObjName.GetBuffer(0); 

	if(!StrObjPName.Compare("UV Single"))
	{
		pProp->SetCurProp(eST_UV_SINGLE, strName);
	}
	else if(!StrObjPName.Compare("UV Dual"))
	{
		pProp->SetCurProp(eST_UV_DUAL, strName);
	}
	else if(!StrObjPName.Compare("Sequence"))
	{
		pProp->SetCurProp(eST_SEQUENCE, strName);
	}

	*pResult = 0;
}

LRESULT CPaletteShoreLine::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
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

VOID CPaletteShoreLine::Reset()
{
	string strDummy;
	static_cast<CNtlPLShoreLineProp*>(CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_SHORELINE, NTL_PLEN_SHORELINE))->SetCurProp(eST_NOTHING, strDummy);
}