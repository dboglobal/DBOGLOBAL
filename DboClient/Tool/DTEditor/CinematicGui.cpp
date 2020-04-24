#include "stdafx.h"
#include "CinematicGui.h"

// core
#include "NtlDebug.h"

// shared
#include "QuestTextDataTable.h"
#include "NPCTable.h"
#include "MobTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"

// presentation
#include "NtlPLEvent.h"
#include "NtlPLGuiManager.h"
#include "NtlPLCinematicObject.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLApi.h"
#include "NtlSobMonster.h"
#include "NtlSobMonsterAttr.h"
#include "NtlSobNpc.h"
#include "NtlSobNpcAttr.h"
#include "NtlSobManager.h"


VOID CCinematicGui::SetCinematicBalloon( void* pData )
{
	SNtlEventCinematicBalloon* pBalloonData = reinterpret_cast<SNtlEventCinematicBalloon*>( pData );
	
	if( pBalloonData->bShow )
	{
		if( m_eBalloonState == BALLOON_STATE_TEXTAPPEAR || m_eBalloonState == BALLOON_STATE_IDLE )
		{
			SetBalloonState( BALLOON_STATE_TEXTAPPEAR );
		}
		else
		{
			SetBalloonState( BALLOON_STATE_BEGIN );			
		}

		CNtlSob* pSobObject = GetNtlSobManager()->GetSobObject( pBalloonData->hSerialId );
		NTL_ASSERT( pSobObject, "CCinematicGui::SetCinematicBalloon : Invalid target" );

		if( pSobObject->GetClassID() == SLCLASS_NPC )
		{
			CNtlSobNpcAttr* pNPCAttr = reinterpret_cast<CNtlSobNpcAttr*>( pSobObject->GetSobAttr() );
			sNPC_TBLDAT* pNPCData = pNPCAttr->GetNpcTbl();
			CTextTable* pNPCText = API_GetTableContainer()->GetTextAllTable()->GetNPCTbl();

			m_pCinematicObject->ShowImage( TRUE );			

			if( !m_pCinematicObject->SetImageTexture( pNPCData->szILLust, pBalloonData->byEmotionType ) )
			{
				// 그림이 없으면 기본값 출력. 이래도 없으면 흰색 그림 출력
				m_pCinematicObject->SetImageTexture( pNPCData->szILLust, 0 );				
			}
			m_pCinematicObject->SetName( pNPCText->GetText( pNPCData->Name ).c_str() );			
		}
		else if( pSobObject->GetClassID() == SLCLASS_MONSTER )
		{
			CNtlSobMonsterAttr* pMobAttr = reinterpret_cast<CNtlSobMonsterAttr*>( pSobObject->GetSobAttr() );
			sMOB_TBLDAT* pMobData = pMobAttr->GetMobTbl();
			CTextTable* pMobText = API_GetTableContainer()->GetTextAllTable()->GetMobTbl();

			m_pCinematicObject->ShowImage( TRUE );			

			if( !m_pCinematicObject->SetImageTexture( pMobData->szILLust, pBalloonData->byEmotionType ) )
			{
				// 그림이 없으면 기본값 출력. 이래도 없으면 흰색 그림 출력
				m_pCinematicObject->SetImageTexture( pMobData->szILLust, 0 );				
			}
			m_pCinematicObject->SetName( pMobText->GetText( pMobData->Name ).c_str() );
		}

		if( pBalloonData->byBalloonType == eNARRATION_GUI_TYPE_NORMAL )
		{
			m_pCinematicObject->SetBalloonType( CNtlPLCinematicObject::BALLOON_TYPE_NORMAL );			
		}
		else if( pBalloonData->byBalloonType == eNARRATION_GUI_TYPE_BOMB )
		{
			m_pCinematicObject->SetBalloonType( CNtlPLCinematicObject::BALLOON_TYPE_BOMB );			
		}
		else
		{
			NTL_ASSERTFAIL( "CQuestNarrationGui::SetUIData : Wrong Gui Type!" );
		}

		if( pBalloonData->uiQuestTextTblId != INVALID_TBLIDX )
		{
			sQUEST_TEXT_DATA_TBLDAT* pTextData = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( API_GetTableContainer()->GetQuestTextDataTable()->FindData( pBalloonData->uiQuestTextTblId ) );

			if( pTextData )
				m_pCinematicObject->SetHtmlText( pTextData->wstrText.c_str(), pTextData->wstrText.size() );		
		}		
	}
	else
	{
		if( m_eBalloonState != BALLOON_STATE_NONE )
			SetBalloonState( BALLOON_STATE_END );
	}
}
