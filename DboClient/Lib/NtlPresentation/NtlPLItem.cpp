#include "precomp_ntlpresentation.h"
#include "NtlPLItem.h"
#include "NtlPLEntityFreeList.h"
#include "NtlPLResourceManager.h"
#include "NtlPLMaterialExtWrapper.h"
#include "NtlAtomic.h"
#include "NtlPLCharacter.h"
#include "NtlPLSceneManager.h"
#include "NtlPLEventGenerator.h"
#include "NtlPLRenderState.h"
#include "NtlPLItemUVTimeLine.h"
#include "NtlPLEntityRenderHelpers.h"
#include "LinkItemTrasformList.h"
#include "NtlPLHelpers.h"
#include "NtlPLPalette.h"


// 기획쪽 요청으로 도복의 기본 색상을 흰색으로 한다
#define dDEFAULT_DOGI_COLOR_RED				(255)
#define dDEFAULT_DOGI_COLOR_GREEN			(255)
#define dDEFAULT_DOGI_COLOR_BLUE			(255)



const char* g_strTraceBoneName[] = 
{
    {"Bip01 R Finger0"},
    {"Bip01 R Finger2"},
    {"Bip01 L Finger0"},
    {"Bip01 L Finger2"}
};

void* CNtlPLItem::operator new(size_t size)
{
	NTL_FUNCTION("CNtlPLObject::operator new");
    
	NTL_RETURN(CNtlPLEntityFreeList::Alloc(PLENTITY_ITEM));
}

void CNtlPLItem::operator delete(void *pObj)
{
	CNtlPLEntityFreeList::Free(PLENTITY_ITEM, pObj);
}

CNtlPLItem::CNtlPLItem() : m_pProperty(NULL),
						   m_pResourceClump(NULL),
						   m_eInstanceEquipSlotType(ITEM_EQUIP_NONE),
						   m_pBaseTexture(NULL),
						   m_pMultiTexture(NULL),
						   m_pOwner(NULL),
                           m_pAnimLayer(NULL),
                           m_pBaseHierarchy(NULL),
                           m_pInstanceAnimTable(NULL),
                           m_uiCurAnimKey(0),
                           m_bAnimUpdate(FALSE),
						   m_bVisualEffect(TRUE),                                                      
                           m_nGrade(0),
                           m_bEnableUpgradeEffect(TRUE),                           
						   m_pUVMatrix(NULL),
						   m_pUVTexture(NULL),
						   m_bRunTimeAlpha(FALSE),
						   m_SimpleMaterial(FALSE),
						   m_PetrifyMaterial(FALSE)
{
	SetLayer(PLENTITY_LAYER_RWWORLD);
	SetClassType(PLENTITY_ITEM);
	SetFlags(NTL_PLEFLAG_PICKING | NTL_PLEFLAG_ATTACH | NTL_PLEFLAG_NOTUPDATE);
	
	m_sColor.red   = 255;
	m_sColor.green = 255;
	m_sColor.blue  = 255;
	m_sColor.alpha = 255;
	
	m_sAddColor.red = 0;
	m_sAddColor.green = 0;
	m_sAddColor.blue = 0;
	m_sAddColor.alpha = 255;

	m_bEnableTrace = FALSE;
	m_bVisualEffect = TRUE;
	RwMatrixSetIdentity(&m_matWorld);

	m_pEMUVController = NULL;
	m_pDogiEmblemTex = NULL;

	// should be a blue belt if it's not, it's not
	m_DogiBeltClr.red	= 255;
	m_DogiBeltClr.green = 255;
	m_DogiBeltClr.blue	= 0;
	m_DogiBeltClr.alpha = 255;

	// 도복 색상
	m_DogiEditedColor.red	= dDEFAULT_DOGI_COLOR_RED;
	m_DogiEditedColor.green = dDEFAULT_DOGI_COLOR_GREEN;
	m_DogiEditedColor.blue	= dDEFAULT_DOGI_COLOR_BLUE;
	m_DogiEditedColor.alpha = 255;

	m_fnDefRenderCallBack = NULL;
}

CNtlPLItem::~CNtlPLItem()
{
	m_pDogiEmblemTex = NULL;
}

RwBool CNtlPLItem::Create(const SPLEntityCreateParam *pParam)
{
	if(pParam)
	{
		SPLItemCreateParam *pItemParam = (SPLItemCreateParam*)pParam;
		m_nGrade = pItemParam->byUpgrade;
		//SetSerialID(pItemParam->uiSerialId);
	}

	if(!CreateScheduling(pParam))
		return FALSE;
		
	return TRUE;
}

RwBool	CNtlPLItem::SetThreadSafeProperty(const CNtlPLProperty *pData)
{
	NTL_ASSERTE(pData);
	if(pData == NULL)
		return FALSE;

	m_pProperty = (CNtlPLItemProperty *)pData;

	m_eInstanceEquipSlotType = m_pProperty->GetEquipSlotType();
    m_bEnableTrace = m_pProperty->GetTraceEnableDefault();

	// Update registration is registered dynamically when it is registered in Visual Manager
	//Can not be done. Registration is required here.
    if( strlen(m_pProperty->m_eventTrace.strStartBoneName) > 0 &&
        strlen(m_pProperty->m_eventTrace.strEndBoneName) > 0 ||
		m_pProperty->m_vLinkEffect.size() || IsHaveAnimData())
    {
        if(GetFlags() & NTL_PLEFLAG_NOTUPDATE)
            SetFlags(GetFlags() ^ NTL_PLEFLAG_NOTUPDATE);
    }

	return TRUE;
}

RwBool	CNtlPLItem::CreateScheduling(const SPLEntityCreateParam * pParam)
{
	NTL_ASSERT(m_pProperty, "CNtlPLItem::CreateScheduling - Property is null.");

	std::string strClumpFileName = m_pProperty->GetMeshPath();
	strClumpFileName += m_pProperty->GetMeshFileName();

	std::string strTexPath = "Texture\\Item\\";
	CNtlPLResourceManager::GetInstance()->LoadSchedulingClump( strClumpFileName.c_str(), strTexPath.c_str(), this );   
	
	return TRUE;
}

void CNtlPLItem::CallSchedulingResource(CNtlPLResource *pResource)
{
	m_pResourceClump = pResource;

	NTL_ASSERTE(m_pResourceClump);
	if(m_pResourceClump == NULL)
		return;
	
	CreateThreadSafe();
}

RwBool CNtlPLItem::CreateThreadSafe(void)
{
	// In case of Body, Clone Atomic List is kept.
	if( g_CheckItemBody(GetResType()) )
		Helper_SetClumpAllAtomics(m_pResourceClump->GetClump(), &m_vecAtomicList, true);
	else
		Helper_SetClumpAllAtomics(m_pResourceClump->GetClump(), &m_vecAtomicList, false);
	
	Helper_GetBoneList(m_pResourceClump->GetClump(), &m_mapFrame);
	
	if(g_CheckItemWeapon(GetResType()))
		SetPicking(FALSE);

	for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
	{
		RpAtomic *pAtomic = m_vecAtomicList[i]->pData;
		RpNtlAtomicSetData(pAtomic, this);
		Helper_SetAtomicAllMaterialSkinInfo(pAtomic);
		
		if( GetEquipSlotType() == ITEM_EQUIP_NONE)
		{
            RpNtlAtomicSetRenderCallBack( pAtomic, RpAtomicGetRenderCallBack(pAtomic) );
			RpAtomicSetRenderCallBack(pAtomic, CNtlPLCharacter::RenderCallBack);
			NtlMatExtSetPipeline(pAtomic);
		}
	}

    // Upgrade Effect
    if(m_nGrade > ITEM_GRADE_2)
    {
        SetUpgradeEffect((ENtlPLItemGrade)m_nGrade);
    }

	m_sScheduleResInfo.bLoadComplete = TRUE;
	AddWorld();
	
	SetVisible( m_sScheduleResInfo.bVisible );

	//SetEMUV();

    // Link Effect를 설정한다.
    for(UINT i = 0; i < m_pProperty->m_vLinkEffect.size(); ++i)
    {
        SEventLinkEffect* pEventLinkEffect = m_pProperty->m_vLinkEffect[i];        
        m_LinkEffectInstance.AttachLinkEffect(this, pEventLinkEffect);
    }

    // 애니메이션을 설정한다.
    if(m_sScheduleResInfo.uiAnimKey > 0)
    {
        SetAnimation(m_sScheduleResInfo.uiAnimKey, 0.0f, TRUE);
    }

    GetSceneManager()->AddUpdate(this);

	return TRUE;
}

RwBool CNtlPLItem::SetProperty(const CNtlPLProperty *pProperty)
{
	return SetThreadSafeProperty(pProperty);
}

void CNtlPLItem::AddWorld(void)
{
	if( g_CheckItemBody( GetResType() ) || 
		m_pResourceClump == NULL ||
		CNtlPLGlobal::m_pRpWorld == NULL)
		return;
	
	if(RpClumpGetWorld( m_pResourceClump->GetClump() ) == NULL)
		RpWorldAddClump(CNtlPLGlobal::m_pRpWorld, m_pResourceClump->GetClump());
}

void CNtlPLItem::RemoveWorld(void)
{
	if( g_CheckItemBody( GetResType() ) ||
		m_pResourceClump == NULL ||
		CNtlPLGlobal::m_pRpWorld == NULL )
		return;

	if(RpClumpGetWorld( m_pResourceClump->GetClump() ) != NULL)
		RpWorldRemoveClump(CNtlPLGlobal::m_pRpWorld, m_pResourceClump->GetClump());
}

RwBool CNtlPLItem::IsExistWorld()
{
	if( m_pResourceClump == NULL )
		return FALSE;

	if( RpClumpGetWorld( m_pResourceClump->GetClump() ) != NULL)
		return TRUE;

	return FALSE;
}

void CNtlPLItem::Destroy(void)
{
	RemoveWorld();
	
	GetSceneManager()->RemoveUpdate(this);

	NTL_DELETE(m_pEMUVController);

	if(m_pUVMatrix)
	{
		RwMatrixDestroy(m_pUVMatrix);
	}

    // Upgrade Effect Delete
    for each(CNtlInstanceEffect* pUpgradeEffect in m_pUpgradeEffects)
    {
        if(!pUpgradeEffect)
            continue;

        GetSceneManager()->DeleteEntity(pUpgradeEffect);
    }
    m_pUpgradeEffects.clear();

	for each(SPLAttachAttr* pPLAttachAttr in m_listAttachAttr)
    {
        if(!pPLAttachAttr->pPLEntity)
            continue;

        if(strcmp(pPLAttachAttr->pPLEntity->GetName(), EFFECT_NAME_TRACE) == 0 ||
           strcmp(pPLAttachAttr->pPLEntity->GetName(), EFFECT_NAME_GRADE) == 0)
        {
            pPLAttachAttr->pPLEntity->SetAttachParent(NULL);
            GetSceneManager()->DeleteEntity(pPLAttachAttr->pPLEntity);
            pPLAttachAttr->pPLEntity = NULL;
        }
    }
	
	
	for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
	{
		if(m_vecAtomicList[i]->bClone)
		{
			if( !m_sScheduleResInfo.bApplyedEquip &&
				g_CheckItemBody(GetResType()) )
			{
				RpAtomic *pAtomic = m_vecAtomicList[i]->pData;
				RpAtomicDestroy(pAtomic);
			}
		}
		NTL_DELETE (m_vecAtomicList[i]);
	}
	m_vecAtomicList.clear();

	CNtlPLResourceManager::GetInstance()->UnLoadScheduling(this, m_pResourceClump );
	m_pResourceClump = NULL;
	
	if(m_pBaseTexture)
	{
		CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_pBaseTexture);
		m_pBaseTexture = NULL;
	}

	if(m_pMultiTexture)
	{
		CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_pMultiTexture);
		m_pMultiTexture = NULL;
	}

	if(m_ToonData.pToonPaint)
		RpToonPaintDestroy(m_ToonData.pToonPaint);
	if(m_ToonData.pToonInk)
		RpToonInkDestroy(m_ToonData.pToonInk);

	if(m_ToonData.pTexture)
		CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_ToonData.pTexture);

    if(m_pInstanceAnimTable)
    {
        m_pInstanceAnimTable->Destroy();
        NTL_DELETE(m_pInstanceAnimTable);
    }

    if(m_pAnimLayer)
    {
        m_pAnimLayer->Destroy();
        NTL_DELETE(m_pAnimLayer);
    }
}

void CNtlPLItem::SetBaseTexture(RwTexture *pTexture)
{
	if(m_pBaseTexture != NULL)
	{
		CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_pBaseTexture);
	}
	
	m_pBaseTexture = pTexture;
	RwTextureAddRef(m_pBaseTexture);
}

void CNtlPLItem::SetMultiTexture(RwTexture *pTexture)
{
	if(m_pMultiTexture != NULL)
	{
		CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_pMultiTexture);
	}

	m_pMultiTexture = pTexture;
	RwTextureAddRef(m_pMultiTexture);
}

void CNtlPLItem::SetMatrix(RwMatrix& matWorld)
{
	m_matWorld = matWorld;

	if( m_pResourceClump == NULL ||
		m_pResourceClump->GetClump() == NULL )
		return;

	RwFrame *pFrame = RpClumpGetFrame( m_pResourceClump->GetClump() );
	RwMatrixCopy( RwFrameGetMatrix( pFrame ), &matWorld );
	RwFrameUpdateObjects(pFrame);
	
	CNtlPLAttach::SetMatrix(matWorld);
}

void CNtlPLItem::SetPosition(const RwV3d *pPos)
{
	*RwMatrixGetPos(&m_matWorld) = *pPos;

	if( m_pResourceClump == NULL ||
		m_pResourceClump->GetClump() == NULL )
		return;

	RwFrame *pFrame = RpClumpGetFrame( m_pResourceClump->GetClump() );
	RwFrameTranslate(pFrame, pPos, rwCOMBINEREPLACE );
	
	CNtlPLAttach::SetPosition(pPos);
}

void CNtlPLItem::SetChangeEquip(CNtlPLCharacter *pOwner, RwBool bApplyAttach)
{
	m_pOwner = pOwner;

	m_sScheduleResInfo.bEquip = TRUE;
	m_sScheduleResInfo.bAttach = bApplyAttach;

	if( m_pOwner->GetCharScheduleResInfo()->bLoadComplete )
		SetApplyEquipData();
}

RwBool CNtlPLItem::GetMeshFilePrefixName(string& _MeshFileName)
{
	if(m_pProperty)
	{
		_MeshFileName = m_pProperty->GetMeshFileName();
		RwInt32 SequenceIdx = _MeshFileName.find_first_of('_');
		SequenceIdx = _MeshFileName.find_first_of('_', SequenceIdx + 1);
		
		_MeshFileName.erase(_MeshFileName.begin() + SequenceIdx, _MeshFileName.end());

		return TRUE;
	}

	return FALSE;
}

void CNtlPLItem::SetApplyEquipData()
{
	if( !m_sScheduleResInfo.bLoadComplete || m_sScheduleResInfo.bApplyedEquip  || !m_pOwner)
		return;

	if( m_sScheduleResInfo.bEquip )
	{
		if( m_sScheduleResInfo.bAttach )
		{
			RpClump *pOwnerClump = m_pOwner->GetClump();
			NTL_ASSERT(pOwnerClump, "pOwnerClump Failed");

			//Body is added to World in Clump of Owner.
			if( g_CheckItemBody(GetResType()) )
			{
				RwBool bOwnerAddWorld = m_pOwner->IsExistWorld();
				m_pOwner->RemoveWorld();
				
				for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
				{
					RpAtomic *pAtomic = m_vecAtomicList[i]->pData;
					NTL_ASSERT(pAtomic, "pAtomic Failed");
					
					RwFrame *pFrame = m_vecAtomicList[i]->pFrame;
					NTL_ASSERT(pFrame, "pFrame Failed");

					RwChar *chBoneName = Helper_BoneName(pFrame);
					NTL_ASSERT(chBoneName, "chBoneName Failed");

					RwFrame *pNewFrame = m_pOwner->GetBoneByName(chBoneName);
					NTL_ASSERT(pNewFrame, "pNewFrame Failed " << chBoneName);
					
					RpAtomicSetFrame(pAtomic, pNewFrame);
					RpSkinAtomicSetHAnimHierarchy(pAtomic, m_pOwner->GetBaseHierarchy());
					RpClumpAddAtomic(pOwnerClump, pAtomic);
					
					SetApplyEquipDataOwner(pAtomic, m_pOwner);
				}

				if(bOwnerAddWorld)
					m_pOwner->AddWorld();
			}
			else
			{
				RwBool bOwnerAddWorld = m_pOwner->IsExistWorld();
				RemoveWorld();

				RwChar	PivotName[64] = {0,};
				RwBool	Ready2LinkBone = TRUE;

				if(GetEquipSlotType() == ITEM_EQUIP_HEAD)
				{
					strcpy_s(PivotName, 64, ITEM_EQUIP_MASK_NAME);
				}
				else if(GetEquipSlotType() == ITEM_EQUIP_TWO_HAND)
				{
					strcpy_s(PivotName, 64, ITEM_EQUIP_TWO_HAND_NAME);
				}
				else if(GetEquipSlotType() == ITEM_EQUIP_L_HAND)
				{
					strcpy_s(PivotName, 64, ITEM_EQUIP_L_HAND_NAME);
				}
				else if(GetEquipSlotType() == ITEM_EQUIP_R_HAND)
				{
					strcpy_s(PivotName, 64, ITEM_EQUIP_R_HAND_NAME);
				}				
				else if(GetEquipSlotType() == ITEM_EQUIP_R_EYE)
				{
					strcpy_s(PivotName, 64, ITEM_EQUIP_R_EYE_NAME);
				}
				else if(GetEquipSlotType() == ITEM_EQUIP_BACK)
				{
					strcpy_s(PivotName, 64, ITEM_EQUIP_BACK_NAME);
				}
				else if(GetEquipSlotType() == ITEM_EQUIP_MASK)
				{
					strcpy_s(PivotName, 64, ITEM_EQUIP_MASK_NAME);
				}
				else if(GetEquipSlotType() == ITEM_EQUIP_USER_MOVE1)
				{
					Ready2LinkBone = FALSE;

					m_pOwner->AttachBoneOffsetPosRootRotateY((CNtlPLAttach *)this, ITEM_EQUIP_USER_MOVE1_NAME, m_pProperty->GetAttachOffset());
				}
				else
				{
					Ready2LinkBone = FALSE;
				}

				if(Ready2LinkBone)
				{
					SCharScheduleResInfo* pInfo	= m_pOwner->GetCharScheduleResInfo();

					string MeshFileName;
					if(GetMeshFilePrefixName(MeshFileName))
					{
						sLINK_ITEM_TRASFORM* pGet = CLinkItemTrasformList::GetInstance().Get(MeshFileName, CNtlDefaultItemTable::GetCharType(pInfo->uiClass, pInfo->uiGender, pInfo->bIsAdult));
						if(pGet)
						{
							SPLAttachAttr TmpSPLAttachAttr;					
							CNtlMath::MathRwV3dAssign(&TmpSPLAttachAttr.vScale, pGet->_SCL[0], pGet->_SCL[1], pGet->_SCL[2]); 
							CNtlMath::MathRwV3dAssign(&TmpSPLAttachAttr.vRot, pGet->_ROT[0], pGet->_ROT[1], pGet->_ROT[2]);
							CNtlMath::MathRwV3dAssign(&TmpSPLAttachAttr.vOffsetPos, pGet->_POS[0], pGet->_POS[1], pGet->_POS[2]);

							m_pOwner->AttachBone((CNtlPLAttach *)this, PivotName, &TmpSPLAttachAttr);
						}
						else
						{
							m_pOwner->AttachBone((CNtlPLAttach*)this, PivotName);
						}
					}
					else
					{
						m_pOwner->AttachBone((CNtlPLAttach*)this, PivotName);
					}
				}

				if(bOwnerAddWorld)
				{
					AddWorld();
				}

				for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
				{
					RpAtomic *pAtomic = m_vecAtomicList[i]->pData;
					SetApplyEquipDataOwner(pAtomic, m_pOwner);
				}
			}
		}
		else
		{
			RwBool bOwnerAddWorld = m_pOwner->IsExistWorld();
			RemoveWorld();
			for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
			{
				RpAtomic *pAtomic = m_vecAtomicList[i]->pData;
				SetApplyEquipDataOwner(pAtomic, m_pOwner);
			}
			if(bOwnerAddWorld)
				AddWorld();
		}
		m_sScheduleResInfo.bApplyedEquip = TRUE;
	}
}

void CNtlPLItem::SetRemoveEquip()
{
	if( !m_sScheduleResInfo.bLoadComplete ||
		!m_sScheduleResInfo.bApplyedEquip )
		return;

	if(m_sScheduleResInfo.bAttach)
	{
		if( g_CheckItemBody(GetResType()) )
		{
			RwBool bOwnerAddWorld = m_pOwner->IsExistWorld();

			RpClump *pOwnerClump = m_pOwner->GetClump();
			
			if( pOwnerClump != NULL)
				m_pOwner->RemoveWorld();

			if( m_sScheduleResInfo.bApplyedEquip )
			{
				for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
				{
					RpAtomic *pAtomic = m_vecAtomicList[i]->pData;
					NTL_ASSERTE(pAtomic);
					RpClumpRemoveAtomic(pOwnerClump, pAtomic);
					RpAtomicSetFrame(pAtomic, m_vecAtomicList[i]->pFrame);
					RpSkinAtomicSetHAnimHierarchy(pAtomic, NULL);
				}
			}

			if(bOwnerAddWorld)
				m_pOwner->AddWorld();
		}
		else
		{
			RemoveWorld();
			
			if( m_sScheduleResInfo.bApplyedEquip )
				m_pOwner->Detach(this);
		}
	}
	else
	{
		RemoveWorld();
	}
	
	m_sScheduleResInfo.bApplyedEquip = FALSE;
	m_pOwner = NULL;
}



void CNtlPLItem::SetApplyEquipDataOwner(RpAtomic *pAtomic, CNtlPLCharacter *pOwner)
{
	SetSerialID(pOwner->GetSerialID());


	// Apparel color
	if( ITEM_EQUIP_JACKET == m_eInstanceEquipSlotType )
	{
		RwUInt8 byColorIndex = Logic_GetDogiColorIndex( pOwner->GetSerialID() );

		if( INVALID_BYTE == byColorIndex )
		{
			m_DogiEditedColor.red	= dDEFAULT_DOGI_COLOR_RED;
			m_DogiEditedColor.green = dDEFAULT_DOGI_COLOR_GREEN;
			m_DogiEditedColor.blue	= dDEFAULT_DOGI_COLOR_BLUE;
			m_DogiEditedColor.alpha = 255;
		}
		else
		{
			COLORREF colorREF = GetNtlPLPalette()->GetColor( PALETTETYPE_DOGI, byColorIndex );

			m_DogiEditedColor.red	= GetRValue(colorREF);
			m_DogiEditedColor.green	= GetGValue(colorREF);
			m_DogiEditedColor.blue	= GetBValue(colorREF);
			m_DogiEditedColor.alpha	= 255;
		}
	}


	SToonData *pToonData = pOwner->GetToonData();
	if( pToonData->pToonInk == NULL ||
		pToonData->pToonPaint == NULL )
		return;

	if( m_ToonData.pToonInk != NULL)
	{
		RpToonInkDestroy(m_ToonData.pToonInk);
		m_ToonData.pToonInk = NULL;
	}

	if( m_ToonData.pToonPaint != NULL)
	{
		RpToonPaintDestroy(m_ToonData.pToonPaint);
		m_ToonData.pToonPaint = NULL;
	}
	
	m_ToonData.pToonInk = pToonData->pToonInk;
	RpToonInkAddRef(m_ToonData.pToonInk);

	m_ToonData.pToonPaint = pToonData->pToonPaint;
	RpToonPaintAddRef(m_ToonData.pToonPaint);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// void	_rpToonAtomicChainAtomicRenderCallback(RpAtomic *atomic) CB is twisted here.
	// There are times when each atom in a dff that holds multiple atoms comes in differently.
	// For example, defaultrender or ToonAtomRender. By the way, m_fnDefRenderCallBack
	// If the current PLItem has 3 atoms, the first CB is stored in m_fnDefRenderCallBack
	// The following children are forced to put this m_fnDefRenderCallBack _rpToonAtomicChainAtomicRenderCallback
	// Here cb is twisted. Elder Nemek in the chair of the item, child not in the chair, above the chair and chair
	// Pool (blending) corresponds to this. When multiple atoms of dff are loaded into memory, the order is not always constant.
	// I have seen it. Also, the current chair mesh is exporting something wrong. All of the basic cbs' information is NULL.
	// It seems to be a problem. Sung Bong must help. I am worried.
	// prevents repeated calls due to dead call.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  	if(m_fnDefRenderCallBack == NULL)
//  	{
//  		m_fnDefRenderCallBack = RpAtomicGetRenderCallBack(pAtomic);
//  	}
//  	else
//  	{
//  		RpAtomicSetRenderCallBack(pAtomic, m_fnDefRenderCallBack);
//  	}
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// I will change it as follows.
	//RpAtomicSetRenderCallBack(pAtomic, RpAtomicGetRenderCallBack(pAtomic));
	RpAtomicSetRenderCallBack(pAtomic, AtomicDefaultRenderCallBack);

	// Toon설정
	Helper_SetToonAtomic(pAtomic, &m_ToonData);

	// RenderCallBack 설정
	//if(RpAtomicGetRenderCallBack(pAtomic) != CNtlPLCharacter::RenderCallBack)
	{
		RpNtlAtomicSetRenderCallBack( pAtomic, RpAtomicGetRenderCallBack(pAtomic) );
		RpAtomicSetRenderCallBack(pAtomic, CNtlPLCharacter::RenderCallBack);
	}

	// Cz : Character Atomic List
	if (pOwner)
	{
		pOwner->UpdateCharacterAtomicList();
	}
}

void CNtlPLItem::SetVisible( RwBool bVisible ) 
{
	m_sScheduleResInfo.bVisible = bVisible;
	if( !GetItemScheduleResInfo()->bLoadComplete )
		return;
	
	CNtlPLAttach::SetVisible(bVisible);
	
	if(!bVisible)
	{
		//m_bAnimUpdate = FALSE;
		m_bVisualEffect = FALSE;
	}  
	else
	{
		m_bAnimUpdate = TRUE;
		m_bVisualEffect = TRUE;
	}
}

RwMatrix* CNtlPLItem::GetBoneMatrix(const RwChar *pszBoneName)
{
	RwFrame *pFrame = GetBoneByName(pszBoneName);
	if(pFrame != NULL)
		return &pFrame->ltm;

	return NULL;
}

RwFrame *CNtlPLItem::GetBoneByName(const RwChar *pszBoneName)
{
	if(m_mapFrame.find( pszBoneName ) == m_mapFrame.end())
		return NULL;

	RwFrame *pFrame = NULL;

	if( g_CheckItemBody( GetResType() ) && m_pOwner)
	{
		pFrame = m_pOwner->GetBoneByName( pszBoneName );
	}
	else
	{
		pFrame = m_mapFrame[pszBoneName];
	}
	
	return pFrame;
}

void CNtlPLItem::SetApplyRunTimeAlpha()
{
	if( m_sColor.alpha >= 255 && GetWeightAlpha() >= 1.f)
	{
		if(!m_bRunTimeAlpha)
			return;

		m_bRunTimeAlpha = FALSE;
	}
	else
	{
		if(m_bRunTimeAlpha)
			return;

		m_bRunTimeAlpha = TRUE;
	}
	
	for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
	{
		RwUInt32 uiRenderFlag = RpNtlAtomicGetFlag(m_vecAtomicList[i]->pData);

		if( m_bRunTimeAlpha )
			RpNtlAtomicSetFlag(m_vecAtomicList[i]->pData, uiRenderFlag | NTL_RUNTIME_ALPHA);
		else
			RpNtlAtomicSetFlag(m_vecAtomicList[i]->pData, uiRenderFlag &(~NTL_RUNTIME_ALPHA));
	}
}

void CNtlPLItem::SetAlpha(RwUInt8 byValue)
{
	m_sColor.alpha = byValue;

	SetApplyRunTimeAlpha();
}

void CNtlPLItem::SetWeightAlpha(RwReal fWeightValue)
{
	CNtlPLAttach::SetWeightAlpha( fWeightValue );

	SetApplyRunTimeAlpha();
}

void CNtlPLItem::SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue)
{
	m_sColor.red   = byRed;
	m_sColor.green = byGreen;
	m_sColor.blue  = byBlue;
}

void CNtlPLItem::SetAddColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue)
{
	m_sAddColor.red		= byRed;
	m_sAddColor.green	= byGreen;
	m_sAddColor.blue	= byBlue;
}

RwBool IsColorMaterial(RwChar *chMatName)
{
	RwChar chBuffer[64];
	strcpy_s(chBuffer, 64, chMatName);
	for(int i =0; i < 64; i++)
	{
		if(chBuffer[i] == '_')
		{
			chBuffer[i] = 0;
			break;
		}
	}

	if(_stricmp(chBuffer, "Skin") == 0)
		return TRUE;

	return FALSE;
}

SToonData *CNtlPLItem::GetToonData()
{
	return &m_ToonData;
}

RwBool CNtlPLItem::RenderToTexture()
{
	if( m_pResourceClump == NULL ||
		m_pResourceClump->GetClump() == NULL )
		return FALSE;
	
	for( unsigned int i = 0; i < m_vecAtomicList.size(); i++)
	{
		RpAtomic *pAtomic = m_vecAtomicList[i]->pData;

		RwUInt32 uiRenderFlags = RpNtlAtomicGetFlag(pAtomic);
	
		if(uiRenderFlags & NTL_ALPHATEST)
			BeginNtlAotmicAlphaTestState();
		if(uiRenderFlags & NTL_TWOSIDE)
			BeginNtlAtomicTwoSideState();

		if ( uiRenderFlags & NTL_ALPHA)		UpdateAlphaState(TRUE);

		pAtomic->renderCallBack(pAtomic);

		if ( uiRenderFlags & NTL_ALPHA)		UpdateAlphaState(FALSE);

		if(uiRenderFlags & NTL_TWOSIDE)
			EndNtlAtomicTwoSideState();
		if(uiRenderFlags & NTL_ALPHATEST)
			EndNtlAtomicAlphaTestState();
	}
	
	return TRUE;
}

RwFrame* CNtlPLItem::GetFrame(void)
{
	if( m_pResourceClump == NULL)
		return NULL;

	RwFrame *pFrame = RpClumpGetFrame( m_pResourceClump->GetClump() );
	
	return pFrame;
}

CNtlInstanceEffect* CNtlPLItem::AttachLinkEffect(SEventLinkEffect* pEventLinkEffect)
{
    if(!pEventLinkEffect)
        return FALSE;

    return m_LinkEffectInstance.AttachLinkEffect(this, pEventLinkEffect);
}

RwBool CNtlPLItem::DetachLinkEffect(CNtlInstanceEffect* pLinkEffect)
{
    if(!pLinkEffect)
        return FALSE;

    return m_LinkEffectInstance.DetachLinkEffect(this, pLinkEffect);
}

VOID CNtlPLItem::SetEMUVController(RwBool _Activation)
{
	if(_Activation)
	{
		if(m_pEMUVController)
		{
			Helper_SetClumpAllAtomicsEMUV(m_pResourceClump->GetClump(), m_pEMUVController->GetEnvTex(), m_pEMUVController->GetTexMat(), m_pEMUVController->GetActivation());
		}
	}
	else
	{
		Helper_ResetClumpAllAtomicsEMUV(m_pResourceClump->GetClump());
	}
}

RwBool CNtlPLItem::Update( RwReal fElapsed ) 
{
	if( IsVisible() )
	{
		CNtlPLAttach::Update(fElapsed);
	}

    if(m_bAnimUpdate)
    {
        if(m_pAnimLayer)
        {
            m_pAnimLayer->Update(fElapsed);
			RpHAnimHierarchyUpdateMatrices(m_pBaseHierarchy);
        }
    }

	if(m_pEMUVController)
	{
		m_pEMUVController->Update(fElapsed);
	}

	return TRUE;
}

RwBool CNtlPLItem::Render( void ) 
{
	return TRUE;
}

/**
* 궤적 이펙트를 생성한다.
* \param pEventTrace 궤적 이펙트를 설정한 이벤트 객체의 포인터. 
*        NULL이면 Item Property에 설정된 궤적 이벤트 설정값을 사용한다.
* return 생성 성공 유무
*/
RwBool CNtlPLItem::ActiveTraceEffect( SEventTrace* pEventTrace) 
{
    // 테스트
    m_bEnableTrace = TRUE;

    if(!m_bEnableTrace || !pEventTrace)
        return FALSE;
	
    for(int i = 0; i < 2; ++i)
    {
        RwFrame* pFrame = GetBoneByName(g_strTraceBoneName[i * 2]);
        if(!pFrame)
            continue;

        CNtlInstanceTraceSystem* pTraceSystem = NTL_NEW CNtlInstanceTraceSystem();

        if(pEventTrace->eTraceKind == SEventTrace::ITEM_TRACE)
        {
            m_pProperty->m_eventTrace.fTime = pEventTrace->fTime;
            m_pProperty->m_eventTrace.fLifeTime = pEventTrace->fLifeTime;
            m_pProperty->m_eventTrace.fEdgeLifeTime = pEventTrace->fEdgeLifeTime;

            // 무기의 Bone을 설정한다.
            strcpy_s(m_pProperty->m_eventTrace.strStartBoneName, MAX_DEFAULT_NAME, g_strTraceBoneName[i * 2]);
            strcpy_s(m_pProperty->m_eventTrace.strEndBoneName, MAX_DEFAULT_NAME, g_strTraceBoneName[i * 2 + 1]);

            pTraceSystem->Create(&m_pProperty->m_eventTrace, this);
        }
        else if(pEventTrace->eTraceKind == SEventTrace::EVENT_TRACE)
        {
            // 무기의 Bone을 설정한다.
            strcpy_s(pEventTrace->strStartBoneName, MAX_DEFAULT_NAME, g_strTraceBoneName[i * 2]);
            strcpy_s(pEventTrace->strEndBoneName, MAX_DEFAULT_NAME, g_strTraceBoneName[i * 2 + 1]);

            pTraceSystem->Create(pEventTrace, this);
        }

        CNtlInstanceEffect* pTraceEffect = NTL_NEW CNtlInstanceEffect();
        pTraceEffect->SetName(EFFECT_NAME_TRACE);        
        pTraceEffect->SetFlags(NTL_PLEFLAG_NOT_PROPERTY_USED);        
        pTraceEffect->AddComponentSystem(pTraceSystem);
        GetSceneManager()->AddPLEntity(pTraceEffect);
        Attach(pTraceEffect);
    }

	return TRUE;
}

RwBool CNtlPLItem::CreateAnimData(RwUInt32 uiAnimKey)
{
    // Item은 Animation이 적용되지 않은것이 많기 때문에, 필요한 경우에만 Animation 관련 데이터들을 생성한다.
    
    NTL_FUNCTION(__FUNCTION__);
    if(!m_pResourceClump)
        NTL_RETURN(FALSE);

    // Instance Anim Table 생성
    m_pInstanceAnimTable = NTL_NEW CNtlInstanceAnimTable();
    m_pInstanceAnimTable->Create(m_pProperty->GetAnimTable());
    SInstanceAnimData* pInstanceAnimData = m_pInstanceAnimTable->Get(uiAnimKey);
    if(!pInstanceAnimData)
    {
        m_pInstanceAnimTable->Destroy();
        NTL_DELETE(m_pInstanceAnimTable);
        NTL_RETURN(FALSE);
    }

    // Hierarchy 설정
    m_pBaseHierarchy = Helper_GetHierarchyClump( m_pResourceClump->GetClump() );
    if(!m_pBaseHierarchy)
        NTL_RETURN(FALSE);

    Helper_SetHierarchyClumpSkin( m_pResourceClump->GetClump() , m_pBaseHierarchy);
	RpHAnimHierarchySetCurrentAnim(m_pBaseHierarchy, pInstanceAnimData->pResource->GetAnimation());

    RpHAnimHierarchySetFlags( m_pBaseHierarchy,
        (RpHAnimHierarchyFlag)(RpHAnimHierarchyGetFlags(m_pBaseHierarchy) | 
        rpHANIMHIERARCHYUPDATELTMS | 
        rpHANIMHIERARCHYUPDATEMODELLINGMATRICES));

    RpHAnimHierarchyAttach(m_pBaseHierarchy);
    RpHAnimUpdateHierarchyMatrices(m_pBaseHierarchy);

    m_pAnimLayer = NTL_NEW CNtlAnimLayer();
    if(!m_pAnimLayer->Create(m_pBaseHierarchy))
        NTL_RETURN(FALSE);

    m_pAnimLayer->SetCallBack(this, &CNtlPLItem::CallBackBaseAnim);

	NTL_RETURN(TRUE);
}

RwBool CNtlPLItem::SetAnimation( RwUInt32 uiAnimKey, RwReal fStartTime /*= 0.0f*/, RwBool bLoop /*= TRUE*/ ) 
{
    NTL_FUNCTION(__FUNCTION__);

    if(!IsSchedulingLoadingComplete())
    {
        m_sScheduleResInfo.uiAnimKey = uiAnimKey;
    }

    if(!m_pAnimLayer)
    {
        if(!CreateAnimData(uiAnimKey))
            NTL_RETURN(FALSE);
    }
	m_uiCurAnimKey = uiAnimKey;
    SInstanceAnimData* pInstanceAnimData = m_pInstanceAnimTable->Get(uiAnimKey);
    if(!pInstanceAnimData)
        NTL_RETURN(FALSE);
	m_bAnimUpdate = TRUE;
	
	NTL_RETURN(m_pAnimLayer->SetAnimation(pInstanceAnimData));
}

int CNtlPLItem::CallBackBaseAnim(void* pEventData ) 
{
    SEventAnim* pEventAnim = (SEventAnim*)pEventData;

    switch(pEventAnim->eEventID)
    {
		case EVENT_ANIM_END:
        {
            SEventAnimEnd* pAnimEnd = (SEventAnimEnd*)pEventData;
 
			if(pAnimEnd->uiAnimKey == ITEM_ATTACK)
				SetAnimation(ITEM_IDLE);
        }
		break;
    }

	return TRUE;
}

RwReal CNtlPLItem::GetBaseDurationAnimTime() 
{
    if(!m_pAnimLayer)
        return 0.0f;

    return m_pAnimLayer->GetDurationAnimTime();
}

RwBool CNtlPLItem::AttachVisualEffect(CNtlInstanceEffect* pInstanceEffect, SEventVisualEffect* pEventVisualEffect)
{
    if(!pInstanceEffect || !pEventVisualEffect)
        return FALSE;

    m_bVisualEffect = TRUE;

	pInstanceEffect->SetSerialID(GetSerialID());

    // Item의 Visual Effect는 무조건 Item의 Bone에 붙는다.
    return AttachBone(pInstanceEffect, pEventVisualEffect->chBoneName);
}

VOID CNtlPLItem::SetEMUV()
{
	//if(GetResType() != ITEM_RES_EQUIP_BODY)
	//{
	//	return;
	//}

	//if(GetEquipSlotType() == ITEM_EQUIP_TWO_HAND)
	//{
	//	return;
	//}

	//if(!Helper_IsArmorType(m_pResourceClump->GetClump()))
	//{
	//	return;
	//}

	CNtlPLItemEMUVController::SetActiveController(m_pEMUVController, m_nGrade);
}

RwBool CNtlPLItem::SetUpgradeEffect(ENtlPLItemGrade eGrade)
{
    //if(m_nGrade == eGrade)
    //    return TRUE;

    m_nGrade = eGrade;

    SetEMUV();

	if (!m_pProperty)
		return FALSE;

    // Erase existing effects.
    if(!m_pUpgradeEffects.empty())
    {
        for each(CNtlInstanceEffect* pEffect in m_pUpgradeEffects)
        {
            if(pEffect)
                pEffect->Finish();
        }
        m_pUpgradeEffects.clear();
    }        

	// 1,2 Upgrades do not display effects.
    if(eGrade < ITEM_GRADE_3)
    {
        return TRUE;
    }

    CNtlInstanceEffect* pGradeEffect = NULL; 

	//// creates upgrade effects for each
	//switch (eGrade)
	//{
	//	case ITEM_GRADE_3: pGradeEffect = (CNtlInstanceEffect*)GetSceneManager()->CreateEntity(PLENTITY_EFFECT, m_pProperty->GetUpgradeEffectProperty()->szEffect1);
	//	case ITEM_GRADE_4: pGradeEffect = (CNtlInstanceEffect*)GetSceneManager()->CreateEntity(PLENTITY_EFFECT, m_pProperty->GetUpgradeEffectProperty()->szEffect2);
	//	case ITEM_GRADE_5: pGradeEffect = (CNtlInstanceEffect*)GetSceneManager()->CreateEntity(PLENTITY_EFFECT, m_pProperty->GetUpgradeEffectProperty()->szEffect3);
	//	case ITEM_GRADE_6: pGradeEffect = (CNtlInstanceEffect*)GetSceneManager()->CreateEntity(PLENTITY_EFFECT, m_pProperty->GetUpgradeEffectProperty()->szEffect4);
	//	case ITEM_GRADE_7: pGradeEffect = (CNtlInstanceEffect*)GetSceneManager()->CreateEntity(PLENTITY_EFFECT, m_pProperty->GetUpgradeEffectProperty()->szEffect5);
	//	case ITEM_GRADE_8: pGradeEffect = (CNtlInstanceEffect*)GetSceneManager()->CreateEntity(PLENTITY_EFFECT, m_pProperty->GetUpgradeEffectProperty()->szEffect6);
	//	case ITEM_GRADE_9: pGradeEffect = (CNtlInstanceEffect*)GetSceneManager()->CreateEntity(PLENTITY_EFFECT, m_pProperty->GetUpgradeEffectProperty()->szEffect7);
	//	case ITEM_GRADE_10: pGradeEffect = (CNtlInstanceEffect*)GetSceneManager()->CreateEntity(PLENTITY_EFFECT, m_pProperty->GetUpgradeEffectProperty()->szEffect8);
	//	case ITEM_GRADE_11: pGradeEffect = (CNtlInstanceEffect*)GetSceneManager()->CreateEntity(PLENTITY_EFFECT, m_pProperty->GetUpgradeEffectProperty()->szEffect9);
	//	case ITEM_GRADE_12: pGradeEffect = (CNtlInstanceEffect*)GetSceneManager()->CreateEntity(PLENTITY_EFFECT, m_pProperty->GetUpgradeEffectProperty()->szEffect10);
	//	case ITEM_GRADE_13: pGradeEffect = (CNtlInstanceEffect*)GetSceneManager()->CreateEntity(PLENTITY_EFFECT, m_pProperty->GetUpgradeEffectProperty()->szEffect11);
	//	case ITEM_GRADE_14: pGradeEffect = (CNtlInstanceEffect*)GetSceneManager()->CreateEntity(PLENTITY_EFFECT, m_pProperty->GetUpgradeEffectProperty()->szEffect12);
	//	case ITEM_GRADE_15: pGradeEffect = (CNtlInstanceEffect*)GetSceneManager()->CreateEntity(PLENTITY_EFFECT, m_pProperty->GetUpgradeEffectProperty()->szEffect13);
	//}

	if( !pGradeEffect )
		return TRUE;

	if( pGradeEffect->IsAutoDelete() )
	{
#ifdef MODEL_TOOL
		::MessageBox(NULL, "[Error] Grade Effect Must Loop Effect", "", MB_OK);
#endif

		NTL_ASSERTFAIL("Grade Effect Must Loop Effect - " << pGradeEffect->GetName());
		return FALSE;        
	}

    // First bone
	if (strlen(m_pProperty->GetUpgradeEffectProperty()->szBone1) > 0)
	{
		SPLAttachAttr sAttachAttr;
		sAttachAttr.vOffsetPos = m_pProperty->GetUpgradeEffectProperty()->vOffset1;
		AttachBone(pGradeEffect, m_pProperty->GetUpgradeEffectProperty()->szBone1, &sAttachAttr);
		m_pUpgradeEffects.push_back(pGradeEffect);
		pGradeEffect->SetVisible(m_bEnableUpgradeEffect);
	}

    // If the second bone is set, add an Effect.
    if(strlen(m_pProperty->GetUpgradeEffectProperty()->szBone2) > 0)
    {
        pGradeEffect = (CNtlInstanceEffect*)GetSceneManager()->CreateEntity(PLENTITY_EFFECT, pGradeEffect->GetName());

        SPLAttachAttr sAttachAttr2;
        sAttachAttr2.vOffsetPos = m_pProperty->GetUpgradeEffectProperty()->vOffset2;
        AttachBone(pGradeEffect, m_pProperty->GetUpgradeEffectProperty()->szBone2, &sAttachAttr2);
        m_pUpgradeEffects.push_back(pGradeEffect);
        pGradeEffect->SetVisible(m_bEnableUpgradeEffect);
    }

    return TRUE;
}

void CNtlPLItem::SetEnableUpgradeEffect( RwBool bEnable ) 
{   
    if(m_bEnableUpgradeEffect == bEnable)
        return;

    m_bEnableUpgradeEffect = bEnable;

    for each(CNtlInstanceEffect* pEffect in m_pUpgradeEffects)
    {
        if(pEffect)
            pEffect->SetVisible(bEnable);
    }    
}

RwBool CNtlPLItem::AttachDogiEmblem(RwTexture* _pTexture)
{
	NTL_ASSERTE(_pTexture != NULL);

	// Current mesh should be DOGI if it's not, nothing will happen
	m_pDogiEmblemTex = _pTexture;

	return TRUE;
}

RwBool CNtlPLItem::DetachDogiEmblem()
{
	m_pDogiEmblemTex = NULL;

	return TRUE;
}

RwBool CNtlPLItem::SetDogiBeltColor(RwRGBA& _BeltRGBA)
{
	m_DogiBeltClr.red	= _BeltRGBA.red; 
	m_DogiBeltClr.green	= _BeltRGBA.green; 
	m_DogiBeltClr.blue	= _BeltRGBA.blue; 

	return TRUE;
}

VOID CNtlPLItem::SetDogiEditedColor(RwRGBA& _DogiEditedColor)
{
	m_DogiEditedColor.red	= _DogiEditedColor.red; 
	m_DogiEditedColor.green	= _DogiEditedColor.green; 
	m_DogiEditedColor.blue	= _DogiEditedColor.blue;
	m_DogiEditedColor.alpha = 255;
}

void CNtlPLItem::CreatePetrifyMaterial()
{
	if(m_PetrifyMaterial)
	{
		for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
		{
			RpGeometry *pGeom = RpAtomicGetGeometry(m_vecAtomicList[i]->pData);
			if(pGeom)
			{
				RwInt32 nMatCount = RpGeometryGetNumMaterials(pGeom);

				for(RwInt32 m = 0; m < nMatCount; m++)
				{
					RpMaterial *pMaterial = RpGeometryGetMaterial(pGeom, m);
					if(pMaterial)
						RpNtlMaterialExtSetFlag(pMaterial, RpNtlMaterialExtGetFlag(pMaterial) | rpNTL_MATERIAL_PETRIFY);
				}
			}	
		}
	}
}

void CNtlPLItem::RestorePetrifyMaterial()
{
	// careful for material sharing, must be reset
	for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
	{
		RpGeometry *pGeom = RpAtomicGetGeometry(m_vecAtomicList[i]->pData);
		if(pGeom)
		{
			RwInt32 nMatCount = RpGeometryGetNumMaterials(pGeom);

			for(RwInt32 m = 0; m < nMatCount; m++)
			{
				RpMaterial *pMaterial = RpGeometryGetMaterial(pGeom, m);
				if(pMaterial)
					RpNtlMaterialExtSetFlag(pMaterial, RpNtlMaterialExtGetFlag(pMaterial) & ~rpNTL_MATERIAL_PETRIFY);
			}
		}	
	}
}

void CNtlPLItem::CreateSimpleMaterial()
{
	if(m_SimpleMaterial)
	{
		for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
		{
			RpGeometry *pGeom = RpAtomicGetGeometry(m_vecAtomicList[i]->pData);
			if(pGeom)
			{
				RwInt32 nMatCount = RpGeometryGetNumMaterials(pGeom);

				for(RwInt32 m = 0; m < nMatCount; m++)
				{
					RpMaterial *pMaterial = RpGeometryGetMaterial(pGeom, m);
					if(pMaterial)
						RpNtlMaterialExtSetFlag(pMaterial, RpNtlMaterialExtGetFlag(pMaterial) | rpNTL_MATERIAL_SIMPLEMATERIAL);
				}
			}	
		}
	}
}

void CNtlPLItem::RestoreSimpleMaterial()
{
	if(m_SimpleMaterial)
	{
		for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
		{
			RpGeometry *pGeom = RpAtomicGetGeometry(m_vecAtomicList[i]->pData);
			if(pGeom)
			{
				RwInt32 nMatCount = RpGeometryGetNumMaterials(pGeom);

				for(RwInt32 m = 0; m < nMatCount; m++)
				{
					RpMaterial *pMaterial = RpGeometryGetMaterial(pGeom, m);
					if(pMaterial)
						RpNtlMaterialExtSetFlag(pMaterial, RpNtlMaterialExtGetFlag(pMaterial) & ~rpNTL_MATERIAL_SIMPLEMATERIAL);
				}
			}	
		}
	}

	m_SimpleMaterial = FALSE;
}

VOID CNtlPLItem::SetToonEdgeVisibility4CurEquipItem(RwBool _Visibility)
{
	for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
	{
		if(_Visibility)
			RpNtlAtomicSetFlag(m_vecAtomicList[i]->pData, RpNtlAtomicGetFlag(m_vecAtomicList[i]->pData) & ~NTL_TOON_EDGE_OFF);
		else
			RpNtlAtomicSetFlag(m_vecAtomicList[i]->pData, RpNtlAtomicGetFlag(m_vecAtomicList[i]->pData) | NTL_TOON_EDGE_OFF);
	}
}

RwBool CNtlPLItem::IsHaveAnimData() 
{
    if(m_pProperty && m_pProperty->GetAnimTable()->GetTypeAnimMap()->size())
        return TRUE;

    return FALSE;    
}

#ifdef dNTL_WORLD_CULLING_NEW
RwBool CNtlPLItem::CullingTest(RwCamera* pRwCamera, RwUInt16 uiRenderFrame)
{
	if (m_pOwner)
	{
		return m_pOwner->CullingTest(pRwCamera, uiRenderFrame);
	}
	return CNtlPLEntity::CullingTest(pRwCamera, uiRenderFrame);
}
#endif
