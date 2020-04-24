// AnimAssetAnimSetDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CharacterEditor.h"
#include ".\animationsetdialog.h"
#include "NewAssetOpen.h"
#include "NtlPLResourceManager.h"

// CAnimAssetAnimSetDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAnimationSetDialog, CDialog)
CAnimationSetDialog::CAnimationSetDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CAnimationSetDialog::IDD, pParent)
{
}

CAnimationSetDialog::~CAnimationSetDialog()
{
}

void CAnimationSetDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ANIM_PROPERTY_LIST, m_propertyControl);
}


BEGIN_MESSAGE_MAP(CAnimationSetDialog, CDialog)
END_MESSAGE_MAP()


// CAnimAssetAnimSetDlg 메시지 처리기입니다.

BOOL CAnimationSetDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	theApp.m_pAnimationSetDialog = this;

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CAnimationSetDialog::UpdateTree()
{
	m_propertyAnimDataManager.SetData(&m_propertyAnimData);
	m_propertyAnimDataManager.AttachCallBack(this);
	m_propertyControl.SetPropertyItemManager(&m_propertyAnimDataManager);	
}

void CAnimationSetDialog::SetAnimDataInfoChange(SToolAnimData *pToolAnimData)
{
	m_propertyControl.ResetContent();
	m_propertyAnimDataManager.UpdateData(pToolAnimData);
	
	m_propertyAnimDataManager.SetData(&m_propertyAnimData);
	m_propertyAnimDataManager.AttachCallBack(this);
	m_propertyControl.SetPropertyItemManager(&m_propertyAnimDataManager);
	theApp.m_DboApplication.SetAllChannelChangeAnimation(pToolAnimData->uiAnimKey);
}

void CAnimationSetDialog::SetAnimationFileOpen(SToolAnimData *pToolAnimData)
{
	char curDirectory[256];
	GetCurrentDirectory(strlen(curDirectory), curDirectory);

	CNewAssetOpen dlg(TRUE);
	if(dlg.DoModal() != IDOK)
		return;
	else
	{
		SetCurrentDirectory(curDirectory);
		if(pToolAnimData != NULL)
		{
			pToolAnimData->pAnimData->strAnimFileName = dlg.GetFileName();
			
			if(pToolAnimData->pAnimData->pAnimRes)
			{
				CNtlPLResourceManager::GetInstance()->UnLoad(pToolAnimData->pAnimData->pAnimRes);
				pToolAnimData->pAnimData->pAnimRes = NULL;

			}
			SetAnimDataInfoChange(pToolAnimData);

			//theApp.m_DboApplication.SetAllChannelChangeAnimation(pToolAnimData->uiAnimKey);
		}
	}
}
