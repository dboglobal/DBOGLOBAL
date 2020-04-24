//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/PointIsInsideCircuits.h"
#include "ElementInFaceLookup.h"
#include "interface/LineCollision.h"
#include "libs/Geometry/interface/tIntersection.h"
#include "libs/Mesh2D/interface/InternalPosition.h"
#include "libs/Mesh2D/interface/CircuitElement.h"
#include "libs/Mesh2D/interface/MeshTraversal.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include <common/STL/vector.h>

template <class tPointLike> void
CountInFirstFace(const std::vector<cElementInFaceEntry>& lookup,
                 const tPointLike& p,
                 const tLine& lineFromP,
                 const tLine& reversedLine,
                 tSigned32& entries, tSigned32& exits
                 )
{
    size_t i;
    bool atCorner_Ignored;
    const tLine* intersect;

    for(i = 0; i < lookup.size(); i++)
    {
        const cElementInFaceEntry& e = lookup[i];
        if(e.element->lineEntersInFaceOrZeroLength(
                            e.faceIndex,
                            lineFromP,
                            atCorner_Ignored, atCorner_Ignored,
                            intersect
                            ))
        {
            if(SideOfLine(*intersect, p) != SIDE_RIGHT)
            {
                entries++;
            }
        }
        else
        if(e.element->lineEntersInFaceOrZeroLength(
                            e.faceIndex,
                            reversedLine,
                            atCorner_Ignored, atCorner_Ignored,
                            intersect
                            ))
        {
            if(SideOfLine(*intersect, p) == SIDE_RIGHT)
            {
                exits++;
            }
        }
    }
}

static void
CountInMidFace(const std::vector<cElementInFaceEntry>& lookup,
               const tLine& lineFromP,
               const tLine& reversedLine,
               tSigned32& entries, tSigned32& exits
               )
{
    size_t i;
    bool atCorner_Ignored;
    const tLine* intersect;
    for(i = 0; i < lookup.size(); i++)
    {
        if(lookup[i].element->lineEntersInFaceOrZeroLength(
                            lookup[i].faceIndex,
                            lineFromP,
                            atCorner_Ignored, atCorner_Ignored,
                            intersect
                            ))
        {
            entries++;
        }
        else
        if(lookup[i].element->lineEntersInFaceOrZeroLength(
                            lookup[i].faceIndex,
                            reversedLine,
                            atCorner_Ignored, atCorner_Ignored,
                            intersect
                            ))
        {
            exits++;
        }
    }
}

static bool
ExitIsToContainedFace(const std::list<tFace>& containedFaces, tEdge exit)
{
    tFace exitFace = exit.twin().face();
    std::list<tFace>::const_iterator j;
    for(j = containedFaces.begin(); j != containedFaces.end(); ++j)
    {
        if(*j == exitFace)
        {
            return true;
        }
    }
    return false;
}

template <class tCircuits, class tPointLike> bool
PointIsInsideCircuits(const tCircuits& circuits,
                      const std::list<tFace>* containedExternalFaces,
                      tFace f,
                      tPointLike p,
                      const tLine lineFromP
                      )
{
    tSigned32 entries = 0;
    tSigned32 exits = 0;
    
    tLine reversedLine(lineFromP.end(), lineFromP.start());

    std::vector<cElementInFaceEntry> elementLookup;

    LookupElementsInFace(f, circuits, elementLookup);
    CountInFirstFace(elementLookup, p, lineFromP, reversedLine, entries, exits);
    elementLookup.clear();

    tEdge crossed;
    while(TraverseToExit_SI<tMesh>(f, lineFromP, crossed))
    {
        LookupElementsInFace(f, circuits, elementLookup);
        CountInMidFace(elementLookup, lineFromP, reversedLine, entries, exits);
        elementLookup.clear();
    }
    tEdge exit(crossed);
  
    if(ExitsThroughExternalSection(exit, lineFromP, circuits))
    {
        exits++;
    }

    if(containedExternalFaces && ExitIsToContainedFace(*containedExternalFaces, exit))
    {
        exits++;
    }

    assertD(exits == entries || exits == entries + 1);
    if(exits == entries + 1)
    {
        return true;
    }

    return false;
}

bool
PointIsInsideCircuits(const tMeshCircuits& circuits, const cInternalPosition& ip)
{
    tLine lineFromP(ip.p, ip.p + tPoint(1, 0, 1));
    return PointIsInsideCircuits(circuits, 0, ip.f, ip.p, lineFromP);
}
bool
PointIsInsideCircuits(const tMeshCircuits& circuits, const std::list<tFace>& containedExternalFaces, const cInternalPosition& ip)
{
    tLine lineFromP(ip.p, ip.p + tPoint(1, 0, 1));
    return PointIsInsideCircuits(circuits, &containedExternalFaces, ip.f, ip.p, lineFromP);
}
bool
PointIsInsideCircuit(const tMeshCutSequence& circuit, tFace f, tIntersection p)
{
    return PointIsInsideCircuits(circuit, 0, f, p, p.refLine());
}
