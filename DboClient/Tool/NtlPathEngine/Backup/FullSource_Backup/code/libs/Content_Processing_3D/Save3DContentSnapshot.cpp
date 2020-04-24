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
#include "interface/Save3DContentSnapshot.h"
#include "libs/Content_Processing/interface/SaveFaceVertexMesh.h"
#include "libs/PathEngine_Core/interface/Version.h"
#include "libs/ParseXML/interface/StandardXMLOutputStream.h"
#include "libs/ParseXML/interface/TokenisedXMLOutputStream.h"
#include "common/interface/iXMLOutputStream.h"
#include "common/CLib/string.h"
#include "i_pathengine.h"

static void
SaveSolidObjects(const iSolidObjects& solidObjects, iXMLOutputStream& xos)
{
    tSigned32 i;
    for(i = 0; i != solidObjects.numberOfConvexSolids(); ++i)
    {
        xos.openElement("convexSolid");
        tSigned32 j;
        for(j = 0; j != solidObjects.convexSolid_Points(i); ++j)
        {
            tSigned32 x, y, z;
            solidObjects.convexSolid_Point(i, j, x, y, z);
            xos.openElement("point");        
            xos.addAttribute("x", x);
            xos.addAttribute("y", y);
            xos.addAttribute("z", z);
            xos.closeElement("point");        
        }
        xos.closeElement("convexSolid");
    }
}

static void
DoSave(const iFaceVertexMesh *const * meshObjectPointers,
        tSigned32 numberOfMeshObjects,
        const iSolidObjects* solidObjects,
        tSigned32 extrudeHeight,
        tSigned32 maxStepHeight,
        tSigned32 maxSlope,
        const char *const* options,
        iXMLOutputStream& xos
        )
{
    xos.openElement("content");
    xos.addAttribute("majorRelease", MajorRelease());
    xos.addAttribute("minorRelease", MinorRelease());
    xos.addAttribute("fileType", "3DContent");
    xos.addAttribute("extrudeHeight", extrudeHeight);
    xos.addAttribute("maxStepHeight", maxStepHeight);
    xos.addAttribute("maxSlope", maxSlope);
    xos.openElement("options");
    xos.addAttributes(options);
    xos.closeElement("options");
    tSigned32 i;
    for(i = 0; i != numberOfMeshObjects; ++i)
    {
        xos.openElement("mesh");
        SaveFaceVertexMesh(*meshObjectPointers[i], xos);
        xos.closeElement("mesh");
    }
    if(solidObjects)
    {
        xos.openElement("solidObjects");
        SaveSolidObjects(*solidObjects, xos);
        xos.closeElement("solidObjects");
    }
    xos.closeElement("content");
}

void
Save3DContentSnapshot(
        const iFaceVertexMesh *const * meshObjectPointers,
        tSigned32 numberOfMeshObjects,
        const iSolidObjects* solidObjects,
        tSigned32 extrudeHeight,
        tSigned32 maxStepHeight,
        tSigned32 maxSlope,
        const char *const* options,
        const char* format,
        iOutputStream* os
        )
{
    if(strcmp(format, "xml") == 0)
    {
        cStandardXMLOutputStream xos(*os);
        DoSave(meshObjectPointers, numberOfMeshObjects, solidObjects, extrudeHeight, maxStepHeight, maxSlope, options, xos);
    }
    else
    {
        INTERFACE_ASSERT(strcmp(format, "tok") == 0);
        cElementAndAttributeEnumerator enumerator;
        DoSave(meshObjectPointers, numberOfMeshObjects, solidObjects, extrudeHeight, maxStepHeight, maxSlope, options, enumerator);
        enumerator.writeTo(*os);
        cTokenisedXMLOutputStream xos(enumerator, *os);
        DoSave(meshObjectPointers, numberOfMeshObjects, solidObjects, extrudeHeight, maxStepHeight, maxSlope, options, xos);
    }
}
