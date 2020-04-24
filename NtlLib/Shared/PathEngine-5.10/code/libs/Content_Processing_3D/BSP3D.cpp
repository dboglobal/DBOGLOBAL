//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "BSP3D.h"
#include "BSPVertices.h"
#include "BSPTree.h"
#include "BSP3DOperand.h"
#include "BSP3DManager.h"
#include "GetOriginAndNormal.h"
#include "StripTerrainHeightDetail.h"
#include "TriangulateDynamic.h"
#include "SideOfProjectedTo2D.h"
#include "ConnectEdgePairs.h"
#include "ConnectBoundaries.h"
#include "SourceDataLookup.h"
#include "BSPStats.h"
#include "SideOfPlane.h"
#include "ProcessVerticalFaces.h"
#include "Approximate.h"
#include "platform_common/CoordinateRanges.h"
#include "common/Containers/BitVector.h"
#include "common/TemplateMesh/Helper.h"
#include "common/TemplateMesh/ValidateMeshConnections.h"
#include "common/interface/Attributes.h"
#include "i_pathengine.h"

static bool
FaceGeometryIdentical(const cBSPVertices& vertices, tFace_Dynamic f, tFace_Dynamic f2)
{
// assumes that vertices are never repeated in a face
    tEdge_Dynamic e = tMesh_Dynamic::beginEdge(f);
    tEdge_Dynamic e2 = tMesh_Dynamic::beginEdge(f2);
    while(1)
    {
        if(e2.vertex()->dynamicPoint(vertices) == e.vertex()->dynamicPoint(vertices))
        {
            break;
        }
        bool iterate = e2.toNextInFace();
        if(!iterate)
        {
            return false;
        }
    }
    do
    {
        if(e2.vertex()->dynamicPoint(vertices) != e.vertex()->dynamicPoint(vertices))
        {
            return false;
        }
        ++e2;
    }
    while(e.toNextInFace());
    return true;
}

void
CombineOperands_PostBSP(
        const cBSPVertices& vertices,
        tMesh_Dynamic& result,
        const std::vector<cPlane>& splitters,
        iProgressCallBack* progressCallBack
        )
{
#ifdef ASSERTIONS_ON
    // check there are no repeated fragments
    if(FullAssertsOn())
    {
        tFace_Dynamic f = result.beginFace();
        do
        {
            if(f->wasDiscarded())
            {
                continue;
            }
            if(progressCallBack)
            {
                progressCallBack->updateProgressL("Checking for repeated fragments", f.index(), result.faceSize());
            }
            tFace_Dynamic f2 = f;
            while((++f2).valid())
            {
                if(f2->wasDiscarded())
                {
                    continue;
                }
                assertD(!FaceGeometryIdentical(vertices, f, f2));
            }
        }
        while((++f).valid());
    }
#endif

    ConnectEdgePairs(vertices, result, progressCallBack);
    bool matchError = ConnectBoundaries(vertices, result, splitters, progressCallBack);
//    assertR(!matchError); // add this assert if we cannot handle unconnected edges in the result
}

void
GetSplitterProjectionTypes(
        const std::vector<cPlane>& splitters,
        std::vector<int>& projectionTypes
        )
{
    assertD(projectionTypes.size() == splitters.size());
    tSigned32 i;
    for(i = 0; i != SizeL(splitters); ++i)
    {
        projectionTypes[i] = GetProjectionType(splitters[i]._normal);
    }
}

void
Simplify_PostBSP(
        const cBSPVertices& vertices,
        iFacesCanBeCombinedCallback* facesCanBeCombinedCallback,
        tMesh_Dynamic& result,
        const std::vector<cPlane>& splitters,
        const std::vector<int>& projectionTypes,
        iProgressCallBack* progressCallBack
        )
{
    std::vector<tSigned32> splitterRemaps(SizeL(splitters));
    {
        tSigned32 i;
        for(i = 0; i != SizeL(splitters); ++i)
        {
            if(progressCallBack)
            {
                progressCallBack->updateProgressL("Generating splitter remaps", i, SizeL(splitters));
            }
            tSigned32 j;
            for(j = 0; j != i; ++j)
            {
                if(splitterRemaps[j] != j)
                { // identical to another splitter that has already been checked
                    continue;
                }
                if(projectionTypes[i] != projectionTypes[j])
                { // early out
                    continue;
                }
                if(PlanesAreEqual(splitters[i], splitters[j]))
                {
                    break;
                }
            }
            splitterRemaps[i] = j;
        }
    }
    {
        tFace_Dynamic f = result.beginFace();
        do
        {
            f->_splitterIndex = splitterRemaps[f->_splitterIndex];
        }
        while((++f).valid());
    }

    TriangulateMesh_Dynamic(result, vertices, projectionTypes, progressCallBack);

    // remove on-plane internal vertices and 'boundary vertices'

    {
        tVertex_Dynamic v = result.beginVertex();
        tSigned32 toProcess = result.vertSize();
        do
        {
            if(progressCallBack)
            {
                progressCallBack->updateProgressL("Removing on-plane and boundary vertices", v.index(), toProcess);
            }
            tVertex_Dynamic candidateV = v;
            ++v;
            if(tMesh_Dynamic::lastEdge(candidateV).valid())
            {
                continue;  // only consider internal vertices
            }
            tEdge_Dynamic e = tMesh_Dynamic::firstEdge(candidateV);
            if(e.face()->wasDiscarded())
            {
                continue;  // don't process verts on discarded geometry
            }

            tEdge_Dynamic eBoundary1, eBoundary2;
            eBoundary1.clear();
            eBoundary2.clear();
            do
            {
                if(e.face()->_splitterIndex == e.twin().face()->_splitterIndex)
                {
                    continue;
                }
                if(!eBoundary1.valid())
                {
                    eBoundary1 = e.twin();
                    continue;
                }
                if(eBoundary2.valid())
                {
                    break; // more than two splitters meet here
                }
                eBoundary2 = e;
            }
            while(e.toNextAroundVertex());
            if(e.valid())
            {
              // broke out of loop above
                continue;
            }

            if(!eBoundary1.valid())
            {
              // on-plane vertex
                if(facesCanBeCombinedCallback)
                {
                    tEdge_Dynamic e2 = tMesh_Dynamic::firstEdge(candidateV);
                    do
                    {
                        if(!(*facesCanBeCombinedCallback)(e2.face(), e2.twin().face()))
                        {
                            break;
                        }
                    }
                    while(e2.toNextAroundVertex());
                    if(e2.valid())
                    {
                        continue;
                    }
                }
                RemoveInternalVertex_Dynamic(result, candidateV, vertices, projectionTypes[tMesh_Dynamic::firstEdge(candidateV).face()->_splitterIndex]);
            }
            else
            {
              // boundary vertex
                if(facesCanBeCombinedCallback)
                {
                    tEdge_Dynamic e2;
                    e2 = eBoundary1.next();
                    while(e2 != eBoundary2)
                    {
                        if(!(*facesCanBeCombinedCallback)(e2.face(), e2.twin().face()))
                        {
                            break;
                        }
                        e2 = e2.twin().next();
                    }
                    if(e2 != eBoundary2)
                    {
                        continue;
                    }
                    e2 = eBoundary2.twin().next();
                    while(e2 != eBoundary1.twin())
                    {
                        if(!(*facesCanBeCombinedCallback)(e2.face(), e2.twin().face()))
                        {
                            break;
                        }
                        e2 = e2.twin().next();
                    }
                    if(e2 != eBoundary1.twin())
                    {
                        continue;
                    }
                }
                RemoveInlineBoundaryVertex_Dynamic(result, eBoundary1, eBoundary2, vertices, projectionTypes[eBoundary1.face()->_splitterIndex], projectionTypes[eBoundary1.twin().face()->_splitterIndex]);
            }
        }
        while(v.valid());
    }
}

void
RemoveDownwardFacingFaces(
        tMesh_Dynamic& mesh,
        const std::vector<cPlane>& splitters
        )
{
    tFace_Dynamic f = mesh.beginFace();
    do
    {
        tFace_Dynamic toProcess = f;
        ++f;
        if(toProcess->wasDiscarded())
        {
            continue;
        }
        if(splitters[toProcess->_splitterIndex]._normal.z().isNegative())
        {
            mesh.eraseFace(toProcess);
        }
    }
    while(f.valid());
}

static bool
IsAboveMaxSlope(
        const cDynamicPoint3& v,
        const cDynamicFraction& maxSlopeFractionSquared
        )
{
    cDynamicFraction verticalComponentSquared = v.x() * v.x() + v.y() * v.y();
    cDynamicFraction horizontalComponentSquared(v.z());
    horizontalComponentSquared *= v.z();
    return verticalComponentSquared > horizontalComponentSquared * maxSlopeFractionSquared;
}

static void
RemoveFacesAboveMaxSlope(
        tMesh_Dynamic& mesh,
        const std::vector<cPlane>& splitters,
        const cDynamicFraction& maxSlopeFraction,
        iProgressCallBack* progressCallBack
        )
{
    cDynamicFraction maxSlopeFractionSquared(maxSlopeFraction);
    maxSlopeFractionSquared *= maxSlopeFraction;
    tFace_Dynamic f = mesh.beginFace();
    do
    {
        tFace_Dynamic toProcess = f;
        ++f;
        if(toProcess->wasDiscarded())
        {
            continue;
        }
        if(splitters[toProcess->_splitterIndex]._normal.z().isZero())
        {
            continue;
        }
        if(progressCallBack && (f.index() & 7) == 0)
        {
            progressCallBack->updateProgressL("Removing faces above max slope.", f.index(), mesh.faceSize());
        }
        if(IsAboveMaxSlope(splitters[toProcess->_splitterIndex]._normal, maxSlopeFractionSquared))
        {
            mesh.eraseFace(toProcess);
        }
    }
    while(f.valid());
}

static void
SetupBitVectors(
        tMesh_Dynamic& result,
        const std::vector<cPlane>& splitters,
        cBitVector& isGroundSurface,
        cBitVector& isVertical,
        iProgressCallBack* progressCallBack
        )
{
    result.consolidate();
    isGroundSurface.resize(result.faces());
    isVertical.resize(result.faces());
    tFace_Dynamic f = result.beginFace();
    do
    {
        if(f->wasDiscarded())
        {
            isGroundSurface.setBit(f.index(), false);
            isVertical.setBit(f.index(), false);
            continue;
        }
        if(progressCallBack && (f.index() & 7) == 0)
        {
            progressCallBack->updateProgressL("Getting ground surface faces", f.index(), result.faces());
        }
        const cPlane& splitter = splitters[f->_splitterIndex];
        isGroundSurface.setBit(f.index(), !splitter._normal.z().isNegative() && !splitter._normal.z().isZero());
        isVertical.setBit(f.index(), splitter._normal.z().isZero());
    }
    while((++f).valid());
}

static void
FillInEdgeZs(cBSPVertices& vertices, tMesh_Dynamic& mesh)
{
    tEdge_Dynamic e = mesh.beginEdge();
    do
    {
        e->_approximateZ = e.vertex()->dynamicPoint(vertices).z().asLong();
    }
    while(e.toNextInMesh());
}

void
CombineOperands(
        const std::vector<tMesh_Simple3D*>& primitives,
        bool keepDiscardedFaces,
        tMesh_Dynamic& result,
        cBSPVertices& vertices,
        cBSPStats& stats,
        cBitVector& isGroundSurface,
        cBitVector& isVertical,
        iProgressCallBack* progressCallBack
        )
{
    assertD(!primitives.empty());
    assertD(result.empty());

    cBSP3DManager manager(progressCallBack, keepDiscardedFaces);
    tSigned32 i;
    for(i = 0; i != SizeL(primitives); ++i)
    {
        manager.addOperand(*primitives[i]);
    }
    manager.buildTreeAndSplitFragments();

    result.swap(manager._result);

    assertF(ValidateMeshConnections(result));

    CombineOperands_PostBSP(manager._vertices, result, manager._splitters, progressCallBack);
    std::vector<int> projectionTypes(manager._splitters.size());
    GetSplitterProjectionTypes(manager._splitters, projectionTypes);
    Simplify_PostBSP(manager._vertices, 0, result, manager._splitters, projectionTypes, progressCallBack);

    SetupBitVectors(result, manager._splitters, isGroundSurface, isVertical, progressCallBack);
    FillInEdgeZs(manager._vertices, result);

    stats = manager._stats;
    vertices.swap(manager._vertices);
}

void
CombineOperandsAndExtractGround(
        const std::vector<tMesh_Simple3D*>& primitives,
        tMesh_Dynamic& result,
        cBSPVertices& vertices,
        cBSPStats& stats,
        const cDynamicFraction& maxStepHeight,
        const cDynamicFraction& maxSlopeFraction,
        bool approximate,
        cBitVector& isGroundSurface,
        cBitVector& isVertical,
        iProgressCallBack* progressCallBack
        )
{
    assertD(!primitives.empty());
    assertD(result.empty());

    cBSP3DManager manager(progressCallBack, false);
    tSigned32 i;
    for(i = 0; i != SizeL(primitives); ++i)
    {
        manager.addOperand(*primitives[i]);
    }
    manager.buildTreeAndSplitFragments();

    result.swap(manager._result);

    assertF(ValidateMeshConnections(result));

    CombineOperands_PostBSP(manager._vertices, result, manager._splitters, progressCallBack);
    std::vector<int> projectionTypes(manager._splitters.size());
    GetSplitterProjectionTypes(manager._splitters, projectionTypes);
    Simplify_PostBSP(manager._vertices, 0, result, manager._splitters, projectionTypes, progressCallBack);
    RemoveDownwardFacingFaces(result, manager._splitters);
    RemoveFacesAboveMaxSlope(result, manager._splitters, maxSlopeFraction, progressCallBack);
    ProcessVerticalFaces(manager._vertices, result, manager._splitters, projectionTypes, maxStepHeight, progressCallBack);

    if(approximate)
    {
        if(progressCallBack)
        {
            progressCallBack->updateProgress("Approximating", 0.f);
        }
        ApproximateZsAndCollapseVerticalFaces(manager._vertices, result, manager._splitters);
        ApproximateHorizontalComponents(manager._vertices, result);
        result.consolidateFaces();
        isGroundSurface.resize(result.faces(), true);
        isVertical.resize(result.faces(), false);
//        SplitVerticalDiscontinuities(result);
        assertD(result.faces() == result.faceSize());
        assertD(result.faces() >= isGroundSurface.size());
        isGroundSurface.resize(result.faces(), false);
        isVertical.resize(result.faces(), true);
        if(progressCallBack)
        {
            progressCallBack->updateProgress("Approximating (completed)", 1.f);
        }
    }
    else
    {
        SetupBitVectors(result, manager._splitters, isGroundSurface, isVertical, progressCallBack);
        FillInEdgeZs(manager._vertices, result);
    }

    stats = manager._stats;
    vertices.swap(manager._vertices);
}

void
CombineOperandsAndExtractGround(
        const std::vector<tMesh_Simple3D*>& primitives,
        iFacesCanBeCombinedCallback* facesCanBeCombinedCallback,
        tMesh_Dynamic& result,
        cBSPVertices& vertices,
        cBSPStats& stats,
        const cDynamicFraction& maxStepHeight,
        const cDynamicFraction& maxSlopeFraction,
        const cSourceDataLookup& sourceDataLookup,
        const char *const* options,
        iProgressCallBack* progressCallBack
        )
{
    assertD(!primitives.empty());
    assertD(result.empty());

    cBSP3DManager manager(progressCallBack, false);
    tSigned32 i;
    for(i = 0; i != SizeL(primitives); ++i)
    {
        manager.addOperand(*primitives[i]);
    }
    manager.buildTreeAndSplitFragments();

    stats = manager._stats;
    result.swap(manager._result);
    vertices.swap(manager._vertices);

    assertF(ValidateMeshConnections(result));

    CombineOperands_PostBSP(vertices, result, manager._splitters, progressCallBack);
    std::vector<int> projectionTypes(manager._splitters.size());
    GetSplitterProjectionTypes(manager._splitters, projectionTypes);
    Simplify_PostBSP(vertices, facesCanBeCombinedCallback, result, manager._splitters, projectionTypes, progressCallBack);
    RemoveDownwardFacingFaces(result, manager._splitters);
    RemoveFacesAboveMaxSlope(result, manager._splitters, maxSlopeFraction, progressCallBack);
    if(result.empty())
    {
        return;
    }
    ProcessVerticalFaces(vertices, result, manager._splitters, projectionTypes, maxStepHeight, progressCallBack);
    if(result.empty())
    {
        return;
    }

    if(ValueAsBoolWithDefault(options, "stripTerrainHeightDetail", false))
    {
        StripTerrainHeightDetail(result, vertices, ValueAsLongWithDefault(options, "numberOfTerrainLayers", 1), sourceDataLookup);
    }

    if(progressCallBack)
    {
        progressCallBack->updateProgress("Approximating", 0.f);
    }
    ApproximateZsAndCollapseVerticalFaces(vertices, result, manager._splitters);
    ApproximateHorizontalComponents(vertices, result);
    if(progressCallBack)
    {
        progressCallBack->updateProgress("Approximating (completed)", 1.f);
    }

    result.consolidate();
}
