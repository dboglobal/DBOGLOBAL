
#include "WaitForFrame.h"
#include <windows.h>
#include <time.h>

static clock_t gTimeLastFrame;
static bool gFirstFrame = true;

void sleep(clock_t wait)
{
   clock_t goal;
   goal = wait + clock();
   while(goal > clock())
   {
       sleep(0);
   }
}



bool
WaitForFrame()
{
    clock_t timeNow = clock();
    if(gFirstFrame)
    {
        gFirstFrame = false;
        gTimeLastFrame = timeNow;
        return false;
    }

    clock_t target = gTimeLastFrame + static_cast<clock_t>(1 / 25.0 * CLOCKS_PER_SEC);
    if(timeNow >= target)
    {
        gTimeLastFrame = timeNow;
        return true;
    }

    while(clock() < target)
    {
    }
    gTimeLastFrame = target;
    return false;
}
