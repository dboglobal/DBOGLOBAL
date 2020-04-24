#include "precomp_ntlsimulation.h"
#include "NtlThemeBGM.h"

// shared
#include "WorldTable.h"
#include "TableContainer.h"

// core
#include "NtlDebug.h"

// util
#include "NtlXMLDoc.h"

// sound
#include "NtlSound.h"
#include "NtlSoundManager.h"
#include "NtlSoundEventGenerator.h"

// presentation
#include "NtlPLResourcePack.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"
#include "NtlSLEvent.h"
#include "NtlSobManager.h"
#include "NtlSobAvatar.h"


#define dTHEME_BGM_UPDATE_TIME				(2.f)


CNtlThemeBGM::CNtlThemeBGM()
:m_eCurWorldType(WORLD_MAIN_WORLD)
,m_hPlayOjbectHandle(INVALID_SERIAL_ID)
,m_hSound(INVALID_SOUND_HANDLE)
,m_fUpdateSkipTime(0.f)
{

}

CNtlThemeBGM::~CNtlThemeBGM()
{

}

RwBool CNtlThemeBGM::Create()
{
	NTL_FUNCTION("CNtlThemeBGM::Create");

	LoadScript();

	NTL_RETURN(TRUE);
}

VOID CNtlThemeBGM::Destory()
{
	NTL_FUNCTION("CNtlThemeBGM::Destroy");

	for( RwUInt8 i = 0 ; i < NUM_WORLD_TYPE ; ++i )
	{
		MAP_THEME_BGM_LIST::iterator it_THEME_BGM = m_mapTHEME_BGM_List[i].begin();
		for( ; it_THEME_BGM != m_mapTHEME_BGM_List[i].end() ; ++it_THEME_BGM )
		{
			sTHEME_BGM* pTHEME_BGM = it_THEME_BGM->second;
			NTL_DELETE(pTHEME_BGM);
		}
		m_mapTHEME_BGM_List[i].clear();
	}

	MAP_THEME_BGM_SUBJECT::iterator it_LiveBGM = m_mapSubjectBGMObejct.begin();
	for( ; it_LiveBGM != m_mapSubjectBGMObejct.end() ; ++it_LiveBGM )
	{
		sTHEME_SUBJECT* pTHEME_SUBJECT = it_LiveBGM->second;
		NTL_DELETE(pTHEME_SUBJECT);
	}
	m_mapSubjectBGMObejct.clear();

	NTL_RETURNVOID();
}

VOID CNtlThemeBGM::Update(RwReal fElapsed)
{
	m_fUpdateSkipTime += fElapsed;
	if( dTHEME_BGM_UPDATE_TIME > m_fUpdateSkipTime )
		return;
	else
		m_fUpdateSkipTime = 0.f;

	if( INVALID_SOUND_HANDLE != m_hSound )
		return;

	if( INVALID_SERIAL_ID != m_hPlayOjbectHandle )
		return;

	if( INVALID_WORLD_TYPE == m_eCurWorldType )
		return;

	if( false == GetNtlSLGlobal()->GetSobAvatar() )
		return;


	RwV3d& v3AvatarPos = GetNtlSLGlobal()->GetSobAvatar()->GetPosition();
	MAP_THEME_BGM_SUBJECT::iterator it_Theme_BMG_subject = m_mapSubjectBGMObejct.begin();

	for( ; it_Theme_BMG_subject != m_mapSubjectBGMObejct.end() ; ++it_Theme_BMG_subject )
	{
		sTHEME_SUBJECT* pTHEME_SUBJECT = it_Theme_BMG_subject->second;
		sTHEME_BGM* pTHEME_BGM = pTHEME_SUBJECT->pTHEME_BGM;

		if( pTHEME_SUBJECT->bPlayed )
			continue;

		CNtlSob* pSob = GetNtlSobManager()->GetSobObject(pTHEME_SUBJECT->hHandle);
		if( !pSob )
		{
			DBO_FAIL("Not exist sob object of handle : " << pTHEME_SUBJECT->hHandle);
			continue;
		}

		if( pTHEME_BGM->fValidRange >= CNtlMath::GetLength(v3AvatarPos, pSob->GetPosition()) )
		{
			sNtlSoundPlayParameta tSoundParam;
			tSoundParam.iChannelGroup	= CHANNEL_GROUP_BGM;
			tSoundParam.pcFileName		= (char*)pTHEME_BGM->strFileName.c_str();
			tSoundParam.eBGM_Type		= BGM_TYPE_THEME_BGM;
			GetSoundManager()->Play(&tSoundParam);			

			if( INVALID_SOUND_HANDLE != tSoundParam.hHandle )
			{
				if( pTHEME_BGM->bLoop )
					CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_LOOP_THEME_BGM);
				else
					CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_NOT_LOOP_THEME_BGM);

				m_hSound					= tSoundParam.hHandle;
				m_hPlayOjbectHandle			= pTHEME_SUBJECT->hHandle;
				pTHEME_SUBJECT->bPlayed		= true;
			}			
			break;
		}
	}
}

VOID CNtlThemeBGM::RegisterLiveSobject(SERIAL_HANDLE hHandle, RwUInt32 uiTableIndex)
{
	if( INVALID_WORLD_TYPE == m_eCurWorldType )
		return;

	if( INVALID_SERIAL_ID == hHandle )
	{
		DBO_FAIL("Theme BGM can not register invalid serial handle");
		return;
	}

	MAP_THEME_BGM_LIST& rMAP_THEME_BGM = m_mapTHEME_BGM_List[m_eCurWorldType];
	MAP_THEME_BGM_LIST::iterator it = rMAP_THEME_BGM.find(uiTableIndex);

	if( it == rMAP_THEME_BGM.end() )
		return;

	 MAP_THEME_BGM_SUBJECT::iterator it_Subject = m_mapSubjectBGMObejct.find(hHandle);
	 if( it_Subject != m_mapSubjectBGMObejct.end() )
		 return;

	sTHEME_SUBJECT* pTHEME_SUBJECT = NTL_NEW sTHEME_SUBJECT;
	pTHEME_SUBJECT->hHandle		= hHandle;
	pTHEME_SUBJECT->pTHEME_BGM	= it->second;
	pTHEME_SUBJECT->bPlayed		= false;

	m_mapSubjectBGMObejct[pTHEME_SUBJECT->hHandle] = pTHEME_SUBJECT;
}

VOID CNtlThemeBGM::UnregiLiveSobject(SERIAL_HANDLE hHandle)
{
	MAP_THEME_BGM_SUBJECT::iterator it = m_mapSubjectBGMObejct.find(hHandle);
	if( it == m_mapSubjectBGMObejct.end() )
		return;


	sTHEME_SUBJECT* pTHEME_SUBJECT = it->second;
	NTL_DELETE(pTHEME_SUBJECT);
	m_mapSubjectBGMObejct.erase(it);


	if( hHandle == m_hPlayOjbectHandle )
	{
		m_hSound = INVALID_SOUND_HANDLE;
		m_hPlayOjbectHandle = INVALID_SERIAL_ID;

		CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_FINISH_THEME_BGM);
	}
}

VOID CNtlThemeBGM::LoadScript()
{
#define dTHEME_BGM_SCRIPT			"script\\ThemeBGM.xml"
#define dREAD_BUFFER_SIZE			(128)

	CNtlXMLDoc XMLDoc;

	XMLDoc.Create();

	if( GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_SCRIPT )
	{
		char*	pcData = NULL;
		RwInt32 iSize = 0;

		GetNtlResourcePackManager()->LoadScript(dTHEME_BGM_SCRIPT, (void**)&pcData, &iSize);
		if( NULL == pcData )
			return;

		// 버퍼 +1 생성
		char* pcBuffer = NTL_NEW char[iSize+1];
		memcpy(pcBuffer, pcData, sizeof(char)*iSize);
		pcBuffer[iSize] = '\0';

		if( false == XMLDoc.LoadXML(pcBuffer) )
		{
			DBO_FAIL("Not exist file : " << dTHEME_BGM_SCRIPT);

			NTL_ARRAY_DELETE( pcData );
			NTL_ARRAY_DELETE( pcBuffer );
			return;
		}

		NTL_ARRAY_DELETE( pcData );
		NTL_ARRAY_DELETE( pcBuffer );
	}
	else
	{
		if( !XMLDoc.Load(dTHEME_BGM_SCRIPT) )
		{
			DBO_FAIL("Not exist file : " << dTHEME_BGM_SCRIPT);
			return;
		}
	}


	IXMLDOMNodeList* pRootNodeList = XMLDoc.SelectNodeList("ThemeBGM/node");
	if( !pRootNodeList )
	{
		DBO_FAIL("Not exist keyward : " << "ThemeBGM/node");
		return;
	}

	long listLen = 0;
	char acReadBuffer[dREAD_BUFFER_SIZE] = "";

	pRootNodeList->get_length(&listLen);

	for(long i = 0 ; i < listLen ; ++i)
	{
		IXMLDOMNode* pNode = NULL;
		pRootNodeList->get_item(i, &pNode);
		if(!pNode)
			continue;

		if( false == XMLDoc.GetTextWithAttributeName(pNode, "index", acReadBuffer, dREAD_BUFFER_SIZE) )
		{
			DBO_FAIL("Not exist keyward : " << "node index");
			continue;
		}

		RwInt32		iNodeIndex	= atoi(acReadBuffer);
		sTHEME_BGM*	pTHEME_BGM	= NTL_NEW sTHEME_BGM;

		IXMLDOMNodeList* pChildNodeList = NULL;

		// 월드 타입
		pNode->selectNodes(L"world", &pChildNodeList);
		if(pChildNodeList)
		{
			long listLenNode = 0;
			char acReadBuffer[dREAD_BUFFER_SIZE] = "";

			pChildNodeList->get_length(&listLenNode);

			for(long i = 0 ; i < listLenNode ; ++i)
			{
				IXMLDOMNode* pChildNode = NULL;
				pChildNodeList->get_item(i, &pChildNode);

				if( !pChildNode )
					continue;

				if( false == XMLDoc.GetTextWithAttributeName(pChildNode, "type", acReadBuffer, dREAD_BUFFER_SIZE))
				{
					DBO_FAIL("Not exist keyward : " << "world type" << " of node index : " << iNodeIndex);
					continue;
				}
			}

			pTHEME_BGM->eWorldType = (eWORLD_TYPE)atoi(acReadBuffer);
		}
		else
		{
			DBO_FAIL("Not exist keyward : " << "world" << " of node index : " << iNodeIndex);
			NTL_DELETE(pTHEME_BGM);
			continue;
		}

		// 테이블 타입과 인덱스
		pNode->selectNodes(L"table", &pChildNodeList);
		if(pChildNodeList)
		{
			long listLenNode = 0;
			char acReadBuffer[dREAD_BUFFER_SIZE] = "";

			pChildNodeList->get_length(&listLenNode);

			for(long i = 0 ; i < listLenNode ; ++i)
			{
				IXMLDOMNode* pChildNode = NULL;
				pChildNodeList->get_item(i, &pChildNode);

				if( !pChildNode )
					continue;

				if( false == XMLDoc.GetTextWithAttributeName(pChildNode, "type", acReadBuffer, dREAD_BUFFER_SIZE))
				{
					DBO_FAIL("Not exist keyward : " << "table type" << " of node index : " << iNodeIndex);
					continue;
				}
				else
				{
					if( strcmp(acReadBuffer, "npc") == 0 )
					{
						pTHEME_BGM->eTableType = TABLE_NPC;
					}
					else if( strcmp(acReadBuffer, "mob") == 0 )
					{
						pTHEME_BGM->eTableType = TABLE_MOB;
					}
					else if( strcmp(acReadBuffer, "object") == 0 )
					{
						pTHEME_BGM->eTableType = TABLE_TRRIGER_OBJECT;
					}
				}

				if( false == XMLDoc.GetTextWithAttributeName(pChildNode, "index", acReadBuffer, dREAD_BUFFER_SIZE))
				{
					DBO_FAIL("Not exist keyward : " << "index of table type" << " of node index : " << iNodeIndex);
					continue;
				}
				else
				{
					pTHEME_BGM->uiTblidx = (RwUInt32)atol(acReadBuffer);
				}
			}			
		}
		else
		{
			DBO_FAIL("Not exist keyward : " << "table" << " of node index : " << iNodeIndex);
			NTL_DELETE(pTHEME_BGM);
			continue;
		}

		// 사운드
		pNode->selectNodes(L"sound", &pChildNodeList);
		if(pChildNodeList)
		{
			long listLenNode = 0;
			char acReadBuffer[dREAD_BUFFER_SIZE] = "";

			pChildNodeList->get_length(&listLenNode);

			for(long i = 0 ; i < listLenNode ; ++i)
			{
				IXMLDOMNode* pChildNode = NULL;
				pChildNodeList->get_item(i, &pChildNode);

				if( !pChildNode )
					continue;

				if( false == XMLDoc.GetTextWithAttributeName(pChildNode, "file", acReadBuffer, dREAD_BUFFER_SIZE))
				{
					DBO_FAIL("Not exist keyward : " << "sound file" << " of node index : " << iNodeIndex);
					continue;
				}
			}

			pTHEME_BGM->strFileName = acReadBuffer;
		}
		else
		{
			DBO_FAIL("Not exist keyward : " << "sound" << " of node index : " << iNodeIndex);
			NTL_DELETE(pTHEME_BGM);
			continue;
		}

		// 범위
		pNode->selectNodes(L"range", &pChildNodeList);
		if(pChildNodeList)
		{
			long listLenNode = 0;
			char acReadBuffer[dREAD_BUFFER_SIZE] = "";

			pChildNodeList->get_length(&listLenNode);

			for(long i = 0 ; i < listLenNode ; ++i)
			{
				IXMLDOMNode* pChildNode = NULL;
				pChildNodeList->get_item(i, &pChildNode);

				if( !pChildNode )
					continue;

				if( false == XMLDoc.GetTextWithAttributeName(pChildNode, "value", acReadBuffer, dREAD_BUFFER_SIZE))
				{
					DBO_FAIL("Not exist keyward : " << "range value" << " of node index : " << iNodeIndex);
					continue;
				}
			}

			pTHEME_BGM->fValidRange = (RwReal)atof(acReadBuffer);
		}
		else
		{
			DBO_FAIL("Not exist keyward : " << "range" << " of node index : " << iNodeIndex);
			NTL_DELETE(pTHEME_BGM);
			continue;
		}

		// 반복여부
		pNode->selectNodes(L"loop", &pChildNodeList);
		if(pChildNodeList)
		{
			long listLenNode = 0;
			char acReadBuffer[dREAD_BUFFER_SIZE] = "";

			pChildNodeList->get_length(&listLenNode);

			for(long i = 0 ; i < listLenNode ; ++i)
			{
				IXMLDOMNode* pChildNode = NULL;
				pChildNodeList->get_item(i, &pChildNode);

				if( !pChildNode )
					continue;

				if( false == XMLDoc.GetTextWithAttributeName(pChildNode, "value", acReadBuffer, dREAD_BUFFER_SIZE))
				{
					DBO_FAIL("Not exist keyward : " << "loop value" << " of node index : " << iNodeIndex);
					continue;
				}
			}

			pTHEME_BGM->bLoop = B2b((RwBool)atoi(acReadBuffer));
		}
		else
		{
			DBO_FAIL("Not exist keyward : " << "range" << " of node index : " << iNodeIndex);
			NTL_DELETE(pTHEME_BGM);
			continue;
		}

		if( INVALID_WORLD_TYPE	<= pTHEME_BGM->eWorldType	||
			NUM_TABLE_TYPE		<= pTHEME_BGM->eTableType	||
			INVALID_TBLIDX		== pTHEME_BGM->uiTblidx		||
			-1.f				== pTHEME_BGM->fValidRange	)
		{
			NTL_DELETE(pTHEME_BGM);
			continue;
		}

		MAP_THEME_BGM_LIST& rMAP_THEME_BGM = m_mapTHEME_BGM_List[pTHEME_BGM->eWorldType];
		rMAP_THEME_BGM[pTHEME_BGM->uiTblidx] = pTHEME_BGM;
	}
}

VOID CNtlThemeBGM::HandleEvents(RWS::CMsg &pMsg)
{	
	NTL_FUNCTION("CNtlThemeBGM::HandleEvents");

	if(pMsg.Id ==  g_EventChangeWorldConceptState)
	{
		SNtlEventWorldConceptState* pEvent = reinterpret_cast<SNtlEventWorldConceptState*>( pMsg.pData );

		// Theme BGM의 월드 타입을 결정하기 위해
		if( GetNtlWorldConcept()->IsGrade(WORLD_CONCEPT_FIRST_GRADE, pEvent->eWorldConcept) )
		{
			if( pEvent->uiState == WORLD_STATE_EXIT )
			{
				m_eCurWorldType = WORLD_MAIN_WORLD;
			}
			else if( pEvent->uiState == WORLD_STATE_ENTER )
			{
				switch(pEvent->eWorldConcept)
				{
				case WORLD_PLAY_PARTY_DUNGEON:
					{
						m_eCurWorldType = WORLD_PARTY_DUNGEON;
						break;
					}
				default:
					{
						if( GetNtlWorldConcept()->IsGrade(WORLD_CONCEPT_FIRST_GRADE, pEvent->eWorldConcept) )
							m_eCurWorldType = INVALID_WORLD_TYPE;

						break;
					}
				}
			}
		}

	}	
	else if(pMsg.Id == g_EventSobCreate)
	{
		SNtlEventSobBaseCreate* pEvent = reinterpret_cast<SNtlEventSobBaseCreate*>( pMsg.pData );

		if( SLCLASS_NPC == pEvent->uiClassId )
		{
			SNtlEventSobNpcCreate* pNPCCreateEvent = reinterpret_cast<SNtlEventSobNpcCreate*>( pEvent );
			RegisterLiveSobject(pEvent->hSerialId, pNPCCreateEvent->pNpcBrief->tblidx);
		}
		else if( SLCLASS_MONSTER == pEvent->uiClassId )
		{
			SNtlEventSobMonsterCreate* pMobCreateEvent = reinterpret_cast<SNtlEventSobMonsterCreate*>( pEvent );
			RegisterLiveSobject(pEvent->hSerialId, pMobCreateEvent->pMobBrief->tblidx);
		}
		else if( SLCLASS_TRIGGER_OBJECT == pEvent->uiClassId )
		{
			SNtlEventSobTriggerObjectCreate* pTriggerCreateEvent = reinterpret_cast<SNtlEventSobTriggerObjectCreate*>( pEvent );
			RegisterLiveSobject(pEvent->hSerialId, pTriggerCreateEvent->uiTriggerObjectTblid);
		}
	}
	else if(pMsg.Id == g_EventSobDelete )
	{
		SNtlEventSobDelete* pEvent = reinterpret_cast<SNtlEventSobDelete*>( pMsg.pData );
		UnregiLiveSobject(pEvent->hSerialId);
	}

	NTL_RETURNVOID();
}