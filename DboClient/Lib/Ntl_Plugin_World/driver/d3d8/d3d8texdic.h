/***************************************************************************
 *                                                                         *
 * Module  : D3D8texdic.h                                                    *
 *                                                                         *
 * Purpose : D3D8 texture dictionary                                       *
 *                                                                         *
 **************************************************************************/

#ifndef D3D8TEXDIC_H
#define D3D8TEXDIC_H

/****************************************************************************
 Includes
 */

#include <d3d8.h>

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

extern RwBool _rwD3D8NativeTextureGetSize(void *sizeIn, void *textureIn, RwInt32 unused3);
extern RwBool _rwD3D8NativeTextureWrite(void *streamIn, void *textureIn, RwInt32 unused3);
extern RwBool _rwD3D8NativeTextureRead(void *streamIn, void *textureIn, RwInt32 unused3);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#endif /* D3D8TEXDIC_H */
