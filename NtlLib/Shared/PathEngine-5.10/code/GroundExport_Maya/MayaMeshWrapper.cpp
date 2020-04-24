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
#include "MayaMeshWrapper.h"
#include <maya/MFnMesh.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MDagPath.h>
#include <maya/MIntArray.h>
#include <maya/MPointArray.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include "common/AttributesBuilder.h"
#include "common/interface/Error.h"
#include "common/interface/Assert.h"
#include "common/CLib/string.h"
#include "common/STL/string.h"

static bool
ColourMatchesAShadeOfGreen(const MColor& colour, int& shadeOfGreen)
{
    if(colour.r != 0.f || colour.g == 0.f || colour.b != 0.f)
    {
        return false;
    }
    float g = colour.g;
    g *= 255.f;
    g += 0.5f;
    shadeOfGreen = static_cast<int>(g);
    assertD(shadeOfGreen > 0 && shadeOfGreen <= 255);
    return true;
}

static bool
FaceColoursMatchAShadeOfGreen(
        MFnMesh* mesh,
        const MColorArray& colourArray,
        tSigned32 f,
        int& shadeOfGreen
        )
{
    assertR(colourArray.length() != 0);
    MStatus status;
    int colourIndex;
    int checkShade;
    status = mesh->getFaceVertexColorIndex(f, 0, colourIndex);
    assertR(status == MStatus::kSuccess);
    assertR(colourIndex >= 0 && colourIndex < static_cast<int>(colourArray.length()));
    if(!ColourMatchesAShadeOfGreen(colourArray[colourIndex], shadeOfGreen))
    {
        return false;
    }
    status = mesh->getFaceVertexColorIndex(f, 1, colourIndex);
    assertR(status == MStatus::kSuccess);
    assertR(colourIndex >= 0 && colourIndex < static_cast<int>(colourArray.length()));
    if(!ColourMatchesAShadeOfGreen(colourArray[colourIndex], checkShade) || checkShade != shadeOfGreen)
    {
        return false;
    }
    status = mesh->getFaceVertexColorIndex(f, 2, colourIndex);
    assertR(status == MStatus::kSuccess);
    assertR(colourIndex >= 0 && colourIndex < static_cast<int>(colourArray.length()));
    if(!ColourMatchesAShadeOfGreen(colourArray[colourIndex], checkShade) || checkShade != shadeOfGreen)
    {
        return false;
    }
    return true;

}

static bool
ColourMatches(const MColor& colour, float r, float g, float b)
{
    return colour.r == r && colour.g == g && colour.b == b;
}

static bool
FaceColoursMatch(
        MFnMesh* mesh,
        const MColorArray& colourArray,
        tSigned32 f,
        float r, float g, float b
        )
{
    assertR(colourArray.length() != 0);
    MStatus status;
    int colourIndex;
    status = mesh->getFaceVertexColorIndex(f, 0, colourIndex);
    assertR(status == MStatus::kSuccess);
    assertR(colourIndex >= 0 && colourIndex < static_cast<int>(colourArray.length()));
    if(!ColourMatches(colourArray[colourIndex], r, g, b))
    {
        return false;
    }
    status = mesh->getFaceVertexColorIndex(f, 1, colourIndex);
    assertR(status == MStatus::kSuccess);
    assertR(colourIndex >= 0 && colourIndex < static_cast<int>(colourArray.length()));
    if(!ColourMatches(colourArray[colourIndex], r, g, b))
    {
        return false;
    }
    status = mesh->getFaceVertexColorIndex(f, 2, colourIndex);
    assertR(status == MStatus::kSuccess);
    assertR(colourIndex >= 0 && colourIndex < static_cast<int>(colourArray.length()));
    if(!ColourMatches(colourArray[colourIndex], r, g, b))
    {
        return false;
    }
    return true;
}

void
cMayaMeshWrapper::extractAttributesFromVertexColours(MColorArray& colourArray)
{
    for(tSigned32 i = 0; i < faces(); ++i)
    {
        int shadeOfGreen;
        if(FaceColoursMatchAShadeOfGreen(_mesh, colourArray, i, shadeOfGreen))
        {
            _sectionIDs.resize(faces(), -1);
            _sectionIDs[i] = 255 - shadeOfGreen;
        }
        if(FaceColoursMatch(_mesh, colourArray, i, 0.f, 0.f, 1.f))
        {
            _markerTypes.resize(faces(), -1);
            _markerTypes[i] = PE_FaceAttribute_MarksPortal;
        }
        else if(FaceColoursMatch(_mesh, colourArray, i, 1.f, 0.f, 1.f))
        {
            _markerTypes.resize(faces(), -1);
            _markerTypes[i] = PE_FaceAttribute_MarksTerrainPortal;
        }
        else if(FaceColoursMatch(_mesh, colourArray, i, 0.f, 0.f, 0.f))
        {
            _markerTypes.resize(faces(), -1);
            _markerTypes[i] = PE_FaceAttribute_MarksExternalShape;
        }
        else if(FaceColoursMatch(_mesh, colourArray, i, 0.f, 1.f, 1.f))
        {
            _markerTypes.resize(faces(), -1);
            _markerTypes[i] = PE_FaceAttribute_MarksConnectingEdge;
        }
    }
}

void
cMayaMeshWrapper::extractAttributesFromMaterial(const MFnDependencyNode& material, tSigned32 faceIndex)
{
    MStatus status;

    MString materialName = material.name();
    const char* materialNameAsChar = materialName.asChar();

	MString notesMString;
	MPlug notesPlug = material.findPlug("notes", &status);
	if (status != MStatus::kSuccess)
    {
        return;
    }
	notesPlug.getValue(notesMString);

    const char* notes = notesMString.asChar();
    std::string notesString(notes);
    std::string::size_type start = notesString.find("{surfaceType=");
    if(start == std::string::npos)
    {
        return;
    }
    std::string::size_type valueStart = start + 13;
    std::string::size_type end = notesString.find("}", valueStart);
    if(end == std::string::npos)
    {
        cAttributesBuilder builder;
        builder.add("materialName", materialNameAsChar);
        Error("Warning", "Failed to find markup end in material notes.", builder.get());
        return;
    }
    char* afterParsing;
    tSigned32 surfaceType = strtol(notes + valueStart, &afterParsing, 10);
    if(end == valueStart || afterParsing != notes + end)
    {
        cAttributesBuilder builder;
        builder.add("materialName", materialNameAsChar);
        Error("Warning", "Could not parse value embedded in material notes.", builder.get());
        return;
    }
    if(surfaceType < 0)
    {
        cAttributesBuilder builder;
        builder.add("materialName", materialNameAsChar);
        Error("Warning", "Negative surface type in material notes will be ignored. (Surface types must be positive.)", builder.get());
        return;
    }
    _surfaceTypes.resize(faces(), 0);
    _surfaceTypes[faceIndex] = surfaceType;
}

void
cMayaMeshWrapper::extractAttributesFromShaders(const MObjectArray& shaders, const MIntArray& indices)
{
    MStatus status;
    for(tSigned32 i = 0; i < faces(); ++i)
    {
        if(static_cast<tUnsigned32>(i) >= indices.length())
        {
            break;
        }
        int shaderIndex = indices[i];
        if(shaderIndex == -1)
        {
            continue;
        }
        assertR(shaderIndex >= 0 && static_cast<unsigned>(shaderIndex) < shaders.length());
        MFnDependencyNode shader(shaders[shaderIndex], &status);
        assertR(status == MStatus::kSuccess);

    	MPlug surfaceShaderPlug = shader.findPlug("surfaceShader", true, &status);
        assertR(status == MStatus::kSuccess);

		MPlugArray materialPlugs;
    	surfaceShaderPlug.connectedTo(materialPlugs, true, true, &status);
		for(unsigned j = 0; j < materialPlugs.length(); ++j)
		{
			MFnDependencyNode material(materialPlugs[j].node(), &status);
            assertR(status == MStatus::kSuccess);
            extractAttributesFromMaterial(material, i);
        }
    }
}

bool
cMayaMeshWrapper::checkCanConstructFrom(const MDagPath& dagPath)
{
	MStatus status;
    MFnMesh mesh(dagPath, &status);
    if(status == MStatus::kFailure)
    {
        cAttributesBuilder builder;
        builder.add("dagPath", dagPath.fullPathName().asChar());
        Error("NonFatal", "cMayaMeshWrapper - Cannot construct MFnMesh from object", builder.get());
        return false;
    }
	unsigned int faces = mesh.numPolygons();
    unsigned int i;
    for(i = 0; i < faces; i++)
    {
		unsigned int vertices = mesh.polygonVertexCount(i, &status);
        assertR(status == MStatus::kSuccess);
        if(vertices != 3)
        {
            cAttributesBuilder builder;
            builder.add("dagPath", dagPath.fullPathName().asChar());
            Error("NonFatal", "cMayaMeshWrapper - Mesh is not triangulated. Cannot construct. (Ground meshes should be triangulated manually before export.)", builder.get());
            return false;
        }
    }
    return true;
}


cMayaMeshWrapper::cMayaMeshWrapper(const MDagPath& dagPath, float scale)
{
	MStatus status;
	_mesh = new MFnMesh(dagPath, &status);
    assertR(status == MStatus::kSuccess);

    MPointArray vertexArray;

	status = _mesh->getPoints(vertexArray, MSpace::kWorld);
    assertR(status == MStatus::kSuccess);

    unsigned vertices = vertexArray.length();
    _transformedVertexXY.resize(vertices * 2);
    _transformedVertexZ.resize(vertices * 2);
    unsigned i;
    for(i = 0; i < vertices; i++)
    {
        _transformedVertexXY[i * 2 + 0] = static_cast<tSigned32>(vertexArray[i].z * scale);
        _transformedVertexXY[i * 2 + 1] = static_cast<tSigned32>(vertexArray[i].x * scale);
        _transformedVertexZ[i] = static_cast<float>(vertexArray[i].y * scale);
    }

    MColorArray colourArray;
    status = _mesh->getFaceVertexColors(colourArray);
    if(status == MStatus::kSuccess)
    {
        extractAttributesFromVertexColours(colourArray);
    }

    unsigned instanceNumber = dagPath.instanceNumber(&status);
    assertR(status == MStatus::kSuccess);
    MObjectArray shaders;
    MIntArray indices;
    status = _mesh->getConnectedShaders(instanceNumber, shaders, indices);
    if(status == MStatus::kSuccess)
    {
        extractAttributesFromShaders(shaders, indices);
    }
}
cMayaMeshWrapper::~cMayaMeshWrapper()
{
    delete _mesh;
}

tSigned32
cMayaMeshWrapper::faces() const
{
	return static_cast<tSigned32>(_mesh->numPolygons());
}
tSigned32
cMayaMeshWrapper::vertices() const
{
	return static_cast<tSigned32>(_mesh->numVertices());
}

tSigned32
cMayaMeshWrapper::vertexIndex(tSigned32 face, tSigned32 vertexInFace) const
{
    MIntArray vertexIndexArray;
	MStatus status;
	status = _mesh->getPolygonVertices(face, vertexIndexArray);
    assertR(status == MStatus::kSuccess);
    return vertexIndexArray[2 - vertexInFace];
}
tSigned32
cMayaMeshWrapper::vertexX(tSigned32 vertex) const
{
    return _transformedVertexXY[vertex * 2 + 0];
}
tSigned32
cMayaMeshWrapper::vertexY(tSigned32 vertex) const
{
    return _transformedVertexXY[vertex * 2 + 1];
}
float
cMayaMeshWrapper::vertexZ(tSigned32 vertex) const
{
    return _transformedVertexZ[vertex];
}
tSigned32
cMayaMeshWrapper::faceAttribute(tSigned32 face, tSigned32 attributeIndex) const
{
    if(attributeIndex == PE_FaceAttribute_SurfaceType)
    {
        if(_surfaceTypes.empty())
        {
            return 0;
        }
        return _surfaceTypes[face];
    }
    if(attributeIndex == PE_FaceAttribute_SectionID)
    {
        if(_sectionIDs.empty())
        {
            return -1;
        }
        return _sectionIDs[face];
    }
    if(attributeIndex == PE_FaceAttribute_MarksPortal
     || attributeIndex == PE_FaceAttribute_MarksTerrainPortal
     || attributeIndex == PE_FaceAttribute_MarksExternalShape
     || attributeIndex == PE_FaceAttribute_MarksConnectingEdge
        )
    {
        if(_markerTypes.empty())
        {
            return 0;
        }
        if(_markerTypes[face] == attributeIndex)
        {
            return 1;
        }
        return 0;
    }
    if(attributeIndex == PE_FaceAttribute_UserData)
    {
        return 0;
    }
    return -1;
}
