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
#include "interface/ClosestUnobstructedPoint.h"
#include "ElementInFaceLookup.h"
#include "BoundaryCrossings2D.h"
#include "ElementLookupCollisionTester.h"
#include "interface/EdgeExpansion.h"
#include "interface/PointCollision.h"
#include "interface/QueryContext.h"
#include "interface/LocalisedConvexShape.h"
#include "interface/LocalisedConvexShapeSet.h"
#include "interface/CompoundContextCollisionPreprocess.h"
#include "interface/CollisionAgainstShapeSet.h"
#include "interface/QueryContext.h"
#include "libs/Mesh2D/interface/InternalPosition.h"
#include "libs/Mesh2D/interface/MeshTraversal.h"
#include "libs/Mesh2D/interface/TranslateToPartitioned.h"
#include "libs/Mesh2D/interface/CircuitElement.h"
#include "libs/Mesh_Common/interface/PositionForPointInFace.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include "libs/Geometry/interface/Intersection_PointLike.h"
#include "libs/Geometry/interface/Point_PointLike.h"
#include "libs/Geometry/interface/tPoint_Multiplication.h"
#include "platform_common/CoordinateRanges.h"
#include "common/STL/list.h"
#include "common/STL/algorithm.h"

using std::vector;
using std::list;

namespace nClosestUnobstructedPoint
{

static tSigned32
getApproximationsAndResolvedFaces(const tIntersection& p, tFace f, tPoint* points, tFace* facesForPoints)
{
    tSigned32 numberOfApproximations;
    p.approximatePoints_IsOnAxis(numberOfApproximations, points);

    if(numberOfApproximations == 1)
    {
        facesForPoints[0] = f;
        return 1;
    }

    if(numberOfApproximations == 2)
    {
        tLine traverseLine;
        tFace endF;
        tEdge edgeCrossed;
        traverseLine = tLine(points[0], points[1]);
        endF = f;
        {
            cPoint_PointLike target(points[1]);
            while(TraverseTowardsTarget_SI<tMesh>(endF, traverseLine, target, edgeCrossed))
            {
            }
            if(endF->isExternal())
            {
                return 0;
            }
        }
        facesForPoints[1] = endF;
        traverseLine = tLine(points[1], points[0]);
        endF = f;
        {
            cPoint_PointLike target(points[0]);
            while(TraverseTowardsTarget_SI<tMesh>(endF, traverseLine, target, edgeCrossed))
            {
            }
            if(endF->isExternal())
            {
                return 0;
            }
        }
        facesForPoints[0] = endF;
        return 2;
    }

    assertD(numberOfApproximations == 4);

    tFace endF = f;
    tSigned32 i;
    for(i = 0; i < 4; ++i)
    {
        tSigned32 nextI = (i == 3) ? 0 : i + 1;
        tLine boundary(points[i], points[nextI]);
        if(p.refLine().side(boundary) != SIDE_RIGHT)
        {
            continue;
        }
        if(p.refLine().side(boundary.start()) == SIDE_RIGHT)
        {
            continue;
        }
        if(p.refLine().side(boundary.end()) != SIDE_RIGHT)
        {
            continue;
        }
        tIntersection intersection(p.refLine(), boundary);
        cIntersection_PointLike target(intersection);
        tEdge edgeCrossed;
        while(TraverseTowardsTarget_SI<tMesh>(endF, p.refLine(), target, edgeCrossed))
        {
        }
        if(endF->isExternal())
        {
            return 0;
        }
        break;
    }
    assertD(i != 4);

    tSigned32 j;
    for(j = 0; j < 4; ++j)
    {
        tSigned32 nextI = (i == 3) ? 0 : i + 1;
        tLine boundary(points[i], points[nextI]);
        cPoint_PointLike target(boundary.end());
        tEdge edgeCrossed;
        while(TraverseTowardsTarget_SI<tMesh>(endF, boundary, target, edgeCrossed))
        {
        }
        if(endF->isExternal())
        {
            return 0;
        }
        facesForPoints[nextI] = endF;
        i = nextI;
    }

    return 4;
}

void
GetElementsWithinBounds(
        const vector<tSigned32>& facesWithinBounds,
//        const vector<vector<cElementInFaceEntry> >& staticCircuitsLookup,
        const cEdgeExpansion& edgeExpansion,
        vector<const cCircuitElement*>& result
        )
{
    tSigned32 i;
    for(i = 0; i < SizeL(facesWithinBounds); i++)
    {
        tSigned32 j;
//        const vector<cElementInFaceEntry>& lookup = staticCircuitsLookup[facesWithinBounds[i]];
//        for(j = 0; j < SizeL(lookup); j++)
        for(j = 0; j != edgeExpansion.elementsInFace(facesWithinBounds[i]); ++j)
        {
//            const cElementInFaceEntry& entry = lookup[j];
//            const cCircuitElement* e = entry.element;
            const cCircuitElement* e = edgeExpansion.elementInFaceInfo(facesWithinBounds[i], j).element;
            if(!e->hasBeenReferenced())
            {
                e->markReferenced();
                result.push_back(e);
            }
        }
    }
    for(i = 0; i < SizeL(result); ++i)
    {
        result[i]->clearReferenced();
    }
}

class cCandidate
{
public:

    cCandidate()
    {
    }

    cCandidate(const tPoint& centre, const tIntersection& p, tFace f, bool checkAgainstBaseCircuits) :
     _p(p),
     _f(f),
     _checkAgainstBaseCircuits(checkAgainstBaseCircuits)
    {
        _approximatePoint = _p.approximatePoint();
        _distance = (_approximatePoint - centre).lengthSquared();
    }
    cCandidate(const tPoint& centre, const tPoint& p, tFace f, bool checkAgainstBaseCircuits) :
     _p(p),
     _f(f),
     _checkAgainstBaseCircuits(checkAgainstBaseCircuits)
    {
        _approximatePoint = p;
        _distance = (_approximatePoint - centre).lengthSquared();
    }

    tIntersection _p;
    tFace _f;
    tPoint::tDistance _distance;
    tSigned32 _checkAgainstBaseCircuits;

    //.... to help debugging
    tPoint _approximatePoint;
};

class cCandidateSet
{
    cQueryContext& _qc;
    tPoint _centre;
    list<cCandidate> _candidates;

    //...... use vector here and stl::push_heap etc.
    list<cCandidate*> _candidateOrder;

    static bool isCloserThan(cCandidate&, cCandidate&);
    void insertInOrder(cCandidate*);


public:

    cCandidateSet(cQueryContext& qc, const tPoint& centre) :
      _qc(qc),
      _centre(centre)
    {
    }

    bool empty() const
    {
        return _candidateOrder.empty();
    }
    const cCandidate& refNext() const
    {
        return *_candidateOrder.front();
    }
    void popNext()
    {
        _candidateOrder.pop_front();
    }

    void add(const tIntersection& p, const vector<tSigned32>& facesToResolveTo, bool checkAgainstBaseCircuits);
    void add(const tPoint& p, const vector<tSigned32>& facesToResolveTo, bool checkAgainstBaseCircuits);
    void add(const tIntersection& p, tFace f, bool checkAgainstBaseCircuits);
    void add(const tPoint& p, tFace f, bool checkAgainstBaseCircuits);
    void add(const tPoint* element1, const tPoint* element2,
             bool atCorner1, bool atCorner2,
             const vector<tSigned32>& facesToResolveTo,
             bool checkAgainstBaseCircuits);
};

// returns true if rhs is closer than lhs
bool
cCandidateSet::isCloserThan(cCandidate& lhs, cCandidate& rhs)
{
    return tMultiplier::compare(lhs._distance, rhs._distance) == -1;
}

void
cCandidateSet::insertInOrder(cCandidate* toInsert)
{
//..... use stl heap
    list<cCandidate*>::iterator i = _candidateOrder.begin();
    while(i != _candidateOrder.end() && isCloserThan(*toInsert, **i))
    {
        ++i;
    }
    _candidateOrder.insert(i, toInsert);
}

void
cCandidateSet::add(const tIntersection& p, const vector<tSigned32>& facesToResolveTo, bool checkAgainstBaseCircuits)
{
    tSigned32 i;
    for(i = 0; i < SizeL(facesToResolveTo); ++i)
    {
        tFace f = _qc.refMesh().faceAt(facesToResolveTo[i]);
        if(PointIsInFace<tMesh>(f, p))
        {
            _candidates.push_back(cCandidate(_centre, p, f, checkAgainstBaseCircuits));
            insertInOrder(&_candidates.back());
        }
    }
}
void
cCandidateSet::add(const tPoint& p, const vector<tSigned32>& facesToResolveTo, bool checkAgainstBaseCircuits)
{
    tSigned32 i;
    for(i = 0; i < SizeL(facesToResolveTo); ++i)
    {
        tFace f = _qc.refMesh().faceAt(facesToResolveTo[i]);
        if(PointIsInFace<tMesh>(f, p))
        {
            _candidates.push_back(cCandidate(_centre, p, f, checkAgainstBaseCircuits));
            insertInOrder(&_candidates.back());
        }
    }
}
void
cCandidateSet::add(const tIntersection& p, tFace f, bool checkAgainstBaseCircuits)
{
    _candidates.push_back(cCandidate(_centre, p, f, checkAgainstBaseCircuits));
    insertInOrder(&_candidates.back());
}
void
cCandidateSet::add(const tPoint& p, tFace f, bool checkAgainstBaseCircuits)
{
    _candidates.push_back(cCandidate(_centre, p, f, checkAgainstBaseCircuits));
    insertInOrder(&_candidates.back());
}
void
cCandidateSet::add(const tPoint* element1, const tPoint* element2,
             bool atCorner1, bool atCorner2,
             const vector<tSigned32>& facesToResolveTo,
             bool checkAgainstBaseCircuits)
{
    if(atCorner1)
    {
        add(element1[1], facesToResolveTo, checkAgainstBaseCircuits);
    }
    else
    if(atCorner2)
    {
        add(element2[1], facesToResolveTo, checkAgainstBaseCircuits);
    }
    else
    {
        add(tIntersection(tLine(element1[1], element1[2]), tLine(element2[1], element2[2])),
            facesToResolveTo, checkAgainstBaseCircuits);
    }
}

// convex corner and normal intersection candidates from base circuits
static void
BaseCircuitCandidates(const vector<const cCircuitElement*>& elements,
                      const tPoint& centre,
                      cCandidateSet& candidates)
{
    tSigned32 i;
    for(i = 0; i < SizeL(elements); ++i)
    {
        const cCircuitElement* e = elements[i];
        assertD(!e->enters());
        if(e->getPrev()->refLine().side(e->refLine()) != SIDE_RIGHT)
        {
            tIntersection p;
            e->getIntersectionForCornerAtStart(p);
            candidates.add(p, e->faceCrossed(0), false);
        }
        if(e->refLine().side(centre) == SIDE_RIGHT)
        {
            tPoint normal = e->refLine().axis().right90();
            tLine intersectingLine = tLine(centre - normal, centre);
            tFace f = e->getFaceForIntersection(intersectingLine);
            if(f.valid())
            {
                tIntersection p(e->refLine(), intersectingLine);
                candidates.add(p, f, false);
            }
        }
    }
}

//static void
//SmallConvexNormalIntersections(const cMeshCut& smallConvex,
//                               const tPoint& centre,
//                               cCandidateSet& candidates)
//{
//    const cCircuitElement* e = smallConvex.getStartElement();
//    do
//    {
//        if(e->refLine().side(centre) == SIDE_RIGHT)
//        {
//            tPoint normal = e->refLine().axis().right90();
//            tLine intersectingLine = tLine(centre - normal, centre);
//            tFace f = e->getFaceForIntersection(intersectingLine);
//            if(f.valid())
//            {
//                tIntersection p(e->refLine(), intersectingLine);
//                candidates.add(p, f, false);
//            }
//        }
//        e = e->getNext();
//    }
//    while(e != smallConvex.getStartElement());
//}

static void
IntersectionsBetweenShapeAndBaseCircuits(const vector<const cCircuitElement*>& baseCircuitElements,
                                          const cLocalisedConvexShape& shape, bool isInverted,
                                          cCandidateSet& candidates)
{
    tSigned32 i;
    for(i = 0; i < SizeL(baseCircuitElements); ++i)
    {
        tPoint element1[3];
        bool element1HasConvexCorner;
        CircuitElement2DData(baseCircuitElements[i], element1, element1HasConvexCorner);
        tSigned32 j;
        for(j = 0; j < shape.sides(); ++j)
        {
            tPoint element2[3];
            shape.getDataForSideElement(j, element2);
            bool atCorner1, atCorner2;
            if(ElementsPotentiallyCreateConcaveCorner(
                    element1, element1HasConvexCorner,
                    element2, !isInverted,
                    atCorner1, atCorner2
                    )
              )
            {
                candidates.add(element1, element2, atCorner1, atCorner2, shape.refFacesOverlapped(), false);
            }
        }
    }
}

static void
IntersectionsBetweenShapes(const cLocalisedConvexShape& shape1, bool isInverted1,
                           const cLocalisedConvexShape& shape2, bool isInverted2,
                           cCandidateSet& candidates)
{
    tSigned32 i;
    for(i = 0; i < shape1.sides(); ++i)
    {
        tPoint element1[3];
        shape1.getDataForSideElement(i, element1);
        tSigned32 j;
        for(j = 0; j < shape2.sides(); ++j)
        {
            tPoint element2[3];
            shape2.getDataForSideElement(j, element2);
            bool atCorner1, atCorner2;
            if(ElementsPotentiallyCreateConcaveCorner(
                    element1, !isInverted1,
                    element2, !isInverted2,
                    atCorner1, atCorner2
                    )
              )
            {
                candidates.add(element1, element2, atCorner1, atCorner2, shape1.refFacesOverlapped(), true);
            }
        }
    }
}

static void
ShapeNormalIntersections(const cLocalisedConvexShape& shape, const tPoint& centre, cCandidateSet& candidates)
{
    tSigned32 i;
    for(i = 0; i < shape.sides(); ++i)
    {
        tLine boundaryLine = shape.poly().getBoundaryLine(i);
        if(boundaryLine.side(centre) != SIDE_RIGHT)
        {
            continue;
        }
        tPoint normal = boundaryLine.axis().right90();
        tLine intersectingLine = tLine(centre - normal, centre);
        if(intersectingLine.side(boundaryLine.start()) != SIDE_LEFT
         && intersectingLine.side(boundaryLine.end()) == SIDE_LEFT)
        {
            tIntersection p(boundaryLine, intersectingLine);
            candidates.add(p, shape.refFacesOverlapped(), true);
        }
    }
}

cInternalPosition
find(cQueryContext& qc,
        const cInternalPosition& centre,
        tSigned32 radius,
//        const vector<vector<cElementInFaceEntry> >& staticCircuitsLookup,
        const cEdgeExpansion& edgeExpansion,
        const cLocalisedConvexShapeSet& staticExpansions,
        const cCompoundContextCollisionPreprocess* dynamicExpansions // zero can be passed in to indicate no dynamic expansions
        )
{
  // initially empty set of geometry that has been added
    bool baseCircuitsAdded = false;
    bool boundsAdded = false;
    vector<tSigned32> staticExpansionsAdded;
    vector<const cLocalisedConvexShape*> dynamicExpansionsAdded;

    cConvexPoly boundsPoly(centre.p, radius, WORLD_RANGE + 1); // add 1 to ensure that the centre point is always inside the resulting poly
    cLocalisedConvexShape bounds(qc, boundsPoly, centre);

    vector<const cCircuitElement*> elementsInBounds;
//    GetElementsWithinBounds(bounds.refFacesOverlapped(), staticCircuitsLookup, elementsInBounds);
    GetElementsWithinBounds(bounds.refFacesOverlapped(), edgeExpansion, elementsInBounds);

    cCandidateSet candidates(qc, centre.p);

    candidates.add(centre.p, centre.f, true);

    while(!candidates.empty())
    {
        const cCandidate& candidate = candidates.refNext();
        candidates.popNext();

      // collision with base circuits and small convex
        if(candidate._checkAgainstBaseCircuits)
        {
//            if(TestPointCollision(qc, staticCircuitsLookup, true, candidate._f, candidate._p))
            if(edgeExpansion.refPointTester().testPointCollision(qc, candidate._f, candidate._p))
            {
                if(!baseCircuitsAdded)
                {
                // add intersections with the bounds, if included, and any included expansions
                    baseCircuitsAdded = true;
                    BaseCircuitCandidates(elementsInBounds, centre.p, candidates);
                    if(boundsAdded)
                    {
                        IntersectionsBetweenShapeAndBaseCircuits(elementsInBounds, bounds, true, candidates);
                    }
                    tSigned32 i;
                    for(i = 0; i < SizeL(dynamicExpansionsAdded); ++i)
                    {
                        IntersectionsBetweenShapeAndBaseCircuits(elementsInBounds, *(dynamicExpansionsAdded[i]), false, candidates);
                    }
                }
                continue;
            }

            tSigned32 expansionIndex;
            if(PointCollision_AgainstShapeSet(staticExpansions, candidate._f.index(), candidate._p, expansionIndex))
            {
                if(std::find(staticExpansionsAdded.begin(), staticExpansionsAdded.end(), expansionIndex) == staticExpansionsAdded.end())
                {
                    staticExpansionsAdded.push_back(expansionIndex);
                    ShapeNormalIntersections(staticExpansions[expansionIndex], centre.p, candidates);
                    if(boundsAdded)
                    {
                        IntersectionsBetweenShapes(staticExpansions[expansionIndex], false, bounds, true, candidates);
                    }
                    tSigned32 i;
                    for(i = 0; i < SizeL(dynamicExpansionsAdded); ++i)
                    {
                        IntersectionsBetweenShapes(staticExpansions[expansionIndex], false, *(dynamicExpansionsAdded[i]), false, candidates);
                    }
                }
                continue;
            }
        }

      // collision with bounds
        if(bounds.testPointCollision_Inverted(candidate._f, candidate._p))
        {
            if(!boundsAdded)
            {
            // add intersections with base circuits, if included, and any included expansions
                boundsAdded = true;
                if(baseCircuitsAdded)
                {
                    IntersectionsBetweenShapeAndBaseCircuits(elementsInBounds, bounds, true, candidates);
                }
                tSigned32 i;
//                if(withSmallConvex)
//                {
//                    for(i = 0; i < SizeL(staticExpansionsAdded); ++i)
//                    {
//                        IntersectionsBetweenShapes(staticExpansions[staticExpansionsAdded[i]], false, bounds, true, candidates);
//                    }
//                }
                for(i = 0; i < SizeL(staticExpansionsAdded); ++i)
                {
                    IntersectionsBetweenShapes(staticExpansions[staticExpansionsAdded[i]], false, bounds, true, candidates);
                }
                for(i = 0; i < SizeL(dynamicExpansionsAdded); ++i)
                {
                    IntersectionsBetweenShapes(*(dynamicExpansionsAdded[i]), false, bounds, true, candidates);
                }
            }
            continue;
        }

      // collision with dynamic obstacles
        if(dynamicExpansions)
        {
            const cCompoundContextCollisionPreprocess& cccp = *dynamicExpansions;
            const cLocalisedConvexShape* shapePtr;
            if(PointCollision_AgainstShapeSet(cccp, candidate._f.index(), candidate._p, shapePtr))
            {
                if(std::find(dynamicExpansionsAdded.begin(), dynamicExpansionsAdded.end(), shapePtr) == dynamicExpansionsAdded.end())
                {
                    ShapeNormalIntersections(*shapePtr, centre.p, candidates);
                    if(baseCircuitsAdded)
                    {
                        IntersectionsBetweenShapeAndBaseCircuits(elementsInBounds, *shapePtr, false, candidates);
                    }
                    if(boundsAdded)
                    {
                        IntersectionsBetweenShapes(*shapePtr, false, bounds, true, candidates);
                    }
                    tSigned32 i;
//                    if(withSmallConvex)
//                    {
//                        for(i = 0; i < SizeL(staticExpansionsAdded); ++i)
//                        {
//                            IntersectionsBetweenShapes(staticExpansions[staticExpansionsAdded[i]], false, shapes[shapeIndex], false, candidates);
//                        }
//                    }
                    for(i = 0; i < SizeL(staticExpansionsAdded); ++i)
                    {
                        IntersectionsBetweenShapes(staticExpansions[staticExpansionsAdded[i]], false, *shapePtr, false, candidates);
                    }
                    for(i = 0; i < SizeL(dynamicExpansionsAdded); ++i)
                    {
                        IntersectionsBetweenShapes(*(dynamicExpansionsAdded[i]), false, *shapePtr, false, candidates);
                    }
                    dynamicExpansionsAdded.push_back(shapePtr);
                }
                continue;
            }
        }

        tSigned32 approximations;
        tPoint approximatePoints[4];
        tFace facesForApproximatePoints[4];
        approximations = getApproximationsAndResolvedFaces(candidate._p, candidate._f, approximatePoints, facesForApproximatePoints);

        tSigned32 i;
        for(i = 0; i < approximations; ++i)
        {
//            if(TestPointCollision(qc, staticCircuitsLookup, true, facesForApproximatePoints[i], approximatePoints[i]))
            if(edgeExpansion.refPointTester().testPointCollision(qc, facesForApproximatePoints[i], approximatePoints[i]))
            {
                continue;
            }
//            if(withSmallConvex)
//            {
//                cPoint_PointLike pl(approximatePoints[i]);
//                tFace fOnSmallConvex = TranslateFace(*smallConvexTranslator, facesForApproximatePoints[i], pl);
//                cInternalPosition ip(fOnSmallConvex, approximatePoints[i]);
//                if(smallConvexOnPartitioned->testPointCollision(ip))
//                {
//                    continue;
//                }
//            }
            {
                cInternalPosition ip(facesForApproximatePoints[i], approximatePoints[i]);
                if(PointCollision_AgainstShapeSet<cLocalisedConvexShapeSet>(staticExpansions, ip))
                {
                    continue;
                }
            }

            cInternalPosition ip(facesForApproximatePoints[i], approximatePoints[i]);
            if(dynamicExpansions && PointCollision_AgainstShapeSet<cCompoundContextCollisionPreprocess>(*dynamicExpansions, ip))
            {
                continue;
            }
            // no check against bounds here, so the result may be slightly out of bounds
            return cInternalPosition(facesForApproximatePoints[i], approximatePoints[i]);
        }

        // an intersection was unobstructed but all surrounding approximations were obstructed
        // or the intersection was right next to the edge of the mesh
    }

    cInternalPosition result;
    result.f.clear();
    return result;
}

cInternalPosition
find_Reference(cQueryContext& qc,
        const cInternalPosition& centre,
        tSigned32 radius,
//        const std::vector<std::vector<cElementInFaceEntry> >& staticCircuitsLookup,
        const cEdgeExpansion& edgeExpansion,
        const cLocalisedConvexShapeSet& staticExpansions,
        const cCompoundContextCollisionPreprocess* dynamicExpansions // zero can be passed in to indicate no dynamic expansions
        )
{
    cConvexPoly boundsPoly(centre.p, radius, WORLD_RANGE + 1); // add 1 to ensure that the centre point is always inside the resulting poly
    cLocalisedConvexShape bounds(qc, boundsPoly, centre);

    cCandidateSet candidates(qc, centre.p);

    tSigned32 x;
    for(x = -radius; x <= radius; x++)
    {
        tSigned32 y;
        for(y = -radius; y <= radius; y++)
        {
            candidates.add(centre.p + tPoint(x, y, SHAPE_RANGE), bounds.refFacesOverlapped(), true);
        }
    }

    while(!candidates.empty())
    {
        const cCandidate& candidate = candidates.refNext();
        candidates.popNext();

        //... remove repetition with above

        tSigned32 approximations;
        tPoint approximatePoints[4];
        tFace facesForApproximatePoints[4];
        approximations = getApproximationsAndResolvedFaces(candidate._p, candidate._f, approximatePoints, facesForApproximatePoints);

        tSigned32 i;
        for(i = 0; i < approximations; ++i)
        {
//            if(TestPointCollision(qc, staticCircuitsLookup, true, facesForApproximatePoints[i], approximatePoints[i]))
            if(edgeExpansion.refPointTester().testPointCollision(qc, facesForApproximatePoints[i], approximatePoints[i]))
            {
                continue;
            }
//            if(withSmallConvex)
//            {
//                cPoint_PointLike pl(approximatePoints[i]);
//                tFace fOnSmallConvex = TranslateFace(*smallConvexTranslator, facesForApproximatePoints[i], pl);
//                cInternalPosition ip(fOnSmallConvex, approximatePoints[i]);
//                if(smallConvexOnPartitioned->testPointCollision(ip))
//                {
//                    continue;
//                }
//            }
            {
                cInternalPosition ip(facesForApproximatePoints[i], approximatePoints[i]);
                if(PointCollision_AgainstShapeSet<cLocalisedConvexShapeSet>(staticExpansions, ip))
                {
                    continue;
                }
            }
            cInternalPosition ip(facesForApproximatePoints[i], approximatePoints[i]);
            if(dynamicExpansions && PointCollision_AgainstShapeSet<cCompoundContextCollisionPreprocess>(*dynamicExpansions, ip))
            {
                continue;
            }
            // no check against bounds here, so the result may be slightly out of bounds
            return cInternalPosition(facesForApproximatePoints[i], approximatePoints[i]);
        }
    }

    cInternalPosition result;
    result.f.clear();
    return result;
}

void checkResultAgainstReference(const tPoint& centre, const tPoint& reference, const tPoint& result)
{
    assertD(Length(result - centre) <= Length(reference - centre) + 2);
}

}
