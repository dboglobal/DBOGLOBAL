
#include "Benchmark_DynamicObstaclesPathfinding.h"
#include "i_testbed.h"
#include <time.h>
#include <vector>
#include <sstream>

static void
GeneratePositions(
        iMesh* mesh,
        iShape* agentShape,
        cPosition& startPosition,
        std::vector<cPosition>& obstaclePositions,
        std::vector<cPosition>& queryPositions
        )
{
    do
    {
        startPosition = mesh->generateRandomPosition();
    }
    while(mesh->testPointCollision(agentShape, 0, startPosition));
    tSigned32 j;
    for(j = 0; j < static_cast<tSigned32>(obstaclePositions.size()); j++)
    {
        obstaclePositions[j] = mesh->generateRandomPosition();
    }
    for(j = 0; j < static_cast<tSigned32>(queryPositions.size()); j++)
    {
        queryPositions[j] = mesh->generateRandomPosition();
    }
}

static double
AddObstacles(
        iMesh* mesh,
        iShape* obstacleShape,
        const std::vector<cPosition>& obstaclePositions,
        iCollisionContext* context
        )
{
    clock_t start, finish;
    tSigned32 j;
    start = clock();
    for(j = 0; j < static_cast<tSigned32>(obstaclePositions.size()); j++)
    {
        iAgent* agent = mesh->placeAgent(obstacleShape, obstaclePositions[j]);
        context->addAgent(agent);
    }
    finish = clock();
    return static_cast<double>(finish - start) / CLOCKS_PER_SEC;
}

static double
GenerateQueryPositions(
        iMesh* mesh,
        iShape* agentShape,
        iCollisionContext* context,
        std::vector<cPosition>& queryPositions
        )
{
    clock_t start, finish;
    tSigned32 j;
    start = clock();
    for(j = 0; j < static_cast<tSigned32>(queryPositions.size()); j++)
    {
        bool blocked = mesh->testPointCollision(agentShape, context, queryPositions[j]);
        if(blocked)
        {
            queryPositions[j].cell = -1;
        }
    }
    finish = clock();
    return static_cast<double>(finish - start) / CLOCKS_PER_SEC;
}

static double
FindPaths(
        iMesh* mesh,
        iShape* agentShape,
        iCollisionContext* context,
        const cPosition& startPosition,
        const std::vector<cPosition>& queryPositions,
        std::vector<iPath*>& pathResults
        )
{
    clock_t start, finish;
    tSigned32 j;
    start = clock();
    for(j = 0; j < static_cast<tSigned32>(queryPositions.size()); j++)
    {
        if(queryPositions[j].cell == -1)
        {
            pathResults[j] = 0;
            continue;
        }
        pathResults[j] = mesh->findShortestPath(agentShape, context, startPosition, queryPositions[j]);
    }
    finish = clock();
    return static_cast<double>(finish - start) / CLOCKS_PER_SEC;
}

static void
CleanUp(iCollisionContext* context, const std::vector<iPath*>& pathResults)
{
    while(context->getNumberOfAgents())
    {
        delete context->getAgent(0);
    }
    delete context;
    tSigned32 j;
    for(j = 0; j != static_cast<tSigned32>(pathResults.size()); ++j)
    {
        delete pathResults[j];
    }
}

void
Benchmark_DynamicObstaclesPathfinding(
        iPathEngine* pathEngine,
        iTestBed* testBed,
        iMesh* mesh,
        iShape* agentShape,
        iShape* obstacleShape,
        tSigned32 obstacles, tSigned32 obstacleIterations, tSigned32 pathIterations,
        tUnsigned32 randomSeed, 
        std::ofstream& os
        )
{
    std::vector<std::string> text;
    clock_t start, finish;

    double timeToPlace = 0.0;
    double timeUpdateCollisionPreprocess = 0.0;
    double timeUpdatePathfindPreprocess = 0.0;
    double timePointCollision = 0.0;
    double timeFindPath = 0.0;
    tSigned32 totalPathQueries = 0;
    tSigned32 totalPathPoints = 0;
    tSigned32 totalUnreachableTargets = 0;

    std::vector<cPosition> obstaclePositions(obstacles);
    std::vector<cPosition> queryPositions(pathIterations);
    std::vector<iPath*> pathResults(pathIterations);

    pathEngine->setRandomSeed(randomSeed);
    tSigned32 i;
    for(i = 0; i < obstacleIterations; i++)
    {
        cPosition startPosition;
        GeneratePositions(mesh, agentShape, startPosition, obstaclePositions, queryPositions);
        iCollisionContext* context = mesh->newContext();
        timeToPlace += AddObstacles(mesh, obstacleShape, obstaclePositions, context);
 
        start = clock();
        context->updateCollisionPreprocessFor(agentShape);
        finish = clock();
        timeUpdateCollisionPreprocess += static_cast<double>(finish - start) / CLOCKS_PER_SEC;

        timePointCollision += GenerateQueryPositions(mesh, agentShape, context, queryPositions);

        start = clock();
        context->updatePathfindingPreprocessFor(agentShape);
        finish = clock();
        timeUpdatePathfindPreprocess += static_cast<double>(finish - start) / CLOCKS_PER_SEC;

        timeFindPath += FindPaths(mesh, agentShape, context, startPosition, queryPositions, pathResults);

        tSigned32 j;
        for(j = 0; j < pathIterations; j++)
        {
            if(queryPositions[j].cell == -1)
            {
                continue;
            }
            ++totalPathQueries;
            if(pathResults[j] == 0)
            {
                ++totalUnreachableTargets;
            }
            else
            {
                totalPathPoints += pathResults[j]->size();
            }
        }

        CleanUp(context, pathResults);
    }

    if(totalPathQueries == 0)
    {
        os << "!!!Error - no path queries in dynamic obstacles pathfinding benchmark.\n";
        return;
    }

    os << "DynamicObstaclesPathfinding.placement per obstacle: " << timeToPlace / obstacleIterations / obstacles << '\n';
    os << "DynamicObstaclesPathfinding.update collision preprocess: " << timeUpdateCollisionPreprocess / obstacleIterations << '\n';
    os << "DynamicObstaclesPathfinding.point collision per query: " << timePointCollision / obstacleIterations / pathIterations << '\n';
    os << "DynamicObstaclesPathfinding.update pathfind preprocess: " << timeUpdatePathfindPreprocess / obstacleIterations << '\n';
    os << "DynamicObstaclesPathfinding.find path: " << timeFindPath / totalPathQueries << '\n';

    {
        std::ostringstream oss;
        oss << "benchmark stats: totalPathQueries = " << totalPathQueries << ", ";
		oss << "totalUnreachableTargets = " << totalUnreachableTargets << ", ";
		oss << "totalPathPoints = " << totalPathPoints;
        text.push_back(oss.str());
    }

    testBed->setMesh(mesh);
    testBed->zoomExtents();

    tSigned32 obstacleIteration = 0;

    while(!testBed->getKeyState("_SPACE"))
    {
        testBed->setColourRGB(0.0f,0.0f,0.85f);
        testBed->drawMesh();
        testBed->setColour("blue");
        testBed->drawMeshEdges();
        testBed->setColour("white");
        testBed->drawBurntInObstacles();
        testBed->printTextLine(10, "press space for next benchmark, or to finish");
        testBed->printTextLine(15, "(see file 'benchmark_results.txt' for full benchmark results)");
        {
            size_t i = text.size();
            while(i > 0)
            {
                i--;
                testBed->printTextLine(15, text[i].c_str());
            }
        }
        testBed->printTextLine(10, "Dynamic obstacles pathfinding benchmark");

        if(obstacleIteration == 0)
        {
            pathEngine->setRandomSeed(randomSeed);
        }
        ++obstacleIteration;
        if(obstacleIteration == obstacleIterations)
        {
            obstacleIteration = 0;
        }

        {
            cPosition startPosition;
            GeneratePositions(mesh, agentShape, startPosition, obstaclePositions, queryPositions);
            iCollisionContext* context = mesh->newContext();
            AddObstacles(mesh, obstacleShape, obstaclePositions, context);
            context->updateCollisionPreprocessFor(agentShape);
            GenerateQueryPositions(mesh, agentShape, context, queryPositions);
            context->updatePathfindingPreprocessFor(agentShape);
            FindPaths(mesh, agentShape, context, startPosition, queryPositions, pathResults);

            tSigned32 i;

          // draw obstacles in context
            testBed->setColour("orange");
            for(i = 0; i != obstacles; i++)
            {
                testBed->drawAgent(context->getAgent(i), 20);
            }

          // draw paths
            testBed->setColour("green");
            for(i = 0; i != pathIterations; i++)
            {
                testBed->drawPath(pathResults[i]);
            }

            CleanUp(context, pathResults);
        }

        testBed->update();
    }

    testBed->releaseMesh();
}
