
#pragma warning(disable : 4786)

#include "Resources.h"
#include "Error.h"
#include "SimpleDOM.h"
#include "Globals.h"
#include "i_pathengine.h"
#include "i_testbed.h"

using std::string;

cResources::cResources(const cSimpleDOM& shapesScript)
{
    long index;
    index = gTestBed->loadSplashImage("demo_begin.png");
    if(index == -1)
    {
        Error("Fatal", "Could not find file 'bin\\demo_begin.png'.");
    }
    assertD(index == SPLASH_BEGIN);
    index = gTestBed->loadSplashImage("demo_died.png");
    if(index == -1)
    {
        Error("Fatal", "Could not file 'bin\\demo_died.png'.");
    }
    assertD(index == SPLASH_DIED);
    index = gTestBed->loadSplashImage("demo_levelcomplete.png");
    if(index == -1)
    {
        Error("Fatal", "Could not file 'bin\\demo_levelcomplete.png'.");
    }
    assertD(index == SPLASH_LEVELCOMPLETE);
    index = gTestBed->loadSplashImage("demo_allcomplete.png");
    if(index == -1)
    {
        Error("Fatal", "Could not file 'bin\\demo_allcomplete.png'.");
    }
    assertD(index == SPLASH_ALLCOMPLETE);
    index = gTestBed->loadSplashImage("demo_camera.png");
    if(index == -1)
    {
        Error("Fatal", "Could not file 'bin\\demo_camera.png'.");
    }
    assertD(index == SPLASH_CAMERA);

    long i;
    for(i = 0; i < static_cast<long>(shapesScript._children.size()); ++i)
    {
        const cSimpleDOM& element = shapesScript._children[i];
        if(element._name != "shape")
        {
            continue;
        }
        string verticesString = element.getAttribute("vertices");
        const char* ptr = verticesString.c_str();
        std::vector<long> coords;
        while(1)
        {
            char* nextPtr;
            coords.push_back(strtol(ptr, &nextPtr, 10));
            assertR(*nextPtr == ',');
            ptr = nextPtr + 1;
            coords.push_back(strtol(ptr, &nextPtr, 10));
            if(*nextPtr == 0)
            {
                break;
            }
            assertR(*nextPtr == ',');
            ptr = nextPtr + 1;
        }
        _shapes.push_back(gPathEngine->newShape(coords.size() / 2, &coords.front()));
    }
}

cResources::~cResources()
{
    size_t i;
    for(i = 0; i < _shapes.size(); ++i)
    {
        delete _shapes[i];
    }
}

