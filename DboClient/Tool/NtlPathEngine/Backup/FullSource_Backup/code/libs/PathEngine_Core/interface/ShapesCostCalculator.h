//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef SHAPES_COST_CALCULATOR_INCLUDED
#define SHAPES_COST_CALCULATOR_INCLUDED

#include "common/interface/Assert.h"
#include "libs/PathEngine_Core/interface/iMovementCostCalculator.h"
#include "libs/PathEngine_Core/interface/LocalisedConvexShapeSet.h"
#include "libs/Geometry/interface/tPoint.h"
#include "libs/Geometry/interface/tLine_Header.h"

class cShapesCostCalculator : public iMovementCostCalculator
{
    cLocalisedConvexShapeSet _shapeSet;
    std::vector<float> _costs;
    std::vector<tPoint> _directions;
    std::vector<float> _directionLengths;

    float
    checkExitAndCalculateCost(
                        const cInternalPosition& from, const tPoint& to,
                        const tLine& segmentLineReversed,
                        float segmentDistance,
                        float startFraction,
                        tSigned32 i                        
                        ) const;

    // prevent copying and assignment
    cShapesCostCalculator(const cShapesCostCalculator&);
    const cShapesCostCalculator& operator=(const cShapesCostCalculator&);

public:

    cShapesCostCalculator(tSigned32 facesInMesh) :
      _shapeSet(facesInMesh)
    {
    }

// initialisation, reinitialisation and modification

    void clear()
    {
        _shapeSet.clear();
        _costs.clear();
        _directions.clear();
        _directionLengths.clear();
    }

    // ownership of the shape is not taken here
    void push_back(const cLocalisedConvexShape* shape, float cost, const tPoint& direction);
    
//    void swapOut(tSigned32 index);
    void setCompleted();

    tSigned32 size() const
    {
        return _shapeSet.size();
    }

// iMovementCostCalculator

    float calculate(cQueryContext& qc, const cInternalPosition& from, const tPoint& to, bool& blocked) const;
};

#endif
