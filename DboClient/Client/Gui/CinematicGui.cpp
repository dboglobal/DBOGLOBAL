#include "precomp_dboclient.h"
#include "CinematicGui.h"

// core
#include "NtlDebug.h"

// shared
#include "QuestTextDataTable.h"
#include "NPCTable.h"
#include "MobTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"
#include "DisplayStringManager.h"

// presentation
#include "NtlPLEvent.h"
#include "NtlPLGuiManager.h"
#include "NtlPLCinematicObject.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLLogic.h"
#include "NtlSLApi.h"
#include "NtlSobMonster.h"
#include "NtlSobMonsterAttr.h"
#include "NtlSobNpc.h"
#include "NtlSobNpcAttr.h"
#include "NtlSobManager.h"
#include "NtlDTCinematicManager.h"

// dbo
#include "DialogManager.h"
#include "AlarmManager.h"



VOID CCinematicGui::SetCinematicBalloon( void* pData )
{
	SNtlEventCinematicBalloon* pBalloonData = reinterpret_cast<SNtlEventCinematicBalloon*>( pData );
	
	if( pBalloonData->bShow )
	{
		if( m_eBalloonState == BALLOON_STATE_TEXTAPPEAR || 
			m_eBalloonState == BALLOON_STATE_IDLE ||
			pBalloonData->byPlayType == SNtlEventCinematicBalloon::PLAY_PROGRESS )
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
			NTL_ASSERTFAIL( "CCinematicGui::SetCinematicBalloon : Wrong Gui Type!" );
		}

		if( pBalloonData->uiQuestTextTblId != INVALID_TBLIDX )
		{
			sQUEST_TEXT_DATA_TBLDAT* pTextData = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( API_GetTableContainer()->GetQuestTextDataTable()->FindData( pBalloonData->uiQuestTextTblId ) );
			
			if( pTextData )
				m_pCinematicObject->SetHtmlText( pTextData->wstrText.c_str(), pTextData->wstrText.size() );
			else if( Logic_IsUIDevInfoVisible() )
				GetAlarmManager()->FormattedAlarmMessage( L"CCinematicGui::SetCinematicBalloon : QuestTextTable (%u) idx is missing", pBalloonData->uiQuestTextTblId );				
		}		
	}
	else
	{
		if( m_eBalloonState != BALLOON_STATE_NONE )
			SetBalloonState( BALLOON_STATE_END );
	}
}

//VOID CCinematicGui::SetCinematicFlash( void* pData )
//{
//	SNtlEventCinematicFlash* pFlashData = reinterpret_cast<SNtlEventCinematicFlash*>( pData );
//
//	if( pFlashData->bShow )
//	{
//		if( IsStateNone( ALL ) )
//		{
//			if( !GetDialogManager()->IsMode( DIALOGMODE_NARRATION ) )
//			{
//				NTL_WARNING(false, "Peessi !!! CinematicFlashStart" );
//				//GetDialogManager()->OnMode( DIALOGMODE_NARRATION );
//			}
//		}
//	}
//	else
//	{
//		if( IsStateNone( FLASH ) )
//		{
//			NTL_WARNING(false, "Peessi >>> CinematicFlashEnd" );
//			//GetDialogManager()->OffMode();
//			//GetDialogManager()->OpenGMDialog();
//			//GetDialogManager()->OpenDefaultDialog();
//		}
//	}
//
//	CNtlPLCinematicGui::SetCinematicFlash( pData );
//}

VOID CCinematicGui::SetCinematicState( CINEMATICSTATE eState )
{
	switch( eState )
	{
		case CINEMATIC_STATE_NONE:
		{
			if (IsStateNone(BALLOON))
			{
				//NTL_WARNING(false,  "Peessi >>> CinematicEnd" );
				GetDialogManager()->OffMode();
				GetDialogManager()->OpenGMDialog();
				GetDialogManager()->OpenDefaultDialog();
			}

			SetSkipAble(false);
		}
		break;
		case CINEMATIC_STATE_BEGIN:
		{
			if (IsStateNone(ALL))
			{
				if (!GetDialogManager()->IsMode(DIALOGMODE_NARRATION))
				{
					//NTL_WARNING(false, "Peessi !!! CinematicStart" );
					GetDialogManager()->OnMode(DIALOGMODE_NARRATION);
				}
			}

			SetSkipAble(GetNtlDTCinematicManager()->CanSkip());
		}
		break;
	}

	CNtlPLCinematicGui::SetCinematicState( eState );
}

VOID CCinematicGui::SetBalloonState( BALLOONSTATE eState )	
{
	switch( eState )
	{
	case BALLOON_STATE_NONE:
		if( IsStateNone( CINEMATIC ) )
		{
			//NTL_WARNING(false, "Peessi >>> CinematicBalloonEnd" );
			GetDialogManager()->OffMode();
			GetDialogManager()->OpenGMDialog();
			GetDialogManager()->OpenDefaultDialog();
		}
		break;
	case BALLOON_STATE_BEGIN:
		if( IsStateNone( ALL ) )
		{
			if( !GetDialogManager()->IsMode( DIALOGMODE_NARRATION ) )
			{
				//NTL_WARNING(false, "Peessi !!! CinematicBalloonStart" );
				GetDialogManager()->OnMode( DIALOGMODE_NARRATION );
			}
		}		
		break;
	}

	CNtlPLCinematicGui::SetBalloonState( eState );
}

void CCinematicGui::SkipCinematics(UINT32 hPc)
{
	if (!m_pCinematicObject->IsShow())
		return;

	const WCHAR* wchPlayerName = Logic_GetName(GetNtlSobManager()->GetSobObject(hPc));
	if (wchPlayerName == NULL)
		return;

	WCHAR buf[128];
	swprintf_s(buf, GetDisplayStringManager()->GetString("DST_CINEMATIC_USER_CANCLE"), wchPlayerName);

	m_pCinematicObject->GetSkippedText()->AddText(buf);
}

void CCinematicGui::SetSkipAble(bool bFlag)
{
	if (bFlag)
	{
		m_pCinematicObject->GetSkipHintText()->SetText(GetDisplayStringManager()->GetString("DST_CINEMATIC_EXPLAIN_ESCKEY"));
		m_pCinematicObject->GetSkipHintText()->Show(true);
		m_pCinematicObject->GetSkippedText()->Show(true);
		m_pCinematicObject->GetSkippedText()->Clear();
	}
	else
	{
		m_pCinematicObject->GetSkipHintText()->Show(false);
		m_pCinematicObject->GetSkippedText()->Clear();
		m_pCinematicObject->GetSkippedText()->Show(false);
	}
}
