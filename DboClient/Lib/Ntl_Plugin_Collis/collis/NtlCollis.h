#ifndef _NTL_COLLIS_H_
#define _NTL_COLLIS_H_


typedef const RwSphere * (*NtlIntersectionCallBackBBox) ( RpAtomic * atomic, void * data );

typedef RpAtomic * (*NtlIntersectionCallBackAtomic) ( RpIntersection * intersection,
													  RpWorldSector * sector,
													  RpAtomic * atomic,
													  RwReal distance,
													  void *data );


#ifdef    __cplusplus
extern "C"
{
#endif /* __cplusplus */

extern
RpWorld * NtlWorldForAllAtomicIntersections_CustomBBox( RpWorld *world,
														RpIntersection *intersection,
														NtlIntersectionCallBackBBox callBBox,
														void *bboxdata,
														NtlIntersectionCallBackAtomic callBack,
														void *atomicdata );

#ifdef    __cplusplus
}
#endif /* __cplusplus */


#endif