//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

template <class F,class E,class V> class cVectorMesh;
namespace nVectorMesh_Internal
{
    template <class F,class E,class V> class edge;
    template <class F,class E,class V> class vertex;
    template <class F,class E,class V> class face;
}

class cE;
class cV;
class cF;

typedef cVectorMesh<cF,cE,cV> tMesh;
typedef nVectorMesh_Internal::edge<cF,cE,cV> tEdge;
typedef nVectorMesh_Internal::vertex<cF,cE,cV> tVertex;
typedef nVectorMesh_Internal::face<cF,cE,cV> tFace;

