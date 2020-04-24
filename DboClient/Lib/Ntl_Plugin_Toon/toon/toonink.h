/*****************************************************************************

    File: toonink.c

    Purpose: A short description of the file.

    Copyright (c) 2002 Criterion Software Ltd.

 */

#ifndef TOONINK_H
#define TOONINK_H

/*****************************************************************************
 Includes
 */
#include "rwcore.h"
#include "rpworld.h"

/*****************************************************************************
 Defines
 */

/*****************************************************************************
 Enums
 */

/*****************************************************************************
 Typedef Enums
 */

/*****************************************************************************
 Typedef Structs
 */
/* RWPUBLIC */
/**
 * \ingroup rptoon
 * \struct RpToonInk
 *
 * Attributes describing how to draw the edges of toon objects are stored here.
 * This should be considered an opaque type.
 * Use the RpToon API functions to access.
 *
 */
typedef struct RpToonInk RpToonInk;

/**
 * \ingroup rptoon
 * \struct RpToonInkDictionary
 *
 * Collections of RpToonInks can be stored here and searched
 * for by name.  Behavior and functionality is similar to that
 * of \ref RwTexDictionary.
 * This should be considered an opaque type.
 * Use the RpToon API functions to access.
 *
 */
typedef struct RpToonInkDictionary RpToonInkDictionary;

typedef struct rpToonInkID      rpToonInkID;
/* RWPUBLICEND */

/* RWPUBLIC */
/*****************************************************************************
 Function Pointers
 */
/**
 * \ingroup rptooninkdictionary
 * \ref RpToonInkCallBack
 * represents the function called from \ref RpToonInkDictionaryForAllInks
 * for all inks in a given paint dictionary. This function should
 * return the current ink to indicate success. The callback may return
 * NULL to terminate further callbacks on the ink dictionary.
 *
 * \param  ink   Pointer to the current ink.
 *
 * \param  data   User-defined data pointer.
 *
 * \return Pointer to the current ink or NULL
 *
 * \see RpToonInkDictionaryForAllInks
 */
typedef RpToonInk * (RpToonInkCallBack)(RpToonInk *ink, void *data);

/*****************************************************************************
 Structs
 */
#if (!defined(DOXYGEN))
struct rpToonInkID
{
    RwChar  name[rwTEXTUREBASENAMELENGTH];
};

struct RpToonInk /* groups of edges point to one - per material? per atomic? */
{
    RwInt32 refCount;

    /* ---------- persistent stuff that's streamed -------------------------------- */

    RwChar name[rwTEXTUREBASENAMELENGTH];

    RwReal overallThickness;        /**/
    RwRGBA color;

    RwReal perspectiveScale;        /* vertices at front of object are scaled by perspectiveScale, back by 1 */

    RwReal farScale;                /* as bound sphere center approaches far clip, it's scaled by farScale */

    /*
    Other interesting features might be textures, exterior color,
    blending modes, stroke based properties... 
    */

    /* ---------- temporaries used during the rendering process ------------------- */

    /* stores factored coefficients for perspective scale effect */
    RwReal perspectiveScale_a, perspectiveScale_b, perspectiveScale_zMin;

    RwReal farScale_factor;
    RwReal farScale_creasefactor;
};
#endif /* (!defined(DOXYGEN)) */
/* RWPUBLICEND */

/* RWPUBLIC */
#ifdef     __cplusplus
extern "C"
{
#endif  /* __cplusplus */
/* RWPUBLICEND */

/*****************************************************************************
 Global Variables
 */

/* RWPUBLIC */
/*****************************************************************************
 Function prototypes
 */
/* INK */
/* Create & Destroy */
extern RpToonInk *RpToonInkCreate(void);
extern void RpToonInkDestroy(RpToonInk *ink);

/* Add ref */
extern void RpToonInkAddRef(RpToonInk *ink);

/* Read & Write */
extern RpToonInk *RpToonInkStreamRead(RwStream *stream);
extern const RpToonInk *RpToonInkStreamWrite(const RpToonInk *ink,
                                             RwStream *stream);

/* Get & Set Thickness */
extern RwReal RpToonInkGetOverallThickness(const RpToonInk *ink);
extern void RpToonInkSetOverallThickness(RpToonInk *ink,
                                         RwReal thickness);

/* Get & Set Color */
extern RwRGBA RpToonInkGetColor(const RpToonInk *ink);
extern void RpToonInkSetColor(RpToonInk *ink, RwRGBA color);

/* Get & Set Name */
extern const RwChar *RpToonInkGetName(const RpToonInk *ink);
extern void RpToonInkSetName(RpToonInk *ink, const RwChar *name);

/* Get & Set Perspective Scale */
extern RwReal RpToonInkGetPerspectiveScale(const RpToonInk *ink);
extern void RpToonInkSetPerspectiveScale(RpToonInk *ink,
                                         RwReal perspectiveScale);

/* Get & Set Far Scale */
extern RwReal RpToonInkGetFarScale(const RpToonInk *ink);
extern void RpToonInkSetFarScale(RpToonInk *ink, RwReal farScale);

/* INK DICTIONARY */
/* Create & Destroy */
extern RpToonInkDictionary *RpToonInkDictionaryCreate(void);
extern void RpToonInkDictionaryDestroy(RpToonInkDictionary *inkDictionary);

/* Add & Remove Ink */
extern RpToonInkDictionary *
RpToonInkDictionaryAddInk(RpToonInkDictionary *inkDictionary,
                          RpToonInk *ink);
extern void
RpToonInkDictionaryRemoveInk(RpToonInkDictionary *inkDictionary,
                             RpToonInk *ink);

/* Find Named */
extern RpToonInk *
RpToonInkDictionaryFindNamedInk(const RpToonInkDictionary *inkDictionary,
                                const RwChar *name);

/* For all Inks */
extern void
RpToonInkDictionaryForAllInks(const RpToonInkDictionary *inkDictionary,
                              RpToonInkCallBack *inkCB,
                              void *data);

/* Read & Write */
extern RpToonInkDictionary *
RpToonInkDictionaryStreamRead(RwStream *stream);
extern const RpToonInkDictionary *
RpToonInkDictionaryStreamWrite(const RpToonInkDictionary *inkDictionary,
                               RwStream *stream);

/* RWPUBLICEND */

/* RWPUBLIC */
#ifdef    __cplusplus
}
#endif /* __cplusplus */
/* RWPUBLICEND */

#endif /* TOONINK_H */
