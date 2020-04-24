
#include "platform_common/Header.h"
#include "interface/ProgressBar.h"
#include "cTestBed.h"
#include "interface/Singletons_TestBed.h"
#include "libs/TestBed_Application/interface/iGraphicsInterface.h"
#include "common/STL/vector.h"

void
DrawProgressBar(iGraphicsInterface *gip, const char* description, float percent, tSigned32 baseHeight)
{
    float sx, sy, ex, ey;
    sx = 10.f;
    sy = static_cast<float>(baseHeight);
    ex = 630.f; //....... query actual window width
    ey = static_cast<float>(baseHeight + 20);
    cGeometrySet* gs = gip->newGeometrySet();
    iRenderGeometry* rg;
    std::vector<float> pointArray(12, 0);

    pointArray[0] = sx;
    pointArray[1] = sy;
    pointArray[3] = sx;
    pointArray[4] = ey;
    rg = gip->newLine(&pointArray.front(), gs);
    gip->render_ScreenSpace(rg);

    pointArray[0] = sx;
    pointArray[1] = sy;
    pointArray[3] = ex;
    pointArray[4] = sy;
    rg = gip->newLine(&pointArray.front(), gs);
    gip->render_ScreenSpace(rg);

    pointArray[0] = ex;
    pointArray[1] = ey;
    pointArray[3] = sx;
    pointArray[4] = ey;
    rg = gip->newLine(&pointArray.front(), gs);
    gip->render_ScreenSpace(rg);

    pointArray[0] = ex;
    pointArray[1] = ey;
    pointArray[3] = ex;
    pointArray[4] = sy;
    rg = gip->newLine(&pointArray.front(), gs);
    gip->render_ScreenSpace(rg);

    ex = percent * 620 + 10.f;

    pointArray[0] = sx;
    pointArray[1] = sy;
    pointArray[3] = sx;
    pointArray[4] = ey;
    pointArray[6] = ex;
    pointArray[7] = ey;
    pointArray[9] = ex;
    pointArray[10] = sy;
    rg = gip->newPolygon(4, &pointArray.front(), gs);
    gip->render_ScreenSpace(rg);

    rg = gip->newText_8By16(10, baseHeight + 30, description, gs);
    gip->render_ScreenSpace(rg);

    gip->deleteGeometrySet(gs);
}

void
cGIPProgressCallBack::updateProgress(const char* description, float percent)
{
    if(description != _lastDescription || enoughTimePassed())
    {
        DrawProgressBar(nSingletons::testBed().getGIP(), description, percent, 10);
        nSingletons::testBed().update();
        _lastUpdate.read();
        _lastDescription = description;
    }
}
