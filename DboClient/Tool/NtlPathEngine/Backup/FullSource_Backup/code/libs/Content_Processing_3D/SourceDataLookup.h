//**********************************************************************
//
// Copyright (c) 2007
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef CONTENT_PROCESSING_3D__SOURCE_DATA_LOOKUP_INCLUDED
#define CONTENT_PROCESSING_3D__SOURCE_DATA_LOOKUP_INCLUDED

#include "iFacesCanBeCombinedCallback.h"
#include "common/STL/vector.h"

class iFaceVertexMesh;
class iSolidObjects;

class cSourceDataLookup : public iFacesCanBeCombinedCallback
{
    const iFaceVertexMesh *const * _meshObjectPointers;
    const iSolidObjects* _solidObjects;
    std::vector<tSigned32> _meshAndFaceLookup;

public:

    // interface object pointers must remain valid for the lifetime of this object
    // meshAndFaceLookup will be swapped in
    cSourceDataLookup(
            const iFaceVertexMesh *const * meshObjectPointers,
            const iSolidObjects* solidObjects,
            std::vector<tSigned32> meshAndFaceLookup
            );

    tSigned32 faceAttribute(tSigned32 operandIndex, tSigned32 attribute) const;
    bool faceAttributesMatch(tSigned32 operandIndex1, tSigned32 operandIndex2) const;

// interface for iFacesCanBeCombinedCallback

    bool operator()(tFace_Dynamic f1, tFace_Dynamic f2);
};

#endif
