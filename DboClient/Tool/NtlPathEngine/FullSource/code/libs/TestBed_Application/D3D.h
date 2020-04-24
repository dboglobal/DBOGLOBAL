//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include <d3d9.h>
#include <d3dx9.h>
#include <windows.h>
#include "common/STL/vector.h"

class cVertexBufferManager;

enum eRenderState
{
    RS_INVALID,
    RS_LINE,
    RS_TEXT,
    RS_POLYGON,
    RS_BLENDED_POLYGON,
};

class cD3D
{
    tSigned32 _sizeX, _sizeY;
    LPDIRECT3D9             _pD3D;
    LPDIRECT3DDEVICE9       _pd3dDevice;
    LPDIRECT3DVERTEXBUFFER9 _pVB;
    LPDIRECT3DTEXTURE9      _pFontTexture;
    LPDIRECT3DTEXTURE9      _pTexture;
    LPDIRECT3DTEXTURE9      _pTexture2;
    LPDIRECT3DTEXTURE9      _pTexture3;
    D3DLIGHT9               _light;
    D3DMATERIAL9            _mtrl;
    D3DXMATRIX              _matProj;
    D3DXMATRIX              _matProj_Biased;
    D3DXMATRIX              _matView;
    D3DXMATRIX              _matTextView;
    cVertexBufferManager*   _vertexBufferManager;
    float _r, _g, _b, _a;
    float _ambientPercent;
    eRenderState _renderState;
    float _zBias;
    bool _in2D;
    float _storedZBias;
    D3DVECTOR _storedLightDirection;
    IDirect3DVertexBuffer9* _currentstreamsource;
    std::vector<LPDIRECT3DTEXTURE9> _loadedImages;
    std::vector<D3DXIMAGE_INFO> _imageInfos;

    void setAmbient();

public:

    static cD3D& refInstance();

    static LPDIRECT3DDEVICE9 device()
    {
        return refInstance()._pd3dDevice;
    }
    static cVertexBufferManager& refBufferManager()
    {
        return *refInstance()._vertexBufferManager;
    }

    tSigned32 getSizeX() const
    {
        return _sizeX;
    }
    tSigned32 getSizeY() const
    {
        return _sizeY;
    }

    HRESULT init(HWND hWnd, tSigned32 sizeX, tSigned32 sizeY);
	void resize(tSigned32 sizeX, tSigned32 sizeY);

    void setupMatrices();
    void cleanup();
    void render();
    void beginScene();
    void endScene();
    void present();

    void setLightColour(float r, float g, float b);
    void setAlpha(float a);
    float getCurrentAlpha() const;

    void setRenderState(eRenderState state);
    void setLoadedImageRenderState(tSigned32 index);
    void setCamera2D(tSigned32 scale, tSigned32 x, tSigned32 y, bool zIsIgnored);
    void setCamera3D(const float* target, float angle1, float angle2, float distance);

    void setZBias(float value);
    float getZBias();

    void setStreamSource(IDirect3DVertexBuffer9* source, UINT size);
    void clearStreamSource();

    void into2D();
    void outOf2D();

    tSigned32 loadImage(const char* fileName);
    tSigned32 getImageWidth(tSigned32 index) const
    {
        return _imageInfos[index].Width;
    }
    tSigned32 getImageHeight(tSigned32 index) const
    {
        return _imageInfos[index].Height;
    }
};
