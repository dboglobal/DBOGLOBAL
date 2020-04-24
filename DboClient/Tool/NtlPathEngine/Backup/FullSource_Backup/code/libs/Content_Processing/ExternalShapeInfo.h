//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "common/Containers/BitVector.h"
#include "common/STL/vector.h"
#include "common/AttributesBuilder.h"

class cExternalShapeInfo
{
public:

    tMesh_3D_FaceTracked _externalShape;
    tFace_3D_FaceTracked _shapeExternalFace;
    std::vector<tEdge_3D_FaceTracked> _insidePortalEdges;
    std::vector<float> _portalResolveRanges;
    std::vector<tEdge_3D_FaceTracked> _behindPortalEdges;
    std::vector<tEdge_3D_FaceTracked> _insideEdgeOnTransformed;
    std::vector<tPoint> _behindPortalEdgePoints;
    cBitVector _portalResolvesToTerrain;
    cAttributesBuilder _errorAttributes;
    std::vector<tSigned32> _portalFaceIndices;
    tSigned32 _shapeFaceIndex;
};
