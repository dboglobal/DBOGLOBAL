//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "DrawMesh.h"
#include "libs/TestBed_Application/interface/iGraphicsInterface.h"
#include "Draw3D.h"
#include "platform_common/CoordinateRanges.h"
#include "libs/MeshPair/interface/TranslationToOverlapped.h"
#include "libs/Mesh2D/interface/MeshTraversal.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Mesh2D/interface/MeshCut.h"
#include "libs/Mesh2D/interface/CircuitElement.h"
#include "libs/Mesh3D/interface/DrawLineOnGround.h"
#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "libs/Mesh_Common/interface/PositionForPointInFace.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include "libs/Geometry/interface/tLine.h"
#include "libs/Geometry/interface/Line.h"
#include "libs/Geometry/interface/Point_PointLike.h"
#include "libs/Geometry/interface/Intersection_PointLike.h"
#include "libs/Geometry/interface/Vector2F.h"
#include "interface/Singletons_TestBed.h"
#include "cTestBed.h"
#include "platform_common/PointMultiplication.h"
#include "common/STL/vector.h"

class cGIPRender3DLinesCallBack : public iRender3DLinesCallBack
{
    float _data[6];
    iGraphicsInterface* _gip;

public:

    cGIPRender3DLinesCallBack(iGraphicsInterface* gip)
    {
        _gip = gip;
    }

    void startVertex(float x, float y, float z)
    {
        _data[0] = x;
        _data[1] = y;
        _data[2] = z;
    }
    void vertex(float x, float y, float z)
    {
        _data[3] = x;
        _data[4] = y;
        _data[5] = z;
        iRenderGeometry* rg = _gip->newLine(_data);
        _gip->render(rg);
        _data[0] = x;
        _data[1] = y;
        _data[2] = z;
    }
};

void
DrawLineOnGround(iGraphicsInterface* gip, tFace_3D f, const tLine& line, eSide spinThroughSide,
                 iPointLike& target, const cVector2F& startP, const cVector2F& endP)
{
    cGIPRender3DLinesCallBack callBack(gip);
    float startHeight = f->heightForF(startP);
    callBack.startVertex(startP.x, startP.y, startHeight);
    DrawLineOnGround(f, line, spinThroughSide, target, endP, tPoint(0, 0, 0), callBack);
}
void
DrawLineOnGround(iGraphicsInterface* gip, tFace_3D f, const tLine& line, eSide spinThroughSide)
{
    cGIPRender3DLinesCallBack callBack(gip);
    DrawLineOnGround(f, line, spinThroughSide, tPoint(0, 0, 0), false, callBack);
}

static cVector2F
ApproximateIntersection(const tIntersection& i)
{
    tPoint::tDistance n, d;
    i.getAxisFraction(n, d);
    cVector2F result = i.refLine().axis();
    result *= tMultiplier::asFloat(n);
    result /= tMultiplier::asFloat(d);
    result += i.refLine().start();
    return result;
}
static cVector2F
ApproximateIntersection(const tLine& line1, const tLine& line2)
{
    return ApproximateIntersection(tIntersection(line1, line2));
}

static void
DrawLineInFace(iGraphicsInterface* gip, tFace_3D f, const cVector2F& p1, const cVector2F& p2)
{
    float z1 = nSingletons::testBed().refMesh().heightAtPointIn3DFace(f.index(), p1.x, p1.y);
    float z2 = nSingletons::testBed().refMesh().heightAtPointIn3DFace(f.index(), p2.x, p2.y);
    Draw3DLine(gip, cVector3F(p1.x, p1.y, z1), cVector3F(p2.x, p2.y, z2));
}

static void
DrawCircuitElement_SI(
        iGraphicsInterface* gip, const cTranslationToOverlapped& translation,
        const cCircuitElement& el,
        tFace_3D& startFace, cVector2F& approximateStartPoint
        )
{
    if(!startFace.valid())
    {
        if(el.enters())
        {
            assertD(!startFace.valid());
            tEdge entryEdge = el.entryEdge();
            startFace = translation.translateEntry(entryEdge, el.refLine()).face();
            approximateStartPoint = ApproximateIntersection(el.refLine(), GetEdgeLine(entryEdge));
        }
        else
        {
            tIntersection intersection;
            el.cornerIntersection(intersection);
            startFace = translation.translate(el.faceCrossed(0), intersection);
            approximateStartPoint = ApproximateIntersection(intersection);
        }
    }

    tFace_3D endFace;
    cVector2F approximateEndPoint;
    if(el.exits())
    {
        tEdge exitEdge = el.exitEdge();
        endFace = translation.translateEntry(exitEdge, el.refLine().reversed()).face();
        approximateEndPoint = ApproximateIntersection(el.refLine(), GetEdgeLine(exitEdge));
    }
    else
    {
        tIntersection intersection;
        el.nextCornerIntersection(intersection);
        cIntersection_PointLike pl(intersection);
        endFace = translation.translate(el.lastFaceCrossed(), pl);
        approximateEndPoint = ApproximateIntersection(intersection);
    }

    while(startFace != endFace)
    {
        tEdge_3D crossed = NextEdgeCrossed_SI(tMesh_3D::beginEdge(startFace), el.refLine());
        cVector2F p = ApproximateIntersection(el.refLine(), GetEdgeLine(crossed));
        DrawLineInFace(gip, startFace, approximateStartPoint, p);
        startFace = crossed.twin().face();
        assertD(!startFace->isExternal());
        approximateStartPoint = p;
    }

    DrawLineInFace(gip, startFace, approximateStartPoint, approximateEndPoint);
    approximateStartPoint = approximateEndPoint;
    startFace = endFace;
}

void
DrawMeshCut(const cMeshCut& cut, iGraphicsInterface *gip, const cTranslationToOverlapped& translation)
{
    const cCircuitElement* e = cut.getStartElement();
    tFace_3D startFace;
    cVector2F approximateStartPoint;
    startFace.clear();
    do
    {
		DrawCircuitElement_SI(gip, translation, *e, startFace, approximateStartPoint);
        e = e->getNext();
    }
    while(e && e != cut.getStartElement());
}

void
DrawMeshCutSequence(const tMeshCutSequence& cut_sequence, iGraphicsInterface *gip, const cTranslationToOverlapped& translation)
{
    tMeshCutSequence::const_iterator i;
    for(i = cut_sequence.begin(); i != cut_sequence.end(); ++i)
    {
        DrawMeshCut(**i, gip, translation);
    }
}

void DrawMeshCircuits(const tMeshCircuits& circuits, iGraphicsInterface *gip, const cTranslationToOverlapped& translation)
{
    tMeshCircuits::const_iterator i;
    for(i = circuits.begin(); i != circuits.end(); ++i)
    {
        DrawMeshCutSequence(**i, gip, translation);
    }
}

