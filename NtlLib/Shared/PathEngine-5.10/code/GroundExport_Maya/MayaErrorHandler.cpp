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
#include "MayaErrorHandler.h"
#include <maya/MGlobal.h>
#include <maya/MStatus.h>
#include <maya/MObject.h>
#include <maya/MSelectionList.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshVertex.h>
#include <maya/MSelectionMask.h>
#include <maya/MDagPath.h>
#include "libs/Exporter_Common/interface/CombinedFaceVertexMeshes.h"
#include "common/STL/sstream.h"
#include "common/interface/Assert.h"
#include "common/interface/Attributes.h"

static void
SelectFaces(MDagPath path, const std::vector<tSigned32>& indices)
{
    MStatus status;
    MItMeshPolygon it(path, MObject(), &status);
    assertR(status == MStatus::kSuccess);
    for(; !it.isDone(); it.next() )
    {
        tSigned32 faceIndex = it.index();
        for(tSigned32 i = 0; i != SizeL(indices); ++i)
        {
            if(indices[i] == faceIndex)
            {
                MGlobal::select(path, it.polygon());
                break;
            }
        }
    }
}

static void
SelectVertices(MDagPath path, const std::vector<tSigned32>& indices)
{
    MStatus status;
    MItMeshVertex it(path, MObject(), &status);
    assertR(status == MStatus::kSuccess);
    for(; !it.isDone(); it.next() )
    {
        tSigned32 vertexIndex = it.index();
        for(tSigned32 i = 0; i != SizeL(indices); ++i)
        {
            if(indices[i] == vertexIndex)
            {
                MGlobal::select(path, it.vertex());
                break;
            }
        }
    }
}

cMayaErrorHandler&
cMayaErrorHandler::refInstance()
{
    static cMayaErrorHandler instance;
    return instance;
}

void
cMayaErrorHandler::setContext(const std::string& description, std::vector<MDagPath>* dagPaths, cCombinedFaceVertexMeshes* combinedMeshes)
{
    assertR(_contextDescription.empty());
    assertR(!description.empty());
    _contextDescription = description;
    _contextDagPaths = dagPaths;
    _contextCombinedMeshes = combinedMeshes;
    _objectsSelected = false;
}
void
cMayaErrorHandler::clearContext()
{
    assertR(!_contextDescription.empty());
    _contextDescription.clear();
    _contextDagPaths = 0;
    _contextCombinedMeshes = 0;
}

iErrorHandler::eAction
cMayaErrorHandler::handle(const char* errorType, const char* errorString, const char *const* attributes)
{
    std::ostringstream messageBoxTitle, s;

    messageBoxTitle << "PathEngine error, type: '" << errorType << "'";
 
    s << "An error was reported by PathEngine:\n";
    s << errorString;
    if(!_contextDescription.empty())
    {
        s << "\n\n";
        s << "In the context:\n";
        s << _contextDescription;
    }
    if(attributes && *attributes)
    {
        const char *const* ptr = attributes;
        s << "\n\nAttributes:";
        do
        {
            s << "\n'" << ptr[0] << "' = '" << ptr[1] << "'";
        }
        while(*++++ptr);
    }

    if(_contextDagPaths && !_objectsSelected)
    {
        tSigned32 partIndex = -1;
        std::vector<tSigned32> faceIndicesInPart;
        std::vector<tSigned32> vertexIndicesInPart;

        if(GetValueForAttribute(attributes, "meshPart"))
        {
            partIndex = ValueAsLong(attributes, "meshPart");
            tSigned32 faceIndex;
            faceIndex = ValueAsLongWithDefault(attributes, "faceIndex", -1);
            if(faceIndex != -1)
            {
                faceIndicesInPart.push_back(faceIndex);
            }
            faceIndex = ValueAsLongWithDefault(attributes, "faceIndex1", -1);
            if(faceIndex != -1)
            {
                faceIndicesInPart.push_back(faceIndex);
            }
            faceIndex = ValueAsLongWithDefault(attributes, "faceIndex2", -1);
            if(faceIndex != -1)
            {
                faceIndicesInPart.push_back(faceIndex);
            }
            tSigned32 vertexIndex;
            vertexIndex = ValueAsLongWithDefault(attributes, "vertexIndex", -1);
            if(vertexIndex != -1)
            {
                vertexIndicesInPart.push_back(vertexIndex);
            }
            vertexIndex = ValueAsLongWithDefault(attributes, "vertexIndex1", -1);
            if(vertexIndex != -1)
            {
                vertexIndicesInPart.push_back(vertexIndex);
            }
            vertexIndex = ValueAsLongWithDefault(attributes, "vertexIndex2", -1);
            if(vertexIndex != -1)
            {
                vertexIndicesInPart.push_back(vertexIndex);
            }
        }

        if(faceIndicesInPart.empty() && vertexIndicesInPart.empty())
        {
            partIndex = -1;
        }

        if(partIndex != -1)
        {
            assertR(partIndex >= 0 && partIndex < SizeL(*_contextDagPaths));

            if(_contextCombinedMeshes)
            {
                assertR(partIndex == 0);
                tSigned32 i;
                for(i = 0; i != SizeL(faceIndicesInPart); ++i)
                {
                    _contextCombinedMeshes->getOriginalMeshAndIndexForFace(faceIndicesInPart[i], partIndex, faceIndicesInPart[i]);
                }
                for(i = 0; i != SizeL(vertexIndicesInPart); ++i)
                {
                    _contextCombinedMeshes->getOriginalMeshAndIndexForVertex(vertexIndicesInPart[i], partIndex, vertexIndicesInPart[i]);
                }
            }

            MStatus status;
            status = MGlobal::setSelectionMode(MGlobal::kSelectComponentMode);
            assertR(status == MStatus::kSuccess);
            MSelectionMask mask = MGlobal::componentSelectionMask(&status);
            assertR(status == MStatus::kSuccess);
            status = MGlobal::setComponentSelectionMask(mask | MSelectionMask::kSelectMeshFaces | MSelectionMask::kSelectMeshVerts);
            assertR(status == MStatus::kSuccess);

            // clear object selection, so that user can just 'frame selection' on the error components
            status = MGlobal::setActiveSelectionList(MSelectionList());
            assertR(status == MStatus::kSuccess);

            // clear the hilite list to make things clearer
            //.. perhaps nicer to hilite the relevant object
            status = MGlobal::setHiliteList(MSelectionList());
            assertR(status == MStatus::kSuccess);

            if(!faceIndicesInPart.empty())
            {
                s << "\n\nFace(s) in the following object will be selected to correspond to these attributes:\n";
    			SelectFaces((*_contextDagPaths)[partIndex], faceIndicesInPart);
            }
            else if(!vertexIndicesInPart.empty())
            {
                s << "\n\nVertices in the following object will be selected to correspond to these attributes:\n";
      			SelectVertices((*_contextDagPaths)[partIndex], vertexIndicesInPart);
            }
            s << (*_contextDagPaths)[partIndex].fullPathName().asChar();

            _objectsSelected = true;
        }
    }

    MGlobal::displayError(s.str().c_str());

    MessageBox(0, s.str().c_str(), messageBoxTitle.str().c_str(), MB_OK);
    return CONTINUE;
}
