//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Geometry/interface/tPoint_Multiplication.h"
#include "common/TemplateMesh/Helper.h"
#include "common/STL/vector.h"

class cAngleAsComponents
{
    tPoint::tDistance x;
    tPoint::tDistance y;
public:
    cAngleAsComponents()
    {
    }
    cAngleAsComponents(
            const tPoint &base, const tPoint &test
            )
    {
        x=base.right90().dotProduct(test);
        y=base.dotProduct(test);
    }

    bool operator>(const cAngleAsComponents &rhs) const
    {
        tPoint::tDistance x=this->x;
        //... provide abs() in largeinteger
        if(tPoint::tMultiplier::isNegative(x))
            x=-x;

        tPoint::tDistance rhs_x=rhs.x;
        if(tPoint::tMultiplier::isNegative(rhs_x))
            rhs_x=-rhs_x;

        tPoint::tMultipliedDistance l,r;

        tPoint::tMultiplier::multiply(x, rhs.y, l);
        tPoint::tMultiplier::multiply(y, rhs_x, r);

        return l>r;
    }
};

inline cAngleAsComponents MinimumAngleFor_Old(
            const tPoint &p0,
            const tPoint &p1,
            const tPoint &p2,
            const tPoint &p3
            )
{
    cAngleAsComponents best;
    cAngleAsComponents candidate;

    best=cAngleAsComponents(p1-p0,p2-p0);
    candidate=cAngleAsComponents(p3-p0,p2-p0);
    if(best>candidate)
        best=candidate;
    candidate=cAngleAsComponents(p1-p2,p0-p2);
    if(best>candidate)
        best=candidate;
    candidate=cAngleAsComponents(p3-p2,p0-p2);
    if(best>candidate)
        best=candidate;
    candidate=cAngleAsComponents(p0-p1,p2-p1);
    if(best>candidate)
        best=candidate;
    candidate=cAngleAsComponents(p0-p3,p2-p3);
    if(best>candidate)
        best=candidate;

    return best;
}

inline cAngleAsComponents MinimumAngleForTri(
            const tPoint &p0,
            const tPoint &p1,
            const tPoint &p2
            )
{
    tPoint::tDistance d0 = (p1 - p0).lengthSquared();
    tPoint::tDistance d1 = (p2 - p1).lengthSquared();
    tPoint::tDistance d2 = (p0 - p2).lengthSquared();
    if(d0 < d1)
    {
        if(d2 < d0)
        {
            return cAngleAsComponents(p0 - p1, p2 - p1);
        }
        return cAngleAsComponents(p0 - p2, p1 - p2);
    }
    if(d2 < d1)
    {
        return cAngleAsComponents(p0 - p1, p2 - p1);
    }
    return cAngleAsComponents(p1 - p0, p2 - p0);
}

inline cAngleAsComponents MinimumAngleFor(
            const tPoint &p0,
            const tPoint &p1,
            const tPoint &p2,
            const tPoint &p3
            )
{
    cAngleAsComponents best;
    cAngleAsComponents candidate;

    best = MinimumAngleForTri(p0, p1, p2);
    candidate = MinimumAngleForTri(p0, p2, p3);
    if(best > candidate)
        best = candidate;

    return best;
}

template <class tMesh>
bool EdgeIsIllegal(const typename tMesh::edge &edge)
{
    tPoint p[4];

    if(GetEdgeLine(edge.twin().prev()).side(GetEdgeLine(edge.next()))!=SIDE_RIGHT)
        return false;
    if(GetEdgeLine(edge.prev()).side(GetEdgeLine(edge.twin().next()))!=SIDE_RIGHT)
        return false;

    p[0]=GetVertexPoint(edge).point();
    p[1]=GetVertexPoint(edge.twin().prev()).point();
    p[2]=GetVertexPoint(edge.next()).point();
    p[3]=GetVertexPoint(edge.prev()).point();

    cAngleAsComponents candidate = MinimumAngleFor(p[1], p[2], p[3], p[0]);
    cAngleAsComponents current = MinimumAngleFor(p[0], p[1], p[2], p[3]);

//    assertD(!(candidate > MinimumAngleFor_Old(p[1], p[2], p[3], p[0])));
//    assertD(!(MinimumAngleFor_Old(p[1], p[2], p[3], p[0]) > candidate));
//    assertD(!(current > MinimumAngleFor_Old(p[0], p[1], p[2], p[3])));
//    assertD(!(MinimumAngleFor_Old(p[0], p[1], p[2], p[3]) > current));

    return candidate > current;
}


template <class tMesh>
void Delaunay_Legacy(tMesh &mesh)
{
restart:
    typename tMesh::face f=mesh.beginFace();
    while(f.valid())
    {
        if(!f->isExternal())
        {
            typename tMesh::edge e=tMesh::beginEdge(f);
            do
            {
                if(e.twin().face()->isExternal())
                {
                    continue;
                }
                if(EdgeIsIllegal<tMesh>(e))
                {
                    mesh.flipEdge(e);
                    goto restart;
                }
            }
            while(++e!=tMesh::beginEdge(f));
        }
        f++;
    }
}

template <class tMesh>
class cDelaunayFilter_NoEdgesOnExternalFaces
{
    typedef typename tMesh::edge tEdge;
public:
    bool operator()(tEdge e) const
    {
        return e.face()->isExternal() || e.twin().face()->isExternal();
    }
};

template <class tMesh>
class cDelaunayFilter_NoExternalEdges
{
    typedef typename tMesh::edge tEdge;
public:
    bool operator()(tEdge e) const
    {
        return !e.twin().valid();
    }
};

template <class tMesh, class tEdgeFilter>
void Delaunay(tMesh& mesh, tEdgeFilter& filter)
{
    typedef typename tMesh::edge tEdge;

    tSigned32 topIndex = 0;
    tEdge e = mesh.beginEdge();
    do
    {
        if(e.index() > topIndex)
        {
            topIndex = e.index();
        }
    }
    while(e.toNextInMesh());

    std::vector<char> checked(topIndex + 1, false);
    
restart:
    e = mesh.beginEdge();
    do
    {
        if(checked[e.index()])
        {
            continue;
        }
        checked[e.index()] = true;
        if(filter(e))
        {
            continue;
        }
        checked[e.twin().index()] = true;
        if(EdgeIsIllegal<tMesh>(e))
        {
            assertD(CountEdgesAround(e) == 3 && CountEdgesAround(e.twin()) == 3);
            mesh.flipEdge(e);

            checked[e.next().index()] = false;
            checked[e.prev().index()] = false;
            checked[e.twin().next().index()] = false;
            checked[e.twin().prev().index()] = false;

            goto restart;
        }
    }
    while(e.toNextInMesh());
}

