//***********************************************************************************
//	File		:	NtlResourceComponentSystem.h
//	Desc		:	
//	Begin		:	2006. 7.28
//	Copyright	:	ⓒ 2006 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once

#include "rwcore.h"

#include "rpprtstd.h"
#include "rpprtadv.h"

#include <string>

#include "NtlEffectDefine.h"

class CNtlResourceComponentSystem
{
public:
	enum ENtlResourceSystemType
	{
		RESOURCE_SYSTEMTYPE_NONE = 0x00,
		RESOURCE_SYSTEMTYPE_PARTICLE,
		RESOURCE_SYSTEMTYPE_MESH,
		RESOURCE_SYSTEMTYPE_BEAM,
		RESOURCE_SYSTEMTYPE_HURRICANE,
		RESOURCE_SYSTEMTYPE_LIGHTNING,
		RESOURCE_SYSTEMTYPE_DECAL,
		RESOURCE_SYSTEMTYPE_POST_EFFECT,
        RESOURCE_SYSTEMTYPE_TRACE,
        RESOURCE_SYSTEMTYPE_LINE,
        RESOURCE_SYSTEMTYPE_UPGRADE,
        RESOURCE_SYSTEMTYPE_TRAIL,
        RESOURCE_VERTEX_1024,                       /// 리소스 시스템은 아니지만, Vertex 메모리 풀을 위해 추가한다.
        RESOURCE_VERTEX_5120,                       /// 리소스 시스템은 아니지만, Vertex 메모리 풀을 위해 추가한다.
	};

//#ifdef RWDEBUG
	RwBool				m_bShow;
//#endif
	RwInt32				m_nSystemType;
	std::string			m_strName;

	RwUInt32			m_nEmitterDataFlag;

	RwBool				m_bSubSystem;

	RwBool				m_bEternity;
	RwReal				m_fStartTime;
	RwReal				m_fLifeTime;

	RwUInt32					m_nPathEmitterDataFlag;
	SNtlPrtEmitterSplinePath	m_EmitterPrtSplinePath;
	SNtlPrtStdEmitterPrtFadeOut	m_EmitterPrtFadeOut;

	RpPrtAdvEmtPrtMultiTextures		m_EmitterMultiTextures;

	RwBool							m_bAlphaBlend;
	RwBlendFunction					m_nSrcBlend;
	RwBlendFunction					m_nDestBlend;

	RwV3d				m_vPosition;
	RwV3d				m_vRotate;

	RwMatrix			m_matWorld;
	RwMatrix			m_matPathWorld;

    SGroundFlag         m_sGroundFlag;              ///< 지면에서 표시될 이펙트를 설정하는 구조체
    RwReal              m_fBoundSphereRadius;       ///< 바운딩 스피어 반지름    

public:
	CNtlResourceComponentSystem(const RwChar* strName);
	virtual ~CNtlResourceComponentSystem(void);

	virtual void	Destroy();

			void	RebuildMatrix();
			void	RebuildPathMatrix();

	virtual RwBool	Load(FILE* pFile);
			RwBool	Load(FILE* pFile, std::string& string);

	const	RwInt32	GetSystemType()											{ return m_nSystemType; }
	const	RwChar*	GetSystemName()											{ return m_strName.c_str(); }
			void	SetSystemName(RwChar* strName)							{ m_strName = strName; }

            RwReal  GetBoundingSphereRadius()                               {return m_fBoundSphereRadius;}
            void    SetBoundingSphereRadius(RwReal fRadius);

	RwBool	IsEmitterDataFlag(RwUInt32 flag)								{ return (m_nEmitterDataFlag & flag) ? TRUE : FALSE; }
};