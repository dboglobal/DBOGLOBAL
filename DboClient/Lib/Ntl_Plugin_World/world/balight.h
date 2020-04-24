
#ifndef RWLIGHT_H
#define RWLIGHT_H

/* {doc} */
/****************************************************************************
 Includes
 */

#include <rwcore.h>
#include "basector.h"

/* RWPUBLIC */

/*
 * Lighting 3D objects.
 * Lights are used to illuminate atomics and worlds
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 */


/****************************************************************************
 Defines
 */

/* Binary Light */
typedef struct RpLightChunkInfo RpLightChunkInfo;
typedef struct RpLightChunkInfo _rpLight;

#if (!defined(DOXYGEN))
struct RpLightChunkInfo
{
        RwReal              radius; /**< radius */
        RwReal              red; /**< red */
        RwReal              green; /**< green */
        RwReal              blue; /**< blue */
        RwReal              minusCosAngle; /**< minusCosAngle */
        RwUInt32            typeAndFlags; /**< typeAndFlags */
};
#endif /* (!defined(DOXYGEN)) */

/* Type ID */
#define rpLIGHT 3

/* Beyond this the lights must be positioned */
#define rpLIGHTPOSITIONINGSTART 0x80

/**
 * \ingroup rplight
 * \ref RpLightType are
 * light sub types.  This type represents the different 
 * types of light source that can be created using the API function \ref RpLightCreate.  
 * Note that lights of types rpLIGHTPOINT, rpLIGHTSPOT and rpLIGHTSPOTSOFT have linear 
 * intensity fall-off with distance from the source, reducing to zero at the light's radius:*/
enum RpLightType
{
    rpNALIGHTTYPE = 0,

    /* These don't take part in the tie mechanism (no position) */
    rpLIGHTDIRECTIONAL, /**<Directional Light */
    rpLIGHTAMBIENT, /**<Ambient Light */

    /* These do take part in the tie mechanism (do have position) */
    rpLIGHTPOINT = rpLIGHTPOSITIONINGSTART, /**<Point Light */
    rpLIGHTSPOT, /**<Spot Light */
    rpLIGHTSPOTSOFT, /**<Soft Spot Light */
    rpLIGHTTYPEFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RpLightType RpLightType;

#define rpLIGHTMINCONEANGLE ((RwReal)0.0f)

#if (defined(XBOX_DRVMODEL_H))
#define rpLIGHTMAXCONEANGLE (rwPIOVER2)
#elif (defined(OPENGL_DRVMODEL_H))
#define rpLIGHTMAXCONEANGLE (rwPIOVER2)
#elif (defined(GCN_DRVMODEL_H))
#define rpLIGHTMAXCONEANGLE (rwPIOVER2)
#elif (defined(D3D8_DRVMODEL_H))
#define rpLIGHTMAXCONEANGLE (rwPIOVER2)
#elif (defined(D3D9_DRVMODEL_H))
#define rpLIGHTMAXCONEANGLE (rwPIOVER2)
#else
#define rpLIGHTMAXCONEANGLE (rwPI)
#endif

/*************/

/*** FLAGS ***/

/*************/

/**
 * \ingroup rplight
 * \ref RpLightFlag defines what geometry is influenced by the light. 
 * The bit-field RpLightFlag specifies the options available for controlling the scope 
 * of a light source (see API function \ref RpLightSetFlags):*/
enum RpLightFlag
{
    rpLIGHTLIGHTATOMICS = 0x01, /**<The light source illuminates all atomics in a world */
    rpLIGHTLIGHTWORLD = 0x02, /**<The light source illuminates all static geometry in a world */
    rpLIGHTFLAGFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RpLightFlag RpLightFlag;

/* rpLIGHTPRIVATENOCHROMA - turns on optimisations to do with
 *                          lights which are a shade of grey
 */
enum rpLightPrivateFlag
{
    rpLIGHTPRIVATENOCHROMA = 0x01,
    rpLIGHTPRIVATEFLAGFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum rpLightPrivateFlag rpLightPrivateFlag;

/*********************/

/* RWPUBLICEND */

/*** PRIVATE FLAGS ***/

/*********************/


/****************************************************************************
 Global Types
 */

typedef struct rpLightGlobals rpLightGlobals;
struct rpLightGlobals
{
    RwFreeList         *lightFreeList;
};

/* RWPUBLIC */

#define RWMINUSCOSFROMTAN(_minusCosAngle, _tanAngle)                  \
MACRO_START                                                           \
{                                                                     \
   const RwReal TanAngle2 = (_tanAngle) * (_tanAngle);                \
   const RwReal CosAngle2 = ((RwReal)1) / ( ((RwReal)1) + TanAngle2); \
   RwReal CosAngle;                                                   \
   rwSqrtMacro(&CosAngle, CosAngle2);                                 \
   (_minusCosAngle) = - CosAngle;                                     \
}                                                                     \
MACRO_STOP

#define RWTANFROMMINUSCOS(_tanAngle, _minusCosAngle)                    \
MACRO_START                                                             \
{                                                                       \
    const RwReal CosAngle2 = (_minusCosAngle) * (_minusCosAngle);       \
    const RwReal TanAngle2 = (((RwReal)1) - (CosAngle2)) / (CosAngle2); \
    rwSqrtMacro(&_tanAngle, TanAngle2);                                 \
}                                                                       \
MACRO_STOP


/**
 * \ingroup rplight
 * \struct RpLight 
 * Light object. This should be 
 * considered an opaque type. Use the RpLight API functions to access.
 */
typedef struct RpLight RpLight;

#if (!defined(DOXYGEN))
struct RpLight
{
        RwObjectHasFrame    object; /**< object */
        RwReal              radius; /**< radius */
        RwRGBAReal          color; /**< color */  /* Light color */
        RwReal              minusCosAngle; /**< minusCosAngle */  
        RwLinkList          WorldSectorsInLight; /**< WorldSectorsInLight */
        RwLLLink            inWorld; /**< inWorld */
        RwUInt16            lightFrame; /**< lightFrame */
        RwUInt16            pad;
};
#endif /* (!defined(DOXYGEN)) */

/**
 * \ingroup rplight
 * \ref RpLightCallBack 
 * represents the function called from \ref RpWorldForAllLights and 
 * \ref RpWorld SectorForAllLights for all lights in a given world or world 
 * sector. This function should return a pointer to the current light to 
 * indicate success. The callback may return NULL to terminate further 
 * callbacks on the world sector.
 *
 * \param  light  Pointer to the current light in the world 
 *                sector.
 * \param  data   Pointer to developer-defined data structure.
 *
 * \return Pointer to the current light.
 */
typedef RpLight    *(*RpLightCallBack) (RpLight * light, void *data);

typedef struct RpLightTie RpLightTie;

/**
 * \ingroup rplight
 * \struct RpLightTie
 *
 * RpLightTie is a linked list of lights inside world sectors. These are
 * created when frame hierarchies are updated as part of an \ref RwCameraBeginUpdate.
 * This is used for determining which lights influence a world sector and the atomics inside it.
 * Creation and destruction of RpLightTies is internal to the world plugin.
 */
struct RpLightTie
{
    /* Information for an atomic sector */
    RwLLLink            lightInWorldSector; /**< Lights IN this ATOMIC SECTOR */
    RpLight            *light;              /**< A pointer to a light */

    /* Information for a atomic */
    RwLLLink            WorldSectorInLight; /**< Atomic sectors HOLDING this Light */
    RpWorldSector      *sect;               /**< A pointer to a world sector */
};


/****************************************************************************
 <macro/inline functionality
 */

#define RpLightGetRadiusMacro(_light)                       \
    ((_light)->radius)

#define RpLightGetColorMacro(_light)                        \
    (&((_light)->color))

#define RpLightSetFrameMacro(_light, _frame)                \
    (rwObjectHasFrameSetFrame((_light), (_frame)), (_light))

#define RpLightGetFrameMacro(_light)                        \
    ((RwFrame *)rwObjectGetParent((_light)))

#define RpLightGetTypeMacro(_light)                         \
    ((RpLightType)rwObjectGetSubType((_light)))

#define RpLightSetFlagsMacro(_light, _flags)                \
    ((rwObjectSetFlags((_light), (_flags))), (_light))

#define RpLightGetFlagsMacro(_light)                        \
    (rwObjectGetFlags((_light)))


#if !(defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

#define RpLightGetRadius(_light)                            \
    RpLightGetRadiusMacro(_light)

#define RpLightGetColor(_light)                             \
    RpLightGetColorMacro(_light)

#define RpLightSetFrame(_light, _frame)                     \
    RpLightSetFrameMacro(_light, _frame)

#define RpLightGetFrame(_light)                             \
    RpLightGetFrameMacro(_light)

#define RpLightGetType(_light)                              \
    RpLightGetTypeMacro(_light)

#define RpLightSetFlags(_light, _flags)                     \
    RpLightSetFlagsMacro(_light, _flags)

#define RpLightGetFlags(_light)                             \
    RpLightGetFlagsMacro(_light)

#endif /* !(defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */


/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern              "C"
{
#endif                          /* __cplusplus */

/* RWPUBLICEND */

/* Starting and stoping this module */
extern void *_rpLightOpen(void *instance, RwInt32 offset, RwInt32 size);
extern void *_rpLightClose(void *instance, RwInt32 offset, RwInt32 size);

/* RWPUBLIC */

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))
extern RwReal RpLightGetRadius(const RpLight *light);
extern const RwRGBAReal *RpLightGetColor(const RpLight *light);
extern RpLight *RpLightSetFrame(RpLight *light, RwFrame *frame);
extern RwFrame *RpLightGetFrame(const RpLight *light);
extern RpLightType RpLightGetType(const RpLight *light);
extern RpLight *RpLightSetFlags(RpLight *light, RwUInt32 flags);
extern RwUInt32 RpLightGetFlags(const RpLight *light);
#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/* API Functions */
extern void RpLightSetFreeListCreateParams( RwInt32 blockSize, RwInt32 numBlocksToPrealloc );
extern RpLight *RpLightCreate(RwInt32 type);
extern RwBool RpLightDestroy(RpLight *light);
extern RpLight *RpLightSetRadius(RpLight *light, RwReal radius);
extern RpLight *RpLightSetColor(RpLight *light, const RwRGBAReal *color);
extern RwReal RpLightGetConeAngle(const RpLight *light);
extern RpLight *RpLightSetConeAngle(RpLight * ight, RwReal angle);
extern RwUInt32 RpLightStreamGetSize(const RpLight *light);
extern RpLight *RpLightStreamRead(RwStream *stream);
extern const RpLight *RpLightStreamWrite(const RpLight *light,
                                         RwStream *stream);
extern RpLightChunkInfo *_rpLightChunkInfoRead(RwStream *stream,
                                               RpLightChunkInfo *lightChunkInfo,
                                               RwInt32 *bytesRead);

/* Attaching toolkits */
extern RwInt32 RpLightRegisterPlugin(RwInt32 size,
                                     RwUInt32 pluginID,
                                     RwPluginObjectConstructor constructCB,
                                     RwPluginObjectDestructor destructCB,
                                     RwPluginObjectCopy copyCB);
extern RwInt32 RpLightRegisterPluginStream(RwUInt32 pluginID,
                                           RwPluginDataChunkReadCallBack readCB,
                                           RwPluginDataChunkWriteCallBack writeCB,
                                           RwPluginDataChunkGetSizeCallBack getSizeCB);
extern RwInt32 RpLightSetStreamAlwaysCallBack(RwUInt32 pluginID,
                                              RwPluginDataChunkAlwaysCallBack alwaysCB);
extern RwInt32 RpLightGetPluginOffset(RwUInt32 pluginID);
extern RwBool RpLightValidatePlugins(const RpLight * light);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#define RpLightChunkInfoRead(stream, lightChunkInfo, bytesRead) \
       _rpLightChunkInfoRead(stream, lightChunkInfo, bytesRead)

/* RWPUBLICEND */

#endif                          /* RWLIGHT_H */

