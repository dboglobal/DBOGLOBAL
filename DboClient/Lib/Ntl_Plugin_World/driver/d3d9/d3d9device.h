/***************************************************************************
 *                                                                         *
 * Module  : d3d9device.h                                                    *
 *                                                                         *
 * Purpose : d3d9 device header                                            *
 *                                                                         *
 **************************************************************************/

#ifndef D3D9DEVICE_H
#define D3D9DEVICE_H

/****************************************************************************
 Includes
 */

#include <d3d9.h>

/****************************************************************************
 Defines
 */

// OPT-IN for PERFHUD
//#define dNTL_OPTIN_FOR_PERFHUD

#define MINZBUFFERVALUE 0.0f
#define MAXZBUFFERVALUE 1.0f

#define RWD3D9_ALIGN16 __declspec(align(16))

/****************************************************************************
 Global Types
 */

typedef LPDIRECT3DVERTEXSHADER9 LPVERTEXSHADER;
typedef LPDIRECT3DPIXELSHADER9  LPPIXELSHADER;

typedef struct _rwD3D9AdapterInformation _rwD3D9AdapterInformation;
struct _rwD3D9AdapterInformation
{
    RwChar          name[MAX_DEVICE_IDENTIFIER_STRING];
    RwInt32         modeCount;
    D3DDISPLAYMODE  mode;
    RwInt32         displayDepth;
    RwInt32         flags;
};

extern HWND                         WindowHandle;
extern LPDIRECT3D9                  _RwDirect3DObject;
extern RwUInt32                     _RwD3DAdapterIndex;
extern RwUInt32                     _RwD3DAdapterType;
extern LPDIRECT3DDEVICE9            _RwD3DDevice;
extern D3DPRESENT_PARAMETERS        Present;
extern _rwD3D9AdapterInformation    _RwD3D9AdapterInformation;
extern LPDIRECT3DSURFACE9           _RwD3D9RenderSurface;
extern LPDIRECT3DSURFACE9           _RwD3D9DepthStencilSurface;
extern D3DCAPS9                     _RwD3D9DeviceCaps;
extern RwInt32                      _RwD3D9ZBufferDepth;
extern RwRGBAReal                   AmbientSaturated;

extern D3DMATRIX            _RwD3D9D3D9ViewTransform;
extern D3DMATRIX            _RwD3D9D3D9ProjTransform;
/* This pointer is set to NULL every time any of the previous matrix is changed */
extern const D3DMATRIX      *_RwD3D9ActiveViewProjTransform;    


/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

#if defined(RWDEBUG)

extern HRESULT  _rwD3D9CheckError(HRESULT hr, const RwChar *function, const RwChar *file, RwUInt32 line);

#define DXCHECK(fn) (_rwD3D9CheckError(fn, #fn, __FILE__, __LINE__))

#else
#define DXCHECK(fn) (fn)
#endif


extern RwBool
_rwD3D9BeginScene(void);

extern RwBool
_rwD3D9SetRenderTarget(RwUInt32 index,
                       LPDIRECT3DSURFACE9 rendertarget);

extern void
_rwD3D9SetDepthStencilSurface(LPDIRECT3DSURFACE9 depthStencilSurface);

extern RwBool   _rwD3D9CameraClear(void *camera, void *color, RwInt32 clearFlags);
extern RwBool   _rwD3D9CameraBeginUpdate(void *out, void *cameraIn, RwInt32 in);
extern RwBool   _rwD3D9CameraEndUpdate(void *out, void *inOut, RwInt32 nIn);
extern RwBool   _rwD3D9RasterShowRaster(void *out, void *inOut, RwInt32 flags);

extern RwBool   _rwD3D9ForceLight(RwInt32 index, const void *light);

extern RwBool   _rwD3D9LightsEqual(const D3DLIGHT9 *light0, const D3DLIGHT9 *light1);
extern RwUInt32 _rwD3D9FindSimilarLight(const void *light); /* returns 0xffffffff on failure */

extern RwBool   _rwD3D9IndexBuffer32bitsCreate(RwUInt32 numIndices, void **indexBuffer);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#endif /* D3D9DEVICE_H */
