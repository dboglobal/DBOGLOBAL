/**
 * \ingroup rptoon
 * \page rptoonoverview RpToon Plugin Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h, rpworld.h, rptoon.h
 * \li \b Libraries: rwcore, rpworld, rptoon
 * \li \b Plugin \b attachments: \ref RpWorldPluginAttach,
 *                               \ref RpToonPluginAttach
 *
 * \subsection toonpluginoverview Overview
 *
 * RpToon provides rendering of outlines and "cel shaded" texture based
 * lighting for RenderWare objects to achieve cartoon-like 3D graphics.
 *
 * Toon rendering can be applied to static, morphed, dmorphed and skinned
 * atomics, and world sectors. An art path is provided to specify line
 * and shading styles for toon objects.  Some functionality for this is
 * provided in the RtToon API for the benefit of our exporters, which you
 * can safely ignore unless you need to customize the tool chain. RtToon
 * is supported on PC platforms only.
 *
 * \subsection toonplugincaveats Caveats
 *
 * Good quality toon rendering, especially silhouette edge rendering, is
 * expensive.  Expect to use much lower polygon counts than you would in a
 * realistically rendered game to achieve satisfactory performance,
 * and/or enable toon rendering on only a subset of game objects.  Note
 * that existing toon games often take various shortcuts to reduce the
 * rendering cost, such as painting edges into textures of "less
 * important" objects rather than rendering them as geometry.
 *
 * RpToon is implemented differently on each platform, and some
 * features are not supported on all platforms because of hardware
 * limitations.  Be sure to check the relevant platform specific
 * documentation for details. Furthermore, toon rendering can NOT be
 * applied to an object that has \ref rpmatfx effects applied to ANY of
 * its materials.  In addition, note that prelit colors, native data,
 * parallel cameras, and alpha blending are not supported on RpToon
 * objects in this release.
 *
 * Only 1 directional light is supported for shading.
 *
 * Before any of the plugin functions are used, the plugin should be
 * attached using \ref RpToonPluginAttach.
 *
 * \subsection toonobjects Objects
 *
 * RpToon adds a number of objects to RenderWare, and also extends
 * several RenderWare objects as follows:
 *
 * \ref RpToonInk defines the style (thickness, color and scaling
 * properties) used to render silhouette and crease edges as 2D outlines
 * of objects.  \ref RpToonInk's are named and have reference counts.
 *
 * \ref RpToonInkDictionary organizes a collection of RpToonInk's
 * analogously to how an \ref RwTexDictionary organizes a collection
 * of \ref RwTexture's.  You can look up by name, add and remove
 * \ref RpToonInk's in an \ref RpToonInkDictionary.  It also provides the most
 * convenient way of applying streamed ink styles to a scene of toon
 * objects.
 *
 * \ref RpToonPaint defines the style (texture and type) of shading used on
 * a toon object's materials. \ref RpToonPaint's are also named and have
 * reference counts.
 *
 * \ref RpToonPaintDictionary organizes a collection of \ref RpToonPaint's and
 * that is the only difference between it and \ref RpToonInkDictionary.
 *
 * RpToon extends \ref RpGeometry and \ref RpWorldSector with an
 * \ref RpToonGeo object which holds additional information on geometry
 * connectivity and other properties necessary for silhouette and crease
 * edge rendering. Once you have an RpToonGeo object, it doesn't matter
 * whether it came from an \ref RpGeometry or an \ref RpWorldSector - you
 * can treat it the same way. In addition, "default" ink and paint styles
 * for an object are stored on an \ref RpToonGeo so it is not necessary
 * to define explicit settings for every material or edge if you don't
 * need to, which is often the case.
 *
 * RpToon extends \ref RpMaterial to hold an \ref RpToonMaterial.  \ref
 * RpToonMaterial in turn has a pointer to an \ref RpToonPaint which if
 * defined, takes precedence over the default paint in an \ref RpToonGeo
 * object.  The shading from the \ref RpToonPaint is blended with the
 * \ref RpMaterial's color and texture.
 *
 * On some platforms, additional information in \ref RpToonMaterial is used
 * for identifying the \ref RpToonInk used for silhouette rendering on a
 * particular mesh referencing that material.  To support this, the art path
 * must re-partition an object's meshes and sometimes introduce additional
 * materials if you define multiple silhouette ink styles on an object.
 */

#include "rwcore.h"
#include "rpworld.h"

#include "rpplugin.h"
#include "rpdbgerr.h"

#include "toonmaterial.h"
#include "toongeo.h"
#include "pipes.h"

#include "toon.h"
//Hodong
#include "NtlMaterialExt.h"

long            rpToonStackDepth = 0;

ToonGlobals     _rpToonGlobals;

static RwReal   _rpToonzBias = 1.0f;

/**
 * \ingroup rptoon
 * \ref RpToonSetCreaseZBias sets the zbias used to offset crease edge
 * polygons z coordinates in camera space.
 *
 * Tweaking this value can help avoid the crease edges being obscured by z
 * fighting.
 *
 * \param zBias  The new value.
 *
 * \return none.
 *
 * \see RpToonGetCreaseZBias
 */
void
RpToonSetCreaseZBias(RwReal zBias)
{
    RWAPIFUNCTION(RWSTRING("RpToonSetCreaseZBias"));

    _rpToonzBias = zBias;

    RWRETURNVOID();
}

/**
 * \ingroup rptoon
 * \ref RpToonGetCreaseZBias gets the zbias used to
 * offset crease edge polygons z coordinates in camera space.
 * Tweaking this value can help avoid
 * the crease edges being obscured by z fighting.
 *
 * \return The zBias value.
 *
 * \see RpToonSetCreaseZBias
 */
RwReal
RpToonGetCreaseZBias(void)
{
    RWAPIFUNCTION(RWSTRING("RpToonGetCreaseZBias"));

    RWRETURN(_rpToonzBias);
}

static RwInt32
ToonGeometrySize(const void *object,
                 RwInt32 offset __RWUNUSED__,
                 RwInt32 bytes  __RWUNUSED__)
{
    const RpGeometry *  geometry;
    const RpToonGeo *   toonGeo;
    RwInt32             size = 0;

    RWFUNCTION(RWSTRING("ToonGeometrySize"));
    RWASSERT(NULL != object);

    /* Get the geometry and the toon geometry. */
    geometry = (const RpGeometry *)object;
    RWASSERT(NULL != geometry);
    RWASSERTISTYPE(geometry, rpGEOMETRY);

    toonGeo = *RPTOONGEOMETRYGETCONSTDATA(geometry);

    if (NULL != toonGeo)
    {
#if defined(PLATFORM_NATIVE)
        if (!(rpGEOMETRYNATIVE & RpGeometryGetFlags(geometry)))
        {
            size = _rpToonGeoSize(toonGeo);
        }
        else
        {
            size = _rpToonGeoNativeSize(toonGeo);
        }
#else /* defined(PLATFORM_NATIVE) */
        size = _rpToonGeoSize(toonGeo);
#endif /* defined(PLATFORM_NATIVE) */
    }

    RWRETURN(size);
}

static RwInt32
ToonSectorSize(const void *object,
               RwInt32 offset __RWUNUSED__,
               RwInt32 bytes  __RWUNUSED__)
{
    const RpWorldSector *   sector;
    const RpToonGeo *       toonGeo;
    RwInt32                 size = 0;

    RWFUNCTION(RWSTRING("ToonSectorSize"));
    RWASSERT(NULL != object);

    /* Get the sector and the toon geometry. */
    sector = (const RpWorldSector *)object;
    RWASSERT(NULL != sector);
    RWASSERTISTYPE(sector, rpWorldSector);

    toonGeo = *RPTOONSECTORGETCONSTDATA(sector);

    if (NULL != toonGeo)
    {
#if defined(PLATFORM_NATIVE)
        if (!(rpWORLDNATIVE & RpWorldGetFlags(RpWorldSectorGetWorld(sector))))
        {
            size = _rpToonGeoSize(toonGeo);
        }
        else
        {
            size = _rpToonGeoNativeSize(toonGeo);
        }
#else /* defined(PLATFORM_NATIVE) */
        size = _rpToonGeoSize(toonGeo);
#endif /* defined(PLATFORM_NATIVE) */
    }

    RWRETURN(size);
}

static RwStream *
ToonGeometryWrite(RwStream *stream,
                  RwInt32 binaryLength __RWUNUSED__,
                  const void *object,
                  RwInt32 offsetInObject __RWUNUSED__,
                  RwInt32 sizeInObject __RWUNUSED__)
{
    const RpGeometry *  geometry;
    const RpToonGeo *   toonGeo;

    RWFUNCTION(RWSTRING("ToonGeometryWrite"));
    RWASSERT(NULL != stream);
    RWASSERT(NULL != object);

    /* Get the geometry and toon geometry. */
    geometry = (const RpGeometry *)object;
    RWASSERT(NULL != geometry);
    toonGeo = *RPTOONGEOMETRYGETCONSTDATA(geometry);

    if (NULL != toonGeo)
    {
#if defined(PLATFORM_NATIVE)
        if (!(rpGEOMETRYNATIVE & RpGeometryGetFlags(geometry)))
        {
            stream = _rpToonGeoWrite(stream, toonGeo);
        }
        else
        {
            stream = _rpToonGeoNativeWrite(stream, toonGeo);
        }
#else /* defined(PLATFORM_NATIVE) */
        stream = _rpToonGeoWrite(stream, toonGeo);
#endif /* defined(PLATFORM_NATIVE) */
    }

    RWRETURN(stream);
}

static RwStream *
ToonSectorWrite(RwStream *stream,
                RwInt32 binaryLength __RWUNUSED__,
                const void *object,
                RwInt32 offsetInObject __RWUNUSED__,
                RwInt32 sizeInObject __RWUNUSED__)
{
    const RpWorldSector *   sector;
    const RpToonGeo *       toonGeo;

    RWFUNCTION(RWSTRING("ToonSectorWrite"));
    RWASSERT(NULL != stream);
    RWASSERT(NULL != object);

    /* Get the sector and toon geometry. */
    sector = (const RpWorldSector *)object;
    RWASSERT(NULL != sector);
    toonGeo = *RPTOONSECTORGETCONSTDATA(sector);

    if (NULL != toonGeo)
    {
#if defined(PLATFORM_NATIVE)
        if (!(rpWORLDNATIVE & RpWorldGetFlags(RpWorldSectorGetWorld(sector))))
        {
            stream = _rpToonGeoWrite(stream, toonGeo);
        }
        else
        {
            stream = _rpToonGeoNativeWrite(stream, toonGeo);
        }
#else /* defined(PLATFORM_NATIVE) */
            stream = _rpToonGeoWrite(stream, toonGeo);
#endif /* defined(PLATFORM_NATIVE) */
    }

    RWRETURN(stream);
}


static RwStream *
ToonGeometryRead(RwStream *stream,
                 RwInt32 binaryLength   __RWUNUSED__,
                 void *object,
                 RwInt32 offsetInObject __RWUNUSED__,
                 RwInt32 sizeInObject   __RWUNUSED__)
{
    RpGeometry  *geometry;
    RpToonGeo   *toonGeo;
    RwStream    *success;

    RWFUNCTION(RWSTRING("ToonGeometryRead"));
    RWASSERT(NULL != stream);
    RWASSERT(NULL != object);

    /* Get the geometry. */
    geometry = (RpGeometry *)object;
    RWASSERT(NULL != geometry);

#if defined(PLATFORM_NATIVE)
    if (!(rpGEOMETRYNATIVE & RpGeometryGetFlags(geometry)))
    {
        success = _rpToonGeoRead(stream, &toonGeo);
    }
    else
    {
        success = _rpToonGeoNativeRead(stream, &toonGeo);
    }
#else /* defined(PLATFORM_NATIVE) */
    success = _rpToonGeoRead(stream, &toonGeo);
#endif /* defined(PLATFORM_NATIVE) */

    if (!success)
    {
        RWRETURN(0);
    }

    /*
    Can't RpToonAtomicEnable - the world render callback is still NULL at this point!
    :-(
    */
    toonGeo->parentGeometry = geometry;
    toonGeo->parentWorldSector = NULL;
    *RPTOONGEOMETRYGETDATA(geometry) = toonGeo;

    RWRETURN(stream);
}

static RwStream *
ToonSectorRead(RwStream *stream,
               RwInt32 binaryLength   __RWUNUSED__,
               void *object,
               RwInt32 offsetInObject __RWUNUSED__,
               RwInt32 sizeInObject   __RWUNUSED__)
{
    RpWorldSector   *sector;
    RpToonGeo       *toonGeo;
    RwStream        *success;

    RWFUNCTION(RWSTRING("ToonSectorRead"));
    RWASSERT(NULL != stream);
    RWASSERT(NULL != object);

    /* Get the sector. */
    sector = (RpWorldSector *)object;
    RWASSERT(NULL != sector);

#if defined(PLATFORM_NATIVE)
    if (!(rpWORLDNATIVE & RpWorldGetFlags(RpWorldSectorGetWorld(sector))))
    {
        success = _rpToonGeoRead(stream, &toonGeo);
    }
    else
    {
        success = _rpToonGeoNativeRead(stream, &toonGeo);
    }
#else /* defined(PLATFORM_NATIVE) */
        success = _rpToonGeoRead(stream, &toonGeo);
#endif /* defined(PLATFORM_NATIVE) */

    if (!success)
    {
        RWRETURN(0);
    }

    /*
    Can't RpToonWorldSectorEnable - the world render callback is still NULL at this point!
    :-(
    */
    toonGeo->parentWorldSector = sector;
    toonGeo->parentGeometry = NULL;
    *RPTOONSECTORGETDATA(sector) = toonGeo;

    RWRETURN(stream);
}


/**
 * \ingroup rptoon
 * \ref RpToonAtomicEnable is used to enable the toon pipeline on an atomic.
 * The atomic must have a \ref RpToonGeo associated with it.
 *
 * \note This function should not be called on the same atomic more than once.
 *
 * \param atomic Pointer to the \ref RpAtomic to enable the toon pipeline on.
 *
 * \return TRUE on success, FALSE otherwise.
 *
 * \see RpToonWorldSectorEnable
 * \see RtToonGeometryCreateToonGeo
 * \see RpToonGeo
 */
RwBool
RpToonAtomicEnable(RpAtomic *atomic)
{
    RWAPIFUNCTION(RWSTRING("RpToonAtomicEnable"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);

    RWASSERT(atomic != NULL);
    RWASSERT(RpToonGeometryGetToonGeo(RpAtomicGetGeometry(atomic)));

    /* attach platform specific pipe */
    /* TODO[3]: SKINTOON WILL MOVE TO TOONSKIN, AT WHICH POINT WE SHOULD ADD A RIGHTSCB TO
     *   TOON (ALA SKIN2/PATCH), RATHER THAT USING THIS FUNCTION TO SET UP PIPELINES... */
    _rpToonAtomicPipelinesAttach(atomic);

    RWRETURN(TRUE);
}

/**
 * \ingroup rptoon
 * \ref RpToonWorldSectorEnable is used to enable the toon pipeline on a world sector.
 * The sector must have a \ref RpToonGeo associated with it.
 *
 * \param sector Pointer to the \ref RpWorldSector to enable the toon pipeline on.
 *
 * \return TRUE on success, FALSE otherwise.
 *
 * \see RpToonAtomicEnable
 * \see RtToonGeometryCreateToonGeo
 * \see RpToonGeo
 */
RwBool
RpToonWorldSectorEnable(RpWorldSector *sector)
{
    RWAPIFUNCTION(RWSTRING("RpToonWorldSectorEnable"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);
    RWASSERT(sector != NULL);
    RWASSERT(RpToonWorldSectorGetToonGeo(sector));

    /* attach platform specific pipe */
    _rpToonSectorPipelinesAttach(sector);

    RWRETURN(TRUE);
}

/**
 * \ingroup rptoongeo
 * \ref RpToonGeometryGetToonGeo gets the \ref RpToonGeo associated with
 * the \ref RpGeometry.
 *
 * \param geometry  A pointer to the source geometry.
 *
 * \return A pointer to RpToonGeo if one exists, or NULL.
 *
 * \see RtToonGeometryCreateToonGeo
 *
 */
RpToonGeo *
RpToonGeometryGetToonGeo(RpGeometry *geometry)
{
    RWAPIFUNCTION(RWSTRING("RpToonGeometryGetToonGeo"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);

    RWRETURN(*RPTOONGEOMETRYGETDATA(geometry));
}


/**
 * \ingroup rptoongeo
 * \ref RpToonWorldSectorGetToonGeo gets the \ref RpToonGeo associated with
 * the \ref RpWorldSector.
 *
 * \param sector  A pointer to the source sector.
 *
 * \return A pointer to RpToonGeo if one exists, or NULL.
 *
 * \see RpToonWorldSectorCreateToonGeo
 *
 */
RpToonGeo *
RpToonWorldSectorGetToonGeo(RpWorldSector *sector)
{
    RWAPIFUNCTION(RWSTRING("RpToonWorldSectorGetToonGeo"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);

    RWRETURN(*RPTOONSECTORGETDATA(sector));
}

static void *
ToonGeometryConstructor(void *object,
                        RwInt32 offset __RWUNUSED__,
                        RwInt32 size   __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("ToonGeometryConstructor"));
    RWASSERT(NULL != object);

    *RPTOONGEOMETRYGETDATA(object) = (RpToonGeo *)NULL;

    RWRETURN(object);
}

static void *
ToonGeometryDestructor(void *object,
                       RwInt32 offset __RWUNUSED__,
                       RwInt32 size   __RWUNUSED__)
{
    RpGeometry *    geometry;
    RpToonGeo *     toonGeo;

    RWFUNCTION(RWSTRING("ToonGeometryDestructor"));
    RWASSERT(NULL != object);

    /* Get the toon data from the geometry extension . */
    geometry = (RpGeometry *)object;
    toonGeo = *RPTOONGEOMETRYGETDATA(geometry);

    if (NULL != toonGeo)
    {
        /* Destroy the toon data, and remove the extension. */
        *RPTOONGEOMETRYGETDATA(geometry) = 0;
        _rpToonGeoDestroy(toonGeo);
    }

    RWASSERT(NULL == *RPTOONGEOMETRYGETDATA(object));
    RWRETURN(object);
}

static void *
ToonGeometryCopy(void *dstObject,
                 const void *srcObject __RWUNUSEDRELEASE__,
                 RwInt32 offset __RWUNUSED__,
                 RwInt32 size   __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("ToonGeometryCopy"));
    RWASSERT(NULL != srcObject);
    RWASSERT(NULL != dstObject);

    /*
     * Should never be able to get here as there is no
     * RpGeometryCopy/Clone function
     */
    RWASSERT(0 && "implement me");

    RWRETURN(dstObject);
}

static void *
ToonSectorConstructor(void *object,
                      RwInt32 offset __RWUNUSED__,
                      RwInt32 size   __RWUNUSED__ )
{
    RWFUNCTION(RWSTRING("ToonSectorConstructor"));
    RWASSERT(NULL != object);

    *RPTOONSECTORGETDATA(object) = (RpToonGeo *)NULL;

    RWRETURN(object);
}

static void *
ToonSectorDestructor(void *object,
                     RwInt32 offset __RWUNUSED__,
                     RwInt32 size   __RWUNUSED__)
{
    RpWorldSector * sector;
    RpToonGeo *     toonGeo;

    RWFUNCTION(RWSTRING("ToonSectorDestructor"));
    RWASSERT(NULL != object);

    /* Get the toon data from the sector extension . */
    sector = (RpWorldSector *)object;
    toonGeo = *RPTOONSECTORGETDATA(sector);

    if (NULL != toonGeo)
    {
        /* Destroy the toon data, and remove the extension. */
        *RPTOONSECTORGETDATA(sector) = _rpToonGeoDestroy(toonGeo);
    }

    RWASSERT(NULL == *RPTOONSECTORGETDATA(object));
    RWRETURN(object);
}

static void *
ToonSectorCopy(void *dstObject,
               const void *srcObject __RWUNUSEDRELEASE__,
               RwInt32 offset __RWUNUSED__,
               RwInt32 size   __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("ToonSectorCopy"));
    RWASSERT(NULL != srcObject);
    RWASSERT(NULL != dstObject);

    /*
     * Should never be able to get here as there is no
     * RpGeometryCopy/Clone function
     */
    RWASSERT(0 && "implement me");

    RWRETURN(dstObject);
}

static void *
ToonOpen(void *instance,
         RwInt32 offset __RWUNUSED__,
         RwInt32 size   __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("ToonOpen"));
    RWASSERT(NULL != instance);

    /* Create the toon pipelines, only once. */
    if (_rpToonGlobals.module.numInstances == 0)
    {
        RwBool success;

        /* Set up the pipeline. */
        success = _rpToonPipelinesCreate();
        RWASSERT(FALSE != success);
    }

    /* Another instance. */
    _rpToonGlobals.module.numInstances++;

    RWRETURN(instance);
}

static void *
ToonClose(void *instance,
          RwInt32 offset __RWUNUSED__,
          RwInt32 size   __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("ToonClose"));
    RWASSERT(NULL != instance);
    RWASSERT(0 < _rpToonGlobals.module.numInstances);

    /* One less module instance. */
    _rpToonGlobals.module.numInstances--;

    if (0 == _rpToonGlobals.module.numInstances)
    {
        RwBool success;

        /* Destroy the toon pipelines. */
        success = _rpToonPipelinesDestroy();
        RWASSERT(FALSE != success);
    }

    RWRETURN(instance);
}

/**
 * \ingroup rptoon
 * \ref RpToonPluginAttach is used to attach the toon plugin.The toon
 * plugin must be attached between initializing the
 * system with \ref RwEngineInit and opening it with \ref RwEngineOpen
 * and the world plugin must already be attached.
 *
 * \return Returns TRUE if successful or FALSE if there is an error
 *
 * \see RwEngineInit
 * \see RwEngineOpen
 * \see RwEngineStart
 *
 */
RwBool
RpToonPluginAttach(void)
{
    RwInt32 success = TRUE;

    RWAPIFUNCTION(RWSTRING("RpToonPluginAttach"));

    /* Register the plugIn */
    _rpToonGlobals.engineOffset =
        RwEngineRegisterPlugin(0,
                               rwID_TOONPLUGIN,
                               ToonOpen,
                               ToonClose);
    RWASSERT(0 < _rpToonGlobals.engineOffset);

    /* Extend geometry to hold skin geometry data. */
    _rpToonGlobals.geometryOffset =
        RpGeometryRegisterPlugin(sizeof(RpToonGeo *),
                                 rwID_TOONPLUGIN,
                                 ToonGeometryConstructor,
                                 ToonGeometryDestructor,
                                 ToonGeometryCopy);
    RWASSERT(0 < _rpToonGlobals.geometryOffset);

    /* Attach the stream handling functions */
    success = RpGeometryRegisterPluginStream(rwID_TOONPLUGIN,
                                             ToonGeometryRead,
                                             ToonGeometryWrite,
                                             ToonGeometrySize);
    RWASSERT(0 < success);

    /* Register worldSector plugIn */
    _rpToonGlobals.sectorOffset =
        RpWorldSectorRegisterPlugin(sizeof(RpToonGeo *),
                                    rwID_TOONPLUGIN,
                                    ToonSectorConstructor,
                                    ToonSectorDestructor,
                                    ToonSectorCopy);

    /* Attach the stream handling functions */
    success = RpWorldSectorRegisterPluginStream(rwID_TOONPLUGIN,
                                                ToonSectorRead,
                                                ToonSectorWrite,
                                                ToonSectorSize);
    RWASSERT(0 < _rpToonGlobals.sectorOffset);

    success = _rpToonMaterialPluginAttach();
    RWASSERT(0 < success);
	
	RWRETURN(success);
}
