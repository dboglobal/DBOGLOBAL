/***************************************************************************
 *                                                                         *
 * Module  : d3d8device.h                                                    *
 *                                                                         *
 * Purpose : d3d8 device header                                            *
 *                                                                         *
 **************************************************************************/

#ifndef D3D8DEVICE_H
#define D3D8DEVICE_H

/****************************************************************************
 Includes
 */

#include <d3d8.h>

/****************************************************************************
 Defines
 */

#define MINZBUFFERVALUE 0.0f
#define MAXZBUFFERVALUE 1.0f

/****************************************************************************
 Global Types
 */

typedef struct _rwD3D8AdapterInformation _rwD3D8AdapterInformation;
struct _rwD3D8AdapterInformation
{
    RwChar          name[MAX_DEVICE_IDENTIFIER_STRING];
    RwInt32         modeCount;
    D3DDISPLAYMODE  mode;
    RwInt32         displayDepth;
    RwInt32         flags;
};

extern HWND                         WindowHandle;
extern RwRGBAReal                   AmbientSaturated;
extern D3DPRESENT_PARAMETERS        Present;
extern _rwD3D8AdapterInformation    _RwD3D8AdapterInformation;
extern LPDIRECT3DSURFACE8           _RwD3D8DepthStencilSurface;
extern D3DCAPS8                     _RwD3D8DeviceCaps;
extern LPDIRECT3DSURFACE8           _RwD3D8RenderSurface;
extern RwInt32                      _RwD3D8ZBufferDepth;
extern LPDIRECT3DDEVICE8            _RwD3DDevice;

/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

#if defined(RWDEBUG)

extern HRESULT  _rwD3D8CheckError(HRESULT hr, const RwChar *function, const RwChar *file, RwUInt32 line);

#define DXCHECK(fn) (_rwD3D8CheckError(fn, #fn, __FILE__, __LINE__))

#else
#define DXCHECK(fn) (fn)
#endif


extern RwBool   _rwD3D8BeginScene(void);

extern RwBool   _rwD3D8CameraClear(void *camera, void *color, RwInt32 clearFlags);
extern RwBool   _rwD3D8CameraBeginUpdate(void *out, void *cameraIn, RwInt32 in);
extern RwBool   _rwD3D8CameraEndUpdate(void *out, void *inOut, RwInt32 nIn);
extern RwBool   _rwD3D8RasterShowRaster(void *out, void *inOut, RwInt32 flags);

extern RwBool   _rwD3D8CheckValidZBufferFormat(RwInt32 format);
extern RwBool   _rwD3D8CheckValidTextureFormat(RwInt32 format);
extern RwBool   _rwD3D8CheckValidCameraTextureFormat(RwInt32 format);
extern RwBool   _rwD3D8CheckValidCubeTextureFormat(RwInt32 format);
extern RwBool   _rwD3D8CheckValidCameraCubeTextureFormat(RwInt32 format);

extern RwBool   _rwD3D8ForceLight(RwInt32 index, const void *light);

extern RwBool   _rwD3D8LightsEqual(const D3DLIGHT8 *light0, const D3DLIGHT8 *light1);
extern RwUInt32 _rwD3D8FindSimilarLight(const void *light); /* returns 0xffffffff on failure */

extern RwBool   _rwD3D8IndexBuffer32bitsCreate(RwUInt32 numIndices, void **indexBuffer);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#endif /* D3D8DEVICE_H */
