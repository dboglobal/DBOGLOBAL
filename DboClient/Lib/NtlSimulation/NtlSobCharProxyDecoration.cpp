#include "precomp_ntlsimulation.h"
#include "NtlSobCharProxy.h"
#include "NtlSobCharProxyDecoration.h"

// shared
#include "ItemTable.h"
#include "MobTable.h"

// presentation
#include "NtlPLSceneManager.h"
#include "NtlPLObject.h"
#include "NtlPLCharacter.h"
#include "NtlPLItem.h"
#include "NtlPLDecal.h"
#include "NtlPLPlayerName.h"
#include "NtlPLHelpers.h"
#include "NtlPLRenderGroup.h"
#include "NTLPLCameraRenderTexture.h"
#include "NtlPLEvent.h"
#include "NtlPLWorldEntity.h"
#include "NtlWorldFieldManager.h"
#include "NtlInstanceEffect.h"
#include "NtlPLEmblemMaker.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLEvent.h"
#include "NtlSob.h"
#include "NtlSobAvatar.h"
#include "NtlSobItem.h"
#include "NtlSobNpcAttr.h"
#include "NtlSobMonsterAttr.h"
#include "NtlSobPetAttr.h"
#include "NtlSobPlayerAttr.h"
#include "NtlSobAvatarAttr.h"
#include "NtlSobItemAttr.h"
#include "NtlSobManager.h"
#include "NtlSLVisualDeclear.h"
#include "TableContainer.h"
#include "NtlInventory.h"
#include "ntlSobElapsedController.h"
#include "NtlSLLogic.h"
#include "DBODirectorIndicate.h"
#include "NtlSobCharProxyTransform.h"
#include "NtlFSMDef.h"
#include "NtlShareTargetMark.h"

#define DECAL_RATIO 0.2f

RwBool CNtlSobCharDecorationProxy::m_bShadowCreate = TRUE;
RwBool CNtlSobCharDecorationProxy::m_bShadowRender = TRUE;

CNtlSobCharDecorationProxy::CNtlSobCharDecorationProxy()
{
	/*m_bNameVisible		= TRUE;	*/
	m_bResLoaded		= FALSE;
	m_bResLoadComplete	= FALSE;
	m_pShadowDecal		= NULL;	
	m_pPlayerName		= NULL;	
	m_pTargetMark		= NULL;	
    m_pTargetMark02     = NULL;
	m_pAttackMark		= NULL;	
    m_pAttackMark02     = NULL;
	m_pQuestMark		= NULL;	    
    m_pDBODirectorIndicate = NULL;    
    m_nPrevSectorIdx    = -1;    
    m_pProxyTransform   = NULL;
    m_pTenkaichiMark    = NULL;
    m_pTargetMarkingMark = NULL;
    m_pShareTargetMark  = NULL;

	m_pEmblemFactor = NTL_NEW sEmblemFactor;

	m_pTitleEffect = NULL;
	m_pGuardEffect = NULL;
	m_pRpChargeEffect = NULL;
}

CNtlSobCharDecorationProxy::~CNtlSobCharDecorationProxy()
{
	NTL_DELETE(m_pEmblemFactor);

	DeletePLShadowDecal();
	DeletePLPlayerName();
	DeletePLAttackMark();
	DeletePLTargetMark();
	DeletePLQuestMark();        
    DeleteDirectorIndicate();
    DetachRPBonusEffect();
    DeleteTenkaichiMark();
    DeleteTargetMarkingMark();
	DeletePLPlayerTitle();
	DeleteGuardEffect();

    NTL_DELETE(m_pShareTargetMark);
    NTL_DELETE(m_pProxyTransform);
    
}

void CNtlSobCharDecorationProxy::SetData(CNtlSob *pSobObj, CNtlPLCharacter *pPLCharacter)
{
	m_pSobObj = pSobObj;
	m_pPLCharacter = pPLCharacter;

	CreatePLShadowDecal();
	CreatePLPlayerName();

    if(m_pDBODirectorIndicate)
    {
        m_pDBODirectorIndicate->SetCharacter(pPLCharacter);
    }

    if(!m_pProxyTransform)
    {
        if(pSobObj->GetClassID() == SLCLASS_AVATAR || pSobObj->GetClassID() == SLCLASS_PLAYER || pSobObj->GetClassID() == SLCLASS_MONSTER)
        {
            m_pProxyTransform = NTL_NEW CNtlSobCharProxyTransform((CNtlSobActor*)pSobObj, pPLCharacter);
        }        
    }
}

void CNtlSobCharDecorationProxy::Update(RwReal fElapsed)
{
    if(!m_pPLCharacter)
        return;

	// check if player invisible
    if(!m_pPLCharacter->IsVisible())
    {
        if(m_pShadowDecal)
            m_pShadowDecal->SetVisible(FALSE);

		if (m_pPlayerName && m_pPlayerName->IsVisible())
		{
			m_pPlayerName->SetVisible(FALSE);
		}

		if (m_pTitleEffect)
			m_pTitleEffect->SetVisible(FALSE);
        
        return;
    }
    else
    {
        if(m_pShadowDecal)
            m_pShadowDecal->SetVisible(TRUE);
        
		if (m_pPlayerName && m_pPlayerName->IsEnablePlayerNameVisible() && !m_pPlayerName->IsVisible())
		{
			m_pPlayerName->SetVisible(TRUE);
		}

		if (m_pTitleEffect)
			m_pTitleEffect->SetVisible(TRUE);
    }

	RwV3d vPos = m_pSobObj->GetPosition();

	if(m_pShadowDecal)
	{
        RwV3d vPosShadow = vPos;
        SWorldHeightStuff sHeightStuff;
        Logic_GetWorldHeight(&vPosShadow, sHeightStuff);        
        vPosShadow.y = sHeightStuff.fFinialHeight;
		m_pShadowDecal->SetPosition(&vPosShadow);
	
        RwInt32 nSectorIdx = GetSceneManager()->GetWorld()->GetWorldFieldMgr()->GetSectorIdx(vPos);
        if(nSectorIdx != m_nPrevSectorIdx && nSectorIdx != -1)
        {
            ChangePLShadowDecal();
            m_nPrevSectorIdx = nSectorIdx;
        }
	}

	UpdatePlayerNamePosition( vPos );

	if(m_pDBODirectorIndicate)
        m_pDBODirectorIndicate->Update(fElapsed);

    if(m_pProxyTransform)
        m_pProxyTransform->Update(fElapsed);
}

void CNtlSobCharDecorationProxy::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventSobFainting)
		SobFaintingEventHandler(pMsg);
    else if(pMsg.Id == g_EventSobStateTransition)
        SobStateTanslation(pMsg);
	else if(pMsg.Id == g_EventSobTargetSelect)
		SobTargetSelectEventHandler(pMsg);
	else if(pMsg.Id == g_EventSobAttackSelect)
		SobAttackSelectEventHandler(pMsg);
	else if(pMsg.Id == g_EventSobTargetSelectRelease)
		SobTargetSelectReleaseEventHandler(pMsg);
	else if(pMsg.Id == g_EventSobAttackMarkRelease)
		SobAttackMarkReleaseEventHandler(pMsg);
	else if(pMsg.Id == g_EventQuestMark)
		SobQuestMark(pMsg);
    else if(pMsg.Id == g_EventDirectionNfy || pMsg.Id == g_EventTMQCleintState || pMsg.Id == g_EventWorldChange)
        SobDirectionNfy(pMsg);    
    else if(pMsg.Id == g_EventShareTargetSelect)
        SobShareTargetSelectHandler(pMsg);
    else if(pMsg.Id == g_EventShareTargetRelease)
        SobShareTargetReleaseHandler(pMsg);
    else if(pMsg.Id == g_EventTenkaichiMarking)
        SobTenkaichiMarkingHandler(pMsg);
	else if (pMsg.Id == g_EventSobEventTitleEffect)
		SobTitleEffectHandler(pMsg);
    else if(pMsg.Id == g_EventSobTargetMarkSelect)
        CreateTargetMarkingMark();
    else if(pMsg.Id == g_EventSobTargetMarkRelease)
        DeleteTargetMarkingMark();
    else if(pMsg.Id == g_EventTransform ||
            pMsg.Id == g_EventMobTransform ||
            pMsg.Id == g_EventSobSkillAction ||
            pMsg.Id == g_EventAnimHit ||
            pMsg.Id == g_EventTransformCandy ||
            pMsg.Id == g_EventSobChangeAdult ||
		pMsg.Id == g_EventCinematicScale)
    {
        if(!m_pProxyTransform)
        {
            m_pProxyTransform = NTL_NEW CNtlSobCharProxyTransform((CNtlSobActor*)m_pSobObj, m_pPLCharacter);
        }
        
        m_pProxyTransform->HandleEvents(pMsg);        
    }
//	else if(pMsg.Id == g_EventSobWarMode)
}

void CNtlSobCharDecorationProxy::SobFaintingEventHandler(RWS::CMsg &pMsg)
{
	DeletePLTargetMark();
    DeletePLAttackMark();
    
    // 천하제일 무도회 우승 마크를 없앤다
    if(m_pTenkaichiMark)
    {
        m_pTenkaichiMark->SetVisible(FALSE);
    }
}


void CNtlSobCharDecorationProxy::SobStateTanslation( RWS::CMsg& pMsg ) 
{
    SNtlEventSobStateTransition* pData = (SNtlEventSobStateTransition*)pMsg.pData;
    if(pData->uiTransState == NTL_FSMSID_IDLE)
    {
        // If he dies and comes back alive, he will reappear the first ball of the world.
        if(m_pTenkaichiMark && !m_pTenkaichiMark->IsVisible())
        {
            m_pTenkaichiMark->SetVisible(TRUE);
        }
    }
}

void CNtlSobCharDecorationProxy::SobTargetSelectEventHandler(RWS::CMsg &pMsg)
{
	SNtlEventSobTargetSelect *pSobTargetSelect = reinterpret_cast<SNtlEventSobTargetSelect*>( pMsg.pData );
	if(m_pSobObj->GetSerialID() == pSobTargetSelect->hSerialId)
	{
		CreatePLTargetMark();
	}
}

void CNtlSobCharDecorationProxy::SobAttackSelectEventHandler(RWS::CMsg &pMsg)
{
	SNtlEventSobAttackSelect *pSobAttackSelect = reinterpret_cast<SNtlEventSobAttackSelect*>( pMsg.pData );
	if(m_pSobObj->GetSerialID() == pSobAttackSelect->hSerialId)
	{
		CreatePLAttackMark();
	}
}

void CNtlSobCharDecorationProxy::SobTargetSelectReleaseEventHandler(RWS::CMsg &pMsg)
{
	DeletePLTargetMark();
	DeletePLAttackMark();
}

void CNtlSobCharDecorationProxy::SobAttackMarkReleaseEventHandler(RWS::CMsg &pMsg)
{
	DeletePLAttackMark();

	if(m_pTargetMark == NULL)
		CreatePLTargetMark();
}

void CNtlSobCharDecorationProxy::SobQuestMark(RWS::CMsg &pMsg)
{
	SNtlEventQuestMark *pQuestMark = reinterpret_cast<SNtlEventQuestMark*>( pMsg.pData );
	if(pQuestMark->byMarkType == EQuestMarkNew)
	{
		CreatePLQuestMark(NTL_VID_QUEST_MARK_NEW);
	}
	else if(pQuestMark->byMarkType == EQuestMarkProgress)
	{
		CreatePLQuestMark(NTL_VID_QUEST_MARK_PROGRESS);
	}
	else if(pQuestMark->byMarkType == EQuestMarkReward)
	{
		CreatePLQuestMark(NTL_VID_QUEST_MARK_REWARD);
	}
	else if(pQuestMark->byMarkType == EQuestMarkVisit)
	{
		CreatePLQuestMark(NTL_VID_QUEST_MARK_VISIT);
	}
	else
	{
		DeletePLQuestMark();
	}
}


void CNtlSobCharDecorationProxy::CreatePLShadowDecal(void)
{
	if(!m_bShadowRender || m_pShadowDecal)
		return;

	RwReal fVariScale = 1.0f;
	CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>( m_pSobObj );
	if(pActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_MOVEABLE)
	{
		CNtlSobMoveableAttr *pMoveableAttr = reinterpret_cast<CNtlSobMoveableAttr*>( pActor->GetSobAttr() );
		if(pMoveableAttr)
		{
			fVariScale = pMoveableAttr->GetScale();
		}
	}

	RwReal fModelWidth = m_pPLCharacter->GetWidth();
	RwReal fModelDepth = m_pPLCharacter->GetDepth();

	RwReal fShadowWeightScale;

	if(fModelWidth > fModelDepth)
		fShadowWeightScale = fModelDepth / fModelWidth;
	else
		fShadowWeightScale = fModelWidth / fModelDepth;

	fShadowWeightScale = 1.0f + 2.0f*fShadowWeightScale;

	m_fDefShadowScale = fVariScale * fShadowWeightScale*(fModelWidth + fModelDepth)/2.0f;

    // 너무 큰경우에는 제한을 시켜준다.
    if(m_fDefShadowScale > 10.0f)
        m_fDefShadowScale = 10.0f;

	// shadow decal
	SPLDecalCreateParam param;
	param.fIntersectionRadius = 3.0f;
	param.fScale = m_fDefShadowScale;	
	param.fVisibleSquaredDist = 1600.0f;	
	param.pTexName = "shadow.dds";
	param.pTexPath = ".\\texture\\effect\\";
	param.fYOffset = 0.01f;						// 지형 Decal이 0.1f이다. 지형 Decal 위로 그림자를 표현하기 위해 조금 더 뛰운다. (by agebreak 2007.5.11)
	param.eDecalType = DECAL_TERRAIN;    

	if(m_bShadowCreate)
	{
		m_pShadowDecal = static_cast<CNtlPLDecal*>(GetSceneManager()->CreateEntity(PLENTITY_DECAL, "NULL", &param));
		NTL_ASSERT(m_pShadowDecal, "CNtlSobCharProxy::CreatePLShadowDecal");
		
		// 컬러 설정
		RwRGBA clrShadow;
		GetSceneManager()->GetWorldShadowColor(m_pPLCharacter->GetPosition(), &clrShadow);
		clrShadow.red = (RwUInt8)((RwReal)clrShadow.red * DECAL_RATIO);
		clrShadow.green = (RwUInt8)((RwReal)clrShadow.green * DECAL_RATIO);
		clrShadow.blue = (RwUInt8)((RwReal)clrShadow.blue * DECAL_RATIO);
		m_pShadowDecal->SetAlphaBlend(TRUE, rwBLENDSRCALPHA, rwBLENDINVSRCALPHA);    
		m_pShadowDecal->SetColor(clrShadow.red, clrShadow.green, clrShadow.blue);    
	}
}

void CNtlSobCharDecorationProxy::DeletePLShadowDecal(void)
{
	if(m_pShadowDecal)
	{
		m_pPLCharacter->Detach(m_pShadowDecal); 
		GetSceneManager()->DeleteEntity(m_pShadowDecal); 
		m_pShadowDecal = NULL;
	}
}

void CNtlSobCharDecorationProxy::CreatePLPlayerName(void)
{
	CNtlSobActor *pSobActor = reinterpret_cast<CNtlSobActor*> ( m_pSobObj );
	if( !(pSobActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_LIFE) )
		return;

	CNtlSobAttr *pSobAttr = m_pSobObj->GetSobAttr(); 
	CNtlSobLifeAttr *pSobLifeAttr = reinterpret_cast<CNtlSobLifeAttr*> ( pSobAttr );
	if(!pSobLifeAttr->IsNameCreate())
		return;

	const WCHAR *pName = pSobLifeAttr->GetName();
	COLORREF color = pSobLifeAttr->GetNameColor();
	
	NTL_ASSERT(m_pPlayerName == NULL, "CNtlSobCharDecorationProxy::CreatePLPlayerName : player name is not null !!!");

	if( pSobActor->GetClassID() == SLCLASS_AVATAR || pSobActor->GetClassID() == SLCLASS_PLAYER )
	{
		CNtlSobPlayer *pSobPlayer = reinterpret_cast<CNtlSobPlayer*>( pSobActor );
		CNtlSobPlayerAttr *pSobPlayerAttr = reinterpret_cast<CNtlSobPlayerAttr*>(pSobPlayer->GetSobAttr());

		m_pPlayerName = Helper_CreatePlayerName(NULL, color, pName, NTL_GUILD_NAME_COLOR, pSobPlayer->GetGuildName(), m_pEmblemFactor, NTL_TITLE_NAME_COLOR, 
			pSobPlayerAttr->GetInvisibleTitleNameFlag() == true ? NULL : pSobPlayerAttr->GetTitleName());
	}
	else
	{
		m_pPlayerName = Helper_CreatePlayerName(NULL, color, pName);
	}

	m_fNameHeightOffset = m_pPLCharacter->GetHeight() + NTL_NAMEBOX_OFFSET;
	m_pPlayerName->SetSerialID(m_pSobObj->GetSerialID());
	m_pPlayerName->SetVisible(FALSE);
}

void CNtlSobCharDecorationProxy::DeletePLPlayerName(void)
{
	if(m_pPlayerName)
	{
		GetSceneManager()->DeleteEntity(m_pPlayerName); 
		m_pPlayerName = NULL;
	}
}

void CNtlSobCharDecorationProxy::CreatePLPlayerTitle(const char *pEffectKey, const char *pBoneKey)
{
	if (m_pTitleEffect)
	{
		DeletePLPlayerTitle();
	}

	CNtlSobActor *pSobActor = reinterpret_cast<CNtlSobActor*> (m_pSobObj);
	CNtlSobAttr *pSobAttr = m_pSobObj->GetSobAttr();

	if (pSobActor->GetClassID() == SLCLASS_AVATAR || pSobActor->GetClassID() == SLCLASS_PLAYER)
	{
		if (m_pPLCharacter->GetBoneByName(pBoneKey))
		{
			m_pTitleEffect = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, pEffectKey);
			if (m_pTitleEffect)
			{
				Helper_AttachBone(m_pPLCharacter, m_pTitleEffect, pBoneKey);

				m_pTitleEffect->SetSerialID(m_pSobObj->GetSerialID());
				m_pTitleEffect->SetVisible(TRUE);
			}
		}
	}
	else
	{
		return;
	}
}

void CNtlSobCharDecorationProxy::DeletePLPlayerTitle()
{
	if (m_pTitleEffect)
	{
		Helper_DetachPLEntity(m_pPLCharacter, m_pTitleEffect);
		GetSceneManager()->DeleteEntity(m_pTitleEffect);
		m_pTitleEffect = NULL;
	}
}

void CNtlSobCharDecorationProxy::CreatePLAttackMark(void)
{
	DeletePLTargetMark();
    
    if(m_pShareTargetMark && m_pShareTargetMark->IsShareTargeting())
    {
        // 공유 타겟 설정이 되어 있으면 일반 마크는 뜨지 않는다.
        CreateShareTargetMark(m_pShareTargetMark->GetSlot(), CNtlShareTargetMark::SHARE_TARGET_ATTACK);
        return;
    }

	if(m_pAttackMark == NULL)
	{
        m_pAttackMark = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_MARK_ATTACK);
        RwV3d vOffset;
        CNtlMath::MathRwV3dAssign(&vOffset, 0.0f, m_pPLCharacter->GetHeight() + NTL_ATTACK_MARK_OFFSET, 0.0f); 
        Helper_AttachWorldPos(m_pPLCharacter, m_pAttackMark, vOffset);

		m_pAttackMark02 = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_MARK_ATTACK_02);		
        RwReal fScale = max(m_pPLCharacter->GetWidth(), m_pPLCharacter->GetHeight());
        if(fScale >= 2.0f) m_pAttackMark02->SetScale(fScale * 0.5f);

		Helper_AttachWorldPos(m_pPLCharacter, m_pAttackMark02);
	}
}

void CNtlSobCharDecorationProxy::DeletePLAttackMark(void)
{
    if(m_pAttackMark)
    {
        if(m_pShareTargetMark && m_pShareTargetMark->IsShareTargeting())
        {
            m_pShareTargetMark->CreateShareTargetMark(m_pShareTargetMark->GetSlot(), CNtlShareTargetMark::SHARE_TARGET_NORMAL);
        }

        Helper_DetachPLEntity(m_pPLCharacter, m_pAttackMark);
        GetSceneManager()->DeleteEntity(m_pAttackMark);
        m_pAttackMark = NULL;

        Helper_DetachPLEntity(m_pPLCharacter, m_pAttackMark02);
        GetSceneManager()->DeleteEntity(m_pAttackMark02);
        m_pAttackMark02 = NULL;
    }
}

void CNtlSobCharDecorationProxy::CreatePLTargetMark(void)
{
	DeletePLAttackMark();

    if(m_pShareTargetMark && m_pShareTargetMark->IsShareTargeting())
    {
        // 공유 타겟 설정이 되어 있으면 일반 마크는 뜨지 않는다.
        CreateShareTargetMark(m_pShareTargetMark->GetSlot(), CNtlShareTargetMark::SHARE_TARGET_TARGET);
        return;
    }

	if(m_pTargetMark == NULL)
	{
        m_pTargetMark = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_MARK_TARGET);
        RwV3d vOffset;
        CNtlMath::MathRwV3dAssign(&vOffset, 0.0f, m_pPLCharacter->GetHeight() + NTL_TARGET_MARK_OFFSET, 0.0f); 
        Helper_AttachWorldPos(m_pPLCharacter, m_pTargetMark, vOffset);

        if(!IsNotCreateDecalMark())
        {
		    m_pTargetMark02 = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_MARK_TARGET_02);		
            RwReal fScale = max(m_pPLCharacter->GetWidth(), m_pPLCharacter->GetHeight());
            if(fScale >= 2.0f) m_pTargetMark02->SetScale(fScale * 0.5f);
            Helper_AttachWorldPosHeight(m_pPLCharacter, m_pTargetMark02);
        }
	}
}

void CNtlSobCharDecorationProxy::DeletePLTargetMark(void)
{
    if(m_pShareTargetMark && m_pShareTargetMark->IsShareTargeting())
    {
        m_pShareTargetMark->CreateShareTargetMark(m_pShareTargetMark->GetSlot(), CNtlShareTargetMark::SHARE_TARGET_NORMAL);
    }
    else if(m_pTargetMark)
    {
        if(m_pTargetMark)
        {
            Helper_DetachPLEntity(m_pPLCharacter, m_pTargetMark);
            GetSceneManager()->DeleteEntity(m_pTargetMark);
            m_pTargetMark = NULL;
        }
        if(m_pTargetMark02)
        {
            Helper_DetachPLEntity(m_pPLCharacter, m_pTargetMark02);
            GetSceneManager()->DeleteEntity(m_pTargetMark02);
            m_pTargetMark02 = NULL;
        }	
    }
}

void CNtlSobCharDecorationProxy::CreatePLQuestMark(const RwChar *pKey)
{
	DeletePLQuestMark();

	RwReal fHeight = m_pPLCharacter->GetHeight();

	if(m_pQuestMark == NULL)
	{
		m_pQuestMark = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, pKey);
		RwV3d vOffset;
		CNtlMath::MathRwV3dAssign(&vOffset, 0.0f, fHeight+NTL_QUEST_MARK_OFFSET, 0.0f); 
		Helper_AttachWorldPos(m_pPLCharacter, m_pQuestMark, vOffset);
	}
}

void CNtlSobCharDecorationProxy::DeletePLQuestMark(void)
{
	if(m_pQuestMark)
	{
		Helper_DetachPLEntity(m_pPLCharacter, m_pQuestMark);
		GetSceneManager()->DeleteEntity(m_pQuestMark);
		m_pQuestMark = NULL;
	}
}

void CNtlSobCharDecorationProxy::AddWorld(void)
{
    if(m_pProxyTransform)
        m_pProxyTransform->AddWorld();
}

void CNtlSobCharDecorationProxy::RemoveWorld(void)
{
    if(m_pProxyTransform)
        m_pProxyTransform->RemoveWorld();
}

void CNtlSobCharDecorationProxy::SetAlpha(RwUInt8 byAlpha)
{
	if(m_pShadowDecal)
		m_pShadowDecal->SetAlpha(byAlpha);

	if (m_pPlayerName)
	{
		m_pPlayerName->SetAlpha(byAlpha);
	}

	if(m_pTargetMark)
    {
		m_pTargetMark->SetAlpha(byAlpha);        
    }
    if(m_pTargetMark02)
    {
        m_pTargetMark02->SetAlpha(byAlpha);
    }

	if(m_pAttackMark)
    {
		m_pAttackMark->SetAlpha(byAlpha);
        m_pAttackMark02->SetAlpha(byAlpha);
    }
}

void CNtlSobCharDecorationProxy::SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue)
{
	if(m_pShadowDecal)
		m_pShadowDecal->SetColor(byRed, byGreen, byBlue);

	if(m_pPlayerName)
		m_pPlayerName->SetColor(byRed, byGreen, byBlue);

	if(m_pTargetMark)
    {
		m_pTargetMark->SetColor(byRed, byGreen, byBlue);        
    }
    if(m_pTargetMark02)
    {
        m_pTargetMark02->SetColor(byRed, byGreen, byBlue);
    }

	if(m_pAttackMark)
    {
		m_pAttackMark->SetColor(byRed, byGreen, byBlue);
        m_pAttackMark02->SetColor(byRed, byGreen, byBlue);
    }
}

void CNtlSobCharDecorationProxy::SetVisible(RwBool bEnable)
{
	if(m_pShadowDecal)
		m_pShadowDecal->SetVisible(bEnable);

	if (m_pPlayerName && m_pPlayerName->IsEnablePlayerNameVisible())
	{
		m_pPlayerName->SetVisible(bEnable);
	}

	if(m_pTargetMark)
    {
		m_pTargetMark->SetVisible(bEnable);
    }
    if(m_pTargetMark02)
    {
        m_pTargetMark02->SetVisible(bEnable);
    }

	if(m_pAttackMark)
    {
		m_pAttackMark->SetVisible(bEnable);
        m_pAttackMark02->SetVisible(bEnable);
    }
}

void CNtlSobCharDecorationProxy::SetScale(RwReal fScale)
{
	// player name
	if(m_pPlayerName)
	{
		Helper_DetachPLEntity(m_pPLCharacter, m_pPlayerName);

		RwV3d vOffset;
		CNtlMath::MathRwV3dAssign(&vOffset, 0.0f, m_pPLCharacter->GetHeight()+NTL_NAMEBOX_OFFSET, 0.0f); 
		Helper_AttachWorldPos(m_pPLCharacter, m_pPlayerName, vOffset);
	}

	// character target mark
	if(m_pTargetMark)
	{
        DeletePLTargetMark();
        CreatePLTargetMark();		
	}

	// character attack mark
	if(m_pAttackMark)
	{
        DeletePLAttackMark();
        CreatePLAttackMark();		
	}

    // Shadow
    if(m_pShadowDecal)
    {
        m_pShadowDecal->SetScale(m_fDefShadowScale * fScale);
    }
}

void CNtlSobCharDecorationProxy::SetShadowOnOff(RwBool bOnOff)
{
	if(bOnOff)
	{
		if(m_bShadowCreate && m_pShadowDecal == NULL)
			CreatePLShadowDecal();
	}
	else
	{
		if(m_pShadowDecal)
			DeletePLShadowDecal();
	}
}

void CNtlSobCharDecorationProxy::EnablePlayerNameVisible(RwBool bVisible)
{
	/*if(m_pPlayerName)
		m_pPlayerName->SetVisible(bVisible);

	m_bNameVisible = bVisible;*/
	if( m_pPlayerName )
		m_pPlayerName->EnablePlayerNameVisible( bVisible );
}

void CNtlSobCharDecorationProxy::CreateWeightElapsedController(RwReal fLifeTime, RwReal fWeightValue)
{
	if(m_pShadowDecal)
		m_pShadowDecal->CreateWeightElapsedController(fLifeTime, fWeightValue);

	if(m_pPlayerName)
		m_pPlayerName->CreateWeightElapsedController(fLifeTime, fWeightValue);

	if(m_pTargetMark)
    {
		m_pTargetMark->CreateWeightElapsedController(fLifeTime, fWeightValue);        
    }
    if(m_pTargetMark02)
    {
        m_pTargetMark02->CreateWeightElapsedController(fLifeTime, fWeightValue);
    }

	if(m_pAttackMark)
    {
		m_pAttackMark->CreateWeightElapsedController(fLifeTime, fWeightValue);
        m_pAttackMark02->CreateWeightElapsedController(fLifeTime, fWeightValue);
    }
}

void CNtlSobCharDecorationProxy::DeleteWeightElapsedController(void)
{
	if(m_pShadowDecal)
		m_pShadowDecal->DeleteWeightElapsedController();

	if(m_pPlayerName)
		m_pPlayerName->DeleteWeightElapsedController();

	if(m_pTargetMark)
    {
		m_pTargetMark->DeleteWeightElapsedController();        
    }
    if(m_pTargetMark02)
    {
        m_pTargetMark02->DeleteWeightElapsedController();
    }

	if(m_pAttackMark)
    {
		m_pAttackMark->DeleteWeightElapsedController();
        m_pAttackMark02->DeleteWeightElapsedController();
    }
}


void CNtlSobCharDecorationProxy::DetachConvertClassEquipItem(void)
{
	// shadow decal
	if(m_pShadowDecal)
		Helper_DetachPLEntity(m_pPLCharacter, m_pShadowDecal);

	// player name
	if(m_pPlayerName)
		Helper_DetachPLEntity(m_pPLCharacter, m_pPlayerName);

	// character target mark
	if(m_pTargetMark)
    {
		Helper_DetachPLEntity(m_pPLCharacter, m_pTargetMark);
    }
    if(m_pTargetMark02)
    {
        Helper_DetachPLEntity(m_pPLCharacter, m_pTargetMark02);
    }

	// character attack mark
	if(m_pAttackMark)
    {
		Helper_DetachPLEntity(m_pPLCharacter, m_pAttackMark);
        Helper_DetachPLEntity(m_pPLCharacter, m_pAttackMark02);
    }

	// character quest mark
	if(m_pQuestMark)
		Helper_DetachPLEntity(m_pPLCharacter, m_pQuestMark);
}

void CNtlSobCharDecorationProxy::AttachConvertClassEquipItem(CNtlPLCharacter *pPLCharacter)
{
	m_pPLCharacter = pPLCharacter;

	// shadow decal
	if(m_pShadowDecal)
	{
        Helper_AttachWorldPosHeight(m_pPLCharacter, m_pShadowDecal);
	}

	// player name
	if(m_pPlayerName)
	{
		RwV3d vOffset;
		CNtlMath::MathRwV3dAssign(&vOffset, 0.0f, m_pPLCharacter->GetHeight()+NTL_NAMEBOX_OFFSET, 0.0f); 
		Helper_AttachWorldPos(m_pPLCharacter, m_pPlayerName, vOffset);
	}

	// character target mark
	if(m_pTargetMark)
	{
        DeletePLTargetMark();
        CreatePLTargetMark();
	}

	// character attack mark
	if(m_pAttackMark)
	{
        DeletePLAttackMark();
        CreatePLAttackMark();		
	}

	// character quest mark
	if(m_pQuestMark)
	{
		RwV3d vOffset;
		RwReal fHeight = m_pPLCharacter->GetHeight();
		CNtlMath::MathRwV3dAssign(&vOffset, 0.0f, fHeight+NTL_QUEST_MARK_OFFSET, 0.0f); 
		Helper_AttachWorldPos(m_pPLCharacter, m_pQuestMark, vOffset);
    }

    // Direction Indicate
    if(m_pDBODirectorIndicate)
    {
        m_pDBODirectorIndicate->SetCharacter(m_pPLCharacter);
    }

    // Transform
    if(m_pProxyTransform)
    {
        m_pProxyTransform->SetActor((CNtlSobActor*)m_pSobObj, m_pPLCharacter);
    }
}

void CNtlSobCharDecorationProxy::SetNameColor(const WCHAR* pwcName, COLORREF nameColor,
											  const WCHAR* pwcGuildName, COLORREF guildColor, 
											  const WCHAR* pwcTitleName, COLORREF titlecolor)
{
	if(m_pPlayerName)
		m_pPlayerName->SetNameColor(pwcName, nameColor, pwcGuildName, guildColor, m_pEmblemFactor, pwcTitleName, titlecolor);
}

void CNtlSobCharDecorationProxy::SetNickNameColor(const WCHAR* pwcNickName, COLORREF nickNameColor)
{
	if(m_pPlayerName)
		m_pPlayerName->SetNickNameColor(pwcNickName, nickNameColor);
}

CNtlPLPlayerName* CNtlSobCharDecorationProxy::GetPLPlayerName()
{
	return m_pPlayerName;
}

void CNtlSobCharDecorationProxy::UpdatePlayerNamePosition( RwV3d& vPos, float fNewHeight/* = 0.0f*/)
{
	if ( m_pPlayerName )
	{
		RwV3dAssignMacro( &m_vNamePos, &vPos );

		if(fNewHeight == 0.0f)
			m_vNamePos.y += m_fNameHeightOffset;
		else
			m_vNamePos.y += fNewHeight + NTL_NAMEBOX_OFFSET;

		m_pPlayerName->SetPosition( &m_vNamePos );
	}
}

const sEmblemFactor* CNtlSobCharDecorationProxy::GetEmblemFactor()
{
	return m_pEmblemFactor;
}

void CNtlSobCharDecorationProxy::SetEmblemFactor(RwUInt8 byTypeA, RwUInt8 byTypeAColor,
												 RwUInt8 byTypeB, RwUInt8 byTypeBColor, 
												 RwUInt8 byTypeC, RwUInt8 byTypeCColor)
{
	m_pEmblemFactor->byTypeA		= byTypeA;
	m_pEmblemFactor->byTypeB		= byTypeB;
	m_pEmblemFactor->byTypeC		= byTypeC;
	m_pEmblemFactor->byTypeAColor	= byTypeAColor;
	m_pEmblemFactor->byTypeBColor	= byTypeBColor;
	m_pEmblemFactor->byTypeCColor	= byTypeCColor;
}

void CNtlSobCharDecorationProxy::CreateDirectorIndicate() 
{
    if(!m_pDBODirectorIndicate)
    {
        m_pDBODirectorIndicate = NTL_NEW CDBODirectorIndicate(m_pPLCharacter);
        m_pDBODirectorIndicate->Create();
    }    
}
void CNtlSobCharDecorationProxy::DeleteDirectorIndicate() 
{
    NTL_DELETE(m_pDBODirectorIndicate);
}

void CNtlSobCharDecorationProxy::SobDirectionNfy( RWS::CMsg &pMsg ) 
{
    CreateDirectorIndicate();
    m_pDBODirectorIndicate->HandleEvents(pMsg);
}

void CNtlSobCharDecorationProxy::ResourceLoadComplete(RwBool bVisible)
{
	if( NULL != m_pPlayerName )
	{
		if( m_pPlayerName->IsEnablePlayerNameVisible() )
			m_pPlayerName->SetVisible(bVisible);

		// m_bNameVisible 이 FALSE 인데 현재 m_pPlayerName이 SetVisible(TRUE)로 세팅됐다면 안 보이게 해준다.
		if( !m_pPlayerName->IsEnablePlayerNameVisible() && m_pPlayerName->IsVisible() )
			m_pPlayerName->SetVisible(FALSE);
	}

	m_bResLoaded		= TRUE;
	m_bResLoadComplete	= TRUE;
}

void CNtlSobCharDecorationProxy::ChangePLShadowDecal()
{
    if(!m_pPLCharacter || !m_pShadowDecal)
        return;

    RwRGBA color;    
    GetSceneManager()->GetWorldShadowColor(m_pPLCharacter->GetPosition(), &color);
    color.red   = (RwUInt8)((RwReal)color.red * DECAL_RATIO);
    color.green = (RwUInt8)((RwReal)color.green * DECAL_RATIO);
    color.blue  = (RwUInt8)((RwReal)color.blue * DECAL_RATIO);
    m_pShadowDecal->SetColor(color.red, color.green, color.blue);
}

RwBool CNtlSobCharDecorationProxy::AttachRPBonusEffect() 
{
    if(!m_vecRPBonusEffect.empty())
        return FALSE;

    // RP Bonus가 붙을 본 리스트
    std::string strBoneList1[7];
    std::string strBoneList2;
    strBoneList1[0] = "Bip01 Head";
    strBoneList1[1] = "Bip01 L Calf";
    strBoneList1[2] = "Bip01 R Calf";
    strBoneList1[3] = "Bip01 L Forearm";
    strBoneList1[4] = "Bip01 R Forearm";
    strBoneList1[5] = "Bip01 L Foot";
    strBoneList1[6] = "Bip01 R Foot";
    strBoneList2 = "Bip01 Pelvis";    

    CNtlInstanceEffect* pEffect;
    for(int i = 0; i < 7; ++i)      // 작은 이펙트
    {
        if(m_pPLCharacter->GetBoneByName(strBoneList1[i].c_str()))
        {
            pEffect = (CNtlInstanceEffect*)GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_RPBOUNS_SMALL);            
            if(pEffect)
            {
                m_pPLCharacter->AttachBone(pEffect, strBoneList1[i].c_str());
                m_vecRPBonusEffect.push_back(pEffect);
            }
        }        
    }

    if(m_pPLCharacter->GetBoneByName(strBoneList2.c_str())) // 큰 이펙트
    {
        pEffect = (CNtlInstanceEffect*)GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_RPBONUS_BIG);
        if(pEffect)
        {
            m_pPLCharacter->AttachBone(pEffect, strBoneList2.c_str());
            m_vecRPBonusEffect.push_back(pEffect);
        }
    }

    return TRUE;
}

RwBool CNtlSobCharDecorationProxy::DetachRPBonusEffect() 
{
    if(m_vecRPBonusEffect.empty())
        return FALSE;

    for each(CNtlInstanceEffect* pEffect in m_vecRPBonusEffect)
    {
        pEffect->Finish();
    }

    m_vecRPBonusEffect.clear();

    return TRUE;
}

void CNtlSobCharDecorationProxy::CreateGuardEffect(RwChar* pKey)
{
	DeleteGuardEffect();

	m_pGuardEffect = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, pKey);
	if (m_pGuardEffect)
	{
		RwV3d vOffset = ZeroAxis;
		m_pGuardEffect->SetPosition(&m_pSobObj->GetSobProxy()->GetPosition());
		m_pGuardEffect->SetDirection(&m_pPLCharacter->GetDirection());
		m_pGuardEffect->SetScale(m_pSobObj->GetSobProxy()->GetScale());
		Helper_AttachWorldPos(m_pPLCharacter, m_pGuardEffect, vOffset);
	}
}

void CNtlSobCharDecorationProxy::DeleteGuardEffect()
{
	if (m_pGuardEffect)
	{
		GetSceneManager()->DeleteEntity(m_pGuardEffect);
		m_pGuardEffect = NULL;
	}
}

void CNtlSobCharDecorationProxy::CreateRpChargeEffect(RwChar *pKey)
{
	DeleteRpChargeEffect();

	m_pRpChargeEffect = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, pKey);

	if (m_pRpChargeEffect)
	{
		RwV3d vOffset = ZeroAxis;
		m_pRpChargeEffect->SetPosition(&m_pSobObj->GetSobProxy()->GetPosition());
		m_pRpChargeEffect->SetScale(m_pPLCharacter->GetScale());
		m_pPLCharacter->AttachBone((CNtlPLAttach*)m_pRpChargeEffect, "nullroot");
	}

}

void CNtlSobCharDecorationProxy::DeleteRpChargeEffect()
{
	if (m_pRpChargeEffect)
	{
		GetSceneManager()->DeleteEntity(m_pRpChargeEffect);
		m_pRpChargeEffect = NULL;
	}
}

void CNtlSobCharDecorationProxy::CreateShareTargetMark(RwUInt8 bySlot, RwUInt32 type)
{
    if(!m_pShareTargetMark)
    {
        m_pShareTargetMark = NTL_NEW CNtlShareTargetMark(m_pSobObj);
    }

    m_pShareTargetMark->CreateShareTargetMark(bySlot, type);
}

void CNtlSobCharDecorationProxy::DeleteShareTargetMark()
{
    if(!m_pShareTargetMark)
        return;

    m_pShareTargetMark->DeleteShareTargetMark();
}


void CNtlSobCharDecorationProxy::CreateTenkaichiMark( BYTE byType ) 
{
    DeleteTenkaichiMark();

	//by daneos
    /*switch(byType)
    {
    case MARKING_TYPE_BUDOKAI_WINNER01:
        m_pTenkaichiMark = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_TENKAICHI_WINNNER_01);
        break;
    case MARKING_TYPE_BUDOKAI_WINNER02:
        m_pTenkaichiMark = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_TENKAICHI_WINNNER_02);
        break;
    case MARKING_TYPE_BUDOKAI_WINNER03:
        m_pTenkaichiMark = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_TENKAICHI_WINNNER_03);
        break;
    }*/

    if(m_pTenkaichiMark)
    {
     //   Helper_AttachWorldPos(m_pPLCharacter, m_pTenkaichiMark, ZeroAxis);
    }    
}

void CNtlSobCharDecorationProxy::DeleteTenkaichiMark() 
{
    if(m_pTenkaichiMark)
    {
        GetSceneManager()->DeleteEntity(m_pTenkaichiMark);
        m_pTenkaichiMark = NULL;
    }
}

void CNtlSobCharDecorationProxy::CreateTargetMarkingMark() 
{
    DeleteTargetMarkingMark();
    
    m_pTargetMarkingMark = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_MARK_TARGET_MARKING);
    if(m_pTargetMarkingMark)
    {
        RwV3d vOffset = ZeroAxis;
        vOffset.y = m_pSobObj->GetSobProxy()->GetPLEntityHeight() * 0.7f;
        Helper_AttachWorldPos(m_pPLCharacter, m_pTargetMarkingMark, vOffset);
    }
}

void CNtlSobCharDecorationProxy::DeleteTargetMarkingMark() 
{
    if(m_pTargetMarkingMark)
    {
        GetSceneManager()->DeleteEntity(m_pTargetMarkingMark);
        m_pTargetMarkingMark = NULL;
    }
}

void CNtlSobCharDecorationProxy::SobShareTargetSelectHandler( RWS::CMsg& pMsg ) 
{
    SNtlEventShareTargetSelect* pData = (SNtlEventShareTargetSelect*)pMsg.pData;
    if(pData->hSerialId == m_pSobObj->GetSerialID())
    {
        // 기존 타겟 마크를 삭제하기 전에 현재 상태를 저장해둔다
        RwBool bAttackMode = m_pAttackMark ? TRUE : FALSE;
        RwBool bTargetMode = m_pTargetMark ? TRUE : FALSE;

        DeletePLAttackMark();
        DeletePLTargetMark();

        if(bAttackMode)
        {
            CreateShareTargetMark(pData->bySlot, CNtlShareTargetMark::SHARE_TARGET_ATTACK);
        }
        else if(bTargetMode)
        {
            CreateShareTargetMark(pData->bySlot, CNtlShareTargetMark::SHARE_TARGET_TARGET);
        }
        else
        {
            CreateShareTargetMark(pData->bySlot, CNtlShareTargetMark::SHARE_TARGET_NORMAL);
        }        
    }
}


void CNtlSobCharDecorationProxy::SobShareTargetReleaseHandler( RWS::CMsg& pMsg ) 
{
    SNtlEventShareTargetRelease* pData = (SNtlEventShareTargetRelease*)pMsg.pData;
    if(pData->hSerialId == m_pSobObj->GetSerialID())
    {
        DeleteShareTargetMark();
    }
}

void CNtlSobCharDecorationProxy::SobTenkaichiMarkingHandler( RWS::CMsg& pMsg ) 
{
    SNtlEventTenkaichiMarking* pData = (SNtlEventTenkaichiMarking*)pMsg.pData;
    if(pData->hSerialId != m_pSobObj->GetSerialID())
        return;

   // CreateTenkaichiMark(pData->byType);        
}

void CNtlSobCharDecorationProxy::SobTitleEffectHandler(RWS::CMsg & pMsg)
{
	SNtlEventTitleEffect* pData = (SNtlEventTitleEffect*)pMsg.pData;
	if (pData->hSerialId != m_pSobObj->GetSerialID())
		return;

	if (pData->bCreate == false)
	{
		DeletePLPlayerTitle();
	}
	else
	{
		CreatePLPlayerTitle(pData->pchEffect, pData->pchBone);
	}
}

RwBool CNtlSobCharDecorationProxy::IsNotCreateDecalMark() 
{
    // 버스는 바닥 데칼을 그리지 않는다.
    return Logic_IsBus((CNtlSobActor*)m_pSobObj);
}


void CNtlSobCharDecorationProxy::SobChangeAdult( CNtlPLCharacter* pPLCharacter ) 
{
    if(m_pProxyTransform)
    {
        m_pProxyTransform->SetActor((CNtlSobActor*)m_pSobObj, pPLCharacter);
        m_pProxyTransform->PostChangeAdult();
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


CNtlSobCharPrivateShopProxy::CNtlSobCharPrivateShopProxy()
{
	m_pPLShop = NULL;
	m_fHeight = 1.0f;
}

CNtlSobCharPrivateShopProxy::~CNtlSobCharPrivateShopProxy()
{
	if(m_pPLShop)
	{
		GetSceneManager()->DeleteEntity(m_pPLShop);
		m_pPLShop = NULL;
	}
}

void CNtlSobCharPrivateShopProxy::SetData(CNtlSob *pSobObj, CNtlPLCharacter *pPLCharacter)
{
	RwBool bLoadScheduling = GetNtlResourceManager()->IsLoadScheduling();
	GetNtlResourceManager()->SetLoadScheduling(FALSE);

	RwV3d vPos = pSobObj->GetPosition();
	RwV3d vDir = pSobObj->GetDirection();

	SPLObjectCreateParam sObjectParam;
	sObjectParam.bLoadMap = TRUE;
	sObjectParam.pPos = &vPos;

	RwReal fAngleY = CNtlMath::LineToAngleY(&vDir);

	m_pPLShop = GetSceneManager()->CreateEntity(PLENTITY_OBJECT, NTL_VID_PRIVATESHOP, &sObjectParam);
	RwUInt32 uiFlags = m_pPLShop->GetFlags();
	m_pPLShop->SetFlags(uiFlags & ~NTL_PLEFLAG_COLLISION);
	
	m_pPLShop->SetSerialID(pSobObj->GetSerialID());
	m_pPLShop->SetRotate(0.0f, fAngleY, 0.0f);

	// width, height, depth
	const RwBBox *pBox = m_pPLShop->GetBoundingBox();
	if(pBox)
	{
		m_fWidth	= pBox->sup.x - pBox->inf.x;
		m_fHeight	= pBox->sup.y - pBox->inf.y;
		m_fDepth	= pBox->sup.z - pBox->inf.z;
	}

	GetNtlResourceManager()->SetLoadScheduling(bLoadScheduling);
}

void CNtlSobCharPrivateShopProxy::Update(RwReal fElapsed)
{
}

RwReal CNtlSobCharPrivateShopProxy::GetHeight(void)
{
	return m_fHeight;
}

