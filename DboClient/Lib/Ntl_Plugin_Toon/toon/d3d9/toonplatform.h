/*****************************************************************************

    File: toonplatform.h

    Purpose: A short description of the file.

    Copyright (c) 2002 Criterion Software Ltd.

 */
/* RWPUBLIC */
#ifndef TOONPLATFORM_H
#define TOONPLATFORM_H
/* RWPUBLICEND */

/*****************************************************************************
 Includes
 */

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

/*****************************************************************************
 Function Pointers
 */

/*****************************************************************************
 Structs
 */

/* RWPUBLIC */
/*****************************************************************************
 Function prototypes
 */

#ifdef     __cplusplus
extern "C"
{
#endif  /* __cplusplus */

extern RpAtomic *
RpD3D9ToonFastSilhouetteAtomicRenderCallback(RpAtomic *atomic);

extern RpAtomic *
RpD3D9ToonFastNotSilhouetteAtomicRenderCallback(RpAtomic *atomic);

extern RpWorldSector *
RpD3D9ToonFastSilhouetteWorldSectorRenderCallback(RpWorldSector *sector);

#ifdef    __cplusplus
}
#endif /* __cplusplus */
/* RWPUBLICEND */

/* RWPUBLIC */
#endif /* TOONPLATFORM_H */
/* RWPUBLICEND */