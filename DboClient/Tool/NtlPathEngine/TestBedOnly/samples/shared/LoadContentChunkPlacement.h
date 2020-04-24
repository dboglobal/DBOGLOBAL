
#include <vector>

class iPathEngine;
class iTestBed;
class cSimpleDOM;
class iContentChunk;
class iContentChunkInstance;

void
LoadContentChunkPlacement(
        iPathEngine* pathEngine, iTestBed* testBed, 
        const cSimpleDOM& placementScript,
        std::vector<iContentChunk*>& loadedChunks,
        std::vector<iContentChunkInstance*>& placedInstances,
        bool setUniqueSectionIDs
        );
