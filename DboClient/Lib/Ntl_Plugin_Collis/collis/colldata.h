/*
 *  RenderWare Graphics Collision Plugin
 *
 *  File:    colldata.h
 *  Purpose: Collision data management
 */

#ifndef COLLDATA_H
#define COLLDATA_H

/******************************************************************************
 *  Global Types
 */

/******************************************************************************
 *  Internal globals/macros
 */

extern RwInt32 _rpCollisionNumInstances;
extern RwInt32 _rpCollisionGlobalRefOffset;
extern RwInt32 _rpCollisionAtomicRefOffset;


#define _rpCollisionGlobalRefMacro() \
    (*RWPLUGINOFFSET(RwUInt32, RwEngineInstance, _rpCollisionGlobalRefOffset))

#define _rpCollisionAtomicRefMacro(_atomic) \
    (*RWPLUGINOFFSET(RwUInt32, _atomic, _rpCollisionAtomicRefOffset))


/* RWPUBLIC */

/******************************************************************************
 *  API types/globals/macros
 */

#if !defined(DOXYGEN)
typedef struct RpCollisionData RpCollisionData;
struct RpCollisionData
{
    RpCollTree  *tree;
};
#endif /* !defined(DOXYGEN) */

#define _rpCollisionGeometryGetDataMacro(_geometry) \
    RWPLUGINOFFSET(RpCollisionData, _geometry, _rpCollisionGeometryDataOffset)

#define _rpCollisionWorldSectorGetDataMacro(_sector) \
    RWPLUGINOFFSET(RpCollisionData, _sector, _rpCollisionWorldSectorDataOffset)


#if !(defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

#define RpCollisionGeometryGetCollTree(_geometry) \
    (_rpCollisionGeometryGetDataMacro(_geometry)->tree)

#define RpCollisionWorldSectorGetCollTree(_sector) \
    (_rpCollisionWorldSectorGetDataMacro(_sector)->tree)

#endif /* !(defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */


/******************************************************************************
 *  Functions
 */

#ifdef    __cplusplus
extern "C"
{
#endif /* __cplusplus */

extern RwInt32 _rpCollisionGeometryDataOffset;
extern RwInt32 _rpCollisionWorldSectorDataOffset;

/*
 *  Collision data
 */

extern RwBool
RpCollisionPluginAttach(void);

/* 
 *  Worlds
 */
extern RwBool
RpCollisionWorldQueryData(RpWorld *world);

extern RpWorld *
RpCollisionWorldDestroyData(RpWorld *world);

/*
 *  World Sectors  
 */

extern RpWorldSector *
RpCollisionWorldSectorDestroyData(RpWorldSector *worldSector);

extern RwBool
RpCollisionWorldSectorQueryData(RpWorldSector *worldSector);

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))
extern RpCollTree *
RpCollisionWorldSectorGetCollTree(RpWorldSector *worldSector);
#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

extern RpWorldSector *
_rpCollisionWorldSectorSetCollTree(RpWorldSector *worldSector, 
                                   RpCollTree *collTree);
/*
 *  Geometries
 */

extern RpGeometry *
RpCollisionGeometryDestroyData(RpGeometry *geometry);

extern RwBool
RpCollisionGeometryQueryData(RpGeometry *geometry);

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))
extern RpCollTree *
RpCollisionGeometryGetCollTree(RpGeometry *geometry);
#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

extern RpGeometry *
_rpCollisionGeometrySetCollTree(RpGeometry *geometry, 
                                RpCollTree *collTree);


#ifdef    __cplusplus
}
#endif /* __cplusplus */

/* RWPUBLICEND */

#endif /* COLLDATA_H */
