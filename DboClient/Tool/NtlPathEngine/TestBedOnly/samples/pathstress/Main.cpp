#include "entrypoint.h"
#include "error.h"
#include "GenerateAndSaveRandomSeed.h"
#include "MessageBox.h"
#include <vector>
#include <string>

void DoPathStress( iTestBed* testbed,
                   iMesh* mesh,
                   iShape* agentShape,
                   iCollisionContext* context,
                   const cPosition& start, const cPosition& goal)
{
    iPath* path = mesh->findShortestPath(agentShape, context, start, goal);
    if(mesh->testPointCollision(agentShape, context, goal))
    {
        test(path == 0);
        test(mesh->testLineCollision(agentShape, context, start, goal) == true);
        {
            long endCell;
            bool collided = mesh->testLineCollision_XY(agentShape, context, start, goal.x, goal.y, endCell);
            test(collided || endCell != goal.cell);
            long endCell2;
            iCollisionInfo* info = mesh->firstCollision(agentShape, context, start, goal.x, goal.y, endCell2);
            test(collided == (info != 0));
            if(!collided)
            {
                test(endCell2 == endCell);
            }
            delete info;
        }
        return;
    }

    if(!path)
    {
        return;
    }

    testbed->setColour("green");
    testbed->drawPath(path);

    long i;
    for(i = 0; i + 1 < path->size(); i++)
    {
        cPosition segmentStart = path->position(i);
        cPosition segmentEnd = path->position(i + 1);
        test(segmentStart != segmentEnd);
        test(mesh->testLineCollision(agentShape, context, segmentStart, segmentEnd) == false);
        test(mesh->testLineCollision(agentShape, context, segmentEnd, segmentStart) == false);
        {
            long endCell;
            bool collided = mesh->testLineCollision_XY(agentShape, context, segmentStart, segmentEnd.x, segmentEnd.y, endCell);
            test(!collided && endCell == segmentEnd.cell);
            iCollisionInfo* info = mesh->firstCollision(agentShape, context, segmentStart, segmentEnd.x, segmentEnd.y, endCell);
            test(info == 0 && endCell == segmentEnd.cell);
        }
    }
    delete path;
}

void DrawContext(iTestBed* testbed, iCollisionContext* context)
{
    long i;
    for(i = 0; i < context->getNumberOfAgents(); i++)
    {
        iAgent* agent = context->getAgent(i);
        testbed->drawAgent(agent, 40);
    }
}

void DoPathIterations(
                   iTestBed* testbed,
                   iMesh* mesh,
                   iShape* agentShape,
                   iCollisionContext* baseObstaclesContext,
                   iCollisionContext* context,
                   long pathIterations)
{
    do
    {
        testbed->setColourRGB(0.0f,0.0f,0.85f);
        testbed->drawMesh();
        testbed->setColour("blue");
        testbed->drawMeshEdges();
        testbed->setColour("red");
        DrawContext(testbed, baseObstaclesContext);
        testbed->setColour("orange");
        DrawContext(testbed, context);

        long i;
        for(i = 0; i < 10; i++)
        {
            if(!pathIterations)
            {
                break;
            }
            cPosition start;
            do
            {
                start = mesh->generateRandomPosition();
            }
            while(start.cell == -1 || mesh->testPointCollision(agentShape, context, start));

            cPosition goal;
            do
            {
                goal = mesh->generateRandomPosition();
            }
            while(goal.cell == -1);

            DoPathStress(testbed, mesh, agentShape, context, start, goal);
            pathIterations--;
        }

        testbed->update();    
    }
    while(pathIterations);
}

iCollisionContext* InitContext(iMesh* mesh, const std::vector<iShape*>& shapes, std::vector<iAgent*>& agents)
{
    iCollisionContext* result = mesh->newContext();
    cPosition randomPosition;
    do
    {
        randomPosition = mesh->generateRandomPosition();
    }
    while(randomPosition.cell == -1);
    size_t i;
    for(i = 0; i < shapes.size(); i++)
    {
        iAgent* placed = mesh->placeAgent(shapes[i], randomPosition);
        result->addAgent(placed);
        agents[i] = placed;
    }
    return result;
}
void DeleteContextAndAgents(iCollisionContext* context)
{
    while(context->getNumberOfAgents())
    {
        iAgent* agent = context->getAgent(0);
        delete agent;
    }
    delete context;
}

void DoDynamicObstacleIterations(
                   iTestBed* testbed,
                   iMesh* mesh,
                   iShape* agentShape,
                   iCollisionContext* baseObstaclesContext,
                   const std::vector<iShape*>& dynamicObstacleShapes,
                   long dynamicObstacleIterations,
                   long pathIterations)
{
    long dynamicObstacles = dynamicObstacleShapes.size();
    std::vector<cPosition> positions(dynamicObstacles);
    std::vector<iAgent*> agents(dynamicObstacles);
    iCollisionContext* context = InitContext(mesh, dynamicObstacleShapes, agents);
    long i;
    for(i = 0; i < dynamicObstacleIterations; i++)
    {
        long j;
        for(j = 0; j < dynamicObstacles; j++)
        {
            cPosition randomPosition;
            do
            {
                randomPosition = mesh->generateRandomPosition();
            }
            while(randomPosition.cell == -1);
            positions[j] = randomPosition; // the positions are recorded for debugging purposes
            agents[j]->moveTo(randomPosition);
        }

        DoPathIterations(testbed, mesh, agentShape, baseObstaclesContext, context, pathIterations);
    }
    DeleteContextAndAgents(context);
}

void RunStressTest(iPathEngine* pathengine, iTestBed* testbed,
                   const std::string& meshName,
                   iShape* agentShape,
                   const std::vector<iShape*>& baseObstacleShapes,
                   const std::vector<iShape*>& dynamicObstacleShapes,
                   long baseObstacleIterations,
                   long dynamicObstacleIterations,
                   long pathIterations,
                   const char** collisionPreprocessAttributes,
                   const char** loadOptions
                   )
{
    char* meshBuffer;
    unsigned long meshBufferSize;
    {
        std::string meshPath = "..\\resource\\meshes\\" + meshName + ".tok";
        meshBuffer = testbed->loadBinary(meshPath.c_str(), meshBufferSize);
    }

    long baseObstacles = baseObstacleShapes.size();
    std::vector<cPosition> positions(baseObstacles);
    long i;
    for(i = 0; i < baseObstacleIterations; i++)
    {
        iMesh* mesh = pathengine->loadMeshFromBuffer("tok", meshBuffer, meshBufferSize, loadOptions);

        iCollisionContext* context = mesh->newContext();
        long j;
        for(j = 0; j < baseObstacles; j++)
        {
            cPosition randomPosition;
            do
            {
                randomPosition = mesh->generateRandomPosition();
            }
            while(randomPosition.cell == -1);
            iAgent* agent = mesh->placeAgent(baseObstacleShapes[j], randomPosition);
            context->addAgent(agent);
            positions[j] = randomPosition; // recorded for debugging purposes
        }

        mesh->burnContextIntoMesh(context);
        mesh->generateCollisionPreprocessFor(agentShape, collisionPreprocessAttributes);
        mesh->generatePathfindPreprocessFor(agentShape, 0);

        testbed->setMesh(mesh);
        if(i == 0)
        {
            testbed->zoomExtents();
        }

        DoDynamicObstacleIterations(testbed, mesh, agentShape, context, dynamicObstacleShapes, dynamicObstacleIterations, pathIterations);

        delete mesh;
    }

    testbed->freeBuffer(meshBuffer);
}


void demo(iPathEngine* pathengine, iTestBed* testbed)
{
    iShape* shape_Square20;
    {
        long array[]=
        {
            -20, 20,
            20, 20,
            20, -20,
            -20, -20,
        };
        shape_Square20 = pathengine->newShape(sizeof(array)/sizeof(*array)/2, array);
    }
    iShape* shape_Octagon8_20;
    {
        long array[]=
        {
            8,20,
            20,8,
            20,-8,
            8,-20,
            -8,-20,
            -20,-8,
            -20,8,
            -8,20,
        };
        shape_Octagon8_20 = pathengine->newShape(sizeof(array)/sizeof(*array)/2,array);
    }
    iShape* shape_Swarmer;
    {
        long array[]=
        {
            0,16,
            14,-8,
            -14,-8,
        };
        shape_Swarmer = pathengine->newShape(sizeof(array)/sizeof(*array)/2,array);
    }
    iShape* shape_Square60;
    {
        long array[]=
        {
            -60,60,
            60,60,
            60,-60,
            -60,-60,
        };
        shape_Square60 = pathengine->newShape(sizeof(array)/sizeof(*array)/2,array);
    }

    const char* attributes[3];
    attributes[0] = "split_with_circumference_below";
    attributes[1] = "2000";
    attributes[2] = 0;

    std::vector<iShape*> baseObstacles;
    baseObstacles.push_back(shape_Square60);
    baseObstacles.push_back(shape_Square60);
    baseObstacles.push_back(shape_Square60);
    baseObstacles.push_back(shape_Square60);
    baseObstacles.push_back(shape_Square60);
    baseObstacles.push_back(shape_Swarmer);
    baseObstacles.push_back(shape_Swarmer);
    baseObstacles.push_back(shape_Swarmer);
    baseObstacles.push_back(shape_Swarmer);
    baseObstacles.push_back(shape_Swarmer);
    baseObstacles.push_back(shape_Swarmer);
    baseObstacles.push_back(shape_Square20);
    baseObstacles.push_back(shape_Square20);
    baseObstacles.push_back(shape_Octagon8_20);
    baseObstacles.push_back(shape_Octagon8_20);
    baseObstacles.push_back(shape_Octagon8_20);
    baseObstacles.push_back(shape_Octagon8_20);
    std::vector<iShape*> dynamicObstacles;
    dynamicObstacles.push_back(shape_Square60);
    dynamicObstacles.push_back(shape_Swarmer);
    dynamicObstacles.push_back(shape_Octagon8_20);

    ShowMessageBox("Path stress test",
        "A random seed will be generated and saved to the file 'savedRandomSeed.txt', "
        "in the bin directory of the release.\n"
        "If a test fails, or if PathEngine crashes or hangs, please report this value.");


    while(1)
    {
        unsigned long seed = GenerateAndSaveRandomSeed();
        pathengine->setRandomSeed(seed);

        RunStressTest(pathengine, testbed, "mesh1", shape_Swarmer, baseObstacles, dynamicObstacles,
                        10, // baseObstacleIterations
                        5, // dynamicObstacleIterations
                        100, // pathIterations
                        0, 0
                        );
        RunStressTest(pathengine, testbed, "mesh1", shape_Swarmer, baseObstacles, dynamicObstacles,
                        10, // baseObstacleIterations
                        5, // dynamicObstacleIterations
                        100, // pathIterations
                        attributes, 0
                        );
        RunStressTest(pathengine, testbed, "mesh1", shape_Octagon8_20, baseObstacles, dynamicObstacles,
                        10, // baseObstacleIterations
                        5, // dynamicObstacleIterations
                        100, // pathIterations
                        0, 0
                        );
        RunStressTest(pathengine, testbed, "mesh1", shape_Octagon8_20, baseObstacles, dynamicObstacles,
                        10, // baseObstacleIterations
                        5, // dynamicObstacleIterations
                        100, // pathIterations
                        attributes, 0
                        );
    }
}
