/***************************************************************************
 *                                                                         *
 * Module  : wrldpipe.h                                                    *
 *                                                                         *
 * Purpose : See wrldpipe.c                                                *
 *                                                                         *
 **************************************************************************/

#ifndef WRLDPIPE_H
#define WRLDPIPE_H

/****************************************************************************
 Includes
 */

#include "rwcore.h"

/****************************************************************************
 Global Defines
 */

/****************************************************************************
 Global Types
 */

/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif /* __cplusplus */

extern RwBool _rpCreatePlatformMaterialPipelines(void);
extern RwBool _rpCreatePlatformWorldSectorPipelines(void);
extern RwBool _rpCreatePlatformAtomicPipelines(void);

extern void _rpDestroyPlatformMaterialPipelines(void);
extern void _rpDestroyPlatformAtomicPipelines(void);
extern void _rpDestroyPlatformWorldSectorPipelines(void);

#ifdef    __cplusplus
}
#endif /* __cplusplus */

#endif /* WRLDPIPE_H */
