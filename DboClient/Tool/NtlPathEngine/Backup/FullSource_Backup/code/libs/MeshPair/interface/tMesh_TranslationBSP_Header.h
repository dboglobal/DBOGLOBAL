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

class cE_TranslationBSP;
class cV_TranslationBSP;
class cF_TranslationBSP;

typedef cVectorMesh<cF_TranslationBSP,cE_TranslationBSP,cV_TranslationBSP> tMesh_TranslationBSP;
typedef nVectorMesh_Internal::edge<cF_TranslationBSP,cE_TranslationBSP,cV_TranslationBSP> tEdge_TranslationBSP;
typedef nVectorMesh_Internal::vertex<cF_TranslationBSP,cE_TranslationBSP,cV_TranslationBSP> tVertex_TranslationBSP;
typedef nVectorMesh_Internal::face<cF_TranslationBSP,cE_TranslationBSP,cV_TranslationBSP> tFace_TranslationBSP;

