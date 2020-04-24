
#include "i_pathengine.h"
#include <fstream>

class iTestBed;

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
        );
