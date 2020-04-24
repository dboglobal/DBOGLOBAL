/*****************************************************************************
*
* File			: NtlPLPinking.h
* Author		: All Author
* Copyright	: (주)NTL
* Date			: 2005. 8. 11	
* Abstract		: Presentation picking.
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_PLPICKING_H__
#define __NTL_PLPICKING_H__

#include <rwcore.h>
#include <rpworld.h>
#include <rtpick.h>
#include <rpcollis.h>
#include "NtlPLEntity.h"

// picking
typedef struct _SWorldIntersect
{
	RpIntersection Intersection;
	RwReal fMinDist;
	RpAtomic *pAtomic;
	RwV3d vCollPos;
	RwV3d vNormal;
	RwBool bCollision;
}SWorldIntersect;

typedef struct _SWorldPickInfo
{
	RwBool bTerrainExclusion;		// 지형은 제외할 것인가?
	RwBool bCharacterExclusion;		// character은 제외할 것인가?
	RwReal fMinDist;				// 계산에 필요한 참조 변수
	RpAtomic *pAtomic;				// 계산에 필요한 참조 변수
	CNtlPLEntity *pPLEntity;		// picking에 성공하면 NULL이 아니고, 실패하면 NULL
	RwV3d vPickPos;

	_SWorldPickInfo()
	{
		bTerrainExclusion	= FALSE;
		bCharacterExclusion = FALSE;
		pAtomic				= NULL;
		pPLEntity			= NULL;
	}

}SWorldPickInfo;

// character
#define NTL_MAX_CHAR_INTERSECION	10

typedef struct _SWorldCharIntersectionData
{
	RpAtomic *pAtomic;
	RwBool bCollision;
	RwReal fMinDist;
	RwV3d vCollPos;
	RwV3d vNormal;
}SWorldCharIntersectionData;

typedef struct _SWorldCharIntersectionInfo
{
	RwUInt8 byAtomicCount;							// character가 충돌하는 object atomic 갯수.
	RpAtomic *pAtomic[NTL_MAX_CHAR_INTERSECION];	// atomic buffer.
}SWorldCharIntersectionInfo;

// camera
#define NTL_MAX_CAMERA_INTERSECION	20

typedef struct _SWorldCameraRayAtomicData
{
	RwUInt8 byCollCount;
	RpAtomic *pAtomic[NTL_MAX_CAMERA_INTERSECION];
	RwBool bCollision[NTL_MAX_CAMERA_INTERSECION];
	RwReal fMinDist[NTL_MAX_CAMERA_INTERSECION];
	RwV3d vCollPos[NTL_MAX_CAMERA_INTERSECION];
}SWorldCameraLineAtomicData;

typedef struct _SWorldCameraIntersectionData
{
	RwBool bCollision;
	SWorldCameraLineAtomicData sLineAtomicData;
	RwBool bSphereCollision;
	RwReal fBackwardDist;
	RwBool bBackwardCollision;
}SWorldCameraIntersectionData;

/**
* 충돌 가능 Atomic인지 검사한다
**/
RwBool IsCollisionAtomic(RpAtomic* pAtomic);

/**
* mouse picking을 한다.
*/
void Pick_WorldIntersectionLine(RwLine& Line, SWorldPickInfo& sPickInfo, RwReal fRayDist);

/**
* 충돌되는 height field 높이를 구하는 collision logic에 optimize 되어 있다.
*/
RwBool Collision_HeightFieldIntersectionLineTopDown(RwLine& Line, SWorldIntersect& sInter);

/**
* 충돌되는 Indoor 높이를 구하는 collision logic에 optimize 되어 있다.
*/
RwBool Collision_IndoorIntersectionLineTopDown(RwLine& Line, SWorldIntersect& sInter);

/**
* 충돌되는 object 높이를 구하는 collision logic에 optimize 되어 있다.
*/
RwBool Collision_WorldIntersectionLineTopDown(RwLine& Line, SWorldIntersect& sInter);

/**
* 충돌되는 mini indoor object 높이를 구하는 collision logic에 optimize 되어 있다.
*/
RwBool Collision_MiniIndoorIntersectionLineTopDown(RwLine& Line, SWorldIntersect& sInter);

/**
* world object중에 충돌되는 atomic을 구한다.
*/
RwBool Collision_WorldIntersectionCharacter(RwBBox& box, SWorldCharIntersectionInfo& sCharInfo);

/**
* world 중에 충돌되는 RpWorldSector(Indoor) Triangle을 구한다.
*/
RwBool Collision_WorldIntersectionWorldSector(RwLine& Line, SWorldCharIntersectionData& sCharDataInfo);

/**
* world object중에 충돌되는 atomic을 구한다.
*/
RwBool Collision_WorldIntersectionCharacterData(RwLine& Line, RpAtomic *pAtomic, SWorldCharIntersectionData& sCharDataInfo);

/**
* world상의 공간에서 camera 충돌 정보를 구한다.
*/
RwBool Collision_WorldIntersectionCameraData(const RwV3d *pCameraPos, const RwV3d *pLookAt, RwReal fRadius, RwV3d& vNewPos);

/**
* 충돌되는 Object가 있는지 여부를 검사한다. (LensFlare용. by agebreak)
*/
RwBool Collision_WorldIntersectionObject(const RwLine& line, SWorldPickInfo* pPickInfo );

/**
* 충돌되는 Object가 있는지 여부를 검사한다. (Targeting용)
*/
RwBool Collision_WorldIntersectionObjectForTarget(const RwLine& line, SWorldPickInfo* pPickInfo );

#endif