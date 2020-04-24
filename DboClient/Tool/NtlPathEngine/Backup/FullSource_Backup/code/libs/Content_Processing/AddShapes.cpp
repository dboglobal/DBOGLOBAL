//**********************************************************************
//
// Copyright (c) 2004-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "AddShapes.h"
#include "common/interface/Error.h"
#include "common/AttributesBuilder.h"
#include "i_pathengine.h"

static void
GetVertexArray(const iAnchorsAndPinnedShapes& anchorsAndShapes, tSigned32 i, std::vector<tSigned32>& result)
{
    tSigned32 vertices = anchorsAndShapes.pinnedShape_NumberOfVertices(i);
    result.resize(vertices * 2);
    tSigned32 j;
    for(j = 0; j != vertices; ++j)
    {
        anchorsAndShapes.pinnedShape_Vertex(i, j, result[j * 2], result[j * 2 + 1]);
    }
}
static void
GetVertexArray_Reversed(const iAnchorsAndPinnedShapes& anchorsAndShapes, tSigned32 i, std::vector<tSigned32>& result)
{
    tSigned32 vertices = anchorsAndShapes.pinnedShape_NumberOfVertices(i);
    result.resize(vertices * 2);
    tSigned32 j;
    for(j = 0; j != vertices; ++j)
    {
        anchorsAndShapes.pinnedShape_Vertex(i, vertices - 1 - j, result[j * 2], result[j * 2 + 1]);
    }
}
static void
AddShapeOrigin(std::vector<tSigned32>& result, tSigned32 originX, tSigned32 originY)
{
    tSigned32 vertices = SizeL(result) / 2;
    assertD(SizeL(result) == vertices * 2);
    tSigned32 j;
    for(j = 0; j != vertices; ++j)
    {
        result[j * 2] += originX;
        result[j * 2 + 1] += originY;
    }
}

static iShape*
ResolveShape(iPathEngine& pathEngine, const iAnchorsAndPinnedShapes& anchorsAndShapes, tSigned32 i)
{
    std::vector<tSigned32> coords;
    GetVertexArray(anchorsAndShapes, i, coords);
    if(pathEngine.shapeIsValid(SizeL(coords) / 2, &coords.front()))
    {
        return pathEngine.newShape(SizeL(coords) / 2, &coords.front());
    }
    GetVertexArray_Reversed(anchorsAndShapes, i, coords);
    if(pathEngine.shapeIsValid(SizeL(coords) / 2, &coords.front()))
    {
        return pathEngine.newShape(SizeL(coords) / 2, &coords.front());
    }
    return 0;
}

static bool
ShapeIsValid(iPathEngine& pathEngine, const iAnchorsAndPinnedShapes& anchorsAndShapes, tSigned32 i)
{
    std::vector<tSigned32> coords;
    tSigned32 anchorIndex, originX, originY;
    GetVertexArray(anchorsAndShapes, i, coords);
    if(pathEngine.shapeIsValid(SizeL(coords) / 2, &coords.front()))
    {
        return true;
    }
    anchorIndex = anchorsAndShapes.pinnedShape_AnchorIndex(i);
    anchorsAndShapes.anchor_HorizontalPosition(anchorIndex, originX, originY);
    AddShapeOrigin(coords, originX, originY);
    if(pathEngine.largeStaticObstacleShapeIsValid(SizeL(coords) / 2, &coords.front(), originX, originY))
    {
        return true;
    }
    GetVertexArray_Reversed(anchorsAndShapes, i, coords);
    if(pathEngine.shapeIsValid(SizeL(coords) / 2, &coords.front()))
    {
        return true;
    }
    AddShapeOrigin(coords, originX, originY);
    if(pathEngine.largeStaticObstacleShapeIsValid(SizeL(coords) / 2, &coords.front(), originX, originY))
    {
        return true;
    }
    return false;
}

bool
PrevalidatePinnedShapes(
        iPathEngine& pathEngine,
        const iAnchorsAndPinnedShapes& anchorsAndShapes
        )
{
    for(tSigned32 i = 0; i < anchorsAndShapes.numberOfPinnedShapes(); i++)
    {
        if(!ShapeIsValid(pathEngine, anchorsAndShapes, i))
        {
            cAttributesBuilder builder;
            builder.add_CopyString("shapeName", anchorsAndShapes.pinnedShape_Name(i));
            Error("NonFatal", "Pinned shape is not valid.", builder.get());
            return false;
        }
    }
    return true;
}

void
AddPinnedShapes(
        iPathEngine& pathEngine,
        const iAnchorsAndPinnedShapes& anchorsAndShapes,
        const std::vector<cPosition>& anchorPositions,
        iMesh* mesh
        )
{
    iCollisionContext* context = mesh->newContext();
    for(tSigned32 i = 0; i < anchorsAndShapes.numberOfPinnedShapes(); i++)
    {
        cPosition p = anchorPositions[anchorsAndShapes.pinnedShape_AnchorIndex(i)];
        if(p.cell == -1)
        {
        // already emitted an error when resolving anchors
            continue;
        }
        iShape* shape = ResolveShape(pathEngine, anchorsAndShapes, i);
        iAgent* agent;
        if(shape)
        {
            agent = mesh->placeAgent(shape, p);
        }
        else if(ShapeIsValid(pathEngine, anchorsAndShapes, i))
        {
            std::vector<tSigned32> coords;
            GetVertexArray(anchorsAndShapes, i, coords);
            tSigned32 anchorIndex, originX, originY;
            anchorIndex = anchorsAndShapes.pinnedShape_AnchorIndex(i);
            anchorsAndShapes.anchor_HorizontalPosition(anchorIndex, originX, originY);
            assertR(originX == p.x);
            assertR(originY == p.y);
            AddShapeOrigin(coords, originX, originY);
            if(!pathEngine.largeStaticObstacleShapeIsValid(SizeL(coords) / 2, &coords.front(), originX, originY))
            {
                GetVertexArray_Reversed(anchorsAndShapes, i, coords);
                AddShapeOrigin(coords, originX, originY);
                assertR(pathEngine.largeStaticObstacleShapeIsValid(SizeL(coords) / 2, &coords.front(), originX, originY));
            }
            agent = mesh->placeLargeStaticObstacle(SizeL(coords) / 2, &coords.front(), p);
        }
        else
        {
            cAttributesBuilder builder;
            builder.add_CopyString("shapeName", anchorsAndShapes.pinnedShape_Name(i));
            builder.add("_type", anchorsAndShapes.pinnedShape_Attribute(i, PE_PinnedShapeAttribute_Type));
            Error("NonFatal", "Pinned shape is not valid. Obstacle will not be placed.", builder.get());
            continue;
        }

        assertC(PE_PinnedShapeAttribute_Top == 0);
        assertC(PE_PinnedShapeType_Top == 1);
        if(anchorsAndShapes.pinnedShape_Attribute(i, PE_PinnedShapeAttribute_Type) == PE_PinnedShapeType_BurntIn)
        {
            context->addAgent(agent);
        }
        else
        {
            assertD(anchorsAndShapes.pinnedShape_Attribute(i, PE_PinnedShapeAttribute_Type) == PE_PinnedShapeType_NamedObstacle);
            mesh->storeNamedObstacle(anchorsAndShapes.pinnedShape_Name(i), agent);
            delete agent;
        }
    }

    mesh->burnContextIntoMesh(context);

    while(context->getNumberOfAgents())
    {
        delete context->getAgent(0);
    }
    delete context;
}
