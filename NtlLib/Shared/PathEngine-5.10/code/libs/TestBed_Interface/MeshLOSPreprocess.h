//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Mesh3D/interface/tMesh_3D_Header.h"
#include "common/STL/vector.h"

namespace nLineCollidesWithTriangle
{
class cTriPreprocess;
class cLinePreprocess;
}

void GenerateMeshLOSPreprocess(tMesh_3D& mesh, std::vector<nLineCollidesWithTriangle::cTriPreprocess>& result);
bool LineCollidesWithMesh(const nLineCollidesWithTriangle::cLinePreprocess& line,
                          const std::vector<nLineCollidesWithTriangle::cTriPreprocess>& meshPreprocess,
                          tSigned32& faceIndex);
bool RayCollidesWithMesh(const nLineCollidesWithTriangle::cLinePreprocess& line,
                         const std::vector<nLineCollidesWithTriangle::cTriPreprocess>& meshPreprocess,
                         tSigned32& faceIndex, float& fractionAlong);
void GetFacesObstructingLine(const nLineCollidesWithTriangle::cLinePreprocess& line,
                         const std::vector<nLineCollidesWithTriangle::cTriPreprocess>& meshPreprocess,
                         std::vector<tSigned32>& appendTo);
