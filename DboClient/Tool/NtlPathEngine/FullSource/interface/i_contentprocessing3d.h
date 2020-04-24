//**********************************************************************
//
// Copyright (c) 2007
// Thomas Young, PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef CONTENT_PROCESSING_3D_INTERFACE_HAS_BEEN_INCLUDED
#define CONTENT_PROCESSING_3D_INTERFACE_HAS_BEEN_INCLUDED

#if defined(__GNUC__) && defined(__x86_64__)
typedef int tSigned32;
typedef unsigned int tUnsigned32;
#else
typedef long tSigned32;
typedef unsigned long tUnsigned32;
#endif

const tSigned32 CONTENT_PROCESSING_3D_INTERFACE_MAJOR_VERSION = 2;
const tSigned32 CONTENT_PROCESSING_3D_INTERFACE_MINOR_VERSION = 0;

class iFaceVertexMesh;
class iSolidObjects;
class iProgressCallBack;
class iOutputStream;

class iContentProcessing3D
{
public:

    virtual const char *const* getVersionAttributes() const=0;
    virtual tSigned32 getInterfaceMajorVersion() const=0;
    virtual tSigned32 getInterfaceMinorVersion() const=0;

    virtual void buildAndSaveGround(
            const iFaceVertexMesh *const * meshObjectPointers,
            tSigned32 numberOfMeshObjects,
            const iSolidObjects* solidObjects,
            tSigned32 extrudeHeight,
            tSigned32 maxStepHeight,
            tSigned32 maxSlope,
            const char *const* options,
            iProgressCallBack* progressCallBack,
            const char* format, bool includeMapping, iOutputStream* outputStream
            ) const = 0;
    virtual void saveSnapshot(
            const iFaceVertexMesh *const * meshObjectPointers,
            tSigned32 numberOfMeshObjects,
            const iSolidObjects* solidObjects,
            tSigned32 extrudeHeight,
            tSigned32 maxStepHeight,
            tSigned32 maxSlope,
            const char *const* options,
            const char* format, iOutputStream* outputStream
            ) const = 0;
};

#endif // Include guard
