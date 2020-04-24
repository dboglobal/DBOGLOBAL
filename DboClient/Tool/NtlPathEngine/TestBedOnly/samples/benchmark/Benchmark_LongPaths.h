
#include <fstream>

class iTestBed;
class iMesh;
class iShape;

void
Benchmark_LongPaths(
        iTestBed* testBed,
        iMesh* mesh,
        iShape* shape,
        std::ofstream& os
        );
