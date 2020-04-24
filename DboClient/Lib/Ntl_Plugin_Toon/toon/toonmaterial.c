#include <string.h>

#include "rwcore.h"
#include "rpworld.h"

#include "rpplugin.h"
#include "rpdbgerr.h"

#include "toon.h"
#include "toonmaterial.h"

#define rpToonMaterialVersionStamp                      0x67890002
#define rpToonMaterialVersionStampWithSilhouetteInkIDs  0x67890002

/**
 * \defgroup rptoonmaterial RpToonMaterial
 * \ingroup rptoon
 */

#define CHECKSTREAMANDRETURN(success) \
MACRO_START                           \
{                                     \
    if(NULL == (success))             \
    {                                 \
        RWRETURN((RwStream *)NULL);   \
    }                                 \
}                                     \
MACRO_STOP

/*
 * C union to synthesise C++ mutable
 */
typedef union RpMutableMaterial RpMutableMaterial;
union RpMutableMaterial
{
    RpMaterial          *Ptr;
    const    RpMaterial *constPtr;
};

/**
 * \ingroup rptoonmaterial
 * \ref RpToonMaterialCreate Creates a toon material with default values.
 *
 * \return A pointer to new material or NULL if there is an error.
 *
 * \see RpToonMaterialDestroy
 */
RpToonMaterial *
RpToonMaterialCreate(void)
{
    RpToonMaterial *mat;

    RWAPIFUNCTION(RWSTRING("RpToonMaterialCreate"));

    mat = (RpToonMaterial *)RwMalloc( sizeof(RpToonMaterial),
                      rwID_TOONPLUGIN | rwMEMHINTDUR_EVENT);

    if (!mat)
    {
        RWRETURN(0);
    }

    memset(mat, 0, sizeof(RpToonMaterial));

    RWRETURN(mat);
}

/**
 * \ingroup rptoonmaterial
 * \ref RpToonMaterialDestroy Destroys a Toon Material.
 *
 * \param mat  The new toon material to destroy.
 *
 * \return none.
 *
 * \see RpToonMaterialCreate
 */
void
RpToonMaterialDestroy( RpToonMaterial *mat )
{
    RWAPIFUNCTION(RWSTRING("RpToonMaterialDestroy"));

    RwFree(mat);

    RWRETURNVOID();
}

/**
 * \ingroup rptoonmaterial
 * \ref RpToonMaterialSetPaintID is used to set a paint name to be associated with
 * a material.  This paint will then take precedence over the default paint
 * associated with an RpToonGeo.
 *
 * \param mat the material
 * \param name the paint name (NULL is allowed)
 *
 * \see RpToonGeoForAllPaintIDs
 * \see RpToonGeoApplyPaintsInPaintDictionary
 *
 */
void
RpToonMaterialSetPaintID( RpMaterial *mat, const RwChar *name )
{
    RpToonMaterial *toonMat;

    RWAPIFUNCTION(RWSTRING("RpToonMaterialSetPaintID"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);
    RWASSERT(mat != 0);

    /* create a toon material if we've not got one already */
    toonMat = *RPTOONMATERIALGETDATA(mat);

    if (!toonMat)
    {
        toonMat = RpToonMaterialCreate();
        *RPTOONMATERIALGETDATA(mat) = toonMat;
    }

    if (name)
    {
        toonMat->overrideGeometryPaint = TRUE;

		//HoDong I do not know if there is a problem to change this number. (Later Test)
		strncpy(toonMat->paintName, name, rwTEXTUREBASENAMELENGTH - 1);
		//strncpy_s(toonMat->paintName, rwTEXTUREBASENAMELENGTH - 1, name, rwTEXTUREBASENAMELENGTH - 1);
    }
    else
    {
        toonMat->overrideGeometryPaint = FALSE;
    }

    RWRETURNVOID();
}

static void        *
ToonMaterialConstructor(void *object,
                        RwInt32 offset __RWUNUSED__,
                        RwInt32 size __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("ToonMaterialConstructor"));
    RWASSERT(object);

    *RPTOONMATERIALGETDATA(object) = (RpToonMaterial *) NULL;

    RWRETURN(object);
}

static  RwInt32
ToonMaterialStreamGetSize(const void *object,
                          RwInt32 __RWUNUSED__ offsetInObject,
                          RwInt32 __RWUNUSED__ sizeInObject)
{
    RwInt32 size = 0;
    const RpToonMaterial *toonMat;

    RWFUNCTION(RWSTRING("ToonMaterialStreamGetSize"));
    RWASSERT(object);

    toonMat = (const RpToonMaterial *) *RPTOONMATERIALGETCONSTDATA(object);

    if (toonMat)
    {
        size =  sizeof(RwInt32)             /* version stamp */
             +  sizeof(RwBool)              /* enabled */
             +  rwTEXTUREBASENAMELENGTH     /* name */
             +  sizeof(RwInt32);            /* silhouette ink ID */
    }

    RWRETURN(size);
}

static RwStream    *
ToonMaterialStreamWrite(RwStream * stream,
                        RwInt32 binaryLength __RWUNUSED__,
                        const void *object,
                        RwInt32 offsetInObject __RWUNUSED__,
                        RwInt32 sizeInObject __RWUNUSED__)
{
    RpMutableMaterial   material;
    RpToonMaterial     *toonMat;
    RwStream           *success;
    RwInt32             currentVersion = rpToonMaterialVersionStamp;

    RWFUNCTION(RWSTRING("ToonMaterialStreamWrite"));
    RWASSERT(stream);
    RWASSERT(object);

    material.constPtr = (const RpMaterial *) object;
    toonMat = (RpToonMaterial *) *RPTOONMATERIALGETDATA(material.Ptr);

    if (toonMat)
    {
        /* write an internal version stamp */
        success = RwStreamWriteInt32(stream,
                                     (const RwInt32 *) &currentVersion,
                                     sizeof(RwInt32));
        CHECKSTREAMANDRETURN(success);

        /* we have a toon material, write out enabled state & name */
        success = RwStreamWriteInt32(stream,
                                     (const RwInt32 *) &toonMat->
                                     overrideGeometryPaint,
                                     sizeof(RwInt32));
        CHECKSTREAMANDRETURN(success);

        /* the paint name */
        success = RwStreamWrite(stream,
                                (const RwInt8 *) (toonMat->paintName),
                                rwTEXTUREBASENAMELENGTH);
        CHECKSTREAMANDRETURN(success);

        /* the silhouette ink ID */
        success = RwStreamWriteInt32(stream,
                                     (const RwInt32 *) &toonMat->
                                     silhouetteInkID, sizeof(RwInt32));
        CHECKSTREAMANDRETURN(success);
    }

    RWRETURN(stream);
}

static RwStream    *
ToonMaterialStreamRead(RwStream * stream,
                        RwInt32 binaryLength __RWUNUSED__,
                        void *object,
                        RwInt32 offsetInObject __RWUNUSED__,
                        RwInt32 sizeInObject __RWUNUSED__)
{
    RpMaterial *material;
    RpToonMaterial *toonMat;
    RwInt32 version;
    RwStream *success;

    RWFUNCTION(RWSTRING("ToonMaterialStreamRead"));
    RWASSERT(stream);
    RWASSERT(object);

    material = (RpMaterial *) object;
    toonMat = (RpToonMaterial *) *RPTOONMATERIALGETDATA(material);

    /* make a toon material if there isn't one already */
    if (!toonMat)
    {
        toonMat = RpToonMaterialCreate();
        if (!toonMat)
        {
            RWRETURN(0);
        }

        *RPTOONMATERIALGETDATA(material) = toonMat;
    }

    success = RwStreamReadInt32(stream, &version, sizeof(RwInt32));
    CHECKSTREAMANDRETURN(success);

    if (version > rpToonMaterialVersionStamp)
    {
        RWERROR((E_RW_BADVERSION, RWSTRING("Incompatible toon material stream version: re-export required")));
        RWRETURN(0);
    }

    success = RwStreamReadInt32(stream, &toonMat->overrideGeometryPaint, sizeof(RwInt32));
    CHECKSTREAMANDRETURN(success);

    if (toonMat->overrideGeometryPaint ||
        (version >= rpToonMaterialVersionStampWithSilhouetteInkIDs))
    {
        if (RwStreamRead( stream, (RwInt8 *)(toonMat->paintName), rwTEXTUREBASENAMELENGTH)
            != rwTEXTUREBASENAMELENGTH)
        {
            RWRETURN(0);
        }
    }

    if (version >= rpToonMaterialVersionStampWithSilhouetteInkIDs)
    {
        success = RwStreamReadInt32(stream, &toonMat->silhouetteInkID, sizeof(RwInt32));
        CHECKSTREAMANDRETURN(success);
    }

    RWRETURN(stream);
}

static void        *
ToonMaterialCopy(void *dstObject,
                  const void *srcObject,
                  RwInt32 offset __RWUNUSED__,
                  RwInt32 size __RWUNUSED__)
{
    const RpToonMaterial *srcToonMat;
    RpToonMaterial *dstToonMat;

    RWFUNCTION(RWSTRING("ToonMaterialCopy"));

    srcToonMat = *RPTOONMATERIALGETCONSTDATA(srcObject);
    dstToonMat = *RPTOONMATERIALGETDATA(dstObject);

    if (srcToonMat)
    {
        /* we have a source material extension, allocate and copy for dst */
        if (dstToonMat)
        {
            RpToonMaterialDestroy(dstToonMat);
        }

        dstToonMat = RpToonMaterialCreate();

        if (!dstToonMat)
        {
            RWRETURN(0);
        }

        /* deep copy */
        *dstToonMat = *srcToonMat;
    }
    else
    {
        /* no source material, destroy dst material if it exists */
        RpToonMaterialDestroy(dstToonMat);
        dstToonMat = 0;
    }

    /* set it to our copied material, whatever it is */
    *RPTOONMATERIALGETDATA(dstObject) = dstToonMat;

    RWRETURN( dstObject );
}

static void        *
ToonMaterialDestructor(void *object,
                        RwInt32 offset __RWUNUSED__,
                        RwInt32 size __RWUNUSED__)
{
    RpToonMaterial *toonMat;

    RWFUNCTION(RWSTRING("ToonMaterialDestructor"));
    RWASSERT(object);

    toonMat = (RpToonMaterial *) *RPTOONMATERIALGETDATA(object);

    if (toonMat)
    {
        RpToonMaterialDestroy(toonMat);

        *RPTOONMATERIALGETDATA(object) = (RpToonMaterial *) NULL;
    }

    RWRETURN(object);
}

RwBool
_rpToonMaterialPluginAttach(void)
{
    RwBool success;

    RWFUNCTION(RWSTRING("_rpToonMaterialPluginAttach"));

    /* Register material plugIn */
    _rpToonGlobals.materialOffset =
        RpMaterialRegisterPlugin(sizeof(RpToonMaterial *),
                                 (RwUInt32)rwID_TOONPLUGIN,
                                 ToonMaterialConstructor,
                                 ToonMaterialDestructor,
                                 ToonMaterialCopy);

    RWASSERT(0 < _rpToonGlobals.materialOffset);

    /* Attach the stream handling functions */
    success = RpMaterialRegisterPluginStream((RwUInt32)rwID_TOONPLUGIN,
                                       ToonMaterialStreamRead,
                                       ToonMaterialStreamWrite,
                                       ToonMaterialStreamGetSize);
    RWASSERT(0 < success);

    RWRETURN(success);
}
