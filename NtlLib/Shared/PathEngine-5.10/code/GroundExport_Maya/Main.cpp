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
#include "Exporter.h"
#include "GetBufferFromResource.h"
#include "FormatVersionString.h"
#include "MayaErrorHandler.h"
#include "resource.h"
#include <maya/MGlobal.h>
#include <maya/MFnPlugin.h>
#include "libs/PathEngine_Interface/interface/Singletons.h"
#include "common/MemoryTracking.h"
#include "i_pathengine.h"

MStatus
initializePlugin(MObject obj)
{
    nSingletons::init_StandAlone();
    nSingletons::pathEngineI().setErrorHandler(&cMayaErrorHandler::refInstance());

	MStatus status;

    std::vector<char> script;
    bool gotResource = GetBufferFromResource(MhInstPlugin, "BIN", UI_SCRIPT, script);
    if(!gotResource)
    {
    	MGlobal::displayInfo("Warning: Failed to load UI script from resource.");
    }

    script.push_back(0);
    MString command = &script.front();
    status = MGlobal::executeCommand(command, true);
    if(!status)
    {
		status.perror("Executing UI script");
    }

	MFnPlugin plugin(obj, "PathEngine", FormatVersionString(nSingletons::pathEngineI()).c_str(), "Any");

	status = plugin.registerFileTranslator("PathEngine data",
											"",
											cExporter::creator,
											"GroundExportOpts",
											"",
											true);
	if(!status)
    {
		status.perror("registerFileTranslator");
		return status;
	}

    return status;
}

MStatus
uninitializePlugin(MObject obj) 
{    
    MStatus status;
	MFnPlugin plugin(obj);

	status = plugin.deregisterFileTranslator("PathEngine data");
	if (!status)
    {
		status.perror("deregisterFileTranslator");
	}

    nSingletons::shutDown_StandAlone();
	return status;
}
