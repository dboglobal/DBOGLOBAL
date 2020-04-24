/*****************************************************************************

    File: toon.h

    Purpose: A short description of the file.

    Copyright (c) 2002 Criterion Software Ltd.

 */

#ifndef TOON_H
#define TOON_H

/*****************************************************************************
 Includes
 */
#include "rwcore.h"
#include "rpworld.h"

#include "toongeo.h"

/*****************************************************************************
 Defines
 */

/* RWPUBLIC */
/*****************************************************************************
 Enums
 */
enum RpToonType
{
    rpNATOONTYPE        = 0,
    rpTOONTYPEGENERIC   = 1,
    /* rpTOONTYPESKIN      = 2, */
    rpTOONTYPEFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};

/*****************************************************************************
 Typedef Enums
 */
typedef enum RpToonType RpToonType;

/*****************************************************************************
 Typedef Structs
 */
typedef struct ToonGlobals  ToonGlobals;

/*****************************************************************************
 Function Pointers
 */

/*****************************************************************************
 Structs
 */
#if (!defined(DOXYGEN))
struct ToonGlobals
{
    /* Offsets */
    RwInt32         engineOffset;
    RwInt32         atomicOffset;
    RwInt32         geometryOffset;
    RwInt32         sectorOffset;
    RwInt32         materialOffset;

    /* Plugin data */
    RwFreeList      *freeList;
    RwModuleInfo    module;

    /* Platform global data */
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
extern ToonGlobals  _rpToonGlobals;

/*****************************************************************************
 Function prototypes
 */
/* Z bias */
extern void RpToonSetCreaseZBias(RwReal zBias);
extern RwReal RpToonGetCreaseZBias(void);

/* Create */
extern RpToonGeo *
RpToonGeometryCreateToonGeo(RpGeometry *geometry,
                            RwBool generateCreaseEdgesFromSmoothingGroups);
RpToonGeo *
RpToonWorldSectorCreateToonGeo(RpWorldSector *sector,
                               RwBool generateCreaseEdgesFromSmoothingGroups);

/* Enable */
extern RwBool
RpToonAtomicEnable(RpAtomic *atomic);
extern RwBool
RpToonWorldSectorEnable(RpWorldSector *sector);

/* Get */
extern RpToonGeo *
RpToonGeometryGetToonGeo(RpGeometry *geometry);
extern RpToonGeo *
RpToonWorldSectorGetToonGeo(RpWorldSector *sector);

/* Plugin */
extern RwBool
RpToonPluginAttach(void);
/* RWPUBLICEND */

/* RWPUBLIC */
#ifdef    __cplusplus
}
#endif /* __cplusplus */
/* RWPUBLICEND */

#endif /* TOON_H */
