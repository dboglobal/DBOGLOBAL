//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef I_GRAPHICS_INTERFACE_INCLUDED
#define I_GRAPHICS_INTERFACE_INCLUDED

class iRenderGeometry;
class cGeometrySet;

class iGraphicsInterface
{
public:

    virtual ~iGraphicsInterface() {}

    virtual void setCamera(const float* target, float angle1, float angle2, float distance) = 0;

    virtual void setColour(const char* colourstring) = 0;
    virtual void setColour_RGB(float r, float g, float b) = 0;
    virtual void setAdditiveBlending(float alpha) = 0;
    virtual void setOpaque() = 0;
    virtual void setZBias(float value) = 0;

    virtual cGeometrySet* newGeometrySet() = 0;

    virtual iRenderGeometry* newLine(const float* pointarray, cGeometrySet* set = 0) = 0;
    virtual iRenderGeometry* newPolygon(int points, const float* pointarray, cGeometrySet* set = 0) = 0;
    virtual iRenderGeometry* newText_8By16(tSigned32 x, tSigned32 y, const char* str, cGeometrySet* set = 0)=0;
    virtual tSigned32 loadImage(const char* fileName)=0;
    virtual iRenderGeometry* newSplashImage(tSigned32 x, tSigned32 y, tSigned32 loadedImageIndex, cGeometrySet* set = 0)=0;
    virtual iRenderGeometry* newSplashImage(tSigned32 loadedImageIndex, cGeometrySet* set = 0)=0;

    virtual void deleteGeometrySet(cGeometrySet* set) = 0;

    virtual void render(iRenderGeometry*) = 0;
    virtual void render_ScreenSpace(iRenderGeometry*)=0;
};

#endif
