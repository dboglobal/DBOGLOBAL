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
#include <maya/MFnDagNode.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MDagPath.h>
#include <maya/MPoint.h>
#include "AnchorsAndShapesFromMaya.h"
#include "libs/Content_Processing/interface/AnchorsAndPinnedShapes.h"
#include "libs/Geometry/interface/tPoint.h"
#include "platform_common/CoordinateRanges.h"
#include "common/interface/Error.h"
#include "common/AttributesBuilder.h"
#include <math.h>

static void
ProcessShape(const MDagPath& transformDagPath, cAnchorsAndPinnedShapes& toBuild, const char* name, const tPoint& origin, float scale, bool asNamedObstacle)
{
	MStatus status;
    MDagPath dagPath(transformDagPath);
    {
        unsigned int numberOfShapesBelow;
        status = dagPath.numberOfShapesDirectlyBelow(numberOfShapesBelow);
        assertR(status == MStatus::kSuccess);
        if(numberOfShapesBelow != 1)
        {
            cAttributesBuilder builder;
            builder.add("dagPath", transformDagPath.fullPathName().asChar());
            Error("NonFatal", "Bad DAG structure around an object marked up as a pinned shape, skipping.", builder.get());
            return;
        }
    }
    status = dagPath.extendToShapeDirectlyBelow(0);
    assertR(status == MStatus::kSuccess);

    MFnNurbsCurve nurbsCurve(dagPath, &status);
    if(status == MStatus::kFailure)
    {
        cAttributesBuilder builder;
        builder.add("dagPath", dagPath.fullPathName().asChar());
        Error("NonFatal", "Cannot construct MFnNurbsCurve from object markup up as a pinned shape, skipping.", builder.get());
        return;
    }

    MFnNurbsCurve::Form form = nurbsCurve.form(&status);
    assertR(status == MStatus::kSuccess);
    if(form != MFnNurbsCurve::kPeriodic)
    {
        cAttributesBuilder builder;
        builder.add("dagPath", dagPath.fullPathName().asChar());
        Error("NonFatal", "Pinned shape form must be 'periodic', skipping.", builder.get());
        return;
    }

    int degree = nurbsCurve.degree(&status);
    assertR(status == MStatus::kSuccess);
    if(degree != 1)
    {
        cAttributesBuilder builder;
        builder.add("dagPath", dagPath.fullPathName().asChar());
        Error("NonFatal", "Pinned shapes must be degree 1, skipping.", builder.get());
        return;
    }

    int numCVs = nurbsCurve.numCVs(&status);
    assertR(status == MStatus::kSuccess);
    int numSpans = nurbsCurve.numSpans(&status);
    assertR(status == MStatus::kSuccess);
    int numKnots = nurbsCurve.numKnots(&status);
    assertR(status == MStatus::kSuccess);

    if(numCVs != numSpans + 1)
    {
        cAttributesBuilder builder;
        builder.add("dagPath", dagPath.fullPathName().asChar());
        Error("NonFatal", "Bad pinned shape, numCVs != numSpans + 1, skipping. (Pinned shapes must be linear.)", builder.get());
        return;
    }
    if(numKnots != numSpans + 1)
    {
        cAttributesBuilder builder;
        builder.add("dagPath", dagPath.fullPathName().asChar());
        Error("NonFatal", "Bad pinned shape, numKnots != numSpans + 1, skipping. (Pinned shapes must be linear.)", builder.get());
        return;
    }

    if(numSpans < 3)
    {
        cAttributesBuilder builder;
        builder.add("dagPath", dagPath.fullPathName().asChar());
        Error("NonFatal", "Pinned shape has less than 3 points, shape will be skipped.", builder.get());
        return;
    }

    std::vector<cPinnedShape>& addTo = toBuild._pinnedShapes;

    addTo.push_back(cPinnedShape());
    if(asNamedObstacle)
    {
        addTo.back()._type = PE_PinnedShapeType_NamedObstacle;
    }
    else
    {
        addTo.back()._type = PE_PinnedShapeType_BurntIn;
    }
    addTo.back()._name = name;
    addTo.back()._anchorIndex = SizeL(toBuild._anchors);
    addTo.back()._coords.resize(numSpans * 2);
    for(int i = 0; i < numSpans; i++)
    {
        MPoint p;
        status = nurbsCurve.getCV(i, p, MSpace::kWorld);
        assertR(status == MStatus::kSuccess);
        addTo.back()._coords[i * 2 + 0] = static_cast<tSigned32>(p.z * scale) - origin.getX();
        addTo.back()._coords[i * 2 + 1] = static_cast<tSigned32>(p.x * scale) - origin.getY();
    }
}

static void
ProcessAnchor(const MDagPath& transformDagPath, const MDagPath& dagPath, cAnchorsAndPinnedShapes& toBuild, const char* name, float scale, bool resolvesToTerrain)
{
	MStatus status;
    MFnNurbsCurve nurbsCurve(dagPath, &status);
    if(status == MStatus::kFailure)
    {
        cAttributesBuilder builder;
        builder.add("dagPath", dagPath.fullPathName().asChar());
        Error("NonFatal", "Cannot construct MFnNurbsCurve from object with anchor prefix, skipping.", builder.get());
        return;
    }

    MFnNurbsCurve::Form form = nurbsCurve.form(&status);
    assertR(status == MStatus::kSuccess);
    if(form != MFnNurbsCurve::kOpen)
    {
        cAttributesBuilder builder;
        builder.add("dagPath", dagPath.fullPathName().asChar());
        Error("NonFatal", "Anchor form must be 'open', skipping.", builder.get());
        return;
    }

    int degree = nurbsCurve.degree(&status);
    assertR(status == MStatus::kSuccess);
    if(degree != 1)
    {
        cAttributesBuilder builder;
        builder.add("dagPath", dagPath.fullPathName().asChar());
        Error("NonFatal", "Anchors must be degree 1, skipping.", builder.get());
        return;
    }

    int numCVs = nurbsCurve.numCVs(&status);
    assertR(status == MStatus::kSuccess);
    int numSpans = nurbsCurve.numSpans(&status);
    assertR(status == MStatus::kSuccess);
    int numKnots = nurbsCurve.numKnots(&status);
    assertR(status == MStatus::kSuccess);

    if(numSpans != 1 && numSpans != 2)
    {
        cAttributesBuilder builder;
        builder.add("dagPath", dagPath.fullPathName().asChar());
        Error("NonFatal", "Anchor has number of spans not equal to 2 or 3, skipping.", builder.get());
        return;
    }

    if(numCVs != numSpans + 1)
    {
        cAttributesBuilder builder;
        builder.add("dagPath", dagPath.fullPathName().asChar());
        Error("NonFatal", "Bad anchor, numCVs != numSpans + 1, skipping. (Anchors must be linear curves with 2 or 3 spans.)", builder.get());
        return;
    }
    if(numKnots != numSpans + 1)
    {
        cAttributesBuilder builder;
        builder.add("dagPath", dagPath.fullPathName().asChar());
        Error("NonFatal", "Bad anchor, numKnots != numSpans + 1, skipping. (Anchors must be linear curves with 2 or 3 spans.)", builder.get());
        return;
    }

    MPoint points[3];
    status = nurbsCurve.getCV(0, points[0], MSpace::kWorld);
    assertR(status == MStatus::kSuccess);
    status = nurbsCurve.getCV(1, points[1], MSpace::kWorld);
    assertR(status == MStatus::kSuccess);
    if(numSpans == 2)
    {
        status = nurbsCurve.getCV(2, points[2], MSpace::kWorld);
        assertR(status == MStatus::kSuccess);
    }

//.... seems to be difficult to make these exactly identical
//.... could warn if not close to vertical, though
//    if(points[0].x != points[1].x || points[0].z != points[1].z)
//    {
//        cAttributesBuilder builder;
//        builder.add("dagPath", dagPath.fullPathName().asChar());
//        Error("NonFatal", "Bad anchor, first span is not exactly vertical, skipping.", builder.get());
//        return;
//    }

    tPoint origin;
    {
        tSigned32 originX = static_cast<tSigned32>(points[1].z * scale);
        tSigned32 originY = static_cast<tSigned32>(points[1].x * scale);
        if(originX < -WORLD_RANGE || originX > WORLD_RANGE || originY < -WORLD_RANGE || originY > WORLD_RANGE)
        {
            cAttributesBuilder builder;
            builder.add("dagPath", dagPath.fullPathName().asChar());
            Error("NonFatal", "Anchor is outside world range.", builder.get());
            return;
        }
        origin = tPoint(originX, originY, WORLD_RANGE);
    }

    unsigned childCount = transformDagPath.childCount(&status);
    assertR(status == MStatus::kSuccess);
    for(unsigned i = 0; i < childCount; i++)
    {
        MObject child = transformDagPath.child(i, &status);
        assertR(status == MStatus::kSuccess);
        MDagPath childDagPath = transformDagPath;
        status = childDagPath.push(child);
        assertR(status == MStatus::kSuccess);

        MFnDagNode dagNode(childDagPath, &status);
        assertR(status == MStatus::kSuccess);
        MString name = dagNode.name(&status);
        assertR(status == MStatus::kSuccess);
        const char* nameAsChar = name.asChar();
        if(strncmp(nameAsChar, "baseObstacle_", 13) == 0)
        {
            ProcessShape(childDagPath, toBuild, nameAsChar + 13, origin, scale, false);
        }
        else if(strncmp(nameAsChar, "obstacle_", 9) == 0)
        {
            ProcessShape(childDagPath, toBuild, nameAsChar + 9, origin, scale, true);
        }
	}    

    toBuild._anchors.push_back(cAnchor());
    if(name[0] == '_')
    {
      // 'anonymous' anchor
        toBuild._anchors.back()._name = "#";
        toBuild._anchors.back()._name.append(name + 1);
    }
    else
    {
        toBuild._anchors.back()._name = name;
    }

    toBuild._anchors.back()._x = origin.getX();
    toBuild._anchors.back()._y = origin.getY();
    toBuild._anchors.back()._zStart = static_cast<float>(points[0].y * scale);
    toBuild._anchors.back()._zEnd = static_cast<float>(points[1].y * scale);
    toBuild._anchors.back()._resolvesToTerrain = resolvesToTerrain;

    toBuild._anchors.back()._orientation = 0;
    if(numSpans == 2)
    {
        double axisX = points[2].z - points[1].z;
        double axisY = points[2].x - points[1].x;
        double orientationAtan2 = atan2(axisY, axisX);
        const double multiplier = 1048576.0f / 6.283185307f;
        tSigned32 orientationL = static_cast<tSigned32>(orientationAtan2 * multiplier);
        tSigned32 orientationPathEngine = ((262144 - orientationL) & 1048575);
        toBuild._anchors.back()._orientation = orientationPathEngine;
    }
}

void
BuildAnchorsAndShapesFromMaya(
    const std::vector<MDagPath>& toProcess,
    float scale,
    cAnchorsAndPinnedShapes& result
    )
{
    for(size_t i = 0; i < toProcess.size(); i++)
    {
	    MStatus status;
        MDagPath shapePath = toProcess[i];
        MDagPath transformPath = shapePath;
        status = transformPath.pop(1);
        assertR(status == MStatus::kSuccess);
        if(status != MStatus::kSuccess)
        {
            continue;
        }
        MFnDagNode dagNode(transformPath, &status);
        assertR(status == MStatus::kSuccess);
        if(status != MStatus::kSuccess)
        {
            continue;
        }
        MString name = dagNode.name(&status);
        assertR(status == MStatus::kSuccess);
        if(status != MStatus::kSuccess)
        {
            continue;
        }
        const char* nameAsChar = name.asChar();
        if(strncmp(nameAsChar, "anchor_", 7) == 0)
        {
            ProcessAnchor(transformPath, shapePath, result, nameAsChar + 7, scale, false);
        }
        else if(strncmp(nameAsChar, "terrainAnchor_", 14) == 0)
        {
            ProcessAnchor(transformPath, shapePath, result, nameAsChar + 14, scale, true);
        }
    }
}
