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
#include "tMesh_Dynamic.h"

//cVector3F
//cVDynamic::getPoint3F(const cBSPVertices& vertices) const
//{
//    cVector3F result = dynamicPoint(vertices).as3F();
//}
tPoint3
cVDynamic::approximatedPoint3(const cBSPVertices& vertices, tSigned32 range) const
{
    return dynamicPoint(vertices).approximated(range);
}
cVector3F GetVertexPoint3F(const cBSPVertices& vertices, const tEdge_Dynamic e)
{
    cVector3F result = e.vertex()->dynamicPoint(vertices).as3F();
    result.z() = static_cast<float>(e->_approximateZ);
    return result;
}
cDynamicPoint3
GetEdgeAxis3(const cBSPVertices& vertices, tEdge_Dynamic e)
{
    cDynamicPoint3 axis = e.next().vertex()->dynamicPoint(vertices);
    axis -= e.vertex()->dynamicPoint(vertices);
    return axis;
}

