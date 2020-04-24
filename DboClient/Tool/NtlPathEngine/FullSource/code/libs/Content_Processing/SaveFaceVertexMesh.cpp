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
#include "interface/SaveFaceVertexMesh.h"
#include "libs/Geometry/interface/GeometryPersistence.h"
#include "i_pathengine.h"
#include "platform_common/FloatPersistence.h"
#include "common/interface/iXMLOutputStream.h"
#include "common/interface/Assert.h"

void
SaveFaceVertexMesh(const iFaceVertexMesh& mesh, iXMLOutputStream& xos)
{
    tSigned32 i;
    for(i = 0; i != mesh.vertices(); ++i)
    {
        xos.openElement("vertex");
        xos.addAttribute("x", mesh.vertexX(i));
        xos.addAttribute("y", mesh.vertexY(i));
        std::string persistent = FloatToPersistent(mesh.vertexZ(i));
        xos.addAttribute("z", persistent.c_str());    
        xos.closeElement("vertex");
    }
    assertC(PE_FaceAttribute_Top == 6);
    for(i = 0; i != mesh.faces(); ++i)
    {
        xos.openElement("face");
        xos.addAttribute("v1", mesh.vertexIndex(i, 0));
        xos.addAttribute("v2", mesh.vertexIndex(i, 1));
        xos.addAttribute("v3", mesh.vertexIndex(i, 2));
        if(mesh.faceAttribute(i, PE_FaceAttribute_SurfaceType) != 0)
        {
            xos.addAttribute("a_surfaceType", mesh.faceAttribute(i, PE_FaceAttribute_SurfaceType));
        }
        if(mesh.faceAttribute(i, PE_FaceAttribute_SectionID) != -1)
        {
            xos.addAttribute("a_sectionID", mesh.faceAttribute(i, PE_FaceAttribute_SectionID));
        }
        if(mesh.faceAttribute(i, PE_FaceAttribute_UserData) != 0)
        {
            xos.addAttribute("a_userData", mesh.faceAttribute(i, PE_FaceAttribute_UserData));
        }
        if(mesh.faceAttribute(i, PE_FaceAttribute_MarksPortal) != 0)
        {
            xos.addAttribute("a_marksPortal", mesh.faceAttribute(i, PE_FaceAttribute_MarksPortal));
        }
        if(mesh.faceAttribute(i, PE_FaceAttribute_MarksTerrainPortal) != 0)
        {
            xos.addAttribute("a_marksTerrainPortal", mesh.faceAttribute(i, PE_FaceAttribute_MarksTerrainPortal));
        }
        if(mesh.faceAttribute(i, PE_FaceAttribute_MarksExternalShape) != 0)
        {
            xos.addAttribute("a_marksExternalShape", mesh.faceAttribute(i, PE_FaceAttribute_MarksExternalShape));
        }
        if(mesh.faceAttribute(i, PE_FaceAttribute_MarksConnectingEdge) != 0)
        {
            xos.addAttribute("a_marksConnectingEdge", mesh.faceAttribute(i, PE_FaceAttribute_MarksConnectingEdge));
        }
        xos.closeElement("face");
    }
}

