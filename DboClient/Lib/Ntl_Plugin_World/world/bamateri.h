
/***************************************************************************
 *                                                                      *
 * Module  : bamateri.h                                                    *
 *                                                                         *
 * Purpose : Device layer                                                  *
 *                                                                         *
 **************************************************************************/

#ifndef RWMATERI_H
#define RWMATERI_H

/****************************************************************************
 Includes
 */

#include <rwcore.h>

/****************************************************************************
 Defines
 */

/* RWPUBLIC */

/*
 * Handling surface materials
 * Materials describe how things are to appear when rendered
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 */

/****************************************************************************
 Global Types
 */

/* RWPUBLICEND */
typedef struct rpMaterialGlobals rpMaterialGlobals;
struct rpMaterialGlobals
{
    RwFreeList           *matFreeList;
};

/* RWPUBLIC */

typedef struct RpMaterialChunkInfo RpMaterialChunkInfo;
typedef struct RpMaterialChunkInfo _rpMaterial;

#if (!defined(DOXYGEN))
struct RpMaterialChunkInfo
{
    RwInt32             flags;       /* Material flags - unused currently - 
                                      * for future expansion */
    RwRGBA              color;       /* Color of material. */
    RwInt32             unused;      /* Not used */
    RwBool              textured;    /* Are we textured? */
    RwSurfaceProperties surfaceProps;/* Surface properties */
};
#endif /* (!defined(DOXYGEN)) */

#if (!defined(RwMaterialAssign))
#define RwMaterialAssign(_target, _source)             \
    ( *(_target) = *(_source) )
#endif /* (!defined(RwMaterialAssign)) */

/**
 * \ingroup rpmaterial
 * \struct RpMaterial 
 * Material object. This should be 
 * considered an opaque type. Use the RpMaterial API functions to access.
 */
typedef struct RpMaterial RpMaterial;

#if (!defined(DOXYGEN))
struct RpMaterial
{
        RwTexture           *texture; /**< texture */
        RwRGBA              color; /**< color */              
        RxPipeline          *pipeline; /**< pipeline */     
        RwSurfaceProperties surfaceProps; /**< surfaceProps */
        RwInt16             refCount;          /* C.f. rwsdk/world/bageomet.h:RpGeometry */
        RwInt16             pad;
};
#endif /* (!defined(DOXYGEN)) */

/**
 * \ingroup rpmaterial
 * \ref RpMaterialCallBack 
 * represents the function called from \ref RpGeometryForAllMaterials and 
 * \ref RpWorldForAllMaterials for all materials referenced by polygons in a 
 * given geometry. This function should return a pointer to the current 
 * material to indicate success. The callback may return NULL to terminate 
 * further callbacks on the materials.
 * 
 * \param  material   Pointer to the current material
 * \param  data       Pointer to developer-defined data structure.
 * 
 * \return Pointer to the current material, or NULL to terminate further callbacks.
 */
typedef RpMaterial *(*RpMaterialCallBack)(RpMaterial *material, void *data);

/****************************************************************************
 <macro/inline functionality
 */

#define RpMaterialAddRefMacro(_material)                            \
    (((_material)->refCount++), (_material))

#define RpMaterialAddRefVoidMacro(_material)                        \
MACRO_START                                                         \
{                                                                   \
    (_material)->refCount++;                                        \
}                                                                   \
MACRO_STOP


#define RpMaterialSetColorMacro(_material, _color)                  \
    (RwRGBAAssign(&((_material)->color), (_color)), (_material))

#define RpMaterialGetColorMacro(_material)                          \
    (&((_material)->color))

#define RpMaterialSetSurfacePropertiesMacro(_material, _surfProps)  \
    (RwSurfacePropertiesAssign(&((_material)->surfaceProps),        \
                               (_surfProps)), (_material))

#define RpMaterialSetSurfacePropertiesVoidMacro(_material, _surfProps)  \
MACRO_START                                                             \
{                                                                       \
    RwSurfacePropertiesAssign(&((_material)->surfaceProps),             \
                              (_surfProps));                            \
}                                                                       \
MACRO_STOP

#define RpMaterialGetSurfacePropertiesMacro(_material)              \
    (&((_material)->surfaceProps))

#define RpMaterialGetTextureMacro(_material)                        \
    ((_material)->texture)


#if !(defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

#define RpMaterialAddRef(_material)                                 \
    RpMaterialAddRefMacro(_material)

#define RpMaterialSetColor(_material, _color)                       \
    RpMaterialSetColorMacro(_material, _color)

#define RpMaterialGetColor(_material)                               \
    RpMaterialGetColorMacro(_material)

#define RpMaterialSetSurfaceProperties(_material, _surfProps)       \
    RpMaterialSetSurfacePropertiesMacro(_material, _surfProps)

#define RpMaterialGetSurfaceProperties(_material)                   \
    RpMaterialGetSurfacePropertiesMacro(_material)

#define RpMaterialGetTexture(_material)                             \
    RpMaterialGetTextureMacro(_material)

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

/* RWPUBLICEND */

/* Opening and closing materials */
extern void *_rpMaterialClose(void *instance, RwInt32 offset, RwInt32 size);
extern void *_rpMaterialOpen(void *instance, RwInt32 offset, RwInt32 size);

extern RwStream *_rpReadMaterialRights(RwStream *s, RwInt32 len, void *obj,
                                     RwInt32 off, RwInt32 size);
extern RwStream *_rpWriteMaterialRights(RwStream *s, RwInt32 len, const void *obj,
                                      RwInt32 off, RwInt32 size);
extern RwInt32 _rpSizeMaterialRights(const void *obj, RwInt32 off, RwInt32 size);

/* Setting default surface properties - LEGACY-SUPPORT hack for older files */
extern void _rpMaterialSetDefaultSurfaceProperties( const RwSurfaceProperties *surfaceProps );

/* RWPUBLIC */

extern void RpMaterialSetFreeListCreateParams( RwInt32 blockSize,
                                               RwInt32 numBlocksToPrealloc );

/* Creating, destroying and referencing materials */
extern RpMaterial *RpMaterialCreate(void);
extern RwBool RpMaterialDestroy(RpMaterial *material);
extern RpMaterial *RpMaterialClone(RpMaterial *material);

/* Textures */
extern RpMaterial *RpMaterialSetTexture(RpMaterial *material, RwTexture *texture);


#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))
extern RpMaterial *RpMaterialAddRef(RpMaterial *material);

/* Textures */
extern RwTexture *RpMaterialGetTexture(const RpMaterial *material);

/* Setting and getting colors */
extern RpMaterial *RpMaterialSetColor(RpMaterial *material, const RwRGBA *color);
extern const RwRGBA *RpMaterialGetColor(const RpMaterial *material);

/* Setting and getting surface properties */
extern RpMaterial *
RpMaterialSetSurfaceProperties(RpMaterial *material,
                               const RwSurfaceProperties *surfaceProperties);

extern const RwSurfaceProperties *
RpMaterialGetSurfaceProperties(const RpMaterial *material);

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */


/* Attaching toolkits */
extern RwInt32 RpMaterialRegisterPlugin(RwInt32 size, RwUInt32 pluginID,
                                        RwPluginObjectConstructor constructCB,
                                        RwPluginObjectDestructor destructCB,
                                        RwPluginObjectCopy copyCB);
extern RwInt32 RpMaterialRegisterPluginStream(RwUInt32 pluginID,
                                              RwPluginDataChunkReadCallBack readCB,
                                              RwPluginDataChunkWriteCallBack writeCB,
                                              RwPluginDataChunkGetSizeCallBack getSizeCB);
extern RwPluginRegistry *RpMaterialGetRegistry(void);
extern RwInt32 RpMaterialSetStreamAlwaysCallBack(
                        RwUInt32 pluginID,
                        RwPluginDataChunkAlwaysCallBack alwaysCB);
extern RwInt32 RpMaterialSetStreamRightsCallBack(RwUInt32 pluginID,
                                                 RwPluginDataChunkRightsCallBack rightsCB);
extern RwInt32 RpMaterialGetPluginOffset(RwUInt32 pluginID);
extern RwBool RpMaterialValidatePlugins(const RpMaterial *material);

/* Binary format */
extern RwUInt32 RpMaterialStreamGetSize(const RpMaterial *material);
extern RpMaterial *RpMaterialStreamRead(RwStream *stream);
extern const RpMaterial *RpMaterialStreamWrite(const RpMaterial *material, RwStream *stream);
extern RpMaterialChunkInfo *
_rpMaterialChunkInfoRead(RwStream *stream, 
                         RpMaterialChunkInfo *materialChunkInfo, 
                         RwInt32 *bytesRead);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#define RpMaterialChunkInfoRead(stream, materialChunkInfo, bytesRead) \
       _rpMaterialChunkInfoRead(stream, materialChunkInfo, bytesRead)

/* RWPUBLICEND */

#endif /* RWMATERI_H */

