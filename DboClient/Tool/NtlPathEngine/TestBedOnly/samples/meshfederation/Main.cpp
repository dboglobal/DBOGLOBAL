
#include "EntryPoint.h"
#include "FileOutputStream.h"
#include "WaitForFrame.h"
#include "BuildFederation_FromWorldMesh.h"
#include "BuildFederation_TileByTile.h"
#include <sstream>
#include <vector>
#include <math.h>

iTestBed* gTestBed;

void
ControlCamera(float& heading, float& elevation)
{
    if(!gTestBed->getRightMouseState())
    {
        return;
    }
    long dx, dy;
    gTestBed->getMouseScreenDelta(dx, dy);
    heading += dx * 0.016f;
    heading = fmod(heading, 6.2856f);
    elevation += dy * 0.016f;
    if(elevation > 1.5f)
    {
        elevation = 1.5f;
    }
    else
    if(elevation < -0.05f)
    {
        elevation = -0.05f;
    }
}

static iMesh*
LoadTileMesh(iTestBed* testBed, iPathEngine* pathEngine, long tileIndex)
{
    std::ostringstream fileName;
    fileName << "federationTile" << tileIndex << ".tok";
    char* buffer;
    unsigned long size;
    buffer = testBed->loadBinary(fileName.str().c_str(), size);
    if(!buffer)
    {
        return 0;
    }
    iMesh* result = pathEngine->loadMeshFromBuffer("tok", buffer, size, 0);
    testBed->freeBuffer(buffer);
    return result;
}

static bool
GetUnobstructedPositionInTileHandledRegion(
        const iMeshFederation* federation,
        long tileIndex,
        iMesh* tileMesh,
        iShape* agentShape,
        long agentSize,
        cPosition& result
        )
{
    if(tileMesh == 0)
    {
        return false; // no geometry overlapped by this tile
    }
    result = tileMesh->generateRandomPosition();
    if(tileMesh->testPointCollision(agentShape, 0, result))
    {
        return false;
    }
    cHorizontalRange hr;
    federation->getHandledRegion_Local(tileIndex, hr);
    if(result.x < hr.minX || result.x > hr.maxX || result.y < hr.minY || result.y > hr.maxY)
    {
        return false; // position generated is outside the tiles handled region;
    }

    cHorizontalRange rr;
    federation->getRepresentedRegion_Local(tileIndex, rr);
    assertD(rr.maxX - rr.minX > agentSize * 2); // the tile overlap must be large than agent size
    assertD(rr.maxY - rr.minY > agentSize * 2);
    rr.minX += agentSize;
    rr.minY += agentSize;
    rr.maxX -= agentSize;
    rr.maxY -= agentSize;
    if(result.x < rr.minX || result.x > rr.maxX || result.y < rr.minY || result.y > rr.maxY)
    {
        return false; // agent would overlap outside federation world bounds at this position
    }

    return true;
}

static void
GetUnobstructedPositionInFederation(
        const iMeshFederation* federation,
        iMesh** tileMeshes,
        iShape* agentShape,
        long agentSize,
        long& tileIndex, cPosition& positionInTile
        )
{
// loop through tiles, attempting to generate a position within each tile
// (in general, we can't guarantee to find a position in any given tile, because it's possible for individual tiles in a federation to be empty)
    while(1)
    {
        for(tileIndex = 0; tileIndex != federation->size(); ++tileIndex)
        {
            if(GetUnobstructedPositionInTileHandledRegion(
                    federation, tileIndex,
                    tileMeshes[tileIndex],
                    agentShape, agentSize,
                    positionInTile
                    ))
            {
                return;
            }
        }
    }    
}

void
demo(iPathEngine* pathEngine, iTestBed* testBed)
{
// build a 'mesh federation' object,
// and an associated set of tiled mesh instances
// and save this all to disk

    // note that this mesh federation construction step would usually happen at content generation time
    // (and could be skipped after the first time the demo is run, as long as the world mesh is not changed)

    const long tileSize = 8000;
    const long overlap = 2000;
    const long agentSize = 20;
    BuildFederation_FromWorldMesh(pathEngine, testBed, "thainesford_town", tileSize, overlap);

    // ** comment out the above and replace with the following to use 'tile by tile' federation construction
    // ** instead of construction from a supplied world mesh

//    const long tileSize = 40000;
//    const long overlap = 20000;
//    const long agentSize = 160;
//    BuildFederation_TileByTile(pathEngine, testBed, "meshfederation_example", tileSize, overlap);

// load the constructed mesh federation and associated tile meshes back from disk
// and run the demo against these loaded objects

    const long agentHeight = agentSize * 6;
    const float agentSpeed = static_cast<float>(agentSize) * 0.75f;
    const float cameraDistance = static_cast<float>(agentSize) * 350.f;
    const long resolveRange = agentSize * 3;

    gTestBed = testBed;

    iMeshFederation* federation;
    {
        char* buffer;
        unsigned long size;
        buffer = testBed->loadBinary("federation.tok", size);
        federation = pathEngine->loadFederation("tok", buffer, size);
        testBed->freeBuffer(buffer);
    }

    iShape* agentShape;
    {
        long array[]=
        {
            -agentSize, agentSize,
            agentSize, agentSize,
            agentSize, -agentSize,
            -agentSize, -agentSize,
        };
        agentShape = pathEngine->newShape(sizeof(array)/sizeof(*array)/2, array);
    }

  // generate collision and pathfinding preprocess for the federation tiles
  // also prepare some per-tile collision contexts
  // display the meshes in the testbed while the preprocess is being generated

    // note that, in a real application,
    // the preprocess could be pregenerated at content time, and then loaded back in at runtime,
    // to avoid delays during application setup

    const char* attributes[3];
    attributes[0] = "split_with_circumference_below";
    attributes[1] = "2000";
    attributes[2] = 0;

    std::vector<iMesh*> tileMeshes(federation->size(), 0);
    std::vector<iCollisionContext*> contexts(federation->size(), 0);
    long i;
    for(i = 0; i != federation->size(); ++i)
    {
        tileMeshes[i] = LoadTileMesh(testBed, pathEngine, i);
        if(tileMeshes[i] == 0)
        {
            continue;
        }

        tileMeshes[i]->generateCollisionPreprocessFor(agentShape, 0);
        tileMeshes[i]->generatePathfindPreprocessFor(agentShape, attributes);

        {
        // set up a collision context for each tile
        // with query bounds that restrict pathfinding to the tile's represented region
        // query bounds act on agent origin, so the region is brought in by agentSize
        // so that the agent's shape does not overlap outside the represented region
            contexts[i] = tileMeshes[i]->newContext();
            cHorizontalRange rr;
            federation->getRepresentedRegion_Local(i, rr);
            assertD(rr.maxX - rr.minX > agentSize * 2); // the tile overlap must be large than agent size
            assertD(rr.maxY - rr.minY > agentSize * 2);
            rr.minX += agentSize;
            rr.minY += agentSize;
            rr.maxX -= agentSize;
            rr.maxY -= agentSize;
            contexts[i]->setQueryBounds(rr);
        }

      // display the meshes as they get loaded
      // with handled and represented regions boundaries drawn
      // note that this also generates any rendering preprocess required by the testbed,
      // and therefore helps to avoid pauses when the testbed subsequently switches between tile meshes

        std::ostringstream fileName;
        fileName << "..\\resource\\meshes\\federationTile" << i << ".tok";
        gTestBed->printTextLine(5, fileName.str().c_str());
        gTestBed->printTextLine(5, "finished loading and preprocessing tile:");
        gTestBed->printTextLine(5, "Loading and Preprocessing Tile Meshes");
        testBed->setMesh(tileMeshes[i]);
        testBed->zoomExtents();
        testBed->setColourRGB(0.f, 0.f, 0.85f);
        testBed->drawMesh();
        testBed->setColour("blue");
        testBed->drawMeshEdges();
        testBed->setColour("white");
        testBed->drawBurntInObstacles();
        {
            cHorizontalRange bounds;
            federation->getRepresentedRegion_Local(i, bounds);
            testBed->setColour("red");
            testBed->drawRangeBounds(bounds);
            federation->getHandledRegion_Local(i, bounds);
            testBed->setColour("yellow");
            testBed->drawRangeBounds(bounds);
        }
        testBed->update();
    }

    iAgent* agent = 0;
    long agentTile = -1;

  // get an arbitrary, unobstructed position within the federation at which to start the agent
    {
        cPosition agentStartPosition;
        GetUnobstructedPositionInFederation(
                federation, &tileMeshes[0],
                agentShape, agentSize,
                agentTile, agentStartPosition
                );
        agent = tileMeshes[agentTile]->placeAgent(agentShape, agentStartPosition);
    }

    testBed->setMesh(tileMeshes[agentTile]);

    float cameraHeading = 0.f;
    float cameraElevation = 1.f;

    float agentHeading = 0;
    float agentPrecisionX = 0.f;
    float agentPrecisionY = 0.f;
    iPath* path = 0;
    bool markerSet = false;
    cPosition markerPosition;
    const char* markerColour;

    bool exitFlag = false;
    while(!exitFlag)
    {
        ControlCamera(cameraHeading, cameraElevation);
        testBed->lookAtWithPrecision(
                agent->getPosition(), agentPrecisionX, agentPrecisionY,
                cameraHeading, cameraElevation,
                cameraDistance
                );

        testBed->setColourRGB(0.f, 0.f, 0.85f);
        testBed->drawMesh();
        testBed->setColour("blue");
        testBed->drawMeshEdges();
        testBed->setColour("white");
        testBed->drawBurntInObstacles();

        {
            cHorizontalRange bounds;
            federation->getRepresentedRegion_Local(agentTile, bounds);
            testBed->setColour("red");
            testBed->drawRangeBounds(bounds);
            federation->getHandledRegion_Local(agentTile, bounds);
            testBed->setColour("yellow");
            testBed->drawRangeBounds(bounds);
        }

        testBed->setColour("orange");
        testBed->drawAgentWithPrecision(agent, agentHeight, agentPrecisionX, agentPrecisionY);
        testBed->setColour("purple");
        testBed->drawAgentHeadingWithPrecision(agent, agentSize * 2, agentHeight, agentHeading, agentPrecisionX, agentPrecisionY);

      // draw path if there is one
        testBed->setColour("green");
        testBed->drawPath(path);

      // draw marker, if set
        if(markerSet)
        {
            testBed->setColour(markerColour);
            testBed->drawPosition(markerPosition, agentSize * 2);
        }

        WaitForFrame();
        testBed->update();

        bool repathRequested = false;

    // receive and process messages for all keys pressed since last frame
        const char* keyPressed;
        while(keyPressed = testBed->receiveKeyMessage())
        {
            if(keyPressed[0] != 'd' || keyPressed[1] != '_')
            {
              // only interested in key down messages for keys with extended key strings, here
                continue;
            }
            if(strcmp("ESCAPE", keyPressed + 2) == 0)
            {
                exitFlag = true;
            }
            else if(strcmp("LMOUSE", keyPressed + 2) == 0)
            {
                repathRequested = true;
            }
        }

        if(repathRequested || (path && path->size() == 1))
        {
          // clear the existing path

            delete path;
            path = 0;
            markerSet = false;

          // and check whether we need to translate to a neighbouring tile

            long worldX, worldY;
            federation->getTileCentre(agentTile, worldX, worldY);
            worldX += agent->getPosition().x;
            worldY += agent->getPosition().y;
            long tileForQuery = federation->tileForQuery(worldX, worldY);
            if(tileForQuery != agentTile)
            {
                cPosition positionOnOverlapped = federation->translatePosition(
                        agentTile, tileMeshes[agentTile],
                        tileForQuery, tileMeshes[tileForQuery],
                        agent->getPosition()
                        );
                assertD(positionOnOverlapped.cell != -1);
                delete agent;
                agentTile = tileForQuery;
                agent = tileMeshes[agentTile]->placeAgent(agentShape, positionOnOverlapped);
                testBed->setMesh(tileMeshes[agentTile]);
                // update camera position,
                // required for the call to positionAtMouse(), below
                testBed->lookAtWithPrecision(
                        agent->getPosition(), agentPrecisionX, agentPrecisionY,
                        cameraHeading, cameraElevation,
                        cameraDistance
                        );
            }
        }

        if(path)
        {
            if(path->size() >= 2)
            {
            // set heading from the vector for the current path section
                cPosition nextTarget = path->position(1);
                cPosition current = agent->getPosition();
                long dx,dy;
                dx = nextTarget.x - current.x;
                dy = nextTarget.y - current.y;
                agentHeading = static_cast<float>(atan2(static_cast<double>(dx), static_cast<double>(dy)));
            }
            agent->advanceAlongPathWithPrecision(path, agentSpeed, 0, agentPrecisionX, agentPrecisionY);
        }

        if(repathRequested)
        {
          // send agent to the position currently under the mouse cursor
          // note that we guaranteed, above, that the agent is in the handled region for agentTile

            cPosition target = testBed->positionAtMouse();
            if(target.cell != -1)
            {
                markerSet = true;
                markerPosition = target;
                cPosition unobstructed = tileMeshes[agentTile]->findClosestUnobstructedPosition(agentShape, 0, target, resolveRange);
                if(unobstructed.cell == -1)
                {
                  // unable to find an unobstructed position within range
                    markerColour = "red";
                }
                else
                {
                    markerColour = "yellow";
                    path = agent->findShortestPathTo(contexts[agentTile], unobstructed);
                }
            }
        }
    }
}
