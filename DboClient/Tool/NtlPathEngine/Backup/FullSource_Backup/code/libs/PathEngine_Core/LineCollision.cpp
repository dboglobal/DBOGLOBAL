//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/LineCollision.h"
#include "libs/Geometry/interface/tLine.h"
#include "libs/Geometry/interface/AxisFraction.h"
#include "libs/Geometry/interface/CompareFractions.h"
#include "libs/Geometry/interface/Point_PointLike.h"
#include "libs/Geometry/interface/Intersection_PointLike.h"
#include "libs/Mesh2D/interface/MeshTraversal.h"
#include "libs/Mesh2D/interface/CircuitElement.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include "ResolveFaceForIntersection.h"
#include "ElementInFaceLookup.h"
#include "BoundaryCrossings2D.h"
#include "ElementCollisionInfo.h"
#include "common/Containers/FlagVector.h"
#include "libs/Instrumentation/interface/Profiler.h"

static tFace
ResolveFaceForHalfLineIntersection(
                tFace traverseFrom,
                const tLine& traverseLine,
                const tLine& intersectingLine)
{
    tEdge edgeCrossed;

    tFace intersectionF = traverseFrom;

    while(1)
    {
        bool exited = !TraverseToExit_SI<tMesh>(traverseFrom, traverseLine, edgeCrossed);
      // check if the intersection occurs before edge crossing or exit
        eSide side = traverseLine.intersectionOrder_Anticlockwise(intersectingLine, GetEdgeLine(edgeCrossed.twin()));
        if(EdgeHasPriority_SI(edgeCrossed))
        {
            if(side != SIDE_RIGHT)
            {
                break;
            }
        }
        else
        {
            if(side == SIDE_LEFT)
            {
                break;
            }
        }
        if(exited)
        {
            return tMesh::endFace(); // intersection occurs after exit
        }
        intersectionF = traverseFrom;
    }
    return intersectionF;
}

bool
TestLineCollisionToXY(
    const std::vector<std::vector<cElementInFaceEntry> >& elementLookup,
    cBitVector& elementBitBuffer,
    tFace startF, const tLine& testLine, tFace& endF)
{
	if(testLine.start() == testLine.end())
	{
		endF = startF;
		return false;
	}
    PROFILER_SCOPE("TestLineCollisionToXY");

    bool iterate, exited;

    cBitVector& elementCrossed2D = elementBitBuffer;

    tFace nextF = startF;
    tEdge edgeCrossedOut = tMesh::endEdge();

    do
    {
        tFace currentF = nextF;
        tEdge edgeCrossedIn = edgeCrossedOut;

        {
            iterate = TraverseTowardsTarget_SI<tMesh>(nextF, testLine, testLine.end(), edgeCrossedOut);
            exited = nextF->isExternal();
            if(!iterate)
            {
                edgeCrossedOut = tMesh::endEdge();
            }
        }

        size_t i;
        for(i = 0; i < elementLookup[currentF.index()].size(); i++)
        {
            const cCircuitElement& el = *(elementLookup[currentF.index()][i].element);
            tSigned32 faceIndexAlongEl = elementLookup[currentF.index()][i].faceIndex;

            if(el.prevIsZeroLength()
             && faceIndexAlongEl == 0
             && LineCrossesBoundaryElement(el.refZeroLengthLine(), el.getPrev()->refLine(), el.refLine(), testLine)
             )
            {
            // this special case doesnt need any further resolution
                return true;
            }

            if( (faceIndexAlongEl > 0 && el.edgeCrossed_DoesntEnter(faceIndexAlongEl - 1) == edgeCrossedIn)
                ||
                (faceIndexAlongEl + 1 < el.facesCrossed() && el.edgeCrossed_DoesntEnter(faceIndexAlongEl).twin() == edgeCrossedIn)
                )
            {
            // this element was in the previous face traversed
            // so a 2d crossing test has been made for this element
            // and entry resolution has been performed
                if(!elementCrossed2D[el.getIndex()])
                {
                    continue;
                }
            }
            else
            {
            // first face in shared sequence for this element
            // perform 2d crossing test and then resolve with respect to entry
                elementCrossed2D.setBit(el.getIndex(), LineCrossesBoundaryElement(el.refLine(), el.refPrevOrZeroLengthLine(), el.refNextOrZeroLengthLine(), testLine));
                if(!elementCrossed2D[el.getIndex()])
                {
                    continue;
                }

                if(edgeCrossedIn.valid())
                {
                    tLine l = GetEdgeLine(edgeCrossedIn);
                    eSide sense = l.side(el.refLine());
                    if(sense == SIDE_CENTRE)
                    {
                    // special case not needing further resolution
                        return true;
                    }

                    // 'sense' determines whether we are interested in a collision
                    // earlier or later in the element's traversal
                    if(sense == SIDE_RIGHT)
                    {
                        if(faceIndexAlongEl + 1 < el.facesCrossed()
                         && el.refLine().side(l.end()) == SIDE_RIGHT)
                        {
                            elementCrossed2D.setBit(el.getIndex(), false);
                            continue;
                        }
                    }
                    else
                    {
                        if(faceIndexAlongEl > 0
                         && el.refLine().side(l.start()) == SIDE_RIGHT)
                        {
                            elementCrossed2D.setBit(el.getIndex(), false);
                            continue;
                        }
                    }
                }
            }

          // testLine crosses el in 2d, and this occurs in or after the current face

            // if testLine stops here then crossing cannot occur after this face
            if(!iterate)
            {
                return true;
            }

            if( (faceIndexAlongEl > 0 && el.edgeCrossed_DoesntEnter(faceIndexAlongEl - 1).twin() == edgeCrossedOut)
                ||
                (faceIndexAlongEl + 1 < el.facesCrossed() && el.edgeCrossed_DoesntEnter(faceIndexAlongEl) == edgeCrossedOut)
                )
            {
            // defer end resolution until the current sequence of shared faces is exited
                continue;
            }

            tLine l = GetEdgeLine(edgeCrossedOut);
            eSide sense = l.side(el.refLine());
            if(sense == SIDE_CENTRE)
            {
            // special case not needing further resolution
                return true;
            }

            // 'sense' determines whether we are interested in a collision
            // earlier or later in the element's traversal
            if(sense == SIDE_RIGHT)
            {
                if(faceIndexAlongEl > 0
                 && el.refLine().side(l.start()) == SIDE_LEFT)
                {
                    continue;
                }
            }
            else
            {
                if(faceIndexAlongEl + 1 < el.facesCrossed()
                 && el.refLine().side(l.start()) == SIDE_LEFT)
                {
                    continue;
                }
            }
            return true;
        }
    }
    while(iterate);

    if(exited)
    {
        return true;
    }

    endF = nextF;
    return false;
}


bool
TestTraversalToXY(
    tFace startF, const tLine& line, tFace& endF)
{
	if(line.start() == line.end())
	{
		endF = startF;
		return false;
	}

    tFace f = startF;
    cPoint_PointLike p(line.end());
    tEdge edgeCrossed;
    bool iterate = TraverseTowardsTarget_SI<tMesh>(f, line, p, edgeCrossed);

    if(f->isExternal())
    {
        return true;
    }

    if(!iterate)
    {
    // the line does not cross any face boundaries
        endF = startF;
        return false;
    }

    while(TraverseTowardsTarget_SI<tMesh>(f, line, p, edgeCrossed))
    {
    }

    if(f->isExternal())
    {
        return true;
    }

    endF = f;
    return false;
}

void
cElementCollisionInfo::getIntersectingLine(const tLine& testLine, tLine& result) const
{
    assertD(element);
    result = element->refLine();
    if(!atCorner)
    {
        return;
    }
    if(testLine.side(result) == SIDE_LEFT)
    {
        return;
    }
    result = element->getStartIntersect_DoesntEnter();
    eSide side = testLine.side(result);
    assertD(side != SIDE_CENTRE);
    if(side != SIDE_LEFT)
    {
        result.reverse();
    }
}

bool
cElementCollisionInfo::isBefore(const cElementCollisionInfo& rhs) const
{
    tPoint::tMultipliedDistance thisMultiplied, rhsMultiplied;
    tPoint::tMultiplier::multiply(n, rhs.d, thisMultiplied);
    tPoint::tMultiplier::multiply(rhs.n, d, rhsMultiplied);
    return thisMultiplied < rhsMultiplied;
}

// =================================

static void
ReplaceIfBefore(const cElementInFaceEntry& e, const tIntersection& intersection, cElementCollisionInfo& info, bool atStart, bool atEnd)
{
    tPoint::tDistance n,d;
    intersection.getAxisFraction(n, d);
    if(info.element)
    {
        tPoint::tMultipliedDistance firstMultiplied, candidateMultiplied;
        tPoint::tMultiplier::multiply(info.n, d, firstMultiplied);
        tPoint::tMultiplier::multiply(n, info.d, candidateMultiplied);
        if(candidateMultiplied < firstMultiplied)
        {
            info.element = 0;
        }
    }
    if(!info.element)
    {
        info.element = e.element;
        info.faceIndex = e.faceIndex;
        info.atCorner = atStart;
        if(atEnd)
        {
            info.element = e.element->getNext();
            info.faceIndex = 0;
            info.atCorner = true;
        }
        info.n = n;
        info.d = d;
    }
}

static void
ReplaceIfBeforeAndResolves(
            tFace startFace,
            const tLine& testLine,
            bool isHalfLine,
            cElementCollisionInfo& info,
            const cElementInFaceEntry& e, bool atStart, bool atEnd)
{
    tPoint::tDistance n,d;
    tIntersection intersection(testLine, e.element->refLine());
    intersection.getAxisFraction(n, d);
    if(info.element)
    {
        tPoint::tMultipliedDistance firstMultiplied, candidateMultiplied;
        tMultiplier::multiply(info.n, d, firstMultiplied);
        tMultiplier::multiply(n, info.d, candidateMultiplied);
        if(tMultiplier::compare(firstMultiplied, candidateMultiplied) != -1)
        {
        // not before current first collision
            return;
        }
    }

    tSigned32 indexForFace;

    // note that startF is used because a crossing may be before entry into the current face
    // this could also be handled with a more specific check
    tFace intersectionF;
    if(isHalfLine)
    {
        intersectionF = ResolveFaceForHalfLineIntersection(startFace, testLine, e.element->refLine());
    }
    else
    {
        intersectionF = ResolveFaceForIntersection(startFace, testLine, e.element->refLine());
    }
    indexForFace = e.element->getFaceIndexForIntersection(testLine);
    if(e.element->faceCrossed(indexForFace) != intersectionF)
    {
        // intersection did not resolve to the same piece of overlapping geometry as the intersected element
        return;
    }

    info.element = e.element;
    info.faceIndex = indexForFace;
    info.atCorner = atStart;
    if(atEnd)
    {
        info.element = e.element->getNext();
        info.faceIndex = 0;
        info.atCorner = true;
    }
    info.n = n;
    info.d = d;
}

static void
ReplaceIfBeforeAndResolves_ZeroLength(
            tFace startFace,
            const tLine& testLine,
            bool isHalfLine,
            cElementCollisionInfo& info,
            const cElementInFaceEntry& e)
{
    tPoint::tDistance n,d;
    tIntersection intersection(testLine, e.element->refZeroLengthLine());
    intersection.getAxisFraction(n, d);
    if(info.element)
    {
        tPoint::tMultipliedDistance firstMultiplied, candidateMultiplied;
        tPoint::tMultiplier::multiply(info.n, d, firstMultiplied);
        tPoint::tMultiplier::multiply(n, info.d, candidateMultiplied);
        if(tMultiplier::compare(firstMultiplied, candidateMultiplied) != -1)
        {
        // not before current first collision
            return;
        }
    }

    // note that startF is used because a crossing may be before entry into the current face
    // this could also be handled with a more specific check
    tFace intersectionF;
    if(isHalfLine)
    {
        intersectionF = ResolveFaceForHalfLineIntersection(startFace, testLine, e.element->refZeroLengthLine());
    }
    else
    {
        intersectionF = ResolveFaceForHalfLineIntersection(startFace, testLine, e.element->refZeroLengthLine());
    }
    if(e.element->faceCrossed(0) != intersectionF)
    {
        // intersection did not resolve to the same piece of overlapping geometry as the intersected element
        return;
    }
    info.element = e.element;
    info.faceIndex = 0;
    info.atCorner = true;
    info.n = n;
    info.d = d;
}

// this is a raycast, so line is treated as a halfline
// startFace need not be right at the start of the line if the beginning of the line is known to be collision free
void
GetTraversalOut(
//static void
//GetTraversalOut_New(
        const std::vector<std::vector<cElementInFaceEntry> >& elementLookup,
        cFlagVector& elementFlags,
        tFace startFace, const tLine& line,
        cEdgesTraversal& traversal,
        cElementCollisionInfo& info,
        eSide bias)
{
    assertD(!info.element);

    traversal.init_InFace(startFace);

    tFace currentF = startFace;

    elementFlags.clear();

    while(1)
    {
        size_t i;
        for(i = 0; i < elementLookup[currentF.index()].size(); i++)
        {
            const cElementInFaceEntry& e = elementLookup[currentF.index()][i];
            const cCircuitElement& el = *e.element;
            if(elementFlags[el.getIndex()])
            {
                continue;
            }
            elementFlags.set(el.getIndex());

            bool atStart, atEnd;
            if(BiasedHalfLineCrossesBoundaryElement(el.refLine(), el.refPrevOrZeroLengthLine(), el.refNextOrZeroLengthLine(), line, bias, atStart, atEnd))
            {
                ReplaceIfBeforeAndResolves(startFace, line, true, info, e, atStart, atEnd);
            }
            else
            if(el.prevIsZeroLength()
             && bias == SIDE_CENTRE
             && HalfLineCrossesBoundaryElement(el.refZeroLengthLine(), el.getPrev()->refLine(), el.refLine(), line, atStart, atEnd))
            {
                ReplaceIfBeforeAndResolves_ZeroLength(startFace, line, true, info, e);
            }
        }

        if(info.element && info.element->faceCrossed(info.faceIndex) == currentF)
        {
            return;
        }
        tEdge edgeCrossed;
        bool notExitedYet = TraverseToExit_SI<tMesh>(currentF, line, edgeCrossed);
        assertD(notExitedYet);

        traversal.pushEdgeCrossing(edgeCrossed);
    }
}

void
FirstLineCollision(
    const std::vector<std::vector<cElementInFaceEntry> >& elementLookup,
    cFlagVector& elementFlags,
    tFace startF, const tLine& line, tFace& endF,
    cElementCollisionInfo& info)
{
    if(info.element)
    {
        cElementCollisionInfo candidateInfo;
        FirstLineCollision(elementLookup, elementFlags, startF, line, endF, candidateInfo);
        if(candidateInfo.element && candidateInfo.isBefore(info))
        {
            info = candidateInfo;
        }
        return;
    }
    PROFILER_SCOPE("FirstLineCollision");

    endF = startF;

    elementFlags.clear();

    cPoint_PointLike target(line.end());
    tEdge edgeCrossed;
    do
    {
        size_t i;
        for(i = 0; i < elementLookup[endF.index()].size(); i++)
        {
            const cElementInFaceEntry& e = elementLookup[endF.index()][i];
            const cCircuitElement& el = *e.element;
            if(elementFlags[el.getIndex()])
            {
                continue;
            }
            elementFlags.set(el.getIndex());

            bool atStart, atEnd;
            if(LineCrossesBoundaryElement(el.refLine(), el.refPrevOrZeroLengthLine(), el.refNextOrZeroLengthLine(), line, atStart, atEnd))
            {
                ReplaceIfBeforeAndResolves(startF, line, false, info, e, atStart, atEnd);
            }
            else
            if(el.prevIsZeroLength()
             && LineCrossesBoundaryElement(el.refZeroLengthLine(), el.getPrev()->refLine(), el.refLine(), line))
            {
                ReplaceIfBeforeAndResolves_ZeroLength(startF, line, false, info, e);
            }
        }
        if(info.element && info.element->faceCrossed(info.faceIndex) == endF)
        {
            return;
        }
    }
    while(TraverseTowardsTarget_SI<tMesh>(endF, line, target, edgeCrossed));

    if(endF->isExternal())
    {
        endF.clear();
    }
}


void
FirstLineCollision(
    const std::vector<std::vector<cElementInFaceEntry> >& elementLookup,
    cFlagVector& elementFlags,
    tFace startF,
    const tLine& line,
    const cAxisFraction& startFraction,
    const cAxisFraction& endFraction,
    tFace& endF,
    cElementCollisionInfo& info)
{
    if(info.element)
    {
        cElementCollisionInfo candidateInfo;
        FirstLineCollision(elementLookup, elementFlags, startF, line, startFraction, endFraction, endF, candidateInfo);
        if(candidateInfo.element && candidateInfo.isBefore(info))
        {
            info = candidateInfo;
        }
        return;
    }
    PROFILER_SCOPE("FirstLineCollision");

    endF = startF;

    elementFlags.clear();

    //.... optimise - the traversal should probably stop at endFraction
    cPoint_PointLike target(line.end());
    tEdge edgeCrossed;
    do
    {
        size_t i;
        for(i = 0; i < elementLookup[endF.index()].size(); i++)
        {
            const cElementInFaceEntry& e = elementLookup[endF.index()][i];
            const cCircuitElement& el = *e.element;
            if(elementFlags[el.getIndex()])
            {
                continue;
            }
            elementFlags.set(el.getIndex());

            bool atStart, atEnd;
            if(LineCrossesBoundaryElement(el.refLine(), el.refPrevOrZeroLengthLine(), el.refNextOrZeroLengthLine(), line,
                startFraction, endFraction,
                atStart, atEnd))
            {
                ReplaceIfBeforeAndResolves(startF, line, false, info, e, atStart, atEnd);
            }
            else
            if(el.prevIsZeroLength()
             && LineCrossesBoundaryElement(el.refZeroLengthLine(), el.getPrev()->refLine(), el.refLine(), line,
                startFraction, endFraction
                ))
            {
                ReplaceIfBeforeAndResolves_ZeroLength(startF, line, false, info, e);
            }
        }
        if(info.element && info.element->faceCrossed(info.faceIndex) == endF)
        {
            return;
        }
    }
    while(TraverseTowardsTarget_SI<tMesh>(endF, line, target, edgeCrossed));

    if(endF->isExternal())
    {
        endF.clear();
    }
}



//==========================

static void
ReplaceIfAfter(const cElementInFaceEntry& e, const tIntersection& intersection, cElementCollisionInfo& info, bool atStart, bool atEnd)
{
    tPoint::tDistance n,d;
    intersection.getAxisFraction(n, d);
    if(info.element)
    {
        tPoint::tMultipliedDistance firstMultiplied, candidateMultiplied;
        tPoint::tMultiplier::multiply(info.n, d, firstMultiplied);
        tPoint::tMultiplier::multiply(n, info.d, candidateMultiplied);
        if(candidateMultiplied > firstMultiplied)
        {
            info.element = 0;
        }
    }
    if(!info.element)
    {
        info.element = e.element;
        info.faceIndex = e.faceIndex;
        info.atCorner = atStart;
        if(atEnd)
        {
            info.element = e.element->getNext();
            info.faceIndex = 0;
            info.atCorner = true;
        }
        info.n = n;
        info.d = d;
    }
}

static void
ReplaceIfAfterAndResolves(
            tFace startFace,
            const tLine& testLine,
            bool isHalfLine,
            cElementCollisionInfo& info,
            const cElementInFaceEntry& e, bool atStart, bool atEnd)
{
    tPoint::tDistance n,d;
    tIntersection intersection(testLine, e.element->refLine());
    intersection.getAxisFraction(n, d);
    if(info.element)
    {
        tPoint::tMultipliedDistance firstMultiplied, candidateMultiplied;
        tMultiplier::multiply(info.n, d, firstMultiplied);
        tMultiplier::multiply(n, info.d, candidateMultiplied);
        if(tMultiplier::compare(firstMultiplied, candidateMultiplied) != 1)
        {
        // not after current first collision
            return;
        }
    }

    tSigned32 indexForFace;

    // note that startF is used because a crossing may be before entry into the current face
    // this could also be handled with a more specific check
    tFace intersectionF;
    if(isHalfLine)
    {
        intersectionF = ResolveFaceForHalfLineIntersection(startFace, testLine, e.element->refLine());
    }
    else
    {
        intersectionF = ResolveFaceForIntersection(startFace, testLine, e.element->refLine());
    }
    indexForFace = e.element->getFaceIndexForIntersection(testLine);
    if(e.element->faceCrossed(indexForFace) != intersectionF)
    {
        // intersection did not resolve to the same piece of overlapping geometry as the intersected element
        return;
    }

    info.element = e.element;
    info.faceIndex = indexForFace;
    info.atCorner = atStart;
    if(atEnd)
    {
        info.element = e.element->getNext();
        info.faceIndex = 0;
        info.atCorner = true;
    }
    info.n = n;
    info.d = d;
}

static void
ReplaceIfAfterAndResolves_ZeroLength(
            tFace startFace,
            const tLine& testLine,
            bool isHalfLine,
            cElementCollisionInfo& info,
            const cElementInFaceEntry& e)
{
    tPoint::tDistance n,d;
    tIntersection intersection(testLine, e.element->refZeroLengthLine());
    intersection.getAxisFraction(n, d);
    if(info.element)
    {
        tPoint::tMultipliedDistance firstMultiplied, candidateMultiplied;
        tPoint::tMultiplier::multiply(info.n, d, firstMultiplied);
        tPoint::tMultiplier::multiply(n, info.d, candidateMultiplied);
        if(tMultiplier::compare(firstMultiplied, candidateMultiplied) != 1)
        {
        // not after current first collision
            return;
        }
    }

    // note that startF is used because a crossing may be before entry into the current face
    // this could also be handled with a more specific check
    tFace intersectionF;
    if(isHalfLine)
    {
        intersectionF = ResolveFaceForHalfLineIntersection(startFace, testLine, e.element->refZeroLengthLine());
    }
    else
    {
        intersectionF = ResolveFaceForHalfLineIntersection(startFace, testLine, e.element->refZeroLengthLine());
    }
    if(e.element->faceCrossed(0) != intersectionF)
    {
        // intersection did not resolve to the same piece of overlapping geometry as the intersected element
        return;
    }
    info.element = e.element;
    info.faceIndex = 0;
    info.atCorner = true;
    info.n = n;
    info.d = d;
}

void
LastLineCollision(
    const std::vector<std::vector<cElementInFaceEntry> >& elementLookup,
    cFlagVector& elementFlags,
    tFace startF, const tLine& line, tFace& endF,
    cElementCollisionInfo& info)
{
    if(info.element)
    {
        cElementCollisionInfo candidateInfo;
        LastLineCollision(elementLookup, elementFlags, startF, line, endF, candidateInfo);
        if(candidateInfo.element && info.isBefore(candidateInfo))
        {
            info = candidateInfo;
        }
        return;
    }

    endF = startF;

    elementFlags.clear();

    cPoint_PointLike target(line.end());
    tEdge edgeCrossed;
    do
    {
        size_t i;
        for(i = 0; i < elementLookup[endF.index()].size(); i++)
        {
            const cElementInFaceEntry& e = elementLookup[endF.index()][i];
            const cCircuitElement& el = *e.element;
            if(elementFlags[el.getIndex()])
            {
                continue;
            }
            elementFlags.set(el.getIndex());

            bool atStart, atEnd;
            if(LineCrossesBoundaryElement(el.refLine(), el.refPrevOrZeroLengthLine(), el.refNextOrZeroLengthLine(), line, atStart, atEnd))
            {
                ReplaceIfAfterAndResolves(startF, line, false, info, e, atStart, atEnd);
            }
            else
            if(el.prevIsZeroLength()
             && LineCrossesBoundaryElement(el.refZeroLengthLine(), el.getPrev()->refLine(), el.refLine(), line)
             )
            {
                ReplaceIfAfterAndResolves_ZeroLength(startF, line, false, info, e);
            }
        }
    }
    while(TraverseTowardsTarget_SI<tMesh>(endF, line, target, edgeCrossed));

    if(info.element)
    {
        endF = info.element->faceCrossed(info.faceIndex);
    }
    else
    if(endF->isExternal())
    {
        endF.clear();
    }
}




class cElementCrossingInfo
{
    bool _crossingFound;
    bool _firstIsEntry;
    tPoint::tDistance _firstN, _firstD;
    tFace _resolvedFace;

public:

    cElementCrossingInfo()
    {
        _crossingFound = false;
    }

    void replaceIfBeforeAndResolves(
            tFace startFace,
            const tLine& testLine, // this is treated as a half line
            const cCircuitElement& crossedElement, bool crossingIsEntry
            );
    void replaceIfBeforeAndResolves_ZeroLength(
            tFace startFace,
            const tLine& testLine, // this is treated as a half line
            const cCircuitElement& crossedElement, bool crossingIsEntry
            );

    bool crossingFoundAndResolvedInThisFace(tFace f) const
    {
        return _crossingFound && _resolvedFace == f;
    }
    bool crossingFound() const
    {
        return _crossingFound;
    }
    bool crossingFoundWasEntry() const
    {
        assertD(_crossingFound);
        return _firstIsEntry;
    }
};

void
cElementCrossingInfo::replaceIfBeforeAndResolves(
            tFace startFace,
            const tLine& testLine, // this is treated as a half line
            const cCircuitElement& crossedElement, bool crossingIsEntry
            )
{
    tPoint::tDistance n,d;
    tIntersection intersection(testLine, crossedElement.refLine());
    intersection.getAxisFraction(n, d);
    if(_crossingFound)
    {
        tPoint::tMultipliedDistance firstMultiplied, candidateMultiplied;
        tMultiplier::multiply(_firstN, d, firstMultiplied);
        tMultiplier::multiply(n, _firstD, candidateMultiplied);
        tSigned32 comparison = tMultiplier::compare(firstMultiplied, candidateMultiplied);
        if(comparison == 1)
        {
        // after current first
            return;
        }
        if(comparison == 0)
        {
        // exits happen just before entries in the case where they intersect at exactly the same point
        // so replace only if candidate is an exit and current first is not
            if(crossingIsEntry || !_firstIsEntry)
            {
                return;
            }
        }
    }

    // note that startF is used because a crossing may be before entry into the current face
    // this could also be handled with a more specific check
    tSigned32 indexForFace;
    tFace intersectionF;
    if(crossingIsEntry)
    {
        intersectionF = ResolveFaceForHalfLineIntersection(startFace, testLine, crossedElement.refLine());
        indexForFace = crossedElement.getFaceIndexForIntersection(testLine);
    }
    else
    {
        intersectionF = ResolveFaceForHalfLineIntersection(startFace, testLine, crossedElement.refLine().reversed());
        indexForFace = crossedElement.getFaceIndexForIntersection(testLine.reversed());
    }
    if(crossedElement.faceCrossed(indexForFace) != intersectionF)
    {
        // intersection did not resolve to the same piece of overlapping geometry as the intersected element
        return;
    }

    _crossingFound = true;
    _firstIsEntry = crossingIsEntry;
    _firstN = n;
    _firstD = d;
    _resolvedFace = intersectionF;
}

void
cElementCrossingInfo::replaceIfBeforeAndResolves_ZeroLength(
            tFace startFace,
            const tLine& testLine, // this is treated as a half line
            const cCircuitElement& crossedElement, bool crossingIsEntry
            )
{
    tPoint::tDistance n,d;
    tIntersection intersection(testLine, crossedElement.refZeroLengthLine());
    intersection.getAxisFraction(n, d);
    if(_crossingFound)
    {
        tPoint::tMultipliedDistance firstMultiplied, candidateMultiplied;
        tMultiplier::multiply(_firstN, d, firstMultiplied);
        tMultiplier::multiply(n, _firstD, candidateMultiplied);
        tSigned32 comparison = tMultiplier::compare(firstMultiplied, candidateMultiplied);
        if(comparison == 1)
        {
        // after current first
            return;
        }
        if(comparison == 0)
        {
        // exits happen just before entries in the case where they intersect at exactly the same point
        // so replace only if candidate is an exit and current first is not
            if(crossingIsEntry || !_firstIsEntry)
            {
                return;
            }
        }
    }

    // note that startF is used because a crossing may be before entry into the current face
    // this could also be handled with a more specific check
    tFace intersectionF;
    if(crossingIsEntry)
    {
        intersectionF = ResolveFaceForHalfLineIntersection(startFace, testLine, crossedElement.refZeroLengthLine());
    }
    else
    {
        intersectionF = ResolveFaceForHalfLineIntersection(startFace, testLine, crossedElement.refZeroLengthLine().reversed());
    }
    if(crossedElement.faceCrossed(0) != intersectionF)
    {
        // intersection did not resolve to the same piece of overlapping geometry as the intersected element
        return;
    }

    _crossingFound = true;
    _firstIsEntry = crossingIsEntry;
    _firstN = n;
    _firstD = d;
    _resolvedFace = intersectionF;
}

template <class tPoint> void
FirstElementCrossing_T(
    const std::vector<std::vector<cElementInFaceEntry> >& elementLookup,
    cFlagVector& elementFlags,
    tFace startFace, const tLine& line, const tPoint& startPoint,
    bool& exitedMesh, bool& isEntry
    )
{
    cElementCrossingInfo info;

    tEdge edgeCrossed;

    tFace currentF = startFace;
    tLine reversedLine(line.end(), line.start());
    elementFlags.clear();
    do
    {
        size_t i;
    // exit checks must come first because a line may both exit and enter at a concave corner
    // or exit one element at the same time as entering another
    // (this is now taken care of internally by cElementCrossingInfo)
        for(i = 0; i < elementLookup[currentF.index()].size(); i++)
        {
            const cElementInFaceEntry& e = elementLookup[currentF.index()][i];
            const cCircuitElement& el = *e.element;
            if(elementFlags[el.getIndex()])
            {
                continue;
            }
            elementFlags.set(el.getIndex());

            bool atStart, atEnd;
            if(InfiniteLineCrossesBoundaryElement(el.refLine(), el.refPrevOrZeroLengthLine(), el.refNextOrZeroLengthLine(), line, atStart, atEnd)
             && el.refLine().side(startPoint) != SIDE_RIGHT
             )
            {                
                info.replaceIfBeforeAndResolves(startFace, line, el, true);
            }
            else
            if(el.prevIsZeroLength()
             && InfiniteLineCrossesBoundaryElement(el.refZeroLengthLine(), el.getPrev()->refLine(), el.refLine(), line, atStart, atEnd)
             && el.refZeroLengthLine().side(startPoint) != SIDE_RIGHT
             )
            {
                info.replaceIfBeforeAndResolves_ZeroLength(startFace, line, el, true);
            }
            else
            if(InfiniteLineCrossesBoundaryElement(el.refLine(), el.refPrevOrZeroLengthLine(), el.refNextOrZeroLengthLine(), reversedLine, atStart, atEnd)
             && el.refLine().side(startPoint) == SIDE_RIGHT
             )
            {   
                info.replaceIfBeforeAndResolves(startFace, line, el, false);
            }
            else
            if(el.prevIsZeroLength()
             && InfiniteLineCrossesBoundaryElement(el.refZeroLengthLine(), el.getPrev()->refLine(), el.refLine(), reversedLine, atStart, atEnd)
             && el.refZeroLengthLine().side(startPoint) == SIDE_RIGHT
             )
            {
                info.replaceIfBeforeAndResolves_ZeroLength(startFace, line, el, false);
            }
        }

        if(info.crossingFoundAndResolvedInThisFace(currentF))
        {
            exitedMesh = false;
            isEntry = info.crossingFoundWasEntry();
            return;
        }
    }
    while(TraverseToExit_SI<tMesh>(currentF, line, edgeCrossed));
    assertD(!info.crossingFound());
    exitedMesh = true;
}

void
FirstElementCrossing(
    const std::vector<std::vector<cElementInFaceEntry> >& elementLookup,
    cFlagVector& elementFlags,
    tFace startFace, const tLine& line,
    bool& exitedMesh, bool& isEntry)
{
    FirstElementCrossing_T(elementLookup, elementFlags, startFace, line, line.start(), exitedMesh, isEntry);
}

void
FirstElementCrossing(
    const std::vector<std::vector<cElementInFaceEntry> >& elementLookup,
    cFlagVector& elementFlags,
    tFace startFace, const tLine& line, const tIntersection& startPointOnLine,
    bool& exitedMesh, bool& isEntry)
{
    FirstElementCrossing_T(elementLookup, elementFlags, startFace, line, startPointOnLine, exitedMesh, isEntry);
}



bool
FirstElementCrossed_FirstFace(
    const std::list<cCutRegistryEntry>& elements,
    const tLine& line,
    const tPoint::tDistance& startN,
    const tPoint::tDistance& startD,
    bool& enters)
{
    bool collides = false;
    tPoint::tDistance firstN, firstD;
    tPoint::tDistance n,d;

    tLine reversedLine = line.reversed();

    std::list<cCutRegistryEntry>::const_iterator i;
    // exit checks must come first because a line may both exit and enter at a concave corner
    // or exit one element at the same time as entering another
    for(i = elements.begin(); i != elements.end(); ++i)
    {
        bool atStart, atEnd;
        const tLine* intersect;
        if(i->element->lineEntersInFaceOrZeroLength(i->faceIndex, reversedLine, atStart, atEnd, intersect))
        {
            tIntersection intersection(line, *intersect);
            intersection.getAxisFraction(n, d);
            if(CompareFractions(startN, startD, n, d) == 1)
            {
                if(collides)
                {
                    tSigned32 comparison = CompareFractions(firstN, firstD, n, d);
                    assertD(comparison != 0);
                    if(comparison == -1)
                    {
                        collides = false;
                    }
                }
                if(!collides)
                {
                    enters = false;
                    collides = true;
                    firstN = n;
                    firstD = d;
                }
            }
        }
    }
    for(i = elements.begin(); i != elements.end(); ++i)
    {
        bool atStart, atEnd;
        const tLine* intersect;
        if(i->element->lineEntersInFaceOrZeroLength(i->faceIndex, line, atStart, atEnd, intersect))
        {
            tIntersection intersection(line, *intersect);
            intersection.getAxisFraction(n, d);
            if(CompareFractions(startN, startD, n, d) != -1)
            {
                if(collides)
                {
                    tSigned32 comparison = CompareFractions(firstN, firstD, n, d);
                    if(comparison == -1)
                    {
                        collides = false;
                    }
                }
                if(!collides)
                {
                    enters = true;
                    collides = true;
                    firstN = n;
                    firstD = d;
                }
            }
        }
    }
    return collides;

}

bool
FirstElementCrossed_MidFace(
    const std::list<cCutRegistryEntry>& elements,
    const tLine& line,
    bool& enters)
{
    bool collides = false;
    tPoint::tDistance firstN, firstD;
    tPoint::tDistance n,d;

    tLine reversedLine = line.reversed();

    std::list<cCutRegistryEntry>::const_iterator i;
    // exit checks must come first because a line may both exit and enter at a concave corner
    // or exit one element at the same time as entering another
    for(i = elements.begin(); i != elements.end(); ++i)
    {
        bool atStart, atEnd;
        const tLine* intersect;
        if(i->element->lineEntersInFaceOrZeroLength(i->faceIndex, reversedLine, atStart, atEnd, intersect))
        {
            tIntersection intersection(line, *intersect);
            intersection.getAxisFraction(n, d);
            if(collides)
            {
                tSigned32 comparison = CompareFractions(firstN, firstD, n, d);
                assertD(comparison != 0);
                if(comparison == -1)
                {
                    collides = false;
                }
            }
            if(!collides)
            {
                enters = false;
                collides = true;
                firstN = n;
                firstD = d;
            }
        }
    }
    for(i = elements.begin(); i != elements.end(); ++i)
    {
        bool atStart, atEnd;
        const tLine* intersect;
        if(i->element->lineEntersInFaceOrZeroLength(i->faceIndex, line, atStart, atEnd, intersect))
        {
            tIntersection intersection(line, *intersect);
            intersection.getAxisFraction(n, d);
            if(collides)
            {
                tSigned32 comparison = CompareFractions(firstN, firstD, n, d);
                if(comparison == -1)
                {
                    collides = false;
                }
            }
            if(!collides)
            {
                enters = true;
                collides = true;
                firstN = n;
                firstD = d;
            }
        }
    }
    return collides;
}

bool
ExitsThroughExternalSection(
                        tEdge exit,
                        const tLine& exitLine,
                        const tMeshCutSequence& circuit
                        )
{
    if(CircuitIsInternal(circuit))
    {
        return false;
    }
    assertD(!CircuitIsInternal(circuit));
    tMeshCutSequence::const_iterator i;
    for(i = circuit.begin(); i != circuit.end(); ++i)
    {
        tMeshCutSequence::const_iterator next_i = i;
        ++next_i;
        if(next_i == circuit.end())
        {
            next_i = circuit.begin();
        }
        const cCircuitElement* before = (*i)->getStartElement()->getPrev();
        const cCircuitElement* after = (*next_i)->getStartElement();
        if(before->exitExitsThroughExternalSection(exit, exitLine, *after))
        {
            return true;
        }
    }
    return false;
}

bool
ExitsThroughExternalSection(
                        tEdge exit,
                        const tLine& exitLine,
                        const tMeshCircuits& circuits
                        )
{
    tMeshCircuits::const_iterator j;
    for(j = circuits.begin(); j != circuits.end(); ++j)
    {
        if(ExitsThroughExternalSection(exit, exitLine, **j))
        {
            return true;
        }
    }
    return false;
}



bool
TestForSnagsToSegmentEnd(
        const std::vector<std::vector<cElementInFaceEntry> >& elementLookup,
        cFlagVector& elementFlags,
        tFace startF, const tLine& line,
        const tLine& segmentLine
        )
{
    tFace endF = startF;

    eSide approximationSide = segmentLine.side(line.start());
    if(approximationSide == SIDE_CENTRE)
    {
        return false;
    }
    eSide otherSideFromApproximation = OtherSide(approximationSide);
    bool checkCornerAfter = (otherSideFromApproximation == SIDE_LEFT);

    elementFlags.clear();

    cPoint_PointLike target(line.end());
    tEdge edgeCrossed;
    do
    {
        size_t i;
        for(i = 0; i < elementLookup[endF.index()].size(); i++)
        {
            const cElementInFaceEntry& e = elementLookup[endF.index()][i];
            const cCircuitElement& el = *e.element;
            if(elementFlags[el.getIndex()])
            {
                continue;
            }
            elementFlags.set(el.getIndex());

            if(checkCornerAfter)
            {
                if(el.exits() || !el.getNext()->cornerIsConvex())
                {
                // only convex corners can snag
                    continue;
                }
            }
            else
            {
                if(el.enters() || !el.cornerIsConvex())
                {
                // only convex corners can snag
                    continue;
                }
            }

            bool atStart, atEnd;
            if(LineCrossesBoundaryElement(el.refLine(), el.refPrevOrZeroLengthLine(), el.refNextOrZeroLengthLine(), line, atStart, atEnd))
            {
                if(ResolveFaceForIntersection(startF, line, el.refLine()).valid())
                {
                  // line collides with this element
                  // but check if this is a snag
                    if(checkCornerAfter)
                    {
                        if(segmentLine.side(el.getNext()->cornerPoint()) != otherSideFromApproximation)
                        {
                            return true;
                        }
                    }
                    else
                    {
                        if(segmentLine.side(el.cornerPoint()) != otherSideFromApproximation)
                        {
                            return true;
                        }
                    }
                }
            }
            // (note that zero length sections are ignored here, since these cant snag)
        }
    }
    while(TraverseTowardsTarget_SI<tMesh>(endF, line, target, edgeCrossed));
    return false;
}
