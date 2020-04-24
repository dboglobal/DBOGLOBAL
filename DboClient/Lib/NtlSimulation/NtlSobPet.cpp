#include "precomp_ntlsimulation.h"
#include "NtlSobPet.h"

// shared
#include "NpcTable.h"
#include "NtlCharacter.h"

// core
#include "NtlDebug.h"
#include "NtlMath.h"

// simulation
#include "NtlBehaviorData.h"
#include "NtlSobAttr.h"
#include "NtlSobPetAttr.h"
#include "NtlSobPlayer.h"
#include "NtlSobCharProxy.h"
#include "NtlSLLogic.h"
#include "NtlFSMCharActLayer.h"
#include "NtlSobManager.h"



DEFINITION_MEMORY_POOL(CNtlSobPet)

CNtlSobPet::CNtlSobPet()
{
	/*SetFlags(SLFLAG_ADD_UPDATE | SLFLAG_CAN_SKILLUSE | SLFLAG_OBJECT_COLLISION | SLFLAG_LUA_INTERLOCKING | SLFLAG_LUA_TRANSFORM_ENABLE |
			SLFLAG_TARGET_SELECT_GUI_UPDATE);*/
	/*SetActorFlags(SLFLAG_ACTOR_HAVE_LIFE | SLFLAG_ACTOR_HAVE_MOVEABLE | SLFLAG_ACTOR_HAVE_BATTLE);*/

	SetFlags(SLFLAG_ADD_UPDATE | SLFLAG_CAN_SKILLUSE | SLFLAG_CAN_GROUND_FLY | SLFLAG_LUA_INTERLOCKING | 
		SLFLAG_LUA_TRANSFORM_ENABLE | SLFLAG_TARGET_SELECT_GUI_UPDATE);
	SetActorFlags(SLFLAG_ACTOR_HAVE_LIFE | SLFLAG_ACTOR_HAVE_MOVEABLE | SLFLAG_ACTOR_HAVE_BATTLE);
}

CNtlSobPet::~CNtlSobPet()
{
}

RwBool CNtlSobPet::Create(void)
{
	NTL_FUNCTION("CNtlSobPet::Create");

	// proxy 설정
	m_pSobProxy = NTL_NEW CNtlSobCharProxy;
	m_pSobProxy->Create(0);
	m_pSobProxy->SetSobObj(this);

	// control data create
	m_pBeData = NTL_NEW CNtlBeCharData; 
	
	if(!CNtlSobFeelingActor::Create())
	{
		NTL_RETURN(FALSE);
	}

	// class name 설정.
	SetClassName(SLCLASS_NAME_PET);

	NTL_RETURN(TRUE);
}

void CNtlSobPet::Destroy(void)
{
	NTL_FUNCTION("CNtlSobPet::Destroy");

	// ower에 pet 해제.
	CNtlSobFeelingActor *pSobFeelingActor = reinterpret_cast<CNtlSobFeelingActor*>( GetNtlSobManager()->GetSobObject(GetOwnerID()) );
	if(pSobFeelingActor)
		pSobFeelingActor->GetPetBuffer()->RemovePet(GetSerialID());

	// behavior data
	NTL_DELETE( m_pBeData );

	// actor destroy
	CNtlSobFeelingActor::Destroy();
	
	NTL_RETURNVOID();
}

void CNtlSobPet::Update(RwReal fElapsed)
{
	CNtlSobFeelingActor::Update(fElapsed);
}

CNtlFSMLayer* CNtlSobPet::CreateFSM( void )
{
	CNtlFSMLayer *pFSMLayer = NTL_NEW CNtlFSMCharActLayer;
	pFSMLayer->SetActor(this);
	pFSMLayer->Create(SLCONTROLLER_SERVER); 

	return pFSMLayer;
}


void CNtlSobPet::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventSobCreate)
	{
		SNtlEventSobPetCreate *pSobCreate = reinterpret_cast<SNtlEventSobPetCreate*>(pMsg.pData);

		// ower에 pet 등록.
		CNtlSobPlayer *pSobPlayer = reinterpret_cast<CNtlSobPlayer*>( GetNtlSobManager()->GetSobObject(pSobCreate->uPetBrief.pPetBrief->hOwner) );
		if(pSobPlayer)
			pSobPlayer->GetPetBuffer()->AddPet(pSobCreate->hSerialId);

		//// owner setting.
		if(pSobCreate->bOwnerAvatar)
		{
		//	m_pFSMLayer = NTL_NEW CNtlFSMCharActLayer;
		//	m_pFSMLayer->SetActor(this);
		//	m_pFSMLayer->Create(SLCONTROLLER_SUMMON_PET); 

			SetServerSyncAvatarType(pSobCreate->uPetBrief.pPetProfile->byAvatarType);
			SetOwnerID(pSobCreate->uPetBrief.pPetProfile->hOwner);

		//	RwUInt32 uiFlags = GetFlags();
		//	SetFlags(uiFlags | SLFLAG_SERVER_SENDER);
		}
		else
		{
		/*	m_pFSMLayer = NTL_NEW CNtlFSMCharActLayer;
			m_pFSMLayer->SetActor(this);
			m_pFSMLayer->Create(SLCONTROLLER_SERVER); */

			SetOwnerID(pSobCreate->uPetBrief.pPetBrief->hOwner);
		}

		// game attribute data setting.
		GetSobAttr()->HandleEvents(pMsg);

		//// skill setting
		m_SkillContainer.HandleEvents(pMsg);

		// proxy setting
		GetSobProxy()->HandleEvents(pMsg);

		// 좌표와 방향 세팅.
		RwV3d vLoc, vDir;
		RwV3dAssignMacro(&vLoc, &pSobCreate->vLoc); 
		RwV3dAssignMacro(&vDir, &pSobCreate->vDir); 

		SWorldHeightStuff sHStuff;
		Logic_GetWorldHeight(this, &vLoc, sHStuff);
		vLoc.y = sHStuff.fFinialHeight;

		SetPosition(&vLoc);
		SetDirection(&vDir);

		CNtlSobPetAttr *pSobPetAttr = reinterpret_cast<CNtlSobPetAttr*>( GetSobAttr() );
		if(pSobPetAttr->GetFlyHeight() > 0.0f)
		{
			RwUInt32 uiFlags = GetFlags();
			SetFlags(uiFlags | SLFLAG_CAN_GROUND_FLY);
		}
	}
	else if(pMsg.Id == g_EventSobSkillAction)
	{
		m_SkillContainer.HandleEvents(pMsg);
		CNtlSobFeelingActor::HandleEvents(pMsg); 
	}

	CNtlSobFeelingActor::HandleEvents(pMsg); 
}