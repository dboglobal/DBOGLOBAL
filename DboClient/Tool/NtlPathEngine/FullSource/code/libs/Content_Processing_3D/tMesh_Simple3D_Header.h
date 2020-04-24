//**********************************************************************
//
// Copyright (c) 2005
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

class cESimple3D;
class cVSimple3D;
class cFSimple3D;
typedef cVectorMesh<cFSimple3D,cESimple3D,cVSimple3D> tMesh_Simple3D;

typedef nVectorMesh_Internal::edge<cFSimple3D,cESimple3D,cVSimple3D> tEdge_Simple3D;
typedef nVectorMesh_Internal::vertex<cFSimple3D,cESimple3D,cVSimple3D> tVertex_Simple3D;
typedef nVectorMesh_Internal::face<cFSimple3D,cESimple3D,cVSimple3D> tFace_Simple3D;
