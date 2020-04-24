//**********************************************************************
//
// Copyright (c) 2004-2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "ResolveAnchors.h"
#include "common/interface/Error.h"
#include "common/AttributesBuilder.h"
#include "i_pathengine.h"

void
ResolveAnchors(
        const iMesh& mesh,
        const iAnchorsAndPinnedShapes& anchorsAndShapes,
        tSigned32 numberOfTerrainLayers,
        std::vector<cPosition>& anchorPositions
        )
{
    anchorPositions.resize(anchorsAndShapes.numberOfAnchors());
    tSigned32 i;
    for(i = 0; i < anchorsAndShapes.numberOfAnchors(); i++)
    {
        cPosition& p = anchorPositions[i];
        tSigned32 x, y;
        float zStart, zEnd;
        anchorsAndShapes.anchor_HorizontalPosition(i, x, y);
        anchorsAndShapes.anchor_VerticalRange(i, zStart, zEnd);
        if(anchorsAndShapes.anchor_Attribute(i, PE_AnchorAttribute_ResolvesToTerrain) == 1)
        {
            p = mesh.positionInSection(0, x, y);
            if(p.cell == -1)
            {
                cAttributesBuilder builder;
                builder.add("anchorName", anchorsAndShapes.anchor_Name(i));
                builder.add("anchorX", x);
                builder.add("anchorY", y);
                builder.add("_resolvesToTerrain", "1");
                Error("NonFatal", "Failed to resolve position for anchor,"
                    " the anchor will be skipped and shapes at this anchor will not be placed.", builder.get());
            }
        }
        else
        {
            tSigned32 coords[3];
            coords[0] = x;
            coords[1] = y;
            coords[2] = static_cast<tSigned32>(zEnd);
            p = mesh.positionFor3DPoint_ExcludeTerrain(coords, numberOfTerrainLayers);
            if(p.cell != -1)
            {
                tSigned32 height = mesh.heightAtPosition(p);
                if(height < zStart)
                {
                    p.cell = -1;
                }
            }
            if(p.cell == -1)
            {
                cAttributesBuilder builder;
                builder.add("anchorName", anchorsAndShapes.anchor_Name(i));
                builder.add("anchorX", x);
                builder.add("anchorY", y);
                Error("NonFatal", "Failed to resolve position for anchor,"
                    " the anchor will be skipped and shapes at this anchor will not be placed.", builder.get());
            }
        }
    }
}
