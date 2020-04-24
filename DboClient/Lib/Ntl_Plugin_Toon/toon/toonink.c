#include <string.h>

#include "rwcore.h"
#include "rpworld.h"

#include "rpplugin.h"
#include "rpdbgerr.h"

#include "toon.h"
#include "toongeo.h"
#include "toonink.h"

#define rpToonInkVersionStamp                   (0x12340002)
#define rpToonInkPerspectiveScaleVersionStamp   (0x12340002)
#define rpToonInkDictionaryVersionStamp         (0x56780001)


/**
 * \defgroup rptoonink RpToonInk
 * \ingroup rptoon
 */

/**
 * \defgroup rptooninkdictionary RpToonInkDictionary
 * \ingroup rptoon
 */

#if (!defined(DOXYGEN))
struct RpToonInkDictionary
{
    int inkCount;
    RpToonInk **inks;
};
#endif /* (!defined(DOXYGEN)) */

/*---------------------------------------------------------------------------*
 *-                           RpToonInk functions                           -*
 *---------------------------------------------------------------------------*/

/**
 * \ingroup rptoonink
 * \ref RpToonInkCreate is used to create a new \ref RpToonInk with default
 * values and a reference count of 1.
 *
 * \return Returns a pointer to the newly created \ref RpToonInk if
 * successful or NULL if there is an error
 *
 * \see RpToonInkAddRef
 * \see RpToonInkDestroy
 *
 */
RpToonInk *
RpToonInkCreate(void)
{
    RpToonInk *ink;

    RWAPIFUNCTION(RWSTRING("RpToonInkCreate"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);
    ink = (RpToonInk *)RwMalloc( sizeof(RpToonInk),
        rwID_TOONPLUGIN | rwMEMHINTDUR_EVENT);
    RWASSERT( (ink != 0) && "RwMalloc failed");
    ink->refCount = 1;
    ink->color.red = ink->color.blue = ink->color.green = 0;
    ink->color.alpha = 0xff;
    ink->overallThickness = 1.0f;
    ink->perspectiveScale = 1.0f;
    ink->farScale = 1.0f;
    rwstrcpy( ink->name, "default ink name" );
    RWRETURN(ink);
}

/**
 * \ingroup rptoonink
 * \ref RpToonInkDestroy is used to destroy an RpToonInk. It decrements the
 * ink's reference count, and if it's 0, it is destroyed.
 *
 * \param ink the \ref RpToonInk to destroy.
 *
 * \return none.
 *
 * \see RpToonInkAddRef
 * \see RpToonInkCreate
 */
void
RpToonInkDestroy(RpToonInk *ink)
{
    RWAPIFUNCTION(RWSTRING("RpToonInkDestroy"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);
    RWASSERT( ink != 0 );
    ink->refCount--;
    if (ink->refCount==0)
    {
        RwFree( (void *)ink );
    }
    RWRETURNVOID();
}

/**
 * \ingroup rptoonink
 * \ref RpToonInkAddRef is used to increment the reference count of an
 * \ref RpToonInk.
 *
 * \param ink the \ref RpToonInk to reference.
 *
 * \return none.
 *
 * \see RpToonInkDestroy
 * \see RpToonInkCreate
 */
void
RpToonInkAddRef(RpToonInk *ink)
{
    RWAPIFUNCTION(RWSTRING("RpToonInkAddRef"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);
    RWASSERT( ink != 0 );
    ink->refCount++;
    RWRETURNVOID();
}

/**
 * \ingroup rptoonink
 * \ref RpToonInkStreamRead is used to read an ink from the specified
 * binary stream.
 *
 * \param stream  A pointer to the binary stream from which the ink will be
 * read
 *
 * \return Returns a pointer to the ink if successful or NULL if there is
 * an error
 *
 * \see RpToonkInkStreamWrite
 */
RpToonInk *
RpToonInkStreamRead(RwStream *stream)
{
    RwInt32 size;
    RwInt32 version;
    RpToonInk *ink;

    RWAPIFUNCTION(RWSTRING("RpToonInkStreamRead"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);
    RWASSERT( stream != 0);

    /* Read header */
    if (!RwStreamFindChunk(stream, rwID_STRUCT, (RwUInt32 *)&size, (RwUInt32 *)&version))
    {
        RWRETURN(NULL);
    }

    ink = RpToonInkCreate();
    if (!ink)
    {
        RWRETURN(0);
    }

    /* internal ink version stamp */
    if (!RwStreamReadInt32(stream, &version, sizeof(RwInt32)))
    {
        RWRETURN(0);
    }

    /*
    If the stream format needs to change, we really must preserve
    backwards compatibility because this is the only way of saving/loading ink data;
    it's NOT exported.
    */
    if (version > rpToonInkVersionStamp)
    {
        RWERROR((E_RW_BADVERSION, RWSTRING("Incompatible toon ink stream version")));
    }

    /* name */
    if (!RwStreamRead( stream, &ink->name, rwTEXTUREBASENAMELENGTH))
    {
        RWRETURN(0);
    }

    /* thickness */
    if (!RwStreamReadReal(stream, &ink->overallThickness, sizeof(RwReal)))
    {
        RWRETURN(0);
    }

    /* color */
    if (!RwStreamRead(stream, &ink->color, sizeof(RwRGBA)))
    {
        RWRETURN(0);
    }

    if (version >= rpToonInkPerspectiveScaleVersionStamp)
    {
        /* perspective */
        if (!RwStreamReadReal(stream, &ink->perspectiveScale, sizeof(RwReal)))
        {
            RWRETURN(0);
        }

        /* far scale */
        if (!RwStreamReadReal(stream, &ink->farScale, sizeof(RwReal)))
        {
            RWRETURN(0);
        }
    }

    RWRETURN(ink);
}

/**
 * \ingroup rptoonink
 * \ref RpToonInkStreamWrite is used to write the specified ink to the
 * given binary stream. The stream must have been opened prior to this
 * function call.
 *
 * \param ink  A pointer to the ink to be written
 * \param stream  A pointer to the binary stream
 *
 * \return Returns a pointer to the ink if successful, or NULL if there is
 * an error
 *
 * \see RpToonInkStreamRead
 * \see RwStreamOpen
 * \see RwStreamClose
 *
 */
const RpToonInk *
RpToonInkStreamWrite(const RpToonInk *ink,
                     RwStream *stream)
{
    RwInt32 size;
    RwInt32 currentVersion = rpToonInkVersionStamp;

    RWAPIFUNCTION(RWSTRING("RpToonInkStreamWrite"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);
    RWASSERT( ink != 0 );
    RWASSERT( stream != 0);

    size = sizeof(RwInt32)  /* ink version stamp */
         + (rwTEXTUREBASENAMELENGTH) /* name */
         + sizeof(RwReal)   /* thickness */
         + sizeof(RwRGBA)   /* color */
         + sizeof(RwReal)   /* perspective */
         + sizeof(RwReal)   /* far scale */
         ;

    if (!RwStreamWriteChunkHeader( stream, rwID_STRUCT, size ))
    {
        RWRETURN(0);
    }

    /* internal ink version stamp */
    if (!RwStreamWriteInt32(stream, &currentVersion, sizeof(RwInt32)))
    {
        RWRETURN(0);
    }

    /* name */
    if (!RwStreamWrite( stream, ink->name, rwTEXTUREBASENAMELENGTH))
    {
        RWRETURN(0);
    }

    /* thickness */
    if (!RwStreamWriteReal(stream, &ink->overallThickness, sizeof(RwReal)))
    {
        RWRETURN(0);
    }

    /* color */
    if (!RwStreamWrite(stream, &ink->color, sizeof(RwRGBA)))
    {
        RWRETURN(0);
    }

    /* perspective */
    if (!RwStreamWriteReal(stream, &ink->perspectiveScale, sizeof(RwReal)))
    {
        RWRETURN(0);
    }

    /* far scale */
    if (!RwStreamWriteReal(stream, &ink->farScale, sizeof(RwReal)))
    {
        RWRETURN(0);
    }

    RWRETURN(ink);
}

/**
 * \ingroup rptoonink
 * \ref RpToonInkGetOverallThickness is used to get an ink's thickness.
 *
 * \param ink  A pointer to the ink
 *
 * \return an RwReal representing the base thickness.
 *
 * \see RpToonInkSetOverallThickness
 * \see RpToonInkSetPerspectiveScale
 * \see RpToonInkSetFarScale
 */
RwReal
RpToonInkGetOverallThickness(const RpToonInk *ink)
{
    RWAPIFUNCTION(RWSTRING("RpToonInkGetOverallThickness"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);
    RWASSERT( ink != 0 );
    RWRETURN(ink->overallThickness);
}

/**
 * \ingroup rptoonink
 * \ref RpToonInkSetOverallThickness is used to set an ink's thickness.
 * Factors in the ink and toon geometry will be multiplied against this value
 * when rendering to give the final thickness of a particular edge.
 *
 * \param ink  A pointer to the ink
 * \param thickness an RwReal representing the base thickness.
 *
 * \return none.
 *
 * \see RpToonInkGetOverallThickness
 * \see RpToonInkGetPerspectiveScale
 * \see RpToonInkGetFarScale
 */
void
RpToonInkSetOverallThickness(RpToonInk *ink,
                             RwReal thickness)
{
    RWAPIFUNCTION(RWSTRING("RpToonInkSetOverallThickness"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);
    RWASSERT( ink != 0 );
    ink->overallThickness = thickness;
    RWRETURNVOID();
}

/**
 * \ingroup rptoonink
 * \ref RpToonInkGetColor is used to get an ink's color.
 *
 * \param ink  A pointer to the ink.
 *
 * \return An \ref RwRGBA representing the color.
 *
 * \see RpToonInkGetColor
 */
RwRGBA
RpToonInkGetColor(const RpToonInk *ink)
{
    RWAPIFUNCTION(RWSTRING("RpToonInkGetColor"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);
    RWASSERT( ink != 0 );
    RWRETURN(ink->color );
}

/**
 * \ingroup rptoonink
 * \ref RpToonInkSetColor is used to set an ink's color.
 *
 * \param ink  A pointer to the ink.
 * \param color  An \ref RwRGBA representing the new color.
 *
 * \return none.
 *
 * \see RpToonInkGetColor
 */
void
RpToonInkSetColor(RpToonInk *ink,
                  RwRGBA color)
{
    RWAPIFUNCTION(RWSTRING("RpToonInkSetColor"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);
    RWASSERT( ink != 0 );
    ink->color = color;
    RWRETURNVOID();
}

/**
 * \ingroup rptoonink
 * \ref RpToonInkGetName is used to get an ink's name.
 * Ink names are used as keys in \ref RpToonInkDictionary objects.
 *
 * \param ink  A pointer to the ink.
 *
 * \return the inks name.
 *
 * \see RpToonInkSetName
 */
const RwChar *
RpToonInkGetName(const RpToonInk *ink)
{
    RWAPIFUNCTION(RWSTRING("RpToonInkGetName"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);
    RWASSERT( ink != 0 );
    RWRETURN( ink->name );
}

/**
 * \ingroup rptoonink
 * \ref RpToonInkSetName is used to set an ink's name.
 * Ink names are used as keys in \ref RpToonInkDictionary objects.
 *
 * \param ink  A pointer to the ink.
 * \param name  A string specifying the new name.
 *
 * \return none.
 *
 * \see RpToonInkGetName
 */
void
RpToonInkSetName(RpToonInk *ink,
                 const RwChar *name)
{
    RWAPIFUNCTION(RWSTRING("RpToonInkSetName"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);

    RWASSERT( ink != 0 );

    rwstrncpy(ink->name, name, rwTEXTUREBASENAMELENGTH - 1);

    RWRETURNVOID();
}

/**
 * \ingroup rptoonink
 * \ref RpToonInkGetPerspectiveScale is used to get an ink's perspective scale.
 *
 * \param ink  A pointer to the ink
 *
 * \return an RwReal representing the perspective scale thickness.
 *
 * \see RpToonInkSetOverallThickness
 * \see RpToonInkSetPerspectiveScale
 * \see RpToonInkSetFarScale
 */
RwReal
RpToonInkGetPerspectiveScale(const RpToonInk *ink)
{
    RWAPIFUNCTION(RWSTRING("RpToonInkGetPerspectiveScale"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);
    RWASSERT( ink != 0 );
    RWRETURN( ink->perspectiveScale );
}

/**
 * \ingroup rptoonink
 * \ref RpToonInkSetPerspectiveScale is used to affect an ink's thickness
 * with distance to the camera per vertex.  Lines near the camera will have
 * their thicknesses multiplied by perspectiveScale.  This will fall off to
 * a multiplier of 1 (no effect) on lines far from the camera.  For RpAtomics,
 * these boundaries are at either extreme of the atomic's bound sphere.
 * For world sectors, these boundaries are just the camera's clip planes.
 * This is especially useful for landscapes or "forced perpective" on close
 * ups of characters.
 *
 * \param ink  A pointer to the ink
 * \param perspectiveScale an RwReal representing the perspective scale
 *        thickness.
 *
 * \return none.
 *
 * \see RpToonInkGetOverallThickness
 * \see RpToonInkGetPerspectiveScale
 * \see RpToonInkGetFarScale
 */
void
RpToonInkSetPerspectiveScale(RpToonInk *ink,
                             RwReal perspectiveScale)
{
    RWAPIFUNCTION(RWSTRING("RpToonInkSetPerspectiveScale"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);
    RWASSERT( ink != 0 );
    RWASSERT( perspectiveScale >= 0.0f );
    ink->perspectiveScale = perspectiveScale;
    RWRETURNVOID();
}

/**
 * \ingroup rptoonink
 * \ref RpToonInkGetFarScale is used to get an ink's far scale thickness.
 *
 * \param ink  A pointer to the ink
 *
 * \return an RwReal representing the far scale thickness.
 *
 * \see RpToonInkSetOverallThickness
 * \see RpToonInkSetPerspectiveScale
 * \see RpToonInkSetFarScale
 */
RwReal
RpToonInkGetFarScale(const RpToonInk *ink)
{
    RWAPIFUNCTION(RWSTRING("RpToonInkGetFarScale"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);
    RWASSERT( ink != 0 );
    RWRETURN( ink->farScale );
}

/**
 * \ingroup rptoonink
 * \ref RpToonInkSetFarScale is used to set an ink's thickness with an
 * RpAtomics distance to the camera. When the object is at the far clip
 * plane, the ink thickness is multiplied by this factor, and this falls off
 * to a multiplier of 1 (no effect) as the object approaches the near clip
 * plane. This is useful to prevent the lines around an object from growing
 * bigger than the object itself as it moves away from the camera, which
 * generally looks horrible. Toon rendered world sectors ignore this
 * attribute.
 *
 * \param ink  A pointer to the ink
 * \param farScale an RwReal representing the far scale thickness.
 *
 * \return none.
 *
 * \see RpToonInkGetOverallThickness
 * \see RpToonInkGetPerspectiveScale
 * \see RpToonInkGetFarScale
 */
void
RpToonInkSetFarScale(RpToonInk *ink,
                     RwReal farScale)
{
    RWAPIFUNCTION(RWSTRING("RpToonInkSetFarScale"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);
    RWASSERT(ink != 0);
    RWASSERT((farScale >= 0.0f) && (farScale <= 1.0f));

    ink->farScale = farScale;

    RWRETURNVOID();
}

/*---------------------------------------------------------------------------*
 *-                      RpToonInkDictionary functions                      -*
 *---------------------------------------------------------------------------*/

/**
 * \ingroup rptooninkdictionary
 * \ref RpToonInkDictionaryCreate is used to create a new empty
 * \ref RpToonInkDictionary.
 *
 * \return Returns a pointer to the newly created \ref RpToonInkDictionary if
 * successful or NULL if there is an error
 *
 * \see RpToonInkDictionaryDestroy
 */
RpToonInkDictionary *
RpToonInkDictionaryCreate(void)
{
    RpToonInkDictionary *id;

    RWAPIFUNCTION(RWSTRING("RpToonInkDictionaryCreate"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);

    id = (RpToonInkDictionary *)RwMalloc( sizeof(RpToonInkDictionary),
                                rwID_TOONPLUGIN | rwMEMHINTDUR_EVENT);

    RWASSERT(id&&"RwMalloc failed");

    if (id)
    {
        id->inkCount = 0;
        id->inks = 0;
    }

    RWRETURN(id);
}

/**
 * \ingroup rptooninkdictionary
 * \ref RpToonInkDictionaryDestroy is used to destroy an
 * \ref RpToonInkDictionary. \ref RpToonInkDestroy is called on each ink in
 * the inkDictionary.
 *
 * \param inkDictionary a pointer to the \ref RpToonInkDictionary to destroy.
 *
 * \return none.
 *
 * \see RpToonInkDictionaryCreate
 * \see RpToonInkDestroy
 */
void
RpToonInkDictionaryDestroy(RpToonInkDictionary *inkDictionary)
{
    RwInt32 i;

    RWAPIFUNCTION(RWSTRING("RpToonInkDictionaryDestroy"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);

    RWASSERT(inkDictionary);

    /* decrement ink ref counts */
    for (i=0; i<inkDictionary->inkCount; i++)
    {
        RpToonInkDestroy(inkDictionary->inks[i]);
    }

    /* free space allocated */
    if (inkDictionary->inks)
    {
        RwFree(inkDictionary->inks);
        inkDictionary->inks = 0;
    }

    inkDictionary->inkCount = 0;

    RwFree(inkDictionary);

    RWRETURNVOID();
}

/**
 * \ingroup rptooninkdictionary
 * \ref RpToonInkDictionaryAddInk is used to add a \ref RpToonInk to a
 * \ref RpToonInkDictionary. \ref RpToonInkAddRef is called on the ink.
 *
 * \param inkDictionary a pointer to the \ref RpToonInkDictionary.
 * \param ink a pointer to the \ref RpToonInk to add.
 * \return a pointer to the inkDictionary on success, NULL otherwise.
 *
 * \see RpToonInkDictionaryRemoveInk
 * \see RpToonInkAddRef
 */
RpToonInkDictionary *
RpToonInkDictionaryAddInk(RpToonInkDictionary *inkDictionary,
                          RpToonInk *ink)
{
    RpToonInk **oldInks;

    RWAPIFUNCTION(RWSTRING("RpToonInkDictionaryAddInk"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);
    RWASSERT(inkDictionary);
    RWASSERT(ink);

    oldInks = inkDictionary->inks;

    /* resize space for inks */
    inkDictionary->inks = (RpToonInk **)
        RwMalloc(sizeof(RpToonInk *) * (inkDictionary->inkCount + 1),
                 rwID_TOONPLUGIN | rwMEMHINTDUR_EVENT);
    RWASSERT(inkDictionary->inks && "RwMalloc failed");

    if (!inkDictionary->inks)
    {
        RWRETURN(0);
    }

    /* preserve old ones */
    if (oldInks)
    {
        memcpy(
            inkDictionary->inks,
            oldInks,
            sizeof(RpToonInk *) * inkDictionary->inkCount);
        RwFree(oldInks);
    }

    /* and new one & bump ref count */
    inkDictionary->inks[inkDictionary->inkCount++] = ink;
    RpToonInkAddRef(ink);

    RWRETURN(inkDictionary);
}

/**
 * \ingroup rptooninkdictionary
 * \ref RpToonInkDictionaryRemoveInk is used to remove a \ref RpToonInk from
 * a \ref RpToonInkDictionary. \ref RpToonInkDestroy is called on the ink to
 * decrement its reference count.
 *
 * \param inkDictionary a pointer to the \ref RpToonInkDictionary.
 * \param ink a pointer to the \ref RpToonInk to remove.
 *
 * \return none.
 *
 * \see RpToonInkDictionaryAddInk
 * \see RpToonInkAddRef
 */
void
RpToonInkDictionaryRemoveInk(RpToonInkDictionary *inkDictionary,
                             RpToonInk *ink)
{
    RpToonInk **inkPtr;
    RpToonInk **removedInkPtr = (RpToonInk **)NULL;

    RWAPIFUNCTION(RWSTRING("RpToonInkDictionaryRemoveInk"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);
    RWASSERT(inkDictionary);

    /* can't be in here */
    if (inkDictionary->inkCount == 0)
    {
        RWRETURNVOID();
    }

    /* find ink to remove */
    for (inkPtr = inkDictionary->inks;
         inkPtr != &inkDictionary->inks[inkDictionary->inkCount];
         ++inkPtr)
    {
        if (*inkPtr == ink)
        {
            removedInkPtr = inkPtr;
            inkPtr++;
            break;
        }
    }

    /* if we found it, dec ref count */
    if (removedInkPtr != &inkDictionary->inks[inkDictionary->inkCount])
    {
        RpToonInkDestroy(*removedInkPtr);
    }

    /* collapse other inks to stomp over it's entry in dictionary */
    for (; inkPtr != &inkDictionary->inks[inkDictionary->inkCount]; )
    {
       *removedInkPtr++ = *inkPtr++;
    }

    RWRETURNVOID();
}

/**
 * \ingroup rptooninkdictionary
 * \ref RpToonInkDictionaryFindNamedInk is used to find a \ref RpToonInk with
 * a particular name in a \ref RpToonInkDictionary.
 *
 * \param inkDictionary a pointer to the \ref RpToonInkDictionary.
 * \param name the name to match.
 * \return a pointer to the first \ref RpToonInk with a name that matched, or
 * NULL if there was no such ink.
 *
 * \see RpToonInkDictionaryAddInk
 * \see RpToonInkDictionaryRemoveInk
 */
RpToonInk *
RpToonInkDictionaryFindNamedInk(const RpToonInkDictionary *inkDictionary,
                                const RwChar *name)
{
    RwInt32 i;

    RWAPIFUNCTION(RWSTRING("RpToonInkDictionaryFindNamedInk"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);
    RWASSERT(inkDictionary);
    RWASSERT(name);

    for (i=0; i<inkDictionary->inkCount; i++)
    {
        if (strncmp(name, inkDictionary->inks[i]->name, rwTEXTUREBASENAMELENGTH - 1) == 0)
        {
            RWRETURN(inkDictionary->inks[i]);
        }
    }

    /* not found */
    RWRETURN(0);
}

/**
 * \ingroup rptooninkdictionary
 * \ref RpToonInkDictionaryForAllInks is used to iterate over all the
 * \ref RpToonInk objects in a \ref RpToonInkDictionary.
 *
 * \param inkDictionary a pointer to the \ref RpToonInkDictionary.
 * \param inkCB callback function called with each ink
 * \param data pointer to arbitrary data for use by the callback function
 *
 * \return none.
 *
 * \see RpToonInkDictionaryAddInk
 * \see RpToonInkDictionaryRemoveInk
 */
void
RpToonInkDictionaryForAllInks(const RpToonInkDictionary *inkDictionary,
                              RpToonInkCallBack *inkCB,
                              void *data)
{
    RwInt32 i;

    RWAPIFUNCTION(RWSTRING("RpToonInkDictionaryForAllInks"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);
    RWASSERT(inkDictionary);
    RWASSERT(inkCB);

    for (i=0; i<inkDictionary->inkCount; i++)
    {
        if (!(*inkCB)(inkDictionary->inks[i], data))
        {
            RWRETURNVOID();
        }
    }

    RWRETURNVOID();
}

/**
 * \ingroup rptooninkdictionary
 * \ref RpToonInkDictionaryStreamRead is used to read an ink dictionary
 * and all the \ref RpToonInk objects in containts from the specified
 * binary stream.
 *
 * \param stream  A pointer to the binary stream from which the ink dictionary
 * will be read
 *
 * \return Returns a pointer to the \ref RpToonInkDictionary if successful or
 * NULL if there is an error
 *
 * \see RpToonInkDictionaryStreamWrite
 */
RpToonInkDictionary *
RpToonInkDictionaryStreamRead(RwStream *stream)
{
    RwInt32 inkCount, i;
    RwInt32 version, size;
    RpToonInkDictionary *inkDict;

    RWAPIFUNCTION(RWSTRING("RpToonInkDictionaryStreamRead"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);
    RWASSERT( stream != 0);

    /* Read header */
    if (!RwStreamFindChunk(stream, rwID_STRUCT, (RwUInt32 *)&size, (RwUInt32 *)&version))
    {
        RWRETURN(NULL);
    }

    /* internal ink dictionary version stamp */
    if (!RwStreamReadInt32(stream, &version, sizeof(RwInt32)))
    {
        RWRETURN(0);
    }

    /*
    If the stream format needs to change, we really must preserve
    backwards compatibility because this is the only way of saving/loading ink data;
    it's NOT exported.
    */
    if (version != rpToonInkDictionaryVersionStamp)
    {
        RWERROR((E_RW_BADVERSION, RWSTRING("Incompatible toon ink dictionary stream version")));
    }

    if (!RwStreamReadInt32(stream, &inkCount, sizeof(RwInt32)))
    {
        RWRETURN(0);
    }

    RWASSERT(inkCount>=0);

    inkDict = RpToonInkDictionaryCreate();

    for (i=0; i<inkCount; i++)
    {
        RpToonInk *ink = RpToonInkStreamRead(stream);

        if (!ink)
        {
            RWRETURN(0);
        }

        if (!RpToonInkDictionaryAddInk(inkDict, ink))
        {
            RWRETURN(0);
        }

        /* created with ref 1, then above added a ref, but we just want 1 so decrement it */
        RpToonInkDestroy(ink);

        /* ink has reference after being read, which is okay since this dict refs it */
    }

    RWRETURN(inkDict);
}

/**
 * \ingroup rptooninkdictionary
 * \ref RpToonInkDictionaryStreamWrite is used to write
 * the specified ink dictionary and all its \ref RpToonInk objects to the
 * given binary stream. The stream must have been opened prior to this
 * function call.
 *
 * \param inkDictionary  A pointer to the ink dictionary to be written
 * \param stream  A pointer to the binary stream
 *
 * \return Returns a pointer to the inkDictionary if successful, or NULL if
 * there is an error
 *
 * \see RpToonInkDictionaryStreamRead
 * \see RwStreamOpen
 * \see RwStreamClose
 *
 */
const RpToonInkDictionary *
RpToonInkDictionaryStreamWrite(const RpToonInkDictionary *inkDictionary,
                               RwStream *stream)
{
    RwInt32 size;
    RwInt32 currentVersion = rpToonInkDictionaryVersionStamp;
    RwInt32 i;

    RWAPIFUNCTION(RWSTRING("RpToonInkDictionaryStreamWrite"));
    RWASSERT(0 < _rpToonGlobals.module.numInstances);
    RWASSERT( inkDictionary != 0 );
    RWASSERT( stream != 0);

    size = sizeof(RwInt32)  /* ink dict version stamp */
         + sizeof(RwInt32)  /* number inks to follow */
         ;

    if (!RwStreamWriteChunkHeader( stream, rwID_STRUCT, size ))
    {
        RWRETURN(0);
    }

    /* internal ink dictionary version stamp */
    if (!RwStreamWriteInt32(stream, &currentVersion, sizeof(RwInt32)))
    {
        RWRETURN(0);
    }

    /* number inks to follow */
    if (!RwStreamWriteInt32(stream, &inkDictionary->inkCount, sizeof(RwInt32)))
    {
        RWRETURN(0);
    }

    for (i=0; i<inkDictionary->inkCount; i++)
    {
        if (!RpToonInkStreamWrite( inkDictionary->inks[i], stream ))
        {
            RWRETURN(0);
        }
    }

    RWRETURN((const RpToonInkDictionary*)inkDictionary);
}
