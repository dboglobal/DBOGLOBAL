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
#include "MayaUtil.h"
#include <maya/MFnDependencyNode.h>
#include <maya/MStatus.h>
#include "common/interface/Assert.h"

void
SetName(MObject& object, const char* name)
{
    MStatus status;
    MFnDependencyNode node(object, &status);
    assertR(MStatus::kSuccess == status);
    node.setName(MString(name), &status);
    assertR(MStatus::kSuccess == status);
}
