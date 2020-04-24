// TextureListDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Venus.h"
#include "VenusFrame.h"

#include "TextureListDialog.h"

#include "TextureLayer.h"

#include "rwcore.h"

#include "NtlEffectSystemFreeList.h"
#include ".\texturelistdialog.h"

// CTextureListDialog 대화 상자입니다.

IMPLEMENT_DYNCREATE(CTextureListDialog, CDialog)
CTextureListDialog::CTextureListDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTextureListDialog::IDD, pParent)
{
	m_Initialized = FALSE;
}

CTextureListDialog::~CTextureListDialog()
{
}

void CTextureListDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXTURE_LIST, m_TextureList);
}


BEGIN_MESSAGE_MAP(CTextureListDialog, CDialog)
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TEXTURE_LIST, OnTvnSelchangedTextureList)
END_MESSAGE_MAP()


// CTextureListDialog 메시지 처리기입니다.
BOOL CTextureListDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_TextureList.InitTreeCtrl();

	m_Initialized = TRUE;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CTextureListDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_Initialized)
	{
	}
}

BOOL CTextureListDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN) 
	{
		if(pMsg->wParam == VK_RETURN) return FALSE;	
		if(pMsg->wParam == VK_ESCAPE) return FALSE;	
        if(pMsg->wParam == VK_F5) 
        {
            OnRefreshList();
        }             
	}	
	return CDialog::PreTranslateMessage(pMsg);
}

//------------------------------------------------------------------
//	FuncName	: SerializeList
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CTextureListDialog::SerializeList()
{
	CNtlResourceFileManager::svdef_String::iterator it;
	for (it = CNtlEffectSystemFreeList::m_TextureFileManager.m_svExt.begin(); it != CNtlEffectSystemFreeList::m_TextureFileManager.m_svExt.end(); ++ it)
	{
		m_TextureList.AddExt((*it).c_str());
	}
	m_TextureList.SerializeList(CNtlEffectSystemFreeList::m_TextureFileManager.m_strPath.c_str());
}

void CTextureListDialog::OnTvnSelchangedTextureList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HTREEITEM hItem = m_TextureList.GetSelectedItem();
	if(hItem == NULL)
	{
		CTextureLayer* pLayer = (CTextureLayer*)gGetEditLayer(EDIT_LAYER_TEXTURE);
		pLayer->CreateBitmap(NULL);
		return;
	}

	CString strFileName = m_TextureList.GetItemText(hItem);
	const RwChar* strPathName = CNtlEffectSystemFreeList::m_TextureFileManager.GetPathName(strFileName.GetBuffer(0));
	if (strPathName != NULL)
	{
		CTextureLayer* pLayer = (CTextureLayer*)gGetEditLayer(EDIT_LAYER_TEXTURE);
		pLayer->CreateBitmap(strPathName);
	}
	else
	{
		CTextureLayer* pLayer = (CTextureLayer*)gGetEditLayer(EDIT_LAYER_TEXTURE);
		pLayer->CreateBitmap(NULL);
	}

	*pResult = 0;
}

void CTextureListDialog::OnRefreshList()
{
    m_TextureList.DeleteAllItems();

    SerializeList();
}