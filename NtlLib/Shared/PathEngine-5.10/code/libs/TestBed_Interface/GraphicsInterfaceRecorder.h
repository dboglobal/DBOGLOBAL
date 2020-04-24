//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef GRAPHICS_INTERFACE_RECORDER_INCLUDED
#define GRAPHICS_INTERFACE_RECORDER_INCLUDED

#include "libs/TestBed_Application/interface/iGraphicsInterface.h"
#include "common/interface/Assert.h"
#include "common/STL/vector.h"

class iRenderGeometry;
class cGeometrySet;

class cGraphicsInterfaceRecorder : public iGraphicsInterface
{
    iGraphicsInterface* _base;
    std::vector<iRenderGeometry*> _vector;
    cGeometrySet* _set;

public:

    cGraphicsInterfaceRecorder(iGraphicsInterface *base)
    {
        _base=base;
        _set = base->newGeometrySet();
    }
    ~cGraphicsInterfaceRecorder()
    {
        _base->deleteGeometrySet(_set);
    }

    void clear()
    {
        _base->deleteGeometrySet(_set);
        _vector.clear();
        _set = _base->newGeometrySet();
    }

    void play()
    {
        tSigned32 i;
        for(i = 0; i < SizeL(_vector); i++)
        {
            _base->render(_vector[i]);
        }
    }

// iGraphicsInterface

    void setCamera(const float* target, float angle1, float angle2, float distance)
    {
        invalid();
    }
    void setColour(const char* colourstring)
    {
        invalid();
    }
    void setColour_RGB(float r, float g, float b)
    {
        invalid();
    }
    void setAdditiveBlending(float value)
    {
        invalid();
    }
    void setOpaque()
    {
        invalid();
    }
    void setZBias(float value)
    {
        invalid();
    }
    void getZBias(float* result)
    {
        invalid();
    }

    cGeometrySet* newGeometrySet()
    {
        invalid();
        return 0;
    }

    iRenderGeometry* newLine(const float* pointArray, cGeometrySet* set)
    {
        assertD(set == 0);
        _vector.push_back(_base->newLine(pointArray, _set));
        return 0;
    }
    iRenderGeometry* newPolygon(int points, const float* pointArray, cGeometrySet* set)
    {
        assertD(set == 0);
        _vector.push_back(_base->newPolygon(points, pointArray, _set));
        return 0;
    }
    iRenderGeometry* newText_8By16(tSigned32 x, tSigned32 y, const char* str, cGeometrySet* set)
    {
        assertD(set == 0);
        _vector.push_back(_base->newText_8By16(x, y, str, _set));
        return 0;
    }
    tSigned32 loadImage(const char* fileName)
    {
        invalid();
        return -1;
    }
    iRenderGeometry* newSplashImage(tSigned32 x, tSigned32 y, tSigned32 loadedImageIndex, cGeometrySet* set)
    {
        assertD(set == 0);
        _vector.push_back(_base->newSplashImage(x, y, loadedImageIndex, _set));
        return 0;
    }
    iRenderGeometry* newSplashImage(tSigned32 loadedImageIndex, cGeometrySet* set)
    {
        assertD(set == 0);
        _vector.push_back(_base->newSplashImage(loadedImageIndex, _set));
        return 0;
    }

    void deleteGeometrySet(cGeometrySet* set)
    {
        invalid();
    }

    void render(iRenderGeometry*)
    {
    }
    void render_ScreenSpace(iRenderGeometry*)
    {
        invalid();
    }
};

#endif
