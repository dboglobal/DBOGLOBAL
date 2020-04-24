//***********************************************************************************
//	File		:	NtlResourceEffect.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	ⓒ 2006 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once

#include <string>
#include <vector>

#include "NtlPLProperty.h"
#include "NtlEffectDefine.h"
#include "NtlResourceComponentSystem.h"

class CNtlResourceEffect : public CNtlPLProperty
{
public:
	typedef std::vector<CNtlResourceComponentSystem*>	svdef_ResourceComponentSystem;
	svdef_ResourceComponentSystem		m_svResourceSystem;

	static	RwUInt32					m_nVersion;
    static  RwBool                      m_bUpdateBoundingSphere;            ///< Whether to update the bounding sphere of the effect

	RwBool								m_bShow;

public:
	CNtlResourceEffect(const RwChar* strName);
	virtual ~CNtlResourceEffect(void);

			RwBool	Destroy();

	static	CNtlResourceComponentSystem*	CreateResourceSystem(RwInt8 nType, const RwChar* strName);

			CNtlResourceComponentSystem*	GetResourceSystem(const RwChar* strSystemName);
			void							ResetSubSystem();

			RwBool							Load(FILE* pFile);

            RwBool                          IsAutoDelete();                 ///< 루프 이펙트인지 아닌지 반환한다.

	virtual RwBool Load(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode)		{ return FALSE; }
	virtual RwBool Save(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode)		{ return FALSE; }

public:
    SEffectSoundEx        m_EffectSound;              ///< Effect Sound Property   
    SEffectBoundingSphere m_EffectBoundingSphere;   ///< Effect의 바운딩 스피어
	RwReal				m_fScaleForTool;			///< Tool에서 테스트를 위한 스케일 변수(저장은 되지 않는다)
};
