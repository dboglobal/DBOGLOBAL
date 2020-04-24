//**********************************************************************
//
// Copyright (c) 2007
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Mutex.h"
#include "i_contentprocessing3d.h"

class cContentProcessing3D : public iContentProcessing3D
{
    cMutex _mutex;

public:

    const char *const* getVersionAttributes() const;
    tSigned32 getInterfaceMajorVersion() const;
    tSigned32 getInterfaceMinorVersion() const;

    void buildAndSaveGround(
            const iFaceVertexMesh *const * meshObjectPointers,
            tSigned32 numberOfMeshObjects,
            const iSolidObjects* solidObjects,
            tSigned32 extrudeHeight,
            tSigned32 maxStepHeight,
            tSigned32 maxSlope,
            const char *const* options,
            iProgressCallBack* progressCallBack,
            const char* format, bool includeMapping, iOutputStream* outputStream
            ) const;
    void saveSnapshot(
            const iFaceVertexMesh *const * meshObjectPointers,
            tSigned32 numberOfMeshObjects,
            const iSolidObjects* solidObjects,
            tSigned32 extrudeHeight,
            tSigned32 maxStepHeight,
            tSigned32 maxSlope,
            const char *const* options,
            const char* format, iOutputStream* outputStream
            ) const;
};
