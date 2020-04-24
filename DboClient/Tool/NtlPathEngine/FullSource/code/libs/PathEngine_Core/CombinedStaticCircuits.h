//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef COMBINED_STATIC_CIRCUITS_INCLUDED
#define COMBINED_STATIC_CIRCUITS_INCLUDED

#include "ElementInFaceLookup_Header.h"
#include "libs/Mesh2D/interface/MeshCut_Header.h"
#include "libs/Mesh2D/interface/tMesh_Header.h"
#include "common/STL/vector.h"

template <class T> class cMeshWithTrackedTransform;
class cCircuitElement;
class cQueryContext;
class iXMLInputStream;
class iXMLOutputStream;
class cLocalisedConvexShapeSet;

class cCombinedStaticCircuits
{
    cHandle<tMeshCircuits> _circuits;
    tSigned32 _circuitsElementCount;
//    std::vector<std::vector<cElementInFaceEntry> >* _circuitsLookup;

    cMeshWithTrackedTransform<tMesh>* _circuitCombinationPartitioned;

    std::vector<const cCircuitElement*> _cornerLookupByIndex;
    std::vector<std::vector<cCornerInFaceEntry> > _cornerLookup;

    void constructorCommon(cQueryContext& qc);

public:

    cCombinedStaticCircuits(
        cQueryContext& qc,
        const tMeshCutSequence& edgeExpansion,
        const cLocalisedConvexShapeSet& staticExpansions,
        bool keepPartitioned,
        bool justGeneratePartitioned
        );

    cCombinedStaticCircuits(iXMLInputStream& is, cQueryContext& qc);

    ~cCombinedStaticCircuits();

    void save(iXMLOutputStream& os) const;

    tSigned32 elementCount() const;

//    const std::vector<std::vector<cElementInFaceEntry> >& refLookup() const;
    // currently used for testing purposes only
    std::vector<std::vector<cElementInFaceEntry> >* buildAndTakeCircuitsLookup(cQueryContext& qc) const;

    tSigned32 numberOfCornerElements() const;
    const cCircuitElement& refCornerElement(tSigned32 index) const;
    const std::vector<std::vector<cCornerInFaceEntry> >& refCornerLookup() const
    {
        return _cornerLookup;
    }
    const cMeshWithTrackedTransform<tMesh>& refCircuitCombinationPartitioned() const
    {
        assertD(_circuitCombinationPartitioned);
        return *_circuitCombinationPartitioned;
    }
    // circuit handles cannot be safely used by multiple threads
    cHandle<tMeshCircuits> getCircuits_NotThreadSafe() const
    {
        return _circuits;
    }

};

#endif
