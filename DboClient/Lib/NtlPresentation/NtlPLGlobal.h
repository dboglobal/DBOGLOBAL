/*****************************************************************************
 *
 * File			: NtlPLGlobal.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 8. 11	
 * Abstract		: Presentation layer global variable class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_PLGLOBAL_H__
#define __NTL_PLGLOBAL_H__

#include <rwcore.h>
#include <rpworld.h>

#include <string>

using std::string;

extern RwReal g_fCharScheduleTime;

/**
 * \ingroup NtlPresentation
 * global data 자료구조.
 * renderware RpWorld와 RwCamera는 많은 곳으로 사용하므로, 전역으로 참조할 수 있게 만들었다.
 *
 */

class CNtlPLEntity;
class CNtlPLGlobal
{
public:

	static RpWorld*					m_pRpWorld;						/* 현재 active 된 RpWorld pointer **/
	static RwCamera*				m_RwCamera;						/* 현재 active 된 RwCamera pointer **/

	// 인도어
	static RpWorldSector*			m_RpWorldSector;				/* 현재 active 된 RpWorldSector pointer **/

	static RwV3d					m_vZeroV3;						/* 초기화 된 vector 변수 **/
	static RwMatrix					m_matIden;						/* 초기화 된 matrix 변수 **/
	static RwV3d					m_vIdenV3;
	static RwV3d					m_vXAxisV3;
	static RwV3d					m_vYAxisV3;
	static RwV3d					m_vZAxisV3;
	static RwMatrix					m_matNameBox;					/* name box matrix 변수 */

	static std::string				m_strAppPath;
	static RwBool					m_bCollObjVisible;				/* collision object rendering on/off flag */
	static RwReal					m_fDistFiterFrequency;			/* visual manager distance filtering update frequency */
	static RwReal					m_fDistFiterObjFrequency;		/* object distance fade in/out filtering update frequency */
	static RwBool					m_bWorldTerrainVisible;			/* world terrain visible on/off */
	static RwBool					m_bWorldSkyVisible;				/* world Sky visible on/off */
	static RwBool					m_bDecalRender;					// Decal Render on/Off	 - Decal Test 용
    static RwBool					m_bCharacterUpdateVisible;		///< 캐릭터가 Visible인 경우에만 Udpate한다는 Flag (맵툴에서 사용)
	static RwReal					m_fLensFlareForDNRatio;			///< D&N이 작용할때 Lens Flare가 얼마만큼의 영향을 받을지의 배율
	
	
	// Cz : 08.1.31
	static CNtlPLEntity*			m_pWHEntity;					// world height 가 object 위에 있는가? // Bool 형 대신 Entity로 변경.
	
	
	static RwBool					m_UseTerrainPVSMode;			// terrain의 PVS data 사용 유무
	static RwUInt32					m_uiRenderFlag;
	static RwBool					m_IsMinimapRendering;
	static RwUInt32					m_uiMinimapLayer;
	static std::vector<std::string> m_vecPathMinimapObject;			// 미니맵 출력 시 제거되어야 할 Object List. 임시로 사용한다.

    static RwBool					m_bObjectRender;				///< Object Render 유무 플래그

	static D3DMATRIX				m_mD3D9IdentityMatrix;
    static RwBool					m_bRenderScouter;				///< 스카우터 렌더링 유무 플래그
    static RwBool					m_bEntityNotCulling;            ///< Entity 컬링 안할때 사용하는 플래그

	static RwReal					m_fIndoorPlaneHeight;			///< Indoor에서 사용하는 지형 평면 높이.

	static RwUInt16					m_uiRenderFrame;				///< Culling에서 사용 되는 RenderFrame

	static RwUInt32					m_uiCullTestCnt;
	static RwUInt32					m_uiCullTestAtomicCnt;
	static RwUInt32					m_uiRenderAtomicCnt;

	// Test PVS Multi Layer
	static RwInt32					m_iCurrentPVSLayer;

   
public:

	/**
	*  Initialize 함수.
	*  \see Terminate
	*/
	static void Init(void);

	/**
	*  Terminate 함수.
	*  \see Init
	*/
	static void Terminate(void);

	/**
	*  RpWorld를 setting하는 함수.
	*  \param pRpWorld 
	*/
	static void SetActiveWorld(const RpWorld *pRpWorld);

	/**
	*  RwCamera를 setting하는 함수.
	*  \param pRwCamera
	*/
	static void SetActiveCamera(const RwCamera *pRwCamera);

	/**
	*  SABER에서 Minimap 출력 시 출력하지 말아야 할 Object List를 Load 한다.
	*  이 함수 및 m_strPathMiniMapObject 변수는 추후 Minimap 기획에 따라 삭제 될 수 있다.
	*/
	static void	LoadMinimapPassObjectList(const RwChar* pFilename);
};


#endif