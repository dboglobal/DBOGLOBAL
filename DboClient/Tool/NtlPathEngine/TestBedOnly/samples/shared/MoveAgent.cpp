
#include "MoveAgent.h"
#include "i_testbed.h"
#include "i_pathengine.h"
#include "Sliding.h"
#include <math.h>

void TurnAgentUnderMouseControl(iTestBed* testbed, float& agent_heading)
{
    long dx, dy;
    testbed->getMouseScreenDelta(dx, dy);
    agent_heading += dx * 0.016f;
    agent_heading = fmod(agent_heading, 6.2856f);
}

void TurnAgentUnderMouseControl(iTestBed* testbed, float& agent_heading, float& camera_elevation)
{
    long dx, dy;
    testbed->getMouseScreenDelta(dx, dy);
    agent_heading += dx * 0.016f;
    agent_heading = fmod(agent_heading,6.2856f);
    camera_elevation += dy * 0.016f;
    if(camera_elevation > 1.5f)
    {
        camera_elevation = 1.5f;
    }
    else
    if(camera_elevation < -0.5f)
    {
        camera_elevation = -0.5f;
    }
}

void
MoveAgentUnderKeyControl(iTestBed *testbed, iPathEngine *pathengine,
                                iAgent *agent, float agent_heading, 
                                iCollisionContext *context,
                                float speed,
                                float& precisionX, float& precisionY)
{
    double sinOf = sin(double(agent_heading));
    double cosOf = cos(double(agent_heading));

    double dx = precisionX;
    double dy = precisionY;

// setup the desired movement vector in <dx,dy> from agent heading and a combination of keys pressed
    if(testbed->getKeyState("W"))
    {
    // forward
        dx += sinOf * speed;
        dy += cosOf * speed;
    }
    if(testbed->getKeyState("S"))
    {
    // backward (half speed)
        dx -= sinOf * speed / 2;
        dy -= cosOf * speed / 2;
    }
    if(testbed->getKeyState("A"))
    {
    // left (half speed)
        dx -= cosOf * speed / 2;
        dy += sinOf * speed / 2;
    }
    if(testbed->getKeyState("D"))
    {
    // right (half speed)
        dx += cosOf * speed / 2;
        dy -= sinOf * speed / 2;
    }

    long dxL = static_cast<long>(dx);
    long dyL = static_cast<long>(dy);

    if(dxL == 0 && dyL == 0)
    {
    // no movement after approximation
        precisionX = static_cast<float>(dx);
        precisionY = static_cast<float>(dy);
        return;
    }

    cPosition current = agent->getPosition();
    cPosition target;
    target.x = current.x + dxL;
    target.y = current.y + dyL;

    iCollisionInfo* collisionInfo = agent->firstCollisionTo(context, target.x, target.y, target.cell);
    int trys = 0;
    while(collisionInfo && trys < 2)
    {
        long coords[4];
        collisionInfo->getCollidingLine(coords);
        delete collisionInfo;

        SlideAgainst(coords, current.x, current.y, dx, dy);
        dxL = static_cast<long>(dx);
        dyL = static_cast<long>(dy);

        target.x = current.x + dxL;
        target.y = current.y + dyL;
        collisionInfo = agent->firstCollisionTo(context, target.x, target.y, target.cell);
        trys++;
    }
    if(collisionInfo)
    {
    // failed to find a non-colliding movement vector
        delete collisionInfo;
        return;
    }
    else
    {
    // succeeded
        precisionX = static_cast<float>(dx - dxL);
        precisionY = static_cast<float>(dy - dyL);
        agent->moveTo(target);
        return;
    }
}

void MoveAgentUnderKeyControl(    iTestBed *testbed, iPathEngine *pathengine,
                                iAgent *agent, float agent_heading, 
                                iCollisionContext *context,
                                float speed)
{
    float dummyPrecisionX = 0;
    float dummyPrecisionY = 0;
    MoveAgentUnderKeyControl(testbed, pathengine, agent, agent_heading, context, speed, dummyPrecisionX, dummyPrecisionY);
}

void
GetMovementVector(iTestBed* testbed, float& x, float& y)
{
    x = 0.0f;
    y = 0.0f;
    if(testbed->getKeyState("W"))
    {
    // forward
        y += 1.0f;
    }
    if(testbed->getKeyState("S"))
    {
    // backward (half speed)
        y -= 0.5f;
    }
    if(testbed->getKeyState("A"))
    {
    // left (half speed)
        x -= 0.5f;
    }
    if(testbed->getKeyState("D"))
    {
    // right (half speed)
        x += 0.5f;
    }
}

void
MoveAgent(iPathEngine* pathengine, iCollisionContext* context,
                      iAgent* agent,
                      float agentHeading, float localX, float localY,
                      float& precisionX, float& precisionY
                      )
{
    double sinOf = sin(double(agentHeading));
    double cosOf = cos(double(agentHeading));
    double dx = precisionX + sinOf * localY + cosOf * localX;
    double dy = precisionY + cosOf * localY - sinOf * localX;
    long dxL = static_cast<long>(dx);
    long dyL = static_cast<long>(dy);

    if(dxL == 0 && dyL == 0)
    {
    // no movement after approximation
        precisionX = static_cast<float>(dx);
        precisionY = static_cast<float>(dy);
        return;
    }

    cPosition current = agent->getPosition();
    cPosition target;
    target.x = current.x + dxL;
    target.y = current.y + dyL;

    iCollisionInfo* collisionInfo = agent->firstCollisionTo(context, target.x, target.y, target.cell);
    int trys = 0;
    while(collisionInfo && trys < 2)
    {
        long coords[4];
        collisionInfo->getCollidingLine(coords);
        delete collisionInfo;

        SlideAgainst(coords, current.x, current.y, dx, dy);
        dxL = static_cast<long>(dx);
        dyL = static_cast<long>(dy);

        target.x = current.x + dxL;
        target.y = current.y + dyL;
        collisionInfo = agent->firstCollisionTo(context, target.x, target.y, target.cell);
        trys++;
    }
    if(collisionInfo)
    {
    // failed to find a non-colliding movement vector
        delete collisionInfo;
    }
    else
    {
    // succeeded
        precisionX = static_cast<float>(dx - dxL);
        precisionY = static_cast<float>(dy - dyL);
        agent->moveTo(target);
    }
}

void
MoveAgent(iPathEngine* pathengine, iCollisionContext* context1, iCollisionContext* context2,
                      iAgent* agent,
                      float agentHeading, float localX, float localY,
                      float& precisionX, float& precisionY
                      )
{
    double sinOf = sin(double(agentHeading));
    double cosOf = cos(double(agentHeading));
    double dx = precisionX + sinOf * localY + cosOf * localX;
    double dy = precisionY + cosOf * localY - sinOf * localX;
    long dxL = static_cast<long>(dx);
    long dyL = static_cast<long>(dy);

    if(dxL == 0 && dyL == 0)
    {
    // no movement after approximation
        precisionX = static_cast<float>(dx);
        precisionY = static_cast<float>(dy);
        return;
    }

    cPosition current = agent->getPosition();
    cPosition target;
    target.x = current.x + dxL;
    target.y = current.y + dyL;

    iCollisionInfo* collisionInfo = agent->firstCollisionTo(context1, target.x, target.y, target.cell);
    int trys = 0;
    while(collisionInfo && trys < 2)
    {
        long coords[4];
        collisionInfo->getCollidingLine(coords);
        delete collisionInfo;

        SlideAgainst(coords, current.x, current.y, dx, dy);
        dxL = static_cast<long>(dx);
        dyL = static_cast<long>(dy);

        target.x = current.x + dxL;
        target.y = current.y + dyL;
        collisionInfo = agent->firstCollisionTo(context1, target.x, target.y, target.cell);
        trys++;
    }
    if(collisionInfo)
    {
    // failed to find a non-colliding movement vector
        delete collisionInfo;
    }
    else
    if(!agent->testCollisionAt(context2, target))
    {
    // succeeded
        precisionX = static_cast<float>(dx - dxL);
        precisionY = static_cast<float>(dy - dyL);
        agent->moveTo(target);
    }
}
