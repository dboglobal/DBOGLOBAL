//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/tMesh_TranslationBSP.h"
#include "libs/Mesh3D/interface/tMesh_3D.h"

cE_TranslationBSP::cE_TranslationBSP()
{
    _e3D.clear();
    _splitterIndex = -1;
}
cE_TranslationBSP::cE_TranslationBSP(const tEdge_3D e)
{
    _e3D = e;
    _splitterIndex = -1;
}

tLine
GetEdgeLine(tEdge_TranslationBSP edge)
{
    return GetEdgeLine(edge->_e3D);
}
tPoint
GetEdgeAxis(tEdge_TranslationBSP edge)
{
    return GetEdgeAxis(edge->_e3D);
}
tIntersection
GetVertexPoint(tEdge_TranslationBSP edge)
{
    tLine l = GetEdgeLine(edge->_e3D);
    if(l.side(GetEdgeLine(edge.prev()->_e3D)) != SIDE_CENTRE)
    {
        return tIntersection(GetEdgeLine(edge->_e3D), GetEdgeLine(edge.prev()->_e3D));
    }
    return tIntersection(GetEdgeLine(edge->_e3D), GetEdgeLine(edge.twin().next()->_e3D));
}
