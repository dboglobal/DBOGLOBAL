//**********************************************************************
//
// Copyright (c) 2007
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "VisibilityGraph.h"
#include "SilhouetteLookup.h"
#include "Graph_StaticComponent.h"
#include "common/interface/Attributes.h"
#include "common/interface/iXMLInputStream.h"
#include "common/interface/iXMLOutputStream.h"
#include "common/STL_Helpers.h"
#include "platform_common/AddWithOverflow.h"

cVisibilityGraph::cVisibilityGraph(iXMLInputStream& is)
{
    const char *const* rootAttributes = is.openElement("visibilityGraph");
    tSigned32 size = ValueAsLong(rootAttributes, "size");
    std::vector<std::vector<cPointsOfVisibilityState> > connections(size);
    std::vector<std::vector<tUnsigned32> > costs(size);
    tSigned32 i;
    for(i = 0; i < SizeL(connections); ++i)
    {
        const char *const* attributes = is.openElement("source");
        tSigned32 targets = ValueAsLong(attributes, "size");
        connections[i].resize(targets);
        costs[i].resize(targets);
        tSigned32 j;
        for(j = 0; j < SizeL(connections[i]); ++j)
        {
            const char *const* attributes = is.openElement("target");
            connections[i][j] = cPointsOfVisibilityState::fromValue(ValueAsUnsignedLong(attributes, "state"));
            costs[i][j] = ValueAsUnsignedLong(attributes, "cost");
            is.closeElement();
        }
        is.closeElement();
    }
    {
        cCollapsedVectorVector<cPointsOfVisibilityState> toSwap(connections);
        _connections.swap(toSwap);
    }
    {
        cCollapsedVectorVector<tUnsigned32> toSwap(costs);
        _costs.swap(toSwap);
    }
    is.closeElement();
}

void
cVisibilityGraph::save(iXMLOutputStream& os) const
{
    os.openElement("visibilityGraph");
    os.addAttribute("size", _connections.size());
    tSigned32 i;
    for(i = 0; i < _connections.size(); ++i)
    {
        os.openElement("source");
        os.addAttribute("size", _connections.subVectorSize(i));
        tSigned32 j;
        for(j = 0; j < _connections.subVectorSize(i); ++j)
        {
            os.openElement("target");
            os.addAttribute("state", _connections[i][j].value());
            os.addAttribute("cost", _costs[i][j]);
            os.closeElement("target");
        }
        os.closeElement("source");
    }
    os.closeElement("visibilityGraph");
}

static void
CheckAndAddConnection_TwoWay(
        cQueryContext& qc,
        const cGraph_StaticComponent& staticGraph,
        tSigned32 sourcePoint, cPointsOfVisibilityState sourceState,
        const cInternalPosition& sourceIP,
        cPointsOfVisibilityState targetState,
        std::vector<std::vector<cPointsOfVisibilityState> >& connections,
        std::vector<std::vector<tUnsigned32> >& costs
        )
{
    tSigned32 targetPoint = targetState.pointIndex();
    cInternalPosition targetIP = staticGraph.position(targetPoint);
    if(staticGraph.pointIsCorner(sourcePoint))
    {
        eSilhouetteZone sourceZone = staticGraph.zoneFor(sourcePoint, targetIP.p);
        if(sourceZone == SZ_NOTSILHOUETTE)
        {
            return;
        }
        sourceZone = OppositeZone(sourceZone); // encodes by approach zone
        sourceState = cPointsOfVisibilityState(sourcePoint, sourceZone);
    }

    if(staticGraph.testLineCollision(qc, sourceIP, targetIP))
    {
        return;
    }

    tUnsigned32 costOut, costBack;
    bool blockedOut, blockedBack;
    staticGraph.getTotalCosts_TwoWay(qc, sourceIP, targetIP, costOut, costBack, blockedOut, blockedBack);

    if(!blockedOut)
    {
        connections[sourceState.value()].push_back(targetState);
        costs[sourceState.value()].push_back(costOut);
    }
    if(!blockedBack)
    {
        cPointsOfVisibilityState rSourceState = sourceState.reversed();
        cPointsOfVisibilityState rTargetState = targetState.reversed();
        connections[rTargetState.value()].push_back(rSourceState);
        costs[rTargetState.value()].push_back(costBack);
    }
}

void
cVisibilityGraph::build(
        cQueryContext& qc,
        const cSilhouetteLookup& silhouetteLookup,
        const cGraph_StaticComponent& staticGraph,
        const cOffMeshConnections& offMeshConnections
        )
{
    std::vector<std::vector<cPointsOfVisibilityState> > connections(staticGraph.points() * 2);
    std::vector<std::vector<tUnsigned32> > costs(staticGraph.points() * 2);

// connections in mesh

    tSigned32 sourcePoint;
    for(sourcePoint = 0; sourcePoint < staticGraph.points(); sourcePoint++)
    {
        if(staticGraph.graphPointIsObstructed(sourcePoint))
        {
            continue;
        }

        cInternalPosition sourceIP = staticGraph.position(sourcePoint);

        cPointsOfVisibilityState sourceState;
        if(!staticGraph.pointIsCorner(sourcePoint))
        {
        // source state corresponds to an off-mesh connection 'landing' at this endpoint and departing on-mesh
            sourceState = cPointsOfVisibilityState(sourcePoint, EPZ_DEPARTING);
        }
        // otherwise source state depends on zone of target with respect to source corner

        tSigned32 j;
        for(j = 0; j < silhouetteLookup.pointsFromFace(sourceIP.cell()); j++)
        {
            tSigned32 targetPoint = silhouetteLookup.pointFromFace(sourceIP.cell(), j);
            if(targetPoint <= sourcePoint)
            {
                continue;
            }
            assertD(staticGraph.pointIsCorner(targetPoint));

            eSilhouetteZone targetZone = staticGraph.zoneFor(targetPoint, sourceIP.p);
            if(targetZone == SZ_NOTSILHOUETTE)
            {
                continue;
            }
            cPointsOfVisibilityState targetState(targetPoint, targetZone);

            CheckAndAddConnection_TwoWay(
                    qc, staticGraph,
                    sourcePoint, sourceState, sourceIP,
                    targetState,
                    connections, costs
                    );
        }
        for(j = 0; j < silhouetteLookup.statesFromFace(sourceIP.cell()); j++)
        {
            cPointsOfVisibilityState targetState = silhouetteLookup.stateFromFace(sourceIP.cell(), j);

            tSigned32 targetPoint = targetState.pointIndex();
            if(targetPoint <= sourcePoint)
            {
                continue;
            }

            if(staticGraph.pointIsCorner(targetPoint))
            {
                eSilhouetteZone targetZone = targetState.approachSilhouetteZone();
                if(staticGraph.zoneFor(targetPoint, sourceIP.p) != targetZone)
                {
                    continue;
                }
            }

            CheckAndAddConnection_TwoWay(
                    qc, staticGraph,
                    sourcePoint, sourceState, sourceIP,
                    targetState,
                    connections, costs
                    );
        }
    }

// connections off mesh

    tSigned32 i;
    for(i = 0; i < offMeshConnections.numberOfEndPoints(); i++)
    {
        tSigned32 sourcePoint = staticGraph.pointIndexForEndPoint(i);
        if(staticGraph.graphPointIsObstructed(sourcePoint))
        {
            continue;
        }
        cPointsOfVisibilityState sourceState(sourcePoint, EPZ_ARRIVING);
        cInternalPosition sourceIP = staticGraph.position(sourcePoint);
        tSigned32 j;
        for(j = 0; j < offMeshConnections.connectionsFromEndPoint(i); j++)
        {
            tSigned32 targetEndPoint;
            tSigned32 penalty;
            offMeshConnections.connectionFromEndPoint(i, j, targetEndPoint, penalty);
            tSigned32 targetPoint = staticGraph.pointIndexForEndPoint(targetEndPoint);
            if(staticGraph.graphPointIsObstructed(targetPoint))
            {
                continue;
            }
            cPointsOfVisibilityState targetState(targetPoint, EPZ_DEPARTING);

            cInternalPosition targetIP = staticGraph.position(targetPoint);

            tUnsigned32 cost = Length(targetIP.p - sourceIP.p);
            if(AddWithOverflow(cost, penalty, cost))
            {
                cost = 0xffffffff;
            }

            connections[sourceState.value()].push_back(targetState);
            costs[sourceState.value()].push_back(cost);
        }
    }

    {
        cCollapsedVectorVector<cPointsOfVisibilityState> toSwap(connections);
        _connections.swap(toSwap);
    }
    {
        cCollapsedVectorVector<tUnsigned32> toSwap(costs);
        _costs.swap(toSwap);
    }
}
