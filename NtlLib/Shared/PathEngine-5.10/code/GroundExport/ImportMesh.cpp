//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"

#include "Max.h"
#include "modstack.h"
#include "istdplug.h"
#include "linshape.h"
#include "utilapi.h"
#undef assert

#include "ImportMesh.h"
#include "libs/PathEngine_Interface/interface/cMesh.h"
#include "libs/PathEngine_Interface/interface/cShape.h"
#include "libs/Geometry/interface/ConvexPoly.h"
#include "libs/Geometry/interface/tPoint.h"
#include "platform_common/CoordinateRanges.h"
#include "common/interface/Assert.h"
#include "common/interface/Attributes.h"
#include "i_pathengine.h"

static void
SetFaceColours(
        Mesh& maxMesh,
        tSigned32 i,
        float r, float g, float b
        )
{
    maxMesh.vertCol[i * 3 + 0].x = r;
    maxMesh.vertCol[i * 3 + 0].y = g;
    maxMesh.vertCol[i * 3 + 0].z = b;
    maxMesh.vertCol[i * 3 + 1].x = r;
    maxMesh.vertCol[i * 3 + 1].y = g;
    maxMesh.vertCol[i * 3 + 1].z = b;
    maxMesh.vertCol[i * 3 + 2].x = r;
    maxMesh.vertCol[i * 3 + 2].y = g;
    maxMesh.vertCol[i * 3 + 2].z = b;
}

INode*
ImportMesh(
        Interface* ip,
        const iFaceVertexMesh& mesh,
        float scale
        )
{
    TriObject* triObject = CreateNewTriObject();
    assertD(triObject);
    Object* obj = triObject;
    IDerivedObject* derivedObject = CreateDerivedObject(obj);
    INode* node = ip->CreateObjectNode(derivedObject);
    
    // Name the node and make the name unique.
    TSTR name(_T("ground_Imported"));
    ip->MakeNameUnique(name);
    node->SetName(name);

    Mesh& maxMesh = triObject->GetMesh();

    int success;
    success = maxMesh.setNumVerts(mesh.vertices());
    assertD(success);
    tSigned32 i;
    for(i = 0; i < mesh.vertices(); ++i)
    {
        Point3 maxP;
        maxP.x = mesh.vertexX(i);
        maxP.y = mesh.vertexY(i);
        maxP.z = mesh.vertexZ(i);
        maxP /= scale;
        maxMesh.verts[i] = maxP;
    }

    success = maxMesh.setNumFaces(mesh.faces());
    assertD(success);
    bool vertexColoursRequired = false;
    for(i = 0; i < mesh.faces(); ++i)
    {
        tSigned32 j;
        for(j = 0; j < 3; ++j)
        {
            maxMesh.faces[i].v[2 - j] = mesh.vertexIndex(i, j);
        }
        maxMesh.faces[i].setMatID(mesh.faceAttribute(i, PE_FaceAttribute_SurfaceType));
        if(mesh.faceAttribute(i, PE_FaceAttribute_SectionID) != -1)
        {
            vertexColoursRequired = true;
        }
        if(mesh.faceAttribute(i, PE_FaceAttribute_MarksPortal) == 1)
        {
            vertexColoursRequired = true;
        }
        if(mesh.faceAttribute(i, PE_FaceAttribute_MarksTerrainPortal) == 1)
        {
            vertexColoursRequired = true;
        }
        if(mesh.faceAttribute(i, PE_FaceAttribute_MarksExternalShape) == 1)
        {
            vertexColoursRequired = true;
        }
        if(mesh.faceAttribute(i, PE_FaceAttribute_MarksConnectingEdge) == 1)
        {
            vertexColoursRequired = true;
        }
    }

    if(vertexColoursRequired)
    {
        success = maxMesh.setNumVCFaces(mesh.faces());
        assertD(success);
        success = maxMesh.setNumVertCol(mesh.faces() * 3);
        assertD(success);
        for(i = 0; i < mesh.faces(); ++i)
        {
            TVFace& colourFace = maxMesh.vcFace[i];
            colourFace.t[0] = i * 3 + 0;
            colourFace.t[1] = i * 3 + 1;
            colourFace.t[2] = i * 3 + 2;
            tSigned32 sectionID = mesh.faceAttribute(i, PE_FaceAttribute_SectionID);
            if(mesh.faceAttribute(i, PE_FaceAttribute_MarksPortal) == 1)
            {
                SetFaceColours(maxMesh, i, 0.f, 0.f, 1.f);
            }
            else if(mesh.faceAttribute(i, PE_FaceAttribute_MarksTerrainPortal) == 1)
            {
                SetFaceColours(maxMesh, i, 1.f, 0.f, 1.f);
            }
            else if(mesh.faceAttribute(i, PE_FaceAttribute_MarksExternalShape) == 1)
            {
                SetFaceColours(maxMesh, i, 0.f, 0.f, 0.f);
            }
            else if(mesh.faceAttribute(i, PE_FaceAttribute_MarksConnectingEdge) == 1)
            {
                SetFaceColours(maxMesh, i, 0.f, 1.f, 1.f);
            }
            else if(sectionID >= 0 && sectionID <= 254)
            {
                float g = 1.f - static_cast<float>(sectionID) / 255.f;
                SetFaceColours(maxMesh, i, 0.f, g, 0.f);
            }
            else
            {
            //... check and warn if sectionID is not in range
                SetFaceColours(maxMesh, i, 1.f, 1.f, 1.f);
            }
        }
    }

    // Redraw the viewports
    ip->RedrawViews(ip->GetTime());

    return node;
}

static INode*
CreateAnchor(
        Interface* ip,
        const tPoint& p,
        float startHeight, float endHeight,
        tSigned32 orientation, float orientationIndicatorLength,
        float scale,
        const char* baseName, bool makeNameUnique
        )
{
    LinearShape* linearShape = new LinearShape();
    assertD(linearShape);
    Object* obj = linearShape;
    IDerivedObject* derivedObject = CreateDerivedObject(obj);
    
    INode* node = ip->CreateObjectNode(derivedObject);

    // Name the node and make the name unique.
    TSTR name(_T(baseName));
    if(makeNameUnique)
    {
        ip->MakeNameUnique(name);
    }
    node->SetName(name);

    PolyShape& shape = linearShape->GetShape();

    PolyLine* line = shape.NewLine();
    PolyPt p1, p2;
    p1.p.x = p.getX();
    p1.p.y = p.getY();
    p1.p.z = startHeight;
    p2.p.x = p.getX();
    p2.p.y = p.getY();
    p2.p.z = endHeight;

    p1.p /= scale;
    p2.p /= scale;

    line->Append(p1);
    line->Append(p2);

    if(orientation)
    {
        float radians = static_cast<float>(orientation);
        radians *= 6.283185f;
        radians /= 1048576;
        float dx = sinf(radians) * orientationIndicatorLength;
        float dy = cosf(radians) * orientationIndicatorLength;
        PolyPt p3;
        p3.p.x = p.getX() + dx;
        p3.p.y = p.getY() + dy;
        p3.p.z = endHeight;
        line->Append(p3);
    }

    Matrix3 m3;
    m3.IdentityMatrix();
    node->Move(ip->GetTime(), m3, p1.p, FALSE, TRUE, PIV_PIVOT_ONLY, FALSE);

    return node;
}

static INode*
CreateShapePolygon(Interface* ip, const std::vector<tSigned32>& points, float height, float scale, const char* baseName, bool makeNameUnique)
{
    LinearShape* linearShape = new LinearShape();
    assertD(linearShape);
    Object* obj = linearShape;
    IDerivedObject* derivedObject = CreateDerivedObject(obj);
    
    INode* node = ip->CreateObjectNode(derivedObject);

    // Name the node and make the name unique.
    TSTR name(_T(baseName));
    if(makeNameUnique)
    {
        ip->MakeNameUnique(name);
    }
    node->SetName(name);

    PolyShape& shape = linearShape->GetShape();

    PolyLine* line = shape.NewLine();
    tSigned32 i;
    for(i = 0; i != SizeL(points) / 2; ++i)
    {
        PolyPt pp;
        pp.p.x = points[i * 2];
        pp.p.y = points[i * 2 + 1];
        pp.p.z = height;
        pp.p /= scale;
        line->Append(pp);
    }
    line->flags |= POLYLINE_CLOSED;
    return node;
}

//static tPoint
//AveragePoint(const std::vector<tPoint>& points)
//{
//    assertD(!points.empty());
//    double x = points[0].getX();
//    double y = points[0].getY();
//    double divisor = 1.0;
//    tSigned32 i;
//    for(i = 1; i != points.size(); ++i)
//    {
//        x += points[i].getX();
//        y += points[i].getY();
//        divisor += 1.0;
//    }
//    return tPoint(static_cast<tSigned32>(x / divisor), static_cast<tSigned32>(y / divisor), WORLD_RANGE);
//}

static void
ImportBaseObstacle(
        Interface* ip, INode* parent,
        const std::vector<tSigned32>& points, float height, float heightRange, float scale
        )
{
    INode* polygon = CreateShapePolygon(ip, points, height, scale, "baseObstacle_imported", true);
    parent->AttachChild(polygon);
}

static void
ImportNamedObstacle(
        Interface* ip, INode* parent,
        const std::vector<tSigned32>& points, float height, float heightRange, float scale,
        const char* name
        )
{
    std::string fullName("obstacle_");
    fullName.append(name);
    INode* polygon = CreateShapePolygon(ip, points, height, scale, fullName.c_str(), false);
    parent->AttachChild(polygon);
}

void
ImportAnchorsAndPinnedShapes(
        Interface* ip,
        INode* parent,
        const iAnchorsAndPinnedShapes& anchorsAndShapes,
        float anchorHeight,
        float scale
        )
{
    tSigned32 i;
    std::vector<INode*> anchorNodes(anchorsAndShapes.numberOfAnchors());
    for(i = 0; i != anchorsAndShapes.numberOfAnchors(); ++i)
    {
        tSigned32 x, y;
        float zStart, zEnd;
        anchorsAndShapes.anchor_HorizontalPosition(i, x, y);
        anchorsAndShapes.anchor_VerticalRange(i, zStart, zEnd);
        tPoint p(x, y, WORLD_RANGE);
        std::string name;
        if(anchorsAndShapes.anchor_Attribute(i, PE_AnchorAttribute_ResolvesToTerrain) == 1)
        {
            name.append("terrainAnchor_");
        }
        else
        {
            name.append("anchor_");
        }
        bool makeNameUnique;
        if(anchorsAndShapes.anchor_Name(i)[0])
        {
            name.append(anchorsAndShapes.anchor_Name(i));
            makeNameUnique = false;
        }
        else
        {
            name.append("#");
            makeNameUnique = true;
        }
        tSigned32 orientation = anchorsAndShapes.anchor_Orientation(i);
        anchorNodes[i] = CreateAnchor(ip, p, zStart, zEnd, orientation, anchorHeight / 3.f, scale, name.c_str(), makeNameUnique);
        parent->AttachChild(anchorNodes[i]);
    }
    for(i = 0; i != anchorsAndShapes.numberOfPinnedShapes(); ++i)
    {
        tSigned32 anchorIndex = anchorsAndShapes.pinnedShape_AnchorIndex(i);
        tSigned32 vertices = anchorsAndShapes.pinnedShape_NumberOfVertices(i);
        std::vector<tSigned32> points(vertices * 2);
        tSigned32 centreX, centreY;
        anchorsAndShapes.anchor_HorizontalPosition(anchorIndex, centreX, centreY);
        tSigned32 j;
        for(j = 0; j != vertices; ++j)
        {
            anchorsAndShapes.pinnedShape_Vertex(i, j, points[j * 2], points[j * 2 + 1]);
            points[j * 2] += centreX;
            points[j * 2 + 1] += centreY;
        }
        float zStart, zEnd;
        anchorsAndShapes.anchor_VerticalRange(anchorIndex, zStart, zEnd);
        float heightRange = zEnd - zStart;
        assertC(PE_PinnedShapeAttribute_Top == 0);
        assertC(PE_PinnedShapeType_Top == 1);
        if(anchorsAndShapes.pinnedShape_Attribute(i, PE_PinnedShapeAttribute_Type) == PE_PinnedShapeType_BurntIn)
        {
            ImportBaseObstacle(ip, anchorNodes[anchorIndex], points, zEnd, heightRange, scale);
        }
        else
        {
            assertD(anchorsAndShapes.pinnedShape_Attribute(i, PE_PinnedShapeAttribute_Type) == PE_PinnedShapeType_NamedObstacle);
            ImportNamedObstacle(ip, anchorNodes[anchorIndex], points, zEnd, heightRange, scale, anchorsAndShapes.pinnedShape_Name(i));
        }
    }
    ip->RedrawViews(ip->GetTime());
}
