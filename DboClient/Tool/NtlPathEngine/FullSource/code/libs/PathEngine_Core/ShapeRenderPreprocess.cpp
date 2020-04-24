//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/ShapeRenderPreprocess.h"
#include "libs/PathEngine_Core/interface/OverlappingPolygon.h"
#include "libs/Geometry/interface/ConvexPoly.h"
#include "libs/Geometry/interface/ConvexPoly.h"

bool
cShapeRenderPreprocess::isValidFor(const cInternalPosition& baseShapePosition) const
{
    return _preprocessedPosition == baseShapePosition;
}
void
cShapeRenderPreprocess::initOrReinit(
        const cInternalPosition& position,
        const cConvexPoly& placedBaseShape,
        const cConvexPoly& expandShape
        )
{
    _preprocessedPosition = position;
    _circuits.clear();
    std::list<tFace> containedExternalFaces;
    cConvexPoly combinedShape;
    placedBaseShape.minkowskiExpand(expandShape, combinedShape);
    CircuitsForOverlappingPolygon(
            _preprocessedPosition.f,
            combinedShape,
            containedExternalFaces,
            _circuits
            );
}
