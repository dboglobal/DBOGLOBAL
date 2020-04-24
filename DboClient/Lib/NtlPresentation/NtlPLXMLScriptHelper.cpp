#include "precomp_ntlpresentation.h"
#include "NtlPLXMLScriptHelper.h"

#include <atlbase.h>
#include <comutil.h>

#include "NtlPLHelpers.h"
#include "NtlSoundDefines.h"

WCHAR CNtlPLXMLScriptHelper::wBuf[128] = {0,};
char  CNtlPLXMLScriptHelper::szBuf[128] = {0,};

CNtlPLXMLScriptHelper::CNtlPLXMLScriptHelper(void)
{
    ZeroMemory(wBuf, sizeof(wBuf));
    ZeroMemory(szBuf, sizeof(szBuf));
}

CNtlPLXMLScriptHelper::~CNtlPLXMLScriptHelper(void)
{
}

RwBool CNtlPLXMLScriptHelper::SaveTMQEvent(IXMLDOMElement* pElemEvent, const SEventAnimCinematic* pEventTMQ)
{
    if(!pElemEvent || !pEventTMQ)
        return FALSE;

    SaveAttribute(pElemEvent, L"TYPE", pEventTMQ->eAnimCinematicEventType);

    return TRUE;
}

RwBool CNtlPLXMLScriptHelper::LoadTMQEvent(IXMLDOMNode* pNodeEvent, SEventAnimCinematic* pEventTMQ)
{
    if(!pNodeEvent || !pEventTMQ)
        return FALSE;

    ZeroMemory(szBuf, sizeof(szBuf));

    GetTextWithAttributeName(pNodeEvent, "TYPE", szBuf, sizeof(szBuf));
    pEventTMQ->eAnimCinematicEventType = (EAnimCinematicEventType)atoi(szBuf);

    return TRUE;
}

RwBool CNtlPLXMLScriptHelper::SaveAlphaEvent( IXMLDOMElement* pElemEvent, const SEventAlpha* pEventAlpha ) 
{
    if(!pElemEvent || !pEventAlpha)
        return FALSE;
    
    SaveAttribute(pElemEvent, L"START_ALPHA", pEventAlpha->nStartAlpha);
    SaveAttribute(pElemEvent, L"DEST_ALPHA", pEventAlpha->nDestAlpha);
    SaveAttribute(pElemEvent, L"LIFE_TIME", pEventAlpha->fLifeTime);
    SaveAttribute(pElemEvent, L"FADE_TIME", pEventAlpha->fFadeTime);
    SaveAttribute(pElemEvent, L"TYPE", pEventAlpha->eAlphaEventType);
    SaveAttribute(pElemEvent, L"ATOMIC", pEventAlpha->bfAtomicList);

    return TRUE;
}

RwBool CNtlPLXMLScriptHelper::LoadAlphaEvent( IXMLDOMNode* pNodeEvent, SEventAlpha* pEventAlpha ) 
{
    if(!pNodeEvent || !pEventAlpha)
        return FALSE;

    GetTextWithAttributeName(pNodeEvent, "START_ALPHA", szBuf, sizeof(szBuf));
    pEventAlpha->nStartAlpha = (RwInt32)atoi(szBuf);

    GetTextWithAttributeName(pNodeEvent, "DEST_ALPHA", szBuf, sizeof(szBuf));
    pEventAlpha->nDestAlpha = (RwInt32)atoi(szBuf);

    GetTextWithAttributeName(pNodeEvent, "LIFE_TIME", szBuf, sizeof(szBuf));
    pEventAlpha->fLifeTime = (RwReal)atof(szBuf);

    GetTextWithAttributeName(pNodeEvent, "FADE_TIME", szBuf, sizeof(szBuf));
    pEventAlpha->fFadeTime = (RwReal)atof(szBuf);

    GetTextWithAttributeName(pNodeEvent, "TYPE", szBuf, sizeof(szBuf));
    pEventAlpha->eAlphaEventType = (SEventAlpha::EAlphaEventType)atoi(szBuf);

    GetTextWithAttributeName(pNodeEvent, "ATOMIC", szBuf, sizeof(szBuf));
    pEventAlpha->bfAtomicList = atoi(szBuf);

    return TRUE;
}

RwBool CNtlPLXMLScriptHelper::SaveExplosionEvent( IXMLDOMElement* pElemEvent, const SEventExplosion* pEventExplosion ) 
{
    if(!pElemEvent || !pEventExplosion)
        return FALSE;

    SaveAttribute(pElemEvent, L"TYPE", pEventExplosion->eType);

    return TRUE;
}

RwBool CNtlPLXMLScriptHelper::LoadExplosionEvent( IXMLDOMNode* pNodeEvent, SEventExplosion* pEventExplosion ) 
{
    if(!pNodeEvent || !pEventExplosion)
        return FALSE;

    GetTextWithAttributeName(pNodeEvent, "TYPE", szBuf, sizeof(szBuf));
    pEventExplosion->eType = (ENtlPLExplosionEventType)atoi(szBuf);

    return TRUE;
}

RwBool CNtlPLXMLScriptHelper::SaveSoundEvent( IXMLDOMElement* pElemEvent, const SEventSound* pEventSound ) 
{
    if(!pElemEvent || !pEventSound)
        return FALSE;

    SaveAttribute(pElemEvent, L"SOUNDFILE", pEventSound->chSoundName);
    SaveAttribute(pElemEvent, L"SOUNDFILE2", pEventSound->chSoundName2);
    SaveAttribute(pElemEvent, L"SOUNDFILE3", pEventSound->chSoundName3);
    SaveAttribute(pElemEvent, L"SOUNDFILE4", pEventSound->chSoundName4);
    SaveAttribute(pElemEvent, L"TYPE", pEventSound->eSoundType);
    SaveAttribute(pElemEvent, L"LOOP", pEventSound->bLoop);
    SaveAttribute(pElemEvent, L"VOL", pEventSound->fSoundVolume);
    SaveAttribute(pElemEvent, L"DIST", pEventSound->fSoundDist);
    SaveAttribute(pElemEvent, L"DECAY", pEventSound->fSoundDecayDist);
    SaveAttribute(pElemEvent, L"PITCH", pEventSound->fSoundPitchMin);
    SaveAttribute(pElemEvent, L"PITCH_MAX", pEventSound->fSoundPitchMax);

    return TRUE;
}

RwBool CNtlPLXMLScriptHelper::LoadSoundEvent( IXMLDOMNode* pNodeEvent, SEventSound* pEventSound ) 
{
    if(!pNodeEvent || !pEventSound)
        return FALSE;

    if(GetTextWithAttributeName(pNodeEvent, "SOUNDFILE", szBuf, sizeof(szBuf)))
        strcpy_s(pEventSound->chSoundName, szBuf);

    if(GetTextWithAttributeName(pNodeEvent, "SOUNDFILE2", szBuf, sizeof(szBuf)))
        strcpy_s(pEventSound->chSoundName2, szBuf);    

    if(GetTextWithAttributeName(pNodeEvent, "SOUNDFILE3", szBuf, sizeof(szBuf)))
        strcpy_s(pEventSound->chSoundName3, szBuf);

    if(GetTextWithAttributeName(pNodeEvent, "SOUNDFILE4", szBuf, sizeof(szBuf)))
        strcpy_s(pEventSound->chSoundName4, szBuf);

    if(GetTextWithAttributeName(pNodeEvent, "TYPE", szBuf, sizeof(szBuf)))
        pEventSound->eSoundType = (eChannelGroupType)atoi(szBuf);

    if(GetTextWithAttributeName(pNodeEvent, "LOOP", szBuf, sizeof(szBuf)))
        pEventSound->bLoop = (RwBool)atoi(szBuf);

    if(GetTextWithAttributeName(pNodeEvent, "VOL", szBuf, sizeof(szBuf)))
    {
        if(atof(szBuf) > 0.0f)
        {
            pEventSound->fSoundVolume = (RwReal)atof(szBuf);
        }
    }

    if(GetTextWithAttributeName(pNodeEvent, "DIST", szBuf, sizeof(szBuf)))
    {
        if(atof(szBuf) > 0.0f)
        {
            pEventSound->fSoundDist = (RwReal)atof(szBuf);
        }
    }

    if(GetTextWithAttributeName(pNodeEvent, "DECAY", szBuf, sizeof(szBuf)))
    {
        if(atof(szBuf) > 0.0f)
        {
            pEventSound->fSoundDecayDist = (RwReal)atof(szBuf);
        }
    }

    if(GetTextWithAttributeName(pNodeEvent, "PITCH", szBuf, sizeof(szBuf)))
    {
        if(atof(szBuf) > 0.0f)
        {
            pEventSound->fSoundPitchMin = (RwReal)atof(szBuf);
        }        
    }

    if(GetTextWithAttributeName(pNodeEvent, "PITCH_MAX", szBuf, sizeof(szBuf)))
    {
        if(atof(szBuf) > 0.0f)
        {
            pEventSound->fSoundPitchMax = (RwReal)atof(szBuf);
        }
    }

    return TRUE;
}

RwBool CNtlPLXMLScriptHelper::SaveEffectEvent( IXMLDOMElement* pElemEvent, const SEventVisualEffect* pEventVisualEffect) 
{
    if(!pElemEvent || !pEventVisualEffect)
        return FALSE;

   SaveAttribute(pElemEvent, L"NAME", pEventVisualEffect->chEffectName);
   SaveAttribute(pElemEvent, L"BONE_TYPE", pEventVisualEffect->eBoneType);
   SaveAttribute(pElemEvent, L"BONE", pEventVisualEffect->chBoneName);
   SaveAttribute(pElemEvent, L"POS", pEventVisualEffect->vOffSetPos);
   SaveAttribute(pElemEvent, L"ATTACH", pEventVisualEffect->bAttach);
   SaveAttribute(pElemEvent, L"ATTACH_BONE", pEventVisualEffect->bAttachBone);
   SaveAttribute(pElemEvent, L"APPLY_SCALE", pEventVisualEffect->bApplyScale);
   SaveAttribute(pElemEvent, L"PROJECTILE", pEventVisualEffect->bProjectileType);   

   return TRUE;
}

RwBool CNtlPLXMLScriptHelper::LoadEffectEvent( IXMLDOMNode* pNodeEvent, SEventVisualEffect* pEventEffect ) 
{
    if(!pNodeEvent || !pEventEffect)
        return FALSE;

    char szEffectName[MAX_DEFAULT_NAME] = {0,};
    char szBoneType[MAX_DEFAULT_NAME] = {0,};
    char szBoneName[MAX_DEFAULT_NAME] = {0,};
    char szOffsetPos[MAX_DEFAULT_NAME] = {0,};
    char szAttachBone[8] = {0,};            
    char szApplyScale[8] = {0,};
    char szProjectile[8] = {0,};
    char szAttach[8] = {0,};


    GetTextWithAttributeName(pNodeEvent, "NAME", szEffectName, sizeof(szEffectName));
    GetTextWithAttributeName(pNodeEvent, "BONE_TYPE", szBoneType, sizeof(szBoneType));
    GetTextWithAttributeName(pNodeEvent, "BONE", szBoneName, sizeof(szBoneName));
    GetTextWithAttributeName(pNodeEvent, "POS", szOffsetPos, sizeof(szOffsetPos));
    GetTextWithAttributeName(pNodeEvent, "ATTACH_BONE", szAttachBone, sizeof(szAttachBone));                                
    GetTextWithAttributeName(pNodeEvent, "PROJECTILE", szProjectile, sizeof(szProjectile));
    
    sprintf(pEventEffect->chEffectName, "%s", szEffectName);
    pEventEffect->eBoneType = (EBoneType)atoi(szBoneType);
    sprintf(pEventEffect->chBoneName, "%s", szBoneName);
    pEventEffect->vOffSetPos = Helper_String2V3D(szOffsetPos);
    pEventEffect->bAttachBone = (RwBool)atoi(szAttachBone);                                
    pEventEffect->bProjectileType = (RwBool)atoi(szProjectile);

    if(GetTextWithAttributeName(pNodeEvent, "APPLY_SCALE", szApplyScale, sizeof(szApplyScale)))
    {
        pEventEffect->bApplyScale = (RwBool)atoi(szApplyScale);
    }

    if(GetTextWithAttributeName(pNodeEvent, "ATTACH", szAttach, sizeof(szAttach)))
    {
        pEventEffect->bAttach = (RwBool)atoi(szAttach);
    }

    return TRUE;
}

void CNtlPLXMLScriptHelper::SaveAttribute(IXMLDOMElement* pElem, WCHAR* szKey, RwInt32 nInt)
{
    swprintf_s(wBuf, L"%d", nInt);
    pElem->setAttribute(szKey, (_variant_t)wBuf);
}

void CNtlPLXMLScriptHelper::SaveAttribute( IXMLDOMElement* pElem, WCHAR* szKey, RwUInt32 nInt ) 
{
    SaveAttribute(pElem, szKey, (RwInt32)nInt);
}

void CNtlPLXMLScriptHelper::SaveAttribute(IXMLDOMElement* pElem, WCHAR* szKey, RwReal fFloat)
{
    swprintf_s(wBuf, L"%.2f", fFloat);
    pElem->setAttribute(szKey, (_variant_t)wBuf);
}

void CNtlPLXMLScriptHelper::SaveAttribute(IXMLDOMElement* pElem, WCHAR* szKey, const char* szChar)
{
    USES_CONVERSION;
    pElem->setAttribute(szKey, (_variant_t)A2W(szChar));
}

void CNtlPLXMLScriptHelper::SaveAttribute(IXMLDOMElement* pElem, WCHAR* szKey, const RwV3d& v3d)
{
    Helper_V3D2StringU(wBuf, v3d);
    pElem->setAttribute(szKey, (_variant_t)wBuf);
}

void CNtlPLXMLScriptHelper::SaveAttribute( IXMLDOMElement* pElem, WCHAR* szKey, const RwV2d& v2d ) 
{
    Helper_V2D2StringU(wBuf, v2d);
    pElem->setAttribute(szKey, (_variant_t)wBuf);
}

void CNtlPLXMLScriptHelper::SaveAttribute( IXMLDOMElement* pElem, WCHAR* szKey, const RwRGBA& rgba ) 
{
    Helper_RGBA2String(wBuf, rgba);
    pElem->setAttribute(szKey, (_variant_t)wBuf);
}

RwBool CNtlPLXMLScriptHelper::SaveAnimData( IXMLDOMElement* pElemAnimData, const STypeAnimData* pAnimData ) 
{
    if(!pElemAnimData || !pAnimData)
        return FALSE;

    SaveAttribute(pElemAnimData, L"KEYID", pAnimData->uiAnimKey);
    SaveAttribute(pElemAnimData, L"FILE", pAnimData->strAnimName.c_str());
    SaveAttribute(pElemAnimData, L"PLAYTIME", pAnimData->fPlayTime);
    SaveAttribute(pElemAnimData, L"FLAG", pAnimData->flagAnim);

    return TRUE;
}

RwBool CNtlPLXMLScriptHelper::LoadAnimData( IXMLDOMNode* pNodeAnimData, STypeAnimData* pAnimData ) 
{
    if(!pNodeAnimData || !pAnimData)
        return FALSE;

    if(GetTextWithAttributeName(pNodeAnimData,"KEYID", szBuf, sizeof(szBuf)))
        pAnimData->uiAnimKey = (RwUInt32)atoi(szBuf);
    
    if(GetTextWithAttributeName(pNodeAnimData,"FILE", szBuf, sizeof(szBuf)))
        pAnimData->strAnimName = szBuf;

    if(GetTextWithAttributeName(pNodeAnimData,"PLAYTIME", szBuf, sizeof(szBuf)))
        pAnimData->fPlayTime = (RwReal)atof(szBuf);

    if(GetTextWithAttributeName(pNodeAnimData, "FLAG", szBuf, sizeof(szBuf)))
        pAnimData->flagAnim = (RwUInt32)atoi(szBuf);

    return TRUE;
}