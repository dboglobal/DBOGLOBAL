
#include "MessageBox.h"
#include "LoadContentProcessing3D.h"
#include "FileOutputStream.h"
#include "i_pathengine.h"
#include "i_contentprocessing3d.h"
#include <windows.h>
#include <vector>

class cErrorHandler : public iErrorHandler
{
    cErrorHandler(){}
public:
    eAction handle(const char* error_type, const char* error_string, const char *const* attributes)
    {
        ShowMessageBox(error_type,error_string);
        return CONTINUE;
    }
    static cErrorHandler& refInstance()
    {
        static cErrorHandler the_instance;
        return the_instance;
    }
};


class cSolidObjects : public iSolidObjects
{
    std::vector<tSigned32> _sizes;
    std::vector<const tSigned32*> _dataPointers;

public:

    void add(tSigned32 size, const tSigned32* dataPointer)
    {
        _sizes.push_back(size);
        _dataPointers.push_back(dataPointer);
    }

// interface to iSolidObjects

    tSigned32 numberOfConvexSolids() const
    {
        return static_cast<tSigned32>(_sizes.size());
    }
    tSigned32 convexSolid_Points(tSigned32 convexSolidIndex) const
    {
        return _sizes[convexSolidIndex];
    }
    void
    convexSolid_Point(tSigned32 convexSolidIndex, tSigned32 pointIndex, tSigned32& x, tSigned32& y, tSigned32& z) const
    {
        x = _dataPointers[convexSolidIndex][pointIndex * 3];
        y = _dataPointers[convexSolidIndex][pointIndex * 3 + 1];
        z = _dataPointers[convexSolidIndex][pointIndex * 3 + 2];
    }
    tSigned32 convexSolid_Attribute(tSigned32 convexSolidIndex, tSigned32 attributeIndex) const
    {
        return -1;
    }
};

int APIENTRY
WinMain(HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nCmdShow
        )
{
  // load the 3D content processing DLL and obtain the root interface pointer

    iContentProcessing3D* cp3D = LoadContentProcessing3D("ContentProcessing3D.dll", &cErrorHandler::refInstance());

  // check if the dll loaded is compatible with the headers used for compilation

    if(cp3D->getInterfaceMajorVersion() != CONTENT_PROCESSING_3D_INTERFACE_MAJOR_VERSION
        ||
        cp3D->getInterfaceMinorVersion() < CONTENT_PROCESSING_3D_INTERFACE_MINOR_VERSION)
    {
        cErrorHandler::refInstance().handle("Fatal", "LoadPathEngine: 3D content processing dll version is incompatible with headers used for compilation.",0);
        return 0;
    }

  // the result of the content processing will be sent directly to this output stream

    cFileOutputStream outputStream("3DContentProcessingResult.tok");

  // a very basic 3D environment to process

    cSolidObjects solidObjects;
    {
        // add a box from 0,0,0 to 100,100,100
        tSigned32 pointData[] =
        {
            0, 0, 0, // x, y, z
            100, 0, 0,
            0, 100, 0,
            100, 100, 0,
            0, 0, 100,
            100, 0, 100,
            0, 100, 100,
            100, 100, 100,
        };
        solidObjects.add(sizeof(pointData) / sizeof(*pointData) / 3, pointData);
    }
    {
        // add a pyramid sunk into the centre of the box
        tSigned32 pointData[] =
        {
            20, 20, 100, // x, y, z
            80, 20, 100,
            20, 80, 100,
            80, 80, 100,
            50, 50, 120,
        };
        solidObjects.add(sizeof(pointData) / sizeof(*pointData) / 3, pointData);
    }

  // when reporting any issues in the 3D content processing the following call can be used
  // to generate a 'snapshot' of the exact source data being processed

//    {
//        cFileOutputStream snapshotOS("3DContentProcessingSnapshot.xml");
//        cp3D->saveSnapshot(0, 0, &solidObjects, 40, 10, 20, 0, "xml", &snapshotOS);
//    }

  // run the content processing

    cp3D->buildAndSaveGround(
          // parameters controlling the actual 3D content process
            0, 0, // no mesh objects
            &solidObjects,
            40, // extrudeHeight
            10, // maxStepHeight
            20, // maxSlope (20 unit rise for 100 units horizontally)
            0, // no additional options
            0,  // no progress call back
          // parameters controlling ground mesh output
            "tok", // output format
            true, // include mapping to 2D
            &outputStream
            );

    ShowMessageBox("'run3dcontentprocessing' example project", "3D content processing completed.");

  // the DLL will be unloaded on application shutdown, but could also be explicitly unloaded earlier, if desired

    return 0;
}
