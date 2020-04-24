#include "precomp_ntlsimulation.h"
#include "NtlSobCharProxyEquip.h"
#include "NtlSobCharProxy.h"

// shared
#include "ItemTable.h"
#include "MobTable.h"
#include "TableContainer.h"
#include "GraphicDataTable.h"

// core
#include "NtlDebug.h"
#include "NtlMath.h"

// presentation
#include "NtlPLSceneManager.h"
#include "NtlPLVisualManager.h"
#include "NtlPLCharacter.h"
#include "NtlPLItem.h"
#include "NtlPLDecal.h"
#include "NtlPLPlayerName.h"
#include "NtlPLHelpers.h"
#include "NtlPLRenderGroup.h"
#include "NTLPLCameraRenderTexture.h"
#include "NtlPLEvent.h"
#include "LinkItemTrasformList.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLEvent.h"
#include "NtlSob.h"
#include "NtlSobAvatar.h"
#include "NtlSobItem.h"
#include "NtlSobNpcAttr.h"
#include "NtlSobMonsterAttr.h"
#include "NtlSobPlayerAttr.h"
#include "NtlSobAvatarAttr.h"
#include "NtlSobItemAttr.h"
#include "NtlSobManager.h"
#include "NtlSLVisualDeclear.h"
#include "NtlInventory.h"
#include "ntlSobElapsedController.h"
#include "NtlSLLogic.h"
#include "NtlSLApi.h"
#include "NtlFSMDef.h"



CNtlSubWeaponController::CNtlSubWeaponController()
{
    m_pActor                = NULL;
	m_pPLCharacter			= NULL;
	m_pPLItem				= NULL;
	m_pMatSubWeaponBone		= NULL;
	m_bResLoaded			= FALSE;
	m_bResLoadComplete		= FALSE;
	m_bySWState				= SSWS_NONE;
}

CNtlSubWeaponController::~CNtlSubWeaponController()
{
	
}

void CNtlSubWeaponController::SetData(CNtlSobActor* pActor, CNtlPLCharacter *pPLCharacter, CNtlPLItem *pPLItem, RwBool bResLoadComplete)
{
    m_pActor        = pActor;
	m_pPLCharacter	= pPLCharacter;
	m_pPLItem		= pPLItem;

	m_bResLoadComplete = bResLoadComplete;

	if(!m_bResLoadComplete)
		m_pPLItem->SetVisible(FALSE);
	else
		m_pPLItem->SetVisible(TRUE);
	
	SetState(SSWS_IDLE);
}

void CNtlSubWeaponController::SetChangeItem(void)
{
	m_pPLCharacter->SetChangeEquipItem(m_pPLItem, FALSE);
}

void CNtlSubWeaponController::RemoveChangeItem(void)
{
	m_pPLCharacter->SetRemoveEquipItem(m_pPLItem);
}

void CNtlSubWeaponController::SetState(RwUInt8 byState)
{
	if(byState == SSWS_IDLE)
	{
		m_pMatSubWeaponBone = m_pPLCharacter->GetBoneMatrix(ITEM_EQUIP_USER_MOVE1_NAME);
		m_pPLItem->SetAnimation(ITEM_IDLE);
	}
	else if(byState == SSWS_MOVE)
	{
	}
	else if(byState == SSWS_ATTACK)
	{
		m_pPLItem->SetAnimation(ITEM_ATTACK);
	}

	m_bySWState = byState;
}

void CNtlSubWeaponController::UpdateCreate(RwReal fElapsed)
{
	
}

void CNtlSubWeaponController::UpdateIdle(RwReal fElapsed)
{
	if(m_pMatSubWeaponBone == NULL)
	{
		m_pMatSubWeaponBone = m_pPLCharacter->GetBoneMatrix(ITEM_EQUIP_USER_MOVE1_NAME);
		if(m_pMatSubWeaponBone == NULL)
			return;
	}
	
	RwV3d vPos = m_pMatSubWeaponBone->pos;
	RwV3d vOffset = m_pPLItem->GetSubWeaponOffset();
	RwV3d vDir = m_pPLCharacter->GetDirection();

    // Reads additional offset data from the table.
    std::string strSubWeapon = "SUB_WEAPON";
    CNtlSobSocialAttr* pAttr = (CNtlSobSocialAttr*)m_pActor->GetSobAttr();    
    sLINK_ITEM_TRASFORM* pGet = CLinkItemTrasformList::GetInstance().Get(strSubWeapon, CNtlDefaultItemTable::GetCharType(pAttr->GetClass(), pAttr->GetGender(), pAttr->IsAdult()));
    if(pGet)
    {
        vOffset.y += pGet->_POS[1];
    }

	RwV3d vTransOffset;
	CNtlMath::MathRwMatrixAssign(&m_matWorld, &CNtlPLGlobal::m_vZeroV3, &vDir);
	RwV3dTransformPoints(&vTransOffset, &vOffset, 1, &m_matWorld);

	RwV3dAdd(&vPos, &vPos, &vTransOffset);
	RwV3dAssignMacro(&m_matWorld.pos, &vPos);

	m_pPLItem->SetMatrix(m_matWorld);

	m_bResLoaded = TRUE;
}

void CNtlSubWeaponController::UpdateMove(RwReal fElapsed)
{
}

void CNtlSubWeaponController::UpdateAttack(RwReal fElapsed)
{

}

void CNtlSubWeaponController::Update(RwReal fElapsed)
{
	if(m_bySWState == SSWS_IDLE)
		UpdateIdle(fElapsed);
	else if(m_bySWState == SSWS_MOVE)
		UpdateMove(fElapsed);
	else if(m_bySWState == SSWS_ATTACK)
		UpdateAttack(fElapsed);
}

void CNtlSubWeaponController::NotifyOwnerStateEnter(RwUInt32 uiStateId)
{
}

void CNtlSubWeaponController::NotifyOwnerStateExit(RwUInt32 uiStateId)
{
}

void CNtlSubWeaponController::ResourceLoadComplete(RwBool bVisible)
{
	if(m_pPLItem)
		m_pPLItem->SetVisible(bVisible);

	m_bResLoadComplete = TRUE;
}

RwBool CNtlSubWeaponController::IsResourceLoaded(void)
{
	return m_bResLoaded;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNtlSobCharEquipProxy::CNtlSobCharEquipProxy()
{
	m_bUIRender				= FALSE;
	m_pSobObj				= NULL;
	m_pPLCharacter			= NULL;

	for(RwInt32 i = 0; i < NTL_MAX_EQUIP_ITEM_SLOT; ++i)
	{
		m_arrSlotItem[i].uiItemTblId	= INVALID_TBLIDX;
		m_arrSlotItem[i].byGrade		= 0;
		m_arrSlotItem[i].pPLItem		= NULL;
		m_arrSlotItem[i].bCharacterAdd	= FALSE;
		m_arrSlotItem[i].bReCreate		= FALSE;
	}

	m_bSunWeaponReCreate	= FALSE;
	m_pSubWeaponController	= NULL;

	m_byStorageWeaponNum	= 0;
	
	for(RwInt32 i = 0; i < EMaxStorageWeapon; i++)
	{
		m_sStorageWeapon[i].bySlotIdx		= 0;
		m_sStorageWeapon[i].uiItemTblId		= INVALID_TBLIDX;
		m_sStorageWeapon[i].byGrade			= 0;
		m_sStorageWeapon[i].pPLItem			= NULL;
	}

	m_bActiveSubWeapon		= FALSE;

	m_bVisible				= TRUE;
	m_bResLoaded			= FALSE;
	m_bResLoadComplete		= FALSE;
	m_byAlpha				= 255;
	m_byRed					= 255;
	m_byGreen				= 255;
	m_byBlue				= 255;

	m_pEmblemFactor			= NULL;
	m_pTexEmblem			= NULL;
}

CNtlSobCharEquipProxy& CNtlSobCharEquipProxy::operator = (CNtlSobCharEquipProxy& EquipProxy)
{
//	DBO_WARNING_MESSAGE("CNtlSobCharEquipProxy::operator =");
	DeleteAllItem();

	RwBool bLoadScheduling = GetNtlResourceManager()->IsLoadScheduling();
	GetNtlResourceManager()->SetLoadScheduling(FALSE);

	m_bActiveSubWeapon = EquipProxy.GetActiveSubWeapon();

	SEquipItem *pEquipItem;

	bool bWearQuestItem = false;
	RwBool bCostume = FALSE;
	RwUInt16 wCostumeBitFlags = 0;

	CItemTable *pItemTbl = API_GetTableContainer()->GetItemTable();

	for(RwInt32 i = EQUIP_SLOT_TYPE_COSTUME_SET; i < NTL_MAX_EQUIP_ITEM_SLOT; ++i)
	{
		pEquipItem = EquipProxy.GetEquipItem(i);
		if(!pEquipItem->pPLItem)
			continue;

		sITEM_TBLDAT *pItemTblData = reinterpret_cast<sITEM_TBLDAT*>(pItemTbl->FindData(pEquipItem->uiItemTblId));
		NTL_ASSERT(pItemTblData, "CNtlSobCharEquipProxy::operator = (CNtlSobCharEquipProxy& EquipProxy)");

		bCostume = TRUE;
		wCostumeBitFlags += pItemTblData->wCostumeHideBitFlag;
	}

	for(RwUInt8 i = 0; i < NTL_MAX_EQUIP_ITEM_SLOT; i++)
	{
		pEquipItem = EquipProxy.GetEquipItem(i);
		if(pEquipItem->uiItemTblId != INVALID_TBLIDX)
		{
			m_arrSlotItem[i].uiItemTblId	= pEquipItem->uiItemTblId;
			m_arrSlotItem[i].byGrade		= pEquipItem->byGrade;
		}

		if(!pEquipItem->pPLItem)
			continue;

		if (i == EQUIP_SLOT_TYPE_QUEST)
			bWearQuestItem = true;

		m_arrSlotItem[i].pPLItem = CreatePLItem(i, pEquipItem->pPLItem->GetName(), m_pSobObj->GetSerialID(), pEquipItem->byGrade);
		m_arrSlotItem[i].bCharacterAdd = pEquipItem->bCharacterAdd;

		if(bCostume)
		{
			if (Logic_IsHideCostumeItem(wCostumeBitFlags, (void*)m_arrSlotItem[i].pPLItem))
			{
				switch (i)
				{
					case EQUIP_SLOT_TYPE_COSTUME_SET:
					case EQUIP_SLOT_TYPE_COSTUME_HAIR_STYLE:
					case EQUIP_SLOT_TYPE_COSTUME_MASK:
					case EQUIP_SLOT_TYPE_COSTUME_HAIR_ACCESSORY:
					case EQUIP_SLOT_TYPE_COSTUME_BACK_ACCESSORY: break;

					default: 
					{
						GetSceneManager()->DeleteEntity(m_arrSlotItem[i].pPLItem);
						m_arrSlotItem[i].pPLItem = NULL;
						continue;
					}
					break;
				}
			}
		}

		if(bCostume && (i == EQUIP_SLOT_TYPE_COSTUME_SET || i == EQUIP_SLOT_TYPE_COSTUME_HAIR_STYLE || i == EQUIP_SLOT_TYPE_COSTUME_MASK || i == EQUIP_SLOT_TYPE_COSTUME_HAIR_ACCESSORY || i == EQUIP_SLOT_TYPE_COSTUME_BACK_ACCESSORY))
		{
			if (i == EQUIP_SLOT_TYPE_COSTUME_BACK_ACCESSORY && bWearQuestItem)
			{
				GetSceneManager()->DeleteEntity(m_arrSlotItem[i].pPLItem);
				m_arrSlotItem[i].pPLItem = NULL;
				continue;
			}

			if(m_arrSlotItem[i].bCharacterAdd)
            {
				sITEM_TBLDAT *pItemTblData = reinterpret_cast<sITEM_TBLDAT*>(pItemTbl->FindData(pEquipItem->uiItemTblId));

				m_pPLCharacter->SetChangeEquipItem(m_arrSlotItem[i].pPLItem, TRUE, pItemTblData->wCostumeHideBitFlag);
                SetAnimationCostumeItem(m_arrSlotItem[i].pPLItem);
            }

		}
		else
		{
			if(m_arrSlotItem[i].bCharacterAdd)
				m_pPLCharacter->SetChangeEquipItem(m_arrSlotItem[i].pPLItem);
		}
	}

	CNtlSubWeaponController *pSubWeaponController = EquipProxy.GetSubWeaponController();
	if(pSubWeaponController)
	{
		m_pSubWeaponController = NTL_NEW CNtlSubWeaponController;
		m_pSubWeaponController->SetData((CNtlSobActor*)m_pSobObj, m_pPLCharacter, m_arrSlotItem[EQUIP_SLOT_TYPE_SUB_WEAPON].pPLItem, m_bResLoadComplete); 
		m_pSubWeaponController->SetChangeItem();    
		m_pSubWeaponController->SetState(CNtlSubWeaponController::SSWS_IDLE);
	}

	m_byStorageWeaponNum = EquipProxy.GetStorageWeaponNum();

	SStorageEquipItem *pStorageEquipItem;
	for(RwUInt8 i = 0; i < m_byStorageWeaponNum; i++)
	{
		pStorageEquipItem = EquipProxy.GetStorageWeaponItem(i);
		m_sStorageWeapon[i].bySlotIdx	= pStorageEquipItem->bySlotIdx;
		m_sStorageWeapon[i].uiItemTblId	= pStorageEquipItem->uiItemTblId;
		m_sStorageWeapon[i].byGrade		= pStorageEquipItem->byGrade;
		m_sStorageWeapon[i].pPLItem		= CreatePLItem(i, pStorageEquipItem->pPLItem->GetName(), m_pSobObj->GetSerialID(), pStorageEquipItem->byGrade);
	}

	if(m_bUIRender)
		SetVisible(FALSE);

	GetNtlResourceManager()->SetLoadScheduling(bLoadScheduling);
	
	return *this;
}


CNtlSobCharEquipProxy::~CNtlSobCharEquipProxy()
{
	DeleteAllItem();
}

void CNtlSobCharEquipProxy::SetData(CNtlSob *pSobObj, CNtlPLCharacter *pPLCharacter, RwBool bUIRender)
{
	m_bUIRender		= bUIRender;
	m_pSobObj		= pSobObj;
	m_pPLCharacter	= pPLCharacter;
}

void CNtlSobCharEquipProxy::UpdateResourceLoadCheck(RwReal fElapsed)
{
	if(m_pSubWeaponController)
	{
		if(!m_pSubWeaponController->IsResourceLoaded())
			return;
	}
	
	CNtlPLItem *pPLItem;

	for(RwInt32 i = 0; i < NTL_MAX_EQUIP_ITEM_SLOT; ++i)
	{
		pPLItem = m_arrSlotItem[i].pPLItem;
		if(pPLItem)
		{
			if(!pPLItem->GetItemScheduleResInfo()->bLoadComplete)
				return;
		}
	}

	m_bResLoaded = TRUE;
}

void CNtlSobCharEquipProxy::Update(RwReal fElapsed)
{
	if(m_pSubWeaponController)
		m_pSubWeaponController->Update(fElapsed);

	if(m_bResLoaded)
		return;

	UpdateResourceLoadCheck(fElapsed);
}

void CNtlSobCharEquipProxy::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventSobCreate)
		SobEquipCreateEventHandler(pMsg);
	else if(pMsg.Id == g_EventSobEquipChange)
	{
		RwBool bScheduling = GetNtlResourceManager()->IsLoadScheduling();
		GetNtlResourceManager()->SetLoadScheduling(FALSE);

		SobEquipChangeEventHandler(pMsg);

		GetNtlResourceManager()->SetLoadScheduling(bScheduling);
	}
	else if(pMsg.Id == g_EventSubWeaponActive)
	{
		RwBool bScheduling = GetNtlResourceManager()->IsLoadScheduling();
		GetNtlResourceManager()->SetLoadScheduling(FALSE);

		SobSubWeaponActiveEventHandler(pMsg);

		GetNtlResourceManager()->SetLoadScheduling(bScheduling);
	}
	else if(pMsg.Id == g_EventSubWeaponDeActive)
	{
		RwBool bScheduling = GetNtlResourceManager()->IsLoadScheduling();
		GetNtlResourceManager()->SetLoadScheduling(FALSE);

		SobSubWeaponDeActiveEventHandler(pMsg);

		GetNtlResourceManager()->SetLoadScheduling(bScheduling);
	}

	if(m_bUIRender)
		SetVisible(FALSE);
}


void CNtlSobCharEquipProxy::NotifyOwnerStateEnter(RwUInt32 uiStateId)
{
	
}

void CNtlSobCharEquipProxy::NotifyOwnerStateExit(RwUInt32 uiOldState, RwUInt32 uiNewState)
{
    // 스킬 캐스팅상태에서는 무기를 자동으로 제거하지 않는다.
    if((uiOldState == NTL_FSMSID_SKILL_READY && uiNewState == NTL_FSMSID_SKILL_CASTING) ||
       (uiOldState == NTL_FSMSID_SKILL_CASTING && uiNewState == NTL_FSMSID_SKILL_ACTION))
    {

    }
    else
    {
        RWS::CMsg msg;
        SobSubWeaponDeActiveEventHandler(msg);    
    }    
}

CNtlPLItem*	CNtlSobCharEquipProxy::CreatePLItem(RwUInt8 bySlotIdx, const RwChar *pModelName, RwUInt32 uiSerialId, RwUInt8 byUpgrade)
{
	SPLItemCreateParam CreateParam;
	CreateParam.byUpgrade	= byUpgrade;
	
	CNtlPLItem *pPLItem = static_cast<CNtlPLItem*>(GetSceneManager()->CreateEntity(PLENTITY_ITEM, pModelName, &CreateParam));
	NTL_ASSERT(pPLItem, "PLItem is Create Fail! - " << pModelName);

	pPLItem->SetSerialID(uiSerialId);

	if(bySlotIdx == EQUIP_SLOT_TYPE_SUB_WEAPON)
		pPLItem->SetEnableTrace(FALSE);
	
    // Item is not created yet.
	//pPLItem->SetUpgradeEffect((ENtlPLItemGrade)byUpgrade);
	
	pPLItem->SetAlpha(m_byAlpha);
	pPLItem->SetColor(m_byRed, m_byGreen, m_byBlue);

	if(m_bResLoadComplete)
		pPLItem->SetVisible(m_bVisible);
	else
		pPLItem->SetVisible(FALSE);
	
	return pPLItem;
}

void CNtlSobCharEquipProxy::DeleteAllItem(void)
{
	if(m_pSubWeaponController)
	{
		NTL_DELETE(m_pSubWeaponController);
	}

	for(RwInt32 i = 0; i < NTL_MAX_EQUIP_ITEM_SLOT; ++i)
	{
		if(m_arrSlotItem[i].pPLItem)
		{
			GetSceneManager()->DeleteEntity(m_arrSlotItem[i].pPLItem);
			m_arrSlotItem[i].uiItemTblId	= INVALID_TBLIDX;
			m_arrSlotItem[i].pPLItem		= NULL;
		}
	}

	if(m_pEmblemFactor)
	{
		NTL_DELETE(m_pEmblemFactor);
		m_pEmblemFactor	= NULL;
	}

	if( m_pTexEmblem )
	{
		GetEmblemMaker()->DestoyEmblem( m_pTexEmblem );
		m_pTexEmblem = NULL;
	}
}

void CNtlSobCharEquipProxy::SobEquipCreateEventHandler(RWS::CMsg &pMsg)
{
//	DBO_WARNING_MESSAGE("SobEquipCreateEventHandler: " << m_pSobObj->GetClassID());
	if(m_pSobObj->GetClassID() == SLCLASS_AVATAR)
	{
		bool bWearQuestItem = false;
		RwBool bCostume = FALSE;
		RwUInt16 wCostumeBitFlags = 0;

		std::string strModelName;
		SERIAL_HANDLE hSerial;
		CNtlSobAvatar *pSobAvatar = reinterpret_cast<CNtlSobAvatar*>( m_pSobObj );
		CNtlInventory *pInventory = pSobAvatar->GetInventory();

		for(RwInt32 i = EQUIP_SLOT_TYPE_COSTUME_SET; i < NTL_MAX_EQUIP_ITEM_SLOT; ++i)
		{
			hSerial = pInventory->GetEquipItem(i);
			if(hSerial == INVALID_SERIAL_ID)
				continue;

			CNtlSobItem *pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(hSerial) );
			CNtlSobItemAttr *pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pSobItem->GetSobAttr() );
			sITEM_TBLDAT *pItemTblData = pSobItemAttr->GetItemTbl();
			NTL_ASSERT( pItemTblData, "CNtlSobCharProxy::CreateEquipPLItem : pItemTblData Must be Present!" );
				
			bCostume = TRUE;
			wCostumeBitFlags += pItemTblData->wCostumeHideBitFlag;
		}

		for(int i = 0; i < NTL_MAX_EQUIP_ITEM_SLOT; i++)
		{
			hSerial = pInventory->GetEquipItem(i);
			if(hSerial == INVALID_SERIAL_ID)
				continue;

			CNtlSobItem *pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(hSerial) );
			CNtlSobItemAttr *pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pSobItem->GetSobAttr() );
			sITEM_TBLDAT *pItemTblData = pSobItemAttr->GetItemTbl();
			NTL_ASSERT( pItemTblData, "CNtlSobCharProxy::SobEquipCreateEventHandler : pItemTblData Must be Present!" );
			
			m_arrSlotItem[i].uiItemTblId	= pItemTblData->tblidx;
			m_arrSlotItem[i].byGrade		= pSobItemAttr->GetGrade();

			if (EQUIP_TYPE_ACCESSORY == pItemTblData->byEquip_Type)
				continue;

			if (i == EQUIP_SLOT_TYPE_QUEST)
				bWearQuestItem = true;
			

			strModelName = pItemTblData->szModel;
			if (strModelName.size() != 0)
			{
				MakeItemModelName(strModelName, pItemTblData->byModel_Type);
				m_arrSlotItem[i].pPLItem = CreatePLItem(i, strModelName.c_str(), m_pSobObj->GetSerialID(), pSobItemAttr->GetGrade());
			}

			if(bCostume)
			{
				if (Logic_IsHideCostumeItem(wCostumeBitFlags, (void*)m_arrSlotItem[i].pPLItem))
				{
					switch (i)
					{
						case EQUIP_SLOT_TYPE_QUEST:
						case EQUIP_SLOT_TYPE_COSTUME_SET:
						case EQUIP_SLOT_TYPE_COSTUME_HAIR_STYLE:
						case EQUIP_SLOT_TYPE_COSTUME_MASK:
						case EQUIP_SLOT_TYPE_COSTUME_HAIR_ACCESSORY:
						case EQUIP_SLOT_TYPE_COSTUME_BACK_ACCESSORY: break;

						default:
						{
							GetSceneManager()->DeleteEntity(m_arrSlotItem[i].pPLItem);
							m_arrSlotItem[i].pPLItem = NULL;
							continue;
						}
						break;
					}
				}
			}

			// sub weapon 중에서 gem과 같이 움직일 수 있는 종류라면...
			if(Logic_IsMoveableSubWeapon(pItemTblData))
			{
				m_pSubWeaponController = NTL_NEW CNtlSubWeaponController;
				m_pSubWeaponController->SetData((CNtlSobActor*)m_pSobObj, m_pPLCharacter, m_arrSlotItem[i].pPLItem, m_bResLoadComplete); 
				m_pSubWeaponController->SetChangeItem();
				m_arrSlotItem[i].bCharacterAdd = FALSE;
			}
			else
			{
				if(m_arrSlotItem[i].pPLItem)
				{
					if(bCostume && (i == EQUIP_SLOT_TYPE_COSTUME_SET || i == EQUIP_SLOT_TYPE_COSTUME_HAIR_STYLE || i == EQUIP_SLOT_TYPE_COSTUME_MASK || i == EQUIP_SLOT_TYPE_COSTUME_HAIR_ACCESSORY || i == EQUIP_SLOT_TYPE_COSTUME_BACK_ACCESSORY))
					{
						if (i == EQUIP_SLOT_TYPE_COSTUME_BACK_ACCESSORY && bWearQuestItem)
						{
							GetSceneManager()->DeleteEntity(m_arrSlotItem[i].pPLItem);
							m_arrSlotItem[i].pPLItem = NULL;
							continue;
						}

					//	SetEmblemFactor(2, 0, 3, 1, 2, 2);
						m_pPLCharacter->SetChangeEquipItem(m_arrSlotItem[i].pPLItem, TRUE, pItemTblData->wCostumeHideBitFlag);
                        SetAnimationCostumeItem(m_arrSlotItem[i].pPLItem);
					//	m_arrSlotItem[i].pPLItem->AttachDogiEmblem(m_pTexEmblem);
					//	RwRGBA rgba;
					//	rgba.alpha	= 255;
					//	rgba.red	= rand()%256;
					//	rgba.green	= rand()%256;
					//	rgba.blue	= rand()%256;
					//	m_arrSlotItem[i].pPLItem->SetDogiBeltColor(rgba);
					}
					else
					{
						m_pPLCharacter->SetChangeEquipItem(m_arrSlotItem[i].pPLItem);
					}

					m_arrSlotItem[i].bCharacterAdd = TRUE;
				}
			}
		}
	}
	else if(m_pSobObj->GetClassID() == SLCLASS_PLAYER)
	{
		bool bWearQuestItem = false;
		RwBool bCostume = FALSE;
		RwUInt16 wCostumeBitFlags = 0;

		SNtlEventSobPlayerCreate *pSobPlayerCreate = reinterpret_cast<SNtlEventSobPlayerCreate*>(pMsg.pData);
		sITEM_BRIEF *pItemBrief = pSobPlayerCreate->pPcBrief->sItemBrief;

		std::string strModelName;
		CItemTable *pItemTbl = API_GetTableContainer()->GetItemTable();

		for(RwInt32 i = EQUIP_SLOT_TYPE_COSTUME_SET; i < NTL_MAX_EQUIP_ITEM_SLOT; ++i)
		{
			if(pItemBrief[i].tblidx == INVALID_TBLIDX)
				continue;
			
			sITEM_TBLDAT *pItemTblData = reinterpret_cast<sITEM_TBLDAT*>( pItemTbl->FindData(pItemBrief[i].tblidx) );
			NTL_ASSERT( pItemTblData, "CNtlSobCharProxy::SobEquipCreateEventHandler : pItemTblData Must be Present!" );
				
			bCostume = TRUE;
			wCostumeBitFlags += pItemTblData->wCostumeHideBitFlag;
		}

		for(RwUInt8 i = 0; i < NTL_MAX_EQUIP_ITEM_SLOT; ++i)
		{
			if(pItemBrief[i].tblidx == INVALID_TBLIDX)
				continue;

			sITEM_TBLDAT *pItemTblData = reinterpret_cast<sITEM_TBLDAT*>( pItemTbl->FindData(pItemBrief[i].tblidx) );

			m_arrSlotItem[i].uiItemTblId	= pItemBrief[i].tblidx;
			m_arrSlotItem[i].byGrade		= pItemBrief[i].byGrade;
			
			strModelName = pItemTblData->szModel;
			if (strModelName.size() > 0)
			{
				MakeItemModelName(strModelName, pItemTblData->byModel_Type);
				m_arrSlotItem[i].pPLItem = CreatePLItem(i, strModelName.c_str(), m_pSobObj->GetSerialID(), pItemBrief[i].byGrade);
			}

			if (i == EQUIP_SLOT_TYPE_QUEST)
				bWearQuestItem = true;

			if(bCostume)
			{
				if (Logic_IsHideCostumeItem(wCostumeBitFlags, (void*)m_arrSlotItem[i].pPLItem))
				{
					switch (i)
					{
					case EQUIP_SLOT_TYPE_COSTUME_SET:
					case EQUIP_SLOT_TYPE_COSTUME_HAIR_STYLE:
					case EQUIP_SLOT_TYPE_COSTUME_MASK:
					case EQUIP_SLOT_TYPE_COSTUME_HAIR_ACCESSORY:
					case EQUIP_SLOT_TYPE_COSTUME_BACK_ACCESSORY: break;

					default:
					{
						GetSceneManager()->DeleteEntity(m_arrSlotItem[i].pPLItem);
						m_arrSlotItem[i].pPLItem = NULL;
						continue;
					}
					break;
					}
				}
			}
			
			// sub weapon 중에서 gem과 같이 움직일 수 있는 종류라면...
			if(Logic_IsMoveableSubWeapon(pItemTblData))
			{
				m_pSubWeaponController = NTL_NEW CNtlSubWeaponController;
				m_pSubWeaponController->SetData((CNtlSobActor*)m_pSobObj, m_pPLCharacter, m_arrSlotItem[i].pPLItem, m_bResLoadComplete); 
				m_pSubWeaponController->SetChangeItem();
				m_arrSlotItem[i].bCharacterAdd = FALSE;
			}
			else
			{
				if(m_arrSlotItem[i].pPLItem)
				{
					if (bCostume && (i == EQUIP_SLOT_TYPE_COSTUME_SET || i == EQUIP_SLOT_TYPE_COSTUME_HAIR_STYLE || i == EQUIP_SLOT_TYPE_COSTUME_MASK || i == EQUIP_SLOT_TYPE_COSTUME_HAIR_ACCESSORY || i == EQUIP_SLOT_TYPE_COSTUME_BACK_ACCESSORY))
					{
						if (i == EQUIP_SLOT_TYPE_COSTUME_BACK_ACCESSORY && bWearQuestItem)
						{
							GetSceneManager()->DeleteEntity(m_arrSlotItem[i].pPLItem);
							m_arrSlotItem[i].pPLItem = NULL;
							continue;
						}

						m_pPLCharacter->SetChangeEquipItem(m_arrSlotItem[i].pPLItem, TRUE, pItemTblData->wCostumeHideBitFlag);
                        SetAnimationCostumeItem(m_arrSlotItem[i].pPLItem);
					}
					else
					{
						m_pPLCharacter->SetChangeEquipItem(m_arrSlotItem[i].pPLItem);
					}

					m_arrSlotItem[i].bCharacterAdd = TRUE;
				}
			}
		}        
	}
}



void CNtlSobCharEquipProxy::CreateEquipItem(RwUInt32 uiTblId, RwUInt8 bySlotIdx, RwUInt8 byGrade)
{
//	DBO_WARNING_MESSAGE("CNtlSobCharEquipProxy::CreateEquipItem");
	if(uiTblId == INVALID_TBLIDX)
		return;

	RwBool bCostume[EQUIP_SLOT_TYPE_COUNT];
	memset(bCostume, FALSE, sizeof(bCostume));

	for (RwUInt16 i = EQUIP_SLOT_TYPE_COSTUME_SET; i < EQUIP_SLOT_TYPE_COUNT; i++)
	{
		bCostume[i] = FALSE;

		if (m_arrSlotItem[i].uiItemTblId != INVALID_TBLIDX)
		{
			bCostume[i] = TRUE;
		}
	}

	CItemTable *pItemTbl = API_GetTableContainer()->GetItemTable();
	sITEM_TBLDAT *pItemTblData = reinterpret_cast<sITEM_TBLDAT*>( pItemTbl->FindData(uiTblId) );

	// Check if new item is costume and hide others
	if(Logic_IsCostumeItem(pItemTblData))
	{
		for(RwInt32 i = 0; i < NTL_MAX_EQUIP_ITEM_SLOT; i++)
		{
			if( m_arrSlotItem[i].pPLItem == NULL )
				continue;
			
			if (Logic_IsHideCostumeItem(pItemTblData->wCostumeHideBitFlag, (void*)m_arrSlotItem[i].pPLItem) == FALSE)
				continue;

			if(m_arrSlotItem[i].bCharacterAdd)
				m_pPLCharacter->SetRemoveEquipItem(m_arrSlotItem[i].pPLItem);

			if(i == EQUIP_SLOT_TYPE_SUB_WEAPON)
			{
				if(m_pSubWeaponController)
				{
					m_pSubWeaponController->RemoveChangeItem();
					NTL_DELETE(m_pSubWeaponController);
				}
			}
			
			GetSceneManager()->DeleteEntity(m_arrSlotItem[i].pPLItem);

			m_arrSlotItem[i].pPLItem		= NULL;
			m_arrSlotItem[i].bCharacterAdd	= FALSE;
		}
	}
	else if (bySlotIdx == EQUIP_SLOT_TYPE_QUEST) // if we equip quest item, then remove back accessory
	{
		if (m_arrSlotItem[EQUIP_SLOT_TYPE_COSTUME_BACK_ACCESSORY].pPLItem)
		{
			if (m_arrSlotItem[EQUIP_SLOT_TYPE_COSTUME_BACK_ACCESSORY].bCharacterAdd)
				m_pPLCharacter->SetRemoveEquipItem(m_arrSlotItem[EQUIP_SLOT_TYPE_COSTUME_BACK_ACCESSORY].pPLItem);

			GetSceneManager()->DeleteEntity(m_arrSlotItem[EQUIP_SLOT_TYPE_COSTUME_BACK_ACCESSORY].pPLItem);

			m_arrSlotItem[EQUIP_SLOT_TYPE_COSTUME_BACK_ACCESSORY].pPLItem = NULL;
			m_arrSlotItem[EQUIP_SLOT_TYPE_COSTUME_BACK_ACCESSORY].bCharacterAdd = FALSE;
		}
	}

	// Mount new items.

	m_arrSlotItem[bySlotIdx].uiItemTblId	= uiTblId;
	m_arrSlotItem[bySlotIdx].byGrade		= byGrade;

	if (bySlotIdx == EQUIP_SLOT_TYPE_COSTUME_BACK_ACCESSORY && m_arrSlotItem[EQUIP_SLOT_TYPE_QUEST].pPLItem) // if we equip back accessory, but have quest item, then return.
	{
		m_arrSlotItem[bySlotIdx].pPLItem = NULL;
		m_arrSlotItem[bySlotIdx].bCharacterAdd = FALSE;
		return;
	}

	std::string strModelName = pItemTblData->szModel;

	if(strModelName.size() != 0)
	{
		MakeItemModelName(strModelName, pItemTblData->byModel_Type);
		m_arrSlotItem[bySlotIdx].pPLItem		= CreatePLItem(bySlotIdx, strModelName.c_str(), m_pSobObj->GetSerialID(), byGrade);
	}

	// check current items and hide new item if required
	for (RwUInt16 i = EQUIP_SLOT_TYPE_COSTUME_SET; i < EQUIP_SLOT_TYPE_COUNT; i++)
	{
		if (bCostume[i])
		{
			sITEM_TBLDAT *pCostumeItemTblData = reinterpret_cast<sITEM_TBLDAT*>(pItemTbl->FindData(m_arrSlotItem[i].uiItemTblId));
			if (pCostumeItemTblData)
			{
				if (Logic_IsHideCostumeItem(pCostumeItemTblData->wCostumeHideBitFlag, (void*)m_arrSlotItem[bySlotIdx].pPLItem))
				{
					GetSceneManager()->DeleteEntity(m_arrSlotItem[bySlotIdx].pPLItem);
					m_arrSlotItem[bySlotIdx].pPLItem = NULL;
					return;
				}
			}
		}
	}

	// 
	// If it's a kind of sub weapon that can move like a gem ...
	if(Logic_IsMoveableSubWeapon(pItemTblData))
	{
		m_pSubWeaponController = NTL_NEW CNtlSubWeaponController;
		m_pSubWeaponController->SetData((CNtlSobActor*)m_pSobObj, m_pPLCharacter, m_arrSlotItem[bySlotIdx].pPLItem, m_bResLoadComplete); 
		m_pSubWeaponController->SetChangeItem();
		m_arrSlotItem[bySlotIdx].bCharacterAdd = FALSE;
	}
	else
	{
		if(m_arrSlotItem[bySlotIdx].pPLItem)
		{
			if(Logic_IsCostumeItem(pItemTblData))
            {
				m_pPLCharacter->SetChangeEquipItem(m_arrSlotItem[bySlotIdx].pPLItem, TRUE, pItemTblData->wCostumeHideBitFlag);
                SetAnimationCostumeItem(m_arrSlotItem[bySlotIdx].pPLItem);
            }
			else
				m_pPLCharacter->SetChangeEquipItem(m_arrSlotItem[bySlotIdx].pPLItem);

			m_arrSlotItem[bySlotIdx].bCharacterAdd = TRUE;
		}
	}

	if(m_arrSlotItem[bySlotIdx].pPLItem)
	{
		// picking flag check
		if(m_pPLCharacter->GetFlags() & NTL_PLEFLAG_PICKING)
			m_arrSlotItem[bySlotIdx].pPLItem->SetPicking(TRUE);
		else
			m_arrSlotItem[bySlotIdx].pPLItem->SetPicking(FALSE);
	}
}

void CNtlSobCharEquipProxy::DeleteEquipItem(RwUInt8 bySlotIdx)
{
	// Delete existing items.
	if(m_arrSlotItem[bySlotIdx].pPLItem)
	{
		CNtlPLItem *pDelPLItem = m_arrSlotItem[bySlotIdx].pPLItem;

		CItemTable *pItemTbl = API_GetTableContainer()->GetItemTable();
		sITEM_TBLDAT *pOldItemTblData = reinterpret_cast<sITEM_TBLDAT*>( pItemTbl->FindData(m_arrSlotItem[bySlotIdx].uiItemTblId) ); 
		if(Logic_IsCostumeItem(pOldItemTblData))
		{
			if(m_arrSlotItem[bySlotIdx].bCharacterAdd)
				m_pPLCharacter->SetRemoveEquipItem(m_arrSlotItem[bySlotIdx].pPLItem, pOldItemTblData->wCostumeHideBitFlag);

			m_arrSlotItem[bySlotIdx].uiItemTblId	= INVALID_TBLIDX;
			m_arrSlotItem[bySlotIdx].byGrade		= 0;
			m_arrSlotItem[bySlotIdx].pPLItem		= NULL;
			m_arrSlotItem[bySlotIdx].bCharacterAdd	= FALSE;

			for(RwUInt8 i = 0; i < NTL_MAX_EQUIP_ITEM_SLOT; i++)
			{
				if( m_arrSlotItem[i].pPLItem )
					continue;

				if( i == bySlotIdx)
					continue;

				if(m_arrSlotItem[i].uiItemTblId == INVALID_TBLIDX)
					continue;

				CreateEquipItem(m_arrSlotItem[i].uiItemTblId, i, m_arrSlotItem[i].byGrade);
			}
		}
		else
		{
			if(m_arrSlotItem[bySlotIdx].bCharacterAdd)
				m_pPLCharacter->SetRemoveEquipItem(m_arrSlotItem[bySlotIdx].pPLItem);

			if(bySlotIdx == EQUIP_SLOT_TYPE_SUB_WEAPON)
			{
				if(m_pSubWeaponController)
				{
					m_pSubWeaponController->RemoveChangeItem();
					NTL_DELETE(m_pSubWeaponController);
				}
			}
		}
		
		GetSceneManager()->DeleteEntity(pDelPLItem);
	}

	m_arrSlotItem[bySlotIdx].uiItemTblId	= INVALID_TBLIDX;
	m_arrSlotItem[bySlotIdx].byGrade		= 0;
	m_arrSlotItem[bySlotIdx].pPLItem		= NULL;
	m_arrSlotItem[bySlotIdx].bCharacterAdd	= FALSE;

	// if we remove quest item, check if we have to add back accessory
	if (bySlotIdx == EQUIP_SLOT_TYPE_QUEST)
	{
		if (m_arrSlotItem[EQUIP_SLOT_TYPE_COSTUME_BACK_ACCESSORY].pPLItem == NULL)
		{
			if (m_arrSlotItem[EQUIP_SLOT_TYPE_COSTUME_BACK_ACCESSORY].uiItemTblId != INVALID_TBLIDX)
			{
				CreateEquipItem(m_arrSlotItem[EQUIP_SLOT_TYPE_COSTUME_BACK_ACCESSORY].uiItemTblId, EQUIP_SLOT_TYPE_COSTUME_BACK_ACCESSORY, m_arrSlotItem[EQUIP_SLOT_TYPE_COSTUME_BACK_ACCESSORY].byGrade);
			}
		}
	}
}

void CNtlSobCharEquipProxy::SobEquipChangeEventHandler(RWS::CMsg &pMsg)
{
	SNtlEventSobEquipChange *pEquipChange = reinterpret_cast<SNtlEventSobEquipChange*>( pMsg.pData );

	DeleteEquipItem(pEquipChange->bySlotIdx);
	CreateEquipItem(pEquipChange->uiTblId, pEquipChange->bySlotIdx, pEquipChange->byGrade);
}

void CNtlSobCharEquipProxy::AddWorld(void)
{
	for(RwInt32 i = 0; i < NTL_MAX_EQUIP_ITEM_SLOT; i++)
	{
		if(m_arrSlotItem[i].pPLItem)
		{
			m_arrSlotItem[i].pPLItem->AddWorld();
		}
	}
}

void CNtlSobCharEquipProxy::RemoveWorld(void)
{
	for(RwInt32 i = 0; i < NTL_MAX_EQUIP_ITEM_SLOT; i++)
	{
		if(m_arrSlotItem[i].pPLItem)
		{
			m_arrSlotItem[i].pPLItem->RemoveWorld();
		}
	}
}

void CNtlSobCharEquipProxy::MakeItemModelName(std::string& strModelName, RwUInt8 byModeNameRule)
{
	CNtlSobPlayerAttr *pPlayerAttr = reinterpret_cast<CNtlSobPlayerAttr*>(m_pSobObj->GetSobAttr());
	RwUInt8 byRace = pPlayerAttr->GetRace();
	RwUInt8 byGender = pPlayerAttr->GetGender();
	RwUInt8 bySkinColor = pPlayerAttr->GetSkinColor();

	MakeItemModelName(strModelName, byRace, byGender, bySkinColor, byModeNameRule);
}

void CNtlSobCharEquipProxy::MakeItemModelName(std::string& strModelName, RwUInt8 byRace, RwUInt8 byGender, RwUInt8 bySkinColor, RwUInt8 byModeNameRule)
{
	if(byModeNameRule == ITEM_MODEL_TYPE_NONE)
		return;

	// 종족
	if(byRace == RACE_HUMAN)
	{
		strModelName += "_H";
	}
	else if(byRace == RACE_NAMEK)
	{
		strModelName += "_N";
	}
	else 
	{
		strModelName += "_M";
	}

	// 성별
	if(byGender == GENDER_MALE)
	{
		strModelName += "_M";
	}
	else if(byGender == GENDER_FEMALE)
	{
		strModelName += "_F";
	}
	else 
	{
		strModelName += "_O";
	}

	// by daneos: disabled the following because we no longer use textures as skin color 
	//// skin color
	if(byRace == RACE_NAMEK)
	{
	//	RwChar chBuff[10];
	//	if(byModeNameRule == ITEM_MODEL_TYPE_RACE_GENDER_SKIN)
	//	{
	//		sprintf_s(chBuff, 10, "_S%d", bySkinColor);
	//		strModelName += chBuff;
	//	}
	//	else
	//	{
	//		sprintf_s(chBuff, 10, "_S%d", 1);
	//		strModelName += chBuff;
	//	}

		strModelName += "_S1";
	}
}


void CNtlSobCharEquipProxy::StorageMainWeapon(void)
{
	// main weapon을 change 해야 하는 sub weapon이면?
	if(m_arrSlotItem[EQUIP_SLOT_TYPE_HAND].pPLItem)
	{
		if(Logic_IsWeaponChangeSubWeapon(m_arrSlotItem[EQUIP_SLOT_TYPE_SUB_WEAPON].uiItemTblId))
		{
			NTL_ASSERT(m_byStorageWeaponNum >= 0 && m_byStorageWeaponNum < EMaxStorageWeapon, "CNtlSobCharEquipProxy::StorageMainWeapon => storage buffer overflow !!!");

			m_pPLCharacter->SetRemoveEquipItem(m_arrSlotItem[EQUIP_SLOT_TYPE_HAND].pPLItem);

			m_sStorageWeapon[m_byStorageWeaponNum].bySlotIdx	= EQUIP_SLOT_TYPE_HAND;
			m_sStorageWeapon[m_byStorageWeaponNum].uiItemTblId	= m_arrSlotItem[EQUIP_SLOT_TYPE_HAND].uiItemTblId;
			m_sStorageWeapon[m_byStorageWeaponNum].byGrade		= m_arrSlotItem[EQUIP_SLOT_TYPE_HAND].byGrade;
			m_sStorageWeapon[m_byStorageWeaponNum].pPLItem		= m_arrSlotItem[EQUIP_SLOT_TYPE_HAND].pPLItem;

			m_byStorageWeaponNum++;
		}
	}
}

void CNtlSobCharEquipProxy::RestoreMainWeapon(void)
{
	for(RwInt32 i = 0; i < m_byStorageWeaponNum; i++)
	{
		RwUInt8 bySlotIdx = m_sStorageWeapon[i].bySlotIdx;
		
		m_arrSlotItem[bySlotIdx].uiItemTblId	= m_sStorageWeapon[i].uiItemTblId;
		m_arrSlotItem[bySlotIdx].byGrade		= m_sStorageWeapon[i].byGrade;
		m_arrSlotItem[bySlotIdx].pPLItem		= m_sStorageWeapon[i].pPLItem;
		m_arrSlotItem[bySlotIdx].bCharacterAdd = TRUE;

		m_pPLCharacter->SetChangeEquipItem(m_arrSlotItem[bySlotIdx].pPLItem);

		m_sStorageWeapon[i].bySlotIdx	= 0;
		m_sStorageWeapon[i].uiItemTblId	= INVALID_TBLIDX;
		m_sStorageWeapon[i].bySlotIdx	= 0;
		m_sStorageWeapon[i].pPLItem		= NULL;
	}

	m_byStorageWeaponNum = 0;
}

void CNtlSobCharEquipProxy::ProcNotAttachSubWeapon(CNtlPLItem *pPLItem)
{
	RwMatrix mat;
	RwMatrix *pMatBone = m_pPLCharacter->GetBoneMatrix(ITEM_EQUIP_USER_MOVE1_NAME);
	
	RwV3d vPos = pMatBone->pos;
	RwV3d vOffset = pPLItem->GetSubWeaponOffset();
	RwV3d vDir = m_pPLCharacter->GetDirection();

    // Reads additional offset data from the table.
    std::string strSubWeapon = "SUB_WEAPON";
    CNtlSobSocialAttr* pAttr = (CNtlSobSocialAttr*)m_pSobObj->GetSobAttr();
    sLINK_ITEM_TRASFORM* pGet = CLinkItemTrasformList::GetInstance().Get(strSubWeapon, CNtlDefaultItemTable::GetCharType(pAttr->GetClass(), pAttr->GetGender(), pAttr->IsAdult()));
    vOffset.y += pGet->_POS[1];

	RwV3d vTransOffset;
	CNtlMath::MathRwMatrixAssign(&mat, &CNtlPLGlobal::m_vZeroV3, &vDir);
	RwV3dTransformPoints(&vTransOffset, &vOffset, 1, &mat);

	RwV3dAdd(&vPos, &vPos, &vTransOffset);
	RwV3dAssignMacro(&mat.pos, &vPos);

	pPLItem->SetMatrix(mat);
//	pPLItem->SetAnimation(SUBWEAPON_ATTACK);
	pPLItem->SetVisible(TRUE);
}

void CNtlSobCharEquipProxy::SobSubWeaponActiveEventHandler(RWS::CMsg &pMsg)
{
	m_bActiveSubWeapon = TRUE;

	RwUInt8 bySlotIdx = EQUIP_SLOT_TYPE_SUB_WEAPON;
	CItemTable *pItemTbl = API_GetTableContainer()->GetItemTable();
	sITEM_TBLDAT *pItemTblData = reinterpret_cast<sITEM_TBLDAT*>( pItemTbl->FindData(m_arrSlotItem[bySlotIdx].uiItemTblId) );

	if(pItemTblData == NULL)
		return;

	if(Logic_IsMoveableSubWeapon(m_arrSlotItem[EQUIP_SLOT_TYPE_SUB_WEAPON].uiItemTblId))
	{
		if(m_pSubWeaponController)
			m_pSubWeaponController->SetState(CNtlSubWeaponController::SSWS_ATTACK);

		return;
	}

	// Remove the existing sub weapon.
	
	if(m_arrSlotItem[bySlotIdx].pPLItem)
	{
		if(m_arrSlotItem[bySlotIdx].bCharacterAdd)
			m_pPLCharacter->SetRemoveEquipItem(m_arrSlotItem[bySlotIdx].pPLItem);

		GetSceneManager()->DeleteEntity(m_arrSlotItem[bySlotIdx].pPLItem);

		m_arrSlotItem[bySlotIdx].pPLItem = NULL;
		m_arrSlotItem[bySlotIdx].bCharacterAdd	= FALSE;
	}

	//What is a sub weapon to change the main weapon?
	StorageMainWeapon();
	
	std::string strModelName = pItemTblData->szSub_Weapon_Act_Model;
	if(strModelName.size() != 0)
	{
		MakeItemModelName(strModelName, pItemTblData->byModel_Type);
		m_arrSlotItem[bySlotIdx].pPLItem = CreatePLItem(bySlotIdx, strModelName.c_str(), m_pSobObj->GetSerialID(), m_arrSlotItem[bySlotIdx].byGrade);
	}

	// It is a moving sub weapon or is not attached in active state.
	RwBool bIsMoveableSubWeapon = Logic_IsMoveableSubWeapon(m_arrSlotItem[bySlotIdx].uiItemTblId);
	RwBool bIsNotAttachSubWeapon = Logic_IsActiveNotAttachSubWeapon(m_arrSlotItem[bySlotIdx].uiItemTblId);
	
	if( bIsMoveableSubWeapon || bIsNotAttachSubWeapon )
	{
		if(bIsNotAttachSubWeapon)
			ProcNotAttachSubWeapon(m_arrSlotItem[bySlotIdx].pPLItem);

		m_arrSlotItem[bySlotIdx].bCharacterAdd	= FALSE;
	}
	else
	{
		if(m_arrSlotItem[bySlotIdx].pPLItem)
		{
			m_pPLCharacter->SetChangeEquipItem(m_arrSlotItem[bySlotIdx].pPLItem);
			m_arrSlotItem[bySlotIdx].bCharacterAdd	= TRUE;
		}
	}

	// Set the sub weapon trace.
	if(m_arrSlotItem[bySlotIdx].pPLItem)
		m_arrSlotItem[bySlotIdx].pPLItem->SetEnableTrace(TRUE);

	
}

void CNtlSobCharEquipProxy::SobSubWeaponDeActiveEventHandler(RWS::CMsg &pMsg)
{
	if(!m_bActiveSubWeapon)
		return;

	m_bActiveSubWeapon = FALSE;

	RwUInt8 bySlotIdx = EQUIP_SLOT_TYPE_SUB_WEAPON;
	CItemTable *pItemTbl = API_GetTableContainer()->GetItemTable();
	sITEM_TBLDAT *pItemTblData = reinterpret_cast<sITEM_TBLDAT*>( pItemTbl->FindData(m_arrSlotItem[bySlotIdx].uiItemTblId) );

	if(pItemTblData == NULL)
		return;

	if(Logic_IsMoveableSubWeapon(m_arrSlotItem[EQUIP_SLOT_TYPE_SUB_WEAPON].uiItemTblId))
	{
		if(m_pSubWeaponController)
			m_pSubWeaponController->SetState(CNtlSubWeaponController::SSWS_IDLE);
		return;
	}

	// Removes the sub weapon you are attaching.
	
	if(m_arrSlotItem[bySlotIdx].pPLItem)
	{
		if(m_arrSlotItem[bySlotIdx].bCharacterAdd)
			m_pPLCharacter->SetRemoveEquipItem(m_arrSlotItem[bySlotIdx].pPLItem);

		GetSceneManager()->DeleteEntity(m_arrSlotItem[bySlotIdx].pPLItem);

		m_arrSlotItem[bySlotIdx].pPLItem		= NULL;
		m_arrSlotItem[bySlotIdx].bCharacterAdd	= FALSE;
	}

	// main weapon restore
	RestoreMainWeapon();

	std::string strModelName = pItemTblData->szModel;

	if(strModelName.size() != 0)
	{
		MakeItemModelName(strModelName, pItemTblData->byModel_Type);
		m_arrSlotItem[bySlotIdx].pPLItem = CreatePLItem(bySlotIdx, strModelName.c_str(), m_pSobObj->GetSerialID(), m_arrSlotItem[bySlotIdx].byGrade);
		m_pPLCharacter->SetChangeEquipItem(m_arrSlotItem[bySlotIdx].pPLItem);
		m_arrSlotItem[bySlotIdx].bCharacterAdd	= TRUE;
	}
}


void CNtlSobCharEquipProxy::SetVisible(RwBool bVisible)
{
	for(RwInt32 i = 0; i < NTL_MAX_EQUIP_ITEM_SLOT; ++i)
	{
		if(m_arrSlotItem[i].pPLItem)
			m_arrSlotItem[i].pPLItem->SetVisible(bVisible);
	}

	m_bVisible = bVisible;
}

void CNtlSobCharEquipProxy::SetVisible( RwBool bVisible, RwUInt8 bySlotIdx ) 
{
    if(m_arrSlotItem[bySlotIdx].pPLItem)
    {
        m_arrSlotItem[bySlotIdx].pPLItem->SetVisible(bVisible);
    }
}

void CNtlSobCharEquipProxy::SetAlpha(RwUInt8 byAlpha)
{
	for(RwInt32 i = 0; i < NTL_MAX_EQUIP_ITEM_SLOT; ++i)
	{
		if(m_arrSlotItem[i].pPLItem)
			m_arrSlotItem[i].pPLItem->SetAlpha(byAlpha);
	}

	m_byAlpha = byAlpha;
}

void CNtlSobCharEquipProxy::SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue)
{
	for(RwInt32 i = 0; i < NTL_MAX_EQUIP_ITEM_SLOT; ++i)
	{
		if(m_arrSlotItem[i].pPLItem)
			m_arrSlotItem[i].pPLItem->SetColor(byRed, byGreen, byBlue);
	}

	m_byRed		= byRed;
	m_byGreen	= byGreen;
	m_byBlue	= byBlue;
}

void CNtlSobCharEquipProxy::SetPicking(RwBool bPicking)
{
	for(RwInt32 i = 0; i < NTL_MAX_EQUIP_ITEM_SLOT; ++i)
	{
		if(m_arrSlotItem[i].pPLItem)
			m_arrSlotItem[i].pPLItem->SetPicking(bPicking);
	}
}

RwV3d CNtlSobCharEquipProxy::GetWeaponBonePosition(const RwChar *pBoneName)
{
	if(m_arrSlotItem[EQUIP_SLOT_TYPE_HAND].pPLItem == NULL)
		return m_pSobObj->GetPosition();

	RwMatrix *pMat = m_arrSlotItem[EQUIP_SLOT_TYPE_HAND].pPLItem->GetBoneMatrix(pBoneName);
	if(pMat == NULL)
		return m_pSobObj->GetPosition();

	return pMat->pos;
}

RwV3d CNtlSobCharEquipProxy::GetSubWeaponBonePosition(const RwChar *pBoneName)
{
	if(m_arrSlotItem[EQUIP_SLOT_TYPE_SUB_WEAPON].pPLItem == NULL)
		return m_pSobObj->GetPosition();

	RwMatrix *pMat = m_arrSlotItem[EQUIP_SLOT_TYPE_SUB_WEAPON].pPLItem->GetBoneMatrix(pBoneName);
	if(pMat == NULL)
		return m_pSobObj->GetPosition();

	return pMat->pos;
}

void CNtlSobCharEquipProxy::DetachConvertClassEquipItem(void)
{
	for(RwInt32 i = 0; i < NTL_MAX_EQUIP_ITEM_SLOT; ++i)
	{
		if(m_arrSlotItem[i].pPLItem)
		{
			if(m_arrSlotItem[i].bCharacterAdd)
				m_pPLCharacter->SetRemoveEquipItem(m_arrSlotItem[i].pPLItem);

			if(m_arrSlotItem[i].pPLItem)
			{
				GetSceneManager()->DeleteEntity(m_arrSlotItem[i].pPLItem);
				m_arrSlotItem[i].pPLItem = NULL;
			}
		}
	}
}

void CNtlSobCharEquipProxy::AttachConvertClassEquipItem(CNtlPLCharacter *pPLCharacter)
{
	//DBO_WARNING_MESSAGE("CNtlSobCharEquipProxy::AttachConvertClassEquipItem");
	m_pPLCharacter = pPLCharacter;

	bool bWearQuestItem = false;
	RwBool bCostume = FALSE;
	RwUInt16 wCostumeBitFlags = 0;

	for(RwInt32 i = EQUIP_SLOT_TYPE_COSTUME_SET; i < NTL_MAX_EQUIP_ITEM_SLOT; ++i)
	{
		if(m_arrSlotItem[i].uiItemTblId == INVALID_TBLIDX)
			continue;

		CItemTable *pItemTbl = API_GetTableContainer()->GetItemTable();
		sITEM_TBLDAT *pItemTblData = reinterpret_cast<sITEM_TBLDAT*>( pItemTbl->FindData(m_arrSlotItem[i].uiItemTblId) );
			
		NTL_ASSERT( pItemTblData, "CNtlSobCharEquipProxy::AttachConvertClassEquipItem : pItemTblData Must be Present!" );
			
		bCostume = TRUE;
		wCostumeBitFlags += pItemTblData->wCostumeHideBitFlag;
	}

	for(RwUInt8 i = 0; i < NTL_MAX_EQUIP_ITEM_SLOT; ++i)
	{
		if(m_arrSlotItem[i].uiItemTblId != INVALID_TBLIDX)
		{
			CItemTable *pItemTbl = API_GetTableContainer()->GetItemTable();
			sITEM_TBLDAT *pItemTblData = reinterpret_cast<sITEM_TBLDAT*>( pItemTbl->FindData(m_arrSlotItem[i].uiItemTblId) );
			std::string strModelName = pItemTblData->szModel;

			if(strModelName.size() != 0)
			{
				MakeItemModelName(strModelName, pItemTblData->byModel_Type);
				m_arrSlotItem[i].pPLItem = CreatePLItem(i, strModelName.c_str(), m_pSobObj->GetSerialID(), m_arrSlotItem[i].byGrade);

				if(bCostume)
				{
					if (Logic_IsHideCostumeItem(wCostumeBitFlags, (void*)m_arrSlotItem[i].pPLItem))
					{
						switch (i)
						{
							case EQUIP_SLOT_TYPE_COSTUME_SET:
							case EQUIP_SLOT_TYPE_COSTUME_HAIR_STYLE:
							case EQUIP_SLOT_TYPE_COSTUME_MASK:
							case EQUIP_SLOT_TYPE_COSTUME_HAIR_ACCESSORY:
							case EQUIP_SLOT_TYPE_COSTUME_BACK_ACCESSORY: break;

							default:
							{
								GetSceneManager()->DeleteEntity(m_arrSlotItem[i].pPLItem);
								m_arrSlotItem[i].pPLItem = NULL;
								continue;
							}
							break;
						}
					}
				}

				if(bCostume && (i == EQUIP_SLOT_TYPE_COSTUME_SET || i == EQUIP_SLOT_TYPE_COSTUME_HAIR_STYLE || i == EQUIP_SLOT_TYPE_COSTUME_MASK || i == EQUIP_SLOT_TYPE_COSTUME_HAIR_ACCESSORY || i == EQUIP_SLOT_TYPE_COSTUME_BACK_ACCESSORY))
				{
					if (i == EQUIP_SLOT_TYPE_COSTUME_BACK_ACCESSORY && bWearQuestItem)
					{
						GetSceneManager()->DeleteEntity(m_arrSlotItem[i].pPLItem);
						m_arrSlotItem[i].pPLItem = NULL;
						continue;
					}

					m_pPLCharacter->SetChangeEquipItem(m_arrSlotItem[i].pPLItem, TRUE, pItemTblData->wCostumeHideBitFlag);
                    SetAnimationCostumeItem(m_arrSlotItem[i].pPLItem);
				}
				else
				{
					if(m_arrSlotItem[i].bCharacterAdd)
						m_pPLCharacter->SetChangeEquipItem(m_arrSlotItem[i].pPLItem);
				}
			}
		}
	}
}

void CNtlSobCharEquipProxy::SetEmblemFactor(RwUInt8 byTypeA, RwUInt8 byTypeAColor,	
											RwUInt8 byTypeB, RwUInt8 byTypeBColor, 
											RwUInt8 byTypeC, RwUInt8 byTypeCColor)
{
	if(byTypeA == dPLEMBLEM_INVALID_FACTOR || byTypeB == dPLEMBLEM_INVALID_FACTOR || 
		byTypeC == dPLEMBLEM_INVALID_FACTOR)
	{
		NTL_DELETE(m_pEmblemFactor);
		m_pEmblemFactor = NULL;

		if( m_pTexEmblem )
		{
			GetEmblemMaker()->DestoyEmblem( m_pTexEmblem );
			m_pTexEmblem = NULL;
		}
	}
	else
	{
		if(m_pEmblemFactor == NULL)
		{
			m_pEmblemFactor = NTL_NEW sEmblemFactor;
		}

		m_pEmblemFactor->byTypeA		= byTypeA;
		m_pEmblemFactor->byTypeB		= byTypeB;
		m_pEmblemFactor->byTypeC		= byTypeC;
		m_pEmblemFactor->byTypeAColor	= byTypeAColor;
		m_pEmblemFactor->byTypeBColor	= byTypeBColor;
		m_pEmblemFactor->byTypeCColor	= byTypeCColor;

		if( m_pTexEmblem )
		{
			GetEmblemMaker()->DestoyEmblem( m_pTexEmblem );
			m_pTexEmblem = NULL;
		}

		m_pTexEmblem = GetEmblemMaker()->CreateEmblem( m_pEmblemFactor );
	}
}


void CNtlSobCharEquipProxy::ResourceLoadComplete(RwBool bVisible)
{
	CNtlPLItem *pPLItem;

	for(RwInt32 i = 0; i < NTL_MAX_EQUIP_ITEM_SLOT; ++i)
	{
		pPLItem = m_arrSlotItem[i].pPLItem;

		if(pPLItem)
		{
			pPLItem->SetVisible(bVisible);
		}
	}

	m_bResLoadComplete = TRUE;
}

SEquipItem*	CNtlSobCharEquipProxy::GetEquipItem(RwInt32 iSlotIdx)
{
	if(iSlotIdx < 0 || iSlotIdx >= NTL_MAX_EQUIP_ITEM_SLOT)
		return NULL;

	return &m_arrSlotItem[iSlotIdx];
}

CNtlSubWeaponController* CNtlSobCharEquipProxy::GetSubWeaponController(void) const
{
	return m_pSubWeaponController;
}

RwUInt8 CNtlSobCharEquipProxy::GetStorageWeaponNum(void) const
{
	return m_byStorageWeaponNum;
}

CNtlSobCharEquipProxy::SStorageEquipItem* CNtlSobCharEquipProxy::GetStorageWeaponItem(RwInt32 iSlotIdx)
{
	return &m_sStorageWeapon[iSlotIdx];
}

RwBool CNtlSobCharEquipProxy::GetActiveSubWeapon(void) const
{
	return m_bActiveSubWeapon;
}

RwBool CNtlSobCharEquipProxy::IsResourceLoadComplete(void) const
{
	return m_bResLoaded;
}

RwBool CNtlSobCharEquipProxy::IsResourceLoaded(void) const
{
	return m_bResLoaded;
}

void CNtlSobCharEquipProxy::SetAnimationCostumeItem( CNtlPLItem* pItem ) 
{
    if(!pItem)
        return;

    if(pItem->GetEquipSlotType() == ITEM_EQUIP_BACK && pItem->IsHaveAnimData())
    {
        pItem->SetAnimation(ITEM_IDLE);
    }
}