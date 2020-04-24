//**********************************************************************
//
// Copyright (c) 2004-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "Mesh3DWrapper.h"
#include "SplitVerticalDiscontinuities.h"
#include "common/TemplateMesh/External.h"
#include "common/STL/sstream.h"

cMesh3DWrapper::cMesh3DWrapper(tMesh_3D& meshToWrap,
            const std::vector<tSigned32>* surfaceTypes,
            const std::vector<tSigned32>* sectionIDs,
            const std::vector<tSigned32>* userData
            )
{
    _mesh.copy(meshToWrap);
    tSigned32 i;
    for(i = 0; i != _mesh.faces(); ++i)
    {
        _mesh.faceAt(i)->_originalIndex = i;
    }

  // attributes are indexed on the original mesh

    if(surfaceTypes)
    {
        _surfaceTypes = *surfaceTypes;
    }
    if(sectionIDs)
    {
        _sectionIDs = *sectionIDs;
    }
    if(userData)
    {
        _userData = *userData;
    }

  // edges with vertical discontinuities in the copied mesh are split
  // this will add faces to the mesh, for which face index will be -1

    RemoveExternalFaces(_mesh);
    tSigned32 internalFacesOnOriginal = _mesh.faces();
    SplitVerticalDiscontinuities(_mesh);

  // build a remap for converting back to indices on the original mesh

    _attributesMapping.resize(_mesh.faces());
    tFace_3D_FaceTracked f = _mesh.beginFace();
    i = 0;
    tSigned32 checkCount = 0;
    do
    {
        tSigned32 remapped = f->_originalIndex;
        if(remapped != -1)
        {
            checkCount++;
        }
        _attributesMapping[i++] = remapped;
    }
    while((++f).valid());
    assertD(checkCount == internalFacesOnOriginal);

    _mesh.consolidate();
}

tSigned32
cMesh3DWrapper::faceAttribute(tSigned32 face, tSigned32 attributeIndex) const
{
    tSigned32 originalFace = _attributesMapping[face];
    if(originalFace == -1)
    {
        return -1;
    }
    if(attributeIndex == PE_FaceAttribute_SurfaceType)
    {
        if(_surfaceTypes.empty())
        {
            return 0;
        }
        assertD(originalFace < SizeL(_surfaceTypes));
        return _surfaceTypes[originalFace];
    }
    if(attributeIndex == PE_FaceAttribute_SectionID)
    {
        if(_sectionIDs.empty())
        {
            return -1;
        }
        assertD(originalFace < SizeL(_sectionIDs));
        return _sectionIDs[originalFace];
    }
    if(attributeIndex == PE_FaceAttribute_UserData)
    {
        if(_userData.empty())
        {
            return 0;
        }
        assertD(originalFace < SizeL(_userData));
        return _userData[originalFace];
    }
    return -1;
}
