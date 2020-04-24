
#include "entrypoint.h"
#include "Benchmark_LongPaths.h"
#include "Benchmark_DynamicObstaclesPathfinding.h"
#include <time.h>
#include <fstream>
#include <string>
#include <vector>

static iMesh*
LoadMeshAndGeneratePreprocess(
        iPathEngine* pathEngine, iTestBed* testbed,
        iShape* shape,
        const std::string& meshName,
        const char** loadOptions,
        const char** collisionPreprocessAttributes,
        std::ofstream& os
        )
{
    os << "** benchmarks with mesh '" << meshName << "'\n";
    iMesh* mesh;
    clock_t start, finish;
    double  duration;
    {
        char* buffer;
        tUnsigned32 size;
        std::string meshPath = "..\\resource\\meshes\\" + meshName + ".tok";
        buffer = testbed->loadBinary(meshPath.c_str(), size);
        start = clock();
        mesh = pathEngine->loadMeshFromBuffer("tok", buffer, size, loadOptions);
        finish = clock();
        testbed->freeBuffer(buffer);
        duration = static_cast<double>(finish - start) / CLOCKS_PER_SEC;
        os << "loading mesh from buffer: " << duration << " seconds\n";
    }
    {
        std::string message = "generating preprocess for: '" + meshName + "'";
        testbed->setColour("orange");
        testbed->printTextLine(10, message.c_str());
        testbed->update();
    }
    {
        start = clock();
        mesh->generateCollisionPreprocessFor(shape, collisionPreprocessAttributes);
        mesh->generatePathfindPreprocessFor(shape, 0);
        finish = clock();
        duration = static_cast<double>(finish - start) / CLOCKS_PER_SEC;
        os << "generating preprocess: " << duration << " seconds\n";
    }
    return mesh;
}

void
demo(iPathEngine* pathEngine, iTestBed* testBed)
{
    std::ofstream os("benchmark_results.txt");
    iShape* shape;
    {
        tSigned32 array[]=
        {
            -20, 20,
            20, 20,
            20, -20,
            -20, -20,
        };
        shape = pathEngine->newShape(sizeof(array)/sizeof(*array)/2, array);
    }
    iShape* bigShape;
    {
        tSigned32 array[]=
        {
            -100, 100,
            100, 100,
            100, -100,
            -100, -100,
        };
        bigShape = pathEngine->newShape(sizeof(array)/sizeof(*array)/2, array);
    }
    iMesh* mesh;
    mesh = LoadMeshAndGeneratePreprocess(pathEngine, testBed, shape, "mesh1", 0, 0, os);
    Benchmark_LongPaths(testBed, mesh, shape, os);
    delete mesh;
    mesh = LoadMeshAndGeneratePreprocess(pathEngine, testBed, shape, "flatshead_dungeon", 0, 0, os);
    Benchmark_LongPaths(testBed, mesh, shape, os);
    Benchmark_DynamicObstaclesPathfinding(
            pathEngine, testBed, mesh,
            shape, // agent shape
            shape, // obstacle shape
            5,  // number of dynamic obstacles
            40,  // number of collision context iterations
            10,  // number of pathfinding queries per collision context iteration
            333, // random seed
            os
            );
    Benchmark_DynamicObstaclesPathfinding(
            pathEngine, testBed, mesh,
            shape, // agent shape
            shape, // obstacle shape
            10,  // number of dynamic obstacles
            40,  // number of collision context iterations
            10,  // number of pathfinding queries per collision context iteration
            333, // random seed
            os
            );
    Benchmark_DynamicObstaclesPathfinding(
            pathEngine, testBed, mesh,
            shape, // agent shape
            shape, // obstacle shape
            20,  // number of dynamic obstacles
            40,  // number of collision context iterations
            10,  // number of pathfinding queries per collision context iteration
            333, // random seed
            os
            );
    delete mesh;
    {
        const char* attributes[3];
        attributes[0] = "split_with_circumference_below";
        attributes[1] = "2000";
        attributes[2] = 0;
        mesh = LoadMeshAndGeneratePreprocess(pathEngine, testBed, shape, "thainesford", 0, attributes, os);
    }
    Benchmark_LongPaths(testBed, mesh, shape, os);
    Benchmark_DynamicObstaclesPathfinding(
            pathEngine, testBed, mesh,
            shape, // agent shape
            bigShape, // obstacle shape
            5,  // number of dynamic obstacles
            40,  // number of collision context iterations
            10,  // number of pathfinding queries per collision context iteration
            333, // random seed
            os
            );
    Benchmark_DynamicObstaclesPathfinding(
            pathEngine, testBed, mesh,
            shape, // agent shape
            bigShape, // obstacle shape
            10,  // number of dynamic obstacles
            40,  // number of collision context iterations
            10,  // number of pathfinding queries per collision context iteration
            333, // random seed
            os
            );
    Benchmark_DynamicObstaclesPathfinding(
            pathEngine, testBed, mesh,
            shape, // agent shape
            bigShape, // obstacle shape
            20,  // number of dynamic obstacles
            40,  // number of collision context iterations
            10,  // number of pathfinding queries per collision context iteration
            333, // random seed
            os
            );
    delete mesh;
}
