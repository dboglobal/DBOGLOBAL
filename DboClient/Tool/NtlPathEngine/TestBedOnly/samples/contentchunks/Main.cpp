
#include "entrypoint.h"
#include "FileOutputStream.h"
#include "LoadWhiteSpaceDelimited.h"
#include "LoadContentChunkPlacement.h"
#include "SimpleDOM.h"
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

class cPartitionedTerrain : public iFaceVertexMesh
{
    long _originX, _originY;
    long _squareSize, _squaresAcross;

public:

    cPartitionedTerrain(
            long originX, long originY,
            long squareSize, long squares
            ) :
     _originX(originX),
     _originY(originY),
     _squareSize(squareSize),
     _squaresAcross(squares)
    {
    }

// iFaceVertexMesh interface

    long faces() const
    {
        return _squaresAcross * _squaresAcross * 2;
    }
    long vertices() const
    {
        return (_squaresAcross + 1) * (_squaresAcross + 1);
    }
    long vertexIndex(long face, long vertexInFace) const
    {
        long baseVertex = face / 2 + face / 2 / _squaresAcross;
        if(face & 1)
        {
            switch(vertexInFace)
            {
            default:
//                invalid
            case 0:
                return baseVertex;
            case 1:
                return baseVertex + 1;
            case 2:
                return baseVertex + (_squaresAcross + 1) + 1;
            }
        }
        switch(vertexInFace)
        {
        default:
//            invalid
        case 0:
            return baseVertex;
        case 1:
            return baseVertex + (_squaresAcross + 1) + 1;
        case 2:
            return baseVertex + (_squaresAcross + 1);
        }
    }
    long vertexX(long vertex) const
    {
        return _originX + vertex / (_squaresAcross + 1) * _squareSize;
    }
    long vertexY(long vertex) const
    {
        return _originY + (vertex % (_squaresAcross + 1)) * _squareSize;
    }
    float vertexZ(long vertex) const
    {
        return 0.0f;
    }
    long faceAttribute(long face, long attributeIndex) const
    {
        if(attributeIndex == PE_FaceAttribute_SectionID)
        {
            return 0; // mark all faces as sectionID == 0 (first terrain layer)
        }
        return -1;
    }
};


void demo(iPathEngine* pathEngine, iTestBed* testBed)
{
    cSimpleDOM placementScript;
    {
        std::ifstream is("..\\resource\\contentChunkPlacement\\contentchunks_example.txt");
        if(!is.good())
        {
            Error("Fatal", "Could not open content chunk placement script('resource\\contentChunkPlacement\\contentchunks_example.txt').");
            return;
        }
        LoadWhiteSpaceDelimited(is, placementScript);
    }

    std::vector<iContentChunk*> loadedChunks;
    std::vector<iContentChunkInstance*> placedInstances;
    LoadContentChunkPlacement(pathEngine, testBed, placementScript, loadedChunks, placedInstances, false);

  // generate terrain stand-in geometry for the range -100000,-100000 -> 100000,100000
  // world coordinates are in millimetres, so this corresponds to a 200 metres by 200 metres region centred on the origin
    cPartitionedTerrain terrain(-100000, -100000, 20000, 10);

    std::vector<const iFaceVertexMesh*> groundParts;

    groundParts.push_back(&terrain);
    for(size_t i = 0; i < placedInstances.size(); ++i)
    {
        if(placedInstances[i]->getContentChunk()->hasGroundComponent())
        {
            groundParts.push_back(placedInstances[i]->getGroundPointer());
        }
    }

    // the following lines can be uncommented to write a file containing a snapshot of the data being passed into content processing
    // this can be imported into 3DS Max, and is useful for troubleshooting any problems with the content processing
//    {
//        cFileOutputStream fos("contentSnapshot.tok");
//        pathEngine->saveContentData(&groundParts.front(), groundParts.size(), "tok", &fos);
//    }

    iMesh* mesh = pathEngine->buildMeshFromContent(&groundParts.front(), groundParts.size(), 0);

    if(mesh)
    {
        for(size_t i = 0; i < placedInstances.size(); ++i)
        {
            std::ostringstream prefix;
            prefix << i << ':';
            placedInstances[i]->addAnchorsAndShapes(mesh, prefix.str().c_str(), 1);
        }
    }
    for(size_t i = 0; i < loadedChunks.size(); ++i)
    {
        delete loadedChunks[i];
    }
    loadedChunks.clear();

    if(mesh == 0)
    {
        return;
    }

    testBed->setMesh(mesh);
    testBed->zoomExtents();

    while(!testBed->getKeyState("_SPACE"))
    {
        testBed->setColourRGB(0.0f,0.0f,0.85f);
        testBed->drawMesh();
        testBed->setColour("blue");
        testBed->drawMeshEdges();
        testBed->setColour("white");
        testBed->drawBurntInObstacles();
        testBed->printTextLine(10, "press space to quit..");
        testBed->printTextLine(10, "result of buildMeshFromContent()");
        testBed->update();
    }
}
