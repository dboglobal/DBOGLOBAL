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
#include "SilhouetteLookup.h"
#include "SilhouetteRegion.h"
#include "CombinedStaticCircuits.h"
#include "Graph_StaticComponent.h"
#include "interface/QueryContext.h"
#include "libs/Mesh2D/interface/TranslateToPartitioned.h"
#include "libs/Mesh2D/interface/CircuitElement.h"
#include "libs/Mesh2D/interface/MeshCut.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Mesh_Common/interface/MeshWithTrackedTransform.h"
#include "common/Containers/FlagVector.h"
#include "common/interface/Attributes.h"
#include "common/interface/iXMLOutputStream.h"
#include "common/interface/iXMLInputStream.h"
#include "common/interface/iXMLInputStream_Helpers.h"

static void 
AddSilhouetteRegion(
          cMeshCut& region,
          cPointsOfVisibilityState state,
          const cMeshWithTrackedTransform<tMesh>& partitioned,
          std::vector<std::vector<cPointsOfVisibilityState> >& silhouetteLookup
          )
{
    const cCircuitElement* e = region.getStartElement();
	if(e == 0)
	{
	  // this can happen in case of silhouette region singularities
		return;
	}
#ifdef ASSERTIONS_ON
    region.validateLinks();
    region.validate();
#endif
    assertD(region.isInternalLoop());

    {
        const iMeshTranslator<tMesh>* asTranslator = &partitioned;
        region.remap(cReversedMeshTranslator<tMesh>(asTranslator));
    }

    tSigned32 faceIndex = 0;
    do
    {
        tFace f = region.iterateOverFacesCrossed(e, faceIndex);

        tSigned32 faceIndex = f.index();
		std::vector<cPointsOfVisibilityState>& lookup = silhouetteLookup[faceIndex];

        if(std::find(lookup.begin(), lookup.end(), state) == lookup.end())
        {
          // not already in lookup
            lookup.push_back(state);
        }
    }
    while(e);
}

static void
AddHardCornerRegions(
        const std::vector<std::vector<cElementInFaceEntry> >& lookup,
        cFlagVector& elementFlags,
        const std::vector<std::vector<cCornerInFaceEntry> >& cornerLookup,
        cFlagVector& facesReferenced,
        cMeshWithTrackedTransform<tMesh>& partitioned,
        const cCircuitElement& cornerElement,
        tSigned32 pointIndex,
        std::vector<std::vector<cPointsOfVisibilityState> >& silhouetteLookupVectors
        )
{
    {
        cMeshCut region;
        nSilhouetteRegion::build(
                lookup,
                elementFlags,
                cornerLookup,
                cornerElement, SZ_LEFTSILHOUETTE,
                facesReferenced,
                region
                );
        cPointsOfVisibilityState state(pointIndex, SZ_LEFTSILHOUETTE);
	    AddSilhouetteRegion(region, state, partitioned, silhouetteLookupVectors);
    }
    {
        cMeshCut region;
        nSilhouetteRegion::build(
                lookup,
                elementFlags,
                cornerLookup,
                cornerElement, SZ_RIGHTSILHOUETTE,
                facesReferenced,
                region
                );
        cPointsOfVisibilityState state(pointIndex, SZ_RIGHTSILHOUETTE);
	    AddSilhouetteRegion(region, state, partitioned, silhouetteLookupVectors);
    }
}

static void
AddSoftCornerRegions(
        const std::vector<std::vector<cElementInFaceEntry> >& lookup,
        cFlagVector& elementFlags,
        const std::vector<std::vector<cCornerInFaceEntry> >& cornerLookup,
        cFlagVector& facesReferenced,
        cMeshWithTrackedTransform<tMesh>& partitioned,
        const cInternalPosition& ipOnPartitioned, const tPoint& axisBefore, const tPoint& axisAfter,
        tSigned32 pointIndex,
        std::vector<std::vector<cPointsOfVisibilityState> >& silhouetteLookupVectors
        )
{
    {
        cMeshCut region;
		nSilhouetteRegion::build_Unanchored(
		        lookup,
		        elementFlags,
		        cornerLookup,
		        ipOnPartitioned, axisBefore, axisAfter, SZ_LEFTSILHOUETTE,
				facesReferenced,
				region
                );
        cPointsOfVisibilityState state(pointIndex, SZ_LEFTSILHOUETTE);
		AddSilhouetteRegion(region, state, partitioned, silhouetteLookupVectors);
    }
    {
        cMeshCut region;
		nSilhouetteRegion::build_Unanchored(
		        lookup,
		        elementFlags,
		        cornerLookup,
		        ipOnPartitioned, axisBefore, axisAfter, SZ_RIGHTSILHOUETTE,
				facesReferenced,
				region
                );
        cPointsOfVisibilityState state(pointIndex, SZ_RIGHTSILHOUETTE);
		AddSilhouetteRegion(region, state, partitioned, silhouetteLookupVectors);
    }
}

void
cSilhouetteLookup::buildLookupFromReachableStateVectors(
        std::vector<std::vector<cPointsOfVisibilityState> >& vectors
        )
{
    tSigned32 f;
    for(f = 0; f != SizeL(vectors); ++f)
    {
        std::sort(vectors[f].begin(), vectors[f].end());
        std::vector<cPointsOfVisibilityState>& v = vectors[f];
        tSigned32 i;
      // points reachable from this face with both approach zones
        _lookup.pushBackSubVector();
        for(i = 0; i + 1 < SizeL(v); ++i)
        {
            if(v[i].valid() && v[i].pointIndex() == v[i + 1].pointIndex())
            {
                _lookup.pushBackInLastSubVector(static_cast<tUnsigned32>(v[i].pointIndex()));
                v[i].setInvalid();
                v[i + 1].setInvalid();
            }
        }
      // remaining states reachable from this face, i.e. points with just one approach zone
        _lookup.pushBackSubVector();
        for(i = 0; i != SizeL(v); ++i)
        {
            if(v[i].valid())
            {
                _lookup.pushBackInLastSubVector(v[i].value());
            }
        }
    }
    _lookup.shrinkToFit();
}

cSilhouetteLookup::cSilhouetteLookup(
        cQueryContext& qc,
        cMeshWithTrackedTransform<tMesh>& partitioned,
        const cCombinedStaticCircuits& combinedCircuits,
        cGraph_StaticComponent& staticGraph,
        tSigned32 numberOfHardCorners
        )
{
    const tMeshCircuits& circuits = *combinedCircuits.getCircuits_NotThreadSafe();

    tMeshCircuits partitionedCircuits;
    CopyCircuits(circuits, partitionedCircuits);
    RemapCircuits(partitionedCircuits, partitioned);

    IndexElements(partitionedCircuits, 0);

    std::vector<std::vector<cElementInFaceEntry> >* elementLookup =
        ConstructAndBuildElementsInFaceLookup(partitionedCircuits, partitioned.faces());

    std::vector<std::vector<cCornerInFaceEntry> > cornerLookup;
    cornerLookup.resize(partitioned.faces());

    std::vector<const cCircuitElement*> cornerOnPartitioned(combinedCircuits.numberOfCornerElements());
    {
    // the element corners need remaps
        tSigned32 i;
        for(i = 0; i < combinedCircuits.numberOfCornerElements(); i++)
        {
            const cCircuitElement& el = combinedCircuits.refCornerElement(i);
            tFace f = el.faceCrossed(0);
            tFace fOnPartitioned = TranslateToPartitioned(partitioned, f, el.cornerPoint());
            tSigned32 fIndex = fOnPartitioned.index();
            cornerOnPartitioned[i] = FindMatchingCorner((*elementLookup)[fIndex], el);
            cornerLookup[fIndex].push_back(cCornerInFaceEntry(cornerOnPartitioned[i], i));
        }
    }

    std::vector<std::vector<cPointsOfVisibilityState> > silhouetteLookupVectors(qc.getNumberOfInternalFaces());

    cFlagVector faceReferencedBuffers(partitioned.faces());

    {
        cFlagVector elementFlags(qc.getMaxElementCount());

        tSigned32 pointIndex;
        for(pointIndex = 0; pointIndex < staticGraph.points(); pointIndex++)
        {
            if(staticGraph.graphPointIsObstructed(pointIndex))
            {
                continue;
            }
            if(staticGraph.pointIsCorner(pointIndex))
            {
                tSigned32 cornerIndex = staticGraph.cornerForPointIndex(pointIndex);
                if(cornerIndex < numberOfHardCorners)
                {
                    AddHardCornerRegions(
                        *elementLookup, elementFlags,
                        cornerLookup,
                        faceReferencedBuffers,
                        partitioned,
                        *cornerOnPartitioned[cornerIndex],
                        pointIndex,
                        silhouetteLookupVectors
                        );
                }
                else
                {
                  // soft obstacle corner
                    cInternalPosition ip = staticGraph.position(pointIndex);
					cInternalPosition ipOnPartitioned = TranslateToPartitioned(partitioned, ip);
					tPoint axisBefore(staticGraph.axisBefore(pointIndex));
					tPoint axisAfter(staticGraph.axisAfter(pointIndex));
                    AddSoftCornerRegions(
                        *elementLookup, elementFlags,
                        cornerLookup,
                        faceReferencedBuffers,
                        partitioned,
                        ipOnPartitioned, axisBefore, axisAfter,
                        pointIndex, 
                        silhouetteLookupVectors
                        );
                }
            }
            else
            {
                tSigned32 endPointIndex = staticGraph.endPointForPointIndex(pointIndex);
                cInternalPosition endPointOnOriginal = staticGraph.position(pointIndex);
                cInternalPosition endPointOnPartitioned = TranslateToPartitioned(partitioned, endPointOnOriginal);
                cMeshCut region;
                nSilhouetteRegion::build_360(
                        *elementLookup, elementFlags,
                        cornerLookup,
                        endPointOnPartitioned,
                        faceReferencedBuffers,
                        region
                        );
                cPointsOfVisibilityState state(pointIndex, EPZ_ARRIVING);
                AddSilhouetteRegion(region, state, partitioned, silhouetteLookupVectors);
            }
        }
    }

    //{
    //    cCollapsedVectorVector<cPointsOfVisibilityState> toSwapIn(silhouetteLookupVectors);
    //    toSwapIn.swap(_lookup);
    //}
    buildLookupFromReachableStateVectors(silhouetteLookupVectors);

    DestroyElementsInFaceLookup(elementLookup);
}

cSilhouetteLookup::cSilhouetteLookup(iXMLInputStream& is)
{
    is.openElement("silhouetteLookup");
    while(SkipElementsUntil(is, "pointsFromFace"))
    {
        _lookup.pushBackSubVector();
        is.openNextElement();
        while(SkipElementsUntil(is, "point"))
        {
            const char *const* attributes = is.openNextElement();
            tSigned32 value = ValueAsLong(attributes, "value");
            _lookup.pushBackInLastSubVector(static_cast<tUnsigned32>(value));
            is.closeElement();
        }
        is.closeElement();
        _lookup.pushBackSubVector();
        bool found = SkipElementsUntil(is, "statesFromFace");
        assertD(found);
        if(!found)
        {
            break;
        }
        is.openNextElement();
        while(SkipElementsUntil(is, "state"))
        {
            const char *const* attributes = is.openNextElement();
            tUnsigned32 value = ValueAsUnsignedLong(attributes, "value");
            _lookup.pushBackInLastSubVector(value);
            is.closeElement();
        }
        is.closeElement();
    }
    is.closeElement();
    _lookup.shrinkToFit();
}

void
cSilhouetteLookup::save(iXMLOutputStream& os) const
{
    os.openElement("silhouetteLookup");
    tSigned32 i;
    for(i = 0; i < _lookup.size() / 2; ++i)
    {
        tSigned32 j;
        os.openElement("pointsFromFace");
        for(j = 0; j < pointsFromFace(i); ++j)
        {
            os.openElement("point");
            os.addAttribute("value", pointFromFace(i, j));
            os.closeElement("state");
        }
        os.closeElement("pointsFromFace");
        os.openElement("statesFromFace");
        for(j = 0; j < statesFromFace(i); ++j)
        {
            os.openElement("state");
            os.addAttribute("value", stateFromFace(i, j).value());
            os.closeElement("state");
        }
        os.closeElement("statesFromFace");
    }
    os.closeElement("silhouetteLookup");
}
