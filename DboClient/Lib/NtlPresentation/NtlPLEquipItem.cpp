#include "precomp_ntlpresentation.h"
#include "NtlPLEquipItem.h"
#include "NtlPLSceneManager.h"
#include "NtlDefaultItemData.h"
#include "NtlPLCharacter.h"
#include "NtlItem.h"


CNtlPLEquipItem::CNtlPLEquipItem() : m_pOwner(NULL), 
m_fFaceTexAniDefaultChangeTime(2.f),
m_fFaceTexAniChangeTime(4.f),
m_fFaceTexAniDisplayTime(0.12f),

m_fFaceTexAniCounter(0.f),
m_iFaceTexAniNum(0)
{
	m_pFaceTexture[0] = NULL;
	m_pFaceTexture[1] = NULL;

	m_pBaseFaceTexture = NULL;
	m_pHeadTexture = NULL;

	m_bFaceChange = TRUE;
}

CNtlPLEquipItem::~CNtlPLEquipItem() 
{

}

void CNtlPLEquipItem::AddItem(EItemEquipSlotType _eIEST)
{
	CNtlPLItem* pDefSkinItem = GetDefItem(_eIEST);

	if(pDefSkinItem != NULL)
	{
		AddItem(pDefSkinItem);
	}
}

void CNtlPLEquipItem::RemoveItem(EItemEquipSlotType _eIEST)
{
	EQUIP_ITEM_LIST_ITER The = m_listItem.begin();
	while(The != m_listItem.end())
	{
		CNtlPLItem* plistItem = *The;
		if(plistItem->GetEquipSlotType() == _eIEST)
		{
			plistItem->SetRemoveEquip();
			m_listItem.erase(The);

			return;
		}

		++The;
	}
}

void CNtlPLEquipItem::AddCItem(CNtlPLItem* _pItem, RwBool _bAttach, RwUInt32 _CBitSet)
{
	NTL_ASSERTE(_pItem != NULL);

	if(_CBitSet & eCBSTS_JACKET)
		RemoveItem(ITEM_EQUIP_JACKET);
	if(_CBitSet & eCBSTS_PANTS)
		RemoveItem(ITEM_EQUIP_PANTS);
	if(_CBitSet & eCBSTS_BOOTS)
		RemoveItem(ITEM_EQUIP_BOOTS);
	if(_CBitSet & eCBSTS_TWO_HAND)
		RemoveItem(ITEM_EQUIP_TWO_HAND);
	if(_CBitSet & eCBSTS_HEAD)
		RemoveItem(ITEM_EQUIP_HEAD);
	if (_CBitSet & eCBSTS_FACE)
		RemoveItem(ITEM_EQUIP_FACE);

	_pItem->SetChangeEquip(m_pOwner, _bAttach);
	m_listItem.push_back(_pItem);
}

void CNtlPLEquipItem::RemoveCItem(CNtlPLItem* _pItem, RwUInt32 _CBitSet)
{
	NTL_ASSERTE(_pItem != NULL);

	RemoveItem(_pItem);

	if(_CBitSet & eCBSTS_JACKET)	AddItem(ITEM_EQUIP_JACKET);
	if(_CBitSet & eCBSTS_PANTS)		AddItem(ITEM_EQUIP_PANTS);
	if(_CBitSet & eCBSTS_BOOTS)		AddItem(ITEM_EQUIP_BOOTS);
	if(_CBitSet & eCBSTS_TWO_HAND)	AddItem(ITEM_EQUIP_TWO_HAND);
	if(_CBitSet & eCBSTS_HEAD)		AddItem(ITEM_EQUIP_HEAD);
	if(_CBitSet & eCBSTS_FACE)		AddItem(ITEM_EQUIP_FACE);
}

void CNtlPLEquipItem::AddItem(CNtlPLItem *pItem, RwBool bAttach)
{
	NTL_ASSERTE(pItem != NULL);

	EQUIP_ITEM_LIST_ITER it = m_listItem.begin();
	EItemEquipSlotType	eEquipSlotType	= pItem->GetEquipSlotType();

	// Body는 Slot에 하나만 장착이 가능 하다.
	for(; it != m_listItem.end(); ++it)
	{
		CNtlPLItem *plistItem = *it;
		if( g_CheckItemBody(plistItem->GetResType()) )
		{
			if(plistItem->GetEquipSlotType() == eEquipSlotType)
			{
				plistItem->SetRemoveEquip();
				m_listItem.erase(it);
				break;
			}
		}
	}

	pItem->SetChangeEquip(m_pOwner, bAttach);
	m_listItem.push_back(pItem);
}

void CNtlPLEquipItem::RemoveItem(CNtlPLItem *pItem)
{
	EQUIP_ITEM_LIST_ITER it = m_listItem.begin();
	EItemResType	eResType = pItem->GetResType();
	EItemEquipSlotType	eEquipSlotType	= pItem->GetEquipSlotType();

	for(; it != m_listItem.end(); it++)
	{
		CNtlPLItem *plistItem = *it;
		if(plistItem == pItem)
		{
			pItem->SetRemoveEquip();
			m_listItem.erase(it);		
			break;
		}
	}

	//If it is a Body Item, replace it with the Default Item.
	if(g_CheckItemBody(eResType))
	{
		CNtlPLItem *pDefSkinItem = GetDefItem(eEquipSlotType);
        if(pDefSkinItem)
        {
            AddItem(pDefSkinItem);
            pDefSkinItem->SetVisible(m_pOwner->IsVisible(0));	
        }
	}
}

VOID CNtlPLEquipItem::SetToonEdgeVisibility4CurEquipItem(RwBool _Visibility)
{
	for(EQUIP_ITEM_LIST_ITER it = m_listItem.begin(); it != m_listItem.end(); ++it)
	{
		static_cast<CNtlPLItem*>(*it)->SetToonEdgeVisibility4CurEquipItem(_Visibility);
	}

	for(DEFAULT_ITEM_MAP_ITER it = m_mapDefaultItem.begin(); it != m_mapDefaultItem.end(); ++it)
	{
		static_cast<CNtlPLItem*>(it->second)->SetToonEdgeVisibility4CurEquipItem(_Visibility);
	}
}

CNtlPLItem *CNtlPLEquipItem::GetDefItem(RwUInt32 uiEquip)
{
	if(m_mapDefaultItem.find( uiEquip ) == m_mapDefaultItem.end())
		return NULL;

	return m_mapDefaultItem[uiEquip];
}

void CNtlPLEquipItem::SetDefItem(CNtlPLItem *pItem, RwUInt32 uiEquip)
{
	if(m_mapDefaultItem.find( uiEquip ) == m_mapDefaultItem.end())
	{
		m_mapDefaultItem[uiEquip] = pItem;
		AddItem(pItem);
	}
	else
	{
		CNtlPLItem *pEquipItem;
		pEquipItem = m_mapDefaultItem[uiEquip];

		if(pEquipItem != NULL)
		{
			pEquipItem->SetRemoveEquip();
			GetSceneManager()->DeleteEntity(pEquipItem);

			m_mapDefaultItem[uiEquip] = pItem;

			AddItem(pItem);
		}

	}
}

void CNtlPLEquipItem::Destroy()
{
	DEFAULT_ITEM_MAP_ITER stMapIT  = m_mapDefaultItem.begin();
	DEFAULT_ITEM_MAP_ITER endMapIT = m_mapDefaultItem.end();
	for(;stMapIT != endMapIT;)
	{
		CNtlPLItem *pItem = stMapIT->second;
		if(pItem != NULL)
		{
			GetSceneManager()->DeleteEntity(pItem);
			pItem = NULL;
		}

		++stMapIT;
	}
	m_mapDefaultItem.clear();

	//Item 관련 Instance는 Simulation 쪽에서 해제를 한다.
	m_listItem.clear();

	if( m_pBaseFaceTexture != NULL)
		CNtlPLResourceManager::GetInstance()->UnLoadTexture( m_pBaseFaceTexture );

	if( m_pFaceTexture[0] != NULL )
		CNtlPLResourceManager::GetInstance()->UnLoadTexture( m_pFaceTexture[0] );

	if( m_pFaceTexture[1] != NULL )
		CNtlPLResourceManager::GetInstance()->UnLoadTexture( m_pFaceTexture[1] );

	if( m_pHeadTexture != NULL )
		CNtlPLResourceManager::GetInstance()->UnLoadTexture( m_pHeadTexture );
}

void CNtlPLEquipItem::SetApplyFaceData(CNtlPLItem *pItem)
{
	if( m_sScheduleResInfo.bApplyedFace )
		return;

	EClassGenderType eClassGenderType = CNtlDefaultItemTable::GetClassGenderType( m_sScheduleResInfo.uiRace, m_sScheduleResInfo.uiGender, m_sScheduleResInfo.bIsAdult);

	std::string *strTexName = NULL;
	std::string strTexPath = "Texture\\Item\\;";

	strTexName = NULL;
	/*if(eClassGenderType == NAMEK_ADULT || eClassGenderType == NAMEK_CHILD)
		strTexName = CNtlDefaultItemTable::GetInstance().GetFaceBaseTextureName(eClassGenderType, m_sScheduleResInfo.uiFaceType, m_sScheduleResInfo.uiSkinColorType);
	else*/
		strTexName = CNtlDefaultItemTable::GetInstance().GetFaceBaseTextureName(eClassGenderType, m_sScheduleResInfo.uiFaceType, DEF_ITEM_START);
	if(!strTexName)
		return;

	if( strTexName->size() )
		m_pBaseFaceTexture = CNtlPLResourceManager::GetInstance()->LoadTexture(strTexName->c_str(), strTexPath.c_str() );

	/*if(eClassGenderType == NAMEK_ADULT || eClassGenderType == NAMEK_CHILD)
		strTexName = CNtlDefaultItemTable::GetInstance().GetFaceMultiTextureName(eClassGenderType, m_sScheduleResInfo.uiFaceType, m_sScheduleResInfo.uiSkinColorType);    
	else*/
		strTexName = CNtlDefaultItemTable::GetInstance().GetFaceMultiTextureName(eClassGenderType, m_sScheduleResInfo.uiFaceType, DEF_ITEM_START);

	m_pFaceTexture[0] = CNtlPLResourceManager::GetInstance()->LoadTexture(strTexName->c_str(), strTexPath.c_str());

	/*if(eClassGenderType == NAMEK_ADULT || eClassGenderType == NAMEK_CHILD)
		strTexName = CNtlDefaultItemTable::GetInstance().GetFaceAniMultiTextureName(eClassGenderType, m_sScheduleResInfo.uiFaceType, m_sScheduleResInfo.uiSkinColorType);
	else*/
		strTexName = CNtlDefaultItemTable::GetInstance().GetFaceAniMultiTextureName(eClassGenderType, m_sScheduleResInfo.uiFaceType, DEF_ITEM_START);

	m_pFaceTexture[1] = CNtlPLResourceManager::GetInstance()->LoadTexture(strTexName->c_str(), strTexPath.c_str());

	RwRGBA *pSkinColor = CNtlDefaultItemTable::GetInstance().GetSkinColor(eClassGenderType, m_sScheduleResInfo.uiSkinColorType);
	if(pSkinColor != NULL)
		m_pOwner->SetSkinColor(pSkinColor->red, pSkinColor->green, pSkinColor->blue );


	if(m_pBaseFaceTexture)
		pItem->SetBaseTexture(m_pBaseFaceTexture);

	if( m_iFaceTexAniNum == 0 )
		m_iFaceTexAniNum = 1;
	else
		m_iFaceTexAniNum = 0;

	if( m_pFaceTexture[m_iFaceTexAniNum] != NULL)
		pItem->SetMultiTexture( m_pFaceTexture[m_iFaceTexAniNum] );

	m_sScheduleResInfo.bApplyedFace = TRUE;
}

void CNtlPLEquipItem::SetApplyHeadData(CNtlPLItem *pItem)
{
	if( m_sScheduleResInfo.bApplyedHead )
		return;

	EClassGenderType eClassGenderType = CNtlDefaultItemTable::GetClassGenderType(m_sScheduleResInfo.uiRace, m_sScheduleResInfo.uiGender, m_sScheduleResInfo.bIsAdult);

	std::string strTexPath = "Texture\\Item\\;";
	std::string *strTexName = NULL;

	/*if(eClassGenderType == NAMEK_ADULT || eClassGenderType == NAMEK_CHILD)
	{
		strTexName = CNtlDefaultItemTable::GetInstance().GetHeadBaseTextureName(eClassGenderType, m_sScheduleResInfo.uiHeadType, m_sScheduleResInfo.uiHeadColorType);
		m_pHeadTexture = CNtlPLResourceManager::GetInstance()->LoadTexture(strTexName->c_str(), strTexPath.c_str());
		pItem->SetBaseTexture( m_pHeadTexture );
	}
	else*/
	{
		RwRGBA *pColor = CNtlDefaultItemTable::GetInstance().GetHeadColor(eClassGenderType,m_sScheduleResInfo.uiHeadColorType);
		if(pColor)
			m_pOwner->SetHeadColor(pColor->red, pColor->green, pColor->blue);
	}
	m_sScheduleResInfo.bApplyedHead = TRUE;
}

RwBool CNtlPLEquipItem::Create(RwUInt32 uiClass, RwUInt32 uiRace, RwUInt32 uiGender, RwUInt32 uiHeadType, RwUInt32 uiFaceType, RwUInt32 uiSkinColorType, RwUInt32 uiHeadColorType, RwBool bIsAdult)
{
	m_sScheduleResInfo.uiClass				= uiClass;
	m_sScheduleResInfo.uiRace				= uiRace;
	m_sScheduleResInfo.uiGender				= uiGender;
	m_sScheduleResInfo.uiHeadType			= uiHeadType;
	m_sScheduleResInfo.uiFaceType			= uiFaceType;
    m_sScheduleResInfo.bIsAdult             = bIsAdult;

	m_sScheduleResInfo.uiSkinColorType		= uiSkinColorType;
	m_sScheduleResInfo.uiHeadColorType		= uiHeadColorType;

	EClassGenderType eClassGenderType	= CNtlDefaultItemTable::GetClassGenderType(m_sScheduleResInfo.uiRace, m_sScheduleResInfo.uiGender, m_sScheduleResInfo.bIsAdult);

	if( !SetChangeDefHeadItem(eClassGenderType, m_sScheduleResInfo.uiHeadType, uiHeadColorType) ||
		!SetChangeDefFaceItem(eClassGenderType, m_sScheduleResInfo.uiFaceType, uiSkinColorType) ||
		!SetChangeDefBodyItem(eClassGenderType, ITEM_EQUIP_JACKET, uiSkinColorType) ||
		!SetChangeDefBodyItem(eClassGenderType, ITEM_EQUIP_PANTS, uiSkinColorType) ||
		!SetChangeDefBodyItem(eClassGenderType, ITEM_EQUIP_BOOTS, uiSkinColorType) ||
		!SetChangeDefBodyItem(eClassGenderType, ITEM_EQUIP_TWO_HAND, uiSkinColorType) )
	{
		NTL_ASSERTFAIL("Character Default Item Create Fail");
		return FALSE;
	}

	return TRUE;
}

RwBool CNtlPLEquipItem::Update(RwReal fElapsed)
{
	CNtlPLItem *pFaceItem = GetDefItem(ITEM_EQUIP_FACE);
	if(pFaceItem && m_bFaceChange)
	{
		m_fFaceTexAniCounter += fElapsed;
		if( (m_fFaceTexAniCounter >= m_fFaceTexAniChangeTime  && m_iFaceTexAniNum == 0) ||
			(m_fFaceTexAniCounter >= m_fFaceTexAniDisplayTime && m_iFaceTexAniNum == 1) )
		{
			if( m_iFaceTexAniNum == 0 )
				m_iFaceTexAniNum = 1;
			else
				m_iFaceTexAniNum = 0;

			if( m_pFaceTexture[m_iFaceTexAniNum] != NULL)
				pFaceItem->SetMultiTexture( m_pFaceTexture[m_iFaceTexAniNum] );

			m_fFaceTexAniCounter = 0.f;
			m_fFaceTexAniChangeTime = m_fFaceTexAniDefaultChangeTime + NtlRandomNumber(2.f, 8.f);
		}
	}

	if(m_pOwner && m_pOwner->GetCharScheduleResInfo()->bLoadComplete )
	{
		EQUIP_ITEM_LIST_ITER it = m_listItem.begin();
		for(; it != m_listItem.end(); ++it)
		{
			CNtlPLItem *pItem = *it;
			NTL_ASSERT( pItem , "CNtlPLEquipItem::Update -- CNtlPLItem pointer is NULL" );


			if( pItem->GetItemScheduleResInfo()->bLoadComplete )
			{
				pItem->SetApplyEquipData();
				if( pItem->GetEquipSlotType() == ITEM_EQUIP_FACE )
				{
					SetApplyFaceData(pItem);
				}

				if( pItem->GetEquipSlotType() == ITEM_EQUIP_HEAD )
				{
					SetApplyHeadData(pItem);
				}
			}
		}
	}
	return TRUE;
}

RwBool CNtlPLEquipItem::SetChangeDefHeadItem(EClassGenderType eClassGenderType, RwUInt32 uiHeadType, RwUInt32 uiColorType)
{
	std::string *strKeyName = CNtlDefaultItemTable::GetInstance().GetHeadKeyName(eClassGenderType, uiHeadType);
	NTL_ASSERTE(strKeyName != NULL);
	if(strKeyName != NULL && strKeyName->size())
	{
		SPLItemCreateParam sParam;
		sParam.byUpgrade	= 0;
		//		sParam.uiSerialId	= m_pOwner->GetSerialID();

		CNtlPLItem *pItem = static_cast<CNtlPLItem *>(GetSceneManager()->CreateEntity(PLENTITY_ITEM, strKeyName->c_str(), &sParam));
		NTL_ASSERT(pItem != NULL, "not find" << strKeyName->c_str() << PLENTITY_ITEM); 
		if(pItem == NULL)
			return FALSE;

		SetDefItem(pItem, ITEM_EQUIP_HEAD);
	}

	return TRUE;
}

RwBool CNtlPLEquipItem::SetChangeDefFaceItem(EClassGenderType eClassGenderType, RwUInt32 uiFaceType, RwUInt32 uiColorType)
{
	std::string *strKeyName;
	/*if( eClassGenderType == NAMEK_ADULT || eClassGenderType == NAMEK_CHILD)
		strKeyName = CNtlDefaultItemTable::GetInstance().GetFaceKeyName(eClassGenderType, uiFaceType);
	else*/
		strKeyName = CNtlDefaultItemTable::GetInstance().GetFaceKeyName(eClassGenderType, DEF_ITEM_START);
	NTL_ASSERTE(strKeyName != NULL);

	if(strKeyName != NULL && strKeyName->size())
	{
		SPLItemCreateParam sParam;
		sParam.byUpgrade	= 0;
		//		sParam.uiSerialId	= m_pOwner->GetSerialID();

		CNtlPLItem *pItem = static_cast<CNtlPLItem *>(GetSceneManager()->CreateEntity(PLENTITY_ITEM, strKeyName->c_str(), &sParam));
		NTL_ASSERTE(pItem != NULL);
		if(pItem == NULL)
			return FALSE;

		SetDefItem(pItem, ITEM_EQUIP_FACE);
	}  

	return TRUE;
}

RwBool CNtlPLEquipItem::SetChangeDefBodyItem(EClassGenderType eClassGenderType, RwUInt32 uiEquip, RwUInt32 uiColorType)
{
	std::string *strKeyName;
	/*if( eClassGenderType == NAMEK_ADULT || eClassGenderType == NAMEK_CHILD)
		strKeyName = CNtlDefaultItemTable::GetInstance().GetBodyKeyName(eClassGenderType, uiColorType, uiEquip);
	else*/
		strKeyName = CNtlDefaultItemTable::GetInstance().GetBodyKeyName(eClassGenderType, DEF_ITEM_START, uiEquip);

	NTL_ASSERTE(strKeyName != NULL);
	if(strKeyName != NULL && strKeyName->size())
	{
		SPLItemCreateParam sParam;
		sParam.byUpgrade	= 0;
		//		sParam.uiSerialId	= m_pOwner->GetSerialID();

		CNtlPLItem *pItem = static_cast<CNtlPLItem *>(GetSceneManager()->CreateEntity(PLENTITY_ITEM, strKeyName->c_str(), &sParam));
		NTL_ASSERTE(pItem != NULL);
		if(pItem == NULL)
			return FALSE;

		SetDefItem(pItem, uiEquip);
	}

	return TRUE;
}



RwBool CNtlPLEquipItem::RenderToTexture()
{
	EQUIP_ITEM_LIST_ITER it = m_listItem.begin();
	for(; it != m_listItem.end(); it++)
	{
		CNtlPLItem *plistItem = *it;

		if( g_CheckItemWeapon( plistItem->GetResType()) )
			plistItem->RenderToTexture();

	}
	return TRUE;
}

void CNtlPLEquipItem::SetDefItemAlpha(RwUInt8 byValue)
{
	for each(CNtlPLItem* pItem in m_listItem)
	{
		if(pItem)
			pItem->SetAlpha(byValue);            
	}
}

void CNtlPLEquipItem::SetDefItemWeightAlpha(RwReal fWeightValue)
{
	//DEFAULT_ITEM_MAP_ITER stMapIT  = m_mapDefaultItem.begin();
	//DEFAULT_ITEM_MAP_ITER endMapIT = m_mapDefaultItem.end();
	//for(;stMapIT != endMapIT;)
	//{
	//	CNtlPLItem *pItem = stMapIT->second;
	//	if(pItem != NULL)
	//	{
	//		pItem->SetWeightAlpha( fWeightValue );
	//	}
	//	++stMapIT;
	//}

	for each(CNtlPLItem* pItem in m_listItem)
	{
		if(pItem)
			pItem->SetWeightAlpha(fWeightValue);
	}
}

void CNtlPLEquipItem::SetDefItemColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue)
{
	DEFAULT_ITEM_MAP_ITER stMapIT  = m_mapDefaultItem.begin();
	DEFAULT_ITEM_MAP_ITER endMapIT = m_mapDefaultItem.end();
	for(;stMapIT != endMapIT;)
	{
		CNtlPLItem *pItem = stMapIT->second;
		if(pItem != NULL)
		{
			pItem->SetColor( byRed, byGreen, byBlue );
		}
		++stMapIT;
	}    
}

void CNtlPLEquipItem::SetDefItemAddColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue)
{
	DEFAULT_ITEM_MAP_ITER stMapIT  = m_mapDefaultItem.begin();
	DEFAULT_ITEM_MAP_ITER endMapIT = m_mapDefaultItem.end();
	for(;stMapIT != endMapIT;)
	{
		CNtlPLItem *pItem = stMapIT->second;
		if(pItem != NULL)
		{
			pItem->SetAddColor( byRed, byGreen, byBlue );
		}
		++stMapIT;
	}
}

void CNtlPLEquipItem::SetDefItemVisible( RwBool bVisible )
{
	//DEFAULT_ITEM_MAP_ITER stMapIT  = m_mapDefaultItem.begin();
	//DEFAULT_ITEM_MAP_ITER endMapIT = m_mapDefaultItem.end();
	//for(;stMapIT != endMapIT;)
	//{
	//	CNtlPLItem *pItem = stMapIT->second;
	//	if(pItem != NULL)
	//	{
	//		pItem->SetVisible( bVisible );
	//	}
	//	++stMapIT;
	//}

	for each(CNtlPLItem* pItem in m_listItem)
	{
		if(pItem)
		{
			pItem->SetVisible(bVisible);
		}
	}
}

void CNtlPLEquipItem::SetChangeFaceEnable( RwBool bEnable ) 
{
	m_bFaceChange = bEnable;
	m_fFaceTexAniCounter = m_fFaceTexAniChangeTime;
}

void CNtlPLEquipItem::SetCullFlags(RwUInt32 uiFlag, RwBool bCulled)
{
	for each(CNtlPLItem* pItem in m_listItem)
	{
		if(pItem)
		{
			pItem->SetCullFlags(uiFlag, bCulled);
		}
	}
}

void CNtlPLEquipItem::SetCullFlags(RwUInt32 uiFlag)
{
	for each(CNtlPLItem* pItem in m_listItem)
	{
		if(pItem)
		{
			pItem->SetCullFlags(uiFlag);
		}
	}
}

VOID CNtlPLEquipItem::SetPetrifyMaterial(RwBool _PetrifyFlag)
{
	for(EQUIP_ITEM_LIST_ITER it = m_listItem.begin(); it != m_listItem.end(); ++it)
	{
		static_cast<CNtlPLItem*>(*it)->SetPetrifyMaterial(_PetrifyFlag);
	}

	for(DEFAULT_ITEM_MAP_ITER it = m_mapDefaultItem.begin(); it != m_mapDefaultItem.end(); ++it)
	{
		static_cast<CNtlPLItem*>(it->second)->SetPetrifyMaterial(_PetrifyFlag);
	}
}

void CNtlPLEquipItem::SetSimpleMaterial(RwBool _SimpleMaterial)
{
	for(EQUIP_ITEM_LIST_ITER it = m_listItem.begin(); it != m_listItem.end(); ++it)
	{
		static_cast<CNtlPLItem*>(*it)->SetSimpleMaterial(_SimpleMaterial);
	}

	for(DEFAULT_ITEM_MAP_ITER it = m_mapDefaultItem.begin(); it != m_mapDefaultItem.end(); ++it)
	{
		static_cast<CNtlPLItem*>(it->second)->SetSimpleMaterial(_SimpleMaterial);
	}
}
/*
void CNtlPLEquipItem::SimplifyMaterial()
{
	for(EQUIP_ITEM_LIST_ITER it = m_listItem.begin(); it != m_listItem.end(); ++it)
	{
		static_cast<CNtlPLItem*>(*it)->SimplifyMaterial();
	}

	for(DEFAULT_ITEM_MAP_ITER it = m_mapDefaultItem.begin(); it != m_mapDefaultItem.end(); ++it)
	{
		static_cast<CNtlPLItem*>(it->second)->SimplifyMaterial();
	}
}
*/