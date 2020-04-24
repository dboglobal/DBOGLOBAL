#include "precomp_ntlsimulation.h"
#include "NtlWorldConceptDBC.h"

#include "NtlSlApi.h"
#include "NtlSlLogic.h"
#include "NtlPLVisualManager.h"
#include "NtlPlWorldEntity.h"
#include "NtlSobManager.h"
#include "NtlPLCharacter.h"
#include "NtlSob.h"
#include "NtlSLEventFunc.h"

#include "TableContainer.h"
#include "DragonBallTable.h"
#include "TextAllTable.h"
#include "NtlDragonBall.h"
#include "NtlCameraController.h"
#include "ntlSobTriggerObject.h"
#include "NPCTable.h"
#include "GUISoundDefine.h"

#define TIME_NIGHT				3.0f
#define TIME_ALTAR_EFFECT		2.0f
#define TIME_SPAWN_DRAGON		2.0f
#define TIME_NARRATION			5.0f
#define TIME_DESTROY_DRAGON		4.5f
#define TIME_DESTROY_DB			2.0f

CNtlWorldConceptDBC::CNtlWorldConceptDBC(void)
{
	m_uiAltarID = NULL;
	m_eDragonBallKind = DRAGON_BALL_TYPE_NONE;
	m_uiRemainTime = 0;
    m_bMyDragon = FALSE;
    m_bUpdateRemainTime = FALSE;
    m_hBGMHandle = INVALID_SOUND_HANDLE;
}

CNtlWorldConceptDBC::~CNtlWorldConceptDBC(void)
{
    if(m_hBGMHandle != INVALID_SOUND_HANDLE)
    {
		GetSoundManager()->Stop(m_hBGMHandle);
    }
}

RwBool CNtlWorldConceptDBC::IsEnableAction( EAvatarAction eAction ) 
{
	switch(eAction)
	{
	case E_ACTION_MOVE:
	case E_ACTION_PICK_WORLD:
	case E_ACTION_PICK_OBJECT:
	case E_ACTION_TARGETTING:
	case E_ACTION_ATTACK:
	case E_ACTION_CHANGE_HEADING:
	case E_ACTION_CAMERA_CONTROL:
	case E_ACTION_CHARGING:
	case E_ACTION_LOOTING:
		return FALSE;
	}

	return FALSE;
}

void CNtlWorldConceptDBC::Update( RwReal fElapsedTime ) 
{
	m_fStateTime += fElapsedTime;

    if(m_bUpdateRemainTime)
    {
        RwUInt32 uiElapsedTime = (RwUInt32)(fElapsedTime * 1000.0f);
        if(m_uiRemainTime - uiElapsedTime > 0)
        {
            m_uiRemainTime -= uiElapsedTime;
        }
        else
        {
            m_uiRemainTime = 0;
            m_bUpdateRemainTime = FALSE;
        }
    }

	switch(m_iWorldState)
	{
	case WORLD_STATE_ENTER:
	case WORLD_STATE_IDLE:
		break;
	case WORLD_DBC_ALTAR_UI:
		break;
	case WORLD_DBC_NIGHT_ON:
		if(m_fStateTime >= TIME_NIGHT)
		{	
			ChangeState(WORLD_DBC_ALTAR_EFFECT);
		}
		break;
	case WORLD_DBC_ALTAR_EFFECT:
		if(m_fStateTime >= TIME_ALTAR_EFFECT)
		{			
			ChangeState(WORLD_DBC_SPAWN_DRAGON);
		}
		break;	
	case WORLD_DBC_SPAWN_DRAGON: // 용신이 완전히 등장했을때
		if(m_fStateTime >= TIME_SPAWN_DRAGON)
		{
			ChangeState(WORLD_DBC_NARRATION_START);
		}
		break;
	case WORLD_DBC_NARRATION_START:
		if(m_fStateTime >= TIME_NARRATION)
		{
			// 나레이션을 끈다.
			CNtlSLEventGenerator::DBCNarration("", L"", SNtlEventDBCNarration::DBC_NARRATION_END_ATONCE);
			ChangeState(WORLD_DBC_REWARD_UI);
		}
		break;
	case WORLD_DBC_REWARD_UI:		
		break;
	case WORLD_DBC_NARRATION_END:
		if(m_fStateTime >= TIME_NARRATION)
		{
			// 나레이션을 끈다.
			CNtlSLEventGenerator::DBCNarration("", L"", SNtlEventDBCNarration::DBC_NARRATION_END_ATONCE);
			ChangeState(WORLD_DBC_DESTROY_DRAGON);
		}
		break;
	case WORLD_DBC_DESTROY_DRAGON:
		if(m_fStateTime >= TIME_DESTROY_DRAGON)
		{
			ChangeState(WORLD_DBC_DESTROY_DB);
		}
		break;
	case WORLD_DBC_DESTROY_DB:
		if(m_fStateTime >= TIME_DESTROY_DB)
		{
			ChangeState(WORLD_DBC_NIGHT_OFF);
		}
		break;
	case WORLD_DBC_NIGHT_OFF:
		if(m_fStateTime >= TIME_NIGHT)
		{
			ChangeState(WORLD_STATE_EXIT);
		}
		break;
	case WORLD_STATE_EXIT:
		CNtlSLEventGenerator::CameraControlDelete(CAMERA_CONTROL_DRAGONBALL);
		ChangeState(WORLD_STATE_NONE);				
		break;
	}
}



void CNtlWorldConceptDBC::ChangeState( RwInt32 iState ) 
{
	CNtlWorldConceptController::ChangeState(iState);

	switch(iState)
	{
	case WORLD_DBC_NIGHT_ON:			
        {
            // 용신 등장시의 배경음악 재생
            CNtlSob* pSobAltar = GetNtlSobManager()->GetSobObject(GetAltarSerialID());
            RwV3d vPos = pSobAltar->GetPosition();
            sNtlSoundPlayParameta tSoundParam;
            tSoundParam.iChannelGroup	= CHANNEL_GROUP_JINGLE_MUSIC;
            tSoundParam.pcFileName		= GSD_JINGLE_DRAGON;
            tSoundParam.fXPos			= vPos.x;
            tSoundParam.fYPos			= vPos.y;
            tSoundParam.fZPos			= vPos.z;
            tSoundParam.bLoop           = TRUE;

			GetSoundManager()->Play(&tSoundParam);
            m_hBGMHandle = tSoundParam.hHandle;
        }

        CNtlPLGlobal::m_fLensFlareForDNRatio = 3.0f;
		break;
	case WORLD_DBC_SPAWN_DRAGON:		          
		break;
	case WORLD_DBC_NARRATION_START:
		{
			sDRAGONBALL_TBLDAT* pData = (sDRAGONBALL_TBLDAT*)API_GetTableContainer()->GetDragonBallTable()->GetDBTbldat((BYTE)m_eDragonBallKind);
			if(pData)
			{
                // 일러스트
                std::string strIllust = "";
                sBOT_TBLDAT* pDragonTable = (sBOT_TBLDAT*)API_GetTableContainer()->GetNpcTable()->FindData(pData->dragonNPCTblidx);
                if(pDragonTable)
                {
                    strIllust = pDragonTable->szILLust;
                    strIllust += "_0";
                }
				std::wstring strDialog = API_GetTableContainer()->GetTextAllTable()->GetDragonTbl()->GetText(pData->startDialog);
				CNtlSLEventGenerator::DBCNarration(strIllust.c_str(),strDialog.c_str(), SNtlEventDBCNarration::DBC_NARRATION_START);
			}
		}		
		break;
	case WORLD_DBC_REWARD_UI:				
		break;
	case WORLD_DBC_DESTROY_DB:
        CNtlSLEventGenerator::DBCScatter();         ///< 드래곤볼 흩어지는 연출을 카메라 연출을 위해 매니저에게 알려준다.
		break;
	case WORLD_DBC_NARRATION_END:
		{
			sDRAGONBALL_TBLDAT* pData = (sDRAGONBALL_TBLDAT*)API_GetTableContainer()->GetDragonBallTable()->GetDBTbldat((BYTE)m_eDragonBallKind);
			if(pData)
			{
                // 일러스트
                std::string strIllust = "";
                sBOT_TBLDAT* pDragonTable = (sBOT_TBLDAT*)API_GetTableContainer()->GetNpcTable()->FindData(pData->dragonNPCTblidx);
                if(pDragonTable)
                {
                    strIllust = pDragonTable->szILLust;
                    strIllust += "_0";
                }                
				std::wstring strDialog = API_GetTableContainer()->GetTextAllTable()->GetDragonTbl()->GetText(pData->endDialog);
				CNtlSLEventGenerator::DBCNarration(strIllust.c_str(),strDialog.c_str(), SNtlEventDBCNarration::DBC_NARRATION_START);
			}
		}		
		break;
	case WORLD_DBC_NIGHT_OFF:		
        CNtlPLGlobal::m_fLensFlareForDNRatio = 1.0f;
		GetSoundManager()->Stop(m_hBGMHandle);
        m_hBGMHandle = INVALID_SOUND_HANDLE;
		break;
	}
}

void CNtlWorldConceptDBC::ResetRemainTime() 
{
    // 서버와의 시간 씽크를 위해서 약간의 오프셋값을 준다. (정확하게 맞추는것은 불가능하다)
	m_uiRemainTime = DBO_MAX_TIMER_DB_UI_OK - 2000;
    m_bUpdateRemainTime = TRUE;
}

void CNtlWorldConceptDBC::AddDragonBall( CNtlSobItem* pItem ) 
{
	m_listDragonBall.push_back(pItem);
}

void CNtlWorldConceptDBC::RemoveDragonBall( CNtlSobItem* pItem ) 
{
	m_listDragonBall.remove(pItem);
}

RwUInt32 CNtlWorldConceptDBC::GetReaminTime() 
{
    m_uiRemainTime = max(0, m_uiRemainTime);

    return m_uiRemainTime;
}
