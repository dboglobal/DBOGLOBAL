#include "rwcore.h"
#include "rpplugin.h"
#include "rpdbgerr.h"
#include "rpworld.h"
#include "rtintsec.h"

#include "ctdata.h"
#include "ctquery.h"
#include "colldata.h"
#include "collquery.h"

#include "NtlCollis.h"


//////////////////////////////////////////////////////////////////////////
//
//	Declaration
//
//////////////////////////////////////////////////////////////////////////


typedef struct
{
	RwLLLink *cur;
	RwLLLink *end;

} WorldSectorAtomicIt;


//////////////////////////////////////////////////////////////////////////
//
//	Macro
//
//////////////////////////////////////////////////////////////////////////


#define WorldSectorAtomicItInitMacro(_it, _sector)                          \
MACRO_START                                                                 \
{                                                                           \
	RwLinkList *list = &(_sector)->collAtomicsInWorldSector;                \
	(_it).end = rwLinkListGetTerminator(list);                              \
	(_it).cur = rwLinkListGetFirstLLLink(list);                             \
}                                                                           \
MACRO_STOP

#define WorldSectorAtomicItFinishedMacro(_it)  ((_it)->cur == (_it)->end)

#define WorldSectorAtomicItGetNextMacro(_it, _atomic)                       \
MACRO_START                                                                 \
{                                                                           \
	RpTie  *tie = rwLLLinkGetData((_it).cur, RpTie, lAtomicInWorldSector);  \
	(_atomic) = tie->apAtom;                                                \
	(_it).cur = rwLLLinkGetNext((_it).cur);                                 \
}                                                                           \
MACRO_STOP


#define LineStartNotEqualToEnd(_line)                           \
	(   ((_line)->start.x != (_line)->end.x)                    \
	||  ((_line)->start.y != (_line)->end.y)                    \
	||  ((_line)->start.z != (_line)->end.z))


#define SphereBBoxMacro(_bb, _center, _rad)                     \
MACRO_START                                                     \
{                                                               \
	(_bb)->inf = (_bb)->sup = *(_center);                       \
	(_bb)->inf.x -= (_rad);                                     \
	(_bb)->inf.y -= (_rad);                                     \
	(_bb)->inf.z -= (_rad);                                     \
	(_bb)->sup.x += (_rad);                                     \
	(_bb)->sup.y += (_rad);                                     \
	(_bb)->sup.z += (_rad);                                     \
}                                                               \
MACRO_STOP

#define IntervalDistMacro(_v, _a, _b)   \
	(((_v)<(_a)) ? ((_a)-(_v)) : (((_v)>(_b)) ? ((_v)-(_b)) : 0))

#define SphereBBoxTestMacro(_result, _sph, _bb, _distSq)                    \
MACRO_START                                                                 \
{                                                                           \
	RwReal  dist;                                                           \
	\
	dist = IntervalDistMacro((_sph)->center.x, (_bb)->inf.x, (_bb)->sup.x); \
	*(_distSq) = dist*dist;                                                 \
	dist = IntervalDistMacro((_sph)->center.y, (_bb)->inf.y, (_bb)->sup.y); \
	*(_distSq) += dist*dist;                                                \
	dist = IntervalDistMacro((_sph)->center.z, (_bb)->inf.z, (_bb)->sup.z); \
	*(_distSq) += dist*dist;                                                \
	\
	(_result) = (*(_distSq) <= ((_sph)->radius * (_sph)->radius));          \
}                                                                           \
MACRO_STOP

#define LineSphereTestMacro(_result, _lineStart, _lineDelta,            \
			_lengthSq, _sphere, _centDist, _offsetSq)                   \
MACRO_START                                                             \
{                                                                       \
	/* Distances scaled by length to keep maths simple */               \
	RwV3d   vcenter, temp;                                              \
	RwReal  perpSq;                                                     \
	\
	/* Projection onto line of vector to sphere center */               \
	RwV3dSubMacro(&vcenter, &(_sphere)->center, _lineStart);                 \
	*(_centDist) = RwV3dDotProduct(&vcenter, _lineDelta);               \
	\
	/* Square of shortest distance from center to line.                 \
	* NO pythagoras here otherwise we can lose precision. */            \
	RwV3dScale(&temp, &vcenter, _lengthSq);                             \
	RwV3dIncrementScaled(&temp, _lineDelta, -*(_centDist));             \
	perpSq = RwV3dDotProduct(&temp, &vcenter);                          \
	\
	/* Squared offset from centDist to sphere surface */                \
	*(_offsetSq) =                                                      \
		(_sphere)->radius * (_sphere)->radius * (_lengthSq) - perpSq;   \
	\
	/* Does line trajectory hit the sphere? */                          \
	if (*(_offsetSq) >= 0)                                              \
	{                                                                   \
		if (*(_centDist) < 0)                                           \
		{                                                               \
			/* Center before line start */                              \
			(_result) = ((*(_centDist) * *(_centDist)) <= *(_offsetSq));\
		}                                                               \
		else if (*(_centDist) > (_lengthSq))                            \
		{                                                               \
			/* Center after line end */                                 \
			RwReal diff = *(_centDist) - (_lengthSq);                   \
			(_result) = ((diff * diff) <= *(_offsetSq));                \
		}                                                               \
		else                                                            \
		{                                                               \
			(_result) = TRUE;                                           \
		}                                                               \
	}                                                                   \
	else                                                                \
	{                                                                   \
		(_result) = FALSE;                                              \
	}                                                                   \
}                                                                       \
MACRO_STOP

//////////////////////////////////////////////////////////////////////////
//
//	Inner implementation
//
//////////////////////////////////////////////////////////////////////////


static void
NtlWorldForAllAtomicsIntsecSphere( RpWorld *world,
								   const RwSphere *sphere,
								   RpIntersection *is,
								   NtlIntersectionCallBackBBox callBBox,
								   void *bboxdata,
								   NtlIntersectionCallBackAtomic callBack,
								   void *atomicdata )
{
	RwBBox                  bbox;
	RpAtomic               *sphereAtomic;
	RwUInt32                ref;
	RpSectorBBoxIt          sectIt;
	RpWorldSector          *sector;
	WorldSectorAtomicIt     atomicIt;
	RpAtomic               *atomic;

	RWFUNCTION(RWSTRING("NtlWorldForAllAtomicsIntsecSphere"));

	/* Sphere info */
	SphereBBoxMacro(&bbox, &sphere->center, sphere->radius);
	sphereAtomic = (is->type == rpINTERSECTATOMIC) ?
		(RpAtomic *)is->t.object : (RpAtomic *)NULL;

	/* Increment global collision reference */
	ref = ++(_rpCollisionGlobalRefMacro());

	/* Iterator over sectors */
	_rpSectorBBoxItInitMacro(&sectIt, world, &bbox);
	while (!RpSectorBBoxItFinished(&sectIt))
	{
		RwReal          distSq;
		RwBool          result;
		const RwBBox   *sectorBB;

		_rpSectorBBoxItFindNextWorldSectorMacro(&sectIt, &sector);

		/* Eliminate sectors not intersecting sphere */
		sectorBB = RpWorldSectorGetBBox(sector);
		SphereBBoxTestMacro(result, sphere, sectorBB, &distSq);
		if (!result)
			continue;

		/* Iterate over atomics */
		WorldSectorAtomicItInitMacro(atomicIt, sector);
		while (!WorldSectorAtomicItFinishedMacro(&atomicIt))
		{
			WorldSectorAtomicItGetNextMacro(atomicIt, atomic);

			/* Check that we're not colliding atomic with itself, that
			* we haven't already tested it via another sector and
			* that it is actually flagged for collision tests. */
			if (atomic
				&& (atomic != sphereAtomic)
				&& (_rpCollisionAtomicRefMacro(atomic) != ref)
				&& (RpAtomicGetFlags(atomic) & rpATOMICCOLLISIONTEST))
			{
				const RwSphere *atomSphere;
				RwV3d   sep;
				RwReal  sumRadSq;

				/* Mark it */
				_rpCollisionAtomicRefMacro(atomic) = ref;

				/* Test for intersection */
				atomSphere = callBBox(atomic, bboxdata);
				RwV3dSub(&sep, &sphere->center, &atomSphere->center);
				distSq = RwV3dDotProduct(&sep, &sep);
				sumRadSq = sphere->radius + atomSphere->radius;
				sumRadSq = sumRadSq * sumRadSq;

				if (distSq <= sumRadSq)
				{
					/* Normalize distance */
					RwReal dist = distSq / sumRadSq;
					rwSqrt(&dist, dist);

					if (!callBack(is, sector, atomic, dist, atomicdata))
					{
						/* Early out */
						RWRETURNVOID();
					}
				}
			}
		}
	}

	RWRETURNVOID();
}


static void
NtlWorldForAllAtomicsIntsecLine( RpWorld *world,
								 RpIntersection *is,
								 NtlIntersectionCallBackBBox callBBox,
								 void *bboxdata,
								 NtlIntersectionCallBackAtomic callBack,
								 void *atomicdata )
{
	RwLine                 *line;
	RwV3d                   delta;
	RwReal                  lengthSq;
	RwUInt32                ref;
	RpSectorLineIt          sectIt;
	RpWorldSector          *sector;
	WorldSectorAtomicIt     atomicIt;
	RpAtomic               *atomic;

	RWFUNCTION(RWSTRING("NtlWorldForAllAtomicsIntsecLine"));

	/* Line params */
	line = &is->t.line;
	RwV3dSub(&delta, &line->end, &line->start);
	lengthSq = RwV3dDotProduct(&delta, &delta);

	/* Increment the global collision reference */
	ref = ++(_rpCollisionGlobalRefMacro());

	/* Iterate over sectors */
	_rpSectorLineItInitMacro(&sectIt, world, line, (RwV3d *)NULL);
	while (!RpSectorLineItFinished(&sectIt))
	{
		_rpSectorLineItFindNextWorldSectorMacro(&sectIt, &sector);

		/* Iterate over atomics */
		WorldSectorAtomicItInitMacro(atomicIt, sector);
		while (!WorldSectorAtomicItFinishedMacro(&atomicIt))
		{
			WorldSectorAtomicItGetNextMacro(atomicIt, atomic);

			/* Check that atomic hasn't already been tested via another
			* sector and that it is flagged for collision tests. */
			if (atomic
				&& (_rpCollisionAtomicRefMacro(atomic) != ref)
				&& (RpAtomicGetFlags(atomic) & rpATOMICCOLLISIONTEST))
			{
				const RwSphere *sphere;
				RwReal          dist, offsetSq;
				RwBool          result;

				/* Mark it */
				_rpCollisionAtomicRefMacro(atomic) = ref;

				/* Test for intersection */
				sphere = callBBox(atomic, bboxdata);
				LineSphereTestMacro(result,
					&line->start, &delta, lengthSq, sphere, &dist, &offsetSq);

				if (result)
				{
					/* Return parametric distance */
					dist /= lengthSq;
					if (!callBack(is, sector, atomic, dist, atomicdata))
					{
						/* Early out */
						RWRETURNVOID();
					}
				}
			}
		}
	}

	RWRETURNVOID();
}


static void
NtlWorldForAllAtomicsIntsecBBox( RpWorld *world,
								 RpIntersection *is,
								 NtlIntersectionCallBackBBox callBBox,
								 void *bboxdata,
								 NtlIntersectionCallBackAtomic callBack,
								 void *atomicdata )
{
	RwBBox                 *bbox;
	RwUInt32                ref;
	RpSectorBBoxIt          sectIt;
	RpWorldSector          *sector;
	WorldSectorAtomicIt     atomicIt;
	RpAtomic               *atomic;

	RWFUNCTION(RWSTRING("NtlWorldForAllAtomicsIntsecBBox"));

	/* Increment global collision reference */
	ref = ++(_rpCollisionGlobalRefMacro());

	/* Iterate over sectors */
	bbox = &is->t.box;
	_rpSectorBBoxItInitMacro(&sectIt, world, bbox);
	while (!RpSectorBBoxItFinished(&sectIt))
	{
		_rpSectorBBoxItFindNextWorldSectorMacro(&sectIt, &sector);

		/* Iterate over atomics */
		WorldSectorAtomicItInitMacro(atomicIt, sector);
		while (!WorldSectorAtomicItFinishedMacro(&atomicIt))
		{
			WorldSectorAtomicItGetNextMacro(atomicIt, atomic);

			/* Check that atomic hasn't already been tested via another
			* sector and that it is flagged for collision tests. */
			if (atomic
				&& (_rpCollisionAtomicRefMacro(atomic) != ref)
				&& (RpAtomicGetFlags(atomic) & rpATOMICCOLLISIONTEST))
			{
				const RwSphere *sphere;
				RwReal          distSq;
				RwBool          result;

				/* Mark it */
				_rpCollisionAtomicRefMacro(atomic) = ref;

				/* Test for intersection */
				sphere = callBBox(atomic, bboxdata);
				SphereBBoxTestMacro(result, sphere, bbox, &distSq);
				if (result)
				{
					if (!callBack(is, sector, atomic, 0.0f, atomicdata))
					{
						/* Early out */
						RWRETURNVOID();
					}
				}
			}
		}
	}

	RWRETURNVOID();
}


//////////////////////////////////////////////////////////////////////////
//
//	NtlWorldForAllAtomicIntersections_CustomBBox
//
//////////////////////////////////////////////////////////////////////////


RpWorld * NtlWorldForAllAtomicIntersections_CustomBBox( RpWorld *world,
													    RpIntersection *intersection,
														NtlIntersectionCallBackBBox callBBox,
														void *bboxdata,
														NtlIntersectionCallBackAtomic callBack,
														void *atomicdata )
{
	RWAPIFUNCTION(RWSTRING("NtlWorldForAllAtomicIntersections_CustomBBox"));
	RWASSERT(_rpCollisionNumInstances);
	RWASSERT(world);
	RWASSERT(intersection);
	RWASSERT(callBBox);
	RWASSERT(callBack);

	/* Temporary fix: we resynch all dirty frames in case users do simulation
	* updates inside a camera update. Ideally (eventually), we will move
	* towards a system where the users do simulation updates whenever they
	* like and frame resynching is entirely their responsibility. */
	_rwFrameSyncDirty();

	switch (intersection->type)
	{
	case (rpINTERSECTPOINT):
		{
			RwSphere sphere;

			sphere.center = intersection->t.point;
			sphere.radius = 0;

			NtlWorldForAllAtomicsIntsecSphere(
				world, &sphere, intersection, callBBox, bboxdata, callBack, atomicdata);
			RWRETURN(world);
		}

	case (rpINTERSECTLINE):
		{
			RWASSERT(LineStartNotEqualToEnd(&intersection->t.line));
			NtlWorldForAllAtomicsIntsecLine(
				world, intersection, callBBox, bboxdata, callBack, atomicdata);
			RWRETURN(world);
		}

	case (rpINTERSECTSPHERE):
		{
			NtlWorldForAllAtomicsIntsecSphere(
				world, &intersection->t.sphere, intersection, callBBox, bboxdata, callBack, atomicdata);
			RWRETURN(world);
		}

	case (rpINTERSECTBOX):
		{
			NtlWorldForAllAtomicsIntsecBBox(
				world, intersection, callBBox, bboxdata, callBack, atomicdata);
			RWRETURN(world);
		}

	case (rpINTERSECTATOMIC):
		{
			RpAtomic *atomic = (RpAtomic *)intersection->t.object;
			RwSphere sphere;

			RWASSERT(atomic);
			RWASSERTISTYPE(atomic, rpATOMIC);
			sphere = *callBBox(atomic, bboxdata);

			NtlWorldForAllAtomicsIntsecSphere(
				world, &sphere, intersection, callBBox, bboxdata, callBack, atomicdata);
			RWRETURN(world);
		}

	default:
		{
			RWASSERT(0);
			RWMESSAGE(("Unsupported or invalid intersection type."));
			RWRETURN(world);
		}
	}
}
