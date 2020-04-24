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
#include "CombinedStaticCircuits.h"
#include "CombineCircuits.h"
#include "SortCornerElements.h"
#include "ElementInFaceLookup.h"
#include "interface/QueryContext.h"
#include "libs/Mesh2D/interface/SaveCircuits_XML.h"
#include "libs/Mesh2D/interface/LoadCircuits_XML.h"
#include "libs/Mesh2D/interface/MeshCut.h"
#include "libs/Mesh2D/interface/CircuitElement.h"
#include "libs/Mesh_Common/interface/MeshWithTrackedTransform.h"
#include "common/STL_Helpers.h"

void
cCombinedStaticCircuits::constructorCommon(cQueryContext& qc)
{
    _circuitsElementCount = IndexElements(*_circuits, 0);
//    _circuitsLookup = ConstructAndBuildElementsInFaceLookup(*_circuits, qc.getNumberOfInternalFaces());
//    ShrinkToFit_Compound(*_circuitsLookup);

    _cornerLookupByIndex.resize(_circuitsElementCount);
    _cornerLookupByIndex.clear();
    {
        cCircuitsConstIterator i(*_circuits);
        while(i.valid())
        {
            if(i->cornerIsConvex())
            {
                _cornerLookupByIndex.push_back(&*i);
            }
            ++i;
        }
    }

    SortCornerElements(_cornerLookupByIndex);

    _cornerLookup.resize(qc.getNumberOfInternalFaces());
    tSigned32 i;
    for(i = 0; i < SizeL(_cornerLookupByIndex); i++)
    {
        const cCircuitElement& el = *_cornerLookupByIndex[i];
        tFace f = el.faceCrossed(0);
        _cornerLookup[f.index()].push_back(cCornerInFaceEntry(&el, i));
    }
}

cCombinedStaticCircuits::cCombinedStaticCircuits(
        cQueryContext& qc,
        const tMeshCutSequence& edgeExpansion,
        const cLocalisedConvexShapeSet& staticExpansions,
        bool keepPartitioned,
        bool justGeneratePartitioned
        )
{
    _circuits = new tMeshCircuits;
    {
        tMeshCutSequence edgeExpansionCopy;
        CopyCutSequence(edgeExpansion, edgeExpansionCopy);
        ConnectBaseObstacleCircuits(
                qc, edgeExpansionCopy, 
                staticExpansions,
                *_circuits,
                keepPartitioned,
                justGeneratePartitioned,
                _circuitCombinationPartitioned
                );
    }
    if(justGeneratePartitioned)
    {
        return;
    }
    assertD(ValidateCircuits(*_circuits));
    assertD(!CircuitsContainInlineCorner(*_circuits));
    NormaliseLinesCircuits(*_circuits);

    constructorCommon(qc);
}


cCombinedStaticCircuits::cCombinedStaticCircuits(iXMLInputStream& is, cQueryContext& qc)
{
    _circuits = new tMeshCircuits;
    LoadCircuits_XML(qc.refMesh(), is, *_circuits);
    _circuitCombinationPartitioned = 0;

    constructorCommon(qc);
}

cCombinedStaticCircuits::~cCombinedStaticCircuits()
{
//    delete _circuitsLookup;
    delete _circuitCombinationPartitioned;
}

void
cCombinedStaticCircuits::save(iXMLOutputStream& os) const
{
    SaveCircuits_XML(*_circuits, os);
}

tSigned32
cCombinedStaticCircuits::elementCount() const
{
    return _circuitsElementCount;
}

//const std::vector<std::vector<cElementInFaceEntry> >&
//cCombinedStaticCircuits::refLookup() const
//{
//    return *_circuitsLookup;
//}

std::vector<std::vector<cElementInFaceEntry> >*
cCombinedStaticCircuits::buildAndTakeCircuitsLookup(cQueryContext& qc) const
{
    return ConstructAndBuildElementsInFaceLookup(*_circuits, qc.getNumberOfInternalFaces());
//    ShrinkToFit_Compound(*_circuitsLookup);
}

tSigned32
cCombinedStaticCircuits::numberOfCornerElements() const
{
    return SizeL(_cornerLookupByIndex);
}
const cCircuitElement&
cCombinedStaticCircuits::refCornerElement(tSigned32 index) const
{
    return *_cornerLookupByIndex[index];
}
