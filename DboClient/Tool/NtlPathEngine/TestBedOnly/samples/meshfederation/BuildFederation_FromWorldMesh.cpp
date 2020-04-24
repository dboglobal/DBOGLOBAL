
#include "BuildFederation_FromWorldMesh.h"
#include "FileOutputStream.h"
#include "i_pathengine.h"
#include "i_testbed.h"
#include <sstream>

class cSaveTileMeshesCallBack : public iMeshFederationTileGeneratedCallBack
{
    iTestBed* _testBed;

public:
    
    cSaveTileMeshesCallBack(iTestBed* testBed);
    void tileGenerated(long tileIndex, iMesh* tileMesh);
};

cSaveTileMeshesCallBack::cSaveTileMeshesCallBack(iTestBed* testBed) :
 _testBed(testBed)
{
}

void
cSaveTileMeshesCallBack::tileGenerated(long tileIndex, iMesh* tileMesh)
{
    std::ostringstream fileName;
    fileName << "federationTile" << tileIndex << ".tok";
    cFileOutputStream fos(fileName.str().c_str());
    tileMesh->saveGround("tok", true, &fos);
    delete tileMesh;
    _testBed->printTextLine(5, fileName.str().c_str());
    _testBed->printTextLine(5, "finished generating content tile:");
    _testBed->printTextLine(5, "Generating Federation Tile Meshes");
    _testBed->update();
}

void
BuildFederation(
        iPathEngine* pathEngine, iTestBed* testBed,
        iMesh* worldMesh,
        long tileSize, long overlap
        )
{
    cSaveTileMeshesCallBack callback(testBed);

    iMeshFederation* federation;
    federation = pathEngine->buildMeshFederation_FromWorldMesh(
            worldMesh,
            tileSize, overlap,
            callback
            );

    std::ostringstream fileName;
    fileName << "federation.tok";
    cFileOutputStream fos(fileName.str().c_str());
    federation->save("tok", &fos);
    delete federation;
}

void
BuildFederation_FromWorldMesh(
        iPathEngine* pathEngine, iTestBed* testBed,
        const char* worldMeshName,
        long tileSize, long overlap
        )
{
    iMesh* worldMesh;
    {
        char* buffer;
        unsigned long size;
        std::string meshPath = "..\\resource\\meshes\\";
        meshPath.append(worldMeshName);
        meshPath.append(".tok");
        buffer = testBed->loadBinary(meshPath.c_str(), size);
        worldMesh = pathEngine->loadMeshFromBuffer("tok", buffer, size, 0);
        testBed->freeBuffer(buffer);
    }
    BuildFederation(pathEngine, testBed, worldMesh, tileSize, overlap);
}
