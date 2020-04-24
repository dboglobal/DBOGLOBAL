#include "precomp_ntlsimulation.h"
#include "ceventhandler.h"
#include "NtlSobCharProxyTransform.h"

// core

// Share
#include "ItemTable.h"
#include "TableContainer.h"
#include "MobTable.h"
#include "TextAllTable.h"

// PL
#include "NtlPLCharacter.h"
#include "NtlPLItem.h"
#include "NtlPLSceneManager.h"
#include "NtlPlEvent.h"
#include "NtlInstanceEffect.h"
#include "NtlPLMaterialExtWrapper.h"

// SL
#include "NtlSobPlayer.h"
#include "NtlSobManager.h"
#include "NtlSobCharProxy.h"
#include "NtlSLEvent.h"
#include "NtlSlEventFunc.h"
#include "NtlSLLogic.h"
#include "NtlSLVisualDeclear.h"
#include "NtlSobProxySystemEffect.h"
#include "NtlSLApi.h"
#include "NtlSobCharProxyEquip.h"
#include "NtlSobAttr.h"
#include "NtlSobCharProxyDecoration.h"
#include "NtlFSMDef.h"
#include "NtlSobMonsterAttr.h"

#define TRANSFORM_NAMEK_ATTACK_RANGE    3.0f                    ///< 거대 나멕 변신후의 공격 범위
#define TRANSFORM_PURE_MAJIN_FACE1      "A_P_majin_FACE01"
#define TRANSFORM_PURE_MAJIN_FACE2      "A_P_majin_FACE01_1"
#define TRANSFORM_PURE_MAJIN_HEAD       "headShape"

#define CANDY_HEIGHT_BIG                3.5f
#define CANDY_HEIGHT_MIDDLE             1.5f
#define CANDY_SCALE_BIG                 3.0f
#define CANDY_SCALE_MIDDLE              2.0f



CNtlSobCharProxyTransform::CNtlSobCharProxyTransform(CNtlSobActor* pActor, CNtlPLCharacter* pPLCharacter)
{
    SetActor(pActor, pPLCharacter);

    m_bIsTransform      = FALSE; 
    m_bReadyTransform   = FALSE;
    m_pItemTransform    = NULL;
    m_pItemTransformUI  = NULL;
    m_pFaceTransform    = NULL;
    m_pSystemEffectColor[0] = NULL;
    m_pSystemEffectColor[1] = NULL;
    m_pKaiOuKenColor[0] = NULL;
    m_pKaiOuKenColor[1] = NULL;
    m_bScheduleLoadingCheck = FALSE;
    m_pItemOrgWeapon = NULL;
    m_pItemOrgSubWeapon = NULL;
    m_pMajinOrgModel = NULL;   
    m_pMajinOrgModelUI = NULL;
    m_pMajinPureModel = NULL;
    m_pMajinPureModelUI = NULL;    
    m_pCandyOrgModel = m_pCandyOrgModelUI = m_pCandyModel = m_pCandyModelUI = NULL;
    m_fOrgAttackRange = 3.0f;
    m_uiTransformId = ASPECTSTATE_INVALID;    
    m_pMajinPureModelUI_Dogi = NULL;
    m_pCandyModelUI_Dogi = NULL;
    m_pPLCharacterUI_Dogi = NULL;
}

CNtlSobCharProxyTransform::~CNtlSobCharProxyTransform(void)
{
    Destory();
}

void CNtlSobCharProxyTransform::SetActor( CNtlSobActor* pActor, CNtlPLCharacter* pPLCharacter ) 
{
    m_pActor = pActor;
    m_pPLCharacter = pPLCharacter;

    CNtlSobCharProxy* pProxy = (CNtlSobCharProxy*)m_pActor->GetSobProxy();
    if(m_pActor->GetClassID() == SLCLASS_AVATAR && pProxy->GetUICharacter())
    {
        m_pPLCharacterUI = pProxy->GetUICharacter()->pPLCharacter;
		m_pPLCharacterUI_Dogi = pProxy->GetUIDogiCharacter()->pPLCharacter;
    }
    else
    {
        m_pPLCharacterUI = NULL;
		m_pPLCharacterUI_Dogi = NULL;
    }
}

void CNtlSobCharProxyTransform::Destory() 
{
    NTL_DELETE(m_pItemOrgWeapon);
    NTL_DELETE(m_pItemOrgSubWeapon);

    if(m_pItemTransform)
    {
        GetSceneManager()->DeleteEntity(m_pItemTransform);
        m_pItemTransform = NULL;
    }

    if(m_pItemTransformUI)
    {
        GetSceneManager()->DeleteEntity(m_pItemTransformUI);
        m_pItemTransformUI = NULL;
    }

    if(m_pFaceTransform)
    {
        GetNtlResourceManager()->UnLoadTexture(m_pFaceTransform);
        m_pFaceTransform = NULL;
    }

    DetachEffect();

    m_pActor->GetSobProxy()->SetInkColor(0, 0, 0);
    
    if(m_pSystemEffectColor[0])
    {
        m_pActor->GetSobProxy()->RemoveVisualSystemEffectColor(m_pSystemEffectColor[0]);
        m_pActor->GetSobProxy()->RemoveVisualSystemEffectColor(m_pSystemEffectColor[1]);
        m_pSystemEffectColor[0] = NULL;
        m_pSystemEffectColor[1] = NULL;
    }    

    if(m_pMajinPureModel)
    {
        if(m_bIsTransform)  // 현재의 모델은 SobProxy에서 지워준다, 쓰지 않는 모델을 삭제한다.
        {
            if(m_pMajinOrgModel)
            {
                GetSceneManager()->DeleteEntity(m_pMajinOrgModel);
                m_pMajinOrgModel = NULL;
            }            

            if(m_pMajinOrgModelUI)
            {
                GetSceneManager()->DeleteEntity(m_pMajinOrgModelUI);
                m_pMajinOrgModelUI = NULL;
            }
        }
        else
        {
            if(m_pMajinPureModel)
            {
                GetSceneManager()->DeleteEntity(m_pMajinPureModel);
                m_pMajinPureModel = NULL;
            }

            if(m_pMajinPureModelUI)
            {
                GetSceneManager()->DeleteEntity(m_pMajinPureModelUI);
                m_pMajinPureModelUI = NULL;
            }

        }
    }

    if(m_pCandyModel)
    {
        if(m_bIsTransform)
        {
            if(m_pCandyOrgModel)
            {
                GetSceneManager()->DeleteEntity(m_pCandyOrgModel);
                m_pCandyOrgModel = NULL;
            }
            
            if(m_pCandyOrgModelUI)
            {
                GetSceneManager()->DeleteEntity(m_pCandyOrgModelUI);
                m_pCandyOrgModelUI = NULL;
            }
        }
        else
        {
            if(m_pCandyModel)
            {
                GetSceneManager()->DeleteEntity(m_pCandyModel);
                m_pCandyModel = NULL;
            }

            if(m_pCandyModelUI)
            {
                GetSceneManager()->DeleteEntity(m_pCandyModelUI);
                m_pCandyModelUI = NULL;
            }

        }
    }
}

void CNtlSobCharProxyTransform::Update( RwReal fElapsed ) 
{
    if(m_bScheduleLoadingCheck)
    {
        if(m_pPLCharacter->IsSchedulingLoadingComplete())
        {            
            EventSobTransform(&m_transFormData);

            m_bScheduleLoadingCheck = FALSE;
        }
    }

    if(m_uiTransformId == ASPECTSTATE_PURE_MAJIN)
    {
        if(m_bIsTransform)
        {
            m_pMajinOrgModel->SetVisible(FALSE);
        }
        else
        {
            m_pMajinPureModel->SetVisible(FALSE);
        }
    }
}

void CNtlSobCharProxyTransform::HandleEvents( RWS::CMsg &pMsg ) 
{
    if(pMsg.Id == g_EventTransform)
    {
        SNtlEventTransform* data = (SNtlEventTransform*)pMsg.pData;
        if(data->hSerialId != m_pActor->GetSerialID())
            return;        

        m_transFormData = *data;

        if(m_pPLCharacter->IsSchedulingLoadingComplete())
        {
            EventSobTransform(data);        
        }
        else
        {
            // Processes the event after the schedule loading finishes.
            m_bScheduleLoadingCheck = TRUE;
        }
    }
    else if(pMsg.Id == g_EventSobSkillAction)       // When using Skill
    {
        EventSobSkillAction(pMsg);
    }
    else if(pMsg.Id == g_EventAnimHit)  // Hit 이벤트가 변신 타이밍이다.
    {
        if(m_bReadyTransform)   
		{
            CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>( m_pActor->GetBehaviorData() );
            SSkillStuff *pSkillStuff = pBeData->GetSkillStuff();            
            RwUInt32 uiTransCode = Logic_IsTransformSkill(pSkillStuff->uiSkillTblId);
            if(uiTransCode > 0)
            {
                switch(uiTransCode)
                {
					case ACTIVE_SUPER_SAIYAN:   uiTransCode = ASPECTSTATE_SUPER_SAIYAN;   break;
					case ACTIVE_GREAT_NAMEK:    uiTransCode = ASPECTSTATE_GREAT_NAMEK;    break;
					case ACTIVE_PURE_MAJIN:     uiTransCode = ASPECTSTATE_PURE_MAJIN;     break;
					case ACTIVE_KAIOKEN:        uiTransCode = ASPECTSTATE_KAIOKEN;        break;
                }

                CNtlSobPlayer* pPlayer = (CNtlSobPlayer*)m_pActor;
                pPlayer->GetAspectState()->sAspectStateBase.byAspectStateId = (BYTE)uiTransCode;
                
                CNtlSLEventGenerator::SobTransform(m_pActor->GetSerialID(), *pPlayer->GetAspectState());
            }   
            
            m_bReadyTransform = FALSE;
        }
    }
    else if(pMsg.Id == g_EventSobEquipChange)
    {
        ChangeWeapon(pMsg);
    }
    else if(pMsg.Id == g_EventMobTransform)
    {
        SNtlEventMobTransform* pData = (SNtlEventMobTransform*)pMsg.pData;
        OnTransform_Mob(pData->tblTransformMobID);
    }
    else if(pMsg.Id == g_EventTransformCandy)
    {
        SNtlEventTransformCandy* pData = (SNtlEventTransformCandy*)pMsg.pData;
        OnTransform_Candy(pData->bTransform);
    }
    else if(pMsg.Id == g_EventSobChangeAdult)
    {
		EventSobChangeAdult(pMsg);
    }
	else if (pMsg.Id == g_EventCinematicScale)
	{
		SNtlEventSobCinematicScale* pData = (SNtlEventSobCinematicScale*)pMsg.pData;

		if (pData->hSerialId != m_pActor->GetSerialID())
			return;

		OnTransform_CinematicScale(pData->fScale, pData->fTime);
	}
}

void CNtlSobCharProxyTransform::EventSobTransform(SNtlEventTransform* pData)
{
    if(m_pActor->GetSerialID() != pData->hSerialId)
        return;

	if (m_pCandyOrgModel && m_pCandyOrgModel->IsVisible())
		return;

    RwBool bTransform = pData->sApsectState.sAspectStateBase.byAspectStateId == ASPECTSTATE_INVALID ? FALSE : TRUE;
    
    if(!bTransform) 
    {
        // An avatar is transformed from a hit event, but before the event occurs, the released packet may come first.    
        m_bReadyTransform = FALSE;
    }

    if(m_bIsTransform == bTransform)
        return;    

    m_bIsTransform = bTransform;
    
    switch(pData->sApsectState.sAspectStateBase.byAspectStateId)
    {
		case ASPECTSTATE_SUPER_SAIYAN:
			OnTransform_SuperSaiyan(bTransform);
        break;
		case ASPECTSTATE_GREAT_NAMEK:
			OnTransform_BigNamek(bTransform);
        break;
		case ASPECTSTATE_PURE_MAJIN:
			OnTransform_PureMajin(bTransform);
        break;
		case ASPECTSTATE_KAIOKEN:
			OnTransform_KaiOuKen(bTransform, pData->sApsectState.sAspectStateDetail.sKaioken.bySourceGrade);
        break;
		case ASPECTSTATE_INVALID:
        {
            switch(m_uiTransformId)
            {
				case ASPECTSTATE_SUPER_SAIYAN:  OnTransform_SuperSaiyan(FALSE);     break;                
				case ASPECTSTATE_GREAT_NAMEK:   OnTransform_BigNamek(FALSE);        break;
				case ASPECTSTATE_PURE_MAJIN:    OnTransform_PureMajin(FALSE);       break;
				case ASPECTSTATE_KAIOKEN:       OnTransform_KaiOuKen(FALSE, 0); break;
            }
        }
        break;
    }

    m_uiTransformId = pData->sApsectState.sAspectStateBase.byAspectStateId;
}

void CNtlSobCharProxyTransform::EventSobSkillAction( RWS::CMsg& pMsg ) 
{
    // After confirming the transformation skill, apply it.   
    SNtlEventSobSkillAction* pData = (SNtlEventSobSkillAction*)pMsg.pData;
    if(pData->hSerialId != m_pActor->GetSerialID())
        return;

    m_byTransformLevel = (BYTE)Logic_IsTransformSkill(pData->uiSkillTblId);
    if(m_byTransformLevel > 0)
    { 
        m_bReadyTransform = TRUE;
    }
    else
    {
        m_bReadyTransform = FALSE;
    }
}

void CNtlSobCharProxyTransform::OnTransform_SuperSaiyan( RwBool bTransform ) 
{
    if(bTransform)
    {
        // Item
        if(!m_pItemTransform)
        {
            m_pItemTransform = GetTransformItem();
        }
        
        m_pItemTransform->SetVisible(TRUE);
        m_pPLCharacter->SetChangeEquipItem(m_pItemTransform);        

		if(m_pPLCharacterUI)
        {
            m_pItemTransformUI = GetTransformItem();
            m_pPLCharacterUI->SetChangeEquipItem(m_pItemTransformUI);
        }   

		if(m_pPLCharacterUI_Dogi)
		{
			m_pItemTransformUI_Dogi = GetTransformItem();
			m_pPLCharacterUI_Dogi->SetChangeEquipItem(m_pItemTransformUI_Dogi);
		}   

        // Face Texture       
        if(m_pFaceTransform)
        {
            GetNtlResourceManager()->UnLoadTexture(m_pFaceTransform);
            m_pFaceTransform = NULL;
        }
        SCharScheduleResInfo* pInfo = m_pPLCharacter->GetCharScheduleResInfo();
        EClassGenderType eClassGenderType = CNtlDefaultItemTable::GetClassGenderType(pInfo->uiRace, 
                                                                                     pInfo->uiGender,
                                                                                     pInfo->bIsAdult);
        std::string* strTexName = CNtlDefaultItemTable::GetInstance().GetSuperSaiyanFaceMultiTextureName(eClassGenderType, pInfo->uiFaceType, DEF_ITEM_START);
        m_pFaceTransform = CNtlPLResourceManager::GetInstance()->LoadTexture(strTexName->c_str(), "Texture\\Item\\;");            

        CNtlPLItem* pFaceItem = m_pPLCharacter->GetEquipItem()->GetDefItem(ITEM_EQUIP_FACE);
        m_pPLCharacter->GetEquipItem()->SetApplyFaceData(pFaceItem);    // 변신 상태에서 성인 상태로 변경될떄 얼굴 페이스를 바꾸기위해서 한번 적용해야 한다.
        pFaceItem->SetMultiTexture(m_pFaceTransform);
        m_pPLCharacter->GetEquipItem()->SetChangeFaceEnable(FALSE);

        if(m_pPLCharacterUI)
        {
            pFaceItem = m_pPLCharacterUI->GetEquipItem()->GetDefItem(ITEM_EQUIP_FACE);
            m_pPLCharacterUI->GetEquipItem()->SetApplyFaceData(pFaceItem);
            pFaceItem->SetMultiTexture(m_pFaceTransform);
            m_pPLCharacterUI->GetEquipItem()->SetChangeFaceEnable(FALSE);            
        }  

		if(m_pPLCharacterUI_Dogi)
		{
			pFaceItem = m_pPLCharacterUI_Dogi->GetEquipItem()->GetDefItem(ITEM_EQUIP_FACE);
            m_pPLCharacterUI_Dogi->GetEquipItem()->SetApplyFaceData(pFaceItem);
			pFaceItem->SetMultiTexture(m_pFaceTransform);
			m_pPLCharacterUI_Dogi->GetEquipItem()->SetChangeFaceEnable(FALSE);            
		}

        // Effect        
        AttachEffectSuperSaiyan(m_pPLCharacter);                                        

        // color
        m_pActor->GetSobProxy()->SetInkColor(255, 255, 255);                
        m_pSystemEffectColor[0] = m_pActor->GetSobProxy()->AddVisualSystemEffectColor(255, 255, 120);            
        m_pSystemEffectColor[1] = m_pActor->GetSobProxy()->AddVisualSystemEffectColor(20, 20, 20, TRUE);
    }
    else
    {
        if(m_pItemTransform)
        {
            if(m_pPLCharacterUI)
            {
                m_pPLCharacterUI->SetRemoveEquipItem(m_pItemTransformUI);
                m_pPLCharacterUI->GetEquipItem()->SetChangeFaceEnable(TRUE);
            }

			if(m_pPLCharacterUI_Dogi)
			{
				m_pPLCharacterUI_Dogi->SetRemoveEquipItem(m_pItemTransformUI_Dogi);
				m_pPLCharacterUI_Dogi->GetEquipItem()->SetChangeFaceEnable(TRUE);
			}

            m_pPLCharacter->SetRemoveEquipItem(m_pItemTransform);
            m_pPLCharacter->GetEquipItem()->SetChangeFaceEnable(TRUE);
        }

        DetachEffect();

        m_pActor->GetSobProxy()->SetInkColor(0, 0, 0);
        m_pActor->GetSobProxy()->RemoveVisualSystemEffectColor(m_pSystemEffectColor[0]);
        m_pActor->GetSobProxy()->RemoveVisualSystemEffectColor(m_pSystemEffectColor[1]);
        m_pSystemEffectColor[0] = NULL;
        m_pSystemEffectColor[1] = NULL;
    }
}


void CNtlSobCharProxyTransform::OnTransform_KaiOuKen(RwBool bTransform, BYTE byLevel)
{
    if(bTransform)
    {
        AttachEffectKaiouKen(m_pPLCharacter, byLevel);

        // color
        m_pActor->GetSobProxy()->SetInkColor(211, 170, 187);                
        m_pKaiOuKenColor[0] = m_pActor->GetSobProxy()->AddVisualSystemEffectColor(242, 181, 140);            
        m_pKaiOuKenColor[1] = m_pActor->GetSobProxy()->AddVisualSystemEffectColor(160, 7, 12, TRUE);
    }
    else
    {
        DetachEffect();

        m_pActor->GetSobProxy()->SetInkColor(0, 0, 0);
        m_pActor->GetSobProxy()->RemoveVisualSystemEffectColor(m_pKaiOuKenColor[0]);
        m_pActor->GetSobProxy()->RemoveVisualSystemEffectColor(m_pKaiOuKenColor[1]);
        m_pKaiOuKenColor[0] = NULL;
        m_pKaiOuKenColor[1] = NULL;
    }
}

void CNtlSobCharProxyTransform::OnTransform_BigNamek( RwBool bTransform ) 
{
    if(bTransform)
    {
        // 크기 변경
        m_pActor->GetSobProxy()->AddVisualSystemEffectScale(Dbo_GetTransformScale(ASPECTSTATE_GREAT_NAMEK, 1), 2.5f);

        // 무기 제거
        DetachWeapon();

        // 공격 범위 설정
        CNtlSobBattleAttr *pSobBattleAttr = reinterpret_cast<CNtlSobBattleAttr*>(m_pActor->GetSobAttr());
        m_fOrgAttackRange = pSobBattleAttr->GetAttackRange();
        pSobBattleAttr->SetAttackRange(TRANSFORM_NAMEK_ATTACK_RANGE);
    }
    else
    {
        // 크기 변경
        m_pActor->GetSobProxy()->AddVisualSystemEffectScale(1.0f, 2.5f);            

        // 무기 장착
        AttachWeapon();

        // 공격 범위 설정
        CNtlSobBattleAttr *pSobBattleAttr = reinterpret_cast<CNtlSobBattleAttr*>(m_pActor->GetSobAttr());        
        pSobBattleAttr->SetAttackRange(m_fOrgAttackRange);

        // Idle 상태라면 애니메이션 변경
        if(Logic_GetActorStateId(m_pActor) == NTL_FSMSID_IDLE ||
           Logic_GetActorStateId(m_pActor) == NTL_FSMSID_FIGHTING_POSE)
        {
            m_pActor->GetSobProxy()->SetBaseAnimation(Logic_GetIdleAnimationID(m_pActor));
        }
    }    
}

void CNtlSobCharProxyTransform::AttachEffectSuperSaiyan(CNtlPLCharacter* pPLCharacter)
{
    if(!m_vecEffect.empty())
        return;

    // This list will have effects
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
    for(int i = 0; i < 7; ++i)      // Small effect
    {
        if(pPLCharacter->GetBoneByName(strBoneList1[i].c_str()))
        {
            pEffect = (CNtlInstanceEffect*)GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_SUPER_SAIYAN_SMALL);            
            if(pEffect)
            {
                pPLCharacter->AttachBone(pEffect, strBoneList1[i].c_str());    
                m_vecEffect.push_back(pEffect);
            }
        }        
    }

    if(pPLCharacter->GetBoneByName(strBoneList2.c_str())) // Large effect
    {
        pEffect = (CNtlInstanceEffect*)GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_SUPER_SAIYAN_BIG);
        if(pEffect)
        {
            pPLCharacter->AttachBone(pEffect, strBoneList2.c_str());
            m_vecEffect.push_back(pEffect);
        }
    }   
}


void CNtlSobCharProxyTransform::AttachEffectKaiouKen(CNtlPLCharacter* pPLCharacter, BYTE byLevel)
{
    if(!m_vecEffect.empty())
        return;

    // This list will have effects
    std::string strBoneList1[7];
    strBoneList1[0] = "Bip01 L Foot"; 
    strBoneList1[1] = "Bip01 R Foot"; 
    strBoneList1[2] = "Bip01 L Calf"; 
    strBoneList1[3] = "Bip01 R Calf"; 
    strBoneList1[4] = "Bip01 L Forearm";
    strBoneList1[5] = "Bip01 R Forearm";
    strBoneList1[6] = "Bip01 Head";

    std::string strBoneList2 = "Bip01 Pelvis";

    CNtlInstanceEffect* pEffect;
    for(int i = 0; i < 7; ++i)      // Small effect
    {
        if(pPLCharacter->GetBoneByName(strBoneList1[i].c_str()))
        {
            pEffect = (CNtlInstanceEffect*)GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_KAIOUKEN_SMALL);            
            if(pEffect)
            {
                pPLCharacter->AttachBone(pEffect, strBoneList1[i].c_str());    
                m_vecEffect.push_back(pEffect);
            }
        }        
    }

    if(pPLCharacter->GetBoneByName(strBoneList2.c_str())) // Large effect
    {
        pEffect = (CNtlInstanceEffect*)GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_KAIOUKEN_BIG);
        if(pEffect)
        {
            pPLCharacter->AttachBone(pEffect, strBoneList2.c_str());
            m_vecEffect.push_back(pEffect);
        }
    }  
}

void CNtlSobCharProxyTransform::DetachEffect()
{
    if(m_vecEffect.empty())
        return;

    for each(CNtlInstanceEffect* pEffect in m_vecEffect)
    {
        pEffect->Finish();
    }

    m_vecEffect.clear();
}

void CNtlSobCharProxyTransform::OnTransform_CinematicScale(float fScale, float fTime)
{
	m_pActor->GetSobProxy()->AddVisualSystemEffectScale(fScale, fTime);
}

CNtlPLItem* CNtlSobCharProxyTransform::GetTransformItem() 
{
    SCharScheduleResInfo* pInfo = m_pPLCharacter->GetCharScheduleResInfo();    

    RwBool bLoadScheduling = GetNtlResourceManager()->IsLoadScheduling();

    if(m_pPLCharacter->IsSchedulingLoadingComplete())
    {
        GetNtlResourceManager()->SetLoadScheduling(FALSE);
    }
    else // 아직 스케쥴 로딩이 끝나지 않았을때
    {
        GetNtlResourceManager()->SetLoadScheduling(TRUE);
        m_bScheduleLoadingCheck = TRUE;
    }

    CNtlPLItem* pTransformItem = NULL;    
    EClassGenderType eClassGenderType = CNtlDefaultItemTable::GetClassGenderType(pInfo->uiRace, 
                                                                                 pInfo->uiGender,
                                                                                 pInfo->bIsAdult);

    std::string* pstrHeadKeyName = CNtlDefaultItemTable::GetInstance().GetSuperSaiyanHeadKeyName(eClassGenderType, pInfo->uiHeadType);
    if(pstrHeadKeyName)
    {
        pTransformItem = (CNtlPLItem*)GetSceneManager()->CreateEntity(PLENTITY_ITEM, pstrHeadKeyName->c_str());        
        if(!pTransformItem)
        {   
             NTL_ASSERTFAIL("Not Exist Transform Item");                           
        }        

        pTransformItem->SetFlags(pTransformItem->GetFlags() & ~NTL_PLEFLAG_PICKING);

        if(m_pPLCharacter->IsSchedulingLoadingComplete())
        {
            pTransformItem->SetVisible(m_pActor->IsVisible());
        }
        else
        {
            pTransformItem->SetVisible(FALSE);
        }
    }

    GetNtlResourceManager()->SetLoadScheduling(bLoadScheduling);

    return pTransformItem;
}

void CNtlSobCharProxyTransform::DetachWeapon() 
{
    if(!m_pItemOrgWeapon)        
    {
        m_pItemOrgWeapon = NTL_NEW SEquipItem();
    }

    if(!m_pItemOrgSubWeapon)
    {
        m_pItemOrgSubWeapon = NTL_NEW SEquipItem();
    }

    
    CNtlSobCharProxy* pCharProxy = (CNtlSobCharProxy*)m_pActor->GetSobProxy();

    memcpy(m_pItemOrgWeapon, pCharProxy->GetEquipItem(EQUIP_SLOT_TYPE_HAND), sizeof(SEquipItem));
    memcpy(m_pItemOrgSubWeapon, pCharProxy->GetEquipItem(EQUIP_SLOT_TYPE_SUB_WEAPON), sizeof(SEquipItem));   
    
    pCharProxy->DeleteEquipItem(EQUIP_SLOT_TYPE_HAND);
    pCharProxy->DeleteEquipItem(EQUIP_SLOT_TYPE_SUB_WEAPON);
}

void CNtlSobCharProxyTransform::AttachWeapon() 
{
    if(!m_pItemOrgWeapon || !m_pItemOrgSubWeapon)
        return;

    if(m_pItemOrgWeapon->uiItemTblId != INVALID_TBLIDX)
    {
        sITEM_TBLDAT* pData = (sITEM_TBLDAT*)API_GetTableContainer()->GetItemTable()->FindData(m_pItemOrgWeapon->uiItemTblId);        
        CNtlSLEventGenerator::SobEquipModelChange(m_pActor->GetSerialID(), EQUIP_SLOT_TYPE_HAND, m_pItemOrgWeapon->uiItemTblId, m_pItemOrgWeapon->byGrade, pData->byBattle_Attribute);
    }

    if(m_pItemOrgSubWeapon->uiItemTblId != INVALID_TBLIDX)
    {
        sITEM_TBLDAT* pData = (sITEM_TBLDAT*)API_GetTableContainer()->GetItemTable()->FindData(m_pItemOrgSubWeapon->uiItemTblId);
        CNtlSLEventGenerator::SobEquipModelChange(m_pActor->GetSerialID(), EQUIP_SLOT_TYPE_SUB_WEAPON, m_pItemOrgSubWeapon->uiItemTblId, m_pItemOrgSubWeapon->byGrade, pData->byBattle_Attribute);
    }
}

void CNtlSobCharProxyTransform::ChangeWeapon( RWS::CMsg& pMsg ) 
{
    SNtlEventSobEquipChange *pEquipChange = reinterpret_cast<SNtlEventSobEquipChange*>( pMsg.pData );
    if(m_pActor->GetSerialID() != pEquipChange->hSerialId)
        return;

    if(!m_bIsTransform)
        return;

    if(!m_pItemOrgWeapon)        
    {
        m_pItemOrgWeapon = NTL_NEW SEquipItem();
    }

    if(!m_pItemOrgSubWeapon)
    {
        m_pItemOrgSubWeapon = NTL_NEW SEquipItem();
    }

    switch(pEquipChange->bySlotIdx)
    {
    case EQUIP_SLOT_TYPE_HAND:        
        m_pItemOrgWeapon->uiItemTblId = pEquipChange->uiTblId;
        m_pItemOrgWeapon->byGrade = pEquipChange->byGrade;
        break;
    case EQUIP_SLOT_TYPE_SUB_WEAPON:                
        m_pItemOrgSubWeapon->uiItemTblId = pEquipChange->uiTblId;
        m_pItemOrgSubWeapon->byGrade = pEquipChange->byGrade;
        break;
    }    
}

void CNtlSobCharProxyTransform::AddWorld() 
{
    // 초사이어인
    if(m_bIsTransform)
    {
        if(m_pItemTransform)
            m_pItemTransform->AddWorld();
        if(m_pItemTransformUI)
            m_pItemTransformUI->AddWorld();
    }
    

    // 순수 마인 (비 변신상태라도 모델을 생성했으면 월드에 추가해야만 한다)    
    if(m_pMajinOrgModel)
        m_pMajinOrgModel->AddWorld();    
    if(m_pMajinPureModel)
        m_pMajinPureModel->AddWorld();
    if(m_pMajinOrgModelUI)
        m_pMajinOrgModelUI->AddWorld();
    if(m_pMajinPureModelUI)
        m_pMajinPureModelUI->AddWorld();

    // 캔디
    if(m_pCandyOrgModel)
        m_pCandyOrgModel->AddWorld();
    if(m_pCandyOrgModelUI)
        m_pCandyOrgModelUI->AddWorld();
    if(m_pCandyModel)
        m_pCandyModel->AddWorld();
    if(m_pCandyModelUI)
        m_pCandyModelUI->AddWorld();
}

void CNtlSobCharProxyTransform::RemoveWorld() 
{
    switch(m_uiTransformId)
    {
    case ASPECTSTATE_SUPER_SAIYAN:
        if(m_bIsTransform)
        {
            if(m_pItemTransform)
                m_pItemTransform->RemoveWorld();
            if(m_pItemTransformUI)
                m_pItemTransformUI->RemoveWorld();
        }        
        break;
    case ASPECTSTATE_GREAT_NAMEK:
        break;
    case ASPECTSTATE_PURE_MAJIN:                
        break;
    }

    // 순수 마인 (비 변신상태라도 모델을 생성했으면 월드에 추가해야만 한다)            
    if(m_pMajinOrgModel)
        m_pMajinOrgModel->RemoveWorld();    
    if(m_pMajinPureModel)
        m_pMajinPureModel->RemoveWorld();
    if(m_pMajinOrgModelUI)
        m_pMajinOrgModelUI->RemoveWorld();
    if(m_pMajinPureModelUI)
        m_pMajinPureModelUI->RemoveWorld();

    // 캔디
    if(m_pCandyOrgModel)
        m_pCandyOrgModel->RemoveWorld();
    if(m_pCandyOrgModelUI)
        m_pCandyOrgModelUI->RemoveWorld();
    if(m_pCandyModel)
        m_pCandyModel->RemoveWorld();
    if(m_pCandyModelUI)
        m_pCandyModelUI->RemoveWorld();
}

// 마진은 컨버트 클래스처럼 모델을 직접 변경한다
void CNtlSobCharProxyTransform::OnTransform_PureMajin( RwBool bTransform ) 
{
    if(bTransform)
    {
        m_pMajinOrgModel = m_pPLCharacter;        
        m_pMajinOrgModelUI = m_pPLCharacterUI;
		m_pMajinOrgModelUI_Dogi = m_pPLCharacterUI_Dogi;

        m_pPLCharacter = CreatePureMajinModel();
        if(!m_pPLCharacter)
        {
            m_pPLCharacter = m_pMajinOrgModel;
            return;
        }
        else
        {
            m_pMajinOrgModel->SetVisible(FALSE);         
            m_pMajinOrgModel->RemoveWorld();

            m_pPLCharacter->SetVisible(TRUE);            
            m_pPLCharacter->AddWorld();

            CNtlSobCharProxy* pSobProxy = (CNtlSobCharProxy*)m_pActor->GetSobProxy();                                                
            pSobProxy->ChangeModel(m_pPLCharacter, FALSE);            
            pSobProxy->GetDecorationProxy()->SetVisible(TRUE);

            if(m_pActor->GetClassID() == SLCLASS_AVATAR)
            {
                pSobProxy->ChangeUIModel(m_pMajinPureModelUI);
                m_pPLCharacterUI = m_pMajinPureModelUI;

				pSobProxy->ChangeUIModel(m_pMajinPureModelUI_Dogi);
				m_pPLCharacterUI_Dogi = m_pMajinPureModelUI_Dogi;
            }
            

            m_pPLCharacter->SetBlend(BLEND_TWEEN, 0.0f, 0.0f);
            m_pPLCharacter->SetBaseAnimation(SKILL_ANIMATION_START, 0.35f, FALSE);            
            m_pPLCharacter->Update(0.0f);            
        }       
    }
    else
    {
        if(m_pMajinOrgModel)
        {
            m_pPLCharacter->SetVisible(FALSE);
            m_pPLCharacter->RemoveWorld();

            m_pMajinOrgModel->SetVisible(TRUE);
            m_pMajinOrgModel->AddWorld();

            m_pPLCharacter = m_pMajinOrgModel;            
        }        

        CNtlSobCharProxy* pSobProxy = (CNtlSobCharProxy*)m_pActor->GetSobProxy();        
        pSobProxy->ChangeModel(m_pMajinOrgModel, FALSE);   

        if(m_pActor->GetClassID() == SLCLASS_AVATAR)
        {
            pSobProxy->ChangeUIModel(m_pMajinOrgModelUI);
			m_pPLCharacterUI = m_pMajinOrgModelUI;

			pSobProxy->ChangeUIModel(m_pMajinOrgModelUI_Dogi);
			m_pPLCharacterUI_Dogi = m_pMajinOrgModelUI_Dogi;
        }        
        
        CNtlPLEntity* pEffect = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_PURE_MAJIN_CHANGE_ORG, NULL);
        pEffect->SetPosition(&m_pActor->GetPosition());
        
    }
}

CNtlPLCharacter* CNtlSobCharProxyTransform::CreatePureMajinModel() 
{
    if(m_pMajinPureModel)
        return m_pMajinPureModel;

    // 스케쥴 로딩을 끈다.
    GetNtlResourceManager()->SetLoadScheduling(FALSE);

    std::string strModelName = "A_PURE_MAJIN";
    SPLCharacterCreateParam sParam;
    ZeroMemory(&sParam, sizeof(sParam));
    sParam.pPos         = &m_pActor->GetPosition();
    sParam.uiSerialId   = m_pActor->GetSerialID();
    sParam.bPcFlag      = FALSE;
    sParam.bNotShadingFlag = FALSE;

    m_pMajinPureModel = (CNtlPLCharacter*)GetSceneManager()->CreateEntity(PLENTITY_CHARACTER, strModelName.c_str(), &sParam);
    NTL_ASSERT(m_pMajinPureModel, "Can't Create Pure Majin Model : " << strModelName.c_str());    

    m_pMajinPureModel->SetSerialID(m_pActor->GetSerialID());    
    

    // 스킨 칼라 적용
    SCharScheduleResInfo* pInfo = m_pPLCharacter->GetCharScheduleResInfo();
    EClassGenderType eClassGenderType = CNtlDefaultItemTable::GetClassGenderType(pInfo->uiRace, pInfo->uiGender, pInfo->bIsAdult);    
    RwRGBA *pSkinColor = CNtlDefaultItemTable::GetInstance().GetSkinColor(eClassGenderType, m_pPLCharacter->GetCharScheduleResInfo()->uiSkinColorType);
    m_pMajinPureModel->UpdateMaterialSkinInfo();    
    m_pMajinPureModel->SetSkinColor(pSkinColor->red, pSkinColor->green, pSkinColor->blue);

    // UI용 순수 마인 모델
    if(m_pActor->GetClassID() == SLCLASS_AVATAR)
    {
        m_pMajinPureModelUI = (CNtlPLCharacter*)GetSceneManager()->CreateEntity(PLENTITY_CHARACTER, strModelName.c_str(), &sParam);
        m_pMajinPureModelUI->SetBaseAnimation(NML_IDLE_LOOP);    
        m_pMajinPureModelUI->SetFlags(m_pMajinPureModelUI->GetFlags() | NTL_PLEFLAG_DUMMY);
        m_pMajinPureModelUI->SetVisible(FALSE);
        m_pMajinPureModelUI->UpdateMaterialSkinInfo();
        m_pMajinPureModelUI->SetSkinColor(pSkinColor->red, pSkinColor->green, pSkinColor->blue);
    }

    GetNtlResourceManager()->SetLoadScheduling(TRUE);

    
    return m_pMajinPureModel;
}

CNtlPLCharacter* CNtlSobCharProxyTransform::CreateCandyModel() 
{
    if(m_pCandyModel)
        return m_pCandyModel;

    GetNtlResourceManager()->SetLoadScheduling(FALSE);
    
    SPLCharacterCreateParam sParam;
    ZeroMemory(&sParam, sizeof(sParam));
    sParam.pPos         = &m_pActor->GetPosition();
    sParam.uiSerialId   = m_pActor->GetSerialID();
    sParam.bPcFlag      = FALSE;
    sParam.bNotShadingFlag = FALSE;

    std::string strModelName;

    if(m_pActor->GetClassID() == SLCLASS_MONSTER)
    {
        strModelName = "A_MON_CANDY";
    }
    else
    {
        switch(Logic_GetPlayerRace(m_pActor))
        {
        case RACE_HUMAN:
            strModelName = "A_HUM_CANDY";
            break;
        case RACE_NAMEK:
            strModelName = "A_NAM_CANDY";
            break;
        case RACE_MAJIN:
            strModelName = "A_MAJ_CANDY";
            break;
        }
    }

    m_pCandyModel = (CNtlPLCharacter*)GetSceneManager()->CreateEntity(PLENTITY_CHARACTER, strModelName.c_str(), &sParam);
    NTL_ASSERT(m_pCandyModel, "Can't Create Candy Model : " << strModelName.c_str());
    m_pCandyModel->SetSerialID(m_pActor->GetSerialID());

    // 사이즈에 맞는 크기를 적용한다.
    if(m_pActor->GetClassID() == SLCLASS_MONSTER)
    {
        RwReal fHeight = m_pPLCharacter->GetHeight();
        if(fHeight >= CANDY_HEIGHT_BIG)
        {
            m_pCandyModel->SetScale(CANDY_SCALE_BIG);
        }
        else if(fHeight >= CANDY_HEIGHT_MIDDLE)
        {
            m_pCandyModel->SetScale(CANDY_SCALE_MIDDLE);
        }
    }    

    // UI용 모델
    if(m_pActor->GetClassID() == SLCLASS_AVATAR)
    {
        m_pCandyModelUI = (CNtlPLCharacter*)GetSceneManager()->CreateEntity(PLENTITY_CHARACTER, strModelName.c_str(), &sParam);
        m_pCandyModelUI->SetBaseAnimation(NML_IDLE_LOOP);
        m_pCandyModelUI->SetFlags(m_pCandyModelUI->GetFlags() | NTL_PLEFLAG_DUMMY);
        m_pCandyModelUI->SetVisible(FALSE);        

		// by daneos: Disable to avoid client crash when relog while in candy mode
        /*m_pCandyModelUI_Dogi = (CNtlPLCharacter*)GetSceneManager()->CreateEntity(PLENTITY_CHARACTER, strModelName.c_str(), &sParam);
        m_pCandyModelUI_Dogi->SetBaseAnimation(NML_IDLE_LOOP);
        m_pCandyModelUI_Dogi->SetFlags(m_pCandyModelUI->GetFlags() | NTL_PLEFLAG_DUMMY);
        m_pCandyModelUI_Dogi->SetVisible(FALSE);*/
    }

    GetNtlResourceManager()->SetLoadScheduling(TRUE);

    return m_pCandyModel;
}

void CNtlSobCharProxyTransform::OnTransform_Candy( RwBool bTransform ) 
{
    if(!m_pPLCharacter)
        return;

    if(m_bIsTransform == bTransform)
        return;

    m_bIsTransform = bTransform;

    if(bTransform)
    {
        m_pCandyOrgModel = m_pPLCharacter;
		m_pCandyOrgModelUI = m_pPLCharacterUI;
		m_pCandyOrgModelUI_Dogi = m_pPLCharacterUI_Dogi;

        m_pPLCharacter = CreateCandyModel();
        if(!m_pPLCharacter)
        {
            m_pPLCharacter = m_pCandyOrgModel;
            return;
        }
        else
        {
            m_pCandyOrgModel->SetVisible(FALSE);
            m_pPLCharacter->SetVisible(TRUE);

            CNtlSobCharProxy* pSobProxy = (CNtlSobCharProxy*)m_pActor->GetSobProxy();
            pSobProxy->ChangeModel(m_pPLCharacter, FALSE);
            pSobProxy->GetDecorationProxy()->SetVisible(TRUE);

            if(m_pActor->GetClassID() == SLCLASS_AVATAR)
            {
                pSobProxy->ChangeUIModel(m_pCandyModelUI);
                m_pPLCharacterUI = m_pCandyModelUI;

				pSobProxy->ChangeUIModel_Dogi(m_pCandyModelUI_Dogi);
				m_pPLCharacterUI_Dogi = m_pCandyModelUI_Dogi;
			}

            m_pPLCharacter->SetBlend(BLEND_TWEEN, 0.0f, 0.0f);
            m_pPLCharacter->SetBaseAnimation(TRANS_IDLE, 0.0f, FALSE);            
            m_pPLCharacter->Update(0.0f);            
        }
    }
    else
    {
        if(!m_pCandyOrgModel)
            return;

        m_pPLCharacter->SetVisible(FALSE);
        m_pCandyOrgModel->SetVisible(TRUE);
        m_pPLCharacter = m_pCandyOrgModel;
        
        CNtlSobCharProxy* pSobProxy = (CNtlSobCharProxy*)m_pActor->GetSobProxy();
        pSobProxy->ChangeModel(m_pCandyOrgModel, FALSE);

        if(m_pActor->GetClassID() == SLCLASS_AVATAR)
        {
            pSobProxy->ChangeUIModel(m_pCandyOrgModelUI);
            m_pPLCharacterUI = m_pCandyOrgModelUI;

			pSobProxy->ChangeUIModel_Dogi(m_pCandyOrgModelUI_Dogi);
			m_pPLCharacterUI_Dogi = m_pCandyOrgModelUI_Dogi;
        }

        CNtlPLEntity* pEffect = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_CANDY_CHANGE_ORG, NULL);
        pEffect->SetPosition(&m_pActor->GetPosition());

        // 이펙트 크기 조절
        if(m_pCandyModel->GetHeight() >= CANDY_HEIGHT_BIG)
        {
            pEffect->SetScale(CANDY_SCALE_BIG);
        }        
        else if(m_pCandyModel->GetHeight() >= CANDY_HEIGHT_MIDDLE)
        {
            pEffect->SetScale(CANDY_SCALE_MIDDLE);
        }
    }    
}

void CNtlSobCharProxyTransform::OnTransform_Mob( RwUInt32 tblIdx ) 
{
    if(!m_pPLCharacter)
        return;

    sMOB_TBLDAT* pTblData = (sMOB_TBLDAT*)API_GetTableContainer()->GetMobTable()->FindData(tblIdx);
    if(!pTblData)
        return;
    
    // 스케쥴 로딩을 끈다.
    GetNtlResourceManager()->SetLoadScheduling(FALSE);

    SPLCharacterCreateParam sParam;
    ZeroMemory(&sParam, sizeof(sParam));
    sParam.pPos         = &m_pActor->GetPosition();
    sParam.uiSerialId   = m_pActor->GetSerialID();
    sParam.bPcFlag      = FALSE;
    sParam.bNotShadingFlag = FALSE;

    CNtlSobCharProxy* pSobProxy = (CNtlSobCharProxy*)m_pActor->GetSobProxy();
    CNtlPLCharacter* pNewMob = (CNtlPLCharacter*)GetSceneManager()->CreateEntity(PLENTITY_CHARACTER, pTblData->szModel, &sParam);
    if(!pNewMob)
        return;
    
    pSobProxy->ChangeModel(pNewMob, FALSE, TRUE);    
    m_pPLCharacter = pNewMob;

    pSobProxy->SetBaseAnimation(TRANS_SPAWN, FALSE);

    // 몹이름 변경
    CNtlSobMonsterAttr* pMonsterAttr = reinterpret_cast<CNtlSobMonsterAttr*>( m_pActor->GetSobAttr() );

	CTextTable* pTextTable = API_GetTableContainer()->GetTextAllTable()->GetMobTbl();
	sTEXT_TBLDAT* pTextTblData = reinterpret_cast<sTEXT_TBLDAT*>(pTextTable->FindData(pTblData->Name));
	if (pTextTblData)
		pMonsterAttr->SetName(pTextTblData->wstrText.c_str());

    Logic_SetHeadNameColor(m_pActor);

    m_bIsTransform = TRUE;

    GetNtlResourceManager()->SetLoadScheduling(TRUE);
}

void CNtlSobCharProxyTransform::EventSobChangeAdult( RWS::CMsg& pMsg ) 
{
    if(!IsTransform())
        return;

    // 어른으로 바뀌면 변신 상태를 해제한다.
    switch(m_uiTransformId)
    {
		case ASPECTSTATE_SUPER_SAIYAN:  OnTransform_SuperSaiyan(FALSE);     break;
		case ASPECTSTATE_GREAT_NAMEK:   OnTransform_BigNamek(FALSE);        break;
		case ASPECTSTATE_PURE_MAJIN:    OnTransform_PureMajin(FALSE);       break;
		case ASPECTSTATE_KAIOKEN:       OnTransform_KaiOuKen(FALSE, 0); break;
    }
}

void CNtlSobCharProxyTransform::PostChangeAdult() 
{
    // If you change from an adult to a child, you can cancel the transformation and change it again.
    if(IsTransform())
    {
        m_bIsTransform = FALSE;
        EventSobTransform(&m_transFormData);
    }
}