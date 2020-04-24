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
#include "Approximate.h"
#include "tMesh_Dynamic.h"
#include "BSPVertices.h"
#include "ProcessVerticalFaces.h"
#include "DynamicPoint2.h"
#include "Plane.h" // required just for identifying vertical faces
#include "ValidateFaceDynamic.h"
#include "platform_common/CoordinateRanges.h"
#include "common/TemplateMesh/Helper.h"
//#include "common/TemplateMesh/ValidateMeshConnections.h"

typedef tMesh_Dynamic tMesh;
typedef tFace_Dynamic tFace;
typedef tEdge_Dynamic tEdge;
typedef tVertex_Dynamic tVertex;

enum eComponent
{
    COMPONENT_X,
    COMPONENT_Y,
};

static const cDynamicFraction&
RefComponent(
        cBSPVertices& vertices,
        tEdge e,
        eComponent component
        )
{
    if(component == COMPONENT_X)
    {
        return e.vertex()->dynamicPoint(vertices).x();
    }
    return e.vertex()->dynamicPoint(vertices).y();
}

static const cDynamicFraction&
RefOtherComponent(
        cBSPVertices& vertices,
        tEdge e,
        eComponent component
        )
{
    if(component == COMPONENT_Y)
    {
        return e.vertex()->dynamicPoint(vertices).x();
    }
    return e.vertex()->dynamicPoint(vertices).y();
}

#ifdef ASSERTIONS_ON
static bool
MeshHasNoSharedEdgePairs(tMesh& mesh)
{
    tSigned32 i;
    for(i = 0; i != mesh.edgeSize(); ++i)
    {
        tEdge e = mesh.edgeAt(i);
        if(e.entryIsDead())
        {
            continue;
        }
        if(e.twin().valid() && e.next().twin().valid() && e.twin().face() == e.next().twin().face())
        {
            return false;
        }
    }
    return true;
}
static bool
MeshHasComponentAlignedDownwardFace(
        cBSPVertices& vertices,
        tMesh& mesh, eComponent component
        )
{
    tSigned32 i;
    for(i = 0; i != mesh.faceSize(); ++i)
    {
        tFace f = mesh.faceAt(i);
        if(f.entryIsDead())
        {
            continue;
        }
        tEdge e = tMesh::beginEdge(f);
        do
        {
            const cDynamicFraction& v0 = RefComponent(vertices, e, component);
            const cDynamicFraction& v1 = RefComponent(vertices, e.next(), component);
            if(v0 != v1)
            {
                continue;
            }
            // component aligned
            const cDynamicFraction& v0Other = RefOtherComponent(vertices, e, component);
            const cDynamicFraction& v1Other = RefOtherComponent(vertices, e.next(), component);
            int otherCompare = v0Other.compare(v1Other);
            if(otherCompare == 0)
            {
                // not downward
                continue;
            }
            int downwardCompare = otherCompare;
            if(component == COMPONENT_X)
            {
                downwardCompare *= -1;
            }
            const cDynamicFraction& v2 = RefComponent(vertices, e.prev(), component);
            if(v0.compare(v2) == downwardCompare)
            {
                return true;
            }
        }
        while(e.toNextInFace());
    }
    return false;
}
#endif

static void
CollapseVerticalFace(
        cBSPVertices& vertices,
        tMesh& mesh,
        tFace& f
        )
{
    tEdge e1, e2;
    GetVerticalFaceConnections(f, e1, e2);
    e1 = e1.twin();
    e2 = e2.twin();
    mesh.eraseFace(f);
    mesh.connectEdges(e1, e2);
}

static bool
VertsMatchHorizontally(
        cBSPVertices& vertices, //.. just for checking
        tVertex v1, tVertex v2
        )
{
    const cDynamicPoint3& p1 = v1->dynamicPoint(vertices);
    const cDynamicPoint3& p2 = v2->dynamicPoint(vertices);
    return p1.x() == p2.x() && p1.y() == p2.y();
}

static void
CollapseOneSideOfZeroLengthEdge(
        cBSPVertices& vertices, //.. just for checking
        tMesh& mesh,
        tEdge e
        )
{
    tEdge connect1 = e.next().twin();
    tEdge connect2 = e.prev().twin();
    mesh.eraseFace(e.face());
//    while(connect1.valid() && connect2.valid() && connect1.prev().twin() == connect2.next())
//    {
//      // faces for connect1 and connect 2 must also be collapsed,
//      // to avoid creating faces with shared edge pairs
//        connect1 = connect1.next().twin();
//        connect2 = connect2.prev().twin();
//        mesh.eraseFace(connect1.twin().face());
//        mesh.eraseFace(connect2.twin().face());
//    }
    while(connect1.valid() && connect2.valid() && connect1.prev().twin().valid() && connect1.prev().twin() == connect2.next())
    {
      // faces for connect1 and connect 2 must also be collapsed,
      // to avoid creating faces with shared edge pairs
        tFace f1 = connect1.face();
        tFace f2 = connect2.face();
        connect1 = connect1.next().twin();
        connect2 = connect2.prev().twin();
        mesh.eraseFace(f1);
        mesh.eraseFace(f2);
    }
    if(connect1.valid() && connect2.valid())
    {
        assertD(VertsMatchHorizontally(vertices, connect1.vertex(), connect2.next().vertex()));
        assertD(VertsMatchHorizontally(vertices, connect1.next().vertex(), connect2.vertex()));
        mesh.connectEdges(connect1, connect2);
    }
}

static void
CollapseZeroLengthEdge(
        cBSPVertices& vertices, //.. just for checking
        tMesh& mesh,
        tEdge e
        )
{
    tEdge eTwin = e.twin();
    if(!eTwin.valid())
    {
        tEdge prevExternal = tMesh::lastEdge(e.vertex()).prev();
        if(prevExternal.vertex() == e.next().vertex())
        {
          // case where a hole in the mesh is reduced to an edge pair
          // this hole will now be collapsed completely
            eTwin = prevExternal;
        }
    }
    if(eTwin.valid())
    {
        CollapseOneSideOfZeroLengthEdge(vertices, mesh, eTwin);
    }
    assertD(!e.entryIsDead());
    if(e.entryIsDead())
    {
        return;
    }
    CollapseOneSideOfZeroLengthEdge(vertices, mesh, e);
}

static void
CollapseZeroLengthEdges(
        cBSPVertices& vertices,
        tMesh& mesh,
        eComponent component //... just for checking
        )
{
    tSigned32 i;
    for(i = 0; i != mesh.edgeSize(); ++i)
    {
        tEdge e = mesh.edgeAt(i);
        if(e.entryIsDead())
        {
            continue;
        }
        if(VertsMatchHorizontally(vertices, e.vertex(), e.next().vertex()))
        {
            CollapseZeroLengthEdge(vertices, mesh, e);
            assertD(e.entryIsDead());
            assertD(!MeshHasComponentAlignedDownwardFace(vertices, mesh, component));//..
        }
    }
}

//static void
//CollapseSharedEdgePairFaces(
//        tMesh& mesh
//        )
//{
//    bool reloop;
//    do
//    {
//        reloop = false;
//        tSigned32 i;
//        for(i = 0; i != mesh.edgeSize(); ++i)
//        {
//            tEdge e = mesh.edgeAt(i);
//            if(e.entryIsDead())
//            {
//                continue;
//            }
//            if(e.twin().valid() && e.next().twin().valid() && e.twin().face() == e.next().twin().face())
//            {
//                tEdge connect = e.prev();
//                tEdge target = e.twin().prev();
//                mesh.eraseFace(e.twin().face());
//                mesh.eraseFace(e.face());
//                if(connect.valid() && target.valid())
//                {
//                    assertD(!connect.entryIsDead());
//                    assertD(!target.entryIsDead());
//                    mesh.connectEdges(connect, target);
//                    reloop = true;
//                }
//            }
//        }
//    }
//    while(reloop);
//    assertD(MeshHasNoSharedEdgePairs(mesh));//...
//}

static void
CheckSplitTypeAndGetLongestEdge(
        cBSPVertices& vertices,
        eComponent splitBy,
        tFace f,
        bool& canBeSplit,
        tEdge& longestE,
        bool& direction_Increasing
        )
{
    tEdge e = tMesh::beginEdge(f);
    const cDynamicFraction& v1 = RefComponent(vertices, e, splitBy);
    ++e;
    const cDynamicFraction& v2 = RefComponent(vertices, e, splitBy);
    int comparison1_2 = v1.compare(v2);
    if(comparison1_2 == 0)
    {
      // face can only be split if there are 3 distinct values of the component
        canBeSplit = false;
        return;
    }
    ++e;
    const cDynamicFraction& v3 = RefComponent(vertices, e, splitBy);
    int comparison2_3 = v2.compare(v3);
    if(comparison2_3 == 0)
    {
      // face can only be split if there are 3 distinct values of the component
        canBeSplit = false;
        return;
    }
    if(comparison2_3 == comparison1_2)
    {
        canBeSplit = true;
        longestE = tMesh::beginEdge(f).prev();
        direction_Increasing = (comparison1_2 == -1);
        return;
    }
    int comparison1_3 = v1.compare(v3);
    if(comparison1_3 == 0)
    {
      // face can only be split if there are 3 distinct values of the component
        canBeSplit = false;
        return;
    }
    if(comparison1_3 == comparison1_2)
    {
        canBeSplit = true;
        longestE = tMesh::beginEdge(f);
        direction_Increasing = (comparison1_2 == 1);
        return;
    }
    canBeSplit = true;
    longestE = tMesh::beginEdge(f).next();
    direction_Increasing = (comparison2_3 == 1);
}

static
void
CollapseIfSharedEdgePair(
        tMesh& mesh,
        tEdge e1
        )
{
    tEdge e2 = e1.next();
    if(!e1.twin().valid() || !e2.twin().valid())
    {
        return;
    }
    if(e1.twin().face() != e2.twin().face())
    {
        return;
    }
    tEdge connect = e1.prev().twin();
    tEdge target = e1.twin().next().twin();
    mesh.eraseFace(e1.twin().face());
    mesh.eraseFace(e1.face());
    if(connect.valid() && target.valid())
    {
        mesh.connectEdges(connect, target);
    }
}

static bool
CollapseBehindFaceIfLongerAndDownward(
        cBSPVertices& vertices,
        tMesh& mesh,
        eComponent splitBy,
        tEdge behindEdge,
        bool direction_Increasing
        );

static bool
CollapseFaceIfDownward_CanBeSplit(
        cBSPVertices& vertices,
        tMesh& mesh,
        eComponent splitBy,
        tEdge longestE,
        bool direction_Increasing
        )
{
  // calculate split ratio

    cDynamicFraction splitRatio(RefComponent(vertices, longestE.prev(), splitBy));
    splitRatio -= RefComponent(vertices, longestE, splitBy);
    {
        cDynamicFraction divisor(RefComponent(vertices, longestE.next(), splitBy));
        divisor -= RefComponent(vertices, longestE, splitBy);
        splitRatio /= divisor;
    }

  // get interpolated value for other component

    cDynamicFraction interpolatedOther(RefOtherComponent(vertices, longestE.next(), splitBy));
    interpolatedOther -= RefOtherComponent(vertices, longestE, splitBy);
    interpolatedOther *= splitRatio;
    interpolatedOther += RefOtherComponent(vertices, longestE, splitBy);

  // determine whether the face is upward facing, zero area, or downward facing

    int comparison = interpolatedOther.compare(RefOtherComponent(vertices, longestE.prev(), splitBy));

    int requiredComparison;
    if(splitBy == COMPONENT_Y)
    {
        if(direction_Increasing)
        {
            requiredComparison = 1;
        }
        else
        {
            requiredComparison = -1;
        }
    }
    else
    {
        if(direction_Increasing)
        {
            requiredComparison = -1;
        }
        else
        {
            requiredComparison = 1;
        }
    }

    if(comparison == requiredComparison)
    {
      // face is upward facing
        return false;
    }

  // face is downward facing, or zero area but with no zero length edges

    if(!longestE.twin().valid())
    {
        mesh.eraseFace(longestE.face());
        return false; // no changes potentially needing to be propagated
    }
    if(!longestE.next().twin().valid() && !longestE.prev().twin().valid())
    {
        mesh.eraseFace(longestE.face());
        return false; // no changes potentially needing to be propagated
    }

    // if face behind longestE is also downward, and is 'longer'
    // then this needs to be collapsed first,
    // to prevent infinite recursive collapses

    bool mayNeedPropagating = CollapseBehindFaceIfLongerAndDownward(vertices, mesh, splitBy, longestE.twin(), direction_Increasing);

    assertD(!longestE.entryIsDead());

    if(!longestE.twin().valid())
    {
      // face behind was erased by CollapseBehindFaceIfLongerAndDownward()
        mesh.eraseFace(longestE.face());
        return mayNeedPropagating;
    }

//... zero length edges are now cleaned up with a second edge collapse pass
//... note that code below does not check for creation of shared edge pairs
//    if(longestE.prev().vertex()->dynamicPoint(vertices).x() == longestE.twin().prev().vertex()->dynamicPoint(vertices).x()
//     && longestE.prev().vertex()->dynamicPoint(vertices).y() == longestE.twin().prev().vertex()->dynamicPoint(vertices).y())
//    {
//      // case where single collapse would result in a zero length edge
//      // collapse both faces directly, in this case
//
//        assertD(longestE.next().twin().face() != longestE.twin().face());
//        assertD(longestE.prev().twin().face() != longestE.twin().face());
//        tEdge connect1 = longestE.next().twin();
//        tEdge target1 = longestE.twin().prev().twin();
//        tEdge connect2 = longestE.prev().twin();
//        tEdge target2 = longestE.twin().next().twin();
//        mesh.eraseFace(longestE.twin().face());
//        mesh.eraseFace(longestE.face());
//        if(connect1.valid() && target1.valid())
//        {
//            assertD(!connect1.entryIsDead());
//            assertD(!target1.entryIsDead());
//            mesh.connectEdges(connect1, target1);
//        }
//        if(connect2.valid() && target2.valid())
//        {
//            assertD(!connect2.entryIsDead());
//            assertD(!target2.entryIsDead());
//            mesh.connectEdges(connect2, target2);
//        }
//        return mayNeedPropagating;
//    }

    // effectively collapse this face, by flipping the longest edge
    // but interpolate Z on the other side of longestE, to avoid disturbing geometry on other side of collapse

    tSigned32 interpolatedZ = longestE.twin()->_approximateZ;
    interpolatedZ -= longestE.twin().next()->_approximateZ;
    interpolatedZ = (cDynamicFraction(interpolatedZ) * splitRatio).asLong();
    interpolatedZ += longestE.twin().next()->_approximateZ;
    
    mesh.flipEdge(longestE);
    longestE.twin().prev()->_approximateZ = longestE.twin()->_approximateZ;
    longestE->_approximateZ = longestE.twin().next()->_approximateZ;
    longestE.twin()->_approximateZ = interpolatedZ;
    longestE.next()->_approximateZ = interpolatedZ;

    CollapseIfSharedEdgePair(mesh, longestE.next());
    if(!longestE.entryIsDead())
    {
        CollapseIfSharedEdgePair(mesh, longestE.twin().next());
    }

    // faces either side of the flipped edge may need to be reprocessed
    return true;
}

static bool
CollapseBehindFaceIfLongerAndDownward(
        cBSPVertices& vertices,
        tMesh& mesh,
        eComponent splitBy,
        tEdge behindE,
        bool direction_Increasing
        )
{
    assertD(CountEdgesAround(behindE) == 3);
    const cDynamicFraction& vStart = RefComponent(vertices, behindE, splitBy);
    const cDynamicFraction& vEnd = RefComponent(vertices, behindE.next(), splitBy);
    const cDynamicFraction& vOpposite = RefComponent(vertices, behindE.prev(), splitBy);
    int comparisonStart_End = direction_Increasing ? -1 : 1;
    assertD(comparisonStart_End == vStart.compare(vEnd));
    int comparisonStart_Opposite = vStart.compare(vOpposite);
    int comparisonEnd_Opposite;
    if(comparisonStart_Opposite == 0)
    {
        // doesnt extend past either end
        return false;
    }
    tEdge longestE;
    if(comparisonStart_Opposite == comparisonStart_End)
    {
      // doesn't extend past start, but could extend past end
        comparisonEnd_Opposite = vEnd.compare(vOpposite);
        if(comparisonEnd_Opposite != comparisonStart_End)
        {
            // does not extend past start or end
            return false;
        }
        longestE = behindE.prev();
    }
    else
    {
        longestE = behindE.next();
    }
#ifdef ASSERTIONS_ON
    {
        cDynamicFraction lengthBefore, lengthThis;
        lengthBefore = vStart - vEnd;
        if(comparisonStart_Opposite == comparisonStart_End)
        {
            lengthThis = vStart - vOpposite;
        }
        else
        {
            lengthThis = vOpposite - vEnd;
        }
        if(!direction_Increasing)
        {
            lengthBefore.negate();
            lengthThis.negate();
        }
        assertD(!lengthBefore.isZero() && !lengthBefore.isNegative());
        assertD(!lengthThis.isZero() && !lengthThis.isNegative());
        assertD(lengthThis > lengthBefore);
    }
#endif
    return CollapseFaceIfDownward_CanBeSplit(vertices, mesh, splitBy, longestE, direction_Increasing);
}

static bool
CollapseFaceIfDownward(
        cBSPVertices& vertices,
        tMesh& mesh,
        eComponent splitBy,
        tFace f
        )
{
    assertD(CountEdgesInFace(mesh, f) == 3);
    bool canBeSplit;
    tEdge longestE;
    bool direction_Increasing;
    CheckSplitTypeAndGetLongestEdge(vertices, splitBy, f, canBeSplit, longestE, direction_Increasing);
    if(!canBeSplit)
    {
      // faces without 3 distinct values of the component cannot be downward facing
        return false;
    }
    return CollapseFaceIfDownward_CanBeSplit(vertices, mesh, splitBy, longestE, direction_Increasing);
}

static void
CollapseDownwardFaces(
        cBSPVertices& vertices,
        tMesh& mesh,
        eComponent splitBy
        )
{
    bool reloop;
    do
    {
        reloop = false;
        mesh.consolidateFaces();
        tSigned32 i;
        for(i = 0; i != mesh.faceSize(); ++i)
        {
            tFace f = mesh.faceAt(i);
            if(f.entryIsDead())
            {
                continue;
            }
            if(CollapseFaceIfDownward(vertices, mesh, splitBy, f))
            {
                reloop = true;
            }
        }
    }
    while(reloop);
}

void
ApproximateZsAndCollapseVerticalFaces(
        cBSPVertices& vertices,
        tMesh_Dynamic& mesh,
        const std::vector<cPlane>& splitters
        )
{
    vertices.approximateZ();

  // fill in approximate Z in edges

    {
        tEdge e = mesh.beginEdge();
        do
        {
            e->_approximateZ = e.vertex()->dynamicPoint(vertices).z().asLong();
        }
        while(e.toNextInMesh());
    }

  // collapse vertical faces

    {
        mesh.consolidateFaces();
        const tSigned32 startingFaces = mesh.faces();
        tSigned32 i;
        for(i = 0; i != startingFaces; ++i)
        {
            tFace f = mesh.faceAt(i);
            if(!IsVertical(f, splitters) || f->wasDiscarded())
            {
                continue;
            }
            CollapseVerticalFace(vertices, mesh, f);
        }
    }
}

void
ApproximateHorizontalComponents(
        cBSPVertices& vertices,
        tMesh_Dynamic& mesh
        )
{
    assertD(!MeshHasComponentAlignedDownwardFace(vertices, mesh, COMPONENT_X));//..
    assertD(!MeshHasComponentAlignedDownwardFace(vertices, mesh, COMPONENT_Y));//..
#ifdef ASSERTIONS_ON
    {
        tFace f = mesh.beginFace();
        while(f.valid())
        {
            assertD(ValidateFaceDynamic_Ground(vertices, f));
            assertD(CountEdgesInFace(mesh, f) == 3);
            ++f;
        }
    }
#endif

  // approximate X and collapse

    vertices.approximateX();
    CollapseZeroLengthEdges(vertices, mesh, COMPONENT_Y);
    assertD(!MeshHasComponentAlignedDownwardFace(vertices, mesh, COMPONENT_Y));//..
    assertD(MeshHasNoSharedEdgePairs(mesh));//...
    CollapseDownwardFaces(vertices, mesh, COMPONENT_Y);
    assertD(!MeshHasComponentAlignedDownwardFace(vertices, mesh, COMPONENT_Y));//..
    assertD(MeshHasNoSharedEdgePairs(mesh));//...
    CollapseZeroLengthEdges(vertices, mesh, COMPONENT_Y);
    assertD(MeshHasNoSharedEdgePairs(mesh));//...

#ifdef ASSERTIONS_ON
    {
        tFace f = mesh.beginFace();
        while(f.valid())
        {
            assertD(ValidateFaceDynamic_Ground(vertices, f));
            assertD(CountEdgesInFace(mesh, f) == 3);
            ++f;
        }
    }
#endif

  // approximate Y and collapse

    assertD(!MeshHasComponentAlignedDownwardFace(vertices, mesh, COMPONENT_X));//..
    vertices.approximateY();
    CollapseZeroLengthEdges(vertices, mesh, COMPONENT_X);
    assertD(!MeshHasComponentAlignedDownwardFace(vertices, mesh, COMPONENT_X));//..
    assertD(MeshHasNoSharedEdgePairs(mesh));//...
    CollapseDownwardFaces(vertices, mesh, COMPONENT_X);
    assertD(!MeshHasComponentAlignedDownwardFace(vertices, mesh, COMPONENT_X));//..
    assertD(MeshHasNoSharedEdgePairs(mesh));//...
    CollapseZeroLengthEdges(vertices, mesh, COMPONENT_X);
    assertD(MeshHasNoSharedEdgePairs(mesh));//...

#ifdef ASSERTIONS_ON
    {
        tFace f = mesh.beginFace();
        while(f.valid())
        {
            assertD(ValidateFaceDynamic_Ground(vertices, f));
            assertD(CountEdgesInFace(mesh, f) == 3);
            ++f;
        }
    }
#endif
}

//void
//SplitVerticalDiscontinuity(
//        tMesh& mesh,
//        tEdge e
//        )
//{
//  // follow discontinuity through vertex
//
//    tEdge eBefore = e.prev();
//    while(eBefore.twin().valid() && eBefore.twin()->_approximateZ == e->_approximateZ)
//    {
//        eBefore = eBefore.twin().next();
//        assertD(eBefore != e.twin());
//    }
//    tEdge eTwin = e.twin();
//    tEdge eBeforeTwin = eBefore.twin();
//    bool atEdge = !eBeforeTwin.valid();
//
//  // break edges
//
//    mesh.breakEdge(e);
//    if(!atEdge)
//    {
//        mesh.breakEdge(eBefore);
//    }
//
//  // create faces
//
//    tFace f, fBefore;
//
//    tMesh::tEdgeData edges[3];
//    tMesh::tVertexData verts[3];
//
//    edges[0] = *e.next();
//    edges[1] = *e;
//    edges[2] = *eTwin.next();
//    verts[0] = *e.next().vertex();
//    verts[1] = *e.vertex();
//    verts[2] = *e.vertex();
//    f = mesh.addFace(tMesh::tFaceData(), 3, edges, verts);
//    f->initAsUnassigned(-1, -1);
//
//    if(!atEdge)
//    {
//        edges[0] = *e;
//        edges[1] = *eBefore;
//        edges[2] = *eBeforeTwin;
//        verts[0] = *e.vertex();
//        verts[1] = *eBefore.vertex();
//        verts[2] = *e.vertex();
//        fBefore = mesh.addFace(tMesh::tFaceData(), 3, edges, verts);
//        fBefore->initAsUnassigned(-1, -1);
//    }
//
//  // connect
//    
//    mesh.connectEdges(e, tMesh::beginEdge(f));
//    mesh.connectEdges(eTwin, tMesh::beginEdge(f).prev());
//    if(!atEdge)
//    {
//        mesh.connectEdges(eBefore, tMesh::beginEdge(fBefore));
//        mesh.connectEdges(eBeforeTwin, tMesh::beginEdge(fBefore).next());
//        mesh.connectEdges(tMesh::beginEdge(f).next(), tMesh::beginEdge(fBefore).prev());
//    }
//}
//
//void
//SplitVerticalDiscontinuities(
//        tMesh_Dynamic& mesh
//        )
//{
//    mesh.consolidateFaces();
//    tSigned32 facesAtStart = mesh.faces();
//    tSigned32 i;
//    for(i = 0; i != facesAtStart; ++i)
//    {
//        tFace f = mesh.faceAt(i);
//        tEdge e = tMesh::beginEdge(f);
//        do
//        {
//            if(e.twin().valid() && e->_approximateZ != e.twin().next()->_approximateZ)
//            {
//                SplitVerticalDiscontinuity(mesh, e);
//            }
//        }
//        while(e.toNextInFace());
//    }
//#ifdef ASSERTIONS_ON
//    {
//        tVertex v = mesh.beginVertex();
//        do
//        {
//            tEdge e = tMesh::firstEdge(v);
//            tSigned32 z = e->_approximateZ;
//            while(e.toNextAroundVertex())
//            {
//                assertD(e->_approximateZ == z);
//            }
//        }
//        while((++v).valid());
//    }
//#endif
//}
