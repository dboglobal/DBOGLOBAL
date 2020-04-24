#include "precomp_dboclient.h"
#include "LittleQuestContainer.h"

// core
#include "NtlDebug.h"

// share
#include "QuestTextDataTable.h"
#include "MobTable.h"
#include "QuestItemTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"

#include "GuiUtil.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLEventfunc.h"
#include "NtlSLTMQ.h"
#include "NtlSobAvatar.h"
#include "DboTSCQCtrl.h"
#include "DboTSCQAgency.h"
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"
#include "NtlWorldconceptTMQ.h"

// dbo
#include "SideDialogManager.h"
#include "DialogManager.h"
#include "QuestListGui.h"
#include "DboEventGenerator.h"
#include "DisplayStringManager.h"

namespace
{
#define QUEST_INDICATOR_COLOR_GRAY	RGB( 168, 168, 168 )
#define QUEST_INDICATOR_COLOR_RED	RGB( 255, 100, 100 )
#define QUEST_INDICATOR_COLOR_BLUE	RGB( 100, 255, 100 )
#define QUEST_INDICATOR_SHADOW_COLOR	RGB( 0, 0, 0 )
}

CLittleQuestContainer::CLittleQuestContainer()
:m_tID(NTL_TS_T_ID_INVALID)
,m_uiQuestType((RwUInt32)eQUEST_SORT_TYPE_INVALID)
{
}	

CLittleQuestContainer::~CLittleQuestContainer()
{

}

RwBool CLittleQuestContainer::Create(RwUInt32 uiQuestType, gui::CStaticBox* pQuestTitle, gui::CStaticBox* pAim_1, gui::CStaticBox* pAim_2,
									 gui::CStaticBox* pAim_3, gui::CStaticBox* pAim_4)
{
	NTL_FUNCTION( "CLittleQuestContainer::Create" );

	m_uiQuestType	= uiQuestType;

	m_pQuestTitle	= pQuestTitle;
	m_pQuestAim[0]	= pAim_1;
	m_pQuestAim[1]	= pAim_2;
	m_pQuestAim[2]	= pAim_3;
	m_pQuestAim[3]	= pAim_4;

	// Set default data
	InitQuestData();

	LinkMsg(g_EventRegQuest_Nfy);
	LinkMsg(g_EventUpdateQuestState_Nfy);
	LinkMsg(g_EventUpdateQuestProgressInfo_Nfy);
	LinkMsg(g_EventGiveUpQuest);
	LinkMsg(g_EventUnregQuest_Nfy);
	LinkMsg(g_EventTSRemovingTMQQuest_Nfy);

	NTL_RETURN(TRUE);
}

VOID CLittleQuestContainer::Destroy()
{
	NTL_FUNCTION("CLittleQuestContainer::Destroy");

	LIST_QUEST_ITER it = m_listQuestData.begin();
	for( ; it != m_listQuestData.end() ; ++it )
	{
		sQuestData* pQuestData = *it;
		NTL_DELETE(pQuestData);
	}

	UnLinkMsg(g_EventRegQuest_Nfy);
	UnLinkMsg(g_EventUpdateQuestState_Nfy);
	UnLinkMsg(g_EventUpdateQuestProgressInfo_Nfy);
	UnLinkMsg(g_EventGiveUpQuest);
	UnLinkMsg(g_EventUnregQuest_Nfy);
	UnLinkMsg(g_EventTSRemovingTMQQuest_Nfy);

	NTL_RETURNVOID();
}

VOID CLittleQuestContainer::InitQuestData()
{
	std::list<stQUESTLISTDATA> listQuest;
	CQuestListGui* pQuestListGui = reinterpret_cast<CQuestListGui*>(GetDialogManager()->GetDialog(DIALOG_QUESTLIST));

	CDboTSCQAgency* pQuestAgency = API_GetQuestAgency();
	if ( NULL == pQuestAgency ) return;

	// Cleint 단의 Gui에서 진행중인 퀘스트 리스트를 얻어온다
	pQuestListGui->GetQuestList(&listQuest);

	for( std::list<stQUESTLISTDATA>::iterator it = listQuest.begin() ; it != listQuest.end() ; ++it )
	{
		if( (*it).m_uiSortType == m_uiQuestType )
		{			
			CDboTSCQCtrl* pQuestCtr = reinterpret_cast<CDboTSCQCtrl*>(pQuestAgency->FindProgressTrigger( (*it).m_TSKey.tID ));
			if( pQuestCtr == NULL )
			{
				NTL_ASSERT(false, "CLittleQuestContainer::InitQuestData, Not exist ts data of id : " << (*it).m_TSKey.tID);
				continue;
			}

			sQuestData* pQuestData = NTL_NEW sQuestData;

			pQuestData->sTSKey			= (*it).m_TSKey;
			pQuestData->uiSortType		= (*it).m_uiSortType;
			pQuestData->uiQuestTitle	= pQuestCtr->GetTitle();
			pQuestData->eEvtInfoType	= pQuestCtr->GetServerEvtDataType();
			pQuestData->uEvtInfoData	= pQuestCtr->GetServerEvtData();
			pQuestData->uiTimeLimit		= pQuestCtr->GetLimitTime();	

			m_listQuestData.push_back(pQuestData);
		}
	}

	// 관련 퀘스트가 하나 이상 있을 때 가장 처음 것을 보여준다
	if( m_listQuestData.size() > 0 )
	{
		LIST_QUEST_ITER it = m_listQuestData.begin();
		SetIndicator(*it);
	}
}

VOID CLittleQuestContainer::SetIndicator(sQuestData* pQuestData)
{
	m_pQuestTitle->Show(false);

	for( RwInt8 i = 0 ; i < dMAX_QUEST_AIM ; ++i )
		m_pQuestAim[i]->Show(false);


	if(pQuestData == NULL)
	{
		m_tID = NTL_TS_T_ID_INVALID;
		return;
	}


	// 현재 보여지고 있는 퀘스트 ID 보관
	m_tID = pQuestData->sTSKey.tID;

	RwUInt32 uiTextColor = 0;
	RwInt32 iProgress = 0;

	CQuestTextDataTable* pQuestTextTable = API_GetTableContainer()->GetQuestTextDataTable();
	sQUEST_TEXT_DATA_TBLDAT* pQUEST_TEXT_DATA_TBLDAT = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData(pQuestData->uiQuestTitle) );
	if( pQUEST_TEXT_DATA_TBLDAT == NULL )
	{
		NTL_ASSERT(false, "CLittleQuestContainer::SetIndicator, Not exist Quest text data table of index " << pQuestData->uiQuestTitle);
		return;
	}	

	// 퀘스트 제목
	std::wstring wTitlestring = gui::GetHtmlFromMemoryString(pQUEST_TEXT_DATA_TBLDAT->wstrText.c_str(), pQUEST_TEXT_DATA_TBLDAT->wstrText.size());
	m_pQuestTitle->Show(true);
	m_pQuestTitle->SetText(wTitlestring.c_str());

	if( pQuestData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_MOB_KILL_CNT )
	{
		CMobTable* pMobTable = API_GetTableContainer()->GetMobTable();
		CTextTable* pMobText = API_GetTableContainer()->GetTextAllTable()->GetMobTbl();

		for( RwInt32 j = 0 ; j < uSTOC_EVT_DATA::MAX_MOB_KILL ; ++j )
		{
			RwUInt32 nTblIdx = pMobTable->FindTblidxByGroup( pQuestData->uEvtInfoData.sMobKillCnt[j].uiMobIdx );
			sMOB_TBLDAT* pMobData = reinterpret_cast<sMOB_TBLDAT*>( pMobTable->FindData( nTblIdx ) );
			if( !pMobData )
				continue;

			if( pQuestData->uEvtInfoData.sMobKillCnt[j].nCurMobCnt >= pQuestData->uEvtInfoData.sMobKillCnt[j].nMobCnt )
				uiTextColor = QUEST_INDICATOR_COLOR_GRAY;
			else
				uiTextColor = QUEST_INDICATOR_COLOR_BLUE;

			m_pQuestAim[iProgress]->Show(true);
			m_pQuestAim[iProgress]->SetTextColor(uiTextColor, TRUE);
			m_pQuestAim[iProgress]->Format(L"%s : %d/%d",
				pMobText->GetText( pMobData->Name ).c_str(),
				pQuestData->uEvtInfoData.sMobKillCnt[j].nCurMobCnt,
				pQuestData->uEvtInfoData.sMobKillCnt[j].nMobCnt);

			++iProgress;
		}
	}
	else if( pQuestData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_MOB_KILL_ITEM_CNT )
	{
		CQuestItemTable* pItemTable = API_GetTableContainer()->GetQuestItemTable();
		CTextTable* pQuestItemText = API_GetTableContainer()->GetTextAllTable()->GetQuestItemTbl();

		for( RwInt32 j = 0 ; j < uSTOC_EVT_DATA::MAX_MOB_KILL_ITEM ; ++j )
		{
			sQUESTITEM_TBLDAT* pItemData = reinterpret_cast<sQUESTITEM_TBLDAT*>( pItemTable->FindData( pQuestData->uEvtInfoData.sMobKillItemCnt[j].uiMobLIIdx ) );
			if( !pItemData )
				continue;

			if( pQuestData->uEvtInfoData.sMobKillItemCnt[j].nCurMobLICnt >= pQuestData->uEvtInfoData.sMobKillItemCnt[j].nMobLICnt )
				uiTextColor = QUEST_INDICATOR_COLOR_GRAY;
			else
				uiTextColor = QUEST_INDICATOR_COLOR_BLUE;

			m_pQuestAim[iProgress]->Show(true);
			m_pQuestAim[iProgress]->SetTextColor(uiTextColor, TRUE);
			m_pQuestAim[iProgress]->Format(L"%s : %d/%d",
				pQuestItemText->GetText( pItemData->ItemName ).c_str(),
				pQuestData->uEvtInfoData.sMobKillItemCnt[j].nCurMobLICnt,
				pQuestData->uEvtInfoData.sMobKillItemCnt[j].nMobLICnt);

			++iProgress;
		}			
	}
	else if( pQuestData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_DELIVERY_ITEM )
	{
		CQuestItemTable* pItemTable = API_GetTableContainer()->GetQuestItemTable();
		CTextTable* pQuestItemText = API_GetTableContainer()->GetTextAllTable()->GetQuestItemTbl();

		for( RwInt32 j = 0 ; j < uSTOC_EVT_DATA::MAX_DELIVERY_ITEM ; ++j )
		{
			sQUESTITEM_TBLDAT* pItemData = reinterpret_cast<sQUESTITEM_TBLDAT*>( pItemTable->FindData( pQuestData->uEvtInfoData.sDeliveryItemCnt[j].uiItemIdx ) );

			if( !pItemData )
				continue;

			if( pQuestData->uEvtInfoData.sDeliveryItemCnt[j].nCurItemCnt >= pQuestData->uEvtInfoData.sDeliveryItemCnt[j].nItemCnt )
				uiTextColor = QUEST_INDICATOR_COLOR_GRAY;
			else
				uiTextColor = QUEST_INDICATOR_COLOR_BLUE;

			m_pQuestAim[iProgress]->Show(true);
			m_pQuestAim[iProgress]->SetTextColor(uiTextColor, TRUE);
			m_pQuestAim[iProgress]->Format(L"%s : %d/%d",
				pQuestItemText->GetText( pItemData->ItemName ).c_str(),
				pQuestData->uEvtInfoData.sDeliveryItemCnt[j].nCurItemCnt,
				pQuestData->uEvtInfoData.sDeliveryItemCnt[j].nItemCnt);

			++iProgress;
		}
	}
	else if( pQuestData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_OBJECT_ITEM )
	{
		CQuestItemTable* pItemTable = API_GetTableContainer()->GetQuestItemTable();
		CTextTable* pQuestItemText = API_GetTableContainer()->GetTextAllTable()->GetQuestItemTbl();

		for( RwInt32 j = 0 ; j < uSTOC_EVT_DATA::MAX_OBJECT_ITEM ; ++j )
		{
			sQUESTITEM_TBLDAT* pItemData = reinterpret_cast<sQUESTITEM_TBLDAT*>( pItemTable->FindData( pQuestData->uEvtInfoData.sObjectItemCnt[j].uiItemIdx ) );

			if( !pItemData )
				continue;

			if( pQuestData->uEvtInfoData.sObjectItemCnt[j].nCurItemCnt >= pQuestData->uEvtInfoData.sObjectItemCnt[j].nItemCnt )
				uiTextColor = QUEST_INDICATOR_COLOR_GRAY;
			else
				uiTextColor = QUEST_INDICATOR_COLOR_BLUE;

			m_pQuestAim[iProgress]->Show(true);
			m_pQuestAim[iProgress]->SetTextColor(uiTextColor, TRUE);
			m_pQuestAim[iProgress]->Format(L"%s : %d/%d",
				pQuestItemText->GetText( pItemData->ItemName ).c_str(),
				pQuestData->uEvtInfoData.sObjectItemCnt[j].nCurItemCnt,
				pQuestData->uEvtInfoData.sObjectItemCnt[j].nItemCnt);

			++iProgress;
		}
	}
	else if( pQuestData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_PUBLIC_MOB_ITEM_CNT )
	{
		CQuestItemTable* pItemTable = API_GetTableContainer()->GetQuestItemTable();
		CTextTable* pQuestItemText = API_GetTableContainer()->GetTextAllTable()->GetQuestItemTbl();

		for( RwInt32 j = 0 ; j < uSTOC_EVT_DATA::MAX_PUBLIC_MOB_ITEM ; ++j )
		{
			sQUESTITEM_TBLDAT* pItemData = reinterpret_cast<sQUESTITEM_TBLDAT*>( pItemTable->FindData( pQuestData->uEvtInfoData.sPublicMobItemCnt[j].uiItemIdx ) );
			//sQUESTITEM_TBLDAT* pRequireItemData = reinterpret_cast<sQUESTITEM_TBLDAT*>( pItemTable->FindData( pQuestData->uEvtInfoData.sPublicMobItemCnt[j].uiRequireItemIdx ) );

			if( !pItemData )
				continue;

			if( pQuestData->uEvtInfoData.sPublicMobItemCnt[j].nCurItemCnt >= pQuestData->uEvtInfoData.sPublicMobItemCnt[j].nItemCnt )
				uiTextColor = QUEST_INDICATOR_COLOR_GRAY;
			else
				uiTextColor = QUEST_INDICATOR_COLOR_BLUE;
			/*
			m_pQuestAim[0]->Show(true);
			m_pQuestAim[0]->SetTextColor(uiTextColor, TRUE);
			m_pQuestAim[0]->Format(L"%s%s", GetDisplayStringManager()->GetString(DST_TMQ_BALLOON_NEED_ITEM), pQuestItemText->GetText( pRequireItemData->ItemName ).c_str());

			m_pQuestAim[1]->Show(true);
			m_pQuestAim[1]->SetTextColor(uiTextColor, TRUE);
			m_pQuestAim[1]->Format(L"%s%d", GetDisplayStringManager()->GetString(DST_TMQ_INDICATE_DROPRATE), pQuestData->uEvtInfoData.sPublicMobItemCnt[j].fDropRate * 100);
			*/
			m_pQuestAim[0]->Show(true);
			m_pQuestAim[0]->SetTextColor(uiTextColor, TRUE);
			m_pQuestAim[0]->Format(L"%s : %d/%d",
				pQuestItemText->GetText( pItemData->ItemName ).c_str(),
				pQuestData->uEvtInfoData.sPublicMobItemCnt[j].nCurItemCnt,
				pQuestData->uEvtInfoData.sPublicMobItemCnt[j].nItemCnt);

			iProgress = 3;
		}
	}
	else if( pQuestData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_CUSTOM_EVT_CNT )
	{
		for( RwInt32 j = 0 ; j < uSTOC_EVT_DATA::MAX_CUSTOM_EVT_CNT ; ++j )
		{
			pQUEST_TEXT_DATA_TBLDAT = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( pQuestData->uEvtInfoData.sCustomEvtCnt[j].uiQTextTblIdx ) );

			if( !pQUEST_TEXT_DATA_TBLDAT )
				continue;

			if( pQuestData->uEvtInfoData.sMobKillCnt[j].nCurMobCnt >= pQuestData->uEvtInfoData.sMobKillCnt[j].nMobCnt )
				uiTextColor = QUEST_INDICATOR_COLOR_GRAY;
			else
				uiTextColor = QUEST_INDICATOR_COLOR_BLUE;


			std::wstring& wstrText = gui::GetHtmlFromMemoryString(pQUEST_TEXT_DATA_TBLDAT->wstrText.c_str(), pQUEST_TEXT_DATA_TBLDAT->wstrText.size());

			m_pQuestAim[iProgress]->Show(true);
			m_pQuestAim[iProgress]->SetTextColor(uiTextColor, TRUE);
			m_pQuestAim[iProgress]->Format(L"%s : %d/%d",
				wstrText.c_str(),
				pQuestData->uEvtInfoData.sCustomEvtCnt[j].nCurCnt,
				pQuestData->uEvtInfoData.sCustomEvtCnt[j].nMaxCnt);

			++iProgress;
		}
	}
	else if( pQuestData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_VISIT )
	{
		//CQuestItemTable* pItemTable = API_GetTableContainer()->GetQuestItemTable();
		//CTextTable* pQuestItemText = API_GetTableContainer()->GetTextAllTable()->GetQuestItemTbl();

		for( RwInt32 j = 0 ; j < uSTOC_EVT_DATA::MAX_VISIT_EVT ; ++j )
		{
			sQUEST_TEXT_DATA_TBLDAT* pQuestTextData = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( pQuestData->uEvtInfoData.sVisitEvt[j].uiIndicatorQText ) );

			if( !pQuestTextData )
				continue;

			if( pQuestData->uEvtInfoData.sVisitEvt[j].bCompleted )
				uiTextColor = QUEST_INDICATOR_COLOR_GRAY;
			else
				uiTextColor = QUEST_INDICATOR_COLOR_BLUE;

			std::wstring& wstrText = gui::GetHtmlFromMemoryString( pQuestTextData->wstrText.c_str(), pQuestTextData->wstrText.size() );

			m_pQuestAim[iProgress]->Show(true);
			m_pQuestAim[iProgress]->SetTextColor(uiTextColor, TRUE);
			m_pQuestAim[iProgress]->Format(L"%s", wstrText.c_str());

			++iProgress;
		}
	}	

	// 시간.
	if( pQuestData->uiTimeLimit != 0xffffffff )
	{
		if( iProgress < dMAX_QUEST_AIM )
		{
			RwUInt32 uiTime;
			RwUInt32 uiMinute;
			RwUInt32 uiSecond;

			if( pQuestData->uiTimeLimit <= 0 )
			{
				uiMinute = 0;
				uiSecond = 0;
				uiTextColor = QUEST_INDICATOR_COLOR_GRAY;
			}
			else
			{
				uiTime = pQuestData->uiTimeLimit / 1000;
				uiMinute = uiTime / 60;
				uiSecond = uiTime % 60;
				uiTextColor = QUEST_INDICATOR_COLOR_BLUE;
			}

			m_pQuestAim[iProgress]->Show(true);
			m_pQuestAim[iProgress]->SetTextColor(uiTextColor, TRUE);
			m_pQuestAim[iProgress]->Format(L"%s%02d:%02d", GetDisplayStringManager()->GetString("DST_TMQ_BALLOON_REMAIN_TIME"), uiMinute, uiSecond);
		}
		else
		{
			NTL_ASSERT(false, "CLittleQuestContainer::SetIndicator, more need staticbox");
		}
	}
}

VOID CLittleQuestContainer::NextQuest()
{
	sQuestData* pQuestData = NextData(m_tID);

	if( pQuestData )
		SetIndicator(pQuestData);
}

CLittleQuestContainer::sQuestData* CLittleQuestContainer::FindData(NTL_TS_T_ID tID)
{
	LIST_QUEST_ITER it = m_listQuestData.begin();
	for( ; it != m_listQuestData.end() ;++it )
	{
		if( (*it)->sTSKey.tID == tID )
			return *it;
	}

	return NULL;
}

CLittleQuestContainer::sQuestData* CLittleQuestContainer::NextData(NTL_TS_T_ID tID_current)
{
	if( m_listQuestData.size() <= 1 )
		return NULL;

	LIST_QUEST_ITER it = m_listQuestData.begin();
	for( ; it != m_listQuestData.end() ;++it )
	{
		if( (*it)->sTSKey.tID == tID_current )
		{
			LIST_QUEST_ITER it_next = it;

			++it_next;

			// 리스트에서 다음 퀘스트가 없으면 가장 처음 데이터를 찾는다
			if( it_next == m_listQuestData.end() )
				it_next = m_listQuestData.begin();

			// 더 이상 다음 퀘스트가 없다
			if( it_next == m_listQuestData.end() )
				return NULL;

			// 리스트의 처음에 있는 퀘스트가 입력받은 퀘스트 아이디와 같다
			if( (*it_next)->sTSKey.tID == tID_current )
				return NULL;

			return *it_next;
		}
	}

	return NULL;
}

VOID  CLittleQuestContainer::DelData(NTL_TS_T_ID& tID)
{
	LIST_QUEST_ITER it = m_listQuestData.begin();
	for( ; it != m_listQuestData.end() ; ++it )
	{
		sQuestData* pQuestData = *it;
		if( pQuestData->sTSKey.tID == tID )
		{
			// 다음에 표시될 퀘스트 데이터를 찾는다
			CLittleQuestContainer::sQuestData* pNextQuestData = NextData(m_tID);

			// 현재 보여지고 있는 퀘스트 정보의 다음 정보가 지워질 정보인지 검사
			if( pNextQuestData )
			{
				if( pNextQuestData->sTSKey.tID == tID )
				{
					pNextQuestData = NextData(tID);
				}
			}

			// avooo's comment : pNextQuestData가 NULL 포인터라도 SetIndicator()를
			//					 한 번 호출하여 GUI를 초기화하자
			SetIndicator(pNextQuestData);

			NTL_DELETE(pQuestData);
			m_listQuestData.erase(it);
			break;
		}
	}
}

VOID CLittleQuestContainer::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CLittleQuestContainer::HandleEvents");

	if( msg.Id == g_EventRegQuest_Nfy )
	{
		SNtlEventRegQuest_Nfy* pEvent = reinterpret_cast<SNtlEventRegQuest_Nfy*>(msg.pData);

		CDboTSCQAgency* pQuestAgency = API_GetQuestAgency();
		if ( NULL == pQuestAgency ) NTL_RETURNVOID();

		if( pEvent->eQuestSortType == (eQUEST_SORT_TYPE)m_uiQuestType )
		{
			sQuestData* pQuestData = NTL_NEW sQuestData;
			CDboTSCQCtrl* pQuestCtr = reinterpret_cast<CDboTSCQCtrl*>(pQuestAgency->FindProgressTrigger( pEvent->sTSKey.tID ));
			if( pQuestCtr )
			{
				pQuestData->sTSKey			= pEvent->sTSKey;
				pQuestData->uiSortType		= m_uiQuestType;
				pQuestData->uiQuestTitle	= pEvent->uiQuestTitle;
				pQuestData->eEvtInfoType	= pQuestCtr->GetServerEvtDataType();
				pQuestData->uEvtInfoData	= pQuestCtr->GetServerEvtData();
				pQuestData->uiTimeLimit		= pQuestCtr->GetLimitTime();	

				m_listQuestData.push_back(pQuestData);

				// 업데이트 된 퀘스트 정보를 보여준다
				SetIndicator(pQuestData);
			}
			else
			{
				NTL_ASSERT(pQuestCtr, "CLittleQuestContainer::HandleEvents, Not exist ts data of id : " << pEvent->sTSKey.tID);
			}
		}
	}
	else if( msg.Id == g_EventUpdateQuestState_Nfy )
	{
		SNtlEventUpdateQuestState_Nfy* pEvent = reinterpret_cast<SNtlEventUpdateQuestState_Nfy*>( msg.pData );
		sQuestData* pQuestData = FindData(pEvent->sTSKey.tID);

		if( pQuestData )
		{
			pQuestData->sTSKey			= pEvent->sTSKey;

			if( m_tID == pQuestData->sTSKey.tID )
				SetIndicator(pQuestData);
		}
	}	
	else if( msg.Id == g_EventUpdateQuestProgressInfo_Nfy )
	{
		SNtlEventUpdateQuestProgressInfo_Nfy* pEvent = reinterpret_cast<SNtlEventUpdateQuestProgressInfo_Nfy*>(msg.pData);
		sQuestData* pQuestData = FindData(pEvent->sTSKey.tID);

		if(pQuestData)
		{
			pQuestData->sTSKey			= pEvent->sTSKey;
			pQuestData->eEvtInfoType	= pEvent->eEvtInfoType;
			pQuestData->uEvtInfoData	= pEvent->uEvtInfoData;
			pQuestData->uiTimeLimit		= pEvent->uiTimeLimit;			

			if( m_tID == pQuestData->sTSKey.tID )
				SetIndicator(pQuestData);
		}		
	}
	else if ( msg.Id == g_EventGiveUpQuest )
	{
		SNtlEventGiveUpQuest* pEvent = reinterpret_cast<SNtlEventGiveUpQuest*>(msg.pData);
		DelData(pEvent->sTSKey.tID);
	}
	else if( msg.Id == g_EventUnregQuest_Nfy )
	{
		SNtlEventUnregQuest_Nfy* pEvent = reinterpret_cast<SNtlEventUnregQuest_Nfy*>(msg.pData);
		DelData(pEvent->sTSKey.tID);
	}
	else if( msg.Id == g_EventTSRemovingTMQQuest_Nfy )
	{
		SNtlEventTSRemovingTMQQuest_Nfy* pEvent = reinterpret_cast<SNtlEventTSRemovingTMQQuest_Nfy*>( msg.pData );
		DelData(pEvent->tRmvTSId);
	}

	NTL_RETURNVOID();
}