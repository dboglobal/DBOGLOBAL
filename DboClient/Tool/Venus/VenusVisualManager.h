//***********************************************************************************
//	File		:	CVenusVisualManager.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once

#include <d3d9.h>

#include <algorithm>

#include "NtlPLVisualManager.h"

#include "NtlEffectDefine.h"

#include "NtlResourceComponentSystem.h"

#include "NtlInstanceEffect.h"


/**
 * \ingroup Venus
 * \brief Venus의 Visual Manager
 * \date 2006-08-09
 * \author agebreak
 */
class CVenusVisualManager : public CNtlPLVisualManager
{
public:
    enum EMITTER_VERTEX
    {
        EMITTER_VERTEX_COUNT = 8,
        EMITTER_INDEX_COUNT = 48
    };

public:
	CVenusVisualManager(void);
	~CVenusVisualManager(void);

	static	CVenusVisualManager&	GetInstance(void);

	virtual RwBool	Create(void);
	virtual void	Destroy(void);
			void	Clear();

	virtual void	Update(RwReal fElapsed);
	virtual void	Render(void);	

    void    RenderEffect(RwBool bRender);                           ///< 이펙트 렌더링을 On/Off 한다.

	RwInt32	GetRenderingEffectCount();
	RwInt32 GetRenderingPolygonCount();
    RwUInt32 GetRenderingEffectUsedMemory();

	RwBool	ResetInstanceEffectFromSplinePath(const RwChar* strName);

	///< EffectEntity의 Property Data가 변경이 되었을 경우 호출이 된다.
	RwBool	ResetInstanceEffect(const RwChar* strName);
	///< EffectEntity를 삭제를 한다.
	RwBool	DeleteInstanceEffect(const RwChar* strName);

	void	EditInstanceEffect(RwInt32 nAxisType, POINT gap);
	void	EditInstanceEffect(RwMatrix* pEditMatrix);

    /// Effect가 선택영역안에 포함됬는지 체크한다.
	void	IntersectionEffect(POINT point, RwBool bShiftButton, RwBool bCtrlButton); 
	void	IntersectionEffect(POINT OldPoint, POINT CurPoint, RwBool bShiftButton, RwBool bCtrlButton); 
	void	InsertSelectEffect(CNtlInstanceEffect* pInstanceEffect, RwBool bShiftButton, RwBool bCtrlButton);

	void	AddSelectInstanceEffectPoint(RwV3d* pEditVector);
	void	SetSelectInstanceEffectPoint(RwV3d* pEditVector);
	void	SetSelectInstanceEffectPoint_X(float x);
	void	SetSelectInstanceEffectPoint_Y(float y);
	void	SetSelectInstanceEffectPoint_Z(float z);
	
	CNtlPLEntity*	CreateEventEffect(const RwChar *pKey);
	void			DeleteEventEffect(CNtlPLEntity *pEntity);

	///< 선택된 Effect System
	void	SetSelectResourceData(CNtlResourceEffect* pResourceEffect = NULL, CNtlResourceComponentSystem* pResourceComponentSystem = NULL, RwInt32 nSystemDataFlag = 0);

    /// 바운딩 스피어를 자동 생성한다.
    void    OnGenerateBoundingBoxAll();

protected:
	virtual void AlarmAutoDelete(CNtlPLEntity *pEntity);			///< 삭제되는 Effect들을 위한 가상 함수 (Repeat 기능을 위해서 오버라이딩 한다.)
    CNtlInstanceEffect* CreateEffect(RwUInt32 nIndex);              ///< 바운딩 박스를 위해서 이펙트를 생성하는 함수

public:
    CNtlResourceEffect*				m_pResourceEffect;
    CNtlResourceComponentSystem*	m_pResourceSystem;
    RwInt32							m_nSystemDataFlag;	

    typedef std::vector<CNtlInstanceEffect*> svdef_InstanceEffect;
    svdef_InstanceEffect			m_svSelectInstanceEffect;

    LPDIRECT3DDEVICE9				m_lpDevice;

    RwReal							m_fDeltaTime;

    
    RwIm3DVertex					m_EmitterVertex[EMITTER_VERTEX_COUNT];
    RwImVertexIndex					m_EmitterIndex[EMITTER_INDEX_COUNT];

protected:
    // 바운딩 박스 계산을 위한 임시 변수들
    CNtlInstanceEffect*             m_pBSCurrEffect;
    RwUInt32                        m_nBSIndex;
    RwBool                          m_bGenerateBS;
    RwReal                          m_fEffectLifeTime;

    RwInt32                         m_nEventEffectUsedMemory;                      ///< 이벤트에서 생성된 이펙트들이 사용하는 메모리
};

inline void CVenusVisualManager::SetSelectResourceData(CNtlResourceEffect* pResourceEffect, CNtlResourceComponentSystem* pResourceComponentSystem, RwInt32 nSystemDataFlag)
{
	m_pResourceEffect = pResourceEffect;
	m_pResourceSystem = pResourceComponentSystem;
	m_nSystemDataFlag = nSystemDataFlag;
}

