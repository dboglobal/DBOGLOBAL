//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "iAction.h"
#include <d3dx9.h>
#include <string>

class iRenderGeometry : public iAction
{
public:

    ~iRenderGeometry() {}

    virtual void render(bool blended) = 0;
    void perform(cActionContext& context)
    {
        if(context._blendingOn == context._renderBlended)
        {
            render(context._blendingOn);
        }
    }
    bool isRenderGeometry()
    {
        return true;
    }
};

class cLine : public iRenderGeometry
{
    tSigned32 _indexInManagedBuffer;
    IDirect3DVertexBuffer9* _vertexBuffer;
    tSigned32 _vertexIndexInVertexBuffer;

public:

    cLine(const float* pointArray);
    ~cLine();
    void render(bool blended);
};


class cPolygon : public iRenderGeometry
{
    tSigned32 _points;
    tSigned32 _indexInManagedBuffer;
    IDirect3DVertexBuffer9* _vertexBuffer;
    tSigned32 _vertexIndexInVertexBuffer;

public:

    cPolygon(tSigned32 points, const float* pointArray);
    ~cPolygon();

    void render(bool blended);
};

class cText : public iRenderGeometry
{
    tSigned32 _x, _y;
    std::string _str;

public:

    cText(tSigned32 x, tSigned32 y, const char* str);
    void render(bool blended);
};

class cSplashImage : public iRenderGeometry
{
    tSigned32 _x, _y;
    tSigned32 _width, _height;
    tSigned32 _loadedImageIndex;

public:

    cSplashImage(tSigned32 x, tSigned32 y, tSigned32 width, tSigned32 height, tSigned32 loadedImageIndex);
    void render(bool blended);
};
