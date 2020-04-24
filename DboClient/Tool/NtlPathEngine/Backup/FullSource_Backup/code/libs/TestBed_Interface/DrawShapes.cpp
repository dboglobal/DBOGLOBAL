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
#include "DrawShapes.h"
#include "DrawMesh.h"
#include "libs/PathEngine_Core/interface/OverlappingPolygon.h"
#include "libs/PathEngine_Core/interface/LocalisedConvexShapeSet.h"
#include "libs/PathEngine_Core/interface/LocalisedConvexShape.h"
#include "libs/Mesh2D/interface/MeshCut.h"
#include "common/STL/list.h"

void
DrawShapes(
        const cLocalisedConvexShapeSet& shapes,
        iGraphicsInterface* gip,
        const cTranslationToOverlapped& translation
        )
{
    tSigned32 i;
    for(i = 0; i != shapes.size(); ++i)
    {
        const cLocalisedConvexShape& shape = shapes[i];
        tMeshCircuits circuits;
        {
            std::list<tFace> containedExternalFaces;
            CircuitsForOverlappingPolygon(
                    shape.centre().f,
                    shape.poly(),
                    containedExternalFaces,
                    circuits
                    );
        }
        DrawMeshCircuits(circuits, gip, translation);
    }
}
