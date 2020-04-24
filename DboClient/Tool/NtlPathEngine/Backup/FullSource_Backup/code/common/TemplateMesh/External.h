//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef EXTERNAL_INCLUDED
#define EXTERNAL_INCLUDED

#include "common/interface/Assert.h"
#include "common/STL/vector.h"

//.. optimise - construct free vertices first and then add faces around these

template <class tMesh>
typename tMesh::face AddExternalFace(tMesh &mesh, typename tMesh::edge edge)
{
    typedef typename tMesh::tEdgeData tEdgeData;
    typedef typename tMesh::tVertexData tVertexData;
    typedef typename tMesh::tFaceData tFaceData;
    typedef typename tMesh::edge tEdge;
    std::vector<tEdgeData> edgevector;
    std::vector<tVertexData> vertexvector;
    std::vector<tEdge> connectvector;

    typename tMesh::edge e=edge;
    do
    {
        assertD(!e.twin().valid());
        connectvector.push_back(e);
        edgevector.push_back(*e.next());
        vertexvector.push_back(*e.next().vertex());
        e=e.prevExternal();
    }
    while(e!=edge);

    typename tMesh::face addedface = mesh.addFaceAtEnd(tFaceData::externalFace(), SizeL(edgevector), &edgevector.front(), &vertexvector.front());

    e = tMesh::beginEdge(addedface);
    tSigned32 i=0;
    do
    {
      // some code depends on existing vertices not being deleted here
      // mesh.connectEdges() guarantees that the vertices connected to the first edge passed in will not be deleted
        mesh.connectEdges(connectvector[i], e);
        i++;
    }
    while(++e!=tMesh::beginEdge(addedface));

    return addedface;
}

template <class tMesh>
void AddExternalFaces(tMesh &mesh)
{
    if(mesh.empty())
    {
        return;
    }

    typename tMesh::edge e = mesh.beginEdge();
    do
    {
        if(!e.twin().valid())
        {
            typename tMesh::face added = AddExternalFace(mesh, e);
        }
    }
    while(e.toNextInMesh());
}

template <class tMesh>
void RemoveExternalFaces(tMesh& mesh)
{
    typename tMesh::face f = mesh.beginFace();
    while(f.valid())
    {
        typename tMesh::face nextF = f;
        ++nextF;
            
        if(f->isExternal())
        {
            mesh.eraseFace(f);
        }

        f = nextF;
    }
}

template <class tMesh> tSigned32
CountInternalFaces(tMesh& mesh)
{
    typename tMesh::face f = mesh.beginFace();
    assertD(f.valid());
    assertD(!f->isExternal());
    tSigned32 result = 0;
    do
    {
        result++;
        ++f;
        assertD(f.valid());
    }
    while(!f->isExternal());
    return result;
}

template <class T>
bool VertexIsExternal(T start)
{
    T e=start;
    do
    {
        if(e.face()->isExternal())
            return true;
        e=e.twin().next();
    }
    while(e!=start);
    return false;
}

#endif
