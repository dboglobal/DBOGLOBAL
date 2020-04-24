#pragma once

#include "NtlXMLDoc.h"
#include "NtlSharedCommon.h"
#include "NtlAnimEventData.h"
#include "NtlTypeAnimData.h"

/**
* \ingroup NtlPresentation
* \brief Model Tool에 사용되는 XML Script중 Event들은 서로 공통된다.
*        이러한 공통된 이벤트를 저장하기 위한 공통된 코드를 모아놓기 위한 Helper 클래스 
* \date 2007-10-25
* \author agebreak
*/
class CNtlPLXMLScriptHelper : public CNtlXMLDoc
{
public:
    CNtlPLXMLScriptHelper(void);
    ~CNtlPLXMLScriptHelper(void);

    // 공통되는 이벤트 저장/로드 헬퍼 함수들
    RwBool  SaveTMQEvent(IXMLDOMElement* pElemEvent, const SEventAnimCinematic* pEventTMQ);               ///< TMQ Event를 Save 한다.
    RwBool  LoadTMQEvent(IXMLDOMNode* pNodeEvent, SEventAnimCinematic* pEventTMQ);

    RwBool  SaveAlphaEvent(IXMLDOMElement* pElemEvent, const SEventAlpha* pEventAlpha);         ///< Alpha Event를 Save 한다.
    RwBool  LoadAlphaEvent(IXMLDOMNode* pNodeEvent, SEventAlpha* pEventAlpha);                  ///< Alpha Event를 Load 한다.

    RwBool  SaveExplosionEvent(IXMLDOMElement* pElemEvent, const SEventExplosion* pEventExplosion); ///< 폭발 이벤트를 Save한다.
    RwBool  LoadExplosionEvent(IXMLDOMNode* pNodeEvent, SEventExplosion* pEventExplosion);          ///< 폭발 이벤트를 Load한다.

    RwBool  SaveSoundEvent(IXMLDOMElement* pElemEvent, const SEventSound* pEventSound);         ///< Sound Event를 저장한다.
    RwBool  LoadSoundEvent(IXMLDOMNode* pNodeEvent, SEventSound* pEventSound);                  ///< Sound Event를 로드한다.

    RwBool  SaveEffectEvent(IXMLDOMElement* pElemEvent, const SEventVisualEffect* pEventEffect);    ///< 비주얼 이펙트 이벤트를 저장한다.
    RwBool  LoadEffectEvent(IXMLDOMNode* pNodeEvent, SEventVisualEffect* pEventEffect);             ///< 비주얼 이펙트 이벤트를 로드한다.

    RwBool  SaveAnimData(IXMLDOMElement* pElemAnimData, const STypeAnimData* pAnimData);        ///< 애니메이션 데이터를 저장한다.    
    RwBool  LoadAnimData(IXMLDOMNode* pNodeAnimData, STypeAnimData* pAnimData);                 ///< 애니메이션 데이터를 로드한다.

    // 내부 헬퍼 함수들
    static void SaveAttribute(IXMLDOMElement* pElem, WCHAR* szKey, RwInt32 nInt);                ///< Int형을 Attribute에 저장한다.
    static void SaveAttribute(IXMLDOMElement* pElem, WCHAR* szKey, RwUInt32 nInt);               ///< Int형을 Attribute에 저장한다.
    static void SaveAttribute(IXMLDOMElement* pElem, WCHAR* szKey, RwReal fFloat);               ///< float형을 Attribute에 저장한다.
    static void SaveAttribute(IXMLDOMElement* pElem, WCHAR* szKey, const char* szChar);          ///< char형을 Attribute에 저장한다.
    static void SaveAttribute(IXMLDOMElement* pElem, WCHAR* szKey, const RwV3d& v3d);            ///< RwV3d형을 Attribute에 저장한다.
    static void SaveAttribute(IXMLDOMElement* pElem, WCHAR* szKey, const RwV2d& v2d);            ///< RwV2d형을 Attribute에 저장한다.
    static void SaveAttribute(IXMLDOMElement* pElem, WCHAR* szKey, const RwRGBA& rgba);

protected:    
    static WCHAR   wBuf[128];
    static char    szBuf[128];    
};
