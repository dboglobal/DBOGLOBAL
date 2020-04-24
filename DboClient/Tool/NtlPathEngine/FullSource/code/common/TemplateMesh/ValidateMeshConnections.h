//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef VALIDATE_MESH_CONNECTIONS_INCLUDED
#define VALIDATE_MESH_CONNECTIONS_INCLUDED

template <class M>
bool ValidateMeshConnections(M& mesh)
{
    typedef typename M::face tFace;
    typedef typename M::edge tEdge;
    typedef typename M::vertex tVertex;

    tFace f = mesh.beginFace();
    tSigned32 countFaces = 0;
    tSigned32 countEdges1 = 0;
    while(f.valid())
    {
        countFaces++;

    // check edge chain
        tEdge e = M::beginEdge(f);
        do
        {
            countEdges1++;

            if(e.next().prev() != e)
            {
                return false;
            }

            // check connection back to face
            if(e.face() != f)
            {
                return false;
            }

            if(e.twin().valid())
            {
            // check connection back from twin
                if(e.twin().twin() != e)
                {
                    return false;
                }
                if(e.twin().next().vertex() != e.vertex())
                {
                    return false;
                }
                if(e.twin().vertex() != e.next().vertex())
                {
                    return false;
                }
                if(e.twin().face() == f)
                {
                  // could potentially be allowed, but in general is not
                  // and some code certainly depends on this not being true
                    return false;
                }
            }

            // the below should be covered by the edge count check at the end,
            // but can be uncommented to help track down the actual edge corresponding to the error
//            tVertex v = e.vertex();
//            tEdge eAround = M::firstEdge(v);
//            do
//            {
//                if(eAround == e)
//                {
//                    break;
//                }
//            }
//            while(eAround.toNextAroundVertex());
//            if(!eAround.valid())
//            {
//                return false;
//            }
        }
        while(e.toNextInFace());

        ++f;
    }

    if(countFaces != mesh.faces())
    {
        return false;
    }

    tVertex v = mesh.beginVertex();
    tSigned32 countVertices = 0;
    tSigned32 countEdges2 = 0;
    while(v.valid())
    {
        countVertices++;

        if(M::lastEdge(v).valid() && M::lastEdge(v).vertex() != v)
        {
            return false;
        }

    // check edges around vertex
        tEdge e = M::firstEdge(v);
        do
        {
            countEdges2++;

            if(e.prev().next() != e)
            {
                return false;
            }
            
            if(e.prev().twin().valid() && e.prev().twin().twin() != e.prev())
            {
                return false;
            }

            if(e.vertex() != v)
            {
                return false;
            }
        }
        while(e.toNextAroundVertex());

        ++v;
    }

    if(countVertices != mesh.vertices())
    {
        return false;
    }
    if(countEdges1 != countEdges2)
    {
        return false;
    }

    return true;
}

#endif
