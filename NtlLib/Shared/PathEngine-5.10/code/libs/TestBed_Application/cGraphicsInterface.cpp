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
#include "cGraphicsInterface.h"
#include "common/interface/Assert.h"
#include "Colours.h"
#include "D3D.h"
#include "VertexTypes.h"
#include "VertexBufferManager.h"
#include "RenderGeometry.h"
#include <list>

cGraphicsInterface::cGraphicsInterface()
{
}

cGraphicsInterface&
cGraphicsInterface::refInstance()
{
    static cGraphicsInterface theInterface;
    return theInterface;
}

void
cGraphicsInterface::endFrame()
{
    cActionContext context;
    size_t i;
    for(i = 0; i < _actions.size(); i++)
    {
        _actions[i]->perform(context);
    }
    context._renderBlended = true;
    for(i = 0; i < _actions.size(); i++)
    {
        _actions[i]->perform(context);
    }
    for(i = 0; i < _actions.size(); i++)
    {
        if(!_actions[i]->isRenderGeometry())
        {
            delete _actions[i];
        }
    }
    _actions.clear();

    while(!_frameHeld.empty())
    {
        delete _frameHeld.front();
        _frameHeld.pop_front();
    }
}

class cSetCameraAction : public iAction
{
    float _target[3];
    float _angle1, _angle2, _distance;

public:

    cSetCameraAction(const float* target, float angle1, float angle2, float distance)
    {
        _target[0] = target[0];
        _target[1] = target[1];
        _target[2] = target[2];
        _angle1 = angle1;
        _angle2 = angle2;
        _distance = distance;
    }
    void perform(cActionContext& context)
    {
        cD3D::refInstance().setCamera3D(_target, _angle1, _angle2, _distance);
    }
};

void
cGraphicsInterface::setCamera(const float* target, float angle1, float angle2, float distance)
{
    _actions.push_back(new cSetCameraAction(target, angle1, angle2, distance));
}

void
cGraphicsInterface::setColour(const char* colourString)
{
    float r, g, b;
    RGBForString(colourString, &r, &g, &b);
    setColour_RGB(r, g, b);
}

class cSetColourAction : public iAction
{
    float r, g, b;
public:
    cSetColourAction(float r, float g, float b) :
     r(r), g(g), b(b)
    {
    }
    void perform(cActionContext& context)
    {
        cD3D::refInstance().setLightColour(r, g, b);
    }
};

void
cGraphicsInterface::setColour_RGB(float r, float g, float b)
{
    _actions.push_back(new cSetColourAction(r, g, b));
}

class cSetTransparencyAction : public iAction
{
    float a;
public:
    cSetTransparencyAction(float a) :
     a(a)
    {
    }
    void perform(cActionContext& context)
    {
        cD3D::refInstance().setAlpha(a);
        context._blendingOn = (a != 1.0f);
    }
};

class cSetAdditiveAction : public iAction
{
    float a;
public:
    cSetAdditiveAction(float a) :
     a(a)
    {
    }
    void perform(cActionContext& context)
    {
        cD3D::refInstance().setAlpha(a);
        context._blendingOn = true;
    }
};
void
cGraphicsInterface::setAdditiveBlending(float alpha)
{
    _actions.push_back(new cSetAdditiveAction(alpha));
}

class cSetOpaqueAction : public iAction
{
public:
    void perform(cActionContext& context)
    {
        cD3D::refInstance().setAlpha(1.0f);
        context._blendingOn = false;
    }
};
void
cGraphicsInterface::setOpaque()
{
    _actions.push_back(new cSetOpaqueAction());
}

class cSetZBiasAction : public iAction
{
    float value;
public:
    cSetZBiasAction(float value) :
     value(value)
    {
    }
    void perform(cActionContext& context)
    {
        cD3D::refInstance().setZBias(value);
    }
};

void
cGraphicsInterface::setZBias(float value)
{
    _actions.push_back(new cSetZBiasAction(value));
}

class cGeometrySet
{
public:

    std::list<iRenderGeometry*> _held;
};

cGeometrySet*
cGraphicsInterface::newGeometrySet()
{
    return new cGeometrySet;
}

iRenderGeometry*
cGraphicsInterface::newLine(const float* pointarray, cGeometrySet* set)
{
    iRenderGeometry* result = new cLine(pointarray);
    if(set)
    {
        set->_held.push_back(result);
    }
    else
    {
        _frameHeld.push_back(result);
    }
    return result;
}
iRenderGeometry*
cGraphicsInterface::newPolygon(int points, const float* pointarray, cGeometrySet* set)
{
    iRenderGeometry* result = new cPolygon(points, pointarray);
    if(set)
    {
        set->_held.push_back(result);
    }
    else
    {
        _frameHeld.push_back(result);
    }
    return result;
}
iRenderGeometry*
cGraphicsInterface::newText_8By16(tSigned32 x, tSigned32 y, const char* str, cGeometrySet* set)
{
    iRenderGeometry* result = new cText(x, y, str);
    if(set)
    {
        set->_held.push_back(result);
    }
    else
    {
        _frameHeld.push_back(result);
    }
    return result;
}

tSigned32
cGraphicsInterface::loadImage(const char* fileName)
{
    return cD3D::refInstance().loadImage(fileName);
}

iRenderGeometry*
cGraphicsInterface::newSplashImage(tSigned32 x, tSigned32 y, tSigned32 loadedImageIndex, cGeometrySet* set)
{
    tSigned32 width, height;
    width = cD3D::refInstance().getImageWidth(loadedImageIndex);
    height = cD3D::refInstance().getImageHeight(loadedImageIndex);
    iRenderGeometry* result = new cSplashImage(x, y, width, height, loadedImageIndex);
    if(set)
    {
        set->_held.push_back(result);
    }
    else
    {
        _frameHeld.push_back(result);
    }
    return result;
}
iRenderGeometry*
cGraphicsInterface::newSplashImage(tSigned32 loadedImageIndex, cGeometrySet* set)
{
    tSigned32 x, y, width, height;
    width = cD3D::refInstance().getImageWidth(loadedImageIndex);
    height = cD3D::refInstance().getImageHeight(loadedImageIndex);
    x = (cD3D::refInstance().getSizeX() - width) / 2;
    y = (cD3D::refInstance().getSizeY() - height) / 2;
    iRenderGeometry* result = new cSplashImage(x, y, width, height, loadedImageIndex);
    if(set)
    {
        set->_held.push_back(result);
    }
    else
    {
        _frameHeld.push_back(result);
    }
    return result;
}

void
cGraphicsInterface::deleteGeometrySet(cGeometrySet* set)
{
    if(set)
    {
        _frameHeld.splice(_frameHeld.end(), set->_held);
    }
    delete set;
}

void
cGraphicsInterface::render(iRenderGeometry* element)
{
//    element->render();
    _actions.push_back(element);
}

class cInto2DAction : public iAction
{
public:
    void perform(cActionContext& context)
    {
        cD3D::refInstance().into2D();
    }
};
class cOutOf2DAction : public iAction
{
public:
    void perform(cActionContext& context)
    {
        cD3D::refInstance().outOf2D();
    }
};

void
cGraphicsInterface::render_ScreenSpace(iRenderGeometry* element)
{
    _actions.push_back(new cInto2DAction());
    _actions.push_back(element);
    _actions.push_back(new cOutOf2DAction());
}
