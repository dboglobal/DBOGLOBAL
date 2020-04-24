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
#include "ImportAnchorsAndShapes_Maya.h"
#include "MayaUtil.h"
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MFnTransform.h>
#include <maya/MFnDagNode.h>
#include <maya/MStatus.h>
#include <maya/MVector.h>
#include <maya/MDagPath.h>
#include "common/interface/Assert.h"
#include "common/interface/Error.h"
#include "common/STL/vector.h"
#include "i_pathengine.h"
#include <math.h>

static MPoint
AsMPoint(double x, double y, float z, float scale)
{
    MPoint p(y, z, x);
    return p / scale;
}

static MObject
ImportAnchor(
        const iAnchorsAndPinnedShapes& anchorsAndShapes,
        tSigned32 i,
        float scale,
        float anchorDirectionIndicatorLength
        )
{
    MStatus status;

    tSigned32 vertices = 2;
    tSigned32 anchorOrientation = anchorsAndShapes.anchor_Orientation(i);
    if(anchorOrientation)
    {
        vertices = 3;
    }

    MPointArray controlVertices(vertices);
    MDoubleArray knotSequences(vertices);

    {
        float zStart, zEnd;
        anchorsAndShapes.anchor_VerticalRange(i, zStart, zEnd);
        controlVertices[0] = AsMPoint(0, 0, 0, scale);
        controlVertices[1] = AsMPoint(0, 0, zEnd - zStart, scale);
        knotSequences[0] = 0.0;
        knotSequences[1] = 1.0;
        if(anchorOrientation)
        {
            float radians = static_cast<float>(anchorOrientation);
            radians *= 6.283185f;
            radians /= 1048576;
            float dx = sinf(radians) * anchorDirectionIndicatorLength;
            float dy = cosf(radians) * anchorDirectionIndicatorLength;
            controlVertices[2] = AsMPoint(dx, dy, zEnd - zStart, scale);
            knotSequences[2] = 2.0;
        }
    }
    
	MFnNurbsCurve curve;
	MObject curveObj;
    curveObj = curve.create(
            controlVertices, knotSequences,
            1, curve.kOpen, false, true,
            MObject::kNullObj, // no parent
            &status
            );
    assertR(status == MStatus::kSuccess);

    std::string name;
    if(anchorsAndShapes.anchor_Attribute(i, PE_AnchorAttribute_ResolvesToTerrain) == 1)
    {
        name.append("terrainAnchor_");
    }
    else
    {
        name.append("anchor_");
    }

    if(anchorsAndShapes.anchor_Name(i)[0])
    {
        if(anchorsAndShapes.anchor_Name(i)[0] == '#')
        {
        // new form of anonymous anchor
            name.append("_");
            name.append(anchorsAndShapes.anchor_Name(i) + 1);
        }
        else
        {
            name.append(anchorsAndShapes.anchor_Name(i));
        }
    }
    else
    {
    // legacy form of anonymous anchor
        name.append("_");
    }
    SetName(curveObj, name.c_str());

    assertC(PE_AnchorAttribute_Top == 0); // deal with any new attributes as they are added

    {
        MDagPath pathToTransform;
        status = MDagPath::getAPathTo(curveObj, pathToTransform);
        assertR(status == MStatus::kSuccess);
        MFnTransform transform(pathToTransform, &status);
        assertR(status == MStatus::kSuccess);
        tSigned32 x, y;
        anchorsAndShapes.anchor_HorizontalPosition(i, x, y);
        float zStart, zEnd;
        anchorsAndShapes.anchor_VerticalRange(i, zStart, zEnd);
        MVector originTranslation(AsMPoint(x, y, zStart, scale));
        status = transform.setTranslation(originTranslation, MSpace::kWorld);
        assertR(status == MStatus::kSuccess);
    }

//    status = curve.updateCurve();
//    assertR(status == MStatus::kSuccess);
//... why does this fail? Is this necessary?

    return curveObj;
}

static MObject
ImportShape(
        const iAnchorsAndPinnedShapes& anchorsAndShapes,
        tSigned32 i,
        float scale
        )
{
    MStatus status;

    tSigned32 vertices = anchorsAndShapes.pinnedShape_NumberOfVertices(i);

    MPointArray controlVertices(vertices + 1);
    MDoubleArray knotSequences(vertices + 1);

    float zOffset;
    {
        tSigned32 anchorIndex = anchorsAndShapes.pinnedShape_AnchorIndex(i);
        float zStart, zEnd;
        anchorsAndShapes.anchor_VerticalRange(anchorIndex, zStart, zEnd);
        zOffset = zEnd - zStart;
    }

    for(tSigned32 j = 0; j != vertices; ++j)
    {
        tSigned32 x, y;
        anchorsAndShapes.pinnedShape_Vertex(i, j, x, y);
        controlVertices[j] = AsMPoint(x, y, zOffset, scale);
        knotSequences[j] = j;
    }
    controlVertices[j] = controlVertices[0];
    knotSequences[j] = j;
    
	MFnNurbsCurve curve;
	MObject curveObj;
    curveObj = curve.create(
            controlVertices, knotSequences,
            1, curve.kPeriodic, false, true,
            MObject::kNullObj, // no parent
            &status
            );
    assertR(status == MStatus::kSuccess);

    std::string name;
    if(anchorsAndShapes.pinnedShape_Attribute(i, PE_PinnedShapeAttribute_Type) == PE_PinnedShapeType_BurntIn)
    {
        name.append("obstacle_");
        name.append(anchorsAndShapes.pinnedShape_Name(i));
    }
    else
    {
        assertD(anchorsAndShapes.pinnedShape_Attribute(i, PE_PinnedShapeAttribute_Type) == PE_PinnedShapeType_NamedObstacle);
        name.append("baseObstacle_#");
    }
    assertC(PE_PinnedShapeType_Top == 1); // deal with any new types as they are added
    SetName(curveObj, name.c_str());

    assertC(PE_PinnedShapeAttribute_Top == 0); // deal with any new attributes as they are added

//    status = curve.updateCurve();
//    assertR(status == MStatus::kSuccess);
//... why does this fail? Is this necessary?

    return curveObj;
}

void
ImportAnchorsAndShapes_Maya(
        const iAnchorsAndPinnedShapes& anchorsAndShapes,
        float scale,
        MObject& linkUnder,
        float anchorDirectionIndicatorLength
        )
{
    MStatus status;
    MFnDagNode linkUnderNode(linkUnder, &status);
    assertR(MStatus::kSuccess == status);
    std::vector<MObject> anchorObjects(anchorsAndShapes.numberOfAnchors());
    tSigned32 i;
    for(i = 0; i != anchorsAndShapes.numberOfAnchors(); ++i)
    {
        anchorObjects[i] = ImportAnchor(anchorsAndShapes, i, scale, anchorDirectionIndicatorLength);
        status = linkUnderNode.addChild(anchorObjects[i], MFnDagNode::kNextPos, false);
        assertR(MStatus::kSuccess == status);
    }
    for(i = 0; i != anchorsAndShapes.numberOfPinnedShapes(); ++i)
    {
        tSigned32 anchorIndex = anchorsAndShapes.pinnedShape_AnchorIndex(i);
        MObject shape = ImportShape(anchorsAndShapes, i, scale);
        MFnDagNode anchorNode(anchorObjects[anchorIndex], &status);
        assertR(MStatus::kSuccess == status);
        status = anchorNode.addChild(shape, MFnDagNode::kNextPos, false);
        assertR(MStatus::kSuccess == status);
    }
}
