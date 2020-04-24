//*********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "SilhouetteRegion.h"
#include "ElementInFaceLookup.h"
#include "SilhouetteZone.h"
#include "ElementCollisionInfo.h"
#include "interface/LineCollision.h"
#include "libs/Mesh2D/interface/CircuitElement.h"
#include "libs/Mesh2D/interface/MeshCut.h"
#include "libs/Mesh2D/interface/MeshTraversal.h"
#include "libs/Mesh2D/interface/InternalPosition.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include "libs/Geometry/interface/Side_Directions.h"
#include "common/Containers/ListSort.h"
#include "common/Containers/FlagVector.h"
#include "common/STL/list.h"

namespace nSilhouetteRegion
{

static bool
BiasTakesBuildOutVectorInsideObstructedSpace(
		const std::vector<std::vector<cElementInFaceEntry> >& lookup,
		const cInternalPosition& ip,
		const tPoint& vector,
		eSide bias
		)
{
	const std::vector<cElementInFaceEntry>& fLookup = lookup[ip.f.index()];
	tSigned32 i;
	for(i = 0; i != SizeL(fLookup); ++i)
	{
        const cCircuitElement& el = *(fLookup[i].element);
	  // must start inline with element
		if(el.refLine().side(ip.p) != SIDE_CENTRE)
		{
			continue;
		}
		if(bias == SIDE_RIGHT)
		{
		  // must go along the element line
			if(el.refLine().axis().direction(vector) != DIRECTION_FORWARD)
			{
				continue;
			}
		  // must start on or after element start
			if(el.getStartIntersect_DoesntEnter().side(ip.p) == SIDE_RIGHT)
			{
				continue;
			}
		  // must start before element end
			if(el.getEndIntersect_DoesntExit().side(ip.p) != SIDE_RIGHT)
			{
				continue;
			}
		}
		else
		{
		  // must go back along the element line
			if(el.refLine().axis().direction(vector) != DIRECTION_BACK)
			{
				continue;
			}
		  // must start after element start
			if(el.getStartIntersect_DoesntEnter().side(ip.p) != SIDE_LEFT)
			{
				continue;
			}
		  // must start on or before element end
			if(el.getEndIntersect_DoesntExit().side(ip.p) == SIDE_LEFT)
			{
				continue;
			}
		}
		return true;
	}
	return false;
}

// =================== cMarker

class cMarker
{
    const cCircuitElement* _element;
    tSigned32 _faceIndex;

public:

    cMarker() {} // construct uninitialised

    cMarker(const cCircuitElement* element, tSigned32 faceIndex) :
      _element(element),
      _faceIndex(faceIndex)
    {
    }
    cMarker(const cElementCollisionInfo& info) :
     _element(info.element),
     _faceIndex(info.faceIndex)
    {
    }

    cMarker& operator++()
    {
        _faceIndex++;
        if(_faceIndex == _element->facesCrossed())
        {
            _element = _element->getNext();
            _faceIndex = 0;
        }
        return *this;
    }
    cMarker& operator--()
    {
        if(_faceIndex == 0)
        {
            _element = _element->getPrev();
            _faceIndex = _element->facesCrossed();
        }
        _faceIndex--;
        return *this;
    }

    bool operator==(const cMarker& rhs) const
    {
        return _element == rhs._element && _faceIndex == rhs._faceIndex;
    }
    bool operator!=(const cMarker& rhs) const
    {
        return !(*this == rhs);
    }

    void setInvalid()
    {
        _element = 0;
    }
    bool valid() const
    {
        return _element != 0;
    }

    tFace face() const
    {
        return _element->faceCrossed(_faceIndex);
    }
    const tLine& line() const
    {
        return _element->refLine();
    }

    const cCircuitElement* element() const
    {
        return _element;
    }
    tSigned32 index() const
    {
        return _faceIndex;
    }

    bool atConvexCorner() const
    {
        return _faceIndex == 0 && _element->cornerIsConvex();
    }
    tPoint cornerPoint() const
    {
        assertD(_faceIndex == 0);
        return _element->cornerPoint();
    }

    const tIntersection start(bool& inclusive) const;
    const tIntersection end(bool& inclusive) const;
};

const tIntersection
cMarker::start(bool& inclusive) const
{
    tIntersection start;
    if(_faceIndex == 0)
    {
        inclusive = true;
        _element->getIntersectionForCornerAtStart(start);
    }
    else
    {
        tEdge e = _element->edgeCrossed(_faceIndex - 1);
        start = tIntersection(_element->refLine(), GetEdgeLine(e));
        inclusive = EdgeHasPriority_SI(e.twin());
    }
    return start;
}

const tIntersection
cMarker::end(bool& inclusive) const
{
    assertD(!_element->exits());
    if(_faceIndex + 1 == _element->facesCrossed())
    {
        inclusive = true;
        tIntersection result;
        _element->getNext()->getIntersectionForCornerAtStart(result);
        return result;
    }
    tEdge e = _element->edgeCrossed(_faceIndex);
    tIntersection result(_element->refLine(), GetEdgeLine(e));
    inclusive = EdgeHasPriority_SI(e);
    return result;
}

// =================== ray cast

static void
getTraversalIn(
         const std::vector<std::vector<cElementInFaceEntry> >& lookup,
         cFlagVector& elementReferencedFlags,
         tFace startFace, const tLine& line,
         cEdgesTraversal& traversal,
         cElementCollisionInfo& info,
         eSide bias = SIDE_CENTRE
         )
{
//... TODO optimise - avoid building then reversing the traversal
    GetTraversalOut(lookup, elementReferencedFlags, startFace, line, traversal, info, bias);
    traversal.reverse();
}

// =================== cCircuitBuilder

class cCircuitBuilder
{
    cCircuitElement* _first;
    cCircuitElement* _last;

    void addElement();

public:

    cCircuitBuilder();
    ~cCircuitBuilder();

    void addNewElement(cCircuitElement* newElement);
    void copyAndAddPartOfExistingElement(const cCircuitElement& existingElement, tSigned32 firstFace, tSigned32 lastFace);
    void copyAndAddEndOfExistingElement(const cCircuitElement& existingElement, tSigned32 firstFace);

    // from and to are inclusive
    void buildAround(const cMarker& from, const cMarker& to);

    void closeAndTakeResult(cMeshCut& result);

    const cCircuitElement& refLastAdded() const
    {
        assertD(_first);
        return *_last;
    }
};

cCircuitBuilder::cCircuitBuilder()
{
    _first = 0;
}
cCircuitBuilder::~cCircuitBuilder()
{
    assertD(_first == 0);
}

void
cCircuitBuilder::addNewElement(cCircuitElement* newElement)
{
    if(_first)
    {
        if(_last->refLine().direction(newElement->refLine()) == DIRECTION_FORWARD)
        {
            _last->addTraversal(*newElement);
            delete newElement;
        }
        else
        {
            _last->connectTo(*newElement);
            _last = newElement;
        }
    }
    else
    {
        _first = newElement;
        _last = _first;
    }
}

void
cCircuitBuilder::copyAndAddPartOfExistingElement(
            const cCircuitElement& existingElement, tSigned32 firstFace, tSigned32 lastFace)
{
    cCircuitElement* copy = new cCircuitElement;
    copy->init_AsCloneOfPart(existingElement, firstFace, lastFace);
    addNewElement(copy);
}

void
cCircuitBuilder::copyAndAddEndOfExistingElement(
            const cCircuitElement& existingElement, tSigned32 firstFace)
{
    tSigned32 lastFace = existingElement.facesCrossed() - 1;
    copyAndAddPartOfExistingElement(existingElement, firstFace, lastFace);
}

void
cCircuitBuilder::buildAround(const cMarker& from, const cMarker& to)
{
    const cCircuitElement* currentElement = from.element();
    tSigned32 currentFaceIndex = from.index();
    const cCircuitElement* targetElement = to.element();
    tSigned32 targetFaceIndex = to.index();
    const cCircuitElement* loopCheck = currentElement;
    while(currentElement != targetElement)
    {
        copyAndAddEndOfExistingElement(*currentElement, currentFaceIndex);
        currentElement = currentElement->getNext();
        assertD(currentElement != loopCheck);
        currentFaceIndex = 0;
    }
    assertD(currentFaceIndex <= targetFaceIndex);
    copyAndAddPartOfExistingElement(
             *currentElement, currentFaceIndex, targetFaceIndex);
}

void
cCircuitBuilder::closeAndTakeResult(cMeshCut& result)
{
    assertD(_first);

    if(_last->refLine().direction(_first->refLine()) == DIRECTION_FORWARD)
    {
        assertD(_first != _last);
        cCircuitElement* toRemove = _first;
        _last->addTraversal(*toRemove);
        _first = _first->getNext();
        toRemove->removeAndDelete();
    }

    _last->connectTo(*_first);
    result = cMeshCut(_first);
    _first = 0;
}

// =================== cBuilder

class cBuilder
{
    const std::vector<std::vector<cElementInFaceEntry> >& _lookup;
    cFlagVector& _elementReferencedFlags;
    tPoint _centreP;
    cCircuitBuilder _circuitBuilder;
    const cCircuitElement* _closingElement;
    tSigned32 _closingFaceIndex;
    const cCircuitElement* _currentElement;
    tSigned32 _currentFaceIndex;

    void buildAroundTo(const cCircuitElement& element, tSigned32 faceIndex);
    void buildAroundToEndOf(const cCircuitElement& element);

public:

    cBuilder(const std::vector<std::vector<cElementInFaceEntry> >& lookup,
        cFlagVector& elementReferencedFlags,
        const cCircuitElement& cornerElement, eSilhouetteZone zone);

    cBuilder(const std::vector<std::vector<cElementInFaceEntry> >& lookup,
        cFlagVector& elementReferencedFlags,
        const cInternalPosition& corner, const tPoint& vectorLeft, const tPoint& vectorRight);

    void buildOutEvent(const cCircuitElement& targetElement, bool& addedZeroLength);
    void buildInEvent(const cCircuitElement& targetElement);
    void buildToCloseAndTakeResult(cMeshCut& result);

    cMarker getCurrentPosition() const
    {
        return cMarker(_currentElement, _currentFaceIndex);
    }
    cMarker getClosingPosition() const
    {
        return cMarker(_closingElement, _closingFaceIndex);
    }
    const cCircuitElement& refLastAdded() const
    {
        return _circuitBuilder.refLastAdded();
    }
};

void
cBuilder::buildAroundTo(const cCircuitElement& element, tSigned32 faceIndex)
{
    const cCircuitElement* loopCheck = _currentElement;
    while(_currentElement != &element)
    {
        _circuitBuilder.copyAndAddEndOfExistingElement(*_currentElement, _currentFaceIndex);
        _currentElement = _currentElement->getNext();
        assertD(_currentElement != loopCheck);
        _currentFaceIndex = 0;
    }
    assertD(_currentFaceIndex <= faceIndex);
    _circuitBuilder.copyAndAddPartOfExistingElement(
             *_currentElement, _currentFaceIndex, faceIndex);
    _currentElement = 0;
}

void
cBuilder::buildAroundToEndOf(const cCircuitElement& element)
{
    buildAroundTo(element, element.facesCrossed() - 1);
}

cBuilder::cBuilder(const std::vector<std::vector<cElementInFaceEntry> >& lookup,
                   cFlagVector& elementReferencedFlags,
                   const cCircuitElement& cornerElement, eSilhouetteZone zone) :
    _lookup(lookup),
    _elementReferencedFlags(elementReferencedFlags)
{
    _centreP = cornerElement.cornerPoint();
    tFace cornerFace = cornerElement.faceCrossed(0);
    cElementCollisionInfo info;
    cEdgesTraversal traversal;
    if(zone == SZ_RIGHTSILHOUETTE)
    {
        tLine lineOut(_centreP, _centreP + cornerElement.getPrev()->refLine().axis());
        getTraversalIn(_lookup, _elementReferencedFlags, cornerFace, lineOut, traversal, info, SIDE_RIGHT);
        cCircuitElement* elementIn = new cCircuitElement;
        elementIn->init(lineOut.reversed(), traversal);
        _circuitBuilder.addNewElement(elementIn);
        if(info.atCorner)
        {
            _closingElement = info.element->getPrev();
            _closingFaceIndex = _closingElement->facesCrossed() - 1;
        }
        else
        {
            _closingElement = info.element;
            _closingFaceIndex = info.faceIndex;
        }
        _currentElement = &cornerElement;
        _currentFaceIndex = 0;
    }
    else
    {
        tLine lineOut(_centreP, _centreP - cornerElement.refLine().axis());
        GetTraversalOut(_lookup, _elementReferencedFlags, cornerFace, lineOut, traversal, info, SIDE_LEFT);
        cCircuitElement* elementOut = new cCircuitElement;
        elementOut->init(lineOut, traversal);
        _circuitBuilder.addNewElement(elementOut);
        _closingElement = cornerElement.getPrev();
        _closingFaceIndex = _closingElement->facesCrossed() - 1;
        _currentElement = info.element;
        _currentFaceIndex = info.faceIndex;
    }
}

cBuilder::cBuilder(const std::vector<std::vector<cElementInFaceEntry> >& lookup,
                   cFlagVector& elementReferencedFlags,
                   const cInternalPosition& corner, const tPoint& vectorLeft, const tPoint& vectorRight
                   ) :
    _lookup(lookup),
    _elementReferencedFlags(elementReferencedFlags)
{
    _centreP = corner.p;
    {
        cElementCollisionInfo info;
        cEdgesTraversal traversal;
        tLine lineOut(corner.p, corner.p + vectorLeft);
        getTraversalIn(_lookup, _elementReferencedFlags, corner.f, lineOut, traversal, info, SIDE_RIGHT);
        cCircuitElement* elementIn = new cCircuitElement;
        elementIn->init(lineOut.reversed(), traversal);
        _circuitBuilder.addNewElement(elementIn);
        if(info.atCorner)
        {
            _closingElement = info.element->getPrev();
            _closingFaceIndex = _closingElement->facesCrossed() - 1;
        }
        else
        {
            _closingElement = info.element;
            _closingFaceIndex = info.faceIndex;
        }
    }
    cElementCollisionInfo info;
    cEdgesTraversal traversal;
    tLine lineOut(corner.p, corner.p + vectorRight);
    GetTraversalOut(_lookup, _elementReferencedFlags, corner.f, lineOut, traversal, info, SIDE_LEFT);
    cCircuitElement* elementOut = new cCircuitElement;
    elementOut->init(lineOut, traversal);
    _circuitBuilder.addNewElement(elementOut);
    _currentElement = info.element;
    _currentFaceIndex = info.faceIndex;
}

void
cBuilder::buildOutEvent(const cCircuitElement& targetElement, bool& addedZeroLength)
{
    addedZeroLength = false;

    tPoint targetP = targetElement.cornerPoint();
    tFace targetFace = targetElement.faceCrossed(0);
    tLine lineOut(_centreP, targetP);
    cEdgesTraversal traversal;
    cElementCollisionInfo info;
    GetTraversalOut(_lookup, _elementReferencedFlags, targetFace, lineOut, traversal, info);

    buildAroundToEndOf(*targetElement.getPrev());

    cCircuitElement* elementOut = new cCircuitElement;
    elementOut->init(lineOut, traversal);
    _circuitBuilder.addNewElement(elementOut);

    if(info.atCorner)
    {
        if(lineOut.side(info.element->refLine()) != SIDE_LEFT)
        {
            if(lineOut.side(info.element->getPrev()->refLine()) != SIDE_LEFT)
            {
            // goes directly into a DIRECTION_BACK corner
                tLine intersectingLine;
                info.getIntersectingLine(lineOut, intersectingLine);
                cEdgesTraversal traversal(info.element->faceCrossed(0));
                cCircuitElement* zeroLength = new cCircuitElement;
                zeroLength->init(intersectingLine, traversal);
                _circuitBuilder.addNewElement(zeroLength);
                addedZeroLength = true;
            }
            else
            {
                _currentElement = info.element->getPrev();
                _currentFaceIndex = _currentElement->facesCrossed() - 1;
                return;
            }
        }
    }
    _currentElement = info.element;
    _currentFaceIndex = info.faceIndex;
}

void
cBuilder::buildInEvent(const cCircuitElement& targetElement)
{
    tPoint targetP = targetElement.cornerPoint();
    tFace targetFace = targetElement.faceCrossed(0);
    tLine lineOut(_centreP, targetP);
    cEdgesTraversal traversal;
    cElementCollisionInfo info;
    getTraversalIn(_lookup, _elementReferencedFlags, targetFace, lineOut, traversal, info);

    if(info.atCorner)
    {
        const cCircuitElement* beforeCorner = info.element->getPrev();
        if(lineOut.side(beforeCorner->refLine()) == SIDE_LEFT)
        {
            buildAroundTo(*beforeCorner, beforeCorner->facesCrossed() - 1);
        }
        else
        {
        // special case required for concave corners
            buildAroundTo(*info.element, info.faceIndex);
        }
    }
    else
    {
        buildAroundTo(*info.element, info.faceIndex);
    }

    cCircuitElement* elementIn = new cCircuitElement;
    elementIn->init(lineOut.reversed(), traversal);
    _circuitBuilder.addNewElement(elementIn);

    _currentElement = &targetElement;
    _currentFaceIndex = 0;
}
void
cBuilder::buildToCloseAndTakeResult(cMeshCut& result)
{
    buildAroundTo(*_closingElement, _closingFaceIndex);
    _circuitBuilder.closeAndTakeResult(result);
}

// =================== stuff to strip targets at an event

static void
stripAllAtEvent(const tPoint& cornerP, tTargetList& targets)
{
    eSilhouetteZone eventZone = targets.back().first;
    tPoint eventOffset = targets.back().second->cornerPoint();
    eventOffset -= cornerP;
    while(1)
    {
        targets.pop_back();
        if(targets.empty())
        {
            break;
        }
        if(targets.back().first != eventZone)
        {
            break;
        }
        tPoint targetOffset = targets.back().second->cornerPoint();
        targetOffset -= cornerP;
        if(eventOffset.side(targetOffset) != SIDE_CENTRE)
        {
            break;
        }
    }
}
static void
stripAllButOneAtEvent(const tPoint& cornerP, tTargetList& targets)
{
    eSilhouetteZone eventZone = targets.back().first;
    tPoint eventOffset = targets.back().second->cornerPoint();
    eventOffset -= cornerP;
    tTarget last_popped;
    while(1)
    {
        last_popped = targets.back();
        targets.pop_back();
        if(targets.empty())
        {
            break;
        }
        if(targets.back().first != eventZone)
        {
            break;
        }
        tPoint targetOffset = targets.back().second->cornerPoint();
        targetOffset -= cornerP;
        if(eventOffset.side(targetOffset) != SIDE_CENTRE)
        {
            break;
        }
    }
    targets.push_back(last_popped);
}

// =================== cRightSilhouetteQueue

class cRightSilhouetteQueue
{
    const tPoint _centreP;
    std::list<const cCircuitElement*> _queue;
    mutable tLine _lineToNext;
    mutable bool _lineToNextValid;

public:

    cRightSilhouetteQueue(const tPoint& centreP);

    void add(const cCircuitElement*);
    void add_360(const cCircuitElement*);
    bool empty() const
    {
        return _queue.empty();
    }
    const cCircuitElement* next() const
    {
        assertD(!_queue.empty());
        return _queue.back();
    }
    const tLine& refLineToNext() const
    {
        if(!_lineToNextValid)
        {
            _lineToNext = tLine(_centreP, _queue.back()->cornerPoint());
            _lineToNextValid = true;
        }
        return _lineToNext;
    }
    void popNext()
    {
        assertD(!_queue.empty());
        _queue.pop_back();
        _lineToNextValid = false;
    }
};


cRightSilhouetteQueue::cRightSilhouetteQueue(const tPoint& centreP) :
 _centreP(centreP)
{
}

void
cRightSilhouetteQueue::add(const cCircuitElement* toAdd)
{
    tPoint toAddAngle = toAdd->cornerPoint() - _centreP;
    std::list<const cCircuitElement*>::iterator i = _queue.begin();
    while(i != _queue.end())
    {
        tPoint existingAngle = (*i)->cornerPoint() - _centreP;
        eSide side = existingAngle.side(toAddAngle);
        if(side == SIDE_LEFT)
        {
            _queue.insert(i, toAdd);
            return;
        }
        if(side == SIDE_CENTRE)
        {
            tSigned32 existingLength, toAddLength;
            if(existingAngle.getX())
            {
                existingLength = existingAngle.getX();
                toAddLength = toAddAngle.getX();
            }
            else
            {
                existingLength = existingAngle.getY();
                toAddLength = toAddAngle.getY();
            }
            if(existingLength < 0)
            {
                existingLength = -existingLength;
                toAddLength = -toAddLength;
            }
            assertD(existingLength > 0);
            assertD(toAddLength > 0);
            if(existingLength < toAddLength)
            {
            // this silhouette is exactly behind one already in the queue
                return;
            }
            if(existingLength > toAddLength)
            {
            // this silhouette is exactly in front of one already in the queue
            // replace the existing entry
                *i = toAdd;
                return;
            }
            // two silhouettes at exactly the same point
            // compare angles of circuit after
            eSide lSide = (*i)->refLine().side(toAdd->refLine());
            assertD(lSide != SIDE_CENTRE);
            if(lSide == SIDE_RIGHT)
            {
                *i = toAdd;
            }
            return;
        }
        ++i;
    }
    _queue.push_back(toAdd);
    _lineToNextValid = false;
}

void
cRightSilhouetteQueue::add_360(const cCircuitElement* toAdd)
{
    tPoint toAddAngle = toAdd->cornerPoint() - _centreP;
    std::list<const cCircuitElement*>::iterator i = _queue.begin();
    while(i != _queue.end())
    {
        tPoint existingAngle = (*i)->cornerPoint() - _centreP;

        tPoint base = tPoint(0,1,1);
        eSide side = Side_Directions_BaseAtEnd(base, existingAngle, toAddAngle);

        if(side == SIDE_LEFT)
        {
            _queue.insert(i, toAdd);
            return;
        }
        if(side == SIDE_CENTRE)
        {
            tSigned32 existingLength, toAddLength;
            if(existingAngle.getX())
            {
                existingLength = existingAngle.getX();
                toAddLength = toAddAngle.getX();
            }
            else
            {
                existingLength = existingAngle.getY();
                toAddLength = toAddAngle.getY();
            }
            if(existingLength < 0)
            {
                existingLength = -existingLength;
                toAddLength = -toAddLength;
            }
            assertD(existingLength > 0);
            assertD(toAddLength > 0);
            if(existingLength < toAddLength)
            {
            // this silhouette is exactly behind one already in the queue
                return;
            }
            if(existingLength > toAddLength)
            {
            // this silhouette is exactly in front of one already in the queue
            // replace the existing entry
                *i = toAdd;
                return;
            }
            // two silhouettes at exactly the same point
            // compare angles of circuit after
            eSide lSide = (*i)->refLine().side(toAdd->refLine());
            assertD(lSide != SIDE_CENTRE);
            if(lSide == SIDE_RIGHT)
            {
                *i = toAdd;
            }
            return;
        }
        ++i;
    }
    _queue.push_back(toAdd);
    _lineToNextValid = false;
}

void
AddTargetsInFace(const cInternalPosition& corner, const tPoint& axisBefore, const tPoint& axisAfter,
                 eSilhouetteZone zone,
                 const std::vector<cCornerInFaceEntry>& cornerLookup,
                 tSigned32 f,
                 cRightSilhouetteQueue& queue
                 )
{
    size_t i;
    for(i = 0; i < cornerLookup.size(); i++)
    {
        const cCircuitElement* targetEl = cornerLookup[i].element;
        tPoint targetP = targetEl->cornerPoint();
        eSilhouetteZone targetZone = ZoneFor(axisBefore, axisAfter, targetP - corner.p);
        if(targetZone != zone)
        {
            continue;
        }
        eSilhouetteZone zoneFromTarget = ZoneFor(targetEl->getPrev()->refLine(), targetEl->refLine(), corner.p);
        if(zoneFromTarget != SZ_RIGHTSILHOUETTE)
        {
            continue;
        }
        bool exits;
        tFace endF;
        tLine testLine(corner.p, targetP);
        exits = TestTraversalToXY(corner.f, testLine, endF);
        if(exits)
        {
            continue;
        }
        if(endF != targetEl->faceCrossed(0))
        {
            continue;
        }
        queue.add(targetEl);
    }
}

static bool
SilhouetteIsBeforeMarkerEnd(const cMarker& marker, const tLine& lineToSilhouette)
{
    const cCircuitElement* element = marker.element();

    tSigned32 faceIndex = marker.index();
    if(faceIndex + 1 == element->facesCrossed())
    {
        if(lineToSilhouette.side(element->getNext()->refLine().start()) == SIDE_CENTRE
            &&
            lineToSilhouette.direction(element->getNext()->refLine()) == DIRECTION_BACK)
        {
        // special case for when the next element turns directly towards the centre,
        // and the line to silhouette goes exactly through the corner
            return true;
        }
        tIntersection end;
        element->getNext()->getIntersectionForCornerAtStart(end);
        return lineToSilhouette.side(end) == SIDE_LEFT;
    }

    tEdge e = element->edgeCrossed(faceIndex);
    tIntersection end(element->refLine(), GetEdgeLine(e));
    eSide side = lineToSilhouette.side(end);
    if(EdgeHasPriority_SI(e) && side == SIDE_CENTRE)
    {
        return true;
    }
    return side == SIDE_LEFT;
}

static bool
SilhouetteIsAtOrAfterMarkerStart(const cMarker& marker, const tLine& lineToSilhouette)
{
    const cCircuitElement* element = marker.element();
    tSigned32 faceIndex = marker.index();
    tIntersection start;
    bool inclusive;
    if(faceIndex == 0)
    {
        element->getIntersectionForCornerAtStart(start);
        inclusive = true;
    }
    else
    {
        tEdge e = element->edgeCrossed(faceIndex - 1);
        start = tIntersection(element->refLine(), GetEdgeLine(e));
        inclusive = EdgeHasPriority_SI(e.twin());
    }
    eSide side = lineToSilhouette.side(start);
    if(inclusive && side == SIDE_CENTRE)
    {
        return true;
    }
    return side == SIDE_RIGHT;
}

void
build_Common(
      const std::vector<std::vector<cCornerInFaceEntry> >& cornerLookup,
           const cInternalPosition& cornerIP, const tPoint& axisBefore, const tPoint& axisAfter,
           eSilhouetteZone zone,
           cFlagVector& facesReferenced,
           cBuilder& builder,
           cRightSilhouetteQueue& queue,
           cMeshCut& result
           )
{
    tPoint centreP = cornerIP.p;

    cMarker closing = builder.getClosingPosition();
    cMarker current = builder.getCurrentPosition();
#ifdef ASSERTIONS_ON
    cMarker loopCheck = current;
#endif

    {
        tFace f = current.face();
        if(!facesReferenced[f.index()])
        {
            facesReferenced.set(f.index());
            AddTargetsInFace(cornerIP, axisBefore, axisAfter, zone,
                cornerLookup[f.index()],
                f.index(),
                queue
                );
        }
    }

    bool justBuiltOut = false;
    tLine justBuiltOutLine;

    while(1)
    {
        if(current == closing)
        {
            if(queue.empty())
            {
                break;
            }
            if(closing.line().side(centreP) == SIDE_CENTRE)
            {
              // special case where there may be queue entries left past the end
                break;
            }
        }

        if(!queue.empty() &&
            current.line().side(centreP) == SIDE_LEFT &&
            SilhouetteIsBeforeMarkerEnd(current, queue.refLineToNext()))
        {
            bool skip = false;

            //...... TODO optimise so that no intersections are involved here
            if(current.line().side(queue.next()->cornerPoint()) == SIDE_RIGHT)
            {
              // silhouette is behind the marker circuit, skip
                skip = true;
            }
            else
            if(justBuiltOut)
            {
                if(justBuiltOutLine.side(queue.refLineToNext()) == SIDE_RIGHT)
                {
                    skip = true;
                }
            }
            else
            {
                if(!SilhouetteIsAtOrAfterMarkerStart(current, queue.refLineToNext()))
                {
                // silhouette is behind circuit traced out already, skip
                    skip = true;
                }
            }

            if(skip)
            {
                queue.popNext();
            }
            else
            {
              // a build in event occurs
                builder.buildInEvent(*queue.next());
                queue.popNext();

                const cCircuitElement& added = builder.refLastAdded();
                tSigned32 i;
                for(i = 0; i < added.facesCrossed(); i++)
                {
                    tFace f = added.faceCrossed(i);
                    if(!facesReferenced[f.index()])
                    {
                        facesReferenced.set(f.index());
                        AddTargetsInFace(cornerIP, axisBefore, axisAfter, zone,
                            cornerLookup[f.index()],
                            f.index(),
                            queue
                            );
                    }
                }

                current = builder.getCurrentPosition();
#ifdef ASSERTIONS_ON
                loopCheck = current;
#endif
                justBuiltOut = false;
            }
        }
        else
        {
          // advance marker along current circuit

            assertD(current != closing);

            ++current;
            assertD(current != loopCheck);
            justBuiltOut = false;

            if(current.atConvexCorner())
            {
                tPoint markerP = current.cornerPoint();
                const cCircuitElement* markerElement = current.element();
                eSilhouetteZone zoneFromMarker = ZoneFor(markerElement->getPrev()->refLine(), markerElement->refLine(), centreP);
                assertD(zoneFromMarker != SZ_RIGHTSILHOUETTE);
                if(zoneFromMarker == SZ_LEFTSILHOUETTE)
                {                        
                  // a build out event occurs
                    justBuiltOut = true;
                    justBuiltOutLine = tLine(centreP, markerP);

                    bool addedZeroLength;
                    builder.buildOutEvent(*markerElement, addedZeroLength);
                    const cCircuitElement& added = addedZeroLength ? *builder.refLastAdded().getPrev() : builder.refLastAdded();

                    tSigned32 i;
                    for(i = 0; i < added.facesCrossed(); i++)
                    {
                        tFace f = added.faceCrossed(i);
                        if(!facesReferenced[f.index()])
                        {
                            facesReferenced.set(f.index());
                            AddTargetsInFace(cornerIP, axisBefore, axisAfter, zone,
                                cornerLookup[f.index()],
                                f.index(),
                                queue
                                );
                        }
                    }

                    current = builder.getCurrentPosition();
#ifdef ASSERTIONS_ON
                    loopCheck = current;
#endif
                    continue;
                }
             }

            tFace f = current.face();
            if(!facesReferenced[f.index()])
            {
                facesReferenced.set(f.index());
                AddTargetsInFace(cornerIP, axisBefore, axisAfter, zone,
                    cornerLookup[f.index()],
                    f.index(),
                    queue
                    );
            }
        }
    }

    builder.buildToCloseAndTakeResult(result);
    result.markZeroLengthSections();
}



void
build(const std::vector<std::vector<cElementInFaceEntry> >& lookup,
      cFlagVector& elementReferencedFlags,
      const std::vector<std::vector<cCornerInFaceEntry> >& cornerLookup,
           const cCircuitElement& corner, eSilhouetteZone zone,
           cFlagVector& facesReferenced,
           cMeshCut& result
           )
{
    cInternalPosition cornerIP(corner.faceCrossed(0), corner.cornerPoint());
    tPoint axisBefore = corner.getPrev()->refLine().axis();
    tPoint axisAfter = corner.refLine().axis();
    cBuilder builder(lookup, elementReferencedFlags, corner, zone);
    cRightSilhouetteQueue queue(cornerIP.p);
    facesReferenced.clear();
    if(zone == SZ_LEFTSILHOUETTE)
    {
        const cCircuitElement& added = builder.refLastAdded();
        tSigned32 i;
        for(i = 0; i < added.facesCrossed(); i++)
        {
            tFace f = added.faceCrossed(i);
            if(!facesReferenced[f.index()])
            {
                facesReferenced.set(f.index());
                AddTargetsInFace(cornerIP, axisBefore, axisAfter, zone,
                    cornerLookup[f.index()],
                    f.index(),
                    queue
                    );
            }
        }
    }
    build_Common(cornerLookup, cornerIP, axisBefore, axisAfter, zone, facesReferenced, builder, queue, result);
}


void
build_Unanchored(const std::vector<std::vector<cElementInFaceEntry> >& lookup,
      cFlagVector& elementReferencedFlags,
      const std::vector<std::vector<cCornerInFaceEntry> >& cornerLookup,
      const cInternalPosition& centre, const tPoint& axisBefore, const tPoint& axisAfter,
      eSilhouetteZone zone,
      cFlagVector& facesReferenced,
      cMeshCut& result
      )
{
    tPoint vectorLeft, vectorRight;
    if(zone == SZ_RIGHTSILHOUETTE)
    {
        vectorLeft = axisBefore;
        vectorRight = axisAfter;
    }
    else
    {
        assertD(zone == SZ_LEFTSILHOUETTE);
        vectorLeft = -axisBefore;
        vectorRight = -axisAfter;
    }
	if(BiasTakesBuildOutVectorInsideObstructedSpace(lookup, centre, vectorLeft, SIDE_RIGHT)
	 || BiasTakesBuildOutVectorInsideObstructedSpace(lookup, centre, vectorRight, SIDE_LEFT)
		)
	{
		assertD(result.getStartElement() == 0);
		return;
	}
    cBuilder builder(lookup, elementReferencedFlags, centre, vectorLeft, vectorRight);
    cRightSilhouetteQueue queue(centre.p);
    facesReferenced.clear();
    const cCircuitElement& added = builder.refLastAdded();
    tSigned32 i;
    for(i = 0; i < added.facesCrossed(); i++)
    {
        tFace f = added.faceCrossed(i);
        if(!facesReferenced[f.index()])
        {
            facesReferenced.set(f.index());
            AddTargetsInFace(centre, axisBefore, axisAfter, zone,
                cornerLookup[f.index()],
                f.index(),
                queue
                );
        }
    }
    build_Common(cornerLookup, centre, vectorLeft, vectorRight, zone, facesReferenced, builder, queue, result);
}

void
AddTargetsInFace_360(
           tFace cornerF, const tPoint& cornerP,
           const std::vector<cCornerInFaceEntry>& cornerLookup,
           tSigned32 f,
           cRightSilhouetteQueue& queue
           )
{
    size_t i;
    for(i = 0; i < cornerLookup.size(); i++)
    {
        const cCircuitElement* targetEl = cornerLookup[i].element;

        //... could store point and face with the lookup
        //... or get by index from another lookup

        tPoint targetP = targetEl->cornerPoint();

        eSilhouetteZone zoneFromTarget = ZoneFor(targetEl->getPrev()->refLine(), targetEl->refLine(), cornerP);
        if(zoneFromTarget != SZ_RIGHTSILHOUETTE)
        {
            continue;
        }

        bool exits;
        tFace endF;
        tLine testLine(cornerP, targetP);
        exits = TestTraversalToXY(cornerF, testLine, endF);
        if(exits)
        {
            continue;
        }
        if(endF != targetEl->faceCrossed(0))
        {
            continue;
        }

        queue.add_360(targetEl);
    }
}

bool
MarkerCrossesZero(const tLine& baseLine, const cMarker& marker,
                  bool justBuiltOut, const tLine& justBuiltOutLine
                 )
{
    tIntersection end;
    {
        bool inclusive;
        end = marker.end(inclusive);
    }
    eDirection endDirection = baseLine.direction(end);

    // check if the marker crosses out of the range
    if(endDirection == DIRECTION_FORWARD || endDirection == DIRECTION_LEFT)
    {
        tIntersection start;
        if(justBuiltOut)
        {
            start = tIntersection(marker.line(), justBuiltOutLine);
        }
        else
        {
            bool inclusive;
            start = marker.start(inclusive);
        }
        eDirection startDirection = baseLine.direction(start);
        if(startDirection == DIRECTION_BACK || startDirection == DIRECTION_RIGHT)
        {
            return true;
        }
    }

    return false;
}

static bool
SilhouetteIsBeforeMarkerEnd_360(const tLine& baseLine,
                                const cMarker& marker,
                                const tLine& lineToSilhouette)
{
    const cCircuitElement* element = marker.element();
    tSigned32 faceIndex = marker.index();
    if(faceIndex + 1 == element->facesCrossed())
    {
        if(lineToSilhouette.side(element->getNext()->refLine().start()) == SIDE_CENTRE
            &&
            lineToSilhouette.direction(element->getNext()->refLine()) == DIRECTION_BACK)
        {
        // special case for when the next element turns directly towards the centre,
        // and the line to silhouette goes exactly through the corner
            return true;
        }
    }

    tIntersection end;
    bool inclusive;
    end = marker.end(inclusive);

    if(faceIndex + 1 == element->facesCrossed())
    {
    // bugfix - if the end is a left silhouette corner then it needs to be non-inclusive
    // might as well always assign corners to the next element then
        inclusive = false;
    }

    eDirection endDirection = baseLine.direction(end);

    eDirection silhouetteDirection = baseLine.direction(lineToSilhouette.end());
    if(endDirection == silhouetteDirection)
    {
        eSide side = lineToSilhouette.side(end);
        return side == SIDE_LEFT || (inclusive && side == SIDE_CENTRE);
    }
    else
    {
        return Side_Directions_BaseAtEnd(endDirection, silhouetteDirection) == SIDE_RIGHT;
    }
}

static bool
SilhouetteIsAtOrAfterMarkerStart_360(const tLine& baseLine, const cMarker& marker, const tLine& lineToSilhouette)
{
    bool inclusive;
    tIntersection start = marker.start(inclusive);

    eDirection startDirection = baseLine.direction(start);
    eDirection silhouetteDirection = baseLine.direction(lineToSilhouette.end());
    if(startDirection == silhouetteDirection)
    {
        eSide side = lineToSilhouette.side(start);
        return side == SIDE_RIGHT || (inclusive && side == SIDE_CENTRE);
    }
    else
    {
        return Side_Directions_BaseAtEnd(startDirection, silhouetteDirection) == SIDE_LEFT;
    }
}

void
build_360(const std::vector<std::vector<cElementInFaceEntry> >& lookup,
      cFlagVector& elementReferencedFlags,
      const std::vector<std::vector<cCornerInFaceEntry> >& cornerLookup,
      const cInternalPosition& centre,
      cFlagVector& facesReferenced,
      cMeshCut& result
      )
{
    tFace centreFace = centre.f;
    tPoint centreP = centre.p;
    facesReferenced.clear();
    cRightSilhouetteQueue queue(centreP);

// traverse out

    tFace startFace = centreFace;
    cElementCollisionInfo info;

    {
        tLine lineOut(centreP, centreP + tPoint(0, 1, 1));
        cEdgesTraversal traversal;
        GetTraversalOut(lookup, elementReferencedFlags, centreFace, lineOut, traversal, info);

        tSigned32 i;
        for(i = 0; i < traversal.facesCrossed(); ++i)
        {
            tFace f = traversal.faceCrossed(i);
            if(!facesReferenced[f.index()])
            {
                facesReferenced.set(f.index());
                AddTargetsInFace_360(centreFace, centreP,
                    cornerLookup[f.index()],
                    f.index(),
                    queue
                    );
            }
        }
    }

// build around

    cCircuitBuilder builder;

    cMarker start(info);
    cMarker closing(start);
    cMarker loopCheck;
    loopCheck.setInvalid();

    bool leftClosingMarker = false;
    bool justBuiltOut = true;
    tLine justBuiltOutLine(centreP, centreP + tPoint(0, 1, 1));;

    tLine baseLine(centreP, centreP + tPoint(0, 1, 1));

    cMarker i(start);
    do
    {
        if(!queue.empty() &&
            i.line().side(centreP) == SIDE_LEFT &&
            (
             MarkerCrossesZero(baseLine, i, justBuiltOut, justBuiltOutLine)
             ||
             SilhouetteIsBeforeMarkerEnd_360(baseLine, i, queue.refLineToNext())
            )
          )
        {
            bool skip = false;

            //...... TODO optimise so that no intersections are involved here
            if(i.line().side(queue.next()->cornerPoint()) == SIDE_RIGHT)
            {
              // silhouette is behind the marker circuit, skip
                skip = true;
            }
            else
            if(justBuiltOut)
            {
                eSide side = Side_Directions_BaseAtEnd(baseLine.axis(), justBuiltOutLine.axis(), queue.refLineToNext().axis());

                if(side == SIDE_RIGHT)
                {
                    skip = true;
                }
            }
            else
            if(!SilhouetteIsAtOrAfterMarkerStart_360(baseLine, i, queue.refLineToNext()))
            {
            // silhouette is behind circuit traced out already, skip
                skip = true;
            }

            if(skip)
            {
                queue.popNext();
            }
            else
            {
              // a build in event occurs

//                builder.buildInEvent(*queue.next());

                const cCircuitElement& targetElement = *queue.next();
                queue.popNext();

                tPoint targetP = targetElement.cornerPoint();
                tFace targetFace = targetElement.faceCrossed(0);
                tLine lineOut(centreP, targetP);
                cEdgesTraversal traversal;
                cElementCollisionInfo info;
                getTraversalIn(lookup, elementReferencedFlags, targetFace, lineOut, traversal, info);

                cMarker intersected(info);
                if(info.atCorner)
                {
                    const cCircuitElement* beforeCorner = info.element->getPrev();
                    if(lineOut.side(beforeCorner->refLine()) == SIDE_LEFT)
                    {
                        --intersected;
                    }
                }

                builder.buildAround(start, i);

                {
                    cCircuitElement* added = new cCircuitElement;
                    added->init(lineOut.reversed(), traversal);

                    tSigned32 j;
                    for(j = 1; j < added->facesCrossed(); j++)
                    {
                        tFace f = added->faceCrossed(j);
                        if(!facesReferenced[f.index()])
                        {
                            facesReferenced.set(f.index());
                            AddTargetsInFace_360(
                                centreFace, centreP,
                                cornerLookup[f.index()],
                                f.index(),
                                queue
                                );
                        }
                    }

                    builder.addNewElement(added);
                }

                start = cMarker(&targetElement, 0);
                loopCheck = start;
                i = start;
                justBuiltOut = false;
                leftClosingMarker = true;
            }
        }
        else
        {
          // advance marker along current circuit

            {
                tFace faceBefore = i.face();
                ++i;
                tFace f = i.face();
                if(f != faceBefore && !facesReferenced[f.index()])
                {
                    facesReferenced.set(f.index());
                    AddTargetsInFace_360(
                        centreFace, centreP,
                        cornerLookup[f.index()],
                        f.index(),
                        queue
                        );
                }
            }        

            assertD(i != loopCheck);
            justBuiltOut = false;
            leftClosingMarker = true;

            if(i.atConvexCorner())
            {
                const cCircuitElement* markerElement = i.element();
                tPoint markerP = markerElement->cornerPoint();
                eSilhouetteZone zoneFromMarker = ZoneFor(markerElement->getPrev()->refLine(), markerElement->refLine(), centreP);
                assertD(zoneFromMarker != SZ_RIGHTSILHOUETTE);
                if(zoneFromMarker == SZ_LEFTSILHOUETTE)
                {
                  // a build out event occurs
                    justBuiltOut = true;
                    justBuiltOutLine = tLine(centreP, markerP);

                    {
                        tFace markerFace = markerElement->faceCrossed(0);
                        tLine lineOut(centreP, markerP);
                        cEdgesTraversal traversal;
                        cElementCollisionInfo info;
                        GetTraversalOut(lookup, elementReferencedFlags, markerFace, lineOut, traversal, info);

                        --i;
                        builder.buildAround(start, i);

                        {
                            cCircuitElement* added = new cCircuitElement;
                            added->init(lineOut, traversal);

                            tSigned32 j;
                            for(j = 1; j < added->facesCrossed(); j++)
                            {
                                tFace f = added->faceCrossed(j);
                                if(!facesReferenced[f.index()])
                                {
                                    facesReferenced.set(f.index());
                                    AddTargetsInFace_360(
                                        centreFace, centreP,
                                        cornerLookup[f.index()],
                                        f.index(),
                                        queue
                                        );
                                }
                            }

                            builder.addNewElement(added);
                        }

                        start = cMarker(info);
                        if(info.atCorner)
                        {
                            if(lineOut.side(info.element->refLine()) != SIDE_LEFT)
                            {
                                assertD(lineOut.side(info.element->getPrev()->refLine()) == SIDE_LEFT);
                                --start;
                            }
                        }
                        loopCheck = start;
                        i = start;
                    }
                }
            }
        }
    }
    while(!leftClosingMarker || i != closing || !queue.empty());

    if(loopCheck.valid())
    {
        builder.buildAround(start, i);
        builder.closeAndTakeResult(result);
        result.markZeroLengthSections();
    }
    else
    {
    // no silhouettes
    // copy single containing cut
        result = cMeshCut(cCircuitElement::copyCircuit(start.element()));
    }
}

}
