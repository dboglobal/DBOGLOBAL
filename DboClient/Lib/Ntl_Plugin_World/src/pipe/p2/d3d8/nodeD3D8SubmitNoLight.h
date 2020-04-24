/****************************************************************************
 *                                                                          *
 * module : nodeD3D8oxSubmitNoLight.h                                         *
 *                                                                          *
 * purpose: see nodeD3D8oxSubmitNoLight.c                                     *
 *                                                                          *
 ****************************************************************************/

#ifndef NODED3D8SUBMITNOLIGHT_H
#define NODED3D8SUBMITNOLIGHT_H

#include "p2core.h"
 
/****************************************************************************
 global prototypes
 */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

extern RwBool _rwD3D8Im3DRenderOpen(void);

extern RxNodeDefinition *RxNodeDefinitionGetD3D8ImmInstance(void);
extern RxNodeDefinition *RxNodeDefinitionGetD3D8SubmitNoLight(void);

extern void _rwD3D8Im3DRenderClose(void);

#ifdef    __cplusplus
}
#endif /* __cplusplus */

#endif /* NODED3D8SUBMITNOLIGHT_H */
