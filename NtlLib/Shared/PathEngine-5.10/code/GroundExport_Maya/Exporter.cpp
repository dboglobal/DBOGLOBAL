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
#include "MayaMeshWrapper.h"
#include "AnchorsAndShapesFromMaya.h"
#include "MayaErrorHandler.h"
#include "GetAllOfType.h"
#include "ImportMesh_Maya.h"
#include "ImportAnchorsAndShapes_Maya.h"
#include <maya/MDagPath.h>
#include <maya/MStatus.h>
#include <maya/MString.h>
#include <maya/MObject.h>
#include <maya/MGlobal.h>
#include <maya/MFnDagNode.h>
#include <maya/MFn.h>
#include "platform_common/FileOutputStream.h"
#include "libs/PathEngine_Interface/interface/Singletons.h"
#include "libs/PathEngine_Interface/interface/cPathEngine.h"
#include "libs/Exporter_Common/interface/AnchorsAndShapesFromIMesh.h"
#include "libs/Exporter_Common/interface/Import.h"
#include "libs/Exporter_Common/interface/CombinedFaceVertexMeshes.h"
#include "libs/Content_Processing/interface/Export.h"
#include "libs/Content_Processing/interface/AnchorsAndPinnedShapes.h"
#include "libs/Content_Processing/interface/LoadFaceVertexMeshSet.h"
#include "libs/ParseXML/interface/ParseXML.h"
#include "libs/ParseXML/interface/DocumentHandler.h"
#include "libs/ParseXML/interface/ParallelHandler.h"
#include "platform_common/LoadSaveBinary.h"
#include "common/AttributesBuilder.h"
#include "common/interface/Error.h"
#include "i_pathengine.h"
#include <fstream>
#include <sstream>
#include <vector>

void*
cExporter::creator() 
{
    return new cExporter();
}

bool
cExporter::haveWriteMethod() const
{
    return true;
}
bool
cExporter::haveReadMethod() const
{
    return true;
}

static bool
LookupBoolOption(const MStringArray& optionList, const char* name)
{
    MStringArray theOption;
    for(unsigned i = 0; i < optionList.length(); i++)
    {
        theOption.clear();
        optionList[i].split('=', theOption);
        if(theOption.length () == 0)
        {
            // empty option
            continue;
        }
        if(theOption[0] == MString(name))
        {
            return (theOption[1].asInt() != 0);
        }
    }
    return false; // not found - default to false
}
static int
LookupIntOption(const MStringArray& optionList, const char* name)
{
    MStringArray theOption;
    for(unsigned i = 0; i < optionList.length(); i++)
    {
        theOption.clear();
        optionList[i].split('=', theOption);
        if(theOption.length () == 0)
        {
            // empty option
            continue;
        }
        if(theOption[0] == MString(name))
        {
            return theOption[1].asInt();
        }
    }
    return 0; // not found - default to zero
}
static float
LookupFloatOption(const MStringArray& optionList, const char* name)
{
    MStringArray theOption;
    for(unsigned i = 0; i < optionList.length(); i++)
    {
        theOption.clear();
        optionList[i].split('=', theOption);
        if(theOption.length () == 0)
        {
            // empty option
            continue;
        }
        if(theOption[0] == MString(name))
        {
            return theOption[1].asFloat();
        }
    }
    return 0.f; // not found - default to zero
}

class cWrappersDeleter
{
    std::vector<iFaceVertexMesh*>& _toDelete;
public:
    cWrappersDeleter(std::vector<iFaceVertexMesh*>& toDelete) :
     _toDelete(toDelete)
    {
    }
    ~cWrappersDeleter()
    {
        for(tSigned32 i = 0; i != SizeL(_toDelete); ++i)
        {
            delete _toDelete[i];
        }
    }
};

MStatus
cExporter::writer(const MFileObject& file, const MString& optionsString, MPxFileTranslator::FileAccessMode mode)
{
    std::vector<MDagPath> toConsider;
    if(MPxFileTranslator::kExportAccessMode == mode)
    {
        GetAllOfType(MFn::kMesh, toConsider);
    }
    else if(MPxFileTranslator::kExportActiveAccessMode == mode)
    {
        GetAllSelectedOfType(MFn::kMesh, toConsider);
    }
    else
    {
        Error("NonFatal", "Unsupported write mode. Only supports 'export selected' and 'export all'. Aborting.");
        return MStatus::kFailure;
    }
    std::vector<MDagPath> toExport;
    for(size_t i = 0; i < toConsider.size(); i++)
    {
	    MStatus status;
        MFnDagNode dagNode(toConsider[i], &status);
        assertR(status == MStatus::kSuccess);
        if(status != MStatus::kSuccess)
        {
            continue;
        }
        MString name = dagNode.name(&status);
        assertR(status == MStatus::kSuccess);
        if(status != MStatus::kSuccess)
        {
            continue;
        }
        if(strncmp(name.asChar(), "ground_", 7) == 0)
        {
            toExport.push_back(toConsider[i]);
        }
    }

    std::ostringstream oss;
    oss << "Attempting to export " << SizeL(toExport) << " pieces";
    MGlobal::displayInfo(oss.str().c_str());

    for(size_t i = 0; i < toExport.size(); i++)
    {
        if(!cMayaMeshWrapper::checkCanConstructFrom(toExport[i]))
        {
            return MStatus::kFailure;
        }
    }

    MStringArray optionList;
    if(optionsString.length () > 0)
    {
        optionsString.split(';', optionList);
    }

    float scale = LookupFloatOption(optionList, "scaleField");

    std::vector<iFaceVertexMesh*> wrappers;
    for(size_t i = 0; i < toExport.size(); i++)
    {
        wrappers.push_back(new cMayaMeshWrapper(toExport[i], scale));
    }
    cWrappersDeleter wrappersDeleter(wrappers);

    cAnchorsAndPinnedShapes anchorsAndShapes;
    {
        std::vector<MDagPath> toConsider;
        if(MPxFileTranslator::kExportAccessMode == mode)
        {
            GetAllOfType(MFn::kNurbsCurve, toConsider);
        }
        else if(MPxFileTranslator::kExportActiveAccessMode == mode)
        {
            GetAllSelectedOfType(MFn::kNurbsCurve, toConsider);
        }
        BuildAnchorsAndShapesFromMaya(toConsider, scale, anchorsAndShapes);
    }

    const MString fileNameMString = file.fullName();
    const char* fileName = fileNameMString.asChar();

    enum eFileType
    {
        GROUND_MESH,
        CONTENT_CHUNK,
        SOURCE_CONTENT,
    } fileType;
    bool tokenise;
    if(LookupBoolOption(optionList, "manualFormatAndTypeCheckBox"))
    {
        if(LookupBoolOption(optionList, "selectXMLGroundButton"))
        {
            tokenise = false;
            fileType = GROUND_MESH;
        }
        else if(LookupBoolOption(optionList, "selectTOKGroundButton"))
        {
            tokenise = true;
            fileType = GROUND_MESH;
        }
        else if(LookupBoolOption(optionList, "selectXMLContentButton"))
        {
            tokenise = false;
            fileType = SOURCE_CONTENT;
        }
        else if(LookupBoolOption(optionList, "selectTOKContentButton"))
        {
            tokenise = true;
            fileType = SOURCE_CONTENT;
        }
        else if(LookupBoolOption(optionList, "selectChunkButton"))
        {
            tokenise = true;
            fileType = CONTENT_CHUNK;
        }
        else
        {
            Error("Warning", "Bad options string, manual format and type specified but no format and type selected, defaulting to XML ground mesh export.");
            tokenise = false;
            fileType = GROUND_MESH;
        }
    }
    else
    {
        size_t length = strlen(fileName);
        if(length > 18 && strcmp(fileName + length - 18, ".sourceContent.tok") == 0)
        {
            tokenise = true;
            fileType = SOURCE_CONTENT;
        }
        else if(length > 18 && strcmp(fileName + length - 18, ".sourceContent.xml") == 0)
        {
            tokenise = false;
            fileType = SOURCE_CONTENT;
        }
        else if(length > 17 && strcmp(fileName + length - 17, ".contentChunk.tok") == 0)
        {
            tokenise = true;
            fileType = CONTENT_CHUNK;
        }
        else if(length > 17 && strcmp(fileName + length - 17, ".contentChunk.xml") == 0)
        {
            Error("NonFatal", "Invalid file suffix '.contentChunk.xml', content chunks may only be exported in tokenised XML format. Aborting.");
            return MStatus::kFailure;
        }
        else if(length > 4 && strcmp(fileName + length - 4, ".tok") == 0)
        {
            tokenise = true;
            fileType = GROUND_MESH;
        }
        else if(length > 4 && strcmp(fileName + length - 4, ".xml") == 0)
        {
            tokenise = false;
            fileType = GROUND_MESH;
        }
        else
        {
            Error("Warning", "Unrecognised file suffix, defaulting to xml ground mesh export. (Use a '.xml' file suffix to avoid this warning.)");
            tokenise = false;
            fileType = GROUND_MESH;
        }
    }

    const char* format = "xml";
    if(tokenise)
    {
        format = "tok";
    }

    if(fileType == SOURCE_CONTENT)
    {
        cFileOutputStream fos(fileName);
        nSingletons::pathEngine().saveContentData(
                &wrappers.front(), SizeL(wrappers), 
                &anchorsAndShapes,
                format, &fos
                );
        MGlobal::displayInfo("Saved source content snapshot to " + fileNameMString + ".");
        return MS::kSuccess;
    }
    else
    if(fileType == CONTENT_CHUNK)
    {
        assertR(tokenise);
//        if(wrappers.size() > 1)
//        {
//            Error("NonFatal", "Only one ground element currently supported for content chunk export.");
//            return MS::kFailure;
//        }
//
//        iFaceVertexMesh* ground = 0;
//        if(!wrappers.empty())
//        {
//            ground = wrappers.front();
//            if(ground->faces() == 0)
//            {
//                ground = 0;
//            }
//        }
        cCombinedFaceVertexMeshes combinedWrappers(wrappers);

        iAnchorsAndPinnedShapes* aaps = 0;
        if(anchorsAndShapes.numberOfAnchors())
        {
            aaps = &anchorsAndShapes;
        }
//        if(ground == 0 && aaps == 0)
        if(combinedWrappers.faces() == 0 && aaps == 0)
        {
            Error("Warning", "Nothing to export!");
            return MS::kFailure;
        }

//        iContentChunk* chunk = nSingletons::pathEngine().newContentChunk(ground, aaps, 0);    
        iContentChunk* chunk = nSingletons::pathEngine().newContentChunk(&combinedWrappers, aaps, 0);
    
        if(LookupBoolOption(optionList, "preValidateCheckBox"))
        {
            cMayaErrorHandler::refInstance().setContext("Pre-validating content chunk", &toExport, &combinedWrappers);
            bool validatedOK = chunk->preValidate(LookupIntOption(optionList, "preValidateScaleField"), LookupIntOption(optionList, "preValidateRangeField"));
            cMayaErrorHandler::refInstance().clearContext();
            if(!validatedOK)
            {
                delete chunk;
                return MS::kFailure;
            }
        }

        cFileOutputStream fos(fileName);
        chunk->save(&fos);

        delete chunk;
        MGlobal::displayInfo("Saved content chunk to " + fileNameMString + ".");
        return MS::kSuccess;
    }
    else
    {
        cAttributesBuilder attributes;

        if(LookupBoolOption(optionList, "filterFacesCheckBox"))
        {
            attributes.add("filterFacesWithSlopeAbove", LookupFloatOption(optionList, "maxSlopeField"));
        }
        if(LookupBoolOption(optionList, "optimiseCheckBox"))
        {
            attributes.add("optimiseWithThreshold", LookupFloatOption(optionList, "maxErrorField"));
        }

        cMayaErrorHandler::refInstance().setContext("Calling iPathEngine::buildMeshFromContent()", &toExport);
        iMesh* mesh = nSingletons::pathEngine().buildMeshFromContent(&wrappers.front(), SizeL(wrappers), attributes.get());
        cMayaErrorHandler::refInstance().clearContext();

        if(mesh)
        {
            AddAnchorsAndPinnedShapes(nSingletons::pathEngine(), mesh, anchorsAndShapes, "", 1);
            cFileOutputStream fos(fileName);

            mesh->saveGround(format, LookupBoolOption(optionList, "addMappingCheckBox"), &fos);

            delete mesh;

            MGlobal::displayInfo("Export to " + fileNameMString + " successful.");
            return MS::kSuccess;
        }
        else
        {
            MGlobal::displayInfo("Export to " + fileNameMString + " failed.");
            return MS::kFailure;
        }
    }
}


MStatus
cExporter::reader(const MFileObject& file, const MString& optionsString, MPxFileTranslator::FileAccessMode mode)
{
    if(MPxFileTranslator::kImportAccessMode != mode)
    {
        Error("NonFatal", "Unsupported read mode. Only mode supported is 'import'. Aborting.");
        return MStatus::kFailure;
    }

    MStringArray optionList;
    if(optionsString.length () > 0)
    {
        optionsString.split(';', optionList);
    }

    float scale = LookupFloatOption(optionList, "scaleField");

    const MString fileNameMS = file.fullName();
    const char* fileName = fileNameMS.asChar();

    std::vector<char> buffer;

    LoadBinary(fileName, buffer);

    if(buffer.empty())
    {
        Error("NonFatal", "Failed to load file for import. Aborting");
        return MS::kFailure;
    }

    const char* format = "xml";
    for(tSigned32 i = 0; i != SizeL(buffer); ++i)
    {
        if(buffer[i] == 0)
        {
            format = "tok";
            break;
        }
    }

    {
        cFaceVertexMeshSetLoader setLoader;
        cAnchorsAndPinnedShapes anchorsAndShapes;
        cNestedHandler nestedHandler(anchorsAndShapes, "anchorsAndPinnedShapes");
        cParallelHandler parallelHandler;
        parallelHandler.addHandler(&setLoader);
        parallelHandler.addHandler(&nestedHandler);
        cDocumentHandler documentHandler(parallelHandler, "content");
        ParseXML(format, &buffer.front(), SizeL(buffer), documentHandler);
        if(documentHandler.matched())
        {
            MObject lastImported;
            tSigned32 i;
            for(i = 0; i != setLoader.size(); ++i)
            {
                lastImported = ImportMesh_Maya(*setLoader[i], scale);
            }
            if(setLoader.size())
            {
                ImportAnchorsAndShapes_Maya(anchorsAndShapes, scale, lastImported, LookupFloatOption(optionList, "anchorHeightField") / 3.f);
            }
            return MS::kSuccess;
        }
    }

    iPathEngine& pathEngine = nSingletons::pathEngineI();

    iMesh* mesh = pathEngine.loadMeshFromBuffer(format, &buffer.front(), SizeL(buffer), 0);
    if(!mesh)
    {
        Error("NonFatal", "iPathEngine::loadMeshFromBuffer() failed. Aborting");
        return MS::kFailure;
    }

    iFaceVertexMesh* asFaceVertex = ConstructMesh3DWrapper(mesh);
    MObject importedGround = ImportMesh_Maya(*asFaceVertex, scale);
    delete asFaceVertex;

    cAnchorsAndPinnedShapes anchorsAndShapes;
    AnchorsAndShapesFromIMesh(*mesh, LookupFloatOption(optionList, "anchorHeightField"), anchorsAndShapes);
    ImportAnchorsAndShapes_Maya(anchorsAndShapes, scale, importedGround, LookupFloatOption(optionList, "anchorHeightField") / 3.f);

    delete mesh;

    return MS::kSuccess;
}
