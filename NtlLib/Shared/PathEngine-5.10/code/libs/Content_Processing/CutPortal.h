//**********************************************************************
//
// Copyright (c) 2004-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Geometry/interface/tLine.h"
#include "libs/Mesh_Common/interface/ElementsTraversal.h"
#include "libs/Mesh_Common/interface/TriangulateFace.h"
#include "libs/Mesh_Common/interface/Triangulate.h"
#include "libs/Mesh_Common/interface/JoinFacesAtVertex.h"
#include "common/TemplateMesh/Helper.h"
#include "common/TemplateMesh/ValidateMeshConnections.h"
#include "common/Containers/BitVector.h"
#include "common/STL/list.h"

template <class tElement> bool
ElementIsPermittedOnPortalCut(
        const tElement& element,
        bool atEnd,
        const cBitVector& edgeMustNotBeCut
        )
{
    typedef typename tElement::tMesh tMesh;
    typedef typename tMesh::edge tEdge;
    switch(element.type())
    {
    default:
        invalid();
    case tElement::FACE:
        return !element.asFace()->isExternal();
    case tElement::VERTEX:
        {
            tEdge e = tMesh::firstEdge(element.asVertex());
            if(VertexIsExternal(e))
            {
                return false;
            }
            if(!atEnd)
            {
              // an internal vertex at an end may connect to another portal
                do
                {
                    if(e.index() < SizeL(edgeMustNotBeCut) && edgeMustNotBeCut[e.index()])
                    {
                        return false;
                    }
                }
                while(e.toNextAroundVertex());
            }
            return true;
        }
    case tElement::EDGE:
        {
            tEdge e = element.asEdge();
            if(e.face()->isExternal())
            {
                return false;
            }
            if(e.twin().face()->isExternal())
            {
                return false;
            }
            if(e.index() < SizeL(edgeMustNotBeCut) && edgeMustNotBeCut[e.index()])
            {
                return false;
            }
            return true;
        }
    }
}

template <class tMesh> typename tMesh::vertex
AddVertexBySplittingEdge(tMesh& mesh, typename tMesh::edge edge,
                         typename tMesh::tVertexData vertexData, typename tMesh::tEdgeData edgeData)
{
    typedef typename tMesh::edge tEdge;
    mesh.splitEdge(edge, vertexData, edgeData);
    tEdge after1, after2;
    after1 = edge.next();
    after2 = after1.next().next();
    mesh.splitFace(after1, after2);
    after1 = edge.twin();
    after2 = after1.next().next();
    mesh.splitFace(after1, after2);
    return edge.next().vertex();
}

template <class tMesh> typename tMesh::vertex
AddVertexInsideFace(tMesh& mesh, typename tMesh::face f,
                    typename tMesh::tVertexData vertexData, typename tMesh::tEdgeData outEdgeData)
{
    typedef typename tMesh::edge tEdge;
    typedef typename tMesh::tEdgeData tEdgeData;
    tEdge e = tMesh::beginEdge(f);
    tSigned32 numberOfSplits = CountEdgesAround(e);
    std::vector<tEdgeData> edgeData(numberOfSplits * 2);
    std::vector<tEdge> after(numberOfSplits);
    tSigned32 i = 0;
    do
    {
        edgeData[i * 2] = outEdgeData;
        edgeData[i * 2 + 1] = *(e.next());
        after[i++] = e;
    }
    while(e.toNextInFace());
    return mesh.splitFaceWithVertex(
        f, numberOfSplits, vertexData, &edgeData.front(), &after.front());
}

template <class tEdge> bool
FlippedEdgeWontCrossLine(tEdge e, const tLine& l)
{
    eSide side, side2;
    side = l.side(GetVertexPoint(e.prev()));
    if(side == SIDE_CENTRE)
    {
        return true;
    }
    side2 = l.side(GetVertexPoint(e.twin().prev()));
    if(side2 == SIDE_CENTRE)
    {
        return true;
    }
    return side == side2;
}

template <class tMesh, class tInterpolator, class tElement> typename tMesh::edge
CutPortal(
        tMesh& mesh,
        tInterpolator& interpolator,
        const cBitVector& edgeMustNotBeCut,
        tElement startElement,
        const tLine& cutLine
        )
{
    typedef typename tMesh::edge tEdge;
    typedef typename tMesh::face tFace;
    typedef typename tMesh::vertex tVertex;
    typedef typename tMesh::tEdgeData tEdgeData;
    typedef typename tMesh::tVertexData tVertexData;

    tElement endElement = startElement;
    while(!ElementContainsPoint(endElement, cutLine.end()))
    {
        if(!ElementIsPermittedOnPortalCut(endElement, endElement == startElement, edgeMustNotBeCut))
        {
            tEdge result;
            result.clear();
            return result;
        }

        endElement = NextElementAlongLine_SpinDoesntMatter(endElement, cutLine);
    }
    if(!ElementIsPermittedOnPortalCut(endElement, true, edgeMustNotBeCut))
    {
        tEdge result;
        result.clear();
        return result;
    }

// portal can be added
// go ahead and modify the mesh

  // insert vertices at start and end, as required

    if(!startElement.isVertex())
    {
      // insert vertex at start
        tVertexData vertexData;
        tEdgeData edgeData;
        interpolator.interpolate(startElement, cutLine.start(), vertexData, edgeData);
        tVertex newVertex;
        if(startElement.isEdge())
        {
            newVertex = AddVertexBySplittingEdge(mesh, startElement.asEdge(), vertexData, edgeData);
        }
        else
        {
            newVertex = AddVertexInsideFace(mesh, startElement.asFace(), vertexData, edgeData);
        }
        assertF(ValidateMeshConnections(mesh));
        if(endElement == startElement)
        {
            endElement = newVertex;
            while(!ElementContainsPoint(endElement, cutLine.end()))
            {
                endElement = NextElementAlongLine(endElement, cutLine);
            }
        }
        startElement = newVertex;
    }
    if(!endElement.isVertex())
    {
      // insert vertex at end
        tVertexData vertexData;
        tEdgeData edgeData;
        interpolator.interpolate(endElement, cutLine.end(), vertexData, edgeData);
        tVertex newVertex;
        if(endElement.isEdge())
        {
            newVertex = AddVertexBySplittingEdge(mesh, endElement.asEdge(), vertexData, edgeData);
        }
        else
        {
            newVertex = AddVertexInsideFace(mesh, endElement.asFace(), vertexData, edgeData);
        }
        assertF(ValidateMeshConnections(mesh));
        endElement = newVertex;
    }

    // BUGFIX the elements were being invalidated because the representing edge is affected
    tVertex startVertex = startElement.asVertex();
    tVertex endVertex = endElement.asVertex();

  // remove any vertices crossed along the traversal

    {
        tElement el;
restart:
        el = tElement(startVertex);
        el = NextElementAlongLine_SpinDoesntMatter(el, cutLine);
        while(1)
        {
            el = NextElementAlongLine(el, cutLine);
            if(el == tElement(endVertex))
            {
                break;
            }
            if(el.isVertex())
            {
                JoinFacesAtVertex_KeepConvex(mesh, el.asVertex(), interpolator);
                assertF(ValidateMeshConnections(mesh));
                goto restart;
            }
        }
    }

  // flip edges crossed to take them out of the line of traversal, wherever this is possible
    
    {
        tElement el;
restart2:
        el = tElement(startVertex);
        el = NextElementAlongLine_SpinDoesntMatter(el, cutLine);
        while(1)
        {
            el = NextElementAlongLine(el, cutLine);
            if(el == tElement(endVertex))
            {
                break;
            }
            if(el.isEdge())
            {
                tEdge e = el.asEdge();
                if(EdgeCanBeFlipped(e) && FlippedEdgeWontCrossLine(e, cutLine))
                {
                    mesh.flipEdge(e);
                    interpolator.interpolateEdge(e);
                    interpolator.interpolateEdge(e.twin());
                    assertF(ValidateMeshConnections(mesh));
                    goto restart2;
                }
            }
        }
    }

  // build the traversal

    std::list<tElement> traversal;
    {
        tElement el = tElement(startVertex);
        traversal.push_back(el);
        el = NextElementAlongLine_SpinDoesntMatter(el, cutLine);
        traversal.push_back(el);
        do
        {
            el = NextElementAlongLine(el, cutLine);
            traversal.push_back(el);
        }
        while(el != tElement(endVertex));
    }

    assertD(traversal.size() >= 3);
    typename std::list<tElement>::iterator i = traversal.begin();
    ++i;
    if(traversal.size() == 3)
    {
        if(i->type() == tElement::EDGE)
        {
            tEdge e = i->asEdge();
            eDirection direction = GetEdgeLine(e).direction(cutLine);
            if(direction == DIRECTION_FORWARD)
            {
                return e;
            }
            assertD(direction == DIRECTION_BACK);
            return e.twin();
        }
        assertD(i->type() == tElement::FACE);
        tFace f = i->asFace();
        tEdge after1 = tMesh::endEdge();
        tEdge after2 = tMesh::endEdge();
        tEdge e;
        e = tMesh::beginEdge(f);
        do
        {
            if(e.vertex() == traversal.front().asVertex())
            {
                after1 = e;
            }
            if(e.vertex() == traversal.back().asVertex())
            {
                after2 = e;
            }
        }
        while(e.toNextInFace());
        assertD(after1.valid());
        assertD(after2.valid());
        mesh.splitFace(after1, after2);
        return after2.prev();
    }

    tEdge after1;
    if(i->type() == tElement::EDGE)
    {
        after1 = i->asEdgeFromVertex(traversal.front().asVertex()).twin().next();
    }
    else
    {
        after1 = EdgeInFaceForVertex<tMesh>(i->asFace(), traversal.front().asVertex());
    }

    tEdge after2;
    typename std::list<tElement>::iterator j = traversal.end();
    ----j;
    if(j->type() == tElement::EDGE)
    {
        after2 = j->asEdgeFromVertex(traversal.back().asVertex()).twin().next();
    }
    else
    {
        after2 = EdgeInFaceForVertex<tMesh>(j->asFace(), traversal.back().asVertex());
    }
    
    ++j;
    do
    {
        if(i->type() == tElement::EDGE)
        {
            mesh.joinFaces(i->asEdge());
        }
        else if(i->type() == tElement::VERTEX)
        {
            mesh.joinFaces(i->asVertex());
        }
        assertF(ValidateMeshConnections(mesh));
    }
    while(++i != j);

    mesh.splitFace(after1, after2);
    assertF(ValidateMeshConnections(mesh));
    tEdge result = after2.prev();

    bool success;
    success = TriangulateFace_NonConvex(mesh, after1.face());
    assertD(success);
    success = TriangulateFace_NonConvex(mesh, after2.face());
    assertD(success);

    return result;
}
