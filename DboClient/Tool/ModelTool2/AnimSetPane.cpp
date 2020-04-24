// AnimSetPane.cpp : implementation file
//

#include "stdafx.h"
#include "ModelTool2.h"
#include "AttackTypePane.h"
#include "AnimPlayPane.h"
#include "AnimSetPane.h"
#include "BoneEditPane.h"
#include "VehicleViewPane.h"
#include "PropertyPane.h"

CAnimSetPane* CAnimSetPane::m_pInstance = NULL;
// CAnimSetPane

IMPLEMENT_DYNCREATE(CAnimSetPane, CXTResizeFormView)

CAnimSetPane::CAnimSetPane()
	: CXTResizeFormView(CAnimSetPane::IDD)
{
    m_pInstance = this;

    m_pCharacter = NULL;
    m_pItem      = NULL;
    m_pAnimTable = NULL;
	m_pObject	 = NULL;
    m_eAnimSetMode = ANIMSET_CHARACTER;
}

CAnimSetPane::~CAnimSetPane()
{
}

void CAnimSetPane::DoDataExchange(CDataExchange* pDX)
{
    CXTResizeFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CB_ANIM_SET, m_cbAnimSet);
    DDX_Control(pDX, IDC_LT_ANIM_ITEM, m_ltAnimItem);    
}

BEGIN_MESSAGE_MAP(CAnimSetPane, CXTResizeFormView)
    ON_CBN_SELCHANGE(IDC_CB_ANIM_SET, &CAnimSetPane::OnCbnSelchangeCbAnimSet)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LT_ANIM_ITEM, &CAnimSetPane::OnLvnItemchangedLtAnimItem)
    ON_NOTIFY(NM_RCLICK, IDC_LT_ANIM_ITEM, &CAnimSetPane::OnNMRclickLtAnimItem)
    ON_COMMAND(ID_MENU_ANIM_SET, &CAnimSetPane::OnMenuAnimSet)
    ON_COMMAND(ID_MENU_ANIM_DELETE, &CAnimSetPane::OnMenuAnimDelete)
END_MESSAGE_MAP()


// CAnimSetPane diagnostics

#ifdef _DEBUG
void CAnimSetPane::AssertValid() const
{
	CXTResizeFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CAnimSetPane::Dump(CDumpContext& dc) const
{
	CXTResizeFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CAnimSetPane message handlers

void CAnimSetPane::OnInitialUpdate()
{
    CXTResizeFormView::OnInitialUpdate();
    
    SetResize(IDC_STATIC_GROUP, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
    SetResize(IDC_CB_ANIM_SET, SZ_TOP_LEFT, SZ_TOP_RIGHT);
    SetResize(IDC_LT_ANIM_ITEM, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);        

    m_ltAnimItem.SetExtendedStyle(m_ltAnimItem.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT);
    m_ltAnimItem.InsertColumn(0, "ID", 0, 30, 0);
    m_ltAnimItem.InsertColumn(1, "Anim Name", 0, 160, 1);
    m_ltAnimItem.InsertColumn(2, "Anim File", 0, 110, 2);
}

void CAnimSetPane::SetModel(CNtlPLAttach* pModel)
{
    if(pModel == NULL)
    {
        m_pCharacter = NULL;
        m_pItem = NULL;
        SetEnable(FALSE);

        m_ltAnimItem.DeleteAllItems();        
    }
    else
    {
        SetEnable(TRUE);

        switch(pModel->GetClassType())
        {
        case PLENTITY_CHARACTER:
            {
                if(m_pCharacter == pModel)
                    return;

                // 기존 애니메이션은 정지 시킨다.
                if(m_pCharacter)
                {
                    m_pCharacter->SetAnimUpdate(FALSE);
                }

                m_pCharacter = (CMTCharacter*)pModel;
                m_pItem = NULL;
				m_pObject = NULL;

                m_pAnimTable = m_pCharacter->GetProperty()->GetAnimTable();                

                // Animation Set을 초기화한다.       
                int nPrevSel = m_cbAnimSet.GetCurSel();
                if(nPrevSel < 0)
                {
                    nPrevSel = 0;
                }

                m_cbAnimSet.ResetContent();

                if(CModelToolApplication::GetInstance()->GetAppMode() == MT_MODE_VEHICLE)
                {
                    // 탈것용 Anim Set                    
                    m_cbAnimSet.AddString("Vehicle Animation Set");
                    m_cbAnimSet.SetCurSel(0);
                    m_eAnimSetMode = ANIMSET_VEHICLE;
                }
                else
                {
                    m_cbAnimSet.InsertString(COMMON_ANIM_SET, "Common Animation Set");
                    m_cbAnimSet.InsertString(ATTACK_ANIM_SET, "Attack Animation Set");
                    m_cbAnimSet.InsertString(BATTLE_ANIM_SET, "Battle Animation Set");
                    m_cbAnimSet.InsertString(SKILL_ANIM_SET, "Skill Animation Set");
                    m_cbAnimSet.InsertString(HTB_ANIM_SET, "HTB Animation Set");
                    m_cbAnimSet.InsertString(SOCIAL_ANIM_SET, "Social Animation Set");
                    m_cbAnimSet.InsertString(TRIGGER_ANIM_SET, "Trigger Animation Set");
                    m_cbAnimSet.InsertString(TRANSFORM_ANIM_SET, "Transform Animation Set");
                    m_cbAnimSet.InsertString(VEHICLE_SRP1_ANIM_SET, "Vehicle SRP 1");
                    m_cbAnimSet.InsertString(VEHICLE_SRP2_ANIM_SET, "Vehicle SRP 2");
                    m_cbAnimSet.InsertString(ITEM_USE_ANIM_SET, "Item Use Animation Set");
                    m_cbAnimSet.SetCurSel(nPrevSel);
                    m_eAnimSetMode = ANIMSET_CHARACTER;
                }                               
            }
            break;
        case PLENTITY_ITEM:
            if(m_pItem == pModel)
                return;

            m_pItem = (CMTItem*)pModel;
            m_pCharacter = NULL;
			m_pObject = NULL;

            m_pAnimTable = m_pItem->GetProperty()->GetAnimTable();
            m_eAnimSetMode = ANIMSET_ITEM;

            m_cbAnimSet.ResetContent();
            m_cbAnimSet.AddString("Item Animation Set");
            m_cbAnimSet.SetCurSel(0);
            break;                
		case PLENTITY_OBJECT:
			if(m_pObject == pModel)
				return;

			m_pObject = (CMTObject*)pModel;
			m_pCharacter = NULL;
			m_pItem = NULL;

			m_pAnimTable = m_pObject->GetProperty()->GetAnimTable();
			m_eAnimSetMode = ANIMSET_OBJECT;
			
			m_cbAnimSet.ResetContent();
			m_cbAnimSet.AddString("Trigger Object Animation Set");
			m_cbAnimSet.SetCurSel(0);			
			break;
		}

        OnCbnSelchangeCbAnimSet();
    }
}

void CAnimSetPane::SetEnable(BOOL bEnable)
{
    m_cbAnimSet.EnableWindow(bEnable);    
    m_ltAnimItem.EnableWindow(bEnable);    
}

void CAnimSetPane::OnCbnSelchangeCbAnimSet()
{
    // 콤보박스가 변경되었을때, 리스트의 아이템 항목을 변경한다.
    USES_CONVERSION;

    if(!m_pAnimTable)
        return;

    m_ltAnimItem.DeleteAllItems();

    int nIndex = m_cbAnimSet.GetCurSel();    
    int nAnimKeyStart = 0;
    int nCount = 0;
    int nAnimKeyEnd   = 0;

    if(m_eAnimSetMode == ANIMSET_CHARACTER)
    {
        switch(nIndex)
        {
        case COMMON_ANIM_SET:
            nAnimKeyStart = COMMON_ANIMATION_START;
            nAnimKeyEnd   = COMMON_ANIMATION_END + 1;                   
            break;
        case ATTACK_ANIM_SET:
            nAnimKeyStart = ATTACK_ANIMATION_START;
            nAnimKeyEnd   = ATTACK_ANIMATION_END + 1;        
            break;
        case BATTLE_ANIM_SET:
            nAnimKeyStart = BATTLE_ANIMATION_START;
            nAnimKeyEnd   = BATTLE_ANIMATION_END + 1;        
            break;
        case HTB_ANIM_SET:
            nAnimKeyStart = HTB_ANIMATION_START;
            nAnimKeyEnd   = HTB_ANIMATION_END;
            break;
        case SKILL_ANIM_SET:
            nAnimKeyStart = SKILL_ANIMATION_START;
            nAnimKeyEnd   = SKILL_ANIMATION_END;
            break;
        case SOCIAL_ANIM_SET:
            nAnimKeyStart = SOCIAL_ANIMATION_START;
            nAnimKeyEnd   = SOCIAL_ANIMATION_END + 1;     
            break;
        case TRIGGER_ANIM_SET:
            nAnimKeyStart = PC_TRIGGER_ANIMATION_START;
            nAnimKeyEnd   = PC_TRIGGER_ANIMATION_END + 1;
            break;
        case TRANSFORM_ANIM_SET:
            nAnimKeyStart = TRANSFORM_ANIMATION_START;
            nAnimKeyEnd   = TRANSFORM_ANIMATION_END + 1;
            break;
        case VEHICLE_SRP1_ANIM_SET:
            nAnimKeyStart = VEHICLE_SRP1_ANIMATION_START;
            nAnimKeyEnd = VEHICLE_SRP1_ANIMATION_END + 1;
            break;
        case VEHICLE_SRP2_ANIM_SET:
            nAnimKeyStart = VEHICLE_SRP2_ANIMATION_START;
            nAnimKeyEnd = VEHICLE_SRP2_ANIMATION_END + 1;
            break;      
        case ITEM_USE_ANIM_SET:
            nAnimKeyStart = ITEM_USE_ANIMATION_START;
            nAnimKeyEnd = ITEM_USE_ANIMATION_END + 1;
            break;
        default:
            NTL_ASSERTFAIL("Animation Set Select Error!");
            break;
        }
    }
    else if(m_eAnimSetMode == ANIMSET_ITEM)
    {
        switch(nIndex)
        {
        case ITEM_ANIM_SET:
            nAnimKeyStart = ITEM_ANIMATION_START;
            nAnimKeyEnd = ITEM_ANIMATION_END + 1;
        }
    }
	else if(m_eAnimSetMode == ANIMSET_OBJECT)
	{
		nAnimKeyStart = OBJECT_ANIMATION_START;
		nAnimKeyEnd = OBJECT_ANIMATION_END + 1;
	}
    else if(m_eAnimSetMode == ANIMSET_VEHICLE)
    {
        nAnimKeyStart = VEHICLE_ANIMATION_START;
        nAnimKeyEnd = VEHICLE_ANIMATION_END;
    }

    for(int i = nAnimKeyStart; i < nAnimKeyEnd; ++i)
    {
        std::string* sAnimName =  NULL;
		std::string strTempObjectAnimName = "";
        switch(CModelToolApplication::GetInstance()->GetAppMode())
        {
        case MT_MODE_PC:
            sAnimName = m_CharacterPaser.GetPCMatchTable()->GetString(i);
            if(!sAnimName)
                sAnimName = &strTempObjectAnimName;
            break;
        case MT_MODE_MOB:
            sAnimName = m_CharacterPaser.GetMobMatchTable()->GetString(i);
            break;
        case MT_MODE_NPC:
            sAnimName = m_CharacterPaser.GetNPCMatchTable()->GetString(i);
            break;
        case MT_MODE_ITEM:
            sAnimName = m_ItemParser.GetItemMatchTable()->GetString(i);
            break;
        case MT_MODE_OBJECT:        
            sAnimName = &strTempObjectAnimName;
            break;
        case MT_MODE_VEHICLE:
            sAnimName = m_CharacterPaser.GetVehicleTable()->GetString(i);
            break;
        }

        if(sAnimName)
        {
            CString sAnimID;
            sAnimID.Format("%d ", i);            
            m_ltAnimItem.InsertItem(nCount, sAnimID);  
            m_ltAnimItem.SetItem(nCount, 1, LVIF_TEXT, (sAnimName->c_str()), 0, 0, 0, 0);			

            // 데이터가 있을경우에는 표시한다.
            if(m_pAnimTable)
            {
                STypeAnimData* pAnimData = m_pAnimTable->Get(i);
                if(pAnimData && pAnimData->strAnimName.size() > 0)
                {
                    m_ltAnimItem.SetItem(nCount, 2, LVIF_TEXT, (pAnimData->strAnimName.c_str()), 0, 0, 0, 0);
                }
            }            
            ++nCount;
        }        
    }
}

void CAnimSetPane::OnLvnItemchangedLtAnimItem(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

    if(!m_pAnimTable)
        return;

    USES_CONVERSION;
    static int nOldIndex = -1;  // 중복 호출을 막기위한 변수

    int nIndex = pNMLV->iItem;
    if(nIndex == nOldIndex)
        return;

    nOldIndex = nIndex;

    // 설정된 애니메이션이 있으면, 그 애니메이션으로 애니메이션을 변경한다.    
    
    CString sAnimFileName = m_ltAnimItem.GetItemText(nIndex, 2);
    if(sAnimFileName != "")
    {
        CString sAnimID = m_ltAnimItem.GetItemText(nIndex, 0);
        RwUInt32 uiAnimKey = (RwUInt32)atof(sAnimID);

        SetAnimData(uiAnimKey);
    }
    else
    {
        if(m_eAnimSetMode == ANIMSET_CHARACTER || m_eAnimSetMode == ANIMSET_VEHICLE)
        {
            m_pCharacter->SetAnimUpdate(FALSE);
        }
        else if(m_eAnimSetMode == ANIMSET_ITEM)
        {
            m_pItem->SetAnimUpdate(FALSE);
        }
        
        // 설정된 애니메이션파일이 없으면 Edit UI를 Disable 시킨다.
        GetSafeInstance(CAttackTypePane)->SetAnimEventData(NULL);
        GetSafeInstance(CAnimPlayPane)->SetAnimData(NULL, NULL);
    }

    *pResult = 0;
}

void CAnimSetPane::OnNMRclickLtAnimItem(NMHDR *pNMHDR, LRESULT *pResult)
{
    // 오른쪽 클릭시 팝업메뉴를 뛰운다.
    int nIndex = m_ltAnimItem.GetSelectionMark();
    if(nIndex >= 0)
    {
        POINT pos;
        GetCursorPos(&pos);        
        CMenu   menuPopup;
        menuPopup.LoadMenu(IDR_MENU_POPUP);
        CMenu*  subMenu = menuPopup.GetSubMenu(2);

        // 애니메이션파일 정보의 존재여부에 따라 메뉴를 설정한다.
        CString sAnimFileName = m_ltAnimItem.GetItemText(nIndex, 2);
        if(sAnimFileName == "")
        {
            subMenu->EnableMenuItem(ID_MENU_ANIM_SET, FALSE);
            subMenu->EnableMenuItem(ID_MENU_ANIM_DELETE, TRUE);
        }
        else
        {
            subMenu->EnableMenuItem(ID_MENU_ANIM_SET, FALSE);
            subMenu->EnableMenuItem(ID_MENU_ANIM_DELETE, FALSE);
        }

        subMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pos.x, pos.y, this);
    }    
    *pResult = 0;
}

void CAnimSetPane::OnMenuAnimSet()
{
    if(!m_pAnimTable)
        return;

    // 항목에 애니메이션 파일을 설정한다.
    USES_CONVERSION;

    int nIndex = m_ltAnimItem.GetSelectionMark();

    CHAR szOpenFilter[] = "Animation File (*.anm)|*.anm||";
    CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szOpenFilter);
    if(fileDlg.DoModal() == IDOK)
    {
        // 기존에 Anim이 존재하면 없애준다.
        CString strPrevAnimName = m_ltAnimItem.GetItemText(nIndex, 2);
        if(strPrevAnimName != "")
        {
            OnMenuAnimDelete();
        }

        CString sLoadFilePath = fileDlg.GetPathName();
        CString sLoadFileName = fileDlg.GetFileName();       
        m_ltAnimItem.SetItem(nIndex, 2, LVIF_TEXT, sLoadFileName, 0, 0, 0, 0);

        // 절대경로를 상대경로로 바꾼다.
        CString strWorkPath = (CModelToolApplication::GetInstance()->GetWorkDir());
        strWorkPath = strWorkPath.MakeUpper();
        sLoadFilePath = sLoadFilePath.MakeUpper();
        sLoadFileName = sLoadFileName.MakeUpper();
        sLoadFilePath.Replace(strWorkPath, ".");
        sLoadFilePath.Replace(sLoadFileName, "");
        std::string sAnimFilePath = (sLoadFilePath);

        // property에 애니메이션 파일 정보를 추가한다.
        CString sAnimID = m_ltAnimItem.GetItemText(nIndex, 0);
        RwUInt32 uiAnimKey = (RwUInt32)atof(sAnimID);

        std::string sAnimFileName = (sLoadFileName);

        // Property에 Anim 폴더 경로를 지정한다. (상대 경로)                                
        m_pAnimTable->SetAnimPath(sAnimFilePath);
        STypeAnimData* pAnimData = m_pAnimTable->Add(uiAnimKey, sAnimFileName);
        SetAnimData(uiAnimKey);      

        if(m_eAnimSetMode == ANIMSET_CHARACTER || m_eAnimSetMode == ANIMSET_VEHICLE)
        {
            // Play Time을 저장한다.
            m_pCharacter->Update(0.1f);
            pAnimData->fPlayTime = m_pCharacter->GetBaseDurationAnimTime();
            
        }
        else if(m_eAnimSetMode == ANIMSET_ITEM)
        {
            // Play Time을 저장한다.
            m_pItem->Update(0.1f);            
            pAnimData->fPlayTime = m_pItem->GetBaseDurationAnimTime();            
        }
		else if(m_eAnimSetMode == ANIMSET_OBJECT)
		{
			// PlayTime을 저장한다.
			// m_pObject->Update(0.1f);			
			pAnimData->fPlayTime = m_pObject->GetBaseDurationAnimTime();
		}

        CModelToolApplication::GetInstance()->SetDataChanged();
    }
}

void CAnimSetPane::OnMenuAnimDelete()
{
    // 항목에 있는 애니메이션을 삭제한다.
    if(!m_pAnimTable)
        return;

    USES_CONVERSION;

    int nIndex = m_ltAnimItem.GetSelectionMark();

    CString sAnimID = m_ltAnimItem.GetItemText(nIndex, 0);
    RwUInt32 uiAnimKey = (RwUInt32)atof(sAnimID);

    // 프로퍼티의 테이블에서 삭제한다.
    m_pAnimTable->Remove(uiAnimKey);

    // 리스트 목록에서 제거한다.
    m_ltAnimItem.SetItemText(nIndex, 2, "");

    if(m_eAnimSetMode == ANIMSET_CHARACTER || m_eAnimSetMode == ANIMSET_VEHICLE)
    {
        m_pCharacter->SetAnimUpdate(FALSE);
    }
    else if(m_eAnimSetMode == ANIMSET_OBJECT)
    {
        m_pObject->SetAnimUpdate(FALSE);
    }
    else if(m_eAnimSetMode == ANIMSET_ITEM)
    {
        m_pItem->SetAnimUpdate(FALSE);
    }

    GetSafeInstance(CAttackTypePane)->SetAnimEventData(NULL);
    GetSafeInstance(CAnimPlayPane)->SetAnimData(NULL, NULL);

    CModelToolApplication::GetInstance()->SetDataChanged();
}

void CAnimSetPane::SetAnimData(RwUInt32 uiKey)
{
    if(!m_pAnimTable)
        return;

    ::SetCurrentDirectoryA(CModelToolApplication::GetInstance()->GetWorkDir());

	STypeAnimData* pAnimData = m_pAnimTable->Get(uiKey);

    // 애니메이션을 모델에 적용한다.
    if(m_eAnimSetMode == ANIMSET_CHARACTER || m_eAnimSetMode == ANIMSET_VEHICLE)
    {
        if(!m_pCharacter->SetAnim(uiKey))
        {
            MessageBox("Animation File is Not Exist\n Check Script File (*.xml)", "Anim Error", MB_ICONERROR);
            return;
        }

        
        if(pAnimData)
        {
            // 데이터를 AttackType Pane에 적용한다.
            GetSafeInstance(CAttackTypePane)->SetAnimEventData(pAnimData);

            //// 애니메이션 플레이 툴뷰를 활성화 시킨다.
            GetSafeInstance(CAnimPlayPane)->SetAnimData(m_pCharacter, pAnimData);    

            // 프로퍼티를 설정한다.
            GetSafeInstance(CPropertyPane)->SetAnimation(pAnimData);
        }

        // 하단뷰의 Bone Edit를 활성화 시킨다.
        GetSafeInstance(CBoneEditPane)->SetEnable(TRUE);

        // 탈것의 애니메이션
        if(CModelToolApplication::GetInstance()->GetAppMode() == MT_MODE_VEHICLE)
        {
            GetSafeInstance(CVehicleViewPane)->SetVehicleAnimation(uiKey);
        }

    }
    else if(m_eAnimSetMode == ANIMSET_ITEM)
    {
        if(!m_pItem->SetAnimation(uiKey))
        {
            MessageBox("Animation File is Not Exist\n Check Script File (*.xml)", "Anim Error", MB_ICONERROR);
            return;
        }
    }
	else if(m_eAnimSetMode == ANIMSET_OBJECT)
	{
		if(m_pObject)
		{
			if(!m_pObject->SetTriggerAnimation(uiKey))
			{
				MessageBox("Animation File is Not Exist\n Check Script File (*.xml)", "Anim Error", MB_ICONERROR);
				return;
			}

			// 애니메이션 플레이 툴뷰를 활성화 시킨다.			
			GetSafeInstance(CAnimPlayPane)->SetTriggerObjectAnimData(m_pObject, pAnimData);

            // 프로퍼티를 설정한다.
            GetSafeInstance(CPropertyPane)->SetAnimation(pAnimData);
		}
	}
}


