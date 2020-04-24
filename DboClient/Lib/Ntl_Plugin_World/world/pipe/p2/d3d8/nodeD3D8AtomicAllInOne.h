/****************************************************************************
 *                                                                          *
 * module : nodeD3D8AtomicAllInOne.h                                        *
 *                                                                          *
 * purpose: see nodeD3D8AtomicAllInOne.c                                    *
 *                                                                          *
 ****************************************************************************/

#ifndef NODED3D8ATOMICALLINONE_H
#define NODED3D8ATOMICALLINONE_H

#include "rwcore.h"


/****************************************************************************
 global prototypes
 */

/* RWPUBLIC */
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

extern RxNodeDefinition *RxNodeDefinitionGetD3D8AtomicAllInOne(void);
/* RWPUBLICEND */

extern D3DCAPS8 _RwD3D8DeviceCaps;

extern void 
_rwD3D8AtomicDefaultLightingCallback(void *object);

/* RWPUBLIC */
#ifdef    __cplusplus
}
#endif /* __cplusplus */
/* RWPUBLICEND */


#endif /* NODED3D8ATOMICALLINONE_H */
