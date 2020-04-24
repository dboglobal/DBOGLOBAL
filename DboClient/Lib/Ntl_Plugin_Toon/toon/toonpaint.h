/*****************************************************************************

    File: toonpaint.c

    Purpose: A short description of the file.

    Copyright (c) 2002 Criterion Software Ltd.

 */

#ifndef TOONPAINT_H
#define TOONPAINT_H

/*****************************************************************************
 Includes
 */
#include "rwcore.h"

/*****************************************************************************
 Defines
 */

/* RWPUBLIC */
/*****************************************************************************
 Enums
 */
/**
 * \ingroup rptoonpaint
 * \ref RpToonPaintType defines the different ways a toon paint can
 * be rendered. Once a paint has been created, it's type can be changed
 * with \ref RpToonPaintSetType and queried with \ref RpToonPaintGetType.
 */
enum RpToonPaintType
{
    RPTOON_PAINTTYPE_FLAT = 0,      /**<Flat shading, material and texture color is fully bright */
    RPTOON_PAINTTYPE_TOONSHADE,     /**<Toon shading, material and texture color blended against shading texture */
    RPTOON_PAINTTYPE_COUNT,         /**<Count of paint types */
    RPTOON_PAINTTYPE_FORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};

/*****************************************************************************
 Typedef Enums
 */
typedef enum RpToonPaintType    RpToonPaintType;

/*****************************************************************************
 Typedef Structs
 */
typedef struct rpToonPaintID    rpToonPaintID;

/**
 * \ingroup rptoon
 * \struct RpToonPaint
 *
 * Attributes describing how to draw the fill/shading
 * of toon objects are stored here.
 * This should be considered an opaque type.
 * Use the RpToon API functions to access.
 *
 */
typedef struct RpToonPaint RpToonPaint;

/**
 * \ingroup rptoon
 * \struct RpToonPaintDictionary
 *
 * Collections of RpToonPaints can be stored here and searched
 * for by name.  Behavior and functionality is similar to that
 * of \ref RwTexDictionary.
 * This should be considered an opaque type.
 * Use the RpToon API functions to access.
 *
 */
typedef struct RpToonPaintDictionary RpToonPaintDictionary;

/*****************************************************************************
 Function Pointers
 */
/**
 * \ingroup rptoonpaintdictionary
 * \ref RpToonPaintCallBack
 * represents the function called from \ref RpToonPaintDictionaryForAllPaints
 * for all paints in a given paint dictionary. This function should
 * return the current paint to indicate success. The callback may return
 * NULL to terminate further callbacks on the paint dictionary.
 *
 * \param  paint   Pointer to the current texture.
 *
 * \param  data   User-defined data pointer.
 *
 * \return Pointer to the current paint or NULL
 *
 * \see RpToonPaintDictionaryForAllPaints
 */
typedef RpToonPaint * (RpToonPaintCallBack)(RpToonPaint *paint, void *data);

/*****************************************************************************
 Structs
 */
#if (!defined(DOXYGEN))
struct rpToonPaintID
{
    RwChar  name[rwTEXTUREBASENAMELENGTH];
};

struct RpToonPaint /* groups of triangles point to one. Store per material? per atomic? per light??? */
{
    RwInt32 refCount;

    RpToonPaintType type;

    RwChar name[rwTEXTUREBASENAMELENGTH];

    /* texture gradient - diffuse */
    RwTexture *gradient;

    /*
    Other interesting features might be specular reflection, god knows what...
    keep an open mind.
    */
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
/* PAINT */
/* Create & Destroy */
extern RpToonPaint * RpToonPaintCreate(void);
extern void RpToonPaintDestroy(RpToonPaint *paint);

/* Add ref */
extern void RpToonPaintAddRef(RpToonPaint *paint);

/* Get & Set Type */
extern RpToonPaintType RpToonPaintGetType(const RpToonPaint *paint);
extern void RpToonPaintSetType(RpToonPaint *paint, RpToonPaintType type);

/* Get & Set Name */
extern const RwChar *RpToonPaintGetName(const RpToonPaint *paint);
extern void RpToonPaintSetName(RpToonPaint *paint, const RwChar *name);

/* Get & Set Gradient Texture */
extern RwTexture *RpToonPaintGetGradientTexture(const RpToonPaint *paint);
extern void RpToonPaintSetGradientTexture(RpToonPaint *paint, RwTexture *texture);

/* Read & Write */
extern RpToonPaint *RpToonPaintStreamRead(RwStream *stream);
extern const RpToonPaint *RpToonPaintStreamWrite(const RpToonPaint *paint, RwStream *stream);


/* PAINT DICTIONARY */
/* Create & Destroy */
extern RpToonPaintDictionary *RpToonPaintDictionaryCreate(void);
extern void RpToonPaintDictionaryDestroy(RpToonPaintDictionary *paintDictionary);

/* Add & Remove Paint */
extern RpToonPaintDictionary *
RpToonPaintDictionaryAddPaint(RpToonPaintDictionary *paintDictionary,
                              RpToonPaint *paint);
extern void
RpToonPaintDictionaryRemovePaint(RpToonPaintDictionary *paintDictionary,
                                 RpToonPaint *paint);

/* Find named */
extern RpToonPaint *
RpToonPaintDictionaryFindNamedPaint(const RpToonPaintDictionary *paintDictionary,
                                    const RwChar *name);

/* For all Paints */
extern void
RpToonPaintDictionaryForAllPaints(const RpToonPaintDictionary *paintDictionary,
                                  RpToonPaintCallBack *paintCB,
                                  void *data);

/* Read & Write */
extern RpToonPaintDictionary *
RpToonPaintDictionaryStreamRead(RwStream *stream);
extern const RpToonPaintDictionary *
RpToonPaintDictionaryStreamWrite(const RpToonPaintDictionary *paintDictionary,
                                 RwStream *stream);
/* RWPUBLICEND */

/* RWPUBLIC */
#ifdef    __cplusplus
}
#endif /* __cplusplus */
/* RWPUBLICEND */

#endif /* TOONPAINT_H */
