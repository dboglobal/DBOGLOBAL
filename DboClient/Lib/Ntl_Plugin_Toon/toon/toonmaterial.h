/*****************************************************************************

    File: toonmaterial.c

    Purpose: A short description of the file.

    Copyright (c) 2002 Criterion Software Ltd.

 */

#ifndef TOONMATERIAL_H
#define TOONMATERIAL_H

/*****************************************************************************
 Includes
 */
#include "rwcore.h"
#include "rpworld.h"

#include "toon.h"
#include "toonpaint.h"

/* RWPUBLIC */
/*****************************************************************************
 Defines
 */
#define RPTOONMATERIALGETDATA(materialInstance)         \
     ((RpToonMaterial **)                               \
      (((RwUInt8 *)materialInstance) +                  \
       _rpToonGlobals.materialOffset))

#define RPTOONMATERIALGETCONSTDATA(materialInstance)    \
     ((const RpToonMaterial * const *)                  \
      (((const RwUInt8 *)materialInstance) +            \
       _rpToonGlobals.materialOffset))

/*****************************************************************************
 Enums
 */

/*****************************************************************************
 Typedef Enums
 */

/*****************************************************************************
 Typedef Structs
 */
/**
 * \ingroup rptoonmaterial
 * \struct RpToonMaterial
 *
 * RpToon keeps a \ref RpToonPaint name here to identify paints associated with a material
 * for streaming.
 * This should be considered an opaque type.
 * Use the RpToon API functions to access.
 *
 */
typedef struct RpToonMaterial RpToonMaterial;

/*****************************************************************************
 Function Pointers
 */

/*****************************************************************************
 Structs
 */
#if (!defined(DOXYGEN))
struct RpToonMaterial
{
    RwBool      overrideGeometryPaint;
    RwChar      paintName[rwTEXTUREBASENAMELENGTH];
    RpToonPaint *paint;
    RwInt32     silhouetteInkID;
};
#endif /* (!defined(DOXYGEN)) */
/* RWPUBLICEND */

/* RWPUBLIC */
#ifdef     __cplusplus
extern "C"
{
#endif  /* __cplusplus */
/* RWPUBLICEND */

/* RWPUBLIC */
/*****************************************************************************
 Global Variables
 */

/*****************************************************************************
 Function prototypes
 */
extern RpToonMaterial *RpToonMaterialCreate(void);
extern void RpToonMaterialDestroy(RpToonMaterial *mat);

extern void RpToonMaterialSetPaintID(RpMaterial *mat, const RwChar *name);
/* RWPUBLICEND */

extern RwBool _rpToonMaterialPluginAttach(void);

/* RWPUBLIC */
#ifdef    __cplusplus
}
#endif /* __cplusplus */
/* RWPUBLICEND */

#endif /* TOONMATERIAL_H */
