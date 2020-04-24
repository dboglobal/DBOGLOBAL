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
#include "interface/CollisionAgainstShapeSet.h"
#include "ResolveFaceForIntersection.h"
#include "interface/LocalisedConvexShapeSet.h"
#include "interface/QueryContext.h"
#include "libs/Mesh2D/interface/MeshTraversal.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include "libs/Geometry/interface/AxisFraction.h"
#include "libs/Geometry/interface/Point_PointLike.h"
#include "libs/Geometry/interface/Intersection_PointLike.h"
#include "libs/Geometry/interface/CompareFractions.h"
#include "common/Containers/CollapsedVectorVector.h"
#include "common/Containers/BitVector.h"

class cUnresolvedCollision
{
public:

    tSigned32 _shapeIndex;
    tSigned32 collidingLineIndex;
    tPoint::tDistance n, d;

    bool isBeforeFaceEnd(const tPoint::tDistance& faceEndN, const tPoint::tDistance& faceEndD, bool faceEndInclusive) const
    {
        tSigned32 comparision = CompareFractions(faceEndN, faceEndD, n, d);
        if(faceEndInclusive)
        {
            return comparision != 1;
        }
        return comparision == -1;
    }

    bool isAfter(const cUnresolvedCollision& rhs) const
    {
        return CompareFractions(rhs.n, rhs.d, n, d) == 1;
    }
};

bool
LineCollision_AgainstShapeSet(
        tFace startF, const tLine& testLine,
        const cLocalisedConvexShapeSet& shapeSet,
        tSigned32& collidingShape,
        tSigned32& collidingLineIndex,
        cAxisFraction& distanceAlong,
        tFace& endF
        )
{
    cBitVector expansionChecked(shapeSet.size(), false);
    std::list<cUnresolvedCollision> unresolvedCollisions;

    bool iterate;

    tFace f = startF;

    bool checkPreviousFaceEnd = false;
    tPoint::tDistance previousFaceEndN, previousFaceEndD;
    bool previousFaceEndInclusive;

    bool checkFaceEnd;
    tPoint::tDistance faceEndN, faceEndD;
    bool faceEndInclusive;

    do
    {
        if(f != startF)
        {
            checkPreviousFaceEnd = true;
            previousFaceEndN = faceEndN;
            previousFaceEndD = faceEndD;
            previousFaceEndInclusive = faceEndInclusive;
        }

        tFace currentFace = f;
        {
            tEdge edgeCrossed;
            bool exited;
            iterate = TraverseTowardsTarget_SI<tMesh>(f, testLine, testLine.end(), edgeCrossed);
            exited = f->isExternal();

            if(iterate || exited)
            {
                checkFaceEnd = true;
                tIntersection intersection(testLine, GetEdgeLine(edgeCrossed));
                intersection.getAxisFraction(faceEndN,faceEndD);
                faceEndInclusive = EdgeHasPriority_SI(edgeCrossed);
            }
            else
            {
                checkFaceEnd = false;
            }
        }

        // resolve any unresolved collisions in this face
        while(!unresolvedCollisions.empty() &&
            (!checkFaceEnd || unresolvedCollisions.front().isBeforeFaceEnd(faceEndN, faceEndD, faceEndInclusive)))
        {
            const cUnresolvedCollision& toCheck = unresolvedCollisions.front();
            const cLocalisedConvexShape& expansion = shapeSet[toCheck._shapeIndex];
            collidingLineIndex = toCheck.collidingLineIndex;
            if(expansion.overlapsFace(currentFace.index()))
            {
                collidingShape = toCheck._shapeIndex;
                distanceAlong.n = toCheck.n;
                distanceAlong.d = toCheck.d;
                return true;
            }
            unresolvedCollisions.pop_front();
        }

        tSigned32 i;
        for(i = 0; i < shapeSet.shapesInFace(currentFace.index()); i++)
        {
            tSigned32 j = shapeSet.indexForShapeInFace(currentFace.index(), i);

            if(expansionChecked[j])
            {
                continue;
            }
            expansionChecked.setBit(j, true);

            const cLocalisedConvexShape& expansion = shapeSet[j];

            if(!expansion.poly().lineCollides2D(testLine.start(), testLine.axis(), collidingLineIndex))
            {
                continue;
            }

            cUnresolvedCollision toAdd;
            tIntersection intersection(testLine, expansion.poly().getBoundaryLine(collidingLineIndex));
            intersection.getAxisFraction(toAdd.n, toAdd.d);
        
            if(checkPreviousFaceEnd && toAdd.isBeforeFaceEnd(previousFaceEndN, previousFaceEndD, previousFaceEndInclusive))
            {
                continue;
            }

            if(!checkFaceEnd
             || toAdd.isBeforeFaceEnd(faceEndN, faceEndD, faceEndInclusive)
             )
            {
              // collision occurs in this face, so resolve straight away
                if(expansion.overlapsFace(currentFace.index()))
                {
                    collidingShape = j;
                    distanceAlong.n = toAdd.n;
                    distanceAlong.d = toAdd.d;
                    return true;
                }
                continue;
            }

            toAdd._shapeIndex = j;
            toAdd.collidingLineIndex = collidingLineIndex;

            // insert in order

            std::list<cUnresolvedCollision>::iterator insertBefore = unresolvedCollisions.begin();
            while(insertBefore != unresolvedCollisions.end() && toAdd.isAfter(*insertBefore))
            {
                ++insertBefore;
            }
            unresolvedCollisions.insert(insertBefore, toAdd);
        }
    }
    while(iterate);

    endF = f;
    return false;
}




//...... remove repetition of stuff above
bool
TestForSnagsToSegmentEnd_AgainstShapeSet(
        tFace startF, const tLine& testLine,
        const cLocalisedConvexShapeSet& shapeSet,
        const tLine& segmentLine
        )
{
    eSide approximationSide = segmentLine.side(testLine.start());
    if(approximationSide == SIDE_CENTRE)
    {
        return false;
    }
    eSide otherSideFromApproximation = OtherSide(approximationSide);
    bool checkCornerAfter = (otherSideFromApproximation == SIDE_LEFT);

    cBitVector expansionChecked(shapeSet.size(), false);
    std::list<cUnresolvedCollision> unresolvedCollisions;

    bool iterate;

    tFace f = startF;

    bool checkPreviousFaceEnd = false;
    tPoint::tDistance previousFaceEndN, previousFaceEndD;
    bool previousFaceEndInclusive;

    bool checkFaceEnd;
    tPoint::tDistance faceEndN, faceEndD;
    bool faceEndInclusive;

    do
    {
        if(f != startF)
        {
            checkPreviousFaceEnd = true;
            previousFaceEndN = faceEndN;
            previousFaceEndD = faceEndD;
            previousFaceEndInclusive = faceEndInclusive;
        }

        tFace currentFace = f;
        {
            tEdge edgeCrossed;
            bool exited;
            iterate = TraverseTowardsTarget_SI<tMesh>(f, testLine, testLine.end(), edgeCrossed);
            exited = f->isExternal();

            if(iterate || exited)
            {
                checkFaceEnd = true;
                tIntersection intersection(testLine, GetEdgeLine(edgeCrossed));
                intersection.getAxisFraction(faceEndN,faceEndD);
                faceEndInclusive = EdgeHasPriority_SI(edgeCrossed);
            }
            else
            {
                checkFaceEnd = false;
            }
        }

        // resolve any unresolved collisions in this face
        while(!unresolvedCollisions.empty() &&
            (!checkFaceEnd || unresolvedCollisions.front().isBeforeFaceEnd(faceEndN, faceEndD, faceEndInclusive)))
        {
            const cUnresolvedCollision& toCheck = unresolvedCollisions.front();
            const cLocalisedConvexShape& expansion = shapeSet[toCheck._shapeIndex];
            if(expansion.overlapsFace(currentFace.index()))
            {
              // testLine collides with this expansion
              // but check for whether this is a snag
                tLine boundaryLine = expansion.poly().getBoundaryLine(toCheck.collidingLineIndex);
                tPoint candidateSnag;
                if(checkCornerAfter)
                {
                    candidateSnag = boundaryLine.end();
                }
                else
                {
                    candidateSnag = boundaryLine.start();
                }
               if(segmentLine.side(candidateSnag) != otherSideFromApproximation)
               {
                    return true;
               }
            }
            unresolvedCollisions.pop_front();
        }

        tSigned32 i;
        for(i = 0; i < shapeSet.shapesInFace(currentFace.index()); i++)
        {
            tSigned32 j = shapeSet.indexForShapeInFace(currentFace.index(), i);

            if(expansionChecked[j])
            {
                continue;
            }
            expansionChecked.setBit(j, true);

            const cLocalisedConvexShape& expansion = shapeSet[j];

            tSigned32 collidingLineIndex;
            if(!expansion.poly().lineCollides2D(testLine.start(), testLine.axis(), collidingLineIndex))
            {
                continue;
            }
            {
              //.. optimise - this is a bug fix for cases where the snag matches the tests made, but another boundary section crosses the segment line
              //.. look at fixing the snag corner test conditions, instead
                tSigned32 collidingLineIndex_Ignored;
                if(expansion.poly().lineCollides2D(segmentLine.start(), segmentLine.axis(), collidingLineIndex_Ignored))
                {
                    continue;
                }
            }

            cUnresolvedCollision toAdd;
            tIntersection intersection(testLine, expansion.poly().getBoundaryLine(collidingLineIndex));
            intersection.getAxisFraction(toAdd.n, toAdd.d);
        
            if(checkPreviousFaceEnd && toAdd.isBeforeFaceEnd(previousFaceEndN, previousFaceEndD, previousFaceEndInclusive))
            {
                continue;
            }

            if(!checkFaceEnd || toAdd.isBeforeFaceEnd(faceEndN, faceEndD, faceEndInclusive))
            {
              // collision occurs in this face, so resolve straight away
                if(expansion.overlapsFace(currentFace.index()))
                {
                // testLine collides with this expansion
                // but check for whether this is a snag
                    tLine boundaryLine = expansion.poly().getBoundaryLine(collidingLineIndex);
                    tPoint candidateSnag;
                    if(checkCornerAfter)
                    {
                        candidateSnag = boundaryLine.end();
                    }
                    else
                    {
                        candidateSnag = boundaryLine.start();
                    }
                    if(segmentLine.side(candidateSnag) != otherSideFromApproximation)
                    {
                        return true;
                    }
                }
                continue;
            }

            toAdd._shapeIndex = j;
            toAdd.collidingLineIndex = collidingLineIndex;

            // insert in order

            std::list<cUnresolvedCollision>::iterator insertBefore = unresolvedCollisions.begin();
            while(insertBefore != unresolvedCollisions.end() && toAdd.isAfter(*insertBefore))
            {
                ++insertBefore;
            }
            unresolvedCollisions.insert(insertBefore, toAdd);
        }
    }
    while(iterate);
    return false;
}

//...... remove repetition of stuff above
bool
LineCollision_AgainstShapeSet(
        tFace startF, const tLine& testLine,
        const cAxisFraction& startFraction, const cAxisFraction& endFraction,
        const cLocalisedConvexShapeSet& shapeSet,
        tSigned32& collidingShape,
        tSigned32& collidingLineIndex,
        cAxisFraction& distanceAlong
        )
{
    cBitVector expansionChecked(shapeSet.size(), false);
    std::list<cUnresolvedCollision> unresolvedCollisions;

    //.... optimise - the traversal should probably stop at endFraction
    cPoint_PointLike target(testLine.end());
    bool iterate;

    tFace f = startF;

    bool checkPreviousFaceEnd = false;
    tPoint::tDistance previousFaceEndN, previousFaceEndD;
    bool previousFaceEndInclusive;

    bool checkFaceEnd;
    tPoint::tDistance faceEndN, faceEndD;
    bool faceEndInclusive;

    do
    {
        if(f != startF)
        {
            checkPreviousFaceEnd = true;
            previousFaceEndN = faceEndN;
            previousFaceEndD = faceEndD;
            previousFaceEndInclusive = faceEndInclusive;
        }

        tFace currentFace = f;
        {
            tEdge edgeCrossed;
            bool exited;
            iterate = TraverseTowardsTarget_SI<tMesh>(f, testLine, target, edgeCrossed);
            exited = f->isExternal();

            if(iterate || exited)
            {
                checkFaceEnd = true;
                tIntersection intersection(testLine, GetEdgeLine(edgeCrossed));
                intersection.getAxisFraction(faceEndN,faceEndD);
                faceEndInclusive = EdgeHasPriority_SI(edgeCrossed);
            }
            else
            {
                checkFaceEnd = false;
            }
        }

        // resolve any unresolved collisions in this face
        while(!unresolvedCollisions.empty() &&
            (!checkFaceEnd || unresolvedCollisions.front().isBeforeFaceEnd(faceEndN, faceEndD, faceEndInclusive)))
        {
            const cUnresolvedCollision& toCheck = unresolvedCollisions.front();
            const cLocalisedConvexShape& expansion = shapeSet[toCheck._shapeIndex];
            collidingLineIndex = toCheck.collidingLineIndex;
            if(expansion.overlapsFace(currentFace.index()))
            {
                collidingShape = toCheck._shapeIndex;
                distanceAlong.n = toCheck.n;
                distanceAlong.d = toCheck.d;
                return true;
            }
            unresolvedCollisions.pop_front();
        }

        tSigned32 i;
        for(i = 0; i < shapeSet.shapesInFace(currentFace.index()); i++)
        {
            tSigned32 j = shapeSet.indexForShapeInFace(currentFace.index(), i);

            if(expansionChecked[j])
            {
                continue;
            }
            expansionChecked.setBit(j, true);

            const cLocalisedConvexShape& expansion = shapeSet[j];

            if(!expansion.poly().lineCollides2D(testLine.start(), testLine.axis(), collidingLineIndex))
            {
                continue;
            }

            cUnresolvedCollision toAdd;
            tIntersection intersection(testLine, expansion.poly().getBoundaryLine(collidingLineIndex));
            intersection.getAxisFraction(toAdd.n, toAdd.d);
            {
                cAxisFraction asFraction(toAdd.n, toAdd.d);
                if(asFraction < startFraction)
                {
                    continue;
                }
                if(asFraction >= endFraction)
                {
                    continue;
                }
            }
        
            if(checkPreviousFaceEnd && toAdd.isBeforeFaceEnd(previousFaceEndN, previousFaceEndD, previousFaceEndInclusive))
            {
                continue;
            }

            if(!checkFaceEnd || toAdd.isBeforeFaceEnd(faceEndN, faceEndD, faceEndInclusive))
            {
              // collision occurs in this face, so resolve straight away
                if(expansion.overlapsFace(currentFace.index()))
                {
                    collidingShape = j;
                    distanceAlong.n = toAdd.n;
                    distanceAlong.d = toAdd.d;
                    return true;
                }
                continue;
            }

            toAdd._shapeIndex = j;
            toAdd.collidingLineIndex = collidingLineIndex;

            // insert in order

            std::list<cUnresolvedCollision>::iterator insertBefore = unresolvedCollisions.begin();
            while(insertBefore != unresolvedCollisions.end() && toAdd.isAfter(*insertBefore))
            {
                ++insertBefore;
            }
            unresolvedCollisions.insert(insertBefore, toAdd);
        }
    }
    while(iterate);
    return false;
}

bool
CollisionToXY_AgainstShapeSet(
        cQueryContext& qc,
        const cLocalisedConvexShapeSet& shapeSet,
        tFace startF, const tLine& line, tFace& endF
        )
{
    cBitVector expansionChecked(shapeSet.size(), false);

    bool iterate;
    tFace f = startF;
    tPoint lineAxis(line.axis());

    do
    {
        tFace currentFace = f;
        {
            tEdge edgeCrossed;
            iterate = TraverseTowardsTarget_SI<tMesh>(f, line, line.end(), edgeCrossed);
        }

        tSigned32 i;
        for(i = 0; i < shapeSet.shapesInFace(currentFace.index()); i++)
        {
            tSigned32 j = shapeSet.indexForShapeInFace(currentFace.index(), i);
            if(expansionChecked[j])
            {
                continue;
            }
            expansionChecked.setBit(j, true);
            const cLocalisedConvexShape& expansion = shapeSet[j];
            tSigned32 collidingLineIndex;
            if(!expansion.poly().lineCollides2D(line.start(), lineAxis, collidingLineIndex))
            {
                continue;
            }

            tLine intersectingLine = expansion.poly().getBoundaryLine(collidingLineIndex);
            tFace resolvedFace = ResolveFaceForIntersection(startF, line, intersectingLine);
            if(!expansion.overlapsFace(resolvedFace.index()))
            {
                continue;
            }

          // collided with this expansion
            return true;
        }
    }
    while(iterate);

    endF = f;
    return false;
}

bool
CollisionToXY_AgainstShapeSet(
        const cLocalisedConvexShapeSet& shapeSet,
        const cBitVector& shapeIsAttached,
        tFace startF, const tLine& line,
        tSigned32& blockingUnattachedIndex
        )
{
    blockingUnattachedIndex = -1;
    tLine blockingUnattachedIntersectingLine;

    cBitVector expansionChecked(shapeSet.size(), false);

    bool iterate;
    tFace f = startF;
    tPoint lineAxis(line.axis());

    do
    {
        tFace currentFace = f;
        {
            tEdge edgeCrossed;
            iterate = TraverseTowardsTarget_SI<tMesh>(f, line, line.end(), edgeCrossed);
        }

        tSigned32 i;
        for(i = 0; i < shapeSet.shapesInFace(currentFace.index()); i++)
        {
            tSigned32 j = shapeSet.indexForShapeInFace(currentFace.index(), i);
            if(expansionChecked[j])
            {
                continue;
            }
            expansionChecked.setBit(j, true);
            const cLocalisedConvexShape& expansion = shapeSet[j];
            tSigned32 collidingLineIndex;
            if(!expansion.poly().lineCollides2D(line.start(), lineAxis, collidingLineIndex))
            {
                continue;
            }

            tLine intersectingLine = expansion.poly().getBoundaryLine(collidingLineIndex);
            tFace resolvedFace = ResolveFaceForIntersection(startF, line, intersectingLine);
            if(!expansion.overlapsFace(resolvedFace.index()))
            {
                continue;
            }

          // collided with this expansion

            if(shapeIsAttached[j])
            {
                blockingUnattachedIndex = -1;
                return true;
            }

            if(blockingUnattachedIndex != -1
             && line.intersectionOrder_Anticlockwise(blockingUnattachedIntersectingLine, intersectingLine) != SIDE_RIGHT
             )
            {
              // at or after current first collision with unattached
                continue;
            }

            // set or replace first collision with unattached info
            blockingUnattachedIndex = j;
            blockingUnattachedIntersectingLine = intersectingLine;
        }
    }
    while(iterate);

    return blockingUnattachedIndex != -1;
}

bool
PointCollision_AgainstShapeSet(
        const cLocalisedConvexShapeSet& shapeSet,
        const cInternalPosition& p,
        tSigned32& collidingShape
        )
{
    tSigned32 i;
    for(i = 0; i != shapeSet.shapesInFace(p.cell()); i++)
    {
        tSigned32 j = shapeSet.indexForShapeInFace(p.cell(), i);
        if(shapeSet[j].poly().pointCollides2D(p.p))
        {
            collidingShape = j;
            return true;
        }
    }
    return false;
}
bool
PointCollision_AgainstShapeSet(
        const cLocalisedConvexShapeSet& shapeSet,
        tSigned32 faceIndex, const tIntersection& p,
        tSigned32& collidingShape
        )
{
    tSigned32 i;
    for(i = 0; i != shapeSet.shapesInFace(faceIndex); i++)
    {
        tSigned32 j = shapeSet.indexForShapeInFace(faceIndex, i);
        if(shapeSet[j].poly().pointCollides2D(p))
        {
            collidingShape = j;
            return true;
        }
    }
    return false;
}

bool
PointCollision_AgainstShapeSet_IgnoringOneShape(
        const cLocalisedConvexShapeSet& shapeSet,
        const cInternalPosition& p,
        tSigned32 shapeToIgnore
        )
{
    tSigned32 i;
    for(i = 0; i != shapeSet.shapesInFace(p.cell()); i++)
    {
        tSigned32 j = shapeSet.indexForShapeInFace(p.cell(), i);
        if(j == shapeToIgnore)
        {
            continue;
        }
        if(shapeSet[j].testPointCollision(p))
        {
            return true;
        }
    }
    return false;
}

void
FirstCollision_AgainstShapeSet(
        const cLocalisedConvexShapeSet& shapeSet,
        tFace startF, const tLine& line,
        bool& collides,
        cAxisFraction& distanceAlong,
        tLine& collidingLine,
        tSigned32& collidingShape
        )
{
    tFace endF;
    cAxisFraction candidateDistanceAlong;
    tSigned32 candidateLineIndex;
    tSigned32 candidateShape;
    if(LineCollision_AgainstShapeSet(startF, line, shapeSet, candidateShape, candidateLineIndex, candidateDistanceAlong, endF))
    {
        if(!collides || candidateDistanceAlong < distanceAlong)
        {
            collides = true;
            distanceAlong = candidateDistanceAlong;
            collidingShape = candidateShape;
            collidingLine = shapeSet[collidingShape].poly().getBoundaryLine(candidateLineIndex);
        }
    }
}

void
FirstCollision_AgainstShapeSet(
        const cLocalisedConvexShapeSet& shapeSet,
        tFace startF,
        const tLine& line, const cAxisFraction& startFraction, const cAxisFraction& endFraction,
        bool& collides,
        cAxisFraction& distanceAlong,
        tLine& collidingLine,
        tSigned32& collidingShape
        )
{
    cAxisFraction candidateDistanceAlong;
    tSigned32 candidateLineIndex;
    tSigned32 candidateShape;
    if(LineCollision_AgainstShapeSet(startF, line, startFraction, endFraction, shapeSet, candidateShape, candidateLineIndex, candidateDistanceAlong))
    {
        if(!collides || candidateDistanceAlong < distanceAlong)
        {
            collides = true;
            distanceAlong = candidateDistanceAlong;
            collidingShape = candidateShape;
            collidingLine = shapeSet[collidingShape].poly().getBoundaryLine(candidateLineIndex);
        }
    }
}

void
LastCollision_AgainstShapeSet(
        const cLocalisedConvexShapeSet& shapeSet,
        tFace startF, const tLine& testLine, bool& collides, cAxisFraction& distanceAlong
        )
{
    if(shapeSet.empty())
    {
        return;
    }
    if(testLine.start() == testLine.end())
    {
        return;
    }
    std::vector<char> expansionChecked(shapeSet.size(), false);

    cPoint_PointLike target(testLine.end());

    bool iterate;
    tFace f = startF;
    do
    {
        tFace currentFace = f;
        {
            tEdge edgeCrossed;
            iterate = TraverseTowardsTarget_SI<tMesh>(f, testLine, target, edgeCrossed);
        }

        tSigned32 i;
        for(i = 0; i < shapeSet.shapesInFace(currentFace.index()); i++)
        {
            tSigned32 j = shapeSet.indexForShapeInFace(currentFace.index(), i);

            if(expansionChecked[j])
            {
                continue;
            }
            expansionChecked[j] = true;

            const cLocalisedConvexShape& expansion = shapeSet[j];

            tSigned32 collidingLineIndex;
            if(!expansion.poly().lineCollides2D(testLine.start(), testLine.axis(), collidingLineIndex))
            {
                continue;
            }

            tLine intersectingLine = expansion.poly().getBoundaryLine(collidingLineIndex);
            cAxisFraction candidateDistanceAlong(testLine, intersectingLine);
            if(collides && candidateDistanceAlong >= distanceAlong)
            {
                continue;
            }

            tFace faceForIntersection = ResolveFaceForIntersection(startF, testLine, intersectingLine);
            if(!expansion.overlapsFace(faceForIntersection.index()))
            {
                continue;
            }

            collides = true;
            distanceAlong = candidateDistanceAlong;
        }
    }
    while(iterate);
}

void
GetAllShapesOverlapped_AgainstShapeSet(
        const cLocalisedConvexShapeSet& shapeSet,
        const cInternalPosition& p, cBitVector& isOverlapped
        )
{
    tSigned32 i;
    for(i = 0; i != shapeSet.shapesInFace(p.f.index()); i++)
    {
        const cLocalisedConvexShape& expansion = shapeSet.shapeInFace(p.f.index(), i);
        if(expansion.testPointCollision(p))
        {
            isOverlapped.setBit(shapeSet.indexForShapeInFace(p.f.index(), i), true);
        }
    }
}

void
GetShapesPotentiallyOverlappedBy(
        const cLocalisedConvexShapeSet& shapeSet,
        const cLocalisedConvexShape& shape, std::vector<tSigned32>& appendTo
        )
{
    cBitVector referenced(shapeSet.size(), false);
    tSigned32 i;
    for(i = 0; i < SizeL(shape.refFacesOverlapped()); ++i)
    {
        tSigned32 fIndex = shape.refFacesOverlapped()[i];
        tSigned32 j;
        for(j = 0; j < shapeSet.shapesInFace(fIndex); ++j)
        {
            tSigned32 k = shapeSet.indexForShapeInFace(fIndex, j);
            if(referenced[k])
            {
                continue;
            }
            referenced.setBit(k, true);
            if(shape.poly().overlaps2D(shapeSet[k].poly()))
            {
                appendTo.push_back(k);
            }
        }
    }
}
