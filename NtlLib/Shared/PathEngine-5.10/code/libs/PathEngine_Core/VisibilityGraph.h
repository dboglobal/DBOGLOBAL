//**********************************************************************
//
// Copyright (c) 2007
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef VISIBILITY_GRAPH_INCLUDED
#define VISIBILITY_GRAPH_INCLUDED

#include "PointsOfVisibilityState.h"
#include "common/Containers/CollapsedVectorVector.h"

class cQueryContext;
class iXMLOutputStream;
class iXMLInputStream;
class cSilhouetteLookup;
class cOffMeshConnections;
class cGraph_StaticComponent;

class cVisibilityGraph
{
    cCollapsedVectorVector<cPointsOfVisibilityState> _connections;
    cCollapsedVectorVector<tUnsigned32> _costs;

    // prevent copying and assignment
    cVisibilityGraph(const cVisibilityGraph&);
    const cVisibilityGraph& operator=(const cVisibilityGraph&);

public:

    // called when generating preprocess, followed by build(), below
    cVisibilityGraph()
    {
    }

    // called when loading preprocess from persistence
    cVisibilityGraph(iXMLInputStream& is);

    void save(iXMLOutputStream& os) const;

    void
    build(
            cQueryContext& qc,
            const cSilhouetteLookup& silhouetteLookup,
            const cGraph_StaticComponent& staticGraph,
            const cOffMeshConnections& offMeshConnections
            );

    tSigned32
    size() const
    {
        return SizeL(_connections);
    }
    tSigned32
    connectionsFrom(cPointsOfVisibilityState sourceState) const
    {
        return _connections.subVectorSize(sourceState.value());
    }
    cPointsOfVisibilityState
    connectionFrom(cPointsOfVisibilityState sourceState, tSigned32 connectionIndex) const
    {
        return _connections[sourceState.value()][connectionIndex];
    }
    tUnsigned32 connectionCost(cPointsOfVisibilityState sourceState, tSigned32 connectionIndex) const
    {
        return _costs[sourceState.value()][connectionIndex];
    }
};

#endif
