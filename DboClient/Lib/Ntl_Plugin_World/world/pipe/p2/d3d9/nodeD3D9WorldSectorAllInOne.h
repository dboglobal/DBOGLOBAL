/****************************************************************************
 *                                                                          *
 * module : nodeD3D9WorldSectorAllInOne.h                                   *
 *                                                                          *
 * purpose: see nodeD3D9WorldSectorAllInOne.c                               *
 *                                                                          *
 ****************************************************************************/

#ifndef NODED3D9WORLDSECTORALLINONE_H
#define NODED3D9WORLDSECTORALLINONE_H

#include "rwcore.h"


/****************************************************************************
 global prototypes
 */

/* RWPUBLIC */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

extern RxNodeDefinition *
RxNodeDefinitionGetD3D9WorldSectorAllInOne(void);

#ifdef RWD3D9_USE_VERTEXSHADER_PIPELINE
extern RxNodeDefinition *
RxNodeDefinitionGetD3D9VertexShaderWorldSectorAllInOne(void);
#endif

/* RWPUBLICEND */

extern D3DCAPS9 _RwD3D9DeviceCaps;

/* RWPUBLIC */
#ifdef    __cplusplus
}
#endif /* __cplusplus */

/* RWPUBLICEND */

#endif /* NODED3D9WORLDSECTORALLINONE_H */
