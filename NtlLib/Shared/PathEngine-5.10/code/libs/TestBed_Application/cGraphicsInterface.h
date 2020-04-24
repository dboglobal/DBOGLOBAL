//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "interface/iGraphicsInterface.h"
#include "Point3.h"
#include <list>
#include <vector>

class iRenderGeometry;
class iAction;

class cGraphicsInterface : public iGraphicsInterface
{
    std::list<iRenderGeometry*> _frameHeld;
    std::vector<iAction*> _actions;

    cGraphicsInterface();

public:

    static cGraphicsInterface& refInstance();
    void endFrame();

// interface

    void setCamera(const float* target, float angle1, float angle2, float distance);

    void setColour(const char* colourstring);
    void setColour_RGB(float r, float g, float b);
    void setAdditiveBlending(float alpha);
    void setOpaque();
    void setZBias(float value);

    cGeometrySet* newGeometrySet();

    iRenderGeometry* newLine(const float* pointarray, cGeometrySet* set = 0);
    iRenderGeometry* newPolygon(int points, const float* pointarray, cGeometrySet* set = 0);
    iRenderGeometry* newText_8By16(tSigned32 x, tSigned32 y, const char* str, cGeometrySet* set = 0);
    tSigned32 loadImage(const char* fileName);
    iRenderGeometry* newSplashImage(tSigned32 x, tSigned32 y, tSigned32 loadedImageIndex, cGeometrySet* set = 0);
    iRenderGeometry* newSplashImage(tSigned32 loadedImageIndex, cGeometrySet* set = 0);

    void deleteGeometrySet(cGeometrySet* set);

    void render(iRenderGeometry*);
    void render_ScreenSpace(iRenderGeometry*);
};
