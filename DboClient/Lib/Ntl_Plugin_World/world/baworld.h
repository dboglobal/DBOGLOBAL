/***************************************************************************
 *                                                                         *
 * Module  : baworld.h                                                     *
 *                                                                         *
 * Purpose : World handling functions.                                     *
 *                                                                         *
 **************************************************************************/

#ifndef RWWORLD_H
#define RWWORLD_H

/****************************************************************************
 Includes
 */

#include <rwcore.h>
#include "baclump.h"
#include "balight.h"
#include "bamatlst.h"
#include "basector.h"
#include <rpdbgerr.h>
#include <rpworld.rpe>

/* RWPUBLIC */

/*
 * World handling.
 * World give objects scope, and provide a mechanism for
 * efficiency static object rendering.
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 *
 */

/****************************************************************************
 Defines
 */

/* Type ID */
#define rpWORLD 7

/* RpWorld private flags (in RwObject) */
enum RpWorldPrivateFlag
{
    rpWORLDSINGLEMALLOC = 0x01,
    rpWORLDPRIVATEFLAGFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RpWorldPrivateFlag RpWorldPrivateFlag;

/**
 * \ingroup rpworldsub
 * The bit-field type \ref RpWorldFlag specifies the options available
 * for creating the static geometry component of a world (see API function \ref RpWorldSetFlags):
 */
enum RpWorldFlag
{
    rpWORLDTRISTRIP  = 0x01, /**<This world's meshes can be rendered
                                 as tri strips */
    rpWORLDPOSITIONS = 0x02, /**<This world has positions */
    rpWORLDTEXTURED  = 0x04, /**<This world has only one set of texture coordinates */
    rpWORLDPRELIT    = 0x08, /**<This world has luminance values */
    rpWORLDNORMALS   = 0x10, /**<This world has normals */
    rpWORLDLIGHT     = 0x20, /**<This world will be lit */
    rpWORLDMODULATEMATERIALCOLOR = 0x40,
                             /**<Modulate material color with vertex colors
                                 (pre-lit + lit) */
    rpWORLDTEXTURED2 = 0x80, /**<This world has 2 or more sets of texture coordinates */

    /*
     * These above flags were stored in the flags field in an RwObject, they
     * are now stored in the flags file of the RpWorld.
     */

    rpWORLDNATIVE               = 0x01000000,
    rpWORLDNATIVEINSTANCE       = 0x02000000,

    rpWORLDFLAGSMASK            = 0x000000FF,
    rpWORLDNATIVEFLAGSMASK      = 0x0F000000,

    rpWORLDSECTORSOVERLAP       = 0x40000000,

    rpWORLDFLAGFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RpWorldFlag RpWorldFlag;

/* Multi texture coordinate format specifier */
#define rpWORLDTEXCOORDSETS(_num)  ((_num & 0xff) << 16)

/* Maximum depth of BSP tree */
#define rpWORLDMAXBSPDEPTH 64


/* macro/inline functionality */
#define RpWorldGetBBoxMacro(_world)                         \
    (&((_world)->boundingBox))

#define _rpWorldGetOriginMacro(_world)                      \
    (&((_world)->worldOrigin))

#define RpWorldGetNumMaterialsMacro(_world)                 \
    ((_world)->matList.numMaterials)

#define RpWorldGetMaterialMacro(_world, _num)               \
    (((_world)->matList.materials)[(_num)])

#define RpWorldGetNumClumpsMacro(_world)                    \
    ((_world)->numClumpsInWorld)

#define RpWorldSetRenderOrderMacro(_world, _renderOrder)    \
    (((_world)->renderOrder = _renderOrder), (_world))

#define RpWorldGetRenderOrderMacro(_world)                  \
    ((_world)->renderOrder)

#define RpWorldSetFlagsMacro(_world, _flags)                \
    (((_world)->flags = (_flags)), (_world))

#define RpWorldGetFlagsMacro(_world)                        \
    ((_world)->flags)


#if !(defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

#define RpWorldGetBBox(_world)                              \
    RpWorldGetBBoxMacro(_world)

#define _rpWorldGetOrigin(_world)                           \
    _rpWorldGetOriginMacro(_world)

#define RpWorldGetNumMaterials(_world)                      \
    RpWorldGetNumMaterialsMacro(_world)

#define RpWorldGetMaterial(_world, _num)                    \
    RpWorldGetMaterialMacro(_world, _num)

#define RpWorldGetNumClumps(_world)                         \
    RpWorldGetNumClumpsMacro(_world)

#define RpWorldSetRenderOrder(_world, _renderOrder)         \
    RpWorldSetRenderOrderMacro(_world, _renderOrder)

#define RpWorldGetRenderOrder(_world)                       \
    RpWorldGetRenderOrderMacro(_world)

#define RpWorldSetFlags(_world, _flags)                     \
    RpWorldSetFlagsMacro(_world, _flags)

#define RpWorldGetFlags(_world)                             \
    RpWorldGetFlagsMacro(_world)

#endif /* !(defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */


/****************************************************************************
 Global types
 */

/**
 * \ingroup rpworldsub
 *
 * A pluginID is a unique identifiers for a plugin. This is a list of the IDs for
 * RpWorld plugin, which is not just one plugin, but a collection of plugins
 * that extend RenderWare Graphics Core. See \ref RwEngineRegisterPlugin
 *
 */
enum RwCriterionWorldID
{
    /* Guard value that should not be used. */
    rwID_NAWORLDID = MAKECHUNKID(rwVENDORID_CRITERIONWORLD, 0x00),

    /* The valid plugin IDs */
    /**< RpMaterial pluginID */
    rwID_MATERIALMODULE      = MAKECHUNKID(rwVENDORID_CRITERIONWORLD, 0x01),
    /**< RpMesh pluginID */
    rwID_MESHMODULE          = MAKECHUNKID(rwVENDORID_CRITERIONWORLD, 0x02),
    /**< RpGeometry pluginID */
    rwID_GEOMETRYMODULE      = MAKECHUNKID(rwVENDORID_CRITERIONWORLD, 0x03),
    /**< RpClump pluginID */
    rwID_CLUMPMODULE         = MAKECHUNKID(rwVENDORID_CRITERIONWORLD, 0x04),
    /**< RpLight pluginID */
    rwID_LIGHTMODULE         = MAKECHUNKID(rwVENDORID_CRITERIONWORLD, 0x05),
    /* Not used */
    rwID_COLLISIONMODULE     = MAKECHUNKID(rwVENDORID_CRITERIONWORLD, 0x06),
    /**< RpWorld pluginID */
    rwID_WORLDMODULE         = MAKECHUNKID(rwVENDORID_CRITERIONWORLD, 0x07),
    /* Not used */
    rwID_RANDOMMODULE        = MAKECHUNKID(rwVENDORID_CRITERIONWORLD, 0x08),
    /**< PluginID for RpWorld's objects */
    rwID_WORLDOBJMODULE      = MAKECHUNKID(rwVENDORID_CRITERIONWORLD, 0x09),
    /**< RpWorldSector pluginID */
    rwID_SECTORMODULE        = MAKECHUNKID(rwVENDORID_CRITERIONWORLD, 0x0A),
    /**< Binary RpWorld pluginID */
    rwID_BINWORLDMODULE      = MAKECHUNKID(rwVENDORID_CRITERIONWORLD, 0x0B),
    /**< RpWorld pipeline pluginID */
    rwID_WORLDPIPEMODULE     = MAKECHUNKID(rwVENDORID_CRITERIONWORLD, 0x0D),
    /**< Binary RpMesh pluginID */
    rwID_BINMESHPLUGIN       = MAKECHUNKID(rwVENDORID_CRITERIONWORLD, 0x0E),
    /**< RpWorld device pluginID */
    rwID_RXWORLDDEVICEMODULE = MAKECHUNKID(rwVENDORID_CRITERIONWORLD, 0x0F),
    /**< PluginID for platform specific serialization data */
    rwID_NATIVEDATAPLUGIN    = MAKECHUNKID(rwVENDORID_CRITERIONWORLD, 0x10),
    /**< \if xbox Vertex format pluginID \endif */
    /**< \if gcn  Vertex format pluginID \endif */
    rwID_VERTEXFMTPLUGIN     = MAKECHUNKID(rwVENDORID_CRITERIONWORLD, 0x11),
    rwCRITERIONWORLDIDFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RwCriterionWorldID RwCriterionWorldID;

/**
 * \ingroup rpworldsub
 * \ref RpWorldRenderOrder
 * represents the options available for
 * the rendering order of world sectors in the camera's view frustum (see
 * API function \ref RpWorldSetRenderOrder).
 */
enum RpWorldRenderOrder
{
    rpWORLDRENDERNARENDERORDER = 0,
    rpWORLDRENDERFRONT2BACK,  /**<Renders nearest sectors first */
    rpWORLDRENDERBACK2FRONT, /**<Renders furthest sectors first */
    rpWORLDRENDERORDERFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RpWorldRenderOrder RpWorldRenderOrder;

/**
 * \ingroup rpworldsector
 * \ref RpWorldSectorCallBackRender represents the function called from
 * \ref RpWorldSectorRender when the specified world sector lies inside the
 * current camera's view frustum. The default callback initiates execution of
 * the world sector rendering pipeline. This function should return a pointer
 * to the world sector to indicate success
 *
 * \param  worldSector   Pointer to the world sector to be
 *                       rendered
 *
 * \return Pointer to the world sector.
 *
 */
typedef RpWorldSector *(*RpWorldSectorCallBackRender) (RpWorldSector *
                                                       worldSector);

/**
 * \ingroup rpworldsub
 * \struct RpWorld
 * World object. This should be considered an opaque type.
 * Use the RpWorld API functions to access.
 */
typedef struct RpWorld RpWorld;

#if (!defined(DOXYGEN))
struct RpWorld
{
    RwObject            object;

    RwUInt32            flags;

    RpWorldRenderOrder  renderOrder;

    /* Materials */
    RpMaterialList      matList;

    /* The world stored as a BSP tree */
    RpSector           *rootSector;

    /* The number of texture coordinate sets in each sector */
    RwInt32             numTexCoordSets;

    /* Render frame used when last rendered */
    RwInt32             numClumpsInWorld;
    RwLLLink           *currentClumpLink;

    /* All the clumps in the world */
    RwLinkList          clumpList;

    /* All of the lights in the world */
    RwLinkList          lightList;

    /* Directional lights in the world */
    RwLinkList          directionalLightList;

    /* The worlds origin offset */
    RwV3d               worldOrigin;

    /* Bounding box around the whole world */
    RwBBox              boundingBox;

    /* The callbacks functions */
    RpWorldSectorCallBackRender renderCallBack;

    RxPipeline         *pipeline;
};
#endif /* (!defined(DOXYGEN)) */

/****************************************************************************
 Global Variables
 */

#ifdef    __cplusplus
extern              "C"
{
#endif                          /* __cplusplus */

extern RwPluginRegistry worldTKList;

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern              "C"
{
#endif                          /* __cplusplus */

extern RwBool       _rpWorldFindBBox(RpWorld * world,
                                    RwBBox * boundingBox);

/* Generate sector bounding boxes from BSP planes (not tight boxes) */
extern RpWorld      *_rpWorldSetupSectorBoundingBoxes(RpWorld *world);

extern void _rpWorldSectorDeinstanceAll(RpSector * sector);

extern void _rpWorldSectorDestroyRecurse(RpSector * sector);


/* RWPUBLICEND */

/* NON API */

/* Find the world size */
extern RpWorldSector *rpWorldFindWorldSector(RpWorld * world,
                                             RwV3d * point);
/* RWPUBLIC */

/* These stealth-API */
extern RwBool       _rpWorldForAllGlobalLights(RpLightCallBack callBack,
                                              void *pData);
extern RpWorldSector *_rpWorldSectorForAllLocalLights(RpWorldSector *
                                                     sector,
                                                     RpLightCallBack
                                                     callBack,
                                                     void *pData);

extern RpWorldSector *_rpSectorDefaultRenderCallBack(RpWorldSector *
                                                    sector);

/* Accessing worlds geometry */
extern RpWorld     *RpWorldLock(RpWorld * world);
extern RpWorld     *RpWorldUnlock(RpWorld * world);

/* Managing a list of the worlds */
extern void         _rpWorldRegisterWorld(RpWorld * world,
                                         RwUInt32 memorySize);
extern void         _rpWorldUnregisterWorld(RpWorld * world);

/* Hook into world management */
extern RwBool       RpWorldPluginAttach(void);

/* Enumerating objects in the world */
extern RpWorld     *RpWorldForAllClumps(RpWorld * world,
                                        RpClumpCallBack fpCallBack,
                                        void *pData);
extern RpWorld     *RpWorldForAllMaterials(RpWorld * world,
                                           RpMaterialCallBack fpCallBack,
                                           void *pData);
extern RpWorld     *RpWorldForAllLights(RpWorld * world,
                                        RpLightCallBack fpCallBack,
                                        void *pData);
extern RpWorld     *RpWorldForAllWorldSectors(RpWorld * world,
                                              RpWorldSectorCallBack
                                              fpCallBack, void *pData);

/* Rendering */
extern RpWorld     *RpWorldRender(RpWorld * world);

/* Creation/destruction */
extern RwBool       RpWorldDestroy(RpWorld * world);
extern RpWorld     *RpWorldCreate(RwBBox * boundingBox);
extern RwBool       RpWorldInstance(RpWorld *world);

/* Sector callbacks */
extern RpWorld     *RpWorldSetSectorRenderCallBack(RpWorld * world,
                                                   RpWorldSectorCallBackRender
                                                   fpCallBack);
extern RpWorldSectorCallBackRender RpWorldGetSectorRenderCallBack(const
                                                                  RpWorld
                                                                  *
                                                                  world);

/* Getting world from sector */
extern RpWorld     *RpWorldSectorGetWorld(const RpWorldSector * sector);

extern RwInt32      RpWorldRegisterPlugin(RwInt32 size, RwUInt32 pluginID,
                                          RwPluginObjectConstructor
                                          constructCB,
                                          RwPluginObjectDestructor
                                          destructCB,
                                          RwPluginObjectCopy copyCB);
extern RwInt32      RpWorldRegisterPluginStream(RwUInt32 pluginID,
                                                RwPluginDataChunkReadCallBack
                                                readCB,
                                                RwPluginDataChunkWriteCallBack
                                                writeCB,
                                                RwPluginDataChunkGetSizeCallBack
                                                getSizeCB);
extern RwInt32      RpWorldSetStreamAlwaysCallBack(
                        RwUInt32 pluginID,
                        RwPluginDataChunkAlwaysCallBack alwaysCB);
extern RwInt32      RpWorldSetStreamRightsCallBack(RwUInt32 pluginID,
                                     RwPluginDataChunkRightsCallBack rightsCB);
extern RwInt32      RpWorldGetPluginOffset(RwUInt32 pluginID);
extern RwBool       RpWorldValidatePlugins(RpWorld * world);


#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/* Getting/setting */
extern const RwBBox *RpWorldGetBBox(const RpWorld *world);
extern const RwV3d *_rpWorldGetOrigin(const RpWorld *world);

/* Accessing a worlds materials */
extern RwInt32 RpWorldGetNumMaterials(const RpWorld *world);
extern RpMaterial *RpWorldGetMaterial(const RpWorld *world, RwInt32 matNum);

/* Clumps */
extern RwInt32 RpWorldGetNumClumps(RpWorld *world);

/* Render order */
extern RpWorld *RpWorldSetRenderOrder(RpWorld *world,
                                      RpWorldRenderOrder renderOrder);
extern RpWorldRenderOrder RpWorldGetRenderOrder(const RpWorld *world);

/* Flags */
extern RpWorld *RpWorldSetFlags(RpWorld *world, RwUInt32 flags);
extern RwUInt32 RpWorldGetFlags(const RpWorld *world);

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#define rpWorldFindBBox(_world, _boundingBox) \
    _rpWorldFindBBox(_world, _boundingBox)

#define rpWorldForAllGlobalLights(_callBack, _pData) \
    _rpWorldForAllGlobalLights(_callBack, _pData)

#define rpWorldSectorForAllLocalLights(_sector, _callBack, _pData) \
    _rpWorldSectorForAllLocalLights(_sector, _callBack, _pData)

#define _rpWorldLock(_world) \
    RpWorldLock(_world)

#define _rpWorldUnlock(_world) \
    RpWorldUnlock(_world)

#define rpWorldLock(_world) \
    RpWorldLock(_world)

#define rpWorldUnlock(_world) \
    RpWorldUnlock(_world)

/* RWPUBLICEND */

#endif                          /* RWWORLD_H */
