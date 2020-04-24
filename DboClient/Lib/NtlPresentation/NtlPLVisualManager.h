/*****************************************************************************
 *
 * File			: NtlPLSceneManager.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 8. 01	
 * Abstract		: Presentation scene manager base class 
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_PLVISUALMANAGER_H__
#define __NTL_PLVISUALMANAGER_H__

#include <map>
#include "rpcollis.h"
#include "ceventhandler.h"
#include "NtlPLSceneManager.h"
#include "NtlPLRenderGroup.h"
#include "NtlPLCollision.h"
#include "NtlPLCullingScheduling.h"

class CNtlPLRenderGroup;
class CNtlPLDistanceFilterGroup;
class CNtlPLAtomicSorterContainter;
class CNtlPLDojoContainer;

class CNtlPLDummyWorld;
class CNtlPLWorldEntity;
class CNtlPLSky;
class CNtlPLFog;
class CNtlPLWater;
class CNtlPLPlant;
class CNtlPicking;


/**
 * \ingroup NtlPresentation
 * client application에 visual 객체의 관리하는 class이다.
 * PL entity들은 각자의 layer를 가지고 있고, layer 순서되로 update 및 rendering 한다.
 * renderware에서는 alpha sorting이 안되고 있으므로 , object 단위로 layer를 이용하여 alpha sorting을 한다.
 * 또한 PL entity를 생성 및 소멸의 interface를 제공하고 있으며, 생성/소멸 관리를 은폐시킬수 있으며, 
 * factory 방식으로 생성/소멸을 관리한다.
 * PL entity를 추가/삭제 interface 함수를 사용함으로써, PL entity 객체 관리를 은폐시킬 수 있으며, 
 * stl 연관 배열 map을 사용한다.
 *
 */

class CNtlPLVisualManager : public CNtlPLSceneManager//, public RWS::CEventHandler
{
protected:

	EActiveWorldType m_ActiveWorldType;

	// active
	RwBool m_bThreadLoad;

	// sorting을 위한 container를 준비한다.
	CNtlPLAtomicSorterContainter *m_pSortContainer;

	// Dojo Data
	CNtlPLDojoContainer	*m_pDojoContainer;
	
	CNtlPLDummyWorld	*m_pRWWorldEntity;		
	CNtlPLWorldEntity	*m_pWorldEntity;		/** 속도를 빠르게 하기 위한 참조 변수. */
	CNtlPLSky			*m_pSkyEntity;			/** 속도를 빠르게 하기 위한 참조 변수. */
	CNtlPLFog			*m_pFogEntity;			/** 속도를 빠르게 하기 위한 참조 변수. */	
	CNtlPLPlant			*m_pPlantEntity;		/** 속도를 빠르게 하기 위한 참조 변수. */	
	CNtlPLWater			*m_pWaterEntity;
	CNtlPicking			*m_pPickingObj;			/** 렌더링 Picking **/
    CNtlPLEntity        *m_pSoundListener;      /** Sound를 듣는 위치가 되는 리스너 객체 */

	typedef std::map< RwInt32, CNtlPLRenderGroup*, std::less<RwUInt32> > MapRenderGroup;
	MapRenderGroup m_mapInstanceGroup;	/** instance를 보관한다. */
	MapRenderGroup m_mapUpdateGroup;	/** update 하는 entity을 따로 보관한다. */
	MapRenderGroup m_mapRenderGroup;	/** rendering 하는 entity을 따로 보관한다. */

	typedef std::map<CNtlPLEntity*, CNtlPLEntity*> MapReservedUpdate;
	MapReservedUpdate m_mapAddUpdate;
	MapReservedUpdate m_mapRemoveUpdate;
	
	// distance filtering
	RwBool m_bDistFiter;							/** object distance filtering on/off flag */    
	RwReal m_fDistFilterTime;						/** object distance filter time */
	RwReal m_fDistFilterDetectTime;					/** object distance filter detecting time */
	CNtlPLDistanceFilterGroup *m_pDistFiterGroup;	/** object distance filter group */

	// auto delete call
	RwUInt32	m_uiAutoDelAlarmFlags;

    // 렌더 레이어 Visible 테스트용 
    std::map<RwUInt32, RwBool> m_mapRenderVisible;

	// Culling Scheduling
	CNtlPLCullingScheduling	m_CullScheduler;

protected:
		
	void RegisterInstanceGroup(const RwInt32 uiClassType, CNtlPLRenderGroup *pRenderGroup);

	void RegisterUpdateGroup(const RwInt32 uiClassType, CNtlPLRenderGroup *pRenderGroup);

	void RegisterRenderingGroup(const RwInt32 uiRenderLayer, CNtlPLRenderGroup *pRenderGroup);


	RwBool GetRWWorldHeight(const RwV3d *pWorldPos, RwReal& fHeight, RwV3d *pNormal, RwReal fLineLen);

	RwBool GetHeightFieldWorldHeight(const RwV3d *pWorldPos, RwReal& fHeight, RwV3d *pNormal, RwReal fLineLen);

	RwBool GetRWTerrainHeight(const RwV3d *pWorldPos, RwReal& fHeight);

	RwBool GetHeightFieldTerrainHeight(const RwV3d *pWorldPos, RwReal& fHeight);


	void UpdateDistanceFilter(RwReal fElapsed);		

	void UpdateSound(RwReal fElapsed);

	void ProcReservedAdd(void);

	void ProcReservedRemove(void);

	void AddReservedUpdate(CNtlPLEntity *pEntity);

	void RemoveReservedUpdate(CNtlPLEntity *pEntity);

	// auto delete marking flag
	void MarkAutoDeleteFlag(ENtlPLEntityType eType);

protected:

	virtual void AlarmAutoDelete(CNtlPLEntity *pEntity) {}

public:

	CNtlPLRenderGroup* FindInstanceGroup(const RwInt32 uiClassType);

	CNtlPLRenderGroup* FindUpdateGroup(const RwInt32 uiClassType);

	CNtlPLRenderGroup* FindRenderingGroup(const RwInt32 uiRenderLayer);
	
public:

	/**
    *  Default constructor for Presentation visual manager 
    *
    */
	CNtlPLVisualManager();

	/**
    *  Virtual destructor for Presentation vitual manager 
    *
    */
	~CNtlPLVisualManager();

	/**
	*  visual manager 객체가 생성된 다음 호출하는 함수.
	*  \see Destroy
	*/
	virtual RwBool Create(void);
	
	/**
	*  visual manager 삭제되기 전에 호출하는 함수.
	*  \see Create
	*/
	virtual void Destroy(void);

	/**
	*  vitual manager update interface 함수
	*  \param fElapsed update delta time
	*/
	virtual void Update(RwReal fElapsed);

	/**
    *  presentation scene manager update 전에 ui에 entity를 rendering 하기 위한 함수.
	* render ware는 camera update들이 겹치면 안된다.
	* UI도 camera texture를 사용하므로, main scene camera와 update가 겹치면 안된다.
    *
    */
	virtual void UpdateBeforeCamera(RwReal fElapsed);

	/**
	*  vitual manager render interface 함수
	*/
	virtual void Render(void);
	virtual void Render4RWWorld(void);

	/**
    *  visual manager postrender 함수.
    *
    */
	virtual void PostRender(void);

	/**
    *  presentation scene active on/off flag.
    *
    */
	virtual void SetThreadLoad(RwBool bThreadLoad);

	/**
    *  reset gui manager
    *
    */
	virtual void ResetGuiManager(RwInt32 iScreenWidth, RwInt32 iScreenHeight);

	/**
	*  PL entity 생성 interface 함수.
	*  \param pEntityName는 entity name.
	*  \param pPropertyName는 entity에 적용될 property name.
	*  \return 생성된 entity pointer
	*  \see DeleteEntity
	*/
	virtual CNtlPLEntity* CreateEntity(ENtlPLEntityType eType, const RwChar *pKey, const SPLEntityCreateParam *pParam = NULL);

	/**
	*  PL entity 소멸 interface 함수.
	*  \param pEntity는 유효한 entity instance pointer.
	*  \see CreateEntity
	*/
	virtual void DeleteEntity(CNtlPLEntity *pEntity);

	/**
	*  PL entity를 entity containter에 add하는 interface 함수.
	*  \param pEntity는 유효한 entity instance pointer.
	*  \return 성공하면 TRUE, 실패하면 FALSE
	*  \see RemovePLEntity
	*/
	virtual RwBool AddPLEntity(CNtlPLEntity *pEntity);

	/**
	*  PL entity를 entity containter에서 remove하는 interface 함수.
	*  \param pEntity는 유효한 entity instance pointer.
	*  \see RemovePLEntity
	*/
	virtual void RemovePLEntity(CNtlPLEntity *pEntity);

	/**
    *  entity를 scene manager의 update에 add하는 interface 함수.
	*  \param pEntity scene manager에 add할 entity pointer
	*  \see RemoveUpdate
    *
    */
	virtual void AddUpdate(CNtlPLEntity *pEntity);

	/**
    *  entity를 scene manager의 update에 remove하는 interface 함수.
	*  \param pEntity scene manager에 remove할 entity pointer
	*  \see AddUpdate
    *
    */
	virtual void RemoveUpdate(CNtlPLEntity *pEntity);

	/**
    *  entity를 distance filtering manager에 추가한다.
	*  \param pEntity distance filtering manager에 add할 entity pointer
	*  \see RemoveDistanceFilter
    *
    */
	virtual void AddDistanceFilter(CNtlPLEntity *pEntity);

	/**
    *  entity를 distance filtering manager에 삭제한다.
	*  \param pEntity distance filtering manager에 remove할 entity pointer
	*  \see AddDistanceFilter
    *
    */
	virtual void RemoveDistanceFilter(CNtlPLEntity *pEntity);

	/**
    *  현재 active 되어 있는 world type(renderware world or height field인가?)
    *
    */
	virtual EActiveWorldType GetActiveWorldType(void);

	/**
	*  world 가 준비 되어 있는가?
	*  \return ready is TRUE or FALSE
	*
	*/
	virtual RwBool IsWorldReady(void);

	/**
    *  world position에 해당하는 terrain의 height를 구하는 interface 함수.
	*  \return terrain height value를 리턴한다.
	*  \param pWorldPos world position에 해당하는 RwV3d pointer
	*
    */
	virtual RwBool GetWorldHeight(const RwV3d *pWorldPos, RwReal& fHeight, RwV3d *pNormal, RwReal fLineLen = 1000.0f);

	/**
    *  world position에 해당하는 terrain 지형만 해당하는 height를 구하는 interface 함수.
	*  \return terrain height value를 리턴한다.
	*  \param pWorldPos world position에 해당하는 RwV3d pointer
	*
    */
	virtual RwBool GetTerrainHeight(const RwV3d *pWorldPos, RwReal& fHeight);


	/**
    *  world에 pick된 polygon을 찾는다.
	*  \return 충동된 polygon의 좌표.
    *
    */

	virtual RwBool PickWorld(RwInt32 iPosX, RwInt32 iPosY, SWorldPickInfo& sPickInfo, RwReal fPickTerrainLimit, RwReal fPickObjLimit);

	//RwBool PickWorld_Old(RwInt32 iPosX, RwInt32 iPosY, SWorldPickInfo& sPickInfo, RwReal fPickTerrainLimit, RwReal fPickObjLimit);
	RwBool PickWorld_New(RwInt32 iPosX, RwInt32 iPosY, SWorldPickInfo& sPickInfo, RwReal fPickTerrainLimit, RwReal fPickObjLimit);

	virtual RwBool PickTerrian(RwV3d& vStartPos, RwV3d& vEndPos, SWorldPickInfo& sPickInfo);
	virtual RwBool PickTerrianAndWater(RwV3d& vStartPos, RwV3d& vEndPos, SWorldPickInfo& sPickInfo);

	/**
    *  camera 충돌 검사를 하여, 새로운 충돌 camera 위치를 구한다.
	*  \param1 pCameraPos 현재 camera의 position
	*  \param2 pCameraDir 현재 camera의 direction
	*  \param2 새로운 camera의 position
	*
    */
	virtual RwBool GetCameraCollision(const RwV3d *pCameraPos, const RwV3d *pLookAt, RwReal fRadius, RwV3d& vNewPos);

	/**
	*  RpWorld pointer를 얻어오는 interface 함수.
	*
	*/
	virtual RpWorld* GetWorldPtr(void);

	// woody1019
// 	RwBool CTChar2Poly(sNPE_COLLISION_PARAM& sNPECollisionParam);
// 	RwBool CTLine2Sphere(RwLine* _pLine, sNPE_COLLISION_PARAM& sNPECollisionParam);

	/**
	*  visual manager의 event handler
	*  \param pMsg는 event가 발생할 경우 넘어오는 message.
	*/
	//virtual void HandleEvents(RWS::CMsg &pMsg);
	
    // Object의 Fade 관련 함수
	virtual void	SetDistanceFilter(RwBool bEnable);        ///< Object의 Fade기능을 끄거나 켠다. (Map Tool에서 사용)
    virtual RwBool  GetDistanceFilterEnable() {return m_bDistFiter;}                        ///< 현재 Fade On/Off 상태를 반환한다.

	/**
	* world attribute
	* \return world의 attribute value를 리턴한다.
	* \see GetWorldNormalAttribute
	* \see GetWorldSpecialAttribute
	*/
	virtual DWORD GetWorldAttribute(RwV3d vPos);

	/**
	* world normal attribute
	* \return world의 normal attribute value를 리턴한다.
	* \see GetWorldSpecialAttribute
	*/
	virtual DWORD GetWorldNormalAttribute(RwV3d vPos);

	/**
	* world special attribute(일반속성 과 특수속성으로 나뉜다)
	* \return world의 special attribute value를 리턴한다.
	* \see GetWorldNormalAttribute
	*/
	virtual DWORD GetWorldSpecialAttribute(RwV3d vPos);

	/**
	* world material attribute
	* \return world의 material attribute value를 리턴한다.
	*/
	virtual BYTE GetWorldMaterialAttribute(RwV3d vPos);

	/**
	* 일반적으로 world의 normal attribute를 체크하여 물일 경우에만 사용한다.
	* \return world의 water 높이.
	*/
	virtual RwReal GetWorldWaterHeight(RwV3d vPos);

	/**
	* 현재 위치의 Bloom effect power factor를 구해온다.
	* \return if TRUE 이면 값이 유효하고, FALSE이면 유효하지 않다.
	*/
	virtual RwReal					GetActiveBloomFactor(void); 

	virtual VOID					SetActiveSky(CNtlPLSky* _pNtlPLSky) { m_pSkyEntity = _pNtlPLSky; }

	virtual CNtlPLDojoContainer*	GetDojoContainer(void)		{ return m_pDojoContainer; } 

	virtual CNtlPLDummyWorld*		GetRwWorld(void)			{ return m_pRWWorldEntity; }
	virtual CNtlPLWorldEntity*		GetWorld(void)				{ return m_pWorldEntity; }
	virtual CNtlPLSky*				GetSky(void)				{ return m_pSkyEntity; }
	virtual CNtlPLFog*				GetFog(void)				{ return m_pFogEntity; }
	virtual CNtlPLPlant*			GetPlant(void)				{ return m_pPlantEntity; }
	virtual CNtlPLWater*			GetWater(void)				{ return m_pWaterEntity; }
	virtual CNtlPicking*			GetPickingObject( void )	{ return m_pPickingObj; }

	virtual RwBool GetAvailablePos(RwV3d& _CurPos);

	virtual VOID SetWorldPVSActivation(RwBool _Flag);

	virtual RwBool GetWorldTerrainRotate(RwMatrix* pmatOut, RwV3d* pvPos, RwBool bPosHeightChange, RwReal fLineLen = 1000.0f);

    virtual VOID SetSoundListener(CNtlPLEntity* pEntity);

	virtual RwBool GetWorldShadowColor(RwV3d& vPos, RwRGBA* pOutColor);

	virtual RwBool GetWorldLightColor(RwV3d& vPos, RwRGBA* pOutColor, RwBool bCalcShadow);

    virtual void   SetEntityVisible(ENtlPLEntityType eType, RwBool bVisible);             ///< 렌더레이어의 렌더링을 On/Off 한다. (테스트용)

	virtual void	SetVisiblePlanet(RwBool bVisible);

	virtual void	RunableRainBloomLight(RwReal fBloomShaderFadeInTime, RwReal fBloomShaderFadeOutTime);

	virtual void	RunableRainSkyLight(RwReal fSkyFadeInTime, RwReal fSkyFadeOutTime);
};

#endif