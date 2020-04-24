// AvatarPropertyDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Venus.h"
#include "VenusFrame.h"
#include "AvatarPropertyDialog.h"
#include ".\avatarpropertydialog.h"

#include "EventLayer.h"

#include "VenusPropertyContainer.h"
#include ".\venusplayereventmanager.h"

#include "VenusConfig.h"
#include "StringManager.h"


// CAvatarPropertyDialog 대화 상자입니다.

IMPLEMENT_DYNCREATE(CAvatarPropertyDialog, CDialog)
CAvatarPropertyDialog::CAvatarPropertyDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CAvatarPropertyDialog::IDD, pParent)
	, m_strCurrentTime(_T(""))
	, m_strMaxTime(_T(""))
	, m_strBallSpeed(_T(""))
	, m_strTargetDistance(_T(""))
{
	m_Initialized = FALSE;
	m_nCurrentPoint = 0;
}

CAvatarPropertyDialog::~CAvatarPropertyDialog()
{
	m_PropertyTree.DeleteAllItems();
}

void CAvatarPropertyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ANIMATION_TIME, m_AnimationSlider);
	DDX_Text(pDX, IDC_CURRENT_TIME, m_strCurrentTime);
	DDX_Text(pDX, IDC_LAST_TIME, m_strMaxTime);
	DDX_Text(pDX, IDC_BALL_SPEED, m_strBallSpeed);
	DDX_Text(pDX, IDC_BALL_LIFETIME, m_strTargetDistance);
}


BEGIN_MESSAGE_MAP(CAvatarPropertyDialog, CDialog)
	ON_WM_SIZE()
	ON_NOTIFY(PTN_ITEMCHANGED, IDC_PROPERTY_STATIC, OnItemChanged)
	ON_WM_TIMER()
	ON_EN_KILLFOCUS(IDC_BALL_LIFETIME, OnEnKillfocusBallLifetime)
	ON_EN_KILLFOCUS(IDC_BALL_SPEED, OnEnKillfocusBallSpeed)
	ON_EN_CHANGE(IDC_BALL_LIFETIME, OnEnChangeBallLifetime)
	ON_EN_CHANGE(IDC_BALL_SPEED, OnEnChangeBallSpeed)
END_MESSAGE_MAP()


// CAvatarPropertyDialog 메시지 처리기입니다.

BOOL CAvatarPropertyDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_AnimationSlider.SetRange(1, 50);	
	m_AnimationSlider.SetTic(10);
	m_AnimationSlider.SetTicFreq(5);
	m_AnimationSlider.SetPos(0);


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

	BuildData();


	// Create CPropTree control
	m_PropertyTree.Create(dwStyle, rc, this, IDC_PROPERTY_STATIC);

	CPropTreeItem* pRoot;
	pRoot = m_PropertyTree.InsertItem(new CPropTreeItem());
	pRoot->SetLabelText(_T("No selected"));

	m_Initialized = TRUE;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CAvatarPropertyDialog::OnSize(UINT nType, int cx, int cy)
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

BOOL CAvatarPropertyDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN) 
	{
		if(pMsg->wParam == VK_RETURN)
		{
			if (this->GetFocus()->GetSafeHwnd() == GetDlgItem(IDC_BALL_SPEED)->GetSafeHwnd())
			{
				CVenusConfig::GetInstance().m_fEventBallSpeed = (float)atof(m_strBallSpeed);
			}
			else if (this->GetFocus()->GetSafeHwnd() == GetDlgItem(IDC_BALL_LIFETIME)->GetSafeHwnd())
			{
                RwReal fDistance = (RwReal)atof(m_strTargetDistance);
                RwReal fSpeed = (RwReal)atof(m_strBallSpeed);
                CVenusConfig::GetInstance().m_fEventBallLifeTime = fDistance / fSpeed;				
			}
		}
		if(pMsg->wParam == VK_RETURN) return FALSE;	
		if(pMsg->wParam == VK_ESCAPE) return FALSE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CAvatarPropertyDialog::OnTimer(UINT nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	KillTimer(nIDEvent);

	switch(nIDEvent)
	{
	case TIME_TYPE_RESET_PROPERTIES:
		{
			ResetProperties(m_nCurrentPoint);
		}
		break;
	case TIME_TYPE_EVENT_NAME:
		{
			if (CVenusPlayerEventManager::GetInstance().m_pSelectPlayerEvent != NULL)
			{
				CEventLayer* pView = (CEventLayer*)gGetEditLayer(EDIT_LAYER_EVENT);
				pView->ResetTreeItemName(CVenusPlayerEventManager::GetInstance().m_pSelectPlayerEvent->GetName());
			}
		}
		break;
	}

	CDialog::OnTimer(nIDEvent);
}


//------------------------------------------------------------------
//	FuncName	: SetProperties
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CAvatarPropertyDialog::ResetProperties(RwUInt32 nCurrentPoint)
{
	m_nCurrentPoint = nCurrentPoint;

	CVenusPlayer* pAvatar = NULL;
	if (CVenusPlayerEventManager::GetInstance().m_pSelectPlayerEvent != NULL)
	{
		pAvatar = &CVenusPlayerEventManager::GetInstance().m_pSelectPlayerEvent->m_Avatar;
	}

	m_PropertyTree.DeleteAllItems();
	if (pAvatar == NULL) return;


	CPropTreeItem* pRoot = m_PropertyTree.InsertItem(new CPropTreeItem());
	pRoot->Expand();
	pRoot->SetLabelText("Avatar Properties");

	// ============================================
	// Name
	// ============================================
	CPropTreeItemEdit* pEdit;
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);	
	pEdit->SetLabelText(_T("Name"));
	pEdit->SetInfoText(_T("Player Event Name"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatText);
	pEdit->SetItemValue((LPARAM)CVenusPlayerEventManager::GetInstance().m_pSelectPlayerEvent->GetName());
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EVENT_NAME);

    
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
			if (_stricmp(strFileName.c_str(), pAvatar->GetClumpName().c_str()) == 0)
			{
				pCombo->SetItemValue(index);
			}
		}
	}
	pCombo->SetCtrlID(_PROPERTY_CONTROL_CLUMP_NAME);

	// ============================================
	// Animation Name
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
		if (!pAvatar->GetClumpName().empty())
		{
			strcpy(strClumpName, pAvatar->GetClumpName().c_str());
			strClumpName[pAvatar->GetClumpName().size() - 4] = '\0';

			CNtlResourceFileManager::smdef_Entity::iterator it;
			for (it = CVenusPlayerEventManager::GetInstance().m_AnimationFileManager.m_smEntity.begin(); it != CVenusPlayerEventManager::GetInstance().m_AnimationFileManager.m_smEntity.end(); ++ it)
			{
				std::string strFileName = (*it).first.c_str();
				if (strstr(strFileName.c_str(), strClumpName) != NULL)
				{
					index = pCombo->AddString(_T(strFileName.c_str()));
					pCombo->SetItemData(index, (DWORD_PTR)(*it).first.c_str());
					if (_stricmp(strFileName.c_str(), pAvatar->GetAnimationName().c_str()) == 0)
					{
						pCombo->SetItemValue(index);
					}
				}
			}
		}
	}
	pCombo->SetCtrlID(_PROPERTY_CONTROL_ANIMATION_NAME);

	// ============================================
	// Num Count
	// ============================================
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatNumber);
	pEdit->SetLabelText(_T("Event Count"));
	pEdit->SetInfoText(_T("Event Count"));
	pEdit->SetItemValue((LPARAM)pAvatar->GetEventVector().size());
	pEdit->SetCtrlID(_PROPERTY_CONTROL_NUM_COUNT);

	// ============================================
	// Current Index
	// ============================================
	CString strPropertiesTitle;
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pRoot);	
	pCombo->SetLabelText(_T("Current Index"));
	pCombo->SetInfoText(_T("Current Index"));
	pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);
	for (RwInt32 i = 0; i < (RwInt32)pAvatar->GetEventVector().size(); ++i)
	{
		strPropertiesTitle.Format(_T("List %02d"), i);
		pCombo->AddString(_T(strPropertiesTitle));
		pCombo->SetItemData(i, i);
	}
	pCombo->SetItemValue(m_nCurrentPoint);
	pCombo->SetCtrlID(_PROPERTY_CONTROL_CURRENT_INDEX);
	NTL_ASSERTE(m_nCurrentPoint < (RwInt32)pAvatar->GetEventVector().size());


	// ============================================
	// Effect Name
	// ============================================
	CPropTreeItem* pListRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pListRoot->Expand();
	pListRoot->SetLabelText(_T("Event List"));

	CVenusPlayer::EVENT* pEvent	= pAvatar->GetEvent(m_nCurrentPoint);
	// ============================================
	// Effect Name
	// ============================================
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pListRoot);	
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
		if (_stricmp(pProperty->GetName(), pEvent->strEffectName.c_str()) == 0)
		{
			pCombo->SetItemValue(index);
		}
	}
	pCombo->SetCtrlID(_PROPERTY_CONTROL_EFFECT_NAME);    

	// ============================================
	// Bone Name
	// ============================================
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pListRoot);	
	pCombo->SetLabelText(_T("Bone"));
	pCombo->SetInfoText(_T("Bone Name"));
	pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);
	pCombo->SetDropDownHeight(300);

	index = pCombo->AddString(_T("NONE"));
	pCombo->SetItemData(index, 0);
	pCombo->SetItemValue(0);

	RwInt32 nNumNodes = pAvatar->GetNumNode();
	for (RwInt32 i = 0; i < nNumNodes; ++ i)
	{
		CString strName = pAvatar->GetBoneName(i);				
        index = pCombo->AddString(strName);
		pCombo->SetItemData(index, index);
		if (i == pEvent->nBoneIndex)
		{
			pCombo->SetItemValue(index);
		}
	}
	pCombo->SetCtrlID(_PROPERTY_CONTROL_BONE_INDEX);
    

	// ============================================
	// Fade In 
	// ============================================
	CPropTreeItem* pFadeInRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pListRoot);		
	pFadeInRoot->Expand();
	pFadeInRoot->SetLabelText(_T("Fade In"));

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pFadeInRoot);	
	pEdit->SetLabelText(_T("Start"));
	pEdit->SetInfoText(_T("Effect Start Time"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pEvent->fFadeInTime);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_FADE_IN_TIME);

	// ============================================
	// Fade Out
	// ============================================
	CPropTreeItem* pFadeOutRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pListRoot);		
	pFadeOutRoot->Expand();
	pFadeOutRoot->SetLabelText(_T("Fade Out"));

	// ============================================
	// Fade Out Type
	// ============================================
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pFadeOutRoot);	
	pCombo->SetLabelText(_T("Type"));
	pCombo->SetInfoText(_T("Fade Out Type"));
	pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);
	pCombo->SetDropDownHeight(300);

	RwChar* strFadeOutType[] = { "NONE", "DELETE", "STOP", "BALL", '\0' };

	for (RwInt32 i = 0; i < 4; ++ i)
	{
		index = pCombo->AddString(_T(strFadeOutType[i]));
		pCombo->SetItemData(index, index);
		if (i == pEvent->nFadeOutType)
		{
			pCombo->SetItemValue(index);
		}
	}
	pCombo->SetCtrlID(_PROPERTY_CONTROL_FADE_OUT_TYPE);

	// ============================================
	// Fade Out Time
	// ============================================
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pFadeOutRoot);	
	pEdit->SetLabelText(_T("End"));
	pEdit->SetInfoText(_T("Effect End Time"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pEvent->fFadeOutTime);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_FADE_OUT_TIME);

    
}

void CAvatarPropertyDialog::OnItemChanged(NMHDR* pNotifyStruct, LRESULT* plResult)
{
	LPNMPROPTREE pNMPropTree = (LPNMPROPTREE)pNotifyStruct;
	
	if (pNMPropTree->pItem)
	{
		ApplyProperties(pNMPropTree->pItem);
		CVenusPlayerEventManager::GetInstance().SetSelectPlayerEvent(CVenusPlayerEventManager::GetInstance().m_pSelectPlayerEvent);
	}
	*plResult = 0;
}

void CAvatarPropertyDialog::ApplyProperties(CPropTreeItem* pItem)
{
	CVenusPlayer* pAvatar = NULL;
	if (CVenusPlayerEventManager::GetInstance().m_pSelectPlayerEvent != NULL)
	{
		pAvatar = &CVenusPlayerEventManager::GetInstance().m_pSelectPlayerEvent->m_Avatar;
	}
	if (pAvatar == NULL) return;

	CVenusPlayer::EVENT* pEvent	= pAvatar->GetEvent(m_nCurrentPoint);

	switch(pItem->GetCtrlID()) 
	{
	case _PROPERTY_CONTROL_EVENT_NAME:
		{
			std::string strName = (RwChar*)pItem->GetItemValue();
			if (strName.empty())
			{
				AfxMessageBox(CStringManager::GetInstance().GetString(STRING_NAME_ERROR), MB_OK);
				SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
				return;
			}

			CVenusPlayerEvent* pSrcPlayerEvent = (CVenusPlayerEvent*)CVenusPlayerEventManager::GetInstance().GetPlayerEvent(strName.c_str());
			if (pSrcPlayerEvent != CVenusPlayerEventManager::GetInstance().m_pSelectPlayerEvent && pSrcPlayerEvent)
			{
				AfxMessageBox(CStringManager::GetInstance().GetString(STRING_NAME_ERROR), MB_OK);
				SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
				return;
			}

			CVenusPlayerEventManager::GetInstance().m_pSelectPlayerEvent->SetName(strName.c_str());

			CEventLayer* pView = (CEventLayer*)gGetEditLayer(EDIT_LAYER_EVENT);

            // TODO: 이름을 변경할시에 뻑이나서 막아둔다. 막으면 데이터는 변경은 문제없지만 (위에서 처리한다.)
            // List Ctrl의 이름은 변경되지 않는 문제가 있다. 아직 원인을 파악하지 못했다. (2006.08.21 by agebreak)
			//pView->ResetTreeItemName(CVenusPlayerEventManager::GetInstance().m_pSelectPlayerEvent->GetName());
//
//			SetTimer(TIME_TYPE_EVENT_NAME, 100, NULL);
		}
		break;
	case _PROPERTY_CONTROL_CLUMP_NAME:
		{
			if (pItem->GetItemValue() != NULL)
			{
				std::string strFileName = (RwChar*)pItem->GetItemValue();
				NTL_ASSERTE(CVenusPlayerEventManager::GetInstance().m_ClumpFileManager.IsFile(strFileName.c_str()));				
                pAvatar->SetClumpName(strFileName);
			}
			else
			{
                std::string strTemp = "";
                pAvatar->SetClumpName(strTemp);
			}

			// Clump가 변경되면 m_nCurrentNode를 초기화 해 준다
			pAvatar->GetAnimationName().clear();
			pEvent->nBoneIndex = -1;
			SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
		}
		break;
	case _PROPERTY_CONTROL_ANIMATION_NAME:
		{
			if (pItem->GetItemValue() != NULL)
			{
				std::string strFileName = (RwChar*)pItem->GetItemValue();
				NTL_ASSERTE(CVenusPlayerEventManager::GetInstance().m_AnimationFileManager.IsFile(strFileName.c_str()));				
                pAvatar->SetAnimationName(strFileName);
			}
			else
			{
                std::string strTemp = "";
                pAvatar->SetAnimationName(strTemp);
			}

			// 애니메이션이 변경되면 m_nCurrentNode를 초기화 해 준다
			pEvent->nBoneIndex = -1;
			SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
		}
		break;
	case _PROPERTY_CONTROL_NUM_COUNT:
		{
			RwInt32 nNumCount = (RwInt32)pItem->GetItemValue();
			if (nNumCount <= 0)
			{
				SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
				break;
			}

			if (nNumCount > (RwInt32)pAvatar->GetEventVector().size())
			{
				RwInt32 nPlus = nNumCount - (RwInt32)pAvatar->GetEventVector().size();
				for (RwInt32 i = 0; i < nPlus; ++ i)
				{
					CVenusPlayer::EVENT* pEvent = NTL_NEW CVenusPlayer::EVENT;
					pAvatar->InitializeEvent(pEvent);
					pAvatar->GetEventVector().push_back(pEvent);
				}
			}
			else if (nNumCount < (RwInt32)pAvatar->GetEventVector().size())
			{
				CVenusPlayer::svdef_EVENT svTempEvent;
				CVenusPlayer::svdef_EVENT::iterator it;
				for (it = pAvatar->GetEventVector().begin(); it != pAvatar->GetEventVector().end(); ++ it)
				{
					svTempEvent.push_back(*it);
				}

				pAvatar->GetEventVector().clear();

				RwInt32 i = 0;
				for (it = svTempEvent.begin(); it != svTempEvent.end(); ++ it)
				{
					if (i < nNumCount)
					{
						pAvatar->GetEventVector().push_back(*it);
					}
					else
					{
						NTL_DELETE(*it);
					}
					++ i;
				}
				svTempEvent.clear();
			}

			if (m_nCurrentPoint >= (RwInt32)pAvatar->GetEventVector().size())
				m_nCurrentPoint = 0;

			SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
		}
		break;
	case _PROPERTY_CONTROL_CURRENT_INDEX:
		{
			m_nCurrentPoint = (RwInt32)pItem->GetItemValue();
			SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
		}
		break;
	case _PROPERTY_CONTROL_EFFECT_NAME:
		{
			if (pItem->GetItemValue() != NULL)
			{
				CNtlPLProperty* pProperty = (CNtlPLProperty*)pItem->GetItemValue();
				pEvent->strEffectName = pProperty->GetName();
			}
			else
			{
				pEvent->strEffectName.clear();
			}
		}
		break;
	case _PROPERTY_CONTROL_BONE_INDEX:
		{
			pEvent->nBoneIndex = (RwInt32)pItem->GetItemValue() - 1;
		}
		break;
	case _PROPERTY_CONTROL_FADE_IN_TIME:
		{
			pEvent->fFadeInTime = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_FADE_OUT_TYPE:
		{
			pEvent->nFadeOutType = (RwInt32)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_FADE_OUT_TIME:
		{
			pEvent->fFadeOutTime = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	}
}

void CAvatarPropertyDialog::ResetAnimationTime(RwReal fCurrentTime, RwUInt32 nMaxTime)
{
	m_AnimationSlider.SetRange(0, nMaxTime);	
	m_AnimationSlider.SetTicFreq(nMaxTime / 20);
	m_AnimationSlider.SetPos((RwUInt32)(fCurrentTime*1000.f));

	m_strCurrentTime.Format("%d", (RwUInt32)(fCurrentTime*1000.f));
	m_strMaxTime.Format("%d", nMaxTime);

	UpdateData(FALSE);
}
void CAvatarPropertyDialog::OnEnKillfocusBallLifetime()
{
    RwReal fDistance = (RwReal)atof(m_strTargetDistance);
    RwReal fSpeed = (RwReal)atof(m_strBallSpeed);
	CVenusConfig::GetInstance().m_fEventBallLifeTime = fDistance / fSpeed;
}

void CAvatarPropertyDialog::OnEnKillfocusBallSpeed()
{
	CVenusConfig::GetInstance().m_fEventBallSpeed = (float)atof(m_strBallSpeed);
}

void CAvatarPropertyDialog::OnEnChangeBallLifetime()
{
	UpdateData(TRUE);
}

void CAvatarPropertyDialog::OnEnChangeBallSpeed()
{
	UpdateData(TRUE);
}

void CAvatarPropertyDialog::BuildData()
{
	m_strBallSpeed.Format("%f", CVenusConfig::GetInstance().m_fEventBallSpeed);
	m_strTargetDistance.Format("%f", CVenusConfig::GetInstance().m_fEventBallSpeed * CVenusConfig::GetInstance().m_fEventBallLifeTime);
	UpdateData(FALSE);
}