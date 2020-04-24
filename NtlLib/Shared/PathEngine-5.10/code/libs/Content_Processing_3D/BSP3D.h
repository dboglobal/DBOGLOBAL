//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef BSP_3D_INCLUDED
#define BSP_3D_INCLUDED

#include "Plane.h"
#include "tMesh_Dynamic.h"
#include "tMesh_Simple3D.h"
#include "iFacesCanBeCombinedCallback.h"
#include "common/STL/vector.h"

class cPlane;
class cBitVector;
class iProgressCallBack;
template <class T> class cBSPTree;
class cBSP3DManager;
class cBSPStats;
class cSourceDataLookup;

void
CombineOperands_PostBSP(
        const cBSPVertices& vertices,
        tMesh_Dynamic& result,
        const std::vector<cPlane>& splitters,
        iProgressCallBack* progressCallBack
        );

void
GetSplitterProjectionTypes(
        const std::vector<cPlane>& splitters,
        std::vector<int>& projectionTypes
        );

void
Simplify_PostBSP(
        const cBSPVertices& vertices,
        iFacesCanBeCombinedCallback* facesCanBeCombinedCallback,
        tMesh_Dynamic& result,
        const std::vector<cPlane>& splitters,
        const std::vector<int>& projectionTypes,
        iProgressCallBack* progressCallBack
        );

void
RemoveDownwardFacingFaces(
        tMesh_Dynamic& mesh,
        const std::vector<cPlane>& splitters
        );

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
        );

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
        );

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
        );

#endif
