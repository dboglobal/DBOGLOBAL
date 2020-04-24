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
#include "CombineEdgeExpansionPieces.h"
#include "ExpansionPieceSplitter.h"
#include "interface/QueryContext.h"
#include "interface/OverlappingPolygon.h"
#include "libs/Mesh2D/interface/MeshCut.h"
#include "libs/Mesh2D/interface/CircuitElement.h"
#include "libs/Geometry/interface/ConvexPoly.h"
#include "libs/Geometry/interface/InsideCorner.h"
#include "libs/Geometry/interface/Side_Directions.h"
#include "libs/Geometry/interface/tIntersection.h"
#include "common/Containers/BitVector.h"

static const tLine& PrevOrStartCapLine(const cCircuitElement& element)
{
    if(element.getPrev())
    {
        return element.getPrev()->refLine();
    }
    return element.refZeroLengthLine();
}

// in the case of elements with approach, the direction of the start cap is used
static bool
ApproachInsideCorner(const cCircuitElement& element, const cCircuitElement& test)
{
    if(InsideCorner(PrevOrStartCapLine(element), element.refLine(), PrevOrStartCapLine(test).reversed()))
    {
        return true;
    }
    if(PrevOrStartCapLine(element).direction(PrevOrStartCapLine(test)) == DIRECTION_FORWARD)
    {
        assertD(test.getIndex() != element.getIndex());
        return test.getIndex() > element.getIndex();
    }
    return false;
}
static bool
DepartInsideCorner(const cCircuitElement& element, const cCircuitElement& test)
{
    if(InsideCorner(PrevOrStartCapLine(element), element.refLine(), test.refLine()))
    {
        return true;
    }
    if(element.refLine().direction(test.refLine()) == DIRECTION_FORWARD)
    {
        assertD(test.getIndex() != element.getIndex());
        return test.getIndex() > element.getIndex();
    }
    return false;
}

static tSigned32
GetFirstDepart(
        const cCircuitElement& approach,
        const std::vector<cCircuitElement*>& departs
        )
{
    assertD(!departs.empty());
    tSigned32 best = 0;
    while(departs[best] == 0)
    {
        ++best;
        assertD(best != SizeL(departs));
    }
    tPoint baseAxis = -approach.refLine().axis();
    tPoint bestAxis = departs[best]->refLine().axis();
    tSigned32 i;
    for(i = best + 1; i != SizeL(departs); ++i)
    {
        if(departs[i])
        {
            eSide side = Side_Directions(baseAxis, bestAxis, departs[i]->refLine().axis());
            assertD(side != SIDE_CENTRE);
            if(side == SIDE_RIGHT)
            {
                best = i;
                bestAxis = departs[i]->refLine().axis();
            }
        }
    }
    return best;
}

static void
DisconnectAll(std::vector<cCircuitElement*>& elements)
{
    tSigned32 i;
    for(i = 0; i != SizeL(elements); ++i)
    {
        if(elements[i]->getPrev())
        {
            elements[i]->disconnectStart_LeaveUncapped();
        }
    }    
}

static void
RedirectAtIntersection(std::vector<cCircuitElement*>& elements)
{
    assertD(SizeL(elements) >= 2);

    cBitVector approachInside(SizeL(elements), false);
    cBitVector departInside(SizeL(elements), false);
    tSigned32 i;
    for(i = 0; i != SizeL(elements); ++i)
    {
        tSigned32 j;
        for(j = 0; j != SizeL(elements); ++j)
        {
            if(i == j)
            {
                continue;
            }
            if(!approachInside[i] && ApproachInsideCorner(*elements[j], *elements[i]))
            {
                approachInside.setBit(i, true);
            }
            if(!departInside[i] && DepartInsideCorner(*elements[j], *elements[i]))
            {
                departInside.setBit(i, true);
            }
        }
    }

    std::vector<cCircuitElement*> approachElements;
    std::vector<cCircuitElement*> departElements;

    for(i = 0; i != SizeL(elements); ++i)
    {
        cCircuitElement* approach = elements[i]->getPrev();
        if(!approachInside[i])
        {
            if(!approach)
            {
                DisconnectAll(elements);
                return;
            }
            approachElements.push_back(approach);
        }
        if(!departInside[i])
        {
            departElements.push_back(elements[i]);
        }
    }

    assertD(SizeL(approachElements) == SizeL(departElements));

    for(i = 0; i != SizeL(elements); ++i)
    {
        cCircuitElement* approach = elements[i]->getPrev();
        if(approach)
        {
            elements[i]->disconnectStart();
        }
    }

    for(i = 0; i != SizeL(approachElements); ++i)
    {
        tSigned32 j = GetFirstDepart(*approachElements[i], departElements);
        approachElements[i]->connectTo(*departElements[j]);
        departElements[j] = 0;
    }
}

void
CombineEdgeExpansionPieces(
        cQueryContext& qc,
        std::vector<cCircuitElement*>& pieces,
        tSigned32 facesInMesh,
        const cConvexPoly& expandShape,
        tMeshCutSequence& result
        )
{
    cExpansionPieceSplitter splitter(facesInMesh);

    assertD((SizeL(pieces) & 1) == 0);
    tSigned32 i;
    tSigned32 index = 0;
    for(i = 0; i != SizeL(pieces); i += 2)
    {
        cCircuitElement* first = pieces[i];
        cCircuitElement* last = pieces[i + 1];
        if(!last)
        {
            last = first->getPrev();
        }
        cCircuitElement* e = first;
        while(e != last)
        {
            e->setIndex(index++);
            e = e->getNext();
        }
        e->setIndex(index++);
        
        splitter.addSequence(pieces[i]);
    }
    pieces.clear();

    assertF(splitter.isValid());

    for(i = 0; i != facesInMesh; ++i)
    {
        std::vector<tIntersection> cornerIntersections;
        std::vector<std::vector<cCircuitElement*> > cornerElements;
        tSigned32 j;
        for(j = 0; j != SizeL(splitter._lookup[i]); ++j)
        {
            cCircuitElement* element = splitter._lookup[i][j];
            if(element->enters() || element->faceCrossed(0).index() != i)
            {
                continue;
            }
            tIntersection elementIntersection;
            element->cornerIntersection(elementIntersection);
            tSigned32 k;
            for(k = 0; k != SizeL(cornerElements); ++k)
            {
                if(elementIntersection == cornerIntersections[k])
                {
                    cornerElements[k].push_back(element);
                    break;
                }
            }
            if(k == SizeL(cornerElements))
            {
                cornerIntersections.push_back(tIntersection());
                element->cornerIntersection(cornerIntersections.back());
                cornerElements.push_back(std::vector<cCircuitElement*>());
                cornerElements.back().push_back(element);
            }
        }
        assertF(splitter.isValid());
        for(j = 0; j != SizeL(cornerElements); ++j)
        {
            if(SizeL(cornerElements[j]) >= 2)
            {
                RedirectAtIntersection(cornerElements[j]);
                assertF(splitter.isValid());
            }
        }
    }

    for(i = 0; i != facesInMesh; ++i)
    {
        tSigned32 j;
        for(j = 0; j != SizeL(splitter._lookup[i]); ++j)
        {
            cCircuitElement* element = splitter._lookup[i][j];
            if(!element)
            {
                continue;
            }
            cCircuitElement* first;
            cCircuitElement* last;
            splitter.stripChain(element, first, last);
            if(last)
            {
                cCircuitElement::deleteCircuit(first);
            }
            else
            {
                tIntersection intersection;
                first->cornerIntersection(intersection);

                bool obstructed;
                cFlagVector& resultBuffer = qc.lockFaceFlagVector();
                tFace* openQueueBuffer = qc.lockFaceBuffer();
                tFace f = first->faceCrossed(0);
                tPoint p;
                if(intersection.isOnIntegerGrid(p))
                {
                    cConvexPoly placedShape;
                    expandShape.offset(p, placedShape);
                    FacesOverlappedByOverlappingPolygon_OF(
                            f,
                            placedShape,
                            openQueueBuffer,
                            resultBuffer,
                            obstructed
                            );
                }
                else
                {
                    FacesOverlappedByOverlappingPolygon_OF(
                            f, intersection,
                            expandShape,
                            openQueueBuffer,
                            resultBuffer,
                            obstructed
                            );
                }
                qc.releaseFlagVector();
                qc.releaseGeneralPurposeBuffer();

                if(obstructed)
                {
                    cCircuitElement::deleteCircuit(first);
                }
                else
                {
                    cHandle<cMeshCut> toAdd = new cMeshCut;
                    toAdd->initFromFragment(*first);
                    toAdd->joinInlineCorners();
                    result.push_back(toAdd);
                }
            }
            assertF(splitter.isValid());
        }
    }
}
