// EnemyPropertyDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Venus.h"
#include "EnemyPropertyDialog.h"
#include ".\enemypropertydialog.h"

#include "VenusEventEnemy.h"
#include "VenusPlayerEventManager.h"
#include "VenusPropertyContainer.h"

// CEnemyPropertyDialog 대화 상자입니다.

IMPLEMENT_DYNCREATE(CEnemyPropertyDialog, CDialog)
CEnemyPropertyDialog::CEnemyPropertyDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CEnemyPropertyDialog::IDD, pParent)
{
	m_Initialized = FALSE;
}

CEnemyPropertyDialog::~CEnemyPropertyDialog()
{
}

void CEnemyPropertyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEnemyPropertyDialog, CDialog)
	ON_WM_SIZE()
	ON_NOTIFY(PTN_ITEMCHANGED, IDC_PROPERTY_STATIC, OnItemChanged)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CEnemyPropertyDialog 메시지 처리기입니다.

BOOL CEnemyPropertyDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	DWORD dwStyle;
	CRect rc;
	dwStyle = WS_CHILD|WS_VISIBLE|PTS_NOTIFY;

	// Init the control's size to cover the entire client area
	CWnd* pPropertyStaticWindow = GetDlgItem(IDC_PROPERTY_STATIC);
	if( pPropertyStaticWindow )
	{
		pPropertyStaticWindow->GetWindowRect(rc);
		this->ScreenToClient( &rc );
	}

	// Create CPropTree control
	m_PropertyTree.Create(dwStyle, rc, this, IDC_PROPERTY_STATIC);

	CPropTreeItem* pRoot;
	pRoot = m_PropertyTree.InsertItem(new CPropTreeItem());
	pRoot->SetLabelText(_T("No selected"));

	ResetProperties();

	m_Initialized = TRUE;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CEnemyPropertyDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(m_Initialized)
	{
		CWnd* pPropertyStaticWindow = GetDlgItem(IDC_PROPERTY_STATIC);
		if(pPropertyStaticWindow)
		{			
			CRect rc;	
			pPropertyStaticWindow->GetWindowRect(rc);
			this->ScreenToClient( &rc );
			rc.right = rc.left + cx - 10;			
			pPropertyStaticWindow->MoveWindow( rc.left, rc.top, cx-2,rc.bottom-rc.top );

			if (::IsWindow(m_PropertyTree.GetSafeHwnd()))
				m_PropertyTree.MoveWindow( &rc );		
		}
	}
}

void CEnemyPropertyDialog::OnTimer(UINT nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	KillTimer(nIDEvent);

	switch(nIDEvent)
	{
	case TIME_TYPE_RESET_PROPERTIES:
		{
			ResetProperties();
		}
		break;

	}
	CDialog::OnTimer(nIDEvent);
}

BOOL CEnemyPropertyDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN) 
	{
		if(pMsg->wParam == VK_RETURN) return FALSE;	
		if(pMsg->wParam == VK_ESCAPE) return FALSE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}


//------------------------------------------------------------------
//	FuncName	: SetProperties
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CEnemyPropertyDialog::ResetProperties(RwUInt32 nCurrentPoint)
{
	CVenusEventEnemy* pEnemy = &CVenusPlayerEventManager::GetInstance().m_EventEmemy;

	m_PropertyTree.DeleteAllItems();

	CPropTreeItem* pRoot = m_PropertyTree.InsertItem(new CPropTreeItem());
	pRoot->Expand();
	pRoot->SetLabelText("Enemy Properties");

	// ============================================
	// Clump Name
	// ============================================
	CPropTreeItemCombo* pCombo;
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pRoot);	
	pCombo->SetLabelText(_T("Clump"));
	pCombo->SetInfoText(_T("Clump Name"));
	pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);
	pCombo->SetDropDownHeight(300);

	RwInt32 index = 0;
	index = pCombo->AddString(_T("NONE"));
	pCombo->SetItemData(index, NULL);
	pCombo->SetItemValue(0);

	{
		CNtlResourceFileManager::smdef_Entity::iterator it;
		for (it = CVenusPlayerEventManager::GetInstance().m_ClumpFileManager.m_smEntity.begin(); it != CVenusPlayerEventManager::GetInstance().m_ClumpFileManager.m_smEntity.end(); ++ it)
		{
			std::string strFileName = (*it).first.c_str();
			index = pCombo->AddString(_T(strFileName.c_str()));
			pCombo->SetItemData(index, (DWORD_PTR)(*it).first.c_str());
			if (_stricmp(strFileName.c_str(), pEnemy->m_strClumpName.c_str()) == 0)
			{
				pCombo->SetItemValue(index);
			}
		}
	}
	pCombo->SetCtrlID(_PROPERTY_CONTROL_CLUMP_NAME);

	// ============================================
	// Idle Animation Name
	// ============================================
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pRoot);	
	pCombo->SetLabelText(_T("Animation"));
	pCombo->SetInfoText(_T("Animation Name"));
	pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);
	pCombo->SetDropDownHeight(300);

	index = 0;
	index = pCombo->AddString(_T("NONE"));
	pCombo->SetItemData(index, NULL);
	pCombo->SetItemValue(0);

	{
		RwChar strClumpName[MAX_PATH];
		memset(strClumpName, 0, MAX_PATH);
		if (!pEnemy->m_strClumpName.empty())
		{
			strcpy(strClumpName, pEnemy->m_strClumpName.c_str());
			strClumpName[pEnemy->m_strClumpName.size() - 4] = '\0';

			CNtlResourceFileManager::smdef_Entity::iterator it;
			for (it = CVenusPlayerEventManager::GetInstance().m_AnimationFileManager.m_smEntity.begin(); it != CVenusPlayerEventManager::GetInstance().m_AnimationFileManager.m_smEntity.end(); ++ it)
			{
				std::string strFileName = (*it).first.c_str();
				if (strstr(strFileName.c_str(), strClumpName) != NULL)
				{
					index = pCombo->AddString(_T(strFileName.c_str()));
					pCombo->SetItemData(index, (DWORD_PTR)(*it).first.c_str());
					if (_stricmp(strFileName.c_str(), pEnemy->m_strIdleAnimationName.c_str()) == 0)
					{
						pCombo->SetItemValue(index);
					}
				}
			}
		}
	}
	pCombo->SetCtrlID(_PROPERTY_CONTROL_IDLE_ANIMATION_NAME);

	// ============================================
	// Damage Animation Name
	// ============================================
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pRoot);	
	pCombo->SetLabelText(_T("Animation"));
	pCombo->SetInfoText(_T("Animation Name"));
	pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);
	pCombo->SetDropDownHeight(300);

	index = 0;
	index = pCombo->AddString(_T("NONE"));
	pCombo->SetItemData(index, NULL);
	pCombo->SetItemValue(0);

	{
		RwChar strClumpName[MAX_PATH];
		memset(strClumpName, 0, MAX_PATH);
		if (!pEnemy->m_strClumpName.empty())
		{
			strcpy(strClumpName, pEnemy->m_strClumpName.c_str());
			strClumpName[pEnemy->m_strClumpName.size() - 4] = '\0';

			CNtlResourceFileManager::smdef_Entity::iterator it;
			for (it = CVenusPlayerEventManager::GetInstance().m_AnimationFileManager.m_smEntity.begin(); it != CVenusPlayerEventManager::GetInstance().m_AnimationFileManager.m_smEntity.end(); ++ it)
			{
				std::string strFileName = (*it).first.c_str();
				if (strstr(strFileName.c_str(), strClumpName) != NULL)
				{
					index = pCombo->AddString(_T(strFileName.c_str()));
					pCombo->SetItemData(index, (DWORD_PTR)(*it).first.c_str());
					if (_stricmp(strFileName.c_str(), pEnemy->m_strDamageAnimationName.c_str()) == 0)
					{
						pCombo->SetItemValue(index);
					}
				}
			}
		}
	}
	pCombo->SetCtrlID(_PROPERTY_CONTROL_DAMAGE_ANIMATION_NAME);

	// ============================================
	// Effect Name
	// ============================================
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pRoot);	
	pCombo->SetLabelText(_T("Effect"));
	pCombo->SetInfoText(_T("Effect Name"));
	pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST|CBS_SORT);
	pCombo->SetDropDownHeight(300);

	index = pCombo->AddString(_T("_NONE"));
	pCombo->SetItemData(index, NULL);
	pCombo->SetItemValue(0);

	CVenusPropertyContainer::svdef_Property::iterator it;
	for (it = CVenusPropertyContainer::GetInstance().m_svEffectProperty.begin(); it != CVenusPropertyContainer::GetInstance().m_svEffectProperty.end(); ++ it)
	{
		CNtlPLProperty* pProperty = (*it);
		index = pCombo->AddString(_T(pProperty->GetName()));
		pCombo->SetItemData(index, (DWORD_PTR)pProperty);
		if (_stricmp(pProperty->GetName(), pEnemy->m_strEffectName.c_str()) == 0)
		{
			pCombo->SetItemValue(index);
		}
	}
	pCombo->SetCtrlID(_PROPERTY_CONTROL_EFFECT_NAME);
}


void CEnemyPropertyDialog::OnItemChanged(NMHDR* pNotifyStruct, LRESULT* plResult)
{
	LPNMPROPTREE pNMPropTree = (LPNMPROPTREE)pNotifyStruct;
	
	if (pNMPropTree->pItem)
	{
		ApplyProperties(pNMPropTree->pItem);
		CVenusEventEnemy* pEnemy = &CVenusPlayerEventManager::GetInstance().m_EventEmemy;
		pEnemy->CreateResource();
	}
	*plResult = 0;
}

void CEnemyPropertyDialog::ApplyProperties(CPropTreeItem* pItem)
{
	CVenusEventEnemy* pEnemy = &CVenusPlayerEventManager::GetInstance().m_EventEmemy;
	switch(pItem->GetCtrlID()) 
	{
	case _PROPERTY_CONTROL_CLUMP_NAME:
		{
			if (pItem->GetItemValue() != NULL)
			{
				std::string strFileName = (RwChar*)pItem->GetItemValue();
				NTL_ASSERTE(CVenusPlayerEventManager::GetInstance().m_ClumpFileManager.IsFile(strFileName.c_str()));
				pEnemy->m_strClumpName = strFileName.c_str();
			}
			else
			{
				pEnemy->m_strClumpName.clear();
			}
			// Clump가 변경되면 m_nCurrentNode를 초기화 해 준다
			pEnemy->m_strIdleAnimationName.clear();
			pEnemy->m_strDamageAnimationName.clear();
			SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
		}
		break;
	case _PROPERTY_CONTROL_IDLE_ANIMATION_NAME:
		{
			if (pItem->GetItemValue() != NULL)
			{
				std::string strFileName = (RwChar*)pItem->GetItemValue();
				NTL_ASSERTE(CVenusPlayerEventManager::GetInstance().m_AnimationFileManager.IsFile(strFileName.c_str()));
				pEnemy->m_strIdleAnimationName = strFileName.c_str();
			}
			else
			{
				pEnemy->m_strIdleAnimationName.clear();
			}
		}
		break;
	case _PROPERTY_CONTROL_DAMAGE_ANIMATION_NAME:
		{
			if (pItem->GetItemValue() != NULL)
			{
				std::string strFileName = (RwChar*)pItem->GetItemValue();
				NTL_ASSERTE(CVenusPlayerEventManager::GetInstance().m_AnimationFileManager.IsFile(strFileName.c_str()));
				pEnemy->m_strDamageAnimationName = strFileName.c_str();
			}
			else
			{
				pEnemy->m_strDamageAnimationName.clear();
			}
		}
		break;
	case _PROPERTY_CONTROL_EFFECT_NAME:
		{
			if (pItem->GetItemValue() != NULL)
			{
				CNtlPLProperty* pProperty = (CNtlPLProperty*)pItem->GetItemValue();
				pEnemy->m_strEffectName = pProperty->GetName();
			}
			else
			{
				pEnemy->m_strEffectName.clear();
			}
		}
		break;
	}
}