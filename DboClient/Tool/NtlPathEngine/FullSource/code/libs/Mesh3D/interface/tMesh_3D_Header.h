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

class cE3D;
class cV3D;
class cF3D;
class cF3D_FaceTracked;
typedef cVectorMesh<cF3D,cE3D,cV3D> tMesh_3D;
typedef cVectorMesh<cF3D_FaceTracked,cE3D,cV3D> tMesh_3D_FaceTracked;

namespace nVectorMesh_Internal
{
    template <class F,class E,class V> class edge;
    template <class F,class E,class V> class vertex;
    template <class F,class E,class V> class face;
}

typedef nVectorMesh_Internal::edge<cF3D,cE3D,cV3D> tEdge_3D;
typedef nVectorMesh_Internal::vertex<cF3D,cE3D,cV3D> tVertex_3D;
typedef nVectorMesh_Internal::face<cF3D,cE3D,cV3D> tFace_3D;

typedef nVectorMesh_Internal::edge<cF3D_FaceTracked,cE3D,cV3D> tEdge_3D_FaceTracked;
typedef nVectorMesh_Internal::vertex<cF3D_FaceTracked,cE3D,cV3D> tVertex_3D_FaceTracked;
typedef nVectorMesh_Internal::face<cF3D_FaceTracked,cE3D,cV3D> tFace_3D_FaceTracked;
