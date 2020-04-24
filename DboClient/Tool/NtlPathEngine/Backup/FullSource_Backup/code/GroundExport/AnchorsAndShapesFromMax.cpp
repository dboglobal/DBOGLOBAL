//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "Max.h"
#include "linshape.h"
#undef assert

#include "platform_common/Header.h"
#include "AnchorsAndShapesFromMax.h"
#include "libs/Content_Processing/interface/AnchorsAndPinnedShapes.h"
#include "common/interface/Error.h"
#include "common/AttributesBuilder.h"
#include <math.h>

static LinearShape*
GetLinearShapeFromObject(Object* object, bool& mustBeDeleted)
{
    mustBeDeleted = false;
    if(object->CanConvertToType(linearShapeClassID))
    { 
        LinearShape* result = (LinearShape*) object->ConvertToType(0, linearShapeClassID);
        if(object != result)
        {
            mustBeDeleted = true;
        }
        return result;
    }
    return 0;
}

static void
ProcessShape(Interface* ip, cAnchorsAndPinnedShapes& toBuild, INode* node, const char* name, const Point3& origin, bool asNamedObstacle, float scale, bool negateY)
{
    TimeValue time = ip->GetTime();
    Object* object = node->EvalWorldState(time).obj;
    bool mustBeDeleted;
    LinearShape* ls = GetLinearShapeFromObject(object, mustBeDeleted);

    if(!ls)
    {
        cAttributesBuilder builder;
        builder.add("node", node->GetName());
        Error("NonFatal", "Object cannot be converted to a linear shape, skipped.", builder.get());
        return;
    }

    PolyShape& ps = ls->GetShape();
    if(ps.numLines != 1)
    {
        cAttributesBuilder builder;
        builder.add("node", node->GetName());
        Error("NonFatal", "Object does not contain exactly 1 polyline, skipped.", builder.get());
        return;
    }

    PolyLine& pl = ps.lines[0];
    if(!(pl.flags & POLYLINE_CLOSED))
    {
        cAttributesBuilder builder;
        builder.add("node", node->GetName());
        Error("NonFatal", "Polyline is not closed, shape will be skipped.", builder.get());
        return;
    }
    if(pl.numPts < 3)
    {
        cAttributesBuilder builder;
        builder.add("node", node->GetName());
        Error("NonFatal", "Polyline has less than 3 points, shape will be skipped.", builder.get());
        return;
    }

	Matrix3 transformation = node->GetObjectTM(time);

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
    addTo.back()._coords.resize(pl.numPts * 2);
    tSigned32 i;
    for(i = 0; i < pl.numPts; i++)
    {
        Point3 transformed = pl.pts[i].p * transformation;
        if(negateY)
        {
            transformed.y = -transformed.y;
        }
        transformed *= scale;
        Point3 relative = transformed - origin;
        addTo.back()._coords[i * 2 + 0] = relative.x;
        addTo.back()._coords[i * 2 + 1] = relative.y;
    }

    if(mustBeDeleted)
    {
        ls->DeleteMe();
    }
}

static void
ProcessAnchor(Interface* ip, cAnchorsAndPinnedShapes& toBuild, INode* node, const char* name, float scale, bool negateY, bool resolvesToTerrain)
{
    TimeValue time = ip->GetTime();
	Matrix3 tm = node->GetNodeTM(time);
	Point3 origin = tm.GetTrans();
    if(negateY)
    {
        origin.y = -origin.y;
    }
    origin *= scale;
    // note that tm is not scaled, and is just used subsequently for getting orientation

    for (int c = 0; c < node->NumberOfChildren(); c++)
    {
        INode* child = node->GetChildNode(c);
        const char* name = child->GetName();
        if(strncmp(name, "shape_", 6) == 0)
        {
            ProcessShape(ip, toBuild, child, name + 6, origin, false, scale, negateY);
        }
        else
        if(strncmp(name, "baseObstacle_", 13) == 0)
        {
            ProcessShape(ip, toBuild, child, name + 13, origin, false, scale, negateY);
        }
        else
        if(strncmp(name, "obstacle_", 9) == 0)
        {
            ProcessShape(ip, toBuild, child, name + 9, origin, true, scale, negateY);
        }
	}
    
	ViewExp *vpt = ip->GetViewport(NULL);
    Box3 bb;
    Object* object = node->EvalWorldState(time).obj;
    object->GetWorldBoundBox(time, node, vpt, bb);    

    toBuild._anchors.push_back(cAnchor());
    toBuild._anchors.back()._name = name;
    toBuild._anchors.back()._x = origin.x;
    toBuild._anchors.back()._y = origin.y;
    toBuild._anchors.back()._zStart = bb.pmin.z * scale;
    toBuild._anchors.back()._zEnd = bb.pmax.z * scale;
    toBuild._anchors.back()._resolvesToTerrain = resolvesToTerrain;

    Point3 xAxis = tm.GetRow(0);
    if(negateY)
    {
        xAxis.y = -xAxis.y;
    }
    double orientationAtan2 = atan2(xAxis.y, xAxis.x);
    const double multiplier = 1048576.0f / 6.283185307f;
    tSigned32 orientationL = static_cast<tSigned32>(orientationAtan2 * multiplier);
    tSigned32 orientationPathEngine = ((262144 - orientationL) & 1048575);
    toBuild._anchors.back()._orientation = orientationPathEngine;
}

static void
BuildAnchorsAndShapes_Recursive(
        Interface* ip,
        const char* prefix, tSigned32 prefixLength,
        cAnchorsAndPinnedShapes& toBuild,
        INode* node,
        float scale, bool negateY
        )
{
    const char* name = node->GetName();
    if(strncmp(name, prefix, prefixLength) == 0)
    {
	    for(int c = 0; c < node->NumberOfChildren(); c++)
        {
            INode* child = node->GetChildNode(c);
            const char* name = child->GetName();
            if(strncmp(name, "anchor_", 7) == 0)
            {
                ProcessAnchor(ip, toBuild, child, name + 7, scale, negateY, false);
            }
            else if(strncmp(name, "terrainAnchor_", 14) == 0)
            {
                ProcessAnchor(ip, toBuild, child, name + 14, scale, negateY, true);
            }
        }
    }
    tSigned32 i;
    for(i = 0; i < node->NumberOfChildren(); ++i)
    {
        BuildAnchorsAndShapes_Recursive(ip,
                    prefix, prefixLength,
                    toBuild,
                    node->GetChildNode(i),
                    scale, negateY
                    );
    }
}

void
BuildAnchorsAndShapesFromMax(Interface* ip, const char* prefix, cAnchorsAndPinnedShapes& toBuild, float scale, bool negateY)
{
    if(prefix)
    {
        tSigned32 prefixLength = static_cast<tSigned32>(strlen(prefix));
        INode* root = ip->GetRootNode();
        BuildAnchorsAndShapes_Recursive(ip,
                    prefix, prefixLength,
                    toBuild,
                    root,
                    scale, negateY
                    );
    }
    else
    {
        int numberSelected = ip->GetSelNodeCount();
        TimeValue time = ip->GetTime();

        tSigned32 i;
        for(i = 0; i < numberSelected; i++)
        {
            INode* node = ip->GetSelNode(i);

	        for (int c = 0; c < node->NumberOfChildren(); c++)
            {
                INode* child = node->GetChildNode(c);
                const char* name = child->GetName();
                if(strncmp(name, "anchor_", 7) == 0)
                {
                    ProcessAnchor(ip, toBuild, child, name + 7, scale, negateY, false);
                }
                else if(strncmp(name, "terrainAnchor_", 14) == 0)
                {
                    ProcessAnchor(ip, toBuild, child, name + 14, scale, negateY, true);
                }
	        }
        }
    }
}


static void
BuildAnchorsAndShapes_AllInScene_Recursive(
        Interface* ip,
        cAnchorsAndPinnedShapes& toBuild,
        INode* node,
        float scale, bool negateY
        )
{
    const char* name = node->GetName();
    if(strncmp(name, "anchor_", 7) == 0)
    {
        ProcessAnchor(ip, toBuild, node, name + 7, scale, negateY, false);
    }
    else if(strncmp(name, "terrainAnchor_", 14) == 0)
    {
        ProcessAnchor(ip, toBuild, node, name + 14, scale, negateY, true);
    }
    tSigned32 i;
    for(i = 0; i < node->NumberOfChildren(); ++i)
    {
        BuildAnchorsAndShapes_AllInScene_Recursive(ip,
                    toBuild,
                    node->GetChildNode(i),
                    scale, negateY
                    );
    }
}

void
BuildAnchorsAndShapesFromMax_AllInScene(Interface* ip, cAnchorsAndPinnedShapes& toBuild, float scale, bool negateY)
{
    INode* root = ip->GetRootNode();
    BuildAnchorsAndShapes_AllInScene_Recursive(ip,
                toBuild,
                root,
                scale, negateY
                );
}

