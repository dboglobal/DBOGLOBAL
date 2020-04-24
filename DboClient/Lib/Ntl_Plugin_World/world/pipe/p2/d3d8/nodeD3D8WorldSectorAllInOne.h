/****************************************************************************
 *                                                                          *
 * module : nodeD3D8WorldSectorAllInOne.h                                   *
 *                                                                          *
 * purpose: see nodeD3D8WorldSectorAllInOne.c                               *
 *                                                                          *
 ****************************************************************************/

#ifndef NODED3D8WORLDSECTORALLINONE_H
#define NODED3D8WORLDSECTORALLINONE_H

#include "rwcore.h"


/****************************************************************************
 global prototypes
 */

/* RWPUBLIC */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

extern RxNodeDefinition *RxNodeDefinitionGetD3D8WorldSectorAllInOne(void);
/* RWPUBLICEND */

extern D3DCAPS8 _RwD3D8DeviceCaps;

/* RWPUBLIC */
#ifdef    __cplusplus
}
#endif /* __cplusplus */

/* RWPUBLICEND */

#endif /* NODED3D8WORLDSECTORALLINONE_H */
