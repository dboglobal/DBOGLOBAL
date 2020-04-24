/*****************************************************************************
 *
 * File			: NtlPLEntity.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 8. 01	
 * Abstract		: Presentation object entity base class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_PLENTITY_H__
#define __NTL_PLENTITY_H__

#define dNTL_WORLD_CULLING_NEW

#include <string>
#include <rwcore.h>
#include <rpworld.h>
#include "NtlPLDef.h"
#include "NtlPLGlobal.h"
#include "NtlPLProperty.h"

#define ENABLE_PLENTITY_FLAGS(flags,check)  (styles & check) 

typedef struct _SPLEntityCreateParam
{
	const RwV3d *pPos;
	RwReal		fAnimSpeed;
	RwReal		fAnimOffset;
	void*		_pUserData;

public:
	inline void operator = (_SPLEntityCreateParam& _EntityCreateParam)
	{
		pPos		= _EntityCreateParam.pPos;
		fAnimSpeed	= _EntityCreateParam.fAnimSpeed;
		fAnimOffset = _EntityCreateParam.fAnimOffset;

		// careful for shallow copy
		_pUserData	= _EntityCreateParam._pUserData;
	}	

	_SPLEntityCreateParam()
	{
		_pUserData = NULL;
	}

}SPLEntityCreateParam;

class CNtlPLResource;
class CNtlElapsedControllerGroup;
class CNtlPLEntityBlendController;


/**
 * \ingroup NtlPresentation
 * Presentation entity base class이다.
 * 
 */
class CNtlPLEntity
{	
protected:
#ifdef dNTL_WORLD_CULLING_NEW
	RwUInt16	m_uiRenderFrame;
#endif

	std::string m_strName;				/**< String describing the presentation entity name */
	RwUInt32 m_uiClassID;				/**< presentation entity class type */
	RwUInt32 m_uiLayer;					/**< presentation entity layer */
	RwUInt32 m_uiMinimapLayer;			/**< presentation entity minimap layer : is flags */
	RwUInt32 m_uiFlags;					/**< presentation entity flags */
	RwUInt32 m_uiSID;					/**< trigger entity id(0xffffffff for default; means triggers are not existed) */
	RwUInt32 m_uiSerialID;				/**< simulaion layer object serial id */
#ifndef dNTL_WORLD_CULLING_NEW
	RwUInt32 m_uiCullIID;				/**< Culling Id */
#endif
	DWORD	 m_ObjNameIdx;				// object name index; not available for client
	RwUInt16 m_byPickOrder;				/**< Picking order */

	RwUInt32 m_uiCullFlags;				/**< presentation entity Culling flags */

	RwReal m_fWeightAlpha;				/**< alpha weight value */
	RwReal m_fWeightElapsedTime;		/**< update weight elapsed time */
	RwReal m_fVisibleCullingDistance;	/**< Object Culling Distance */

	CNtlElapsedControllerGroup *m_pElapController;
	CNtlPLEntityBlendController *m_pBlendController;

public:

	/**
    *  Default constructor for Presentation entity 
    *
    */
	CNtlPLEntity(); 

	/**
    *  Virtual destructor for Presentation entity 
    *
    */
	virtual ~CNtlPLEntity();

	/**
	*  entity가 생성된 다음 호출하는 interface 함수.
	*  \see Destroy
	*/
	virtual RwBool Create(const SPLEntityCreateParam *pParam = NULL) = 0;

	/**
	*  entity가 creat 함수가 호출된 다음 실행되는 interface 함수.
	*  \see Destroy
	*/
	virtual RwBool PostCreate(void) { return TRUE; }

	/**
	*  entity가 삭제되기 전에 호출하는 interface 함수.
	*  \see Create
	*/
	virtual void Destroy(void) = 0;

	/**
	*  entity update interface 함수
	*  \param fElapsed update delta time
	*  \see Render
	*/
	virtual RwBool Update(RwReal fElapsed);

	/**
	*  entity CullingTest interface 함수
	*  \see SetCull
	*  \see IsCulled
	*/
#ifdef dNTL_WORLD_CULLING_NEW
	virtual RwBool	CullingTest(RwCamera* pRwCamera, RwUInt16 uiRenderFrame);
#else
	virtual RwBool	CullingTest(RwCamera* pRwCamera);	
#endif
	//virtual RwBool	CullingTest(RwCamera* pRwCamera, const RpAtomic* pAtomic);

    /************************************************************************/
    /* When you exit and delete the current update without deleting the Entity immediately*/
    /* Interface to call                                                  */
    /************************************************************************/
    virtual void   Finish() {}

	/**
	*  entity rendering interface 함수
	*  \see Upate
	*/
	virtual RwBool Render(void) = 0;

	/**
	*  entity rendertotexture interface 함수
	*/
	virtual RwBool RenderToTexture(void) { return TRUE; }

	/**
	*  entity property setting interface 함수
	*  \param pData Property base pointer
	*/
	virtual RwBool SetProperty(const CNtlPLProperty *pData) = 0;

	/**
	*  Add renderware resource to world.
	*  \see RemoveWorld
	*/
	virtual void AddWorld(void)		{}

	/**
	*  renderware resource를 world에서 삭제한다.
	*  \see AddWorld
	*/
	virtual void RemoveWorld(void)	{}

	/**
	*  resource manager에서 scheduling 한 resource를 생성하기 전에 알려준다.
	*/
	virtual void CallPreSchedulingResource(void) {}

	/**
	*  resource manager에서 scheduling 한 resource를 알려준다.
	*/
	virtual void CallSchedulingResource(CNtlPLResource *pResource) {}
	virtual void CallSchedulingResourceOnly() {}
    virtual RwBool IsSchedulingLoadingComplete() {return TRUE;}

	/**
	*  entity position interface 함수
	*  \param pPos entity position pointer.
	*  \see GetPosition
	*/
	virtual void SetPosition(const RwV3d *pPos) { }

	/**
	*  entity direction interface function
	*  \param pDir entity dir pointer.
	*/
	virtual void SetDirection(const RwV3d *pDir) { }
    virtual RwV3d GetDirection() {RwV3d dir;dir.x = dir.y = 0.0f;dir.z = 1.0f; return dir;}

	/**
	*  entity position을 리턴하는 interface 함수.
	*  \return entity position을 리턴한다.
	*  \see SetPosition
	*/
	// by agebreak
	// Would not it be better to use the return type as a pointer or a reference?
	virtual RwV3d GetPosition(void) { return CNtlPLGlobal::m_vZeroV3; }

	/**
	*  entity scale 함수.
	*  \param fScale entity scale value.
	*  \see GetScale
	*/
	virtual void SetScale(RwReal fScale) {}

	/**
	*  entity scale value을 얻어오는 함수.
	*  \return fScale entity scale value.
	*  \see SetScale
	*/
	virtual RwReal GetScale(void) { return 1.0f; }

	/**
	*  entity scale vector을 얻어오는 함수.
	*  \return entity scale vector value.
	*  \see SetScale
	*/
	virtual const RwV3d* GetScaleVector(void) const { return NULL; }

	/**
	*  entity rotate를 시키는 함수.
	*  \param fXAngle x-axis rotate.
	*  \param fXAngle y-axis rotate.
	*  \param fXAngle z-axis rotate.
	*/
	virtual void SetRotate(RwReal fXAngle, RwReal fYAngle, RwReal fZAngle) {;}

	/**
	*  entity rotate vector을 얻어오는 함수.
	*  \return entity rotate value.
	*  \see SetRotate
	*/
	virtual const RwV3d* GetRotate(void) const { return NULL;} 

	/**
	*  entity bounding sphere을 얻어오는 함수.
	*  \return bounding sphere pointer.
	*/
	virtual const RwSphere* GetBoundingSphere(void) { return NULL; }

	/**
	*  entity bounding box을 얻어오는 함수.
	*  \return bounding box pointer.
	*/
	virtual const RwBBox* GetBoundingBox(void) { return NULL; }

	/**
	*  entity matrix setting시키는 함수.
	*  \param matWorld RwMatrix 변수.
	*/
	virtual void SetMatrix(RwMatrix& matWorld) {;}

	/**
	*  entity matrix setting시키는 함수.
	*  \return matrix 
	*/
	virtual RwMatrix& GetMatrix(void) { return CNtlPLGlobal::m_matIden; } 

	/**
	*  entity alpha 값을 setting 한다.
	*/
	virtual void SetAlpha(RwUInt8 byValue) { }

	/**
	*  entity color 값을 setting 한다.
	*/
	virtual void SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue) { }

	/**
	*  entity Addcolor 값을 setting 한다.
	*/
	virtual void SetAddColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue) { }

	/**
	*  entity distance culling value을 setting 한다.
	*/
	virtual void SetVisibleCullingDistance(RwReal fDistance);

	/** 
	*  entity 현재 distance culling value
	*  \return culling value
	*/
	virtual RwReal	GetVisibleCullingDistance(void) const { return m_fVisibleCullingDistance; }

	/**
	*  entity weight alpha value을 setting 한다.
	*/
	virtual void SetWeightAlpha(RwReal fWeightValue);

	/** 
	*  entity 현재 weight alpha
	*  \return weight alpha
	*/
	RwReal GetWeightAlpha(void) const;

	/**
	*  atomic weight alpha value을 setting 한다.
	*/
	virtual void SetAtomicWeightAlpha(const RwChar *pAtomicName, RwReal fWeightValue) {}

	/** 
	*  entity 현재 weight alpha
	*  \return weight alpha
	*/
	CNtlPLEntityBlendController* GetAlphaBlendController(void);

	/**
	*  weight elapsed time value을 setting 한다.
	*/
	void SetWeightElapsedTime(RwReal fWeightValue);

	/** 
	*  entity 현재 weight elapsed time
	*  \return weight elapsed time
	*/
	RwReal GetWeightElapsedTime(void) const;

	/** 
	*  entity 현재 weight elapsed controller 생성.
	*/
	virtual void CreateWeightElapsedController(RwReal fLifeTime, RwReal fWeightValue); 

	/** 
	*  entity 현재 weight elapsed controller 소멸.
	*/
	virtual void DeleteWeightElapsedController(void);

	/** 
	*  entity rendering on/off 시키는 함수.
	*  \param rendering on/off flag 변수.
	*/
	virtual void	SetVisible(RwBool bVisible);	
	/** 
	*  entity  현재 rendering이 되고 있는 entity 인가?
	*  \param  함께 연산될 CullFlags.
	*  \return rendering on이면 TRUE, rendering off이면 FALSE
	*/
	virtual RwBool	IsVisible(RwUInt32 uiFlags = 0xFFFFFFFF);

	/** 
	*  entity camera collision을 위해 존재하는 Visible 검사 함수로
	*  Performance 향상을 위한 Culling을 고려하지 않은 함수
	*  \return rendering 중이면 TRUE, 그렇지 않으면 FALSE
	*/
	//virtual RwBool	IsVisibleForCameraCollision(void);

	/** 
	*  entity cullflags setting 시키는 함수.
	*  \param cullflags check bit 변수.
	*  \param cullflasgs on/off 변수.
	*/
	virtual void	SetCullFlags(RwUInt32 uiFlag, RwBool bCulled);
	virtual void	SetCullFlags(RwUInt32 uiFlag);

	virtual RwUInt32 GetCullFlags();

	/** 
	*  entity picking on/off 시키는 함수.
	*  \param picking on/off flag 변수.
	*/
	virtual void SetPicking(RwBool bPicking);

	/** 
	*  entity picking order 정의 함수.
	*  \param picking order 변수.
	*/
	virtual RwUInt16 GetPickingOrder(void);

	virtual void SetPickingOrder(RwUInt16 byPickOrder);

	/** 
	*  entity weight elapsed time flag on/off 시키는 함수.
	*/
	virtual void EnableWeightElapsedTime(RwBool bEnable);
  
	/**
	*  entity name setting
	*  \see GetName
	*/
	void SetName(const RwChar *pName);

	/**
	*  entity name을 얻어오는 함수.
	*  \return entity name을 리턴한다.
	*  \see SetName
	*/
	const RwChar* GetName(void); 

	/**
	*  entity class id setting.
	*  RTTI 대체용으로써 class id를 저장함으로써 entity 종류를 판별한다.
	*  \see GetClassID
	*/
	void SetClassType(const ENtlPLEntityType eType);

	/**
	*  entity class id를 얻어오는 함수.
	*  \return class id를 리턴한다.
	*  \see SetClassID
	*/
	ENtlPLEntityType GetClassType(void) const;

	/**
	*  entity layer를 setting
	*  entity layer를 사용하여, rendering 순서를 조절할 수 있다.
	*  entity layer는 NtlPLEntity.h에 enum으로 정의되어 있고, 필요에 의해 추가할 수 있다.
	*  \see GetLayer
	*/
	void SetLayer(RwUInt32 wLayer);

	/**
	*  entity layer를 얻어 오는 함수.
	*  \return entity의 layer value.
	*  \see SetLayer
	*/
	RwUInt32 GetLayer(void) const;

	/**
	*  entity flag를 setting 한다.
	*  \see GetFlags
	*/
	virtual void SetFlags(RwUInt32 uiFlags);

	/**
	*  entity flag를 얻어온다.
	*  \see SetFlags
	*/
	RwUInt32 GetFlags(void);

	/**
	*  entity minimap layer를 setting
	*  entity minimap layer를 사용하여, 현재 minimap layer 출력에만 사용된다.
	*  entity minimap layer는 NtlPLEntity.h에 enum으로 정의되어 있고, 필요에 의해 추가할 수 있다.
	*  \see GetMinimapLayer
	*/
	void SetMinimapLayer(RwUInt32 uiLayerFlags);

	/**
	*  entity layer를 얻어 오는 함수.
	*  \return entity의 minimap layer flags value.
	*  \see SetMinimapLayer
	*/
	RwUInt32 GetMinimapLayer(void) const;

	/**
	*  simulation object serial id를 setting 한다.
	*  \see GetSerialID
	*/
	void SetSerialID(RwUInt32 uiSerialID);

	/**
	*  simulation object serial id를 얻어온다.
	*  \see SetSerialID
	*/
	RwUInt32 GetSerialID(void);

	VOID		SetSID(RwUInt32 _uiSID);
	RwUInt32	GetSID(VOID);
	VOID		SetObjNameIdx(DWORD _Idx);
	DWORD		GetObjNameIdx();

#ifndef dNTL_WORLD_CULLING_NEW
	void		SetCullingID(RwUInt32 uiCullID);
	RwUInt32	GetCullingID(void);
#endif

	/**
	*  entity에 layer 및 class id와 layer가 유효한 데이터를 가지고 있는지를 판별하여, entity에 정확한 정보가 들어
	*  있는지를 검사하는 함수.
	*  \return class id 및 layer에 유효한 정보가 들어 있으면 true, 그렇지 않으면 false
	*/
	RwBool IsValid(void) const;

	/**
	*  update time에 자동으로 삭제되는 entity인가..
	*/
	virtual RwBool IsAutoDelete(void);


//-------------------------------------------------------------------------------
// event function
//-------------------------------------------------------------------------------

	// 용신소환시 수행되는 함수
	virtual void OnDragonSkyAppearence(RwBool _FlgDragonAppeared) {}

	/**
	*  entity 가 click 되었을 때 발생한다.
	*/
	virtual void	OnClicked(void) {}

	/**
	*  entity 가 double click 되었을 때 발생한다.
	*/
	virtual void	OnDbClicked(void) {}

	/**
	*  entity에 mouse focus가 들어올 때 발생한다.
	*/
	virtual void	OnGotFocus(void) {}

	/**
	*  entity에 mouse focus를 잃어버렸을 때 호출한다.
	*/
	virtual void	OnLostFocus(void) {}

//-------------------------------------------------------------------------------
// World Editor를 위한 트리거 정보를 가져오는 인터페이스 (by agebreak 2006-11-28)
//-------------------------------------------------------------------------------
    virtual RwBBox  GetTriggerAABBInfo() {RwBBox bbbox; ZeroMemory(&bbbox, sizeof(RwBBox)); return bbbox;}                                  ///< 월드에디터의 트리거 정보에 사용하기 위한 AABB의 정보를 반환한다.
    virtual RwBBox  GetTriggerAABBInfo(const RwV3d& vPos, const RwV3d& vRotate, const RwV3d& vScale) {RwBBox bbbox; ZeroMemory(&bbbox, sizeof(RwBBox)); return bbbox;}

};

inline VOID	CNtlPLEntity::SetObjNameIdx(DWORD _Idx)
{
	m_ObjNameIdx = _Idx;
}

inline DWORD CNtlPLEntity::GetObjNameIdx()
{
	return m_ObjNameIdx;
}

#ifndef dNTL_WORLD_CULLING_NEW
inline void CNtlPLEntity::SetCullingID(RwUInt32 uiCullID)
{
	m_uiCullIID = uiCullID;
}

inline RwUInt32 CNtlPLEntity::GetCullingID(void)
{
	return m_uiCullIID;
}
#endif

inline void CNtlPLEntity::SetVisible(RwBool bVisible)
{
	if(bVisible)
		m_uiFlags = m_uiFlags & (~NTL_PLEFLAG_NOT_VISIBLE);
	else
		m_uiFlags = m_uiFlags | NTL_PLEFLAG_NOT_VISIBLE;
}

inline void CNtlPLEntity::SetCullFlags(RwUInt32 uiFlag, RwBool bCulled)
{
	if(bCulled)
		m_uiCullFlags = m_uiCullFlags | uiFlag;
	else
		m_uiCullFlags = m_uiCullFlags & (~uiFlag);
}

inline void CNtlPLEntity::SetCullFlags(RwUInt32 uiFlag)
{
	m_uiCullFlags = uiFlag;
}

inline RwUInt32 CNtlPLEntity::GetCullFlags()
{
	return m_uiCullFlags;
}

inline RwBool CNtlPLEntity::IsVisible(RwUInt32 uiFlags)
{
	// USER VISIBLE CHECK
	if ((m_uiFlags & NTL_PLEFLAG_NOT_VISIBLE))
	{
		return FALSE;
	}

    if(CNtlPLGlobal::m_bEntityNotCulling)
	{
        return TRUE;
	}

#ifdef dNTL_WORLD_CULLING_NEW
	// If m_uiRenderFrame is equal to the initial value (-1), it is an entity that does not perform CullingTest.
	if (uiFlags)
	{
		uiFlags &= m_uiCullFlags;
		// PVS Flags are owned by every Entitiy. PVS Flags exist even if CullingTest is not performed.
		if (((uiFlags & NTL_PLEFLAG_CULLED_PVS) && CNtlPLGlobal::m_UseTerrainPVSMode))
		{
			return FALSE;
		}		
		// Except for PVS, the Flag should have the same RenderFrame but is up-to-date and not different.
		// However, if the RenderFrame is -1 and the FrameTest is not used, the Flag information has no value.
		if (m_uiRenderFrame == static_cast<RwUInt16>(-1))
		{
			return TRUE;
		}
		
		if ((CNtlPLGlobal::m_uiRenderFrame != m_uiRenderFrame) ||
			(uiFlags & (NTL_PLEFLAG_CULLED_CAMERA_FRUSTUM_OUT | NTL_PLEFLAG_CULLED_OCCLUDER)))
		{
			return FALSE;
		}
	}
	return TRUE;
#endif

#ifndef dNTL_WORLD_CULLING_NEW
	// CULL VISIBLE CHECK
	if (uiFlags)
	{
		uiFlags &= m_uiCullFlags;
		if (((uiFlags & NTL_PLEFLAG_CULLED_PVS) && CNtlPLGlobal::m_UseTerrainPVSMode) ||
			(uiFlags & NTL_PLEFLAG_CULLED_CAMERA_FRUSTUM_OUT) ||
			(uiFlags & NTL_PLEFLAG_CULLED_OCCLUDER))
		{
			return FALSE;
		}
	}

	return TRUE;
#endif
}

/*inline RwBool CNtlPLEntity::IsVisibleForCameraCollision(void)
{
	if ( m_uiFlags & NTL_PLEFLAG_NOT_VISIBLE )
	{
		return FALSE;
	}
	return TRUE;
}*/

inline void CNtlPLEntity::SetPicking(RwBool bPicking)
{
	if(bPicking)
		m_uiFlags = m_uiFlags | NTL_PLEFLAG_PICKING;
	else
		m_uiFlags = m_uiFlags & (~NTL_PLEFLAG_PICKING);
}

inline RwUInt16 CNtlPLEntity::GetPickingOrder(void)
{
	return m_byPickOrder;
}

inline void CNtlPLEntity::SetPickingOrder(RwUInt16 byPickOrder)
{
	m_byPickOrder = byPickOrder;
}

inline void CNtlPLEntity::EnableWeightElapsedTime(RwBool bEnable)
{
	if(bEnable)
		m_uiFlags = m_uiFlags | NTL_PLEFLAG_WEIGHT_ELAPSED_TIME;
	else
		m_uiFlags = m_uiFlags & (~NTL_PLEFLAG_WEIGHT_ELAPSED_TIME);
}


inline void CNtlPLEntity::SetName(const RwChar *pName)
{
	if(pName)
	{
		m_strName = pName;
	}
}

inline const RwChar* CNtlPLEntity::GetName(void)
{
	return m_strName.c_str();
}


inline void CNtlPLEntity::SetClassType(const ENtlPLEntityType eType)
{
	m_uiClassID = eType;
}

inline ENtlPLEntityType CNtlPLEntity::GetClassType(void) const
{
	return (ENtlPLEntityType)m_uiClassID;
}

inline void CNtlPLEntity::SetLayer(RwUInt32 uiLayer)
{
	m_uiLayer = uiLayer;
}

inline RwUInt32 CNtlPLEntity::GetLayer(void) const
{
	return m_uiLayer;
}

inline void CNtlPLEntity::SetMinimapLayer(RwUInt32 uiLayerFlags)
{
	m_uiMinimapLayer = uiLayerFlags;
}

inline RwUInt32 CNtlPLEntity::GetMinimapLayer(void) const
{
	return m_uiMinimapLayer;
}


inline void CNtlPLEntity::SetFlags(RwUInt32 uiFlags)
{
	m_uiFlags = uiFlags;
}

inline RwUInt32 CNtlPLEntity::GetFlags(void)
{
	return m_uiFlags;
}

inline void CNtlPLEntity::SetSerialID(RwUInt32 uiSerialID)
{
	m_uiSerialID = uiSerialID;
}

inline RwUInt32 CNtlPLEntity::GetSerialID(void)
{
	return m_uiSerialID;
}

inline void CNtlPLEntity::SetSID(RwUInt32 _uiSID)
{
	m_uiSID = _uiSID;
}

inline RwUInt32 CNtlPLEntity::GetSID(void)
{
	return m_uiSID;
}


inline RwReal CNtlPLEntity::GetWeightAlpha(void) const
{
	return m_fWeightAlpha;
}


inline void CNtlPLEntity::SetWeightElapsedTime(RwReal fWeightValue)
{
	m_fWeightElapsedTime = fWeightValue;
}


inline RwReal CNtlPLEntity::GetWeightElapsedTime(void) const
{
	return m_fWeightElapsedTime;
}

inline RwBool CNtlPLEntity::IsValid(void) const
{
	return (m_uiLayer == PLENTITY_LAYER_INVALID || m_uiClassID == PLENTITY_INVALID_TYPE)  ? FALSE : TRUE;
}


inline RwBool CNtlPLEntity::IsAutoDelete(void)
{
	return FALSE;
}

#endif
