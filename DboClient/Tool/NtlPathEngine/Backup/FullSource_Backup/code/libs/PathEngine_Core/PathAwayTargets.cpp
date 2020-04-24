//**********************************************************************
//
// Copyright (c) 2002-2007
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "PathAwayTargets.h"
#include "ElementInFaceLookup.h"
#include "interface/OverlappingPolygon.h"
#include "interface/EdgeExpansion.h"
#include "interface/PointIsInsideCircuits.h"
#include "interface/QueryContext.h"
#include "interface/LocalisedConvexShapeSet.h"
#include "interface/LocalisedConvexShape.h"
#include "libs/Mesh2D/interface/CircuitElement.h"
#include "libs/Mesh2D/interface/InternalPosition.h"
#include "libs/Geometry/interface/tPoint_Multiplication.h"
#include "libs/Geometry/interface/AxisFraction.h"
#include "libs/Geometry/interface/ConvexPoly.h"
#include "platform_common/AddWithOverflow.h"
#include "common/Containers/FlagVector.h"
#include "common/STL/algorithm.h"

static void
GetEntryAndExitPoints(
        const cCircuitElement& el,
        const cLocalisedConvexShapeSet& expansions,
        std::vector<tPoint>& points
        )
{
    tPoint::tDistance startN, startD;
    tPoint::tDistance endN, endD;
    el.getStartOrEntryFraction(startN, startD);
    el.getEndOrExitFraction(endN, endD);
    cAxisFraction startFraction(startN, startD);
    cAxisFraction endFraction(endN, endD);
    cAxisFraction startFraction_Reversed(endD - endN, endD);
    cAxisFraction endFraction_Reversed(startD - startN, startD);
    tLine line_Reversed = el.refLine();
    line_Reversed.reverse();

    cBitVector expansionChecked(expansions.size(), false);
    tSigned32 i;
    for(i = 0; i != el.facesCrossed(); ++i)
    {
        tFace f = el.faceCrossed(i);
        tSigned32 j;
        for(j = 0; j != expansions.shapesInFace(f.index()); ++j)
        {
            tSigned32 expansionIndex = expansions.indexForShapeInFace(f.index(), j);
            if(expansionChecked[expansionIndex])
            {
                continue;
            }
            expansionChecked.setBit(expansionIndex, true);
            const cLocalisedConvexShape& shape = expansions[expansionIndex];
            tSigned32 collidingLineIndex;
            if(shape.poly().lineCollides2D(el.refLine(), startFraction, endFraction, collidingLineIndex))
            {
                tLine intersectingLine = shape.poly().getBoundaryLine(collidingLineIndex);
                points.push_back(el.refLine().approximateIntersection(intersectingLine.reversed()));
            }
            if(shape.poly().lineCollides2D(line_Reversed, startFraction_Reversed, endFraction_Reversed, collidingLineIndex))
            {
                tLine intersectingLine = shape.poly().getBoundaryLine(collidingLineIndex);
                points.push_back(el.refLine().approximateIntersection(intersectingLine.reversed()));
            }
        }
    }
}

static void
GetEntryAndExitPoints(
        const cCircuitElement* baseStartElement,
        const cLocalisedConvexShapeSet& expansions,
        std::vector<tPoint>& points
        )
{
    const cCircuitElement* e1 = baseStartElement;
    do
    {
        GetEntryAndExitPoints(*e1, expansions, points);
        e1 = e1->getNext();
    }
    while(e1 != baseStartElement);
}

static void
GetEntryAndExitPoints(
        const cCircuitElement* baseStartElement,
        const cCircuitElement& test,
        std::vector<tPoint>& points
        )
{
    const cCircuitElement* e1 = baseStartElement;
    do
    {
        tSigned32 faceIndex1, faceIndex2;
        bool atStart1, atEnd1, atStart2, atEnd2;
        if(e1->elementEntersThroughThis(
                test,
                atStart1, atEnd1, atStart2,
                faceIndex1, faceIndex2))
        {
            if(atStart1)
            {
                const tLine& intersect = e1->getIntersectAtCorner(test.refLine());
                points.push_back(intersect.approximateIntersection(test.refLine()));
            }
            else
            if(atEnd1)
            {
                const tLine& intersect = e1->getNext()->getIntersectAtCorner(test.refLine());
                points.push_back(intersect.approximateIntersection(test.refLine()));
            }
            else
            {
                points.push_back(e1->refLine().approximateIntersection(test.refLine()));
            }
        }
        if(e1->elementExitsThroughThis(
                test,
                atStart1, atEnd1, atEnd2,
                faceIndex1, faceIndex2))
        {
            if(atStart1)
            {
                const tLine& intersect = e1->getIntersectAtCorner(test.refLine());
                points.push_back(intersect.approximateIntersection(test.refLine().reversed()));
            }
            else
            if(atEnd1)
            {
                const tLine& intersect = e1->getNext()->getIntersectAtCorner(test.refLine());
                points.push_back(intersect.approximateIntersection(test.refLine().reversed()));
            }
            else
            {
                points.push_back(e1->refLine().approximateIntersection(test.refLine().reversed()));
            }
        }
        e1 = e1->getNext();
    }
    while(e1 != baseStartElement);
}

cPathAwayTargets::cPathAwayTargets(const cPathAwayTargets::cInitParams& params)
{
    {
        tSigned32 minX, maxX, minY, maxY;
        const tSigned32 range = WORLD_RANGE + 1;
        bool overflow;
        overflow = SubtractWithOverflow(params._centre.p.getX(), params._radius, minX);
        if(overflow || minX < -range)
        {
            minX = -range;
        }
        overflow = AddWithOverflow(params._centre.p.getX(), params._radius, maxX);
        if(overflow || maxX > range)
        {
            maxX = range;
        }
        overflow = SubtractWithOverflow(params._centre.p.getY(), params._radius, minY);
        if(overflow || minY < -range)
        {
            minY = -range;
        }
        overflow = AddWithOverflow(params._centre.p.getY(), params._radius, maxY);
        if(overflow || maxY > range)
        {
            maxY = range;
        }
        _bottomLeft = tPoint(minX, minY, range);
        _topRight = tPoint(maxX, maxY, range);
        tPoint* pointArray = new tPoint[4];
        pointArray[0] = _bottomLeft;
        pointArray[1] = tPoint(minX, maxY, range);
        pointArray[2] = _topRight;
        pointArray[3] = tPoint(maxX, minY, range);
        cConvexPoly boundsPoly(4, pointArray);
        CircuitsForOverlappingPolygon(params._centre.f, boundsPoly, _containedExternalFaces, _boundCircuits);
    }


    cFlagVector& elementFlags = params._qc.lockElementFlagVector();
    tMeshCircuits::const_iterator i;
    for(i = _boundCircuits.begin(); i != _boundCircuits.end() ; ++i)
    {
        tMeshCutSequence::const_iterator j;
        for(j = (*i)->begin(); j != (*i)->end() ; ++j)
        {
            const cCircuitElement* e1 = (*j)->getStartElement();

            elementFlags.clear();
            tSigned32 k;
            for(k = 0; k < e1->facesCrossed(); ++k)
            {
                tSigned32 fIndex = e1->faceCrossed(k).index();
                tSigned32 l;
                for(l = 0; l < params._edgeExpansion.elementsInFace(fIndex); ++l)
                {
                    const cCircuitElement* e2 = params._edgeExpansion.elementInFaceInfo(fIndex, l).element;
                    if(elementFlags[e2->getIndex()])
                    {
                        continue;
                    }
                    elementFlags.set(e2->getIndex());
                    GetEntryAndExitPoints(e1, *e2, _pointTargets);
                }
            }

            GetEntryAndExitPoints(e1, params._expansions, _pointTargets);
        }
    }
    params._qc.releaseFlagVector();
}

//cPathAwayTargets::cPathAwayTargets(
//        cQueryContext& qc,
//        const cInternalPosition& centre, tSigned32 radius,
//        const cEdgeExpansion& edgeExpansion,
//        const cLocalisedConvexShapeSet& expansions
//        )
//{
//    {
//        tSigned32 minX, maxX, minY, maxY;
//        const tSigned32 range = WORLD_RANGE + 1;
//        bool overflow;
//        overflow = SubtractWithOverflow(centre.p.getX(), radius, minX);
//        if(overflow || minX < -range)
//        {
//            minX = -range;
//        }
//        overflow = AddWithOverflow(centre.p.getX(), radius, maxX);
//        if(overflow || maxX > range)
//        {
//            maxX = range;
//        }
//        overflow = SubtractWithOverflow(centre.p.getY(), radius, minY);
//        if(overflow || minY < -range)
//        {
//            minY = -range;
//        }
//        overflow = AddWithOverflow(centre.p.getY(), radius, maxY);
//        if(overflow || maxY > range)
//        {
//            maxY = range;
//        }
//        _bottomLeft = tPoint(minX, minY, range);
//        _topRight = tPoint(maxX, maxY, range);
//        tPoint* pointArray = new tPoint[4];
//        pointArray[0] = _bottomLeft;
//        pointArray[1] = tPoint(minX, maxY, range);
//        pointArray[2] = _topRight;
//        pointArray[3] = tPoint(maxX, minY, range);
//        cConvexPoly boundsPoly(4, pointArray);
//        CircuitsForOverlappingPolygon(centre.f, boundsPoly, _containedExternalFaces, _boundCircuits);
//    }
//
//
//    cFlagVector& elementFlags = qc.lockElementFlagVector();
//    tMeshCircuits::const_iterator i;
//    for(i = _boundCircuits.begin(); i != _boundCircuits.end() ; ++i)
//    {
//        tMeshCutSequence::const_iterator j;
//        for(j = (*i)->begin(); j != (*i)->end() ; ++j)
//        {
//            const cCircuitElement* e1 = (*j)->getStartElement();
//
//            elementFlags.clear();
//            tSigned32 k;
//            for(k = 0; k < e1->facesCrossed(); ++k)
//            {
//                tSigned32 fIndex = e1->faceCrossed(k).index();
//                tSigned32 l;
//                for(l = 0; l < edgeExpansion.elementsInFace(fIndex); ++l)
//                {
//                    const cCircuitElement* e2 = edgeExpansion.elementInFaceInfo(fIndex, l).element;
//                    if(elementFlags[e2->getIndex()])
//                    {
//                        continue;
//                    }
//                    elementFlags.set(e2->getIndex());
//                    GetEntryAndExitPoints(e1, *e2, _pointTargets);
//                }
//            }
//
//            GetEntryAndExitPoints(e1, expansions, _pointTargets);
//        }
//    }
//    qc.releaseFlagVector();
//}

bool
cPathAwayTargets::positionIsInside(const cInternalPosition& p) const
{
    return PointIsInsideCircuits(_boundCircuits, _containedExternalFaces, p);
}

cInternalPosition
cPathAwayTargets::getTargetPosition(tSigned32 index, const cInternalPosition& parent) const
{
    tPoint resultP = getTargetPoint(index, parent.p);
    tLine l(parent.p, resultP);
    tFace resultF = FaceAtEndOfLine<tMesh>(parent.f, l);
    return cInternalPosition(resultF, resultP);
}
