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
#include "ExpandExternalEdges.h"
#include "Expand_SI.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Mesh2D/interface/MeshCut.h"
#include "libs/Mesh_Common/interface/Vertex.h"
#include "platform_common/PointMultiplication.h"

static bool
ConcaveCheck(tEdge e)
{
    return ExternalVertexIsConcave(e);
}

bool
NextExternalExpansionForMesh(tEdge& e, tEdge& first, tEdge& last)
{
    if(!e.valid())
    {
        return false;
    }

    tFace f = e.face();

    if(e == tMesh::beginEdge(f))
    {
    // first time this face is processed

        // skip any non external faces
        while(!f->isExternal())
        {
            ++f;
            if(!f.valid())
            {
                e.clear();
                return false;
            }
        }
        e = tMesh::beginEdge(f);

        // find first concave in face, or determine that there are none
        first.clear();
        do
        {
            if(ConcaveCheck(e))
            {
                first = e;
                break;
            }
        }
        while(++e != tMesh::beginEdge(f));

        if(!first.valid())
        {
            // face contains no concave corners and so is expanded in one go
            first = tMesh::beginEdge(f);
            last.clear();
            ++f;
            if(f.valid())
            {
                e = tMesh::beginEdge(f);
            }
            else
            {
                e.clear();
            }
            return true;
        }

        // expand a sequence of edges starting at the first concave corner

        last = first;
        while(1)
        {
            if(ConcaveCheck(last.next()))
            {
                break;
            }
            ++last;
            assertD(last != first);
        }

        // increment edge to start of next section
        // or to next face if there is only one section for this face

        e = last.next();
        if(e == first)
        {
            ++f;
            if(f.valid())
            {
                e = tMesh::beginEdge(f);
            }
            else
            {
                e.clear();
            }
        }
        return true;
    }

    // face has already been processed
    // expand the next convex section

    first = e;
    bool reachedLastEdge = false;

    last = first;
    while(1)
    {
        if(last.next() == tMesh::beginEdge(f))
        {
            reachedLastEdge = true;
        }
        if(ConcaveCheck(last.next()))
        {
            break;
        }
        ++last;
        assertD(last != first);
    }

    if(reachedLastEdge)
    {
        ++f;
        if(f.valid())
        {
            e = tMesh::beginEdge(f);
        }
        else
        {
            e.clear();
        }        
    }
    else
    {
        e = last.next();
    }
    return true;
}

void
ExpandExternalEdges(
        tMesh& mesh, const cConvexPoly& shape,
        std::vector<cCircuitElement*>& appendTo
        )
{
    tEdge e = mesh.beginEdge();
    tEdge first, last;
    while(NextExternalExpansionForMesh(e, first, last))
    {
        if(last.valid())
        {
            ExpandExternalEdges_SI(first, last, shape, appendTo);
        }
        else
        {
            ExpandExternalFace_SI(first.face(), shape, appendTo);
        }
    }
}

