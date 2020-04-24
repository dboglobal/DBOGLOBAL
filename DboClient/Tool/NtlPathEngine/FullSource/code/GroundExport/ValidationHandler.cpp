//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "Max.h"

#undef assert
#include "platform_common/Header.h"
#include "ValidationHandler.h"
#include "MeshesForSelectionSet.h"
#include "ExecuteMaxScript.h"
#include "libs/Exporter_Common/interface/CombinedFaceVertexMeshes.h"
#include "common/interface/Attributes.h"
#include "common/STL/sstream.h"

static void
SelectFace(Interface* ip, std::string nodeName, tSigned32 faceIndex)
{
    std::ostringstream s;
    s << "setFaceSelection $'";
    s << nodeName;
    s << "' #{";
    s << faceIndex + 1;
    s << "}";
    std::vector<char> command(s.str().size()+1);
    size_t i;
    for(i = 0; i < s.str().size(); i++)
    {
        command[i] = s.str()[i];
    }
    command[i] = 0;
    ExecuteMaxScript(ip, &command.front());
}

static void
SelectFaces(Interface* ip, std::string nodeName, const std::vector<tSigned32>& faces)
{
    std::ostringstream s;
    s << "setFaceSelection $'";
    s << nodeName;
    s << "' #(";
    tSigned32 i;
    for(i = 0; i < faces.size(); ++i)
    {
        if(i)
        {
            s << ',';
        }
        s << faces[i] + 1;
    }
    s << ")";
    std::vector<char> command(s.str().size()+1);
    for(i = 0; i < s.str().size(); i++)
    {
        command[i] = s.str()[i];
    }
    command[i] = 0;
    ExecuteMaxScript(ip, &command.front());
}

static void
SelectVertices(Interface* ip, std::string nodeName, const std::vector<tSigned32>& vertices)
{
    std::ostringstream s;
    s << "setVertSelection $'";
    s << nodeName;
    s << "' #(";
    tSigned32 i;
    for(i = 0; i < vertices.size(); ++i)
    {
        if(i)
        {
            s << ',';
        }
        s << vertices[i] + 1;
    }
    s << ")";
    std::vector<char> command(s.str().size()+1);
    for(i = 0; i < s.str().size(); i++)
    {
        command[i] = s.str()[i];
    }
    command[i] = 0;
    ExecuteMaxScript(ip, &command.front());
}

void
cValidationHandler::logString(const std::string& toLog)
{
    if(!_logFile)
    {
        return;
    }
    size_t written = fwrite(toLog.c_str(), 1, toLog.length(), _logFile);
    if(written != toLog.length())
    {
        Error("Warning", "PathEngine 3DS Max exporter error handler: fwrite failed when writing to log file. Will not attempt to write further errors to this log file.");
        fclose(_logFile); // already an error, so no point really in checking return value
        _logFile = 0;
    }
    else
    {
        int error = fflush(_logFile);
        if(error)
        {
            Error("Warning", "PathEngine 3DS Max exporter error handler: fflush returned non-zero.");
        }
    }
}

void
cValidationHandler::logErrorIfLogEnabled(const std::string& title, const std::string& description)
{
    if(!_logToFileEnabled)
    {
        return;
    }
    if(!_attemptedToOpenLogFile)
    {
        _attemptedToOpenLogFile = true;
        _logFile = fopen(_logFileName.c_str(), "wb");
        if(!_logFile)
        {
            Error("Warning", "PathEngine 3DS Max exporter error handler: Failed to open error log file. Errors will not be logged.");
        }
    }
    logString(title);
    logString("\n");
    logString(description);
    logString("\n");
}

cValidationHandler::~cValidationHandler()
{
    assertD(_contextDescription == 0);
    assertD(!_logToFileEnabled);
}

void
cValidationHandler::enableLogToFile(const std::string& logFileName)
{
    assertD(!_logToFileEnabled);
    _logToFileEnabled = true;
    _logFileName = logFileName;
    _attemptedToOpenLogFile = false;
    _logFile = 0;
}
void
cValidationHandler::disableLogToFile()
{
    assertD(_logToFileEnabled);
    _logToFileEnabled = false;
    if(_logFile)
    {
        int error = fclose(_logFile);
        if(error)
        {
            Error("Warning", "PathEngine 3DS Max exporter error handler: Error closing error log file.");
        }
    }
}

iErrorHandler::eAction
cValidationHandler::handle(const char* errorType, const char* errorString, const char *const* attributes)
{
    std::ostringstream s;
    s << "An error was reported by PathEngine:\n";
    s << errorString;
    if(_contextDescription)
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

    if(_meshes)
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

    	    std::string nodeName = _meshes->name(partIndex);
            if(_meshes->wasConverted(partIndex))
            {
                if(!_wasConvertedErrorEmittedForThisContext)
                {
                    s << "\n\nThese error attributes enable the error to be localised to specific geometry on object: '" << nodeName << "',";
                    s << " but this object is not a TriObject (and was converted automatically at start of export).";
                    s << "  In order to get feedback about the locality of this error convert this object manually to an editable mesh and then re-export.";
                    _wasConvertedErrorEmittedForThisContext = true;
                }
            }
            else
            {
                if(!faceIndicesInPart.empty() && !_facesSelectedForThisContext)
                {
                    if(faceIndicesInPart.size() > 1)
                    {
        			    s << "\n\nFaces in " << nodeName << " will be selected to correspond to these attributes.";
                    }
                    else
                    {
        			    s << "\n\nFace " << faceIndicesInPart[0] + 1 << " in " << nodeName << " will be selected to correspond to these attributes.";
                    }

    			    SelectFaces(_ip, nodeName, faceIndicesInPart);
                    _facesSelectedForThisContext = true;
                }
                if(!vertexIndicesInPart.empty() && !_vertsSelectedForThisContext)
                {
    			    s << "\n\nVertices in " << nodeName << " will be selected to correspond to these attributes.";
      			    SelectVertices(_ip, nodeName, vertexIndicesInPart);
                    _vertsSelectedForThisContext = true;
                }
            }
        }
    }

    std::ostringstream messageBoxTitle;
    messageBoxTitle << "PathEngine error, type " << errorType;

    if(!_skipFurtherErrorsInThisContext)
    {
        if(_contextDescription)
        {
            s << "\n\nSelect 'Yes' to continue to display message boxes for errors during this operation";
            s << "\nor 'No' to skip errors for the remainder of the operation.";
            int choice = MessageBox(0, s.str().c_str(), messageBoxTitle.str().c_str(), MB_YESNO);
            if(choice == IDNO)
            {
                _skipFurtherErrorsInThisContext = true;
            }
        }
        else
        {
          // outside an error 'context'
            MessageBox(0, s.str().c_str(), messageBoxTitle.str().c_str(), MB_OK);
        }
    }

    logErrorIfLogEnabled(messageBoxTitle.str(), s.str());

    return CONTINUE;
}
