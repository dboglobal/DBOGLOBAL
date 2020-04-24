/***************************************************************************
 *                                                                         *
 * Module  : D3D9texdic.h                                                    *
 *                                                                         *
 * Purpose : D3D9 texture dictionary                                       *
 *                                                                         *
 **************************************************************************/

#ifndef D3D9TEXDIC_H
#define D3D9TEXDIC_H

/****************************************************************************
 Includes
 */

#include <d3d9.h>

#include "batypes.h"

/****************************************************************************
 Defines
 */

/****************************************************************************
 Global Types
 */

/****************************************************************************
 Global variables (across program)
 */

/****************************************************************************
 Function prototypes
 */
#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

extern RwBool _rwD3D9NativeTextureGetSize(void *sizeIn, void *textureIn, RwInt32 unused3);
extern RwBool _rwD3D9NativeTextureWrite(void *streamIn, void *textureIn, RwInt32 unused3);
extern RwBool _rwD3D9NativeTextureRead(void *streamIn, void *textureIn, RwInt32 unused3);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#endif /* D3D9TEXDIC_H */
