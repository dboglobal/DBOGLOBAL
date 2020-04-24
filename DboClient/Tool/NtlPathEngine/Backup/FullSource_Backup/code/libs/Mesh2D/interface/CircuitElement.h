//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef CIRCUITELEMENT_INCLUDED
#define CIRCUITELEMENT_INCLUDED

#include "libs/Geometry/interface/tLine.h"
#include "libs/Geometry/interface/tPoint.h"
#include "libs/Geometry/interface/tIntersection_Header.h"
#include "libs/Mesh2D/interface/EdgesTraversal.h"
#include "libs/Mesh2D/interface/InternalPosition.h"
#include "libs/Geometry/interface/SideEnum.h"
#include "common/STL/vector.h"

class cExpandedCornerEntry;
class cExpandedConcaveCornerEntry;
class cExpandedLineEntry;
class cFaceElements;
class cAgent;
class cMeshPair;
class iSerialiser;

class bCircuitElementTraversal : public cEdgesTraversal
{
    tLine _line;

protected:

    bCircuitElementTraversal() {}

    // the traversal is swapped in
    void init(const tLine& line, cEdgesTraversal& traversal);

    void init_InSingleFace(const tLine& line, tFace f);
    void init_ZeroLength(const tLine& line);
    void init_AsClone(const bCircuitElementTraversal& clone_of);
    void init_AsCloneOfPart(const bCircuitElementTraversal& toClone, tSigned32 firstFace, tSigned32 lastFace);
    void init_ForAfterSplit(const bCircuitElementTraversal& toClone, tSigned32 firstFace);

    bool isValid_SI() const;

    void truncate(tSigned32 indexForLastFace);
    void reverse();
    void remakeTraversal(tEdge newEntry);
    void remakeTraversal(tEdge newEntry, const tLine& intersect);
    void remakeTraversal(tFace newStartFace);
    void remakeTraversal(tFace newStartFace, const tLine& intersect);

    bool isZeroLength() const
    {
        return cEdgesTraversal::empty();
    }
    void reverseZeroLength()
    {
        assertD(isZeroLength());
        _line.reverse();
    }

    void normaliseLine_Internal(const tLine& lineBefore, const tLine& lineAfter);

public:

    //.... TODO make these private
    void addTraversal(const bCircuitElementTraversal& element2);
    void addTraversal(const bCircuitElementTraversal* ptr)
    {
        addTraversal(*ptr);
    }

    tLine getEntryIntersect(bool& inclusive) const;
    tLine getExitIntersect(bool& inclusive) const;

    const tLine& refLine() const {return _line;}
};

class cCircuitElement : public bCircuitElementTraversal
{
    cCircuitElement* next;
    cCircuitElement* prev;
    mutable tSigned32 _index;

    void unlink();
    void unlinkStart();
    void unlinkEnd();
    void connectTo_Internal(cCircuitElement& target);
    bool validationCommon() const;

public:

    cCircuitElement()
    {
        _index = 0;
    }
    ~cCircuitElement();

    void setIndex(tSigned32 value)
    {
        assertD(value >= 0);
        if(_index < 0)
        {
            value = -1 - value;
        }
        _index = value;
    }
    tSigned32 getIndex() const
    {
        if(_index < 0)
        {
            return -1 - _index;
        }
        return _index;
    }

    void markReferenced() const
    {
        assertD(_index >= 0);
        _index = -1 - _index;
    }
    bool hasBeenReferenced() const
    {
        return _index < 0;
    }
    void clearReferenced() const
    {
        assertD(_index < 0);
        _index = -1 - _index;
    }

    void init(const tLine& line, cEdgesTraversal& traversal);

    void init_InSingleFace(const tLine& line, tFace f);

    void init_WithStartCap(const tLine& line, const tLine& startCap, cEdgesTraversal& traversal);
    void init_WithEndCap(const tLine& line, const tLine& endCap, cEdgesTraversal& traversal);
    void init_WithStartAndEndCaps(const tLine& line, const tLine& startCap, const tLine& endCap, cEdgesTraversal& traversal);

    void init_AsClone(const cCircuitElement& toClone);
    void init_AsCloneOfPart(const cCircuitElement& toClone, tSigned32 firstFace, tSigned32 lastFace);
    void connectTo(cCircuitElement&);
    void connectTo(cCircuitElement* ptr)
    {
        connectTo(*ptr);
    }

    void disconnectStart();
    void disconnectStart_LeaveUncapped();
    void capEnd(const tLine& capLine);

//    void operator>>(iSerialiser&);
//    static void remapConnection(cCircuitElement& e1, cCircuitElement& e2);

    bool operator==(const cCircuitElement& rhs) const;
    bool operator!=(const cCircuitElement& rhs) const
    {
        return !((*this)==rhs);
    }

    void unlinkEntry()
    {
        assertD(prev);
        assertD(!prev->isZeroLength());
        prev->next=0;
        prev=0;
    }
    void unlinkExit()
    {
        assertD(next);
        assertD(!next->isZeroLength());
        next->prev=0;
        next=0;
    }

    void removeAndDelete();
    void split(tSigned32 faceIndexForSplit, const tLine& intersectLine);
    void join();
    void reverse();

    // redirects corners at ends of the two elements
    // TODO standardise this to corner at start
    void redirect(cCircuitElement&);

    // TODO get rid of this - zero length stuff should be internal only
    void markAsZeroLength();

    void popEdgeCrossing()
    {
        bCircuitElementTraversal::popEdgeCrossing();
    }

    void normaliseLine();

    void remakeTraversal(tEdge newEntry);
    void remakeTraversal(tFace newStartFace);

    bool cornerIsInline() const;
    bool sectionIsZeroLength() const;
    bool cornerIsConvex() const;

    tLine getStartIntersect_DoesntEnter() const;
    tLine getStartIntersect(bool& inclusive) const;
    tLine getStartIntersect() const;
    tLine getEndIntersect_DoesntExit() const;
    tLine getEndIntersect(bool& inclusive) const;
    tLine getEndIntersect() const;

    void getStartFraction(tPoint::tDistance& n, tPoint::tDistance& d) const;
    void getEndFraction(tPoint::tDistance& n, tPoint::tDistance& d) const;
    void getStartOrEntryFraction(tPoint::tDistance& n, tPoint::tDistance& d) const;
    void getEndOrExitFraction(tPoint::tDistance& n, tPoint::tDistance& d) const;

    // intersecting lines must be to the right of this element's line
    // return invalid face or -1 if intersection is not on this element
    tSigned32 getFaceIndexForIntersection(const tLine& intersectingLine) const;
    tFace getFaceForIntersection(const tLine& intersectingLine) const;
    tSigned32 getFaceIndexForIntersection(const tLine& intersectingLine, bool& atCorner) const; // corner could be either start or end

    tPoint cornerPoint() const;
    void getIntersectionForCornerAtStart(tIntersection& result) const; //... get rid of this
    void cornerIntersection(tIntersection& result) const
    {
        getIntersectionForCornerAtStart(result);
    }
    void nextCornerIntersection(tIntersection& result) const;
    const tLine& getIntersectAtCorner(const tLine& line) const;    // refers to the start corner
    cInternalPosition cornerIP() const
    {
        return cInternalPosition(faceCrossed(0), cornerPoint());
    }

    cCircuitElement* getNext()
    {
        if(next && next->isZeroLength())
            return next->next;
        return next;
    }
    cCircuitElement* getPrev()
    {
        if(prev && prev->isZeroLength())
            return prev->prev;
        return prev;
    }
    const cCircuitElement* getNext() const
    {
        if(next && next->isZeroLength())
            return next->next;
        return next;
    }
    const cCircuitElement* getPrev() const
    {
        if(prev && prev->isZeroLength())
            return prev->prev;
        return prev;
    }

    bool isValid_SI() const;

    bool prevIsZeroLength() const
    {
        return prev->isZeroLength();
    }
    const tLine& refZeroLengthLine() const
    {
        assertD(prev->isZeroLength());
        return prev->refLine();
    }
    const tLine& refPrevOrZeroLengthLine() const
    {
        return prev->refLine();
    }
    const tLine& refNextOrZeroLengthLine() const
    {
        return next->refLine();
    }
    const tLine& refEndCapLine() const
    {
        assertD(next->isZeroLength());
        return next->refLine();
    }

// ============= collision queries

    bool exitExitsThroughExternalSection(
                    tEdge exitEE,
                    const tLine& exitLine,
                    const cCircuitElement& next
                    ) const;

    void getFaceBounds(tSigned32 faceIndex,
                    bool& startInclusive,
                    tLine& startIntersect,
                    bool& endInclusive,
                    tLine& endIntersect
                    ) const;

    bool lineEntersZeroLength(const tLine& line) const;
    bool lineEntersInFace_New(
        tSigned32 faceIndex, const tLine& line, bool& atStartCorner, bool& atEndCorner
        ) const;
    bool lineEntersInFaceOrZeroLength(
         tSigned32 faceIndex, 
         const tLine& line,
         bool& atStartCorner, bool& atEndCorner, const tLine*& intersectLine
         ) const;

  // circuit crossing tests

    // for cases where face indices are known

    bool elementEntersThroughThis(
            tSigned32 thisElementFaceIndex,
            const cCircuitElement& testElement,
            tSigned32 testElementFaceIndex,
            bool& atStartOfThis, bool& atEndOfThis,
            bool& atStartOfTest
            ) const;
    bool elementExitsThroughThis(
            tSigned32 thisElementFaceIndex,
            const cCircuitElement& testElement,
            tSigned32 testElementFaceIndex,
            bool& atStartOfThis, bool& atEndOfThis,
            bool& atEndOfTest
            ) const;

    // for cases where face indices are not known

    bool elementEntersThroughThis(
            const cCircuitElement& testElement,
            bool& atStartOfThis, bool& atEndOfThis,
            bool& atStartOfTest,
            tSigned32& thisFaceIndex, tSigned32& testFaceIndex
            ) const;
    bool elementExitsThroughThis(
            const cCircuitElement& testElement,
            bool& atStartOfThis, bool& atEndOfThis,
            bool& atEndOfTest,
            tSigned32& thisFaceIndex, tSigned32& testFaceIndex
            ) const;

    // test for if point is exactly on the boundary
    bool testPointIsOnBoundary(tFace f, const tIntersection& p) const;


// =========== stuff operating on whole circuits
//... move all this kind of stuff to cMeshCut?

    static void deleteCircuit(cCircuitElement* start);

    static bool circuitExits(const cCircuitElement& start);

    bool circuitIsSingleCut() const;
    bool circuitIsInternal() const;

    static cCircuitElement* copyCircuit(const cCircuitElement* first);
};

// ============== stuff for comparing exits
//.... move to another file

eSide
SideOfExit(tEdge exitEE, const tLine& exitLine, const tLine& testLine);
eSide
SideOfEntry(tEdge entryEE, const tLine& entryLine, const tLine& testLine);

bool
EntryIsAfterExit(tEdge elementEdge, const tLine& exitLine, const tLine& entryLine);
bool
EntryIsAfterEntry(tEdge elementEdge, const tLine& baseEntryLine, const tLine& testEntryLine);

inline void
CircuitElement2DData(const cCircuitElement* e, tPoint* elementDataDest, bool& hasConvexCorner)
{
    elementDataDest[0] = e->getPrev()->refLine().end();
    elementDataDest[1] = e->refLine().start();
    elementDataDest[2] = e->refLine().end();
    hasConvexCorner = e->cornerIsConvex();
}

inline bool
ElementCrossesFace(const bCircuitElementTraversal& e, tFace f)
{
    tSigned32 i;
    for(i = 0; i < e.facesCrossed(); ++i)
    {
        if(e.faceCrossed(i) == f)
        {
            return true;
        }
    }
    return false;
}

#endif
