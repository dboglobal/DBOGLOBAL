//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "i_testbedextras.h"
#include "cTestBed.h"
#include "DrawMesh.h"
#include "interface/Singletons_TestBed.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Mesh2D/interface/MeshTraversal.h"
#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "libs/Geometry/interface/InfinitessimallyOffset_PointLike.h"

class cTestBedExtras : public iTestBedExtras
{
    cTestBed& m_testbed;

public:

    cTestBedExtras(cTestBed &testbed)
        :m_testbed(testbed)
    {
    }

    void drawEdge(tSigned32 index, tSigned32 size)
    {
        iGraphicsInterface *gip=m_testbed.getGIP();
        tEdge e = m_testbed.refMesh().lookupEdge(index);
        tFace_3D f3D = m_testbed.refMesh().getOverlappingFaceOn3DJustAfterStartOfEdge(e);
        DrawLineOnGround(nSingletons::testBed().getGIP(), f3D, GetEdgeLine(e), SIDE_RIGHT);
    }
    void draw3DMeshInternalEdges()
    {
        m_testbed.draw3DMeshInternalEdges();
    }
    void drawIntegerCrossesAround(const cPosition& position, tSigned32 radius, tSigned32 cross_size)
    {
        m_testbed.drawIntegerCrossesAround(position,radius,cross_size);
    }
    void draw2DFaceEdges(const cPosition& position)
    {
        iGraphicsInterface *gip=m_testbed.getGIP();
        cInternalPosition ip;
        m_testbed.refMesh().convertPosition(position, ip);
        tEdge e = tMesh::beginEdge(ip.f);
        do
        {
        //... optimise - draw edges that coincide with 3d mesh using their preprocess
        // (certainly at least for the external edges)
            tFace_3D f3D = m_testbed.refMesh().getOverlappingFaceOn3DJustAfterStartOfEdge(e);
            DrawLineOnGround(nSingletons::testBed().getGIP(), f3D, GetEdgeLine(e), SIDE_RIGHT);
        }
        while(e.toNextInFace());
    }
};

