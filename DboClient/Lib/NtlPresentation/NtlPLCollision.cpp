#include "precomp_ntlpresentation.h"
#include "ntlworldcommon.h"
#include "NtlPLCollision.h"
#include "NtlPLObject.h"
#include "NtlMath.h"


void sNPE_COLLISION_PARAM::SetCollisionParam_CTChar2Poly(RpIntersection *pIntersection, RpCollisionTriangle *pRpCollisionTriangle, RwReal fDistance)
{
	if(!_pAtomicCur)
		return;

	if(_pAtomicCur)
	{
		for(unsigned int i = 0; i < _vecNPECollisionParam.size(); ++i)
		{
			if(_pAtomicCur == _vecNPECollisionParam[i]._pAtomic)
			{
				if(static_cast<RwReal>(::fabs(_vecNPECollisionParam[i]._Dist)) > static_cast<RwReal>(::fabs(fDistance)))
				{
					_vecNPECollisionParam[i]._Dist	= fDistance * pIntersection->t.sphere.radius;
					RwV3dTransformPoints(&_vecNPECollisionParam[i]._Intersection, &pRpCollisionTriangle->point, 1, RwFrameGetLTM(RpAtomicGetFrame(_pAtomicCmp)));
					RwV3dTransformPoints(&_vecNPECollisionParam[i]._Normal, &pRpCollisionTriangle->normal, 1, RwFrameGetLTM(RpAtomicGetFrame(_pAtomicCmp)));
					RwV3dNormalize(&_vecNPECollisionParam[i]._Normal, &_vecNPECollisionParam[i]._Normal);
				}

				return;
			}
		}
	}

	sCOLLISION_PARAM sCollisionParam;
	sCollisionParam._pAtomic		= _pAtomicCur;
	sCollisionParam._pNtlPLEntity	= static_cast<CNtlPLEntity*>(RpNtlAtomicGetData(_pAtomicCur));
	sCollisionParam._Dist			= fDistance * pIntersection->t.sphere.radius;
	RwV3dTransformPoints(&sCollisionParam._Intersection, &pRpCollisionTriangle->point, 1, RwFrameGetLTM(RpAtomicGetFrame(_pAtomicCmp)));
	RwV3dTransformPoints(&sCollisionParam._Normal, &pRpCollisionTriangle->normal, 1, RwFrameGetLTM(RpAtomicGetFrame(_pAtomicCmp)));
	RwV3dNormalize(&sCollisionParam._Normal, &sCollisionParam._Normal);
	_vecNPECollisionParam.push_back(sCollisionParam);
}


CNtlPLCollision::CNtlPLCollision(void)
{
}

CNtlPLCollision::~CNtlPLCollision(void)
{
}

RpCollisionTriangle* CNtlPLCollision::WorldIntersection4AtomicTriCB_CTChar2Poly(RpIntersection *pIntersection, RpCollisionTriangle *pRpCollisionTriangle, RwReal fDistance, void* pData)
{
	static_cast<sNPE_COLLISION_PARAM*>(pData)->SetCollisionParam_CTChar2Poly(pIntersection, pRpCollisionTriangle, fDistance);

	return pRpCollisionTriangle;
}

RpAtomic* CNtlPLCollision::WorldIntersection4AtomicCB_CTChar2Poly(RpIntersection* pIntersection, RpWorldSector* pWorldSector, RpAtomic* pAtomic, RwReal fDistance, void* pData)
{
	if((static_cast<CNtlPLEntity*>(RpNtlAtomicGetData(pAtomic))->GetClassType() != PLENTITY_OBJECT))
	{
		return pAtomic;
	}

	sNPE_COLLISION_PARAM*	pNPECollisionParam	= static_cast<sNPE_COLLISION_PARAM*>(pData);
	CNtlPLObject*			pNtlPLObjectCur		= static_cast<CNtlPLObject*>(RpNtlAtomicGetData(pAtomic));

	// ignore the objects that don't need to test
	if(pNtlPLObjectCur->GetClassType() != PLENTITY_OBJECT)
	{
		return pAtomic;
	}

	// ignore self-checkup
	if(pNtlPLObjectCur == pNPECollisionParam->_pNtlPLEntity)
	{
		return pAtomic;
	}

	// loop clump
	CNtlPLObject*				pNtlPLObjectSrc	= static_cast<CNtlPLObject*>(pNPECollisionParam->_pNtlPLEntity);
	RpClump*					pClump			= pNtlPLObjectSrc->GetClump();
	RwLLLink*					pCur;
	RwLLLink*					pEnd;
	RwLLLink*					pNext;
	RpAtomic*					pCurAtomicInClump;
	RwSphere*					pSphere;

	pCur = rwLinkListGetFirstLLLink(&pClump->atomicList);
	pEnd = rwLinkListGetTerminator(&pClump->atomicList);

	while(pCur != pEnd)
	{
		pCurAtomicInClump	= rwLLLinkGetData(pCur, RpAtomic, inClumpLink);
		pSphere				= RpAtomicGetBoundingSphere(pCurAtomicInClump);

		// Collision flag should be applied partially.
		if(RpNtlAtomicGetFlag(pCurAtomicInClump) & NTL_COLLISION)
		{
			pIntersection->t.sphere.center.x	= pSphere->center.x;
			pIntersection->t.sphere.center.y	= pSphere->center.y;
			pIntersection->t.sphere.center.z	= pSphere->center.z;
			pIntersection->t.sphere.radius		= pSphere->radius;
			RwV3dTransformPoints(&pIntersection->t.sphere.center, &pIntersection->t.sphere.center, 1, RwFrameGetLTM(RpAtomicGetFrame(pCurAtomicInClump)));

			pNPECollisionParam->SetCurAtomic(pCurAtomicInClump);
			pNPECollisionParam->SetCmpAtomic(pAtomic);
			RpAtomicForAllIntersections(pAtomic, pIntersection, CNtlPLCollision::WorldIntersection4AtomicTriCB_CTChar2Poly, pNPECollisionParam);
		}
		
		pNext	= rwLLLinkGetNext(pCur);
		pCur	= pNext;
	}

	return pAtomic;
}

RpAtomic* CNtlPLCollision::WorldIntersection4AtomicCB_CTLine2Sphere(RpIntersection* pIntersection, RpWorldSector* pWorldSector, RpAtomic* pAtomic, RwReal fDistance, void* pData)
{
	if((static_cast<CNtlPLEntity*>(RpNtlAtomicGetData(pAtomic))->GetClassType() != PLENTITY_OBJECT))
	{
		return pAtomic;
	}

	sNPE_COLLISION_PARAM*	pNPECollisionParam	= static_cast<sNPE_COLLISION_PARAM*>(pData);
	CNtlPLObject*			pNtlPLObjectCur		= static_cast<CNtlPLObject*>(RpNtlAtomicGetData(pAtomic));
	RwReal					DistSphere2LineSPos;

	// ignore the objects that don't need to test
	if(pNtlPLObjectCur->GetClassType() != PLENTITY_OBJECT)
	{
		return pAtomic;
	}

	RwSphere*	pSphere = RpAtomicGetBoundingSphere(pAtomic);
	RwV3d		SphereC;
	RwV3d		Src2Dst;
	RwV3dTransformPoints(&SphereC, &pSphere->center, 1, RwFrameGetLTM(RpAtomicGetFrame(pAtomic)));

	for(unsigned int i = 0; i < pNPECollisionParam->_vecNPECollisionParam.size(); ++i)
	{
		if(pAtomic == pNPECollisionParam->_vecNPECollisionParam[i]._pAtomic)
		{
			RwV3dSubMacro(&Src2Dst, &SphereC, &pIntersection->t.sphere.center);
			DistSphere2LineSPos = ::sqrtf(CNtlMath::MathRwV3dSquaredLength(&Src2Dst));

			if(static_cast<RwReal>(::fabs(pNPECollisionParam->_vecNPECollisionParam[i]._Dist)) > DistSphere2LineSPos)
			{
				pNPECollisionParam->_vecNPECollisionParam[i]._Dist = fDistance * DistSphere2LineSPos;
			}

			return pAtomic;
		}
	}

	sCOLLISION_PARAM sCollisionParam;
	RwV3dSubMacro(&Src2Dst, &SphereC, &pIntersection->t.sphere.center);
	DistSphere2LineSPos = ::sqrtf(CNtlMath::MathRwV3dSquaredLength(&Src2Dst));
	sCollisionParam._Dist			= fDistance * DistSphere2LineSPos;
	sCollisionParam._pAtomic		= pAtomic;
	sCollisionParam._pNtlPLEntity	= static_cast<CNtlPLEntity*>(pNtlPLObjectCur);
	pNPECollisionParam->_vecNPECollisionParam.push_back(sCollisionParam);

	return pAtomic;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RwBool NtlTestLineSphere(RwV3d* pvLineStart, RwV3d* pvLineEnd, RwSphere* pSphere)
{
	RwBool bResult	= FALSE;

	RwV3d vStart	= *pvLineStart;
	RwV3d vDelta	= *pvLineEnd - *pvLineStart;
	RwV3d vCenter	= pSphere->center - vStart;

	RwReal fDeltaSq	= RwV3dDotProduct(&vDelta, &vDelta);
	RwReal fCenterSq= RwV3dDotProduct(&vCenter, &vDelta);

	RwV3d	vTemp;
	RwReal	fPerpSq;
	RwV3dScale(&vTemp, &vCenter, fDeltaSq);
	RwV3dIncrementScaled(&vTemp, &vDelta, -fCenterSq);
	fPerpSq = RwV3dDotProduct(&vTemp, &vCenter);

	RwReal fOffsetSq;

	fOffsetSq = pSphere->radius * pSphere->radius * fDeltaSq - fPerpSq;

	if (fOffsetSq >= 0)
	{
		if (fCenterSq < 0)
		{
			bResult = ((fCenterSq * fCenterSq) <= fOffsetSq);
		}
		else if (fCenterSq > fDeltaSq)
		{
			RwReal fDiff = fCenterSq - fDeltaSq;
			bResult = ((fDiff * fDiff) <= fOffsetSq);
		}
		else
		{
			bResult = TRUE;
		}
	}
	else
	{
		bResult = FALSE;
	}

	return bResult;
}

RwBool NtlTestSphereShpere(RwSphere* pSphereA, RwSphere* pSphereB)
{
	RwV3d	vDelta		= pSphereA->center - pSphereB->center;
	RwReal	fDeltaSq	= RwV3dDotProduct(&vDelta, &vDelta);

	RwReal  fOffsetSq	= (pSphereA->radius + pSphereB->radius) * (pSphereA->radius + pSphereB->radius);

	if (fOffsetSq >= fDeltaSq)
	{
		return TRUE;
	}
	return FALSE;
}