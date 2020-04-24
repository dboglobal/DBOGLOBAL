#include <string.h>

#include "rwcore.h"
#include "rpworld.h"

#include "rpplugin.h"
#include "rpdbgerr.h"

#include "toongeo.h"
#include "toon.h"

#include "toonpaint.h"


#define rpToonPaintVersionStamp             0x43210001
#define rpToonPaintDictionaryVersionStamp   ((RwInt32)0xbcde0001)



/**
 * \defgroup rptoonpaintdictionary RpToonPaintDictionary
 * \ingroup rptoon
 */
#if (!defined(DOXYGEN))
struct RpToonPaintDictionary
{
    int paintCount;
    RpToonPaint **paints;
};
#endif /* (!defined(DOXYGEN)) */

/**
 * \defgroup rptoonpaint RpToonPaint
 * \ingroup rptoon
 */

/**
 * \ingroup rptoonpaint
 * \ref RpToonPaintCreate is used to create a new \ref RpToonPaint with default values
 * and a reference count of 1.
 *
 * \return Returns a pointer to the newly created \ref RpToonPaint if successful or NULL
 * if there is an error
 *
 * \see RpToonPaintAddRef
 * \see RpToonPaintDestroy
 *
 */
RpToonPaint *
RpToonPaintCreate(void)
{
    RpToonPaint *paint;

    RWAPIFUNCTION(RWSTRING("RpToonPaintCreate"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);

    paint = (RpToonPaint *)RwMalloc( sizeof(RpToonPaint),
        rwID_TOONPLUGIN | rwMEMHINTDUR_EVENT );
    RWASSERT( (paint != 0) && "RwMalloc failed");
    paint->refCount = 1;

    /* need an RwTexture to stream before we can call it shaded */
    paint->type = RPTOON_PAINTTYPE_FLAT;

    paint->gradient = 0;
    rwstrcpy( paint->name, "default paint name" );

    RWRETURN(paint);
}

/**
 * \ingroup rptoonpaint
 * \ref RpToonPaintDestroy is used to destroy an RpToonPaint.  It decrements the
 * paint's reference count, and if it's 0, it is destroyed.
 *
 * \param paint the \ref RpToonPaint to destroy.
 *
 * \return none.
 *
 * \see RpToonPaintAddRef
 * \see RpToonPaintDestroy
 *
 */
void
RpToonPaintDestroy( RpToonPaint * paint )
{
    RWAPIFUNCTION(RWSTRING("RpToonPaintDestroy"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);

    RWASSERT( paint != 0 );

    paint->refCount--;
    if (paint->refCount == 0)
    {
        if (paint->gradient)
        {
            RwTextureDestroy(paint->gradient);
        }

        RwFree( (void *)paint );
    }

    RWRETURNVOID();
}

/**
 * \ingroup rptoonpaint
 * \ref RpToonPaintAddRef is used to increment the reference count of an \ref RpToonPaint.
 *
 * \param paint the \ref RpToonPaint to reference.
 *
 * \return none.
 *
 * \see RpToonPaintDestroy
 * \see RpToonPaintCreate
 *
 */
void
RpToonPaintAddRef( RpToonPaint * paint )
{
    RWAPIFUNCTION(RWSTRING("RpToonPaintAddRef"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);

    RWASSERT( paint != 0 );
    paint->refCount++;
    RWRETURNVOID();
}

/**
 * \ingroup rptoonpaint
 * \ref RpToonPaintGetType is used to get a paint's type.
 *
 * \param paint  A pointer to the paint.
 *
 * \return The paint's type.
 *
 * \see RpToonPaintSetType
 * \see RpToonPaintType
 */
RpToonPaintType
RpToonPaintGetType( const RpToonPaint *paint )
{
    RWAPIFUNCTION(RWSTRING("RpToonPaintGetType"));
    RWASSERT( paint != 0 );
    RWRETURN( paint->type );
}

/**
 * \ingroup rptoonpaint
 * \ref RpToonPaintSetType is used to set a paint's type.
 *
 * \param paint  A pointer to the paint.
 * \param type  The new type.
 *
 * \return none.
 *
 * \see RpToonPaintGetType
 * \see RpToonPaintType
 */
void
RpToonPaintSetType( RpToonPaint *paint, RpToonPaintType type )
{
    RWAPIFUNCTION(RWSTRING("RpToonPaintSetType"));
    RWASSERT( paint != 0 );

    paint->type = type;

    RWRETURNVOID();
}

/**
 * \ingroup rptoonpaint
 * \ref RpToonPaintGetName is used to get a paint's name.
 * Paint names are used as keys in \ref RpToonPaintDictionary objects.
 *
 * \param paint  A pointer to the paint.
 *
 * \return the paints name.
 *
 * \see RpToonPaintSetName
 */
const RwChar *
RpToonPaintGetName( const RpToonPaint *paint )
{
    RWAPIFUNCTION(RWSTRING("RpToonPaintGetName"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);

    RWASSERT( paint != 0 );

    RWRETURN( paint->name );
}

/**
 * \ingroup rptoonpaint
 * \ref RpToonPaintSetName is used to set a paint's name.
 * Paint names are used as keys in \ref RpToonPaintDictionary objects.
 *
 * \param paint  A pointer to the paint.
 * \param name  A string specifying the new name.
 *
 * \return none.
 *
 * \see RpToonPaintGetName
 */
void
RpToonPaintSetName( RpToonPaint *paint, const RwChar *name )
{
    RWAPIFUNCTION(RWSTRING("RpToonPaintSetName"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);

    RWASSERT( paint != 0 );

    rwstrncpy( paint->name, name, rwTEXTUREBASENAMELENGTH - 1);

    RWRETURNVOID();
}

/**
 * \ingroup rptoonpaint
 * \ref RpToonPaintGetGradientTexture is used to get a paint's gradient texture.
 *
 * \param paint  A pointer to the paint.
 *
 * \return the paints gradient texture.
 *
 * \see RpToonPaintSetGradientTexture
 */
RwTexture *
RpToonPaintGetGradientTexture( const RpToonPaint *paint )
{
    RWAPIFUNCTION(RWSTRING("RpToonPaintGetGradientTexture"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);

    RWASSERT( paint != 0 );

    RWRETURN( paint->gradient );
}

#ifdef RTOONTRYCUBEMAPS
extern RwRaster * RpCubeCreate( RwInt32 dimensions, RwInt32 bitdepth, RwInt32 flags );
extern void RpCubeGetFace( const RwRaster *cubeRaster, RwInt32 faceIndex, RwRaster *dstRaster );
#endif


/**
 * \ingroup rptoonpaint
 * \ref RpToonPaintSetGradientTexture is used to set a paint's gradient texture.
 * The gradient texture is used when the paint's type is RPTOON_PAINTTYPE_TOONSHADE.
 *
 * \param paint  A pointer to the paint.
 * \param texture the new gradient texture.
 *
 * \return none.
 *
 * \see RpToonPaintGetGradientTexture
 * \see RpToonPaintType
 */
void
RpToonPaintSetGradientTexture( RpToonPaint *paint, RwTexture *texture )
{
    RwTexture *status;

    RWAPIFUNCTION(RWSTRING("RpToonPaintSetGradientTexture"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);

    RWASSERT( paint != 0 );

    /* dec ref count on old texture */
    if (paint->gradient)
    {
        RwTextureDestroy(paint->gradient);
    }

    /* add ref to new texture */
    if (texture)
    {
        RwTextureAddRef(texture);
    }
    else
    {
        paint->gradient = 0;
        paint->type = RPTOON_PAINTTYPE_FLAT;
        RWRETURNVOID();
    }

    paint->type = RPTOON_PAINTTYPE_TOONSHADE;

#ifndef RTOONTRYCUBEMAPS
    paint->gradient = texture;
#else
    {
        RwRaster *cubeRas, *srcRas, *faceRaster;
        RwInt32 faceIndex, width, cubeWidth;
        RwUInt32 *srcPixels;

        srcRas = RwTextureGetRaster(texture);
        width = RwRasterGetWidth(srcRas);

        if (paint->gradient)
        {
            RwTextureDestroy(paint->gradient);
        }

        cubeWidth = 512;
        cubeRas = RpCubeCreate( cubeWidth, 0, rwRASTERFORMAT8888 | rwRASTERTYPETEXTURE );
        paint->gradient = RwTextureCreate(cubeRas);

        srcPixels = (RwUInt32 *)RwRasterLock(srcRas, 0, rwRASTERLOCKREAD);

        faceRaster = RwRasterCreate(0, 0, 0, rwRASTERTYPETEXTURE );

        for (faceIndex = 0; faceIndex<6; faceIndex++)
        {
            RwUInt32 *pixels;
            RwInt32 stride;

            RpCubeGetFace( cubeRas, faceIndex, faceRaster );

            pixels = (RwUInt32 *)RwRasterLock(faceRaster, 0, rwRASTERLOCKWRITE);

            stride = RwRasterGetStride(faceRaster);

            {
            RwReal       w, h, dot;
            RwInt32      x, y;
            RwV3d n;
            RwV3d z = {0,0,1};

            for(y = 0; y < cubeWidth; y++ )
            {
                h  = (RwReal)y / (RwReal)(cubeWidth-1);  /* 0 to 1 */
                h  = ( h * 2.0f ) - 1.0f;           /* -1 to 1 */

                for(x = 0; x < cubeWidth; x++ )
                {
                    w = (RwReal)x / (RwReal)(cubeWidth-1);   /* 0 to 1 */
                    w = ( w * 2.0f ) - 1.0f;            /* -1 to 1 */

                    /* Calc the normal for this texel */
                    switch( faceIndex )
                    {
                        case 0:    /* +x */
                            n.x = +1.0;
                            n.y = -h;
                            n.z = -w;
                            break;

                        case 1:    /* -x */
                            n.x = -1.0;
                            n.y = -h;
                            n.z = +w;
                            break;

                        case 2:    /* y */
                            n.x = +w;
                            n.y = +1.0;
                            n.z = +h;
                            break;

                        case 3:    /* -y */
                            n.x = +w;
                            n.y = -1.0;
                            n.z = -h;
                            break;

                        case 4:    /* +z */
                            n.x = +w;
                            n.y = -h;
                            n.z = +1.0;
                            break;

                        case 5:    /* -z */
                            n.x = -w;
                            n.y = -h;
                            n.z = -1.0;
                            break;
                    }

                    /* Store the normal as an RGBA color */
                    RwV3dNormalize(&n, &n);
                    dot = max( RwV3dDotProduct(&n, &z), 0.0f) * (RwReal)(width - 1) + 0.5f;

                    pixels[ y * cubeWidth + x ] = srcPixels[ (RwInt32)dot ];
                }
            }

            }

            RwRasterUnlock(faceRaster);
        }

        RwRasterDestroy(faceRaster);

        RwRasterUnlock(srcRas);
    }
#endif

    status = RwTextureSetAddressing(paint->gradient,
                                    rwTEXTUREADDRESSCLAMP );

    RWRETURNVOID();
}

/**
 * \ingroup rptoonpaint
 * \ref RpToonPaintStreamRead is used to read an paint from the specified
 * binary stream.
 *
 * \param stream  A pointer to the binary stream from which the paint will be read
 *
 * \return Returns a pointer to the paint if successful or NULL if there is
 * an error
 *
 * \see RpToonkPaintStreamWrite
 */
RpToonPaint *
RpToonPaintStreamRead(RwStream *stream)
{
    RwInt32 size;
    RwInt32 version;
    RpToonPaint *paint;

    RWAPIFUNCTION(RWSTRING("RpToonPaintStreamRead"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);
    RWASSERT( stream != 0);

    /* Read header */
    if (!RwStreamFindChunk(stream, rwID_STRUCT, (RwUInt32 *)&size, (RwUInt32 *)&version))
    {
        RWRETURN(NULL);
    }

    paint = RpToonPaintCreate();
    if (!paint)
    {
        RWRETURN(0);
    }

    /* internal paint version stamp */
    if (!RwStreamReadInt32(stream, &version, sizeof(RwInt32)))
    {
        RWRETURN(0);
    }

    /*
    If the stream format needs to change, we really must preserve
    backwards compatibility because this is the only way of saving/loading paint data;
    it's NOT exported.
    */
    if (version != rpToonPaintVersionStamp)
    {
        RWERROR((E_RW_BADVERSION, RWSTRING("Incompatible toon paint stream version")));
    }

    /* paint type */
    if (!RwStreamReadInt32(stream, (RwInt32*)&paint->type, sizeof(RwInt32)))
    {
        RWRETURN(0);
    }

    /* name */
    if (!RwStreamRead( stream, &paint->name, rwTEXTUREBASENAMELENGTH))
    {
        RWRETURN(0);
    }

    /* texture */
    if (paint->type == RPTOON_PAINTTYPE_TOONSHADE)
    {
        RwUInt32 version;

        if (!RwStreamFindChunk(stream, (RwUInt32)rwID_TEXTURE,
                               (RwUInt32 *)NULL, &version))
        {
            RWRETURN(0);
        }

        paint->gradient = RwTextureStreamRead(stream);
    }

    RWRETURN(paint);
}

/**
 * \ingroup rptoonpaint
 * \ref RpToonPaintStreamWrite is used to write the specified paint to the
 * given binary stream. The stream must have been opened prior to this
 * function call.
 *
 * \param paint  A pointer to the paint to be written
 * \param stream  A pointer to the binary stream
 *
 * \return Returns a pointer to the paint if successful, or NULL if there is
 * an error
 *
 * \see RpToonPaintStreamRead
 * \see RwStreamOpen
 * \see RwStreamClose
 *
 */
const RpToonPaint *
RpToonPaintStreamWrite(const RpToonPaint *paint, RwStream *stream)
{
    RwInt32 size;
    RwInt32 currentVersion = rpToonPaintVersionStamp;
    RwBool  writeTexture;
    RpToonPaintType writePaintType;

    RWAPIFUNCTION(RWSTRING("RpToonPaintStreamWrite"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);
    RWASSERT( paint != 0 );
    RWASSERT( stream != 0);

    size = sizeof(RwInt32)  /* paint version stamp */
         + sizeof(RwInt32)  /* paint type */
         + rwTEXTUREBASENAMELENGTH /* name */
         ;

    /* diffuse gradient texture, if we've got one */
    writePaintType = paint->type;
    writeTexture = (paint->type == RPTOON_PAINTTYPE_TOONSHADE) && paint->gradient;
    if (writeTexture)
    {
        size += RwTextureStreamGetSize(paint->gradient) + rwCHUNKHEADERSIZE;
    }
    else
    {
        /* type & texture settings don't match - don't write out inconsistent data */
        if (paint->type == RPTOON_PAINTTYPE_TOONSHADE)
        {
            writePaintType = RPTOON_PAINTTYPE_FLAT;
        }
    }

    if (!RwStreamWriteChunkHeader( stream, rwID_STRUCT, size ))
    {
        RWRETURN(0);
    }

    /* internal paint version stamp */
    if (!RwStreamWriteInt32(stream, &currentVersion, sizeof(RwInt32)))
    {
        RWRETURN(0);
    }

    /* paint type */
    if (!RwStreamWriteInt32(stream, (RwInt32*)&writePaintType, sizeof(RwInt32)))
    {
        RWRETURN(0);
    }

    /* name */
    if (!RwStreamWrite( stream, paint->name, rwTEXTUREBASENAMELENGTH))
    {
        RWRETURN(0);
    }

    /* texture */
    if (writeTexture)
    {
        if (!RwTextureStreamWrite(paint->gradient, stream))
        {
            RWRETURN(0);
        }
    }

    RWRETURN(paint);
}

/*---------------------------------------------------------------------------*
 *-                     RpToonPaintDictionary functions                     -*
 *---------------------------------------------------------------------------*/

/**
 * \ingroup rptoonpaintdictionary
 * \ref RpToonPaintDictionaryCreate is used to create a new empty \ref RpToonPaintDictionary.
 *
 * \return Returns a pointer to the newly created \ref RpToonPaintDictionary if successful or NULL
 * if there is an error
 *
 * \see RpToonPaintDictionaryDestroy
 *
 */
RpToonPaintDictionary *
RpToonPaintDictionaryCreate(void)
{
    RpToonPaintDictionary *id;

    RWAPIFUNCTION(RWSTRING("RpToonPaintDictionaryCreate"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);

    id = (RpToonPaintDictionary *)RwMalloc( sizeof(RpToonPaintDictionary),
                                    rwID_TOONPLUGIN | rwMEMHINTDUR_EVENT);

    RWASSERT(id&&"RwMalloc failed");

    if (id)
    {
        id->paintCount = 0;
        id->paints = 0;
    }

    RWRETURN(id);
}

/**
 * \ingroup rptoonpaintdictionary
 * \ref RpToonPaintDictionaryDestroy is used to destroy an \ref RpToonPaintDictionary.
 * \ref RpToonPaintDestroy is called on each paint in paintDictionary.
 *
 * \param paintDictionary a pointer to the \ref RpToonPaintDictionary to destroy.
 *
 * \return none.
 *
 * \see RpToonPaintDictionaryCreate
 * \see RpToonPaintDestroy
 */
void
RpToonPaintDictionaryDestroy(RpToonPaintDictionary *paintDictionary)
{
    RwInt32 i;

    RWAPIFUNCTION(RWSTRING("RpToonPaintDictionaryDestroy"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);

    RWASSERT(paintDictionary);

    /* decrement paint ref counts */
    for (i=0; i<paintDictionary->paintCount; i++)
    {
        RpToonPaintDestroy(paintDictionary->paints[i]);
    }

    /* free space allocated */
    if (paintDictionary->paints)
    {
        RwFree(paintDictionary->paints);
        paintDictionary->paints = 0;
    }

    paintDictionary->paintCount = 0;

    RwFree(paintDictionary);

    RWRETURNVOID();
}

/**
 * \ingroup rptoonpaintdictionary
 * \ref RpToonPaintDictionaryAddPaint is used to add a \ref RpToonPaint to a \ref RpToonPaintDictionary.
 * \ref RpToonPaintAddRef is called on the paint.
 *
 * \param paintDictionary a pointer to the \ref RpToonPaintDictionary.
 * \param paint a pointer to the \ref RpToonPaint to add.
 *
 * \return a pointer to the paintDictionary on success, NULL otherwise.
 *
 * \see RpToonPaintDictionaryRemovePaint
 * \see RpToonPaintAddRef
 */
RpToonPaintDictionary *
RpToonPaintDictionaryAddPaint(RpToonPaintDictionary *paintDictionary, RpToonPaint *paint )
{
    RpToonPaint **oldPaints;

    RWAPIFUNCTION(RWSTRING("RpToonPaintDictionaryAddPaint"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);
    RWASSERT(paintDictionary);
    RWASSERT(paint);

    oldPaints = paintDictionary->paints;

    /* resize space for paints */
    paintDictionary->paints = (RpToonPaint **)RwMalloc( sizeof(RpToonPaint *) * (paintDictionary->paintCount + 1),
                                                        rwID_TOONPLUGIN | rwMEMHINTDUR_EVENT);
    RWASSERT(paintDictionary->paints && "RwMalloc failed");

    if (!paintDictionary->paints)
    {
        RWRETURN(0);
    }

    /* preserve old ones */
    if (oldPaints)
    {
        memcpy(paintDictionary->paints, oldPaints, sizeof(RpToonPaint *) * paintDictionary->paintCount);
        RwFree(oldPaints);
    }

    /* and new one & bump ref count */
    paintDictionary->paints[paintDictionary->paintCount++] = paint;
    RpToonPaintAddRef(paint);

    RWRETURN(paintDictionary);
}

/**
 * \ingroup rptoonpaintdictionary
 * \ref RpToonPaintDictionaryRemovePaint is used to remove a \ref RpToonPaint from a \ref RpToonPaintDictionary.
 * \ref RpToonPaintDestroy is called on the paint to decrement its reference count.
 *
 * \param paintDictionary a pointer to the \ref RpToonPaintDictionary.
 * \param paint a pointer to the \ref RpToonPaint to remove.
 *
 * \return none.
 *
 * \see RpToonPaintDictionaryAddPaint
 * \see RpToonPaintAddRef
 */
void
RpToonPaintDictionaryRemovePaint(RpToonPaintDictionary *paintDictionary,
                                 RpToonPaint *paint )
{
    RpToonPaint **paintPtr;
    RpToonPaint  **removedPaintPtr = (RpToonPaint **)NULL;

    RWAPIFUNCTION(RWSTRING("RpToonPaintDictionaryRemovePaint"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);
    RWASSERT(paintDictionary);

    /* can't be in here */
    if (paintDictionary->paintCount == 0)
    {
        RWRETURNVOID();
    }

    /* find paint to remove */
    for (paintPtr = paintDictionary->paints; paintPtr != &paintDictionary->paints[paintDictionary->paintCount]; ++paintPtr )
    {
        if (*paintPtr == paint)
        {
            removedPaintPtr = paintPtr;
            paintPtr++;
            break;
        }
    }

    /* if we found it, dec ref count */
    if (removedPaintPtr != &paintDictionary->paints[paintDictionary->paintCount])
    {
        RpToonPaintDestroy(*removedPaintPtr);
    }

    /* collapse other paints to stomp over it's entry in dictionary */
    for (; paintPtr != &paintDictionary->paints[paintDictionary->paintCount]; )
    {
       *removedPaintPtr++ = *paintPtr++;
    }

    RWRETURNVOID();
}

/**
 * \ingroup rptoonpaintdictionary
 * \ref RpToonPaintDictionaryFindNamedPaint is used to find a \ref RpToonPaint with a
 * particular name in a \ref RpToonPaintDictionary.
 *
 * \param paintDictionary a pointer to the \ref RpToonPaintDictionary.
 * \param name the name to match.
 *
 * \return a pointer to the first \ref RpToonPaint with a name that matched, or NULL if there was no such paint.
 *
 * \see RpToonPaintDictionaryAddPaint
 * \see RpToonPaintDictionaryRemovePaint
 */
RpToonPaint *
RpToonPaintDictionaryFindNamedPaint(const RpToonPaintDictionary *paintDictionary,
                                    const RwChar *name )
{
    RwInt32 i;

    RWAPIFUNCTION(RWSTRING("RpToonPaintDictionaryFindNamedPaint"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);
    RWASSERT(paintDictionary);
    RWASSERT(name);

    for (i=0; i<paintDictionary->paintCount; i++)
    {
        if (strncmp(name, paintDictionary->paints[i]->name, rwTEXTUREBASENAMELENGTH - 1) == 0)
        {
            RWRETURN(paintDictionary->paints[i]);
        }
    }

    /* not found */
    RWRETURN(0);
}

/**
 * \ingroup rptoonpaintdictionary
 * \ref RpToonPaintDictionaryForAllPaints is used to iterate over all the \ref RpToonPaint
 * objects in a \ref RpToonPaintDictionary.
 *
 * \param paintDictionary a pointer to the \ref RpToonPaintDictionary.
 * \param paintCB callback function called with each paint
 * \param data pointer to arbitrary data for use by the callback function
 *
 * \return none.
 *
 * \see RpToonPaintDictionaryAddPaint
 * \see RpToonPaintDictionaryRemovePaint
 */
void
RpToonPaintDictionaryForAllPaints(const RpToonPaintDictionary *paintDictionary,
                                  RpToonPaintCallBack *paintCB,
                                  void *data)
{
    RwInt32 i;

    RWAPIFUNCTION(RWSTRING("RpToonPaintDictionaryForAllPaints"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);
    RWASSERT(paintDictionary);
    RWASSERT(paintCB);

    for (i=0; i<paintDictionary->paintCount; i++)
    {
        if (!(*paintCB)(paintDictionary->paints[i], data))
        {
            RWRETURNVOID();
        }
    }

    RWRETURNVOID();
}

/**
 * \ingroup rptoonpaintdictionary
 * \ref RpToonPaintDictionaryStreamRead is used to read an paint dictionary
 * and all the \ref RpToonPaint objects in containts from the specified
 * binary stream.
 *
 * \param stream  A pointer to the binary stream from which the paint dictionary will be read
 *
 * \return Returns a pointer to the \ref RpToonPaintDictionary if successful or NULL if there is
 * an error
 *
 * \see RpToonPaintDictionaryStreamWrite
 */
RpToonPaintDictionary *
RpToonPaintDictionaryStreamRead(RwStream *stream)
{
    RwInt32 paintCount, i;
    RwInt32 version, size;
    RpToonPaintDictionary *paintDict;

    RWAPIFUNCTION(RWSTRING("RpToonPaintDictionaryStreamRead"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);
    RWASSERT( stream != 0);

    /* Read header */
    if (!RwStreamFindChunk(stream, rwID_STRUCT, (RwUInt32 *)&size, (RwUInt32 *)&version))
    {
        RWRETURN(NULL);
    }

    /* internal paint dictionary version stamp */
    if (!RwStreamReadInt32(stream, &version, sizeof(RwInt32)))
    {
        RWRETURN(0);
    }

    /*
    If the stream format needs to change, we really must preserve
    backwards compatibility because this is the only way of saving/loading paint data;
    it's NOT exported.
    */
    if (version != rpToonPaintDictionaryVersionStamp)
    {
        RWERROR((E_RW_BADVERSION, RWSTRING("Incompatible toon paint dictionary stream version")));
    }

    if (!RwStreamReadInt32(stream, &paintCount, sizeof(RwInt32)))
    {
        RWRETURN(0);
    }

    RWASSERT(paintCount>=0);

    paintDict = RpToonPaintDictionaryCreate();

    for (i=0; i<paintCount; i++)
    {
        RpToonPaint *paint = RpToonPaintStreamRead(stream);

        if (!paint)
        {
            RWRETURN(0);
        }

        if (!RpToonPaintDictionaryAddPaint(paintDict, paint))
        {
            RWRETURN(0);
        }

        /* created with ref 1, then above added a ref, but we just want 1 so decrement it */
        RpToonPaintDestroy(paint);

        /* paint has reference after being read, which is okay since this dict refs it */
    }

    RWRETURN(paintDict);
}

/**
 * \ingroup rptoonpaintdictionary
 * \ref RpToonPaintDictionaryStreamWrite is used to write the specified paint dictionary
 * and all its \ref RpToonPaint objects to the
 * given binary stream. The stream must have been opened prior to this
 * function call.
 *
 * \param paintDictionary  A pointer to the paint dictionary to be written
 * \param stream  A pointer to the binary stream
 *
 * \return Returns a pointer to the paintDictionary if successful, or NULL if there is
 * an error
 *
 * \see RpToonPaintDictionaryStreamRead
 * \see RwStreamOpen
 * \see RwStreamClose
 *
 */
const RpToonPaintDictionary *
RpToonPaintDictionaryStreamWrite(const RpToonPaintDictionary *paintDictionary,
                                 RwStream *stream)
{
    RwInt32 size;
    RwInt32 currentVersion = rpToonPaintDictionaryVersionStamp;
    RwInt32 i;

    RWAPIFUNCTION(RWSTRING("RpToonPaintDictionaryStreamWrite"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);
    RWASSERT( paintDictionary != 0 );
    RWASSERT( stream != 0);

    size = sizeof(RwInt32)  /* paint dict version stamp */
         + sizeof(RwInt32)  /* number paints to follow */
         ;

    if (!RwStreamWriteChunkHeader( stream, rwID_STRUCT, size ))
    {
        RWRETURN(0);
    }

    /* internal paint dictionary version stamp */
    if (!RwStreamWriteInt32(stream, &currentVersion, sizeof(RwInt32)))
    {
        RWRETURN(0);
    }

    /* number paints to follow */
    if (!RwStreamWriteInt32(stream, &paintDictionary->paintCount, sizeof(RwInt32)))
    {
        RWRETURN(0);
    }

    for (i=0; i<paintDictionary->paintCount; i++)
    {
        if (!RpToonPaintStreamWrite( paintDictionary->paints[i], stream ))
        {
            RWRETURN(0);
        }
    }

    RWRETURN(paintDictionary);
}
