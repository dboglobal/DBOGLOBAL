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
#include "SourceDataLookup.h"
#include "common/interface/Assert.h"
#include "i_pathengine.h"


cSourceDataLookup::cSourceDataLookup(
        const iFaceVertexMesh *const * meshObjectPointers,
        const iSolidObjects* solidObjects,
        std::vector<tSigned32> meshAndFaceLookup
        ) :
    _meshObjectPointers(meshObjectPointers),
    _solidObjects(solidObjects)
{
    _meshAndFaceLookup.swap(meshAndFaceLookup);
}

tSigned32
cSourceDataLookup::faceAttribute(tSigned32 operandIndex, tSigned32 attribute) const
{
    assertD(operandIndex != -1); // should only ever be called for actual ground geometry
    if(operandIndex < SizeL(_meshAndFaceLookup) / 2)
    {
      // this face comes from face vertex source geometry
        tSigned32 meshIndex, faceInMesh;
        meshIndex = _meshAndFaceLookup[operandIndex * 2];
        faceInMesh = _meshAndFaceLookup[operandIndex * 2 + 1];
        return _meshObjectPointers[meshIndex]->faceAttribute(faceInMesh, attribute);
    }
    // this face comes from solid object source data
    assertD(_solidObjects);
    operandIndex -= _solidObjects->numberOfConvexSolids();
    return _solidObjects->convexSolid_Attribute(operandIndex, attribute);
}
bool
cSourceDataLookup::faceAttributesMatch(tSigned32 operandIndex1, tSigned32 operandIndex2) const
{
    tSigned32 j;
    for(j = 0; j <= PE_FaceAttribute_Top; ++j)
    {
        if(faceAttribute(operandIndex1, j) != faceAttribute(operandIndex2, j))
        {
            return false;
        }
    }
    return true;
}

bool
cSourceDataLookup::operator()(tFace_Dynamic f1, tFace_Dynamic f2)
{
    return faceAttributesMatch(f1->_operandIndex, f2->_operandIndex);
}
