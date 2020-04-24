#pragma once
#pragma warning( disable : 4996 )

#include "NtlSharedCommon.h"
#include "NtlXMLDoc.h"
#include "NtlPLCharacterProperty.h"
#include "NtlPLXMLScriptHelper.h"

// 1.1 : Bone Scale이 적용되지 않은 경우 Bone Scale Data를 저장/로드하지 않도록 변경

#define CHARACTER_XML_SCRIPT_VER        L"2"

/**
 * \ingroup NtlPresentation
 * \brief 캐릭터 스크립트(XML)을 Save/Load 하는 클래스
 * \date 2006-05-16
 * \author agebreak
 */
class CNtlCharacterXMLScript : public CNtlXMLDoc
{
public:
    CNtlCharacterXMLScript(void);
    virtual ~CNtlCharacterXMLScript(void);

    RwBool SaveCharacterScript(const char* strFileName, CNtlPLCharacterProperty* pProperty);         ///< 캐릭터 프로퍼티를 XML 스크립트로 저장한다.
    RwBool LoadCharacterScript(const char* strFileName, CNtlPLCharacterProperty* pProperty);         ///< XML 스크립트를 로드하여 캐릭터 프로퍼티에 세팅한다.

protected:
    RwBool  SaveHeader(CNtlPLCharacterProperty* pProperty);                        ///< Header 부분을 저장한다.
    RwBool  SaveBoneData(CNtlPLCharacterProperty* pProperty);                      ///< Bone Data 부분을 저장한다
    RwBool  SaveLinkEffect(CNtlPLCharacterProperty* pProperty);                    ///< Link Effect Data 부분을 저장한다.
    RwBool  SaveAnimTable(CNtlPLCharacterProperty* pProperty);                     ///< AnimTable 부분을 저장한다.      

    RwBool  LoadHeader(CNtlPLCharacterProperty* pProperty);                        ///< Header 부분을 로드한다.
    RwBool  LoadBoneData(CNtlPLCharacterProperty* pProperty);                      ///< Bone Data 부분을 로드한다.
    RwBool  LoadLinkEffect(CNtlPLCharacterProperty* pProperty);                    ///< Link Effect Data 부분을 로드한다.
    RwBool  LoadAnimTable(CNtlPLCharacterProperty* pProperty);                     ///< AnimTable  부분을 로드한다.


    //////////////////////////////////////////////////////////////////////////
    void    SaveHitEvent(IXMLDOMElement* pElemEvent, const SEventAnimHit* pEventHit);  ///< Hit Event를 Save 한다.    
    void    LoadHitEvent(IXMLDOMNode* pNodeEvent, SEventAnimHit* pEventHit);           ///< Hit Event를 Load 한다.

    void    SaveTraceEvent(IXMLDOMElement* pElemEvent, const SEventTrace* pEventTrace);  ///< Trace Event를 Save 한다.    
    void    LoadTraceEvent(IXMLDOMNode* pNodeEvent, SEventTrace* pEventTrace);           ///< Trace Event를 Load 한다.

    void    SaveSubWeaponEvent(IXMLDOMElement* pElemEvent, const SEventSubWeapon* pEventSubWeapon); ///< SubWeapon Event를 Save한다.
    void    LoadSubWeaponEvent(IXMLDOMNode* pNodeEvent, SEventSubWeapon* pEventSubWeapon);          ///< SubWeapon Event를 Load한다.

	void	SavePostEffectEvent(IXMLDOMElement* pElemEvent, const SEventPostEffect* pEventPostEffect);	///< Post Effect Event를 Save 한다.
	void	LoadPostEffectEvent(IXMLDOMNode* pNodeEvent, SEventPostEffect* pPostEffect);				///< Post Effect Event를 Load 한다.

    void    SaveFootStepEvent(IXMLDOMElement* pElemEvent, const SEventFootStep* pEventFootStep);        ///< 풋스텝 이벤트를 Save한다.
    void    LoadFootStepEvent(IXMLDOMNode* pNodeEvent, SEventFootStep* pFootStep);                      ///< 풋스텝 이벤트를 Load한다.

    void    SaveDirectEvent(IXMLDOMElement* pElemEvent, const SEventDirect* pEventDirect);
    void    LoadDirectEvent(IXMLDOMNode* pNodeEvent, SEventDirect* pEventDirect);

    void    SaveColorChangeEvent(IXMLDOMElement* pElemEvent, const SEventColorChange* pEvent);         
    void    LoadColorChangeEvent(IXMLDOMNode* pNodeEvent, SEventColorChange* pEvent);

    void    SaveStretchEvent(IXMLDOMElement* pElemEvent, const SEventStretch* pEvent);
    void    LoadStretchEvent(IXMLDOMNode* pNodeEvent, SEventStretch* pEvent);
    
    void    SaveTriggerEvent(IXMLDOMElement* pElemEvent, const SEventTrigger* pEvent);
    void    LoadTriggerEvent(IXMLDOMNode* pNodeEvent, SEventTrigger* pEvent);

    void    SaveSkillCancelEvent(IXMLDOMElement* pElemEvent, const SEventSkillCancel* pEvent);
    void    LoadSkillCancelEvent(IXMLDOMNode* pNodeEvent, SEventSkillCancel* pEvent);   

protected:
    IXMLDOMElement*     m_pElemRoot;             ///< Root Element
    CNtlPLXMLScriptHelper m_XMLScriptHelper;     ///< DBO XML Script Helper 
};
