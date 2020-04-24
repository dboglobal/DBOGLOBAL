//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef TMESH_TRANSLATIONBSP_INCLUDED
#define TMESH_TRANSLATIONBSP_INCLUDED

#include "libs/MeshPair/interface/tMesh_TranslationBSP_Iterators.h"
#include "libs/Mesh3D/interface/tMesh_3D_Iterators.h"
#include "libs/Geometry/interface/tLine.h"
#include "libs/Geometry/interface/tIntersection.h"

class cE_TranslationBSP
{
public:

    tEdge_3D _e3D;
    tSigned32 _splitterIndex;

    cE_TranslationBSP();
    cE_TranslationBSP(const tEdge_3D e);
};

class cV_TranslationBSP
{
public:

    cV_TranslationBSP()
    {
    }
};

class cF_TranslationBSP
{
public:

    tSigned32 _subSet;
    tSigned32 _originalIndex;

//    cF_TranslationBSP()
//    {
//        index = -1;
//    }

    static bool isExternal()
    {
        return false;
    }
};

tLine GetEdgeLine(tEdge_TranslationBSP edge);
tPoint GetEdgeAxis(tEdge_TranslationBSP edge);
tIntersection GetVertexPoint(tEdge_TranslationBSP edge);

#endif
