//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef TRANSLATION_BSP_INCLUDED
#define TRANSLATION_BSP_INCLUDED

#include "libs/MeshPair/interface/tMesh_TranslationBSP_Iterators.h"
#include "libs/Mesh3D/interface/tMesh_3D_Iterators.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include "libs/Geometry/interface/tPoint_Header.h"
#include "common/STL/vector.h"

class cTranslationBSP
{
    tSigned32 _leafIndexForUnpartitionedCase;
    std::vector<tEdge_3D> _splitters;
    std::vector<tSigned32> _subTreeIndices;

    void
    performSplit(
            tMesh_TranslationBSP& mesh, std::vector<tEdge_TranslationBSP>& potentialSplitters,
            tSigned32 splitterToUse,
            tSigned32& nextSubSetIndex
            );
    void
    performSplit(
            tMesh_TranslationBSP& mesh, std::vector<tEdge_TranslationBSP>& potentialSplitters,
            tSigned32& nextSubSetIndex
            );

    template <class tPointLike> tSigned32
    query_Recursive(tSigned32 subTree, const tPointLike& p)
    {
        eSide side = SideOfEdge_SI(_splitters[subTree], p);
        if(side == SIDE_LEFT)
        {
            subTree = _subTreeIndices[subTree * 2 + 0];
        }
        else
        {
            subTree = _subTreeIndices[subTree * 2 + 1];
        }
        if(subTree < 0)
        {
            return -1 - subTree;
        }
        return query_Recursive(subTree, p);
    }

public:

    cTranslationBSP(tMesh_TranslationBSP& mesh);

    template <class tPointLike> tSigned32
    query(const tPointLike& p)
    {
        if(_leafIndexForUnpartitionedCase != -1)
        {
            return _leafIndexForUnpartitionedCase;
        }
        return query_Recursive(0, p);
    }
};

#endif
