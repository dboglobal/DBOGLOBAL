#include "precomp_dboclient.h"
#include "CharacterStage.h"

// network
#include "NtlClientNet.h"

// core
#include "NtlDebug.h"
#include "NtlEventQueue.h"
#include "NtlNetSender.h"

// Presentation
#include "NtlPLSceneManager.h"
#include "NtlPLApi.h"
#include "NtlWorldCommon.h"
#include "NtlPLEventGenerator.h"

// Simulation
#include "NtlSLGlobal.h"
#include "NtlSLEventFunc.h"
#include "NtlSobProxy.h"

// dbo
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DboPacketGenerator.h"
#include "CharSelectGui.h"
#include "NtlCameraManager.h"
#include "CharStageState.h"
#include "NtlGameTableContainer.h"

// shared
#include "PCTable.h"
#include "NewbieTable.h"
#include "ItemTable.h"

CCharacterStage::CCharacterStage(const char *pStageName)
:CNtlStage(pStageName)
,m_pState(NULL)
,m_pWorldEntity(NULL)
,m_pAvatar(NULL)
,m_pObjectEntity(NULL)
,m_pLtAmbient(NULL)
,m_pLtDirectional(NULL)
,m_pSkyEntity(NULL)
,m_pGuiGroup(NULL)
,m_bWorldAddCamera(FALSE)
{

}

CCharacterStage::~CCharacterStage()
{
}

void CCharacterStage::Init(void)
{
}

bool CCharacterStage::Create(void)
{
	NTL_FUNCTION("CCharacterStage::Create" );

	// event link
	LinkMsg(g_EventUpdateTick);
	LinkMsg(g_EventCharMake);
	LinkMsg(g_EventLobbyMessage);
	LinkMsg(g_EventCharStageStateEnter);

	// state create 
	m_pState = NTL_NEW CCharStageState;
			
	//Create World
	SPLDummyWorldCreateParam sCreateParam;
	sCreateParam.bb.inf.x = -1000.0f; sCreateParam.bb.inf.y = -1000.0f; sCreateParam.bb.inf.z = -1000.0f;
	sCreateParam.bb.sup.x = sCreateParam.bb.sup.y = sCreateParam.bb.sup.z = 1000.0f;
	m_pWorldEntity = (CNtlPLDummyWorld*)GetSceneManager()->CreateEntity(PLENTITY_DUMMY_WORLD, "NULL", NULL);
	
	//Camera
	RpWorldAddCamera(CNtlPLGlobal::m_pRpWorld, CNtlPLGlobal::m_RwCamera);
	m_bWorldAddCamera = TRUE;
	RwV3d vCamPos = { 0.f, 0.54198f, 5.6921f };
	RwV3d vTargetPos = { 0.f, 1.f, 0.f };
	API_PL_CameraLookAt(CNtlPLGlobal::m_RwCamera, &vCamPos, &vTargetPos);
	GetNtlGameCameraManager()->SetFov(45.f);
	
	//Create Object and Create Sky
	CreateBackGroundStage(RACE_UNKNOWN);
	RwV3d vPos = { 0.f, -1.f, 0.f };
	if(m_pSkyEntity == NULL)
	{
		//Create Sky
		m_pSkyEntity = (CNtlPLSky *)GetSceneManager()->CreateEntity( PLENTITY_SKY, "null" );
		m_pSkyEntity->SetPosition(&vPos);
		sNTL_FIELD_PROP NtlFieldProp;
		CNtlPLEventGenerator::IsAnotherFieldChanged(&NtlFieldProp);
	}

	// Ambient Light Create
    m_pLtAmbient = RpLightCreate(rpLIGHTAMBIENT);
    RwRGBAReal color = {0.9f, 0.9f, 0.9f, 1.0f};
    RpLightSetColor(m_pLtAmbient, &color);
    RpWorldAddLight(CNtlPLGlobal::m_pRpWorld, m_pLtAmbient);

    // Directional Light Create
    m_pLtDirectional = RpLightCreate(rpLIGHTDIRECTIONAL);
    if(m_pLtDirectional)
    {
        RwFrame* pFrame = NULL;		
        RpLightSetColor(m_pLtDirectional, &color);
		pFrame = RwFrameCreate();
        if(pFrame)
        {
            RwFrameRotate(pFrame, &CNtlPLGlobal::m_vXAxisV3, 135.0f, rwCOMBINEREPLACE);
            RwFrameRotate(pFrame, &CNtlPLGlobal::m_vYAxisV3, -45.0f, rwCOMBINEPOSTCONCAT);
			RpLightSetFrame(m_pLtDirectional, pFrame);
            RpWorldAddLight(CNtlPLGlobal::m_pRpWorld, m_pLtDirectional);
        }
    }
	
	// Avatar Option
	m_iAvatarRotState = LMT_ROTATION_STOP;
	m_fRotSpeedPerSec = 20.f;

	// Gui Group
	m_pGuiGroup = new CCharGuiGroup;
	if(!m_pGuiGroup->Create())
	{
		NTL_DELETE(m_pGuiGroup);
		NTL_RETURN(false);
	}
	
	m_pState->ChangeState(CHAR_STATE_SERVER_INFORMAION);

	CEventTimer::RemoveAll();
	
	NTL_RETURN(true);
}

void CCharacterStage::Destroy(void)
{
	NTL_FUNCTION("CCharacterStage::Destroy" );
	
	// event unlink
	UnLinkMsg(g_EventUpdateTick);
	UnLinkMsg(g_EventCharMake);
	UnLinkMsg(g_EventLobbyMessage);
	UnLinkMsg(g_EventCharStageStateEnter);

	// state destroy
	if(m_pState)
	{
		m_pState->Destroy();
	}

	DestroyAvatar();
		
	if(m_pLtAmbient && m_pLtDirectional)
	{
		RpWorldRemoveLight(CNtlPLGlobal::m_pRpWorld, m_pLtAmbient);
		RpWorldRemoveLight(CNtlPLGlobal::m_pRpWorld, m_pLtDirectional);

		RwFrame* frame = RpLightGetFrame(m_pLtDirectional);
		RpLightSetFrame(m_pLtDirectional, NULL);
		RwFrameDestroy(frame);
		RpLightDestroy(m_pLtDirectional);
		RpLightDestroy(m_pLtAmbient);
	}
	
	if(m_pObjectEntity)
	{
		GetSceneManager()->DeleteEntity(m_pObjectEntity);
		m_pObjectEntity = NULL;
	}

	if(m_pSkyEntity)
	{
		GetSceneManager()->DeleteEntity(m_pSkyEntity);
		m_pSkyEntity = NULL;
	}
	
	if(m_bWorldAddCamera)
		RpWorldRemoveCamera(CNtlPLGlobal::m_pRpWorld, CNtlPLGlobal::m_RwCamera);

	if(m_pWorldEntity)
	{
		GetSceneManager()->DeleteEntity(m_pWorldEntity);
		m_pWorldEntity = NULL;
	}

	if(m_pGuiGroup)
	{
		m_pGuiGroup->Destroy();
		delete m_pGuiGroup;
		m_pGuiGroup = NULL;
	}	

	NTL_RETURNVOID();
}

void CCharacterStage::HandleEvents(RWS::CMsg &pMsg)
{	
	if(pMsg.Id == g_EventUpdateTick)
	{
		float fElapsed = *reinterpret_cast<float*>(pMsg.pData);
		EventProcUpdateTick(fElapsed);
	}
	else if( pMsg.Id == g_EventCharMake )
	{
		SNtlEventCharMake* pCharMake = reinterpret_cast<SNtlEventCharMake*>(pMsg.pData);
		CreateDummyAvatar( pCharMake->byRace, pCharMake->byClass, pCharMake->byGender, pCharMake->byFace, pCharMake->byHair, pCharMake->bySkinColor, pCharMake->byHairColor);
	}
	else if( pMsg.Id == g_EventLobbyMessage )
	{
		SNtlEventLobbyMessage* pLobbyMessage = reinterpret_cast<SNtlEventLobbyMessage*>(pMsg.pData);
		if(pLobbyMessage->byMessage == LMT_SELECTE_CHARACTER)
		{
			RwUInt32 uiCharIndex = (RwUInt32)pLobbyMessage->fValue;
			if(uiCharIndex == INVALID_INDEX)
			{
				CreateBackGroundStage(INVALID_INDEX);
				DestroyAvatar();
			}
			else
			{
				SERIAL_HANDLE hServer = GetDboGlobal()->GetSelectedServerHandle();
				LobbyInfo* pLobbyInfo = GetDboGlobal()->GetLobbyInfo(hServer);
				sPC_SUMMARY charInfo = pLobbyInfo->Summary[uiCharIndex];
				
				CreateBackGroundStage(charInfo.byRace);
				CreateDownLoadAvatar(&charInfo);
			}
		}
		else if(pLobbyMessage->byMessage == LMT_ROTATION_LEFT ||
			    pLobbyMessage->byMessage == LMT_ROTATION_RIGHT ||
				pLobbyMessage->byMessage == LMT_ROTATION_STOP)
		{
			m_iAvatarRotState = (RwInt32)pLobbyMessage->byMessage;
			m_fRotSpeedPerSec = pLobbyMessage->fValue;

		}
		else if(pLobbyMessage->byMessage == LMT_DESTORY_LOBBY_AVATAR)
		{
			DestroyAvatar();
		}
	}
	else if( pMsg.Id == g_EventCharStageStateEnter)
	{
		SDboEventCharStageStateEnter* pStageStateEnter = reinterpret_cast<SDboEventCharStageStateEnter*>(pMsg.pData);

		if(pStageStateEnter->byState == CHAR_STATE_MAKE_ENTER)
			CreateBackGroundStage(INVALID_INDEX);
	}
}


void CCharacterStage::EventProcUpdateTick(RwReal fElapsed)
{
	m_pState->Update(fElapsed);

	if(m_pAvatar != NULL && m_iAvatarRotState != LMT_ROTATION_STOP)
	{
		CNtlSobProxy *pSobProxy = m_pAvatar->GetSobProxy();
		CNtlPLCharacter *pCharacter = (CNtlPLCharacter *)pSobProxy->GetPLMainEntity();
		
		if(pCharacter)
		{
			if(m_iAvatarRotState == LMT_ROTATION_LEFT)
			{
				pCharacter->SetAngleY(pCharacter->GetAngleY() - m_fRotSpeedPerSec * fElapsed);
			}
			else if(m_iAvatarRotState == LMT_ROTATION_RIGHT)
			{
				pCharacter->SetAngleY(pCharacter->GetAngleY() + m_fRotSpeedPerSec * fElapsed);
			}
		}
	}
}

void CCharacterStage::OnTimer(void)
{
	
}

void CCharacterStage::CreateBackGroundStage( RwUInt32 uiRace)
{
	RwV3d vPos = { 0.f, 0.f, 0.f };
	SPLEntityCreateParam PLEntityCreateParam;
	PLEntityCreateParam.pPos = &vPos;
	
	if(m_pObjectEntity != NULL)
	{
		GetSceneManager()->DeleteEntity(m_pObjectEntity);
		m_pObjectEntity = NULL;
	}

	if(m_pObjectEntity == NULL)
	{
		if(uiRace == RACE_HUMAN)
			m_pObjectEntity = (CNtlPLObject *)GetSceneManager()->CreateEntity(PLENTITY_OBJECT, "UI_HUMAN_BACK", &PLEntityCreateParam);
		else if(uiRace == RACE_NAMEK)
			m_pObjectEntity = (CNtlPLObject *)GetSceneManager()->CreateEntity(PLENTITY_OBJECT, "UI_NAMEK_BACK", &PLEntityCreateParam);
		else
			m_pObjectEntity = (CNtlPLObject *)GetSceneManager()->CreateEntity(PLENTITY_OBJECT, "UI_BACKGROUND", &PLEntityCreateParam);
	}
}

void CCharacterStage::CreateDummyAvatar( BYTE byRace, BYTE byClass, BYTE byGender, BYTE byFace, BYTE byHair, BYTE bySkinColor, BYTE byHairColor)
{
	SAvatarInfo *pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
	RwReal fCurAnimTime = 0.f;
	if(m_pAvatar)
	{
		CNtlSobProxy *pSobProxy = m_pAvatar->GetSobProxy();
		CNtlPLCharacter *pCharacter = (CNtlPLCharacter *)pSobProxy->GetPLMainEntity();
		fCurAnimTime = pCharacter->GetBaseCurrentAnimTime();
		DestroyAvatar();
	}

	SERIAL_HANDLE hSerialId = 1;
	pAvatarInfo->uiSerialId = hSerialId++;		

	CPCTable *pPcTable = GetGameTableContainer()->GetPCTable();
	sPC_TBLDAT *pTblData = (sPC_TBLDAT*)pPcTable->GetPcTbldat(byRace, byClass, byGender);   
	pAvatarInfo->sCharPf.tblidx = pTblData->tblidx;	
	
	//Character Name
	//wcscpy_s(pAvatarInfo->sCharPf.awchName, NTL_MAX_SIZE_CHAR_NAME_UNICODE, L"동동동");

	pAvatarInfo->sCharPf.sPcShape.byFace = byFace;
	pAvatarInfo->sCharPf.sPcShape.byHair = byHair;
	pAvatarInfo->sCharPf.sPcShape.byHairColor = byHairColor;
	pAvatarInfo->sCharPf.sPcShape.bySkinColor = bySkinColor;
	
	pAvatarInfo->sCharPf.avatarAttribute.byBaseStr = 1;
	pAvatarInfo->sCharPf.avatarAttribute.byLastStr = 1;
	pAvatarInfo->sCharPf.avatarAttribute.byBaseCon = 1;
	pAvatarInfo->sCharPf.avatarAttribute.byLastCon = 1;
	pAvatarInfo->sCharPf.avatarAttribute.byBaseFoc = 1;
	pAvatarInfo->sCharPf.avatarAttribute.byLastFoc = 1;
	pAvatarInfo->sCharPf.avatarAttribute.byBaseDex = 1;
	pAvatarInfo->sCharPf.avatarAttribute.byLastDex = 1;
	pAvatarInfo->sCharPf.avatarAttribute.byBaseSol = 1;
	pAvatarInfo->sCharPf.avatarAttribute.byLastSol = 1;
	pAvatarInfo->sCharPf.avatarAttribute.byBaseEng = 1;
	pAvatarInfo->sCharPf.avatarAttribute.byLastEng = 1;

	pAvatarInfo->sCharPf.wCurLP = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wBaseMaxLP = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wLastMaxLP = 100;
	pAvatarInfo->sCharPf.wCurEP = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wBaseMaxEP = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wLastMaxEP = 100;
	pAvatarInfo->sCharPf.wCurRP = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wBaseMaxRP = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wLastMaxRP = 100;

	pAvatarInfo->sCharPf.avatarAttribute.wBasePhysicalOffence = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wLastPhysicalOffence = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wBasePhysicalDefence = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wLastPhysicalDefence = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wBaseEnergyOffence = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wLastEnergyOffence = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wBaseEnergyDefence = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wLastEnergyDefence = 100;
		
	pAvatarInfo->sCharPf.avatarAttribute.wBaseAttackRate = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wLastAttackRate = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wBaseDodgeRate = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wLastDodgeRate = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wBaseBlockRate = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wLastBlockRate = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wLastPhysicalCriticalRate = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wLastEnergyCriticalRate = 100;
		
	pAvatarInfo->sCharPf.avatarAttribute.fLastRunSpeed = 8.0f;
	pAvatarInfo->sCharPf.byLevel = 1;
	pAvatarInfo->sCharPf.dwCurExp = 1;

	pAvatarInfo->sCharState.sCharStateBase.vCurLoc.x = 0.0f;
	pAvatarInfo->sCharState.sCharStateBase.vCurLoc.y = 0.0f;
	pAvatarInfo->sCharState.sCharStateBase.vCurLoc.z = 0.0f;

	pAvatarInfo->sCharState.sCharStateBase.vCurDir.x = 0.0f;  
	pAvatarInfo->sCharState.sCharStateBase.vCurDir.y = 0.0f;  
	pAvatarInfo->sCharState.sCharStateBase.vCurDir.z = 1.0f;  

	pAvatarInfo->byItemCount = 0;
	pAvatarInfo->bySkillCount = 0;
	pAvatarInfo->byHTBCount = 0;

	pAvatarInfo->sCharState.sCharStateBase.vCurLoc.x = 0.0f;
	pAvatarInfo->sCharState.sCharStateBase.vCurLoc.y = 0.0f;
	pAvatarInfo->sCharState.sCharStateBase.vCurLoc.z = 0.0f;

	pAvatarInfo->sCharState.sCharStateBase.vCurDir.x = 0.0f;  
	pAvatarInfo->sCharState.sCharStateBase.vCurDir.y = 0.0f;  
	pAvatarInfo->sCharState.sCharStateBase.vCurDir.z = 1.0f;  
	
	pAvatarInfo->sCharState.sCharStateBase.byStateID = CHARSTATE_STANDING;

	pAvatarInfo->sWorldInfo.tblidx = 0;
	pAvatarInfo->sWorldInfo.worldID = 0;

	
	// Newbie Item Create
	CNewbieTable *pNewbieTable = GetGameTableContainer()->GetNewbieTable();
	sNEWBIE_TBLDAT *pNewbieTblData = reinterpret_cast<sNEWBIE_TBLDAT*>( pNewbieTable->GetNewbieTbldat(byRace, byClass) );

	pAvatarInfo->byItemCount = 0;
	for(RwInt32 i = 0; i < NTL_MAX_NEWBIE_ITEM; i++)
	{
		if(pNewbieTblData->aitem_Tblidx[i] == INVALID_TBLIDX)
			continue;
		
		if(pNewbieTblData->abyPos[i] == EQUIP_SLOT_TYPE_UNKNOWN)
			continue;

		pAvatarInfo->sItemPf[pAvatarInfo->byItemCount].handle				= hSerialId++;
		pAvatarInfo->sItemPf[pAvatarInfo->byItemCount].tblidx				= pNewbieTblData->aitem_Tblidx[i];
		pAvatarInfo->sItemPf[pAvatarInfo->byItemCount].byPlace				= CONTAINER_TYPE_EQUIP;
		pAvatarInfo->sItemPf[pAvatarInfo->byItemCount].byPos				= pNewbieTblData->abyPos[i];
		pAvatarInfo->sItemPf[pAvatarInfo->byItemCount].byStackcount			= 1;
		pAvatarInfo->sItemPf[pAvatarInfo->byItemCount].byRank				= ITEM_RANK_NORMAL;
		pAvatarInfo->sItemPf[pAvatarInfo->byItemCount].byCurDur;			// 여기는 채워 넣으시면 됩니다.
		pAvatarInfo->sItemPf[pAvatarInfo->byItemCount].Option				= INVALID_TBLIDX;
		pAvatarInfo->sItemPf[pAvatarInfo->byItemCount].bNeedToIdentify		= FALSE;
		pAvatarInfo->sItemPf[pAvatarInfo->byItemCount].byGrade				= 0;	
		pAvatarInfo->byItemCount++;
	}

	m_pAvatar = reinterpret_cast<CNtlSobAvatar*>(CNtlSLEventGenerator::SobAvatarCreate(SLCLASS_AVATAR, pAvatarInfo->uiSerialId, &pAvatarInfo->sCharState));
	if(m_pAvatar)
	{
		RwV3d vPos = {0.f, 0.f, 0.f };
		CNtlSobProxy *pSobProxy = m_pAvatar->GetSobProxy();
		CNtlPLCharacter *pCharacter = (CNtlPLCharacter *)pSobProxy->GetPLMainEntity();
		m_pAvatar->SetPosition(&vPos);
		pCharacter->SetVisible(TRUE);
		pCharacter->SetBaseCurrentAnimTime(fCurAnimTime);
	}
}

void CCharacterStage::CreateDownLoadAvatar(sPC_SUMMARY *pCharInfo )
{

	BYTE byRace		= pCharInfo->byRace;
    BYTE byClass	= pCharInfo->byClass;
	BYTE byGender   = pCharInfo->byGender;
	BYTE byFace	    = pCharInfo->byFace;
	BYTE byHair		= pCharInfo->byHair;
	BYTE bySkinColor= pCharInfo->bySkinColor;
	BYTE byHairColor= pCharInfo->byHairColor;
	
	CPCTable *pPcTable = GetGameTableContainer()->GetPCTable();
	sPC_TBLDAT *pTblData = (sPC_TBLDAT*)pPcTable->GetPcTbldat(byRace, byClass, byGender);   
	
	SAvatarInfo *pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
	RwReal fCurAnimTime = 0.f;
	if(m_pAvatar)
	{
		CNtlSobProxy *pSobProxy = m_pAvatar->GetSobProxy();
		CNtlPLCharacter *pCharacter = (CNtlPLCharacter *)pSobProxy->GetPLMainEntity();
		fCurAnimTime = pCharacter->GetBaseCurrentAnimTime();
		DestroyAvatar();
	}

	pAvatarInfo->sCharPf.tblidx = pTblData->tblidx;	
	pAvatarInfo->uiSerialId = pCharInfo->charId;
	
	//CItemTable *pItemTbl = GetGameTableContainer()->GetItemTable();
	
	SERIAL_HANDLE hSerialId = pAvatarInfo->uiSerialId;
	hSerialId = hSerialId++;
	
	//wcscpy_s(pAvatarInfo->sCharPf.awchName, NTL_MAX_SIZE_CHAR_NAME_UNICODE, characterInfo.awchName);
	pAvatarInfo->sCharPf.sPcShape.byFace = byFace;
	pAvatarInfo->sCharPf.sPcShape.byHair = byHair;
	pAvatarInfo->sCharPf.sPcShape.byHairColor = byHairColor;
	pAvatarInfo->sCharPf.sPcShape.bySkinColor = bySkinColor;
	
	pAvatarInfo->sCharPf.avatarAttribute.byBaseStr = 1;
	pAvatarInfo->sCharPf.avatarAttribute.byLastStr = 1;
	pAvatarInfo->sCharPf.avatarAttribute.byBaseCon = 1;
	pAvatarInfo->sCharPf.avatarAttribute.byLastCon = 1;
	pAvatarInfo->sCharPf.avatarAttribute.byBaseFoc = 1;
	pAvatarInfo->sCharPf.avatarAttribute.byLastFoc = 1;
	pAvatarInfo->sCharPf.avatarAttribute.byBaseDex = 1;
	pAvatarInfo->sCharPf.avatarAttribute.byLastDex = 1;
	pAvatarInfo->sCharPf.avatarAttribute.byBaseSol = 1;
	pAvatarInfo->sCharPf.avatarAttribute.byLastSol = 1;
	pAvatarInfo->sCharPf.avatarAttribute.byBaseEng = 1;
	pAvatarInfo->sCharPf.avatarAttribute.byLastEng = 1;

	pAvatarInfo->sCharPf.wCurLP = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wBaseMaxLP = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wLastMaxLP = 100;
	pAvatarInfo->sCharPf.wCurEP = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wBaseMaxEP = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wLastMaxEP = 100;
	pAvatarInfo->sCharPf.wCurRP = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wBaseMaxRP = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wLastMaxRP = 100;

	pAvatarInfo->sCharPf.avatarAttribute.wBasePhysicalOffence = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wLastPhysicalOffence = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wBasePhysicalDefence = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wLastPhysicalDefence = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wBaseEnergyOffence = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wLastEnergyOffence = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wBaseEnergyDefence = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wLastEnergyDefence = 100;
		
	pAvatarInfo->sCharPf.avatarAttribute.wBaseAttackRate = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wLastAttackRate = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wBaseDodgeRate = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wLastDodgeRate = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wBaseBlockRate = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wLastBlockRate = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wLastPhysicalCriticalRate = 100;
	pAvatarInfo->sCharPf.avatarAttribute.wLastEnergyCriticalRate = 100;
		
	pAvatarInfo->sCharPf.avatarAttribute.fLastRunSpeed = 8.0f;
	pAvatarInfo->sCharPf.byLevel = 1;
	pAvatarInfo->sCharPf.dwCurExp = 1;

	pAvatarInfo->sCharState.sCharStateBase.vCurLoc.x = 0.0f;
	pAvatarInfo->sCharState.sCharStateBase.vCurLoc.y = 0.0f;
	pAvatarInfo->sCharState.sCharStateBase.vCurLoc.z = 0.0f;

	pAvatarInfo->sCharState.sCharStateBase.vCurDir.x = 0.0f;  
	pAvatarInfo->sCharState.sCharStateBase.vCurDir.y = 0.0f;  
	pAvatarInfo->sCharState.sCharStateBase.vCurDir.z = 1.0f;  

	pAvatarInfo->byItemCount = 0;
	pAvatarInfo->bySkillCount = 0;
	pAvatarInfo->byHTBCount = 0;

	pAvatarInfo->sCharState.sCharStateBase.vCurLoc.x = 0.0f;
	pAvatarInfo->sCharState.sCharStateBase.vCurLoc.y = 0.0f;
	pAvatarInfo->sCharState.sCharStateBase.vCurLoc.z = 0.0f;

	pAvatarInfo->sCharState.sCharStateBase.vCurDir.x = 0.0f;  
	pAvatarInfo->sCharState.sCharStateBase.vCurDir.y = 0.0f;  
	pAvatarInfo->sCharState.sCharStateBase.vCurDir.z = 1.0f;  
	pAvatarInfo->sCharState.sCharStateBase.byStateID = CHARSTATE_STANDING;
	pAvatarInfo->sWorldInfo.tblidx = 0;
	pAvatarInfo->sWorldInfo.worldID = 0;
	
	pAvatarInfo->byItemCount = 0;
	CItemTable *pItemTbl = GetGameTableContainer()->GetItemTable();
	for(RwInt32 i = 0; i < EQUIP_SLOT_TYPE_COUNT; i++)
	{
		if(pCharInfo->sItem[i].tblidx == INVALID_TBLIDX)
			continue;
		
		sITEM_TBLDAT* pItemTblData = (sITEM_TBLDAT*)GetGameTableContainer()->GetItemTable()->FindData(pCharInfo->sItem[i].tblidx);
		if( pItemTblData == NULL ||
		    pItemTblData->byEquip_Type == EQUIP_TYPE_UNKNOWN)
			continue;

		pAvatarInfo->sItemPf[pAvatarInfo->byItemCount].handle				= hSerialId++;
		pAvatarInfo->sItemPf[pAvatarInfo->byItemCount].tblidx				= pCharInfo->sItem[i].tblidx;
		pAvatarInfo->sItemPf[pAvatarInfo->byItemCount].byPlace				= CONTAINER_TYPE_EQUIP;
		pAvatarInfo->sItemPf[pAvatarInfo->byItemCount].byPos				= i;
		pAvatarInfo->sItemPf[pAvatarInfo->byItemCount].byStackcount			= 1;
		pAvatarInfo->sItemPf[pAvatarInfo->byItemCount].byRank				= pCharInfo->sItem[i].byRank;
		pAvatarInfo->sItemPf[pAvatarInfo->byItemCount].byCurDur;			// 여기는 채워 넣으시면 됩니다.
		pAvatarInfo->sItemPf[pAvatarInfo->byItemCount].Option				= pCharInfo->sItem[i].Option;
		pAvatarInfo->sItemPf[pAvatarInfo->byItemCount].bNeedToIdentify		= FALSE;
		pAvatarInfo->sItemPf[pAvatarInfo->byItemCount].byGrade				= pCharInfo->sItem[i].byGrade;
		pAvatarInfo->byItemCount++;
	}

	m_pAvatar = reinterpret_cast<CNtlSobAvatar*>(CNtlSLEventGenerator::SobAvatarCreate(SLCLASS_AVATAR, pAvatarInfo->uiSerialId, &pAvatarInfo->sCharState));
	if(m_pAvatar)
	{
		RwV3d vPos = {0.f, 0.f, 0.f };
		CNtlSobProxy *pSobProxy = m_pAvatar->GetSobProxy();
		CNtlPLCharacter *pCharacter = (CNtlPLCharacter *)pSobProxy->GetPLMainEntity();
		m_pAvatar->SetPosition(&vPos);
		pCharacter->SetVisible(TRUE);
		pCharacter->SetBaseCurrentAnimTime(fCurAnimTime);
	}
}

void CCharacterStage::DestroyAvatar()
{
	if(m_pAvatar)
	{
		SAvatarInfo *pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
		CNtlSLEventGenerator::SobDelete(pAvatarInfo->uiSerialId);
		m_pAvatar = NULL;
		GetNtlSLGlobal()->SetSobAvatar(NULL);
	}	
}


