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
#include "libs/Mesh2D/interface/MeshCut.h"
#include "RemapEntry.h"
#include "FacesContainedByBoundary.h"
#include "interface/TranslateToPartitioned.h"
#include "libs/Mesh2D/interface/CircuitElement.h"
#include "libs/Mesh2D/interface/ArrangeIntoCircuits.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include "libs/Mesh_Common/interface/iMeshTranslator.h"
#include "libs/Mesh_Common/interface/PositionForPointInFace.h"
#include "libs/Geometry/interface/Intersection_PointLike.h"
#include "libs/Geometry/interface/Point_PointLike.h"
#include "libs/Geometry/interface/InfinitessimallyOffset_PointLike.h"
#include "libs/Geometry/interface/CompareFractions.h"
#include "libs/Geometry/interface/InsideCorner.h"
#include "common/interface/Error.h"
#include "common/HandleList.h"
#include "common/Containers/FlagVector.h"
#include "common/Containers/ListSort.h"
#include "common/interface/iOutputStream_Operators.h"

cMeshCut::~cMeshCut()
{
    _affected = false;
    cCircuitElement::deleteCircuit(_startElement);
}

cMeshCut::cMeshCut(const cMeshCut& rhs)
{
    _affected = false;
    _startElement = cCircuitElement::copyCircuit(rhs._startElement);
}

//void
//cMeshCut::setAgent(tSigned32 agent)
//{
//    assertD(_startElement);
//    cCircuitElement* e = _startElement;
//    do
//    {
//        e->setAgentIndex(agent);
//        e = e->getNext();
//    }
//    while(e != _startElement);
//}

//void
//cMeshCut::operator>>(iSerialiser& serialiser)
//{
//    assertD(_startElement);
//    tSigned32 numberOfElements;
//    if(serialiser.isConstructing())
//    {
//        serialiser.child(numberOfElements);
//        if(numberOfElements == 0)
//        {
//            _startElement = 0;
//        }
//        else
//        {
//            _startElement = new cCircuitElement;
//            serialiser.child(*_startElement);
//            cCircuitElement* e = _startElement;
//            tSigned32 i;
//            for(i = 0; i < numberOfElements; i++)
//            {
//                cCircuitElement* newE = new cCircuitElement;
//                serialiser.child(*newE);
//                cCircuitElement::remapConnection(*e, *newE);
//                e = newW;
//            }
//            cCircuitElement::remapConnection(*e, *_startElement);
//        }
//    }
//    else
//    {
//        tSigned32 numberOfElements = getNumberOfElements();
//        cCircuitElement* e = _startElement;
//        do
//        {
//            serialiser.child(*e);
//            e = e->getNext();
//        }
//        while(e != _startElement);
//    }
//    serialiser.child(_affected);
//}

void
cMeshCut::addToStats(cCircuitStats& stats) const
{
    assertD(_startElement);
    stats._cuts++;
    cCircuitElement* e = _startElement;
    do
    {
        stats._elements++;
        if(e->cornerIsInline())
        {
            stats._inlineCorners++;
        }
        e = e->getNext();
    }
    while(e != _startElement);
}
tSigned32
cMeshCut::countElements() const
{
    assertD(_startElement);
    tSigned32 result = 0;
    cCircuitElement* e = _startElement;
    do
    {
        result++;
        e = e->getNext();
    }
    while(e != _startElement);
    return result;
}
tSigned32
cMeshCut::indexElements(tSigned32 startIndex) const
{
    assertD(_startElement);
    tSigned32 currentIndex = startIndex;
    cCircuitElement* e = _startElement;
    do
    {
        e->setIndex(currentIndex++);
        e = e->getNext();
    }
    while(e != _startElement);
    return currentIndex;
}

void
cMeshCut::changeStartElement()
{
    assertD(isInternalLoop());
    _startElement = _startElement->getNext();
}

void
cMeshCut::initFromFragment(cCircuitElement& midElement)
{
    assertD(_startElement == 0);
    assertD(!midElement.enters());
    assertD(midElement.getPrev());
    assertD(midElement.getPrev() != &midElement);
    cCircuitElement *e;
    
    e = midElement.getPrev();
    while(e != &midElement && !e->enters())
    {
        e = e->getPrev();
    }
    if(e == &midElement)
    // circular fragment
    {
        _startElement = &midElement;
//        result.checkValid();
        return;
    }

    cCircuitElement* start = e;
    e = &midElement;
    while(!e->exits())
    {
        e = e->getNext();
        assertD(e);
    }

    if(e->getNext())
    {
        e->unlinkExit();
    }
    if(start->getPrev())
    {
        start->unlinkEntry();
    }
    e->connectTo(*start);

    _startElement = start;
//    result.checkValid();
}

void
cMeshCut::initFromUnlinked(cCircuitElement& start_element)
{
    assertD(_startElement == 0);
    cCircuitElement* e = &start_element;
    while(e->getNext())
    {
        e = e->getNext();
    }
    e->connectTo(start_element);
    _startElement = &start_element;
}

void
cMeshCut::initFromFragment(cCircuitElement* first, cCircuitElement* last)
{
    if(!last)
    {
        initFromFragment(*first);
        return;
    }
    _startElement = first;
}

bool
cMeshCut::isInternalLoop() const
{
    assertD(_startElement);
    return _startElement->getPrev() && !_startElement->enters();
}

const cCircuitElement*
cMeshCut::getLeftMostVertex() const
{
    assertD(isInternalLoop());
    const cCircuitElement* best = _startElement;
    const cCircuitElement* candidate = _startElement->getNext();

    tPoint::tMultipliedDistance bestN, candidateN;
    tPoint::tDistance bestD, candidateD;

    tIntersection intersection;
    best->getIntersectionForCornerAtStart(intersection);
    intersection.getXAsFraction(bestN, bestD);

    while(candidate != _startElement)
    {
        candidate->getIntersectionForCornerAtStart(intersection);
        intersection.getXAsFraction(candidateN, candidateD);

        if(CompareFractions(bestN, bestD, candidateN, candidateD) == -1)
        {
            best = candidate;
            bestN = candidateN;
            bestD = candidateD;
        }
        candidate = candidate->getNext();
    }

    return best;
}

// the element with lowest index starting face is chosen
// then least x in that face
// then least y
// then rightmost vector to next
bool
cMeshCut::lessThanForNormalise(const cCircuitElement* e1, const cCircuitElement* e2)
{
    if(e1->faceCrossed(0).index() < e2->faceCrossed(0).index())
    {
        return true;
    }
    if(e1->faceCrossed(0).index() > e2->faceCrossed(0).index())
    {
        return false;
    }

    tSigned32 comparison;

    tPoint::tMultipliedDistance n1, n2;
    tPoint::tDistance d1, d2;

    tIntersection intersection1;
    e1->getIntersectionForCornerAtStart(intersection1);
    intersection1.getXAsFraction(n1, d1);

    tIntersection intersection2;
    e2->getIntersectionForCornerAtStart(intersection2);
    intersection2.getXAsFraction(n2, d2);

    comparison = CompareFractions(n1, d1, n2, d2);
    if(comparison == -1)
    {
        return true;
    }
    if(comparison == 1)
    {
        return false;
    }

    intersection1.getYAsFraction(n1, d1);
    intersection2.getYAsFraction(n2, d2);
    comparison = CompareFractions(n1, d1, n2, d2);
    if(comparison == -1)
    {
        return true;
    }
    if(comparison == 1)
    {
        return false;
    }

    eSide side = e1->refLine().side(e2->refLine());
    assertD(side != SIDE_CENTRE);
    return side == SIDE_RIGHT;
}

tSigned32
cMeshCut::compareEntryOrExitEdges(tEdge lhs, tEdge rhs)
{
    if(rhs.index() < lhs.index())
    {
        return -1;
    }
    if(rhs.index() > lhs.index())
    {
        return 1;
    }
    return 0;
}

tSigned32
cMeshCut::compareIntersections(const tIntersection& lhs, const tIntersection& rhs)
{
    tPoint::tMultipliedDistance n1, n2;
    tPoint::tDistance d1, d2;

    lhs.getXAsFraction(n1, d1);
    rhs.getXAsFraction(n2, d2);

    tSigned32 comparison = CompareFractions(n1, d1, n2, d2);
    if(comparison != 0)
    {
        return comparison;
    }

    lhs.getYAsFraction(n1, d1);
    rhs.getYAsFraction(n2, d2);
    comparison = CompareFractions(n1, d1, n2, d2);
    return comparison;
}

tSigned32
cMeshCut::compareEntries(const cCircuitElement* lhs, const cCircuitElement* rhs)
{
    tEdge lhs_ee, rhs_ee;
    lhs_ee = lhs->entryEdge();
    rhs_ee = rhs->entryEdge();
    tSigned32 comparison = compareEntryOrExitEdges(lhs_ee, rhs_ee);
    if(comparison != 0)
    {
        return comparison;
    }
    bool inclusive;
    tIntersection lhs_intersection(lhs->refLine(), lhs->getEntryIntersect(inclusive));
    tIntersection rhs_intersection(rhs->refLine(), lhs->getEntryIntersect(inclusive));
    return compareIntersections(lhs_intersection, rhs_intersection);
}
tSigned32
cMeshCut::compareCorners(const cCircuitElement* lhs, const cCircuitElement* rhs)
{
    tFace lhs_face, rhs_face;
    lhs_face = lhs->faceCrossed(0);
    rhs_face = rhs->faceCrossed(0);
    if(lhs_face.index() != rhs_face.index())
    {
        if(rhs_face.index() < lhs_face.index())
        {
            return -1;
        }
        return 1;
    }
    tIntersection lhs_intersection, rhs_intersection;
    lhs->getIntersectionForCornerAtStart(lhs_intersection);
    rhs->getIntersectionForCornerAtStart(rhs_intersection);
    return compareIntersections(lhs_intersection, rhs_intersection);
}
tSigned32
cMeshCut::compareExits(const cCircuitElement* lhs, const cCircuitElement* rhs)
{
    tEdge lhs_ee, rhs_ee;
    lhs_ee = lhs->exitEdge();
    rhs_ee = rhs->exitEdge();
    tSigned32 comparison = compareEntryOrExitEdges(lhs_ee, rhs_ee);
    if(comparison != 0)
    {
        return comparison;
    }
    bool inclusive;
    tIntersection lhs_intersection(lhs->refLine(), lhs->getExitIntersect(inclusive));
    tIntersection rhs_intersection(rhs->refLine(), lhs->getExitIntersect(inclusive));
    return compareIntersections(lhs_intersection, rhs_intersection);
}

tSigned32
cMeshCut::getNumberOfElements() const
{
    if(!_startElement)
    {
        return 0;
    }
    tSigned32 result = 0;
    const cCircuitElement* e = _startElement;
    do
    {
        result++;
        e = e->getNext();
    }
    while(e != _startElement);
    return result;
}

bool
cMeshCut::isHole() const
{
    const cCircuitElement* leftMost = getLeftMostVertex();

    const tLine& before = leftMost->getPrev()->refLine();
    const tLine& after = leftMost->refLine();
    return InsideCorner_Vector(before, after, tPoint(-1, 0, 1));
}

const cMeshCut&
cMeshCut::operator=(const cMeshCut& rhs)
{
    _startElement = cCircuitElement::copyCircuit(rhs._startElement);
    return *this;
}

bool cMeshCut::operator==(const cMeshCut& rhs) const
{
    if(_startElement==0)
        return rhs._startElement==0;
    if(rhs._startElement==0)
        return false;
    const cCircuitElement *e1,*e2;
    e1=_startElement;
    e2=rhs._startElement;
    do
    {
        if(*e1!=*e2)
            return false;
        e1=e1->getNext();
        e2=e2->getNext();
    }
    while(e1 && e2 && e1!=_startElement && e2!=rhs._startElement);
    if(!e1 || !e2)
    {
        return e1 == e2;
    }
    return e1==_startElement && e2==rhs._startElement;
}

// internal cuts come after non internal cuts
// then compare corners lexographically
tSigned32
cMeshCut::compare(const cMeshCut& lhs, const cMeshCut& rhs)
{
    if(lhs.isInternalLoop() != rhs.isInternalLoop())
    {
        if(rhs.isInternalLoop())
        {
            return 1;
        }
        return -1;
    }
    if(!lhs.getStartElement()->getPrev() || !rhs.getStartElement()->getPrev())
    {
        if(lhs.getStartElement()->getPrev())
        {
            return 1;
        }
        if(rhs.getStartElement()->getPrev())
        {
            return -1;
        }
    }
    tSigned32 comparison;
    if(!lhs.isInternalLoop() && lhs.getStartElement()->getPrev())
    {
        comparison = compareEntries(lhs._startElement, rhs._startElement);
        if(comparison != 0)
        {
            return comparison;
        }
        const cCircuitElement* lhs_e = lhs._startElement;
        const cCircuitElement* rhs_e = rhs._startElement;
        while(lhs_e->getNext() != lhs._startElement
            && rhs_e->getNext() != rhs._startElement
            )
        {
            lhs_e = lhs_e->getNext();
            rhs_e = rhs_e->getNext();
            comparison = compareCorners(lhs_e, rhs_e);
            if(comparison != 0)
            {
                return comparison;
            }
        }
        if(lhs_e->getNext() != lhs._startElement)
        {
        // lhs is longer
            return -1;
        }
        if(rhs_e->getNext() != rhs._startElement)
        {
        // rhs is longer
            return 1;
        }
        return compareExits(lhs_e, rhs_e);
    }

    const cCircuitElement* lhs_e = lhs._startElement;
    const cCircuitElement* rhs_e = rhs._startElement;
    do
    {
        comparison = compareCorners(lhs_e, rhs_e);
        if(comparison != 0)
        {
            return comparison;
        }
        lhs_e = lhs_e->getNext();
        rhs_e = rhs_e->getNext();
    }
    while(lhs_e && rhs_e && lhs_e->getNext() != lhs._startElement
            && rhs_e->getNext() != rhs._startElement
            );

    if(lhs_e == 0 || rhs_e == 0)
    {
        if(lhs_e)
        {
        // lhs is longer
            return -1;
        }
        if(rhs_e)
        {
        // rhs is longer
            return 1;
        }
        return 0;
    }

    if(lhs_e->getNext() != lhs._startElement)
    {
    // lhs is longer
        return -1;
    }
    if(rhs_e->getNext() != rhs._startElement)
    {
    // rhs is longer
        return 1;
    }
    return 0;
}

tEdge
cMeshCut::entryEdge() const
{
    assertD(_startElement);
    return _startElement->entryEdge();
}
tEdge
cMeshCut::exitEdge() const
{
    assertD(_startElement);
    return _startElement->getPrev()->exitEdge();
}

tFace
cMeshCut::entersFrom() const
{
    assertD(_startElement);
    return _startElement->externalFaceForEntry();
}

tFace
cMeshCut::exitsTo() const
{
    assertD(_startElement);
    return _startElement->getPrev()->externalFaceForExit();
}

tFace
cMeshCut::startFace() const
{
    assertD(_startElement);
    return _startElement->faceCrossed(0);
}

const tLine&
cMeshCut::refEntryLine() const
{
    assertD(!isInternalLoop());
    return _startElement->refLine();
}
const tLine&
cMeshCut::refExitLine() const
{
    assertD(!isInternalLoop());
    return _startElement->getPrev()->refLine();
}

void
cMeshCut::invert()
{
    std::vector<cCircuitElement*> elements;
    assertD(_startElement);
    cCircuitElement* e = _startElement;
    do
    {
        elements.push_back(e);
        e = e->getNext();
    }
    while(e != _startElement);
    size_t i;
    for(i = 0; i < elements.size(); i++)
    {
        elements[i]->reverse();
    }
    // this ensures that start element remains at the entry for non internal cuts
    _startElement = _startElement->getNext();
}

void
cMeshCut::remap(const iMeshTranslator<tMesh>& translator)
{
    cCircuitElement* e = _startElement;
    if(!e->enters())
    {
        tIntersection intersection;
        _startElement->getIntersectionForCornerAtStart(intersection);
        cIntersection_PointLike p(intersection);
        cMeshElement<tMesh> el = ElementForPointInOverlappingFace<tMesh, tIntersection>(_startElement->faceCrossed(0), intersection);
        cMeshElement<tMesh> remappedEl = translator.translate(el, p);
        tFace startFace = TranslateToOwned_SI(remappedEl, p);
        tFace checkStartFace = startFace;
        assertD(!startFace->isExternal());
        do
        {
            e->remakeTraversal(startFace);
            startFace = e->lastFaceCrossed();
            e = e->getNext();
        }
        while(e && e != _startElement);
    }
    else
    {
        tEdge entry = e->entryEdge();
        tEdge remappedEntry;
        RemapEntry(translator, entry, e->refLine(), remappedEntry);
        e->remakeTraversal(remappedEntry);
        while(!e->exits() && e->getNext())
        {
            tFace startFace = e->lastFaceCrossed();
            e = e->getNext();
            e->remakeTraversal(startFace);
        }
    }
}

void
cMeshCut::normalise()
{
    if(!isInternalLoop())
    {
        return;
    }
    cCircuitElement* best = _startElement;
    cCircuitElement* candidate = _startElement->getNext();
    while(candidate && candidate != _startElement)
    {
        if(lessThanForNormalise(best, candidate))
        {
            best = candidate;
        }
        candidate = candidate->getNext();
    }
    _startElement = best;
}

void
cMeshCut::normaliseLines()
{
    assertD(isInternalLoop());
    cCircuitElement* e = _startElement;
    do
    {
        e->normaliseLine();
        e = e->getNext();
    }
    while(e != _startElement);
}

bool
cMeshCut::isSmallConvex(cFacesContainedByBoundary& fcbb, tSigned32 maximumCircumference) const
{
    if(!isInternalLoop())
    {
        return false;
    }
    cCircuitElement* e = _startElement;
    do
    {
        if(e->getPrev()->refLine().side(e->refLine()) != SIDE_RIGHT)
        {
            return false;
        }
        e = e->getNext();
    }
    while(e != _startElement);
    {
        tSigned32 spinCount = 0;
        tPoint baseAxis = e->refLine().axis();
        do
        {
            UpdateSpinCount(baseAxis, spinCount, e->getNext()->refLine().axis());
            e = e->getNext();
        }
        while(e != _startElement);
        if(spinCount != 2)
        {
            assertD((spinCount & 1) == 0);
            assertD(spinCount > 2);
            return false;
        }
    }

    do
    {
        tSigned32 i;
        for(i = 0; i < e->edgesCrossed(); i++)
        {
            fcbb.edgeCrossed(e->edgeCrossed(i));
        }
        e = e->getNext();
    }
    while(e != _startElement);

    {
        std::vector<tFace> containedFaces;
        fcbb.closeAndTakeResult(containedFaces);
        tSigned32 i;
        for(i = 0; i != SizeL(containedFaces); ++i)
        {
            if(containedFaces[i]->isExternal())
            {
                return false;
            }
        }
    }

    tSigned32 circumference = 0;

    tIntersection intersection;
    e->getIntersectionForCornerAtStart(intersection);
    tPoint prevP = intersection.approximatePoint();

    do
    {
        e = e->getNext();

        tIntersection intersection;
        e->getIntersectionForCornerAtStart(intersection);
        tPoint p = intersection.approximatePoint();
        tPoint v = p - prevP;
        if(v.getRangeUsed() > maximumCircumference)
        {
            return false;
        }

        circumference += Length(v);
        if(circumference > maximumCircumference)
        {
            return false;
        }

        prevP = p;
    }
    while(e != _startElement);

    return true;
}

bool
cMeshCut::testPointIsOnBoundary(tFace f, const tIntersection& p) const
{
    assertD(_startElement);
    cCircuitElement* e = _startElement;
    do
    {
        if(e->testPointIsOnBoundary(f, p))
        {
            return true;
        }
        e = e->getNext();
    }
    while(e != _startElement);
    return false;
}

void
cMeshCut::getStartIntersection(tIntersection& result) const
{
    assertD(_startElement);
    assertD(isInternalLoop());
    _startElement->getIntersectionForCornerAtStart(result);
}

tFace
cMeshCut::getStartFace() const
{
    assertD(_startElement);
    assertD(isInternalLoop());
    return _startElement->faceCrossed(0);
}

void
cMeshCut::joinInlineCorners()
{
    assertD(_startElement);

    cCircuitElement* e = _startElement->getNext();
    cCircuitElement* lastProcessed = _startElement;
    cCircuitElement* processed;
    do
    {
        processed = e;
        e = e->getNext();
        if(processed->cornerIsInline())
        {
            if(_startElement == processed)
            {
                _startElement = e;
            }
            processed->getPrev()->join();
        }
    }
    while(processed != lastProcessed);

    assertD(!containsInlineCorner());
}

//void
//cMeshCut::removeZeroLengthSections()
//{
//    assertD(_startElement);
//    cCircuitElement::removeZeroLengthSections(_startElement);
//}

void
cMeshCut::checkNoZeroLengthSections()
{
    assertR(_startElement);
    cCircuitElement* e = _startElement;
    do
    {
        assertR(!e->sectionIsZeroLength());
        e = e->getNext();
    }
    while(e != _startElement);
}

void
cMeshCut::markZeroLengthSections()
{
    assertR(_startElement);
    cCircuitElement* e = _startElement->getNext();
    while(1)
    {
        cCircuitElement* processed = e;
        e = e->getNext();
        if(processed->sectionIsZeroLength())
        {
			if(processed->prevIsZeroLength() || e->prevIsZeroLength())
			{
				if(processed->getNext()->getNext() == processed)
				{
				  // collapse singularity
					cCircuitElement::deleteCircuit(_startElement);
					_startElement = 0;
					return;
				}
				processed->removeAndDelete();
			}
			else
			{
				processed->markAsZeroLength();
			}
            if(processed == _startElement)
            {
                _startElement = e;
                break;
            }
        }
        if(processed == _startElement)
        {
            break;
        }
    }
}

//void
//cMeshCut::convertZeroLengthSectionsToEndIntersects()
//{
//    assertD(isInternalLoop()); // non internal cuts are not currently supported by this method
//    cCircuitElement* e = _startElement->getNext();
//    cCircuitElement* last_processed = _startElement;
//    cCircuitElement* processed;
//    do
//    {
//        processed = e;
//        e = e->getNext();
//
//        if(processed->sectionIsZeroLength())
//        {
//            cCircuitElement* prev = processed->getPrev();
//            assertD(prev->refLine().side(processed->refLine()) != SIDE_CENTRE);
//            if(prev->refLine().side(processed->refLine()) == SIDE_RIGHT)
//            {
//                prev->setEndIntersect(processed->refLine());
//            }
//            else
//            {
//                prev->setEndIntersect(processed->refLine().reversed());
//            }
//
//            cCircuitElement* next = processed->getNext();
//            assertD(prev->refLine().direction(next->refLine()) != DIRECTION_FORWARD);
//
//            processed->remove();
//            if(_startElement == processed)
//            {
//                _startElement = e;
//            }
//            delete processed;
//        }
//    }
//    while(processed != last_processed);
//
//}

void
InvertCircuits(tMeshCircuits& circuits, tMeshCircuits& result)
{
    tMeshCutSequence exploded;
    tMeshCircuits::iterator i;
    for(i = circuits.begin(); i != circuits.end(); ++i)
    {
        if(CircuitIsInternal(**i))
        {
            (*i)->front()->invert();
            result.push_back(*i);
        }
        else
        {
            tMeshCutSequence::iterator j;
            for(j = (*i)->begin(); j != (*i)->end(); ++j)
            {
                (*j)->invert();
                exploded.push_back(*j);
            }
        }
    }
    ArrangeIntoCircuits(exploded, result);
    circuits.clear();
}

void RemapCutSequence(tMeshCutSequence& sequence, iMeshTranslator<tMesh>& translator)
{
    tMeshCutSequence::iterator j;
    for(j = sequence.begin(); j != sequence.end(); ++j)
    {
        (*j)->remap(translator);
    }
}
void RemapCircuits(tMeshCircuits& circuits, iMeshTranslator<tMesh>& translator)
{
    tMeshCircuits::iterator i;
    for(i = circuits.begin(); i != circuits.end(); ++i)
    {
        tMeshCutSequence::iterator j;
        for(j = (*i)->begin(); j != (*i)->end(); ++j)
        {
            (*j)->remap(translator);
        }
    }
}

class NormaliseCutSequencePredicate
{
public:

    bool operator()(cHandle<cMeshCut> lhs, cHandle<cMeshCut> rhs)
    {
        return cMeshCut::compare(*lhs, *rhs) == 1;
    }
};

void
NormaliseCutSequence(tMeshCutSequence& sequence)
{
    tMeshCutSequence::iterator i;
    for(i = sequence.begin(); i != sequence.end(); ++i)
    {
        (*i)->normalise();
    }
    ListSort(sequence._list, NormaliseCutSequencePredicate());
}

tSigned32
CompareCutSequences(const tMeshCutSequence& lhs, const tMeshCutSequence& rhs)
{
    tMeshCutSequence::const_iterator i, j;
    i = lhs.begin();
    j = rhs.begin();
    while(i != lhs.end() && j != rhs.end())
    {
        tSigned32 comparison = cMeshCut::compare(**i, **j);
        if(comparison != 0)
        {
            return comparison;
        }
        ++i;
        ++j;
    }
    if(i != lhs.end())
    {
    // lhs is longer
        return -1;
    }
    if(j != lhs.end())
    {
    // rhs is longer
        return 1;
    }
    return 0;
}

class NormaliseCircuitsPredicate
{
public:

    bool operator()(cHandle<tMeshCutSequence> lhs, cHandle<tMeshCutSequence> rhs)
    {
        return CompareCutSequences(*lhs, *rhs) == 1;
    }
};

void
NormaliseCircuits(tMeshCircuits& circuits)
{
    tMeshCircuits::iterator i;
    for(i = circuits.begin(); i != circuits.end(); ++i)
    {
        NormaliseCutSequence(**i);
    }
    ListSort(circuits._list, NormaliseCircuitsPredicate());
}

void
NormaliseLinesCutSequence(tMeshCutSequence& sequence)
{
    tMeshCutSequence::iterator i;
    for(i = sequence.begin(); i != sequence.end(); ++i)
    {
        (*i)->normaliseLines();
    }
}

void
NormaliseLinesCircuits(tMeshCircuits& circuits)
{
    tMeshCircuits::iterator i;
    for(i = circuits.begin(); i != circuits.end(); ++i)
    {
        NormaliseLinesCutSequence(**i);
    }
}

void
GetIntersectedExternalFaces(tSigned32 facesInMesh, tMeshCircuits& circuits,
                                 std::list<tFace>& result)
{
    std::vector<char> faceAdded(facesInMesh, false);
    tMeshCircuits::iterator i;
    for(i = circuits.begin(); i != circuits.end(); ++i)
    {
        tMeshCutSequence::iterator j;
        for(j = (*i)->begin(); j != (*i)->end(); ++j)
        {
            if(!(*j)->isInternalLoop())
            {
                tFace f = (*j)->entersFrom();
                if(!faceAdded[f.index()])
                {
                    faceAdded[f.index()] = true;
                    result.push_back(f);
                }
            }
        }
    }
}

cHandle<tMeshCutSequence>
CopyCutSequence(const tMeshCutSequence& to_copy)
{
    cHandle<tMeshCutSequence> result = new tMeshCutSequence;
    tMeshCutSequence::const_iterator i;
    for(i = to_copy.begin(); i != to_copy.end(); ++i)
    {
        result->push_back(new cMeshCut(**i));
    }
    return result;
}

cHandle<tMeshCircuits>
CopyCircuits(const tMeshCircuits& to_copy)
{
    cHandle<tMeshCircuits> result = new tMeshCircuits;
    tMeshCircuits::const_iterator i;
    for(i = to_copy.begin(); i != to_copy.end(); ++i)
    {
        result->push_back(CopyCutSequence(**i));
    }
    return result;
}

void
CopyCutSequence(const tMeshCutSequence& to_copy, tMeshCutSequence& appendTo)
{
    tMeshCutSequence::const_iterator i;
    for(i = to_copy.begin(); i != to_copy.end(); ++i)
    {
        appendTo.push_back(new cMeshCut(**i));
    }
}
void
CopyCircuits(const tMeshCircuits& to_copy, tMeshCircuits& appendTo)
{
    tMeshCircuits::const_iterator i;
    for(i = to_copy.begin(); i != to_copy.end(); ++i)
    {
        appendTo.push_back(CopyCutSequence(**i));
    }
}

bool
CircuitIsInternal(const tMeshCutSequence& circuit)
{
    return circuit._list.size() == 1 && circuit.front()->isInternalLoop();
}

bool
TestPointIsOnBoundary(const tMeshCutSequence& circuit, tFace f, const tIntersection& p)
{
    tMeshCutSequence::const_iterator i;
    for(i = circuit.begin(); i != circuit.end(); ++i)
    {
        if((*i)->testPointIsOnBoundary(f, p))
        {
            return true;
        }
    }
    return false;
}

void ConvertLegacyCircuitToCutSequence(
        cCircuitElement* circuit_start,
        tMeshCutSequence& sequence)
{
    if(!cCircuitElement::circuitExits(*circuit_start))
    {
        sequence._list.push_back(new cMeshCut(circuit_start));
        return;
    }
    cCircuitElement *e=circuit_start;
    while(!e->enters())
    {
        e=e->getPrev();
        assertD(e!=circuit_start); // must enter and exit somewhere to be converted to cuts
    }

    cCircuitElement *nexte;
    e->unlinkEntry();
    do
    {
        cCircuitElement *start_of_section=e;
        while(!e->exits())
        {
            e=e->getNext();
            assertD(e);
        }
        nexte=e->getNext();
        if(nexte)
        {
            e->unlinkExit();
        }
        e->connectTo(*start_of_section);
        sequence._list.push_back(new cMeshCut(start_of_section));
        e=nexte;
    }
    while(e);
}


cCircuitElement* ConvertCutSequenceToLegacyCircuit(tMeshCutSequence& sequence)
{
    if(sequence._list.empty())
    {
        return 0;
    }
    cCircuitElement* start_element = sequence._list.front()->takeCircuit();
    sequence._list.pop_front();

    if(sequence._list.empty())
    {
        return start_element;
    }

    assertD(start_element->enters());
    cCircuitElement* end_element = start_element->getPrev();
    end_element->unlinkExit();
    do
    {
        cCircuitElement* next_start = sequence._list.front()->takeCircuit();
        sequence._list.pop_front();
        assertD(next_start->enters());

        cCircuitElement* next_end = next_start->getPrev();
        next_end->unlinkExit();
        end_element->connectTo(*next_start);
        end_element = next_end;
    }
    while(!sequence._list.empty());
    end_element->connectTo(*start_element);
    return start_element;
}

//cHandle<tMeshCutSequence>
//ConvertLegacyCircuitToCutSequence(std::list<cCircuit>& legacy)
//{
//    cHandle<tMeshCutSequence> result = new tMeshCutSequence;
//
//    while(!legacy.empty())
//    {
//        result->push_back(new cMeshCut(legacy.front().getCircuit()));
//        legacy.pop_front();
//    }
//
//    return result;
//}

//void
//ConvertCutSequenceToLegacyCircuit(tMeshCutSequence& sequence,
//                                       std::list<cCircuit>& result)
//{
//    tMeshCutSequence::const_iterator i;
//    for(i = sequence.begin(); i != sequence.end(); ++i)
//    {
//        result.push_back(cCircuit());
//        (*i)->takeCircuit(result.back());
//    }
//    sequence.clear();
//}

bool
cMeshCut::containsInlineCorner() const
{
    assertD(_startElement);
    const cCircuitElement* e = _startElement;
    do
    {
        if(e->cornerIsInline())
        {
            return true;
        }
        e = e->getNext();
    }
    while(e && e != _startElement);
    return false;
}

bool
CircuitIsSmallConvex(cFacesContainedByBoundary& fcbb, const tMeshCutSequence& circuit, tSigned32 maximumCircumference)
{
    return circuit._list.size() == 1 && circuit.front()->isSmallConvex(fcbb, maximumCircumference);
}

void
SplitSmallConvex(tSigned32 edgesInMesh, tMeshCircuits& circuits, tSigned32 maximumCircumference, tMeshCircuits& appendTo)
{
    cFacesContainedByBoundary fcbb(edgesInMesh);
    tMeshCircuits::iterator i;
    for(i = circuits.begin(); i != circuits.end(); )
    {
        if(CircuitIsSmallConvex(fcbb, **i, maximumCircumference))
        {
            appendTo.push_back(*i);
            i = circuits._list.erase(i);
        }
        else
        {
            ++i;
        }
    }
}

bool
cMeshCut::validateLinks() const
{
    assertD(_startElement);
    const cCircuitElement* e = _startElement;
    do
    {
        if(!e->getNext())
        {
            Error("NonFatal","cMeshCut::validateLinks(): Link is not set. Validation fails.");
            return false;
        }
        if(e->getNext()->getPrev() != e)
        {
            Error("NonFatal","cMeshCut::validateLinks(): Links are inconsistent. Validation fails.");
            return false;
        }
        e = e->getNext();
    }
    while(e != _startElement);

    return true;
}

bool
cMeshCut::validate() const
{
    assertD(_startElement);
    const cCircuitElement* e = _startElement;
    do
    {
        if(!e->isValid_SI())
        {
            return false;
        }
        e = e->getNext();
    }
    while(e && e != _startElement);

    e = _startElement;
    while(e->getNext() && e->getNext() != _startElement)
    {
        if(e->exits() || e->getNext()->enters())
        {
            Error("NonFatal","cMeshCut::validate(): Exit found mid cut. Validation fails.");
            return false;
        }
        e = e->getNext();
    }

//    if(_startElement->enters())
//    {
//        if(!_startElement->getPrev()->exits())
//        {
//            Error("NonFatal","cMeshCut::validate(): Enters at start but doesn't exit at end. Validation fails.");
//            return false;
//        }
//    }
//    else
//    {
//        if(_startElement->getPrev()->exits())
//        {
//            Error("NonFatal","cMeshCut::validate(): Exits at end but doesn't enter at start. Validation fails.");
//            return false;
//        }
//    }

    // inline corners are not normally permitted
    e = _startElement;
    do
    {
        if(!e->exits() && e->getNext())
        {
            eDirection direction = 
                e->refLine().direction(e->getNext()->refLine());
            if(direction == DIRECTION_FORWARD)
            {
                Error("NonFatal","cMeshCut::validate(): Corner is inline. Validation fails.");
                return false;
            }
        }
        e = e->getNext();
    }
    while(e && e != _startElement);

    return true;
}

bool
cMeshCut::toNextElement(const cCircuitElement*& e) const
{
    e = e->getNext();
    if(e == _startElement)
    {
        e = 0;
        return false;
    }
    return true;
}

tFace
cMeshCut::remappedEntryFace(const iMeshTranslator<tMesh>& translator) const
{
    tEdge entry = entryEdge();
    tEdge entryOnPartitioned;
    const cCircuitElement* e = getStartElement();
    RemapEntry(translator, entry, e->refLine(), entryOnPartitioned);
    return entryOnPartitioned.face();
}

tFace
cMeshCut::remappedStartFace(const iMeshTranslator<tMesh>& translator) const
{
    tFace f = getStartFace();
    tIntersection intersection;
    getStartIntersection(intersection);
    cIntersection_PointLike ipl(intersection);
    return TranslateToPartitioned(translator, f, ipl);
}

tFace
cMeshCut::iterateOverFacesCrossed(const cCircuitElement*& e, tSigned32& faceIndex) const
{
    tFace result = e->faceCrossed(faceIndex);
    faceIndex++;
    if(faceIndex == e->facesCrossed())
    {
        faceIndex = 0;
        e = e->getNext();
        if(e == _startElement)
        {
            e = 0;
        }
    }
    return result;
}

void
JoinInlineCorners(tMeshCutSequence& cuts)
{
    tMeshCutSequence::iterator i;
    for(i = cuts._list.begin(); i != cuts._list.end(); ++i)
    {
        (*i)->joinInlineCorners();
    }
}

bool
CutSequenceContainsInlineCorner(const tMeshCutSequence& cuts)
{
    tMeshCutSequence::const_iterator i;
    for(i = cuts._list.begin(); i != cuts._list.end(); ++i)
    {
        if((*i)->containsInlineCorner())
        {
            return true;
        }
    }
    return false;
}
bool
CircuitsContainInlineCorner(const tMeshCircuits& circuits)
{
    tMeshCircuits::const_iterator i;
    for(i = circuits._list.begin(); i != circuits._list.end(); ++i)
    {
        if(CutSequenceContainsInlineCorner(**i))
        {
            return true;
        }
    }
    return false;
}


bool
ValidateCuts(const tMeshCutSequence& cuts)
{
    tMeshCutSequence::const_iterator i;
    for(i = cuts._list.begin(); i != cuts._list.end(); ++i)
    {
        if(!(*i)->getStartElement())
        {
            Error("NonFatal","ValidateCuts(): Cut has no elements. Validation fails.");
            return false;
        }

        if(!(*i)->validateLinks())
        {
            return false;
        }
        if(!(*i)->validate())
        {
            return false;
        }
    }
    return true;
}

bool
ValidateCircuitPart(const tMeshCutSequence& circuit)
{
    tMeshCutSequence::const_iterator i;
    for(i = circuit._list.begin(); i != circuit._list.end(); ++i)
    {
        if(!(*i)->getStartElement())
        {
            Error("NonFatal","ValidateCircuitPart(): Cut has no elements. Validation fails.");
            return false;
        }

        if(!(*i)->validateLinks())
        {
            return false;
        }
        if(!(*i)->validate())
        {
            return false;
        }

        if(circuit._list.size() > 1 && (*i)->isInternalLoop())
        {
            Error("NonFatal","ValidateCircuitPart(): Circuit with more than one cut contains an internal cut. Validation fails.");
            return false;
        }
    }
    return true;
}

bool
ValidateCircuit(const tMeshCutSequence& circuit)
{
    if(circuit._list.empty())
    {
        Error("NonFatal","ValidateCircuit(): Circuit contains no cuts. Validation fails.");
        return false;
    }
    if(!ValidateCircuitPart(circuit))
    {
        return false;
    }
    tMeshCutSequence::const_iterator i;
    for(i = circuit._list.begin(); i != circuit._list.end(); ++i)
    {
        if(!(*i)->isInternalLoop())
        {
            tMeshCutSequence::const_iterator next_i = i;
            ++next_i;
            if(next_i == circuit._list.end())
            {
                next_i = circuit._list.begin();
            }
            if((*i)->exitsTo() != (*next_i)->entersFrom())
            {
                Error("NonFatal","ValidateCircuit(): Faces do not agree for exit and entry. Validation fails.");
                return false;
            }
        }
    }
    return true;
}

bool ValidateCircuits(const tMeshCircuits& circuits)
{
    tMeshCircuits::const_iterator i;
    for(i = circuits.begin(); i != circuits.end(); ++i)
    {
        if(!ValidateCircuit(**i))
        {
            return false;
        }
    }
    return true;
}

bool
CutSequenceContainsAnExternalFace(const tMeshCutSequence& sequence)
{
    std::vector<tEdge> threaded;

    // run around the edge of region represented by this cut sequence
    // i.e. along the cuts themselves and along external edges between cut exit and entry
    {
        tMeshCutSequence::const_iterator j;
        for(j = sequence.begin(); j != sequence.end(); ++j)
        {
            const cCircuitElement* e = (*j)->getStartElement();
            do
            {
                tSigned32 k;
                for(k = 0; k < e->facesCrossed(); k++)
                {
                    if(k)
                    {
                        tEdge crossed = e->edgeCrossed(k - 1);
                        if(!threaded.empty() && threaded.back() == crossed.twin())
                        {
                            threaded.pop_back();
                        }
                        else
                        {
                            threaded.push_back(crossed);
                        }
                    }
                }
                e = e->getNext();
            }
            while(e != (*j)->getStartElement());

            if(e->enters())
            {
            // sequence is not internal
            // process external edges between this cut's exit and the next cut's entry
                tMeshCutSequence::const_iterator nextJ = j;
                ++nextJ;
                if(nextJ == sequence.end())
                {
                    nextJ = sequence.begin();
                }
                const cCircuitElement* before = e->getPrev();
                const cCircuitElement* after = (*nextJ)->getStartElement();
                tEdge start, end;
                start = before->exitEdge();
                end = after->entryEdge();
                while(start != end)
                {
                    ++start;
                    while(!start.twin().face()->isExternal())
                    {
                        tEdge crossed = start;
                        if(!threaded.empty() && threaded.back() == crossed.twin())
                        {
                            threaded.pop_back();
                        }
                        else
                        {
                            threaded.push_back(crossed);
                        }
                        start = start.twin().next();
                    }
                }
            }
        }
    }
    return !threaded.empty();
}

void
AddSequenceToStats(const tMeshCutSequence& sequence, cCircuitStats& stats)
{
    stats._sequences++;
    tMeshCutSequence::const_iterator j;
    for(j = sequence.begin(); j != sequence.end(); ++j)
    {
        (*j)->addToStats(stats);
    }
}
void
AddCircuitsToStats(const tMeshCircuits& circuits, cCircuitStats& stats)
{
    stats._circuits++;
    tMeshCircuits::const_iterator j;
    for(j = circuits.begin(); j != circuits.end(); ++j)
    {
        AddSequenceToStats(**j, stats);
    }
}
tSigned32
CountElements(const tMeshCutSequence& sequence)
{
    tSigned32 result = 0;
    tMeshCutSequence::const_iterator j;
    for(j = sequence.begin(); j != sequence.end(); ++j)
    {
        result += (*j)->countElements();
    }
    return result;
}
tSigned32
CountElements(const tMeshCircuits& circuits)
{
    tSigned32 result = 0;
    tMeshCircuits::const_iterator j;
    for(j = circuits.begin(); j != circuits.end(); ++j)
    {
        result += CountElements(**j);
    }
    return result;
}

tSigned32
IndexElements(const tMeshCutSequence& sequence, tSigned32 startIndex)
{
    tSigned32 currentIndex = startIndex;
    tMeshCutSequence::const_iterator j;
    for(j = sequence.begin(); j != sequence.end(); ++j)
    {
        currentIndex = (*j)->indexElements(currentIndex);
    }
    return currentIndex;
}
tSigned32
IndexElements(const tMeshCircuits& circuits, tSigned32 startIndex)
{
    tSigned32 currentIndex = startIndex;
    tMeshCircuits::const_iterator j;
    for(j = circuits.begin(); j != circuits.end(); ++j)
    {
        currentIndex = IndexElements(**j, currentIndex);
    }
    return currentIndex;
}

tFace ReTraverseElement(const cCircuitElement* element, tFace startF)
{
    tIntersection targetI;
    element->getNext()->getIntersectionForCornerAtStart(targetI);
    cIntersection_PointLike target(targetI);
    tFace endF = startF;
    tEdge edgeCrossed;
    while(TraverseToTarget_SI<tMesh>(endF, element->refLine(), target, edgeCrossed));
    return endF;
}

void
GenerateCircuitsCheckSum(const tMeshCircuits& circuits, iOutputStream& os)
{
    os << static_cast<tSigned32>(circuits.size());
    tMeshCircuits::const_iterator i;
    for(i = circuits.begin(); i != circuits.end(); ++i)
    {
        os << static_cast<tSigned32>((*i)->size());
        tMeshCutSequence::const_iterator j;
        for(j = (*i)->begin(); j != (*i)->end(); ++j)
        {
            const cMeshCut& cut = **j;

            {
                const cCircuitElement* e = cut.getStartElement();
                tSigned32 faceIndex = 0;
                do
                {
                    tFace f = cut.iterateOverFacesCrossed(e, faceIndex);
                    os << f.index();
                }
                while(e);
            }
            const cCircuitElement* e = cut.getStartElement();
            do
            {
                if(!e->enters() && e->cornerIsConvex())
                {
                    os << e->cornerPoint().getX();
                    os << e->cornerPoint().getY();
                }
                e = e->getNext();
            }
            while(e != cut.getStartElement());
        }
    }
}

cCircuitsConstIterator::cCircuitsConstIterator(tMeshCircuits& circuits)
{
    if(circuits.empty())
    {
        _el = 0;
        return;
    }
    _circuits = &circuits;
    _circuit = circuits.begin();
    assertD(!(*_circuit)->empty());
    _cutSequence = &**_circuit;
    assertD(!_cutSequence->empty());
    _cut = _cutSequence->begin();
    _startEl = (*_cut)->getStartElement();
    assertD(_startEl);
    _el = _startEl;
}

cCircuitsConstIterator&
cCircuitsConstIterator::operator++()
{
    assertD(_el);
    _el = _el->getNext();
    if(_el != _startEl)
    {
        return *this;
    }
//    if(!_cutSequence)
//    {
//        _el = 0;
//        return *this;
//    }
    ++_cut;
    if(_cut != _cutSequence->end())
    {
        _startEl = (*_cut)->getStartElement();
        assertD(_startEl);
        _el = _startEl;
        return *this;
    }
//    if(!_circuits)
//    {
//        _el = 0;
//        return *this;
//    }
    ++_circuit;
    if(_circuit != _circuits->end())
    {
        _cutSequence = &**_circuit;
        assertD(!(*_circuit)->empty());
        _cut = (*_circuit)->begin();
        _startEl = (*_cut)->getStartElement();
        assertD(_startEl);
        _el = _startEl;
        return *this;
    }
    _el = 0;
    return *this;
}

