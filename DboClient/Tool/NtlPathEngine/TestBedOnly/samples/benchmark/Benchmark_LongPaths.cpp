
#include "Benchmark_LongPaths.h"
#include "i_pathengine.h"
#include "i_testbed.h"
#include <time.h>
#include <vector>
#include <sstream>

static void
FindPaths(iMesh* mesh, iShape* shape, const std::vector<cPosition>& points)
{
    size_t i;
    for(i = 0; i < points.size(); i++)
    {
        size_t j;
        for(j = i + 1; j < points.size(); j++)
        {
            iPath* path = mesh->findShortestPath(shape, 0, points[i], points[j]);
            delete path;
        }
    }
}

void
GetPathStats(
        iMesh* mesh, iShape* shape,
        const std::vector<cPosition>& points,
        long& totalPaths, long& totalPoints,
        long& numberFailed
        )
{
    totalPaths = 0;
    totalPoints = 0;
    numberFailed = 0;
    size_t i;
    for(i = 0; i < points.size(); i++)
    {
        size_t j;
        for(j = i + 1; j < points.size(); j++)
        {
            iPath* path = mesh->findShortestPath(shape, 0, points[i], points[j]);
            if(path)
            {
                totalPoints += path->size();
                delete path;
            }
            else
            {
                numberFailed++;
            }
            totalPaths++;
        }
    }
}

void
Benchmark_LongPaths(
        iTestBed* testBed,
        iMesh* mesh,
        iShape* shape,
        std::ofstream& os
        )
{
    clock_t start, finish;
    double  duration;
    std::vector<std::string> text;
    std::vector<cPosition> points(60);

    {
        size_t i = 0;
        while(i < points.size())
        {
            cPosition randomPoint = mesh->generateRandomPosition();
            if(randomPoint.cell != -1 && !mesh->testPointCollision(shape, 0, randomPoint))
            {
                points[i++] = randomPoint;
            }
        }
    }

    start = clock();
    FindPaths(mesh, shape, points);
    finish = clock();

    duration = static_cast<double>(finish - start) / CLOCKS_PER_SEC;

    long totalPaths, totalPoints, numberFailed;
    GetPathStats(mesh, shape, points, totalPaths, totalPoints, numberFailed);

    {
        std::ostringstream os;
        os << "failed paths = " << numberFailed;
        text.push_back(os.str());
    }
    os << "(failed paths = " << numberFailed << ", total points = " << totalPoints << ")\n";
    long successfulPaths = totalPaths - numberFailed;
    if(successfulPaths)
    {
        std::ostringstream os;
        os << "successful paths = " << successfulPaths;
        os << ", total points = " << totalPoints;
        os << ", average points = " << static_cast<double>(totalPoints) / successfulPaths;
        text.push_back(os.str());
    }
    {
        std::ostringstream os;
        os << "total time = " << duration << ", average time = " << duration / totalPaths;
        text.push_back(os.str());
    }
    os << "average long path query time: " << duration / totalPaths << '\n';
    testBed->setMesh(mesh);
    testBed->zoomExtents();

    tSigned32 point1 = 0;
    tSigned32 point2 = 1;

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
        testBed->printTextLine(10, "Long paths benchmark");

        testBed->setColour("green");
        {
            tSigned32 i;
            for(i = 0; i < 40; i++)
            {
                iPath* path = mesh->findShortestPath(shape, 0, points[point1], points[point2]);
                testBed->drawPath(path);
                delete path;
                ++point2;
                if(point2 == static_cast<tSigned32>(points.size()))
                {
                    ++point1;
                    point2 = point1 + 1;
                    if(point2 == static_cast<tSigned32>(points.size()))
                    {
                        point1 = 0;
                        point2 = point1 + 1;
                    }
                }
            }
        }

        testBed->update();
    }

    testBed->releaseMesh();
}
