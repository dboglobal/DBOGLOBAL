#include "precomp_ntlsimulation.h"
#include "DboTLEntity.h"

#include "GUISoundDefine.h"
#include "DboTLDefine.h"
#include "NtlSLEventFunc.h"
#include "gui_define.h"
#include "NtlLuaState.h"
#include "NtlSLLuaGlue.h"
#include "NtlSobGroup.h"
#include "NtlSob.h"
#include "NtlSobManager.h"
#include "NtlSobMonsterAttr.h"
#include "MobTable.h"
#include "NtlSLLogic.h"


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLEntity
//
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLSyncGroup
//
//////////////////////////////////////////////////////////////////////////


RwBool CDboTLSyncGroup::IsFinish( void )
{
	return m_defChildEntities.empty() ? TRUE : FALSE;
}

void CDboTLSyncGroup::Enter( void )
{
	listdef_Child_List::iterator it = m_defChildEntities.begin();
	for ( ; it != m_defChildEntities.end(); ++it )
	{
		(*it)->Enter();
	}
}

void CDboTLSyncGroup::Update( RwReal fElapsedTime )
{
	CDboTLEntity* pEntity;

	listdef_Child_List::iterator it = m_defChildEntities.begin();
	for ( ; it != m_defChildEntities.end(); )
	{
		pEntity = *it;

		pEntity->Update( fElapsedTime );

		if ( pEntity->IsFinish() )
		{
			pEntity->Leave();

			it = m_defChildEntities.erase( it );
		}
		else
		{
			++it;
		}
	}
}

void CDboTLSyncGroup::Leave( void )
{
	listdef_Child_List::iterator it = m_defChildEntities.begin();
	for ( ; it != m_defChildEntities.end(); ++it )
	{
		(*it)->Leave();
	}
}

void CDboTLSyncGroup::HandleEvents( RWS::CMsg &pMsg )
{
	listdef_Child_List::iterator it = m_defChildEntities.begin();
	for ( ; it != m_defChildEntities.end(); ++it )
	{
		(*it)->HandleEvents( pMsg );
	}
}

void CDboTLSyncGroup::PushChildEntity( CDboTLEntity* pEntity )
{
	m_defChildEntities.push_back( pEntity );
}

void CDboTLSyncGroup::ClearChildEntityList( void )
{
	m_defChildEntities.clear();
}


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLPause
//
//////////////////////////////////////////////////////////////////////////


void CDboTLPause::Update( RwReal fElapsedTime )
{
	m_fPauseTime -= fElapsedTime;

	if ( m_fPauseTime <= 0.f )
	{
		m_bFinish = TRUE;
	}
}


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLDefaultSkin
//
//////////////////////////////////////////////////////////////////////////

CDboTLDefaultSkin::CDboTLDefaultSkin( void )
{
	m_strFontName = DEFAULT_FONT;

	m_rgbFontBackColor.red = DEFAULT_TEXTCOLOR_RED;
	m_rgbFontBackColor.green = DEFAULT_TEXTCOLOR_GREEN;
	m_rgbFontBackColor.blue = DEFAULT_TEXTCOLOR_BLUE;
	m_rgbFontBackColor.alpha = 255;

	m_rgbFontColor.red = DEFAULT_TEXTCOLOR_RED;
	m_rgbFontColor.green = DEFAULT_TEXTCOLOR_GREEN;
	m_rgbFontColor.blue = DEFAULT_TEXTCOLOR_BLUE;
	m_rgbFontColor.alpha = 255;

	m_nFontSize = DEFAULT_FONT_SIZE;
	m_uiFontAllign = DEFAULT_FONT_ATTR;

	m_vFontOffset.x = m_vFontOffset.y = 0.f;

	m_vMsgLayerOffset.x = m_vMsgLayerOffset.y = 0.f;

	m_vOkOffset.x = m_vOkOffset.y = 0.f;

	m_vIMGOffset.x = m_vIMGOffset.y = 0.f;
}


void CDboTLDefaultSkin::Enter( void )
{
	CNtlSLEventGenerator::TLDefaultSkin( m_strFontName.c_str(), m_rgbFontBackColor, m_rgbFontColor, m_nFontSize, m_uiFontAllign, m_vFontOffset, m_vMsgLayerOffset, m_vOkOffset, m_vIMGOffset );
	m_bFinish = TRUE;
}

//////////////////////////////////////////////////////////////////////////
//
//	CDboTLGuideMsg_Show
//
//////////////////////////////////////////////////////////////////////////


void CDboTLGuideMsg_Show::Enter( void )
{
	CNtlSLEventGenerator::TLGuideMsg_ShowHide( m_uiTextTblIdx, m_fFadeInTime, TRUE );
}

void CDboTLGuideMsg_Show::Update( RwReal fElapsedTime )
{
	m_fFadeInTime -= fElapsedTime;

	if ( m_fFadeInTime <= 0.f )
	{
		m_bFinish = TRUE;
	}
}

void CDboTLGuideMsg_Show::SetFadeInTime( RwReal fFadeInTime )
{
	m_fFadeInTime = fFadeInTime + TL_SAFE_GUIDE_TIME;
}


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLGuideMsg_Hide
//
//////////////////////////////////////////////////////////////////////////


void CDboTLGuideMsg_Hide::Enter( void )
{
	CNtlSLEventGenerator::TLGuideMsg_ShowHide( m_uiTextTblIdx, m_fFadeOutTime, FALSE );
}

void CDboTLGuideMsg_Hide::Update( RwReal fElapsedTime )
{
	m_fFadeOutTime -= fElapsedTime;

	if ( m_fFadeOutTime <= 0.f )
	{
		m_bFinish = TRUE;
	}
}

void CDboTLGuideMsg_Hide::SetFadeOutTime( RwReal fFadeOutTime )
{
	m_fFadeOutTime = fFadeOutTime + TL_SAFE_GUIDE_TIME;
}


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLGuideMsg_ChangeColor
//
//////////////////////////////////////////////////////////////////////////


CDboTLGuideMsg_ChangeColor::CDboTLGuideMsg_ChangeColor( void )
{
	m_uiTextTblIdx = 0xffffffff;

	m_rgbFontBackColor.red = DEFAULT_TEXTCOLOR_RED;
	m_rgbFontBackColor.green = DEFAULT_TEXTCOLOR_GREEN;
	m_rgbFontBackColor.blue = DEFAULT_TEXTCOLOR_BLUE;
	m_rgbFontBackColor.alpha = 255;

	m_rgbFontColor.red = DEFAULT_TEXTCOLOR_RED;
	m_rgbFontColor.green = DEFAULT_TEXTCOLOR_GREEN;
	m_rgbFontColor.blue = DEFAULT_TEXTCOLOR_BLUE;
	m_rgbFontColor.alpha = 255;

	m_fFadeInOutTime = 0.f;
}


void CDboTLGuideMsg_ChangeColor::Enter( void )
{
	CNtlSLEventGenerator::TLGuideMsg_ChangeColor( m_uiTextTblIdx, m_fFadeInOutTime, m_rgbFontBackColor, m_rgbFontColor );
}


void CDboTLGuideMsg_ChangeColor::Update( RwReal fElapsedTime )
{
	m_fFadeInOutTime -= fElapsedTime;

	if ( m_fFadeInOutTime <= 0.f )
	{
		m_bFinish = TRUE;
	}
}


void CDboTLGuideMsg_ChangeColor::SetFadeInOutTime( RwReal fFadeInOutTime )
{
	m_fFadeInOutTime = m_fFadeInOutTime + TL_SAFE_GUIDE_TIME;
}


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLGuideIMG_Show
//
//////////////////////////////////////////////////////////////////////////


void CDboTLGuideIMG_Show::Enter( void )
{
	CNtlSLEventGenerator::TLGuideIMG_ShowHide( (RwChar*)m_strIMGName.c_str(), m_fFadeInTime, TRUE, m_fSizeRate );
}


void CDboTLGuideIMG_Show::Update( RwReal fElapsedTime )
{
	m_fFadeInTime -= fElapsedTime;

	if ( m_fFadeInTime <= 0.f )
	{
		m_bFinish = TRUE;
	}
}


void CDboTLGuideIMG_Show::SetFadeInTime( RwReal fFadeInTime )
{
	m_fFadeInTime = fFadeInTime + TL_SAFE_GUIDE_TIME;
}


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLGuideIMG_Hide
//
//////////////////////////////////////////////////////////////////////////


void CDboTLGuideIMG_Hide::Enter( void )
{
	CNtlSLEventGenerator::TLGuideIMG_ShowHide( (RwChar*)m_strIMGName.c_str(), m_fFadeOutTime, FALSE );
}


void CDboTLGuideIMG_Hide::Update( RwReal fElapsedTime )
{
	m_fFadeOutTime -= fElapsedTime;

	if ( m_fFadeOutTime <= 0.f )
	{
		m_bFinish = TRUE;
	}
}


void CDboTLGuideIMG_Hide::SetFadeOutTime( RwReal fFadeOutTime )
{
	m_fFadeOutTime = fFadeOutTime + TL_SAFE_GUIDE_TIME;
}


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLGuideIMGOk_Show
//
//////////////////////////////////////////////////////////////////////////


void CDboTLGuideIMGOk_Show::Enter( void )
{
	CNtlSLEventGenerator::TLGuideIMGOk_ShowHide( m_fFadeInTime, TRUE );
}


void CDboTLGuideIMGOk_Show::Update( RwReal fElapsedTime )
{
	m_fFadeInTime -= fElapsedTime;

	if ( m_fFadeInTime <= 0.f )
	{
		m_bFinish = TRUE;
	}
}


void CDboTLGuideIMGOk_Show::SetFadeInTime( RwReal fFadeInTime )
{
	m_fFadeInTime = fFadeInTime + TL_SAFE_GUIDE_TIME;
}


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLGuideIMGOk_Hide
//
//////////////////////////////////////////////////////////////////////////


void CDboTLGuideIMGOk_Hide::Enter( void )
{
	CNtlSLEventGenerator::TLGuideIMGOk_ShowHide( m_fFadeOutTime, FALSE );
}


void CDboTLGuideIMGOk_Hide::Update( RwReal fElapsedTime )
{
	m_fFadeOutTime -= fElapsedTime;

	if ( m_fFadeOutTime <= 0.f )
	{
		m_bFinish = TRUE;
	}
}


void CDboTLGuideIMGOk_Hide::SetFadeOutTime( RwReal fFadeOutTime )
{
	m_fFadeOutTime = fFadeOutTime + TL_SAFE_GUIDE_TIME;
}

//////////////////////////////////////////////////////////////////////////
//
//	CDboTLCheckPoint
//
//////////////////////////////////////////////////////////////////////////


void CDboTLCheckPoint::Enter( void )
{
	CNtlSLEventGenerator::SLDirection_Nfy( TRUE, m_vTargetPos );

	m_bFinish = TRUE;
}


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLUIIndicator_Show
//
//////////////////////////////////////////////////////////////////////////


CDboTLUIIndicator_Show::CDboTLUIIndicator_Show( void )
{
	m_uiTLIndicatorType = 0xffffffff;
	m_vFocusOffPos.x = m_vFocusOffPos.y = 0.f;
}


void CDboTLUIIndicator_Show::Enter( void )
{
	CNtlSLEventGenerator::TLUIIndicator_ShowHide( m_uiTLIndicatorType, (RwChar*)m_strFlashName.c_str(), m_vFocusOffPos, TRUE );

	m_bFinish = TRUE;
}


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLUIIndicator_Hide
//
//////////////////////////////////////////////////////////////////////////


void CDboTLUIIndicator_Hide::Enter( void )
{
	RwV2d vBlank; vBlank.x = vBlank.y = 0.f;
	CNtlSLEventGenerator::TLUIIndicator_ShowHide( m_uiTLIndicatorType, NULL, vBlank, FALSE );

	m_bFinish = TRUE;
}


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLMouseInputLock
//
//////////////////////////////////////////////////////////////////////////


void CDboTLMouseInputLock::Enter( void )
{
	CNtlSLEventGenerator::TLMouseInputLockUnlock( m_uiMouseInputType, TRUE );

	m_bFinish = TRUE;
}


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLMouseInputUnlock
//
//////////////////////////////////////////////////////////////////////////


void CDboTLMouseInputUnlock::Enter( void )
{
	CNtlSLEventGenerator::TLMouseInputLockUnlock( m_uiMouseInputType, FALSE );

	m_bFinish = TRUE;
}


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLKeyboardInputLock
//
//////////////////////////////////////////////////////////////////////////


void CDboTLKeyboardInputLock::Enter( void )
{
	CNtlSLEventGenerator::TLKeyboardInputLockUnlock( m_uiKeyboardInputType, TRUE );

	m_bFinish = TRUE;
}


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLKeyboardInputUnlock
//
//////////////////////////////////////////////////////////////////////////


void CDboTLKeyboardInputUnlock::Enter( void )
{
	CNtlSLEventGenerator::TLKeyboardInputLockUnlock( m_uiKeyboardInputType, FALSE );

	m_bFinish = TRUE;
}


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLActionMapBackup
//
//////////////////////////////////////////////////////////////////////////


void CDboTLActionMapBackup::Enter( void )
{
	CNtlSLEventGenerator::TLActionMapBackup();

	m_bFinish = TRUE;
}


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLActionMapRestore
//
//////////////////////////////////////////////////////////////////////////


void CDboTLActionMapRestore::Enter( void )
{
	CNtlSLEventGenerator::TLActionMapRestore();

	m_bFinish = TRUE;
}


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLExcuteQuest
//
//////////////////////////////////////////////////////////////////////////


void CDboTLExcuteQuest::Enter( void )
{
	CNtlSLEventGenerator::TLExcuteQuest( m_tQuestID );

	m_bFinish = TRUE;
}


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLBroadMsg
//
//////////////////////////////////////////////////////////////////////////


CDboTLBroadMsg::CDboTLBroadMsg( void )
{
	m_uiNPCTblIdx = 0xffffffff;
	m_uiSpeechTblIdx = INVALID_TBLIDX;
	m_fRemainTime = 0.f;
}


void CDboTLBroadMsg::Enter( void )
{
	CNtlSLEventGenerator::TLDisableMinimap();

	CNtlSLEventGenerator::BroadMsgNfy_TSBeg( eBROAD_MSG_UI_DIR_TYPE_NORMAL,
											 eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_NORMAL,
											 eBROAD_MSG_UI_BALLOON_SHAPE_TYPE_BC_NORMAL,
											 eBROAD_MSG_UI_SPEECH_DIR_TYPE_LINE,
											 eBROAD_MSG_OWNER_CONDITION_1,
											 eBROAD_MSG_OWNER_TYPE_NPC,
											 m_uiNPCTblIdx,
											 m_uiSpeechTblIdx,
											 m_fRemainTime );

	m_bFinish = TRUE;
}


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLAttackLock
//
//////////////////////////////////////////////////////////////////////////


void CDboTLAttackLock::Enter( void )
{
	CNtlSLEventGenerator::TLAttackLockUnlock( m_uiAttackType, TRUE );

	m_bFinish = TRUE;
}


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLAttackUnlock
//
//////////////////////////////////////////////////////////////////////////


void CDboTLAttackUnlock::Enter( void )
{
	CNtlSLEventGenerator::TLAttackLockUnlock( m_uiAttackType, FALSE );

	m_bFinish = TRUE;
}


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLPlayCinematic
//
//////////////////////////////////////////////////////////////////////////


void CDboTLPlayCinematic::Enter( void )
{
	CNtlLuaState *pLuaState = GetLuaState();
	pLuaState->RunString( m_strLuaFuncName.c_str() );

	m_bFinish = TRUE;
}


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLPlaySound
//
//////////////////////////////////////////////////////////////////////////


void CDboTLPlaySound::Enter( void )
{
	std::string strSoundName = "System\\";
	strSoundName += m_strSoundName.c_str();

	Logic_PlayGUISound((char*)strSoundName.c_str());

	m_bFinish = TRUE;
}


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLPlayCamera
//
//////////////////////////////////////////////////////////////////////////


CDboTLPlayCamera::CDboTLPlayCamera( void )
{
	m_uiCameraType = ETL_CAMERA_TYPE_MOB;
	m_uiTblIdx = 0xffffffff;
	m_vTargetPos.x = m_vTargetPos.y = m_vTargetPos.z = 0.f;
}


void CDboTLPlayCamera::Update( RwReal fElapsedTime )
{
	switch ( m_uiCameraType )
	{
	case ETL_CAMERA_TYPE_MOB:
		{
			CNtlSobGroup* pGroup = GetNtlSobManager()->GetSobGroup( SLCLASS_MONSTER );
			if ( NULL == pGroup )
			{
				return;
			}

			CNtlSobMonsterAttr* pAttr = NULL;

			CNtlSobGroup::MapObject_Iter it = pGroup->Begin();
			CNtlSobGroup::MapObject_Iter itEnd = pGroup->End();

			for ( ; it != itEnd; ++it )
			{
				pAttr = (CNtlSobMonsterAttr*)it->second->GetSobAttr();

				if ( pAttr->GetMobTbl()->tblidx == m_uiTblIdx )
				{
					CNtlSLEventGenerator::TLPlayCamera_TBLIDX( m_uiCameraType, m_uiTblIdx );

					m_bFinish = TRUE;

					return;
				}
			}
		}
		break;

	case ETL_CAMERA_TYPE_NPC:
		{
			SERIAL_HANDLE hHandle = GetNtlSobManager()->GetNpcTriggerSerialFromId( m_uiTblIdx );

			CNtlSob* pSobObj = GetNtlSobManager()->GetSobObject( hHandle );

			if ( NULL == pSobObj )
			{
				return;
			}

			CNtlSLEventGenerator::TLPlayCamera_TBLIDX( m_uiCameraType, m_uiTblIdx );

			m_bFinish = TRUE;

			return;
		}
		break;

	case ETL_CAMERA_TYPE_POSITION:
		{
			CNtlSLEventGenerator::TLPlayCamera_POS( m_uiCameraType, m_vTargetPos );

			m_bFinish = TRUE;

			return;
		}
		break;
	}
}


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLSoundOnOff
//
//////////////////////////////////////////////////////////////////////////


CDboTLSoundOnOff::CDboTLSoundOnOff( void )
{
	m_bSoundOn = TRUE;
}

void CDboTLSoundOnOff::Enter( void )
{
	// 먼가 사운드 연출을 필요로 할 때까지 무조건 다 끄고 킨다
	if( m_bSoundOn )
	{
		for( RwUInt8 i = 0 ; i < NUM_CHANNEL_GROUP ; ++i )
			GetSoundManager()->FadeIn(i, 1.f, dBGM_FADE_OUT_TIME);
	}
	else
	{
		for( RwUInt8 i = 0 ; i < NUM_CHANNEL_GROUP ; ++i )
			GetSoundManager()->FadeOut(i, 0.f, 0);
	}

	m_bFinish = TRUE;
}


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLCondition
//
//////////////////////////////////////////////////////////////////////////


void CDboTLCondition::HandleEvents( RWS::CMsg &pMsg )
{
	if ( pMsg.Id == g_EventTLCondition )
	{
		SNtlEventTLCondition* pData = (SNtlEventTLCondition*) pMsg.pData;

		if ( pData->uiCondType == m_uiCondType )
		{
			if ( m_uiCondType == ETL_CONDITION_TYPE_QUEST_FINISH )
			{
				if ( pData->uiParam1 == m_uiParam1 )
				{
					m_bFinish = TRUE;
				}
			}
			else
			{
				m_bFinish = TRUE;
			}
		}
	}
	else if ( g_EventDirectionNfy )
	{
		SNtlEventDirectionNfy* pData = (SNtlEventDirectionNfy*)pMsg.pData;

		if ( !pData->bCreate && (m_uiCondType == ETL_CONDITION_TYPE_IN_CHECK_POINT) )
		{
			m_bFinish = TRUE;
		}
	}

}


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLDialogMove
//
//////////////////////////////////////////////////////////////////////////

void CDboTLDialogMove::Enter()
{
	CNtlSLEventGenerator::TLDisableDialogMoveOption(m_bDisableDialogMoveOption);
	m_bFinish = TRUE;
}


//////////////////////////////////////////////////////////////////////////
//
//	CDboTLEnableSkip
//
//////////////////////////////////////////////////////////////////////////

void CDboTLEnableSkip::Enter()
{
	CNtlSLEventGenerator::TLEnableSkip(m_bEnableSkip);
	m_bFinish = TRUE;
}