//**********************************************************************
//
// Copyright (c) 2002-2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef C_TESTBED_INCLUDED
#define C_TESTBED_INCLUDED

#include "i_testbed.h"

#include "platform_common/LoadSaveBinary.h"
#include "common/interface/Error.h"
#include "common/AttributesBuilder.h"
#include "DrawMesh.h"
#include "CameraControl.h"
#include "Camera.h"
#include "Draw3D.h"
#include "i_pathengine.h"
#include "interface/ZoomExtents.h"
#include "libs/PathEngine_Interface/interface/cMesh.h"
#include "libs/PathEngine_Interface/interface/cShape.h"
#include "libs/Instrumentation/interface/Profiler.h"
#include "platform_common/CoordinateRanges.h"
#include "LineCollidesWithTriangle.h"
#include "common/Containers/LazyPointerVector.h"

#include <stdio.h>

class cMeshRenderInfo;
class controlinterface;
class iGraphicsInterface;
class osinterface;
class cGraphicsInterfaceRecorder;
class cAgentRenderPreprocess;
class cTestBedUserData;

class cTestBed : public iTestBed
{
    cLazyPointerVector<cGraphicsInterfaceRecorder> _expansionRecorders;
    cLazyPointerVector<cGraphicsInterfaceRecorder> _smallConvexRecorders;
    cLazyPointerVector<cAgentRenderPreprocess> _agentRenderPreprocess;

    bool _cachedBoundsToggle;
    bool _cachedBoundsValid;
    cHorizontalRange _cachedBounds;
    cGraphicsInterfaceRecorder* _cachedBoundsRecorder;
    bool _cachedBounds2Valid;
    cHorizontalRange _cachedBounds2;
    cGraphicsInterfaceRecorder* _cachedBounds2Recorder;

    controlinterface* cip;
    iGraphicsInterface* gip;
    osinterface* ip;

    cHandle<cMesh> _mesh;

    bool _pickedThisFrame;
    cPosition _pickPosition;

    tSigned32 _pickScreenX;
    tSigned32 _pickScreenY;

    bool _previousMouseValid;
    tSigned32 _previousMouseX;
    tSigned32 _previousMouseY;

    tUnsigned32 _startHigh, _startLow;
    double _clocksBuffer[20];
    int _clocksBufferPos;

    tSigned32 _currentY;

    cProfile* _profile;

    bool _cameraPositionSet;

    cTestBedUserData& meshUserData();

public:

    //... make private again
    cTestBed(osinterface *ip);
    ~cTestBed();

// iTestBed

    const char *const* getVersionAttributes() const;

    tSigned32 getInterfaceMajorVersion() const
    {
        return TESTBED_INTERFACE_MAJOR_VERSION;
    }
    tSigned32 getInterfaceMinorVersion() const
    {
        return TESTBED_INTERFACE_MINOR_VERSION;
    }

    void reportError(const char *error_type, const char *error_string)
    {
        Error(error_type,error_string);
    }

    char* loadBinary(const char* filename, tUnsigned32& buffer_size)
    {
        return LoadBinary_AllocatedBuffer(filename,buffer_size);
    }
    void freeBuffer(char *buffer)
    {
        delete [] buffer;
    }

    void setMesh(iMesh *mesh);
    void releaseMesh();

    void zoomExtents();

    const char* receiveKeyMessage();
    bool getKeyState(const char *keystring);
    bool getLeftMouseState();
    bool getRightMouseState();

    void getMouseScreenPosition(tSigned32 &screenx, tSigned32 &screeny);
    void getMouseScreenDelta(tSigned32 &screendx, tSigned32 &screendy);

    cPosition positionAtMouse();

    void setColour(const char *colour);
    void setColourRGB(float r, float g, float b);

    void drawMesh();
    void drawMeshEdges();
    void drawMeshExpansion(iShape *shape);
    void drawAgent(iAgent *agent, tSigned32 height);
    void drawShape(iShape *shape, const cPosition &position, tSigned32 height);
    void drawAgentHeading(iAgent *agent, tSigned32 size, tSigned32 height, float heading);
    void drawPath(iPath *path);
    void drawPosition(const cPosition &position, tSigned32 size);
    void drawLine(const cPosition &start, const cPosition &end);

    void drawSquareBounds(const cPosition &start, tSigned32 distance);
    void drawAgentExpansion(iAgent *agent, iShape *expandedby);

    void printText8by16(tSigned32 x, tSigned32 y, const char *text);
    void printTextLine(tSigned32 x, const char *text);
    
    void update();

    iMesh* getMesh()
    {
        return _mesh.get();
    }

    void drawBurntInObstacles();
    void drawSmallConvexExpansions(iShape *shape);

    void drawAgentsInContext(iCollisionContext* context, tSigned32 height);

    void lookAt(const cPosition& target, float angle, float elevation, float distance);
    bool meshObstructsCamera();
    void setAdditiveBlending(float alpha);
    void setOpaque();

    void drawAgentWithPrecision(iAgent* agent, tSigned32 height, float precisionX, float precisionY);
    void drawAgentHeadingWithPrecision(iAgent* agent, tSigned32 size, tSigned32 height, float heading, float precisionX, float precisionY);
    void lookAtWithPrecision(const cPosition& target, float precisionX, float precisionY, float angle, float elevation, float distance);

    tSigned32 createSound(const char* fileName, tSigned32 numberOfBuffers);
    void playSound(tSigned32 index);

    tSigned32 loadSplashImage(const char* fileName);
    void displaySplashImage(tSigned32 x, tSigned32 y, tSigned32 imageIndex);
    void displaySplashImage_Centred(tSigned32 imageIndex);

    void drawRangeBounds(const cHorizontalRange& bounds);

    void operator delete(void* p) {::operator delete(p);}

//======================

    void update_DontBeginScene();
    void update_Redraw();
    void beginScene();

    iGraphicsInterface* getGIP()
    {
        return gip;
    }
    osinterface* getIP()
    {
        return ip;
    }
    controlinterface* getCIP()
    {
        return cip;
    }

    cMesh& refMesh()
    {
        assertD(_mesh);
        return *_mesh;
    }

    bool meshHasBeenSet()
    {
        return _mesh!=0;
    }

    void draw3DMeshInternalEdges();
    void drawIntegerCrossesAround(const cPosition& position, tSigned32 radius, tSigned32 cross_size);
    void draw3DFace(tFace_3D f);
    void draw3DEdge(tEdge_3D e);


	// by Kell
	void* getWindowHandle();
};

inline cTestBed* FromI(iTestBed* ptr)
{
    return static_cast<cTestBed*>(ptr);
}

#endif
