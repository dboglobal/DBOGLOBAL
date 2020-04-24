//***********************************************************************************
//	File		:	VenusAPI.cpp
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#ifndef _VENUS_API_
#define _VENUS_API_

#include "rwcore.h"
#include "rpworld.h"
#include "libgfl.h"

void VENUS_TRACE(const RwChar* fmt, ...);

void GetDIBFromBitmap(GFL_BITMAP *bitmap, BITMAPINFOHEADER *bitmap_info, unsigned char **data);

RwBool	API_IntersectionPolygonFromRay(const RwV3d* pRayPos, const RwV3d* pRayDir, const RwV3d* v0, const RwV3d* v1, const RwV3d* v2, FLOAT* pfSize);
RwReal	API_DistancePointFromRay(const RwV3d* pRayPos, const RwV3d* pRayDir, RwV3d* pPoint, RwReal* pfParam = NULL);
RwBool	API_IntersectionSphereFromRay(const RwV3d* pRayPos, const RwV3d* pRayDir, RwV3d* pPoint, RwReal fSphrerRadius, RwReal* pfParam = NULL);

void	API_GetLocalRay(RwV3d* pLocalRayPos, RwV3d* pLocalRayDir, const RwMatrix* pLocalMatrix, const RwV3d* pRayPos, const RwV3d* pRayDir);
RwBool	API_IntersectionPlane(const RwV3d* pRayPos, const RwV3d* pRayDir, RwV3d* p0, RwV3d* p1, RwV3d* p2, RwV3d* p3, RwReal& fDistance, RwBool b2Side = FALSE);

void	API_CalculationShpere(RwSphere& sphere, RpClump* pClump);


RwBool  API_GetAtUpVecApplyAngle(RwV3d* outLookAtVec, RwV3d* outLookUpVec, const RwV3d vAngle); ///< 주어진 각도로 LookAt,LookUp Vector를 구한다.
RwV3d   API_GetAngle(RwV3d vUpVec, RwV3d vAtVec);                    ///< 주어진 vector로 Angle을 구한다.   

#endif