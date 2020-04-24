//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/BuildMeshFrom3DContent.h"
#include "BuildSolidObjects.h"
#include "Extrude.h"
#include "BSP3D.h"
#include "OperandsFromFaceVertex.h"
#include "BSPStats.h"
#include "BSPVertices.h"
#include "Approximate.h"
#include "ConvertToMesh3D.h"
#include "ValidateVolume.h"
#include "SourceDataLookup.h"
#include "RemoveFragments.h"
#include "libs/PathEngine_Interface/interface/cPathEngine.h"
#include "libs/PathEngine_Interface/interface/Singletons.h"
#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "common/interface/Error.h"
#include "platform_common/CoordinateRanges.h"

iMesh*
BuildMeshFrom3DContent(
        const iFaceVertexMesh *const * meshObjectPointers,
        tSigned32 numberOfMeshObjects,
        const iSolidObjects* solidObjects,
        tSigned32 extrudeHeight,
        tSigned32 maxStepHeight,
        tSigned32 maxSlope,
        const char *const* options,
        iProgressCallBack* progressCallBack
        )
{
    INTERFACE_ASSERT(extrudeHeight >= 1);
    if(extrudeHeight < 1)
    {
        extrudeHeight = 1;
    }
    if(extrudeHeight > 8191)
    {
        const char *const attributes[] =
        {
            "supportedRange", "8191",
            0
        };
        Error("NonFatal", "BuildMeshFrom3DContent() - The extrude height specified is outside the supported range and will be reduced to fit this range.", attributes);
        extrudeHeight = 8191;
    }
    tPoint3 extrudeOffset(0, 0, -extrudeHeight, 8191);

    INTERFACE_ASSERT(numberOfMeshObjects >= 0);
    if(numberOfMeshObjects < 0)
    {
        numberOfMeshObjects = 0;
    }

    std::vector<tMesh_Simple3D*> operands;
    std::vector<tSigned32> meshAndFaceLookup;
    tSigned32 i;
    for(i = 0; i != numberOfMeshObjects; ++i)
    {
        ExtrudedOperandsFromFaceVertex(*meshObjectPointers[i], extrudeOffset, i, operands, meshAndFaceLookup);
    }
//    for(i = 0; i != SizeL(operands); ++i)
//    {
//        ValidateVolume(*operands[i]);
//    }

    if(solidObjects)
    {
        tSigned32 firstToExtrude = SizeL(operands);
        BuildSolidObjects(*solidObjects, operands);
        for(i = firstToExtrude; i != SizeL(operands); ++i)
        {
            assertD(operands[i]->faces() >= 4);
            Extrude(*operands[i], extrudeHeight);
            assertD(operands[i]->faces() >= 4);
            operands[i]->consolidate();
        }
    }

    cSourceDataLookup sourceDataLookup(meshObjectPointers, solidObjects, meshAndFaceLookup);

    cDynamicFraction maxSlopeFraction(maxSlope);
    maxSlopeFraction /= 100;
    cBSPStats stats;
    cBSPVertices resultVertices;
    tMesh_Dynamic resultDynamic;
    CombineOperandsAndExtractGround(operands, &sourceDataLookup, resultDynamic, resultVertices, stats, maxStepHeight, maxSlopeFraction, sourceDataLookup, options, progressCallBack);

    for(i = 0; i != SizeL(operands); ++i)
    {
        delete operands[i];
    }
    operands.clear();

//    {
//        const char* value = GetValueForAttribute(options, "removeFragmentsWithThreshold");
//        if(value)
//        {
//            RemoveFragments(resultDynamic, resultVertices, ValueAsLong(value));
//        }
//    }

    if(resultDynamic.empty())
    {
        return 0;
    }

    tMesh_3D resultMesh3D;

    ConvertToMesh3D(resultVertices, resultDynamic, resultMesh3D);

    std::vector<tSigned32>* surfaceTypes = 0;
    std::vector<tSigned32>* sectionIDs = 0;
    std::vector<tSigned32>* userData = 0;

    assertD(resultDynamic.faceSize() == resultDynamic.faces());
    for(i = 0; i < resultDynamic.faces(); ++i)
    {
        tSigned32 operandIndex = resultDynamic.faceAt(i)->_operandIndex;
        assertD(operandIndex != -1);

        tSigned32 surfaceType = sourceDataLookup.faceAttribute(operandIndex, PE_FaceAttribute_SurfaceType);
        if(surfaceType != 0)
        {
            if(!surfaceTypes)
            {
                surfaceTypes = new std::vector<tSigned32>(resultDynamic.faces(), 0);
            }
            (*surfaceTypes)[i] = surfaceType;
        }
        tSigned32 sectionID = sourceDataLookup.faceAttribute(operandIndex, PE_FaceAttribute_SectionID);
        if(sectionID != -1)
        {
            if(!sectionIDs)
            {
                sectionIDs = new std::vector<tSigned32>(resultDynamic.faces(), -1);
            }
            (*sectionIDs)[i] = sectionID;
        }
        tSigned32 userDataValue = sourceDataLookup.faceAttribute(operandIndex, PE_FaceAttribute_UserData);
        if(userDataValue != 0)
        {
            if(!userData)
            {
                userData = new std::vector<tSigned32>(resultDynamic.faces(), 0);
            }
            (*userData)[i] = userDataValue;
        }
    }

    iMesh* result = nSingletons::pathEngine().constructMeshFrom3D(
            resultMesh3D,
            surfaceTypes, sectionIDs, userData,
            options
            );
    return result;
}
