//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"

#include "Max.h"
#include "modstack.h"
#include "istdplug.h"
#include "utilapi.h"
#include "linshape.h"
#undef assert

#include "MeshesForSelectionSet.h"
#include "ValidationHandler.h"
#include "ImportMesh.h"
#include "AnchorsAndShapesFromMax.h"
#include "libs/Exporter_Common/interface/AnchorsAndShapesFromIMesh.h"
#include "libs/Exporter_Common/interface/Import.h"
#include "libs/Exporter_Common/interface/CombinedFaceVertexMeshes.h"
#include "libs/Content_Processing/interface/Export.h"
#include "libs/Content_Processing/interface/AnchorsAndPinnedShapes.h"
#include "libs/Content_Processing/interface/LoadFaceVertexMeshSet.h"
#include "libs/PathEngine_Interface/interface/Singletons.h"
#include "libs/PathEngine_Interface/interface/cPathEngine.h"
#include "libs/PathEngine_Core/interface/Version.h"
#include "libs/ParseXML/interface/ParseXML.h"
#include "libs/ParseXML/interface/DocumentHandler.h"
#include "libs/ParseXML/interface/ParallelHandler.h"
#include "platform_common/FileOutputStream.h"
#include "platform_common/LoadSaveBinary.h"
#include "common/MemoryTracking.h"
#include "common/AttributesBuilder.h"
#include "common/interface/Attributes.h"
#include "common/interface/Assert.h"
#include "i_pathengine.h"

#include <vector>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <sstream>

#include "resource.h"

TCHAR *GetString(int id);

extern ClassDesc* GetGroundExportDesc();

extern HINSTANCE hInstance;

#define UV_CLASS_ID    Class_ID(0x2ef50279, 0x70573cb1)
HINSTANCE hInstance;
int controlsInit = FALSE;

__declspec( dllexport ) const TCHAR* LibDescription()
{
    return GetString(IDS_LIBDESCRIPTION);
}

//TODO: Must change this number when adding a new class
__declspec( dllexport ) int LibNumberClasses()
{
    return 1;
}

__declspec( dllexport ) ClassDesc* LibClassDesc(int i)
{
    switch(i) {
        case 0: return GetGroundExportDesc();
        default: return 0;
    }
}

__declspec( dllexport ) ULONG LibVersion()
{
    return VERSION_3DSMAX;
}

TCHAR *GetString(int id)
{
    static TCHAR buf[256];

    if (hInstance)
        return LoadString(hInstance, id, buf, sizeof(buf)) ? buf : NULL;
    return NULL;
}

class GroundExport : public UtilityObj
{
public:

    IUtil* iu;
    Interface* ip;
    ISpinnerControl* iScale;
    ISpinnerControl* iMaxSlope;
    ISpinnerControl* iMaxError;
    ISpinnerControl* iMaxScale;
    ISpinnerControl* iRange;
    ISpinnerControl* iAnchorHeight;
    float _storedScale;
    float _storedMaxSlope;
    float _storedMaxError;
    int _storedMaxScale;
    int _storedRange;
    float _storedAnchorHeight;
    UINT _storedOptions[12];
    HWND hPanel;
    cValidationHandler errorHandler;
    
    //Constructor/Destructor
    GroundExport();
    ~GroundExport();

    void BeginEditParams(Interface *ip,IUtil *iu);
    void EndEditParams(Interface *ip,IUtil *iu);
    void DeleteThis() {}

    void Init(HWND hWnd);
    void Destroy(HWND hWnd);
    void SelectionSetChanged(Interface *ip,IUtil *iu);
};



static GroundExport&
theGroundExport()
{
    static GroundExport instance;
    return instance;
}

class GroundExportClassDesc:public ClassDesc {
    public:
    int             IsPublic() {return 1;}
    void *            Create(BOOL loading = FALSE) {return &theGroundExport();}
    const TCHAR *    ClassName() {return GetString(IDS_UV_NAME);}
    SClass_ID        SuperClassID() {return UTILITY_CLASS_ID;}
    Class_ID        ClassID() {return UV_CLASS_ID;}
    const TCHAR*     Category() {return _T("");}
};

static GroundExportClassDesc GroundExportDesc;
ClassDesc* GetGroundExportDesc() {return &GroundExportDesc;}


void DoFWrite(FILE *file, const char *buffer, tUnsigned32 length)
{
    fwrite(buffer,length,1,file);
}
void DoFWrite_NullTerminated(FILE *file, const char *buffer)
{
    fwrite(buffer,strlen(buffer),1,file);
}
static bool
DoGetSaveFileName(HWND hWnd, char* Dest, int DestSize)
{
    // Remember to #include <commdlg.h> for Common Dialogs
    OPENFILENAME info;
    memset( &info, 0, sizeof( info ) );
    info.lStructSize = sizeof( info );
    info.hwndOwner = hWnd;
//    info.lpstrFilter = "All Files\0*.*\0XML Files\0*.xml\0";
    info.lpstrFilter = "All Files\0*.*\0";
    info.nFilterIndex = 1;
    info.lpstrFile = Dest;
    info.nMaxFile = DestSize;
    info.lpstrTitle = "Select file to save as";
    return GetSaveFileName( &info )!=0;
}
static bool
DoGetOpenFileName(HWND hWnd, char* Dest, int DestSize)
{
    OPENFILENAME info;
    memset( &info, 0, sizeof( info ) );
    info.lStructSize = sizeof( info );
    info.hwndOwner = hWnd;
//    info.lpstrFilter = "All Files\0*.*\0XML Files\0*.xml\0";
    info.lpstrFilter = "All Files\0*.*\0";
    info.nFilterIndex = 1;
    info.lpstrFile = Dest;
    info.nMaxFile = DestSize;
    info.lpstrTitle = "Select file to load";
    return GetOpenFileName( &info )!=0;
}

void
DoExport(const char* fileName, float scale,
        const cExportOptions& options, bool useMaterialIDs, bool setTerrainLayer, bool processConnections,
        bool addAnchors, bool namedGround, bool optimise, float maxError, bool saveSourceContent,
        bool logErrors, bool negateY
        )
{
    const char* format = "xml";
    if(options.tokenise)
    {
        format = "tok";
    }

    if(logErrors)
    {
        std::string logFileName(fileName);
        logFileName.append(".errorLog.txt");
        theGroundExport().errorHandler.enableLogToFile(logFileName);
    }

    const char* prefix = 0;
    if(namedGround)
    {
        prefix = "ground_";
    }
    cMeshesForSelectionSet meshes(theGroundExport().ip, prefix, useMaterialIDs, setTerrainLayer, processConnections, scale, negateY);

    cAnchorsAndPinnedShapes anchorsAndShapes;
    if(addAnchors)
    {
        if(namedGround)
        {
            BuildAnchorsAndShapesFromMax_AllInScene(theGroundExport().ip, anchorsAndShapes, scale, negateY);
        }
        else
        {
            BuildAnchorsAndShapesFromMax(theGroundExport().ip, prefix, anchorsAndShapes, scale, negateY);
        }
    }

    if(saveSourceContent)
    {
        std::string contentFileName(fileName);
        size_t length = contentFileName.length();
        if(length > 4
         && contentFileName[length - 4] == '.'
         && contentFileName[length - 3] == format[0]
         && contentFileName[length - 2] == format[1]
         && contentFileName[length - 1] == format[2]
            )
        {
            contentFileName.resize(length - 4);
        }
        contentFileName.append(".sourceContent.");
        contentFileName.append(format);
        cFileOutputStream fos(contentFileName.c_str());
        nSingletons::pathEngine().saveContentData(
                &meshes.refMeshes().front(), SizeL(meshes.refMeshes()), 
                &anchorsAndShapes,
                format, &fos
                );
    }

    theGroundExport().errorHandler.setContext(
                    "Adding content to the base mesh.",
                    &meshes, 
                    theGroundExport().ip);

    cAttributesBuilder attributes;
    if(options.filterFacesBySlope)
    {
        attributes.add("filterFacesWithSlopeAbove", options.maxSlope);
    }
    if(optimise)
    {
        attributes.add("optimiseWithThreshold", maxError);
    }
    iMesh* mesh = nSingletons::pathEngine().buildMeshFromContent(
            &meshes.refMeshes().front(), SizeL(meshes.refMeshes()),
            attributes.get()
            );

    if(mesh)
    {
        if(addAnchors)
        {
            AddAnchorsAndPinnedShapes(nSingletons::pathEngine(), mesh, anchorsAndShapes, "", 1);
        }

        cFileOutputStream fos(fileName);
        mesh->saveGround(format, options.addMapping, &fos);

        delete mesh;
    }

    theGroundExport().errorHandler.clearContext();
    if(logErrors)
    {
        theGroundExport().errorHandler.disableLogToFile();
    }
}



void
DoExportNamedChunk(
        const char* fileName, 
        float scale, bool preValidate,
        tSigned32 maxScale, tSigned32 translationRange,
        bool logErrors, bool negateY
        )
{
    if(logErrors)
    {
        std::string logFileName(fileName);
        logFileName.append(".errorLog.txt");
        theGroundExport().errorHandler.enableLogToFile(logFileName);
    }

    const char* prefix = "ground_";
    cMeshesForSelectionSet meshes(theGroundExport().ip, prefix, true, true, true, scale, negateY);
//    if(meshes.refMeshes().size() > 1)
//    {
//        Error("NonFatal", "Only one ground element currently supported for content chunk export.");
//        if(logErrors)
//        {
//            theGroundExport().errorHandler.disableLogToFile();
//        }
//        return;
//    }
    const char* format = "tok";
    cAnchorsAndPinnedShapes anchorsAndShapes;
    BuildAnchorsAndShapesFromMax_AllInScene(theGroundExport().ip, anchorsAndShapes, scale, negateY);

    cCombinedFaceVertexMeshes combinedWrappers(meshes.refMeshes());
//    iFaceVertexMesh* ground = 0;
//    if(!meshes.refMeshes().empty())
//    {
//        ground = meshes.refMeshes().front();
//        if(ground->faces() == 0)
//        {
//            ground = 0;
//        }
//    }
    iAnchorsAndPinnedShapes* aaps = 0;
    if(anchorsAndShapes.numberOfAnchors())
    {
        aaps = &anchorsAndShapes;
    }
//    if(ground == 0 && aaps == 0)
    if(combinedWrappers.faces() == 0 && aaps == 0)
    {
        Error("Warning", "Nothing to export!");
        if(logErrors)
        {
            theGroundExport().errorHandler.disableLogToFile();
        }
        return;
    }

//    iContentChunk* chunk = nSingletons::pathEngine().newContentChunk(ground, aaps, 0);    
    iContentChunk* chunk = nSingletons::pathEngine().newContentChunk(&combinedWrappers, aaps, 0);    
    
    theGroundExport().errorHandler.setContext(
            "Pre-validating content chunk.",
            &meshes,
            theGroundExport().ip,
            &combinedWrappers
            );

    bool validationErrorOccurred = false;
    if(preValidate)
    {
        validationErrorOccurred = !chunk->preValidate(maxScale, translationRange);
    }
    if(!validationErrorOccurred)
    {
        cFileOutputStream fos(fileName);
        chunk->save(&fos);
    }

    theGroundExport().errorHandler.clearContext();
    if(logErrors)
    {
        theGroundExport().errorHandler.disableLogToFile();
    }
    delete chunk;
}


void
DoImport(const char *fileName, float scale, float anchorHeight)
{
    char buffer[2000];
    std::vector<char> mesh_buffer;

    const char* format = "xml";
    tSigned32 fileNameLength = static_cast<tSigned32>(strlen(fileName));
    if(fileNameLength >= 4
      && fileName[fileNameLength - 4] == '.'
      && fileName[fileNameLength - 3] == 't'
      && fileName[fileNameLength - 2] == 'o'
      && fileName[fileNameLength - 1] == 'k'
      )
    {
        format = "tok";
    }

    LoadBinary(fileName, mesh_buffer);

    if(mesh_buffer.empty())
    {
        sprintf(buffer,"Failed to load file '%s'.", fileName);
        MessageBox(0,buffer,"Ground Export - Error",MB_OK);
        return;
    }

    Interface* ip = theGroundExport().ip;

    {
        cFaceVertexMeshSetLoader setLoader;
        cAnchorsAndPinnedShapes anchorsAndShapes;
        cNestedHandler nestedHandler(anchorsAndShapes, "anchorsAndPinnedShapes");
        cParallelHandler parallelHandler;
        parallelHandler.addHandler(&setLoader);
        parallelHandler.addHandler(&nestedHandler);
        cDocumentHandler documentHandler(parallelHandler, "content");
        ParseXML(format, &mesh_buffer.front(), SizeL(mesh_buffer), documentHandler);
        if(documentHandler.matched())
        {
            INode* lastImported;
            tSigned32 i;
            for(i = 0; i != setLoader.size(); ++i)
            {
                lastImported = ImportMesh(ip, *setLoader[i], scale);
            }
            if(setLoader.size())
            {
                ImportAnchorsAndPinnedShapes(ip, lastImported, anchorsAndShapes, anchorHeight, scale);
            }
            return;
        }
    }

    iPathEngine& pathEngine = nSingletons::pathEngineI();

    iMesh* mesh = pathEngine.loadMeshFromBuffer(format, &mesh_buffer.front(), SizeL(mesh_buffer), 0);
    if(!mesh)
    {
        MessageBox(0,"Failed to load mesh with PathEngine. Import aborted."
            ,"Ground Export - Error",MB_OK);
        return;
    }

    iFaceVertexMesh* asFaceVertex = ConstructMesh3DWrapper(mesh);
    INode* meshNode = ImportMesh(ip, *asFaceVertex, scale);
    delete asFaceVertex;

    cAnchorsAndPinnedShapes anchorsAndShapes;
    AnchorsAndShapesFromIMesh(*mesh, anchorHeight, anchorsAndShapes);
    ImportAnchorsAndPinnedShapes(ip, meshNode, anchorsAndShapes, anchorHeight, scale);

    delete mesh;
}

static BOOL CALLBACK GroundExportDlgProc(
        HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_INITDIALOG:
        theGroundExport().Init(hWnd);
        char s[50];
        sprintf(s,"%i Object(s) Selected",theGroundExport().ip->GetSelNodeCount());
        SetWindowText(GetDlgItem(hWnd, IDC_SEL),s);
        {
            if(InternalRelease())
            {
                sprintf(s,"Version: %i.%02i.%02i", MajorRelease(), MinorRelease(), InternalRelease());
            }
            else
            {
                sprintf(s,"Version: %i.%02i", MajorRelease(), MinorRelease());
            }
            SetWindowText(GetDlgItem(hWnd, IDC_VERSION), s);
        }
        break;

    case WM_DESTROY:
        theGroundExport().Destroy(hWnd);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {        
        char buffer[2000];

        case IDC_EXPORT_SELECTED:
            {
                if(IsDlgButtonChecked(hWnd, IDC_TOKENISE) != 0)
                {
                    strcpy(buffer,"ground.tok");
                }
                else
                {
                    strcpy(buffer,"ground.xml");
                }
                bool fileSelected = DoGetSaveFileName(hWnd, buffer, sizeof(buffer) / sizeof(*buffer));
                if(fileSelected)
                {
                    cExportOptions options;
                    options.tokenise = (IsDlgButtonChecked(hWnd, IDC_TOKENISE) != 0);
                    options.addMapping = (IsDlgButtonChecked(hWnd, IDC_ADD_MAPPING) != 0);
                    if(IsDlgButtonChecked(hWnd, IDC_PROCESS_CONNECTIONS) || IsDlgButtonChecked(hWnd, IDC_ADD_ANCHORS))
                    {
                        options.addMapping = true;
                    }
                    options.filterFacesBySlope = (IsDlgButtonChecked(hWnd, IDC_FILTER_FACES_BY_SLOPE) != 0);
                    options.maxSlope = theGroundExport().iMaxSlope->GetFVal();
                    DoExport(buffer,
                            theGroundExport().iScale->GetFVal(),
                            options, 
                            IsDlgButtonChecked(hWnd, IDC_MATERIALIDS) != 0,
                            IsDlgButtonChecked(hWnd, IDC_SETTERRAINLAYER) != 0,
                            IsDlgButtonChecked(hWnd, IDC_PROCESS_CONNECTIONS) != 0,
                            IsDlgButtonChecked(hWnd, IDC_ADD_ANCHORS) != 0,
                            false,
                            IsDlgButtonChecked(hWnd, IDC_OPTIMISE) != 0,
                            theGroundExport().iMaxError->GetFVal(),
                            IsDlgButtonChecked(hWnd, IDC_SAVE_SOURCE_CONTENT) != 0,
                            IsDlgButtonChecked(hWnd, IDC_LOG_ERRORS) != 0,
                            IsDlgButtonChecked(hWnd, IDC_NEGATE_Y) != 0
                            );
                }
            }
            break;

        case IDC_EXPORT_NAMED_GROUND:
            {
                if(IsDlgButtonChecked(hWnd, IDC_TOKENISE) != 0)
                {
                    strcpy(buffer,"ground.tok");
                }
                else
                {
                    strcpy(buffer,"ground.xml");
                }
                bool fileSelected = DoGetSaveFileName(hWnd, buffer, sizeof(buffer) / sizeof(*buffer));
                if(fileSelected)
                {
                    cExportOptions options;
                    options.tokenise = (IsDlgButtonChecked(hWnd, IDC_TOKENISE) != 0);
                    options.addMapping = (IsDlgButtonChecked(hWnd, IDC_ADD_MAPPING) != 0);
                    if(IsDlgButtonChecked(hWnd, IDC_PROCESS_CONNECTIONS) || IsDlgButtonChecked(hWnd, IDC_ADD_ANCHORS))
                    {
                        options.addMapping = true;
                    }
                    options.filterFacesBySlope = (IsDlgButtonChecked(hWnd, IDC_FILTER_FACES_BY_SLOPE) != 0);
                    options.maxSlope = theGroundExport().iMaxSlope->GetFVal();
                    DoExport(buffer,
                            theGroundExport().iScale->GetFVal(),
                            options, 
                            IsDlgButtonChecked(hWnd, IDC_MATERIALIDS) != 0,
                            IsDlgButtonChecked(hWnd, IDC_SETTERRAINLAYER) != 0,
                            IsDlgButtonChecked(hWnd, IDC_PROCESS_CONNECTIONS) != 0,
                            IsDlgButtonChecked(hWnd, IDC_ADD_ANCHORS) != 0,
                            true,
                            IsDlgButtonChecked(hWnd, IDC_OPTIMISE) != 0,
                            theGroundExport().iMaxError->GetFVal(),
                            IsDlgButtonChecked(hWnd, IDC_SAVE_SOURCE_CONTENT) != 0,
                            IsDlgButtonChecked(hWnd, IDC_LOG_ERRORS) != 0,
                            IsDlgButtonChecked(hWnd, IDC_NEGATE_Y) != 0
                            );
                }
            }
            break;

        case IDC_EXPORT_NAMED_CHUNK:
            {
                strcpy(buffer,"contentChunk.tok");
                bool fileSelected = DoGetSaveFileName(hWnd, buffer, sizeof(buffer) / sizeof(*buffer));
                if(fileSelected)
                {
                    DoExportNamedChunk(
                            buffer,
                            theGroundExport().iScale->GetFVal(),
                            IsDlgButtonChecked(hWnd, IDC_PREVALIDATE) != 0,
                            theGroundExport().iMaxScale->GetIVal(),                            
                            theGroundExport().iRange->GetIVal(),
                            IsDlgButtonChecked(hWnd, IDC_LOG_ERRORS) != 0,
                            IsDlgButtonChecked(hWnd, IDC_NEGATE_Y) != 0
                            );
                }
            }
            break;

        case IDC_IMPORT_VALIDATED:
            {
                strcpy(buffer,"ground.xml");
                bool fileSelected = DoGetOpenFileName(hWnd, buffer, sizeof(buffer) / sizeof(*buffer));
                if(fileSelected)
                {
                    DoImport(buffer, theGroundExport().iScale->GetFVal(),
                        theGroundExport().iAnchorHeight->GetFVal()
                        );
                }
            }
            break;
        }
        break;

    default:
        return FALSE;
    }
    return TRUE;
}



//--- GroundExport -------------------------------------------------------
GroundExport::GroundExport()
{
    iu = NULL;
    ip = NULL;    
    hPanel = NULL;
    _storedScale = 1.f;
    _storedMaxSlope = 2.f;
    _storedMaxError = 4.f;
    _storedMaxScale = 50.f;
    _storedRange = 1000000.f;
    _storedAnchorHeight = 5.f;
    tSigned32 i;
//    IDC_MATERIALIDS
//    IDC_SETTERRAINLAYER
//    IDC_ADD_ANCHORS
//    IDC_PROCESS_CONNECTIONS
    for(i = 0; i <= 3; ++i)
    {
        _storedOptions[i] = BST_CHECKED;
    }
//    IDC_ADD_MAPPING
//    IDC_TOKENISE
//    IDC_FILTER_FACES_BY_SLOPE
//    IDC_OPTIMISE
//    IDC_SAVE_SOURCE_CONTENT
    for(; i <= 8; ++i)
    {
        _storedOptions[i] = BST_UNCHECKED;
    }
//    IDC_PREVALIDATE
    _storedOptions[i++] = BST_CHECKED;
//    IDC_LOG_ERRORS
    _storedOptions[i++] = BST_UNCHECKED;
//    IDC_NEGATE_Y
    _storedOptions[i++] = BST_UNCHECKED;
}

GroundExport::~GroundExport()
{
}

void GroundExport::SelectionSetChanged(Interface *ip,IUtil *iu)
{
    char s[50]; sprintf(s,"%i Object(s) Selected",ip->GetSelNodeCount());
    SetWindowText(GetDlgItem(hPanel,IDC_SEL),s);
}

void GroundExport::BeginEditParams(Interface *ip,IUtil *iu) 
{
    this->iu = iu;
    this->ip = ip;

    hPanel = ip->AddRollupPage(
        hInstance,
        MAKEINTRESOURCE(IDD_UV_PANEL),
        GroundExportDlgProc,
        GetString(IDS_PARAMS),
        0);
}

void GroundExport::EndEditParams(Interface *ip,IUtil *iu) 
{
    this->iu = NULL;
    this->ip = NULL;
    ip->DeleteRollupPage(hPanel);
    hPanel = NULL;
}

void GroundExport::Init(HWND hWnd)
{
    iScale = GetISpinner(GetDlgItem(hWnd, IDC_SCALE_SPINNER));
	iScale->SetLimits(0.00001f, 1000000.f, FALSE);
	iScale->SetValue(_storedScale, FALSE);
	iScale->SetScale(10.f);
	iScale->LinkToEdit(GetDlgItem(hWnd, IDC_SCALE), EDITTYPE_POS_FLOAT);

    iMaxSlope = GetISpinner(GetDlgItem(hWnd, IDC_MAX_SLOPE_SPINNER));
	iMaxSlope->SetLimits(0.f, 1000.f, FALSE);
	iMaxSlope->SetValue(_storedMaxSlope, FALSE);
	iMaxSlope->SetScale(0.5f);
	iMaxSlope->LinkToEdit(GetDlgItem(hWnd, IDC_MAX_SLOPE), EDITTYPE_POS_FLOAT);

    iMaxError = GetISpinner(GetDlgItem(hWnd, IDC_MAX_ERROR_SPINNER));
	iMaxError->SetLimits(0.f, 1000000.f, FALSE);
	iMaxError->SetValue(_storedMaxError, FALSE);
	iMaxError->SetScale(0.5f);
	iMaxError->LinkToEdit(GetDlgItem(hWnd, IDC_MAX_ERROR), EDITTYPE_POS_FLOAT);

    iMaxScale = GetISpinner(GetDlgItem(hWnd, IDC_MAX_SCALE_SPINNER));
	iMaxScale->SetLimits(1.f, 1000000.f, FALSE);
	iMaxScale->SetValue(_storedMaxScale, FALSE);
	iMaxScale->SetScale(10.f);
	iMaxScale->LinkToEdit(GetDlgItem(hWnd, IDC_MAX_SCALE), EDITTYPE_POS_INT);

    iRange = GetISpinner(GetDlgItem(hWnd, IDC_RANGE_SPINNER));
	iRange->SetLimits(0.f, 1000000.f, FALSE);
	iRange->SetValue(_storedRange, FALSE);
	iRange->SetScale(100000.f);
	iRange->LinkToEdit(GetDlgItem(hWnd, IDC_RANGE), EDITTYPE_POS_INT);

    iAnchorHeight = GetISpinner(GetDlgItem(hWnd, IDC_ANCHOR_HEIGHT_SPINNER));
	iAnchorHeight->SetLimits(0.00001f, 10000.f, FALSE);
	iAnchorHeight->SetValue(_storedAnchorHeight, FALSE);
	iAnchorHeight->SetScale(1.f);
	iAnchorHeight->LinkToEdit(GetDlgItem(hWnd, IDC_ANCHOR_HEIGHT), EDITTYPE_POS_FLOAT);

    CheckDlgButton(hWnd, IDC_MATERIALIDS, _storedOptions[0]);
    CheckDlgButton(hWnd, IDC_SETTERRAINLAYER, _storedOptions[1]);
    CheckDlgButton(hWnd, IDC_ADD_ANCHORS, _storedOptions[2]);
    CheckDlgButton(hWnd, IDC_PROCESS_CONNECTIONS, _storedOptions[3]);
    CheckDlgButton(hWnd, IDC_ADD_MAPPING, _storedOptions[4]);
    CheckDlgButton(hWnd, IDC_TOKENISE, _storedOptions[5]);
    CheckDlgButton(hWnd, IDC_FILTER_FACES_BY_SLOPE, _storedOptions[6]);
    CheckDlgButton(hWnd, IDC_OPTIMISE, _storedOptions[7]);
    CheckDlgButton(hWnd, IDC_SAVE_SOURCE_CONTENT, _storedOptions[8]);
    CheckDlgButton(hWnd, IDC_PREVALIDATE, _storedOptions[9]);
    CheckDlgButton(hWnd, IDC_LOG_ERRORS, _storedOptions[10]);
    CheckDlgButton(hWnd, IDC_NEGATE_Y, _storedOptions[11]);
}

void GroundExport::Destroy(HWND hWnd)
{
    _storedScale = iScale->GetFVal();
	ReleaseISpinner(iScale);
    _storedMaxSlope = iMaxSlope->GetFVal();
	ReleaseISpinner(iMaxSlope);
    _storedMaxError = iMaxError->GetFVal();
	ReleaseISpinner(iMaxError);
    _storedMaxScale = iMaxScale->GetIVal();
	ReleaseISpinner(iMaxScale);
    _storedRange = iRange->GetIVal();
	ReleaseISpinner(iRange);
    _storedAnchorHeight = iAnchorHeight->GetFVal();
	ReleaseISpinner(iAnchorHeight);

    _storedOptions[0] = IsDlgButtonChecked(hWnd, IDC_MATERIALIDS);
    _storedOptions[1] = IsDlgButtonChecked(hWnd, IDC_SETTERRAINLAYER);
    _storedOptions[2] = IsDlgButtonChecked(hWnd, IDC_ADD_ANCHORS);
    _storedOptions[3] = IsDlgButtonChecked(hWnd, IDC_PROCESS_CONNECTIONS);
    _storedOptions[4] = IsDlgButtonChecked(hWnd, IDC_ADD_MAPPING);
    _storedOptions[5] = IsDlgButtonChecked(hWnd, IDC_TOKENISE);
    _storedOptions[6] = IsDlgButtonChecked(hWnd, IDC_FILTER_FACES_BY_SLOPE);
    _storedOptions[7] = IsDlgButtonChecked(hWnd, IDC_OPTIMISE);
    _storedOptions[8] = IsDlgButtonChecked(hWnd, IDC_SAVE_SOURCE_CONTENT);
    _storedOptions[9] = IsDlgButtonChecked(hWnd, IDC_PREVALIDATE);
    _storedOptions[10] = IsDlgButtonChecked(hWnd, IDC_LOG_ERRORS);
    _storedOptions[11] = IsDlgButtonChecked(hWnd, IDC_NEGATE_Y);
}


BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG reason,LPVOID lpvReserved)
{
    hInstance = hinstDLL;

    if (!controlsInit)
    {
        controlsInit = TRUE;
        InitCustomControls(hInstance);
        InitCommonControls();
    }

    switch(reason)
    {
    case DLL_PROCESS_ATTACH:
        if(!nSingletons::isInitialised_StandAlone())
        {
            nSingletons::init_StandAlone();
            nSingletons::pathEngineI().setErrorHandler(&theGroundExport().errorHandler);
        }
        break;
    case DLL_PROCESS_DETACH:
        if(nSingletons::isInitialised_StandAlone())
        {
            nSingletons::shutDown_StandAlone();
        }
        break;
    }
    return true;
}

