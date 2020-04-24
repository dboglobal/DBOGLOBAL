/*****************************************************************************
 *
 * File			: NtlPLItemData.h
 * Author		: HongHoDong
 * Copyright	: (주)NTL
 * Date			: 2006. 8. 29.	
 * Abstract		: NTL CNtlPLItem
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/
#ifndef _NTL_PL_ITEM_DATA_H_
#define _NTL_PL_ITEM_DATA_H_

#define ITEM_EQUIP_TWO_HAND_NAME		"nullRhand"
#define ITEM_EQUIP_L_HAND_NAME			"nullLhand"
#define ITEM_EQUIP_R_HAND_NAME			"nullRhand"
#define ITEM_EQUIP_R_EYE_NAME			"nulleye"
#define ITEM_EQUIP_BACK_NAME			"nullback"
#define ITEM_EQUIP_MASK_NAME            "Bip01 Head"

#define MAX_ITEM_WEAPON_SHOT_BONE		3

#define ITEM_SUBWEAPON_SHOT1_NAME		"Weapon_VE_1"	
#define ITEM_SUBWEAPON_SHOT2_NAME		"Weapon_VE_2"
#define ITEM_SUBWEAPON_SHOT3_NAME		"Weapon_VE_3"

#define ITEM_WEAPON_SHOT_NAME			"W_STF"
#define ITEM_EQUIP_USER_MOVE1_NAME		"Bip01"

enum EItemResType
{
	ITEM_RES_EQUIP_BODY				= 0,			// Body용 Item(Owner에 장착이 됨)
	ITEM_RES_EQUIP_MAIN				= 1,			// Main Item
	ITEM_RES_EQUIP_SUB				= 100,			// Sub Item
	
	ITEM_RES_NOT_EQUIP				= 200,			// World용 Item
};

enum EItemEquipSlotType
{	
	ITEM_EQUIP_NONE  = 0,							// Not Equip(Drop Item)
	
	ITEM_EQUIP_HEAD,								// Head
	ITEM_EQUIP_FACE,								// Face
	ITEM_EQUIP_JACKET,								// Jacket
	ITEM_EQUIP_PANTS,								// Pants
	ITEM_EQUIP_BOOTS,								// Boots
	ITEM_EQUIP_TWO_HAND,							// Glove

	ITEM_EQUIP_R_EYE = 8,							// Scouter
	ITEM_EQUIP_R_HAND = 10,							// Weapon
	ITEM_EQUIP_L_HAND,								// Weapon
	ITEM_EQUIP_BACK,								// SubWeapon....
    ITEM_EQUIP_MASK,                                // 마인용 마스크
	
	ITEM_EQUIP_USER_MOVE1 = 20,						// In the Weapon (Gem) Model Tool, decrease by the Offset value.
	ITEM_EQUIP_SCOUTER	= 100,						// Scouter (Offset value applied, Offset value applied)
};

struct SItemScheduleResInfo
{
	RwBool	bEquip;									// 장착을 하는지
	RwBool	bAttach;								// 실제 Attach를 하는지
	RwBool	bVisible;								// 화면에 보여주는지

	RwBool	bApplyedEquip;							// 장착이 되었는지
	RwBool	bLoadComplete;							// Resource Load가 완료 되었는지

	RwV3d	vOffset;								// 장착시 Offset
	RwV3d	vScale;									// 원본 Scale
    RwUInt32 uiAnimKey;                             ///< 애니메이션 ID
	

	SItemScheduleResInfo() :  bEquip(FALSE),
							  bAttach(FALSE),
							  bVisible(TRUE),
						      bApplyedEquip(FALSE), 
							  bLoadComplete(FALSE) 
	{
		vOffset.x = 0.f;
		vOffset.y = 0.f;
		vOffset.z = 0.f;

		vScale.x = 1.f;
		vScale.y = 1.f;
		vScale.z = 1.f;
        uiAnimKey = 0;
	}
};

// Body Item의 경우는 Owner에서 World에 Add가 된다.
inline RwBool	g_CheckItemBody(EItemResType	eItemResType)
{
	if(eItemResType == ITEM_RES_EQUIP_BODY)
		return TRUE;
	return FALSE;
};

inline RwBool	g_CheckItemWeapon(EItemResType	eItemResType)
{
	if( (eItemResType == ITEM_RES_EQUIP_MAIN) ||
		(eItemResType == ITEM_RES_EQUIP_SUB) )
		return TRUE;
	return FALSE;
};

/// Upgrade Effect 관련 속성
struct SUpgradeEffectProperty
{
    RwChar  szBone1[MAX_DEFAULT_NAME];
    RwV3d   vOffset1;
    RwChar  szBone2[MAX_DEFAULT_NAME];
    RwV3d   vOffset2;

    char	szEffect[MAX_UPGRADE_EFFECT_COUNT][MAX_DEFAULT_NAME];

    SUpgradeEffectProperty()
    {
        ZeroMemory(szBone1, sizeof(szBone1));
        ZeroMemory(szBone2, sizeof(szBone2));
		vOffset1 = vOffset2 = ZeroAxis;

		ZeroMemory(szEffect, sizeof(szEffect));
    }
};

#endif