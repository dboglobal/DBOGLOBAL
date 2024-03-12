#ifndef __NTL_PL_ENTITY_HELPERS_H__
#define __NTL_PL_ENTITY_HELPERS_H__

#include "rptoon.h"
#include "rphanim.h"

///< Bone Data ��
typedef	std::map<std::string, RwFrame *>			FRAME_MAP;
typedef	std::map<std::string, RwFrame *>::iterator	FRAME_MAP_ITER;

#define DEFAULT_INK_THICKNESS		1.f
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

void Helper_SetToonClump(RpClump *pClump, SToonData *pToonData);					///< Toon Data �����Ѵ�.
void Helper_SetToonAtomic(RpAtomic *pAtomic, SToonData *pToonData);					///< Toon Data �����Ѵ�.
void Helper_SetToonAtomicRender(RpAtomic *pAtomic, SToonData *pToonData);			///< Rendering�� �Ҷ�Toon Data(Ink, Paint)�� �����Ѵ�.
RwBool Helper_IsToonClump(RpClump* pClump);                                         ///< ���� ����� Ŭ�������� Ȯ���Ѵ�.

RpAtomic* Helper_GetEnvMapAtomic(RpClump* pClump);                                  ///< ȯ����� ����� Atomic�� ��ȯ�Ѵ�.


void Helper_RemoveClumpAllAtomics(RpClump *pClump);									///< Clump�� ��� Atomic���� Remove �Ѵ�.
void Helper_SetAtomicAllMaterialSkinInfo(RpAtomic *pAtomic);						///< Skin Material�� ���� �ϴ� ��� Material�� Skin ����


void Helper_SetClumpAllAtomics( RpClump *pClump, 
							    ENTITY_ATOMIC_VEC *pVecAtomicList, 
								bool bClone = false);								///< Atomic List�� ���´�.
void Helper_DestroyAllAtomics( ENTITY_ATOMIC_VEC *pVecAtomicList);					///< Atomic List�� Destory�� �Ѵ�.

void Helper_SetHierarchyClumpSkin(RpClump *pClump, RpHAnimHierarchy *pHierarchy);	///< Clump�� ��� Atomic�� Skin�� Hierarchy�� �־��ش�.
RpHAnimHierarchy *Helper_GetHierarchyClump(RpClump *pClump);						///< Clump�� Hierarchy ��´�.

RwChar *Helper_BoneName(RwFrame *pFrame);											///< Bone�� �̸��� Return�� �Ѵ�.
RwChar *Helper_AtomicName(RpAtomic *pAtomic);										///< Atomic�� �̸��� Return�� �Ѵ�.
RwChar *Helper_MaterialName(RpMaterial *pMaterial);									///< Material�� �̸��� Return�� �Ѵ�.

RwBool	Helper_GetBoneList(RpClump *pClump, FRAME_MAP *pFrame);						///< Bone List�� ��� �´�.
RwBool	Helper_AtomicTestAlpha(RpAtomic *pAtomic);									///< Atomic�� Alpha�� �ִ��� �˾Ƴ��� �Լ�
RwBool	Helper_IsArmorType(RpClump *pClump);


void Helper_ClumpCalcBoundingSphere(RpClump *pClump, RwSphere *pSphere);			///< Clump�� Bounding Sphere�� ���Ѵ�.(Object Space)
void Helper_ClumpBoundingSphereRender(RpClump *pClump, RwSphere *pSphere);
void Helper_ClumpWorldBoundingSphereRender(RwSphere *pSphere, RwUInt8 byR = 196, RwUInt8 byG = 196, RwUInt8 byB = 0, RwUInt8 byA = 255 );

void Helper_SetClumpAllAtomicsEMUV(RpClump *pClump, RwTexture* _pTexMap, RwMatrix* _pTexMat, RwBool _EMUVWorking);
void Helper_ResetClumpAllAtomicsEMUV(RpClump *pClump);

void Helper_SetClumpAllAtomicsSimpleMaterial(RpClump* pClump, RwBool _SimpleMaterial);

#endif