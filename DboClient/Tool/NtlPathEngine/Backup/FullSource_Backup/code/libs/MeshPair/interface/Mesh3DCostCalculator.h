//**********************************************************************
//
// Copyright (c) 2004-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef MESH_3D_COST_CALCULATOR_INCLUDED
#define MESH_3D_COST_CALCULATOR_INCLUDED

#include "libs/Geometry/interface/tPoint.h"
#include "libs/Mesh3D/interface/tMesh_3D_Iterators.h"
#include "libs/PathEngine_Core/interface/iMovementCostCalculator.h"
#include "common/STL/vector.h"

class cInternalPosition;
class cTranslationToOverlapped;
template <class T> class cMeshElement;

class cMesh3DCostCalculator : public iMovementCostCalculator
{
    const cTranslationToOverlapped& _tto;
    const std::vector<tSigned32>& _surfaceTypes;
    std::vector<float> _costs;
    std::vector<tPoint> _costDirections;
    std::vector<float> _directionLengths;

    tFace_3D getFaceWithLowestCost(const cMeshElement<tMesh_3D>& el) const;

public:

    // meshPair and surfaceTypes must be immutable over lifetime
    // cost and cost direction vectors are copied
    cMesh3DCostCalculator(
        const cTranslationToOverlapped& tto,
        const std::vector<tSigned32>& surfaceTypes,
        const std::vector<float>& costs,
        const std::vector<tPoint>& costDirections);

// iMovementCostCalculator

    float calculate(cQueryContext& qc, const cInternalPosition& from, const tPoint& to, bool& blocked) const;
};

#endif
