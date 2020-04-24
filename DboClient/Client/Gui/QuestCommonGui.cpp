#include "precomp_dboclient.h"
#include "QuestCommonGui.h"

// core
#include "NtlDebug.h"

// shared
#include "QuestTextDataTable.h"
#include "ItemTable.h"
#include "QuestItemTable.h"
#include "SkillTable.h"
#include "HTBSetTable.h"
#include "TableContainer.h"

// gui
#include "GuiUtil.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "DboTSCQAgency.h"
#include "NtlSLApi.h"
#include "DisplayStringManager.h"

// dbo
#include "DialogManager.h"
#include "DboLogic.h"
#include "InfoWndManager.h"

// QuestTextTable의 값이 바뀌면 변경해 주어야함 !!!!
#define QUEST_GRADE_OFFSET_FROM_TABLE 90

CQuestCommonGui::CQuestCommonGui( const RwChar *pName )
: CNtlPLGui( pName )
{
	m_pstbQuestTitle = NULL;
	m_pstbQuestGrade = NULL;

	m_pstbTitle = NULL;
	m_pbtnClose = NULL;
	m_pstbStandardRewardTitle = NULL;
	m_pstbSelectRewardTitle = NULL;

	memset( m_pstbStackCount, 0, sizeof( gui::CStaticBox* ) * QUEST_MAX_TOTAL_REWARD_SLOT );
	memset( m_sReward, 0, sizeof( sREWARD_INFO ) * QUEST_MAX_TOTAL_REWARD_SLOT );

	m_nMouseOnIdx = -1;

	InitQuestData();
	
	m_pQuestGui = NULL;
}

CQuestCommonGui::~CQuestCommonGui(VOID)
{

}

VOID CQuestCommonGui::SetQuestTitle( RwUInt32 uiTitle, RwUInt32 uiSort, RwInt32 eGrade )
{
	// peessi : uiSort 퀘스트텍스트에서 지우고, eQuestSortType데이터로 바꾼다. 
	CQuestTextDataTable* pQuestTextTable = API_GetTableContainer()->GetQuestTextDataTable();
	sQUEST_TEXT_DATA_TBLDAT* pQuestText1 = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( uiTitle ) );
	sQUEST_TEXT_DATA_TBLDAT* pQuestText2 = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( uiSort ) );
	
	m_uiTitleIdx = uiTitle;

	std::wstring wstrText1;
	std::wstring wstrText2;
	std::wstring wstrText3;

	if( pQuestText1 )
		wstrText1 = gui::GetHtmlFromMemoryString( pQuestText1->wstrText.c_str(), pQuestText1->wstrText.size() );

	if( pQuestText2 )
		wstrText2 = gui::GetHtmlFromMemoryString( pQuestText2->wstrText.c_str(), pQuestText2->wstrText.size() );

	m_pstbQuestTitle->SetText( wstrText1.c_str() );
	if( m_pstbQuestTitle->GetFitWidthToString() > m_pstbQuestTitle->GetWidth() )
		m_pstbQuestTitle->SetToolTip( wstrText1.c_str() );
	else
		m_pstbQuestTitle->DeleteToolTip();

	if( eGrade == eGRADE_TYPE_INVALID )
	{
		m_pstbQuestGrade->SetText( wstrText2.c_str() );		
	}
	else
	{		
		sQUEST_TEXT_DATA_TBLDAT* pQuestText3 = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( eGrade + QUEST_GRADE_OFFSET_FROM_TABLE ) );
		
		if( pQuestText3 )
			wstrText3 = gui::GetHtmlFromMemoryString( pQuestText3->wstrText.c_str(), pQuestText3->wstrText.size() );

		m_pstbQuestGrade->Format( L"%s / %s", wstrText2.c_str(), wstrText3.c_str() );
	}

	// set min level : TO DO
	//m_pstbQuestLevel->Format(L"%s / %s);
}

VOID CQuestCommonGui::SetRewardRectHardcode(VOID)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	for( RwInt32 i = 0 ; i < QUEST_MAX_REWARD_SLOT ; ++i )
	{	
		m_rtReward[i].left		= QUEST_REWARDSLOT_X + QUEST_REWARDSLOT_X_DIFF * i;
		m_rtReward[i].top		= QUEST_REWARDSLOT_Y2;
		m_rtReward[i].right		= m_rtReward[i].left + DBOGUI_ICON_SIZE;
		m_rtReward[i].bottom	= m_rtReward[i].top + DBOGUI_ICON_SIZE;

		m_rtReward[i+QUEST_MAX_REWARD_SLOT].left		= QUEST_REWARDSLOT_X + QUEST_REWARDSLOT_X_DIFF * i;
		m_rtReward[i+QUEST_MAX_REWARD_SLOT].top			= QUEST_REWARDSLOT_Y;
		m_rtReward[i+QUEST_MAX_REWARD_SLOT].right		= m_rtReward[i+QUEST_MAX_REWARD_SLOT].left + DBOGUI_ICON_SIZE;
		m_rtReward[i+QUEST_MAX_REWARD_SLOT].bottom		= m_rtReward[i+QUEST_MAX_REWARD_SLOT].top + DBOGUI_ICON_SIZE;

		m_surReward[i].SetRectWH( rtScreen.left + m_rtReward[i].left, rtScreen.top + m_rtReward[i].top, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );
		m_surReward[i+QUEST_MAX_REWARD_SLOT].SetRectWH( rtScreen.left + m_rtReward[i+QUEST_MAX_REWARD_SLOT].left, rtScreen.top + m_rtReward[i+QUEST_MAX_REWARD_SLOT].top, DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE );
	}

	m_pstbStandardRewardTitle->SetText( GetDisplayStringManager()->GetString( "DST_QUESTREWARD_TEXT_BASIC" ) );
	m_pstbSelectRewardTitle->SetText( GetDisplayStringManager()->GetString( "DST_QUESTREWARD_TEXT_SELECT" ) );

	m_pstbRewardGetExpText->SetText(GetDisplayStringManager()->GetString("DST_QUESTREWARD_TEXT_GET_EXP"));
	m_pstbRewardGetZennyText->SetText(GetDisplayStringManager()->GetString("DST_QUESTREWARD_TEXT_GET_ZENNY"));
	m_pstbRewardSelectText->SetText(GetDisplayStringManager()->GetString("DST_QUESTREWARD_TEXT_SEL_CNT"));
	m_pstbRewardSelectAmountText->SetText(L"0 / 1");

	m_pstbRewardSelectText->Show(false);
	m_pstbRewardSelectAmountText->Show(false);
}

VOID CQuestCommonGui::SetRewardSlot( RwInt32 nIdx, sREWARD_INFO *pRewardInfo)
{
	if( m_sReward[nIdx].m_eType != eREWARD_TYPE_INVALID )
		DeleteRewardSlot( nIdx );

	if( pRewardInfo->m_eType == eREWARD_TYPE_INVALID )
		return;

	// Data 복사.
	m_sReward[nIdx] = (*pRewardInfo);

	switch( pRewardInfo->m_eType )
	{
	case eREWARD_TYPE_NORMAL_ITEM:
		{
			CItemTable* pItemTable = API_GetTableContainer()->GetItemTable();
			sITEM_TBLDAT* pTableData = reinterpret_cast<sITEM_TBLDAT*>( pItemTable->FindData( pRewardInfo->m_uiIdx ) );
			if( pTableData )
			{
				m_surReward[nIdx].SetTexture( Logic_CreateTexture( const_cast<RwChar*>( pTableData->szIcon_Name ) ) );
				if( pTableData->byMax_Stack > 1 )
					CreateStackNumber( nIdx, pRewardInfo->m_nValue );			
			}
		}		
		break;
	case eREWARD_TYPE_QUEST_ITEM:
		{
			CQuestItemTable* pQuestItemTable = API_GetTableContainer()->GetQuestItemTable();
			sQUESTITEM_TBLDAT* pTableData = reinterpret_cast<sQUESTITEM_TBLDAT*>( pQuestItemTable->FindData( pRewardInfo->m_uiIdx ) );
			if( pTableData )
			{
				m_surReward[nIdx].SetTexture( Logic_CreateTexture( const_cast<RwChar*>( pTableData->szIconName ) ) );
				CreateStackNumber( nIdx, pRewardInfo->m_nValue );
			}			
		}
		break;
	case eREWARD_TYPE_EXP:
		m_surReward[nIdx].SetTexture( Logic_CreateTexture( QUEST_REWARD_ICONNAME_EXP ) );
		break;
	case eREWARD_TYPE_SKILL:
	case eREWARD_TYPE_BUFF:
		{
			// peessi : REWARD의 경우, 스킬이 HTB인지 아닌지의 구분이 없이 모두 스킬이므로 예외처리 해주어야 한다.
			CSkillTable* pSkillTable = API_GetTableContainer()->GetSkillTable();
			sSKILL_TBLDAT* pTableData = reinterpret_cast<sSKILL_TBLDAT*>( pSkillTable->FindData( pRewardInfo->m_uiIdx ) );
			if( pTableData )
			{
				if( pTableData->bySkill_Class == NTL_SKILL_CLASS_HTB )
				{
					CHTBSetTable* pHTBTable = API_GetTableContainer()->GetHTBSetTable();
					sHTB_SET_TBLDAT* pHTBData = reinterpret_cast<sHTB_SET_TBLDAT*>( pHTBTable->FindData( pRewardInfo->m_uiIdx ) );

					m_surReward[nIdx].SetTexture( Logic_CreateTexture( const_cast<RwChar*>( pHTBData->szIcon_Name ) ) );
				}
				else
				{
					m_surReward[nIdx].SetTexture( Logic_CreateTexture( const_cast<RwChar*>( pTableData->szIcon_Name ) ) );
				}				
			}
		}
		break;
	case eREWARD_TYPE_ZENY:
		m_surReward[nIdx].SetTexture( Logic_CreateTexture( QUEST_REWARD_ICONNAME_ZENY ) );
		break;
	case eREWARD_TYPE_CHANGE_CLASS:
		m_surReward[nIdx].SetTexture( Logic_CreateTexture( QUEST_REWARD_ICONNAME_CLASS ) );
		break;
	case eREWARD_TYPE_PROBABILITY:
		m_surReward[nIdx].SetTexture( Logic_CreateTexture( QUEST_REWARD_ICONNAME_PROB ) );
		break;
	case eREWARD_TYPE_REPUTATION:
		// peessitemp: 현재 더미 텍스쳐.
		m_surReward[nIdx].SetTexture( Logic_CreateTexture( QUEST_REWARD_ICONNAME_REPUTATION ) );
		break;
	case eREWARD_TYPE_CHANGE_ADULT:
		m_surReward[nIdx].SetTexture( Logic_CreateTexture( QUEST_REWARD_ICONNAME_CHANGE_ADULT ) );
		break;
	case eREWARD_TYPE_GET_CONVERT_CLASS_RIGHT:
		m_surReward[nIdx].SetTexture( Logic_CreateTexture( QUEST_REWARD_ICONNAME_GET_CONVERT_CLASS_RIGHT ) );
		break;

	default: DBO_WARNING_MESSAGE("eREWARD_TYPE_USE_SELECT_REWARD_TABLE not set"); break;
	}
}

VOID CQuestCommonGui::DeleteRewardSlot( RwInt32 nIdx )
{
	m_sReward[nIdx].m_eType = eREWARD_TYPE_INVALID;
	Logic_DeleteTexture( m_surReward[nIdx].GetTexture() );
	m_surReward[nIdx].UnsetTexture();
	DeleteStackNumber( nIdx );
}

VOID CQuestCommonGui::DeleteAllRewardSlot(VOID)
{
	for( RwInt32 i = 0 ; i < QUEST_MAX_TOTAL_REWARD_SLOT ; ++i )
	{
		DeleteRewardSlot( i );
	}
}

VOID CQuestCommonGui::ShowInfoWnd( RwInt32 nIdx )
{
	if( m_sReward[nIdx].m_eType == eREWARD_TYPE_INVALID )
		return;

	CRectangle rtScreen = m_pThis->GetScreenRect();

	stINFOWND_REWARD stRewardInfo;
	stRewardInfo.eType = m_sReward[nIdx].m_eType;
	stRewardInfo.nValue = m_sReward[nIdx].m_nValue;

	RwInt32 eState = CInfoWndManager::INFOWND_REWARD;
	VOID*   pValue = reinterpret_cast<VOID*>( &stRewardInfo );	

	switch( m_sReward[nIdx].m_eType )
	{
	case eREWARD_TYPE_NORMAL_ITEM:
		{
			CItemTable* pItemTable = API_GetTableContainer()->GetItemTable();
			sITEM_TBLDAT* pTableData = reinterpret_cast<sITEM_TBLDAT*>( pItemTable->FindData( m_sReward[nIdx].m_uiIdx ) );

			eState = CInfoWndManager::INFOWND_TABLE_ITEM;
			pValue = reinterpret_cast<VOID*>( pTableData );
		}
		break;
	case eREWARD_TYPE_QUEST_ITEM:
		{
			CQuestItemTable* pQuestItemTable = API_GetTableContainer()->GetQuestItemTable();
			sQUESTITEM_TBLDAT* pTableData = reinterpret_cast<sQUESTITEM_TBLDAT*>( pQuestItemTable->FindData( m_sReward[nIdx].m_uiIdx ) );

			eState = CInfoWndManager::INFOWND_QUESTITEM;
			pValue = reinterpret_cast<VOID*>( pTableData );
		}
		break;
	case eREWARD_TYPE_SKILL:
		{
			// peessi : REWARD의 경우, 스킬이 HTB인지 아닌지의 구분이 없이 모두 스킬이므로 예외처리 해주어야 한다.
			CSkillTable* pSkillTable = API_GetTableContainer()->GetSkillTable();
			sSKILL_TBLDAT* pTableData = reinterpret_cast<sSKILL_TBLDAT*>( pSkillTable->FindData( m_sReward[nIdx].m_uiIdx ) );

			if( pTableData->bySkill_Class == NTL_SKILL_CLASS_HTB )
			{
				CHTBSetTable* pHTBTable = API_GetTableContainer()->GetHTBSetTable();
				sHTB_SET_TBLDAT* pHTBData = reinterpret_cast<sHTB_SET_TBLDAT*>( pHTBTable->FindData( m_sReward[nIdx].m_uiIdx ) );

				eState = CInfoWndManager::INFOWND_NOTLEARN_HTBSKILL;
				pValue = reinterpret_cast<VOID*>( pHTBData );
			}
			else
			{
				eState = CInfoWndManager::INFOWND_NOTLEARN_SKILL;
				pValue = reinterpret_cast<VOID*>( pTableData );
			}							
		}
		break;
	case eREWARD_TYPE_CHANGE_CLASS:
		stRewardInfo.nValue = m_sReward[nIdx].m_uiIdx;
		break;
	case eREWARD_TYPE_BUFF:
		{
			CSkillTable* pSkillTable = API_GetTableContainer()->GetSkillTable();
			sSKILL_TBLDAT* pTableData = reinterpret_cast<sSKILL_TBLDAT*>( pSkillTable->FindData( m_sReward[nIdx].m_uiIdx ) );

			eState = CInfoWndManager::INFOWND_NOTSOBBUFF;
			pValue = reinterpret_cast<VOID*>( pTableData );
		}
		break;	
	}

	GetInfoWndManager()->ShowInfoWindow( TRUE, (CInfoWndManager::eINFOWNDSTATE)eState,
		m_rtReward[nIdx].left + rtScreen.left,	m_rtReward[nIdx].top + rtScreen.top,
		pValue, DIALOG_QUESTREWARD );
}

RwInt32 CQuestCommonGui::GetRewardSlotIdx( RwInt32 nX, RwInt32 nY )
{
	for( RwInt32 i = 0 ; i < QUEST_MAX_TOTAL_REWARD_SLOT ; ++i )
	{
		if( m_rtReward[i].PtInRect( nX, nY ) )
			return i;
	}

	return -1;
}

RwBool CQuestCommonGui::IsAvailableSelect(RwInt32 nIdx)
{
	if (nIdx < 0 || nIdx > QUEST_MAX_REWARD_SLOT - 1)
		return FALSE;

	if (m_sReward[nIdx].m_eType != eREWARD_TYPE_INVALID)
		return TRUE;

	return FALSE;
}

RwBool CQuestCommonGui::CreateStackNumber( RwInt32 nSlotIdx, RwInt32 nValue )
{
	NTL_FUNCTION( "CQuestInventoryGui::CreateStackNubmer" );
	
	CRectangle rect;
	rect.SetRect( m_rtReward[nSlotIdx].left, m_rtReward[nSlotIdx].bottom - DBOGUI_STACKNUM_HEIGHT,
		m_rtReward[nSlotIdx].left + DBOGUI_STACKNUM_WIDTH, m_rtReward[nSlotIdx].bottom );
	m_pstbStackCount[nSlotIdx] = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), DBOGUI_STACKNUM_ALIGN );
	if( !m_pstbStackCount[nSlotIdx] )
		NTL_RETURN( FALSE );

	m_pstbStackCount[nSlotIdx]->CreateFontStd( DBOGUI_STACKNUM_FONT, DBOGUI_STACKNUM_FONTHEIGHT, DBOGUI_STACKNUM_FONTATTR );
	m_pstbStackCount[nSlotIdx]->SetEffectMode( DBOGUI_STACKNUM_FONTEFFECTMODE );
	m_pstbStackCount[nSlotIdx]->SetText( nValue );
	m_pstbStackCount[nSlotIdx]->Enable( false );

	NTL_RETURN( TRUE );
}

VOID CQuestCommonGui::DeleteStackNumber( RwInt32 nSlotIdx )
{
	if( m_pstbStackCount[nSlotIdx] )
		NTL_DELETE( m_pstbStackCount[nSlotIdx] );
}

VOID CQuestCommonGui::InitQuestData(VOID)
{
	m_pTCUnit = NULL;
	m_TSKey.Init();
	m_uiTitleIdx = INVALID_TBLIDX;
}

VOID CQuestCommonGui::OnMouseMove( RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos )
{
	RwInt32 nMouseOnIdx = GetRewardSlotIdx( nXPos, nYPos );

	if( nMouseOnIdx >= 0 )
	{
		if( m_nMouseOnIdx != nMouseOnIdx )
		{
			ShowInfoWnd( nMouseOnIdx );
			m_nMouseOnIdx = nMouseOnIdx;
		}
	}
	else
	{
		if( m_nMouseOnIdx >= 0 )
		{
			GetInfoWndManager()->ShowInfoWindow( FALSE );
			m_nMouseOnIdx = -1;
		}
	}
}

VOID CQuestCommonGui::OnMouseOut( gui::CComponent* pComponent )
{
	if( m_nMouseOnIdx >= 0 )
	{
		GetInfoWndManager()->ShowInfoWindow( FALSE );
		m_nMouseOnIdx = -1;
	}
}

VOID CQuestCommonGui::OnPaint(VOID)
{
	for( RwInt32 i = 0 ; i < QUEST_MAX_TOTAL_REWARD_SLOT ; ++i )
	{
		if( m_surReward[i].GetTexture() )
			m_surReward[i].Render();
	}
}

VOID CQuestCommonGui::OnMove( RwInt32 nX, RwInt32 nY )
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	for( RwInt32 i = 0 ; i < QUEST_MAX_TOTAL_REWARD_SLOT ; ++i )
	{
		m_surReward[i].SetPosition( m_rtReward[i].left + rtScreen.left, m_rtReward[i].top + rtScreen.top );
	}
}

