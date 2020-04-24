//**********************************************************************
//
// Copyright (c) 2004-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/Mesh3DCostCalculator.h"
#include "interface/TranslationToOverlapped.h"
#include "libs/Geometry/interface/AxisFraction.h"
#include "libs/Mesh2D/interface/InternalPosition.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "libs/Mesh_Common/interface/ElementsTraversal.h"
#include "libs/Mesh_Common/interface/PositionForPointInFace.h"

static float
GetCrossingRatio(const tLine& line, float lineLength, const cMeshElement<tMesh_3D>& fromEl, const cMeshElement<tMesh_3D>& toEl)
{
    assertD(!toEl.isVertex());
    if(fromEl.isVertex())
    {
        tVertex_3D v = fromEl.asVertex();
        tPoint vertexP = GetVertexPoint(v);
        float distanceToVertex = SquareRoot(tMultiplier::asFloat((vertexP - line.start()).lengthSquared()));
        return distanceToVertex / lineLength;
    }
    if(fromEl.isEdge())
    {
        assertD(toEl.isFace());
        cAxisFraction af(line, GetEdgeLine(fromEl.asEdge()));
        return af.asFloat();
    }
    assertD(toEl.isEdge());
    cAxisFraction af(line, GetEdgeLine(toEl.asEdge()));
    return af.asFloat();
}

tFace_3D
cMesh3DCostCalculator::getFaceWithLowestCost(const cMeshElement<tMesh_3D>& el) const
{
    switch(el.type())
    {
    default:
        invalid(); // vertex case not supported - these are skipped in the main loop, below
    case bMeshElement::FACE:
        return el.asFace();
    case bMeshElement::EDGE:
        {
            tFace_3D f1 = el.asEdge().face();
            tFace_3D f2 = el.asEdge().twin().face();
            if(_costs[_surfaceTypes[f1.index()]] == -1.f)
            {
                return f2;
            }
            if(_costs[_surfaceTypes[f2.index()]] == -1.f)
            {
                return f1;
            }
            if(_costs[_surfaceTypes[f1.index()]] < _costs[_surfaceTypes[f2.index()]])
            {
                return f1;
            }
            return f2;
        }
    }
}

cMesh3DCostCalculator::cMesh3DCostCalculator(
    const cTranslationToOverlapped& tto,
    const std::vector<tSigned32>& surfaceTypes,
    const std::vector<float>& costs,
    const std::vector<tPoint>& costDirections) :
 _tto(tto),
 _surfaceTypes(surfaceTypes),
 _costs(costs),
 _costDirections(costDirections),
 _directionLengths(costDirections.size(), 0.f)
{
    tSigned32 i;
    for(i = 0; i < SizeL(_costDirections); ++i)
    {
        if(_costs[i] == 0.f)
        {
            // to prevent 'false crossings' being detected
            // for direction changes between areas of no cost
            _costDirections[i] = tPoint(0,0,0);
            continue;
        }
        if(_costDirections[i] == tPoint(0,0,0))
        {
            continue;
        }
        float directionLengthSquared = tMultiplier::asFloat(_costDirections[i].lengthSquared());
        _directionLengths[i] = SquareRoot(directionLengthSquared);
        assertD(_directionLengths[i] > 0.f); // minimum length for a non-zero vector is 1 unit
    }
}

float
cMesh3DCostCalculator::calculate(cQueryContext& qc, const cInternalPosition& from, const tPoint& to, bool& blocked) const
{
    blocked = false;
    if(from.p == to)
    {
        return 0.f;
    }

    tLine l(from.p, to);

    tFace_3D overlappingF = _tto.translate(from.f, from.p);
    cMeshElement<tMesh_3D> el3D = ElementForPointInOverlappingFace<tMesh_3D>(overlappingF, from.p);

    if(el3D.isVertex())
    {
        el3D = NextElementAlongLine(el3D, l);
    }

    tFace_3D f = getFaceWithLowestCost(el3D);

    tSigned32 currentSurfaceType = _surfaceTypes[f.index()];
    float startRatio = 0.f;

    float segmentDistance = SquareRoot(tMultiplier::asFloat(l.axis().lengthSquared()));

    float result = 0.f;

    while(!ElementContainsPoint(el3D, to))
    {
        const cMeshElement<tMesh_3D> previousEl3D = el3D;
        el3D = NextElementAlongLine(el3D, l);

        if(el3D.type() == bMeshElement::VERTEX)
        {
            continue;
        }

        const tFace_3D previousF = f;
        tFace_3D f = getFaceWithLowestCost(el3D);
        const tSigned32 previousSurfaceType = currentSurfaceType;
        currentSurfaceType = _surfaceTypes[f.index()];
        if(previousSurfaceType == currentSurfaceType)
        {
            continue;
        }
        if(_costs[previousSurfaceType] == _costs[currentSurfaceType]
            && _costDirections[previousSurfaceType] == _costDirections[currentSurfaceType])
        {
            continue;
        }

      // crosses to a region with different cost

        float crossingRatio = GetCrossingRatio(l, segmentDistance, previousEl3D, el3D);

        if(_costs[previousSurfaceType] != 0.f)
        {
            float distanceInDirection;
            if(_directionLengths[previousSurfaceType] != 0.f)
            {
                tLine l(from.p, from.p + _costDirections[previousSurfaceType].right90());
                if(l.side(to) != SIDE_LEFT)
                {
                // travelling away from (or at right angles to) direction for cost
                    startRatio = crossingRatio; //... optimise - only need to calculate this if next segment actually has a cost
                    continue;
                }
                tPoint travelDirection = to - from.p;
                distanceInDirection = tMultiplier::asFloat(travelDirection.dotProduct(_costDirections[previousSurfaceType]));
                distanceInDirection /= _directionLengths[previousSurfaceType];
            }
            else
            {
            // no direction associated with this cost
                distanceInDirection = segmentDistance;
            }

            assertD(crossingRatio >= startRatio);
            if(_costs[previousSurfaceType] == -1.f)
            {
                blocked = true;
                return 0.f;
            }
            result += distanceInDirection * (crossingRatio - startRatio) * _costs[previousSurfaceType];
        }
        startRatio = crossingRatio;
    }

    if(_costs[currentSurfaceType] != 0.f)
    {
        float distanceInDirection;
        if(_directionLengths[currentSurfaceType] != 0.f)
        {
            tLine l(from.p, from.p + _costDirections[currentSurfaceType].right90());
            if(l.side(to) != SIDE_LEFT)
            {
            // travelling away from (or at right angles to) direction for cost
                return result;
            }
            tPoint travelDirection = to - from.p;
            distanceInDirection = tMultiplier::asFloat(travelDirection.dotProduct(_costDirections[currentSurfaceType]));
            distanceInDirection /= _directionLengths[currentSurfaceType];
        }
        else
        {
        // no direction associated with this cost
            distanceInDirection = segmentDistance;
        }

        assertD(1.f >= startRatio);
        if(_costs[currentSurfaceType] == -1.f)
        {
            blocked = true;
            return 0.f;
        }
        result += distanceInDirection * (1.f - startRatio) * _costs[currentSurfaceType];
    }

    return result;
}
