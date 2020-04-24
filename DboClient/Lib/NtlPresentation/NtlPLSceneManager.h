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

#ifndef __NTL_PLSCENEMANAGER_H__
#define __NTL_PLSCENEMANAGER_H__

#include <rwcore.h>
#include <rpworld.h>
#include "NtlPLEntity.h"
#include "NtlPLSceneManagerFactory.h"
#include "NtlPLPicking.h"

class	CNtlPLDojoContainer;

class	CNtlPLFog;
class	CNtlPLSky;
class	CNtlPLPlant;
class	CNtlPlLightEvent;
class   CNtlPLWorldEntity;
struct	sNPE_COLLISION_PARAM;

enum EActiveWorldType
{
	AW_NONE,
	AW_RWWORLD,
	AW_HEGITHFIELD
};

/**
 * \ingroup NtlPresentation
 * presentation의 scene manager 추상화 class 이다.
 * 기능적인 위주의 추상함수로 구성되어 있다.
 * Dbo client와 editor에서는 이 class를 상속받아 재정의 한다.
 *
 */
class CNtlPLSceneManager
{
public:

	/**
    *  presentation scene manager가 생성된 다음 호출하는 interface 함수.
	*  \return 성공하면 true를 돌려주고, 실패하면 false를 돌려준다.
    *
    */
	virtual RwBool Create(void) = 0;

	/**
    *  presentation scene manager가 소멸되기 전에 호출하는 interface 함수.
    *
    */
	virtual void Destroy(void) = 0;


	/**
    *  presentation scene manager update 함수.
    *
    */
	virtual void Update(RwReal fElapsed) = 0;

	/**
    *  presentation scene manager update 전에 ui에 entity를 rendering 하기 위한 함수.
	* render ware는 camera update들이 겹치면 안된다.
	* UI도 camera texture를 사용하므로, main scene camera와 update가 겹치면 안된다.
    *
    */
	virtual void UpdateBeforeCamera(RwReal fElapsed) = 0;


	/**
    *  presentation scene manager render 함수.
    *
    */
	virtual void Render(void) = 0;
	virtual void Render4RWWorld(void) = 0;

	/**
    *  presentation scene manager postrender 함수.
    *
    */
	virtual void PostRender(void) = 0;

	/**
    *  presentation scene active on/off flag.
    *
    */
	virtual void SetThreadLoad(RwBool bThreadLoad) = 0;


	/**
    *  reset gui manager
    *
    */
	virtual void ResetGuiManager(RwInt32 iScreenWidth, RwInt32 iScreenHeight) = 0;

	/**
    *  entity를 create 하는 factory interface  함수.
	*  entity factory class를 따로 만들것인지를 검토해야 한다.
	*  \return 성공하면 entity base pointer를 돌려주고, 실패하면 NULL를 돌려준다.
	*  \see DeleteEntity
    *
    */
	virtual CNtlPLEntity* CreateEntity(ENtlPLEntityType eType, const RwChar *pKey, const SPLEntityCreateParam *pParam = NULL) = 0;

	/**
    *  entity를 delete 하는 factory interface 함수.
	*  entity factory class를 따로 만들것인지를 검토해야 한다.
	*  \param pEntity 삭제할 entity pointer
	*  \see CreateEntity
    *
    */
	virtual void DeleteEntity(CNtlPLEntity *pEntity) = 0;

	/**
    *  entity를 scene manager에 add interface 함수.
	*  \param pEntity scene manager에 추가할 entity pointer
	*  \see RemovePLEntity
    *
    */
	virtual RwBool AddPLEntity(CNtlPLEntity *pEntity) = 0;

	/**
    *  entity를 scene manager에서 remove하는 interface 함수.
	*  \param pEntity scene manager에 제거할 entity pointer
	*  \see AddPLEntity
    *
    */
	virtual void RemovePLEntity(CNtlPLEntity *pEntity) = 0;


	/**
    *  entity를 scene manager의 update에 add하는 interface 함수.
	*  \param pEntity scene manager에 add할 entity pointer
	*  \see RemoveUpdate
    *
    */
	virtual void AddUpdate(CNtlPLEntity *pEntity) = 0;

	/**
    *  entity를 scene manager의 update에 remove하는 interface 함수.
	*  \param pEntity scene manager에 remove할 entity pointer
	*  \see AddUpdate
    *
    */
	virtual void RemoveUpdate(CNtlPLEntity *pEntity) = 0;

	/**
    *  entity를 distance filtering manager에 추가한다.
	*  \param pEntity distance filtering manager에 add할 entity pointer
	*  \see RemoveDistanceFilter
    *
    */
	virtual void AddDistanceFilter(CNtlPLEntity *pEntity) = 0;

	/**
    *  entity를 distance filtering manager에 삭제한다.
	*  \param pEntity distance filtering manager에 remove할 entity pointer
	*  \see AddDistanceFilter
    *
    */
	virtual void RemoveDistanceFilter(CNtlPLEntity *pEntity) = 0;

	/**
    *  현재 active 되어 있는 world type(renderware world or height field인가?)
    *
    */
	virtual EActiveWorldType GetActiveWorldType(void) = 0;

	/**
	*  world 가 준비 되어 있는가?
	*  \return ready is TRUE or FALSE
	*
	*/
	virtual RwBool IsWorldReady(void) = 0;

	/**
    *  world position에 해당하는 terrain 및 오브젝트를 포함한 world의 height를 구하는 interface 함수.
	*  \return terrain or object height value를 리턴한다.
	*  \param pWorldPos world position에 해당하는 RwV3d pointer
	*
    */
	virtual RwBool GetWorldHeight(const RwV3d *pWorldPos, RwReal& fHeight, RwV3d *pNormal, RwReal fLineLen = 1000.0f) = 0;

	/**
    *  world position에 해당하는 terrain 지형만 해당하는 height를 구하는 interface 함수.
	*  \return terrain height value를 리턴한다.
	*  \param pWorldPos world position에 해당하는 RwV3d pointer
	*
    */
	virtual RwBool GetTerrainHeight(const RwV3d *pWorldPos, RwReal& fHeight) = 0;

	/**
    *  mouse picking시에 picking된 entity를 검출하는 interface 함수.
	*  \param iPosX mouse의 screen x-좌표
	*  \param iPosX mouse의 screen y-좌표
	*
    */
	virtual RwBool PickWorld(RwInt32 iPosX, RwInt32 iPosY, SWorldPickInfo& sPickInfo, RwReal fPickTerrainLimit, RwReal fPickObjLimit) = 0;

	/**
	*  World의 물과 지형에 Ray InterSection을 실행한다.
	*/
	virtual RwBool PickTerrian(RwV3d& vStartPos, RwV3d& vEndPos, SWorldPickInfo& sPickInfo) = 0;
	virtual RwBool PickTerrianAndWater(RwV3d& vStartPos, RwV3d& vEndPos, SWorldPickInfo& sPickInfo) = 0;

	/**
    *  camera 충돌 검사를 하여, 새로운 충돌 camera 위치를 구한다.
	*  \param1 pCameraPos 현재 camera의 position
	*  \param2 pLookAt 현재 camera의 look at position
	*  \param2 새로운 camera의 position
	*
    */
	virtual RwBool GetCameraCollision(const RwV3d *pCameraPos, const RwV3d *pLookAt, RwReal fRadius, RwV3d& vNewPos) = 0;

	/**
	*  character 충돌 처리 함수.
	*/
// 	virtual RwBool CTChar2Poly(sNPE_COLLISION_PARAM& sNPECollisionParam) = 0;
// 	virtual RwBool CTLine2Sphere(RwLine* _pLine, sNPE_COLLISION_PARAM& sNPECollisionParam) = 0;

	/**
    *  RpWorld pointer를 얻어오는 interface 함수.
	*
    */
	virtual RpWorld* GetWorldPtr(void) = 0;

	virtual void SetDistanceFilter(RwBool bEnable) = 0;

    virtual RwBool GetDistanceFilterEnable() = 0;

	/**
	* world attribute
	* \return world의 attribute value를 리턴한다.
	* \see GetWorldNormalAttribute
	* \see GetWorldSpecialAttribute
	*/
	virtual DWORD GetWorldAttribute(RwV3d vPos) = 0;

	/**
	* world normal attribute
	* \return world의 normal attribute value를 리턴한다.
	* \see GetWorldSpecialAttribute
	*/
	virtual DWORD GetWorldNormalAttribute(RwV3d vPos) = 0;

	/**
	* world special attribute(일반속성 과 특수속성으로 나뉜다)
	* \return world의 special attribute value를 리턴한다.
	* \see GetWorldNormalAttribute
	*/
	virtual DWORD GetWorldSpecialAttribute(RwV3d vPos) = 0;

	/**
	* world material attribute
	* \return world의 material attribute value를 리턴한다.
	*/
	virtual BYTE GetWorldMaterialAttribute(RwV3d vPos) = 0;

	/**
	* 일반적으로 world의 normal attribute를 체크하여 물일 경우에만 사용한다.
	* \return world의 water 높이.
	*/
	virtual RwReal GetWorldWaterHeight(RwV3d vPos) = 0;



	/**
	* 현재 위치의 Bloom effect power factor를 구해온다.
	* \return if TRUE 이면 값이 유효하고, FALSE이면 유효하지 않다.
	*/
	virtual RwReal						GetActiveBloomFactor(void) = 0; 

	virtual VOID						SetActiveSky(CNtlPLSky* _pNtlPLSky) = 0;

	virtual	CNtlPLDojoContainer*		GetDojoContainer(void) = 0; 

	virtual CNtlPLSky*					GetSky(void) = 0;
	virtual CNtlPLFog*					GetFog(void) = 0;
	virtual CNtlPLPlant*				GetPlant(void) = 0;
	virtual CNtlPLWorldEntity*			GetWorld(void) = 0;

	// 실제 월드 좌표에 이동할수 있는지 없는지를 체크해준다.
	virtual RwBool	GetAvailablePos(RwV3d& _CurPos) = 0;

	// 게임에서 실시간으로 배경부분랜더링에 PVS 사용 유무를 결정하고 싶을때
	virtual VOID	SetWorldPVSActivation(RwBool _Flag) = 0;

	// 실제 월드 좌표의 지형과 수직인 회전 매트릭트를 얻는다.
	virtual RwBool	GetWorldTerrainRotate(RwMatrix* pmatOut, RwV3d* pvPos, RwBool bPosHeightChange, RwReal fLineLen = 1000.0f) = 0;

    // 사운드를 듣는 리스너 객체를 설정한다. (NULL로 설정하면 카메라 위치 중심으로 사운드가 들린다)
    virtual VOID	SetSoundListener(CNtlPLEntity* pEntity) = 0;

	// 지정된 위치의 그림자 색상을 얻고 싶을 때 사용
	virtual RwBool	GetWorldShadowColor(RwV3d& vPos, RwRGBA* pOutColor) = 0;

	// 지정된 위치의 연산된 월드색상을 알고 싶을 때 사용
	virtual RwBool	GetWorldLightColor(RwV3d& vPos, RwRGBA* pOutColor, RwBool bCalcShadow) = 0;

    virtual void	SetEntityVisible(ENtlPLEntityType eLayer, RwBool bVisible) = 0;             ///< 렌더레이어의 렌더링을 On/Off 한다. (테스트용)

	// PLENTITY_SUN Entity Visible을 변경한다.
	virtual void	SetVisiblePlanet(RwBool bVisible) = 0;

	virtual void	RunableRainBloomLight(RwReal fBloomShaderFadeInTime, RwReal fBloomShaderFadeOutTime) = 0;

	virtual void	RunableRainSkyLight(RwReal fSkyFadeInTime, RwReal fSkyFadeOutTime) = 0;
};

/**
*  scene manager의 instace 함수.
*
*/
inline CNtlPLSceneManager* GetSceneManager(void)
{
	return CNtlPLSceneManagerFactory::GetInstance(); 
}

#endif