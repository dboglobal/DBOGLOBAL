//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************


#include "platform_common/Header.h"
#include "SidesForSplit.h"
#include "SideOfPlane.h"
#include "StoreSideOfPlane.h"
#include "BSP3DOperand.h"
#include "libs/Instrumentation/interface/Profiler.h"

template <int xNZP, int yNZP, int zNZP> void
SidesForSplit_3Template(
        const cBSPVertices& vertices,
        const std::vector<cBSP3DOperand*>& operands,
        const cPlane& splitPlane,
        cBitVector& resultBuffer
        )
{
    tSigned32 i;
    for(i = 0; i != SizeL(operands); ++i)
    {
        assertD(operands[i]->vertsAreConsolidated());
        tVertex_Dynamic v = operands[i]->beginVertex();
        do
        {
            eSideOfPlane side;
            if(v->isOriginalVertex(vertices))
            {
                side = SideOfPlane(splitPlane, v->originalPoint(vertices));
            }
            else
            {
                side = SideOfPlane_TemplatedSpecialCases<xNZP, yNZP, zNZP>(splitPlane, v->dynamicPoint(vertices));
            }
            StoreSideOfPlane(resultBuffer, side);
        }
        while((++v).valid());
    }
}

// ======== template rigging

template <int xNZP, int yNZP> void
SidesForSplit_2Template(
        const cBSPVertices& vertices,
        const std::vector<cBSP3DOperand*>& operands,
        const cPlane& splitPlane,
        cBitVector& resultBuffer
        )
{
    int zNZP = GetNZP(splitPlane._normal.z());
    if(zNZP == -1)
    {
        SidesForSplit_3Template<xNZP, yNZP, -1>(vertices, operands, splitPlane, resultBuffer);
    }
    else if(zNZP == 0)
    {
        SidesForSplit_3Template<xNZP, yNZP, 0>(vertices, operands, splitPlane, resultBuffer);
    }
    else
    {
        SidesForSplit_3Template<xNZP, yNZP, 1>(vertices, operands, splitPlane, resultBuffer);
    }
}
template <int xNZP> void
SidesForSplit_1Template(
        const cBSPVertices& vertices,
        const std::vector<cBSP3DOperand*>& operands,
        const cPlane& splitPlane,
        cBitVector& resultBuffer
        )
{
    int yNZP = GetNZP(splitPlane._normal.y());
    if(yNZP == -1)
    {
        SidesForSplit_2Template<xNZP, -1>(vertices, operands, splitPlane, resultBuffer);
    }
    else if(yNZP == 0)
    {
        SidesForSplit_2Template<xNZP, 0>(vertices, operands, splitPlane, resultBuffer);
    }
    else
    {
        SidesForSplit_2Template<xNZP, 1>(vertices, operands, splitPlane, resultBuffer);
    }
}
void
SidesForSplit(
        const cBSPVertices& vertices,
        const std::vector<cBSP3DOperand*>& operands,
        const cPlane& splitPlane,
        cBitVector& resultBuffer
        )
{
    PROFILER_SCOPE("SidesForSplit");
    int xNZP = GetNZP(splitPlane._normal.x());
    if(xNZP == -1)
    {
        SidesForSplit_1Template<-1>(vertices, operands, splitPlane, resultBuffer);
    }
    else if(xNZP == 0)
    {
        SidesForSplit_1Template<0>(vertices, operands, splitPlane, resultBuffer);
    }
    else
    {
        SidesForSplit_1Template<1>(vertices, operands, splitPlane, resultBuffer);
    }
}
