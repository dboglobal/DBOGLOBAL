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
#include "Expand_SI.h"
#include "eExpandSectionType.h"
#include "ExpansionFrame_SI.h"
#include "ExpandSweep.h"
#include "libs/Mesh2D/interface/CutBuilder.h"
#include "libs/Mesh2D/interface/CircuitElement.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include "libs/Geometry/interface/tPoint.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "common/interface/Assert.h"
#include "common/STL/vector.h"

enum eStartType
{
    ET_FRAME_END,
    ET_FOLLOWING_EXTERNAL,
    ET_LEFT_SILHOUETTE,
    ET_MESH_EXIT,
};

static void
DoTraverseOut(tFace f, const tLine& traverseLine, std::list<tEdge>& crossedOut)
{
    tEdge crossed;
    while(TraverseTowardsTarget_SI<tMesh, tPoint>(f, traverseLine, traverseLine.end(), crossed))
    {
        crossedOut.push_back(crossed);
    }
    if(f->isExternal())
    {
        crossedOut.push_back(crossed);
    }
}
//static void
//DoTraverseOut(tFace f, const tLine& traverseLine, std::list<tEdge>& crossedOut, cCircuitBuilder& builder)
//{
//    tEdge crossed;
//    while(TraverseTowardsTarget_SI<tMesh, tPoint>(f, traverseLine, traverseLine.end(), crossed))
//    {
//        crossedOut.push_back(crossed);
//        builder.edgeCrossed(crossed);
//    }
//    if(f->isExternal())
//    {
//        crossedOut.push_back(crossed);
//        builder.meshExit(crossed);
//    }
//}

static void
PerformSweep(
        const cExpansionFrame_SI& frame,
        cExpandSweep& sweep,
        eStartType endType, tEdge endTypeInfo,
        cCutsBuilder& builder
        )
{
    std::list<tEdge> endSilhouetteCrossedOut;
    tEdge endSilhouetteEdge = tMesh::endEdge();
    tLine endSilhouetteLine;
    if(endType == ET_LEFT_SILHOUETTE)
    {
        endSilhouetteEdge = endTypeInfo;
        endSilhouetteLine = frame.expandLine(frame.size() - 1, GetVertexPoint(endTypeInfo));
        DoTraverseOutJustToTheLeft(endTypeInfo.face(), endSilhouetteLine, frame.boundary(frame.size() - 1), endSilhouetteCrossedOut);
    }

    while(true)
    {
        if(endType == ET_FOLLOWING_EXTERNAL
         && sweep.expandSection() + 1 == frame.size()
         && sweep.exitsMesh()
         && sweep.exitEdge() == endTypeInfo
         )
        {
            return;
        }

        if(endType == ET_LEFT_SILHOUETTE
         && sweep.expandSection() + 1 == frame.size()
         && sweep.crossedOutEquals(endSilhouetteCrossedOut)
         )
        {
            tLine buildInLine = frame.expandLine(sweep.expandSection(), GetVertexPoint(endTypeInfo)).reversed();
            if(!sweep.exitsMesh())
            {
              // build right up to the silhouette line
                tFace f = sweep.faceAtBoundary();
                tIntersection target(frame.boundary(sweep.expandSection()), buildInLine);
                tEdge crossed;
                while(TraverseTowardsTarget_SI<tMesh, tIntersection>(f, frame.boundary(sweep.expandSection()), target, crossed))
                {
                    builder.edgeCrossed(crossed);
                }
                if(f->isExternal())
                {
                    builder.meshExit(crossed);
                }
                else
                {
                    builder.endCapped(buildInLine);
                }
            }
            return;
        }

        cExpandSweep::eEvent sweepEvent = sweep.nextEvent();

        if(sweepEvent == cExpandSweep::EXPAND_SECTION_ENDS)
        {
            if(sweep.expandSection() + 1 == frame.size())
            {
                assertD(endType == ET_FRAME_END);
                return;
            }
            sweep.advancePastEvent();
            if(!sweep.exitsMesh())
            {
                builder.changeDirectionIfDifferent(frame.boundary(sweep.expandSection()));
            }
            continue;
        }

        if(sweepEvent == cExpandSweep::INTERNAL_EDGE_CROSSED)
        {
            builder.edgeCrossed(sweep.nextEventEdge());
            sweep.advancePastEvent();
            continue;
        }
        if(sweepEvent == cExpandSweep::MESH_ENTRY)
        {
            builder.meshEntry(sweep.nextEventEdge().twin(), frame.boundary(sweep.expandSection()));
            sweep.advancePastEvent();
            continue;
        }
        if(sweepEvent == cExpandSweep::MESH_EXIT)
        {
            tEdge e = sweep.nextEventEdge();
            builder.meshExit(e);
            if(endType == ET_MESH_EXIT
                && sweep.expandSection() + 1 == frame.size()
                && e == endTypeInfo
                )
            {
                return;
            }
            sweep.advancePastEvent();
            continue;
        }

        assertD(sweepEvent == cExpandSweep::EXTERNAL_VERTEX);
        tEdge vertexSwept = sweep.nextEventEdge();

        if(!sweep.exitsMesh() || sweep.exitEdge().next().vertex() != vertexSwept.vertex())
        {
            tLine buildInLine = frame.expandLine(sweep.expandSection(), GetVertexPoint(vertexSwept)).reversed();

            if(!sweep.exitsMesh())
            {
              // build right up to the silhouette line
                tFace f = sweep.faceAtBoundary();
                tIntersection target(frame.boundary(sweep.expandSection()), buildInLine);
                tEdge crossed;
                while(TraverseTowardsTarget_SI<tMesh, tIntersection>(f, frame.boundary(sweep.expandSection()), target, crossed))
                {
                    builder.edgeCrossed(crossed);
                }
                if(f->isExternal())
                {
                    builder.meshExit(crossed);
                }
                else
                {
                    builder.endCapped(buildInLine);
                }
            }

            if(endType == ET_LEFT_SILHOUETTE
             && sweep.expandSection() + 1 == frame.size()
             && endSilhouetteLine.side(GetVertexPoint(vertexSwept)) == SIDE_CENTRE
             )
            {
                assertD(endSilhouetteLine.direction(buildInLine) == DIRECTION_BACK);
                return;
            }
        }

        sweep.advancePastEvent();

        if(frame.sense(sweep.expandSection(), vertexSwept) == SIDE_CENTRE)
        {
            vertexSwept = vertexSwept.twin().next();
        }

        if(!sweep.exitsMesh() || sweep.exitEdge().vertex() != vertexSwept.vertex())
        {
            if(!sweep.exitsMesh())
            {
                tLine buildOut = frame.expandLine(sweep.expandSection(), GetVertexPoint(vertexSwept));
                tLine boundary = frame.boundary(sweep.expandSection());

                tFace f = sweep.faceAtBoundary();
                tIntersection target(boundary, buildOut); 
                
                tEdge crossed;
                if(TraverseTowardsTarget_SI<tMesh>(f, boundary.reversed(), target, crossed))
                {
                    builder.startCapped(f, buildOut, boundary);
                    builder.edgeCrossed(crossed.twin());
                    assertD(!TraverseTowardsTarget_SI<tMesh>(f, boundary.reversed(), target, crossed));
                }
                else if(f->isExternal())
                {
                    builder.meshEntry(crossed, boundary);
                }
                else
                {
                    while(TraverseTowardsTarget_SI<tMesh>(f, boundary, target, crossed))
                    {
                        assertD(sweep.nextEvent() == cExpandSweep::INTERNAL_EDGE_CROSSED);
                        assertD(sweep.nextEventEdge() == crossed);
                        sweep.advancePastEvent();
                    }
                    builder.startCapped(f, buildOut, boundary);
                }
            }
        }
    }
}

static void
ExpandInternalLoop(
        const cExpansionFrame_SI& frame,
        std::vector<cCircuitElement*>& cuts
        )
{
    cCutsBuilder builder(cuts);

    cExpansionFrame_SI::eStartType frameStartType;
    tEdge startTypeInfo;
    frame.getStartInfo(frameStartType, startTypeInfo);
    assertD(frameStartType == cExpansionFrame_SI::STARTS_INSIDE);
    std::list<tEdge> crossedOut;
    DoTraverseOut(frame.startFace(), frame.sectionStart(0), crossedOut);
    cExpandSweep sweep(frame, frame.startFace(), crossedOut);
    if(!sweep.exitsMesh())
    {
        builder.startLoop(sweep.faceAtBoundary(), frame.boundary(0));
    }
    PerformSweep(frame, sweep, ET_FRAME_END, tEdge(), builder);
    if(!sweep.exitsMesh())
    {
        builder.endLoop();
    }
}

static void
ExpandCut(
        const cExpansionFrame_SI& frame,
        std::vector<cCircuitElement*>& cuts
        )
{
    cCutsBuilder builder(cuts);

    cExpansionFrame_SI::eStartType frameStartType, frameEndType;
    tEdge startTypeInfo, endTypeInfo;
    frame.getStartInfo(frameStartType, startTypeInfo);
    frame.getEndInfo(frameEndType, endTypeInfo);

    assertD(frameStartType != cExpansionFrame_SI::STARTS_INSIDE);

    std::list<tEdge> crossedOut;
    if(frameStartType == cExpansionFrame_SI::BLOCKED)
    {
        eSide sense = frame.sense(0, startTypeInfo);
        if(sense == SIDE_RIGHT)
        {
          // FOLLOWING_EXTERNAL
            crossedOut.push_back(startTypeInfo);
        }
        else
        {
            cExpansionFrame_SI::eTraverseResult traverseResult = frame.reverseTraverse(0, startTypeInfo);
            if(traverseResult == cExpansionFrame_SI::EXITS)
            {
              // MESH_ENTRY
                builder.meshEntry(startTypeInfo, frame.boundary(0));
            }
            else
            if(traverseResult == cExpansionFrame_SI::CROSSES_TO_NEXT)
            {
              // FRAME_START
                DoTraverseOut(frame.startFace(), frame.sectionStart(0), crossedOut);
                if(crossedOut.empty())
                {
                    builder.startCapped(frame.startFace(), frame.sectionStart(0), frame.boundary(0));
                }
                else if(!crossedOut.back().twin().face()->isExternal())
                {
                    builder.startCapped(crossedOut.back().twin().face(), frame.sectionStart(0), frame.boundary(0));
                }
            }
            else
            {
              // RIGHT_SILHOUETTE
                tLine traverseLine = frame.expandLine(0, GetVertexPoint(startTypeInfo.next()));
                DoTraverseOutJustToTheRight(frame.startFace(), traverseLine, frame.boundary(0), crossedOut);
                if(crossedOut.empty())
                {
                    builder.startCapped(frame.startFace(), traverseLine, frame.boundary(0));
                }
                else if(!crossedOut.back().twin().face()->isExternal())
                {
                    builder.startCapped(crossedOut.back().twin().face(), traverseLine, frame.boundary(0));
                }
            }
        }
    }
    else
    {
        assertD(frameStartType == cExpansionFrame_SI::MESH_ENTRY);
      // FRAME_START
        DoTraverseOut(frame.startFace(), frame.sectionStart(0), crossedOut);
        if(crossedOut.empty())
        {
            builder.startCapped(frame.startFace(), frame.sectionStart(0), frame.boundary(0));
        }
        else if(!crossedOut.back().twin().face()->isExternal())
        {
            builder.startCapped(crossedOut.back().twin().face(), frame.sectionStart(0), frame.boundary(0));
        }
    }

    cExpandSweep sweep(frame, frame.startFace(), crossedOut);

    eStartType sweepEndType;
    tSigned32 lastSection = frame.size() - 1;
    if(frameEndType == cExpansionFrame_SI::BLOCKED)
    {
        eSide sense = frame.sense(lastSection, endTypeInfo);
        if(sense == SIDE_RIGHT)
        {
            sweepEndType = ET_FOLLOWING_EXTERNAL;
        }
        else
        {
            cExpansionFrame_SI::eTraverseResult traverseResult = frame.traverse(lastSection, endTypeInfo.twin());
            if(traverseResult == cExpansionFrame_SI::EXITS)
            {
                sweepEndType = ET_MESH_EXIT;
            }
            else
            if(traverseResult == cExpansionFrame_SI::CROSSES_TO_NEXT)
            {
                sweepEndType = ET_FRAME_END;
            }
            else
            {
                sweepEndType = ET_LEFT_SILHOUETTE;
            }
        }
    }
    else
    {
        assertD(frameEndType == cExpansionFrame_SI::MESH_ENTRY);
        sweepEndType = ET_FRAME_END;
    }

    PerformSweep(frame, sweep, sweepEndType, endTypeInfo, builder);

    if(sweepEndType == ET_FRAME_END)
    {
        if(!sweep.exitsMesh())
        {
            builder.endCapped(frame.sectionEnd(lastSection));
        }
    }
}


void
ExpandExternalFace_SI(
        tFace convexExternalFace,
        const cConvexPoly& expandBy,
        std::vector<cCircuitElement*>& appendTo
        )
{
    cExpansionFrame_SI frame(convexExternalFace, expandBy);
    ExpandInternalLoop(frame, appendTo);
}
        
void
ExpandExternalEdges_SI(
        tEdge first, tEdge last,
        const cConvexPoly& expandBy,
        std::vector<cCircuitElement*>& appendTo
        )
{
    cExpansionFrame_SI frame(first, last, expandBy);
    ExpandCut(frame, appendTo);
}

//void
//ExpandCut_SI(
//        const cCircuitElement* cutStartElement,
//        const cConvexPoly& expandBy,
//        std::vector<cCircuitElement*>& cuts
//        )
//{
//    cExpansionFrame_SI frame(cutStartElement, expandBy);
//    if(cutStartElement->enters())
//    {
//        ExpandCut(frame, cuts);
//    }
//    else
//    {
//        ExpandInternalLoop(frame, cuts);
//    }
//}
