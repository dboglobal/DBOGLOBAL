/**
 * \ingroup rpatomic
 * \page rpatomicoverview RpAtomic Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h, rpworld.h
 * \li \b Libraries: rwcore, rpworld
 * \li \b Plugin \b attachments: \ref RpWorldPluginAttach
 *
 * \subsection atomicoverview Overview
 *
 * Atomics are container objects for Geometry and Frame objects.
 * Complex models are often built up of sections grouped into a
 * hierarchy and Atomics provide the necessary linkage with Frame
 * \ref RwFrame objects to enable this. When used in this way, related
 * Atomics are usually grouped together by adding them to a Clump
 * \ref RpClump object for convenience, although this is not mandatory.
 * Atomics contain a reference to a Geometry \ref RpGeometry , which holds
 * the actual model data. In addition, a reference to a Frame can be added;
 * indeed, this is required if the Atomic is to be rendered as it cannot
 * be positioned without one.
 *
 * Further information is available in the
 *  Dynamic Models chapter of the User Guide.
 *
 */

/**
 * \ingroup rpclump
 * \page rpclumpoverview RpClump Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h, rpworld.h
 * \li \b Libraries: rwcore, rpworld
 * \li \b Plugin \b attachments: \ref RpWorldPluginAttach
 *
 * \subsection clumpoverview Overview
 *
 * Clumps are containers for Atomics \ref RpAtomic and can also be linked
 * to their own Frame \ref RwFrame.
 * Clumps are intended to group related Atomics together.
 * The Atomics are stored by reference, so an Atomic can be stored in
 * more than one Clump if necessary.
 * A Frame object can be added to a Clump.
 * If the Frame is then linked to others attached to the Atomics contained
 * within the Clump, developers can then move the entire group of Atomics
 * by simply repositioning the Clump as changes to the Clump's Frame will
 * get propagated down through the hierarchy.
 * Geometry lighting by dynamic lights \ref RpLight can be enabled
 * and disabled at the Atomic level.
 * Further information is available in the Dynamic Models chapter of the
 * User Guide.
 */

/**
 * \ingroup rpinterpolator
 * \page rpinterpolatoroverview RpInterpolator Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h, rpworld.h
 * \li \b Libraries: rwcore, rpworld
 * \li \b Plugin \b attachments: \ref RpWorldPluginAttach
 *
 * \subsection interpolatoroverview Overview
 *
 * An Interpolator object contains an instance of dynamic geometry
 * data, interpolated from keyframes stored in Morph Targets. It is
 * this data which ends up being rendered when working with
 * keyframe-animated models.
*/


/****************************************************************************
 Includes
 */


#include <stdlib.h>
#include <string.h>

#include <rwcore.h>

#include "bamateri.h"
#include "bageomet.h"
#include "balight.h"
#include "baworld.h"
#include "bapipew.h"
#include "baclump.h"
#include "native.h"
#include "baworobj.h"

/****************************************************************************
 Local Types
 */

typedef struct RpClumpCameraExt RpClumpCameraExt;
struct RpClumpCameraExt
{
    RpClump            *clump;
    RwLLLink            inClumpLink;
};

typedef struct RpClumpLightExt RpClumpLightExt;
struct RpClumpLightExt
{
    RpClump            *clump;
    RwLLLink            inClumpLink;
};

typedef struct rpGeometryList rpGeometryList;
struct rpGeometryList
{
    RpGeometry        **geometries;
    RwInt32             numGeoms;
};

typedef struct RpClumpCloneStatus RpClumpCloneStatus;
struct RpClumpCloneStatus
{
    RpClump            *oldClump;
    RpClump            *newClump;
    RwBool              success;
};

typedef struct RpClumpStreamWriteStatus RpClumpStreamWriteStatus;
struct RpClumpStreamWriteStatus
{
    RwStream           *stream;
    rwFrameList         fl;
    rpGeometryList      gl;
    RwBool              success;
};

/* Binary Representations */
typedef struct rpAtomicBinary _rpAtomicBinary;
struct rpAtomicBinary
{
    RwInt32             frameIndex;
    RwInt32             geomIndex;
    RwInt32             flags;
    RwInt32             unused;
};

typedef struct rpAtomicLightData _rpAtomicLightData;
struct rpAtomicLightData
{
    RwResEntry          *repEntry;
    RwSurfaceProperties *surface;
    RwMatrix            invMat;
    RwReal              invScale;
};

/****************************************************************************
 Local (Static) Prototypes
 */

/****************************************************************************
 Local Defines
 */
#define CLUMPCAMERAEXTFROMCAMERA(camera) \
   ((RpClumpCameraExt *)(((RwUInt8 *)(camera)) + _rpClumpCameraExtOffset))
#define CLUMPCAMERAEXTFROMCONSTCAMERA(camera) \
   ((const RpClumpCameraExt *)(((const RwUInt8 *)(camera)) + _rpClumpCameraExtOffset))

#define CLUMPLIGHTEXTFROMLIGHT(light) \
   ((RpClumpLightExt *)(((RwUInt8 *)(light)) + _rpClumpLightExtOffset))
#define CLUMPLIGHTEXTFROMCONSTLIGHT(light) \
   ((const RpClumpLightExt *)(((const RwUInt8 *)(light)) + _rpClumpLightExtOffset))

#define RWCLUMPGLOBAL(var)                              \
   (RWPLUGINOFFSET(rpClumpGlobals,                      \
                   RwEngineInstance,                    \
                   clumpModule.globalsOffset)->var)

/****************************************************************************
 Globals (across program)
 */

/****************************************************************************
 Local (static) Globals
 */

static RwPluginRegistry atomicTKList =
    { sizeof(RpAtomic),
      sizeof(RpAtomic),
      0,
      0,
      (RwPluginRegEntry *)NULL,
      (RwPluginRegEntry *)NULL };

static RwPluginRegistry clumpTKList =
    { sizeof(RpClump),
      sizeof(RpClump),
      0,
      0,
      (RwPluginRegEntry *)NULL,
      (RwPluginRegEntry *)NULL };

static RwInt32  _rpClumpCameraExtOffset = 0;
static RwInt32  _rpClumpLightExtOffset = 0;

static RwModuleInfo clumpModule;

/****************************************************************************
 Local (Static) Functions
 */

/****************************************************************************
 ClumpTidyDestroyClump

 Destroy clumps on library exit for people too lazy to do it themselves

 On entry   : Object to destroy (clump), user data
 On exit    : FALSE on failure
 */

static void
ClumpTidyDestroyClump( void *object,
                       void *data __RWUNUSED__ )
{
    RWFUNCTION(RWSTRING("ClumpTidyDestroyClump"));
    RWASSERT(object);

    RpClumpDestroy((RpClump *) object);

    RWRETURNVOID();
}

/****************************************************************************
 ClumpTidyDestroyAtomic

 Destroy atomics on library exit for people too lazy to do it themselves

 On entry   : Object to destroy (atomic), user data
 On exit    : FALSE on failure
 */

static void
ClumpTidyDestroyAtomic( void *object,
                        void *data __RWUNUSED__ )
{
    RWFUNCTION(RWSTRING("ClumpTidyDestroyAtomic"));
    RWASSERT(object);

    RpAtomicDestroy((RpAtomic *) object);

    RWRETURNVOID();
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                     Rights support

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

static RwUInt32 lastSeenRightsPluginId;
static RwUInt32 lastSeenExtraData;

RwStream*
_rpReadAtomicRights(RwStream *s,
                    RwInt32 len,
                    void *obj __RWUNUSED__,
                    RwInt32 off __RWUNUSED__,
                    RwInt32 size __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("_rpReadAtomicRights"));

    RWASSERT((len == 4) || (len == 8));

    if (!RwStreamReadInt32(s, (RwInt32*)&(lastSeenRightsPluginId),
                           sizeof(RwInt32)))
    {
        RWRETURN((RwStream *) NULL);
    }
    if (len == 8)
    {
        if (!RwStreamReadInt32(s, (RwInt32*)&(lastSeenExtraData),
                               sizeof(RwInt32)))
        {
            RWRETURN((RwStream *) NULL);
        }
    }
    RWRETURN(s);
}

RwStream*
_rpWriteAtomicRights(RwStream *s,
                     RwInt32 len __RWUNUSED__,
                     const void *obj,
                     RwInt32 off __RWUNUSED__,
                     RwInt32 size __RWUNUSED__)
{
    const RpAtomic *at;

    RWFUNCTION(RWSTRING("_rpWriteAtomicRights"));
    RWASSERT(obj);

    at = (const RpAtomic*)obj;
    RWASSERT(at->pipeline);
    RWASSERT(at->pipeline->pluginId);

    if (!RwStreamWriteInt32(s, (RwInt32*)&(at->pipeline->pluginId),
                            sizeof(RwInt32)))
    {
        RWRETURN((RwStream *) NULL);
    }
    if (!RwStreamWriteInt32(s, (RwInt32*)&(at->pipeline->pluginData),
                            sizeof(RwInt32)))
    {
        RWRETURN((RwStream *) NULL);
    }

    RWRETURN(s);
}

RwInt32
_rpSizeAtomicRights(const void *obj,
                    RwInt32 off __RWUNUSED__,
                    RwInt32 size __RWUNUSED__)
{
    const RpAtomic *at;

    RWFUNCTION(RWSTRING("_rpSizeAtomicRights"));
    RWASSERT(obj);

    at = (const RpAtomic*)obj;
    if ((at->pipeline) && (at->pipeline->pluginId))
    {
        RWRETURN(sizeof(RwUInt32) * 2);
    }
    else
    {
        RWRETURN(0);
    }
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                     Finding the num of atomics

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 CountAtomic

 Counts atomics

 On entry   : atomic
            : user data (counter)
 On exit    : atomic pointer on success
 */

static RpAtomic    *
CountAtomic(RpAtomic * atomic, void *pData)
{
    RWFUNCTION(RWSTRING("CountAtomic"));
    RWASSERT(atomic);
    RWASSERT(pData);
    RWASSERTISTYPE(atomic, rpATOMIC);

    (*(RwInt32 *) pData)++;

    RWRETURN(atomic);
}


/****************************************************************************
 CountLight

 Counts lights

 On entry   : light
            : user data (counter)
 On exit    : light pointer on success
 */

static RpLight    *
CountLight(RpLight * light, void *pData)
{
    RWFUNCTION(RWSTRING("CountLight"));
    RWASSERT(light);
    RWASSERT(pData);
    RWASSERTISTYPE(light, rpLIGHT);

    (*(RwInt32 *) pData)++;

    RWRETURN(light);
}


/****************************************************************************
 CountCamera

 Counts cameras

 On entry   : camera
            : user data (counter)
 On exit    : camera pointer on success
 */

static RwCamera    *
CountCamera(RwCamera * camera, void *pData)
{
    RWFUNCTION(RWSTRING("CountCamera"));
    RWASSERT(camera);
    RWASSERT(pData);
    RWASSERTISTYPE(camera, rwCAMERA);

    (*(RwInt32 *) pData)++;

    RWRETURN(camera);
}

/****************************************************************************
 AtomicSync

 On entry   : Atomic
 On exit    :
 */

static RwObjectHasFrame *
AtomicSync(RwObjectHasFrame * object)
{
    RpAtomic           *atomic = (RpAtomic *) object;
    RpInterpolator     *interpolator;

    RWFUNCTION(RWSTRING("AtomicSync"));
    RWASSERT(object);
    RWASSERTISTYPE(object, rpATOMIC);

    interpolator = &atomic->interpolator;
    if (interpolator->flags & rpINTERPOLATORDIRTYSPHERE)
    {
        _rpAtomicResyncInterpolatedSphere(atomic);
    }

    /* This doesn't do much.
     * The work is done in the function that the world adds to this chain */
    rwObjectSetPrivateFlags(object,
                            rwObjectGetPrivateFlags(object) |
                            rpATOMICPRIVATEWORLDBOUNDDIRTY);

    RWRETURN(object);
}

/****************************************************************************
 AtomicDefaultRenderCallBack

 Executes the object pipeline on the atomic.

 On entry       : RpAtomic
                : message
 On exit        : atomic pointer if OK
 */

RpAtomic *
AtomicDefaultRenderCallBack(RpAtomic *atomic)
{
    RxPipeline  *pipeline = atomic->pipeline;

    RWFUNCTION(RWSTRING("AtomicDefaultRenderCallBack"));
    RWASSERT(atomic);

    if (pipeline == NULL)
    {
        pipeline = RXPIPELINEGLOBAL(currentAtomicPipeline);
    }

    if (RxPipelineExecute(pipeline, (void *) atomic, TRUE))
    {
        RWRETURN(atomic);
    }

    RWRETURN((RpAtomic *)NULL);
}

/****************************************************************************
 ClumpAtomicCloneAndAttach

 On entry   : atomic
            : user data pointer
 On exit    : atomic pointer on success
 */

static RpAtomic    *
ClumpAtomicCloneAndAttach(RpAtomic * atomic, void *pData)
{
    RpClumpCloneStatus *cloneStatus;
    RpAtomic           *newAtomic;
    RwFrame            *frame;

    RWFUNCTION(RWSTRING("ClumpAtomicCloneAndAttach"));
    RWASSERT(atomic);
    RWASSERTISTYPE(atomic, rpATOMIC);
    RWASSERT(pData);

    cloneStatus = (RpClumpCloneStatus *) pData;
    newAtomic = RpAtomicClone(atomic);

    if (!newAtomic)
    {
        /* Couldn't clone! */
        cloneStatus->success = FALSE;
        RWRETURN((RpAtomic *)NULL);
    }

    /* Copy over the atomics NEW frame - we shoved it in the frames root */
    frame = (RwFrame *) rwObjectGetParent(atomic);
    if (frame)
    {
        RpAtomicSetFrame(newAtomic, frame->root);
    }
    else
    {
        RpAtomicDestroy(newAtomic);
        RWERROR((E_RP_WORLD_ATOMICNOFRAME));
        RWRETURN((RpAtomic *)NULL);
    }

    RpClumpAddAtomic(cloneStatus->newClump, newAtomic);

    RWRETURN(atomic);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                         Lists of Geometry

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 GeometryListDeinitialize

 On entry   : Geometry list
 On exit    : Geometry list on success
 */

static rpGeometryList     *
GeometryListDeinitialize(rpGeometryList * geomList)
{
    RwInt32             i;

    RWFUNCTION(RWSTRING("GeometryListDeinitialize"));
    RWASSERT(geomList);

    /* remove the read reference to each geometry */
    for (i = 0; i < geomList->numGeoms; i++)
    {
        RpGeometryDestroy(geomList->geometries[i]);
    }

    if (geomList->geometries)
    {
        RwFree(geomList->geometries);
        geomList->geometries = (RpGeometry **)NULL;
    }

    RWRETURN(geomList);
}


/****************************************************************************
 GeometryListFindGeometry

 On entry   : Array of Geometry
            : Geometry to look for
            : Index
 On exit    : TRUE if found
 */

static RwBool
GeometryListFindGeometry(const rpGeometryList * geomList,
                           const RpGeometry * geom, RwInt32 * npIndex)
{
    RwInt32             i;

    RWFUNCTION(RWSTRING("GeometryListFindGeometry"));
    RWASSERT(geomList);
    RWASSERT(geom);

    for (i = 0; i < geomList->numGeoms; i++)
    {
        if (geomList->geometries[i] == geom)
        {
            if (npIndex)
                (*npIndex) = i;
            RWRETURN(TRUE);
        }
    }

    RWRETURN(FALSE);
}

/****************************************************************************
 GeometryListStreamGetSize

 On entry   :
 On exit    : Size of Binary FrameList
 */

static RwUInt32
GeometryListStreamGetSize(const rpGeometryList * geomList)
{
    RwUInt32            size;
    RwInt32             i;

    RWFUNCTION(RWSTRING("GeometryListStreamGetSize"));
    RWASSERT(geomList);

    /* Size of geometry list itself */
    size = sizeof(RwInt32) + rwCHUNKHEADERSIZE;

    /* Add the size of the geometry chunks for each geometry in the list */
    for (i = 0; i < geomList->numGeoms; i++)
    {
        RpGeometry         *geom = geomList->geometries[i];

        /* Add the size of the included chunk */
        size += RpGeometryStreamGetSize(geom) + rwCHUNKHEADERSIZE;
    }

    /* Return the size */
    RWRETURN(size);
}

static rpGeometryList     *
GeometryListInitialize(rpGeometryList * geomList, RpClump * clump)
{
    RwInt32             numGeoms;
    RpGeometry        **fppCur;
    RwLLLink           *cur, *end;

    RWFUNCTION(RWSTRING("GeometryListInitialize"));
    RWASSERT(geomList);
    RWASSERT(clump);
    RWASSERTISTYPE(clump, rpCLUMP);

    numGeoms = RpClumpGetNumAtomics(clump);
    if (numGeoms > 0)
    {
        geomList->geometries =
            (RpGeometry **) RwMalloc(sizeof(RpGeometry *) * numGeoms,
                                     rwID_GEOMETRY | rwMEMHINTDUR_EVENT);
        if (!geomList->geometries)
        {
            RWERROR((E_RW_NOMEM, (sizeof(RpGeometry *) * numGeoms)));
            RWRETURN((rpGeometryList *)NULL);
        }
    }
    else
    {
        geomList->geometries = (RpGeometry **)NULL;
    }
    geomList->numGeoms = 0;

    /* fill it with _unique_ geometries */
    fppCur = geomList->geometries;
    cur = rwLinkListGetFirstLLLink(&clump->atomicList);
    end = rwLinkListGetTerminator(&clump->atomicList);
    while (cur != end)
    {
        RpAtomic           *apAtom =

            rwLLLinkGetData(cur, RpAtomic, inClumpLink);
        RpGeometry         *geom = RpAtomicGetGeometry(apAtom);

        /* is it already in the list? (should really mark atomic) */
        if (!GeometryListFindGeometry(geomList, geom, (RwInt32 *)NULL))
        {
            RpGeometryAddRef(geom);
            *fppCur++ = geom;
            geomList->numGeoms++;
        }

        cur = rwLLLinkGetNext(cur);
    }

    /* Done */
    RWRETURN(geomList);
}

/****************************************************************************
 GeometryListStreamWrite

 On entry   : Stream to write to
 On exit    :
 */
static const rpGeometryList *
GeometryListStreamWrite(const rpGeometryList * geomList, RwStream * stream)
{
    RwInt32             gl;
    RwInt32             i;

    RWFUNCTION(RWSTRING("GeometryListStreamWrite"));
    RWASSERT(geomList);
    RWASSERT(stream);

    if (!RwStreamWriteChunkHeader(stream, rwID_GEOMETRYLIST,
                                  GeometryListStreamGetSize(geomList)))
    {
        RWRETURN((const rpGeometryList *)NULL);
    }

    /* wrap structured data */
    if (!RwStreamWriteChunkHeader(stream, rwID_STRUCT, sizeof(RwInt32)))
    {
        RWRETURN((const rpGeometryList *)NULL);
    }

    /* Fill it */
    gl = geomList->numGeoms;

    /* Convert it */
    (void)RwMemLittleEndian32(&gl, sizeof(gl));

    /* Write it */
    if (!RwStreamWrite(stream, &gl, sizeof(gl)))
    {
        RWRETURN((const rpGeometryList *)NULL);
    }

    /* Now write off all the geometries */
    for (i = 0; i < geomList->numGeoms; i++)
    {
        RpGeometry         *geom = geomList->geometries[i];

        if (!RpGeometryStreamWrite(geom, stream))
        {
            RWRETURN((const rpGeometryList *)NULL);
        }
    }

    RWRETURN(geomList);
}


/****************************************************************************
 GeometryListStreamRead

 On entry   : Stream to read from
 On exit    : FrameList created
 */
static rpGeometryList     *
GeometryListStreamRead(RwStream * stream, rpGeometryList * geomList)
{
    RwInt32             gl;
    RwInt32             i;
    RwUInt32            size;
    RwUInt32            version;

    RWFUNCTION(RWSTRING("GeometryListStreamRead"));
    RWASSERT(stream);

    if (!RwStreamFindChunk(stream, rwID_STRUCT, &size, &version))
    {
        RWRETURN((rpGeometryList *)NULL);
    }

    if ((version >= rwLIBRARYBASEVERSION) && (version <= rwLIBRARYCURRENTVERSION))
    {
        /* Read it */
        if (RwStreamRead(stream, &gl, sizeof(gl)) != sizeof(gl))
        {
            RWRETURN((rpGeometryList *)NULL);
        }

        /* Convert it */
        (void)RwMemNative32(&gl, sizeof(gl));

        /* Set up the geometry list */
        geomList->numGeoms = 0;

        if (gl > 0)
        {
            geomList->geometries = (RpGeometry **) RwMalloc(sizeof(RpGeometry *) * gl, rwID_GEOMETRY | rwMEMHINTDUR_EVENT);
            if (!geomList->geometries)
            {
                RWERROR((E_RW_NOMEM, (sizeof(RpGeometry *) * gl)));
                RWRETURN((rpGeometryList *)NULL);
            }
        }
        else
        {
            geomList->geometries = (RpGeometry **)NULL;
        }

        for (i = 0; i < gl; i++)
        {
            /* Read the geometry */
            if (!RwStreamFindChunk(stream, rwID_GEOMETRY, (RwUInt32 *)NULL, &version))
            {
                GeometryListDeinitialize(geomList);
                RWRETURN((rpGeometryList *)NULL);
            }

            if ((version >= rwLIBRARYBASEVERSION) && (version <= rwLIBRARYCURRENTVERSION))
            {
                if (!(geomList->geometries[i] = RpGeometryStreamRead(stream)))
                {
                    GeometryListDeinitialize(geomList);
                    RWRETURN((rpGeometryList *)NULL);
                }
                /* Increment the number of geometries in the list */
                geomList->numGeoms++;
            }
            else
            {
                GeometryListDeinitialize(geomList);
                RWRETURN((rpGeometryList *)NULL);
            }
        }

        /* check the correct number of geometries were streamed */
        RWASSERT(geomList->numGeoms == gl);
    }
    else
    {
        RWERROR((E_RW_BADVERSION));
        RWRETURN((rpGeometryList *)NULL);
    }

    RWRETURN(geomList);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                          Clump lights and cameras

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/******************************************************************************
 ClumpInitCameraExt

 On entry   : Object, offset in object, size in object
 On exit    : Object pointer on success
 */

static void        *
ClumpInitCameraExt(void *object,
                   RwInt32 offsetInObject __RWUNUSED__,
                   RwInt32 sizeInObject __RWUNUSED__)
{
    RpClumpCameraExt *cameraExt = CLUMPCAMERAEXTFROMCAMERA(object);

    RWFUNCTION(RWSTRING("ClumpInitCameraExt"));
    RWASSERT(object);

    rwLLLinkInitialize(&cameraExt->inClumpLink);
    cameraExt->clump = (RpClump *)NULL;

    RWRETURN(object);
}

/******************************************************************************
 ClumpDeInitCameraExt

 On entry   : Object, offset in object, size in object
 On exit    : Object pointer on success
 */

static void        *
ClumpDeInitCameraExt(void *object,
                     RwInt32 offsetInObject __RWUNUSED__,
                     RwInt32 sizeInObject __RWUNUSED__)
{
    /* This predication avoids unnecessary compile warnings */
#if (defined(RWDEBUG))
    RpClumpCameraExt   *cameraExt = CLUMPCAMERAEXTFROMCAMERA(object);
#endif /* (defined(RWDEBUG)) */

    RWFUNCTION(RWSTRING("ClumpDeInitCameraExt"));
    RWASSERT(object);

    /* Nothing to do, except assert that the camera being destroyed
     * isn't in a clump. */
    RWASSERT(!rwLLLinkAttached(&cameraExt->inClumpLink));
    RWASSERT(!(cameraExt->clump));

    RWRETURN(object);
}


/******************************************************************************
 ClumpInitLightExt

 On entry   : Object, offset in object, size in object
 On exit    : Object pointer on success
 */

static void        *
ClumpInitLightExt(void *object,
                   RwInt32 offsetInObject __RWUNUSED__,
                   RwInt32 sizeInObject __RWUNUSED__)
{
    RpClumpLightExt *lightExt = CLUMPLIGHTEXTFROMLIGHT(object);

    RWFUNCTION(RWSTRING("ClumpInitLightExt"));
    RWASSERT(object);

    rwLLLinkInitialize(&lightExt->inClumpLink);
    lightExt->clump = (RpClump *)NULL;

    RWRETURN(object);
}

/******************************************************************************
 ClumpDeInitLightExt

 On entry   : Object, offset in object, size in object
 On exit    : Object pointer on success
 */

static void        *
ClumpDeInitLightExt(void *object,
                     RwInt32 offsetInObject __RWUNUSED__,
                     RwInt32 sizeInObject __RWUNUSED__)
{
    /* This predication avoids unnecessary compile warnings */
#if (defined(RWDEBUG))
    RpClumpLightExt *lightExt = CLUMPLIGHTEXTFROMLIGHT(object);
#endif /* (defined(RWDEBUG)) */

    RWFUNCTION(RWSTRING("ClumpDeInitLightExt"));
    RWASSERT(object);

    /* Nothing to do, except assert that the light being destroyed
     * isn't in a clump. */
    RWASSERT(!rwLLLinkAttached(&(lightExt)->inClumpLink));
    RWASSERT(!(lightExt->clump));

    RWRETURN(object);
}

/****************************************************************************
 DestroyClumpLight

 On entry   : Light
            : User data pointer
 On exit    : Light pointer on success
 */

static RpLight    *
DestroyClumpLight( RpLight *light,
                   void *data __RWUNUSED__ )
{
    const RpClumpLightExt *lightExt = CLUMPLIGHTEXTFROMCONSTLIGHT(light);

    RWFUNCTION(RWSTRING("DestroyClumpLight"));
    RWASSERT(light);
    RWASSERTISTYPE(light, rpLIGHT);

    /* And the light */
    RpClumpRemoveLight(lightExt->clump, light);
    RpLightDestroy(light);

    RWRETURN(light);
}

/****************************************************************************
 DestroyClumpCamera

 On entry   : Camera
            : User data pointer
 On exit    : Camera pointer on success
 */

static RwCamera    *
DestroyClumpCamera( RwCamera * camera,
                    void *data __RWUNUSED__ )
{
    const RpClumpCameraExt *cameraExt = CLUMPCAMERAEXTFROMCONSTCAMERA(camera);

    RWFUNCTION(RWSTRING("DestroyClumpCamera"));
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);

    /* And the camera */
    RpClumpRemoveCamera(cameraExt->clump, camera);
    RwCameraDestroy(camera);

    RWRETURN(camera);
}

/****************************************************************************
 ClumpLightStreamWrite

 On entry   : Light
            : User data pointer
 On exit    : Light pointer on success
 */

static RpLight    *
ClumpLightStreamWrite(RpLight * light, void *pData)
{
    RpClumpStreamWriteStatus *status = (RpClumpStreamWriteStatus *) pData;
    RwInt32                   frameIndex;

    RWFUNCTION(RWSTRING("ClumpLightStreamWrite"));
    RWASSERT(light);
    RWASSERTISTYPE(light, rpLIGHT);
    RWASSERT(status);
    RWASSERT(status->fl.numFrames);
    RWASSERT(RpLightGetFrame(light));

    /* Write frame index, then the light */
    if (   !rwFrameListFindFrame(&status->fl, RpLightGetFrame(light), &frameIndex)
        || !RwStreamWriteChunkHeader(status->stream, rwID_STRUCT, sizeof(RwInt32))
        || !RwStreamWriteInt32(status->stream, &frameIndex, sizeof(frameIndex))
        || !RpLightStreamWrite(light, status->stream) )
    {
        status->success = FALSE;
        RWRETURN((RpLight *)NULL);
    }

    RWRETURN(light);
}

/****************************************************************************
 ClumpCameraStreamWrite

 On entry   : Camera
            : User data pointer
 On exit    : Camera pointer on success
 */

static RwCamera    *
ClumpCameraStreamWrite(RwCamera * camera, void *pData)
{
    RpClumpStreamWriteStatus *status = (RpClumpStreamWriteStatus *) pData;
    RwInt32                   frameIndex;

    RWFUNCTION(RWSTRING("ClumpCameraStreamWrite"));
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);
    RWASSERT(status);
    RWASSERT(status->stream);
    RWASSERT(status->fl.numFrames);
    RWASSERT(RwCameraGetFrame(camera));

    /* Write frame index, then camera */
    if (   !rwFrameListFindFrame(&status->fl, RwCameraGetFrame(camera), &frameIndex)
        || !RwStreamWriteChunkHeader(status->stream, rwID_STRUCT, sizeof(RwInt32))
        || !RwStreamWriteInt32(status->stream, &frameIndex, sizeof(frameIndex))
        || !RwCameraStreamWrite(camera, status->stream) )
    {
        status->success = FALSE;
        RWRETURN((RwCamera *)NULL);
    }

    RWRETURN(camera);
}

/****************************************************************************
 ClumpLightAddSize

 On entry   : Light
            : User data pointer
 On exit    : Light pointer on success
 */
static RpLight    *
ClumpLightAddSize(RpLight *light, void *pData)
{
    RwUInt32           *size = (RwUInt32 *) pData;

    RWFUNCTION(RWSTRING("ClumpLightAddSize"));
    RWASSERT(light);
    RWASSERTISTYPE(light, rpLIGHT);
    RWASSERT(size);

    /* Frame index chunk */
    (*size) += rwCHUNKHEADERSIZE + sizeof(RwInt32);

    /* Light chunk */
    (*size) += rwCHUNKHEADERSIZE + RpLightStreamGetSize(light);

    RWRETURN(light);
}

/****************************************************************************
 ClumpCameraAddSize

 On entry   : Camera
            : User data pointer
 On exit    : Camera pointer on success
 */
static RwCamera    *
ClumpCameraAddSize(RwCamera *camera, void *pData)
{
    RwUInt32           *size = (RwUInt32 *) pData;

    RWFUNCTION(RWSTRING("ClumpCameraAddSize"));
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);
    RWASSERT(size);

    /* Frame index chunk */
    (*size) += rwCHUNKHEADERSIZE + sizeof(RwInt32);

    /* Camera chunk */
    (*size) += rwCHUNKHEADERSIZE + RwCameraStreamGetSize(camera);

    RWRETURN(camera);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                                  Clumps

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 ClumpCallBack

 On entry   : Clump, user data
 On exit    : clump pointer on success
 */

static RpClump     *
ClumpCallBack( RpClump * clump,
               void * data __RWUNUSED__ )
{
    RWFUNCTION(RWSTRING("ClumpCallBack"));
    RWASSERT(clump);

    RWRETURN(clump);
}

/****************************************************************************
 DestroyClumpAtomic

 On entry   : Atomic
            : User data pointer
 On exit    : Atomic pointer on success
 */

static RpAtomic    *
DestroyClumpAtomic( RpAtomic * atomic,
                    void * pData __RWUNUSED__ )
{
    RWFUNCTION(RWSTRING("DestroyClumpAtomic"));
    RWASSERT(atomic);
    RWASSERTISTYPE(atomic, rpATOMIC);

    /* And the atomic */
    RpAtomicDestroy(atomic);

    RWRETURN(atomic);
}

/****************************************************************************
 *
 * Binary Format Functions
 */

static              RwUInt32
AtomicStreamGetSizeActual(RpAtomic * atomic __RWUNUSED__)
{
    RwUInt32            size;

    RWFUNCTION(RWSTRING("AtomicStreamGetSizeActual"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(atomic);
    RWASSERTISTYPE(atomic, rpATOMIC);

    /* Get the size of the atomic (without extension chunks) */
    size = sizeof(_rpAtomicBinary);

    RWRETURN(size);
}

static RpAtomic    *
ClumpAtomicAddSize(RpAtomic * atomic, void *pData)
{
    RwUInt32           *size = (RwUInt32 *) pData;

    RWFUNCTION(RWSTRING("ClumpAtomicAddSize"));
    RWASSERT(atomic);
    RWASSERTISTYPE(atomic, rpATOMIC);
    RWASSERT(size);

    /*
     * We need to add the total size of the streamed atomic, including
     * the rwID_ATOMIC chunk header.
     */
    (*size) += rwCHUNKHEADERSIZE;

    (*size) += ( AtomicStreamGetSizeActual(atomic) +
                 rwCHUNKHEADERSIZE);

    (*size) += ( rwPluginRegistryGetSize(&atomicTKList, atomic) +
        rwCHUNKHEADERSIZE );

    RWRETURN(atomic);
}

static RwUInt32
ClumpAtomicStreamGetSize(RpAtomic * atomic)
{
    RwUInt32           size = 0;

    RWFUNCTION(RWSTRING("ClumpAtomicStreamGetSize"));
    RWASSERT(atomic);
    RWASSERTISTYPE(atomic, rpATOMIC);

    (size) += ( AtomicStreamGetSizeActual(atomic) +
                rwCHUNKHEADERSIZE );

    (size) += ( rwPluginRegistryGetSize(&atomicTKList, atomic) +
                rwCHUNKHEADERSIZE );

    RWRETURN(size);
}

/****************************************************************************
 ClumpAtomicStreamWrite

 On entry   : Atomic
            : User data pointer
 On exit    : Atomic pointer on success
 */

static RpAtomic    *
ClumpAtomicStreamWrite(RpAtomic * atomic, void *pData)
{
    RpClumpStreamWriteStatus *status = (RpClumpStreamWriteStatus *) pData;

    _rpAtomicBinary        a;

    RWFUNCTION(RWSTRING("ClumpAtomicStreamWrite"));
    RWASSERT(atomic);
    RWASSERTISTYPE(atomic, rpATOMIC);
    RWASSERT(status);

    if (status->gl.numGeoms == 0)
    {
        /* wrap structured data */
        if (!RwStreamWriteChunkHeader(status->stream, rwID_ATOMIC,
                                      RpAtomicStreamGetSize(atomic)))
        {
            status->success = FALSE;
            RWRETURN((RpAtomic *)NULL);
        }
    }
    else
    {
        /* wrap structured data */
        if (!RwStreamWriteChunkHeader(status->stream, rwID_ATOMIC,
                                      ClumpAtomicStreamGetSize(atomic)))
        {
            status->success = FALSE;
            RWRETURN((RpAtomic *)NULL);
        }
    }

    /* wrap structured data */
    if (!RwStreamWriteChunkHeader(status->stream, rwID_STRUCT, sizeof(a)))
    {
        RWRETURN((RpAtomic *)NULL);
    }

    /* Fill it */
    a.flags = RpAtomicGetFlags(atomic);
    a.unused = 0;              /* Make it known */

    /* only if we have frame list */
    if (status->fl.numFrames)
    {
        if (!rwFrameListFindFrame
            (&status->fl, (RwFrame *) rwObjectGetParent(atomic),
             &a.frameIndex))
        {
            status->success = FALSE;
            RWRETURN((RpAtomic *)NULL);
        }
    }

    /* only if we have geometry list */
    if (status->gl.numGeoms)
    {
        if (!GeometryListFindGeometry
            (&status->gl, atomic->geometry, &a.geomIndex))
        {
            status->success = FALSE;
            RWRETURN((RpAtomic *)NULL);
        }
    }

    /* Convert it */
    (void)RwMemLittleEndian32(&a, sizeof(a));

    /* Write it */
    if (!RwStreamWrite(status->stream, &a, sizeof(a)))
    {
        status->success = FALSE;
        RWRETURN((RpAtomic *)NULL);
    }

    /* if no geomlist then embed geometry */
    if (status->gl.numGeoms == 0)
    {
        if (!RpGeometryStreamWrite(atomic->geometry, status->stream))
        {
            status->success = FALSE;
            RWRETURN((RpAtomic *)NULL);
        }
    }

    /* Atomic extension data */
    if (!rwPluginRegistryWriteDataChunks
        (&atomicTKList, status->stream, atomic))
    {
        /* Failed to write extension data */
        status->success = FALSE;
        RWRETURN((RpAtomic *)NULL);
    }

    RWRETURN(atomic);
}

static RpAtomic    *
ClumpAtomicStreamRead(RwStream * stream, rwFrameList * fl,
                      rpGeometryList * gl)
{
    RwBool              status;
    RwUInt32            size;
    RwUInt32            version;

    RWFUNCTION(RWSTRING("ClumpAtomicStreamRead"));
    RWASSERT(stream);
    RWASSERT(fl);
    RWASSERT(gl);

    status = RwStreamFindChunk(stream, rwID_STRUCT, &size, &version);

    if (!status)
    {
        RWERROR((E_RW_READ));
        RWRETURN((RpAtomic *)NULL);
    }

    if ((version >= rwLIBRARYBASEVERSION) &&
        (version <= rwLIBRARYCURRENTVERSION))
    {
        RpAtomic           *atom;
        _rpAtomicBinary     a;
        RpGeometry         *geom;

        /* Read the atomic */
        RWASSERT(size <= sizeof(a));
        memset(&a, 0, sizeof(a));

        status = (size == RwStreamRead(stream, &a, size));
        if (!status)
        {
            RWERROR((E_RW_READ));
            RWRETURN((RpAtomic *)NULL);
        }
        (void)RwMemNative32(&a, sizeof(a));

        atom = RpAtomicCreate();
        if (!atom)
        {
            RWRETURN((RpAtomic *)NULL);
        }

        /* Set the atomic types */
        RpAtomicSetFlags(atom, a.flags);
        if (fl->numFrames)
        {
            RpAtomicSetFrame(atom, fl->frames[a.frameIndex]);
        }

        /* get the geometry */
        if (gl->numGeoms)
        {
            RpAtomicSetGeometry(atom, gl->geometries[a.geomIndex], 0);
        }
        else
        {
            status = RwStreamFindChunk(stream, rwID_GEOMETRY,
                                       (RwUInt32 *)NULL, &version);
            if (!status)
            {
                RpAtomicDestroy(atom);
                RWERROR((E_RW_READ));
                RWRETURN((RpAtomic *)NULL);
            }

            if ((version >= rwLIBRARYBASEVERSION) &&
                (version <= rwLIBRARYCURRENTVERSION))
            {
                geom = RpGeometryStreamRead(stream);
                status = (NULL != geom);

                if (!status)
                {
                    RpAtomicDestroy(atom);
                    RWERROR((E_RW_READ));
                    RWRETURN((RpAtomic *)NULL);
                }
            }
            else
            {
                RpAtomicDestroy(atom);
                RWERROR((E_RW_BADVERSION));
                RWRETURN((RpAtomic *)NULL);
            }

            RpAtomicSetGeometry(atom, geom, 0);

            /* Bring the geometry reference count back down, so that
             * when the atomic is destroyed, so is the geometry.
             */
            RpGeometryDestroy(geom);
        }

        /* A bit ugly. If a rights owner is spotted, this will be set */
        lastSeenRightsPluginId = 0;
        lastSeenExtraData = 0;

        /* Atomic extension data */
        status = (NULL !=
                  rwPluginRegistryReadDataChunks(&atomicTKList, stream, atom));

        if (!status)
        {
            RWERROR((E_RW_READ));
            RWRETURN((RpAtomic *)NULL);
        }

        if (lastSeenRightsPluginId)
        {
            /* Silently ignore errors if any */
            _rwPluginRegistryInvokeRights(&atomicTKList, lastSeenRightsPluginId,
                                          atom, lastSeenExtraData);
        }

        RWRETURN(atom);
    }
    else
    {
        RWERROR((E_RW_BADVERSION));
        RWRETURN((RpAtomic *)NULL);
    }
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                         Opening and closing

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                               Helper functions

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

void
_rpAtomicResyncInterpolatedSphere(RpAtomic * atomic)
{
    RpGeometry         *geom;

    RWFUNCTION(RWSTRING("_rpAtomicResyncInterpolatedSphere"));
    RWASSERT(atomic);

    /* Need geometry to get the spheres */
    geom = atomic->geometry;
    if (geom)
    {
        RpInterpolator     *interpolator = &atomic->interpolator;

        if ((interpolator->startMorphTarget == interpolator->endMorphTarget)
            || (interpolator->startMorphTarget >= geom->numMorphTargets)
            || (interpolator->endMorphTarget >= geom->numMorphTargets))
        {
            /* Single key frame */
            if (
                (interpolator->startMorphTarget >= geom->numMorphTargets)
                || (interpolator->endMorphTarget >= geom->numMorphTargets))
            {
                /* One of the morph target indices is invalid,
                 * we can't interpolate,
                 * so just grab key frame zero, which is bound to exist.
                 */
                atomic->boundingSphere = geom->morphTarget[0].boundingSphere;
            }
            else
            {
                /* Both morph targets must be the same,
                 * so grab one or the other */
                atomic->boundingSphere =
                    geom->morphTarget[interpolator->
                                      startMorphTarget].boundingSphere;
            }
        }
        else
        {
            /* Interpolate morph targets */
            RpMorphTarget      *startMorphTarget =

                &geom->morphTarget[interpolator->startMorphTarget];
            RpMorphTarget      *endMorphTarget =

                &geom->morphTarget[interpolator->endMorphTarget];
            RwReal              scale =

                ((interpolator->recipTime) * (interpolator->position));

            /* Interpolate Radius */
            atomic->boundingSphere.radius =
                ((((((endMorphTarget->boundingSphere.radius)
                     -
                     (startMorphTarget->boundingSphere.radius))) * (scale))) +
                 (startMorphTarget->boundingSphere.radius));

            /* Interpolate Centre */
            RwV3dSubMacro(&atomic->boundingSphere.center,
                     &endMorphTarget->boundingSphere.center,
                     &startMorphTarget->boundingSphere.center);
            RwV3dScale(&atomic->boundingSphere.center,
                       &atomic->boundingSphere.center, scale);
            RwV3dAdd(&atomic->boundingSphere.center,
                     &atomic->boundingSphere.center,
                     &startMorphTarget->boundingSphere.center);
        }

        /* Interpolated sphere is not dirty now */
        interpolator->flags &= ~rpINTERPOLATORDIRTYSPHERE;

        /* But world sphere is */
        rwObjectSetPrivateFlags(atomic,
                                rwObjectGetPrivateFlags(atomic) |
                                rpATOMICPRIVATEWORLDBOUNDDIRTY);
    }

    RWRETURNVOID();
}

/**
 * \ingroup rpatomic
 * \ref RpAtomicGetWorldBoundingSphere is used to retrieve the specified
 * atomic's world bounding sphere. This is the atomic's bounding sphere
 * transformed to world space using the atomic's LTM.
 * The bounding sphere returned by this function is specified in world space.
 *
 * The world bounding sphere is typically used in rejection test such as
 * \ref RwCameraFrustumTestSphere.
 *
 * The world plugin must be attached before using this function.
 *
 * \param atomic  Pointer to the atomic.
 *
 * \return Returns pointer to the atomic's world bounding sphere if successful
 * or NULL if there is an error.
 *
 * \see RpAtomicGetGeometry
 * \see RpAtomicSetGeometry
 * \see RpAtomicGetInterpolator
 * \see RpAtomicGetFlags
 * \see RpAtomicSetFlags
 * \see RpAtomicGetFrame
 * \see RpAtomicSetFrame
 * \see RpAtomicGetBoundingSphere
 * \see RpAtomicGetRenderCallBack
 * \see RpAtomicSetRenderCallBack
 * \see RpWorldPluginAttach
 *
 */

const RwSphere     *
RpAtomicGetWorldBoundingSphere(RpAtomic * atomic)
{
    RwMatrix           *matrix;
    RwFrame            *frame = (RwFrame *) rwObjectGetParent(atomic);

    RWAPIFUNCTION(RWSTRING("RpAtomicGetWorldBoundingSphere"));
    RWASSERT(atomic);

    /* First make sure interpolated sphere is up to date */
    if (atomic->interpolator.flags & rpINTERPOLATORDIRTYSPHERE)
    {
        _rpAtomicResyncInterpolatedSphere(atomic);
    }

    /* Then make sure transformed sphere is OK */
    if (RwFrameDirty(frame)
        || rwObjectTestPrivateFlags(atomic, rpATOMICPRIVATEWORLDBOUNDDIRTY))
    {
        RwReal              atomicXScale2;
        RwReal              atomicYScale2;
        RwReal              atomicZScale2;
        RwReal              atomicScale;
        RwReal              atomicScale2;
        RwV3d              *right, *up, *at;

        /* Find the geometry center of the atomic to test */
        matrix = RwFrameGetLTM(frame);
        RwV3dTransformPoint(&atomic->worldBoundingSphere.center,
                            &atomic->boundingSphere.center, matrix);

        /* We only update the bounding sphere radius if the ltm matrix
         * isn't ortho normal. */
        if (rwMatrixTestFlags(matrix, rwMATRIXTYPEMASK) !=
            rwMATRIXTYPEORTHONORMAL)
        {
            /* Find the geometry radius of the atomic to test,
             * we no longer assume non-uniform scaling here */
            right = RwMatrixGetRight(matrix);
            up = RwMatrixGetUp(matrix);
            at = RwMatrixGetAt(matrix);
            atomicXScale2 = RwV3dDotProduct(right, right);
            atomicYScale2 = RwV3dDotProduct(up, up);
            atomicZScale2 = RwV3dDotProduct(at, at);

            atomicScale2 =
                RwRealMax3(atomicXScale2, atomicYScale2, atomicZScale2);
            rwSqrt(&atomicScale, atomicScale2);

            atomic->worldBoundingSphere.radius =
                atomic->boundingSphere.radius * atomicScale;
        }
        else
        {
            atomic->worldBoundingSphere.radius =
                atomic->boundingSphere.radius;
        }

        /* Reset the dirty flag */
        rwObjectSetPrivateFlags(atomic,
                                rwObjectGetPrivateFlags(atomic) &
                                (~rpATOMICPRIVATEWORLDBOUNDDIRTY));
    }

    /* Return the sphere in the atomic */
    RWRETURN(&atomic->worldBoundingSphere);
}

/****************************************************************************
 _rpClumpClose

 On entry   :
 On exit    : FALSE on failure
 */

void *
_rpClumpClose(void *instance,
              RwInt32 offset __RWUNUSED__,
              RwInt32 size __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("_rpClumpClose"));

    /* Kill any objects which are left over */
    RwFreeListForAllUsed(RWCLUMPGLOBAL(clumpFreeList),
                         ClumpTidyDestroyClump, NULL);
    RwFreeListForAllUsed(RWCLUMPGLOBAL(atomicFreeList),
                         ClumpTidyDestroyAtomic, NULL);

    /* Then blow away the free lists themselves */
    RwFreeListDestroy(RWCLUMPGLOBAL(atomicFreeList));
    RwFreeListDestroy(RWCLUMPGLOBAL(clumpFreeList));
    RWCLUMPGLOBAL(atomicFreeList) = (RwFreeList *)NULL;
    RWCLUMPGLOBAL(clumpFreeList) = (RwFreeList *)NULL;

    /* One less module instance */
    clumpModule.numInstances--;

    /* Success */
    RWRETURN(instance);
}

static RwInt32 _rpAtomicFreeListBlockSize = 128,
               _rpAtomicFreeListPreallocBlocks = 1;
static RwFreeList _rpAtomicFreeList;

/**
 * \ingroup rpatomic
 * \ref RpAtomicSetFreeListCreateParams allows the developer to specify
 * how many \ref RpAtomic s to preallocate space for.
 * Call before \ref RwEngineInit.
 *
 * \param blockSize  number of entries per freelist block.
 * \param numBlocksToPrealloc  number of blocks to allocate on
 * \ref RwFreeListCreateAndPreallocateSpace.
 *
 * \see RwFreeList
 *
 */
void
RpAtomicSetFreeListCreateParams( RwInt32 blockSize, RwInt32 numBlocksToPrealloc )
{
#if 0
    /*
     * Can not use debugging macros since the debugger is not initialized before
     * RwEngineInit.
     */
    /* Do NOT comment out RWAPIFUNCTION as gnumake verify will not function */
    RWAPIFUNCTION(RWSTRING("RpAtomicSetFreeListCreateParams"));
#endif
    _rpAtomicFreeListBlockSize = blockSize;
    _rpAtomicFreeListPreallocBlocks = numBlocksToPrealloc;
#if 0
    RWRETURNVOID();
#endif
}

static RwInt32 _rpClumpFreeListBlockSize = 128,
               _rpClumpFreeListPreallocBlocks = 1;
static RwFreeList _rpClumpFreeList;

/**
 * \ingroup rpclump
 * \ref RpClumpSetFreeListCreateParams allows the developer to specify
 * how many \ref RpClump s to preallocate space for.
 * Call before \ref RwEngineInit.
 *
 * \param blockSize  number of entries per freelist block.
 * \param numBlocksToPrealloc  number of blocks to allocate on
 * \ref RwFreeListCreateAndPreallocateSpace.
 *
 * \see RwFreeList
 *
 */
void
RpClumpSetFreeListCreateParams( RwInt32 blockSize, RwInt32 numBlocksToPrealloc )
{
#if 0
    /*
     * Can not use debugging macros since the debugger is not initialized before
     * RwEngineInit.
     */
    /* Do NOT comment out RWAPIFUNCTION as gnumake verify will not function */
    RWAPIFUNCTION(RWSTRING("RpClumpSetFreeListCreateParams"));
#endif
    _rpClumpFreeListBlockSize = blockSize;
    _rpClumpFreeListPreallocBlocks = numBlocksToPrealloc;
#if 0
    RWRETURNVOID();
#endif
}

/****************************************************************************
 _rpClumpOpen

 On entry   :
 On exit    : FALSE on failure
 */

void *
_rpClumpOpen( void *instance,
              RwInt32 offset,
              RwInt32 size __RWUNUSED__ )
{
    RWFUNCTION(RWSTRING("_rpClumpOpen"));

    /* Save the offset for globals access (same across all instances) */
    clumpModule.globalsOffset = offset;

    RWCLUMPGLOBAL(atomicFreeList) =
        RwFreeListCreateAndPreallocateSpace(atomicTKList.sizeOfStruct, _rpAtomicFreeListBlockSize,
        rwMATRIXALIGNMENT,
        _rpAtomicFreeListPreallocBlocks, &_rpAtomicFreeList, rwID_CLUMP | rwMEMHINTDUR_GLOBAL);
    if (RWCLUMPGLOBAL(atomicFreeList))
    {
        RWCLUMPGLOBAL(clumpFreeList) =
            RwFreeListCreateAndPreallocateSpace(clumpTKList.sizeOfStruct, _rpClumpFreeListBlockSize,
            rwMATRIXALIGNMENT,
            _rpClumpFreeListPreallocBlocks, &_rpClumpFreeList, rwID_ATOMIC | rwMEMHINTDUR_GLOBAL);
        if (RWCLUMPGLOBAL(clumpFreeList))
        {
            /* One more module instance */
            clumpModule.numInstances++;

            /* Success */
            RWRETURN(instance);
        }

        /* Failure */
        RwFreeListDestroy(RWCLUMPGLOBAL(atomicFreeList));
        RWCLUMPGLOBAL(atomicFreeList) = (RwFreeList *)NULL;
    }

    /* Failure */
    RWRETURN(NULL);
}


/******************************************************************************
 _rpClumpRegisterExtensions

 Registers the necessary memory/functions to maintain objects in the clump

 On entry   :
 On exit    : TRUE on success
 */
RwBool
_rpClumpRegisterExtensions()
{
    RWFUNCTION(RWSTRING("_rpClumpRegisterExtensions"));

    /* This attaches the necessary data to lights and cameras
     * to facilitate their maintenance within a clump.
     * No binary data is saved, so just use rwID_CLUMP.
     * No copy functions since lights and cameras aren't added to a clump
     * when they are cloned from a clump.
     */
    _rpClumpCameraExtOffset = RwCameraRegisterPlugin(sizeof(RpClumpCameraExt),
                                             rwID_CLUMP,
                                             ClumpInitCameraExt,
                                             ClumpDeInitCameraExt,
                                             NULL);
    if (_rpClumpCameraExtOffset < 0)
    {
        /* Failed */
        RWRETURN(FALSE);
    }

    _rpClumpLightExtOffset = RpLightRegisterPlugin(sizeof(RpClumpLightExt),
                                           rwID_CLUMP,
                                           ClumpInitLightExt,
                                           ClumpDeInitLightExt,
                                           NULL);
    if (_rpClumpLightExtOffset < 0)
    {
        /* Failed */
        RWRETURN(FALSE);
    }

    /* Success */
    RWRETURN(TRUE);
}


/**
 * \ingroup rpclump
 * \ref RpClumpGetNumAtomics is used to retrieve the number of atomics
 * in the specified clump.
 *
 * The world plugin must be attached before using this function.
 *
 * \param clump  Pointer to the clump containing the atomics.
 *
 * \return Returns the number of atomics if successful or -1 if there is
 * an error.
 *
 * \see RpClumpForAllAtomics
 * \see RpClumpAddAtomic
 * \see RpClumpRemoveAtomic
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpClumpGetNumAtomics(RpClump * clump)
{
    RwInt32             nNumAtomics = 0;

    RWAPIFUNCTION(RWSTRING("RpClumpGetNumAtomics"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(clump);
    RWASSERTISTYPE(clump, rpCLUMP);

    RpClumpForAllAtomics(clump, CountAtomic, &nNumAtomics);
    RWRETURN(nNumAtomics);
}

/**
 * \ingroup rpclump
 * \ref RpClumpGetNumLights is used to retrieve the number of lights
 * in the specified clump.
 *
 * The world plugin must be attached before using this function.
 *
 * \param clump  Pointer to the clump containing the lights.
 *
 * \return Returns the number of lights if successful or -1 if there is
 * an error.
 *
 * \see RpClumpForAllLights
 * \see RpClumpAddLight
 * \see RpClumpRemoveLight
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpClumpGetNumLights(RpClump * clump)
{
    RwInt32             nNumLights = 0;

    RWAPIFUNCTION(RWSTRING("RpClumpGetNumLights"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(clump);
    RWASSERTISTYPE(clump, rpCLUMP);

    RpClumpForAllLights(clump, CountLight, &nNumLights);
    RWRETURN(nNumLights);
}

/**
 * \ingroup rpclump
 * \ref RpClumpGetNumCameras is used to retrieve the number of cameras
 * in the specified clump.
 *
 * The world plugin must be attached before using this function.
 *
 * \param clump  Pointer to the clump containing the cameras.
 *
 * \return Returns the number of cameras if successful or -1 if there is
 * an error.
 *
 * \see RpClumpForAllCameras
 * \see RpClumpAddCamera
 * \see RpClumpRemoveCamera
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpClumpGetNumCameras(RpClump * clump)
{
    RwInt32             nNumCameras = 0;

    RWAPIFUNCTION(RWSTRING("RpClumpGetNumCameras"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(clump);
    RWASSERTISTYPE(clump, rpCLUMP);

    RpClumpForAllCameras(clump, CountCamera, &nNumCameras);
    RWRETURN(nNumCameras);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                       Creating a clump to mark space

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rpclump
 * \ref RpClumpCreateSpace is used to create a space holder clump at the
 * given position in world space and with the specified bounding sphere radius.
 * This function creates a clump that is not rendered but still takes place
 * in collision detection. Useful for immediate mode hooks and frustum
 * testing on points in the world via clump callbacks.
 *
 * The world plugin must be attached before using this function.
 *
 * \param position  Pointer to a RwV3d value equal to
 *                  the position of the clump.
 * \param radius  A RwReal value equal to the bounding sphere radius.
 *
 * \return Returns pointer to the clump if successful or NULL if there
 * is an error.
 *
 * \see RpClumpCreate
 * \see RpClumpDestroy
 * \see RpClumpClone
 * \see RpGeometryCreateSpace
 * \see RpWorldPluginAttach
 *
 */
RpClump            *
RpClumpCreateSpace(const RwV3d * position, RwReal radius)
{
    RpClump            *clump;
    RpGeometry         *geometry;
    RwFrame            *frame;
    RpAtomic           *atomic;
    RwV3d              *pos;

    RWAPIFUNCTION(RWSTRING("RpClumpCreateSpace"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(position);
    RWASSERT(radius >= 0.0f);

    geometry = RpGeometryCreateSpace(radius);
    if (!geometry)
    {
        RWRETURN((RpClump *)NULL);
    }

    frame = RwFrameCreate();
    if (!frame)
    {
        RpGeometryDestroy(geometry);
        RWRETURN((RpClump *)NULL);
    }

    /* Set the frames position */

    pos = &RwFrameGetMatrix(frame)->pos;

    RwV3dAssign(pos, position);

    atomic = RpAtomicCreate();
    if (!atomic)
    {
        RwFrameDestroy(frame);
        RpGeometryDestroy(geometry);
        RWRETURN((RpClump *)NULL);
    }

    clump = RpClumpCreate();
    if (!clump)
    {
        RpAtomicDestroy(atomic);
        RwFrameDestroy(frame);
        RpGeometryDestroy(geometry);
        RWRETURN((RpClump *)NULL);
    }

    /* We have everything required to make the clump */
    RpAtomicSetFrame(atomic, frame);
    RpAtomicSetGeometry(atomic, geometry, 0);

    /* Remove the atomic's render flag. This'll stop the empty
     * atomic being rendered when added to the world. */
    RpAtomicSetFlags(atomic, RpAtomicGetFlags(atomic) & ~rpATOMICRENDER);

    /* Bring the geometry reference count back down, so that
     * when the atomic is destroyed, so is the geometry.
     */
    RpGeometryDestroy(geometry); /* No panic, it doesn't really */

    RpClumpAddAtomic(clump, atomic);
    RpClumpSetFrame(clump, frame);

    /* All done */
    RWRETURN(clump);
}

#if ( defined(RWDEBUG) || defined(RWSUPPRESSINLINE) )

/**
 * \ingroup rpatomic
 * \ref RpAtomicRender is used to explicitly render the specified atomic
 * into the current camera's image raster. Rendering is attempted whether or
 * not the atomic (bounding-sphere) actually falls inside the camera's view
 * frustum.
 *
 * This function should only be called from within the context of a
 * \ref RwCameraBeginUpdate...\ref RwCameraEndUpdate block. This ensures that
 * any rendering that takes place is directed towards an image raster connected
 * to a camera. It also ensures that the atomic's local transformation matrix
 * is recalculated, if necessary, so that the atomic is rendered in its correct
 * position and orientation (also taking account of the current interpolation
 * value).
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param atomic  Pointer to the atomic.
 *
 * \return Returns a pointer to the atomic if successful or NULL
 * if there is an error.
 *
 * \see RwCameraBeginUpdate
 * \see RwCameraEndUpdate
 * \see RpAtomicGetRenderCallBack
 * \see RpAtomicSetRenderCallBack
 * \see RpClumpRender
 * \see RpWorldRender
 * \see RpWorldSectorRender
 * \see RwCameraShowRaster
 * \see RpWorldPluginAttach
 *
 */
RpAtomic           *
RpAtomicRender(RpAtomic * atomic)
{
    RWAPIFUNCTION(RWSTRING("RpAtomicRender"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(atomic);
    RWASSERTISTYPE(atomic, rpATOMIC);

    /* Call its render callback */
    if (atomic->renderCallBack(atomic))
    {
        /* Something has gone wrong in the instancing/rendering */
        RWRETURN(atomic);
    }

    /* Something went wrong for the rendering */
    RWRETURN(NULL);
}

#endif /* ( defined(RWDEBUG) || defined(RWSUPPRESSINLINE) ) */

/**
 * \ingroup rpclump
 * \ref RpClumpRender is used to render the specified clump into the current
 * camera's image raster. All atomics that comprise the clump are rendered.
 * Rendering is attempted whether or not the clump actually falls inside the
 * camera's view frustum.
 *
 * Clumps/atomics only need to be added to the world if they are to be rendered
 * via the \ref RpWorldRender call or if "world" lights are to influence the
 * objects.  World lights being lights that have a position, so a point light,
 * spot light and soft spot lights.  The directional and ambient lights will
 * still work without the object being added to the world.
 *
 * \note This function should only be called between
 * \ref RwCameraBeginUpdate and \ref RwCameraEndUpdate to ensure that any
 * rendering that takes place is directed towards an image raster connected
 * to a camera.
 *
 * It also ensures that each of the clump's atomics is synchronized,
 * if necessary, so that the clump is rendered in its correct position and
 * orientation (also taking account of the each atomic's current interpolation
 * value).
 *
 * The world plugin must be attached before using this function.
 *
 * \param clump  Pointer to the clump.
 *
 * \return Returns a pointer to the clump if successful or NULL
 * if there is an error.
 *
 * \see RwCameraBeginUpdate
 * \see RwCameraEndUpdate
 * \see RpAtomicRender
 * \see RpWorldRender
 * \see RwCameraShowRaster
 * \see RpWorldPluginAttach
 *
 */
RpClump            *
RpClumpRender(RpClump * clump)
{
    RpClump *result = clump;
    RwLLLink           *cur, *end;

    RWAPIFUNCTION(RWSTRING("RpClumpRender"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(clump);
    RWASSERTISTYPE(clump, rpCLUMP);

    cur = rwLinkListGetFirstLLLink(&clump->atomicList);
    end = rwLinkListGetTerminator(&clump->atomicList);

    while (cur != end)
    {
        RpAtomic *apAtom = rwLLLinkGetData(cur, RpAtomic, inClumpLink);

        if (rwObjectTestFlags(apAtom, rpATOMICRENDER))
        {
            /* Force this baby to be synced */
            RwFrameGetLTM((RwFrame *) rwObjectGetParent(apAtom));

            /* Then try and render it */
            if( NULL == RpAtomicRender(apAtom) )
            {
                result = NULL;
            }
        }

        cur = rwLLLinkGetNext(cur);
    }

    RWRETURN(result);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                             Getting atomics

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rpclump
 * \ref RpClumpForAllAtomics is used to apply the given callback function
 * to all atomics in the specified clump.
 * The format of the callback function is
 *
 * RpAtomic * (*RpAtomicCallBack)(RpAtomic *atomic, void *data)
 *
 * where data is a user-supplied data pointer to pass to the callback function.
 *
 * If any invocation of the callback
 * function returns a failure status the interation is terminated. However,
 * \ref RpClumpForAllAtomics will still return successfully.
 *
 * The world plugin must be attached before using this function.
 *
 * \param clump  Pointer to the clump containing the atomics.
 * \param callback  Pointer to the callback function to apply to each atomic.
 * \param pData  Pointer to user-supplied data to pass to callback function.
 *
 * \return Returns pointer to the clump if successful or NULL if there is an
 * error.
 *
 * \see RpClumpGetNumAtomics
 * \see RpWorldPluginAttach
 *
 */
RpClump            *
RpClumpForAllAtomics(RpClump * clump, RpAtomicCallBack callback, void *pData)
{
    RwLLLink           *cur, *end, *next;

    RWAPIFUNCTION(RWSTRING("RpClumpForAllAtomics"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(clump);
    RWASSERT(callback);
    RWASSERTISTYPE(clump, rpCLUMP);

    /* Enumerate all of the atomics in this clump */
    cur = rwLinkListGetFirstLLLink(&clump->atomicList);
    end = rwLinkListGetTerminator(&clump->atomicList);

    while (cur != end)
    {
        RpAtomic *atomic = rwLLLinkGetData(cur, RpAtomic, inClumpLink);
        RWASSERTISTYPE(atomic, rpATOMIC);

        /* Find next now, just in case we destroy the link */
        next = rwLLLinkGetNext(cur);

        if (!callback(atomic, pData))
        {
            /* Early out */
            RWRETURN(clump);
        }

        /* Onto the next atomic */
        cur = next;
    }

    RWRETURN(clump);
}


/**
 * \ingroup rpclump
 * \ref RpClumpForAllCameras is used to apply the given callback function
 * to all cameras in the specified clump.
 * The format of the callback function is
 *
 * RwCamera * (*RwCameraCallBack)(RwCamera *camera, void *data)
 *
 * where data is a user-supplied data pointer to pass to the callback function.
 *
 * If any invocation of the callback
 * function returns a failure status the interation is terminated. However,
 * \ref RpClumpForAllCameras will still return successfully.
 *
 * The world plugin must be attached before using this function.
 *
 * \param clump  Pointer to the clump containing the cameras.
 * \param callback  Pointer to the callback function to apply to each camera.
 * \param pData  Pointer to user-supplied data to pass to callback function.
 *
 * \return Returns pointer to the clump if successful or NULL if there is an
 * error.
 *
 * \see RpClumpGetNumCameras
 * \see RpWorldPluginAttach
 *
 */
RpClump            *
RpClumpForAllCameras(RpClump * clump, RwCameraCallBack callback, void *pData)
{
    RwLLLink           *cur, *end, *next;

    RWAPIFUNCTION(RWSTRING("RpClumpForAllCameras"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(clump);
    RWASSERT(callback);
    RWASSERTISTYPE(clump, rpCLUMP);

    /* Enumerate all of the cameras in this clump */
    cur = rwLinkListGetFirstLLLink(&clump->cameraList);
    end = rwLinkListGetTerminator(&clump->cameraList);

    while (cur != end)
    {
        RwCamera *camera = (RwCamera *) ( (RwUInt8 *)(cur) -
            offsetof(RpClumpCameraExt, inClumpLink) - _rpClumpCameraExtOffset);

        RWASSERTISTYPE(camera, rwCAMERA);

        /* Find next now, just in case we destroy the link */
        next = rwLLLinkGetNext(cur);

        if (!callback(camera, pData))
        {
            /* Early out */
            RWRETURN(clump);
        }

        /* Onto the next camera */
        cur = next;
    }

    RWRETURN(clump);
}


/**
 * \ingroup rpclump
 * \ref RpClumpForAllLights is used to apply the given callback function
 * to all lights in the specified clump.
 * The format of the callback function is
 *
 * RpLight * (*RpLightCallBack)(RpLight *light, void *data)
 *
 * where data is a user-supplied data pointer to pass to the callback function.
 *
 * If any invocation of the callback
 * function returns a failure status the interation is terminated. However,
 * \ref RpClumpForAllLights will still return successfully.
 *
 * The world plugin must be attached before using this function.
 *
 * \param clump  Pointer to the clump containing the lights.
 * \param callback  Pointer to the callback function to apply to each light.
 * \param pData  Pointer to user-supplied data to pass to callback function.
 *
 * \return Returns pointer to the clump if successful or NULL if there is an
 * error.
 *
 * \see RpClumpGetNumLights
 * \see RpWorldPluginAttach
 *
 */
RpClump            *
RpClumpForAllLights(RpClump * clump, RpLightCallBack callback, void *pData)
{
    RwLLLink           *cur, *end, *next;

    RWAPIFUNCTION(RWSTRING("RpClumpForAllLights"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(clump);
    RWASSERT(callback);
    RWASSERTISTYPE(clump, rpCLUMP);

    /* Enumerate all of the lights in this clump */
    cur = rwLinkListGetFirstLLLink(&clump->lightList);
    end = rwLinkListGetTerminator(&clump->lightList);

    while (cur != end)
    {
        RpLight *light = (RpLight *) ((RwUInt8 *)(cur) -
            offsetof(RpClumpLightExt, inClumpLink) - _rpClumpLightExtOffset);

        RWASSERTISTYPE(light, rpLIGHT);

        /* Find next now, just in case we destroy the link */
        next = rwLLLinkGetNext(cur);

        if (!callback(light, pData))
        {
            /* Early out */
            RWRETURN(clump);
        }

        /* Onto the next light */
        cur = next;
    }

    RWRETURN(clump);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                            Atomic handling

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rpatomic
 * \ref RpAtomicCreate is used to create and initialize a new atomic.
 * An atomic can be thought of as an instance of geometry which defines the
 * atomic's physical structure. The geometry may hold one
 * or more morph targets each containing vertex position and vertex normal
 * data. The atomic's structure in any one instance is defined by
 * its interpolator which linearly blends the morph target data according
 * to the current interpolator value and scale.
 * If only one morph target is defined
 * the atomic's structure is defined solely by that morph target.
 * The interpolator
 * for a newly created atomic is set such that the start and end morph targets
 * point to the first morph target in the atomic's geometry and the
 * interpolator's
 * value and scale are zero and one, respectively.
 *
 * Before the atomic can be used in any rendering it must be attached
 * to a frame
 * and added to a world (unless it is part of a clump, then adding the clump
 * to the world will suffice). The frame enables the atomic to be positioned
 * and oriented within the world. The frame may also be attached to an
 * existing frame hierarchy containing other atomics grouped together into
 * an articulated clump.
 *
 * The world plugin must be attached before using this function.
 *
 * \return Returns a pointer to the new atomic if successful or NULL if
 * there is an error.
 *
 * \see RpAtomicClone
 * \see RpAtomicSetGeometry
 * \see RpAtomicGetGeometry
 * \see RpAtomicSetFrame
 * \see RpAtomicGetFrame
 * \see RpAtomicGetInterpolator
 * \see RpAtomicGetBoundingSphere
 * \see RpAtomicSetFlags
 * \see RpAtomicGetFlags
 * \see RpAtomicDestroy
 * \see RpAtomicSetRenderCallBack
 * \see RpAtomicGetRenderCallBack
 * \see RpWorldAddAtomic
 * \see RpAtomicRender
 * \see RpClumpAddAtomic
 * \see RpGeometryCreate
 * \see RpWorldPluginAttach
 *
 */
RpAtomic           *
RpAtomicCreate(void)
{
    RpAtomic           *atomic;

    RWAPIFUNCTION(RWSTRING("RpAtomicCreate"));
    RWASSERT(clumpModule.numInstances);

    RWASSERT(RWCLUMPGLOBAL(atomicFreeList));
    atomic = (RpAtomic *) RwFreeListAlloc(RWCLUMPGLOBAL(atomicFreeList),
                                      rwID_ATOMIC | rwMEMHINTDUR_EVENT);
    if (!atomic)
    {
        RWRETURN((RpAtomic *)NULL);
    }

    /* We don't care about the sub type */
    rwObjectHasFrameInitialize(atomic, rpATOMIC, 0, AtomicSync);

    /* NOTE: the repEntry is set to NULL because (a) if the geom is
     * non-animated then IT will hold the repEntry and (b) if the geom is
     * animated then the atomic will have a repEntry created during its
     * next pipeline execute. [A similar argument applies to the PS2
     * rwMeshCache plugin set up during rwPluginRegistryInitObject, below] */
    atomic->repEntry = (RwResEntry *)NULL;

    /* Flag the atomic as dirty -> ie vertices need expanding,
     * and the world space bounding sphere too...
     */
    RpAtomicSetFlags(atomic, rpATOMICCOLLISIONTEST | rpATOMICRENDER);
    rwObjectSetPrivateFlags(atomic, rpATOMICPRIVATEWORLDBOUNDDIRTY);

    /* No frame yet */
    RpAtomicSetFrame(atomic, (RwFrame *)NULL);

    /* Set up clump status */
    atomic->geometry = (RpGeometry *)NULL;

    /* And the bounding sphere's (these get updated during frame
     * synchronization and instancing)
     */
    atomic->boundingSphere.radius = (RwReal) (0);
    atomic->boundingSphere.center.x = (RwReal) (0);
    atomic->boundingSphere.center.y = (RwReal) (0);
    atomic->boundingSphere.center.z = (RwReal) (0);

    atomic->worldBoundingSphere.radius = (RwReal) (0);
    atomic->worldBoundingSphere.center.x = (RwReal) (0);
    atomic->worldBoundingSphere.center.y = (RwReal) (0);
    atomic->worldBoundingSphere.center.z = (RwReal) (0);

    /* Set up the default render callback */
    RpAtomicSetRenderCallBack(atomic, AtomicDefaultRenderCallBack);

    /* Set on the first frame of the geometry */
    atomic->interpolator.startMorphTarget = 0;
    atomic->interpolator.endMorphTarget = 0;
    atomic->interpolator.time = (RwReal) (1.0);
    atomic->interpolator.recipTime = (RwReal) (1.0);
    atomic->interpolator.position = (RwReal) (0.0);
    atomic->interpolator.flags = (RwInt32)
        (rpINTERPOLATORDIRTYINSTANCE | rpINTERPOLATORDIRTYSPHERE);

    /* membership of clump */
    rwLLLinkInitialize(&atomic->inClumpLink);
    atomic->clump = (RpClump *)NULL;

    /* use the default atomic object pipeline */
    atomic->pipeline = (RxPipeline *)NULL;

    /* Not in any atomic sectors */
    rwLinkListInitialize(&atomic->llWorldSectorsInAtomic);

    /* Initialize memory allocated to toolkits */
    rwPluginRegistryInitObject(&atomicTKList, atomic);

    RWRETURN(atomic);
}

/**
 * \ingroup rpatomic
 * \ref RpAtomicSetGeometry is used to attach the specified geometry
 * to the given atomic. The geometry defines the physical structure of the
 * atomic via one or more morph targets each containing vertex position
 * and vertex normal data. The atomic's structure in any one instance is
 * defined by the atomic's interpolator which blends the morph target data
 * according to the current interpolator value and scale. If only one morph
 * target  is defined the atomic's structure is defined solely by that
 * morph target. The atomic's bounding sphere is also defined as the
 * interpolation of the morph targets' bounding spheres.
 *
 * Setting the geometry also sets the atomic's bounding sphere equal to the
 * bounding sphere of the first morph target. The atomic's bounding sphere will
 * be recalculated, if necessary, when the atomic is instanced (just prior to
 * rendering) according to the current interpolator value and scale.
 *
 * If the flags parameter is zero, setting the geometry also sets the atomic's
 * bounding sphere equal to the bounding sphere of the first morph target.
 * Otherwise, it is assumed the bounding-sphere has not changed and should not
 * be recalculated (useful for LOD models). Also, the atomic's bounding sphere
 * will be recalculated, if necessary, when the atomic is instanced (just prior
 * to rendering) according to the current interpolator value and scale.
 *
 * If a geometry is already attached to the given atomic then that geometry
 * is destroyed (unless it is still used by another atomic) before
 * the new one is added.
 *
 * The world plugin must be attached before using this function.
 *
 * \param atomic  Pointer to the atomic.
 * \param geometry  Pointer to the geometry.
 * \param flags A RwUInt32 value which defines how the atomic references the
 *              new geometry. The range of know values are defined in
 *              the \ref RpAtomicSetGeomFlag enumerated type.
 *              Specify zero to have the bounding-sphere recalculated.
 *
 * \return Returns pointer to the atomic if successful or NULL if
 * there was an error.
 *
 * \see RpAtomicGetGeometry
 * \see RpAtomicSetFrame
 * \see RpAtomicGetFrame
 * \see RpAtomicGetInterpolator
 * \see RpAtomicGetBoundingSphere
 * \see RpAtomicSetFlags
 * \see RpAtomicGetFlags
 * \see RpAtomicSetRenderCallBack
 * \see RpAtomicGetRenderCallBack
 * \see RpAtomicGetWorld
 * \see RpWorldAddAtomic
 * \see RpGeometryCreate
 * \see RpGeometryDestroy
 * \see RpWorldPluginAttach
 *
 */
RpAtomic           *
RpAtomicSetGeometry(RpAtomic * atomic, RpGeometry * geometry, RwUInt32 flags)
{
    RWAPIFUNCTION(RWSTRING("RpAtomicSetGeometry"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(atomic);
    RWASSERTISTYPE(atomic, rpATOMIC);

    if (geometry != atomic->geometry)
    {
        RwFrame            *frame;

        if (geometry)
        {
            /* Add ref the new geometry */
            RpGeometryAddRef(geometry);
        }

        if (atomic->geometry)
        {
            /* Reduce refrence count on the old geometry */
            RpGeometryDestroy(atomic->geometry);
        }

        /* The instanced copy will be updated when the
         * mesh serial numbers don't match during instancing
         */

        /* Point to the geometry used */
        atomic->geometry = geometry;

        if (!(flags & rpATOMICSAMEBOUNDINGSPHERE))
        {
            if (geometry)
            {
                atomic->boundingSphere =
                    geometry->morphTarget[0].boundingSphere;
            }

            frame = (RwFrame *) rwObjectGetParent(atomic);
            if (frame && RpAtomicGetWorld(atomic))
            {
                /* Mark the frame as dirty so the ties get updated. */
                RwFrameUpdateObjects(frame);
            }
        }

        /* The instanced copy will be updated when the
         * mesh serial numbers don't match during instancing
         */
    }

    RWRETURN(atomic);
}

#if ( defined(RWDEBUG) || defined(RWSUPPRESSINLINE) )

/**
 * \ingroup rpatomic
 * \ref RpAtomicGetGeometry is used to retrieve the geometry referenced
 * by the specified atomic.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param atomic  Pointer to the atomic containing the geometry.
 *
 * \return Returns pointer to the atomic's geometry if successful or NULL
 * if there is an error or if the geometry is undefined.
 *
 * \see RpAtomicSetGeometry
 * \see RpAtomicSetFrame
 * \see RpAtomicGetFrame
 * \see RpAtomicGetInterpolator
 * \see RpAtomicGetBoundingSphere
 * \see RpAtomicSetFlags
 * \see RpAtomicGetFlags
 * \see RpAtomicSetRenderCallBack
 * \see RpAtomicGetRenderCallBack
 * \see RpAtomicGetWorld
 * \see RpWorldAddAtomic
 * \see RpGeometryCreate
 * \see RpWorldPluginAttach
 *
 */
RpGeometry         *
RpAtomicGetGeometry(const RpAtomic * atomic)
{
    RWAPIFUNCTION(RWSTRING("RpAtomicGetGeometry"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(atomic);
    RWASSERTISTYPE(atomic, rpATOMIC);

    RWRETURN(atomic->geometry);
}

#endif /* ( defined(RWDEBUG) || defined(RWSUPPRESSINLINE) ) */

/**
 * \ingroup rpatomic
 * \ref RpAtomicDestroy is used to destroy the specified atomic. It is
 * recommended that the atomic is first removed from the world, if necessary,
 * before calling this function.
 *
 * If the atomic references a geometry, the geometry is also destroyed if the
 * geometry's reference count is zero.
 *
 * Note also that if the atomic is attached to a frame, the
 * frame is not destroyed by this function and should be done so explicitly
 * if required.
 *
 * The world plugin must be attached before using this function.
 *
 * \param atomic  Pointer to the atomic.
 *
 * \return Returns TRUE if successful or FALSE if there
 * is an error.
 *
 * \see RpAtomicCreate
 * \see RpWorldRemoveAtomic
 * \see RpAtomicGetWorld
 * \see RpWorldAddAtomic
 * \see RpGeometryDestroy
 * \see RpWorldPluginAttach
 *
 */
RwBool
RpAtomicDestroy(RpAtomic * atomic)
{
    RWAPIFUNCTION(RWSTRING("RpAtomicDestroy"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(atomic);
    RWASSERTISTYPE(atomic, rpATOMIC);

    /* De-init the clump plugin registered memory */
    rwPluginRegistryDeInitObject(&atomicTKList, atomic);

    /* Blow away it's instance copy if it has one */
    if (atomic->repEntry)
    {
        RwResourcesFreeResEntry(atomic->repEntry);
    }

    /* Detach the geometry
     * - it gets destroyed if not referenced from elsewhere */
    RpAtomicSetGeometry(atomic, (RpGeometry *)NULL, 0);

    /* Remove it from the list of objects if attached to frame */
    rwObjectHasFrameReleaseFrame(atomic);

    /* Free the atom */
    RwFreeListFree(RWCLUMPGLOBAL(atomicFreeList), atomic);

    RWRETURN(TRUE);
}

#if ( defined(RWDEBUG) || defined(RWSUPPRESSINLINE) )

/**
 * \ingroup rpatomic
 * \ref RpAtomicSetRenderCallBack is used to define the render
 * callback function for the specified atomic. The callback function is
 * executed from \ref RpAtomicRender when the atomic lies inside the
 * current camera's view frustum,
 * giving the application the opportunity to alter the way in which
 * the atomic is rendered. The default callback function will invoke the
 * atomic rendering pipeline. The default function can be reinstated by
 * specifying NULL in the callback function parameter.
 *
 * The format of the callback function is:
 *
 * void (*RpAtomicCallBackRender)(RpAtomic *atomic);
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param atomic    Pointer to the atomic.
 * \param callback  Pointer to the render callback function.
 *                  See \ref RpAtomicCallBackRender.
 *
 * \return Returns no value.
 *
 * \see RpAtomicGetRenderCallBack
 * \see RpAtomicRender
 * \see RpAtomicSetGeometry
 * \see RpAtomicSetFrame
 * \see RpAtomicGetFrame
 * \see RpAtomicGetInterpolator
 * \see RpAtomicGetBoundingSphere
 * \see RpAtomicSetFlags
 * \see RpAtomicGetFlags
 * \see RpAtomicGetWorld
 * \see RpWorldAddAtomic
 * \see RpWorldPluginAttach
 *
 */
void
RpAtomicSetRenderCallBack(RpAtomic * atomic, RpAtomicCallBackRender callback)
{
    RWAPIFUNCTION(RWSTRING("RpAtomicSetRenderCallBack"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(atomic);
    RWASSERTISTYPE(atomic, rpATOMIC);

    RpAtomicSetRenderCallBackMacro(atomic, callback);

    RWRETURNVOID();
}

/**
 * \ingroup rpatomic
 * \ref RpAtomicGetRenderCallBack is used to retrieve the render
 * callback function of the specified atomic. The render callback
 * is the function that is executed from \ref RpAtomicRender when an atomic
 * lies inside the camera's view frustum, giving the application the
 * opportunity to alter the way in which an atomic is rendered.
 * \note If the atomic does not have a custom render callback, then the
 * default RenderWare Graphics atomic render callback is returned.
 *
 * The format of the callback function is:
 *
 * \verbatim
   RpAtomic *(*RpAtomicCallBackRender)(RpAtomic *atomic);
   \endverbatim
 *
 * \note This function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param atomic  Pointer to the atomic.
 *
 * \return Returns pointer to the callback function if successful or NULL
 * if there is an error.
 *
 * \see RpAtomicSetRenderCallBack
 * \see RpAtomicRender
 * \see RpAtomicSetGeometry
 * \see RpAtomicSetFrame
 * \see RpAtomicGetFrame
 * \see RpAtomicGetInterpolator
 * \see RpAtomicGetBoundingSphere
 * \see RpAtomicSetFlags
 * \see RpAtomicGetFlags
 * \see RpAtomicGetWorld
 * \see RpWorldAddAtomic
 * \see RpWorldPluginAttach
 *
 */
RpAtomicCallBackRender
RpAtomicGetRenderCallBack(const RpAtomic * atomic)
{
    RWAPIFUNCTION(RWSTRING("RpAtomicGetRenderCallBack"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(atomic);
    RWASSERTISTYPE(atomic, rpATOMIC);

    RWRETURN(atomic->renderCallBack);
}


/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                           Interpolations

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rpatomic
 * \ref RpAtomicGetInterpolator is used to retrieve the interpolator used
 * by the specified atomic. The interpolator is responsible for defining
 * which morph targets will be linearly blended into a geometry that can be
 * rendered.  The proportion of each morph target is determined by the
 * interpolator's current value and scale.
 *
 * The world plugin must be attached before using this function.
 *
 * \param atomic  Pointer to the atomic containing the interpolator.
 *
 * \return Returns pointer to the atomic's interpolator if successful or
 * NULL if there is an error.
 *
 * \see RpInterpolatorSetStartMorphTarget
 * \see RpInterpolatorGetStartMorphTarget
 * \see RpInterpolatorSetEndMorphTarget
 * \see RpInterpolatorGetEndMorphTarget
 * \see RpInterpolatorSetValue
 * \see RpInterpolatorGetValue
 * \see RpInterpolatorSetScale
 * \see RpInterpolatorGetScale
 * \see RpWorldPluginAttach
 *
 */
RpInterpolator     *
RpAtomicGetInterpolator(RpAtomic * atomic)
{
    RWAPIFUNCTION(RWSTRING("RpAtomicGetInterpolator"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(atomic);
    RWASSERTISTYPE(atomic, rpATOMIC);

    RWRETURN(&atomic->interpolator);
}


/**
 * \ingroup rpinterpolator
 * \ref RpInterpolatorGetStartMorphTarget is used to retrieve the index
 * of the start morph target for the given interpolator. The start morph
 * target contains the geometry which will be rendered when the interpolator
 * value is zero.  By default the start morph target has index zero
 * corresponding to the first morph target in the atomic's geometry.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param interpolator  Pointer to the interpolator.
 *
 * \return Returns pointer to the start morph target if successful or -1 if
 * there is an error.
 *
 * \see RpInterpolatorSetStartMorphTarget
 * \see RpInterpolatorSetEndMorphTarget
 * \see RpInterpolatorGetEndMorphTarget
 * \see RpInterpolatorSetValue
 * \see RpInterpolatorGetValue
 * \see RpInterpolatorSetScale
 * \see RpInterpolatorGetScale
 * \see RpAtomicGetInterpolator
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpInterpolatorGetStartMorphTarget(const RpInterpolator * interpolator)
{
    RWAPIFUNCTION(RWSTRING("RpInterpolatorGetStartMorphTarget"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(interpolator);

    RWRETURN(interpolator->startMorphTarget);
}

/**
 * \ingroup rpinterpolator
 * \ref RpInterpolatorGetEndMorphTarget is used to retrieve the index of
 * the end morph target for the given interpolator. The end morph target
 * contains the geometry which will be rendered when the interpolator reaches
 * its maximum value (equal to the interpolator's scale).
 * By default the end morph target has index zero corresponding to the first
 * morph target in the atomic's geometry.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param interpolator  Pointer to the interpolator.
 *
 * \return Returns pointer to the end morph target if successful or -1 if
 * there is an error.
 *
 * \see RpInterpolatorSetEndMorphTarget
 * \see RpInterpolatorGetStartMorphTarget
 * \see RpInterpolatorSetStartMorphTarget
 * \see RpInterpolatorSetValue
 * \see RpInterpolatorGetValue
 * \see RpInterpolatorSetScale
 * \see RpInterpolatorGetScale
 * \see RpAtomicGetInterpolator
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpInterpolatorGetEndMorphTarget(const RpInterpolator * interpolator)
{
    RWAPIFUNCTION(RWSTRING("RpInterpolatorGetEndMorphTarget"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(interpolator);

    RWRETURN(interpolator->endMorphTarget);
}

/**
 * \ingroup rpinterpolator
 * \ref RpInterpolatorGetValue is used to retrieve the current value of
 * the specified interpolator. Valid interpolator values are in the range
 * from zero up to the interpolator's scale. For a new atomic the value
 * is zero.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param interpolator  Pointer to the interpolator.
 *
 * \return Returns a RwReal value equal to the interpolator's value
 * if successful
 * or zero if there is an error or if its value is also zero.
 *
 * \see RpInterpolatorSetValue
 * \see RpInterpolatorSetStartMorphTarget
 * \see RpInterpolatorGetStartMorphTarget
 * \see RpInterpolatorSetEndMorphTarget
 * \see RpInterpolatorGetEndMorphTarget
 * \see RpInterpolatorSetScale
 * \see RpInterpolatorGetScale
 * \see RpAtomicGetInterpolator
 * \see RpWorldPluginAttach
 *
 */
RwReal
RpInterpolatorGetValue(const RpInterpolator * interpolator)
{
    RWAPIFUNCTION(RWSTRING("RpInterpolatorGetValue"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(interpolator);

    RWRETURN(interpolator->position);
}

/**
 * \ingroup rpinterpolator
 * \ref RpInterpolatorGetScale is used to retrieve the current scale
 * of the specified interpolator. The scale corresponds to the maximum value
 * the interpolator can have. A new atomic has a scale of one.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param interpolator  Pointer to the interpolator.
 *
 * \return Returns a RwReal value equal to the interpolator's scale
 * if successful
 * or zero if there is an error or if its value is also zero.
 *
 * \see RpInterpolatorSetScale
 * \see RpInterpolatorSetStartMorphTarget
 * \see RpInterpolatorGetStartMorphTarget
 * \see RpInterpolatorSetEndMorphTarget
 * \see RpInterpolatorGetEndMorphTarget
 * \see RpInterpolatorSetValue
 * \see RpInterpolatorGetValue
 * \see RpAtomicGetInterpolator
 * \see RpWorldPluginAttach
 *
 */
RwReal
RpInterpolatorGetScale(const RpInterpolator * interpolator)
{
    RWAPIFUNCTION(RWSTRING("RpInterpolatorGetScale"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(interpolator);

    RWRETURN(interpolator->time);
}

/**
 * \ingroup rpinterpolator
 * \ref RpInterpolatorSetStartMorphTarget is used to define the index
 * of the start morph target for the given interpolator.
 * The start morph target contains
 * the geometry which will be rendered when the interpolator value is zero.
 * By default the start morph target has index zero corresponding to the first
 * morph target in the atomic's geometry.
 *
 * Calling this function will cause the owning atomic's frame to be flagged
 * as dirty, so that it is brought up to date at the next synchronization
 * stage (see \ref RwFrameUpdateObjects). This is so that any changes in
 * the atomics bounding sphere can be taken into account when calculating
 * what sectors the atomic overlaps. This behavior can be overridden (in
 * the case where the application can bring bounding spheres up-to-date
 * in a more efficient manner) by setting the rpINTERPOLATORNOFRAMEDIRTY
 * flag for the interpolator.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param interpolator  Pointer to the interpolator.
 * \param morphTarget   A RwInt32 value equal to the index of
 *                      the morph target
 *                      (the first morph target has index zero).
 * \param atomic        A pointer to the atomic owning this interpolator.
 *
 * \return Returns pointer to the interpolator if successful or NULL if
 * there is an error.
 *
 * \see RpInterpolatorGetStartMorphTarget
 * \see RpInterpolatorSetEndMorphTarget
 * \see RpInterpolatorGetEndMorphTarget
 * \see RpInterpolatorSetValue
 * \see RpInterpolatorGetValue
 * \see RpInterpolatorSetScale
 * \see RpInterpolatorGetScale
 * \see RpAtomicGetInterpolator
 * \see RpWorldPluginAttach
 *
 */
RpInterpolator     *
RpInterpolatorSetStartMorphTarget(RpInterpolator * interpolator,
                                  RwInt32 morphTarget,
                                  RpAtomic *atomic)
{
    RWAPIFUNCTION(RWSTRING("RpInterpolatorSetStartMorphTarget"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(interpolator);
    RWASSERT(morphTarget >= 0);

    interpolator->startMorphTarget = (RwInt16) morphTarget;
    interpolator->flags |= (RwInt32)
        (rpINTERPOLATORDIRTYINSTANCE | rpINTERPOLATORDIRTYSPHERE);

    if (!(interpolator->flags & rpINTERPOLATORNOFRAMEDIRTY))
    {
        /* Get the bounding sphere (inc. the world one) updating during
         * frame resycnhing in camerabeginupdate. This ensures that the
         * atomic ends up in the right world sectors during rendering.
         * Note that users can override this (if they think resynching
         * the spheres manually is quicker) using the interpolator flag
         * rpINTERPOLATORNOFRAMEDIRTY */
        RwFrame *frame;

        frame = RpAtomicGetFrame(atomic);
        if( frame )
        {
            RwFrameUpdateObjects(frame);
        }
    }

    RWRETURN(interpolator);
}

/**
 * \ingroup rpinterpolator
 * \ref RpInterpolatorSetEndMorphTarget is used to define the index of the
 * end morph target for the given interpolator. The end morph target contains
 * the geometry which will be rendered when the interpolator reaches
 * its maximum value (equal to the interpolator's scale).
 * By default the end morph target has index zero corresponding to the first
 * morph target in the atomic's geometry.
 *
 * Calling this function will cause the owning atomic's frame to be flagged
 * as dirty, so that it is brought up to date at the next synchronization
 * stage (see \ref RwFrameUpdateObjects). This is so that any changes in
 * the atomics bounding sphere can be taken into account when calculating
 * what sectors the atomic overlaps. This behavior can be overridden (in
 * the case where the application can bring bounding spheres up-to-date
 * in a more efficient manner) by setting the rpINTERPOLATORNOFRAMEDIRTY
 * flag for the interpolator.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param interpolator  Pointer to the interpolator.
 * \param morphTarget   A RwInt32 value equal to the index of the morph target
 *                      (the first morph trarget has index zero).
 * \param atomic        A pointer to the atomic owning this interpolator.
 *
 * \return Returns pointer to the interpolator if successful or NULL if
 * there is an error.
 *
 * \see RpInterpolatorGetEndMorphTarget
 * \see RpInterpolatorSetStartMorphTarget
 * \see RpInterpolatorGetStartMorphTarget
 * \see RpInterpolatorSetValue
 * \see RpInterpolatorGetValue
 * \see RpInterpolatorSetScale
 * \see RpInterpolatorGetScale
 * \see RpAtomicGetInterpolator
 * \see RpWorldPluginAttach
 *
 */
RpInterpolator     *
RpInterpolatorSetEndMorphTarget(RpInterpolator * interpolator,
                                RwInt32 morphTarget,
                                RpAtomic *atomic)
{
    RWAPIFUNCTION(RWSTRING("RpInterpolatorSetEndMorphTarget"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(interpolator);
    RWASSERT(morphTarget >= 0);

    interpolator->endMorphTarget = (RwInt16) morphTarget;
    interpolator->flags |= (RwInt32)
        (rpINTERPOLATORDIRTYINSTANCE | rpINTERPOLATORDIRTYSPHERE);

    if (!(interpolator->flags & rpINTERPOLATORNOFRAMEDIRTY))
    {
        /* Get the bounding sphere (inc. the world one) updating during
         * frame resycnhing in camerabeginupdate. This ensures that the
         * atomic ends up in the right world sectors during rendering.
         * Note that users can override this (if they think resynching
         * the spheres manually is quicker) using the interpolator flag
         * rpINTERPOLATORNOFRAMEDIRTY */
        RwFrame *frame;

        frame = RpAtomicGetFrame(atomic);
        if( frame )
        {
            RwFrameUpdateObjects(frame);
        }
    }

    RWRETURN(interpolator);
}

/**
 * \ingroup rpinterpolator
 * \ref RpInterpolatorSetValue is used to define the current value
 * for the given interpolator. Valid interpolator values are in the range
 * from zero up to the interpolator's scale. For a new atomic the value
 * is zero.
 *
 * Calling this function will cause the owning atomic's frame to be flagged
 * as dirty, so that it is brought up to date at the next synchronization
 * stage (see \ref RwFrameUpdateObjects). This is so that any changes in
 * the atomics bounding sphere can be taken into account when calculating
 * what sectors the atomic overlaps. This behavior can be overridden (in
 * the case where the application can bring bounding spheres up-to-date
 * in a more efficient manner) by setting the rpINTERPOLATORNOFRAMEDIRTY
 * flag for the interpolator.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param interpolator  Pointer to the interpolator.
 * \param value         A RwReal value equal to the interpolation value.
 * \param atomic        A pointer to the atomic owning this interpolator.
 *
 * \return Returns pointer to the interpolator if successful or NULL if
 * there is an error.
 *
 * \see RpInterpolatorGetValue
 * \see RpInterpolatorSetStartMorphTarget
 * \see RpInterpolatorGetStartMorphTarget
 * \see RpInterpolatorSetEndMorphTarget
 * \see RpInterpolatorGetEndMorphTarget
 * \see RpInterpolatorSetScale
 * \see RpInterpolatorGetScale
 * \see RpAtomicGetInterpolator
 * \see RpWorldPluginAttach
 *
 */
RpInterpolator     *
RpInterpolatorSetValue(RpInterpolator * interpolator,
                       RwReal value,
                       RpAtomic *atomic)
{
    RWAPIFUNCTION(RWSTRING("RpInterpolatorSetValue"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(interpolator);

    interpolator->position = value;
    interpolator->flags |= (RwInt32)
        (rpINTERPOLATORDIRTYINSTANCE | rpINTERPOLATORDIRTYSPHERE);

    if (!(interpolator->flags & rpINTERPOLATORNOFRAMEDIRTY))
    {
        /* Get the bounding sphere (inc. the world one) updating during
         * frame resycnhing in camerabeginupdate. This ensures that the
         * atomic ends up in the right world sectors during rendering.
         * Note that users can override this (if they think resynching
         * the spheres manually is quicker) using the interpolator flag
         * rpINTERPOLATORNOFRAMEDIRTY */
        RwFrame *frame;

        frame = RpAtomicGetFrame(atomic);
        if( frame )
        {
            RwFrameUpdateObjects(frame);
        }
    }

    RWRETURN(interpolator);
}

/**
 * \ingroup rpinterpolator
 * \ref RpInterpolatorSetScale is used to define the current scale
 * for the given interpolator. The scale corresponds to the maximum value
 * the interpolator can have. A new atomic has a scale of one.
 *
 * Calling this function will cause the owning atomic's frame to be flagged
 * as dirty, so that it is brought up to date at the next synchronization
 * stage (see \ref RwFrameUpdateObjects). This is so that any changes in
 * the atomics bounding sphere can be taken into account when calculating
 * what sectors the atomic overlaps. This behavior can be overridden (in
 * the case where the application can bring bounding spheres up-to-date
 * in a more efficient manner) by setting the rpINTERPOLATORNOFRAMEDIRTY
 * flag for the interpolator.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param interpolator  Pointer to the interpolator.
 * \param scale         A RwReal value equal to the interpolation scale value.
 * \param atomic        A pointer to the atomic owning this interpolator.
 *
 * \return Returns pointer to the interpolator if successful or NULL if
 * there is an error.
 *
 * \see RpInterpolatorGetScale
 * \see RpInterpolatorSetStartMorphTarget
 * \see RpInterpolatorGetStartMorphTarget
 * \see RpInterpolatorSetEndMorphTarget
 * \see RpInterpolatorGetEndMorphTarget
 * \see RpInterpolatorSetValue
 * \see RpInterpolatorGetValue
 * \see RpAtomicGetInterpolator
 * \see RpWorldPluginAttach
 *
 */
RpInterpolator     *
RpInterpolatorSetScale(RpInterpolator * interpolator,
                       RwReal scale,
                       RpAtomic *atomic)
{
    RWAPIFUNCTION(RWSTRING("RpInterpolatorSetScale"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(interpolator);

    interpolator->time = scale;
    interpolator->recipTime = 1.0f / scale;
    interpolator->flags |= (RwInt32)
        (rpINTERPOLATORDIRTYINSTANCE | rpINTERPOLATORDIRTYSPHERE);

    if (!(interpolator->flags & rpINTERPOLATORNOFRAMEDIRTY))
    {
        /* Get the bounding sphere (inc. the world one) updating during
         * frame resycnhing in camerabeginupdate. This ensures that the
         * atomic ends up in the right world sectors during rendering.
         * Note that users can override this (if they think resynching
         * the spheres manually is quicker) using the interpolator flag
         * rpINTERPOLATORNOFRAMEDIRTY */
        RwFrame *frame;

        frame = RpAtomicGetFrame(atomic);
        if( frame )
        {
            RwFrameUpdateObjects(frame);
        }
    }

    RWRETURN(interpolator);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                       Getting an atomic's clump

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rpatomic
 * \ref RpAtomicGetClump is used to retrieve the specified atomic's
 * parent clump.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param atomic  Pointer to the atomic.
 *
 * \return Returns pointer to the parent clump if successful or NULL if there
 * is an error or if the atomic has no parent clump.
 *
 * \see RpClumpAddAtomic
 * \see RpClumpRemoveAtomic
 * \see RpAtomicGetGeometry
 * \see RpAtomicSetGeometry
 * \see RpAtomicGetInterpolator
 * \see RpAtomicGetFlags
 * \see RpAtomicSetFlags
 * \see RpAtomicGetBoundingSphere
 * \see RpAtomicGetFrame
 * \see RpAtomicSetFrame
 * \see RpAtomicGetWorld
 * \see RpWorldAddAtomic
 * \see RpAtomicGetRenderCallBack
 * \see RpAtomicSetRenderCallBack
 * \see RpWorldPluginAttach
 *
 */
RpClump            *
RpAtomicGetClump(const RpAtomic * atomic)
{
    RWAPIFUNCTION(RWSTRING("RpAtomicGetClump"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(atomic);
    RWASSERTISTYPE(atomic, rpATOMIC);

    RWRETURN(atomic->clump);
}

/**
 * \ingroup rpatomic
 * \ref RpAtomicGetBoundingSphere is used to retrieve the specified
 * atomic's bounding sphere. This is the interpolated sphere obtained from
 * the geometry's morph target's bounding spheres based upon the current
 * interpolation value and scale. The bounding sphere is generally only
 * valid after the atomic has been instanced just prior to rendering.
 *
 * Initially the atomic's bounding sphere equals the bounding sphere of
 * the first morph target in the atomic's geometry.
 * Hence, the atomic's bounding* sphere is only valid if the morph
 * target's sphere has been set (this must be done explicitly
 * using \ref RpMorphTargetCalcBoundingSphere
 * and \ref RpMorphTargetSetBoundingSphere).
 * The bounding sphere returned by this function is specified in object-space.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * Initially, the atomic's bounding sphere equals the bounding sphere of the
 * first morph target in the atomic's geometry.
 *
 * \param atomic  Pointer to the atomic.
 *
 * \return Returns pointer to the atomic's bounding sphere if successful
 * or NULL if there is an error.
 *
 * \see RpAtomicGetGeometry
 * \see RpAtomicSetGeometry
 * \see RpAtomicGetInterpolator
 * \see RpAtomicGetFlags
 * \see RpAtomicSetFlags
 * \see RpAtomicGetFrame
 * \see RpAtomicSetFrame
 * \see RpAtomicGetWorld
 * \see RpWorldAddAtomic
 * \see RpAtomicGetRenderCallBack
 * \see RpAtomicSetRenderCallBack
 * \see RpWorldPluginAttach
 *
 */
RwSphere           *
RpAtomicGetBoundingSphere(RpAtomic * atomic)
{
    RWAPIFUNCTION(RWSTRING("RpAtomicGetBoundingSphere"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(atomic);
    RWASSERTISTYPE(atomic, rpATOMIC);

    if (atomic->interpolator.flags & rpINTERPOLATORDIRTYSPHERE)
    {
        _rpAtomicResyncInterpolatedSphere(atomic);
    }

    RWRETURN(&atomic->boundingSphere);
}

#endif /* ( defined(RWDEBUG) || defined(RWSUPPRESSINLINE) ) */

/**
 * \ingroup rplight
 * \ref RpLightGetClump is used to retrieve the specified light's
 * parent clump, if one exists.
 *
 * The world plugin must be attached before using this function.
 *
 * \param light  Pointer to the light.
 *
 * \return Returns pointer to the parent clump if successful or NULL if there
 * is an error or if the light has no parent clump.
 *
 * \see RpClumpAddLight
 * \see RpClumpRemoveLight
 * \see RpLightGetFrame
 * \see RpLightSetFrame
 * \see RpWorldPluginAttach
 *
 */
RpClump            *
RpLightGetClump(const RpLight * light)
{
    const RpClumpLightExt *lightExt = CLUMPLIGHTEXTFROMCONSTLIGHT(light);

    RWAPIFUNCTION(RWSTRING("RpLightGetClump"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(light);
    RWASSERTISTYPE(light, rpLIGHT);

    RWRETURN(lightExt->clump);
}

/**
 * \ingroup rpcamera
 * \ref RwCameraGetClump is used to retrieve the specified camera's
 * parent clump, if it is attached to one.
 *
 * The world plugin must be attached before using this function.
 *
 * \param camera  Pointer to the camera.
 *
 * \return Returns pointer to the parent clump if successful or NULL if there
 * is an error or if the camera has no parent clump.
 *
 * \see RpClumpAddCamera
 * \see RpClumpRemoveCamera
 * \see RwCameraGetFrame
 * \see RwCameraSetFrame
 * \see RwCameraGetWorld
 * \see RpWorldAddCamera
 * \see RpWorldPluginAttach
 *
 */
RpClump            *
RwCameraGetClump(const RwCamera * camera)
{
    const RpClumpCameraExt *cameraExt = CLUMPCAMERAEXTFROMCONSTCAMERA(camera);

    RWAPIFUNCTION(RWSTRING("RwCameraGetClump"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);

    RWRETURN(cameraExt->clump);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                                  Cloning

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rpatomic
 * \ref RpAtomicClone is used to duplicate the specified atomic. This
 * function creates and initializes a new atomic.
 *
 * This function will copy the original atomic's flags, interpolator,
 * bounding sphere, render callback function and
 * pipeline to the new one. If there are any plugins registered with
 * the atomic then the associated plugin data is also copied to the new
 * atomic using the registered copy constructor function. Note that the
 * new atomic is created without a frame.
 * 
 * The original atomic's referenced geometry is not copied when 
 * RpAtomicClone is called. Instead the new cloned atomic references the 
 * same RpGeometry.
 *
 * The world plugin must be attached before using this function.
 *
 * \param atomic  Pointer to the atomic.
 *
 * \return Returns pointer to the new atomic if successful or NULL if
 * there is an error.
 *
 * \see RpAtomicCreate
 * \see RpAtomicSetFrame
 * \see RpClumpClone
 * \see RwFrameCloneHierarchy
 * \see RpAtomicRegisterPlugin
 * \see RpWorldPluginAttach
 *
 */
RpAtomic *
RpAtomicClone(RpAtomic *atomic)
{
    RpAtomic *newAtomic;

    RWAPIFUNCTION(RWSTRING("RpAtomicClone"));

    RWASSERT(clumpModule.numInstances);
    RWASSERT(atomic);
    RWASSERTISTYPE(atomic, rpATOMIC);

    /* Get a shell to duplicate into */
    newAtomic = RpAtomicCreate();
    if (!newAtomic)
    {
        RWRETURN((RpAtomic *)NULL);
    }

    /* Duplicate the type */
    rwObjectCopy(newAtomic, atomic);

    /* Copy the flags */
    RpAtomicSetFlags(newAtomic, RpAtomicGetFlags(atomic));

    /* It's not attached to a RwFrame as yet.
     * [NOTE: yes this is already done by RpAtomicCreate(),
     *  we're just being explicitly clear about it, okay?] */
    RpAtomicSetFrame(newAtomic, (RwFrame *)NULL);

    RpAtomicSetGeometry(newAtomic, atomic->geometry, 0);
    newAtomic->boundingSphere = atomic->boundingSphere;
    newAtomic->renderCallBack = atomic->renderCallBack;
    newAtomic->pipeline = atomic->pipeline;

    /* NOTE: renderFrame gets initialised by WorldInitAtomicExt() */

    /* Interpolation info */
    newAtomic->interpolator = atomic->interpolator;

    /* Copy over plugin info */
    rwPluginRegistryCopyObject(&atomicTKList, newAtomic, atomic);

    RWRETURN(newAtomic);
}

/**
 * \ingroup rpclump
 * \ref RpClumpClone is used to duplicate the specified clump. This
 * function creates and initializes a new clump and copies the old clump's
 * atomics and their frame hierarchy to the new one. The frustum callback
 * function is also copied. If there are any plugins registered with the
 * clump then the associated plugin data is also copied to the new clump
 * using the registered copy constructor function.
 *
 * RpClumpClone does not guarantee to preserve the order of the atomics.
 * If the order is important, contact Developer Relations.
 *
 * Cloning of any lights and cameras attached to the clump is not currently
 * supported. The new clump will have empty light and camera lists.
 *
 * Note that each new atomic created using this function will reference the
 * same geometry as the original and the reference count on the geometry will
 * be incremented. When the copy is destroyed the referenced geometry will not
 * be destroyed as it will still be referenced by the original atomic.
 *
 * Note that if the source clump has been added to a world then the cloned
 * clump will be automatically added to the same world.
 *
 * The world plugin must be attached before using this function.
 *
 * \param clump  Pointer to the clump that is to be cloned.
 *
 * \return Returns a pointer to the new clump if successful or NULL if
 * there is an error.
 *
 * \see RpClumpCreate
 * \see RpClumpDestroy
 * \see RpClumpCreateSpace
 * \see RpClumpGetCallBack
 * \see RpClumpSetCallBack
 * \see RpAtomicClone
 * \see RwFrameCloneHierarchy
 * \see RpClumpRegisterPlugin
 * \see RpWorldPluginAttach
 *
 */

RpClump            *
RpClumpClone(RpClump * clump)
{
    RpClumpCloneStatus  clumpCloneStatus;
    RwFrame            *clone;

    RWAPIFUNCTION(RWSTRING("RpClumpClone"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(clump);
    RWASSERTISTYPE(clump, rpCLUMP);

    clumpCloneStatus.oldClump = clump;

    /* Create the clump */
    if (!(clumpCloneStatus.newClump = RpClumpCreate()))
    {
        RWRETURN((RpClump *)NULL);
    }

    /* Clone the tree */
    clone =
        _rwFrameCloneAndLinkClones(RpClumpGetFrame(clumpCloneStatus.oldClump));
    (void)RpClumpSetFrame(clumpCloneStatus.newClump, clone);

    /* Check if alloced ok */
    if (!RpClumpGetFrame(clumpCloneStatus.newClump))
    {
        RpClumpDestroy(clumpCloneStatus.newClump);

        RWRETURN((RpClump *)NULL);
    }

    clumpCloneStatus.success = TRUE;
    RpClumpForAllAtomics(clumpCloneStatus.oldClump,
                         ClumpAtomicCloneAndAttach, &clumpCloneStatus);

    /* Currently do not clone lights and cameras */

    _rwFramePurgeClone(RpClumpGetFrame(clumpCloneStatus.oldClump));

    if (!clumpCloneStatus.success)
    {
        RpClumpDestroy(clumpCloneStatus.newClump);

        RWRETURN((RpClump *)NULL);
    }

    /* Copy over the callback */
    clumpCloneStatus.newClump->callback = clumpCloneStatus.oldClump->callback;

    /* Copy the clump's plugin data */
    rwPluginRegistryCopyObject(&clumpTKList,
                               clumpCloneStatus.newClump,
                               clumpCloneStatus.oldClump);

    /* All done */
    RWRETURN(clumpCloneStatus.newClump);
}

/**
 * \ingroup rpclump
 * \ref RpClumpSetCallBack is used to define a callback function for the
 * specified clump.
 *
 * The callback function is executed from \ref RwCameraForAllClumpsInFrustum
 * when the clump enters the camera's frustum. This allows an application
 * to place animation code in the callback function and to only process
 * this when the clump is on screen.
 *
 * To return to the default frustum callback use NULL in the callback
 * argument.
 *
 * The world plugin must be attached before using this function.
 *
 * \param clump  Pointer to the clump.
 * \param callback  Pointer to the clump callback function.
 *
 * \return Returns pointer to the clump if successful or NULL if there is
 * an error.
 *
 * \see RpClumpGetCallBack
 * \see RwCameraForAllClumpsInFrustum
 * \see RpWorldPluginAttach
 *
 */
RpClump            *
RpClumpSetCallBack(RpClump * clump, RpClumpCallBack callback)
{
    RWAPIFUNCTION(RWSTRING("RpClumpSetCallBack"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(clump);
    RWASSERTISTYPE(clump, rpCLUMP);

    if (callback != NULL)
    {
        clump->callback = callback;
    }
    else
    {
        clump->callback = ClumpCallBack;
    }

    RWRETURN(clump);
}

/**
 * \ingroup rpclump
 * \ref RpClumpGetCallBack is used to determine the callback function
 * of the specified clump. Note that if the default frustum callback
 * is operational \ref RpClumpGetCallBack will return NULL.
 *
 * The world plugin must be attached before using this function.
 *
 * \param clump  Pointer to the clump.
 *
 * \return Returns pointer the clump's callback function if successful or
 * NULL if there is an error or if the default frustum callback is operational.
 *
 * \see RpClumpSetCallBack
 * \see RwCameraForAllClumpsInFrustum
 * \see RpWorldPluginAttach
 */
RpClumpCallBack
RpClumpGetCallBack(const RpClump * clump)
{
    RWAPIFUNCTION(RWSTRING("RpClumpGetCallBack"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(clump);
    RWASSERTISTYPE(clump, rpCLUMP);

    if (clump->callback == ClumpCallBack)
    {
        RWRETURN((RpClump * (*)(RpClump *, void *))NULL);
    }
    else
    {
        RWRETURN(clump->callback);
    }
}

/**
 * \ingroup rpclump
 * \ref RpClumpCreate is used to create and initialize a new clump.
 * Clumps are used as containers for groups of atomics and their frame
 * hierarchy.
 *
 * The world plugin must be attached before using this function.
 *
 * \return Returns a pointer to the new clump if successful or NULL if
 * there is an error.
 *
 * \see RpClumpDestroy
 * \see RpClumpClone
 * \see RpClumpCreateSpace
 * \see RpClumpSetFrame
 * \see RpClumpGetFrame
 * \see RpClumpAddAtomic
 * \see RpClumpRemoveAtomic
 * \see RpClumpForAllAtomics
 * \see RpClumpGetNumAtomics
 * \see RpClumpRender
 * \see RpClumpGetWorld
 * \see RpWorldAddClump
 * \see RpWorldPluginAttach
 *
 */

RpClump            *
RpClumpCreate(void)
{
    RpClump            *clump;

    RWAPIFUNCTION(RWSTRING("RpClumpCreate"));
    RWASSERT(clumpModule.numInstances);

    RWASSERT(RWCLUMPGLOBAL(clumpFreeList));
    clump = (RpClump *) RwFreeListAlloc(RWCLUMPGLOBAL(clumpFreeList),
                                      rwID_CLUMP | rwMEMHINTDUR_EVENT);
    if (!clump)
    {
        RWRETURN(clump);
    }

    rwObjectInitialize(clump, rpCLUMP, 0);
    RpClumpSetFrame(clump, NULL);

    /* Contains nothing */
    rwLinkListInitialize(&clump->atomicList);
    rwLinkListInitialize(&clump->lightList);
    rwLinkListInitialize(&clump->cameraList);

    /* Its not in the world */
    rwLLLinkInitialize(&clump->inWorldLink);

    /* Set the callback */
    RpClumpSetCallBack(clump, (RpClumpCallBack)NULL);

    /* Initialize memory allocated to toolkits */
    rwPluginRegistryInitObject(&clumpTKList, clump);

    /* All Done */
    RWRETURN(clump);
}

/**
 * \ingroup rpclump
 * \ref RpClumpDestroy is used to destroy the specified clump
 * including all atomics and frame hierarchies attached to it but it will not
 * destroy the frames of the atomics in the clump if those frames are not in
 * the frame hierarchy.
 *
 * \ref RpClumpDestroy also destroys clumps that are attached to lights and
 * cameras.
 *
 * Each geometry referenced by atomics in the clump will only be destroyed if
 * their reference counts are zero.
 *
 * \note If the clump was acquired from a binary stream and the clump
 * contains geometries, then the geometries are automatically destroyed with
 * \ref RpClumpDestroy. Otherwise, it is assumed the geometries were created
 * by the application and must be explicitly destroyed.
 *
 * The world plugin must be attached before using this function.
 *
 * \param clump  Pointer to the clump.
 *
 * \return Returns TRUE if successful or FALSE if there is an error
 *
 * \see RpClumpCreate
 * \see RpAtomicDestroy
 * \see RpGeometryDestroy
 * \see RpWorldPluginAttach
 *
 */
RwBool
RpClumpDestroy(RpClump * clump)
{
    RwFrame            *frame;

    RWAPIFUNCTION(RWSTRING("RpClumpDestroy"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(clump);
    RWASSERTISTYPE(clump, rpCLUMP);

    /* De-init the clump plugin registered memory */
    rwPluginRegistryDeInitObject(&clumpTKList, clump);

    RpClumpForAllAtomics(clump, DestroyClumpAtomic, NULL);
    RpClumpForAllLights(clump, DestroyClumpLight, NULL);
    RpClumpForAllCameras(clump, DestroyClumpCamera, NULL);

    /* Destroy the frame hierarchy if one exists */
    frame = RpClumpGetFrame(clump);
    if (frame)
    {
        RwFrameDestroyHierarchy(frame);
    }

    /* Destroy the clump */
    RwFreeListFree(RWCLUMPGLOBAL(clumpFreeList), clump);

    RWRETURN(TRUE);
}

/**
 * \ingroup rpclump
 * \ref RpClumpAddAtomic is used to add the specified atomic to the
 * given clump. It is assumed that the atomic has not been previously added
 * to a world (it is not necessary to add an atomic to a world if the atomic
 * is already attached to a clump). The frame hierarchy defined in the clump
 * determines where in the clump the atomic resides.
 *
 * The world plugin must be attached before using this function.
 *
 * \param clump  Pointer to the clump.
 * \param atomic  Pointer to the atomic.
 *
 * \return Returns pointer to the clump if successful or NULL if there
 * is an error.
 *
 * \see RpClumpRemoveAtomic
 * \see RpAtomicSetFrame
 * \see RpClumpSetFrame
 * \see RwFrameCreate
 * \see RpWorldAddAtomic
 * \see RpWorldPluginAttach
 *
 */
RpClump            *
RpClumpAddAtomic(RpClump * clump, RpAtomic * atomic)
{
    RWAPIFUNCTION(RWSTRING("RpClumpAddAtomic"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(clump);
    RWASSERTISTYPE(clump, rpCLUMP);
    RWASSERT(atomic);
    RWASSERTISTYPE(atomic, rpATOMIC);

    /* It is assumed the atomic is NOT in the world although the
     * clump might be
     */

    rwLinkListAddLLLink(&clump->atomicList, &atomic->inClumpLink);
    atomic->clump = clump;

    RWRETURN(clump);
}

/**
 * \ingroup rpclump
 * \ref RpClumpRemoveAtomic is used to detach the specified atomic
 * from the given clump. Note that this function does not destroy the
 * atomic and it is assumed that the clump does not belong to any worlds.
 * Note also that the atomic's frame, if any, remains attached to the
 * clump's frame hierarchy.
 *
 * The world plugin must be attached before using this function.
 *
 * \param clump  Pointer to the clump containing the atomic.
 * \param atomic  Pointer to the atomic to remove.
 *
 * \return Returns pointer to the clump if successful or NULL if
 * there is an error.
 *
 * \see RpClumpAddAtomic
 * \see RpClumpForAllAtomics
 * \see RpClumpGetNumAtomics
 * \see RpAtomicSetFrame
 * \see RwFrameRemoveChild
 * \see RpWorldPluginAttach
 *
 */
RpClump            *
RpClumpRemoveAtomic(RpClump * clump, RpAtomic * atomic)
{
    RWAPIFUNCTION(RWSTRING("RpClumpRemoveAtomic"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(clump);
    RWASSERTISTYPE(clump, rpCLUMP);
    RWASSERT(atomic);
    RWASSERTISTYPE(atomic, rpATOMIC);

    /* NOTE !!!! - this assumes the clump is NOT IN THE WORLD */
    rwLinkListRemoveLLLink(&atomic->inClumpLink);
    atomic->clump = (RpClump *)NULL;

    RWRETURN(clump);
}

/**
 * \ingroup rpclump
 * \ref RpClumpAddLight is used to add the specified light to the
 * given clump. It is assumed that the light has not been previously added
 * to a world (it is not necessary to add a light to a world if the light
 * is already attached to a clump).
 *
 * The world plugin must be attached before using this function.
 *
 * \param clump  Pointer to the clump.
 * \param light  Pointer to the light.
 *
 * \return Returns pointer to the clump if successful or NULL if there
 * is an error.
 *
 * \see RpClumpRemoveLight
 * \see RpLightSetFrame
 * \see RpClumpSetFrame
 * \see RwFrameCreate
 * \see RpWorldAddLight
 * \see RpWorldPluginAttach
 *
 */
RpClump            *
RpClumpAddLight(RpClump * clump, RpLight * light)
{
    RpClumpLightExt *lightExt = CLUMPLIGHTEXTFROMLIGHT(light);

    RWAPIFUNCTION(RWSTRING("RpClumpAddLight"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(clump);
    RWASSERTISTYPE(clump, rpCLUMP);
    RWASSERT(light);
    RWASSERTISTYPE(light, rpLIGHT);

    /* It is assumed the light is NOT in the world although the
     * clump might be
     */

    rwLinkListAddLLLink(&clump->lightList, &lightExt->inClumpLink);
    lightExt->clump = clump;

    RWRETURN(clump);
}

/**
 * \ingroup rpclump
 * \ref RpClumpRemoveLight is used to detach the specified light
 * from the given clump. Note that this function does not destroy the
 * light and it is assumed that the clump does not belong to any worlds.
 * Note also that the light's frame, if any, remains attached to the
 * clump's frame hierarchy.
 *
 * The world plugin must be attached before using this function.
 *
 * \param clump  Pointer to the clump containing the light.
 * \param light  Pointer to the light to remove.
 *
 * \return Returns pointer to the clump if successful or NULL if
 * there is an error.
 *
 * \see RpClumpAddLight
 * \see RpClumpForAllLights
 * \see RpClumpGetNumLights
 * \see RpLightSetFrame
 * \see RwFrameRemoveChild
 * \see RpWorldPluginAttach
 *
 */
RpClump            *
RpClumpRemoveLight(RpClump * clump, RpLight * light)
{
    RpClumpLightExt *lightExt = CLUMPLIGHTEXTFROMLIGHT(light);

    RWAPIFUNCTION(RWSTRING("RpClumpRemoveLight"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(clump);
    RWASSERTISTYPE(clump, rpCLUMP);
    RWASSERT(light);
    RWASSERTISTYPE(light, rpLIGHT);

    /* NOTE !!!! - this assumes the clump is NOT IN THE WORLD */
    /* We should find a way to ASSERT about this (perhaps clumps
     * should be used *only* as handy containers - so we should
     * remove RpClumpRender and RpWorldAddClump altogether) */
    rwLinkListRemoveLLLink(&lightExt->inClumpLink);
    rwLLLinkInitialize(&lightExt->inClumpLink);
    lightExt->clump = (RpClump *)NULL;

    RWRETURN(clump);
}

/**
 * \ingroup rpclump
 * \ref RpClumpAddCamera is used to add the specified camera to the
 * given clump. It is assumed that the camera has not been previously added
 * to a world (it is not necessary to add an camera to a world if the camera
 * is already attached to a clump). The frame hierarchy defined in the clump
 * determines where in the clump the camera resides.
 *
 * The world plugin must be attached before using this function.
 *
 * \param clump  Pointer to the clump.
 * \param camera  Pointer to the camera.
 *
 * \return Returns pointer to the clump if successful or NULL if there
 * is an error.
 *
 * \see RpClumpRemoveCamera
 * \see RwCameraSetFrame
 * \see RpClumpSetFrame
 * \see RwFrameCreate
 * \see RpWorldAddCamera
 * \see RpWorldPluginAttach
 *
 */
RpClump            *
RpClumpAddCamera(RpClump * clump, RwCamera * camera)
{
    RpClumpCameraExt *cameraExt = CLUMPCAMERAEXTFROMCAMERA(camera);

    RWAPIFUNCTION(RWSTRING("RpClumpAddCamera"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(clump);
    RWASSERTISTYPE(clump, rpCLUMP);
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);

    /* It is assumed the camera is NOT in the world although the
     * clump might be
     */

    rwLinkListAddLLLink(&clump->cameraList, &cameraExt->inClumpLink);
    cameraExt->clump = clump;

    RWRETURN(clump);
}

/**
 * \ingroup rpclump
 * \ref RpClumpRemoveCamera is used to detach the specified camera
 * from the given clump. Note that this function does not destroy the
 * camera and it is assumed that the clump does not belong to any worlds.
 * Note also that the camera's frame, if any, remains attached to the
 * clump's frame hierarchy.
 *
 * The world plugin must be attached before using this function.
 *
 * \param clump  Pointer to the clump containing the camera.
 * \param camera  Pointer to the camera to remove.
 *
 * \return Returns pointer to the clump if successful or NULL if
 * there is an error.
 *
 * \see RpClumpAddCamera
 * \see RpClumpForAllCameras
 * \see RpClumpGetNumCameras
 * \see RwCameraSetFrame
 * \see RwFrameRemoveChild
 * \see RpWorldPluginAttach
 *
 */
RpClump            *
RpClumpRemoveCamera(RpClump * clump, RwCamera * camera)
{
    RpClumpCameraExt *cameraExt = CLUMPCAMERAEXTFROMCAMERA(camera);

    RWAPIFUNCTION(RWSTRING("RpClumpRemoveCamera"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(clump);
    RWASSERTISTYPE(clump, rpCLUMP);
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);

    /* NOTE !!!! - this assumes the clump is NOT IN THE WORLD */
    rwLinkListRemoveLLLink(&cameraExt->inClumpLink);
    rwLLLinkInitialize(&cameraExt->inClumpLink);
    cameraExt->clump = (RpClump *)NULL;

    RWRETURN(clump);
}

/**
 * \ingroup rpatomic
 * \ref RpAtomicStreamGetSize is used to  determine the size in bytes
 * of the binary representation of the given atomic. This is used in
 * the binary chunk header to indicate the size of the chunk. The size does
 * not include the size of the chunk header.
 *
 * The world plugin must be attached before using this function.
 *
 * \param atomic  Pointer to the atomic.
 *
 * \return Returns a RwUInt32 value equal to the chunk size (in bytes) of
 * the atomic or zero if there is an error.
 *
 * \see RpAtomicStreamRead
 * \see RpAtomicStreamWrite
 * \see RpWorldPluginAttach
 *
 */
RwUInt32
RpAtomicStreamGetSize(RpAtomic * atomic)
{
    RwUInt32            size;

    RWAPIFUNCTION(RWSTRING("RpAtomicStreamGetSize"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(atomic);
    RWASSERTISTYPE(atomic, rpATOMIC);

    size = AtomicStreamGetSizeActual(atomic) + rwCHUNKHEADERSIZE;

    /* Add the size of the included chunk */
    size += RpGeometryStreamGetSize(atomic->geometry) + rwCHUNKHEADERSIZE;

    /* Add the size of the atomic extension chunks */
    size += rwPluginRegistryGetSize(&atomicTKList, atomic) + rwCHUNKHEADERSIZE;

    RWRETURN(size);
}

/**
 * \ingroup rpclump
 * \ref RpClumpStreamGetSize is used to  determine the size in bytes
 * of the binary representation of the given clump. This value is used in
 * the binary chunk header to indicate the size of the chunk. The size does
 * not include the size of the chunk header.
 *
 * The world plugin must be attached before using this function.
 *
 * \param clump  Pointer to the clump.
 *
 * \return Returns a RwUInt32 value equal to the chunk size (in bytes) of
 * the clump or zero if there is an error.
 *
 * \see RpClumpStreamRead
 * \see RpClumpStreamWrite
 * \see RpWorldPluginAttach
 *
 */
RwUInt32
RpClumpStreamGetSize(RpClump * clump)
{
    RwUInt32            size;
    rwFrameList         fl;
    rpGeometryList      gl;

    RWAPIFUNCTION(RWSTRING("RpClumpStreamGetSize"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(clump);
    RWASSERTISTYPE(clump, rpCLUMP);

    /* Get the size of the clump (without extension chunks) */
    size = sizeof(_rpClump) + rwCHUNKHEADERSIZE;

    /* Create a frame list */
    if (!rwFrameListInitialize(&fl, RpClumpGetFrame(clump)))
    {
        RWRETURN(0);
    }
    size += rwFrameListStreamGetSize(&fl) + rwCHUNKHEADERSIZE;
    rwFrameListDeinitialize(&fl);

    gl.numGeoms = 0;

    /* Create a geometry list */
    if (!GeometryListInitialize(&gl, clump))
    {
        RWRETURN(0);
    }
    size += GeometryListStreamGetSize(&gl) + rwCHUNKHEADERSIZE;
    GeometryListDeinitialize(&gl);

    /* Iterate over the objects */
    RpClumpForAllAtomics(clump, ClumpAtomicAddSize, &size);
    RpClumpForAllLights(clump, ClumpLightAddSize, &size);
    RpClumpForAllCameras(clump, ClumpCameraAddSize, &size);

    /* Add the size of the clump extension chunks */
    size += rwPluginRegistryGetSize(&clumpTKList, clump) + rwCHUNKHEADERSIZE;

    RWRETURN(size);
}

/**
 * \ingroup rpatomic
 * \ref RpAtomicStreamWrite is used to write the specified atomic to the
 * given binary stream. The atomic's frame is not written out with this 
 * function. 
 *
 * \note The stream will have been opened prior to this function call.
 *
 * The world plugin must be attached before using this function.
 *
 * \param atomic  Pointer to the atomic.
 * \param stream  Pointer to the stream.
 *
 * \return Returns pointer to the atomic if successful or NULL if
 * there is an error.
 *
 * \see RpAtomicStreamRead
 * \see RwStreamOpen
 * \see RwStreamClose
 * \see RpWorldPluginAttach
 *
 */

RpAtomic           *
RpAtomicStreamWrite(RpAtomic * atomic, RwStream * stream)
{
    RpClumpStreamWriteStatus streamStatus;

    RWAPIFUNCTION(RWSTRING("RpAtomicStreamWrite"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(atomic);
    RWASSERTISTYPE(atomic, rpATOMIC);
    RWASSERT(stream);

    streamStatus.success = TRUE;
    streamStatus.stream = stream;
    streamStatus.fl.numFrames = 0;
    streamStatus.gl.numGeoms = 0;
    RWRETURN(ClumpAtomicStreamWrite(atomic, (void *) &streamStatus));
}

/**
 * \ingroup rpclump
 * \ref RpClumpStreamWrite is used to write the specified clump to the
 * given binary stream. This function will assert if given a clump without
 * a frame.
 *
 * \note The stream will have been opened prior to this function call.
 *
 * The world plugin must be attached before using this function.
 *
 * \param clump  Pointer to the clump.
 * \param stream  Pointer to the stream.
 *
 * \return Returns pointer to the clump if successful or NULL if
 * there is an error.
 *
 * \see RpClumpStreamRead
 * \see RwStreamOpen
 * \see RwStreamClose
 * \see RpWorldPluginAttach
 *
 */
RpClump            *
RpClumpStreamWrite(RpClump * clump, RwStream * stream)
{
    _rpClump            cl;
    RpClumpStreamWriteStatus streamStatus;

    RWAPIFUNCTION(RWSTRING("RpClumpStreamWrite"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(clump);
    RWASSERTISTYPE(clump, rpCLUMP);
    RWASSERT(stream);

    /* This writes a header for the clump
     * without the clump extension data */
    if (!RwStreamWriteChunkHeader
        (stream, rwID_CLUMP, RpClumpStreamGetSize(clump)))
    {
        RWRETURN((RpClump *)NULL);
    }

    /* wrap structured data */
    if (!RwStreamWriteChunkHeader(stream, rwID_STRUCT, sizeof(_rpClump)))
    {
        RWRETURN((RpClump *)NULL);
    }

    /* Fill it */
    cl.numAtomics = RpClumpGetNumAtomics(clump);
    cl.numLights  = RpClumpGetNumLights(clump);
    cl.numCameras = RpClumpGetNumCameras(clump);

    /* Convert it */
    (void)RwMemLittleEndian32(&cl, sizeof(cl));

    /* Write it */
    if (!RwStreamWrite(stream, &cl, sizeof(cl)))
    {
        RWRETURN((RpClump *)NULL);
    }

    /* Create a frame list */
    if (!rwFrameListInitialize(&streamStatus.fl, RpClumpGetFrame(clump)))
    {
        RWRETURN(FALSE);
    }

    /* Write a frame list */
    if (!rwFrameListStreamWrite(&streamStatus.fl, stream))
    {
        rwFrameListDeinitialize(&streamStatus.fl);
        RWRETURN((RpClump *)NULL);
    }

    streamStatus.gl.numGeoms = 0;

    /* Create a geometry list */
    if (!GeometryListInitialize(&streamStatus.gl, clump))
    {
        RWRETURN(FALSE);
    }

    /* Write a frame list */
    if (!GeometryListStreamWrite(&streamStatus.gl, stream))
    {
        GeometryListDeinitialize(&streamStatus.gl);
        RWRETURN((RpClump *)NULL);
    }

    /* Iterate over the objects - assume success until it fails */
    streamStatus.success = TRUE;
    streamStatus.stream = stream;

    RpClumpForAllAtomics(clump, ClumpAtomicStreamWrite, &streamStatus);

    /* Write any lights */
    if ((cl.numLights > 0) && streamStatus.success)
    {
        RpClumpForAllLights(clump, ClumpLightStreamWrite, &streamStatus);
    }

    /* Write out any cameras */
    if ((cl.numCameras > 0) && streamStatus.success)
    {
        RpClumpForAllCameras(clump, ClumpCameraStreamWrite, &streamStatus);
    }

    /* Don't need the geometry list anymore */
    GeometryListDeinitialize(&streamStatus.gl);

    /* Don't need the frame list anymore */
    rwFrameListDeinitialize(&streamStatus.fl);

    if (!streamStatus.success)
    {
        RWRETURN((RpClump *)NULL);
    }

    /* Clump extension data */
    if (!rwPluginRegistryWriteDataChunks(&clumpTKList, stream, clump))
    {
        /* Failed to write extension data */
        RWRETURN((RpClump *)NULL);
    }

    RWRETURN(clump);
}

/**
 * \ingroup rpatomic
 * \ref RpAtomicStreamRead is used to read an atomic object from the
 * specified binary stream. Note that prior to this function call a binary
 * atomic chunk must be found in the stream using the
 * \ref RwStreamFindChunk API function.
 *
 * The world plugin must be attached before using this function.
 *
 * The sequence to locate and read an atomic from a binary stream connected
 * to a disk file is as follows:
 * \code
   RwStream *stream;
   RpAtomic *newAtomic;

   stream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, "mybinary.xxx");
   if( stream )
   {
       if( RwStreamFindChunk(stream, rwID_ATOMIC, NULL, NULL) )
       {
           newAtomic = RpAtomicStreamRead(stream);
       }

       RwStreamClose(stream, NULL);
   }
  \endcode
 *
 * \param stream  Pointer to the stream.
 *
 * \return Returns pointer to the atomic if successful
 * or NULL if there is an error.
 *
 * \see RpAtomicStreamWrite
 * \see RpAtomicStreamGetSize
 * \see RwStreamOpen
 * \see RwStreamClose
 * \see RwStreamFindChunk
 * \see RpWorldPluginAttach
 *
 */
RpAtomic           *
RpAtomicStreamRead(RwStream * stream)
{
    rwFrameList         fl;
    rpGeometryList      gl;

    RWAPIFUNCTION(RWSTRING("RpAtomicStreamRead"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(stream);

    fl.numFrames = 0;
    gl.numGeoms = 0;

    RWRETURN(ClumpAtomicStreamRead(stream, &fl, &gl));
}

/**
 * \ingroup rpclump
 * \ref RpClumpStreamRead is used to read a clump
 * from the specified binary
 * stream. Note that prior to this function call, a binary clump chunk must
 * be found in the stream using the \ref RwStreamFindChunk API function.
 *
 * The world plugin must be attached before using this function.
 *
 * The sequence to locate and read a clump from a binary stream is
 * as follows:
 * \code
   RwStream *stream;
   RpClump *newClump;

   stream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, "mybinary.xxx");
   if( stream )
   {
       if( RwStreamFindChunk(stream, rwID_CLUMP, NULL, NULL) )
       {
           newClump = RpClumpStreamRead(stream);
       }

       RwStreamClose(stream, NULL);
   }
  \endcode
 *
 * \param stream  Pointer to the stream.
 *
 * \return Returns pointer to the clump if successful
 * or NULL if there is an error.
 *
 * \see RpClumpStreamWrite
 * \see RpClumpStreamGetSize
 * \see RwStreamOpen
 * \see RwStreamClose
 * \see RwStreamFindChunk
 * \see RpWorldPluginAttach
 */
RpClump            *
RpClumpStreamRead(RwStream * stream)
{
    RwBool              status;
    RwUInt32            size;
    RwUInt32            version;

    RWAPIFUNCTION(RWSTRING("RpClumpStreamRead"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(stream);

    status = RwStreamFindChunk(stream, rwID_STRUCT, &size, &version);

    if (!status)
    {
        RWERROR((E_RW_READ));
        RWRETURN((RpClump *)NULL);
    }

    if ((version >= rwLIBRARYBASEVERSION) && (version <= rwLIBRARYCURRENTVERSION))
    {
        RpClump            *clump;
        RpAtomic           *atom;
        _rpClump            cl;
        rwFrameList         fl;
        rpGeometryList      gl;
        RwInt32             i;
        RwUInt32            chunkversion;

        status = (sizeof(cl) == RwStreamRead(stream, &cl, sizeof(cl)) );
        if (!status)
        {
            RWERROR((E_RW_READ));
            RWRETURN((RpClump *)NULL);
        }

        (void)RwMemNative32(&cl, sizeof(cl));

        clump = RpClumpCreate();
        if (!clump)
        {
            RWRETURN((RpClump *)NULL);
        }

        /* Read the frame list */
        status = RwStreamFindChunk(stream, (RwUInt32)rwID_FRAMELIST,
                                   (RwUInt32 *)NULL, &chunkversion);
        if (!status)
        {
            RpClumpDestroy(clump);
            RWERROR((E_RW_READ));
            RWRETURN((RpClump *)NULL);
        }

        status = (NULL != rwFrameListStreamRead(stream, &fl));
        if (!status)
        {
            RpClumpDestroy(clump);
            RWERROR((E_RW_READ));
            RWRETURN((RpClump *)NULL);
        }

        /* Set the frame root */
        RpClumpSetFrame(clump, fl.frames[0]);

        /* Read the geometry list */
        status = RwStreamFindChunk (stream, (RwUInt32)rwID_GEOMETRYLIST,
                                    (RwUInt32 *)NULL, &chunkversion);
        if (!status)
        {
            rwFrameListDeinitialize(&fl);
            RpClumpDestroy(clump);
            RWERROR((E_RW_READ));
            RWRETURN((RpClump *)NULL);
        }

        status = (NULL != GeometryListStreamRead(stream, &gl));
        if (!status)
        {
            rwFrameListDeinitialize(&fl);
            RpClumpDestroy(clump);
            RWERROR((E_RW_READ));
            RWRETURN((RpClump *)NULL);
        }

        /* Iterate over the atomics */
        for (i = 0; i < cl.numAtomics; i++)
        {
            status = RwStreamFindChunk(stream, (RwUInt32)rwID_ATOMIC,
                                       (RwUInt32 *)NULL, &version);
            if (status)
            {
                atom = ClumpAtomicStreamRead(stream, &fl, &gl);
                status = (NULL != atom);
            }

            if (!status)
            {
                GeometryListDeinitialize(&gl);
                rwFrameListDeinitialize(&fl);
                RpClumpDestroy(clump);
                RWERROR((E_RW_READ));
                RWRETURN((RpClump *)NULL);
            }

            /* Add the atomic to the clump */
            RpClumpAddAtomic(clump, atom);
        }

        /* Iterate over the lights */
        for (i = 0; i < cl.numLights; i++)
        {
            RpLight *light;
            RwInt32 frameIndex;

            /* Read the frame index for this light */
            status = RwStreamFindChunk(stream,
                        rwID_STRUCT, (RwUInt32 *)NULL, (RwUInt32 *)NULL)
                 && (RwStreamReadInt32(stream, &frameIndex, sizeof(frameIndex)) != NULL);

            /* Read in the light */
            if (status)
            {
                status = RwStreamFindChunk(stream,
                            rwID_LIGHT, (RwUInt32 *)NULL, (RwUInt32 *)NULL);
            }

            if (status)
            {
                light = RpLightStreamRead(stream);
                status = (light != NULL);
            }

            /* Failure? */
            if (!status)
            {
                GeometryListDeinitialize(&gl);
                rwFrameListDeinitialize(&fl);
                RpClumpDestroy(clump);
                RWERROR((E_RW_READ));
                RWRETURN((RpClump *)NULL);
            }

            /* Add the light to the clump */
            RpLightSetFrame(light, fl.frames[frameIndex]);
            RpClumpAddLight(clump, light);
        }

        /* Iterate over the cameras */
        for (i = 0; i < cl.numCameras; i++)
        {
            RwCamera   *camera;
            RwInt32     frameIndex;

            /* Read the frame index for this camera */
            status = RwStreamFindChunk(stream,
                        rwID_STRUCT, (RwUInt32 *)NULL, (RwUInt32 *)NULL)
                 && (RwStreamReadInt32(stream, &frameIndex, sizeof(frameIndex)) != NULL);

            /* Read in the camera */
            if (status)
            {
                status = RwStreamFindChunk(stream,
                            rwID_CAMERA, (RwUInt32 *)NULL, (RwUInt32 *)NULL);
            }

            if (status)
            {
                camera = RwCameraStreamRead(stream);
                status = (camera != NULL);
            }

            /* Failure? */
            if (!status)
            {
                GeometryListDeinitialize(&gl);
                rwFrameListDeinitialize(&fl);
                RpClumpDestroy(clump);
                RWERROR((E_RW_READ));
                RWRETURN((RpClump *)NULL);
            }

            /* Add the cameras to the clump */
            RwCameraSetFrame(camera, fl.frames[frameIndex]);
            RpClumpAddCamera(clump, camera);
        }

        /* Dont need the geometry list anymore */
        GeometryListDeinitialize(&gl);

        /* Dont need the frame list anymore */
        rwFrameListDeinitialize(&fl);

        /* Clump extension data */
        status = (NULL !=
                  rwPluginRegistryReadDataChunks(&clumpTKList, stream, clump));

        if (!status)
        {
            RpClumpDestroy(clump);
            RWERROR((E_RW_READ));
            RWRETURN((RpClump *)NULL);
        }

        RWRETURN(clump);
    }
    else
    {
        RWERROR((E_RW_BADVERSION));
        RWRETURN((RpClump *)NULL);
    }
}

/*
 * $ref _rpClumpChunkInfoRead extracts
 * Chunk Info data from a RenderWare stream. The data is converted
 * from its original format and inserted into an RpClumpChunkInfo
 * structure. A pointer to this structure is returned on success.
 *
 * $param stream Pointer to the stream from which to read the chunk info.
 * $param clumpChunkInfo  Pointer to an RpClumpChunkInfo object.
 * $param bytesRead  Pointer to a RwInt32 which will be used to return the
 * number of bytes read.
 *
 * $return Returns pointer to RpClumpChunkInfo if successful or NULL
 * if there is an error
 *
 * $see RpClumpStreamRead
 *
 */
RpClumpChunkInfo   *
_rpClumpChunkInfoRead(RwStream * stream,
                      RpClumpChunkInfo * clumpChunkInfo,
                      RwInt32 * bytesRead)
{
    RwUInt32            size, readSize, version;

    RWFUNCTION(RWSTRING("_rpClumpChunkInfoRead"));
    RWASSERT(stream);
    RWASSERT(clumpChunkInfo);

    if (!RwStreamFindChunk(stream, (RwUInt32)rwID_STRUCT, &size,  &version))
    {
        RWRETURN((RpClumpChunkInfo *)NULL);
    }

    readSize = sizeof(RpClumpChunkInfo);

    RWASSERT(size == readSize);

    if (RwStreamRead(stream, clumpChunkInfo, readSize) != readSize)
    {
        RWRETURN((RpClumpChunkInfo *)NULL);
    }

    (void)RwMemNative32(clumpChunkInfo, readSize);

    *bytesRead = size + rwCHUNKHEADERSIZE;

    RWRETURN(clumpChunkInfo);
}

/**
 * \ingroup rpatomic
 * \ref RpAtomicRegisterPlugin is used to register a plugin and
 * reserve some space within an atomic. This must happen after the engine
 * has been initialized but before the engine is opened.
 *
 * The world plugin must be attached before using this function.
 *
 * \param size  A RwInt32 value equal to the size of
 *              the memory block to reserve.
 * \param pluginID  A RwInt32 value equal to the plugin ID
 *                 (must be unique; used to identify binary chunks).
 * \param constructCB  Constructor for the plugin data block.
 * \param destructCB  Destructor for the plugin data block.
 * \param copyCB  Copy constructor for the plugin data block.
 *
 * \return Returns a RwInt32 value equal to the byte offset within the atomic
 * of memory reserved for this plugin or a negative value if there is an error.
 *
 * \see RpAtomicValidatePlugins
 * \see RpAtomicGetPluginOffset
 * \see RpAtomicRegisterPluginStream
 * \see RpAtomicSetStreamAlwaysCallBack
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpAtomicRegisterPlugin(RwInt32 size, RwUInt32 pluginID,
                       RwPluginObjectConstructor constructCB,
                       RwPluginObjectDestructor destructCB,
                       RwPluginObjectCopy copyCB)
{
    RwInt32             plug;

    RWAPIFUNCTION(RWSTRING("RpAtomicRegisterPlugin"));
    RWASSERT(!clumpModule.numInstances);
    RWASSERT(size >= 0);

    /* Everything's cool, so pass it on */
    plug = rwPluginRegistryAddPlugin(&atomicTKList, size, pluginID,
                                     constructCB, destructCB, copyCB);
    RWRETURN(plug);
}

/**
 * \ingroup rpclump
 * \ref RpClumpRegisterPlugin is used to register a plugin and
 * reserve some space within a clump. This must happen after the engine
 * has been initialized but before the engine is opened.
 *
 * The world plugin must be attached before using this function.
 *
 * \param size  A RwInt32 value equal to the size of
 *              the memory block to reserve.
 * \param pluginID  A RwInt32 value equal to the plugin ID
 *                  (must be unique; used to identify binary chunks).
 * \param constructCB  Constructor for the plugin data block.
 * \param destructCB  Destructor for the plugin data block.
 * \param copyCB  Copy constructor for the plugin data block.
 *
 * \return Returns a RwInt32 value equal to the byte offset within the clump
 * of memory reserved for this plugin or a negative value if there is an error.
 *
 * \see RpClumpValidatePlugins
 * \see RpClumpRegisterPluginStream
 * \see RpClumpSetStreamAlwaysCallBack
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpClumpRegisterPlugin(RwInt32 size, RwUInt32 pluginID,
                      RwPluginObjectConstructor constructCB,
                      RwPluginObjectDestructor destructCB,
                      RwPluginObjectCopy copyCB)
{
    RwInt32             plug;

    RWAPIFUNCTION(RWSTRING("RpClumpRegisterPlugin"));
    RWASSERT(!clumpModule.numInstances);
    RWASSERT(size >= 0);

    /* Everything's cool, so pass it on */
    plug = rwPluginRegistryAddPlugin(&clumpTKList, size, pluginID,
                                     constructCB, destructCB, copyCB);

    RWRETURN(plug);
}

/**
 * \ingroup rpatomic
 * \ref RpAtomicRegisterPluginStream is used to associate a set of
 * binary stream functionality with a previously registered plugin.
 *
 * The world plugin must be attached before using this function.
 *
 * \param pluginID  A RwInt32 value equal to the plugin ID
 *                  (must be unique; used to identify binary chunks).
 * \param readCB  Callback used when a chunk is read that is
 *                identified as being for this plugin.
 * \param writeCB  Callback used when a chunk should be written out
 *                 for this plugin.
 * \param getSizeCB  Callback used to determine the
 *                   binary stream size required for this
 *                   plugin (return negative to suppress chunk writing).
 *
 * \return Returns a RwInt32 value equal to the byte offset within the atomic
 * of memory reserved for this plugin or a negative value if there is an error.
 *
 * \see RpAtomicSetStreamAlwaysCallBack
 * \see RpAtomicValidatePlugins
 * \see RpAtomicRegisterPlugin
 * \see RpAtomicGetPluginOffset
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpAtomicRegisterPluginStream(RwUInt32 pluginID,
                             RwPluginDataChunkReadCallBack readCB,
                             RwPluginDataChunkWriteCallBack writeCB,
                             RwPluginDataChunkGetSizeCallBack getSizeCB)
{
    RwInt32             plug;

    RWAPIFUNCTION(RWSTRING("RpAtomicRegisterPluginStream"));
    RWASSERT(readCB);
    RWASSERT(writeCB);
    RWASSERT(getSizeCB);

    /* Everything's cool, so pass it on */
    plug = rwPluginRegistryAddPluginStream(
               &atomicTKList, pluginID, readCB, writeCB, getSizeCB);

    RWRETURN(plug);
}

/**
 * \ingroup rpatomic
 * \ref RpAtomicSetStreamAlwaysCallBack is used to associate a
 * binary stream callback with a previously registered plugin. This callback
 * is called for all plugins after stream data reading has completed.
 *
 * The world plugin must be attached before using this function.
 *
 * \param pluginID  A RwInt32 value equal to the plugin ID
 *                  (must be unique; used to identify binary chunks).
 * \param alwaysCB  Callback used when object base and plugin data reading
 *                  is complete.
 *
 * \return Returns a RwInt32 value equal to the byte offset within the atomic
 *         of memory reserved for this plugin or
 *         a negative value if there is an error.
 *
 * \see RpAtomicRegisterPluginStream
 * \see RpAtomicValidatePlugins
 * \see RpAtomicRegisterPlugin
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpAtomicSetStreamAlwaysCallBack(RwUInt32 pluginID,
                                     RwPluginDataChunkAlwaysCallBack alwaysCB)
{
    RwInt32             plug;

    RWAPIFUNCTION(RWSTRING("RpAtomicSetStreamAlwaysCallBack"));
    RWASSERT(alwaysCB);

    /* Everything's cool, so pass it on */
    plug = rwPluginRegistryAddPluginStreamAlwaysCB(
               &atomicTKList, pluginID, alwaysCB);

    RWRETURN(plug);
}

/**
 * \ingroup rpatomic
 * \ref RpAtomicSetStreamRightsCallBack is used to associate a
 * binary stream callback with a previously registered plugin. This callback
 * is called for the plugin whose rights are asserted on the read atomic.
 * Rights are asserted if the atomic had an attached pipeline with a plugin
 * id when it was streamed.
 *
 * The world plugin must be attached before using this function.
 *
 * \param pluginID  A RwInt32 value equal to the plugin ID
 *                  (must be unique; used to identify binary chunks).
 * \param rightsCB  Callback used when object base and plugin data reading
 *                  is complete.
 *
 * \return Returns a RwInt32 value equal to the byte offset within the atomic
 *         of memory reserved for this plugin or
 *         a negative value if there is an error.
 *
 * \see RpAtomicRegisterPluginStream
 * \see RpAtomicValidatePlugins
 * \see RpAtomicRegisterPlugin
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpAtomicSetStreamRightsCallBack(RwUInt32 pluginID,
                                     RwPluginDataChunkRightsCallBack rightsCB)
{
    RwInt32             plug;

    RWAPIFUNCTION(RWSTRING("RpAtomicSetStreamRightsCallBack"));
    RWASSERT(rightsCB);

    /* Everything's cool, so pass it on */
    plug = _rwPluginRegistryAddPlgnStrmRightsCB(&atomicTKList, pluginID,
                                                rightsCB);

    RWRETURN(plug);
}

/**
 * \ingroup rpclump
 * \ref RpClumpRegisterPluginStream is used to associate a set of
 * binary stream functionality with a previously registered plugin.
 *
 * The world plugin must be attached before using this function.
 *
 * \param pluginID  A RwInt32 value equal to the plugin ID
 *                  (must be unique; used to identify binary chunks).
 * \param readCB  Callback used when a chunk is read that is
 *                identified as being for this plugin.
 * \param writeCB  Callback used when a chunk should be written out
 *                  for this plugin.
 * \param getSizeCB  Callback used to determine the
 *                   binary stream size required for this plugin
 *                    (return negative to suppress chunk writing).
 *
 * \return Returns a RwInt32 value equal to the byte offset within the clump
 *         of memory reserved for this plugin or
 *         a negative value if there is an error.
 *
 * \see RpClumpSetStreamAlwaysCallBack
 * \see RpClumpValidatePlugins
 * \see RpClumpRegisterPlugin
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpClumpRegisterPluginStream(RwUInt32 pluginID,
                            RwPluginDataChunkReadCallBack readCB,
                            RwPluginDataChunkWriteCallBack writeCB,
                            RwPluginDataChunkGetSizeCallBack getSizeCB)
{
    RwInt32             plug;

    RWAPIFUNCTION(RWSTRING("RpClumpRegisterPluginStream"));
    RWASSERT(readCB);
    RWASSERT(writeCB);
    RWASSERT(getSizeCB);

    /* Everything's cool, so pass it on */
    plug = rwPluginRegistryAddPluginStream(&clumpTKList, pluginID,
                                           readCB, writeCB, getSizeCB);

    RWRETURN(plug);
}

/**
 * \ingroup rpclump
 * \ref RpClumpSetStreamAlwaysCallBack is used to associate a
 * binary stream callback with a previously registered plugin. This callback
 * is called for all plugins after stream data reading has completed.
 *
 * The world plugin must be attached before using this function.
 *
 * \param pluginID  A RwInt32 value equal to the plugin ID
 *                  (must be unique; used to identify binary chunks).
 * \param alwaysCB  Callback used when object base and
 *                  plugin data reading is complete.
 *
 * \return Returns a RwInt32 value equal to the byte offset within the clump
 * of memory reserved for this plugin or a negative value if there is an error.
 *
 * \see RpClumpRegisterPluginStream
 * \see RpClumpValidatePlugins
 * \see RpClumpRegisterPlugin
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpClumpSetStreamAlwaysCallBack(RwUInt32 pluginID,
                                    RwPluginDataChunkAlwaysCallBack alwaysCB)
{
    RwInt32             plug;

    RWAPIFUNCTION(RWSTRING("RpClumpSetStreamAlwaysCallBack"));
    RWASSERT(alwaysCB);

    /* Everything's cool, so pass it on */
    plug = rwPluginRegistryAddPluginStreamAlwaysCB(
               &clumpTKList, pluginID, alwaysCB);

    RWRETURN(plug);
}

/**
 * \ingroup rpatomic
 * \ref RpAtomicGetPluginOffset is used to get the offset of a previously
 * registered atomic plugin.
 *
 * The world plugin must be attached before using this function.
 *
 * \param pluginID  A RwUInt32 value equal to the plugin ID.
 *
 * \return Returns the data block offset if successful or -1 if the plugin
 * is not registered.
 *
 * \see RpAtomicRegisterPlugin
 * \see RpAtomicRegisterPluginStream
 * \see RpAtomicSetStreamAlwaysCallBack
 * \see RpAtomicValidatePlugins
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpAtomicGetPluginOffset(RwUInt32 pluginID)
{
    RwInt32             offset;

    RWAPIFUNCTION(RWSTRING("RpAtomicGetPluginOffset"));

    offset = rwPluginRegistryGetPluginOffset(&atomicTKList, pluginID);

    RWRETURN(offset);
}

/**
 * \ingroup rpclump
 * \ref RpClumpGetPluginOffset is used to get the offset of a previously
 * registered clump plugin.
 *
 * The world plugin must be attached before using this function.
 *
 * \param pluginID  A RwUInt32 value equal to the plugin ID.
 *
 * \return Returns a RwInt32 value equal to the data block offset
 *         if successful or -1 if the plug-in is not registered.
 *
 * \see RpClumpRegisterPlugin
 * \see RpClumpRegisterPluginStream
 * \see RpClumpSetStreamAlwaysCallBack
 * \see RpClumpValidatePlugins
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpClumpGetPluginOffset(RwUInt32 pluginID)
{
    RwInt32             offset;

    RWAPIFUNCTION(RWSTRING("RpClumpGetPluginOffset"));

    offset = rwPluginRegistryGetPluginOffset(&clumpTKList, pluginID);

    RWRETURN(offset);
}

/**
 * \ingroup rpatomic
 * \ref RpAtomicValidatePlugins is used to validate the plugin memory
 * allocated within the specified atomic. This function is useful for
 * determining where memory trampling may be occuring within an application.
 *
 * This function only returns a meaningful response under a debug library.
 *
 * The world plugin must be attached before using this function.
 *
 * \param  atomic   Pointer to the atomic to validate.
 *
 * \return Returns TRUE is the atomic data is valid or FALSE if there is an
 * error or if the atomic data has become corrupt.
 *
 * \see RpAtomicRegisterPlugin
 * \see RpAtomicRegisterPluginStream
 * \see RpAtomicSetStreamAlwaysCallBack
 * \see RpWorldPluginAttach
 *
 */
RwBool
RpAtomicValidatePlugins(const RpAtomic * atomic __RWUNUSEDRELEASE__)
{
    RWAPIFUNCTION(RWSTRING("RpAtomicValidatePlugins"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(atomic);
    RWASSERTISTYPE(atomic, rpATOMIC);

#ifdef RWDEBUG
    {
        RwInt32             valid;

        valid = rwPluginRegistryValidateObject(&atomicTKList, atomic);
        RWRETURN(valid);
    }
#else /* RWDEBUG */
    RWRETURN(TRUE);
#endif /* RWDEBUG */
}

/**
 * \ingroup rpclump
 * \ref RpClumpValidatePlugins is used to validate the plugin memory
 * allocated within the specified clump. This function is useful for
 * determining where memory trampling may be occuring within an application.
 *
 * This function only returns a meaningful response under a debug library.
 *
 * The world plugin must be attached before using this function.
 *
 * \param  clump   Pointer to the clump to validate.
 *
 * \return Returns TRUE is the clump data is valid or FALSE if there is an
 * error or if the clump data has become corrupt.
 *
 * \see RpClumpRegisterPlugin
 * \see RpClumpRegisterPluginStream
 * \see RpClumpSetStreamAlwaysCallBack
 * \see RpWorldPluginAttach
 *
 */
RwBool
RpClumpValidatePlugins(const RpClump * clump __RWUNUSEDRELEASE__)
{
    RWAPIFUNCTION(RWSTRING("RpClumpValidatePlugins"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(clump);
    RWASSERTISTYPE(clump, rpCLUMP);

#ifdef RWDEBUG
    {
        RwBool              valid;

        valid = rwPluginRegistryValidateObject(&clumpTKList, clump);
        RWRETURN(valid);
    }
#else /* RWDEBUG */
    RWRETURN(TRUE);
#endif /* RWDEBUG */
}

#if ( defined(RWDEBUG) || defined(RWSUPPRESSINLINE) )

/**
 * \ingroup rpatomic
 * \ref RpAtomicGetFrame is used to retrieve the frame, if any, attached
 * to the specified atomic.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param atomic  Pointer to the atomic.
 *
 * \return Returns pointer to the atomic's frame if successful or
 *  NULL if there is an error or if the atomic has no frame attached.
 *
 * \see RpAtomicSetFrame
 * \see RpAtomicCreate
 * \see RpAtomicGetGeometry
 * \see RpAtomicSetGeometry
 * \see RpAtomicGetInterpolator
 * \see RpAtomicGetBoundingSphere
 * \see RpAtomicSetFlags
 * \see RpAtomicGetFlags
 * \see RpAtomicSetRenderCallBack
 * \see RpAtomicGetRenderCallBack
 * \see RpAtomicGetWorld
 * \see RpWorldAddAtomic
 * \see RwFrameCreate
 * \see RwFrameAddChild
 * \see RwFrameRemoveChild
 * \see RpWorldPluginAttach
 *
 */
RwFrame            *
RpAtomicGetFrame(const RpAtomic * atomic)
{
    RwFrame            *frameRet;

    RWAPIFUNCTION(RWSTRING("RpAtomicGetFrame"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(atomic);
    RWASSERTISTYPE(atomic, rpATOMIC);

    frameRet = (RwFrame *) rwObjectGetParent(atomic);
    RWRETURN(frameRet);
}

#endif /* ( defined(RWDEBUG) || defined(RWSUPPRESSINLINE) ) */

/**
 * \ingroup rpatomic
 * \ref RpAtomicSetFrame is used to attach the specified frame to the
 * given atomic. The frame describes the atomic's position and orientation
 * with respect to the world.
 *
 * The world plugin must be attached before using this function.
 *
 * \param atomic  Pointer to the atomic.
 * \param frame  Pointer to the frame.
 *
 * \return Returns pointer to the atomic if successful or
 *  NULL if there is an error.
 *
 * \see RpAtomicGetFrame
 * \see RpAtomicCreate
 * \see RpAtomicGetGeometry
 * \see RpAtomicSetGeometry
 * \see RpAtomicGetInterpolator
 * \see RpAtomicGetBoundingSphere
 * \see RpAtomicSetFlags
 * \see RpAtomicGetFlags
 * \see RpAtomicSetRenderCallBack
 * \see RpAtomicGetRenderCallBack
 * \see RpAtomicGetWorld
 * \see RpWorldAddAtomic
 * \see RwFrameCreate
 * \see RwFrameAddChild
 * \see RwFrameRemoveChild
 * \see RpWorldPluginAttach
 *
 */
RpAtomic           *
RpAtomicSetFrame(RpAtomic * atomic, RwFrame * frame)
{
    RWAPIFUNCTION(RWSTRING("RpAtomicSetFrame"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(atomic);
    RWASSERTISTYPE(atomic, rpATOMIC);

    rwObjectHasFrameSetFrame(atomic, frame);

    /* World bounding sphere is no longer valid */
    rwObjectSetPrivateFlags(atomic, rwObjectGetPrivateFlags(atomic) |
                            rpATOMICPRIVATEWORLDBOUNDDIRTY);

    RWRETURN(atomic);
}

#if ( defined(RWDEBUG) || defined(RWSUPPRESSINLINE) )

/**
 * \ingroup rpclump
 * \ref RpClumpSetFrame is used to attach the specified frame to the
 * given clump. The frame describes the clump's position and orientation
 * with respect to the world.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param clump  Pointer to the clump.
 * \param frame  Pointer to the frame.
 *
 * \return Returns pointer to the clump if successful or NULL if there
 * is an error.
 *
 * \see RpClumpGetFrame
 * \see RwFrameCreate
 * \see RwFrameAddChild
 * \see RwFrameRemoveChild
 * \see RpWorldPluginAttach
 *
 */
RpClump            *
RpClumpSetFrame(RpClump * clump, RwFrame * frame)
{
    RWAPIFUNCTION(RWSTRING("RpClumpSetFrame"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(clump);
    RWASSERTISTYPE(clump, rpCLUMP);

    if (frame)
    {
        RWASSERTISTYPE(frame, rwFRAME);
    }

    RWRETURN(RpClumpSetFrameMacro(clump, frame));
}

/**
 * \ingroup rpclump
 * \ref RpClumpGetFrame is used to retrieve the frame, if any, attached
 * to the specified clump.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param clump  Pointer to the clump.
 *
 * \return Returns pointer to the clump's frame if successful or
 *         NULL if there is an error or if the clump has no frame attached.
 *
 * \see RpClumpSetFrame
 * \see RwFrameCreate
 * \see RwFrameAddChild
 * \see RwFrameRemoveChild
 * \see RpWorldPluginAttach
 *
 */
RwFrame            *
RpClumpGetFrame(const RpClump * clump)
{
    RWAPIFUNCTION(RWSTRING("RpClumpGetFrame"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(clump);
    RWASSERTISTYPE(clump, rpCLUMP);

    RWRETURN((RwFrame *) rwObjectGetParent(clump));
}

/**
 * \ingroup rpatomic
 * \ref RpAtomicSetFlags is used to define the property flags for
 * the specified atomic.
 *
 * Note that this function replaces existing flags with the new ones,
 * hence any previously set flags are lost. Therefore, if any of the
 * old flags are still required they must be set again with this function.
 *
 * Note also that if the collision test flag has changed, the API function
 * \ref RwFrameUpdateObjects should be immediately called on the atomic's frame
 * to reflect the change.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param atomic  Pointer to the atomic.
 * \param flags  A 32-bit integer bit-field specifying the atomic's
 * properties: Valid values for this parameter are defined in
 * the \ref RpAtomicFlag enumerated type.
 *
 * \return Returns pointer to the atomic if successful or NULL if
 * there is an error.
 *
 * \see RpAtomicGetFlags
 * \see RpAtomicCreate
 * \see RpAtomicGetGeometry
 * \see RpAtomicSetGeometry
 * \see RpAtomicGetInterpolator
 * \see RpAtomicGetBoundingSphere
 * \see RpAtomicSetRenderCallBack
 * \see RpAtomicGetRenderCallBack
 * \see RpAtomicGetWorld
 * \see RpWorldAddAtomic
 * \see RpWorldPluginAttach
 *
 */
RpAtomic           *
RpAtomicSetFlags(RpAtomic * atomic, RwUInt32 flags)
{
    RWAPIFUNCTION(RWSTRING("RpAtomicSetFlags"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(atomic);
    RWASSERTISTYPE(atomic, rpATOMIC);

    rwObjectSetFlags(atomic, flags);

    RWRETURN(atomic);
}

/**
 * \ingroup rpatomic
 * \ref RpAtomicGetFlags is used to retrieve the property flags from
 * the specified atomic.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param atomic  Pointer to the atomic.
 *
 * \return Returns a 32-bit integer bit-field whose value is
 *         the bit-wise OR of the values defined in
 *         the \ref RpAtomicFlag enumerated type.
 *
 * \see RpAtomicSetFlags
 * \see RpAtomicCreate
 * \see RpAtomicGetGeometry
 * \see RpAtomicSetGeometry
 * \see RpAtomicGetInterpolator
 * \see RpAtomicGetBoundingSphere
 * \see RpAtomicSetRenderCallBack
 * \see RpAtomicGetRenderCallBack
 * \see RpAtomicGetWorld
 * \see RpWorldAddAtomic
 * \see RpWorldPluginAttach
 *
 */
RwUInt32
RpAtomicGetFlags(const RpAtomic * atomic)
{
    RWAPIFUNCTION(RWSTRING("RpAtomicGetFlags"));
    RWASSERT(clumpModule.numInstances);
    RWASSERT(atomic);
    RWASSERTISTYPE(atomic, rpATOMIC);

    RWRETURN(rwObjectGetFlags(atomic));
}

#endif /* ( defined(RWDEBUG) || defined(RWSUPPRESSINLINE) ) */


/* native.h will overload this for PS2 which does things its own way */
#if (!defined(_rpGeometryNativeFreeResEntry))
#define _rpGeometryNativeFreeResEntry(_gmty) \
    (((_gmty)->repEntry)?(RwResourcesFreeResEntry((_gmty)->repEntry)):(TRUE))
#endif /* (!defined(_rpGeometryNativeFreeResEntry)) */

/**
 * \ingroup rpatomic
 * \ref RpAtomicInstance is used to instance a geometry outside of the
 * resource arena. This means that the geometry will never be instanced again.
 *
 * Once this function has been call two representations of the geometry will
 * exist, one platform independent (PI) and one platform specific (PS).
 *
 * If the geometry is serialized \ref RpGeometryStreamWrite,
 * \ref RpAtomicStreamWrite or \ref RpClumpStreamWrite after
 * \ref RpAtomicInstance has been called only the PS representation will be
 * saved. When the geometry is reloaded by \ref RpGeometryStreamRead,
 * \ref RpAtomicStreamRead or \ref RpClumpStreamRead only the PS
 * representation will exist. This data can only be loaded and used on the
 * platform it was created on.
 *
 * All PI geometry data will be lost so certain things will no longer work.
 * For example, collision detection will not work as this relies on a PI data.
 * A second low res collision geometry can be used to solve this problem.
 *
 * The number of vertices in a geometry \ref RpGeometryGetNumVertices
 * and the number of triangles \ref RpGeometryGetNumTriangles are preserved.
 * Meshes \ref RpMesh are also preserved but the indices themselves are not
 * present.
 *
 * The correct rendering pipelines must be attached to the atomic and
 * materials before this function is called as those rendering pipelines may
 * introduce PS data that is required to give the desired rendering effect.
 *
 * This function must be called between \ref RwCameraBeginUpdate and
 * \ref RwCameraEndUpdate as this function has to execute the rendering
 * pipelines to guarantee that all the needed data is generated. In essence
 * this function behaves exactly like \ref RpAtomicRender, the only difference
 * being that the instance data does not get created in the resource arena but
 * is allocated from the heap and is therefore persistent. All geometry will be
 * processed and instanced even if not inside the camera frustum.
 *
 * e.g.
 *
 * \verbatim

    if (RwCameraBeginUpdate(Camera))
    {
        RpAtomicInstance(Atomic);

        RwCameraEndUpdate(Camera);
    }
    \endverbatim
 *
 *
 * NOTE: See the platform specific documentation to see if this feature is
 * not supported on your target platform.
 *
 * Geometry with more than one morph target is not supported. It is only safe
 * to use this function on geometry with one morph target or skinned geometry.
 *
 * The data generated by this function should be considered as being highly
 * volatile and the binary format will change.
 *
 *
 * The world plugin must be attached before using this function.
 *
 * \param atomic  Pointer to a RpAtomic that contains the geometry to be
 *                instanced.
 *
 * \return Returns TRUE if successful or FALSE if there is an error
 *
 * \see RpWorldPluginAttach
 * \see RpGeometryGetNumVertices
 * \see RpGeometryGetNumTriangles
 * \see RpGeometryForAllMeshes
 * \see RpGeometryStreamWrite
 * \see RpGeometryStreamRead
 * \if gcn \see \ref preinstanceoverview \endif
 *
 */
RwBool
RpAtomicInstance(RpAtomic *atomic)
{
    RpGeometry  *geometry;

    RWAPIFUNCTION(RWSTRING("RpAtomicInstance"));
    RWASSERT(atomic);

    geometry = RpAtomicGetGeometry(atomic);
    RWASSERT(geometry);

    if (1 != RpGeometryGetNumMorphTargets(geometry))
    {
        RWRETURN(FALSE);
    }

    if (rpGEOMETRYNATIVE & RpGeometryGetFlags(geometry))
    {
        /* Already native */
        RWRETURN(TRUE);
    }

    /* Destroy the old resource entry if one exists.
     * (this macro is overloaded for PS2) */
    _rpGeometryNativeFreeResEntry(geometry);

    /* Mark as persistent but needs instancing */
    RpGeometrySetFlags(geometry, RpGeometryGetFlags(geometry) |
                                 rpGEOMETRYNATIVEINSTANCE);

    /* Instance the sucker by executing the pipeline... */
    RpAtomicRender(atomic);

    /* Mark as persistent */
    RpGeometrySetFlags(geometry, (~rpGEOMETRYNATIVEINSTANCE &
                                  RpGeometryGetFlags(geometry)) |
                                 rpGEOMETRYNATIVE);

    RWRETURN(TRUE);
}
