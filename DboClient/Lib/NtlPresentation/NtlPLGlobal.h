/*****************************************************************************
 *
 * File			: NtlPLGlobal.h
 * Author		: HyungSuk, Jang
 * Copyright	: (��)NTL
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
 * global data �ڷᱸ��.
 * renderware RpWorld�� RwCamera�� ���� ������ ����ϹǷ�, �������� ������ �� �ְ� �������.
 *
 */

class CNtlPLEntity;
class CNtlPLGlobal
{
public:

	static RpWorld*					m_pRpWorld;						/* ���� active �� RpWorld pointer **/
	static RwCamera*				m_RwCamera;						/* ���� active �� RwCamera pointer **/

	// �ε���
	static RpWorldSector*			m_RpWorldSector;				/* ���� active �� RpWorldSector pointer **/

	static RwV3d					m_vZeroV3;						/* �ʱ�ȭ �� vector ���� **/
	static RwMatrix					m_matIden;						/* �ʱ�ȭ �� matrix ���� **/
	static RwV3d					m_vIdenV3;
	static RwV3d					m_vXAxisV3;
	static RwV3d					m_vYAxisV3;
	static RwV3d					m_vZAxisV3;
	static RwMatrix					m_matNameBox;					/* name box matrix ���� */

	static std::string				m_strAppPath;
	static RwBool					m_bCollObjVisible;				/* collision object rendering on/off flag */
	static RwReal					m_fDistFiterFrequency;			/* visual manager distance filtering update frequency */
	static RwReal					m_fDistFiterObjFrequency;		/* object distance fade in/out filtering update frequency */
	static RwBool					m_bWorldTerrainVisible;			/* world terrain visible on/off */
	static RwBool					m_bWorldSkyVisible;				/* world Sky visible on/off */
	static RwBool					m_bDecalRender;					// Decal Render on/Off	 - Decal Test ��
    static RwBool					m_bCharacterUpdateVisible;		///< ĳ���Ͱ� Visible�� ��쿡�� Udpate�Ѵٴ� Flag (�������� ���)
	static RwReal					m_fLensFlareForDNRatio;			///< D&N�� �ۿ��Ҷ� Lens Flare�� �󸶸�ŭ�� ������ �������� ����
	
	
	// Cz : 08.1.31
	static CNtlPLEntity*			m_pWHEntity;					// world height �� object ���� �ִ°�? // Bool �� ��� Entity�� ����.
	
	
	static RwBool					m_UseTerrainPVSMode;			// terrain�� PVS data ��� ����
	static RwUInt32					m_uiRenderFlag;
	static RwBool					m_IsMinimapRendering;
	static RwUInt32					m_uiMinimapLayer;
	static std::vector<std::string> m_vecPathMinimapObject;			// �̴ϸ� ��� �� ���ŵǾ�� �� Object List. �ӽ÷� ����Ѵ�.

    static RwBool					m_bObjectRender;				///< Object Render ���� �÷���

	static D3DMATRIX				m_mD3D9IdentityMatrix;
    static RwBool					m_bRenderScouter;				///< ��ī���� ������ ���� �÷���
    static RwBool					m_bEntityNotCulling;            ///< Entity �ø� ���Ҷ� ����ϴ� �÷���

	static RwReal					m_fIndoorPlaneHeight;			///< Indoor���� ����ϴ� ���� ��� ����.

	static RwUInt16					m_uiRenderFrame;				///< Culling���� ��� �Ǵ� RenderFrame

	static RwUInt32					m_uiCullTestCnt;
	static RwUInt32					m_uiCullTestAtomicCnt;
	static RwUInt32					m_uiRenderAtomicCnt;

	// Test PVS Multi Layer
	static RwInt32					m_iCurrentPVSLayer;

	static RwBool					m_bItemDropDisplay;


   
public:

	/**
	*  Initialize �Լ�.
	*  \see Terminate
	*/
	static void Init(void);

	/**
	*  Terminate �Լ�.
	*  \see Init
	*/
	static void Terminate(void);

	/**
	*  RpWorld�� setting�ϴ� �Լ�.
	*  \param pRpWorld 
	*/
	static void SetActiveWorld(const RpWorld *pRpWorld);

	/**
	*  RwCamera�� setting�ϴ� �Լ�.
	*  \param pRwCamera
	*/
	static void SetActiveCamera(const RwCamera *pRwCamera);

	/**
	*  SABER���� Minimap ��� �� ������� ���ƾ� �� Object List�� Load �Ѵ�.
	*  �� �Լ� �� m_strPathMiniMapObject ������ ���� Minimap ��ȹ�� ���� ���� �� �� �ִ�.
	*/
	static void	LoadMinimapPassObjectList(const RwChar* pFilename);
};


#endif