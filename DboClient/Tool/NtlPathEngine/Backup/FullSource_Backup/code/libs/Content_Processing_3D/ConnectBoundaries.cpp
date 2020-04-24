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
#include "ConnectBoundaries.h"
#include "tMesh_Dynamic.h"
#include "Plane.h"
#include "SideOfPlane.h"
#include "platform_common/CoordinateRanges.h"
#include "common/TemplateMesh/Helper.h"
#include "common/interface/Error.h"
#include "i_pathengine.h"
#include "common/STL/map.h"

class cDynamicVertexLookup
{
    const cBSPVertices& _vertices;
    const std::vector<cPlane>& _splitters;
    typedef std::map<tPoint3, std::vector<tVertex_Dynamic> > tMap;
    tMap _map;

    eSideOfPlane sideOfAnotherFaceSharingEdgeGeometry(tEdge_Dynamic sharedEdge1, tEdge_Dynamic sharedEdge2) const;
    bool connectingSurfaceIsBehind(tEdge_Dynamic eConnecting, tEdge_Dynamic e1, tEdge_Dynamic e2) const;

    bool edgesMatch(tVertex_Dynamic v, tVertex_Dynamic testV, bool& connectsToFirstEdge) const;
    bool edgesMatch(bool matchStartOfE, tEdge_Dynamic e, tEdge_Dynamic testE) const;

public:

    cDynamicVertexLookup(const cBSPVertices& vertices, const std::vector<cPlane>& splitters) :
     _vertices(vertices),
     _splitters(splitters)
    {
    }

    void
    add(tVertex_Dynamic v)
    {
          tPoint3 p = v->approximatedPoint3(_vertices, WORLD_RANGE);
         _map[p].push_back(v);
    }

    tVertex_Dynamic getMatch(tVertex_Dynamic v, bool& connectsToFirstEdge) const;

//    typedef typename tMap::iterator iterator;
//    iterator begin()
//    {
//        return _map.begin();
//    }
//    iterator end()
//    {
//        return _map.end();
//    }
};

eSideOfPlane
cDynamicVertexLookup::sideOfAnotherFaceSharingEdgeGeometry(tEdge_Dynamic e1, tEdge_Dynamic e2) const
{
    const cPlane& plane = _splitters[e1.face()->_splitterIndex];
    assertD(CountEdgesAround(e2) >= 3);
    tEdge_Dynamic test = e2.next().next();
    eSideOfPlane side;
    do
    {
        side = SideOfPlane(plane, test.vertex()->dynamicPoint(_vertices));
        if(side != ON_PLANE)
        {
            return side;
        }
        ++test;
    }
    while(test != e2);
    return ON_PLANE;
}

bool
cDynamicVertexLookup::connectingSurfaceIsBehind(tEdge_Dynamic eConnecting, tEdge_Dynamic e1, tEdge_Dynamic e2) const
{
    eSideOfPlane side1 = sideOfAnotherFaceSharingEdgeGeometry(eConnecting, e1);
    eSideOfPlane side2 = sideOfAnotherFaceSharingEdgeGeometry(eConnecting, e2);
    if(side1 != side2)
    {
        switch(side1)
        {
        case IN_FRONT:
            return true;
        case BEHIND:
            return false;
        case ON_PLANE:
            return side2 == BEHIND;
        }
    }
    eSideOfPlane side = sideOfAnotherFaceSharingEdgeGeometry(e1, e2);
    assertR(side != ON_PLANE);
    return side == BEHIND;
}

bool
cDynamicVertexLookup::edgesMatch(tVertex_Dynamic v, tVertex_Dynamic testV, bool& connectsToFirstEdge) const
{
    if(testV.entryIsDead() || testV.isInternalVertex())
    {
        return false;
    }
    if(v == testV)
    {
        return false;
    }
    if(v->dynamicPoint(_vertices) != testV->dynamicPoint(_vertices))
    {
        return false;
    }
    cDynamicPoint3 axis, testAxis;
    axis = GetEdgeAxis3(_vertices, tMesh_Dynamic::firstEdge(v));
    testAxis = GetEdgeAxis3(_vertices, tMesh_Dynamic::lastEdge(testV).prev());
    if(axis.inLine(testAxis) && axis.direction_KnownToBeInline(testAxis) == DIRECTION_BACK)
    {
        connectsToFirstEdge = true;
        return true;
    }
    axis = GetEdgeAxis3(_vertices, tMesh_Dynamic::lastEdge(v).prev());
    testAxis = GetEdgeAxis3(_vertices, tMesh_Dynamic::firstEdge(testV));
    if(axis.inLine(testAxis) && axis.direction_KnownToBeInline(testAxis) == DIRECTION_BACK)
    {
        connectsToFirstEdge = false;
        return true;
    }
    return false;
}
bool
cDynamicVertexLookup::edgesMatch(bool matchStartOfE, tEdge_Dynamic e, tEdge_Dynamic testE) const
{
    if(matchStartOfE)
    {
        if(e.vertex()->dynamicPoint(_vertices) != testE.next().vertex()->dynamicPoint(_vertices))
        {
            return false;
        }
    }
    else
    {
        if(e.next().vertex()->dynamicPoint(_vertices) != testE.vertex()->dynamicPoint(_vertices))
        {
            return false;
        }
    }
    cDynamicPoint3 axis, testAxis;
    axis = GetEdgeAxis3(_vertices, e);
    testAxis = GetEdgeAxis3(_vertices, testE);
    if(axis.inLine(testAxis) && axis.direction_KnownToBeInline(testAxis) == DIRECTION_BACK)
    {
        return true;
    }
    return false;
}

tVertex_Dynamic
cDynamicVertexLookup::getMatch(tVertex_Dynamic v, bool& connectsToFirstEdge) const
{
    tPoint3 p = v->approximatedPoint3(_vertices, WORLD_RANGE);
    tMap::const_iterator i = _map.find(p);
    assertR(i != _map.end());
    const std::vector<tVertex_Dynamic>& vec = i->second;
    tSigned32 j = 0;
    while(!edgesMatch(v, vec[j], connectsToFirstEdge))
    {
        ++j;
//        assertR(j != SizeL(vec));
        if(j == SizeL(vec))
        {
            return tMesh_Dynamic::endVertex();
        }
    }
    tVertex_Dynamic best = vec[j];
    tEdge_Dynamic connectingE, bestE;
    ++j;
    if(connectsToFirstEdge)
    {
        connectingE = tMesh_Dynamic::firstEdge(v);
        bestE = tMesh_Dynamic::lastEdge(v).prev();
        for(; j != SizeL(vec); ++j)
        {
            if(vec[j].entryIsDead() || vec[j].isInternalVertex())
            {
                continue;
            }
            if(v == vec[j])
            {
                continue;
            }
            tEdge_Dynamic candidateE = tMesh_Dynamic::lastEdge(vec[j]).prev();
            if(!edgesMatch(connectsToFirstEdge, connectingE, candidateE))
            {
                continue;
            }
            if(connectingSurfaceIsBehind(connectingE, bestE, candidateE))
            {
                best = vec[j];
                bestE = candidateE;
            }
        }
    }
    else
    {
        connectingE = tMesh_Dynamic::lastEdge(v).prev();
        bestE = tMesh_Dynamic::firstEdge(v);
        for(; j != SizeL(vec); ++j)
        {
            if(vec[j].entryIsDead() || vec[j].isInternalVertex())
            {
                continue;
            }
            if(v == vec[j])
            {
                continue;
            }
            tEdge_Dynamic candidateE = tMesh_Dynamic::firstEdge(vec[j]);
            if(!edgesMatch(connectsToFirstEdge, connectingE, candidateE))
            {
                continue;
            }
            if(connectingSurfaceIsBehind(connectingE, bestE, candidateE))
            {
                best = vec[j];
                bestE = candidateE;
            }
        }
    }
    return best;
}

static void
SplitAndJoin(
        const cBSPVertices& vertices,
        tMesh_Dynamic& mesh,
        tEdge_Dynamic e,
        tEdge_Dynamic targetE
        )
{
    cDynamicPoint3 offset = e.next().vertex()->dynamicPoint(vertices);
    offset -= targetE.vertex()->dynamicPoint(vertices);
    eDirection dir = GetEdgeAxis3(vertices, e).direction_KnownToBeInline(offset);
    if(dir == DIRECTION_FORWARD)
    {
        mesh.splitEdge(e, *targetE.vertex());
        tVertex_Dynamic vAdded = e.next().vertex();
        mesh.connectEdges(targetE, e);
        assertD(vAdded.entryIsDead()); // ConnectFromVertex guarantees not to permanently create any verts
    }
    else
    {
        mesh.splitEdge(targetE, *e.next().vertex());
        ++targetE;
        tVertex_Dynamic vAdded = targetE.vertex();
        mesh.connectEdges(e, targetE);
        assertD(vAdded.entryIsDead()); // ConnectFromVertex guarantees not to permanently create any verts
    }
}

static void
FollowConnectionIfPossible(
        const cBSPVertices& vertices,
        tMesh_Dynamic& mesh,
        tVertex_Dynamic v
        )
{
    do
    {
        tEdge_Dynamic e, targetE;
        e = tMesh_Dynamic::firstEdge(v);
        targetE = tMesh_Dynamic::lastEdge(v).prev();
        if(e.next().vertex()->dynamicPoint(vertices) == targetE.vertex()->dynamicPoint(vertices))
        {
            mesh.connectEdges(e, targetE);
        }
        else
        {
            cDynamicPoint3 axis = GetEdgeAxis3(vertices, e);
            cDynamicPoint3 targetAxis = GetEdgeAxis3(vertices, targetE);
            if(!axis.inLine(targetAxis))
            {
                return;
            }
            if(axis.direction_KnownToBeInline(targetAxis) != DIRECTION_BACK)
            {
                return;
            }
            SplitAndJoin(vertices, mesh, e, targetE);
        }
        v = e.next().vertex();
    }
    while(!v.isInternalVertex());
}

static void
ConnectFromVertex(
        const cBSPVertices& vertices,
        tMesh_Dynamic& mesh,
        tVertex_Dynamic v,
        tVertex_Dynamic targetV,
        bool connectsToFirstEdge
        )
{
    tEdge_Dynamic e, targetE;
    if(connectsToFirstEdge)
    {
        e = tMesh_Dynamic::firstEdge(v);
        targetE = tMesh_Dynamic::lastEdge(targetV).prev();
    }
    else
    {
        e = tMesh_Dynamic::firstEdge(targetV);
        targetE = tMesh_Dynamic::lastEdge(v).prev();
    }
    if(e.next().vertex()->dynamicPoint(vertices) != targetE.vertex()->dynamicPoint(vertices))
    {
        assertD(GetEdgeAxis3(vertices, e).inLine(GetEdgeAxis3(vertices, targetE)) && GetEdgeAxis3(vertices, e).direction_KnownToBeInline(GetEdgeAxis3(vertices, targetE)) == DIRECTION_BACK);
      // one of these edges needs to be split before the connect
        SplitAndJoin(vertices, mesh, e, targetE);
    }
    else
    {
        mesh.connectEdges(e, targetE);
    }
    assertD(!e.next().vertex().isInternalVertex());
    FollowConnectionIfPossible(vertices, mesh, e.next().vertex());
    if(!e.vertex().isInternalVertex())
    {
        FollowConnectionIfPossible(vertices, mesh, e.vertex());
    }
}

bool
ConnectBoundaries(
        const cBSPVertices& vertices,
        tMesh_Dynamic& mesh,
        const std::vector<cPlane>& splitters,
        iProgressCallBack* progressCallBack
        )
{
    cDynamicVertexLookup lookup(vertices, splitters);
    std::vector<tVertex_Dynamic> toProcess;

  // add all boundary vertices to the map
  // (and a toProcess vector, for simplicity - we could also iterate through the map, or again through the mesh itself)

    tVertex_Dynamic v = mesh.beginVertex();
    do
    {
        if(v.isInternalVertex())
        {
            continue;
        }
        if(tMesh_Dynamic::firstEdge(v).face()->wasDiscarded())
        {
            continue;
        }        
        lookup.add(v);
        toProcess.push_back(v);
    }
    while((++v).valid());

  // now loop through verts again, connecting as we go

    bool reportedMatchError = false;

    std::vector<tVertex_Dynamic> toProcessAgain;
    do
    {
        tSigned32 i;
        for(i = 0; i != SizeL(toProcess); ++i)
        {
            tVertex_Dynamic v = toProcess[i];
            if(v.entryIsDead() || v.isInternalVertex())
            {
                continue; // already processed as the target of a previous connection
            }
            if(progressCallBack)
            {
                progressCallBack->updateProgressL("Connecting boundaries", i, SizeL(toProcess) + SizeL(toProcessAgain));
            }
            bool connectsToFirstEdge;
            tVertex_Dynamic targetV = lookup.getMatch(v, connectsToFirstEdge);
//            assertR(targetV.valid());
            if(!targetV.valid())
            {
                // this can happen, for example, if v was at a T-junction
                // otherwise, this is an indication that boundaries do not match in the geometry passed in
                if(!reportedMatchError)
                {
                    cDynamicPoint3 axis1 = GetEdgeAxis3(vertices, tMesh_Dynamic::firstEdge(v));
                    cDynamicPoint3 axis2 = GetEdgeAxis3(vertices, tMesh_Dynamic::lastEdge(v).prev());
                    if(!axis1.inLine(axis2) || axis1.direction_KnownToBeInline(axis2) != DIRECTION_FORWARD)
                    {
                        Error("NonFatal", "ConnectBoundaries(): failed to find a match at vertex. Looks like boundaries in the geometry passed in do not match.");
                        reportedMatchError = true;
                    }
                }
                continue;
            }
            ConnectFromVertex(vertices, mesh, v, targetV, connectsToFirstEdge);
            if(!v.entryIsDead() && !v.isInternalVertex())
            {
                toProcessAgain.push_back(v);
            }
        }
        toProcess.swap(toProcessAgain);
        toProcessAgain.resize(0);
    }
    while(!toProcess.empty());

    return reportedMatchError;
}
