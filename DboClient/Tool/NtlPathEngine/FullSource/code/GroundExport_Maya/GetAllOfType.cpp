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
#include "GetAllOfType.h"
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MItDag.h>
#include <maya/MItSelectionList.h>
#include <maya/MPlug.h>
#include <maya/MGlobal.h>
#include "common/interface/Assert.h"

static bool
IsVisible(MFnDagNode& fnDag, MStatus& status) 
{
    if(fnDag.isIntermediateObject())
    {
        return false;
    }
    MPlug visPlug = fnDag.findPlug("visibility", &status);
    assertR(MStatus::kSuccess == status);
    bool visible;
    status = visPlug.getValue(visible);
    assertR(MStatus::kSuccess == status);
    return visible;
}

void 
GetAllOfType(MFn::Type type, std::vector<MDagPath>& appendTo)
{
    MStatus status;
    MItDag itDag(MItDag::kDepthFirst, type, &status);
    assertR(MStatus::kSuccess == status);
    for(; !itDag.isDone(); itDag.next())
    {
        MDagPath dagPath;
        status = itDag.getPath(dagPath);
        assertR(MStatus::kSuccess == status);
        MFnDagNode visTester(dagPath);
        if(IsVisible(visTester, status))
        {
            appendTo.push_back(dagPath);
        }
        assertR(MStatus::kSuccess == status);
    }
}

void
GetAllSelectedOfType(MFn::Type type, std::vector<MDagPath>& appendTo)
{
    MStatus status;
    MSelectionList selectionList;
    status = MGlobal::getActiveSelectionList(selectionList);
    assertR(MStatus::kSuccess == status);
    MItSelectionList itSelectionList(selectionList, type, &status);    
    assertR(MStatus::kSuccess == status);
    for(itSelectionList.reset(); !itSelectionList.isDone(); itSelectionList.next())
    {
        MDagPath dagPath;
        status = itSelectionList.getDagPath(dagPath);
        assertR(MStatus::kSuccess == status);
        appendTo.push_back(dagPath);
    }
}
