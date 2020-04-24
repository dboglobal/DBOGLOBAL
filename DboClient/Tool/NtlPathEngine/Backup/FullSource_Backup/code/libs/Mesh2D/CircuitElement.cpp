//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "HighestCommonFactor.h"
#include "libs/Mesh2D/interface/CircuitElement.h"
#include "libs/Mesh2D/interface/MeshTraversal.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include "libs/Mesh_Common/interface/Vertex.h"
#include "libs/Geometry/interface/InsideCorner.h"
#include "libs/Geometry/interface/tIntersection.h"
#include "libs/Geometry/interface/CompareFractions.h"
#include "libs/Geometry/interface/Intersection_PointLike.h"
#include "platform_common/CoordinateRanges.h"
#include "common/interface/Error.h"
#include "common/STL/utility.h"

void
bCircuitElementTraversal::init(const tLine& line, cEdgesTraversal& traversal)
{
    assertD(cEdgesTraversal::empty());
    cEdgesTraversal::swap(traversal);
    _line = line;
}

void
bCircuitElementTraversal::init_InSingleFace(const tLine& line, tFace f)
{
    assertD(cEdgesTraversal::empty());
    _line = line;
    cEdgesTraversal::init_InFace(f);
}
void
bCircuitElementTraversal::init_ZeroLength(const tLine& line)
{
    assertD(cEdgesTraversal::empty());
    _line = line;
}
void
bCircuitElementTraversal::init_AsClone(const bCircuitElementTraversal& cloneOf)
{
    assertD(cEdgesTraversal::empty());
    _line = cloneOf._line;
    cEdgesTraversal::init_AsClone(cloneOf);
}
void
bCircuitElementTraversal::init_AsCloneOfPart(const bCircuitElementTraversal& toClone, tSigned32 firstFace, tSigned32 lastFace)
{
    assertD(cEdgesTraversal::empty());
    _line = toClone._line;
    cEdgesTraversal::init_InFace(toClone.faceCrossed(firstFace));
    tSigned32 i = firstFace;
    while(i != lastFace)
    {
        pushEdgeCrossing(toClone.edgeCrossed(i));
        i++;
    }
}
void
bCircuitElementTraversal::init_ForAfterSplit(const bCircuitElementTraversal& toClone, tSigned32 firstFace)
{
    assertD(cEdgesTraversal::empty());
    _line = toClone._line;
    cEdgesTraversal::init_InFace(toClone.faceCrossed(firstFace));

    // encode edges crossed
    tSigned32 i = firstFace;
    while(i != toClone.facesCrossed() - 1)
    {
        pushEdgeCrossing(toClone.edgeCrossed(i));
        i++;
    }

    if(toClone.exits())
    {
        pushExit(toClone.exitEdge());
    }
}

bool
bCircuitElementTraversal::isValid_SI() const
{
    if(isZeroLength())
    {
        Error("NonFatal", "Circuit element validation: called for an internal element. Validation fails.");
        return false;
    }
    if(refLine().start() == refLine().end())
    {
        Error("NonFatal", "Circuit element validation: Line is zero length. Validation fails.");
        return false;
    }

    if(!cEdgesTraversal::isValid())
    {
        Error("NonFatal", "Circuit element validation: Invalid edges traversal. Validation fails.");
        return false;
    }
    {
        tFace startF = faceCrossed(0);
        tEdge entryEdge;
        if(!InfiniteLineEntersFace_SI<tMesh>(startF, refLine(), entryEdge))
        {
            Error("NonFatal", "Circuit element validation: line does not cross first face. Validation fails.");
            return false;
        }
        if(enters())
        {
            if(entryEdge != cEdgesTraversal::entryEdge())
            {
                Error("NonFatal", "Circuit element validation: line does not enter here. Validation fails.");
                return false;
            }
        }
        tSigned32 i;
        for(i = 0; i < edgesCrossed(); i++)
        {
            tEdge e = edgeCrossed(i);
            if(SideOfLine_SI(refLine(), e.vertex()) != SIDE_LEFT || SideOfLine_SI(refLine(), e.next().vertex()) != SIDE_RIGHT)
            {
                Error("NonFatal", "Circuit element validation: traversal does not agree with edge crossed. Validation fails.");
                return false;
            }
        }
        if(exits())
        {
            tEdge e = exitEdge();
            bool agrees = (SideOfLine_SI(refLine(), e.vertex()) == SIDE_LEFT && SideOfLine_SI(refLine(), e.next().vertex()) == SIDE_RIGHT);
            if(!agrees)
            {
                Error("NonFatal", "Circuit element validation: line does not exit here. Validation fails.");
                return false;
            }
        }
    }

    return true;
}


void
bCircuitElementTraversal::truncate(tSigned32 indexForLastFace)
{
    assertD(indexForLastFace >= 0 && indexForLastFace < facesCrossed());
    if(exits())
    {
        popExit();
    }
    while(indexForLastFace + 1 < facesCrossed())
    {
        popEdgeCrossing();
    }
}

void
bCircuitElementTraversal::reverse()
{
    cEdgesTraversal::reverse();
    _line.reverse();
}

void
bCircuitElementTraversal::remakeTraversal(tEdge newEntry)
{
    assertD(enters() && exits());

    cEdgesTraversal::clear();
    cEdgesTraversal::init_Entry(newEntry);

    tEdge crossed;
    tFace f = newEntry.face();

    while(TraverseToExit_SI<tMesh>(f, refLine(), crossed))
    {
        cEdgesTraversal::pushEdgeCrossing(crossed);
    }
    cEdgesTraversal::pushExit(crossed);
}

void
bCircuitElementTraversal::remakeTraversal(tEdge newEntry, const tLine& intersect)
{
    assertD(enters() && !exits());

    cEdgesTraversal::clear();
    cEdgesTraversal::init_Entry(newEntry);

    tEdge crossed;
    tFace f = newEntry.face();

    tIntersection intersection(_line, intersect);
    cIntersection_PointLike p(intersection);
    while(TraverseToTarget_SI<tMesh>(f, refLine(), p, crossed))
    {
        cEdgesTraversal::pushEdgeCrossing(crossed);
    }
}

void
bCircuitElementTraversal::remakeTraversal(tFace newStartFace)
{
    assertD(!enters() && exits());

    cEdgesTraversal::clear();
    cEdgesTraversal::init_InFace(newStartFace);

    tEdge crossed;
    tFace f = newStartFace;

    while(TraverseToExit_SI<tMesh>(f, refLine(), crossed))
    {
        cEdgesTraversal::pushEdgeCrossing(crossed);
    }
    cEdgesTraversal::pushExit(crossed);
}

void
bCircuitElementTraversal::remakeTraversal(tFace newStartFace, const tLine& intersect)
{
    assertD(!enters() && !exits());

    cEdgesTraversal::clear();
    cEdgesTraversal::init_InFace(newStartFace);

    tEdge crossed;
    tFace f = newStartFace;

    tIntersection intersection(_line, intersect);
    cIntersection_PointLike p(intersection);
    while(TraverseToTarget_SI<tMesh>(f, refLine(), p, crossed))
    {
        cEdgesTraversal::pushEdgeCrossing(crossed);
    }
}

void
bCircuitElementTraversal::normaliseLine_Internal(const tLine& lineBefore, const tLine& lineAfter)
{
    tPoint axis = _line.axis();
    axis /= HighestCommonFactor(axis.getX(), axis.getY());
    assertD(_line.axis().direction(axis) == DIRECTION_FORWARD);

    eSide sideStart = lineBefore.side(_line);
    assertD(sideStart != SIDE_CENTRE);
    eSide sideEnd = _line.side(lineAfter);
    assertD(sideEnd != SIDE_CENTRE);

    tPoint start = _line.start();
    while(lineBefore.side(start) != sideStart)
    {
        start = tPoint(start.getX() + axis.getX(), start.getY() + axis.getY(), WORLD_RANGE + SHAPE_RANGE);
    }
    while(lineBefore.side(start) == sideStart)
    {
        start = tPoint(start.getX() - axis.getX(), start.getY() - axis.getY(), WORLD_RANGE + SHAPE_RANGE);
    }

    tPoint end = _line.end();
    while(lineAfter.side(end) != sideEnd)
    {
        end = tPoint(end.getX() - axis.getX(), end.getY() - axis.getY(), WORLD_RANGE + SHAPE_RANGE);
    }
    while(lineAfter.side(end) == sideEnd)
    {
        end = tPoint(end.getX() + axis.getX(), end.getY() + axis.getY(), WORLD_RANGE + SHAPE_RANGE);
    }

    if(start == end)
    {
        assertD(isZeroLength());
        //.. should the zero length be replaced with a point in this case?
        start = tPoint(start.getX() - axis.getX(), start.getY() - axis.getY(), WORLD_RANGE + SHAPE_RANGE);
        end = tPoint(end.getX() + axis.getX(), end.getY() + axis.getY(), WORLD_RANGE + SHAPE_RANGE);
    }

    _line = tLine(start, end);
    assertD(_line.axis().direction(axis) == DIRECTION_FORWARD);
}

void
bCircuitElementTraversal::addTraversal(const bCircuitElementTraversal &element2)
{
    assertD(!isZeroLength());
    assertD(!element2.isZeroLength());
    assertD(!exits());
    assertD(!element2.enters());
    tSigned32 i;
    for(i = 0; i < element2.edgesCrossed(); i++)
    {
        cEdgesTraversal::pushEdgeCrossing(element2.edgeCrossed(i));
    }
    if(element2.exits())
    {
        cEdgesTraversal::pushExit(element2.exitEdge());
    }
}

tLine
bCircuitElementTraversal::getEntryIntersect(bool &inclusive) const
{
    assertD(enters());
    inclusive = (SideOfEdge_SI(entryEdge()) == SIDE_RIGHT);
    return GetEdgeLine(entryEdge().twin());
}
tLine
bCircuitElementTraversal::getExitIntersect(bool &inclusive) const
{
    assertD(exits());
    inclusive = (SideOfEdge_SI(exitEdge()) == SIDE_RIGHT);
    return GetEdgeLine(exitEdge());
}

// ====================== cCircuitElement

void
cCircuitElement::unlink()
{
    if(next)
    {
        next->prev=0;
    }
    next=0;
    if(prev)
    {
        prev->next=0;
    }
    prev=0;
}
void
cCircuitElement::unlinkStart()
{
    assertD(prev);
    prev->next=0;
    prev=0;
}
void
cCircuitElement::unlinkEnd()
{
    assertD(next);
    next->prev=0;
    next=0;
}
void
cCircuitElement::connectTo_Internal(cCircuitElement& target)
{
    next = &target;
    target.prev = this;
}

bool
cCircuitElement::validationCommon() const
{
    if(!exits() && next)
    {
//        if(!next)
//        {
//            Error("NonFatal", "cCircuitElement validation: element doesn't exit but has zero next pointer. Validation fails.");
//            return false;
//        }
        if(refLine().side(next->refLine()) == SIDE_CENTRE)
        {
            Error("NonFatal", "cCircuitElement validation: next is SIDE_CENTRE. Validation fails.");
            return false;
        }
        if(next->isZeroLength() && next->next)
        {
//            if(!next->next)
//            {
//                Error("NonFatal", "cCircuitElement validation: next is marked as zero length, but there is no next->next. Validation fails.");
//            }
            if(next->refLine().side(next->next->refLine()) == SIDE_CENTRE)
            {
                Error("NonFatal", "cCircuitElement validation: next->next is SIDE_CENTRE to next. Validation fails.");
                return false;
            }
            tIntersection i1(refLine(), next->refLine());
            tIntersection i2(next->refLine(), next->next->refLine());
            if(i1 != i2)
            {
                Error("NonFatal", "cCircuitElement validation: next is marked as zero length, but is not. Validation fails.");
            }
        }
    }
    if(!enters() && prev)
    {
//        if(!prev)
//        {
//            Error("NonFatal", "cCircuitElement validation: element doesn't enter but has zero prev pointer. Validation fails.");
//            return false;
//        }
        if(refLine().side(prev->refLine()) == SIDE_CENTRE)
        {
            Error("NonFatal", "cCircuitElement validation: prev is SIDE_CENTRE. Validation fails.");
            return false;
        }
//        if(prev->isZeroLength())
//        {
//            if(!prev->prev)
//            {
//                Error("NonFatal", "cCircuitElement validation: prev is marked as zero length, but there is no prev->prev. Validation fails.");
//            }
//        }
    }
   
    if(!enters())
    {
        if(getPrev() && faceCrossed(0) != getPrev()->lastFaceCrossed())
        {
            Error("NonFatal", "cCircuitElement validation: Start face of element does not agree with end of previous. Validation fails.");
            return false;
        }
    }
    return true;
}

cCircuitElement::~cCircuitElement()
{
    assertD(getNext() == 0);
    assertD(getPrev() == 0);
    if(next)
    {
        assertD(!isZeroLength());
        assertD(next->isZeroLength());
        next->removeAndDelete();
    }
    if(prev)
    {
        assertD(!isZeroLength());
        assertD(prev->isZeroLength());
        prev->removeAndDelete();
    }
}

void
cCircuitElement::init(const tLine& line, cEdgesTraversal& traversal)
{
    bCircuitElementTraversal::init(line, traversal);
    next = 0;
    prev = 0;
}
void
cCircuitElement::init_InSingleFace(const tLine& line, tFace f)
{
    bCircuitElementTraversal::init_InSingleFace(line, f);
    next = 0;
    prev = 0;
}


void
cCircuitElement::init_WithStartCap(const tLine& line, const tLine& startCap, cEdgesTraversal& traversal)
{
    assertD(!traversal.enters());
    bCircuitElementTraversal::init(line, traversal);
    next = 0;
    cCircuitElement* zeroLength = new cCircuitElement;
    zeroLength->init_ZeroLength(startCap);
    prev = zeroLength;
    zeroLength->next = this;
    zeroLength->prev = 0;
}
void
cCircuitElement::init_WithEndCap(const tLine& line, const tLine& endCap, cEdgesTraversal& traversal)
{
    assertD(!traversal.exits());
    bCircuitElementTraversal::init(line, traversal);
    prev = 0;
    cCircuitElement* zeroLength = new cCircuitElement;
    zeroLength->init_ZeroLength(endCap);
    next = zeroLength;
    zeroLength->prev = this;
    zeroLength->next = 0;
}
void
cCircuitElement::init_WithStartAndEndCaps(const tLine& line, const tLine& startCap, const tLine& endCap, cEdgesTraversal& traversal)
{
    assertD(!traversal.enters());
    assertD(!traversal.exits());
    bCircuitElementTraversal::init(line, traversal);
    cCircuitElement* zeroLength;
    zeroLength = new cCircuitElement;
    zeroLength->init_ZeroLength(startCap);
    prev = zeroLength;
    zeroLength->next = this;
    zeroLength->prev = 0;
    zeroLength = new cCircuitElement;
    zeroLength->init_ZeroLength(endCap);
    next = zeroLength;
    zeroLength->prev = this;
    zeroLength->next = 0;
}

void
cCircuitElement::init_AsClone(const cCircuitElement& toClone)
{
    bCircuitElementTraversal::init_AsClone(toClone);
    next=0;
    prev=0;
}

void
cCircuitElement::init_AsCloneOfPart(const cCircuitElement& toClone, tSigned32 firstFace, tSigned32 lastFace)
{
    assertD(!toClone.enters() && !toClone.exits());

    bCircuitElementTraversal::init_AsCloneOfPart(toClone, firstFace, lastFace);

    if(firstFace == 0)
    {
      // start is capped
        cCircuitElement* startCap = new cCircuitElement();
        startCap->init_ZeroLength(toClone.getStartIntersect_DoesntEnter());
        startCap->prev = 0;
        startCap->next = this;
        prev = startCap;
    }
    else
    {
        prev = 0;
    }

    if(lastFace == facesCrossed() - 1)
    {
      // end is capped
        cCircuitElement* endCap = new cCircuitElement();
        endCap->init_ZeroLength(toClone.getEndIntersect_DoesntExit());
        next = endCap;
        endCap->prev = this;
        endCap->next = 0;
    }
    else
    {
        next = 0;
    }
}

void
cCircuitElement::connectTo(cCircuitElement& element2)
{
    assertD(getNext() == 0);
    assertD(element2.getPrev() == 0);

    if(exits())
    {
        assertD(element2.enters());
        assertD(next == 0);
        assertD(element2.prev == 0);
      // Note - no assert about external faces being identical, because they may not be when closing single cuts
        next = &element2;
        element2.prev = this;
        return;
    }
    assertD(!element2.enters());
    assertD(lastFaceCrossed() == element2.faceCrossed(0));

    if(refLine().side(element2.refLine()) == SIDE_CENTRE)
    {
      // you can't connect inline elements without one of them being capped
        assertD(next || element2.prev);
        if(next)
        {
            if(element2.prev)
            {
                element2.prev->removeAndDelete();
            }
            next->connectTo_Internal(element2);
        }
        else
        {
            connectTo_Internal(*element2.prev);
        }
        if(refLine().direction_KnownToBeInline(element2.refLine()) == DIRECTION_BACK)
        {
        // direction of zero length line must be consistent in this case
            if(refLine().side(next->refLine()) == SIDE_RIGHT)
            {
                next->reverseZeroLength();
            }
        }
        return;
    }

    if(next)
    {
        next->removeAndDelete();
    }
    if(element2.prev)
    {
        element2.prev->removeAndDelete();
    }
    connectTo_Internal(element2);
}

//void
//cCircuitElement::operator>>(iSerialiser& serialiser)
//{
//    serialiser.child(refLine());
//    serialiser.child(_edges);
//    if(serialiser.isConstructing())
//    {
//        isZeroLength() = false;
//        bool nextIsZeroLength;
//        serialiser.child(nextIsZeroLength);
//        if(nextIsZeroLength)
//        {
//            next = new cCircuitElement;
//            next->isZeroLength() = true;
//            serialiser.child(next->refLine());
//            serialiser.child(next->_edges);
//            next->prev = this;
//        }
//    }
//    else
//    {
//        assertD(next); // not valid to serialise unconnected elements
//        assertD(prev);
//        assertD(isZeroLength() == false);
//        serialiser.child(next->isZeroLength());
//        if(next->isZeroLength())
//        {
//            serialiser.child(next->refLine());
//            serialiser.child(next->_edges);
//        }
//    }
//}
//void
//cCircuitElement::remapConnection(cCircuitElement& e1, cCircuitElement& e2)
//{
//    cCircuitElement* toConnect = &e1;
//    if(e1.next)
//    {
//      // there is a zero length element between
//        toConnect = e1.next;
//    }
//    toConnect->next = &e2;
//    e2.prev = toConnect;
//}


void
cCircuitElement::disconnectStart()
{
    cCircuitElement* before = prev;
    cCircuitElement* startCap;
    cCircuitElement* endCap;
    if(before->isZeroLength())
    {
        startCap = before;
        before = before->prev;
        endCap = new cCircuitElement();
        endCap->init_ZeroLength(startCap->refLine());
    }
    else
    {
        startCap = new cCircuitElement();
        startCap->init_ZeroLength(before->refLine());
        endCap = new cCircuitElement();
        endCap->init_ZeroLength(refLine());
    }
    prev = startCap;
    startCap->next = this;
    startCap->prev = 0;

    before->next = endCap;
    endCap->prev = before;
    endCap->next = 0;
}
void
cCircuitElement::disconnectStart_LeaveUncapped()
{
    cCircuitElement* before = prev;
    if(before->isZeroLength())
    {
        cCircuitElement* zeroLength = before;
        before = before->prev;
        zeroLength->prev = 0;
        zeroLength->next = 0;
        delete zeroLength;
    }
    before->next = 0;
    prev = 0;
}
void
cCircuitElement::capEnd(const tLine& capLine)
{
    assertD(!next);
    assertD(refLine().side(capLine) != SIDE_CENTRE);
    next = new cCircuitElement();
    next->init_ZeroLength(capLine);
    next->prev = this;
    next->next = 0;
}

bool cCircuitElement::operator==(const cCircuitElement& rhs) const
{
    if(refLine().side(rhs.refLine()) != SIDE_CENTRE)
    {
        return false;
    }
    if(refLine().side(rhs.refLine().start()) != SIDE_CENTRE)
    {
        return false;
    }
    if(refLine().direction_KnownToBeInline(rhs.refLine()) != DIRECTION_FORWARD)
    {
        return false;
    }
    if(enters() != rhs.enters())
    {
        return false;
    }
    if(enters())
    {
        if(entryEdge() != rhs.entryEdge())
        {
            return false;
        }
    }
    if(exits() != rhs.exits())
    {
        return false;
    }
    if(exits())
    {
        if(exitEdge() != rhs.exitEdge())
        {
            return false;
        }
    }
    if(facesCrossed() != rhs.facesCrossed())
    {
        return false;
    }
    tSigned32 i;
    for(i = 0; i < facesCrossed(); i++)
    {
        if(faceCrossed(i) != rhs.faceCrossed(i))
        {
            return false;
        }
    }
    return true;
}

void
cCircuitElement::removeAndDelete()
{
    if(next)
    {
        next->prev=prev;
    }
    if(prev)
    {
        prev->next=next;
    }
    prev=0;
    next=0;
    delete this;
}

void
cCircuitElement::split(tSigned32 faceIndexForSplit, const tLine& intersectLine)
{
    cCircuitElement* elementAfter = new cCircuitElement;
    elementAfter->init_ForAfterSplit(*this, faceIndexForSplit);
    elementAfter->_index = _index;

    truncate(faceIndexForSplit);

    cCircuitElement* zeroLength = new cCircuitElement;
    zeroLength->init_ZeroLength(intersectLine);

    elementAfter->next = next;
    if(next)
    {
        next->prev = elementAfter;
    }

    next = zeroLength;
    zeroLength->prev = this;

    zeroLength->next = elementAfter;
    elementAfter->prev = zeroLength;

    assertD(lastFaceCrossed() == elementAfter->faceCrossed(0));
}

void
cCircuitElement::join()
{
    assertD(!exits());
    assertD(next);
    assertD(next->isZeroLength());
    assertD(next->next);
    addTraversal(next->next);
    next->removeAndDelete();
    next->removeAndDelete();
}

void
cCircuitElement::reverse()
{
    bCircuitElementTraversal::reverse();
    std::swap(next, prev);
}

void
cCircuitElement::redirect(cCircuitElement& e2)
{
    assertD(!exits());
    assertD(!e2.exits());
    cCircuitElement* zeroLength = 0;
    if(refLine().side(e2.getNext()->refLine()) == SIDE_CENTRE)
    {
        zeroLength = new cCircuitElement;
        // ensure that zero length for DIRECTION_BACK corners goes in the right direction
        if(refLine().side(next->refLine()) == SIDE_LEFT)
        {
            zeroLength->init_ZeroLength(next->refLine());
        }
        else
        {
            assertD(refLine().side(next->refLine()) == SIDE_RIGHT);
            zeroLength->init_ZeroLength(next->refLine().reversed());
        }
    }
    cCircuitElement* zeroLength2 = 0;
    if(e2.refLine().side(getNext()->refLine()) == SIDE_CENTRE)
    {
        zeroLength2 = new cCircuitElement;
        // ensure that zero length for DIRECTION_BACK corners goes in the right direction
        if(e2.refLine().side(e2.next->refLine()) == SIDE_LEFT)
        {
            zeroLength2->init_ZeroLength(e2.next->refLine());
        }
        else
        {
            assertD(e2.refLine().side(e2.next->refLine()) == SIDE_RIGHT);
            zeroLength2->init_ZeroLength(e2.next->refLine().reversed());
        }
    }

    if(next->isZeroLength())
    {
        next->removeAndDelete();
    }
    if(e2.next->isZeroLength())
    {
        e2.next->removeAndDelete();
    }

    cCircuitElement* nextAfterRedirect = e2.next;
    cCircuitElement* nextAfterRedirect2 = next;
    unlinkEnd();
    e2.unlinkEnd();

    if(zeroLength)
    {
        zeroLength->connectTo_Internal(*nextAfterRedirect);
        nextAfterRedirect = zeroLength;
    }
    connectTo_Internal(*nextAfterRedirect);

    if(zeroLength2)
    {
        zeroLength2->connectTo_Internal(*nextAfterRedirect2);
        nextAfterRedirect2 = zeroLength2;
    }
    e2.connectTo_Internal(*nextAfterRedirect2);
}



void
cCircuitElement::markAsZeroLength()
{
    assertD(next);
    assertD(prev);
    assertD(!enters());
    assertD(!exits());
    assertD(!isZeroLength());
    assertD(!next->isZeroLength());
    assertD(!prev->isZeroLength());
    cEdgesTraversal::clear();
}

void
cCircuitElement::normaliseLine()
{
    assertD(!enters());
    assertD(!exits());
    normaliseLine_Internal(prev->refLine(), next->refLine());
    if(next->isZeroLength())
    {
        next->normaliseLine_Internal(refLine(), next->next->refLine());
    }
}

void
cCircuitElement::remakeTraversal(tEdge newEntry)
{
    if(exits())
    {
        bCircuitElementTraversal::remakeTraversal(newEntry);
    }
    else
    {
        bCircuitElementTraversal::remakeTraversal(newEntry, next->refLine());
    }
}
void
cCircuitElement::remakeTraversal(tFace newStartFace)
{
    if(exits())
    {
        bCircuitElementTraversal::remakeTraversal(newStartFace);
    }
    else
    {
        bCircuitElementTraversal::remakeTraversal(newStartFace, next->refLine());
    }
}

bool
cCircuitElement::cornerIsInline() const
{
    if(!getPrev() || enters())
    {
        return false;
    }
    return 
        getPrev()->refLine().side(refLine().start()) == SIDE_CENTRE
        &&
        getPrev()->refLine().direction(refLine()) == DIRECTION_FORWARD;
}

tLine
cCircuitElement::getStartIntersect_DoesntEnter() const
{
    assertD(!enters());
    eSide side = refLine().side(prev->refLine());
    assertD(side != SIDE_CENTRE);
    if(side==SIDE_RIGHT)
        return prev->refLine();
    return prev->refLine().reversed();
}
tLine
cCircuitElement::getStartIntersect(bool &inclusive) const
{
    if(enters())
    {
        return getEntryIntersect(inclusive);
    }
    inclusive = false;
    return getStartIntersect_DoesntEnter();
}
tLine 
cCircuitElement::getStartIntersect() const
{
    if(enters())
    {
        bool inclusive;
        return getEntryIntersect(inclusive);
    }
    return getStartIntersect_DoesntEnter();
}
tLine 
cCircuitElement::getEndIntersect_DoesntExit() const
{
    assertD(!exits());
    eSide side = refLine().side(next->refLine());
    assertD(side != SIDE_CENTRE);
    if(side == SIDE_RIGHT)
    {
        return next->refLine();
    }
    return next->refLine().reversed();
}
tLine 
cCircuitElement::getEndIntersect(bool &inclusive) const
{
    if(exits())
    {
        return getExitIntersect(inclusive);
    }
    inclusive=false;
    return getEndIntersect_DoesntExit();
}
tLine 
cCircuitElement::getEndIntersect() const
{
    if(exits())
    {
        bool inclusive;
        return getExitIntersect(inclusive);
    }
    return getEndIntersect_DoesntExit();
}

void
cCircuitElement::getStartFraction(tPoint::tDistance& n, tPoint::tDistance& d) const
{
    tIntersection i(refLine(), getStartIntersect_DoesntEnter());
    i.getAxisFraction(n, d);
}
void
cCircuitElement::getEndFraction(tPoint::tDistance& n, tPoint::tDistance& d) const
{
    tIntersection i(refLine(), getEndIntersect_DoesntExit());
    i.getAxisFraction(n, d);
}
void
cCircuitElement::getStartOrEntryFraction(tPoint::tDistance& n, tPoint::tDistance& d) const
{
    tIntersection i(refLine(), getStartIntersect());
    i.getAxisFraction(n, d);
}
void
cCircuitElement::getEndOrExitFraction(tPoint::tDistance& n, tPoint::tDistance& d) const
{
    tIntersection i(refLine(), getEndIntersect());
    i.getAxisFraction(n, d);
}



tSigned32
cCircuitElement::getFaceIndexForIntersection(const tLine& intersectingLine) const
{
    bool inclusive;
    tLine base;
    eSide side;

    base=getStartIntersect(inclusive);
    if(!enters())
    {
        inclusive = true;     // return a face even if intersection is on an adjoining corner
    }
    side = refLine().intersectionOrder_Clockwise(base, intersectingLine);
    if(!inclusive && side == SIDE_CENTRE)
    {
        side = SIDE_RIGHT;
    }
    if(side == SIDE_RIGHT)
    {
        return -1;
    }

    base = getEndIntersect(inclusive);
    if(!exits())
    {
        inclusive = true;     // return a face even if intersection is on an adjoining corner
    }
    side = refLine().intersectionOrder_Clockwise(base, intersectingLine);
    if(!inclusive && side == SIDE_CENTRE)
    {
        side = SIDE_LEFT;
    }
    if(side == SIDE_LEFT)
    {
        return -1;
    }

    tSigned32 i;
    for(i = 0; i < edgesCrossed(); i++)
    {
        tEdge e = edgeCrossed(i);
        inclusive = EdgeHasPriority_SI(e);
        side = refLine().intersectionOrder_Clockwise(GetEdgeLine(e), intersectingLine);
        if(inclusive && side == SIDE_CENTRE)
        {
            side = SIDE_RIGHT;
        }
        if(side == SIDE_RIGHT)
        {
            break;
        }
    }
    return i;
}

tSigned32
cCircuitElement::getFaceIndexForIntersection(const tLine& intersectingLine, bool& atCorner) const
{
    atCorner = false;

    bool inclusive;
    tLine base;
    eSide side;

    base=getStartIntersect(inclusive);
    if(!enters())
    {
        inclusive = true;     // return a face even if intersection is on an adjoining corner
    }
    side = refLine().intersectionOrder_Clockwise(base, intersectingLine);
    if(side == SIDE_CENTRE)
    {
        if(!inclusive)
        {
            return -1;
        }
        atCorner = true;
    }
    if(side == SIDE_RIGHT)
    {
        return -1;
    }

    base = getEndIntersect(inclusive);
    if(!exits())
    {
        inclusive = true;     // return a face even if intersection is on an adjoining corner
    }
    side = refLine().intersectionOrder_Clockwise(base, intersectingLine);
    if(side == SIDE_CENTRE)
    {
        if(!inclusive)
        {
            return -1;
        }
        atCorner = true;
    }
    else if(side == SIDE_LEFT)
    {
        return -1;
    }

    tSigned32 i;
    for(i = 0; i < edgesCrossed(); i++)
    {
        tEdge e = edgeCrossed(i);
        inclusive = EdgeHasPriority_SI(e);
        side = refLine().intersectionOrder_Clockwise(GetEdgeLine(e), intersectingLine);
        if(inclusive && side == SIDE_CENTRE)
        {
            side = SIDE_RIGHT;
        }
        if(side == SIDE_RIGHT)
        {
            break;
        }
    }
    return i;
}

tFace
cCircuitElement::getFaceForIntersection(const tLine& intersectingLine) const
{
    tSigned32 i = getFaceIndexForIntersection(intersectingLine);
    if(i == -1)
    {
        return tMesh::endFace();
    }
    return faceCrossed(i);
}


tPoint
cCircuitElement::cornerPoint() const
{
    assertD(!enters());
    return tIntersection(refLine(), getStartIntersect_DoesntEnter()).point();
}

void cCircuitElement::getIntersectionForCornerAtStart(tIntersection& result) const
{
    assertD(!enters());
    result=tIntersection(refLine(),getStartIntersect_DoesntEnter());
}
void
cCircuitElement::nextCornerIntersection(tIntersection& result) const
{
    assertD(!exits());
    result = tIntersection(refLine(), getEndIntersect_DoesntExit());
}

const tLine& cCircuitElement::getIntersectAtCorner(const tLine &line) const
{
    assertD(!enters());
    if(line.side(refLine()) != SIDE_CENTRE)
        return refLine();
	return prev->refLine();
}

bool
cCircuitElement::isValid_SI() const
{
    if(!bCircuitElementTraversal::isValid_SI())
    {
        return false;
    }
    if(isZeroLength())
    {
        Error("NonFatal", "cCircuitElement validation: Element passed in is marked as zero length. Validation fails.");
        return false;
    }
    if(!validationCommon())
    {
        return false;
    }
    if(!enters() && prev)
    {
        tIntersection corner;
        cornerIntersection(corner);
        if(!PointIsInFace<tMesh>(faceCrossed(0), corner))
        {
            Error("NonFatal", "cCircuitElement validation: Corner is not in start face. Validation fails.");
            return false;
        }
    }
    if(!enters() && !exits() && prev && next)
    {
        eSide side = refLine().intersectionOrder_Clockwise(getStartIntersect_DoesntEnter(), getEndIntersect_DoesntExit());
        if(side != SIDE_LEFT)
        {
            Error("NonFatal", "cCircuitElement validation: End intersect is not after start intersect. Validation fails.");
            return false;
        }
    }
    if(getPrev())
    {
        assertD(getPrev() != this);
    }
    if(prev && prev->isZeroLength() && prev->prev)
    {
        if(refLine().direction(getPrev()->refLine()) == DIRECTION_BACK)
        {
            if(refLine().side(prev->refLine()) != SIDE_RIGHT)
            {
                Error("NonFatal", "Circuit element validation: bad zero length section for DIRECTION_BACK corner. Validation fails.");
                return false;
            }
        }
    }
    return true;
}


//****************************
// collision stuff

bool
cCircuitElement::exitExitsThroughExternalSection(
                    tEdge exitEE,
                    const tLine& exitLine,
                    const cCircuitElement& next
                    ) const
{
    tEdge startEE, endEE;
    startEE = exitEdge();
    if(startEE.twin().face() != exitEE.twin().face())
    {
        return false;
    }
    endEE = next.entryEdge();

    if(startEE == endEE)
    {
        if(EntryIsAfterExit(startEE, refLine(), next.refLine()))
        {
        // external section is in the middle of this element edge
            if(exitEE != startEE)
            {
                return false;
            }
            return
                SideOfExit(exitEE, refLine(), exitLine) == SIDE_RIGHT
                &&
                SideOfEntry(exitEE, next.refLine(), exitLine) == SIDE_LEFT;
        }

        // external section is outer part of this element edge

        if(exitEE != startEE)
        {
            return true;
        }
        return
            SideOfExit(exitEE, refLine(), exitLine) == SIDE_RIGHT
            ||
            SideOfEntry(exitEE, next.refLine(), exitLine) == SIDE_LEFT;
    }

    if(exitEE == startEE)
    {
        return SideOfExit(exitEE, refLine(), exitLine) == SIDE_RIGHT;
    }
    if(exitEE == endEE)
    {
        return SideOfEntry(exitEE, next.refLine(), exitLine) == SIDE_LEFT;
    }

    startEE = startEE.twin().prev().twin();
    while(startEE != endEE)
    {
        if(exitEE == startEE)
        {
            return true;
        }
        startEE = startEE.twin().prev().twin();
    }
    return false;
}



bool
cCircuitElement::elementEntersThroughThis(
            tSigned32 thisElementFaceIndex,
            const cCircuitElement &testElement,
            tSigned32 testElementFaceIndex,
            bool& atStartOfThis, bool& atEndOfThis,
            bool& atStartOfTest
            ) const
{
    tSigned32 faces = testElement.facesCrossed();
    bool checkStart = (testElementFaceIndex == 0 && !testElement.enters());
    bool checkEnd = (testElementFaceIndex == faces - 1 && !testElement.exits());
    const tLine* intersect;
    if(checkStart || checkEnd)
    {
    // we need distance to intersection
        if(!lineEntersInFaceOrZeroLength(thisElementFaceIndex, testElement.refLine(),
                                atStartOfThis, atEndOfThis, intersect))
        {
            return false;
        }
        tPoint::tDistance n,d;
        if(checkStart || checkEnd)
        {
            tIntersection intersection(testElement.refLine(), *intersect);
            intersection.getAxisFraction(n, d);
        }
        if(checkStart)
        {
        // no crossing if the intersection occurred before the start of the test element
            //.... TODO optimise - don't calculate this fraction lots of times
            tPoint::tDistance startN, startD;
            testElement.getStartFraction(startN, startD);
            tSigned32 comparison =  CompareFractions(startN, startD, n, d);
            if(comparison == 0)
            {
                atStartOfTest = true;
                return true;
            }
            if(comparison == -1)
            {
                return false;
            }
        }
        if(checkEnd)
        {
        // no crossing if the intersection occurred exactly at or after the end of the test element
            //.... TODO optimise - don't calculate this fraction lots of times
            tPoint::tDistance endN, endD;
            testElement.getEndFraction(endN, endD);
            if(CompareFractions(endN, endD, n, d) != -1)
            {
                return false;
            }
        }
        atStartOfTest = false;
        return true;
    }
    if(lineEntersInFaceOrZeroLength(thisElementFaceIndex, testElement.refLine(),
                                atStartOfThis, atEndOfThis, intersect))
    {
    // mid face, testElement line can be treated as an infinite line
        atStartOfTest = false;
        return true;
    }
    return false;
}

bool
cCircuitElement::elementExitsThroughThis(
            tSigned32 thisElementFaceIndex,
            const cCircuitElement &testElement,
            tSigned32 testElementFaceIndex,
            bool& atStartOfThis, bool& atEndOfThis,
            bool& atEndOfTest
            ) const
{
    tSigned32 faces = testElement.facesCrossed();
    bool checkStart = (testElementFaceIndex == 0 && !testElement.enters());
    bool checkEnd = (testElementFaceIndex == faces - 1 && !testElement.exits());
    const tLine* intersect;
    tLine reversedLine = testElement.refLine().reversed();
    if(checkStart || checkEnd)
    {
    // we need distance to intersection
        if(!lineEntersInFaceOrZeroLength(thisElementFaceIndex, reversedLine,
                        atStartOfThis, atEndOfThis, intersect))
        {
            return false;
        }
        tPoint::tDistance n,d;
        if(checkStart || checkEnd)
        {
            tIntersection intersection(testElement.refLine(), *intersect);
            intersection.getAxisFraction(n, d);
        }
        if(checkStart)
        {
        // no crossing if the intersection occurred before or exactly at the start of the test element
            //.... TODO optimise - don't calculate this fraction lots of times
            tPoint::tDistance startN, startD;
            testElement.getStartFraction(startN, startD);
            if(CompareFractions(startN, startD, n, d) != 1)
            {
                return false;
            }
        }
        if(checkEnd)
        {
        // no crossing if the intersection occurred after the end of the test element
            //.... TODO optimise - don't calculate this fraction lots of times
            tPoint::tDistance endN, endD;
            testElement.getEndFraction(endN, endD);
            tSigned32 comparison = CompareFractions(endN, endD, n, d);
            if(comparison == 0)
            {
                atEndOfTest = true;
                return true;
            }
            if(comparison == 1)
            {
                return false;
            }
        }
        atEndOfTest = false;
        return true;
    }
    if(lineEntersInFaceOrZeroLength(thisElementFaceIndex, reversedLine,
                    atStartOfThis, atEndOfThis, intersect))
    {
    // mid face, testElement line can be treated as an infinite line
        atEndOfTest = false;
        return true;
    }
    return false;
}


bool
cCircuitElement::elementEntersThroughThis(
            const cCircuitElement& testElement,
            bool& atStartOfThis, bool& atEndOfThis,
            bool& atStartOfTest,
            tSigned32& thisFaceIndex, tSigned32& testFaceIndex
            ) const
{
    tSigned32 faces = testElement.facesCrossed();
    for(testFaceIndex = 0; testFaceIndex < faces; testFaceIndex++)
    {
        tFace face = testElement.faceCrossed(testFaceIndex);
        thisFaceIndex = indexForFace(face);
        if(thisFaceIndex == -1)
        {
            continue;
        }
        if(elementEntersThroughThis(thisFaceIndex, testElement, testFaceIndex,
                        atStartOfThis, atEndOfThis, atStartOfTest))
        {
            return true;
        }
    }
    return false;
}
bool
cCircuitElement::elementExitsThroughThis(
            const cCircuitElement& testElement,
            bool& atStartOfThis, bool& atEndOfThis,
            bool& atEndOfTest,
            tSigned32& thisFaceIndex, tSigned32& testFaceIndex
            ) const
{
    tSigned32 faces = testElement.facesCrossed();
    for(testFaceIndex = 0; testFaceIndex < faces; testFaceIndex++)
    {
        tFace face = testElement.faceCrossed(testFaceIndex);
        thisFaceIndex = indexForFace(face);
        if(thisFaceIndex == -1)
        {
            continue;
        }
        if(elementExitsThroughThis(thisFaceIndex, testElement, testFaceIndex,
                        atStartOfThis, atEndOfThis, atEndOfTest))
        {
            return true;
        }
    }
    return false;
}

void
cCircuitElement::getFaceBounds(tSigned32 faceIndex,
                    bool& startInclusive,
                    tLine& startIntersect,
                    bool& endInclusive,
                    tLine& endIntersect
                    ) const
{
    if(faceIndex == 0)
    {
        startIntersect = getStartIntersect(startInclusive);
    }
    else
    {
        tEdge e = edgeCrossed(faceIndex - 1);
        startInclusive = EdgeHasPriority_SI(e.twin());
        startIntersect = GetEdgeLine(e);
    }
    if(faceIndex == facesCrossed()-1)
    {
        endIntersect = getEndIntersect(endInclusive);
    }
    else
    {
        tEdge e = edgeCrossed(faceIndex);
        endInclusive = EdgeHasPriority_SI(e);
        endIntersect = GetEdgeLine(e);
    }
}

bool
cCircuitElement::lineEntersZeroLength(const tLine& line) const
{
    if(prev->isZeroLength() && getPrev()->refLine().direction_KnownToBeInline(refLine()) == DIRECTION_BACK)
    {
        if(line.side(prev->refLine()) == SIDE_LEFT)
        {
            tIntersection intersection(refLine(), prev->refLine());
            if(line.side(intersection) == SIDE_CENTRE)
            {
                return true;
            }
        }
    }
    return false;
}

bool
cCircuitElement::lineEntersInFace_New(
        tSigned32 faceIndex, const tLine& line, bool& atStartCorner, bool& atEndCorner
        ) const
{
    if(line.side(refLine()) != SIDE_LEFT)
    {
        return false;
    }

    bool startInclusive;
    tLine startIntersect;
    bool endInclusive;
    tLine endIntersect;
    getFaceBounds(faceIndex, startInclusive, startIntersect, endInclusive, endIntersect);

    eSide side;

    side=refLine().intersectionOrder_Clockwise(startIntersect,line);
    if(side==SIDE_CENTRE)
    {
        atStartCorner = true;
        atEndCorner = false;
        if(faceIndex == 0 && !enters())
        {
            if(prev->isZeroLength())
            {
                if(getPrev()->refLine().direction_KnownToBeInline(refLine()) == DIRECTION_FORWARD)
                {
                    return true;
                }
                return false;
            }
            else
            if(line.side(getPrev()->refLine()) == SIDE_LEFT
                ||
                getPrev()->refLine().side(refLine()) == SIDE_LEFT)
            {
                return true;
            }
        }
        if(!startInclusive)
        {
            return false;
        }
    }
    if(side==SIDE_RIGHT)
        return false;

    side=refLine().intersectionOrder_Clockwise(endIntersect,line);
    if(side==SIDE_CENTRE)
    {
        atStartCorner = false;
        atEndCorner = true;
        if(faceIndex == facesCrossed() - 1 && !exits())
        {
            if(next->isZeroLength())
            {
                // inline case is covered by start of next element
                return false;
            }
            else
            // only true for cases not covered by start of next element
            if(refLine().side(getNext()->refLine()) == SIDE_LEFT &&
                line.side(getNext()->refLine()) != SIDE_LEFT)
            {
                return true;
            }
        }
        if(!endInclusive)
        {
            return false;
        }
    }
    if(side==SIDE_LEFT)
        return false;

    atStartCorner = false;
    atEndCorner = false;
    return true;
}

bool
cCircuitElement::lineEntersInFaceOrZeroLength(
         tSigned32 faceIndex, 
         const tLine& line,
         bool& atStartCorner, bool& atEndCorner, const tLine*& intersectLine
         ) const
{
    if(lineEntersInFace_New(faceIndex, line, atStartCorner, atEndCorner))
    {
        intersectLine = &refLine();
        return true;
    }
    if(faceIndex == 0 && lineEntersZeroLength(line))
    {
        atStartCorner = true;
        atEndCorner = false;
        intersectLine = &prev->refLine();
        return true;
    }
    return false;
}



//..... TODO explicit zero length sections should be considered invalid
// (instead rely on internal zero length sections)
bool cCircuitElement::sectionIsZeroLength() const
{
    if(enters() || exits())
    {
        return false;
    }
    tLine startintersect = getStartIntersect();
    tLine endintersect = getEndIntersect();
    eSide side = refLine().intersectionOrder_Clockwise(startintersect, endintersect);
    assertD(side != SIDE_RIGHT);
    return side != SIDE_LEFT;
}

bool
cCircuitElement::cornerIsConvex() const
{
    assertD(!enters());
    return getPrev()->refLine().side(refLine()) == SIDE_RIGHT;
}



bool
cCircuitElement::testPointIsOnBoundary(tFace f, const tIntersection& p) const
{
    if(refLine().side(p) != SIDE_CENTRE)
    {
        return false;
    }
    bool inclusive;
    eSide side;
    tLine startIntersect = getStartIntersect(inclusive);
    if(!enters())
    {
        // corners are included at start of the following element
        inclusive = true;
    }
    side = startIntersect.side(p);
    if(inclusive)
    {
        if(side == SIDE_RIGHT)
        {
            return false;
        }
    }
    else
    {
        if(side != SIDE_LEFT)
        {
            return false;
        }
    }
    tLine endIntersect = getEndIntersect(inclusive);
    side = endIntersect.side(p);
    if(inclusive)
    {
        if(side == SIDE_LEFT)
        {
            return false;
        }
    }
    else
    {
        if(side != SIDE_RIGHT)
        {
            return false;
        }
    }
    return indexForFace(f) != -1;
}




// =========== stuff operating on whole circuits


void
cCircuitElement::deleteCircuit(cCircuitElement* start)
{
    if(start == 0)
    {
        return;
    }
    if(start->next == start)
    {
        start->next = 0;
        start->prev = 0;
        delete start;
        return;
    }
    cCircuitElement* e = start;
    cCircuitElement* before = start->prev;
    do
    {
        cCircuitElement* nextE = e->next;
        e->unlink();
        delete e;
        e = nextE;
    }
    while(e && e != before);
    if(before)
    {
        e = before;
        do
        {
            cCircuitElement* prevE = e->prev;
            e->unlink();
            delete e;
            e = prevE;
        }
        while(e);
    }
}

bool cCircuitElement::circuitExits(const cCircuitElement &start)
{
    const cCircuitElement *e=&start;
    do
    {
        if(e->exits())
            return true;
        e=e->getNext();
    }
    while(e!=&start);
    return false;
}

bool cCircuitElement::circuitIsSingleCut() const
{
    if(!enters())
    {
      // cuts must be handled by entering element
        return false;
    }
    const cCircuitElement* e=this;
    if(e->enters())
    while(!e->exits())
    {
        e=e->getNext();
        assertD(e);
    }
    return(e->getNext()==this);
}
bool cCircuitElement::circuitIsInternal() const
{
    const cCircuitElement* e=this;
    do
    {
        if(e->exits())
            return false;
        e=e->getNext();
        assertD(e);
    }
    while(e!=this);
    return true;
}

cCircuitElement* cCircuitElement::copyCircuit(const cCircuitElement* first)
{
    if(first==0)
        return 0;
    const cCircuitElement* e=first;
    cCircuitElement* prev=0;
    cCircuitElement* new_first=0;
    do
    {
        cCircuitElement* new_e;
        new_e=new cCircuitElement();
        new_e->init_AsClone(*e);
        if(prev)
            prev->connectTo_Internal(*new_e);
        else
            new_first=new_e;
        prev=new_e;
        e = e->next;
    }
    while(e && e!=first);
    if(e)
    {
        prev->connectTo_Internal(*new_first);
    }
    return new_first;
}


// ============== stuff for comparing exits
//... put this in a separate object

eSide
SideOfExit(tEdge exitEE, const tLine& exitLine, const tLine& testLine)
{
    eSide result;
    result = GetEdgeLine(exitEE.twin()).intersectionOrder_Clockwise(exitLine, testLine);
    if(result != SIDE_CENTRE)
    {
        return result;
    }
    result = exitLine.side(testLine);
    if(result == SIDE_CENTRE)
    {
        result = SIDE_LEFT;
    }
    return result;
}

eSide
SideOfEntry(tEdge entryEE, const tLine& entryLine, const tLine& testLine)
{
    eSide result;
    result = GetEdgeLine(entryEE.twin()).intersectionOrder_Clockwise(entryLine.reversed(), testLine);
    if(result != SIDE_CENTRE)
    {
        return result;
    }
    result = testLine.side(entryLine);
    if(result == SIDE_CENTRE)
    {
        result = SIDE_RIGHT;
    }
    return result;
}

bool
EntryIsAfterExit(tEdge elementEdge, const tLine& exitLine, const tLine& entryLine)
{
    eSide result;
    result = GetEdgeLine(elementEdge.twin()).intersectionOrder_Clockwise(exitLine, entryLine.reversed());
    if(result != SIDE_CENTRE)
    {
        return result == SIDE_RIGHT;
    }
    return exitLine.side(entryLine) == SIDE_RIGHT;
}

bool
EntryIsAfterEntry(tEdge elementEdge, const tLine& baseEntryLine, const tLine& testEntryLine)
{
    eSide side;
    side = GetEdgeLine(elementEdge).intersectionOrder_Clockwise(testEntryLine, baseEntryLine);
    if(side != SIDE_CENTRE)
    {
        return side == SIDE_RIGHT;
    }
    side = baseEntryLine.side(testEntryLine);
    assertD(side != SIDE_CENTRE);
    return side == SIDE_LEFT;
}

