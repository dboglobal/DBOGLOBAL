#include "precomp_ntlsimulation.h"
#include "NtlWorldConceptPVP.h"
#include "NtlWorldConcept.h"

// sound
#include "NtlSoundEventGenerator.h"
#include "GUISoundDefine.h"

// presentation
#include "NtlPLCharacter.h"
#include "NtlPLSceneManager.h"

// simulation
#include "NtlSLLogic.h"
#include "NtlSLVisualDeclear.h"
#include "NtlSobManager.h"
#include "NtlSobPlayer.h"


CNtlWorldConceptPVP::CNtlWorldConceptPVP(void)
{
	ZeroMemory(&m_FreeBattleInfo, sizeof(FreeBattleInfo));
	m_FreeBattleInfo.hFreePvPTarget = INVALID_SERIAL_ID;
}

CNtlWorldConceptPVP::~CNtlWorldConceptPVP(void)
{
	DestroyFreePvPMark();
}

void CNtlWorldConceptPVP::Update( RwReal fElapsedTime ) 
{
	switch(m_iWorldState)
	{
	case WORLD_STATE_ENTER:
		if(m_FreeBattleInfo.pCharacter == NULL)
		{
			CreateFreePvPMark();
		}

		if(m_FreeBattleInfo.pCharacter)
		{
			// PVP 마크(심판 캐릭터)의 등장 부분. 알파값과 Ink를 조절한다.
			RwUInt8 byAlpha = (RwUInt8)((RwReal)255*m_fStateTime/2.0f);
			m_FreeBattleInfo.pCharacter->SetAlpha(byAlpha);

			RwReal fThickness = ((RwReal)DEFAULT_INK_THICKNESS*m_fStateTime/2.0f); 
			m_FreeBattleInfo.pCharacter->SetInkThickness(fThickness);
		}

		m_fStateTime += fElapsedTime;

		if(m_fStateTime >= 2.0f)
		{
			if(m_FreeBattleInfo.pCharacter)
			{
				m_FreeBattleInfo.pCharacter->SetAlpha(255);
				m_FreeBattleInfo.pCharacter->SetInkThickness(DEFAULT_INK_THICKNESS);
			}

			ChangeState(WORLD_STATE_IDLE);
		}
		
		break;
	case WORLD_STATE_IDLE:
		break;
	case WORLD_STATE_EXIT:
		if(m_FreeBattleInfo.pCharacter)
		{
			// PVP 마크(심판 캐릭터)의 소멸 부분
			RwUInt8 byAlpha = 255 - (RwUInt8)((RwReal)255*m_fStateTime/2.0f);
			m_FreeBattleInfo.pCharacter->SetAlpha(byAlpha);

			RwReal fThickness = DEFAULT_INK_THICKNESS - ((RwReal)DEFAULT_INK_THICKNESS*m_fStateTime/2.0f); 
			m_FreeBattleInfo.pCharacter->SetInkThickness(fThickness);
		}
		else
		{
			ChangeState(WORLD_STATE_NONE);
		}

		m_fStateTime += fElapsedTime;
		if(m_fStateTime >= 2.0f)
		{
			DestroyFreePvPMark();
			ChangeState(WORLD_STATE_NONE);
		}		
		break;
	}
}

void CNtlWorldConceptPVP::CreateFreePvPMark( void ) 
{
	if(m_FreeBattleInfo.pCharacter == NULL)
	{
		SPLCharacterCreateParam sParam;
		sParam.pPos			= &m_FreeBattleInfo.vFreePvPPos;
		sParam.uiSerialId	= INVALID_SERIAL_ID;
		sParam.bPcFlag		= FALSE;
		sParam.uiClass		= 0;
		sParam.uiRace		= 0;
		sParam.uiGender		= 0;
		sParam.uiFaceType	= 0;
		sParam.uiHeadType	= 0;
		sParam.uiHeadColorType = 0;
		sParam.uiSkinColorType = 0;
		sParam.bNotShadingFlag = FALSE;

		m_FreeBattleInfo.pCharacter = static_cast<CNtlPLCharacter*>(GetSceneManager()->CreateEntity(PLENTITY_CHARACTER, NTL_VID_FREEPVP_MARK, &sParam));
		NTL_ASSERT(m_FreeBattleInfo.pCharacter, "CNtlWorldConcept::CreateFreePvPMark is NULL  : " << NTL_VID_FREEPVP_MARK);

        m_FreeBattleInfo.pBoundEffect = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_FREEPVP_BOUND_EFFECT);
        if(m_FreeBattleInfo.pBoundEffect)
        {
            m_FreeBattleInfo.pBoundEffect->SetPosition(&m_FreeBattleInfo.vFreePvPPos);
        }
	}

	if(m_FreeBattleInfo.pCharacter)
	{
		SWorldHeightStuff sHStuff;
		Logic_GetWorldHeight(&m_FreeBattleInfo.vFreePvPPos, sHStuff);
		m_FreeBattleInfo.vFreePvPPos.y = sHStuff.fFinialHeight;

		m_FreeBattleInfo.pCharacter->SetPosition(&m_FreeBattleInfo.vFreePvPPos);

        if(m_FreeBattleInfo.pBoundEffect)
            m_FreeBattleInfo.pBoundEffect->SetPosition(&m_FreeBattleInfo.vFreePvPPos);
	}
}

void CNtlWorldConceptPVP::DestroyFreePvPMark( void ) 
{
	if(m_FreeBattleInfo.pCharacter)
	{
		GetSceneManager()->DeleteEntity(m_FreeBattleInfo.pCharacter);
		m_FreeBattleInfo.pCharacter = NULL;

        if(m_FreeBattleInfo.pBoundEffect)
        {
            GetSceneManager()->DeleteEntity(m_FreeBattleInfo.pBoundEffect);
            m_FreeBattleInfo.pBoundEffect = NULL;
        }
	}
}

void CNtlWorldConceptPVP::SetPvpName(SERIAL_HANDLE hTarget)
{
	if (hTarget == INVALID_SERIAL_ID)
		return;

	CNtlSob* pSob = GetNtlSobManager()->GetSobObject(hTarget);
	NTL_ASSERT(pSob, "Not exist sob player of pvp target handle : " << hTarget);

	NTL_ASSERT(pSob->GetClassID() == SLCLASS_PLAYER, "It is not sob player");

	// to do
}

void CNtlWorldConceptPVP::ChangeState( RwInt32 iState ) 
{
	CNtlWorldConceptController::ChangeState(iState);

	switch(iState)
	{
	case WORLD_STATE_ENTER:
		{
			CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_START_PVP_BGM);
/*
			sNtlSoundPlayParameta tSoundParam;
			tSoundParam.iChannelGroup	= CHANNEL_GROUP_JINGLE_MUSIC_WITHOUT_FADE;
			tSoundParam.pcFileName		= GSD_JINGLE_PVP_START;
			GetSoundManager()->Play(&tSoundParam);
			*/
			break;
		}		
		case WORLD_STATE_EXIT:
		{
			SERIAL_HANDLE hFreePvPTarget = m_FreeBattleInfo.hFreePvPTarget;
			m_FreeBattleInfo.hFreePvPTarget = INVALID_SERIAL_ID;

			SetPvpName(hFreePvPTarget);

			CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_END_PVP_BGM);

			sNtlSoundPlayParameta tSoundParam;
			tSoundParam.iChannelGroup	= CHANNEL_GROUP_JINGLE_MUSIC_WITHOUT_FADE;
			tSoundParam.pcFileName		= GSD_JINGLE_PVP_END;
			GetSoundManager()->Play(&tSoundParam);
			break;
		}		
	}	
}