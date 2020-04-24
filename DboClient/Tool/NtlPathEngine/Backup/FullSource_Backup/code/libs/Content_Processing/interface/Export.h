//**********************************************************************
//
// Copyright (c) 2004-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef EXPORT_INCLUDED
#define EXPORT_INCLUDED

#include "libs/Mesh3D/interface/tMesh_3D_Header.h"
#include "common/STL/vector.h"

class iFaceVertexMesh;
class iAnchorsAndPinnedShapes;
class iMesh;
class cPathEngine;
class iPathEngine;

class cExportOptions
{
public:

    bool tokenise;
    bool addMapping;
    bool filterFacesBySlope;
    float maxSlope;
    bool explicitlyManagedEdgeConnections;

    cExportOptions()
    {
        tokenise = false;
        addMapping = false;
        filterFacesBySlope = false;
        explicitlyManagedEdgeConnections = false;
        maxSlope = 1.f;
    }
};

bool
PerformContentProcessing(
        const std::vector<const iFaceVertexMesh*>& meshes,
        const cExportOptions& exportOptions,
        bool optimise, float horizontalThreshold, float verticalThreshold,
        tSigned32 numberOfTerrainLayers,
        const char *const* options,
        tMesh_3D& result,
        std::vector<tSigned32>*& surfaceTypes,
        std::vector<tSigned32>*& sectionIDs,
        std::vector<tSigned32>*& userData
        );

bool
PrevalidateAnchorsAndPinnedShapes(iPathEngine& pathEngine, const iAnchorsAndPinnedShapes& anchorsAndShapes);

void
AddAnchorsAndPinnedShapes(iPathEngine& pathEngine, iMesh* mesh, const iAnchorsAndPinnedShapes& anchorsAndShapes, const char* idPrefix, tSigned32 numberOfTerrainLayers);

#endif
