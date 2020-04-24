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
#include "interface/Import.h"
#include "Mesh3DWrapper.h"
#include "libs/PathEngine_Interface/interface/cMesh.h"

iFaceVertexMesh*
ConstructMesh3DWrapper(iMesh* meshI)
{
    cMesh* mesh = static_cast<cMesh*>(meshI);
    return new cMesh3DWrapper(mesh->ref3D(),
                              mesh->getSurfaceTypesVector(),
                              mesh->getSectionIDsVector(),
                              mesh->getUserDataVector()
                              );
}
