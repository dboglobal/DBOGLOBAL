#include <string.h>

#include "rwcore.h"
#include "rpworld.h"

#include "rpplugin.h"
#include "rpdbgerr.h"

#include "toon.h"
#include "toonink.h"
#include "toonpaint.h"
#include "toonmaterial.h"

#include "toongeo.h"

/**
 * \defgroup rptoongeo RpToonGeo
 * \ingroup rptoon
 */


static void
ToonPerspectiveScaleCoefficientsInit(RpToonInk *ink,
                                     RwReal zMin,
                                     RwReal zMax)
{
    RWFUNCTION(RWSTRING("ToonPerspectiveScaleCoefficientsInit"));

    /*
     * Creates factored coefficients for
     * perspective scale effect from camera space min & max
     * Z values & perspective scale factor for an object
     */

    ink->perspectiveScale_zMin = zMin;

    /*
    back most verts scaled by 1 - s, foremost by 1 + s.
    not really exciting because only factor of 2 possible.
    ink->perspectiveScale_a = 1.0f + ink->perspectiveScale;
    ink->perspectiveScale_b = -2.0f * ink->perspectiveScale / (zMax - zMin);
    */

    /*
    back most verts scaled by 1, front most by s
    */
    ink->perspectiveScale_a = ink->perspectiveScale;
    ink->perspectiveScale_b = (1.0f - ink->perspectiveScale) / (zMax - zMin);

    RWRETURNVOID();
}

void
_rpToonUpdateWorldPerspectiveScale(RpToonGeo *geo)
{
    RwReal zMin, zMax;
    RwCamera *camera;
    RwInt32 inkIndex;
    RwReal pixelSize;

    RWFUNCTION(RWSTRING("_rpToonUpdateWorldPerspectiveScale"));

    RWASSERT(0 != geo);

    /* just use camera's near & far clip planes */
    camera = RwCameraGetCurrentCamera();
    RWASSERT(0 != camera);

    zMin = RwCameraGetNearClipPlane(camera);
    zMax = RwCameraGetFarClipPlane(camera);

    pixelSize = 1.0f / RwRasterGetWidth(RwCameraGetRaster(RwCameraGetCurrentCamera()));

    for (inkIndex=0; inkIndex<geo->inkIDCount; inkIndex++)
    {
        RpToonInk *ink = geo->inks[inkIndex];

        if (ink)
        {
            ToonPerspectiveScaleCoefficientsInit(ink,zMin,zMax);

            /* worlds don't need these - 1.0f does nothing */
            ink->farScale_factor = pixelSize * ink->overallThickness;

            /* We draw 2 quads, want it to be as wide as 1 silhoutte quad in total */
            ink->farScale_creasefactor = 0.5f * ink->farScale_factor;
        }
    }

    RWRETURNVOID();
}

void
_rpToonUpdateAtomicPerspectiveScale(RpToonGeo *geo, RpAtomic *atomic )
{
    RwReal zMin, zMax;
    RwCamera *camera;
    RwInt32 inkIndex;
    const RwSphere *boundSphere;
    RwMatrix invCameraLTM, toCameraSpace;
    RwV3d cameraSpaceCenter;
    RwReal proportionBetweenClipPlanes;
    RwReal pixelSize;

    RWFUNCTION(RWSTRING("_rpToonUpdateAtomicPerspectiveScale"));

    /* work out min & max z values from object bound sphere */
    camera = RwCameraGetCurrentCamera();
    RWASSERT(0 != camera);
    RWASSERT(atomic != 0);
    boundSphere = RpAtomicGetBoundingSphere( atomic );
    RWASSERT(boundSphere != 0);

    /* transform bound sphere center into camera space */
    RwMatrixInvert(&invCameraLTM, RwFrameGetLTM(RwCameraGetFrame(camera)));
    RwMatrixMultiply(&toCameraSpace, RwFrameGetLTM(RpAtomicGetFrame(atomic)), &invCameraLTM);
    RwV3dTransformPoint(&cameraSpaceCenter, &boundSphere->center, &toCameraSpace);

    zMin = cameraSpaceCenter.z - boundSphere->radius;
    zMax = cameraSpaceCenter.z + boundSphere->radius;

    proportionBetweenClipPlanes = (cameraSpaceCenter.z - RwCameraGetNearClipPlane(camera))
                               /  (RwCameraGetFarClipPlane(camera) - RwCameraGetNearClipPlane(camera));

    pixelSize = 1.0f / RwRasterGetWidth(RwCameraGetRaster(RwCameraGetCurrentCamera()));

    for (inkIndex=0; inkIndex<geo->inkIDCount; inkIndex++)
    {
        RpToonInk *ink = geo->inks[inkIndex];

        if (ink)
        {
            ToonPerspectiveScaleCoefficientsInit(ink,zMin,zMax);

            ink->farScale_factor =
                pixelSize * (1.0f + proportionBetweenClipPlanes * (ink->farScale - 1.0f)) * ink->overallThickness;

            /* We draw 2 quads, want it to be as wide as 1 silhoutte quad in total */
            ink->farScale_creasefactor = 0.5f * ink->farScale_factor;
        }
    }

    RWRETURNVOID();
}

RpToonGeo *
_rpToonGeoInit(RwInt32 numVerts,
               RwInt32 numTriangles)
{
    RwInt32 i;
    RpToonGeo *toonGeo;

    RWFUNCTION(RWSTRING("_rpToonGeoInit"));

    /* make a toon geo struct and stick it on the geometry */
    toonGeo = (RpToonGeo *)RwMalloc( sizeof(RpToonGeo),
        rwID_TOONPLUGIN | rwMEMHINTDUR_EVENT);
    RWASSERT(toonGeo && "RwMalloc failed");

    if (!toonGeo)
    {
        RWRETURN(0);
    }

    /* make some vertex thicknesses and stick those on the geometry */
    toonGeo->vertexThicknesses = (RwReal *)RwMalloc( sizeof(RwReal) * numVerts,
                                         rwID_TOONPLUGIN | rwMEMHINTDUR_EVENT );
    RWASSERT( toonGeo->vertexThicknesses && "RwMalloc failed");
    if (!toonGeo->vertexThicknesses)
    {
        RWRETURN(0);
    }

    for (i=0; i<numVerts; i++)
    {
        toonGeo->vertexThicknesses[i] = 1.0f;
    }

    toonGeo->numVerts = numVerts;

    /* make some space for faces */
    toonGeo->numTriangles = numTriangles;
    toonGeo->triangles = (rpToonTriangle *)RwMalloc( sizeof(rpToonTriangle) * numTriangles,
                                                     rwID_TOONPLUGIN | rwMEMHINTDUR_EVENT);
    RWASSERT(toonGeo->triangles && "RwMalloc failed");
    if (!toonGeo->triangles)
    {
        RWRETURN(0);
    }

    /* clean everything else out */
    toonGeo->numEdges = 0;
    toonGeo->numCreaseEdges = 0;
    toonGeo->edges = NULL;

    /*
    Get some face normals for silhoutte test
    need to be recalculated for skinning/morphing
    */
    toonGeo->faceNormals = (RwV3d *) RwMalloc( numTriangles * sizeof(RwV3d),
                                       rwID_TOONPLUGIN | rwMEMHINTDUR_EVENT);
    RWASSERT((toonGeo->faceNormals != 0) && "RwMalloc failed");
    if (!toonGeo->faceNormals)
    {
        RWRETURN(0);
    }

    /*
    Create a set of normals to extrude along (smoothing group normals aren't always very good)
    */
    toonGeo->extrusionVertexNormals = (RwV3d *)RwMalloc(sizeof(RwV3d) * numVerts,
                                            rwID_TOONPLUGIN | rwMEMHINTDUR_EVENT);
    RWASSERT((toonGeo->extrusionVertexNormals != 0) && "RwMalloc failed");
    if (!toonGeo->extrusionVertexNormals)
    {
        RWRETURN(0);
    }

    toonGeo->paint = 0;

    toonGeo->defAtomicRenderCallback = NULL;
    toonGeo->defSectorRenderCallback = NULL;
    toonGeo->parentGeometry = NULL;
    toonGeo->parentWorldSector = NULL;
    toonGeo->vertexPositionsDirty = FALSE;
    toonGeo->isSkinned = FALSE;

    /* We always have at least two inks, on for "default" silhoutte & "default" crease */
    toonGeo->inkIDCount = 2;
    toonGeo->inkIDList = (rpToonInkID *)RwMalloc( sizeof(rpToonInkID) * toonGeo->inkIDCount,
                                                  rwID_TOONPLUGIN | rwMEMHINTDUR_EVENT);
    RWASSERT(toonGeo->inkIDList&&"RwMalloc failed");
    if (!toonGeo->inkIDList)
    {
        RWRETURN(0);
    }
    memset(toonGeo->inkIDList, 0, sizeof(rpToonInkID) * toonGeo->inkIDCount );

    toonGeo->inks = (RpToonInk **)RwMalloc( sizeof(RpToonInk *) * toonGeo->inkIDCount,
                                            rwID_TOONPLUGIN | rwMEMHINTDUR_EVENT);
    RWASSERT(toonGeo->inks&&"RwMalloc failed");
    if (!toonGeo->inks)
    {
        RWRETURN(0);
    }
    memset(toonGeo->inks, 0, sizeof(RpToonInk *) * toonGeo->inkIDCount );

    toonGeo->edgeInkIDs = NULL;

    /* default edgeInkIDs are optimized until someone messes them up */
    toonGeo->edgeInkIDsOptimized = TRUE;

    RWRETURN(toonGeo);
}

RpToonGeo *
_rpToonGeoDestroy(RpToonGeo *toonGeo)
{
    RWFUNCTION(RWSTRING("_rpToonGeoDestroy"));

    if (toonGeo->triangles)
    {
        RwFree(toonGeo->triangles);
        toonGeo->triangles = 0;
    }

    if (toonGeo->edges)
    {
        RwFree(toonGeo->edges);
        toonGeo->edges = 0;
    }

    if (toonGeo->vertexThicknesses)
    {
        RwFree(toonGeo->vertexThicknesses);
        toonGeo->vertexThicknesses = 0;
    }

    if (toonGeo->faceNormals)
    {
        RwFree(toonGeo->faceNormals);
        toonGeo->faceNormals = 0;
    }

    if (toonGeo->extrusionVertexNormals)
    {
        RwFree(toonGeo->extrusionVertexNormals);
        toonGeo->extrusionVertexNormals = 0;
    }

    if (toonGeo->inkIDList)
    {

        RwFree(toonGeo->inkIDList);
        toonGeo->inkIDList = 0;
    }

    if (toonGeo->inks)
    {
        /* decrement ink ref counts */
        RwInt32 i;
        for (i=0; i<toonGeo->inkIDCount; i++)
        {
            if (toonGeo->inks[i])
            {
                RpToonInkDestroy(toonGeo->inks[i]);
                toonGeo->inks[i] = 0;
            }
        }
        RwFree(toonGeo->inks);
        toonGeo->inks = 0;
    }

    if (toonGeo->edgeInkIDs)
    {
        RwFree(toonGeo->edgeInkIDs);
        toonGeo->edgeInkIDs = 0;
    }

    if (toonGeo->paint)
    {
        RpToonPaintDestroy(toonGeo->paint);
    }

    RwFree(toonGeo);

    RWRETURN(0);
}

/*---------------------------------------------------------------------------*
 *-   RpToonGeo Streaming functions                                         -*
 *---------------------------------------------------------------------------*/

#define RPTOON_INTERNALTOONGEOSTREAMVERSION (RwInt32)0xabcd0002

RwInt32
_rpToonGeoSize( const RpToonGeo *toonGeo )
{
    RwInt32 size = 0;

    RWFUNCTION(RWSTRING("_rpToonGeoSize"));

    /* internal version stamp */
    size += sizeof(RwInt32);

    /* Num of vertices. */
    size += sizeof(RwInt32);

    /* Num of triangles */
    size += sizeof(RwInt32);

    /* Triangle data */
    size += toonGeo->numTriangles * sizeof(rpToonTriangle);

    /* Vertex thickness data */
    size += toonGeo->numVerts * sizeof(RwReal);

    /* Num of edges */
    size += sizeof(RwInt32);

    /* Num of crease edges */
    size += sizeof(RwInt32);

    /* Edge connectivity data */
    size += toonGeo->numEdges * sizeof(RpToonEdge);

    /* Face normals */
    size += toonGeo->numTriangles * sizeof(RwV3d);

    /* extrusion normals, to be removed? */
    size += toonGeo->numVerts * sizeof(RwV3d);

    /* Num of ink IDs used on this mesh */
    size += sizeof(RwInt32);

    /* the ink ID list */
    size += toonGeo->inkIDCount * sizeof(rpToonInkID);

    /*
    Ink IDs for each edge in geometry
    */
    size += toonGeo->numEdges * sizeof(rpToonEdgeInkID);

    /* default paint */
    size += sizeof(rpToonPaintID);

    /* default render callback... ooh, weird */

    RWRETURN(size);
}

#define CHECKSTREAMANDRETURN(success) \
MACRO_START                           \
{                                     \
    if(NULL == (success))             \
    {                                 \
        RWRETURN((RwStream *)NULL);   \
    }                                 \
}                                     \
MACRO_STOP

RwStream *
_rpToonGeoRead(RwStream *stream,
               RpToonGeo **toonGeoPtr)
{
    RwInt32 numVertices, numTriangles, version;
    RpToonGeo *toonGeo;
    RwStream *success;

    RWFUNCTION(RWSTRING("_rpToonGeoRead"));

     /* internal version stamp */
    success = RwStreamReadInt32(stream,
                  (RwInt32 *)&version,
                  sizeof(RwInt32));
    CHECKSTREAMANDRETURN(success);

    if (version != RPTOON_INTERNALTOONGEOSTREAMVERSION)
    {
        RWERROR((E_RW_BADVERSION, RWSTRING("Incompatible toon geometry stream version: re-export required")));
        RWRETURN(0);
    }

    /* Get the number of vertices. */
    success = RwStreamReadInt32( stream,
                                 (RwInt32 *)&numVertices,
                                 sizeof(RwInt32));
    CHECKSTREAMANDRETURN(success);

    /* Get the number of triangles */
    success = RwStreamReadInt32( stream,
                                 (RwInt32 *)&numTriangles,
                                 sizeof(RwInt32));
    CHECKSTREAMANDRETURN(success);

    /* allocate a toonGeo structure */
    toonGeo = _rpToonGeoInit( numVertices, numTriangles );

    if (!toonGeo)
    {
        RWRETURN(0);
    }

    /* Triangle data */
    if (numTriangles)
    {
        success = RwStreamReadInt16(stream,
                      (RwInt16 *)(toonGeo->triangles),
                      sizeof(rpToonTriangle) * toonGeo->numTriangles);
        CHECKSTREAMANDRETURN(success);
    }

    /* Vertex thickness data */
    if (numVertices)
    {
        success = RwStreamReadReal(stream,
                      (RwReal *)(toonGeo->vertexThicknesses),
                      sizeof(RwReal) * toonGeo->numVerts);
        CHECKSTREAMANDRETURN(success);
    }

    /* Num of edges */
    success = RwStreamReadInt32(stream,
                  (RwInt32 *)&(toonGeo->numEdges),
                  sizeof(RwInt32));
    CHECKSTREAMANDRETURN(success);

    /* Num of crease edges */
    success = RwStreamReadInt32(stream,
                  (RwInt32 *)&(toonGeo->numCreaseEdges),
                  sizeof(RwInt32));
    CHECKSTREAMANDRETURN(success);

    toonGeo->edges = (RpToonEdge *) RwMalloc(sizeof(RpToonEdge) * toonGeo->numEdges,
                                             rwID_TOONPLUGIN | rwMEMHINTDUR_EVENT);
    RWASSERT( toonGeo->edges && "RwMalloc failed");
    if (!toonGeo->edges)
    {
        RWRETURN(0);
    }

    toonGeo->edgeInkIDs = (rpToonEdgeInkID *)
        RwMalloc(sizeof(rpToonEdgeInkID) * toonGeo->numEdges,
            rwID_TOONPLUGIN | rwMEMHINTDUR_EVENT);
    RWASSERT( toonGeo->edgeInkIDs && "RwMalloc failed");
    if (!toonGeo->edgeInkIDs)
    {
        RWRETURN(0);
    }

    /* Edge connectivity data */
    if (toonGeo->numEdges)
    {
        success = RwStreamReadInt16(stream,
                      (RwInt16 *)(toonGeo->edges),
                      sizeof(RpToonEdge) * toonGeo->numEdges);
        CHECKSTREAMANDRETURN(success);
    }

    /* Face normals */
    if (numTriangles)
    {
        success = RwStreamReadReal(stream,
          (RwReal *)(toonGeo->faceNormals),
          sizeof(RwV3d) * toonGeo->numTriangles);
        CHECKSTREAMANDRETURN(success);
    }

    /* extrusion normals, to be removed? */
    if (numVertices)
    {
        success = RwStreamReadReal(stream,
                      (RwReal *)(toonGeo->extrusionVertexNormals),
                      sizeof(RwV3d) * toonGeo->numVerts);
        CHECKSTREAMANDRETURN(success);
    }

    /* Num of ink IDs used on this mesh */
    success = RwStreamReadInt32(stream,
                  (RwInt32 *)&(toonGeo->inkIDCount),
                  sizeof(RwInt32));
    CHECKSTREAMANDRETURN(success);

    /* the ink ID list */
    if (toonGeo->inkIDList)
    {
        RwFree(toonGeo->inkIDList);
    }
    toonGeo->inkIDList = (rpToonInkID *)RwMalloc(sizeof(rpToonInkID) * toonGeo->inkIDCount,
                                                 rwID_TOONPLUGIN | rwMEMHINTDUR_EVENT);
    RWASSERT(toonGeo->inkIDList && "RwMalloc failed");
    if (!toonGeo->inkIDList)
    {
        RWRETURN(0);
    }

    if (RwStreamRead( stream,
                  (RwInt8 *)(toonGeo->inkIDList),
                  toonGeo->inkIDCount * sizeof(rpToonInkID))
        != toonGeo->inkIDCount * sizeof(rpToonInkID))
    {
        RWASSERT(0&&"didn't read enough bytes from stream");
        RWRETURN(0);
    }

    /* Initialize inks to null, it's up to the user to apply them after loading */
    if (toonGeo->inks)
    {
        RwFree(toonGeo->inks);
    }
    toonGeo->inks = (RpToonInk **)RwMalloc(sizeof(RpToonInk *) * toonGeo->inkIDCount,
                                           rwID_TOONPLUGIN | rwMEMHINTDUR_EVENT);
    RWASSERT(toonGeo->inks && "RwMalloc failed");
    memset(toonGeo->inks, 0, sizeof(RpToonInk*) * toonGeo->inkIDCount );

    /* Ink IDs for each edge in geometry */
    if (toonGeo->numEdges)
    {
        if (RwStreamRead( stream,
                      (RwInt8 *)(toonGeo->edgeInkIDs),
                      toonGeo->numEdges * sizeof(rpToonEdgeInkID))
            != toonGeo->numEdges * sizeof(rpToonEdgeInkID))
        {
            RWASSERT(0&&"didn't read enough bytes from stream");
            RWRETURN(0);
        }
    }

    /* default paint ID */
    if (RwStreamRead( stream,
                  (RwInt8 *)(toonGeo->defaultPaintID.name),
                  sizeof(rpToonPaintID))
        != sizeof(rpToonPaintID))
    {
        RWASSERT(0&&"didn't read enough bytes from stream");
        RWRETURN(0);
    }

    /* default render callback... ooh, weird */

    *toonGeoPtr = toonGeo;

    RWRETURN(success);
}

RwStream *
_rpToonGeoWrite(RwStream *stream,
                const RpToonGeo *toonGeo)
{
    RwStream *success;
    RwInt32 currentVersion = RPTOON_INTERNALTOONGEOSTREAMVERSION;

    RWFUNCTION(RWSTRING("_rpToonGeoWrite"));

    /*
    There's no point streaming out unoptimized edges.
    */
    RWASSERT(toonGeo->edgeInkIDsOptimized && "Use RpToonGeoOptimizeEdgeInkIDs before streaming");

    /* internal version stamp */
    success = RwStreamWriteInt32(stream,
                  (const RwInt32 *)&currentVersion,
                  sizeof(RwInt32));
    CHECKSTREAMANDRETURN(success);

    /* Num of vertices. */
    success = RwStreamWriteInt32(stream,
                  (const RwInt32 *)&(toonGeo->numVerts),
                  sizeof(RwUInt32));
    CHECKSTREAMANDRETURN(success);

    /* Num triangles */
    success = RwStreamWriteInt32(stream,
                  (const RwInt32 *)&(toonGeo->numTriangles),
                  sizeof(RwUInt32));
    CHECKSTREAMANDRETURN(success);

    /* Triangle data */
    success = RwStreamWriteInt16(stream,
                  (const RwInt16 *)(toonGeo->triangles),
                  sizeof(rpToonTriangle) * toonGeo->numTriangles);
    CHECKSTREAMANDRETURN(success);

    /* Vertex thickness data */
    success = RwStreamWriteReal(stream,
                  (const RwReal *)(toonGeo->vertexThicknesses),
                  sizeof(RwReal) * toonGeo->numVerts);
    CHECKSTREAMANDRETURN(success);

    /* Num of edges */
    success = RwStreamWriteInt32(stream,
                  (const RwInt32 *)&(toonGeo->numEdges),
                  sizeof(RwInt32));
    CHECKSTREAMANDRETURN(success);

    /* Num of crease edges */
    success = RwStreamWriteInt32(stream,
                  (const RwInt32 *)&(toonGeo->numCreaseEdges),
                  sizeof(RwInt32));
    CHECKSTREAMANDRETURN(success);

    /* Edge connectivity data */
    success = RwStreamWriteInt16(stream,
                  (const RwInt16 *)(toonGeo->edges),
                  sizeof(RpToonEdge) * toonGeo->numEdges);
    CHECKSTREAMANDRETURN(success);

    /* Face normals */
    success = RwStreamWriteReal(stream,
      (const RwReal *)(toonGeo->faceNormals),
      sizeof(RwV3d) * toonGeo->numTriangles);
    CHECKSTREAMANDRETURN(success);

    /* extrusion normals, to be removed? */
    success = RwStreamWriteReal(stream,
                  (const RwReal *)(toonGeo->extrusionVertexNormals),
                  sizeof(RwV3d) * toonGeo->numVerts);
    CHECKSTREAMANDRETURN(success);

    /* Num of ink IDs used on this mesh */
    success = RwStreamWriteInt32(stream,
                  (const RwInt32 *)&(toonGeo->inkIDCount),
                  sizeof(RwInt32));
    CHECKSTREAMANDRETURN(success);

    /* the ink ID list */
    success = RwStreamWrite( stream,
                  (const RwInt8 *)(toonGeo->inkIDList),
                  toonGeo->inkIDCount * sizeof(rpToonInkID));
    CHECKSTREAMANDRETURN(success);

    /* Ink IDs for each edge in geometry */
    success = RwStreamWrite( stream,
                  (const RwInt8 *)(toonGeo->edgeInkIDs),
                  toonGeo->numEdges * sizeof(rpToonEdgeInkID));
    CHECKSTREAMANDRETURN(success);

    /* default paint ID */
    success = RwStreamWrite( stream,
                  (const RwInt8 *)(toonGeo->defaultPaintID.name),
                  sizeof(rpToonPaintID));
    CHECKSTREAMANDRETURN(success);

    /* default render callback... ooh, weird */

    RWRETURN(success);
}

/*---------------------------------------------------------------------------*
 *-                     RpToonGeo Ink fiddling functions                     -*
 *---------------------------------------------------------------------------*/

/**
 * \ingroup rptoongeo
 * \ref RpToonGeoGetSilhouetteInk is used to retrieve the ink used as the
 * default for rendering silhouette edges on an \ref RpToonGeo.
 *
 * \param toonGeo   A pointer to the toon geometry.
 *
 * \return A \ref RpToonInk.
 *
 * \see RpToonGeoSetSilhouetteInk
 */
RpToonInk *
RpToonGeoGetSilhouetteInk(const RpToonGeo *toonGeo)
{
    RWAPIFUNCTION(RWSTRING("RpToonGeoGetSilhouetteInk"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);
    RWASSERT(toonGeo);
    RWASSERT(toonGeo->inks);

    RWRETURN(toonGeo->inks[RPTOON_INKTYPE_SILHOUETTE]);
}

/**
 * \ingroup rptoongeo
 * \ref RpToonGeoSetSilhouetteInk is used to set a \ref RpToonInk as the default
 * ink to use when rendering silhouette edges on the \ref RpToonGeo.  The \ref RpToonGeo
 * adds a reference to the ink and destroys it when it is destroyed.  If it had
 * a previous ink, that ink is destroyed.
 *
 * \param toonGeo   A pointer to the toon geometry.
 * \param ink       A pointer to the toon ink.
 *
 * \return none.
 *
 * \see RpToonGeoGetSilhouetteInk
 * \see RpToonInkAddRef
 * \see RpToonInkDestroy
 */
void
RpToonGeoSetSilhouetteInk(RpToonGeo *toonGeo,
                          RpToonInk *ink)
{
    RpToonInk *oldInk;
    RWAPIFUNCTION(RWSTRING("RpToonGeoSetSilhouetteInk"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);

    RWASSERT(toonGeo);
    RWASSERT(ink);

    /* dec old ink ref count */
    oldInk = toonGeo->inks[RPTOON_INKTYPE_SILHOUETTE];

    if (oldInk)
    {
        RpToonInkDestroy(oldInk);
    }

    /* add ref to new ink */
    RpToonInkAddRef(ink);

    /* put new ink in the "default" silhouette ink slot */
    toonGeo->inks[RPTOON_INKTYPE_SILHOUETTE] = ink;
    rwstrcpy(toonGeo->inkIDList[RPTOON_INKTYPE_SILHOUETTE].name, ink->name);

    RWRETURNVOID();
}

/**
 * \ingroup rptoongeo
 * \ref RpToonGeoGetCreaseInk is used to retrieve the ink used as the
 * default for rendering crease edges on an \ref RpToonGeo.
 *
 * \param toonGeo   A pointer to the toon geometry.
 *
 * \return A \ref RpToonInk.
 *
 * \see RpToonGeoSetCreaseInk
 */
RpToonInk *
RpToonGeoGetCreaseInk(const RpToonGeo *toonGeo)
{
    RWAPIFUNCTION(RWSTRING("RpToonGeoGetCreaseInk"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);
    RWASSERT(toonGeo);
    RWASSERT(toonGeo->inks);

    RWRETURN(toonGeo->inks[RPTOON_INKTYPE_CREASE]);
}

/**
 * \ingroup rptoongeo
 * \ref RpToonGeoSetCreaseInk is used to set a \ref RpToonInk as the default
 * ink to use when rendering crease edges on the \ref RpToonGeo. The \ref RpToonGeo
 * adds a reference to the ink and destroys it when it is destroyed.  If it had
 * a previous ink, that ink is destroyed.
 *
 * \param toonGeo   A pointer to the toon geometry.
 * \param ink       A pointer to the toon ink.
 *
 * \return none.
 *
 * \see RpToonGeoGetCreaseInk
 * \see RpToonInkAddRef
 * \see RpToonInkDestroy
 */
void
RpToonGeoSetCreaseInk(RpToonGeo *toonGeo,
                      RpToonInk *ink)
{
    RpToonInk *oldInk;

    RWAPIFUNCTION(RWSTRING("RpToonGeoSetCreaseInk"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);

    RWASSERT(toonGeo);
    RWASSERT(ink);

    /* dec old ink ref count */
    oldInk = toonGeo->inks[RPTOON_INKTYPE_CREASE];

    if (oldInk)
    {
        RpToonInkDestroy(oldInk);
    }

    /* add ref to new ink */
    RpToonInkAddRef(ink);

    /* put new ink in the "default" crease ink slot */
    toonGeo->inks[RPTOON_INKTYPE_CREASE] = ink;
    rwstrcpy(toonGeo->inkIDList[RPTOON_INKTYPE_CREASE].name, ink->name);

    RWRETURNVOID();
}

/**
 * \ingroup rptoongeo
 * \ref RpToonGeoForAllInkIDs is used to iterate over ink names in a
 * \ref RpToonGeo object. The function enumerateInkIDsCallBack is
 * called with each ink ID (name) the \ref RpToonGeo stores.
 * This is useful if you're writing an editor
 * for toon inks, or are looking for a certain ink on an object that
 * needs to be treated specially.
 *
 * \param toonGeo   A pointer to toon geometry.
 * \param enumerateInkIDsCallBack     The callback function.
 * \param data      data passed on to the callback function.
 *
 * \see RpToonGeoForAllInkIDsCallBack
 *
 * \return none.
 */
void
RpToonGeoForAllInkIDs(const RpToonGeo *toonGeo,
                      RpToonGeoForAllInkIDsCallBack *enumerateInkIDsCallBack,
                      void *data)
{
    RwInt32 i;

    RWAPIFUNCTION(RWSTRING("RpToonGeoForAllInkIDs"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);
    RWASSERT(toonGeo);
    RWASSERT(enumerateInkIDsCallBack);

    for (i = 0; i < toonGeo->inkIDCount; i++)
    {
        if (!(*enumerateInkIDsCallBack)((const char*)(toonGeo->inkIDList[i].name), data))
        {
            RWRETURNVOID();
        }
    }

    RWRETURNVOID();
}

/**
 * \ingroup rptoongeo
 * \ref RpToonGeoApplyInksInInkDictionary is used to link named inks in a
 * \ref RpToonInkDictionary to a \ref RpToonGeo object.  The \ref RpToonGeo
 * checks each ink ID it stores against the contents of the dictionary. If
 * a \ref RpToonInk s name matches, it's reference count is incremented and
 * the \ref RpToonGeo keeps a direct pointer to that ink.  Next time the \ref RpToonGeo
 * is rendered, it will use that ink style.  When the \ref RpToonGeo
 * is destroyed, it will destroy the \ref RpToonInk too.
 *
 * This is different to the way in which \ref RwTexture's in a \ref RwTexDictionary
 * are applied to an object to allow you to quickly and easily change
 * ink styles on a character without reloading it.
 *
 * \param toonGeo   A pointer to toon geometry.
 * \param inkDictionary     A pointer to an ink dictionary.
 *
 * \return none.
 */
void
RpToonGeoApplyInksInInkDictionary(RpToonGeo *toonGeo,
                                  const RpToonInkDictionary *inkDictionary)
{
    RwInt32 i;

    RWAPIFUNCTION(RWSTRING("RpToonGeoApplyInksInInkDictionary"));

    RWASSERT(toonGeo);
    RWASSERT(inkDictionary);

    /* check each inkID entry, if we find a matching ink in dictionary, set it */
    for (i=0; i<toonGeo->inkIDCount; i++)
    {
       /* dec old ink's ref count */
       RpToonInk *newInk = RpToonInkDictionaryFindNamedInk(
            inkDictionary, toonGeo->inkIDList[i].name );

       if (newInk)
       {
           /* dec old ink's ref count */
           if (toonGeo->inks[i])
           {
                RpToonInkDestroy(toonGeo->inks[i]);
           }

           /* and bump new ink's ref count */
           RpToonInkAddRef(newInk);

           toonGeo->inks[i] = newInk;
       }
    }

    RWRETURNVOID();
}

/*---------------------------------------------------------------------------*
 *-                    RpToonGeo Paint fiddling functions                   -*
 *---------------------------------------------------------------------------*/

/**
 * \ingroup rptoongeo
 * \ref RpToonGeoGetPaint is used to retrieve the paint used as the
 * default for rendering a \ref RpToonGeo.
 *
 * \param toonGeo   A pointer to the toon geometry.
 *
 * \return A \ref RpToonPaint.
 *
 * \see RpToonGeoSetPaint
 */
RpToonPaint *
RpToonGeoGetPaint(RpToonGeo *toonGeo)
{
    RWAPIFUNCTION(RWSTRING("RpToonGeoGetPaint"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);

    RWASSERT(toonGeo != 0);

    RWRETURN(toonGeo->paint);
}

/**
 * \ingroup rptoongeo
 * \ref RpToonGeoSetPaint is used to set a \ref RpToonPaint as the default
 * paint to use when rendering the \ref RpToonGeo.  The \ref RpToonGeo
 * adds a reference to the paint and destroys it when it is destroyed.  If it had
 * a previous paint, that paint is destroyed.
 *
 * \param toonGeo   A pointer to the toon geometry.
 * \param paint     A pointer to the toon paint.
 *
 * \return none.
 *
 * \see RpToonGeoGetPaint
 * \see RpToonPaintAddRef
 * \see RpToonPaintDestroy
 */
void
RpToonGeoSetPaint(RpToonGeo *toonGeo,
                  RpToonPaint *paint)
{
    RWAPIFUNCTION(RWSTRING("RpToonGeoSetPaint"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);

    RWASSERT(toonGeo != 0);
    RWASSERT(paint != 0);

    /* put it in the "default" paint slot */
    if (toonGeo->paint)
    {
        RpToonPaintDestroy(toonGeo->paint);
    }

    toonGeo->paint = paint;
    rwstrcpy(toonGeo->defaultPaintID.name, paint->name);

    /* add a reference to it*/
    RpToonPaintAddRef(paint);

    RWRETURNVOID();
}

static RpMeshHeader *
GetMeshHeaderForToonGeo(const RpToonGeo *toonGeo)
{
    RpMeshHeader *meshHeader;

    RWFUNCTION(RWSTRING("GetMeshHeaderForToonGeo"));

    if (toonGeo->parentGeometry)
    {
        meshHeader = toonGeo->parentGeometry->mesh;
    }
    else
    {
        RWASSERT(toonGeo->parentWorldSector);
        meshHeader = toonGeo->parentWorldSector->mesh;
    }

    RWASSERT( meshHeader );

    RWRETURN(meshHeader);
}

/**
 * \ingroup rptoongeo
 * \ref RpToonGeoForAllPaintIDs is used to iterate over paint names referenced by a
 * \ref RpToonGeo object and any of its materials. The function forAllPaintIDsCallBack is
 * called with each paint ID (name) the \ref RpToonGeo stores.
 * This is useful if you're writing an editor
 * for toon paints, or are looking for a certain paint on an object that
 * needs to be treated specially.
 *
 * \param toonGeo                       A pointer to toon geometry.
 * \param forAllPaintIDsCallBack        The callback function.
 * \param data                          data passed on to the callback function.
 *
 * \return none.
 *
 * \see RpToonGeoForAllPaintIDsCallBack
 */
void
RpToonGeoForAllPaintIDs(const RpToonGeo *toonGeo,
                        RpToonGeoForAllPaintIDsCallBack *forAllPaintIDsCallBack,
                        void *data )
{
    RwInt32 i;
    RpMesh *mesh;
    RpMeshHeader *meshHeader;

    RWAPIFUNCTION(RWSTRING("RpToonGeoForAllPaintIDs"));
    RWASSERT(toonGeo != 0);
    RWASSERT(forAllPaintIDsCallBack != 0);

    if (!(*forAllPaintIDsCallBack)(toonGeo->defaultPaintID.name, data))
    {
        RWRETURNVOID();
    }

    /* do materials with overrides.... */
    meshHeader = GetMeshHeaderForToonGeo(toonGeo);

    mesh = (RpMesh *)(meshHeader + 1);

    for (i=0; i<meshHeader->numMeshes; i++, mesh++)
    {
        RpMaterial *mat = mesh->material;
        RpToonMaterial *toonMat;

        toonMat = *RPTOONMATERIALGETDATA(mat);

        if (toonMat && toonMat->overrideGeometryPaint)
        {
            if (!(*forAllPaintIDsCallBack)(toonMat->paintName, data))
            {
                RWRETURNVOID();
            }
        }
    }

    RWRETURNVOID();
}

/**
 * \ingroup rptoongeo
 * \ref RpToonGeoApplyPaintsInPaintDictionary is used to link named paints in a
 * \ref RpToonPaintDictionary to a \ref RpToonGeo object.  The \ref RpToonGeo
 * checks each paint ID it stores against the contents of the dictionary. If
 * a \ref RpToonPaint s name matches, it's reference count is incremented and
 * the \ref RpToonGeo keeps a direct pointer to that paint.  Next time the \ref RpToonGeo
 * is rendered, it will use that paint style.  When the \ref RpToonGeo
 * is destroyed, it will destroy the \ref RpToonPaint too.
 *
 * This is different to the way in which \ref RwTexture's in a \ref RwTexDictionary
 * are applied to an object to allow you to quickly and easily change
 * paint styles on a character without reloading it.
 *
 * \param toonGeo   A pointer to toon geometry.
 * \param paintDictionary     A pointer to the paint dictionary.
 *
 * \return none.
 */
void
RpToonGeoApplyPaintsInPaintDictionary(RpToonGeo *toonGeo,
                                      const RpToonPaintDictionary *paintDictionary)
{
    /*
    Paste the default paint and all material's paints used in from the dictionary
    Change this for multiple light support
    - Each light has a paint dictionary that tells it what each paint looks like to it.
    - On render we add the effect of each light, looking up each paint on the geometry/material's
      dictionary to find its contribution???
    */
    RpToonPaint *paint;
    RwInt32 i;
    RpMeshHeader *meshHeader;
    RpMesh *mesh;

    RWAPIFUNCTION(RWSTRING("RpToonGeoApplyPaintsInPaintDictionary"));
    RWASSERT(toonGeo != 0);
    RWASSERT(paintDictionary != 0);

    paint = RpToonPaintDictionaryFindNamedPaint(paintDictionary, toonGeo->defaultPaintID.name);

    if (paint)
    {
        if (toonGeo->paint)
        {
            RpToonPaintDestroy(toonGeo->paint);
        }

        toonGeo->paint = paint;

        RpToonPaintAddRef(paint);
    }

    /* do materials with overrides.... */
    meshHeader = GetMeshHeaderForToonGeo(toonGeo);

    mesh = (RpMesh *)(meshHeader + 1);

    for (i=0; i<meshHeader->numMeshes; i++, mesh++)
    {
        RpMaterial *mat = mesh->material;
        RpToonMaterial *toonMat;

        toonMat = *RPTOONMATERIALGETDATA(mat);

        if (toonMat && toonMat->overrideGeometryPaint)
        {
            paint = RpToonPaintDictionaryFindNamedPaint(paintDictionary, toonMat->paintName);

            toonMat->paint = paint;
        }
    }

    RWRETURNVOID();
}

/**
 * \ingroup rptoongeo
 * \ref RpToonGeoNotifyVertexPositionsDirty is used to tell an \ref RpToonGeo that vertex positions
 * have been modified, and dependent information it stores will need to be updated.
 * In general, this shouldn't be necessary, but on platforms that chain the render callback
 * to do line rendering (PC, GameCube, Xbox), \ref rpdmorph can update the information in a
 * way in which the \ref RpToonGeo can't reasonably figure out for itself.  Additional chained
 * custom render callbacks can have this effect too.
 *
 * \param toonGeo   A pointer to toon geometry.
 *
 * \return none.
 */
void
RpToonGeoNotifyVertexPositionsDirty(RpToonGeo *toonGeo)
{
    RWAPIFUNCTION(RWSTRING("RpToonGeoNotifyVertexPositionsDirty"));
    RWASSERT(toonGeo);
    RWASSERT(0 < _rpToonGlobals.module.numInstances);

    toonGeo->vertexPositionsDirty = TRUE;

    RWRETURNVOID();
}

/*---------------------------------------------------------------------------*
 *-                    RpToonGeoVertexThickness functions                   -*
 *---------------------------------------------------------------------------*/

/**
 * \ingroup rptoongeo
 * \ref RpToonGeoGetVertexThicknesses is used to gain read only access to
 * vertex thickness multipliers stored in a \ref RpToonGeo.
 *
 * \param toonGeo   A pointer to toon geometry.
 *
 * \return An array of RwReals.
 *
 * \see RpToonGeoLockVertexThicknesses
 * \see RpToonGeoUnlockVertexThicknesses
 */
const RwReal *
RpToonGeoGetVertexThicknesses(RpToonGeo *toonGeo)
{
    RWAPIFUNCTION(RWSTRING("RpToonGeoGetVertexThicknesses"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);

    RWASSERT(toonGeo != 0);

    RWRETURN(toonGeo->vertexThicknesses);
}

/**
 * \ingroup rptoongeo
 * \ref RpToonGeoLockVertexThicknesses is used to begin read/write access to
 * vertex thickness multipliers stored in a \ref RpToonGeo.
 *
 * \param toonGeo   A pointer to toon geometry.
 *
 * \return An array of RwReals.
 *
 * \see RpToonGeoGetVertexThicknesses
 * \see RpToonGeoUnlockVertexThicknesses
 */
RwReal *
RpToonGeoLockVertexThicknesses(RpToonGeo *toonGeo)
{
    RWAPIFUNCTION(RWSTRING("RpToonGeoLockVertexThicknesses"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);

    RWASSERT(toonGeo != 0);

    RWRETURN(toonGeo->vertexThicknesses);
}

/**
 * \ingroup rptoongeo
 * \ref RpToonGeoUnlockVertexThicknesses is used to end read/write access to
 * vertex thickness multipliers stored in a \ref RpToonGeo.  The multipliers
 * will be reinstanced if necessary for the particular platform.
 *
 * \param toonGeo   A pointer to toon geometry.
 *
 * \return none.
 *
 * \see RpToonGeoGetVertexThicknesses
 * \see RpToonGeoLockVertexThicknesses
 */
void
RpToonGeoUnlockVertexThicknesses(RpToonGeo * toonGeo __RWUNUSEDRELEASE__)
{
    RWAPIFUNCTION(RWSTRING("RpToonGeoUnlockVertexThicknesses"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);

    RWASSERT(toonGeo != 0);

    RWRETURNVOID();
}
