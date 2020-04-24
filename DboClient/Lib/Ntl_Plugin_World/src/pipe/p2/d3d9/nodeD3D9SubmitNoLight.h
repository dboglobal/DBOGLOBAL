/****************************************************************************
 *                                                                          *
 * module : nodeD3D9oxSubmitNoLight.h                                         *
 *                                                                          *
 * purpose: see nodeD3D9oxSubmitNoLight.c                                     *
 *                                                                          *
 ****************************************************************************/

#ifndef NODED3D9SUBMITNOLIGHT_H
#define NODED3D9SUBMITNOLIGHT_H

#include "p2core.h"
 
/****************************************************************************
 global prototypes
 */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

extern RwBool _rwD3D9Im3DRenderOpen(void);

extern RxNodeDefinition *RxNodeDefinitionGetD3D9ImmInstance(void);
extern RxNodeDefinition *RxNodeDefinitionGetD3D9SubmitNoLight(void);

extern void _rwD3D9Im3DRenderClose(void);

#ifdef    __cplusplus
}
#endif /* __cplusplus */

#endif /* NODED3D9SUBMITNOLIGHT_H */
