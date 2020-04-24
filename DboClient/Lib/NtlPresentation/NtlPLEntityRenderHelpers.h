#ifndef __NTL_PL_ENTITY_HELPERS_H__
#define __NTL_PL_ENTITY_HELPERS_H__

#include "rptoon.h"
#include "rphanim.h"

///< Bone Data 용
typedef	std::map<std::string, RwFrame *>			FRAME_MAP;
typedef	std::map<std::string, RwFrame *>::iterator	FRAME_MAP_ITER;

#define DEFAULT_INK_THICKNESS		1.7f
struct SToonData
{
	RpToonInk		*pToonInk;
	RpToonPaint		*pToonPaint;
	RwTexture		*pTexture;

	SToonData() : pToonInk(NULL), pToonPaint(NULL), pTexture(NULL) {}
};

struct SEntityAtomicData
{
	RpAtomic	*pData;
	RwBool		bClone;
	RwFrame		*pFrame;
	
	SEntityAtomicData() : pData(NULL), bClone(FALSE), pFrame(NULL) { }
};

typedef	std::vector< SEntityAtomicData *>				ENTITY_ATOMIC_VEC;
typedef	std::vector< SEntityAtomicData *>::iterator		ENTITY_ATOMIC_VEC_ITER;

struct SEntityMaterialData
{
	RpMaterial	*pMaterial;

	SEntityMaterialData() : pMaterial(NULL) { }
};

void Helper_SetToonClump(RpClump *pClump, SToonData *pToonData);					///< Toon Data 설정한다.
void Helper_SetToonAtomic(RpAtomic *pAtomic, SToonData *pToonData);					///< Toon Data 설정한다.
void Helper_SetToonAtomicRender(RpAtomic *pAtomic, SToonData *pToonData);			///< Rendering을 할때Toon Data(Ink, Paint)를 설정한다.
RwBool Helper_IsToonClump(RpClump* pClump);                                         ///< 툰이 적용된 클럼프인지 확인한다.

RpAtomic* Helper_GetEnvMapAtomic(RpClump* pClump);                                  ///< 환경맵이 적용된 Atomic을 반환한다.


void Helper_RemoveClumpAllAtomics(RpClump *pClump);									///< Clump의 모든 Atomic들을 Remove 한다.
void Helper_SetAtomicAllMaterialSkinInfo(RpAtomic *pAtomic);						///< Skin Material이 존재 하는 경우 Material에 Skin 정보


void Helper_SetClumpAllAtomics( RpClump *pClump, 
							    ENTITY_ATOMIC_VEC *pVecAtomicList, 
								bool bClone = false);								///< Atomic List를 얻어온다.
void Helper_DestroyAllAtomics( ENTITY_ATOMIC_VEC *pVecAtomicList);					///< Atomic List를 Destory를 한다.

void Helper_SetHierarchyClumpSkin(RpClump *pClump, RpHAnimHierarchy *pHierarchy);	///< Clump의 모든 Atomic의 Skin에 Hierarchy를 넣어준다.
RpHAnimHierarchy *Helper_GetHierarchyClump(RpClump *pClump);						///< Clump의 Hierarchy 얻는다.

RwChar *Helper_BoneName(RwFrame *pFrame);											///< Bone의 이름을 Return을 한다.
RwChar *Helper_AtomicName(RpAtomic *pAtomic);										///< Atomic의 이름을 Return을 한다.
RwChar *Helper_MaterialName(RpMaterial *pMaterial);									///< Material의 이름을 Return을 한다.

RwBool	Helper_GetBoneList(RpClump *pClump, FRAME_MAP *pFrame);						///< Bone List를 얻어 온다.
RwBool	Helper_AtomicTestAlpha(RpAtomic *pAtomic);									///< Atomic에 Alpha가 있는지 알아내는 함수
RwBool	Helper_IsArmorType(RpClump *pClump);


void Helper_ClumpCalcBoundingSphere(RpClump *pClump, RwSphere *pSphere);			///< Clump의 Bounding Sphere를 구한다.(Object Space)
void Helper_ClumpBoundingSphereRender(RpClump *pClump, RwSphere *pSphere);
void Helper_ClumpWorldBoundingSphereRender(RwSphere *pSphere, RwUInt8 byR = 196, RwUInt8 byG = 196, RwUInt8 byB = 0, RwUInt8 byA = 255 );

void Helper_SetClumpAllAtomicsEMUV(RpClump *pClump, RwTexture* _pTexMap, RwMatrix* _pTexMat, RwBool _EMUVWorking);
void Helper_ResetClumpAllAtomicsEMUV(RpClump *pClump);

void Helper_SetClumpAllAtomicsSimpleMaterial(RpClump* pClump, RwBool _SimpleMaterial);

#endif