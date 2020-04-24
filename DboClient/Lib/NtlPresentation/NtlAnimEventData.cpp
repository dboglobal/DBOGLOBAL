#include "precomp_ntlpresentation.h"
#include "NtlAnimEventData.h"
#include "NtlPLApi.h"

void SEventAnimHit::SaveSerialize(CNtlSerializer& sOut)
{
    sOut<<fTime;
    sOut<<eEventID;
	sOut<<uiDamage;
	sOut<<bPowerEffect;
	sOut<<eAttackType;
	sOut<<eTargetBehavior;
    sOut<<bKB2Push;
	sOut<<eHandType;
	sOut<<uiProjectileEffectType;
	sOut<<eProjectileShotType;
	sOut<<nSubWeaponFlag;
	sOut.In(chBoneName, MAX_DEFAULT_NAME);
	sOut.In(chProjectileEffectName, MAX_DEFAULT_NAME);	
	sOut<<fProjectileSpeed;
	sOut<<bTargetAttach;
	sOut<<fTargetHeight;
	sOut.In(chTargetEffectName, MAX_DEFAULT_NAME);
    sOut<<eTargetEffectType;
    sOut.In(chSubTargetEffect, MAX_DEFAULT_NAME);
	sOut.In(chTargetSoundName, MAX_DEFAULT_NAME);	
	sOut<<eSoundType;    
    sOut<<bHitSoundEcho;
	sOut<<bCameraShake;    
	sOut.In(chWordEffect, MAX_DEFAULT_NAME);	
    
    switch(uiProjectileEffectType)
    {
    case BEID_PROJ_HISSIDAN:
        sOut<<uEffectTypeExtraData.hissidanData.bApplyAngle;
        sOut.In(&uEffectTypeExtraData.hissidanData.v2dAngle, sizeof(RwV2d));
        break;
    case BEID_PROJ_MULTI_HISSIDAN:
        sOut<<uEffectTypeExtraData.multiHissidanData.nCount;
        sOut.In(uEffectTypeExtraData.multiHissidanData.pArrayAngle, sizeof(RwV2d) * uEffectTypeExtraData.multiHissidanData.nCount);
        break;
    case BEID_PROJ_HELLZONE:
        sOut.In(&uEffectTypeExtraData.hellZoneData.vTargetEffectOffset, sizeof(RwV3d));
        sOut<<uEffectTypeExtraData.hellZoneData.fTargetEffectStartWaitTime;
        sOut<<uEffectTypeExtraData.hellZoneData.fTargetEffectSpeed;
        break;
    }

    // 카메라 셰이크
    if(bCameraShake)
    {
        sOut<<fCameraShakeFactor;
        sOut<<fCameraShakeMaxHeight;
    }
}

void SEventAnimHit::LoadSerialize(CNtlSerializer& sIn)
{
    sIn>>fTime;
    sIn>>E2I(eEventID);
	sIn>>uiDamage;
	sIn>>bPowerEffect;
	sIn>>E2I(eAttackType);
	sIn>>E2I(eTargetBehavior);
    sIn>>bKB2Push;
	sIn>>E2I(eHandType);
	sIn>>E2I(uiProjectileEffectType);
	sIn>>E2I(eProjectileShotType);
	sIn>>nSubWeaponFlag;
	sIn.Out(chBoneName, MAX_DEFAULT_NAME);
	sIn.Out(chProjectileEffectName, MAX_DEFAULT_NAME);	
	sIn>>fProjectileSpeed;
	sIn>>bTargetAttach;
	sIn>>fTargetHeight;
	sIn.Out(chTargetEffectName, MAX_DEFAULT_NAME);
    sIn>>E2I(eTargetEffectType);
    sIn.Out(chSubTargetEffect, MAX_DEFAULT_NAME);
	sIn.Out(chTargetSoundName, MAX_DEFAULT_NAME);	
	sIn>>E2I(eSoundType);    
    sIn>>bHitSoundEcho;
	sIn>>bCameraShake;
	sIn.Out(chWordEffect, MAX_DEFAULT_NAME);		

    switch(uiProjectileEffectType)
    {
    case BEID_PROJ_HISSIDAN:
        sIn>>uEffectTypeExtraData.hissidanData.bApplyAngle;
        sIn.Out(&uEffectTypeExtraData.hissidanData.v2dAngle, sizeof(RwV2d));
        break;
	case BEID_PROJ_HELLZONE:
		sIn.Out(&uEffectTypeExtraData.hellZoneData.vTargetEffectOffset, sizeof(RwV3d));
		sIn >> uEffectTypeExtraData.hellZoneData.fTargetEffectStartWaitTime;
		sIn >> uEffectTypeExtraData.hellZoneData.fTargetEffectSpeed;
		break;
    case BEID_PROJ_MULTI_HISSIDAN:
        sIn>>uEffectTypeExtraData.multiHissidanData.nCount;
        uEffectTypeExtraData.multiHissidanData.pArrayAngle = NTL_NEW RwV2d[uEffectTypeExtraData.multiHissidanData.nCount];
        for(int i = 0; i < uEffectTypeExtraData.multiHissidanData.nCount; ++i)
        {
            sIn.Out(&uEffectTypeExtraData.multiHissidanData.pArrayAngle[i], sizeof(RwV2d));
        }
        break;
    }

    if(bCameraShake)
    {
        sIn>>fCameraShakeFactor;
        sIn>>fCameraShakeMaxHeight;
    }
}