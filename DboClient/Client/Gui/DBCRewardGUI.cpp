#include "precomp_dboclient.h"
#include "DBCRewardGUI.h"

#include "NtlSLEvent.h"
#include "NtlSlApi.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "NtlSobAvatarAttr.h"

#include "DboLogic.h"
#include "TableContainer.h"
#include "TextAllTable.h"

#include "NtlPLGuiManager.h"
#include "DialogManager.h"
#include "InfoWndManager.h"
#include "DisplayStringManager.h"
#include "DboEventGenerator.h"
#include "DboEvent.h"

#include "NtlWorldConcept.h"
#include "NtlWorldConceptDBC.h"

#include "DboGlobal.h"
#include "NtlResultCode.h"

#include "DBCDragonDialogGUI.h"

#define TIME_HURRYUP	60 * 1000				// 1분

#define REWARD_LIST_X		26
#define REWARD_LIST_Y		48
#define REWARD_LIST_MARGIN_HEIGHT	2
#define REWARD_ITEM_WIDTH	284
#define REWARD_ITEM_HEIGHT	25

#define REWARD_ITEM_SUF_X	157	
#define REWARD_ITEM_SUF_Y	90


CDBCRewardGUI* CDBCRewardGUI::m_pInstance = NULL;

CDBCRewardGUI::CDBCRewardGUI(void)
{
	Init();
}

CDBCRewardGUI::CDBCRewardGUI( const RwChar* pName ) 
: CNtlPLGui(pName)
{
	Init();
}

CDBCRewardGUI::~CDBCRewardGUI(void)
{
	Destroy();
}

VOID CDBCRewardGUI::Init() 
{
	m_pBtnBack			= NULL;
	m_pBtnOK			= NULL;
	m_pBtnCancel		= NULL;
	m_pSttTime			= NULL;
	m_bDisplayHurryUp	= FALSE;
	m_bDelete			= FALSE;
	m_pDBRewardTextTable = NULL;
	m_pSttItemName		= NULL;
	m_pSttItemText		= NULL;
	m_nDepthIdx			= 0;
	m_nPrevDepthIdx		= 0;	
	m_pPrevItem			= NULL;
	m_pRewardItemTblData = NULL;
	m_pScrollBar		= NULL;	
	m_pRewardItem		= NULL;
	m_sufItem.UnsetTexture();	
	m_pDragonBallTable  = NULL;
	m_pDragonBallRewardTable = NULL;
	m_pWorldConceptDBC	= NULL;
    m_nBtnCount         = 0;

	ZeroMemory(m_pBuf, sizeof(m_pBuf));
}

RwBool CDBCRewardGUI::Create() 
{
	NTL_FUNCTION(__FUNCTION__);

	if(!CNtlPLGui::Create("", "gui\\DBCReward.srf", "gui\\DBCReward.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_pThis			= (gui::CDialog*)GetComponent("dlgMain");
    m_pPnlSelectMenu = (gui::CPanel*)GetComponent("pnlSelectMenu");
    m_pPnlSelectItem = (gui::CPanel*)GetComponent("pnlSelectItem");
	m_pBtnBack		= (gui::CButton*)GetComponent("btnSelectMenuBack");
	m_pBtnOK		= (gui::CButton*)GetComponent("btnSelectItemOK");
	m_pBtnCancel	= (gui::CButton*)GetComponent("btnSelectItemCancel");
	m_pSttTime		= (gui::CStaticBox*)GetComponent("sttTime");
	m_pSttItemName	= (gui::CStaticBox*)GetComponent("sttItemName");
	m_pSttItemText	= (gui::CStaticBox*)GetComponent("sttItemText");
	m_pScrollBar	= (gui::CScrollBar*)GetComponent("scbScroll");	
	
	m_ButtonList.Create(m_pThis, this, &CDBCRewardGUI::OnCreateItem, 
						&CDBCRewardGUI::OnDestroyItem,
						&CDBCRewardGUI::OnClickedItem,
						REWARD_LIST_X,
						REWARD_LIST_Y,
						REWARD_LIST_MARGIN_HEIGHT,
						REWARD_LIST_MAX_VISIBLE);						
	
    m_pBtnBack->SetText(GetDisplayStringManager()->GetString("DST_DBC_ITEM_BACK"));
	m_pBtnOK->SetText(GetDisplayStringManager()->GetString("DST_DBC_ITEM_OK"));
	m_pBtnCancel->SetText(GetDisplayStringManager()->GetString("DST_DBC_ITEM_BACK"));
	m_pSttItemText->SetText(GetDisplayStringManager()->GetString("DST_DBC_ITEM_CONFIRM"));
	
	m_pSttTime->SetEffectColor(RGB(0, 0, 0));
	m_pSttTime->SetEffectMode(TE_SHADOW);

	m_pBtnOK->SetTextDownColor(RGB(241, 163, 100));
	m_pBtnCancel->SetTextDownColor(RGB(241, 163, 100));

	m_slotPaint		   = m_pPnlSelectItem->SigPaint().Connect(this, &CDBCRewardGUI::OnPaint);
	m_slotMove	 	   = m_pThis->SigMove().Connect( this, &CDBCRewardGUI::OnMove );	
	m_slotMouseMove	   = m_pThis->SigMouseMove().Connect(this, &CDBCRewardGUI::OnMouseMove);
	m_slotMouseLeave   = m_pThis->SigMouseLeave().Connect(this, &CDBCRewardGUI::OnMouseLeave);
	m_slotClickBackBtn = m_pBtnBack->SigClicked().Connect(this, &CDBCRewardGUI::OnClickBackBtn);
	m_slotClickOKBtn   = m_pBtnOK->SigClicked().Connect(this, &CDBCRewardGUI::OnClickOkBtn);
	m_slotClickCancelBtn = m_pBtnCancel->SigClicked().Connect(this, &CDBCRewardGUI::OnClickCancelBtn);
	m_slotSliderMove   = m_pScrollBar->SigSliderMoved().Connect(this, &CDBCRewardGUI::OnScrollBarChanged);
	m_slotSliderChanged = m_pScrollBar->SigValueChanged().Connect(this, &CDBCRewardGUI::OnScrollBarChanged);
    m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect(this, &CDBCRewardGUI::OnCaptureMouseDown);

	m_pWorldConceptDBC = (CNtlWorldConceptDBC*)GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_DRAGONBALL_COLLECT);

	// 관련 테이블
	m_pDBRewardTextTable = API_GetTableContainer()->GetTextAllTable()->GetDBRewardTbl();
	m_pDragonBallTable	 = API_GetTableContainer()->GetDragonBallTable();
	m_pDragonBallRewardTable = API_GetTableContainer()->GetDragonBallRewardTable();

	SetStatus(E_REWARD_STATUS_SELECT);	
	
	CreateRewardTree(&m_RewardItemRoot, 1);	
	ShowList(&m_RewardItemRoot, 0);

	GetNtlGuiManager()->AddUpdateFunc(this);
	
	Show(FALSE);

    LinkMsg(g_EventDBCNarrationEnd);
    LinkMsg(g_EventDBCReward_Res);
    LinkMsg(g_EventDBCCollect_Nfy);

	NTL_RETURN(TRUE);
}

VOID CDBCRewardGUI::Destroy() 
{
	if(m_sufItem.GetTexture())
	{
		gui::CTexture* pTexture = m_sufItem.GetTexture();
		m_sufItem.UnsetTexture();
		Logic_DeleteTexture(pTexture);		
	}

	m_ButtonList.Destroy();

	GetNtlGuiManager()->RemoveUpdateFunc(this);

	UnLinkMsg(g_EventDBCCollect_Nfy);
	UnLinkMsg(g_EventDBCReward_Res);	
	UnLinkMsg(g_EventDBCNarrationEnd);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CDBCRewardGUI::Update( RwReal fElapsed ) 
{
	if(m_bDelete)
	{
		CDBCRewardGUI::DeleteInstance();
		return;
	}

    m_pWorldConceptDBC = (CNtlWorldConceptDBC*)GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_DRAGONBALL_COLLECT);
    if(!m_pWorldConceptDBC)
        return;

	RwUInt32 uiRemainTime = m_pWorldConceptDBC->GetReaminTime();
	if(uiRemainTime <= 0 && !m_bDisplayHurryUp)
    {
        NTL_ASSERTFAIL("WorldConcept Fail");
        m_pWorldConceptDBC = (CNtlWorldConceptDBC*)GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_DRAGONBALL_COLLECT);
		return;
    }

	// 제한 시간을 표시한다.
	RwInt32 nMinute = (RwInt32)(uiRemainTime / (60 * 1000));
	RwInt32 nSecond = (RwInt32)((uiRemainTime - nMinute * 60 * 1000) / 1000);
	
	swprintf_s(m_pBuf, L"%s %02d:%02d", GetDisplayStringManager()->GetString("DST_DBC_TIME_LIMIT"), nMinute, nSecond);
	m_pSttTime->SetText(m_pBuf);

	if(!m_bDisplayHurryUp && uiRemainTime <= TIME_HURRYUP)
	{
		//드래곤의 대사 (시간이 없다. 빨리 선택해라)		
		sDRAGONBALL_TBLDAT* pData = (sDRAGONBALL_TBLDAT*)m_pDragonBallTable->GetDBTbldat((BYTE)m_pWorldConceptDBC->GetDBKind());
		if(pData)
		{
			CDBCDragonDialogGUI::GetInstance()->SetText(pData->hurryDialog, CDBCDragonDialogGUI::E_DIALOG_WARN);
			m_bDisplayHurryUp = TRUE;
		}		
	}
}

RwInt32 CDBCRewardGUI::SwitchDialog( bool bOpen ) 
{
	if(B2b(IsShow()) == bOpen)
		return 1;

	if(bOpen)
	{
		Show(TRUE);
	}
	else
	{
		Show(FALSE);
		m_bDelete = TRUE;
	}

	return 1;
}

VOID CDBCRewardGUI::HandleEvents( RWS::CMsg &pMsg ) 
{
	if(pMsg.Id == g_EventDBCNarrationEnd)
	{
		// 나레이션이 끝난후 보상 UI 등장
        RwInt32 nDBCState = GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_DRAGONBALL_COLLECT)->GetState();

		if(nDBCState == WORLD_DBC_REWARD_UI ||
           nDBCState == WORLD_DBC_NARRATION_START)                  /// ESC로 캔슬했을때
		{
			GetDialogManager()->OpenDialog(DIALOG_DBC_REWARD);
		}
	}
	else if(pMsg.Id == g_EventDBCReward_Res)	// 보상 선택
	{
		SNtlEventDBC_Res* pData = (SNtlEventDBC_Res*)pMsg.pData;
		if(pData)
		{
			switch(pData->wResultCode)
			{
			case GAME_SUCCESS:
				{
					GetDialogManager()->CloseDialog(DIALOG_DBC_REWARD);		
					GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_DRAGONBALL_COLLECT)->ChangeState(WORLD_DBC_NARRATION_END);
				}
				break;
			case GAME_FAIL:
				{
					NTL_ASSERTFAIL("Result is Fail");
				}
				break;
			case GAME_DRAGONBALL_REWARD_NOT_FOUND:
				{
					NTL_ASSERTFAIL("Reward Not Found");
				}
				break;
			case GAME_ITEM_INVEN_FULL:
				{
					// 용신 대사 "인벤이 가득찼다 인벤을 비워라"					
					sDRAGONBALL_TBLDAT* pDBData = (sDRAGONBALL_TBLDAT*)m_pDragonBallTable->GetDBTbldat((BYTE)m_pWorldConceptDBC->GetDBKind());
					CDBCDragonDialogGUI::GetInstance()->SetText(pDBData->inventoryFullDialog, CDBCDragonDialogGUI::E_DIALOG_WARN);					
				}
				break;
			case GAME_ITEM_MAX_COUNT_OVER:
				break;			
			case GAME_SKILL_TRIED_TO_LEARN_AN_EXISTING_SKILL:
				break;
			case GAME_SKILL_NOT_EXISTING_SKILL:
				break;
			case GAME_SKILL_TRIED_TO_LEARN_WRONG_CLASS_SKILL:
				break;
			case GAME_SKILL_UPGRADE_ONLY_SKILL:
				break;
			case GAME_SKILL_TOO_HIGH_LEVEL_TO_TRAIN:
				break;
			case GAME_SKILL_NOT_ENOUGH_ZENNY:
				break;
			case GAME_COMMON_ZENNY_CANT_BE_USED_RIGHT_NOW:
				break;
			case GAME_SKILL_NO_EMPTY_SKILL_SLOT:
				break;
			case GAME_SKILL_NO_PREREQUISITE_SKILLS_YOU_HAVE:
				break;
			
			default:
				NTL_ASSERTFAIL("Result Code is Not Found");
				break;
			}
		}
	}
	else if(pMsg.Id == g_EventDBCCollect_Nfy)	// 제한 시간 초과
	{
		// 시간이 경과하면 보상 UI를 없애고, 대사 표시후 용신 소멸 상태로 전이한다.
		// 드래곤의 대사 (시간이 너무 걸리는군. 그냥 가겠다)
		sDRAGONBALL_TBLDAT* pData = (sDRAGONBALL_TBLDAT*)m_pDragonBallTable->GetDBTbldat((BYTE)m_pWorldConceptDBC->GetDBKind());
		CDBCDragonDialogGUI::GetInstance()->SetText(pData->timeoverDialog, CDBCDragonDialogGUI::E_DIALOG_WARN);
		CDBCDragonDialogGUI::GetInstance()->ShowNormalDialog(FALSE);

		GetDialogManager()->CloseDialog(DIALOG_DBC_REWARD);
		GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_DRAGONBALL_COLLECT)->ChangeState(WORLD_DBC_DESTROY_DRAGON);
		UnLockDragonBall();
	}
}

void CDBCRewardGUI::CreateInstance() 
{
	// 싱글톤 생성과 함께 다이얼로그 매니저에 등록한다.
	if(!m_pInstance)
	{
		m_pInstance = NTL_NEW CDBCRewardGUI("DBCRewardGUI");
		if(!m_pInstance->Create())
		{
			m_pInstance->Destroy();
			NTL_DELETE(m_pInstance);
		}

		// Gui Manager에 추가한다.
		GetNtlGuiManager()->AddGui(m_pInstance);
		GetDialogManager()->RegistDialog(DIALOG_DBC_REWARD, m_pInstance, &CDBCRewardGUI::SwitchDialog);

        GetDialogManager()->LocationDialogs(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());
	}
}

void CDBCRewardGUI::DeleteInstance() 
{
	if(m_pInstance)
	{
		GetDialogManager()->UnRegistDialog(DIALOG_DBC_REWARD);
		GetNtlGuiManager()->RemoveGui(m_pInstance);
		m_pInstance->Destroy();
		NTL_DELETE(m_pInstance);
	}
}

VOID CDBCRewardGUI::OnClickBackBtn( gui::CComponent* pComponent ) 
{
	// 상위 단계로 돌아간다.
	if(m_pPrevItem)
	{
		ShowList(m_pPrevItem, 0);
		m_pPrevItem = m_pPrevItem->pParent;
	}
}

VOID CDBCRewardGUI::OnClickOkBtn( gui::CComponent* pComponent ) 
{
	// 보상을 선택했다
	if(!m_pRewardItem)
		return;

	// 일반 대화창을 없앤다
	CDBCDragonDialogGUI::GetInstance()->ShowNormalDialog(FALSE);

    if(!m_pDragonBallRewardTable->FindData(m_pRewardItem->pData->tblidx))
    {
        NTL_ASSERTFAIL("Not Data in Reward Table");
        return;
    }

	// 서버에 보상 패킷을 보낸다.
	GetDboGlobal()->GetGamePacketGenerator()->SendDBCRewardReq(m_pWorldConceptDBC->GetAltarSerialID(), m_pRewardItem->pData->tblidx);
}

VOID CDBCRewardGUI::OnClickCancelBtn( gui::CComponent* pComponent ) 
{
	SetStatus(E_REWARD_STATUS_SELECT);
	OnClickBackBtn(pComponent);
}

VOID CDBCRewardGUI::OnPaint() 
{
	if(m_sufItem.GetTexture())
	{
		m_sufItem.Render();
	}
}

gui::CButton* CDBCRewardGUI::OnCreateItem() 
{
	gui::CButton* pButton = NTL_NEW gui::CButton(CRectangle(0, 0, REWARD_ITEM_WIDTH, REWARD_ITEM_HEIGHT), L"", NULL,
												 GetNtlGuiManager()->GetSurfaceManager());

	pButton->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("DBCReward.srf", "srfDBCRewardDlgListFoc"));
	pButton->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("DBCReward.srf", "srfDBCRewardDlgListFoc"));

	pButton->SetTextUpColor(RGB(255, 247, 181));
	pButton->SetTextFocusColor(RGB(255, 247, 220));
	pButton->SetTextDownColor(RGB(255, 247, 181));	    

	pButton->SetTextStyle(0);
	pButton->SetTextCoord(17, 4);
	pButton->SetTextDownCoordDiff(0, 2);

    pButton->ApplyText();

    m_slotListWheelMove[m_nBtnCount] = pButton->SigMouseWheel().Connect(this, &CDBCRewardGUI::OnCaptureWheelMove);    
    ++m_nBtnCount;

	return pButton;
}

void CDBCRewardGUI::OnDestroyItem(gui::CButton* pButton)
{
	NTL_DELETE(pButton);
}

void CDBCRewardGUI::OnClickedItem( RwInt32 iIndex ) 
{
	m_nPrevDepthIdx = m_nDepthIdx;

	CButtonList<CDBCRewardGUI, SRewardItem*>::SItem sItem = m_ButtonList.GetItem(iIndex);

	SRewardItem* pRewardItem = sItem.TData;
	
	if(pRewardItem->itemChild.size() == 0)
	{
		if(pRewardItem->pData->byRewardType == DRAGONBALL_REWARD_TYPE_SKILL) // 이미 가지고 있는 스킬이면 선택하지 못 한다.
		{
			if(IsExistSkill(pRewardItem->pData->rewardLinkTblidx))
				return;
		}

		m_pPrevItem = pRewardItem->pParent;
		// 자식 노드가 없으면 마지막 노드, 즉 보상 항목이다.
		SetStatus(E_REWARD_STATUS_CONFIRM);

		// 아이템 아이콘과 Name을 표시한다.
		ShowItemInfo(pRewardItem);
	}
	else
	{
		m_pPrevItem = pRewardItem->pParent;
		SetStatus(E_REWARD_STATUS_SELECT);
		ShowList(pRewardItem, 0);
	}	
}

void CDBCRewardGUI::ShowList(SRewardItem* pItem, RwInt32 nPos)
{
	m_ButtonList.ClearItem();

	//  용신의 코멘트 표시 (말풍선)
	std::wstring strComment = L"";
	if(pItem->pData && m_pDBRewardTextTable->GetText(pItem->pData->rewardCategoryDialog, &strComment))
	{
		CDBCDragonDialogGUI::GetInstance()->SetText(strComment);
	}	

	// 항목 표시
	for each(SRewardItem* pItemChild in pItem->itemChild)
	{
		// 보상이 스킬인 경우에는 자신의 클래스에 해당하는 스킬만 보상 리스트에 표시한다.
		if(pItemChild->pData->byRewardType == DRAGONBALL_REWARD_TYPE_SKILL)
		{
			sSKILL_TBLDAT* pData = (sSKILL_TBLDAT*)API_GetTableContainer()->GetSkillTable()->FindData(pItemChild->pData->rewardLinkTblidx);
			CNtlSobAvatarAttr* pAvatarAttr = (CNtlSobAvatarAttr*)GetNtlSLGlobal()->GetSobAvatar()->GetSobAttr();
			RwUInt32 uiFlag = pAvatarAttr->GetRaceFlag();
			if((uiFlag & pData->dwPC_Class_Bit_Flag) == 0)
			{
				continue;
			}			
		}
		
		std::wstring strText = m_pDBRewardTextTable->GetText(pItemChild->pData->rewardCategoryName);
		m_ButtonList.AddItem(strText, pItemChild);
	}

	m_ButtonList.SetValue(nPos);

	RwInt32 nMax = pItem->itemChild.size() - REWARD_LIST_MAX_VISIBLE;
	if(nMax < 0)
	{
		nMax = 0;
		m_pScrollBar->Show(FALSE);
	}
	else
	{
		m_pScrollBar->Show(TRUE);
		m_pScrollBar->SetRange(0, nMax);
		m_pScrollBar->SetValue(nPos);
	}
}

void CDBCRewardGUI::CreateRewardTree(SRewardItem* pParent, RwInt32 nDepth)
{
	if(!pParent)
		return;
		
	RwInt32 nIdx = 0;
	RwInt32 nOffset = 0;

	if(pParent->pData)
	{
		nIdx = pParent->pData->tblidx;
	}

	switch(nDepth)
	{
	case 1:	nOffset = 100000; break;
	case 2: nOffset = 10000; break;
	case 3: nOffset = 1000; break;
    case 4: nOffset = 100; break;
    case 5: nOffset = 10; break;
	default: return;
	}

	nIdx += nOffset;

	while(m_pDragonBallRewardTable->FindData(nIdx))
	{
        sDRAGONBALL_REWARD_TBLDAT* pRewardTbldata = (sDRAGONBALL_REWARD_TBLDAT*)m_pDragonBallRewardTable->FindData(nIdx);
        if(pRewardTbldata->byBallType == m_pWorldConceptDBC->GetDBKind())
        {
            SRewardItem* pItem = NTL_NEW SRewardItem();
            pItem->pData = pRewardTbldata;
            pItem->pParent = pParent;
            pParent->itemChild.push_back(pItem);

            CreateRewardTree(pItem, nDepth + 1);
        }		

		nIdx += nOffset;
	}
}

VOID CDBCRewardGUI::SetStatus( ERewardUIStatus eStatus ) 
{
	if(eStatus == E_REWARD_STATUS_SELECT)
	{
		m_pPnlSelectItem->Show(FALSE);
        m_pBtnOK->Show(FALSE);
		m_pBtnCancel->Show(FALSE);
		m_pSttItemName->Show(FALSE);
		m_pSttItemText->Show(FALSE);
		m_sufItem.Show(FALSE);		
		
        m_pPnlSelectMenu->Show(TRUE);
		m_pBtnBack->Show(TRUE);
		m_ButtonList.Show(TRUE);
		m_pScrollBar->Show(TRUE);
		
	}
	else if(eStatus == E_REWARD_STATUS_CONFIRM)
	{
        m_pPnlSelectItem->Show(TRUE);
		m_pBtnOK->Show(TRUE);
		m_pBtnCancel->Show(TRUE);
		m_pSttItemName->Show(TRUE);
		m_pSttItemText->Show(TRUE);
		m_sufItem.Show(TRUE);		

        m_pPnlSelectMenu->Show(FALSE);
		m_pBtnBack->Show(FALSE);
		m_ButtonList.Show(FALSE);
		m_pScrollBar->Show(FALSE);
	}
}

void CDBCRewardGUI::ShowItemInfo( SRewardItem* pItem ) 
{
	if(!pItem || !pItem->pData)
		return;

    std::wstring strRewardName = m_pDBRewardTextTable->GetText(pItem->pData->rewardName);
	if(m_sufItem.GetTexture())
	{
		Logic_DeleteTexture(m_sufItem.GetTexture());
		m_sufItem.UnsetTexture();
	}

	if(pItem->pData->byRewardType == DRAGONBALL_REWARD_TYPE_ITEM)
	{
		// 아이템 아이콘
		m_pRewardItemTblData = API_GetTableContainer()->GetItemTable()->FindData(pItem->pData->rewardLinkTblidx);
		if(m_pRewardItemTblData)
		{
			sITEM_TBLDAT* pTblData = (sITEM_TBLDAT*)m_pRewardItemTblData;
			m_sufItem.SetTexture(Logic_CreateTexture(pTblData->szIcon_Name));
		}
	}
	else if(pItem->pData->byRewardType == DRAGONBALL_REWARD_TYPE_SKILL ||
            pItem->pData->byRewardType == DRAGONBALL_REWARD_TYPE_HTB)
	{
		// 스킬 아이콘
		m_pRewardItemTblData = API_GetTableContainer()->GetSkillTable()->FindData(pItem->pData->rewardLinkTblidx);
		if(m_pRewardItemTblData)
		{
			sSKILL_TBLDAT* pTblData = (sSKILL_TBLDAT*)m_pRewardItemTblData;
			m_sufItem.SetTexture(Logic_CreateTexture(pTblData->szIcon_Name));
		}
	}
	else if(pItem->pData->byRewardType == DRAGONBALL_REWARD_TYPE_ZENNY)
	{
		// 제니 아이콘 (보상 제니는 테이블 액수 * 레벨)
		m_sufItem.SetTexture(Logic_CreateTexture("money.PNG"));
        DWORD dwRewardZenny = pItem->pData->dwRewardZenny * Logic_GetLevel(GetNtlSLGlobal()->GetSobAvatar());
        WCHAR szZenny[64] = {0,};
        swprintf_s(szZenny, GetDisplayStringManager()->GetString("DST_DROPITEM_ZENNY"), Logic_FormatZeni(dwRewardZenny));
        strRewardName = szZenny;    
	}
	else
	{
		NTL_ASSERTFAIL("Not Define Reward Type");
	}

	// 텍스트
	m_pSttItemName->SetText(strRewardName.c_str());

	// 용신 대화
	CDBCDragonDialogGUI::GetInstance()->SetText(m_pDBRewardTextTable->GetText(pItem->pData->rewardCategoryDialog));

	m_pRewardItem = pItem;
}

VOID CDBCRewardGUI::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	m_sufItem.SetRectWH(m_pThis->GetPosition().left + REWARD_ITEM_SUF_X, 
						m_pThis->GetPosition().top + REWARD_ITEM_SUF_Y,
						DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE);
}

VOID CDBCRewardGUI::OnMouseMove( RwInt32 nFlags, RwInt32 nX, RwInt32 nY ) 
{
	if(!m_sufItem.IsShow() || !m_sufItem.GetTexture() || !m_pRewardItem)
		return;

	nX += m_pThis->GetPosition().left;
	nY += m_pThis->GetPosition().top;

	if(m_sufItem.PtInRect(nX, nY))
	{
		if(m_pRewardItem->pData->byRewardType == DRAGONBALL_REWARD_TYPE_ITEM)
		{
			GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_TABLE_ITEM, nX, nY, (VOID*)m_pRewardItemTblData, DIALOG_DBC_REWARD);	
		}
		else if(m_pRewardItem->pData->byRewardType == DRAGONBALL_REWARD_TYPE_SKILL)
		{
			GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_NOTLEARN_SKILL, nX, nY, (VOID*)m_pRewardItemTblData, DIALOG_DBC_REWARD);	
		}		        
	}
	else
	{
		if(GetInfoWndManager()->GetRequestGui() == DIALOG_DBC_REWARD)
		{
			GetInfoWndManager()->ShowInfoWindow(FALSE); 
		}		
	}
}

VOID CDBCRewardGUI::OnMouseLeave( gui::CComponent* pComponent ) 
{
	// 커서가 UI밖으로 나가면 툴팁을 지운다.
	if(GetInfoWndManager()->GetRequestGui() == DIALOG_DBC_REWARD)
	{
		GetInfoWndManager()->ShowInfoWindow(FALSE);
	}
}

VOID CDBCRewardGUI::OnScrollBarChanged( RwInt32 iValue ) 
{
	m_ButtonList.SetValue(iValue);
}

void CDBCRewardGUI::UnLockDragonBall() 
{
	ListSobItem* pListDB = m_pWorldConceptDBC->GetDBList();
	for each(CNtlSobItem* pItem in *pListDB)
	{
		CDboEventGenerator::DialogEvent( DIALOGEVENT_END_UPGRADE_ITEM_IN_BAG, PLACE_ITEMUPGRADE, PLACE_BAG, pItem->GetParentItemSlotIdx(), pItem->GetItemSlotIdx() );
	}
}

RwBool CDBCRewardGUI::IsExistSkill( RwUInt32 nTblIdx ) 
{
	if(GetNtlSLGlobal()->GetSobAvatar()->GetSkillContainer()->GetSkillFromTableId(nTblIdx))
	{
		// 용신의 대사 출력 (그 스킬은 이미 가지고 있어서 선택할 수 없다)
		sDRAGONBALL_TBLDAT* pData = (sDRAGONBALL_TBLDAT*)m_pDragonBallTable->GetDBTbldat((BYTE)m_pWorldConceptDBC->GetDBKind());
		CDBCDragonDialogGUI::GetInstance()->SetText(pData->noRepeatDialog);
		return TRUE;
	}

	return FALSE;
}

VOID CDBCRewardGUI::OnCaptureMouseDown( const CKey& key )
{
    CAPTURE_MOUSEDOWN_RAISE(DIALOG_DBC_REWARD, key.m_fX, key.m_fY);
}

VOID CDBCRewardGUI::OnCaptureWheelMove( RwInt32 nFlag, RwInt16 sDelta, CPos& pos ) 
{
    if(!IsShow() || !m_pScrollBar->IsVisible())
        return;

    RwInt32 nDelta = m_pScrollBar->GetValue() - sDelta / GUI_MOUSE_WHEEL_DELTA;
    nDelta = max(0, min(nDelta, m_pScrollBar->GetMaxValue()));    

    m_pScrollBar->SetValue(nDelta);
}       