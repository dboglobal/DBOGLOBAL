//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "D3D.h"
#include "VertexTypes.h"
#include "VertexBufferManager.h"

cD3D&
cD3D::refInstance()
{
    static cD3D theInstance;
    return theInstance;
}

HRESULT
cD3D::init(HWND hWnd, tSigned32 sizeX, tSigned32 sizeY)
{
    _sizeX = sizeX;
    _sizeY = sizeY;
    _renderState = RS_INVALID;
    _zBias = 0.0f;

    if(0 == (_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
    {
        MessageBox(0, "Direct3dCreate9() failed", "Error in renderer.", MB_OK);
        return E_FAIL;
    }

    // Get the current desktop display mode, so we can set up a back
    // buffer of the same format
    D3DDISPLAYMODE d3ddm;
    if(FAILED(_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
    {
        MessageBox(0, "GetAdapterDisplayMode() failed", "Error in renderer.", MB_OK);
        cleanup();
        return E_FAIL;
    }

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = d3ddm.Format;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    d3dpp.BackBufferCount = 2;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    HRESULT result;
    result = _pD3D->CreateDevice(
			D3DADAPTER_DEFAULT,
//			D3DDEVTYPE_REF,
			D3DDEVTYPE_HAL,
			hWnd,
            D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_FPU_PRESERVE,
            &d3dpp, &_pd3dDevice
			);
    if(result != D3D_OK)
    {
        if(result == D3DERR_INVALIDCALL)
        {
            MessageBox(0, "CreateDevice() failed  - D3DERR_INVALIDCALL", "Error in renderer.", MB_OK);
        }
        else
        if(result == D3DERR_NOTAVAILABLE)
        {
            MessageBox(0, "CreateDevice() failed  - D3DERR_NOTAVAILABLE", "Error in renderer.", MB_OK);
        }
        else
        if(result == D3DERR_OUTOFVIDEOMEMORY)
        {
            MessageBox(0, "CreateDevice() failed  - D3DERR_OUTOFVIDEOMEMORY", "Error in renderer.", MB_OK);
        }
        else
        {
            MessageBox(0, "CreateDevice() failed  - unnexpected error code", "Error in renderer.", MB_OK);
        }
        cleanup();
        return E_FAIL;
    }

    if(FAILED(_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE)))
    {
        MessageBox(0, "SetRenderState() failed for D3DRS_CULLMODE", "Error in renderer.", MB_OK);
        cleanup();
        return E_FAIL;
    }

    ZeroMemory(&_mtrl, sizeof(D3DMATERIAL9));
    _mtrl.Specular.r = _mtrl.Diffuse.r = _mtrl.Ambient.r = 1.0f;
    _mtrl.Specular.g = _mtrl.Diffuse.g = _mtrl.Ambient.g = 1.0f;
    _mtrl.Specular.b = _mtrl.Diffuse.b = _mtrl.Ambient.b = 1.0f;
    _mtrl.Specular.a = _mtrl.Diffuse.a = _mtrl.Ambient.a = 1.0f;
    _mtrl.Power = 10.0f;

    ZeroMemory(&_light, sizeof(D3DLIGHT9));
    _light.Type       = D3DLIGHT_DIRECTIONAL;
    _light.Diffuse.r  = 1.0f;
    _light.Diffuse.g  = 1.0f;
    _light.Diffuse.b  = 1.0f;
    _light.Diffuse.a  = 1.0f;
    _light.Range       = 1000000.0f;

    if(FAILED(_pd3dDevice->SetRenderState(D3DRS_LIGHTING, true)))
    {
        MessageBox(0, "SetRenderState() failed for D3DRS_LIGHTING", "Error in renderer.", MB_OK);
        cleanup();
        return E_FAIL;
    }
    if(FAILED(_pd3dDevice->LightEnable(0, true)))
    {
        MessageBox(0, "LightEnable failed", "Error in renderer.", MB_OK);
        cleanup();
        return E_FAIL;
    }
    if(FAILED(_pd3dDevice->SetRenderState(D3DRS_COLORVERTEX, true)))
    {
        MessageBox(0, "SetRenderState() failed", "Error in renderer.", MB_OK);
        cleanup();
        return E_FAIL;
    }
    if(FAILED(_pd3dDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL)))
    {
        MessageBox(0, "SetRenderState() failed", "Error in renderer.", MB_OK);
        cleanup();
        return E_FAIL;
    }
    if(FAILED(_pd3dDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL)))
    {
        MessageBox(0, "SetRenderState() failed", "Error in renderer.", MB_OK);
        cleanup();
        return E_FAIL;
    }
    if(FAILED(_pd3dDevice->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL)))
    {
        MessageBox(0, "SetRenderState() failed", "Error in renderer.", MB_OK);
        cleanup();
        return E_FAIL;
    }
    if(FAILED(_pd3dDevice->SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL)))
    {
        MessageBox(0, "SetRenderState() failed", "Error in renderer.", MB_OK);
        cleanup();
        return E_FAIL;
    }

    if(FAILED(_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE)))
    {
        MessageBox(0, "SetRenderState() failed for D3DRS_ALPHABLENDENABLE", "Error in renderer.", MB_OK);
        cleanup();
        return E_FAIL;
    }
//    if(FAILED(_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false)))
//    {
//        MessageBox(0, "SetRenderState() failed for D3DRS_ALPHABLENDENABLE", "Error in renderer.", MB_OK);
//        cleanup();
//        return E_FAIL;
//    }
    if(FAILED(_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true)))
    {
        MessageBox(0, "SetRenderState() failed for D3DRS_ALPHABLENDENABLE", "Error in renderer.", MB_OK);
        cleanup();
        return E_FAIL;
    }
    if(FAILED(_pd3dDevice->SetRenderState(D3DRS_ALPHAREF, 1)))
    {
        MessageBox(0, "SetRenderState() failed for D3DRS_ALPHAREF", "Error in renderer.", MB_OK);
        cleanup();
        return E_FAIL;
    }
    if(FAILED(_pd3dDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL)))
    {
        MessageBox(0, "SetRenderState() failed for D3DRS_ALPHAREF", "Error in renderer.", MB_OK);
        cleanup();
        return E_FAIL;
    }

    if(FAILED(_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA)))
    {
        MessageBox(0, "SetRenderState() failed.", "Error in renderer.", MB_OK);
        cleanup();
        return E_FAIL;
    }
//    if(FAILED(_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA)))
    if(FAILED(_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE)))
    {
        MessageBox(0, "SetRenderState() failed.", "Error in renderer.", MB_OK);
        cleanup();
        return E_FAIL;
    }
    if(FAILED(_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE)))
    {
        MessageBox(0, "SetRenderState() failed for D3DRS_ZENABLE", "Error in renderer.", MB_OK);
        cleanup();
        return E_FAIL;
    }

    {
//        HRESULT result;
//        result=_pd3dDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_POINT);
//        assertD(result==D3D_OK);
//        result=_pd3dDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_POINT);
//        assertD(result==D3D_OK);
//        result=_pd3dDevice->SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTEXF_POINT);
//        assertD(result==D3D_OK);
    }

    if(FAILED(D3DXCreateTextureFromFileEx(_pd3dDevice, "font.bmp",
                                            0, 0, 1, 0,
                                            D3DFMT_A8R8G8B8,
                                            D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE,
                                            0xFF000000,
                                            0, 0,
                                           &_pFontTexture)))
    {
//        if(FAILED(D3DXCreateTextureFromFile(_pd3dDevice, "font.bmp",
//                                               &_pFontTexture)))
//        {
//            MessageBox(0, "D3DXCreateTextureFromFile() failed for \"font.bmp\"", "Error in renderer.", MB_OK);
//            cleanup();
//            return E_FAIL;
//        }
        MessageBox(0, "D3DXCreateTextureFromFile() failed for \"font.bmp\"", "Error in renderer.", MB_OK);
        cleanup();
        return E_FAIL;
    }

    if(FAILED(D3DXCreateTextureFromFile(_pd3dDevice, "texture.bmp",
                                           &_pTexture2)))
    {
        MessageBox(0, "D3DXCreateTextureFromFile() failed for \"texture.bmp\"", "Error in renderer.", MB_OK);
        cleanup();
        return E_FAIL;
    }
    if(FAILED(D3DXCreateTextureFromFile(_pd3dDevice, "texture.bmp",
                                           &_pTexture3)))
    {
        MessageBox(0, "D3DXCreateTextureFromFile() failed for \"texture.bmp\"", "Error in renderer.", MB_OK);
        cleanup();
        return E_FAIL;
    }

    D3DXMATRIX matWorld;
    D3DXMatrixIdentity(&matWorld);
    if(FAILED(_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld)))
    {
        MessageBox(0, "SetTransform() failed", "Error in renderer.", MB_OK);
        cleanup();
        return E_FAIL;
    }

    const float standardCameraDistance = 1280.0f;
    float nearPlaneDistance = 40.0f;
    float viewRatio = standardCameraDistance / nearPlaneDistance;
    D3DXMatrixPerspectiveLH(&_matProj, sizeX / viewRatio, sizeY / viewRatio, nearPlaneDistance, 1200000);
    _matProj_Biased=_matProj;
    if(FAILED(_pd3dDevice->SetTransform(D3DTS_PROJECTION, &_matProj_Biased)))
    {
        MessageBox(0, "SetTransform() failed", "Error in renderer.", MB_OK);
        cleanup();
        return E_FAIL;
    }

    setCamera2D(1,0,0,false);
    _matTextView=_matView;

    _vertexBufferManager=new cVertexBufferManager;

    _in2D = false;

    return S_OK;
}

void
cD3D::setAmbient()
{
    tUnsigned32 value = 0xff;
    value <<= 8;
    value += static_cast<tUnsigned32>(_r * 0xff * _ambientPercent);
    value <<= 8;
    value += static_cast<tUnsigned32>(_g * 0xff * _ambientPercent);
    value <<= 8;
    value += static_cast<tUnsigned32>(_b * 0xff * _ambientPercent);
    HRESULT result;
    result = _pd3dDevice->SetRenderState(D3DRS_AMBIENT, value);
    assertD(result == D3D_OK);
}
void
cD3D::setLightColour(float r, float g, float b)
{
    _light.Diffuse.r = r * 0.65f;
    _light.Diffuse.g = g * 0.65f;
    _light.Diffuse.b = b * 0.65f;
    HRESULT result;
    result = _pd3dDevice->SetLight(0, &_light);
    assertD(result == D3D_OK);
    _r = r;
    _g = g;
    _b = b;
    setAmbient();
}

void
cD3D::setAlpha(float a)
{
    _mtrl.Specular.a = _mtrl.Diffuse.a = _mtrl.Ambient.a = a;
    HRESULT result;
    result = _pd3dDevice->SetMaterial(&_mtrl);
    assertD(result == D3D_OK);
    _a = a;
}

float
cD3D::getCurrentAlpha() const
{
    return _a;
}

void
cD3D::cleanup()
{
    size_t i;
    for(i = 0; i < _loadedImages.size(); ++i)
    {
        _loadedImages[i]->Release();
    }

    if(_pFontTexture != 0)
        _pFontTexture->Release();

    if(_pTexture != 0)
        _pTexture->Release();

    if(_pTexture2 != 0)
        _pTexture2->Release();

    if(_pTexture3 != 0)
        _pTexture3->Release();

    if(_pVB != 0)
        _pVB->Release();

    if(_pd3dDevice != 0)
        _pd3dDevice->Release();

    if(_pD3D != 0)
        _pD3D->Release();

    delete _vertexBufferManager; 
}


void
cD3D::beginScene()
{
    // Clear the backbuffer and the zbuffer
    _pd3dDevice->Clear(0, 0, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
                         D3DCOLOR_XRGB(0,0,0), 1.0f, 0);

    _pd3dDevice->BeginScene();
    setLightColour(1.0f, 1.0f, 1.0f);
    setAlpha(1.0f);
}


void
cD3D::endScene()
{
    _pd3dDevice->EndScene();

    // Present the backbuffer contents to the display
    _pd3dDevice->Present(0, 0, 0, 0);
}

void
cD3D::present()
{
    // Present the backbuffer contents to the display
    _pd3dDevice->Present(0, 0, 0, 0);
}

void
cD3D::setRenderState(eRenderState state)
{
    HRESULT result;
    if(state == _renderState)
    {
        return;
    }
    switch(state)
    {
    case RS_INVALID:
        invalid();
    case RS_LINE:
        _ambientPercent = 1.0f;
        setAmbient();
        result=_pd3dDevice->SetFVF(D3DFVF_LINEVERTEX);
        assertD(result==D3D_OK);

        result = _pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
        assertD(result==D3D_OK);

        result=_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_DISABLE);
        assertD(result==D3D_OK);
        _renderState=RS_LINE;
        return;
    case RS_TEXT:
        _ambientPercent = 1.0f;
        setAmbient();
        result=_pd3dDevice->SetFVF(D3DFVF_TEXTVERTEX);
        assertD(result==D3D_OK);

        result = _pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        assertD(result==D3D_OK);
        result = _pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
        assertD(result==D3D_OK);

    // Setup our texture. Using textures introduces the texture stage states,
    // which govern how textures get blended together (in the case of multiple
    // textures) and lighting information. In this case, we are modulating
    // (blending) our texture with the diffuse color of the vertices.
        result=_pd3dDevice->SetTexture(0, _pFontTexture);
        assertD(result==D3D_OK);
        result=_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
        assertD(result==D3D_OK);
        result=_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        assertD(result==D3D_OK);
        result=_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
        assertD(result==D3D_OK);

        result=_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        assertD(result==D3D_OK);
        result=_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
        assertD(result==D3D_OK);
        result=_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
        assertD(result==D3D_OK);
        
        result=_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
        assertD(result==D3D_OK);
        _renderState=RS_TEXT;
        return;
    case RS_POLYGON:
        _ambientPercent = 0.35f;
        setAmbient();
        result=_pd3dDevice->SetFVF(D3DFVF_POLYGONVERTEX);
        assertD(result==D3D_OK);

        result = _pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        assertD(result==D3D_OK);
        result = _pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
        assertD(result==D3D_OK);

        // Setup our texture. Using textures introduces the texture stage states,
        // which govern how textures get blended together (in the case of multiple
        // textures) and lighting information. In this case, we are modulating
        // (blending) our texture with the diffuse color of the vertices.
        result=_pd3dDevice->SetTexture(0, _pTexture2);
        assertD(result==D3D_OK);
        result=_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
        assertD(result==D3D_OK);
        result=_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        assertD(result==D3D_OK);
        result=_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
        assertD(result==D3D_OK);

        result=_pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
        assertD(result==D3D_OK);
        result=_pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
        assertD(result==D3D_OK);

        result=_pd3dDevice->SetTexture(1, _pTexture3);
        assertD(result==D3D_OK);
        result=_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_MODULATE);
        assertD(result==D3D_OK);
        result=_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        assertD(result==D3D_OK);
        result=_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
        assertD(result==D3D_OK);

        result=_pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
        assertD(result==D3D_OK);
        result=_pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
        assertD(result==D3D_OK);
        result=_pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
        assertD(result==D3D_OK);

        result=_pd3dDevice->SetTextureStageState(2, D3DTSS_COLOROP,   D3DTOP_DISABLE);
        assertD(result==D3D_OK);

        _renderState=RS_POLYGON;
        return;

    case RS_BLENDED_POLYGON:
        _ambientPercent = 0.35f;
        setAmbient();
        result=_pd3dDevice->SetFVF(D3DFVF_POLYGONVERTEX);
        assertD(result==D3D_OK);

        result = _pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
        assertD(result==D3D_OK);
        result = _pd3dDevice->SetRenderState(D3DRS_ZENABLE, false);
        assertD(result==D3D_OK);

        // Setup our texture. Using textures introduces the texture stage states,
        // which govern how textures get blended together (in the case of multiple
        // textures) and lighting information. In this case, we are modulating
        // (blending) our texture with the diffuse color of the vertices.
        result=_pd3dDevice->SetTexture(0, _pTexture2);
        assertD(result==D3D_OK);
        result=_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
        assertD(result==D3D_OK);
        result=_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        assertD(result==D3D_OK);
        result=_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
        assertD(result==D3D_OK);

        result=_pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
        assertD(result==D3D_OK);
        result=_pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
        assertD(result==D3D_OK);

        result=_pd3dDevice->SetTexture(1, _pTexture3);
        assertD(result==D3D_OK);
        result=_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_MODULATE);
        assertD(result==D3D_OK);
        result=_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        assertD(result==D3D_OK);
        result=_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
        assertD(result==D3D_OK);

        result=_pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
        assertD(result==D3D_OK);
        result=_pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
        assertD(result==D3D_OK);
        result=_pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
        assertD(result==D3D_OK);

        result=_pd3dDevice->SetTextureStageState(2, D3DTSS_COLOROP,   D3DTOP_DISABLE);
        assertD(result==D3D_OK);

        _renderState=RS_BLENDED_POLYGON;
        return;
    }
}
void
cD3D::setLoadedImageRenderState(tSigned32 index)
{
    HRESULT result;
    _ambientPercent = 1.0f;
    setAmbient();
    result=_pd3dDevice->SetFVF(D3DFVF_TEXTVERTEX);
    assertD(result==D3D_OK);

    result = _pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    assertD(result==D3D_OK);
    result = _pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
    assertD(result==D3D_OK);

    result=_pd3dDevice->SetTexture(0, _loadedImages[index]);
    assertD(result==D3D_OK);
    result=_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
    assertD(result==D3D_OK);
    result=_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    assertD(result==D3D_OK);
    result=_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    assertD(result==D3D_OK);

    result=_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    assertD(result==D3D_OK);
    result=_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    assertD(result==D3D_OK);
    result=_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
    assertD(result==D3D_OK);
    
    result=_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
    assertD(result==D3D_OK);


    _renderState = RS_INVALID;
}

void
cD3D::setStreamSource(IDirect3DVertexBuffer9* source, UINT size)
{
    if(source != _currentstreamsource)
    {
        HRESULT result;
        result = _pd3dDevice->SetStreamSource(0, source, 0, size);
        assertD(result == D3D_OK);
        _currentstreamsource = source;
    }
}
void
cD3D::clearStreamSource()
{
    _currentstreamsource = 0;
}

void
cD3D::setCamera2D(tSigned32 scale, tSigned32 x, tSigned32 y, bool zIsIgnored)
{
    _light.Direction.x=0.0f;
    _light.Direction.y=0.0f;
    _light.Direction.z=1.0f;
    _pd3dDevice->SetLight( 0, &_light );

    D3DXMatrixIdentity( &_matView );
    _matView(0,0)=1/32.0f;
    _matView(1,1)=1/32.0f;
    _matView(3,0)=-(_sizeX / 2 + x)/32.0f;
    _matView(3,1)=-(_sizeY / 2 + y)/32.0f;
    _matView(3,2)=40;

    if(zIsIgnored)
    {
        _matView(0,2)=0;
        _matView(1,2)=0;
        _matView(2,2)=0;
    }

    _pd3dDevice->SetTransform( D3DTS_VIEW, &_matView );
}

void
VectorForAngles(float angle1, float angle2, float distance, D3DXVECTOR3 &vector)
{
    float xy_component=cosf(angle2)*distance;
    vector.x=sinf(angle1)*xy_component;
    vector.y=cosf(angle1)*xy_component;
    vector.z=-sinf(angle2)*distance;
}

void
cD3D::setCamera3D(const float* target, float angle1, float angle2, float distance)
{
    D3DXVECTOR3 cameraVector;
    VectorForAngles(angle1, angle2, distance, cameraVector);
    cameraVector = -cameraVector;
    D3DXVec3Normalize((D3DXVECTOR3*)&_light.Direction, &cameraVector );
    cameraVector = -cameraVector;
    _pd3dDevice->SetLight( 0, &_light );

    D3DXVECTOR3 targetPosition;
    targetPosition.x = target[0];
    targetPosition.y = target[1];
    targetPosition.z = -target[2];
    
    D3DXVECTOR3 cameraPosition = targetPosition + cameraVector;

    D3DXVECTOR3 upVector;
    VectorForAngles(angle1, angle2 + D3DX_PI / 2.0f, 1.0f, upVector);

    D3DXMatrixLookAtLH(&_matView, &cameraPosition,
                                  &targetPosition,
                                  &upVector);

    _pd3dDevice->SetTransform(D3DTS_VIEW, &_matView);
}

void
cD3D::setZBias(float value)
{
    if(_zBias != value)
    {
        _matProj_Biased(3,2) = _matProj(3,2) + value;
        HRESULT result;
        result = _pd3dDevice->SetTransform(D3DTS_PROJECTION, &_matProj_Biased);
        assertD(result==D3D_OK);
        _zBias = value;
    }
}
float
cD3D::getZBias()
{
    return _zBias;
}

void
cD3D::into2D()
{
    assertD(!_in2D);
    _in2D = true;

    _storedZBias = _zBias;
    _storedLightDirection = _light.Direction;

    _light.Direction.x=0.0f;
    _light.Direction.y=0.0f;
    _light.Direction.z=1.0f;
    _pd3dDevice->SetLight( 0, &_light );
    HRESULT result;
    result=_pd3dDevice->SetTransform( D3DTS_VIEW, &_matTextView );
    assertD(result==D3D_OK);
    setZBias(0.001f);
}
void
cD3D::outOf2D()
{
    assertD(_in2D);
    _in2D=false;
    HRESULT result;
    result=_pd3dDevice->SetTransform( D3DTS_VIEW, &_matView );
    assertD(result==D3D_OK);
    setZBias(_storedZBias);
    _light.Direction = _storedLightDirection;
    _pd3dDevice->SetLight( 0, &_light );
}

tSigned32
cD3D::loadImage(const char* fileName)
{
    LPDIRECT3DTEXTURE9 texture;
    D3DXIMAGE_INFO imageInfo;
//    if(FAILED(D3DXCreateTextureFromFile(_pd3dDevice, fileName, &texture)))
//    {
//        return -1;
//    }

	if(FAILED(D3DXCreateTextureFromFileEx(_pd3dDevice, fileName,
                                 0, 0,
                                 1,   // Don't create mip-maps when you plan on using D3DXSPRITE. It throws off the pixel math for sprite animation.
                                 D3DPOOL_DEFAULT,
                                 D3DFMT_UNKNOWN,
                                 D3DPOOL_DEFAULT,
                                 D3DX_DEFAULT,
                                 D3DX_DEFAULT,
                                 0,
                                 &imageInfo,
                                 NULL,
                                 &texture)))
    {
        return -1;
    }
    
   
    tSigned32 result = static_cast<tSigned32>(_loadedImages.size());
    _loadedImages.push_back(texture);
    _imageInfos.push_back(imageInfo);
    return result;
}
