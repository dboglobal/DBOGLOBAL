#include "StdAfx.h"
#include "ModelToolApplication.h"

#include "MTItem.h"

CMTItem::CMTItem(void)
{
    SetFlags(NTL_PLEFLAG_PICKING | NTL_PLEFLAG_ATTACH);
    SetClassType(PLENTITY_ITEM);

    m_pProperty = NULL;
    m_pProperty = new CNtlPLItemProperty();

    m_bEnableTrace = TRUE;    
    m_bEquipView = FALSE;
}

CMTItem::~CMTItem(void)
{
    Destroy();
    NTL_DELETE(m_pProperty);
}

void* CMTItem::operator new(size_t size)
{
    return ::operator new(size);
}

void CMTItem::operator delete(void* pObj)
{
    ::operator delete(pObj);
}

RwBool CMTItem::Create(const RwChar* szDffFileName, RwBool bEquip /* = FALSE */)
{
   if(!szDffFileName)
       return FALSE;

   if(m_pResourceClump)
   {       
       Destroy();       
   }

   m_pResourceClump = CNtlPLResourceManager::GetInstance()->LoadClump(szDffFileName, CModelToolApplication::GetInstance()->GetTexturePath());
   if(!m_pResourceClump)
       return FALSE;

   // Atomic List를 보관을 하고 있는다. m_pOwner에 장착이 되는 Skin인 경우는
   // Clone을 한다 -> 왜? 캐릭터의 Clump가 atomic을 들고 있고, Item 객체도 Atomic을 들고 있기 때문에
   // Clone을 하지 않으면 두번 delete되어 뻑이난다.   
   if(bEquip && g_CheckItemBody(GetResType()) )
   {
       Helper_SetClumpAllAtomics(m_pResourceClump->GetClump(), &m_vecAtomicList, true);
   }
   else
   {
       Helper_SetClumpAllAtomics(m_pResourceClump->GetClump(), &m_vecAtomicList, false);
   }
   
   Helper_GetBoneList(m_pResourceClump->GetClump(), &m_mapFrame);

   for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
   {
       RpAtomic *pAtomic = m_vecAtomicList[i]->pData;
       RpNtlAtomicSetData(pAtomic, this);
       Helper_SetAtomicAllMaterialSkinInfo(pAtomic);
       
       RpGeometry *pGeom = RpAtomicGetGeometry(pAtomic);
       if(pGeom)
       {
           RwInt32 nMatCount = RpGeometryGetNumMaterials(pGeom);
           for(int i = 0; i < nMatCount; i++)
           {//
               RpMaterial *pMaterial = RpGeometryGetMaterial(pGeom, i);
               if(pMaterial)
               {
                   // get through the proper renderpipe
                   RpNtlMaterialSetRenderCB(pMaterial, CNtlPLCharacter::fpRenderCB);
               }
           }
       }
   }


   m_sScheduleResInfo.bLoadComplete = TRUE;
   m_sScheduleResInfo.bVisible = TRUE;   
   m_eInstanceEquipSlotType = m_pProperty->GetEquipSlotType();
   m_bEquipView = bEquip;
   AddWorld();

   if(!m_MTClump.Create(m_pResourceClump->GetClump(), (RwChar*)szDffFileName))
       return FALSE;

   m_MTClump.SetCartoon();
   
   // Link Effect를 설정한다.
   for(UINT i = 0; i < m_pProperty->m_vLinkEffect.size(); ++i)
   {
       SEventLinkEffect* pEventLinkEffect = m_pProperty->m_vLinkEffect[i];       
       AttachLinkEffect(pEventLinkEffect);
   }

   SetPosition(&m_pProperty->GetAttachOffset());

   SetEMUV();

   return TRUE;
}

void CMTItem::Destroy( void ) 
{
    CNtlPLItem::Destroy();
}

CNtlInstanceEffect* CMTItem::GetLinkEffectFromName(RwChar* szName)
{
    return m_LinkEffectInstance.GetLinkEffectFromName(szName);
}


void CMTItem::SetRemoveEquip() 
{
    if( g_CheckItemBody(GetResType()) )
    {
	    RpClump *pOwnerClump = m_pOwner->GetClump();            
        
	    m_pOwner->RemoveWorld();
	    for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
	    {
		    RpAtomic *pAtomic = m_vecAtomicList[i]->pData;
		    NTL_ASSERTE(pAtomic);
		    RpClumpRemoveAtomic(pOwnerClump, pAtomic);                
            RpAtomicSetFrame(pAtomic, m_vecAtomicList[i]->pFrame);
            RpSkinAtomicSetHAnimHierarchy(pAtomic, NULL);		    
	    }
	    m_pOwner->AddWorld();
    }
    else
    {
	    SetVisible(FALSE);
	    m_pOwner->Detach(this);
    }

    m_sScheduleResInfo.bApplyedEquip = FALSE;        
	m_pOwner = NULL;        
}

void CMTItem::SetVisible( RwBool bVisible ) 
{
    if(!m_pResourceClump)
        return;

    if(!m_pResourceClump->GetClump() || !CNtlPLGlobal::m_pRpWorld)
        return;

    m_sScheduleResInfo.bVisible = bVisible;

    CNtlPLAttach::SetVisible(bVisible);
    
    // Link Effect도 지운다.
    m_LinkEffectInstance.SetVisible(bVisible);    

    if(!bVisible)
    {
        m_bAnimUpdate = FALSE;
        m_bVisualEffect = FALSE;
    }  
    else
    {
        m_bAnimUpdate = TRUE;
    }
}
    
void CMTItem::AddWorld( void ) 
{
    if(!m_bEquipView || !g_CheckItemBody(GetResType()))
    {
        if(RpClumpGetWorld( m_pResourceClump->GetClump() ) == NULL)
            RpWorldAddClump(CNtlPLGlobal::m_pRpWorld, m_pResourceClump->GetClump());
    }    
}

void CMTItem::RemoveWorld() 
{
    if(m_pResourceClump && (!m_bEquipView || g_CheckItemBody(GetResType())))
    {
        if(RpClumpGetWorld( m_pResourceClump->GetClump() ))
            RpWorldRemoveClump(CNtlPLGlobal::m_pRpWorld, m_pResourceClump->GetClump());
    }
}