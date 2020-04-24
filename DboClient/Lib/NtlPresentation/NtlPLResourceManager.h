/*****************************************************************************
 *
 * File			: NtlPLResourceManager.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 7. 21	
 * Abstract		: Presentation layer resource manager class
 *****************************************************************************
 * Desc         : 
 * Adjust		: HuunSuk, Jang(2005. 7 29)
 *			        - renderware texture reseouce 추가. 
 *				    - resorce에 dictory type 추가(texture를 group별로 관리하기 위한 interface) 
 *
 *****************************************************************************/

#ifndef __NTL_PLRESOURCE_MANAGER_H__
#define __NTL_PLRESOURCE_MANAGER_H__

#include <rwcore.h>
#include <rpworld.h>
#include <string>
#include <map>
#include "NtlPLResource.h"

//RwBool TextureLoadLevelPath(const RwChar* pInPath, RwChar** ppOutPath, RwInt32* pOutPathSize);

class CNtlPLEntity;
class CNtlResourceScheduleManager;

/**
* \ingroup NtlPresentation
* RenderWare resource manager class이다.
* world, clump, atomic, texture가 제공되고 있으며, uva file은 추후 제공 유무를 검토한다.
* Note : module은 아직 검증되지 않았으므로 사용자는 주의해야 한다.
*        또한 resource container를 clump 와 atomic을 따로 생성할 것인지 검토해야 한다.
* 
*/
class CNtlPLResourceManager
{
public:

	enum EResDictType	 
	{
		RES_TEX_DEFAULT_DICT,
		RES_TEX_TERRAIN_DICT,
		RES_TEX_OBJECT_DICT,
		RES_TEX_CHARACTER_DICT,
		RES_TEX_EFFECT_DICT,
		RES_TEX_UI_DICT,
	};

	// TEST
// public:
// 	void SetTextureLoadLevel(RwUInt32 eResDictType, RwUInt32 uiLevel);
// 
// 	struct STextureLoadLevelDirectory
// 	{
// 		std::string strDirectory;
// 		RwUInt32	uiLevelCur;
// 		RwUInt32	uiLevelMax;
// 	};
// 
// 	
// 	typedef std::map<RwUInt32, STextureLoadLevelDirectory>	TextureLoadDirectoryMap;
// 	TextureLoadDirectoryMap m_mapTextureLoadDirectroy;	
// 
// 	std::string m_strImagePath;
// 	RwInt32		m_iImagePathSize;
	//TEST : END

private:
	typedef std::map<std::string, CNtlPLResource*> ResourceMap;
	typedef std::multimap<std::string, CNtlPLResource*> ResourceMultiMap;

	ResourceMap m_mapResTbl; /**< CNtlPlResource의 container*/

	ResourceMap m_mapWorld;
	ResourceMap m_mapClump;
	ResourceMap m_mapAtomic;
	ResourceMultiMap m_mmapCloneClump; 
	ResourceMultiMap m_mmapCloneAtomic;
	ResourceMap m_mapAnim;
	
	RwBool	m_bLoadScheduling;
	RwBool	m_bDeleteScheduling;

	CNtlResourceScheduleManager *m_pScheduleManager;

private:
	
	/**
    *  RenderWare data를 읽는 함수.
    *
	*  \param pStrName file name
	*  \param pResourcePath resource path
	*  \param uiType RednerWare resource type
	*  \param bClone container에 file name과 같은 resource가 존재한다면? clone으로 생성할 것인가?
	*  \return resource class pointer
	*/
	CNtlPLResource* LoadDefault(const char *pStrName, const char * pResourcePath, unsigned int uiType);

	/**
    *  CNtlPLResource 객체를 생성하는 함수.
    *
    *  \param pName resource name을 CNtlPLResource에 담는다.
	*  \param uiType renderware data의 type.
	*  \param bClone renderware data의 clone flag.
	*  \param pData renderware data의 pointer.
	*  \return resource class pointer
    */
	CNtlPLResource* CreateResource(const char *pName, unsigned int uiType, bool bClone,	void *pData);

	/**
    *  CNtlPLResource 객체를 container에 등록하는 함수.
    *
    *  \param pResource container 등록할 resource pointer
	*  \see UnRegisterResource
    */
	void RegisterResource(const CNtlPLResource *pResource);

	/**
    *  CNtlPLResource 객체를 container에서 해제하는 함수.
    *
    *  \param pResource container 해제할 resource pointer
	*  \see RegisterResource
    */
	void UnRegisterResource(const CNtlPLResource *pResource);

	/**
    *  renderware data type에 해당하는 data를 생성하는 함수.
    *
    *  \param pSteam stream 객체 pointer.
	*  \param iType renderware data type.
    */
	void* CreateRwData(RwStream *pSteam, unsigned int iType);

	/**
    *  renderware data에 해당하는 clone data를 생성한다.
	*  type을 비교하여 적절한 renderware clone 함수를 적용한다.
    *
    *  \param iType renderware data type.
	*  \param pData clone할 renderware data.
	*  \return renderware data pointer
	*  \see DestroyRwData
    */
	void* CreateCloneRwData(unsigned int iType, void *pData); 

	/**
    *  renderware data에 해당하는 type에 적절하게 삭제한다.
    *
    *  \param iType renderware data type.
	*  \param pData 삭제할 renderware data.
    */
	void  DestroyRwData(unsigned int iType, void *pData);

	/**
    *  renderware world를 생성하는 함수.
    *
	*  \param pStream world stream data
	*  \return RenderWare world pointer
	*  \see AtomicStreamRead
	*  \see ClumpStreamRead
    */
	RpWorld* WorldStreamRead (RwStream *pStream);
	
	/**
    *  renderware clump를 생성하는 함수.
    *
	*  \param pStream clump stream data
	*  \return RenderWare clump pointer
	*  \see WorldStreamRead
	*  \see AtomicStreamRead
    */
	RpClump* ClumpStreamRead (RwStream *pStream);

	/**
    *  renderware atomic를 생성하는 함수.
    *
	*  \param pStream atomic stream data
	*  \return RenderWare atomic pointer
	*  \see WorldStreamRead
	*  \see ClumpStreamRead
    */
	RpAtomic* AtomicStreamRead (RwStream *pStream);

	/**
    *  container에서 name으로 CNtlPLResource를 찾는다.
    *
	*  \param pName name string
	*  \param uiType the resource type
	*  \param uiType the resource dictory type
	*  \return pName에 해당하는 resource class pointer
    */
	CNtlPLResource* Find(const char *pName, unsigned int uiType, EResDictType eDictType = RES_TEX_DEFAULT_DICT);

    /**
    *  application이 종료 되었을 때 해지되지 않은 resource를 debugging을 위한 함수.
	*  debugging 내용을 file or output window로 display 할 수 있도록 한다.
    *
	*/
	void DebugResource(void);

public:

	CNtlPLResourceManager();

	/**
    *  singleton CNtlPLResourceManager interface 함수.
    *
	*/
	static CNtlPLResourceManager* GetInstance(void);

	/**
    * Initial 함수.
	*  \return resource manager의 초기화가 성공했으면 true, 실패하면 false
    *
	*/
	bool Crate(void);

	/**
    *  Terminate 함수.
    *
	*/
	void Destroy(void);

	/**
    *  Update 함수.
    *
	*/
	void Update(RwReal fElapsed);

	/**
    *  RenderWare world data를 읽는 함수.
    *
	*  \param pStrName file name
	*  \param pResourcePath resource path
	*  \return Pointer resource class
	*  \see LoadClump	
	*  \see LoadAtomic
	*  \see LoadTexture
	*  \see UnLoad
	*/
	CNtlPLResource* LoadWorld(const char *pStrName, const char * pResourcePath);

	/**
    *  RenderWare clump data를 읽는 함수.
    *
	*  \param pStrName file name
	*  \param pResourcePath resource path
	*  \param bClone clump를 clone으로 생성할 것인지를 결정하는 변수.
	*  \return Pointer resource class
	*  \see LoadWorld
	*  \see LoadAtomic
	*  \see LoadTexture
	*  \see UnLoad
	*/
	CNtlPLResource* LoadClump(const char *pStrName, const char * pResourcePath);


	/**
    *  RenderWare clump data를 읽는 함수. 일정한 시간마다 scheduling 하면서 loading 한다(thread loading 대체용)
    *
	*  \param pStrName file name
	*  \param pResourcePath resource path
	*  \param bClone clump를 clone으로 생성할 것인지를 결정하는 변수.
	*  \return Pointer resource class
	*  \see LoadWorld
	*  \see LoadAtomic
	*  \see LoadTexture
	*  \see UnLoad
	*/
	void LoadSchedulingClump(const char *pStrName, const char * pResourcePath, CNtlPLEntity *pEntity);

	/**
    *  RenderWare atomic data를 읽는 함수.
    *
	*  \param pStrName file name
	*  \param pResourcePath resource path
	*  \param bClone atomic를 clone으로 생성할 것인지를 결정하는 변수.
	*  \return Pointer resource class
	*  \see LoadWorld
	*  \see LoadClump
	*  \see LoadTexture
	*  \see UnLoad
	*/
	CNtlPLResource* LoadAtomic(const char *pStrName, const char * pResourcePath);

	
	/**
    *  RenderWare animation data를 읽는 함수.
    *
	*  \param pStrName file name
	*  \return Pointer resource class
	*  \see LoadWorld
	*  \see LoadClump
	*  \see LoadTexture
	*  \see LoadAtomic
	*  \see UnLoad
	*/
	CNtlPLResource* LoadAnimation(const char *pStrName);

	/**
    *  RenderWare texture data를 읽는 함수.
    *
	*  \param pStrName file name
	*  \param pResourcePath resource path
	*  \param eFilterMode texture의 filtering mode. renderware RwTextureFilterMode를 참조.
	*  \param EResDictType texture를 group으로 나누어 관리할 경우에 group의 id가 된다.
	*         group으로 관리하는 이유는 검색 속도를 빠르게 할 수 있다.     
	*         일반적으로 object, character, terrain, texture, ui등의 group을 생각해 볼 수 있다.
	*  \return Pointer resource class
	*  \see UnLoadTexture
	*/
	RwTexture* LoadTexture(const char *pStrName, const char * pResourcePath, EResDictType eType = RES_TEX_DEFAULT_DICT, const char *pStrMaskName = NULL);

	/**
    *  RenderWare texture를 강제적으로 생성하는 함수.
    *
	*  \param pStrName file name.
	*  \param iWidth texture width.
	*  \param iHeight texture height.
	*  \param iDepth pixel 깊이.
	*  \param eFilterMode texture filter mode.
	*  \param eFormat raster format.
	*  \param eType texture dictionary type.(CNtlPLResourceManager 의 EResDictType 사용).
	*  \return Pointer renderware texture pointer
	*  \see LoadTexture
	*  \see UnLoadTexture
	*/

	RwTexture* CreateTexture(const char *pStrName, 
							RwInt32 iWidth, 
							RwInt32 iHeight,
							RwInt32 iDepth,
							RwRasterFormat eFormat, 
							EResDictType eType = RES_TEX_DEFAULT_DICT);

	/**
    *  resource를 unload 한다.
    *
	*  \param pResource unload할 resource pointer
	*  \see LoadWorld	
	*  \see LoadClump	
	*  \see LoadAtomic
	*  \see LoadTexture
	*/
	void UnLoad(CNtlPLResource *pResource);

	
	/**
    *  scheduling상에 등록되어 있는 entity에 해당하는 scheduling node를 삭제한다.
    *
	*/
	void UnLoadScheduling(CNtlPLEntity *pPLEntity, CNtlPLResource *pResource);

	/**
    *  texture를 unload 한다.
    *
	*  \param pResource unload할 resource pointer
	*  \see LoadTexture	
	*/
	void UnLoadTexture(RwTexture *pTexture);

	// woody1019
	//RwBool SetAlphaValue(RwTexture *pTexture, RwUInt8 Value, RwV2d Pos);
	RwBool	SetColor(RwTexture* _pTexture, DWORD _FillColor);
	RwBool	SetColor(RwTexture *pTexture, RwReal OpaLvl, POINT& TexelPos);
	RwBool	SetColor(RwTexture *pTexture, RwInt32 Value, RwV2d Pos);
	RwBool	SetAlphaValueInc(RwTexture *pTexture, RwInt32 Value, RwV2d Pos);
	RwBool	SetAlphaValueDec(RwTexture *pTexture, RwInt32 Value, RwV2d Pos);
	RwBool	SetAlphaBits(RwTexture *pTexture, RwUInt8 Value, RwV2d Pos);
	RwBool	SaveAlphaBits(RwTexture *pTexture, FILE *pFile);
	RwBool	SaveTexture(RwTexture *pTexture, FILE *pFile);
	RwBool	LoadTexture(RwTexture *pTexture, FILE *pFile);
	RwBool	LoadTexture(RwTexture *pTexture, BYTE* pBuff);
	RwBool	LoadAlphaBits(RwTexture *pTexture, RwUInt8 *pPreparedBits);
	RwBool	LoadAlphaBitsShadowMap(RwTexture *pTexture, FILE* pFile, RwRGBA* pShdowClr);
	RwBool	SaveAlphaBitsShadowMap(RwTexture *pTexture, FILE* pFile);
	RwBool	SaveTextureShadowMap(RwTexture *pTexture, FILE *pFile);
	RwBool	LoadTextureShadowMap(RwTexture *pTexture, FILE *pFile);

	RwBool	CopyRaster(RwTexture *pSrcTex, RwTexture *pDstTex);


	void	SetLoadScheduling(RwBool bSchedule); 
	RwBool	IsLoadScheduling();

	void	SetDeleteScheduling(RwBool bSchedule);
	RwBool	IsDeleteScheduling();

	RwBool	IsEmptyLoadScheduling(void);

	RwReal	GetAlphaAverageDensity(RwTexture* _pTex);
	RwReal	GetAlphaAverageDensity(BYTE* _pAlphaBits, RwInt32 _Cnt);

	RwBool	TextureLoadLevelPath(const RwChar* pInPath, RwChar** ppOutPath, RwInt32* pOutPathSize);
};


static CNtlPLResourceManager* GetNtlResourceManager(void)
{
	return CNtlPLResourceManager::GetInstance();
}

inline void	CNtlPLResourceManager::SetLoadScheduling(RwBool bSchedule)
{
	m_bLoadScheduling = bSchedule;
}

inline void	CNtlPLResourceManager::SetDeleteScheduling(RwBool bSchedule)
{
	m_bDeleteScheduling = bSchedule;
}

inline RwBool CNtlPLResourceManager::IsLoadScheduling()
{
	return m_bLoadScheduling;
}

#endif
