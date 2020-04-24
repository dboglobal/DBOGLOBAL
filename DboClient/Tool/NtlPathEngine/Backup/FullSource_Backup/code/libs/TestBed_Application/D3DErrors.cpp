//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include <windows.h>
#include <dxerr8.h>

char *d3derrstring(HRESULT value)
{
#ifdef DEBUG
    switch(value)
    {
    default:
        return "Unknown d3d error.";
    case D3D_OK:
        return "D3D_OK - No error occurred.";
    case D3DERR_BADMAJORVERSION:
        return "D3DERR_BADMAJORVERSION - The service you requested is unavailable in this major version of DirectX. (A 'major version' denotes a primary release, such as DirectX 6.0.)";
    case D3DERR_BADMINORVERSION:
        return "D3DERR_BADMINORVERSION - The service you requested is available in this major version of DirectX, but not in this minor version. Get the latest version of the component runtime from Microsoft. (A 'minor version' denotes a secondary release, such as DirectX 6.1.)";
    case D3DERR_COLORKEYATTACHED:
        return "D3DERR_COLORKEYATTACHED - The application attempted to create a texture with a surface that uses a color key for transparency.";
    case D3DERR_CONFLICTINGTEXTUREFILTER:
        return "D3DERR_CONFLICTINGTEXTUREFILTER - The current texture filters cannot be used together.";
    case D3DERR_CONFLICTINGTEXTUREPALETTE:
        return "D3DERR_CONFLICTINGTEXTUREPALETTE - The current textures cannot be used simultaneously. This generally occurs when a multi-texture device requires that all palettized textures simultaneously enabled also share the same palette.";
    case D3DERR_CONFLICTINGRENDERSTATE:
        return "D3DERR_CONFLICTINGRENDERSTATE - The currently set render states cannot be used together.";
    case D3DERR_DEVICEAGGREGATED:
        return "D3DERR_DEVICEAGGREGATED - The IDirect3DDevice3::SetRenderTarget method was called on a device that was retrieved from the render target surface.";
    case D3DERR_EXECUTE_CLIPPED_FAILED:
        return "D3DERR_EXECUTE_CLIPPED_FAILED - The execute buffer could not be clipped during execution.";
    case D3DERR_EXECUTE_CREATE_FAILED:
        return "D3DERR_EXECUTE_CREATE_FAILED - The execute buffer could not be created. This typically occurs when no memory is available to allocate the execute buffer.";
    case D3DERR_EXECUTE_DESTROY_FAILED:
        return "D3DERR_EXECUTE_DESTROY_FAILED - The memory for the execute buffer could not be deallocated.";
    case D3DERR_EXECUTE_FAILED:
        return "D3DERR_EXECUTE_FAILED - The contents of the execute buffer are invalid and cannot be executed.";
    case D3DERR_EXECUTE_LOCK_FAILED:
        return "D3DERR_EXECUTE_LOCK_FAILED - The execute buffer could not be locked.";
    case D3DERR_EXECUTE_LOCKED:
        return "D3DERR_EXECUTE_LOCKED - The operation requested by the application could not be completed because the execute buffer is locked.";
    case D3DERR_EXECUTE_NOT_LOCKED:
        return "D3DERR_EXECUTE_NOT_LOCKED - The execute buffer could not be unlocked because it is not currently locked.";
    case D3DERR_EXECUTE_UNLOCK_FAILED:
        return "D3DERR_EXECUTE_UNLOCK_FAILED - The execute buffer could not be unlocked.";
    case D3DERR_INITFAILED:
        return "D3DERR_INITFAILED - A rendering device could not be created because the new device could not be initialized.";
    case D3DERR_INBEGIN:
        return "D3DERR_INBEGIN - The requested operation cannot be completed while scene rendering is taking place. Try again after the scene is completed and the IDirect3DDevice::EndScene method (or equivalent method) is called.";
    case D3DERR_INVALID_DEVICE:
        return "D3DERR_INVALID_DEVICE - The requested device type is not valid.";
    case D3DERR_INVALIDCURRENTVIEWPORT:
        return "D3DERR_INVALIDCURRENTVIEWPORT - The currently selected viewport is not valid.";
    case D3DERR_INVALIDMATRIX:
        return "D3DERR_INVALIDMATRIX - The requested operation could not be completed because the combination of the currently set world, view, and projection matrices is invalid (the determinant of the combined matrix is zero).";
    case D3DERR_INVALIDPALETTE:
        return "D3DERR_INVALIDPALETTE - The palette associated with a surface is invalid.";
    case D3DERR_INVALIDPRIMITIVETYPE:
        return "D3DERR_INVALIDPRIMITIVETYPE - The primitive type specified by the application is invalid.";
    case D3DERR_INVALIDRAMPTEXTURE:
        return "D3DERR_INVALIDRAMPTEXTURE - Ramp mode is being used and the texture handle in the current material does not match the current texture handle that is set as a render state.";
    case D3DERR_INVALIDVERTEXFORMAT:
        return "D3DERR_INVALIDVERTEXFORMAT - The combination of flexible vertex format flags specified by the application is not valid.";
    case D3DERR_INVALIDVERTEXTYPE:
        return "D3DERR_INVALIDVERTEXTYPE - The vertex type specified by the application is invalid.";
    case D3DERR_LIGHT_SET_FAILED:
        return "D3DERR_LIGHT_SET_FAILED - The attempt to set lighting parameters for a light object failed.";
    case D3DERR_LIGHTHASVIEWPORT:
        return "D3DERR_LIGHTHASVIEWPORT - The requested operation failed because the light object is associated with another viewport.";
    case D3DERR_LIGHTNOTINTHISVIEWPORT:
        return "D3DERR_LIGHTNOTINTHISVIEWPORT - The requested operation failed because the light object has not been associated with this viewport.";
    case D3DERR_MATERIAL_CREATE_FAILED:
        return "D3DERR_MATERIAL_CREATE_FAILED - The material could not be created. This typically occurs when no memory is available to allocate for the material.";
    case D3DERR_MATERIAL_DESTROY_FAILED:
        return "D3DERR_MATERIAL_DESTROY_FAILED - The memory for the material could not be deallocated.";
    case D3DERR_MATERIAL_GETDATA_FAILED:
        return "D3DERR_MATERIAL_GETDATA_FAILED - The material parameters could not be retrieved.";
    case D3DERR_MATERIAL_SETDATA_FAILED:
        return "D3DERR_MATERIAL_SETDATA_FAILED - The material parameters could not be set.";
    case D3DERR_MATRIX_CREATE_FAILED:
        return "D3DERR_MATRIX_CREATE_FAILED - The matrix could not be created. This can occur when no memory is available to allocate for the matrix.";
    case D3DERR_MATRIX_DESTROY_FAILED:
        return "D3DERR_MATRIX_DESTROY_FAILED - The memory for the matrix could not be deallocated.";
    case D3DERR_MATRIX_GETDATA_FAILED:
        return "D3DERR_MATRIX_GETDATA_FAILED - The matrix data could not be retrieved. This can occur when the matrix was not created by the current device.";
    case D3DERR_MATRIX_SETDATA_FAILED:
        return "D3DERR_MATRIX_SETDATA_FAILED - The matrix data could not be set. This can occur when the matrix was not created by the current device.";
    case D3DERR_NOCURRENTVIEWPORT:
        return "D3DERR_NOCURRENTVIEWPORT - The viewport parameters could not be retrieved because none have been set.";
    case D3DERR_NOTINBEGIN:
        return "D3DERR_NOTINBEGIN - The requested rendering operation could not be completed because scene rendering has not begun. Call IDirect3DDevice3::BeginScene to begin rendering then try again.";
    case D3DERR_NOVIEWPORTS:
        return "D3DERR_NOVIEWPORTS - The requested operation failed because the device currently has no viewports associated with it.";
    case D3DERR_SCENE_BEGIN_FAILED:
        return "D3DERR_SCENE_BEGIN_FAILED - Scene rendering could not begin.";
    case D3DERR_SCENE_END_FAILED:
        return "D3DERR_SCENE_END_FAILED - Scene rendering could not be completed.";
    case D3DERR_SCENE_IN_SCENE:
        return "D3DERR_SCENE_IN_SCENE - Scene rendering could not begin because a previous scene was not completed by a call to the IDirect3DDevice3::EndScene method.";
    case D3DERR_SCENE_NOT_IN_SCENE:
        return "D3DERR_SCENE_NOT_IN_SCENE - Scene rendering could not be completed because a scene was not started by a previous call to the IDirect3DDevice3::BeginScene method.";
    case D3DERR_SETVIEWPORTDATA_FAILED:
        return "D3DERR_SETVIEWPORTDATA_FAILED - The viewport parameters could not be set.";
    case D3DERR_STENCILBUFFER_NOTPRESENT:
        return "D3DERR_STENCILBUFFER_NOTPRESENT - The requested stencil buffer operation could not be completed because there is no stencil buffer attached to the render target surface.";
    case D3DERR_SURFACENOTINVIDMEM:
        return "D3DERR_SURFACENOTINVIDMEM - The device could not be created because the render target surface is not located in video-memory. (Hardware-accelerated devices require video-memory render target surfaces.)";
    case D3DERR_TEXTURE_BADSIZE:
        return "D3DERR_TEXTURE_BADSIZE - The dimensions of a current texture are invalid. This can occur when an application attempts to use a texture that has non-power-of-two dimensions with a device that requires them.";
    case D3DERR_TEXTURE_CREATE_FAILED:
        return "D3DERR_TEXTURE_CREATE_FAILED - The texture handle for the texture could not be retrieved from the driver.";
    case D3DERR_TEXTURE_DESTROY_FAILED:
        return "D3DERR_TEXTURE_DESTROY_FAILED - The device was unable to deallocate the texture memory.";
    case D3DERR_TEXTURE_GETSURF_FAILED:
        return "D3DERR_TEXTURE_GETSURF_FAILED - The DirectDraw surface used to create the texture could not be retrieved.";
    case D3DERR_TEXTURE_LOAD_FAILED:
        return "D3DERR_TEXTURE_LOAD_FAILED - The texture could not be loaded.";
    case D3DERR_TEXTURE_LOCK_FAILED:
        return "D3DERR_TEXTURE_LOCK_FAILED - The texture could not be locked.";
    case D3DERR_TEXTURE_LOCKED:
        return "D3DERR_TEXTURE_LOCKED - The requested operation could not be completed because the texture surface is currently locked.";
    case D3DERR_TEXTURE_NO_SUPPORT:
        return "D3DERR_TEXTURE_NO_SUPPORT - The device does not support texture mapping.";
    case D3DERR_TEXTURE_NOT_LOCKED:
        return "D3DERR_TEXTURE_NOT_LOCKED - The requested operation could not be completed because the texture surface is not locked.";
    case D3DERR_TEXTURE_SWAP_FAILED:
        return "D3DERR_TEXTURE_SWAP_FAILED - The texture handles could not be swapped.";
    case D3DERR_TEXTURE_UNLOCK_FAILED:
        return "D3DERR_TEXTURE_UNLOCK_FAILED - The texture surface could not be unlocked.";
    case D3DERR_TOOMANYOPERATIONS:
        return "D3DERR_TOOMANYOPERATIONS - The application is requesting more texture filtering operations than the device supports.";
    case D3DERR_TOOMANYPRIMITIVES:
        return "D3DERR_TOOMANYPRIMITIVES - The device is unable to render the provided quantity of primitives in a single pass.";
    case D3DERR_UNSUPPORTEDALPHAARG:
        return "D3DERR_UNSUPPORTEDALPHAARG - The device does not support one of the specified texture blending arguments for the alpha channel.";
    case D3DERR_UNSUPPORTEDALPHAOPERATION:
        return "D3DERR_UNSUPPORTEDALPHAOPERATION - The device does not support one of the specified texture blending operations for the alpha channel.";
    case D3DERR_UNSUPPORTEDCOLORARG:
        return "D3DERR_UNSUPPORTEDCOLORARG - The device does not support the one of the specified texture blending arguments for color values.";
    case D3DERR_UNSUPPORTEDCOLOROPERATION:
        return "D3DERR_UNSUPPORTEDCOLOROPERATION - The device does not support the one of the specified texture blending operations for color values.";
    case D3DERR_UNSUPPORTEDFACTORVALUE:
        return "D3DERR_UNSUPPORTEDFACTORVALUE - The specified texture factor value is not supported by the device.";
    case D3DERR_UNSUPPORTEDTEXTUREFILTER:
        return "D3DERR_UNSUPPORTEDTEXTUREFILTER - The specified texture filter is not supported by the device.";
    case D3DERR_VBUF_CREATE_FAILED:
        return "D3DERR_VBUF_CREATE_FAILED - The vertex buffer could not be created. This can happen when there is insufficient memory to allocate a vertex buffer.";
    case D3DERR_VERTEXBUFFERLOCKED:
        return "D3DERR_VERTEXBUFFERLOCKED - The requested operation could not be completed because the vertex buffer is locked.";
    case D3DERR_VERTEXBUFFEROPTIMIZED:
        return "D3DERR_VERTEXBUFFEROPTIMIZED - The requested operation could not be completed because the vertex buffer is optimized. (The contents of optimized vertex buffers are driver specific, and considered private.)";
    case D3DERR_VIEWPORTDATANOTSET:
        return "D3DERR_VIEWPORTDATANOTSET - The requested operation could not be completed because viewport parameters have not yet been set. Set the viewport parameters by calling IDirect3DViewport3::SetViewport method and try again.";
    case D3DERR_VIEWPORTHASNODEVICE:
        return "D3DERR_VIEWPORTHASNODEVICE - The requested operation could not be completed because the viewport has not yet been associated with a device. Associate the viewport with a rendering device by calling IDirect3DDevice3::AddViewport and try again.";
    case D3DERR_WRONGTEXTUREFORMAT:
        return "D3DERR_WRONGTEXTUREFORMAT - The pixel format of the texture surface is not valid.";
    case D3DERR_ZBUFF_NEEDS_SYSTEMMEMORY:
        return "D3DERR_ZBUFF_NEEDS_SYSTEMMEMORY - The requested operation could not be completed because the specified device requires system-memory depth-buffer surfaces. (Software rendering devices require system-memory depth buffers.)";
    case D3DERR_ZBUFF_NEEDS_VIDEOMEMORY:
        return "D3DERR_ZBUFF_NEEDS_VIDEOMEMORY - The requested operation could not be completed because the specified device requires video-memory depth-buffer surfaces. (Hardware-accelerated devices require video-memory depth buffers.)";
    case D3DERR_ZBUFFER_NOTPRESENT:
        return "D3DERR_ZBUFFER_NOTPRESENT - The requested operation could not be completed because the render target surface does not have an attached depth buffer.";
    }
#else
    return "**d3d errors not in release mode**";
#endif
}
