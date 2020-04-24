//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/OverlappingPolygon.h"
#include "libs/Instrumentation/interface/Profiler.h"
#include "interface/QueryContext.h"
#include "libs/Mesh2D/interface/CircuitElement.h"
#include "libs/Mesh2D/interface/ArrangeIntoCircuits.h"
#include "libs/Mesh2D/interface/InternalPosition.h"
#include "libs/Mesh2D/interface/MeshTraversal.h"
#include "libs/Mesh_Common/interface/PositionForPointInFace.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include "libs/Geometry/interface/ConvexPoly.h"
#include "libs/Geometry/interface/Point_PointLike.h"
#include "libs/Geometry/interface/tPoint_Multiplication.h"
#include "platform_common/CoordinateRanges.h"
#include "common/Containers/FlagVector.h"
#include "common/STL/algorithm.h"

static bool
PolygonIsEntirelyWithinStartFace(const tFace& startFace, const cConvexPoly& polygon)
{
    tSigned32 i;
    for(i = 0; i < polygon.sides(); i++)
    {
        if(!PointIsInFace<tMesh>(startFace, polygon.vertex(i)))
        {
            return false;
        }
    }
    return true;
}

static cCircuitElement*
CircuitForPolygonEntirelyWithinStartFace(const tFace& startFace, const cConvexPoly& polygon)
{
    cCircuitElement* head = 0;
    cCircuitElement* prev = 0;
    tSigned32 i;
    for(i = 0; i < polygon.sides(); i++)
    {
        tLine line(polygon.vertex(i), polygon.vertex_WrappedAround(i + 1));
        cCircuitElement* e = new cCircuitElement;
        e->init_InSingleFace(line, startFace);
        if(prev)
        {
            prev->connectTo(*e);
        }
        else
            head = e;
        prev = e;
    }
    prev->connectTo(*head);
    return head;
}

class cEntryOrExitInfo
{
public:

    tEdge e;
    tSigned32 i;

    bool operator==(const cEntryOrExitInfo &rhs) const
    {
        return e == rhs.e &&
               i == rhs.i;
    }
};

static tSigned32
GetEntriesAndExits(
        tFace f,
        const cConvexPoly& polygon,
        std::vector<cEntryOrExitInfo>& info
        )
{
    PROFILER_SCOPE("GetEntriesAndExits");
    tSigned32 i;

  // loop through lines in polygon to find any that enter this face

    tSigned32 numberOfEntries = 0;
    tSigned32 prevI = polygon.sides() - 1;
    for(i = 0; i < polygon.sides(); i++)
    {
        tLine reverseLine(polygon.vertex(i), polygon.vertex(prevI));
        bool enters;
        tEdge e;
        enters = LineCrossesAndExitsFace_SI<tMesh>(f, reverseLine, e);
        if(enters)
        {
            info[numberOfEntries * 2].i = prevI;
            info[numberOfEntries * 2].e = e;
            numberOfEntries++;
        }
        prevI = i;
    }

    PROFILER_CHANGES("GetEntriesAndExits exits");

  // get exit info for each entry

    for(i = 0; i < numberOfEntries; i++)
    {
        tSigned32 j = info[i * 2].i;
        tEdge edgeCrossed;
        bool exited;

        while(1)
        {
            tSigned32 nextJ = polygon.nextVertex(j);
            tLine line(polygon.vertex(j), polygon.vertex(nextJ));
            cPoint_PointLike target(line.end());

            tFace traverseF = f;
            bool iterate;
            iterate = TraverseTowardsTarget_SI<tMesh>(traverseF, line, target, edgeCrossed);
            exited = traverseF->isExternal();
            if(iterate || exited)
            {
                break;
            }

            j = nextJ;
        }

        info[i * 2 + 1].i = j;
        info[i * 2 + 1].e = edgeCrossed;
    }
    return numberOfEntries;
}

static void
AddAllAdjoining(
        tFace f,
        std::list<tFace> &open,
        const std::list<tFace> &closed,
        std::list<tFace> &external
        )
{
    std::list<tFace> added;
    tEdge e=tMesh::beginEdge(f);
    do
    {
        tFace f2=e.twin().face();
        if(f2->isExternal())
        {
            if(std::find(external.begin(),external.end(),f2)!=external.end())
                continue;
            external.push_back(f2);
        }
        else
        {
            if(std::find(open.begin(),open.end(),f2)!=open.end())
                continue;
            if(std::find(closed.begin(),closed.end(),f2)!=closed.end())
                continue;
            added.push_back(f2);
        }
    }
    while(++e!=tMesh::beginEdge(f));
    open.splice(open.begin(),added);
}

static void
AddAdjoiningBetween(
        const cEntryOrExitInfo& exit,
        const cEntryOrExitInfo& entry,
        std::list<tFace>& open,
        const std::list<tFace>& closed,
        std::list<tFace>& external
        )
{
    std::list<tFace> added;
    tEdge e = exit.e;
    tEdge after = entry.e;
    ++after;
    do
    {
        tFace f2 = e.twin().face();
        if(f2->isExternal())
        {
            if(std::find(external.begin(),external.end(),f2)!=external.end())
            {
                continue;
            }
            external.push_back(f2);
        }
        else
        {
            if(std::find(open.begin(),open.end(),f2)!=open.end())
            {
                continue;
            }
            if(std::find(closed.begin(),closed.end(),f2)!=closed.end())
            {
                continue;
            }
            added.push_back(f2);
        }
    }
    while(++e != after);
    open.splice(open.begin(),added);
}

static void
AddAdjoiningFaces(
        tFace f,
        const std::vector<cEntryOrExitInfo>& info,
        tSigned32 numberOfEntries,
        std::list<tFace>& open,
        const std::list<tFace>& closed,
        std::list<tFace>& external
        )
{
    if(numberOfEntries == 0)
    {
        AddAllAdjoining(f, open, closed, external);
        return;
    }
    tSigned32 i;
    for(i = 0; i < numberOfEntries; i++)
    {
        tSigned32 nextI = i + 1;
        if(nextI == numberOfEntries)
        {
            nextI = 0;
        }
        AddAdjoiningBetween(info[i * 2 + 1], info[nextI * 2 + 0], open, closed, external);
    }
}

static bool
CircuitElementsConnect(const cCircuitElement& e1, const cCircuitElement& e2)
{
    if(e1.exits() || e2.enters())
    {
        return false;
    }
    if(e1.edgesCrossed() == 0 || e2.edgesCrossed() == 0)
    {
        return false;
    }
    if(e1.refLine() != e2.refLine())
    {
        return false;
    }
    return e1.lastEdgeCrossed() == e2.edgeCrossed(0);
}

static void
AddCircuitPart(
        const cEntryOrExitInfo &entry,
        const cEntryOrExitInfo &exit,
        const cConvexPoly& polygon,
        std::list<cCircuitElement*> &circuits,
        std::list<cCircuitElement*> &circuitEnds
        )
{
    assertD(!(exit == entry));

// construct circuit part

    cCircuitElement* part = 0;
    cCircuitElement* partEnd;
    tSigned32 i = entry.i;
    while(1)
    {
        tSigned32 nextI = polygon.nextVertex(i);
        tLine line(polygon.vertex(i), polygon.vertex(nextI));

        cEdgesTraversal traversal;
        if(i == entry.i)
        {
            if(entry.e.twin().face()->isExternal())
            {
                tEdge entryEE(entry.e);
                traversal.init_Entry(entryEE);
            }
            else
            {
                traversal.init_InFace(entry.e.twin().face());
                traversal.pushEdgeCrossing(entry.e.twin());
            }
        }
        else
        {
            traversal.init_InFace(entry.e.face());
        }
        if(i == exit.i)
        {
            if(exit.e.twin().face()->isExternal())
            {
                tEdge exitEE(exit.e);
                traversal.pushExit(exitEE);
            }
            else
            {
                traversal.pushEdgeCrossing(exit.e);
            }
        }

        cCircuitElement* e = new cCircuitElement;
        e->init(line, traversal);

        if(part)
        {
            partEnd->connectTo(*e);
        }
        else
        {
            part = e;
        }
        partEnd = e;

        if(i == exit.i)
        {
            break;
        }

        i = nextI;
    }


// check if this part must be connected to any other part

    std::list<cCircuitElement*>::iterator it, endIt;

    // check for end of a previously added circuit connecting to start of part
    for(it = circuits.begin(), endIt = circuitEnds.begin();
        it != circuits.end();
        ++it, ++endIt)
    {
        if(CircuitElementsConnect(**endIt, *part))
        {
            (*endIt)->popEdgeCrossing();
            (*endIt)->addTraversal(*part);
            if(part == partEnd)
            {
                partEnd = *endIt;
                delete part;
            }
            else
            {
                cCircuitElement* afterFirst = part->getNext();
                part->removeAndDelete();
                (*endIt)->connectTo(afterFirst);
            }

            part=*it;
            circuits.erase(it);
            circuitEnds.erase(endIt);

            // check for case where resulting circuit must be closed
            if(CircuitElementsConnect(*partEnd, *part))
            {
                assertD(part != partEnd);
                partEnd->popEdgeCrossing();
                partEnd->addTraversal(part);
                cCircuitElement* start = part->getNext();
                part->removeAndDelete();
                partEnd->connectTo(start);
                circuits.push_back(start);
                circuitEnds.push_back(partEnd);

                return;
            }
            break;
        }
    }

    // check for end of part connecting to start of a previously added circuit
    for(it=circuits.begin(),endIt=circuitEnds.begin();
        it!=circuits.end();
        ++it,++endIt)
    {
        if(CircuitElementsConnect(*partEnd, **it))
        {
            partEnd->popEdgeCrossing();
            partEnd->addTraversal(*it);
            if(*it == *endIt)
            {
                *endIt = partEnd;
                delete *it;
            }
            else
            {
                cCircuitElement* afterFirst = (*it)->getNext();
                (*it)->removeAndDelete();
                partEnd->connectTo(afterFirst);
            }

            *it=part;
            return;
        }
    }

    circuits.push_back(part);
    circuitEnds.push_back(partEnd);
}

static void
AddCircuitPartsInFace(
        const std::vector<cEntryOrExitInfo> &info,
        tSigned32 numberOfEntries,
        const cConvexPoly& polygon,
        std::list<cCircuitElement*> &circuits,
        std::list<cCircuitElement*> &circuitEnds
        )
{
    tSigned32 i;
    for(i = 0; i < numberOfEntries; i++)
    {
        AddCircuitPart(info[i * 2], info[i * 2 + 1], polygon, circuits, circuitEnds);
    }
}

static void
CircuitPartsForOverlappingPolygon(
        tFace centreF,
        const cConvexPoly& polygon,
        std::list<cCircuitElement*>& circuits,
        std::list<cCircuitElement*>& circuitEnds,
        std::list<tFace>& containedExternalFaces
        )
{
    std::vector<cEntryOrExitInfo> info(polygon.sides() * 2);

    std::list<tFace> open, closed;
    open.push_back(centreF);
    while(!open.empty())
    {
        tFace f = open.front();
        open.pop_front();
        tSigned32 numberOfEntries = GetEntriesAndExits(f, polygon, info);
        AddCircuitPartsInFace(info, numberOfEntries, polygon, circuits, circuitEnds);
        AddAdjoiningFaces(f, info, numberOfEntries, open, closed, containedExternalFaces);
        closed.push_back(f);
    }

    // at this point we can remove external faces that any circuits enter from

    std::list<cCircuitElement*>::iterator it;
    for(it = circuits.begin(); it != circuits.end(); ++it)
    {
        if((*it)->getPrev() == 0) // checks that the circuit part is not already closed
        {
            tFace entryFace = (*it)->externalFaceForEntry();
            std::list<tFace>::iterator i;
            i = std::find(containedExternalFaces.begin(), containedExternalFaces.end(), entryFace);
            if(i != containedExternalFaces.end())
            {
                containedExternalFaces.erase(i);
            }
        }
    }
}

static void
CircuitsFromCircuitParts(
        std::list<cCircuitElement*>& circuits,
        std::list<cCircuitElement*>& circuitEnds,
        tMeshCircuits& appendTo
        )
{
// connect entries and exits

    // first add any circuits that are already closed

    std::list<cCircuitElement*>::iterator it, endIt;

    for(it = circuits.begin(), endIt = circuitEnds.begin();
        it != circuits.end();
        )
    {
        if((*it)->getPrev()!=0)
        {
            assertD((*it)->getPrev() == (*endIt));
            assertD((*endIt)->getNext() == (*it));
            
            appendTo.push_back(new tMeshCutSequence);
            appendTo.back()->push_back(new cMeshCut(*it));

            it = circuits.erase(it);
            endIt = circuitEnds.erase(endIt);
        }
        else
        {
            ++it;
            ++endIt;
        }
    }

    cHandle<tMeshCutSequence> toArrange = new tMeshCutSequence;
    while(!circuits.empty())
    {
        circuitEnds.front()->connectTo(*circuits.front());
        toArrange->push_back(new cMeshCut(circuits.front()));
        circuits.pop_front();
        circuitEnds.pop_front();
    }

    ArrangeIntoCircuits_Convex(*toArrange, appendTo);
}


void
CircuitsForOverlappingPolygon(
        tFace startF,
        const cConvexPoly& polygon,
        std::list<tFace>& containedExternalFaces,
        tMeshCircuits& appendTo
        )
{
    if(PolygonIsEntirelyWithinStartFace(startF, polygon))
    {
        cCircuitElement* e = CircuitForPolygonEntirelyWithinStartFace(startF, polygon);
        appendTo.push_back(new tMeshCutSequence);
        appendTo.back()->push_back(new cMeshCut(e));
        return;
    }

    std::list<cCircuitElement*> circuits;
    std::list<cCircuitElement*> circuitEnds;
    CircuitPartsForOverlappingPolygon(startF, polygon, circuits, circuitEnds, containedExternalFaces);
    CircuitsFromCircuitParts(circuits, circuitEnds, appendTo);
}


template <class tPolygon> bool
VertexIsInPolygon(const tPolygon& polygon, const tPoint& p)
{
    tSigned32 i;
    for(i = 0; i < polygon.edges(); i++)
    {
        if(polygon.sideOfBoundaryForPoint(i, p) != SIDE_RIGHT)
        {
            return false;
        }
    }
    return true;
}
template <class tPolygon> bool
PolygonCrossesEdge(const tLine& l, const tPolygon& polygon)
{
    tSigned32 i;
    for(i = 0; i != polygon.edges(); ++i)
    {
        if(polygon.sideOfLineForVertex(l, i) == SIDE_LEFT)
        {
            tSigned32 prevI = i - 1;
            if(prevI == -1)
            {
                prevI = polygon.edges() - 1;
            }
            if(polygon.sideOfLineForVertex(l, prevI) != SIDE_LEFT)
            {
                if(polygon.sideOfBoundaryForPoint(prevI, l.start()) != SIDE_RIGHT
                 && polygon.sideOfBoundaryForPoint(prevI, l.end()) == SIDE_RIGHT
                 )
                {
                    return true;
                }
            }
        }
    }
    return false;
}

template <class tPolygon> void
AddAdjoiningFaces(
        tFace f,
        const tPolygon& polygon,
        tFace* openBegin, tFace*& openEnd,
        cFlagVector& closed,
        bool& exitsMesh
        )
{
    tEdge e = tMesh::beginEdge(f);
    bool vertexInside = VertexIsInPolygon(polygon, GetVertexPoint(e));
    do
    {
        bool prevVertexInside = vertexInside;
        vertexInside = VertexIsInPolygon(polygon, GetVertexPoint(e.next()));

        tFace f2 = e.twin().face();

        if(prevVertexInside && vertexInside)
        {
            if(f2->isExternal())
            {
                exitsMesh = true;
                continue;
            }
            if(!closed[f2.index()])
            {
                closed.set(f2.index());
                *openEnd++ = f2;
            }
            continue;
        }

        // in the following cases we don't care if the polygon crosses out of this face
        //.. optimise - look at avoiding point in polygon tests for vertices when these conditions apply to edges in sequence
        if(exitsMesh && f2->isExternal())
        {
            continue;
        }
        if(!f2->isExternal() && closed[f2.index()])
        {
            continue;
        }

        if(!prevVertexInside && !vertexInside)
        {
          // in this case it is necessary to test for whether the polygon crosses out of the middle of the edge
            if(!PolygonCrossesEdge(GetEdgeLine(e), polygon))
            {
                continue;
            }
        }

      // the polygon crosses over this edge

        if(f2->isExternal())
        {
            exitsMesh = true;
            continue;
        }

        if(!closed[f2.index()])
        {
            closed.set(f2.index());
            *openEnd++ = f2;
        }
    }
    while(e.toNextInFace());
}

template <class tPolygon> void
FacesOverlappedByOverlappingPolygon(
        tFace centreF,
        const tPolygon& polygon,
        tFace* openQueueBuffer,
        cFlagVector& resultBuffer,
        bool& exitsMesh
        )
{
    exitsMesh = false;
    resultBuffer.clear();
    resultBuffer.set(centreF.index());
    tFace* openBegin = openQueueBuffer;
    tFace* openEnd = openQueueBuffer;
    AddAdjoiningFaces(centreF, polygon, openBegin, openEnd, resultBuffer, exitsMesh);
    while(openBegin != openEnd)
    {
        tFace f = *openBegin++;
        AddAdjoiningFaces(f, polygon, openBegin, openEnd, resultBuffer, exitsMesh);
    }
}


class cSIModelConvexPoly
{
    const cConvexPoly& _polygon;

public:

    cSIModelConvexPoly(const cConvexPoly& polygon) :
     _polygon(polygon)
    {
    }
    tSigned32 edges() const
    {
        return _polygon.sides();
    }
    eSide sideOfBoundaryForPoint(tSigned32 edge, const tPoint& p) const
    {
        tPoint axis, offset;
        axis = _polygon.vertex_WrappedAround(edge + 1) - _polygon.vertex(edge);
        offset = p - _polygon.vertex(edge);
        eSide result = axis.side(offset);
        if(result == SIDE_CENTRE)
        {
            result = SideOfAxis_SI(axis);
        }
        return result;
    }
    eSide sideOfLineForVertex(const tLine& l, tSigned32 vertex) const
    {
        eSide result = l.side(_polygon.vertex(vertex));
        if(result == SIDE_CENTRE)
        {
            return OtherSide(SideOfLine_SI(l));
        }
        return result;
    }
};
void
FacesOverlappedByOverlappingPolygon_SI(
        tFace centreF,
        const cConvexPoly& polygon,
        tFace* openQueueBuffer,
        cFlagVector& resultBuffer,
        bool& exitsMesh
        )
{
    cSIModelConvexPoly modelPolygon(polygon);
    FacesOverlappedByOverlappingPolygon(centreF, modelPolygon, openQueueBuffer, resultBuffer, exitsMesh);
}

class cOFModelConvexPoly
{
    const cConvexPoly& _polygon;

public:

    cOFModelConvexPoly(const cConvexPoly& polygon) :
     _polygon(polygon)
    {
    }
    tSigned32 edges() const
    {
        return _polygon.sides();
    }
    eSide sideOfBoundaryForPoint(tSigned32 edge, const tPoint& p) const
    {
        tLine l = _polygon.getBoundaryLine(edge);
        return l.side(p);
    }
    eSide sideOfLineForVertex(const tLine& l, tSigned32 vertex) const
    {
        return l.side(_polygon.vertex(vertex));
    }
};
void
FacesOverlappedByOverlappingPolygon_OF(
        tFace centreF,
        const cConvexPoly& polygon,
        tFace* openQueueBuffer,
        cFlagVector& resultBuffer,
        bool& exitsMesh
        )
{
    cOFModelConvexPoly modelPolygon(polygon);
    FacesOverlappedByOverlappingPolygon(centreF, modelPolygon, openQueueBuffer, resultBuffer, exitsMesh);
}

class cOFModelIntersectionAndOffsetPoly
{
    const tIntersection& _centre;
    const cConvexPoly& _offsetPoly;

public:

    cOFModelIntersectionAndOffsetPoly(const tIntersection& centre, const cConvexPoly& offsetPoly) :
     _centre(centre),
     _offsetPoly(offsetPoly)
    {
    }
    tSigned32 edges() const
    {
        return _offsetPoly.sides();
    }
    eSide sideOfBoundaryForPoint(tSigned32 edge, const tPoint& p) const
    {
        tLine l(p - _offsetPoly.vertex(edge), p - _offsetPoly.vertex_WrappedAround(edge + 1));
        return l.side(_centre);
    }
    eSide sideOfLineForVertex(const tLine& l, tSigned32 vertex) const
    {
        return l.side(_centre + _offsetPoly.vertex(vertex));
    }
};
void
FacesOverlappedByOverlappingPolygon_OF(
        tFace centreF, const tIntersection& centreP,
        const cConvexPoly& offsetPoly,
        tFace* openQueueBuffer,
        cFlagVector& resultBuffer,
        bool& exitsMesh
        )
{
    cOFModelIntersectionAndOffsetPoly polygon(centreP, offsetPoly);
    FacesOverlappedByOverlappingPolygon(centreF, polygon, openQueueBuffer, resultBuffer, exitsMesh);
}

bool
ShapeCollidesWithMeshEdge(
        cQueryContext& qc,
        tFace centreF,
        const cConvexPoly& polygon
        )
{
    tFace* openQueueBuffer = qc.lockFaceBuffer();
    cFlagVector& resultBuffer = qc.lockFaceFlagVector();
    bool exitsMesh;
    FacesOverlappedByOverlappingPolygon_OF(centreF, polygon, openQueueBuffer, resultBuffer, exitsMesh);
    qc.releaseFlagVector();
    qc.releaseGeneralPurposeBuffer();
    return exitsMesh;
}

static bool
FaceIsCompletelyContained(const cConvexPoly& polygon, tFace f)
{
    tEdge e = tMesh::beginEdge(f);
    do
    {
        if(!polygon.pointCollides2D(GetVertexPoint(e)))
        {
            return false;
        }
    }
    while(++e != tMesh::beginEdge(f));
    return true;
}

void
CircuitsForInfiniteCylinder(
        tMesh& mesh,
        const cConvexPoly& polygon,
        std::list<tFace>& containedExternalFaces,
        tMeshCircuits& appendTo
        )
{
// get circuit parts and completely contained external faces

    std::list<cCircuitElement*> circuits;
    std::list<cCircuitElement*> circuitEnds;

    std::vector<cEntryOrExitInfo> info(polygon.sides() * 2);

    tFace f = mesh.beginFace();
    do
    {
        if(f->isExternal())
        {
            if(FaceIsCompletelyContained(polygon, f))
            {
                containedExternalFaces.push_back(f);
            }
            continue;
        }
        if(PolygonIsEntirelyWithinStartFace(f, polygon))
        {
            cCircuitElement* e;
            e = CircuitForPolygonEntirelyWithinStartFace(f, polygon);
            appendTo.push_back(new tMeshCutSequence);
            appendTo.back()->push_back(new cMeshCut(e));
            continue;
        }
        tSigned32 numberOfEntries = GetEntriesAndExits(f, polygon, info);
        AddCircuitPartsInFace(info, numberOfEntries, polygon, circuits, circuitEnds);
    }
    while((++f).valid());

    CircuitsFromCircuitParts(circuits, circuitEnds, appendTo);
}

//void
//GetSquareBounds(
//        const cInternalPosition& centre,
//        tSigned32 radius,
//        std::list<tFace>& containedExternalFaces,
//        tMeshCircuits& appendTo
//        )
//{
//    tPoint* pointArray = new tPoint[4];
//    pointArray[0] = centre.p + tPoint(-radius, radius, SHAPE_RANGE);
//    pointArray[1] = centre.p + tPoint(radius, radius, SHAPE_RANGE);
//    pointArray[2] = centre.p + tPoint(radius, -radius, SHAPE_RANGE);
//    pointArray[3] = centre.p + tPoint(-radius, -radius, SHAPE_RANGE);
//    cConvexPoly polygon(4, pointArray);
//    CircuitsForOverlappingPolygon(centre.f, polygon, containedExternalFaces, appendTo);
//}
