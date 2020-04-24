//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "BSP3DOperand.h"
#include "Plane.h"
#include "GetOriginAndNormal.h"
#include "SideOfPlane.h"
#include "tMesh_Simple3D.h"
//#include "SideOfProjectedTo2D.h" // for validation
#include "StoreSideOfPlane.h"
#include "ValidateFaceDynamic.h"
#include "libs/Instrumentation/interface/Profiler.h"
//#include "common/TemplateMesh/Helper.h"

class cVertexIsBehindPredicate
{
    const cBitVector* _sidesBuffer;
    tSigned32 _sidesStart;
    tSigned32 _vertsBeforeSplit;
    eSideOfPlane sideForVert(tVertex_Dynamic v) const
    {
        if(v.index() < _vertsBeforeSplit)
        {
            return RetrieveSideOfPlane(*_sidesBuffer, _sidesStart, v.index());
        }
        return ON_PLANE;
    }
public:
    cVertexIsBehindPredicate(const cBitVector* sidesBuffer, tSigned32 sidesStart, tSigned32 vertsBeforeSplit) :
      _sidesBuffer(sidesBuffer),
      _sidesStart(sidesStart),
      _vertsBeforeSplit(vertsBeforeSplit)
    {
    }
    bool operator()(tVertex_Dynamic v) const
    {
        eSideOfPlane side = sideForVert(v);
        if(side == ON_PLANE)
        {
            tEdge_Dynamic e = tMesh_Dynamic::firstEdge(v);
            while(1)
            {
                side = sideForVert(e.next().vertex());
                if(side != ON_PLANE)
                {
                    break;
                }
                e.toNextAroundVertex();
                assertD(e.valid());
            }
        }
        return side == BEHIND;
    }
};

// this will be called after the operand has been split
static eSideOfPlane
SideForEdgeStart(
        tEdge_Dynamic e, 
        const cBitVector& sidesBuffer, tSigned32 sidesStart, tSigned32 verticesBeforeSplit
        )
{
    tSigned32 i = e.vertex().index();
    if(i >= verticesBeforeSplit)
    {
        return ON_PLANE;
    }
    return RetrieveSideOfPlane(sidesBuffer, sidesStart, i);
}

static bool
SplitFace(
        cBSPVertices& vertices,
        tMesh_Dynamic& mesh,
        tFace_Dynamic toSplit,
        const cPlane& splitter,
        const cBitVector& sidesBuffer, tSigned32 sidesStart, tSigned32 verticesBeforeSplit,
        tEdge_Dynamic& edgeBehindSplit, tEdge_Dynamic& edgeInFrontOfSplit,
        tSigned32& nextEdgePair
        )
{
    tEdge_Dynamic e, e1, e2;
    bool e1AtVertex, e2AtVertex;
    e1.clear();
    e2.clear();
    e = tMesh_Dynamic::beginEdge(toSplit);
    do
    {
        eSideOfPlane sideForStart = SideForEdgeStart(e, sidesBuffer, sidesStart, verticesBeforeSplit);
        eSideOfPlane sideForEnd = SideForEdgeStart(e.next(), sidesBuffer, sidesStart, verticesBeforeSplit);
        if(sideForStart != IN_FRONT && sideForEnd == IN_FRONT)
        {
            assertD(!e1.valid());
            e1 = e;
            e1AtVertex = (sideForStart == ON_PLANE);
        }
        if(sideForStart != BEHIND && sideForEnd == BEHIND)
        {
            assertD(!e2.valid());
            e2 = e;
            e2AtVertex = (sideForStart == ON_PLANE);
        }
    }
    while(e.toNextInFace());

    if(!e1.valid() && !e2.valid())
    {
        // this face doesn't touch, or cross, the split plane
        return false;
    }

    if(e1.valid() && !e2.valid())
    {
        // face is in front of split plane, and touches at a vertex or at an edge
        assertD(e1AtVertex == true);
        assertD(SideForEdgeStart(e1.prev(), sidesBuffer, sidesStart, verticesBeforeSplit) != BEHIND);
        if(SideForEdgeStart(e1.prev(), sidesBuffer, sidesStart, verticesBeforeSplit) == ON_PLANE)
        {
            edgeInFrontOfSplit = e1.prev();
            if(edgeInFrontOfSplit->_pairIndex == -1 && edgeInFrontOfSplit.twin()->_pairIndex == -1) //... optimise - this will assign pair indices when splitting faces between internal edges
            {
                edgeInFrontOfSplit->_pairIndex = nextEdgePair++;
                edgeInFrontOfSplit.twin()->_pairIndex = nextEdgePair++;
            }
            mesh.breakEdge(edgeInFrontOfSplit);
        }
        return false;
    }
    if(!e1.valid() && e2.valid())
    {
        // face is behind split plane, and touches at a vertex or at an edge
        assertD(e2AtVertex == true);
        assertD(SideForEdgeStart(e2.prev(), sidesBuffer, sidesStart, verticesBeforeSplit) != IN_FRONT);
        if(SideForEdgeStart(e2.prev(), sidesBuffer, sidesStart, verticesBeforeSplit) == ON_PLANE)
        {
            edgeBehindSplit = e2.prev();
            // note that edgeBehindSplit will be broken by the conditional above, when split face is called for the face the other side of the split
        }
        return false;
    }

    if(!e1AtVertex)
    {
        mesh.splitEdge(e1);
        e1.next()->_pairIndex = e1->_pairIndex;
        if(e1.twin().valid())
        {
            e1.twin()->_pairIndex = e1.twin().prev()->_pairIndex;
        }
        ++e1;
        cDynamicPoint3 axis(e1.next().vertex()->dynamicPoint(vertices));
        axis -= e1.prev().vertex()->dynamicPoint(vertices);
        cDynamicPoint3 splitPoint;
        Intersection(
                splitter,
                e1.prev().vertex()->dynamicPoint(vertices), axis,
                splitPoint
                );
        e1.vertex()->initForSplitPoint(vertices, splitPoint);
    }

    if(!e2AtVertex)
    {
        mesh.splitEdge(e2);
        e2.next()->_pairIndex = e2->_pairIndex;
        if(e2.twin().valid())
        {
            e2.twin()->_pairIndex = e2.twin().prev()->_pairIndex;
        }
        ++e2;
        cDynamicPoint3 axis(e2.next().vertex()->dynamicPoint(vertices));
        axis -= e2.prev().vertex()->dynamicPoint(vertices);
        cDynamicPoint3 splitPoint;
        Intersection(
                splitter,
                e2.prev().vertex()->dynamicPoint(vertices), axis,
                splitPoint
                );
        e2.vertex()->initForSplitPoint(vertices, splitPoint);
    }

    mesh.splitFace(e1, e2);
    assertD(e1.prev().twin() == e2.prev()); // e1 and e2 are after each of the verts for the split

    mesh.breakEdge(e1.prev());

    e1.prev()->_pairIndex = nextEdgePair++;
    e2.prev()->_pairIndex = nextEdgePair++;

    edgeInFrontOfSplit = e1.prev();
    edgeBehindSplit = e2.prev();
    return true;
}

static void
AddInsideFace(tMesh_Dynamic& mesh, tEdge_Dynamic edgeOnBoundary, tSigned32 splitNode, bool facesOppositeDirection)
{
    std::vector<tMesh_Dynamic::tVertexData> vertData;
    tEdge_Dynamic e = edgeOnBoundary;
    do
    {
        vertData.push_back(*e.vertex());
        assertD(!e.vertex().isInternalVertex());
        assertD(e == tMesh_Dynamic::firstEdge(e.vertex()));
        e = tMesh_Dynamic::lastEdge(e.vertex()).prev();
    }
    while(e != edgeOnBoundary);
    std::vector<tMesh_Dynamic::tEdgeData> edgeData(vertData.size());
    tFace_Dynamic fAdded = mesh.addFace(
            tMesh_Dynamic::tFaceData(),
            SizeL(vertData),
            &edgeData[0], &vertData[0]
            );
    fAdded->initAsInternalFace(splitNode, facesOppositeDirection);
    tEdge_Dynamic eAdded = tMesh_Dynamic::beginEdge(fAdded);
    e = edgeOnBoundary;
    do
    {
        e = tMesh_Dynamic::lastEdge(e.vertex()).prev();
        mesh.connectEdges(e, eAdded);
    }
    while(eAdded.toNextInFace());
}

#ifdef ASSERTIONS_ON
bool
cBSP3DOperand::validate(const cBSPVertices& vertices) const
{
    tSigned32 count = 0;
    tFace_Dynamic f = _mesh.beginFace();
    do
    {
        if(f->isUnassigned())
        {
            count++;
        }
        if(!ValidateFaceDynamic(vertices, f))
        {
            return false;
        }
    }
    while((++f).valid());
    return count == _numberOfUnassignedBounds;
}
#endif

// this is only called when operand has not been split
void
cBSP3DOperand::markCoplanarBound(
        const cBitVector& sidesBuffer, tSigned32 sidesStart,
        tSigned32 splitNode, bool facesOppositeDirection
        )
{
    tFace_Dynamic f = _mesh.beginFace();
    do
    {
        tEdge_Dynamic e = tMesh_Dynamic::beginEdge(f);
        if(RetrieveSideOfPlane(sidesBuffer, sidesStart, e.vertex().index()) != ON_PLANE)
        {
            continue;
        }
        ++e;
        if(RetrieveSideOfPlane(sidesBuffer, sidesStart, e.vertex().index()) != ON_PLANE)
        {
            continue;
        }
        ++e;
        if(RetrieveSideOfPlane(sidesBuffer, sidesStart, e.vertex().index()) != ON_PLANE)
        {
            continue;
        }
        // this is our coplanar bound
        if(f->isUnassigned())
        {
            assertD(_numberOfUnassignedBounds > 0);
            _numberOfUnassignedBounds--;
            f->assignSplitNode(splitNode, facesOppositeDirection);
            return;
        }
    }
    while((++f).valid());
    invalid();
}

cBSP3DOperand::cBSP3DOperand(tMesh_Dynamic& swapIn)
{
    _mesh.swap(swapIn);
    assertD(_mesh.vertices() == _mesh.vertSize());
    _numberOfUnassignedBounds = 0;
    tFace_Dynamic f = _mesh.beginFace();
    do
    {
        if(f->isUnassigned())
        {
            _numberOfUnassignedBounds++;
        }
    }
    while((++f).valid());
}

cBSP3DOperand::cBSP3DOperand(
        cBSPVertices& vertices,
        tMesh_Simple3D& meshSimple3D, tSigned32 operandIndex, std::vector<cPlane>& splitters
        )
{
    _mesh.copyStructure(meshSimple3D);
    assertD(_mesh.vertices() == _mesh.vertSize());
    tFace_Simple3D fSimple3D = meshSimple3D.beginFace();
    tFace_Dynamic f = _mesh.beginFace();
    do
    {
        f->initAsUnassigned(SizeL(splitters), operandIndex);
//        f->initAsUnassigned(SizeL(splitters), SizeL(splitters));
        splitters.push_back(cPlane());
        GetOriginAndNormal(fSimple3D, splitters.back());
        ++fSimple3D;
    }
    while((++f).valid());
    tVertex_Simple3D vSimple3D = meshSimple3D.beginVertex();
    tVertex_Dynamic v = _mesh.beginVertex();
    do
    {
        v->initForOriginalPoint(vertices, vSimple3D->_p);
        ++vSimple3D;
    }
    while((++v).valid());
    tEdge_Dynamic e = _mesh.beginEdge();
    do
    {
        e->_pairIndex = -1;
    }
    while(e.toNextInMesh());
    _numberOfUnassignedBounds = _mesh.faces();
}

bool
cBSP3DOperand::predict_WillBeSplit(
        const cBitVector& preprocessBuffer,
        tSigned32 bufferPosition
        )
{
    bool crossesInFront = false;
    bool crossesBehind = false;
    for(tSigned32 i = 0; i != _mesh.vertices(); ++i)
    {
        eSideOfPlane side = RetrieveSideOfPlane(preprocessBuffer, bufferPosition, i);
        if(side == IN_FRONT)
        {
            crossesInFront = true;
            if(crossesBehind)
            {
                return true;
            }
        }
        if(side == BEHIND)
        {
            crossesBehind = true;
            if(crossesInFront)
            {
                return true;;
            }
        }
    }
    return false;
}

void
cBSP3DOperand::predict_UnassignedBoundsSplit(
        const cBitVector& preprocessBuffer,
        tSigned32 bufferPosition,
        tSigned32& unassignedInFront, tSigned32& unassignedBehind,
        bool& containsFront, bool& containsBack
        )
{
    unassignedInFront = 0;
    unassignedBehind = 0;
    tSigned32 result = 0;
    tFace_Dynamic f = _mesh.beginFace();
    do
    {
        if(!f->isUnassigned())
        {
            continue;
        }

        tEdge_Dynamic e = tMesh_Dynamic::beginEdge(f);
        eSideOfPlane sideAtStart = RetrieveSideOfPlane(preprocessBuffer, bufferPosition, e.vertex().index());

        if(sideAtStart == ON_PLANE)
        {
            eSideOfPlane sideAfter = RetrieveSideOfPlane(preprocessBuffer, bufferPosition, e.next().vertex().index());
            --e;
            eSideOfPlane sideBefore = RetrieveSideOfPlane(preprocessBuffer, bufferPosition, e.vertex().index());
            if(sideBefore == ON_PLANE && sideAfter == ON_PLANE)
            {
              // coplanar face
                assertD(result == 0); // operand cannot cross the split plane
                e = e.twin().prev();
                eSideOfPlane sideForOperand = RetrieveSideOfPlane(preprocessBuffer, bufferPosition, e.vertex().index());
                assertD(sideForOperand != ON_PLANE);
                if(sideForOperand == BEHIND)
                {
                    containsFront = false;
                    unassignedInFront = 0;
                    unassignedBehind = _numberOfUnassignedBounds - 1;
                    containsBack = (unassignedBehind == 0);
                }
                else
                {
                    containsBack = false;
                    unassignedBehind = 0;
                    unassignedInFront = _numberOfUnassignedBounds - 1;
                    containsFront = (unassignedInFront == 0);
                }
                return;
            }
            if(sideBefore == ON_PLANE || sideAfter == ON_PLANE || sideBefore == sideAfter)
            {
              // face just touches split plane
                if(sideBefore == IN_FRONT || sideAfter == IN_FRONT)
                {
                    unassignedInFront++;
                }
                else
                {
                    unassignedBehind++;
                }
                continue;
            }
            // face is split
            unassignedInFront++;
            unassignedBehind++;
            continue;
        }

        eSideOfPlane sideAtChange;
        ++e;
        do
        {
            sideAtChange = RetrieveSideOfPlane(preprocessBuffer, bufferPosition, e.vertex().index());
        }
        while(sideAtChange == sideAtStart && e.toNextInFace());
        if(sideAtChange == sideAtStart)
        {
            // touches split
            if(sideAtStart == IN_FRONT)
            {
                unassignedInFront++;
            }
            else
            {
                unassignedBehind++;
            }
            continue;
        }
        if(sideAtChange != ON_PLANE)
        {
            // face is split
            unassignedInFront++;
            unassignedBehind++;
            continue;
        }
        eSideOfPlane sideAfter = RetrieveSideOfPlane(preprocessBuffer, bufferPosition, e.next().vertex().index());
        if(sideAfter == ON_PLANE || sideAfter == sideAtStart)
        {
            // touches split
            if(sideAtStart == IN_FRONT)
            {
                unassignedInFront++;
            }
            else
            {
                unassignedBehind++;
            }
            continue;
        }
        // face is split (at vertex)
        unassignedInFront++;
        unassignedBehind++;
    }
    while((++f).valid());

    containsFront = false;
    containsBack = false;
    // check for containment that is not due to a coplanar face
    if(unassignedInFront == 0)
    {
        assertD(unassignedBehind != 0);
        tSigned32 i;
        for(i = 0; i != _mesh.vertices(); ++i)
        {
            if(RetrieveSideOfPlane(preprocessBuffer, bufferPosition, i) == IN_FRONT)
            {
                containsFront = true;
                return;
            }
        }
    }
    else if(unassignedBehind == 0)
    {
        tSigned32 i;
        for(i = 0; i != _mesh.vertices(); ++i)
        {
            if(RetrieveSideOfPlane(preprocessBuffer, bufferPosition, i) == BEHIND)
            {
                containsBack = true;
                return;
            }
        }
    }
}

void
cBSP3DOperand::split(
        cBSPVertices& vertices,
        const cPlane& splitter,
        const cBitVector& preprocessBuffer,
        tSigned32 bufferPosition,
        tSigned32 splitNode, 
        cBSP3DOperand*& front, cBSP3DOperand*& back,
        tSigned32& nextEdgePair
        )
{
    PROFILER_SCOPE("cBSP3DOperand::split()");
    assertF(validate(vertices));

#ifdef ASSERTIONS_ON
    tSigned32 predictedUnassignedInFront;
    tSigned32 predictedUnassignedBehind;
    bool predictedFrontIsContained;
    bool predictedBackIsContained;
    predict_UnassignedBoundsSplit(
            preprocessBuffer, bufferPosition,
            predictedUnassignedInFront,
            predictedUnassignedBehind,
            predictedFrontIsContained,
            predictedBackIsContained
            );
#endif

    bool crossesInFront = false;
    bool crossesBehind = false;
    tSigned32 numberOfOnPlaneVerts = 0;

    tSigned32 i;
    for(i = 0; i != _mesh.vertices(); ++i)
    {
        eSideOfPlane side = RetrieveSideOfPlane(preprocessBuffer, bufferPosition, i);
        if(side == IN_FRONT)
        {
            crossesInFront = true;
        }
        if(side == BEHIND)
        {
            crossesBehind = true;
        }
        if(side == ON_PLANE)
        {
            numberOfOnPlaneVerts++;
        }
    }

    tSigned32 sidesStart = bufferPosition;
    tSigned32 vertsBeforeSplit = _mesh.vertices();

    assertD(crossesInFront || crossesBehind);
    if(crossesInFront && !crossesBehind)
    {
        if(numberOfOnPlaneVerts >= 3)
        {
            markCoplanarBound(preprocessBuffer, sidesStart, splitNode, true);
        }
        front = this;
        back = 0;
        return;
    }
    if(!crossesInFront && crossesBehind)
    {
        if(numberOfOnPlaneVerts >= 3)
        {
            markCoplanarBound(preprocessBuffer, sidesStart, splitNode, false);
        }
        back = this;
        front = 0;
        return;
    }

    tEdge_Dynamic anEdgeBehindSplit, anEdgeInFrontOfSplit;

#ifdef ASSERTIONS_ON
    anEdgeBehindSplit.clear();
    anEdgeInFrontOfSplit.clear();
#endif

    {
        _mesh.consolidateFaces();
        tSigned32 facesBeforeSplit = _mesh.faces();
        tSigned32 i;
        for(i = 0; i != facesBeforeSplit; ++i)
        {
            tFace_Dynamic f = _mesh.faceAt(i);
            bool wasSplit = SplitFace(vertices, _mesh, f, splitter, preprocessBuffer, sidesStart, vertsBeforeSplit, anEdgeBehindSplit, anEdgeInFrontOfSplit, nextEdgePair);
            if(wasSplit && f->isUnassigned())
            {
                _numberOfUnassignedBounds++;
            }
        }
    }

    assertD(anEdgeBehindSplit.valid());
    assertD(anEdgeInFrontOfSplit.valid());

    AddInsideFace(_mesh, anEdgeBehindSplit, splitNode, false);
    AddInsideFace(_mesh, anEdgeInFrontOfSplit, splitNode, true);

    front = this;
    tMesh_Dynamic backMesh;
    cVertexIsBehindPredicate predicate(&preprocessBuffer, sidesStart, vertsBeforeSplit);
    _mesh.split(predicate, backMesh);

    if(_mesh.vertices() != _mesh.vertSize())
    {
        _mesh.consolidateVertices();
    }
    if(backMesh.vertices() != backMesh.vertSize())
    {
        backMesh.consolidateVertices();
    }

    back = new cBSP3DOperand(backMesh);
    _numberOfUnassignedBounds -= back->_numberOfUnassignedBounds;

    assertF(validate(vertices));
    assertF(back->validate(vertices));

    assertD(predictedUnassignedInFront == _numberOfUnassignedBounds);
    assertD(predictedUnassignedBehind == back->_numberOfUnassignedBounds);
}

void
cBSP3DOperand::takeMeshAndInvalidate(tMesh_Dynamic& swapOutTo)
{
    assertD(swapOutTo.empty());
    assertD(!swapOutTo.hasDeadEntries());
    swapOutTo.swap(_mesh);
}

tSigned32
cBSP3DOperand::splitForUnassignedBoundIndex(tSigned32 index) const
{
    tFace_Dynamic f = _mesh.beginFace();
    do
    {
        if(!f->isUnassigned())
        {
            continue;
        }
        if(index == 0)
        {
            return f->_splitterIndex;
        }
        --index;
    }
    while((++f).valid());
    invalid();
    return 0;
}

