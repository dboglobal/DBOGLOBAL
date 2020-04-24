/*****************************************************************************

    File: genericpipes.h

    Purpose: A short description of the file.

    Copyright (c) 2002 Criterion Software Ltd.

 */

#ifndef GENERICPIPES_H
#define GENERICPIPES_H

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

#ifdef     __cplusplus
extern "C"
{
#endif  /* __cplusplus */

/*****************************************************************************
 Global Variables
 */

/*****************************************************************************
 Function prototypes
 */
extern RwBool _rpToonPipelinesCreate(void);
extern RwBool _rpToonPipelinesDestroy(void);
extern RwBool _rpToonAtomicPipelinesAttach(RpAtomic *atomic);
extern RwBool _rpToonSectorPipelinesAttach(RpWorldSector *sector);
extern void _rpToonAtomicChainAtomicRenderCallback(RpAtomic *atomic);
extern void _rpToonAtomicChainSkinnedAtomicRenderCallback(RpAtomic *atomic);

#ifdef    __cplusplus
}
#endif /* __cplusplus */

#endif /* GENERICPIPES_H */
