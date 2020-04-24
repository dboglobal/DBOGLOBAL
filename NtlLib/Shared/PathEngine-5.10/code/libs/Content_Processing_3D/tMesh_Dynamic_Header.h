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

class cFDynamic;
class cEDynamic;
class cVDynamic;
typedef cVectorMesh<cFDynamic,cEDynamic,cVDynamic> tMesh_Dynamic;

typedef nVectorMesh_Internal::edge<cFDynamic,cEDynamic,cVDynamic> tEdge_Dynamic;
typedef nVectorMesh_Internal::vertex<cFDynamic,cEDynamic,cVDynamic> tVertex_Dynamic;
typedef nVectorMesh_Internal::face<cFDynamic,cEDynamic,cVDynamic> tFace_Dynamic;
