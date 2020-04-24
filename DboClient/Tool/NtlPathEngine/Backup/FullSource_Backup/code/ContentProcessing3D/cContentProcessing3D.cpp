//**********************************************************************
//
// Copyright (c) 2007
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "cContentProcessing3D.h"
#include "libs/Content_Processing_3D/interface/BuildMeshFrom3DContent.h"
#include "libs/Content_Processing_3D/interface/Save3DContentSnapshot.h"
#include "libs/PathEngine_Core/interface/Version.h"
#include "common/MemoryTracking_Interface.h"
#include "common/MutexLock.h"
#include "common/AttributesBuilder.h"
#include "i_pathengine.h"

const char *const*
cContentProcessing3D::getVersionAttributes() const
{
    cMutexLock lock(_mutex);
    static bool firstTime = true;
    static const char *const* attributes;
    if(firstTime)
    {
        static cAttributesBuilder builder;
        firstTime = false;
        builder.add("buildstring", VersionTimeString());
        builder.add("release", MajorRelease());
        builder.add("minor_release", MinorRelease());
        builder.add("interface_major_version", CONTENT_PROCESSING_3D_INTERFACE_MAJOR_VERSION);
        builder.add("interface_minor_version", CONTENT_PROCESSING_3D_INTERFACE_MINOR_VERSION);
        if(MemoryTrackingIsEnabled())
        {
            builder.add("memory_tracking_enabled", "true");
        }
#ifdef ASSERTIONS_ON
        builder.add("assertions_on", "true");
#endif
#ifdef RANGE_CHECKING
        builder.add("range_checking", "true");
#endif
#ifdef PATHENGINE_FULL_RELEASE
        builder.add("full_release", "true");
#endif
#ifdef _DEBUG
        builder.add("debug_build", "true");
#endif
        attributes = builder.get();
    }
    return attributes;
}
tSigned32
cContentProcessing3D::getInterfaceMajorVersion() const
{
    return CONTENT_PROCESSING_3D_INTERFACE_MAJOR_VERSION;
}
tSigned32
cContentProcessing3D::getInterfaceMinorVersion() const
{
    return CONTENT_PROCESSING_3D_INTERFACE_MINOR_VERSION;
}

void
cContentProcessing3D::buildAndSaveGround(
        const iFaceVertexMesh *const * meshObjectPointers,
        tSigned32 numberOfMeshObjects,
        const iSolidObjects* solidObjects,
        tSigned32 extrudeHeight,
        tSigned32 maxStepHeight,
        tSigned32 maxSlope,
        const char *const* options,
        iProgressCallBack* progressCallBack,
        const char* format, bool includeMapping, iOutputStream* outputStream
        ) const
{
    iMesh* result = BuildMeshFrom3DContent(
            meshObjectPointers, numberOfMeshObjects,
            solidObjects,
            extrudeHeight,
            maxStepHeight,
            maxSlope,
            options,
            progressCallBack
            );
    if(result)
    {
        result->saveGround(format, includeMapping, outputStream);
        delete result;
    }
}
void
cContentProcessing3D::saveSnapshot(
        const iFaceVertexMesh *const * meshObjectPointers,
        tSigned32 numberOfMeshObjects,
        const iSolidObjects* solidObjects,
        tSigned32 extrudeHeight,
        tSigned32 maxStepHeight,
        tSigned32 maxSlope,
        const char *const* options,
        const char* format, iOutputStream* outputStream
        ) const
{
    Save3DContentSnapshot(
            meshObjectPointers, numberOfMeshObjects,
            solidObjects,
            extrudeHeight, maxStepHeight, maxSlope, options,
            format, outputStream
            );
}
