
#include "BuildFederation_FromWorldMesh.h"
#include "FileOutputStream.h"
#include "SimpleDOM.h"
#include "LoadWhiteSpaceDelimited.h"
#include "LoadContentChunkPlacement.h"
#include "i_pathengine.h"
#include "i_testbed.h"
#include <string>
#include <fstream>
#include <sstream>


static void
GetWorldRange(
        const std::vector<iContentChunkInstance*>& placedInstances,
        cHorizontalRange& result
        )
{
    assertD(!placedInstances.empty());
    placedInstances[0]->getHorizontalRange(result);
    long i;
    for(i = 1; i != static_cast<long>(placedInstances.size()); ++i)
    {
        cHorizontalRange instanceRange;
        placedInstances[i]->getHorizontalRange(instanceRange);
        if(instanceRange.minX < result.minX)
        {
            result.minX = instanceRange.minX;
        }
        else if(instanceRange.maxX > result.maxX)
        {
            result.maxX = instanceRange.maxX;
        }
        if(instanceRange.minY < result.minY)
        {
            result.minY = instanceRange.minY;
        }
        else if(instanceRange.maxY > result.maxY)
        {
            result.maxY = instanceRange.maxY;
        }
    }
}

static bool
RangesOverlap(const cHorizontalRange& r1, const cHorizontalRange& r2)
{
    if(r1.minX > r2.maxX)
    {
        return false;
    }
    if(r1.maxX < r2.minX)
    {
        return false;
    }
    if(r1.minY > r2.maxY)
    {
        return false;
    }
    if(r1.maxY < r2.minY)
    {
        return false;
    }
    return true;
}

static void
BuildTile(
        iTestBed* testBed,
        iMeshFederation* federation,
        long tileIndex,
        const std::vector<iContentChunkInstance*>& placedInstances
        )
{
    std::vector<const iContentChunkInstance*> overlappingInstances;

    cHorizontalRange representedRegion;
    federation->getRepresentedRegion_World(tileIndex, representedRegion);

    long i;
    for(i = 0; i != static_cast<long>(placedInstances.size()); ++i)
    {
        cHorizontalRange instanceRange;
        placedInstances[i]->getHorizontalRange(instanceRange);
        if(RangesOverlap(instanceRange, representedRegion))
        {
            overlappingInstances.push_back(placedInstances[i]);
        }
    }

    const iContentChunkInstance** instancesBuffer = 0;
    if(!overlappingInstances.empty())
    {
        instancesBuffer = &overlappingInstances[0];
    }

    iOutputStream* snapshotOSPtr = 0;

    // the following lines can be uncommented to write a file containing a snapshot of the data being passed into content processing
    // this can be imported into 3DS Max, and is useful for troubleshooting any problems with the content processing
//    std::ostringstream snapshotFileName;
//    snapshotFileName << "..\\resource\\meshes\\federationTile" << tileIndex << ".sourceContent.tok";
//    cFileOutputStream snapshotFOS(snapshotFileName.str().c_str());
//    snapshotOSPtr = &snapshotFOS;

    iMesh* tileMesh = federation->buildTileMeshFromContent(
            tileIndex, 
            instancesBuffer, static_cast<long>(overlappingInstances.size()),
            0, // no additional options
            snapshotOSPtr
            );

    std::ostringstream fileName;
    fileName << "federationTile" << tileIndex << ".tok";
    cFileOutputStream fos(fileName.str().c_str());
    tileMesh->saveGround("tok", true, &fos);
    delete tileMesh;
    testBed->printTextLine(5, fileName.str().c_str());
    testBed->printTextLine(5, "finished generating content tile:");
    testBed->printTextLine(5, "Generating Federation Tile Meshes");
    testBed->update();
}

void
BuildFederation_TileByTile(
        iPathEngine* pathEngine, iTestBed* testBed,
        const char* placementFileName,
        long tileSize, long overlap
        )
{
    cSimpleDOM placementScript;
    {
        std::string fileName("..\\resource\\contentChunkPlacement\\");
        fileName.append(placementFileName);
        fileName.append(".txt");
        std::ifstream is(fileName.c_str());
        if(!is.good())
        {
            Error("Fatal", "Could not open content chunk placement script.");
            return;
        }
        LoadWhiteSpaceDelimited(is, placementScript);
    }

    std::vector<iContentChunk*> loadedChunks;
    std::vector<iContentChunkInstance*> placedInstances;
    LoadContentChunkPlacement(pathEngine, testBed, placementScript, loadedChunks, placedInstances, true);
    if(placedInstances.empty())
    {
        Error("Fatal", "No content chunk instances placed.");
        return;
    }

    cHorizontalRange worldRange;
    GetWorldRange(placedInstances, worldRange);

    iMeshFederation* federation = pathEngine->buildMeshFederation_TilingOnly(worldRange, tileSize, overlap);

    long i;
    for(i = 0; i != federation->size(); ++i)
    {
        BuildTile(testBed, federation, i, placedInstances);
    }

    std::ostringstream fileName;
    fileName << "federation.tok";
    cFileOutputStream fos(fileName.str().c_str());
    federation->save("tok", &fos);
    delete federation;

    for(size_t i = 0; i != loadedChunks.size(); ++i)
    {
        delete loadedChunks[i];
    }
}

