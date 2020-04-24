
#pragma warning(disable : 4786)

#include "EntryPoint.h"

#define extern
#include "Globals.h"
#undef extern

#include "MoveAgent.h"
#include "Error.h"
#include "WaitForFrame.h"
#include "SimpleDOM.h"
#include "LoadWhiteSpaceDelimited.h"
#include "Resources.h"
#include "GameState.h"
#include "GameObject.h"
#include <vector>
#include <math.h>
#include <fstream>

using std::vector;
using std::string;

void
LoadSounds(const cSimpleDOM& sounds)
{
    string pathToSounds = "..\\resource\\sounds\\";
    size_t i;
    for(i = 0; i < sounds._children.size(); i++)
    {
        const cSimpleDOM& sound = sounds._children[i];
        if(sound._name != "sound")
        {
            continue;
        }
        string fileName = pathToSounds;
        fileName += sound.getAttribute("name");
        long buffers = sound.attributeAsLongWithDefault("buffers", 4);
        long index = gTestBed->createSound(fileName.c_str(), buffers);
        if(index == -1)
        {
            const char* attributes[3];
            attributes[0] = "file_name";
            attributes[1] = fileName.c_str();
            attributes[2] = 0;
            Error("Fatal", "Could not find sound file.", attributes);
        }
    }
}

string
PlayLevel(const cSimpleDOM& templates, const cSimpleDOM& script, bool onLastLevel)
{
    gTestBed->update();

    new cGameState(templates, script);

    long i = script.firstChildWithName("initialisation");
    if(i >= 0)
    {
        const cSimpleDOM& initialisationScript = script._children[i];
        size_t j;
        for(j = 0; j < initialisationScript._children.size(); j++)
        {
            gGameState->execute(initialisationScript._children[j]);
        }
    }

    const cSimpleDOM& frameScript = script.refFirstChildWithName("frame");

    string result;
    while(!gGameState->levelCompleted(result))
    {
        gGameState->draw();

        WaitForFrame();

      // tell the testbed to render this frame

        gTestBed->update();

        static bool paused = false;
        if(gTestBed->getKeyState("P"))
        {
            paused = gTestBed->getKeyState("_RSHIFT");
        }

        if(!paused)
        {
            gGameState->update();
        }

    }

    bool pressedSpace = false;
    do
    {
        gGameState->draw();

        if(result == "win")
        {
            if(onLastLevel)
            {
                gTestBed->displaySplashImage_Centred(cResources::SPLASH_ALLCOMPLETE);
            }
            else
            {
                gTestBed->displaySplashImage_Centred(cResources::SPLASH_LEVELCOMPLETE);
            }
        }
        else
        {
            gTestBed->displaySplashImage_Centred(cResources::SPLASH_DIED);
        }

        gTestBed->update();
        const char* keyPressed;
        while(keyPressed = gTestBed->receiveKeyMessage())
        {
            if(!strcmp("d_SPACE", keyPressed))
            {
                pressedSpace = true;
            }
        }
    }
    while(!pressedSpace);

    delete gGameState;
    gGameState = 0;
    return result;
}

void
demo(iPathEngine *pathEngine, iTestBed *testBed)
{
    gPathEngine = pathEngine;
    gTestBed = testBed;

    cSimpleDOM script;
    {
        std::ifstream is("..\\resource\\scripts\\demo_script.txt");
        if(!is.good())
        {
            Error("Fatal", "Could not open demo script ('resource\\scripts\\demo_script.txt').");
        }
        LoadWhiteSpaceDelimited(is, script);
    }
    assertR(script._name == "script");

    long child = script.firstChildWithName("sounds");
    if(child >= 0)
    {
        LoadSounds(script._children[child]);
    }

    cResources resources(script.refFirstChildWithName("shapes"));
    gResources = &resources;

    cSimpleDOM& templates = script.refFirstChildWithName("templates");
    cSimpleDOM& levels = script.refFirstChildWithName("levels");

    assertR(!levels._children.empty());

    {

        bool pressedSpace = false;
        do
        {
            gTestBed->displaySplashImage_Centred(cResources::SPLASH_BEGIN);
            gTestBed->update();
            const char* keyPressed;
            while(keyPressed = gTestBed->receiveKeyMessage())
            {
                if(!strcmp("d_SPACE", keyPressed))
                {
                    pressedSpace = true;
                }
            }
        }
        while(!pressedSpace);
    }


    size_t i = 0;
    while(1)
    {
        bool onLastLevel = (i + 1 == levels._children.size());
        string result = PlayLevel(templates, levels._children[i], onLastLevel);
        if(result == "win")
        {
            i++;
            if(onLastLevel)
            {
                i = 0;
            }
        }
    }
}
