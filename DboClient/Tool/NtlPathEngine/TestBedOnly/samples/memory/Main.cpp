
#include "LoadPathEngine.h"
#include "i_pathengine.h"
#include "LoadBinary.h"
#include "ErrorHandler.h"
#include <sstream>
#include <vector>
#include <windows.h>

void
runBenchmark(
        iPathEngine* pathengine,
        std::string meshName, const char** collisionPreprocessAttributes
        )
{
    std::ostringstream results;

    results << "memory benchmark for '" << meshName << "'" << '\n';

    results << "at start = " << pathengine->totalMemoryAllocated() << '\n';

    iMesh* mesh;
    {
        std::vector<char> buffer;
        std::string fullName = "..\\resource\\meshes\\" + meshName + ".tok";
        LoadBinary(fullName.c_str(), buffer);
        mesh = pathengine->loadMeshFromBuffer("tok", &buffer.front(), buffer.size(), 0);
    }

    results << "mesh loaded = " << pathengine->totalMemoryAllocated() << '\n';

    iShape* shape;
    {
        long array[]=
        {
            0,16,
            14,-8,
            -14,-8,
        };
        shape = pathengine->newShape(sizeof(array) / sizeof(*array) / 2, array);
    }

    mesh->generateCollisionPreprocessFor(shape, collisionPreprocessAttributes);

    results << "with collision preprocess = " << pathengine->totalMemoryAllocated() << '\n';

    mesh->generatePathfindPreprocessFor(shape, 0);

    results << "with pathfind preprocess = " << pathengine->totalMemoryAllocated() << '\n';

    mesh->preprocessGenerationCompleted();

    results << "called preprocessGenerationCompleted = " << pathengine->totalMemoryAllocated() << '\n';

    {
        MessageBox(0, results.str().c_str(), "Results:", MB_OK);
    }

    pathengine->deleteAllObjects();
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
    iPathEngine* pathengine = LoadPathEngine("pathengine", &cErrorHandler::refInstance());
    if(!pathengine)
        return 0;

// check if interface for dll loaded is compatible with the headers used for compilation
    if(pathengine->getInterfaceMajorVersion()!=PATHENGINE_INTERFACE_MAJOR_VERSION
        ||
        pathengine->getInterfaceMinorVersion()<PATHENGINE_INTERFACE_MINOR_VERSION)
    {
        cErrorHandler::refInstance().handle("Fatal","LoadPathEngine: pathengine version is incompatible with headers used for compilation.",0);
        return 0;
    }

    bool memoryTrackingEnabled = false;
    {
        const char *const* attributes = pathengine->getVersionAttributes();
        while(attributes && *attributes)
        {
            if(strcmp(attributes[0], "memory_tracking_enabled") == 0 &&
                strcmp(attributes[1], "true") == 0)
            {
                memoryTrackingEnabled = true;
            }
            attributes += 2;
        }
    }

    if(!memoryTrackingEnabled)
    {
        cErrorHandler::refInstance().handle("Fatal","The pathengine dll loaded is not built with memory handler enabled.",0);
        return 0;
    }

    runBenchmark(pathengine, "mesh1", 0);
    runBenchmark(pathengine, "flatshead_dungeon", 0);
    {
        const char* attributes[3];
        attributes[0] = "split_with_circumference_below";
        attributes[1] = "2000";
        attributes[2] = 0;
        runBenchmark(pathengine, "thainesford", attributes);
    }

    return 0;
}
