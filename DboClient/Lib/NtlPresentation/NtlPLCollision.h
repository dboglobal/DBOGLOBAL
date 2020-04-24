#pragma once


#include <vector>
using std::vector;


class CNtlPLEntity;


#pragma pack(1)


struct sCOLLISION_PARAM
{
public:

	sCOLLISION_PARAM()
	{
		::ZeroMemory(&_Intersection, sizeof(RpIntersection));
		_pNtlPLEntity	= NULL;
		_pAtomic		= NULL;
		_Dist			= RwRealMAXVAL;
	}

public:

	RwV3d			_Intersection;
	RwV3d			_Normal;
	RwReal			_Dist;
	RpAtomic*		_pAtomic;
	CNtlPLEntity*	_pNtlPLEntity;
};

// Ntl PL Entity Collision Param
struct sNPE_COLLISION_PARAM
{
public:

	sNPE_COLLISION_PARAM()
	{
		_pAtomicCur		= NULL;
		_pNtlPLEntity	= NULL;

		_vecNPECollisionParam.reserve(2);
	}

	~sNPE_COLLISION_PARAM()
	{
		_vecNPECollisionParam.clear();
	}

private:

	RpAtomic*	_pAtomicCur;
	RpAtomic*	_pAtomicCmp;

public:

	CNtlPLEntity*				_pNtlPLEntity;
	vector<sCOLLISION_PARAM>	_vecNPECollisionParam;

public:
	void SetCurAtomic(RpAtomic*	pAtomic) { _pAtomicCur = pAtomic; }
	void SetCmpAtomic(RpAtomic*	pAtomic) { _pAtomicCmp = pAtomic; }
	void SetCollisionParam_CTChar2Poly(RpIntersection *pIntersection, RpCollisionTriangle *pRpCollisionTriangle, RwReal fDistance);
};


#pragma pack()


class CNtlPLCollision
{
public:
	CNtlPLCollision(void);
	virtual ~CNtlPLCollision(void);

public:
	static RpAtomic* WorldIntersection4AtomicCB_CTChar2Poly(RpIntersection* pIntersection, RpWorldSector* pWorldSector, RpAtomic* pAtomic, RwReal fDistance, void* pData);
	static RpAtomic* WorldIntersection4AtomicCB_CTLine2Sphere(RpIntersection* pIntersection, RpWorldSector* pWorldSector, RpAtomic* pAtomic, RwReal fDistance, void* pData);
	static RpCollisionTriangle* WorldIntersection4AtomicTriCB_CTChar2Poly(RpIntersection *pIntersection, RpCollisionTriangle *pRpCollisionTriangle, RwReal fDistance, void* pData);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Renderware intersection & Callback
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RwBool NtlTestLineSphere(RwV3d* pvLineStart, RwV3d* pvLineEnd, RwSphere* pSphere);
RwBool NtlTestSphereShpere(RwSphere* pSphereA, RwSphere* pSphereB);

