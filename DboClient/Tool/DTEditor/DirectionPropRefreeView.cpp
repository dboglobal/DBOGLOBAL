// DirectionPropRefreeView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DTEditor.h"

// presentation
#include "NtlPLCharacterParser.h"
#include "NtlCharacterXMLScript.h"

// simulation
#include "NtlSLCENode.h"

// DTEdit
#include "DirectionPropRefreeView.h"

namespace
{
	enum eGridItemID
	{
		GRID_ITEM_ANGLE,

		GRID_ITEM_POS_BUTTON,
		GRID_ITEM_POS_X,
		GRID_ITEM_POS_Y,
		GRID_ITEM_POS_Z,

		GRID_ITEM_MOVE_TYPE,
		GRID_ITEM_MOVE_MOVE_SPEED,
		GRID_ITEM_MOVE_BUTTON,
		GRID_ITEM_MOVE_X,
		GRID_ITEM_MOVE_Y,
		GRID_ITEM_MOVE_Z,
	};
};

// CDirectionPropRefreeView

IMPLEMENT_DYNCREATE(CDirectionPropRefreeView, CXTResizeFormView)

CDirectionPropRefreeView::CDirectionPropRefreeView()
: CXTResizeFormView(CDirectionPropRefreeView::IDD)
,m_eButtonInputType(BUTTON_INPUT_TYPE_NONE)
{

}

CDirectionPropRefreeView::~CDirectionPropRefreeView()
{
}

void CDirectionPropRefreeView::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REFREE, m_staticRefree);
	DDX_Control(pDX, IDC_LIST1, m_lcAnimationList);
	DDX_Control(pDX, IDC_LIST2, m_lcSelectedAnimationList);
	DDX_Control(pDX, IDC_COMBO1, m_ComboAnimPlayType);
	DDX_Control(pDX, IDC_LIST5, m_ListBoxNarrationList);
	DDX_Control(pDX, IDC_COMBO2, m_ComboBoxNarrationPlayType);
}

BEGIN_MESSAGE_MAP(CDirectionPropRefreeView, CXTResizeFormView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON1, &CDirectionPropRefreeView::OnBnClickedButton_Delete)
	ON_BN_CLICKED(IDC_BUTTON2, &CDirectionPropRefreeView::OnBnClickedButton_Up)
	ON_BN_CLICKED(IDC_BUTTON3, &CDirectionPropRefreeView::OnBnClickedButton_Down)
	ON_LBN_DBLCLK(IDC_LIST1, &CDirectionPropRefreeView::OnLbnDblclkList_AnimList)
	ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, OnGridNotify)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDirectionPropRefreeView::OnCbnSelchangeCombo1)
	ON_LBN_XT_NEWITEM(IDC_LIST5, OnListBoxNewItem_NarrationList)
	ON_LBN_XT_DELETEITEM(IDC_LIST5, OnListBoxDeleteItem_NarrationList)
	ON_LBN_XT_LABELEDITEND(IDC_LIST5, OnSelchangeEditList_Narration)
	ON_BN_CLICKED(IDC_BUTTON7, &CDirectionPropRefreeView::OnBnClickedButton_NarrationDelete)
	ON_BN_CLICKED(IDC_BUTTON8, &CDirectionPropRefreeView::OnBnClickedButton_NarrationUp)
	ON_BN_CLICKED(IDC_BUTTON9, &CDirectionPropRefreeView::OnBnClickedButton_NarrationDown)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDirectionPropRefreeView::OnCbnSelchangeCombo_NarrationPlayType)
	ON_LBN_SELCHANGE(IDC_LIST5, &CDirectionPropRefreeView::OnLbnSelchangeList_Narration)
END_MESSAGE_MAP()


// CDirectionPropRefreeView 진단입니다.

#ifdef _DEBUG
void CDirectionPropRefreeView::AssertValid() const
{
	CXTResizeFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDirectionPropRefreeView::Dump(CDumpContext& dc) const
{
	CXTResizeFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDirectionPropRefreeView 메시지 처리기입니다.

void CDirectionPropRefreeView::OnDestroy()
{
	CXTResizeFormView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	DestroyListItem();
}

int CDirectionPropRefreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if( m_PropGrid.Create(CRect(0, 0, 0, 0), this, IDS_STRING_REFREE) )
	{
		CString strTemp;

		m_PropGrid.SetVariableItemsHeight(TRUE);
		m_PropGrid.SetVariableItemsHeight(TRUE);        
		m_PropGrid.ShowToolBar(TRUE);
		m_PropGrid.ShowHelp(FALSE);
		m_PropGrid.HighlightChangedItems(TRUE);
		m_PropGrid.SetTheme(xtpGridThemeWhidbey);

		// Direction type
		m_pCategoryItem	= (CXTPPropertyGridItemCategory*)m_PropGrid.AddCategory("Direction Type");

		strTemp.LoadString(IDS_STRING_REFREE);
		m_pDirectionTypeItemEnum = (CXTPPropertyGridItemEnum*)(m_pCategoryItem->AddChildItem(new CXTPPropertyGridItemEnum(_T("Type"), 1)));
		m_pDirectionTypeItemEnum->GetConstraints()->AddConstraint(strTemp, 1);
		m_pDirectionTypeItemEnum->SetEnum(1);
		m_pDirectionTypeItemEnum->SetReadOnly(TRUE);

		m_pGridItemAngle		= (CXTPPropertyGridItemDouble*)(m_pCategoryItem->AddChildItem(new CXTPPropertyGridItemDouble(_T("Angle"), 0.f)));
		m_pGridItemAngle->SetID(GRID_ITEM_ANGLE);

		m_pCategoryItem->Expand();

		// Move		
		m_pCategoryItem	= (CXTPPropertyGridItemCategory*)m_PropGrid.AddCategory("Move");

		m_pGridItemMoveType = (CXTPPropertyGridItemEnum*)(m_pCategoryItem->AddChildItem(new CXTPPropertyGridItemEnum(_T("Type"), 1)));
		m_pGridItemMoveType->GetConstraints()->AddConstraint(_T("STOP"), 0);
		m_pGridItemMoveType->GetConstraints()->AddConstraint(_T("EDGE"), 1);
		m_pGridItemMoveType->GetConstraints()->AddConstraint(_T("TARGET"), 2);
		m_pGridItemMoveType->SetEnum(1);
		m_pGridItemMoveType->SetID(GRID_ITEM_MOVE_TYPE);

		m_pGridItemMoveSpeed	= (CXTPPropertyGridItemDouble*)(m_pCategoryItem->AddChildItem(new CXTPPropertyGridItemDouble(_T("Speed"), 0)));
		m_pGridItemMoveSpeed->SetID(GRID_ITEM_MOVE_MOVE_SPEED);		

		m_pGridItemPosButton = (CCustomItemButton*)m_pCategoryItem->AddChildItem(new CCustomItemButton(_T("Position"), TRUE, TRUE));
		m_pGridItemPosButton->SetID(GRID_ITEM_POS_BUTTON);

		m_pGridItemPosX	= (CXTPPropertyGridItemDouble*)(m_pCategoryItem->AddChildItem(new CXTPPropertyGridItemDouble(_T("X"), 0, _T("%.6f"))));
		m_pGridItemPosY	= (CXTPPropertyGridItemDouble*)(m_pCategoryItem->AddChildItem(new CXTPPropertyGridItemDouble(_T("Y"), 0, _T("%.6f"))));
		m_pGridItemPosZ	= (CXTPPropertyGridItemDouble*)(m_pCategoryItem->AddChildItem(new CXTPPropertyGridItemDouble(_T("Z"), 0, _T("%.6f"))));

		m_pGridItemPosX->SetID(GRID_ITEM_POS_X);
		m_pGridItemPosY->SetID(GRID_ITEM_POS_Y);
		m_pGridItemPosZ->SetID(GRID_ITEM_POS_Z);

		m_pGridItemTargetButton = (CCustomItemButton*)m_pCategoryItem->AddChildItem(new CCustomItemButton(_T("Target Postion"), TRUE, TRUE));
		m_pGridItemTargetButton->SetID(GRID_ITEM_MOVE_BUTTON);

		m_pGridItemTargetX	= (CXTPPropertyGridItemDouble*)(m_pCategoryItem->AddChildItem(new CXTPPropertyGridItemDouble(_T("X"), 0, _T("%.6f"))));
		m_pGridItemTargetY	= (CXTPPropertyGridItemDouble*)(m_pCategoryItem->AddChildItem(new CXTPPropertyGridItemDouble(_T("Y"), 0, _T("%.6f"))));
		m_pGridItemTargetZ	= (CXTPPropertyGridItemDouble*)(m_pCategoryItem->AddChildItem(new CXTPPropertyGridItemDouble(_T("Z"), 0, _T("%.6f"))));

		m_pGridItemTargetX->SetID(GRID_ITEM_MOVE_X);
		m_pGridItemTargetY->SetID(GRID_ITEM_MOVE_Y);
		m_pGridItemTargetZ->SetID(GRID_ITEM_MOVE_Z);	

		m_pCategoryItem->Expand();
	}	

	return 0;
}

void CDirectionPropRefreeView::OnInitialUpdate()
{
	CXTResizeFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	// Call SetListEditStyle to set the type of edit list. You can
	// pass in LBS_XT_NOTOOLBAR if you don't want the toolbar displayed.
	m_ListBoxNarrationList.SetListEditStyle(_T(" Search for Directories:"), LBS_XT_NOTOOLBAR);

	// initialize the edit list box.
	m_ListBoxNarrationList.Initialize();
}

void CDirectionPropRefreeView::OnSize(UINT nType, int cx, int cy)
{
	CXTResizeFormView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if( m_PropGrid.GetSafeHwnd() && m_staticRefree.GetSafeHwnd() )
	{
		RECT rc;

		m_staticRefree.GetWindowRect(&rc);
		ScreenToClient(&rc);
		m_PropGrid.MoveWindow(&rc);
	}
}

void CDirectionPropRefreeView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if( !bShow )
		m_eButtonInputType = BUTTON_INPUT_TYPE_NONE;
}

void CDirectionPropRefreeView::SetProperty(CNtlSLCENode* pNode)
{
	m_pCurRefreeData = (CNtlSLCENodeReferee*)pNode;

	m_iLastSelectedIndex_in_ListBoxNarrationList = LB_ERR;

	RwV3d v3Pos = m_pCurRefreeData->GetRefreePosition();

	m_pGridItemAngle->SetDouble(m_pCurRefreeData->GetAngleY());

	m_pGridItemPosX->SetDouble(v3Pos.x);
	m_pGridItemPosY->SetDouble(v3Pos.y);
	m_pGridItemPosZ->SetDouble(v3Pos.z);

	m_pGridItemMoveType->SetEnum( m_pCurRefreeData->GetMoveType() );
	m_pGridItemMoveSpeed->SetDouble( m_pCurRefreeData->GetMoveSpeed() );

	v3Pos = m_pCurRefreeData->GetTargetPos();

	m_pGridItemTargetX->SetDouble(v3Pos.x);
	m_pGridItemTargetY->SetDouble(v3Pos.y);
	m_pGridItemTargetZ->SetDouble(v3Pos.z);	


	SetAnimIDEnum();

	if( m_pCurRefreeData )
	{
		VEC_ANIM_ID vecAnim = m_pCurRefreeData->GetAnimList();
		for( RwInt32 i = 0 ; i < m_lcAnimationList.GetCount() ; ++i )
		{
			sANIM_DATA* pAnimData = (sANIM_DATA*)m_lcAnimationList.GetItemData(i);

			for each( RwUInt32 uiAnimID in vecAnim )
			{
				if( uiAnimID == pAnimData->uiAnimID )
				{
					m_lcSelectedAnimationList.AddString(pAnimData->strAnimName);
					m_lcSelectedAnimationList.SetItemData(m_lcSelectedAnimationList.GetCount() - 1, (DWORD_PTR)pAnimData);
					break;
				}
			}			
		}
	}

	m_ComboAnimPlayType.AddString(_T("Random"));
	m_ComboAnimPlayType.AddString(_T("Sequence"));
	m_ComboAnimPlayType.AddString(_T("Loop"));

	m_ComboAnimPlayType.SetCurSel( m_pCurRefreeData->GetAnimPlayType() );



	if( m_pCurRefreeData )
	{
		char acBuffer[24] = "";
		VEC_NARRATION_ID vecNarration = m_pCurRefreeData->GetNarrationList();
		for( RwUInt8 i = 0 ; i < vecNarration.size() ; ++i )
		{
			int iNarrationIndex = vecNarration[i];
			itoa(iNarrationIndex, acBuffer, 10);
			m_ListBoxNarrationList.AddString( acBuffer );
		}
	}

	m_ComboBoxNarrationPlayType.AddString(_T("Random"));
	m_ComboBoxNarrationPlayType.AddString(_T("Sequence"));

	m_ComboBoxNarrationPlayType.SetCurSel( m_pCurRefreeData->GetNarrationPlayType() );
}

LRESULT CDirectionPropRefreeView::OnGridNotify(WPARAM wParam, LPARAM lParam)
{
	if( !m_pCurRefreeData )
		return 0;

	if(wParam == XTP_PGN_ITEMVALUE_CHANGED)
	{
		CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam;

		switch( pItem->GetID() )
		{
		case GRID_ITEM_ANGLE:
			{
				RwReal fAngle = (float)m_pGridItemAngle->GetDouble();
				if( m_pCurRefreeData )
					m_pCurRefreeData->SetAngleY(fAngle);

				break;
			}
		case GRID_ITEM_POS_BUTTON:
			{
				m_eButtonInputType = BUTTON_INPUT_TYPE_POS;
				break;
			}
		case GRID_ITEM_POS_X:
			{
				RwV3d v3Pos = m_pCurRefreeData->GetRefreePosition();
				v3Pos.x = (float)m_pGridItemPosX->GetDouble();
				m_pCurRefreeData->SetRefreePosition(v3Pos);
				break;
			}
		case GRID_ITEM_POS_Y:
			{
				RwV3d v3Pos = m_pCurRefreeData->GetRefreePosition();
				v3Pos.y = (float)m_pGridItemPosY->GetDouble();
				m_pCurRefreeData->SetRefreePosition(v3Pos);
				break;
			}
		case GRID_ITEM_POS_Z:
			{
				RwV3d v3Pos = m_pCurRefreeData->GetRefreePosition();
				v3Pos.z = (float)m_pGridItemPosZ->GetDouble();
				m_pCurRefreeData->SetRefreePosition(v3Pos);
				break;
			}
		case GRID_ITEM_MOVE_TYPE:
			{
				m_pCurRefreeData->SetMoveType( (CNtlSLCENodeReferee::ERefreeMoveType)m_pGridItemMoveType->GetEnum() );
				break;
			}
		case GRID_ITEM_MOVE_MOVE_SPEED:
			{
				m_pCurRefreeData->SetMoveSpeed( (RwReal)m_pGridItemMoveSpeed->GetDouble() );
				break;
			}
		case GRID_ITEM_MOVE_BUTTON:
			{
				m_eButtonInputType = BUTTON_INPUT_TYPE_TARGET_POS;
				break;
			}
		case GRID_ITEM_MOVE_X:
			{
				RwV3d v3Pos = m_pCurRefreeData->GetTargetPos();
				v3Pos.x = (float)m_pGridItemTargetX->GetDouble();
				m_pCurRefreeData->SetTargetPos(v3Pos);
				break;
			}
		case GRID_ITEM_MOVE_Y:
			{
				RwV3d v3Pos = m_pCurRefreeData->GetTargetPos();
				v3Pos.y = (float)m_pGridItemTargetX->GetDouble();
				m_pCurRefreeData->SetTargetPos(v3Pos);
				break;
			}
		case GRID_ITEM_MOVE_Z:
			{
				RwV3d v3Pos = m_pCurRefreeData->GetTargetPos();
				v3Pos.z = (float)m_pGridItemTargetX->GetDouble();
				m_pCurRefreeData->SetTargetPos(v3Pos);
				break;
			}
		}
	}

	return 0;
}

void CDirectionPropRefreeView::SetPickInfo(_SWorldPickInfo* pPickInfo)
{
	if( m_eButtonInputType == BUTTON_INPUT_TYPE_NONE )
	{
		return;
	}
	else if( m_eButtonInputType == BUTTON_INPUT_TYPE_POS )
	{
		m_pGridItemPosX->SetDouble(pPickInfo->vPickPos.x);
		m_pGridItemPosY->SetDouble(pPickInfo->vPickPos.y);
		m_pGridItemPosZ->SetDouble(pPickInfo->vPickPos.z);

		m_pCurRefreeData->SetRefreePosition(pPickInfo->vPickPos);
	}
	else if( m_eButtonInputType == BUTTON_INPUT_TYPE_TARGET_POS )
	{
		m_pGridItemTargetX->SetDouble(pPickInfo->vPickPos.x);
		m_pGridItemTargetY->SetDouble(pPickInfo->vPickPos.y);
		m_pGridItemTargetZ->SetDouble(pPickInfo->vPickPos.z);

		m_pCurRefreeData->SetTargetPos(pPickInfo->vPickPos);
	}


	m_eButtonInputType = BUTTON_INPUT_TYPE_NONE;
}

void CDirectionPropRefreeView::SetAnimIDEnum() 
{
	if( m_lcAnimationList.GetCount() > 0 )
		return;

	RwUInt32 iCount = 0;
	std::string* pstrAnimName = NULL;
	CNtlPLCharacterParser parser;
	CNtlCharacterXMLScript xmlScript;
	CNtlPLCharacterProperty charPropery;

	TCHAR chCurDir[1024];
	GetCurrentDirectory(1024, chCurDir);
	std::string strFullName = (LPCSTR)(LPCTSTR)chCurDir;
	strFullName += "\\devdata\\character\\N_HFI_M_H1.XML";

	if( xmlScript.LoadCharacterScript(strFullName.c_str(), &charPropery) == false )
	{
		// 심판의 애니메이션 XML 파일이 없다
		AfxMessageBox("Can not found N_HFI_M_H1.XML file");
		return;
	}

	CNtlTypeAnimTable* pAnimTable = charPropery.GetAnimTable();
	if( !pAnimTable )
	{
		AfxMessageBox("Not exist animation table of N_HFI_M_H1.XML");
		return;
	}

	// Common animation set
	for(int i = COMMON_ANIMATION_START ; i < COMMON_ANIMATION_END + 1 ; ++i)
	{
		pstrAnimName = parser.GetNPCMatchTable()->GetString(i);
		if( !pstrAnimName )
			continue;

		sANIM_DATA* pAnimData = NTL_NEW sANIM_DATA;

		pAnimData->uiAnimID			= i;
		pAnimData->strAnimName		= (LPCTSTR)(LPCSTR)pstrAnimName->c_str();

		m_lcAnimationList.InsertString(iCount, pstrAnimName->c_str() );
		m_lcAnimationList.SetItemData(iCount, (DWORD_PTR)pAnimData);

		++iCount;
	}

	// trigger animation set
	for(int i = PC_TRIGGER_ANIMATION_START ; i < PC_TRIGGER_ANIMATION_END + 1 ; ++i)
	{
		pstrAnimName = parser.GetNPCMatchTable()->GetString(i);
		if( !pstrAnimName )
			continue;
		
		STypeAnimData* pTypeAnimData = pAnimTable->Get(i);
		if( !pTypeAnimData || pTypeAnimData->strAnimName.size() == 0 )
			continue;


		sANIM_DATA* pAnimData = NTL_NEW sANIM_DATA;

		pAnimData->uiAnimID			= i;
		pAnimData->strAnimName		= (LPCTSTR)(LPCSTR)pTypeAnimData->strAnimName.c_str();

		m_lcAnimationList.InsertString(iCount, pTypeAnimData->strAnimName.c_str() );
		m_lcAnimationList.SetItemData(iCount, (DWORD_PTR)pAnimData);

		++iCount;
	}
}

void CDirectionPropRefreeView::OnLbnDblclkList_AnimList()
{
	// 선택된 m_lcAnimationList의 항목을 m_lcSelectedAnimationList에 추가한다

	RwInt32 iIndex = m_lcAnimationList.GetCurSel();
	sANIM_DATA* pAnimData = (sANIM_DATA*)m_lcAnimationList.GetItemData( iIndex );	

	if( m_pCurRefreeData->IsExistAnimId(pAnimData->uiAnimID) )
		return;

	m_pCurRefreeData->AppendAnimId(pAnimData->uiAnimID);

	m_lcSelectedAnimationList.AddString(pAnimData->strAnimName);
	m_lcSelectedAnimationList.SetItemData(m_lcSelectedAnimationList.GetCount() - 1, (DWORD_PTR)pAnimData);
}

void CDirectionPropRefreeView::OnBnClickedButton_Delete()
{
	// m_lcSelectedAnimationList의 항목을 삭제한다

	RwInt32 iIndex = m_lcSelectedAnimationList.GetCurSel();
	if( iIndex == LB_ERR )
		return;

	if( m_pCurRefreeData->IsPlay() )
	{
		AfxMessageBox(_T("Refree Playing!"));
		return;
	}

	sANIM_DATA* pAnimData = (sANIM_DATA*)m_lcSelectedAnimationList.GetItemData(iIndex);
	m_pCurRefreeData->RemoveAnimId(pAnimData->uiAnimID);

	m_lcSelectedAnimationList.DeleteString(iIndex);
}

void CDirectionPropRefreeView::OnBnClickedButton_Up()
{
	// 선택된 m_lcSelectedAnimationList의 항목을 한단계 위로 올린다

	RwInt32 iIndex = m_lcSelectedAnimationList.GetCurSel();
	if( iIndex == LB_ERR )
		return;

	if( iIndex <= 0 )
		return;

	if( m_pCurRefreeData->IsPlay() )
	{
		AfxMessageBox(_T("Refree Playing!"));
		return;
	}

	sANIM_DATA* pAnimData = (sANIM_DATA*)m_lcSelectedAnimationList.GetItemData(iIndex);

	m_lcSelectedAnimationList.InsertString(iIndex - 1, pAnimData->strAnimName);
	m_lcSelectedAnimationList.SetItemData(iIndex - 1, (DWORD_PTR)pAnimData);

	m_lcSelectedAnimationList.DeleteString(iIndex + 1);

	m_lcSelectedAnimationList.SetCurSel(iIndex - 1);


	// CNtlSLCENode 정렬
	m_pCurRefreeData->RemoveAllAnimId();

	for(RwInt32 i = 0 ; i < m_lcSelectedAnimationList.GetCount() ; ++i )
	{
		sANIM_DATA* pAnimData = (sANIM_DATA*)m_lcSelectedAnimationList.GetItemData( i );
		m_pCurRefreeData->AppendAnimId(pAnimData->uiAnimID);
	}
}

void CDirectionPropRefreeView::OnBnClickedButton_Down()
{
	// 선택된 m_lcSelectedAnimationList의 항목을 한단계 아래로 올린다

	RwInt32 iIndex = m_lcSelectedAnimationList.GetCurSel();
	if( iIndex == LB_ERR )
		return;

	if( iIndex >= m_lcSelectedAnimationList.GetCount() - 1 )
		return;

	if( m_pCurRefreeData->IsPlay() )
	{
		AfxMessageBox(_T("Refree Playing!"));
		return;
	}

	sANIM_DATA* pAnimData = (sANIM_DATA*)m_lcSelectedAnimationList.GetItemData(iIndex);

	m_lcSelectedAnimationList.InsertString(iIndex + 2, pAnimData->strAnimName);
	m_lcSelectedAnimationList.SetItemData(iIndex + 2, (DWORD_PTR)pAnimData);

	m_lcSelectedAnimationList.SetCurSel(iIndex + 2);

	m_lcSelectedAnimationList.DeleteString(iIndex);


	// CNtlSLCENode 정렬
	m_pCurRefreeData->RemoveAllAnimId();

	for(RwInt32 i = 0 ; i < m_lcSelectedAnimationList.GetCount() ; ++i )
	{
		sANIM_DATA* pAnimData = (sANIM_DATA*)m_lcSelectedAnimationList.GetItemData( i );
		m_pCurRefreeData->AppendAnimId(pAnimData->uiAnimID);
	}
}

void CDirectionPropRefreeView::DestroyListItem()
{
	for( RwInt32 i = 0 ; i < m_lcAnimationList.GetCount() ; ++i )
	{
		sANIM_DATA* pAnimData = (sANIM_DATA*)m_lcAnimationList.GetItemData(i);
		NTL_DELETE(pAnimData);
	}

	m_lcAnimationList.ResetContent();
	m_lcSelectedAnimationList.ResetContent();
}

void CDirectionPropRefreeView::OnCbnSelchangeCombo1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pCurRefreeData->SetAnimPlayType( m_ComboAnimPlayType.GetCurSel() ); 	 
}

void CDirectionPropRefreeView::OnListBoxNewItem_NarrationList()
{	
	CString strText;
	m_ListBoxNarrationList.GetText(m_ListBoxNarrationList.GetCurrentIndex(), strText);

	int iNarrationIndex = atoi(strText.GetString());
	if( iNarrationIndex == 0 )
	{
		m_ListBoxNarrationList.DeleteString(m_ListBoxNarrationList.GetCount() - 1);
		AfxMessageBox("You only can input number");
		return;
	}

	m_pCurRefreeData->AppendNarrationIndex(iNarrationIndex);
}

void CDirectionPropRefreeView::OnListBoxDeleteItem_NarrationList()
{
	OnBnClickedButton_NarrationDelete();
}

void CDirectionPropRefreeView::OnBnClickedButton_NarrationDelete()
{
	RwInt32 iIndex = m_ListBoxNarrationList.GetCurSel();
	if( iIndex == LB_ERR )
		return;

	if( m_pCurRefreeData->IsPlay() )
	{
		AfxMessageBox(_T("Refree Playing!"));
		return;
	}

	m_pCurRefreeData->RemoveNarrationIndex(iIndex);
	m_ListBoxNarrationList.DeleteString(iIndex);
}

void CDirectionPropRefreeView::OnLbnSelchangeList_Narration()
{
	m_iLastSelectedIndex_in_ListBoxNarrationList = m_ListBoxNarrationList.GetCurSel();
}

void CDirectionPropRefreeView::OnSelchangeEditList_Narration()
{
	if( m_iLastSelectedIndex_in_ListBoxNarrationList == LB_ERR )
		return;

	CString strText;
	m_ListBoxNarrationList.GetText(m_iLastSelectedIndex_in_ListBoxNarrationList, strText);

	int iNarrationIndex = atoi(strText.GetString());
	if( iNarrationIndex == 0 )
	{
		m_ListBoxNarrationList.ResetContent();

		char acBuffer[10] = "";
		VEC_NARRATION_ID& vecNarration = m_pCurRefreeData->GetNarrationList();
		for( int i = 0 ; i < (int)vecNarration.size() ; ++i )
		{
			int iNarrationIndex = vecNarration[i];
			itoa(iNarrationIndex, acBuffer, 10);
			m_ListBoxNarrationList.AddString(acBuffer);
		}

		AfxMessageBox("You only can input number");
		return;
	}

	RefreshNarrationList();
}

void CDirectionPropRefreeView::OnBnClickedButton_NarrationUp()
{
	// 선택된 m_ListBoxNarrationList의 항목을 한단계 위로 올린다

	RwInt32 iIndex = m_ListBoxNarrationList.GetCurSel();
	if( iIndex == LB_ERR )
		return;

	if( iIndex <= 0 )
		return;

	if( m_pCurRefreeData->IsPlay() )
	{
		AfxMessageBox(_T("Refree Playing!"));
		return;
	}

	CString strText;
	m_ListBoxNarrationList.GetText(iIndex, strText);

	m_ListBoxNarrationList.InsertString(iIndex - 1, strText);
	m_ListBoxNarrationList.DeleteString(iIndex + 1);

	m_ListBoxNarrationList.SetCurSel(iIndex - 1);


	RefreshNarrationList();
}

void CDirectionPropRefreeView::OnBnClickedButton_NarrationDown()
{
	// 선택된 m_ListBoxNarrationList의 항목을 한단계 아래로 올린다

	RwInt32 iIndex = m_ListBoxNarrationList.GetCurSel();
	if( iIndex == LB_ERR )
		return;

	if( iIndex >= m_ListBoxNarrationList.GetCount() - 1 )
		return;

	if( m_pCurRefreeData->IsPlay() )
	{
		AfxMessageBox(_T("Refree Playing!"));
		return;
	}

	CString strText;
	m_ListBoxNarrationList.GetText(iIndex, strText);

	m_ListBoxNarrationList.InsertString(iIndex + 2, strText);
	m_ListBoxNarrationList.SetCurSel(iIndex + 2);

	m_ListBoxNarrationList.DeleteString(iIndex);

	RefreshNarrationList();
}

void CDirectionPropRefreeView::OnCbnSelchangeCombo_NarrationPlayType()
{
	m_pCurRefreeData->SetNarrationPlayType( m_ComboBoxNarrationPlayType.GetCurSel() ); 	 
}

void CDirectionPropRefreeView::RefreshNarrationList()
{
	CString strText;

	// CNtlSLCENode 정렬
	m_pCurRefreeData->RemoveAllNarrationIndex();
	
	for(RwInt32 i = 0 ; i < m_ListBoxNarrationList.GetCount() ; ++i )
	{
		m_ListBoxNarrationList.GetText(i, strText);

		if( strText.GetLength() > 0 )
		{
			int iNarrationIndex = atoi(strText.GetString());
			m_pCurRefreeData->AppendNarrationIndex(iNarrationIndex);
		}		
	}
}