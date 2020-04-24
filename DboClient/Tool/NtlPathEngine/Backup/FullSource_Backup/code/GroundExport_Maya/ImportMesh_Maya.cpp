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
#include "ImportMesh_Maya.h"
#include "MayaUtil.h"
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MIntArray.h>
#include <maya/MFnMesh.h>
#include <maya/MFnSet.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MDagPath.h>
#include <maya/MStatus.h>
#include <maya/MPlug.h>
#include "common/interface/Assert.h"
#include "common/interface/Error.h"
#include "i_pathengine.h"


bool
GetInitialShadingGroupSet(MObject& result)
{
    MStatus status;
    MItDependencyNodes it(MFn::kSet, &status);
    assertR(MStatus::kSuccess == status);
    for(; !it.isDone(); it.next())
    {
        result = it.thisNode();
        MFnDependencyNode dNode(result, &status);
        assertR(MStatus::kSuccess == status);
        MString name = dNode.name(&status);
        assertR(status == MStatus::kSuccess);
        const char* nameAsChar = name.asChar();
        if(strcmp(nameAsChar, "initialShadingGroup") == 0)
        {
            return true;
        }
    }
    return false;
}

MObject
ImportMesh_Maya(
        const iFaceVertexMesh& fvMesh,
        float scale
        )
{
    MStatus status;

    MPointArray vertices(fvMesh.vertices());
    MIntArray polygonCounts(fvMesh.faces(), 3);
    MIntArray polygonConnects(fvMesh.faces() * 3);

    for(tSigned32 i = 0; i != fvMesh.vertices(); ++i)
    {
        MPoint p(fvMesh.vertexY(i), fvMesh.vertexZ(i), fvMesh.vertexX(i));
        vertices[i] = p / scale;
    }
    for(tSigned32 i = 0; i != fvMesh.faces(); ++i)
    {
        for(tSigned32 j = 0; j != 3; ++j)
        {
            polygonConnects[i * 3 + j] = fvMesh.vertexIndex(i, 2 - j);
        }
    }

	MFnMesh mesh;
	MObject meshObj;
    meshObj = mesh.create(
            vertices.length(), polygonCounts.length(),
            vertices, polygonCounts, polygonConnects, 
            MObject::kNullObj, // no parent
            &status
            );
    assertR(status == MStatus::kSuccess);

	{
		MPlug plug = mesh.findPlug("doubleSided");
		plug.setValue(0);
	}

    MObject initialShadingGroupObject;
    if(GetInitialShadingGroupSet(initialShadingGroupObject))
    {
        MFnSet initialShadingGroupSet(initialShadingGroupObject, &status);
        assertR(status == MStatus::kSuccess);
        status = initialShadingGroupSet.addMember(meshObj);
        assertR(status == MStatus::kSuccess);
    }
    else
    {
        Error("NonFatal", "Could not find 'initialShadingGroup' set. Imported mesh will need shading applied manually");
    }

    for(i = 0; i < fvMesh.faces(); ++i)
    {
        tSigned32 sectionID = fvMesh.faceAttribute(i, PE_FaceAttribute_SectionID);
        if(fvMesh.faceAttribute(i, PE_FaceAttribute_MarksPortal) == 1)
        {
            status = mesh.setFaceColor(MColor(0.f, 0.f, 1.f), i);
            assertR(status == MStatus::kSuccess);
        }
        else if(fvMesh.faceAttribute(i, PE_FaceAttribute_MarksTerrainPortal) == 1)
        {
            status = mesh.setFaceColor(MColor(1.f, 0.f, 1.f), i);
            assertR(status == MStatus::kSuccess);
        }
        else if(fvMesh.faceAttribute(i, PE_FaceAttribute_MarksExternalShape) == 1)
        {
            status = mesh.setFaceColor(MColor(0.f, 0.f, 0.f), i);
            assertR(status == MStatus::kSuccess);
        }
        else if(fvMesh.faceAttribute(i, PE_FaceAttribute_MarksConnectingEdge) == 1)
        {
            status = mesh.setFaceColor(MColor(0.f, 1.f, 1.f), i);
            assertR(status == MStatus::kSuccess);
        }
        else if(sectionID >= 0 && sectionID <= 254)
        {
            float g = 1.f - static_cast<float>(sectionID) / 255.f;
            status = mesh.setFaceColor(MColor(0.f, g, 0.f), i);
            assertR(status == MStatus::kSuccess);
        }
    }

    SetName(meshObj, "ground_Imported#");

	status = mesh.updateSurface();
    assertR(status == MStatus::kSuccess);

    return meshObj;
}
