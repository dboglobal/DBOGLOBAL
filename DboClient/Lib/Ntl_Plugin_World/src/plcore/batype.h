/***************************************************************************
 *                                                                         *
 * Module  : batype.h                                                      *
 *                                                                         *
 * Purpose : Generic type handling                                         *
 *                                                                         *
 **************************************************************************/

#ifndef RWTYPE_H
#define RWTYPE_H

/****************************************************************************
 Includes
 */

#include "batypes.h"

/* RWPUBLIC */
/****************************************************************************
 Defines
*/

/*
 * Object Types - these are used in the binary object
 * representations and in the debug library. They must
 * be unique.  They are the old system.
 */

#define rwID_DATABASE 0x64617462     /* datb */

#define MAKECHUNKID(vendorID, chunkID) (((vendorID & 0xFFFFFF) << 8) | (chunkID & 0xFF))
#define GETOBJECTID(chunkID) (chunkID & 0xFF)
#define GETVENDORID(chunkID) ((chunkID >> 8) & 0xFFFFFF)

/***
 *** These are the vendor IDs.  A customer must reserve a vendor ID in order
 *** to be able to write toolkits (this prevents clashes between toolkits).
 *** We reserve some for our own use as shown below.  These are all 24 bit.
 ***
 *** IMPORTANT NOTE: DO NOT UNDER ANY CIRCUMSTANCES CHANGE THESE VALUES. IF
 ***                 YOU ARE ADDING A NEW ONE, APPEND IT!
 ***
 *** They must all be unique.
 ***/

enum RwPluginVendor
{
    rwVENDORID_CORE             = 0x000000L,
    rwVENDORID_CRITERIONTK      = 0x000001L,
    rwVENDORID_REDLINERACER     = 0x000002L,
    rwVENDORID_CSLRD            = 0x000003L,
    rwVENDORID_CRITERIONINT     = 0x000004L,
    rwVENDORID_CRITERIONWORLD   = 0x000005L,
    rwVENDORID_BETA             = 0x000006L,
    rwVENDORID_CRITERIONRM      = 0x000007L,
    rwVENDORID_CRITERIONRWA     = 0x000008L, /* RenderWare Audio */
    rwVENDORID_CRITERIONRWP     = 0x000009L, /* RenderWare Physics */
    rwPLUGINVENDORFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RwPluginVendor RwPluginVendor;

/***
 *** These are the core objects (8 bit IDs).  They must all be unique.
 *** We can get away without using the MAKECHUNKID macro because the
 *** vendor ID in all cases will be zero (rwVENDORID_CORE).
 ***
 *** IMPORTANT NOTE: DO NOT UNDER ANY CIRCUMSTANCES CHANGE THESE VALUES. IF
 ***                 YOU ARE ADDING A NEW ONE, APPEND IT!
 ***/

/* These are the internal ones.  Because the core ID is 0, we can get away without
 * using the MAKECHUNKID macro for the CORE chunks.
 */

#ifndef RWADOXYGENEXTERNAL
/**
 * \ingroup rwstream
 * \ref RwCorePluginID
 *
 * A chunk is a block of binary stream data containing objects. These objects
 * are referenced using object IDs which are listed below. Before an object
 * stream is read, a chunk needs to be obtained containing the object ID.
 * These IDs are used with the function \ref RwStreamFindChunk.
 *
 * To write a chunk into a stream \ref RwStreamWriteChunkHeader is used
 * with one of the following object IDs.
 *
 * \note \ref rt2d and \ref rt2danim use different object IDs. These
 * are listed in \ref rt2dobjectids and \ref rt2danimobjectids.
 *
 * \see RwStreamFindChunk
 * \see RwStreamWriteChunkHeader
 */
enum RwCorePluginID
{
    rwID_NAOBJECT               = MAKECHUNKID(rwVENDORID_CORE, 0x00),
    rwID_STRUCT                 = MAKECHUNKID(rwVENDORID_CORE, 0x01),
    rwID_STRING                 = MAKECHUNKID(rwVENDORID_CORE, 0x02),
    rwID_EXTENSION              = MAKECHUNKID(rwVENDORID_CORE, 0x03),

    rwID_CAMERA                 = MAKECHUNKID(rwVENDORID_CORE, 0x05),
/**< RwCamera chunk. See \ref RwCameraStreamRead */

    rwID_TEXTURE                = MAKECHUNKID(rwVENDORID_CORE, 0x06),
/**< RwTexture chunk. See \ref RwTextureStreamRead */

    rwID_MATERIAL               = MAKECHUNKID(rwVENDORID_CORE, 0x07),
/**< RpMaterial chunk. See \ref RpMaterialStreamRead. */

    rwID_MATLIST                = MAKECHUNKID(rwVENDORID_CORE, 0x08),
    rwID_ATOMICSECT             = MAKECHUNKID(rwVENDORID_CORE, 0x09),
    rwID_PLANESECT              = MAKECHUNKID(rwVENDORID_CORE, 0x0A),

    rwID_WORLD                  = MAKECHUNKID(rwVENDORID_CORE, 0x0B),
/**< RpWorld chunk. See \ref RpWorldStreamRead. */

    rwID_SPLINE                 = MAKECHUNKID(rwVENDORID_CORE, 0x0C),
/**< RpSpline chunk. See \ref RpSplineStreamRead */

    rwID_MATRIX                 = MAKECHUNKID(rwVENDORID_CORE, 0x0D),
/**< RwMatrix chunk. See \ref RwMatrixStreamRead */

    rwID_FRAMELIST              = MAKECHUNKID(rwVENDORID_CORE, 0x0E),

    rwID_GEOMETRY               = MAKECHUNKID(rwVENDORID_CORE, 0x0F),
/**< RpGeometry chunk. See \ref RpGeometryStreamRead. */

    rwID_CLUMP                  = MAKECHUNKID(rwVENDORID_CORE, 0x10),
/**< RpClump chunk. See \ref RpClumpStreamRead. */

    rwID_LIGHT                  = MAKECHUNKID(rwVENDORID_CORE, 0x12),
/**< RpLight chunk. See \ref RpLightStreamRead. */

    rwID_UNICODESTRING          = MAKECHUNKID(rwVENDORID_CORE, 0x13),

    rwID_ATOMIC                 = MAKECHUNKID(rwVENDORID_CORE, 0x14),
/**< RpAtomic chunk. See \ref RpAtomicStreamRead */

    rwID_TEXTURENATIVE          = MAKECHUNKID(rwVENDORID_CORE, 0x15),

    rwID_TEXDICTIONARY          = MAKECHUNKID(rwVENDORID_CORE, 0x16),
/**< RwTexDictionary - platform specific texture dictionary.
 * See \ref RwTexDictionaryStreamRead. */

    rwID_ANIMDATABASE           = MAKECHUNKID(rwVENDORID_CORE, 0x17),

    rwID_IMAGE                  = MAKECHUNKID(rwVENDORID_CORE, 0x18),
/**< RwImage chunk. See \ref RwImageStreamRead */

    rwID_SKINANIMATION          = MAKECHUNKID(rwVENDORID_CORE, 0x19),
    rwID_GEOMETRYLIST           = MAKECHUNKID(rwVENDORID_CORE, 0x1A),

    rwID_ANIMANIMATION          = MAKECHUNKID(rwVENDORID_CORE, 0x1B),
/**< RtAnimAnimation chunk. See \ref RtAnimAnimationStreamRead. */
    rwID_HANIMANIMATION         = MAKECHUNKID(rwVENDORID_CORE, 0x1B),
/**< RtAnimAnimation chunk. For backwards compatibility. See \ref rwID_ANIMANIMATION. */

    rwID_TEAM                   = MAKECHUNKID(rwVENDORID_CORE, 0x1C),
/**< \ref RpTeam chunk. See \ref RpTeamStreamRead */
    rwID_CROWD                  = MAKECHUNKID(rwVENDORID_CORE, 0x1D),

    rwID_DMORPHANIMATION        = MAKECHUNKID(rwVENDORID_CORE, 0x1E),
/**< RpDMorphAnimation - delta morph animation chunk. See \ref RpDMorphAnimationStreamRead */

    rwID_RIGHTTORENDER          = MAKECHUNKID(rwVENDORID_CORE, 0x1f),

    rwID_MTEFFECTNATIVE         = MAKECHUNKID(rwVENDORID_CORE, 0x20),
/**< \if xbox RpMTEffect - multi-texture effect chunk. See \ref RpMTEffectStreamRead \endif */
/**< \if gcn  RpMTEffect - multi-texture effect chunk. See \ref RpMTEffectStreamRead \endif */

    rwID_MTEFFECTDICT           = MAKECHUNKID(rwVENDORID_CORE, 0x21),
/**< \if xbox RpMTEffectDict - multi-texture effect dictionary chunk. See \ref RpMTEffectDictStreamRead \endif */
/**< \if gcn  RpMTEffectDict - multi-texture effect dictionary chunk. See \ref RpMTEffectDictStreamRead \endif */

    rwID_TEAMDICTIONARY         = MAKECHUNKID(rwVENDORID_CORE, 0x22),
/**< \ref RpTeamDictionary chunk. See \ref RpTeamDictionaryStreamRead */

    rwID_PITEXDICTIONARY        = MAKECHUNKID(rwVENDORID_CORE, 0x23),
/**< RwTexDictionary - platform independent texture dictionary. See \ref RtPITexDictionaryStreamRead. */

    rwID_TOC                    = MAKECHUNKID(rwVENDORID_CORE, 0x24),
/**< RtTOC chunk. See \ref RtTOCStreamRead */

    rwID_PRTSTDGLOBALDATA       = MAKECHUNKID(rwVENDORID_CORE, 0x25),
/**< RpPrtStdEmitterClass, RpPrtStdParticleClass and RpPrtStdPropertyTable chunks.
 *   See \ref RpPrtStdEClassStreamRead, \ref RpPrtStdPClassStreamRead
 *   \ref RpPrtStdPropTabStreamRead and \ref RpPrtStdGlobalDataStreamRead */

    rwID_ALTPIPE                = MAKECHUNKID(rwVENDORID_CORE, 0x26),
    rwID_PIPEDS                 = MAKECHUNKID(rwVENDORID_CORE, 0x27),
    rwID_PATCHMESH              = MAKECHUNKID(rwVENDORID_CORE, 0x28),
/**< RpPatchMesh chunk. See \ref RpPatchMeshStreamRead */

    rwID_CHUNKGROUPSTART        = MAKECHUNKID(rwVENDORID_CORE, 0x29),
    rwID_CHUNKGROUPEND          = MAKECHUNKID(rwVENDORID_CORE, 0x2A),

    rwID_UVANIMDICT             = MAKECHUNKID(rwVENDORID_CORE, 0x2B),
/**< UV anim dictionary chunk. See \ref RpUVAnimGetDictSchema */

    rwID_COLLTREE               = MAKECHUNKID(rwVENDORID_CORE, 0x2C),

    rwID_ENVIRONMENT            = MAKECHUNKID(rwVENDORID_CORE, 0x2D),
/**< RpEnvironment chunk is used internally to convey art package background
   < settings to the Visualizer.*/    

/* Insert before MAX and increment MAX */
    rwID_COREPLUGINIDMAX        = MAKECHUNKID(rwVENDORID_CORE, 0x2E),
    rwCOREPLUGINIDFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RwCorePluginID RwCorePluginID ;
#endif /* RWADOXYGENEXTERNAL */

/***
 *** These are the Criterion internal plugin extensions.  Use with rwVENDORID_CRITERIONINT.
 ***
 *** IMPORTANT NOTE: DO NOT UNDER ANY CIRCUMSTANCES CHANGE THESE VALUES. IF
 ***                 YOU ARE ADDING A NEW ONE, APPEND IT!
 ***/

enum RwCriterionPluginID
{
    rwID_COREPLUGIN             = 0x01,
    rwID_WORLDPLUGIN            = 0x02,
    rwID_TOOLPLUGIN             = 0x03,
    rwID_TOOL2PLUGIN            = 0x04,
    rwCRITERIONPLUGINIDFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RwCriterionPluginID RwCriterionPluginID;

/**
 * \ingroup rwengine
 * The Core Library is consisted from a collection of plugins that constitute
 * it. This is a list of the pluginIDs that belong to the Core. See
 * \ref RwEngineRegisterPlugin
 */
enum RwCriterionCoreID
{
    /* Guard value that should not be used. */
    rwID_NACOREID               = MAKECHUNKID(rwVENDORID_CRITERIONINT, 0x00),

    /* The valid plugin IDs */
    /** Vector module pluginID. See \ref rwv2d and \ref rwv3d */
    rwID_VECTORMODULE           = MAKECHUNKID(rwVENDORID_CRITERIONINT, 0x01),
    /** Matrix module pluginID. See \ref rwmatrix */
    rwID_MATRIXMODULE           = MAKECHUNKID(rwVENDORID_CRITERIONINT, 0x02),
    /** Frame module pluginID. See \ref rwframe */
    rwID_FRAMEMODULE            = MAKECHUNKID(rwVENDORID_CRITERIONINT, 0x03),
    /** Stream module pluginID. See \ref rwstream */
    rwID_STREAMMODULE           = MAKECHUNKID(rwVENDORID_CRITERIONINT, 0x04),
    /** Camera module pluginID. See \ref rwcamera */
    rwID_CAMERAMODULE           = MAKECHUNKID(rwVENDORID_CRITERIONINT, 0x05),
    /** Image module pluginID. See \ref rwimage */
    rwID_IMAGEMODULE            = MAKECHUNKID(rwVENDORID_CRITERIONINT, 0x06),
    /** Raster module pluginID. See \ref rwraster */
    rwID_RASTERMODULE           = MAKECHUNKID(rwVENDORID_CRITERIONINT, 0x07),
    /** Texture module pluginID. See \ref rwtexture */
    rwID_TEXTUREMODULE          = MAKECHUNKID(rwVENDORID_CRITERIONINT, 0x08),
    /** Pipeline module pluginID. See \ref RxPipeline */
    rwID_PIPEMODULE             = MAKECHUNKID(rwVENDORID_CRITERIONINT, 0x09),
    /** Immediate module pluginID. See \ref rwim3d */
    rwID_IMMEDIATEMODULE        = MAKECHUNKID(rwVENDORID_CRITERIONINT, 0x0A),
    /** Resource module pluginID. See \ref rwresources */
    rwID_RESOURCESMODULE        = MAKECHUNKID(rwVENDORID_CRITERIONINT, 0x0B),
    /** Device module pluginID */
    rwID_DEVICEMODULE           = MAKECHUNKID(rwVENDORID_CRITERIONINT, 0x0C),
    /** Color module pluginID. See \ref rwrgba */
    rwID_COLORMODULE            = MAKECHUNKID(rwVENDORID_CRITERIONINT, 0x0D),
    /* Not used */
    rwID_POLYPIPEMODULE         = MAKECHUNKID(rwVENDORID_CRITERIONINT, 0x0E),
    /** Error module pluginID. See \ref rwerror */
    rwID_ERRORMODULE            = MAKECHUNKID(rwVENDORID_CRITERIONINT, 0x0F),
    /** Metrics module pluginID. See \ref RwMetrics */
    rwID_METRICSMODULE          = MAKECHUNKID(rwVENDORID_CRITERIONINT, 0x10),
    /** Driver module pluginID */
    rwID_DRIVERMODULE           = MAKECHUNKID(rwVENDORID_CRITERIONINT, 0x11),
    /** Chunk group module pluginID. See \ref rwchunkgroup */
    rwID_CHUNKGROUPMODULE       = MAKECHUNKID(rwVENDORID_CRITERIONINT, 0x12),
    rwCRITERIONCOREIDFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RwCriterionCoreID RwCriterionCoreID;

/***
 *** These are the Criterion internal platform identifies.
 ***
 *** IMPORTANT NOTE: DO NOT UNDER ANY CIRCUMSTANCES CHANGE THESE VALUES. IF
 ***                 YOU ARE ADDING A NEW ONE, APPEND IT!
 ***/
enum RwPlatformID
{
    rwID_PCD3D7 = 1,
    rwID_PCOGL,
    rwID_MAC,
    rwID_PS2,
    rwID_XBOX,
    rwID_GAMECUBE,
    rwID_SOFTRAS,
    rwID_PCD3D8,
    rwID_PCD3D9,
    rwID_PSP,
    rwPLATFROMIDFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RwPlatformID RwPlatformID;

/* RWPUBLICEND */

/* RWPUBLIC */

/****************************************************************************
 Global Types
 */

typedef struct RwObject RwObject;
#ifndef RWADOXYGENEXTERNAL
/**
 * \ingroup rwobject
 * \struct RwObject
 * This should be considered an opaque type. Use
 * the RwObject API functions to access.
 */
#endif /* RWADOXYGENEXTERNAL */
struct RwObject
{
        RwUInt8 type;                /**< Internal Use */
        RwUInt8 subType;             /**< Internal Use */
        RwUInt8 flags;               /**< Internal Use */
        RwUInt8 privateFlags;        /**< Internal Use */
        void   *parent;              /**< Internal Use */
                                     /* Often a Frame  */
};
#ifndef RWADOXYGENEXTERNAL
/**
 * \ingroup rwobject
 * \ref RwObjectCallBack
 * callback function supplied for object callback functions.
 *
 * \param  object   Pointer to the current object, supplied by
 *                  iterator.
 * \param  data     Pointer to developer-defined data structure.
 *
 * \return Pointer to the current object
 *
 * \see RwFrameForAllObjects
 *
 */
#endif /* RWADOXYGENEXTERNAL */
typedef RwObject *(*RwObjectCallBack)(RwObject *object, void *data);

/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

/* TYPE METHODS */

/* Creation/cloning */

#define rwObjectCopy(d,s)                               \
MACRO_START                                             \
{                                                       \
    ((RwObject *)(d))->type =                           \
        ((const RwObject *)(s))->type;                  \
    ((RwObject *)(d))->subType =                        \
        ((const RwObject *)(s))->subType;               \
    ((RwObject *)(d))->flags =                          \
        ((const RwObject *)(s))->flags;                 \
    ((RwObject *)(d))->privateFlags =                   \
        ((const RwObject *)(s))->privateFlags;          \
    ((RwObject *)(d))->parent =                         \
        NULL;                                           \
}                                                       \
MACRO_STOP

#define rwObjectInitialize(o, t, s)                     \
MACRO_START                                             \
{                                                       \
    ((RwObject *)(o))->type = (RwUInt8)(t);             \
    ((RwObject *)(o))->subType = (RwUInt8)(s);          \
    ((RwObject *)(o))->flags = 0;                       \
    ((RwObject *)(o))->privateFlags = 0;                \
    ((RwObject *)(o))->parent = NULL;                   \
}                                                       \
MACRO_STOP

/* Debug */
#define RwObjectGetType(o)                  (((const RwObject *)(o))->type)

#define rwObjectSetType(o, t)               (((RwObject *)(o))->type) = (RwUInt8)(t)

/* Sub type */
#define rwObjectGetSubType(o)               (((const RwObject *)(o))->subType)
#define rwObjectSetSubType(o, t)            (((RwObject *)(o))->subType) = (RwUInt8)(t)

/* Flags */
#define rwObjectGetFlags(o)                 (((const RwObject *)(o))->flags)
#define rwObjectSetFlags(o, f)              (((RwObject *)(o))->flags) = (RwUInt8)(f)
#define rwObjectTestFlags(o, f)             ((((const RwObject *)(o))->flags) & (RwUInt8)(f))

/* Private flags */
#define rwObjectGetPrivateFlags(c)          (((const RwObject *)(c))->privateFlags)
#define rwObjectSetPrivateFlags(c,f)        (((RwObject *)(c))->privateFlags) = (RwUInt8)(f)
#define rwObjectTestPrivateFlags(c,flag)    ((((const RwObject *)(c))->privateFlags) & (RwUInt8)(flag))

/* Hierarchy */
#define rwObjectGetParent(object)           (((const RwObject *)(object))->parent)
#define rwObjectSetParent(c,p)              (((RwObject *)(c))->parent) = (void *)(p)

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

/* RWPUBLICEND */

#endif /* RWTYPE_H */

