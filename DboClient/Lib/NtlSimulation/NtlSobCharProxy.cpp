#include "precomp_ntlsimulation.h"
#include "NtlSobCharProxy.h"

// shared
#include "ItemTable.h"
#include "MobTable.h"

// core
#include "NtlDebug.h"
#include "NtlMath.h"

// presentation
#include "NtlPLSceneManager.h"
#include "NtlPLCharacter.h"
#include "NtlPLItem.h"
#include "NtlPLDecal.h"
#include "NtlPLPlayerName.h"
#include "NtlPLHelpers.h"
#include "NtlPLRenderGroup.h"
#include "NTLPLCameraRenderTexture.h"
#include "NtlPLEvent.h"

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
#include "NtlFSMDef.h"
#include "DBODirectorIndicate.h"
#include "DboTSCQAgency.h"
#include "DboTSCQCtrl.h"
#include "NtlSobCharProxyDecoration.h"
#include "NtlSobCharProxyEquip.h"
#include "NtlSobCharProxyTransform.h"
#include "NtlSLLogicDef.h"
#include "NtlCameraManager.h"
#include "NtlSLEventFunc.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define SOB_CHAR_PitchANGLE_PROXY	0.05f

CNtlSobCharPitchAngleProxy::CNtlSobCharPitchAngleProxy()
{
	m_fAngleX			= 0.0f;
	m_fUpdateAngleTime	= 0.0f;
	m_bCalcAngle		= FALSE;

	m_pSobObj			= NULL;
	m_pPLCharacter		= NULL;    
}


void CNtlSobCharPitchAngleProxy::Update(RwReal fElapsed)
{
	if(m_bCalcAngle)
	{
//		m_fUpdateAngleTime += fElapsed;
//		if(m_fUpdateAngleTime >= SOB_CHAR_PitchANGLE_PROXY)
//		{
			CalcAngle();
//		}
	}
}

void CNtlSobCharPitchAngleProxy::CalcAngle(void)
{
	RwV3d vPos = m_pSobObj->GetPosition();
	RwV3d vDir = m_pSobObj->GetDirection();

	vDir.y = 0.0f; 
	RwV3dNormalize(&vDir, &vDir);

	RwReal fDepth = m_pPLCharacter->GetDepth()/2.0f * 0.7f;

	RwV3d vFront, vBack;
	vFront.x = vPos.x + vDir.x * fDepth;
	vFront.y = vPos.y;
	vFront.z = vPos.z + vDir.z * fDepth;

	vBack.x = vPos.x - vDir.x * fDepth;
	vBack.y = vPos.y;
	vBack.z = vPos.z - vDir.z * fDepth;

	SWorldHeightStuff sStuff;
	Logic_GetWorldHeight(&vFront, sStuff);
	vFront.y = sStuff.fWorldHeight;
	
	Logic_GetWorldHeight(&vBack, sStuff);
	vBack.y = sStuff.fWorldHeight;

	RwV3dSubMacro(&vDir, &vFront, &vBack);
	RwV3dNormalize(&vDir, &vDir);

	m_fAngleX = CNtlMath::LineToAngleX(&vDir);

	m_pPLCharacter->SetAngleX(m_fAngleX);

	m_fUpdateAngleTime	= 0.0f;
	m_bCalcAngle		= FALSE;
}

void CNtlSobCharPitchAngleProxy::ReCalcAngle(CNtlSob *pSobObj, CNtlPLCharacter *pPLCharacter)
{
	m_pSobObj		= pSobObj;
	m_pPLCharacter	= pPLCharacter;
	m_bCalcAngle	= TRUE;
}


RwReal CNtlSobCharPitchAngleProxy::GetAngleX(void)
{
	return m_fAngleX;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNtlSobCharProxy::MapFuncEventHandler CNtlSobCharProxy::m_mapEventFunc;

CNtlSobCharProxy::CNtlSobCharProxy()
{
	m_pCharacter				= NULL;
	m_pEquipProxy				= NULL;
	m_pDecorationProxy			= NULL;
	m_pPrivateShopProxy			= NULL;
	m_pPitchAngleProxy			= NULL;

	m_fInkThickness				= DEFAULT_INK_THICKNESS;
	m_fCurrHalfOneSecTime		= 0.0f;
	m_fWeightInk				= 1.0f;
	m_bEnableHighlight			= TRUE;
	m_bResLoaded				= FALSE;
	m_byAnimBlendNoneOneTime	= 0;	

	m_pUIPcStatusWnd			= NULL;
	m_pUIPcStatusRotate			= NULL;
	m_pUIPcStatusBarCamera		= NULL;       

	m_pUIPcDogiWnd				= NULL;
	m_pUIPcDogiRotate			= NULL;

    m_bRemoveColorChangeEffect  = FALSE;
    m_ppColorChangeEffect[0]    = NULL;
    m_ppColorChangeEffect[1]    = NULL;
    m_pFocusColorEffect       = NULL;    
}

CNtlSobCharProxy::~CNtlSobCharProxy()
{

}

void CNtlSobCharProxy::RegisterEventHandler(void)
{
	m_mapEventFunc[g_EventSobCreate.Get_pEventId()] = &CNtlSobCharProxy::SobCreateEventHandler;	
	m_mapEventFunc[g_EventSobEquipChange.Get_pEventId()] = &CNtlSobCharProxy::SobEquipChangeEventHandler;
	m_mapEventFunc[g_EventSobGotFocus.Get_pEventId()] = &CNtlSobCharProxy::SobGotFocusEventHandler;
	m_mapEventFunc[g_EventSobLostFocus.Get_pEventId()] = &CNtlSobCharProxy::SobLostFocusEventHandler;
	m_mapEventFunc[g_EventQuestAcceptProposal_Nfy.Get_pEventId()] = &CNtlSobCharProxy::SobQuestAcceptProposalNfy;
	m_mapEventFunc[g_EventQuestAcceptReward_Nfy.Get_pEventId()] = &CNtlSobCharProxy::SobQuestAcceptRewardNfy;
	m_mapEventFunc[g_EventSubWeaponActive.Get_pEventId()] = &CNtlSobCharProxy::SobSubWeaponActiveEventHandler;
	m_mapEventFunc[g_EventSubWeaponDeActive.Get_pEventId()] = &CNtlSobCharProxy::SobSubWeaponDeActiveEventHandler;
	m_mapEventFunc[g_EventSobConvertClass.Get_pEventId()] = &CNtlSobCharProxy::SobConvertClass;
    m_mapEventFunc[g_EventSobChangeAdult.Get_pEventId()] = &CNtlSobCharProxy::SobChangeAdult;
    m_mapEventFunc[g_EventAnimPostEffect.Get_pEventId()] = &CNtlSobCharProxy::SobPostEffectEventHandler;
    m_mapEventFunc[g_EventAnimColorChange.Get_pEventId()] = &CNtlSobCharProxy::SobColorChangeEventHandler;

    // 데코레이션에서 처리하는 이벤트들
    m_mapEventFunc[g_EventSobFainting.Get_pEventId()] =             &CNtlSobCharProxy::SobDecorationNotify;
    m_mapEventFunc[g_EventSobTargetSelect.Get_pEventId()] =         &CNtlSobCharProxy::SobDecorationNotify;
    m_mapEventFunc[g_EventSobAttackSelect.Get_pEventId()] =         &CNtlSobCharProxy::SobDecorationNotify;
    m_mapEventFunc[g_EventSobTargetSelectRelease.Get_pEventId()] =  &CNtlSobCharProxy::SobDecorationNotify;
    m_mapEventFunc[g_EventSobAttackMarkRelease.Get_pEventId()] =    &CNtlSobCharProxy::SobDecorationNotify;
    m_mapEventFunc[g_EventQuestMark.Get_pEventId()] =               &CNtlSobCharProxy::SobDecorationNotify;
    m_mapEventFunc[g_EventDirectionNfy.Get_pEventId()] =            &CNtlSobCharProxy::SobDecorationNotify;
    m_mapEventFunc[g_EventTMQCleintState.Get_pEventId()] =          &CNtlSobCharProxy::SobDecorationNotify;
    m_mapEventFunc[g_EventWorldChange.Get_pEventId()] =             &CNtlSobCharProxy::SobDecorationNotify;        
    m_mapEventFunc[g_EventShareTargetSelect.Get_pEventId()] =       &CNtlSobCharProxy::SobDecorationNotify;
    m_mapEventFunc[g_EventShareTargetRelease.Get_pEventId()] =      &CNtlSobCharProxy::SobDecorationNotify;
    m_mapEventFunc[g_EventTenkaichiMarking.Get_pEventId()] =        &CNtlSobCharProxy::SobDecorationNotify;

	m_mapEventFunc[g_EventSobEventTitleEffect.Get_pEventId()] = &CNtlSobCharProxy::SobDecorationNotify;

    m_mapEventFunc[g_EventSobTargetMarkSelect.Get_pEventId()] =     &CNtlSobCharProxy::SobDecorationNotify;
    m_mapEventFunc[g_EventSobTargetMarkRelease.Get_pEventId()] =    &CNtlSobCharProxy::SobDecorationNotify;
    m_mapEventFunc[g_EventTransform.Get_pEventId()] =               &CNtlSobCharProxy::SobDecorationNotify;
    m_mapEventFunc[g_EventMobTransform.Get_pEventId()] =            &CNtlSobCharProxy::SobDecorationNotify;
    m_mapEventFunc[g_EventTransformCandy.Get_pEventId()] =          &CNtlSobCharProxy::SobDecorationNotify;
    m_mapEventFunc[g_EventSobSkillAction.Get_pEventId()] =          &CNtlSobCharProxy::SobDecorationNotify;
    m_mapEventFunc[g_EventAnimHit.Get_pEventId()] =                 &CNtlSobCharProxy::SobDecorationNotify;

	m_mapEventFunc[g_EventCinematicScale.Get_pEventId()] = &CNtlSobCharProxy::SobDecorationNotify;
}

RwBool CNtlSobCharProxy::Create(RwUInt32 uiCompType)
{
	CNtlSobProxy::Create(uiCompType);

	return TRUE;
}

/*Human_Male*/
void CNtlSobCharProxy::Destroy(void)
{
    DetachColorChangeEffect();
	DeleteDecorationProxy();
	DeleteEquipProxy();
	DeletePLCharacter();
	
	DeleteUIPcStatusWndCharacter();
	DeleteUIPcStatusWndEquipPLItem();
	DeleteUIPcStatusWndCamera();

	DeleteUIPcDogiCharacter();
	DeleteUIPcDogiEquipPLItem();
	DeleteUIPcDogiCamera();

	NTL_DELETE(m_pUIPcStatusWnd);
	NTL_DELETE(m_pUIPcStatusRotate);

	NTL_DELETE(m_pUIPcDogiWnd);
	NTL_DELETE(m_pUIPcDogiRotate);

	if(m_pPrivateShopProxy)
	{
		NTL_DELETE(m_pPrivateShopProxy);
	}

	if(m_pPitchAngleProxy)
	{
		NTL_DELETE(m_pPitchAngleProxy);
	}

	CNtlSobProxy::Destroy();
}

void CNtlSobCharProxy::UpdateHalfOneSecTick(RwReal fElapsed)
{
	if(m_fCurrHalfOneSecTime >= 0.5f)
	{
		UpdateInkThicknessCheck(fElapsed);
		UpdateGUIUpdateCheck(fElapsed);

		m_fCurrHalfOneSecTime = 0.0f;
	}
}

void CNtlSobCharProxy::UpdateInkThicknessCheck(RwReal fElapsed)
{
	RwV3d vPos = m_pSobObj->GetPosition();
	RwMatrix *pMatrix = RwFrameGetMatrix( RwCameraGetFrame( CNtlPLGlobal::m_RwCamera ) );
	
	RwV3d vDelta;
	RwV3dSubMacro(&vDelta, RwMatrixGetPos( pMatrix ), &vPos);
	RwReal fLen = RwV3dLength(&vDelta);

	RwReal	fWeightInk = 1.0f;

	if(fLen < 20.0f)
	{
		fWeightInk = 1.0f;
	}
	else if(fLen < 100.0f)
	{
		fWeightInk = 1.0f - fLen/NTL_CHARACTER_EDGE_WEIGHT_DIST;
		if(fWeightInk < 0.5f)
			fWeightInk = 0.5f;
	}
	else
	{
		fWeightInk = 0.0f;
	}

	fWeightInk = m_fWeightInk * fWeightInk;

	RwReal fInkThinkness = fWeightInk * m_fInkThickness;
	m_pCharacter->SetInkThickness(fInkThinkness);
	
	m_fCurrHalfOneSecTime = 0.0f;
}

void CNtlSobCharProxy::UpdateDecoration(RwReal fElapsed)
{
	if(m_pEquipProxy)
		m_pEquipProxy->Update(fElapsed);

	if(m_pUIPcStatusWnd && m_pUIPcStatusWnd->pUIEquipProxy)
		m_pUIPcStatusWnd->pUIEquipProxy->Update(fElapsed);

	if(m_pUIPcStatusRotate && m_pUIPcStatusWnd)
	{
		if(m_pUIPcStatusRotate->bLeftRot)
		{
            m_pUIPcStatusRotate->fAngle -= 1.0f; 
		}

		if(m_pUIPcStatusRotate->bRightRot)
		{
            m_pUIPcStatusRotate->fAngle += 1.0f;
		}
	}

	if(m_pUIPcDogiWnd && m_pUIPcDogiWnd->pUIEquipProxy)
		m_pUIPcDogiWnd->pUIEquipProxy->Update(fElapsed);

	if(m_pUIPcDogiRotate && m_pUIPcDogiWnd)
	{
		if(m_pUIPcDogiRotate->bLeftRot)
		{
			m_pUIPcDogiRotate->fAngle -= 1.0f; 
		}

		if(m_pUIPcDogiRotate->bRightRot)
		{
			m_pUIPcDogiRotate->fAngle += 1.0f;
		}
	}

    if(m_pDecorationProxy)
        m_pDecorationProxy->Update(fElapsed);

	if(m_pPitchAngleProxy)
		m_pPitchAngleProxy->Update(fElapsed);
}

void CNtlSobCharProxy::UpdateResourceLoadCheck(RwReal fElapsed)
{
	if(m_pEquipProxy)
	{
		if(!m_pEquipProxy->IsResourceLoaded())
			return;
	}

	if(m_pCharacter)
	{
		if(!m_pCharacter->GetCharScheduleResInfo()->bLoadComplete)
			return;
	}
    else
    {
        return;
    }

	// character default item
	DEFAULT_ITEM_MAP_ITER it;
	CNtlPLEquipItem *pPLEquipItem = m_pCharacter->GetEquipItem();
	DEFAULT_ITEM_MAP *pPLDefaultMap = pPLEquipItem->GetDefaultItemList();
	
	if(pPLDefaultMap)
	{
		for(it = pPLDefaultMap->begin(); it != pPLDefaultMap->end(); it++)
		{
			if(!(*it).second->GetItemScheduleResInfo()->bLoadComplete)
				return;
		}
	}

	// resource load complete...
	RwBool bVisible = FALSE;

	if(IsProxySystemVisible())
	{
		bVisible = m_pSobObj->IsVisible();
		
		if(m_pPrivateShopProxy)
			bVisible = FALSE;
	}

	if(m_pEquipProxy)
		m_pEquipProxy->ResourceLoadComplete(bVisible);

	ResourceLoadComplete(bVisible);

	// 3D 이름과 2D 이름이 같이 보이는 것 때문에 수정 -by Kell
	if(m_pDecorationProxy)
		m_pDecorationProxy->ResourceLoadComplete(bVisible);

	m_bResLoaded = TRUE;
}

void CNtlSobCharProxy::UpdateGUIUpdateCheck(RwReal fElapsed)
{
	RwUInt32 uiClassID = m_pSobObj->GetClassID();
	if(uiClassID != SLCLASS_NPC)
		return;

	CNtlSobNpcAttr *pSobNpcAttr = reinterpret_cast<CNtlSobNpcAttr*>( m_pSobObj->GetSobAttr());
	sNPC_TBLDAT *pNpcTblData = pSobNpcAttr->GetNpcTbl();
	if(pNpcTblData == NULL)
		return;

	if( !(pNpcTblData->dwFunc_Bit_Flag & NPC_FUNC_FLAG_DISCLOSE_LP) )
		return;


	RwV3d vPos = m_pSobObj->GetPosition();
	const RwV3d *pCamPos = GetNtlGameCameraManager()->GetCameraPos();
	
	RwV3d vSub;
	RwV3dSubMacro(&vSub, &vPos, pCamPos);

	RwReal fLen = RwV3dLength(&vSub);

	if(fLen >= 40.0f)
	{
		if(m_uiSystemFlags & NTL_PROXYSYSTEM_LP_VISIBLE)
		{
			EnableProxySystemGUILPVisible(FALSE);
			CNtlSLEventGenerator::SobProxyGUIUpdate(m_pSobObj->GetSerialID(), NTL_PROXYSYSTEM_LP_VISIBLE, FALSE);
		}
	}
	else
	{
		if( !(m_uiSystemFlags & NTL_PROXYSYSTEM_LP_VISIBLE) )
		{
			EnableProxySystemGUILPVisible(TRUE);
			CNtlSLEventGenerator::SobProxyGUIUpdate(m_pSobObj->GetSerialID(), NTL_PROXYSYSTEM_LP_VISIBLE, TRUE);
		}
	}
}

void CNtlSobCharProxy::Update(RwReal fElapsed)
{
	m_fCurrHalfOneSecTime += fElapsed;

	UpdateHalfOneSecTick(fElapsed);

	UpdateDecoration(fElapsed);

	if(!m_bResLoaded)
	{
		UpdateResourceLoadCheck(fElapsed);
	}

	CNtlSobProxy::Update(fElapsed);
}

void CNtlSobCharProxy::HandleEvents(RWS::CMsg &pMsg)
{
	MapFuncEventHandler::iterator it = m_mapEventFunc.find(pMsg.Id);
	if(it != m_mapEventFunc.end())
	{
		(this->*(*it).second)(pMsg);
	}
}	

void CNtlSobCharProxy::ResourceLoadComplete(RwBool bVisible)
{
	m_pCharacter->SetVisible(bVisible);
}

void CNtlSobCharProxy::SobCreateEventHandler(RWS::CMsg &pMsg)
{
	m_pCharacter = CreatePLCharacter(FALSE);
	if(m_pSobObj->GetClassID() == SLCLASS_AVATAR)
	{
		SNtlEventSobAvatarCreate *pSobAvatarCreate = reinterpret_cast<SNtlEventSobAvatarCreate*>(pMsg.pData);
		if(pSobAvatarCreate->bUIAvatarCreate)
		{
			CreateUIPcStatusWndCharacter();
			CreateUIPcStatusWndCamera();

			CreateUIPcDogiCharacter();
			CreateUIPcDogiCamera();
		}
	}

	CreateDecorationProxy(pMsg);

	if(m_uiCompType & NTL_SLPROXY_COMP_EQUIP_ITEM)
	{
		CreateEquipProxy(pMsg);
		if(m_pSobObj->GetClassID() == SLCLASS_AVATAR)
		{
			SNtlEventSobAvatarCreate *pSobAvatarCreate = reinterpret_cast<SNtlEventSobAvatarCreate*>(pMsg.pData);
			if(pSobAvatarCreate->bUIAvatarCreate)
			{
				CreateUIPcStatusWndEquipPLItem();
				CreateUIPcDogiEquipPLItem();
			}
		}
	}

	if(m_pCharacter->IsApplyGroundPitchAngle())
	{
		m_pPitchAngleProxy = NTL_NEW CNtlSobCharPitchAngleProxy;
	}

	if ( m_pSobObj->GetClassID() == SLCLASS_NPC )
	{
		CNtlSobNpcAttr *pSobNpcAttr = reinterpret_cast<CNtlSobNpcAttr*>( m_pSobObj->GetSobAttr() );
		sNPC_TBLDAT *pNpcTblData = pSobNpcAttr->GetNpcTbl();
		if( pNpcTblData && (pNpcTblData->dwFunc_Bit_Flag & NPC_FUNC_FLAG_DISCLOSE_LP) )
		{
			EnableProxySystemGUILPVisible(TRUE);
			CNtlSLEventGenerator::SobProxyGUIUpdate(m_pSobObj->GetSerialID(), NTL_PROXYSYSTEM_LP_VISIBLE, TRUE);
			m_pCharacter->SetPickingOrder( NTL_PICK_ORDER_HIGH );
		}
	}
}

void CNtlSobCharProxy::SobEquipChangeEventHandler(RWS::CMsg &pMsg)
{
    // It keeps the original weapon information.
    if(m_pDecorationProxy && m_pDecorationProxy->GetProxyTransform())
        m_pDecorationProxy->GetProxyTransform()->HandleEvents(pMsg);
	
    SNtlEventSobEquipChange *pEquipChange = reinterpret_cast<SNtlEventSobEquipChange*>( pMsg.pData );

    // GM character exception handling
    if(m_pSobObj->GetClassID() == SLCLASS_PLAYER && Logic_IsCondition((CNtlSobActor*)m_pSobObj, CHARCOND_FLAG_TRANSPARENT))
        return;

    if(m_pUIPcStatusWnd && m_pUIPcStatusWnd->pUIEquipProxy)
        m_pUIPcStatusWnd->pUIEquipProxy->HandleEvents(pMsg);

	if(m_pUIPcDogiWnd && m_pUIPcDogiWnd->pUIEquipProxy)
		m_pUIPcDogiWnd->pUIEquipProxy->HandleEvents(pMsg);

    // Do not attach weapons if you are transforming.
    if(pEquipChange->bySlotIdx == EQUIP_SLOT_TYPE_HAND || pEquipChange->bySlotIdx == EQUIP_SLOT_TYPE_SUB_WEAPON)
    {
        if(Logic_IsTransformGreatNamek((CNtlSobActor*)m_pSobObj))        
            return;
    }

    if(m_pEquipProxy)
        m_pEquipProxy->HandleEvents(pMsg);
}

void CNtlSobCharProxy::SobGotFocusEventHandler(RWS::CMsg &pMsg)
{
	SetFocus();
}

void CNtlSobCharProxy::SobLostFocusEventHandler(RWS::CMsg &pMsg)
{
	ReleaseFocus();
}

void CNtlSobCharProxy::SobQuestAcceptProposalNfy(RWS::CMsg &pMsg)
{
	RwV3d vOffset;
	CNtlMath::MathRwV3dAssign(&vOffset, 0.0f, 0.0f, 0.0f);

	CreatePLChildEffect(NTL_VID_QUEST_ACCEPT_PROPOSAL, vOffset);

	SetBaseAnimation(NTL_ANI_QUEST_ACCEPT_PROPOSAL, FALSE);
}

void CNtlSobCharProxy::SobQuestAcceptRewardNfy(RWS::CMsg &pMsg)
{
	SNtlEventQuestAcceptReward_Nfy* pRewardInfo = reinterpret_cast< SNtlEventQuestAcceptReward_Nfy*> (pMsg.pData);

	RwV3d vOffset;
	CNtlMath::MathRwV3dAssign(&vOffset, 0.0f, 0.0f, 0.0f);

	CDboTSCQAgency* pQuestAgency = CNtlSLGlobal::GetInstance()->GetSobAvatar()->GetQuestAgency();
	if ( NULL == pQuestAgency ) return;

	CDboTSCQCtrl* pQCtrl = dynamic_cast< CDboTSCQCtrl* > ( pQuestAgency->FindProgressTrigger( pRewardInfo->sTSKey.tID ) );

	if ( pQCtrl )
	{
		char* pEffectName = NULL;
		switch ( pQCtrl->GetGradeType() )
		{
		case eGRADE_TYPE_EASY:
			pEffectName = NTL_VID_QUEST_ACCEPT_REWARD_01;
			break;
		case eGRADE_TYPE_NORMAL:
			pEffectName = NTL_VID_QUEST_ACCEPT_REWARD_02;
			break;
		case eGRADE_TYPE_HARD:
			pEffectName = NTL_VID_QUEST_ACCEPT_REWARD_03;
			break;
		case eGRADE_TYPE_VERY_HARD:
			pEffectName = NTL_VID_QUEST_ACCEPT_REWARD_04;
			break;
		}

		if ( pEffectName )
		{
			CreatePLChildEffect( pEffectName, vOffset, TRUE );
		}
	}

	SetBaseAnimation(NTL_ANI_QUEST_ACCEPT_REWARD, FALSE);
}

void CNtlSobCharProxy::SobSubWeaponActiveEventHandler(RWS::CMsg &pMsg)
{
	if(m_pEquipProxy)
		m_pEquipProxy->HandleEvents(pMsg);
}

void CNtlSobCharProxy::SobSubWeaponDeActiveEventHandler(RWS::CMsg &pMsg)
{
	if(m_pEquipProxy)
		m_pEquipProxy->HandleEvents(pMsg);
}

void CNtlSobCharProxy::SobConvertClass(RWS::CMsg &pMsg)
{
    // 이펙트를 표시한다.
    CNtlInstanceEffect* pEffect = (CNtlInstanceEffect*)GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_CLASS_CHANGE);
    if(pEffect)
    {
        m_pCharacter->AttachWorldPos(pEffect, ZeroAxis);
    }
}

void CNtlSobCharProxy::SobChangeAdult( RWS::CMsg &pMsg ) 
{
    // 스케쥴 로딩을 끈다.
	GetNtlResourceManager()->SetLoadScheduling(FALSE);

    // 변신 해제등을 위해서 캐릭터가 체인지 되기 전에 호출한다.
    if(m_pDecorationProxy)
    {
		m_pDecorationProxy->HandleEvents(pMsg);
    }

	m_pEquipProxy->DetachConvertClassEquipItem();    

    // character create	
    ChangeModel(CreatePLCharacter(FALSE), TRUE, TRUE);

	if(m_pUIPcStatusWnd)
    {
        m_pUIPcStatusWnd->pUIEquipProxy->DetachConvertClassEquipItem();
        DeleteUIPcStatusWndCamera();
        DeleteUIPcStatusWndCharacter();

        m_pUIPcStatusWnd->pPLCharacter = CreatePLCharacter(TRUE);
        m_pUIPcStatusWnd->pPLCharacter->SetVisible(FALSE);

        RwUInt32 uiPLFlags = m_pUIPcStatusWnd->pPLCharacter->GetFlags();
        m_pUIPcStatusWnd->pPLCharacter->SetFlags(uiPLFlags | NTL_PLEFLAG_DUMMY);

        RwV3d vPos;
        CNtlMath::MathRwV3dAssign(&vPos, 0.0f, 0.0f, 0.0f);
        m_pUIPcStatusWnd->pPLCharacter->SetPosition(&vPos);
        m_pUIPcStatusWnd->pPLCharacter->SetBaseAnimation(NML_IDLE_LOOP, 0.0f, TRUE);

        CreateUIPcStatusWndCamera();
        m_pUIPcStatusWnd->pUIEquipProxy->AttachConvertClassEquipItem(m_pUIPcStatusWnd->pPLCharacter);
    }

	if(m_pUIPcDogiWnd)
	{
		m_pUIPcDogiWnd->pUIEquipProxy->DetachConvertClassEquipItem();
		DeleteUIPcDogiCamera();
		DeleteUIPcDogiCharacter();

		m_pUIPcDogiWnd->pPLCharacter = CreatePLCharacter(TRUE);
		m_pUIPcDogiWnd->pPLCharacter->SetVisible(FALSE);

		RwUInt32 uiPLFlags = m_pUIPcDogiWnd->pPLCharacter->GetFlags();
		m_pUIPcDogiWnd->pPLCharacter->SetFlags(uiPLFlags | NTL_PLEFLAG_DUMMY);

		RwV3d vPos;
		CNtlMath::MathRwV3dAssign(&vPos, 0.0f, 0.0f, 0.0f);
		m_pUIPcDogiWnd->pPLCharacter->SetPosition(&vPos);
		m_pUIPcDogiWnd->pPLCharacter->SetBaseAnimation(NML_IDLE_LOOP, 0.0f, TRUE);

		CreateUIPcDogiCamera();
		m_pUIPcDogiWnd->pUIEquipProxy->AttachConvertClassEquipItem(m_pUIPcDogiWnd->pPLCharacter);
	}

	GetNtlResourceManager()->SetLoadScheduling(TRUE);

    // 이펙트를 표시한다.
	CNtlInstanceEffect* pEffect = (CNtlInstanceEffect*)GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_CLASS_CHANGE);
	if(pEffect)
	{
		m_pCharacter->AttachWorldPos(pEffect, ZeroAxis);
	}

	if(m_pDecorationProxy)
	{
		m_pDecorationProxy->SobChangeAdult(m_pCharacter);
	}
}

// 모델 포인터를 변경한다. 원래 모델을 삭제하지는 않는다. 
void CNtlSobCharProxy::ChangeModel(CNtlPLCharacter* pCharacter, RwBool bEquipItem, RwBool bDeleteOrgModel)
{
	if(m_pCharacter)
	{
		m_pDecorationProxy->DetachConvertClassEquipItem();
	}

    // 원래 모델 삭제 유무 
    if(bDeleteOrgModel)
    {
		GetSceneManager()->DeleteEntity(m_pCharacter);
    }

	m_pCharacter = pCharacter;
    
    RwV3d vPos = m_pSobObj->GetPosition();
    RwV3d vDir = m_pSobObj->GetDirection();
    SetPosition(&vPos);
    SetDirection(&vDir);

    if(m_pDecorationProxy)
    {
		m_pDecorationProxy->AttachConvertClassEquipItem(m_pCharacter);   
    }    

    if(bEquipItem)
    {
        // 아이템 장착
		m_pEquipProxy->AttachConvertClassEquipItem(m_pCharacter);        
    }    
}

void CNtlSobCharProxy::ChangeUIModel(CNtlPLCharacter* pCharacter)
{
	if ( NULL == pCharacter )
	{
		return;
	}

	if ( m_pUIPcStatusWnd )
	{
		m_pUIPcStatusWnd->pPLCharacter = pCharacter;

		RwV3d vLookAt = m_pUIPcStatusWnd->pPLCharacter->GetFaceCameraLookAt();
		RwV3d vCamPos = m_pUIPcStatusWnd->pPLCharacter->GetFaceCameraPos();

		m_pUIPcStatusBarCamera->SetCameraPosition(&vCamPos, &vLookAt);				///< Camera Transform
	}
}

void CNtlSobCharProxy::ChangeUIModel_Dogi(CNtlPLCharacter* pCharacter)
{
	if ( NULL == pCharacter )
	{
		return;
	}

	if ( m_pUIPcDogiWnd )
	{
		m_pUIPcDogiWnd->pPLCharacter = pCharacter;
	}
}

void CNtlSobCharProxy::NotifyStateEnter(RwUInt32 uiStateId)
{
	if(m_pEquipProxy)
		m_pEquipProxy->NotifyOwnerStateEnter(uiStateId);

	if(uiStateId == NTL_FSMSID_PRIVATESHOP)
	{
		EnableVisible(FALSE);
		m_pPrivateShopProxy = NTL_NEW CNtlSobCharPrivateShopProxy;
		m_pPrivateShopProxy->SetData(m_pSobObj, m_pCharacter);
	}
}

void CNtlSobCharProxy::NotifyStateExit(RwUInt32 uiOldState, RwUInt32 uiNewState)
{
	SetAnimSpeed(1.0f);

	if(m_pEquipProxy)
		m_pEquipProxy->NotifyOwnerStateExit(uiOldState, uiNewState);

	if(uiOldState == NTL_FSMSID_PRIVATESHOP)
	{
		EnableVisible(TRUE);
		if(m_pPrivateShopProxy)
		{
			NTL_DELETE(m_pPrivateShopProxy);
		}
	}
}

RwTexture* CNtlSobCharProxy::UIPcStatusWndRender(void)
{
	if(m_pUIPcStatusWnd == NULL)
		return NULL;

    RwFrame* pCameraFrame = RwCameraGetFrame(m_pUIPcStatusWnd->pUICamera->GetCamera());
    RwMatrix* pMatCam = RwFrameGetMatrix( pCameraFrame );    
    RwMatrix matOrg = *pMatCam;
    RwMatrixRotate(pMatCam, &YAxis, m_pUIPcStatusRotate->fAngle, rwCOMBINEPOSTCONCAT);    
    RwMatrixUpdate( pMatCam );
    RwFrameUpdateObjects( pCameraFrame );

	m_pUIPcStatusWnd->pUICamera->RenderTexture(m_pUIPcStatusWnd->pPLCharacter);

    *pMatCam = matOrg;
    RwMatrixUpdate(pMatCam);

	return m_pUIPcStatusWnd->pUICamera->GetTexture();
}

RwTexture* CNtlSobCharProxy::UIPcStatusBarRender(void)
{
	if(m_pUIPcStatusBarCamera == NULL)
		return NULL;

	m_pUIPcStatusBarCamera->RenderTexture(m_pUIPcStatusWnd->pPLCharacter);

	return m_pUIPcStatusBarCamera->GetTexture();
}

RwTexture* CNtlSobCharProxy::UIPcDogiWndRender(void)
{
	if(m_pUIPcDogiWnd == NULL)
		return NULL;

	RwFrame* pCameraFrame = RwCameraGetFrame(m_pUIPcDogiWnd->pUICamera->GetCamera());
	RwMatrix* pMatCam = RwFrameGetMatrix( pCameraFrame );
	RwMatrix matOrg = *pMatCam;
	RwMatrixRotate(pMatCam, &YAxis, m_pUIPcDogiRotate->fAngle, rwCOMBINEPOSTCONCAT);    
	RwMatrixUpdate( pMatCam );
	RwFrameUpdateObjects( pCameraFrame );

	m_pUIPcDogiWnd->pUICamera->RenderTexture(m_pUIPcDogiWnd->pPLCharacter);

	*pMatCam = matOrg;
	RwMatrixUpdate(pMatCam);

	return m_pUIPcDogiWnd->pUICamera->GetTexture();
}

void CNtlSobCharProxy::SetInkThickness(RwReal fValue /* = 1.2f */)
{
	m_fInkThickness = fValue;

	if(m_pCharacter)
	{
		m_pCharacter->SetInkThickness(fValue * m_fWeightInk);
	}
}

void CNtlSobCharProxy::SetInkColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue)
{
    if(!m_pCharacter)
        return;

    // 초사이야인 변신중이면 Ink Color는 흰색이다.
    if(byRed + byGreen + byBlue == 0 && 
       Logic_GetPlayerRace((CNtlSobActor*)m_pSobObj) == RACE_HUMAN && Logic_IsTransform((CNtlSobActor*)m_pSobObj))
    {
        byRed = byGreen = byBlue = 255;
    }
	
    m_pCharacter->SetInkColor(byRed, byGreen, byBlue);	
}

void CNtlSobCharProxy::SetInkThicknessWeight(RwReal fWeightValue) 
{
	m_fWeightInk = fWeightValue;

	if(m_pCharacter)
	{
		m_pCharacter->SetInkThickness(m_fInkThickness * m_fWeightInk);
	}
}

void CNtlSobCharProxy::EnableProxySystemVisible(RwBool bVisible)
{
	CNtlSobProxy::EnableProxySystemVisible(bVisible);

	if(bVisible)
	{
		RwBool bCurrVisible = m_pSobObj->IsVisible();
		if(m_pPrivateShopProxy)
			bCurrVisible = FALSE;
		
		EnableVisible(bCurrVisible);
	}
	else
	{
		EnableVisible(FALSE);
	}
}

void CNtlSobCharProxy::EnablePlayerNameVisible(RwBool bVisible)
{
	if(m_pDecorationProxy)
		m_pDecorationProxy->EnablePlayerNameVisible(bVisible);
}

RwBool CNtlSobCharProxy::GetActiveSubWeapon(void)
{
	if(m_pEquipProxy)
		return m_pEquipProxy->GetActiveSubWeapon();

	return FALSE;
}

CNtlPLEntity* CNtlSobCharProxy::CreatePLChildEffect(const RwChar *pKey, const RwChar *pAttachBoneName, RwV3d vOffset /* = ZeroAxis */, RwBool bIgnoreVisible /* = FALSE */)
{
	CNtlPLEntity *pPLEntity = CNtlSobProxy::CreatePLChildEffect(pKey, bIgnoreVisible);
	if(pPLEntity == NULL)
		return NULL;

	if(m_pCharacter == NULL || pAttachBoneName == NULL)
		return pPLEntity;

	Helper_AttachBone(m_pCharacter, pPLEntity, pAttachBoneName);

	return pPLEntity;
}

// offset position에 attach시키면서 effect 생성.
CNtlPLEntity* CNtlSobCharProxy::CreatePLChildEffect(const RwChar *pKey, RwV3d vOffset, RwBool bIgnoreVisible /* = FALSE */, RwBool bApplyRotate /* = FALSE */)
{
	CNtlPLEntity *pPLEntity = CNtlSobProxy::CreatePLChildEffect(pKey, bIgnoreVisible);
	if(pPLEntity == NULL)
		return NULL;

	if(m_pCharacter == NULL)
		return pPLEntity;

	Helper_AttachWorldPos(m_pCharacter, pPLEntity, vOffset, bApplyRotate);

	return pPLEntity;
}

void CNtlSobCharProxy::AddWorld(void)
{
	
	if(m_pCharacter)
		m_pCharacter->AddWorld();

	if(m_pEquipProxy)
		m_pEquipProxy->AddWorld();

	if(m_pDecorationProxy)
		m_pDecorationProxy->AddWorld();

	
	if(m_pUIPcStatusWnd)
	{
		if(m_pUIPcStatusWnd->pPLCharacter)
			m_pUIPcStatusWnd->pPLCharacter->AddWorld();

		if(m_pUIPcStatusWnd->pUIEquipProxy)
			m_pUIPcStatusWnd->pUIEquipProxy->AddWorld();
	}

	if(m_pUIPcDogiWnd)
	{
		if(m_pUIPcDogiWnd->pPLCharacter)
			m_pUIPcDogiWnd->pPLCharacter->AddWorld();

		if(m_pUIPcDogiWnd->pUIEquipProxy)
			m_pUIPcDogiWnd->pUIEquipProxy->AddWorld();
	}
}

void CNtlSobCharProxy::RemoveWorld(void)
{
	if(m_pDecorationProxy)
		m_pDecorationProxy->RemoveWorld();

	if(m_pEquipProxy)
		m_pEquipProxy->RemoveWorld();

	if(m_pCharacter)
		m_pCharacter->RemoveWorld();
	
	if(m_pUIPcStatusWnd)
	{
		if(m_pUIPcStatusWnd->pUIEquipProxy)
			m_pUIPcStatusWnd->pUIEquipProxy->RemoveWorld();

		if(m_pUIPcStatusWnd->pPLCharacter)
			m_pUIPcStatusWnd->pPLCharacter->RemoveWorld();
	}

	if(m_pUIPcDogiWnd)
	{
		if(m_pUIPcDogiWnd->pUIEquipProxy)
			m_pUIPcDogiWnd->pUIEquipProxy->RemoveWorld();

		if(m_pUIPcDogiWnd->pPLCharacter)
			m_pUIPcDogiWnd->pPLCharacter->RemoveWorld();
	}
}


void CNtlSobCharProxy::SetPosition(const RwV3d *pPos)
{
	if(m_pCharacter)
	{
		m_pCharacter->SetPosition(pPos);
		if(m_pPitchAngleProxy)
			m_pPitchAngleProxy->ReCalcAngle(m_pSobObj, m_pCharacter);
	}
}

void CNtlSobCharProxy::SetDirection(const RwV3d *pDir)
{
	if(m_pCharacter)
	{
		CNtlSobActor* pActor = dynamic_cast< CNtlSobActor* > ( m_pSobObj );

		if ( pActor && (Logic_IsBus( pActor ) || Logic_IsVehicleDriver( pActor ) || pActor->IsAirMode()) )
		{
			RwReal fXAngle, fYAngle;
			CNtlMath::LineToAngleXY( pDir, fXAngle, fYAngle );
			m_pCharacter->SetAngleX( fXAngle );
			m_pCharacter->SetAngleY( fYAngle );
		}
		else
		{
			RwReal fAngle = CNtlMath::LineToAngleY(pDir);
			m_pCharacter->SetAngleY(fAngle);

			if(m_pPitchAngleProxy)
				m_pPitchAngleProxy->ReCalcAngle(m_pSobObj, m_pCharacter);
		}
	}
}

void CNtlSobCharProxy::SetAngleY(RwReal fAngle)
{
	if(m_pCharacter == NULL)
		return;

	m_pCharacter->SetAngleY(fAngle);
	RwV3d vDir = m_pCharacter->GetDirection();
	m_pSobObj->SetDirection(&vDir);
}

void CNtlSobCharProxy::SetDeltaAngleY(RwReal fAngle)
{
	if(m_pCharacter == NULL)
		return;

	RwV3d vDir = m_pSobObj->GetDirection();
	RwReal fCurrAngle = CNtlMath::LineToAngleY(&vDir);
	fCurrAngle += fAngle;
	m_pCharacter->SetAngleY(fCurrAngle);

	vDir = m_pCharacter->GetDirection();
	m_pSobObj->SetDirection(&vDir);
}

void CNtlSobCharProxy::SetAngleX(RwReal fAngle)
{
	if (m_pCharacter == NULL)
		return;

	m_pCharacter->SetAngleX(fAngle);
	RwV3d vDir = m_pCharacter->GetDirection();
	m_pSobObj->SetDirection(&vDir);
}

void CNtlSobCharProxy::SetDeltaAngleX(RwReal fAngle)
{
	if (m_pCharacter == NULL)
		return;

	RwV3d vDir = m_pSobObj->GetDirection();
	RwReal fCurrAngle = CNtlMath::LineToAngleX(&vDir);
	fCurrAngle += fAngle;
	
	if (fabsf(fCurrAngle) < 60.f) // NTL_MAX_HEADING_ANGLE_X
	{
		m_pCharacter->SetAngleX(fCurrAngle);

		vDir = m_pCharacter->GetDirection();
		m_pSobObj->SetDirection(&vDir);
	}
}

void CNtlSobCharProxy::SetScale(RwReal fScale)
{
	if(m_pCharacter)
		m_pCharacter->SetScale(fScale);    

	if(m_pDecorationProxy)
		m_pDecorationProxy->SetScale(fScale);
}

void CNtlSobCharProxy::SetAlpha(RwUInt8 byAlpha)
{
	if(m_pCharacter)
		m_pCharacter->SetAlpha(byAlpha);

	if(m_pEquipProxy)
		m_pEquipProxy->SetAlpha(byAlpha);

	if(m_pDecorationProxy)
		m_pDecorationProxy->SetAlpha(byAlpha);

	CNtlSobProxy::SetAlpha(byAlpha);
}

void CNtlSobCharProxy::SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue)
{
	if(m_pCharacter)
		m_pCharacter->SetColor(byRed, byGreen, byBlue);

	if(m_pEquipProxy)
		m_pEquipProxy->SetColor(byRed, byGreen, byBlue);

	CNtlSobProxy::SetColor(byRed, byGreen, byBlue);
}

CNtlPLCharacter* CNtlSobCharProxy::CreatePLCharacter(RwBool bNotShading)
{
	NTL_FUNCTION("CNtlSobCharProxy::CreatePLCharacter");

	const RwChar *pModelKey = NULL;
	CNtlSobAttr *pSobAttr = m_pSobObj->GetSobAttr(); 

	RwUInt8 byClass = PC_CLASS_HUMAN_FIGHTER;
	RwUInt8 byRace = RACE_HUMAN;
	RwUInt8 byGender = GENDER_MALE;
	RwUInt8 byFace = DEF_ITEM_START;
	RwUInt8 byHair = DEF_ITEM_START;
	RwUInt8 byHairColor = DEF_ITEM_START;
	RwUInt8 bySkinColor = DEF_ITEM_START;
	RwBool bPcFlag = TRUE;
    RwBool bIsAdult = FALSE;

	CNtlSobActor *pSobActor = reinterpret_cast<CNtlSobActor*>( m_pSobObj );
	if(pSobActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_SOCIAL)
	{
		CNtlSobSocialAttr *pSobSocialAttr = reinterpret_cast<CNtlSobSocialAttr*>(pSobAttr);

		byClass		= pSobSocialAttr->GetClass();
		byRace		= pSobSocialAttr->GetRace();
		byGender	= pSobSocialAttr->GetGender();
		pModelKey	= pSobSocialAttr->GetModelName();
		byFace		= pSobSocialAttr->GetFace();
		byHair		= pSobSocialAttr->GetHair();
		byHairColor = pSobSocialAttr->GetHairColor();
		bySkinColor = pSobSocialAttr->GetSkinColor();
        bIsAdult    = pSobSocialAttr->IsAdult();
	}
	else
	{
		if(pSobActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_LIFE)
		{
			CNtlSobLifeAttr *pSobLifeAttr = reinterpret_cast<CNtlSobLifeAttr*>(pSobAttr);
			pModelKey = pSobLifeAttr->GetModelName();
		}

		bPcFlag = FALSE;
	}
		
	SPLCharacterCreateParam sParam;

	RwV3d vPos			= m_pSobObj->GetPosition();
	sParam.pPos			= &vPos;
	sParam.uiSerialId	= m_pSobObj->GetSerialID();
	sParam.bPcFlag		= bPcFlag;
	sParam.uiClass		= byClass;
	sParam.uiRace		= byRace;
	sParam.uiGender		= byGender;
	sParam.uiFaceType	= byFace;
	sParam.uiHeadType	= byHair;
	sParam.uiHeadColorType = byHairColor;
	sParam.uiSkinColorType = bySkinColor;
	sParam.bNotShadingFlag = bNotShading;
    sParam.bIsAdult        = bIsAdult;
    
//	DBO_WARNING_MESSAGE("CREATE MODEL: " << pModelKey);
	CNtlPLCharacter *pPLCharacter; 
	pPLCharacter = static_cast<CNtlPLCharacter*>(GetSceneManager()->CreateEntity(PLENTITY_CHARACTER, pModelKey, &sParam));
	NTL_ASSERT(pPLCharacter, "Character proxy is NULL  : " << pModelKey);
	pPLCharacter->SetSerialID(m_pSobObj->GetSerialID());

	// Monster일 경우 resize picking box를 flag를 setting 해 준다.
	if(m_pSobObj->GetClassID() == SLCLASS_MONSTER)
	{
		RwUInt32 uiFlags = pPLCharacter->GetFlags();
		pPLCharacter->SetFlags(uiFlags | NTL_PLEFLAG_RESIZE_PICKING_BBOX);
	}

	if(m_bResLoaded)
		pPLCharacter->SetVisible(TRUE);
	else
		pPLCharacter->SetVisible(FALSE);
	
	NTL_RETURN(pPLCharacter);
}

void CNtlSobCharProxy::DeletePLCharacter(void)
{
	// pl character destroy
	if(m_pCharacter)
	{
		GetSceneManager()->DeleteEntity(m_pCharacter);
		m_pCharacter = NULL;
	}
}


void CNtlSobCharProxy::CreateEquipProxy(RWS::CMsg &pMsg)
{
	m_pEquipProxy = NTL_NEW CNtlSobCharEquipProxy;
	m_pEquipProxy->SetData(m_pSobObj, m_pCharacter, FALSE);
	m_pEquipProxy->HandleEvents(pMsg);
}



void CNtlSobCharProxy::DeleteEquipProxy(void)
{
	if(m_pEquipProxy)
	{
		NTL_DELETE(m_pEquipProxy);
	}
}

void CNtlSobCharProxy::CreateDecorationProxy(RWS::CMsg &pMsg)
{	
	m_pDecorationProxy = NTL_NEW CNtlSobCharDecorationProxy;

	SNtlEventSobBaseCreate* pSobBaseCreate = reinterpret_cast<SNtlEventSobBaseCreate*>( pMsg.pData );
	if( pSobBaseCreate->uiClassId == SLCLASS_PLAYER )
	{
		SNtlEventSobPlayerCreate *pSobPlayerCreate = (SNtlEventSobPlayerCreate*)pMsg.pData;
		SetEmblemFactor(pSobPlayerCreate->pPcBrief->sMark.byMarkMain, 
						pSobPlayerCreate->pPcBrief->sMark.byMarkMainColor, 
						pSobPlayerCreate->pPcBrief->sMark.byMarkInLine, 
						pSobPlayerCreate->pPcBrief->sMark.byMarkInColor, 
						pSobPlayerCreate->pPcBrief->sMark.byMarkOutLine, 
						pSobPlayerCreate->pPcBrief->sMark.byMarkOutColor);
	}
	
	m_pDecorationProxy->SetData(m_pSobObj, m_pCharacter);
}


void CNtlSobCharProxy::DeleteDecorationProxy(void)
{
	if(m_pDecorationProxy)
	{
		NTL_DELETE(m_pDecorationProxy);
	}
}

void CNtlSobCharProxy::CreateUIPcStatusWndCharacter(void)
{
	m_pUIPcStatusWnd = NTL_NEW SUICharacter;
	m_pUIPcStatusWnd->pPLCharacter = CreatePLCharacter(TRUE);
	m_pUIPcStatusWnd->pPLCharacter->SetVisible(FALSE);

	RwUInt32 uiPLFlags = m_pUIPcStatusWnd->pPLCharacter->GetFlags();
	m_pUIPcStatusWnd->pPLCharacter->SetFlags(uiPLFlags | NTL_PLEFLAG_DUMMY);

	RwV3d vPos;
	CNtlMath::MathRwV3dAssign(&vPos, 0.0f, 0.0f, 0.0f);
	m_pUIPcStatusWnd->pPLCharacter->SetPosition(&vPos);
	m_pUIPcStatusWnd->pPLCharacter->SetBaseAnimation(NML_IDLE_LOOP, 0.0f, TRUE);

	m_pUIPcStatusRotate = NTL_NEW SUIPcStatusRotate;
	m_pUIPcStatusRotate->bLeftRot = FALSE;
	m_pUIPcStatusRotate->bRightRot = FALSE;
    m_pUIPcStatusRotate->fAngle = 0.0f;
}

void CNtlSobCharProxy::DeleteUIPcStatusWndCharacter(void)
{
	if(m_pUIPcStatusWnd && m_pUIPcStatusWnd->pPLCharacter)
	{
		GetSceneManager()->DeleteEntity(m_pUIPcStatusWnd->pPLCharacter);
		m_pUIPcStatusWnd->pPLCharacter = NULL;
	}
}

void CNtlSobCharProxy::CreateUIPcStatusWndEquipPLItem(void)
{
	m_pUIPcStatusWnd->pUIEquipProxy = NTL_NEW CNtlSobCharEquipProxy;
	m_pUIPcStatusWnd->pUIEquipProxy->SetData(m_pSobObj, m_pUIPcStatusWnd->pPLCharacter, TRUE);

	*m_pUIPcStatusWnd->pUIEquipProxy = *m_pEquipProxy;
}
	
void CNtlSobCharProxy::DeleteUIPcStatusWndEquipPLItem(void)
{
	if(m_pUIPcStatusWnd && m_pUIPcStatusWnd->pUIEquipProxy)
	{
		NTL_DELETE(m_pUIPcStatusWnd->pUIEquipProxy);
	}
}

void CNtlSobCharProxy::CreateUIPcStatusWndCamera(void)
{
	m_pUIPcStatusWnd->pUICamera = NTL_NEW CNtlPLCameraRenderTexture;
	m_pUIPcStatusWnd->pUICamera->Create(512, 512, 386, 512);

	RwReal fHeight = m_pUIPcStatusWnd->pPLCharacter->GetHeight();
	RwV3d vPos = m_pUIPcStatusWnd->pPLCharacter->GetPosition();
	vPos.y += fHeight*0.5f;

	RwV3d vCamPos, vLookAt;

	RwV3dAssignMacro(&vLookAt, &vPos);
	CNtlMath::MathRwV3dAssign(&vCamPos, vPos.x, vPos.y, vPos.z + fHeight);

	m_pUIPcStatusWnd->pUICamera->SetCameraPosition(&vCamPos, &vLookAt);				///< Camera Transform


	// pc status bar camera
	m_pUIPcStatusBarCamera = NTL_NEW CNtlPLCameraRenderTexture;
	m_pUIPcStatusBarCamera->Create(128, 128, 128, 128);
	vLookAt = m_pUIPcStatusWnd->pPLCharacter->GetFaceCameraLookAt();
	vCamPos = m_pUIPcStatusWnd->pPLCharacter->GetFaceCameraPos();
	m_pUIPcStatusBarCamera->SetCameraPosition(&vCamPos, &vLookAt);				///< Camera Transform
}

void CNtlSobCharProxy::DeleteUIPcStatusWndCamera(void)
{
	if(m_pUIPcStatusWnd && m_pUIPcStatusWnd->pUICamera)
	{
		m_pUIPcStatusWnd->pUICamera->Destroy();
		NTL_DELETE(m_pUIPcStatusWnd->pUICamera);
	}

	if(m_pUIPcStatusBarCamera)
	{
		m_pUIPcStatusBarCamera->Destroy();
		NTL_DELETE(m_pUIPcStatusBarCamera);
	}
}

void CNtlSobCharProxy::CreateUIPcDogiCharacter(void)
{
	m_pUIPcDogiWnd = NTL_NEW SUICharacter;
	m_pUIPcDogiWnd->pPLCharacter = CreatePLCharacter(TRUE);
	m_pUIPcDogiWnd->pPLCharacter->SetVisible(FALSE);

	RwUInt32 uiPLFlags = m_pUIPcDogiWnd->pPLCharacter->GetFlags();
	m_pUIPcDogiWnd->pPLCharacter->SetFlags(uiPLFlags | NTL_PLEFLAG_DUMMY);

	RwV3d vPos;
	CNtlMath::MathRwV3dAssign(&vPos, 0.0f, 0.0f, 0.0f);
	m_pUIPcDogiWnd->pPLCharacter->SetPosition(&vPos);
	m_pUIPcDogiWnd->pPLCharacter->SetBaseAnimation(NML_IDLE_LOOP, 0.0f, TRUE);

	m_pUIPcDogiRotate = NTL_NEW SUIPcStatusRotate;
	m_pUIPcDogiRotate->bLeftRot = FALSE;
	m_pUIPcDogiRotate->bRightRot = FALSE;
	m_pUIPcDogiRotate->fAngle = 0.0f;
}

void CNtlSobCharProxy::DeleteUIPcDogiCharacter(void)
{
	if(m_pUIPcDogiWnd && m_pUIPcDogiWnd->pPLCharacter)
	{
		GetSceneManager()->DeleteEntity(m_pUIPcDogiWnd->pPLCharacter);
		m_pUIPcDogiWnd->pPLCharacter = NULL;
	}
}

void CNtlSobCharProxy::CreateUIPcDogiEquipPLItem(void)
{
	m_pUIPcDogiWnd->pUIEquipProxy = NTL_NEW CNtlSobCharEquipProxy;
	m_pUIPcDogiWnd->pUIEquipProxy->SetData(m_pSobObj, m_pUIPcDogiWnd->pPLCharacter, TRUE);

	*m_pUIPcDogiWnd->pUIEquipProxy = *m_pEquipProxy;
}

void CNtlSobCharProxy::DeleteUIPcDogiEquipPLItem(void)
{
	if(m_pUIPcDogiWnd && m_pUIPcDogiWnd->pUIEquipProxy)
	{
		NTL_DELETE(m_pUIPcDogiWnd->pUIEquipProxy);
	}
}

void CNtlSobCharProxy::CreateUIPcDogiCamera(void)
{
	m_pUIPcDogiWnd->pUICamera = NTL_NEW CNtlPLCameraRenderTexture;
	m_pUIPcDogiWnd->pUICamera->Create(512, 512, 386, 512);

	RwReal fHeight = m_pUIPcDogiWnd->pPLCharacter->GetHeight();
	RwV3d vPos = m_pUIPcDogiWnd->pPLCharacter->GetPosition();
	vPos.y += fHeight * 0.5f;

	RwV3d vCamPos, vLookAt;

	RwV3dAssignMacro(&vLookAt, &vPos);
	CNtlMath::MathRwV3dAssign(&vCamPos, vPos.x, vPos.y, vPos.z + fHeight);

	m_pUIPcDogiWnd->pUICamera->SetCameraPosition(&vCamPos, &vLookAt);				///< Camera Transform
}

void CNtlSobCharProxy::DeleteUIPcDogiCamera(void)
{
	if(m_pUIPcDogiWnd && m_pUIPcDogiWnd->pUICamera)
	{
		m_pUIPcDogiWnd->pUICamera->Destroy();
		NTL_DELETE(m_pUIPcDogiWnd->pUICamera);
	}
}

CNtlPLEntity* CNtlSobCharProxy::GetPLMainEntity(void) 
{
	return m_pCharacter;
}

void CNtlSobCharProxy::SetBaseAnimation(RwUInt32 uiAnimKey, RwBool bLoop /*= TRUE*/, RwReal fStartTime /*= 0.0f*/)
{
	if(m_pCharacter == NULL)
		return;

	if(m_pUIPcStatusWnd)
	{
		if(uiAnimKey == NML_IDLE_LOOP || uiAnimKey == NML_STAFF_IDLE_LOOP || uiAnimKey == NML_DGUN_IDLE_LOOP)
		{
			RwUInt32 uiCurrTargetAnimKey = m_pUIPcStatusWnd->pPLCharacter->GetCurBaseAnimKey();
			if(uiAnimKey != uiCurrTargetAnimKey)
				m_pUIPcStatusWnd->pPLCharacter->SetBaseAnimation(uiAnimKey, 0.0f, TRUE);
		}
	}

	if(m_pUIPcDogiWnd)
	{
		if(uiAnimKey == NML_IDLE_LOOP || uiAnimKey == NML_STAFF_IDLE_LOOP || uiAnimKey == NML_DGUN_IDLE_LOOP)
		{
			RwUInt32 uiCurrTargetAnimKey = m_pUIPcDogiWnd->pPLCharacter->GetCurBaseAnimKey();
			if(uiAnimKey != uiCurrTargetAnimKey)
				m_pUIPcDogiWnd->pPLCharacter->SetBaseAnimation(uiAnimKey, 0.0f, TRUE);
		}
	}

	if(m_byAnimBlendNoneOneTime == 1)
	{
		m_byAnimBlendNoneOneTime = 2;
		m_pCharacter->SetBlend(BLEND_TWEEN, 0.0f, 0.0f);
	}
	else if(m_byAnimBlendNoneOneTime == 2)
	{
		m_byAnimBlendNoneOneTime = 0;
		m_pCharacter->SetBlend(BLEND_TWEEN);
	}

    if(m_bRemoveColorChangeEffect)
    {
        DetachColorChangeEffect();
        m_bRemoveColorChangeEffect = TRUE;
    }
	
	m_pCharacter->SetBaseAnimation(uiAnimKey, fStartTime, bLoop);
}

RwBool CNtlSobCharProxy::IsExistBaseAnimation(RwUInt32 uiAnimKey)
{
	if(m_pCharacter == NULL)
		return FALSE;

	return m_pCharacter->IsExistAnim(uiAnimKey); 
}


RwUInt32 CNtlSobCharProxy::GetBaseAnimationKey(void)
{
	if(m_pCharacter == NULL)
		return 0;

	return m_pCharacter->GetCurBaseAnimKey();
}


RwBool CNtlSobCharProxy::IsBaseAnimationEnd(void)
{
	if(m_pCharacter == NULL)
		return CNtlSobProxy::IsBaseAnimationEnd();

	return m_pCharacter->GetBaseCurrentAnimEnd();
}

void CNtlSobCharProxy::SetAnimSpeed(RwReal fAniSpeed)
{
	if(m_pCharacter == NULL)
		return;
		
	m_pCharacter->SetAnimSpeed(fAniSpeed);
}

RwReal CNtlSobCharProxy::GetAnimSpeed() 
{
    if(!m_pCharacter)
        return 1.0f;

    return m_pCharacter->GetAnimSpeed();
}

void CNtlSobCharProxy::SetAnimBlendNoneOneTime(void)
{
	m_byAnimBlendNoneOneTime = 1;
}


RwV3d CNtlSobCharProxy::GetBonePosition(const RwChar *pBoneName)
{
	RwMatrix *pMat = m_pCharacter->GetBoneMatrix(pBoneName);
	if(pMat == NULL)
		return CNtlSobProxy::GetBonePosition(pBoneName);

	return pMat->pos;
}

RwV3d CNtlSobCharProxy::GetWeaponBonePosition(const RwChar *pBoneName)
{
	if(m_pEquipProxy == NULL)
		return CNtlSobProxy::GetWeaponBonePosition(pBoneName);

	return m_pEquipProxy->GetWeaponBonePosition(pBoneName);
}

RwV3d CNtlSobCharProxy::GetSubWeaponBonePosition(const RwChar *pBoneName)
{
	if(m_pEquipProxy == NULL)
		return CNtlSobProxy::GetSubWeaponBonePosition(pBoneName);

	return m_pEquipProxy->GetSubWeaponBonePosition(pBoneName);
}

// pl entity의 높이를 얻어온다.
RwReal CNtlSobCharProxy::GetPLEntityHeight(void)
{
	if(m_pPrivateShopProxy)
		return m_pPrivateShopProxy->GetHeight();

	return m_pCharacter->GetHeight();
}

// pl entity의 폭을 얻어온다.
RwReal CNtlSobCharProxy::GetPLEntityWidth(void)
{
	return m_pCharacter->GetWidth();
}

// pl entity의 깊이을 얻어온다.
RwReal CNtlSobCharProxy::GetPLEntityDepth(void)
{
	return m_pCharacter->GetDepth();
}

RwReal CNtlSobCharProxy::GetPLEntityBaseScale(void)
{
	return m_pCharacter->GetBaseScale();
}


void CNtlSobCharProxy::EnableVisible(RwBool bEnable)
{
    // 투명상태 처리
    if(bEnable && Logic_IsCondition((CNtlSobActor*)m_pSobObj, CHARCOND_FLAG_TRANSPARENT))
    {
        if(m_pSobObj->GetClassID() == SLCLASS_PLAYER)
        {
            bEnable = FALSE;            
        }        
    }

	if(IsProxySystemVisible())
	{
		// 캐릭터는 로딩되지 않아도 그림자는 로딩되기 깨문에, 캐릭터와 별개로 처리한다. (by agebreak)
		if(m_pDecorationProxy)
			m_pDecorationProxy->SetVisible(bEnable);

		if(!m_bResLoaded)
			return;

		CNtlSobProxy::EnableVisible(bEnable);

		if(m_pCharacter)
			m_pCharacter->SetVisible(bEnable);

		if(m_pEquipProxy)
			m_pEquipProxy->SetVisible(bEnable);
	}
	else
	{
		if(m_pDecorationProxy)
			m_pDecorationProxy->SetVisible(FALSE);

		CNtlSobProxy::EnableVisible(FALSE);

		if(m_pCharacter)
			m_pCharacter->SetVisible(FALSE);

		if(m_pEquipProxy)
			m_pEquipProxy->SetVisible(FALSE);
	}
}


void CNtlSobCharProxy::EnablePicking(RwBool bEnable)
{
	if(m_pCharacter)
		m_pCharacter->SetPicking(bEnable);

	if(m_pEquipProxy)
		m_pEquipProxy->SetPicking(bEnable);
}

void CNtlSobCharProxy::EnableShadowRendering(RwBool bEnable)
{
	if(m_pDecorationProxy)
		m_pDecorationProxy->SetShadowOnOff(bEnable);
}

void CNtlSobCharProxy::EnableHighlight(RwBool bEnable)
{
	m_bEnableHighlight = bEnable;

	if(!m_bEnableHighlight)
	{
		if(m_pCharacter)
		{
			m_pCharacter->SetAddColor(0, 0, 0);
		}

		if(m_pFocusColorEffect)
		{
			RemoveVisualSystemEffectColor(m_pFocusColorEffect);
			m_pFocusColorEffect = NULL;
		}
	}
}

void CNtlSobCharProxy::CreateElapsedController(RwReal fLifeTime, RwReal fWeightValue)
{
	CNtlSobProxy::CreateElapsedController(fLifeTime, fWeightValue);

	if(m_pCharacter)
		m_pCharacter->CreateWeightElapsedController(fLifeTime, fWeightValue);

	if(m_pDecorationProxy)
		m_pDecorationProxy->CreateWeightElapsedController(fLifeTime, fWeightValue);

	/*
	// 일단은 attach된 entity만 control 조정한다.
	// 여기는 attach된 entity외에 attach 되지 않으면서, character 가 생성한 entity도 찾을 수 있는 방법인데,
	// attach된 entity와 겹칠 수 있다... 또한 모든 effect를 검색하므로 속도가 저하될 수 있다. (형석)
	// 기존에 출력된 effect에 elapsed control setting
	if(GetNtlSobElapsedControlManager()->GetControlFactor(m_pSobObj, fLifeTime, fWeightValue))
	{
		SERIAL_HANDLE hSerialId = m_pSobObj->GetSerialID();
		CNtlPLVisualManager *pVisualManager = reinterpret_cast<CNtlPLVisualManager*>( GetSceneManager() );
		CNtlPLRenderGroup *pGroup = pVisualManager->FindInstanceGroup(PLENTITY_EFFECT);
		if(pGroup)
		{
			CNtlPLEntity *pPLEffect;
			CNtlPLRenderGroup::MapEntity::iterator it;
			CNtlPLRenderGroup::MapEntity *pMapEntities = pGroup->GetEntities();

			for(it = pMapEntities->begin(); it != pMapEntities->end(); ++it)
			{
				pPLEffect = (*it).second;
				if(pPLEffect->GetSerialID() == hSerialId)
				{
					pPLEffect->CreateWeightElapsedController(fLifeTime, fWeightValue);
				}
			}
		}
	}
	*/
}


void CNtlSobCharProxy::DeleteElapsedController(void)
{
	CNtlSobProxy::DeleteElapsedController();

	if(m_pCharacter)
		m_pCharacter->DeleteWeightElapsedController();

	if(m_pDecorationProxy)
		m_pDecorationProxy->DeleteWeightElapsedController();
}


// mouse focus를 받았을 경우.
void CNtlSobCharProxy::SetFocus(void) 
{
	// input enable 켜 있지 않으면?
	if(!m_pSobObj->IsInput())
		return;

	if(m_pCharacter)
	{
		if(m_bEnableHighlight && !m_pFocusColorEffect)
            m_pFocusColorEffect = AddVisualSystemEffectColor(40, 40, 40, TRUE);			
	}
}

// mouse focus를 읽어 버렸을 경우.
void CNtlSobCharProxy::ReleaseFocus(void) 
{
	// input enable 켜 있지 않으면?
	if(!m_pSobObj->IsInput())
		return;

	if(m_pCharacter)
	{
		if(m_pFocusColorEffect)
        {
            RemoveVisualSystemEffectColor(m_pFocusColorEffect);
            m_pFocusColorEffect = NULL;
        }
	}
}

void CNtlSobCharProxy::SetNameColor(const WCHAR* pwcName, COLORREF nameColor,
									const WCHAR* pwcGuildName, COLORREF guildColor, 
									const WCHAR* pwcTitleName, COLORREF titlecolor)
{
	m_pDecorationProxy->SetNameColor(pwcName, nameColor, pwcGuildName, guildColor, pwcTitleName, titlecolor);
}

void CNtlSobCharProxy::SetNickNameColor(const WCHAR* pwcNickName, COLORREF nickNameColor)
{
	m_pDecorationProxy->SetNickNameColor( pwcNickName, nickNameColor );
}

void CNtlSobCharProxy::SetEmblemFactor(RwUInt8 byTypeA, RwUInt8 byTypeAColor,	
										RwUInt8 byTypeB, RwUInt8 byTypeBColor, 
										RwUInt8 byTypeC, RwUInt8 byTypeCColor)
{
	if(m_pDecorationProxy)
		m_pDecorationProxy->SetEmblemFactor(byTypeA, byTypeAColor, byTypeB,	byTypeBColor, byTypeC, byTypeCColor);

	if(m_pEquipProxy)
		m_pEquipProxy->SetEmblemFactor(byTypeA, byTypeAColor, byTypeB, byTypeBColor, byTypeC, byTypeCColor);

	if(m_pUIPcStatusWnd && m_pUIPcStatusWnd->pUIEquipProxy)
		m_pUIPcStatusWnd->pUIEquipProxy->SetEmblemFactor(byTypeA, byTypeAColor, byTypeB, byTypeBColor, byTypeC, byTypeCColor);

	if(m_pUIPcDogiWnd && m_pUIPcDogiWnd->pUIEquipProxy)
		m_pUIPcDogiWnd->pUIEquipProxy->SetEmblemFactor(byTypeA, byTypeAColor, byTypeB, byTypeBColor, byTypeC, byTypeCColor);
}

void CNtlSobCharProxy::SetDecorationProxyAlpha(RwUInt8 byAlpha)
{
	if(m_pDecorationProxy)
	{
		m_pDecorationProxy->SetAlpha(byAlpha);
	}
}

void CNtlSobCharProxy::EnableDecorationProxyVisible(RwBool bShow)
{
	if(m_pDecorationProxy)
	{
		m_pDecorationProxy->SetVisible(bShow);
	}
}

void CNtlSobCharProxy::PcStatusRotateLeft(void)
{
	if(m_pUIPcStatusRotate == NULL)
		return;

	m_pUIPcStatusRotate->bLeftRot	= TRUE;
	m_pUIPcStatusRotate->bRightRot	= FALSE;
}

void CNtlSobCharProxy::PcStatusRotateRight(void)
{
	if(m_pUIPcStatusRotate == NULL)
		return;

	m_pUIPcStatusRotate->bLeftRot	= FALSE;
	m_pUIPcStatusRotate->bRightRot	= TRUE;
}

void CNtlSobCharProxy::PcStatusRotateStop(void)
{
	if(m_pUIPcStatusRotate == NULL)
		return;

	m_pUIPcStatusRotate->bLeftRot	= FALSE;
	m_pUIPcStatusRotate->bRightRot	= FALSE;
}

void CNtlSobCharProxy::PcStatusRotateReset(void)
{
	if(m_pUIPcStatusRotate == NULL)
		return;

	m_pUIPcStatusRotate->bLeftRot	= FALSE;
	m_pUIPcStatusRotate->bRightRot	= FALSE;
    m_pUIPcStatusRotate->fAngle     = 0.0f;
}

void CNtlSobCharProxy::PcStatusRotate(RwReal fDelta)
{
	if( m_pUIPcStatusRotate == NULL )
		return;
    
    m_pUIPcStatusRotate->fAngle -= fDelta;
}

void CNtlSobCharProxy::PcDogiRotateLeft(void)
{
	if(m_pUIPcDogiRotate == NULL)
		return;

	m_pUIPcDogiRotate->bLeftRot		= TRUE;
	m_pUIPcDogiRotate->bRightRot	= FALSE;
}

void CNtlSobCharProxy::PcDogiRotateRight(void)
{
	if(m_pUIPcDogiRotate == NULL)
		return;

	m_pUIPcDogiRotate->bLeftRot		= FALSE;
	m_pUIPcDogiRotate->bRightRot	= TRUE;
}

void CNtlSobCharProxy::PcDogiRotateStop(void)
{
	if(m_pUIPcDogiRotate == NULL)
		return;

	m_pUIPcDogiRotate->bLeftRot		= FALSE;
	m_pUIPcDogiRotate->bRightRot	= FALSE;
}

void CNtlSobCharProxy::PcDogiRotateReset(void)
{
	if(m_pUIPcDogiRotate == NULL)
		return;

	m_pUIPcDogiRotate->bLeftRot		= FALSE;
	m_pUIPcDogiRotate->bRightRot	= FALSE;
	m_pUIPcDogiRotate->fAngle		= 0.f;
}

void CNtlSobCharProxy::PcDogiRotate(RwReal fDelta)
{
	if( m_pUIPcDogiRotate == NULL )
		return;

	m_pUIPcDogiRotate->fAngle -= fDelta;
}

void CNtlSobCharProxy::SobDecorationNotify(RWS::CMsg &pMsg)
{
    if(!m_pDecorationProxy)
        return;

    m_pDecorationProxy->HandleEvents(pMsg);
}

RwReal CNtlSobCharProxy::GetScale() 
{
    if(!m_pCharacter)
        return 1.0f;

    return m_pCharacter->GetScale();
}

void CNtlSobCharProxy::AttachRPBonusEffect() 
{
    // 이펙트 생성
    m_pDecorationProxy->AttachRPBonusEffect();
}

void CNtlSobCharProxy::DetachRPBonusEffect() 
{
    // 이펙트 해제
    m_pDecorationProxy->DetachRPBonusEffect();    
}

void CNtlSobCharProxy::CreateGuardEffect()
{
	//m_pDecorationProxy->CreateGuardEffect();
	RwUInt8 mGrade = m_pEquipProxy->GetEquipItem(EQUIP_SLOT_TYPE_JACKET)->byGrade; //get's jacket grade

	switch (mGrade)
	{
		case (0): 
		{
			m_pDecorationProxy->CreateGuardEffect(NTL_VID_GUARD_SUCCESS);
			break;
		}
		case (1):
		{
			m_pDecorationProxy->CreateGuardEffect(NTL_VID_GUARD_BLUE);
			break;
		}
		case (2):
		{
			m_pDecorationProxy->CreateGuardEffect(NTL_VID_GUARD_BLUE);
			break;
		}
		case (3):
		{
			m_pDecorationProxy->CreateGuardEffect(NTL_VID_GUARD_BLUE);
			break;
		}
		case (4):
		{
			m_pDecorationProxy->CreateGuardEffect(NTL_VID_GUARD_BLUE);
			break;
		}
		case (5):
		{
			m_pDecorationProxy->CreateGuardEffect(NTL_VID_GUARD_GREEN);
			break;
		}
		case (6):
		{
			m_pDecorationProxy->CreateGuardEffect(NTL_VID_GUARD_GREEN);
			break;
		}
		case (7):
		{
			m_pDecorationProxy->CreateGuardEffect(NTL_VID_GUARD_GREENT);
			break;
		}
		case (8):
		{
			m_pDecorationProxy->CreateGuardEffect(NTL_VID_GUARD_GREENT);
			break;
		}
		case (9):
		{
			m_pDecorationProxy->CreateGuardEffect(NTL_VID_GUARD_YELLOW);
			break;
		}
		case (10):
		{
			m_pDecorationProxy->CreateGuardEffect(NTL_VID_GUARD_YELLOW);
			break;
		}
		case (11):
		{
			m_pDecorationProxy->CreateGuardEffect(NTL_VID_GUARD_YELLOWT);
			break;
		}
		case (12):
		{
			m_pDecorationProxy->CreateGuardEffect(NTL_VID_GUARD_YELLOWT);
			break;
		}
		case (13):
		{
			m_pDecorationProxy->CreateGuardEffect(NTL_VID_GUARD_RED);
			break;
		}
		case (14):
		{
			m_pDecorationProxy->CreateGuardEffect(NTL_VID_GUARD_REDT);
			break;
		}
		case (15):
		{
			m_pDecorationProxy->CreateGuardEffect(NTL_VID_GUARD_PURPLE);
			break;
		}
	}

}

void CNtlSobCharProxy::DeleteGuardEffect()
{
	m_pDecorationProxy->DeleteGuardEffect();
}

void CNtlSobCharProxy::CreateRpChargeEffect()
{
	RwUInt8 mGrade = m_pEquipProxy->GetEquipItem(EQUIP_SLOT_TYPE_PANTS)->byGrade; //get's pants grade


	switch (mGrade) 
	{
		case (0): //if no upgrades then it's a normal aura
		{
			m_pDecorationProxy->CreateRpChargeEffect(NTL_VID_RP_CHARGE);
			break;
		}
		case (1): //if upgraded successfully once then it has a blue aura 
		{
			m_pDecorationProxy->CreateRpChargeEffect(NTL_VID_RP_CHARGE_BLUE);
			break;
		}
		case (2):  //if upgraded successfully twice then it still has a blue aura 
		{
			m_pDecorationProxy->CreateRpChargeEffect(NTL_VID_RP_CHARGE_BLUE);
			break;
		}
		case (3): //if upgraded successfully three times then it still has a blue aura
		{
			m_pDecorationProxy->CreateRpChargeEffect(NTL_VID_RP_CHARGE_BLUE);
			break;
		}
		case (4): //if upgraded successfully four times then it still has a blue aura
		{
			m_pDecorationProxy->CreateRpChargeEffect(NTL_VID_RP_CHARGE_BLUE);
			break;
		}
		case (5): //if upgraded successfully 5 times then it has a green aura
		{
			m_pDecorationProxy->CreateRpChargeEffect(NTL_VID_RP_CHARGE_GREEN);
			break;
		}
		case (6): //if upgraded successfully 6 times then it has a green aura
		{
			m_pDecorationProxy->CreateRpChargeEffect(NTL_VID_RP_CHARGE_GREEN);
			break;
		}
		case (7): //if upgraded successfully 7 times then it has a refined green aura
		{
			m_pDecorationProxy->CreateRpChargeEffect(NTL_VID_RP_CHARGE_GREENT);
			break;
		}
		case (8): //if upgraded successfully 8 times then it has a refined green aura
		{
			m_pDecorationProxy->CreateRpChargeEffect(NTL_VID_RP_CHARGE_GREENT);
			break;
		}
		case (9): //if upgraded successfully 9 times then it has a yellow aura
		{
			m_pDecorationProxy->CreateRpChargeEffect(NTL_VID_RP_CHARGE_YELLOW);
			break;
		}
		case (10): //if upgraded successfully 10 times then it has a yellow aura
		{
			m_pDecorationProxy->CreateRpChargeEffect(NTL_VID_RP_CHARGE_YELLOW);
			break;
		}
		case (11): //if upgraded successfully 11 times then it has a refined yellow aura
		{
			m_pDecorationProxy->CreateRpChargeEffect(NTL_VID_RP_CHARGE_YELLOWT);
			break;
		}
		case (12): //if upgraded successfully 12 times then it has a refined yellow aura
		{
			m_pDecorationProxy->CreateRpChargeEffect(NTL_VID_RP_CHARGE_YELLOWT);
			break;
		}
		case (13): //if upgraded successfully 13 times then it has a red aura
		{
			m_pDecorationProxy->CreateRpChargeEffect(NTL_VID_RP_CHARGE_RED);
			break;
		}
		case (14): //if upgraded successfully 14 times then it has a refined red aura
		{
			m_pDecorationProxy->CreateRpChargeEffect(NTL_VID_RP_CHARGE_REDT);
			break;
		}
		case (15): //if upgraded successfully 15 times then it has a purple aura
		{
			m_pDecorationProxy->CreateRpChargeEffect(NTL_VID_RP_CHARGE_PURPLE);
			break;
		}

	}

}

void CNtlSobCharProxy::DeleteRpChargeEffect()
{
	m_pDecorationProxy->DeleteRpChargeEffect();
}

void CNtlSobCharProxy::SobPostEffectEventHandler( RWS::CMsg& pMsg ) 
{
    SNtlEventPostEffect* pData = (SNtlEventPostEffect*)pMsg.pData;    
    SEventPostEffect* pEventPostEffect = (SEventPostEffect*)pData->pData;
    CNtlInstanceEffect* pPLPostEffect = NULL;

    if(pEventPostEffect->eTarget == POST_EFFECT_TARGET_TYPE_SELF)
    {
        if(strlen(pEventPostEffect->szPCBoneName))
        {
            pPLPostEffect = (CNtlInstanceEffect*)CreatePLChildEffect(pEventPostEffect->szPostEffectName, pEventPostEffect->szPCBoneName, pEventPostEffect->v3dOffset);
        }        
        else
        {
            pPLPostEffect = (CNtlInstanceEffect*)CreatePLChildEffect(pEventPostEffect->szPostEffectName, pEventPostEffect->v3dOffset);                        
        }		
    }
    else if(pEventPostEffect->eTarget == POST_EFFECT_TARGET_TYPE_TARGET)
    {
        // 타겟이 대상인 경우에는 타겟에 이펙트 생성
        CNtlSob* pSobTarget = GetNtlSobManager()->GetSobObject(Logic_GetActorTargetSerialId((CNtlSobActor*)m_pSobObj));
        if(!pSobTarget)
            return;

        RwV3d vTargetOffset = pEventPostEffect->v3dOffset;
        vTargetOffset.y += GetPLEntityHeight() * pEventPostEffect->fTargetHeight;
        pSobTarget->GetSobProxy()->CreatePLChildEffect(pEventPostEffect->szPostEffectName, vTargetOffset);
    }

    if(pPLPostEffect)
    {
        // Center Fix Enable
        pPLPostEffect->SetPostEffect_CenterFixEnable(pEventPostEffect->bCenterFixEnable);
    }    
}

void CNtlSobCharProxy::SobColorChangeEventHandler(RWS::CMsg& pMsg)
{
    SNtlEventColorChange* pData = (SNtlEventColorChange*)pMsg.pData;
    SEventColorChange* pEvent = (SEventColorChange*)pData->pData;
    
    DetachColorChangeEffect();

    switch(pEvent->eType)
    {
    case COLOR_CHANGE_TYPE_START:        
        SetInkColor(pEvent->colorEdge.red, pEvent->colorEdge.green, pEvent->colorEdge.blue);
        m_ppColorChangeEffect[0] = AddVisualSystemEffectColor(pEvent->colorBody.red, pEvent->colorBody.green, pEvent->colorBody.blue);
        m_ppColorChangeEffect[1] = AddVisualSystemEffectColor(pEvent->colorAdd.red, pEvent->colorAdd.green, pEvent->colorAdd.blue, TRUE);
        m_bRemoveColorChangeEffect = FALSE;
        break;
    case COLOR_CHANGE_TYPE_END:        
        break;
    case COLOR_CHANGE_TYPE_ANIM:
        SetInkColor(pEvent->colorEdge.red, pEvent->colorEdge.green, pEvent->colorEdge.blue);
        m_ppColorChangeEffect[0] = AddVisualSystemEffectColor(pEvent->colorBody.red, pEvent->colorBody.green, pEvent->colorBody.blue);
        m_ppColorChangeEffect[1] = AddVisualSystemEffectColor(pEvent->colorAdd.red, pEvent->colorAdd.green, pEvent->colorAdd.blue, TRUE);
        m_bRemoveColorChangeEffect = TRUE;
        break;
    }
}

SEquipItem* CNtlSobCharProxy::GetEquipItem( RwUInt8 bySlotIdx ) 
{
    return m_pEquipProxy->GetEquipItem(bySlotIdx);
}

void CNtlSobCharProxy::CreateEquipItem( RwUInt32 uiTblId, RwUInt8 bySlotIdx, RwUInt8 byGrade ) 
{
    if(m_pEquipProxy)
        m_pEquipProxy->CreateEquipItem(uiTblId, bySlotIdx, byGrade);
}

void CNtlSobCharProxy::DeleteEquipItem( RwUInt8 bySlotIdx ) 
{
    if(m_pEquipProxy)
        m_pEquipProxy->DeleteEquipItem(bySlotIdx);
}

void CNtlSobCharProxy::SetVisibleEquipItem(RwBool bVisible, RwUInt8 bySlotIdx)
{
    if(m_pEquipProxy)
        m_pEquipProxy->SetVisible(bVisible, bySlotIdx);
}

void CNtlSobCharProxy::DetachColorChangeEffect()
{
    SetInkColor(0, 0, 0);

    if(m_ppColorChangeEffect[0])
        RemoveVisualSystemEffectColor(m_ppColorChangeEffect[0]);        
    if(m_ppColorChangeEffect[1])
        RemoveVisualSystemEffectColor(m_ppColorChangeEffect[1]);

    m_ppColorChangeEffect[0] = NULL;
    m_ppColorChangeEffect[1] = NULL;
    UpdateSystemEffectColor(0.0f);
}

void CNtlSobCharProxy::SetStoneMaterial( RwBool bApply ) 
{
    if(!m_pCharacter)
        return;
    
    m_pCharacter->SetPetrifyMaterial(bApply);
}

void CNtlSobCharProxy::SetSkillCancel() 
{
    if(!m_pCharacter)
        return;

    m_pCharacter->OnAfterAnimEventRun();
}