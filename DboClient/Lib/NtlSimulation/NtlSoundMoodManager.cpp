#include "precomp_ntlsimulation.h"
#include "NtlSoundMoodManager.h"

// shared
#include "TableContainer.h"
#include "WorldTable.h"
#include "NtlPacketGU.h"

// core
#include "NtlSysEvent.h"
#include "NtlDebug.h"

// sound
#include "NtlSound.h"
#include "NtlSoundManager.h"
#include "NtlSoundEventGenerator.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"
#include "NtlSLEvent.h"
#include "NtlSobManager.h"
#include "NtlSoundNode.h"
#include "NtlSobAvatar.h"
#include "NtlRangeBGM.h"
#include "NtlThemeBGM.h"


CNtlSoundMoodManager* CNtlSoundMoodManager::m_pInstance = NULL;

RwBool CNtlSoundMoodManager::CreateInstance()
{
	NTL_FUNCTION( "CNtlSoundMoodManager::CreateInstance" );

	if( m_pInstance )
		NTL_RETURN(FALSE);

	m_pInstance = NTL_NEW CNtlSoundMoodManager;
	if( !m_pInstance )
	{
		DBO_FAIL("Instance Allocate Fail" );
		NTL_RETURN(FALSE);
	}

	CNtlSoundNode* pThemeBGM = NTL_NEW CNtlThemeBGM;
	if( !pThemeBGM->Create() )
	{
		DBO_FAIL("Theme BGM create fail" );
		NTL_RETURN(FALSE);
	}
	m_pInstance->m_listSoundNode.push_back(pThemeBGM);

	CNtlSoundNode* pRangeBGM = NTL_NEW CNtlRangeBGM;
	if( !pRangeBGM->Create() )
	{
		DBO_FAIL("Range BGM create fail" );
		NTL_RETURN(FALSE);
	}
	m_pInstance->m_listSoundNode.push_back(pRangeBGM);

	m_pInstance->LinkMsg(g_EventUpdateTick);
	m_pInstance->LinkMsg(g_EventChangeWorldConceptState);	
	m_pInstance->LinkMsg(g_EventSoundAddListRangeBGM);
	m_pInstance->LinkMsg(g_EventSoundDelListRangeBGM);
	m_pInstance->LinkMsg(g_EventSobCreate);
	m_pInstance->LinkMsg(g_EventSobDelete);
	m_pInstance->LinkMsg(g_EventSoundEventMusic);

	NTL_RETURN(TRUE);
}

VOID CNtlSoundMoodManager::DestroyInstance()
{
	NTL_FUNCTION( "CNtlSoundMoodManager::DestroyInstance" );

	if( !m_pInstance )
		return;

	LIST_SOUND_NODE::iterator it_SoundNode = m_pInstance->m_listSoundNode.begin();
	for( ; it_SoundNode != m_pInstance->m_listSoundNode.end() ; ++it_SoundNode )
	{
		CNtlSoundNode* pSoundNode = *it_SoundNode;
		pSoundNode->Destory();
		NTL_DELETE(pSoundNode);
	}
	m_pInstance->m_listSoundNode.clear();

	m_pInstance->UnLinkMsg(g_EventUpdateTick);
	m_pInstance->UnLinkMsg(g_EventChangeWorldConceptState);	
	m_pInstance->UnLinkMsg(g_EventSoundAddListRangeBGM);
	m_pInstance->UnLinkMsg(g_EventSoundDelListRangeBGM);
	m_pInstance->UnLinkMsg(g_EventSobCreate);
	m_pInstance->UnLinkMsg(g_EventSobDelete);
	m_pInstance->UnLinkMsg(g_EventSoundEventMusic);

	NTL_DELETE(m_pInstance);

	NTL_RETURNVOID();
}

CNtlSoundMoodManager::CNtlSoundMoodManager()
{
	m_tTMQ_BGM.hHandle = INVALID_SOUND_HANDLE;
}

CNtlSoundMoodManager::~CNtlSoundMoodManager()
{
	
}

VOID CNtlSoundMoodManager::Update(RwReal fElapsed)
{
	LIST_SOUND_NODE::iterator it_SoundNode = m_listSoundNode.begin();
	for( ; it_SoundNode != m_listSoundNode.end() ; ++it_SoundNode )
	{
		CNtlSoundNode* pSoundNode = *it_SoundNode;
		pSoundNode->Update(fElapsed);
	}
}

VOID CNtlSoundMoodManager::Condition_EnterWorld(sWORLD_TBLDAT* pWorldTblData)
{
	if( NULL == pWorldTblData )
	{
		DBO_FAIL("invalid world table");
		return;
	}

	CNtlSoundEventGenerator::SendMusicRestItem(CHANNEL_GROUP_BGM, pWorldTblData->fBGMRestTime);
}

VOID CNtlSoundMoodManager::Condition_AskPlay_from_Server(sGU_PLAY_BGM* pPlayBGM)
{
	if( NULL == pPlayBGM )
	{
		DBO_FAIL("Invalid PLAY BGM pointer");
		return;
	}

	if( pPlayBGM->bPlay )
	{
		CNtlSoundEventGenerator::SendMusicRestItem(CHANNEL_GROUP_BGM, 1000.f * (RwReal)pPlayBGM->dwDelay, BGM_TYPE_SERVER_PLAYED);

		sNtlSoundPlayParameta tSoundParam;
		tSoundParam.iChannelGroup	= CHANNEL_GROUP_BGM;
		tSoundParam.pcFileName		= pPlayBGM->szName;
		tSoundParam.eBGM_Type		= BGM_TYPE_SERVER_PLAYED;
		tSoundParam.bLoop			= pPlayBGM->bLoop;
		GetSoundManager()->Play(&tSoundParam);

		m_tTMQ_BGM.hHandle			= tSoundParam.hHandle;
	}
	else
	{
		GetSoundManager()->Stop(m_tTMQ_BGM.hHandle);
	}
}

VOID CNtlSoundMoodManager::HandleEvents(RWS::CMsg &pMsg)
{	
	NTL_FUNCTION("CNtlSoundMoodManager::HandleEvents");

	if(pMsg.Id == g_EventUpdateTick)
	{
		float fElapsed = *reinterpret_cast<float*>(pMsg.pData);
		Update(fElapsed);
	}
	else if(pMsg.Id == g_EventChangeWorldConceptState)
	{
		LIST_SOUND_NODE::iterator it_SoundNode = m_listSoundNode.begin();
		for( ; it_SoundNode != m_listSoundNode.end() ; ++it_SoundNode )
		{
			CNtlSoundNode* pSoundNode = *it_SoundNode;
			pSoundNode->HandleEvents(pMsg);
		}

		SNtlEventWorldConceptState* pEvent = reinterpret_cast<SNtlEventWorldConceptState*>( pMsg.pData );

		if( pEvent->uiState == WORLD_STATE_EXIT )			
		{
			if( WORLD_PLAY_TIME_MACHINE == pEvent->eWorldConcept )				
			{
				if( INVALID_SOUND_HANDLE != m_tTMQ_BGM.hHandle )
				{
					m_tTMQ_BGM.hHandle = INVALID_SOUND_HANDLE;
					CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_TEMP_FINISH_TMQ);
				}
			}
			else if( GetNtlWorldConcept()->IsGrade(WORLD_CONCEPT_FIRST_GRADE, pEvent->eWorldConcept) )
			{
				CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_TEMP_FINISH_TMQ);
			}
		}
	}
	else
	{
		LIST_SOUND_NODE::iterator it_SoundNode = m_listSoundNode.begin();
		for( ; it_SoundNode != m_listSoundNode.end() ; ++it_SoundNode )
		{
			CNtlSoundNode* pSoundNode = *it_SoundNode;
			pSoundNode->HandleEvents(pMsg);
		}
	}

	NTL_RETURNVOID();
}