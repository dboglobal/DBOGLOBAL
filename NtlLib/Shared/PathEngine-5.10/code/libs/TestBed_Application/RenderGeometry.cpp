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
#include "RenderGeometry.h"
#include "D3D.h"
#include "VertexTypes.h"
#include "VertexBufferManager.h"
#include "Point3.h"

cLine::cLine(const float* pointArray)
{
    _indexInManagedBuffer = cD3D::refBufferManager().allocateLine();
    tSigned32 bytePositionInVertexBuffer;
    if(_indexInManagedBuffer == -1)
    {
        HRESULT result = cD3D::device()->CreateVertexBuffer(
            sizeof(LINEVERTEX) * 2,
            D3DUSAGE_WRITEONLY,
            D3DFVF_LINEVERTEX,
            D3DPOOL_DEFAULT,
            &_vertexBuffer,
			0
			);
        if(result != D3D_OK)
        {
            assertD(result == E_OUTOFMEMORY);
            _vertexBuffer = 0;
            return;
        }
        bytePositionInVertexBuffer = 0;
        _vertexIndexInVertexBuffer = 0;
    }
    else
    {
        _vertexBuffer = cD3D::refBufferManager().getLinesBuffer();
        bytePositionInVertexBuffer = _indexInManagedBuffer * sizeof(LINEVERTEX) * 2;
        _vertexIndexInVertexBuffer = _indexInManagedBuffer * 2;
    }
    void* ptr;
    {
        HRESULT result=
            _vertexBuffer->Lock(
                bytePositionInVertexBuffer,
                sizeof(LINEVERTEX) * 2,
                &ptr,
                0); //... set some flags here
        assertD(result == D3D_OK);
    }
    LINEVERTEX* data = (LINEVERTEX*)ptr;

    data[0].position.x = pointArray[0];
    data[0].position.y = pointArray[1];
    data[0].position.z = -pointArray[2];

    data[1].position.x = pointArray[3];
    data[1].position.y = pointArray[4];
    data[1].position.z = -pointArray[5];

    {
        HRESULT result = _vertexBuffer->Unlock();
        assertD(result == D3D_OK);
    }
}

cLine::~cLine()
{
    if(_indexInManagedBuffer == -1)
    {
        if(_vertexBuffer)
        {
            _vertexBuffer->Release();
        }
    }
    else
    {
        cD3D::refBufferManager().freeLine(_indexInManagedBuffer);
    }
}

void
cLine::render(bool blended)
{
    if(!_vertexBuffer)
    {
        return;
    }
    cD3D::refInstance().setRenderState(RS_LINE);
    cD3D::refInstance().setStreamSource(_vertexBuffer, sizeof(LINEVERTEX));
    cD3D::device()->DrawPrimitive(D3DPT_LINELIST, _vertexIndexInVertexBuffer, 1);
}

cPolygon::cPolygon(tSigned32 points, const float* pointArray)
{
    assertD(points > 2);
    _points = points;

    _indexInManagedBuffer = -1;
    if(_points == 3)
    {
        _indexInManagedBuffer = cD3D::refBufferManager().allocateTri();
    }

    tSigned32 bytePositionInVertexBuffer;
    if(_indexInManagedBuffer == -1)
    {
        HRESULT result=
            cD3D::device()->CreateVertexBuffer(
            sizeof(POLYGONVERTEX) * _points,
            D3DUSAGE_WRITEONLY,
            D3DFVF_POLYGONVERTEX,
            D3DPOOL_DEFAULT,
            &_vertexBuffer,
			0
			);

        if(result != D3D_OK)
        {
            assertD(result == E_OUTOFMEMORY);
            _vertexBuffer = 0;
            return;
        }

        bytePositionInVertexBuffer = 0;
        _vertexIndexInVertexBuffer = 0;
    }
    else
    {
        _vertexBuffer = cD3D::refBufferManager().getTrisBuffer();
        bytePositionInVertexBuffer = _indexInManagedBuffer * sizeof(POLYGONVERTEX) * 3;
        _vertexIndexInVertexBuffer = _indexInManagedBuffer * 3;
    }

    cPoint3 origin(pointArray);

    tSigned32 endIndex = 1;
    float distance = (cPoint3(pointArray + 3) - origin).lengthSquared();
    tSigned32 i;
    for(i = 2; i < points; i++)
    {
        float candidateDistance = (cPoint3(pointArray + i * 3) - origin).lengthSquared();
        if(candidateDistance > distance)
        {
            distance = candidateDistance;
            endIndex = i;
        }
    }
    cPoint3 component0 = cPoint3(pointArray + endIndex * 3) - origin;

    cPoint3 component1;
    component1.clear();
    for(i = 1; i < endIndex; i++)
    {
        component1 += cPoint3(pointArray + i * 3) - origin;
    }
    for(i = endIndex + 1; i < points; i++)
    {
        component1 -= cPoint3(pointArray + i * 3) - origin;
    }
    component1.makeNormalTo(component0);

    cPoint3 normal = component0.crossProduct(component1);
    normal.setLength(1.0f);

    void* ptr;
    {
        HRESULT result = _vertexBuffer->Lock(
            bytePositionInVertexBuffer,
            sizeof(POLYGONVERTEX) * _points,
            &ptr,
            0); //... set some flags here
        assertD(result == D3D_OK);
    }

    POLYGONVERTEX* data = (POLYGONVERTEX*)ptr;

    for(i = 0; i < _points; i++)
    {
        data[i].position.x = pointArray[i * 3 + 0];
        data[i].position.y = pointArray[i * 3 + 1];
        data[i].position.z = -pointArray[i * 3 + 2];

        data[i].tu = ((float)pointArray[i * 3 + 0]) / 100;
        data[i].tv = ((float)pointArray[i * 3 + 1]) / 100;
        data[i].tu2 = ((float)pointArray[i * 3 + 0]) / 3000;
        data[i].tv2 = ((float)pointArray[i * 3 + 1]) / 3000;

        data[i].normal.x = normal.x(); 
        data[i].normal.y = normal.y(); 
        data[i].normal.z = -normal.z();
    }

    {
        HRESULT result = _vertexBuffer->Unlock();
        assertD(result == D3D_OK);
    }
}

cPolygon::~cPolygon()
{
    if(_indexInManagedBuffer == -1)
    {
        if(_vertexBuffer)
        {
            _vertexBuffer->Release();
        }
    }
    else
    {
        cD3D::refBufferManager().freeTri(_indexInManagedBuffer);
    }
}
void
cPolygon::render(bool blended)
{
    if(!_vertexBuffer)
    {
        return;
    }
    if(blended)
    {
        cD3D::refInstance().setRenderState(RS_BLENDED_POLYGON);
    }
    else
    {
        cD3D::refInstance().setRenderState(RS_POLYGON);
    }
	cD3D::refInstance().setStreamSource(_vertexBuffer, sizeof(POLYGONVERTEX));
    cD3D::device()->DrawPrimitive(D3DPT_TRIANGLEFAN, _vertexIndexInVertexBuffer, _points - 2);
}

static void
CharacterUV(char c, float &us, float &vs, float &ue, float &ve)
{
    us = static_cast<float>(c & 15) * 8;
    vs = static_cast<float>(c / 16) * 16;
    ue = us + 8;
    ve = vs + 16;
    us /= 128.0f;
    vs /= 128.0f;
    ue /= 128.0f;
    ve /= 128.0f;
}

cText::cText(tSigned32 x, tSigned32 y, const char* str) :
 _x(x),
 _y(y),
 _str(str)
{
}

void
cText::render(bool blended)
{
    TEXTVERTEX data[4];
    HRESULT result;
    cD3D::refInstance().setRenderState(RS_TEXT);

    tSigned32 x = _x;
    tSigned32 y = _y;
    const char* str = _str.c_str();

    data[0].position.y = y - 0.5f;
    data[0].position.z = 0.0f;
    data[1].position.y = y + 16 - 0.5f;
    data[1].position.z = 0.0f;
    data[2].position.y = y +16-0.5f;
    data[2].position.z = 0.0f;
    data[3].position.y = y-0.5f;
    data[3].position.z = 0.0f;

    while(*str)
    {
        tSigned32 c=*str;
        float us,vs,ue,ve;
        CharacterUV(*str,us,vs,ue,ve);
        data[0].position.x = x - 0.5f;
        data[0].tu = us;
        data[0].tv = vs;
        data[1].position.x = x - 0.5f;
        data[1].tu = us;
        data[1].tv = ve;
        data[2].position.x = x + 8 - 0.5f;
        data[2].tu = ue;
        data[2].tv = ve;
        data[3].position.x = x + 8 - 0.5f;
        data[3].tu = ue;
        data[3].tv = vs;
        result = cD3D::device()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, data, sizeof(*data));
        assertD(result == D3D_OK);
        x += 8;
        str++;
    }
    cD3D::refInstance().clearStreamSource(); //..... why is this required?
}

cSplashImage::cSplashImage(tSigned32 x, tSigned32 y, tSigned32 width, tSigned32 height, tSigned32 loadedImageIndex) :
 _x(x),
 _y(y),
 _width(width),
 _height(height),
 _loadedImageIndex(loadedImageIndex)
{
}

void
cSplashImage::render(bool blended)
{
    TEXTVERTEX data[4];

    //.. flipped vertically because the image appeared upside down
    data[0].tu = 0.f;
    data[0].tv = 1.f;
    data[1].tu = 0.f;
    data[1].tv = 0.f;
    data[2].tu = 1.f;
    data[2].tv = 0.f;
    data[3].tu = 1.f;
    data[3].tv = 1.f;

    HRESULT result;
    cD3D::refInstance().setLoadedImageRenderState(_loadedImageIndex);

    data[0].position.x = _x;
    data[0].position.y = _y;
    data[0].position.z = 0.0f;

    data[1].position.x = _x;
    data[1].position.y = _y + _height;
    data[1].position.z = 0.0f;

    data[2].position.x = _x + _width;
    data[2].position.y = _y + _height;
    data[2].position.z = 0.0f;

    data[3].position.x = _x + _width;
    data[3].position.y = _y;
    data[3].position.z = 0.0f;

    result = cD3D::device()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, data, sizeof(*data));
    assertD(result == D3D_OK);

    cD3D::refInstance().clearStreamSource(); //..... why is this required?
}
