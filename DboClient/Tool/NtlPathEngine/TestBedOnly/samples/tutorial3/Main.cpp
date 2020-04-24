
#include <string.h>
#include <math.h>
#include "i_pathengine.h"
#include "i_testbed.h"

void demo(iPathEngine *pathengine, iTestBed *testbed);

extern "C"
{
    __declspec(dllexport) void __stdcall TestApplicationEntryPoint(
                                            iPathEngine *pathengine, iTestBed *testbed)
    {             
    // check if interfaces are compatible with the headers used for compilation
        if(testbed->getInterfaceMajorVersion()!=TESTBED_INTERFACE_MAJOR_VERSION
            ||
            testbed->getInterfaceMinorVersion()<TESTBED_INTERFACE_MINOR_VERSION)
        {
            testbed->reportError("Fatal","TestApplication: testbed version is incompatible with headers used for compilation.");
            return;
        }
        if(pathengine->getInterfaceMajorVersion()!=PATHENGINE_INTERFACE_MAJOR_VERSION
            ||
            pathengine->getInterfaceMinorVersion()<PATHENGINE_INTERFACE_MINOR_VERSION)
        {
            testbed->reportError("Fatal","TestApplication: pathengine version is incompatible with headers used for compilation.");
            return;
        }

        demo(pathengine,testbed);
    }
}



void demo(iPathEngine *pathengine, iTestBed *testbed)
{
    iMesh* mesh;
    {
        char *buffer;
        unsigned long size;
        buffer=testbed->loadBinary("..\\resource\\meshes\\mesh1.xml",size);
        mesh=pathengine->loadMeshFromBuffer("xml", buffer, size, 0);
        testbed->freeBuffer(buffer);
    }
    testbed->setMesh(mesh);
    testbed->zoomExtents();


// create shapes
    iShape *agent_shape;
    iShape *obstruction_shape1;
    iShape *obstruction_shape2;
    {
        long array[]=
        {
            8,20,
            20,8,
            20,-8,
            8,-20,
            -8,-20,
            -20,-8,
            -20,8,
            -8,20,
        };
        agent_shape=pathengine->newShape(sizeof(array)/sizeof(*array)/2,array);
    }
    {
        long array[]=
        {
            30,30,
            30,-30,
            -30,-30,
            -30,30,
        };
        obstruction_shape1=pathengine->newShape(sizeof(array)/sizeof(*array)/2,array);
    }
    {
        long array[]=
        {
            10,35,
            20,-20,
            -10,-30,
            -15,32,
        };
        obstruction_shape2=pathengine->newShape(sizeof(array)/sizeof(*array)/2,array);
    }

// generate collision and pathfinding preprocess for the agent shape
    mesh->generateCollisionPreprocessFor(agent_shape, 0);
    mesh->generatePathfindPreprocessFor(agent_shape, 0);

// iAgent objects are used to instantiate shapes on a mesh - either as true agents or as obstructions
// values are initialised to 0 here to indicate that nothing has yet been placed
    iAgent* agent=0;
    iAgent* obstruction_agent1=0;
    iAgent* obstruction_agent2=0;

// iCollisionContext objects encapsulate a state of collision on a mesh - essentially this is a set of agents
    iCollisionContext *context=mesh->newContext();

// direction agent is currently facing
    float agent_heading=0;

// the agents current path
    iPath* path=0;

    bool exitflag=false;
    while(!exitflag)
    {
        testbed->setColourRGB(0.0f,0.0f,0.85f);
        testbed->drawMesh();
        testbed->setColour("blue");
        testbed->drawMeshEdges();

    // draw agent if it has been placed
        if(agent)
        {
            testbed->setColour("white");
            testbed->drawAgent(agent,20);
            testbed->setColour("orange");
            testbed->drawAgentHeading(agent,30,20,agent_heading);
        }

    // draw any obstructions that have been placed
        if(obstruction_agent1)
        {
            testbed->setColour("red");
            testbed->drawAgent(obstruction_agent1,20);
        }
        if(obstruction_agent2)
        {
            testbed->setColour("red");
            testbed->drawAgent(obstruction_agent2,20);
        }

    // draw path if there is one
        testbed->setColour("green");
        testbed->drawPath(path);

        testbed->update();

        if(agent)
        {
            if(testbed->getLeftMouseState())
            {
            // path towards position under mouse
                delete path;
                path = 0;
                cPosition target=testbed->positionAtMouse();
                if(target.cell!=-1)
                {
                // modified to add curved path post processing
                    iPath* base_path = agent->findShortestPathTo(context, target);
                    if(base_path)
                    {
                        path = agent->generateCurvedPath(base_path, context, 0, 0, 20, 0.3f, 0.4f);
                        delete base_path;
                    }
                }
            }
            else
            if(testbed->getRightMouseState())
            {
            // path away from position under mouse
                delete path;
                path = 0;
                cPosition awayfrom=testbed->positionAtMouse();
                if(awayfrom.cell!=-1)
                {
                    iAgent* imaginary_agent=mesh->placeAgent(obstruction_shape1,awayfrom);
                    context->addAgent(imaginary_agent);
                // modified to add curved path post processing
                    iPath* base_path = agent->findPathAway(context,awayfrom,500);
                    if(base_path)
                    {
                        path = agent->generateCurvedPath(base_path, context, 0, 0, 20, 0.3f, 0.4f);
                        delete base_path;
                    }
                    delete imaginary_agent;
                }
            }
            if(path)
            {
                if(path->size() >= 2)
                {
                // set heading from the vector for the current path section
                    cPosition next_target=path->position(1);
                    cPosition current=agent->getPosition();
                    long dx,dy;
                    dx=next_target.x-current.x;
                    dy=next_target.y-current.y;
                    agent_heading=static_cast<float>(atan2(static_cast<double>(dx),static_cast<double>(dy)));
                }
                agent->advanceAlongPath(path,3,context);                
            }
        }

    // receive and process messages for all keys pressed since last frame
        const char *keypressed;
        while(keypressed=testbed->receiveKeyMessage())
        {
            if(keypressed[0]!='d') // is it a key down message?
                continue;

            switch(keypressed[1])
            {
            case '_':
                {
                    if(strcmp("ESCAPE",keypressed+2)==0)
                        exitflag=true;
                    break;
                }
            case '4':
            // place or move the agent
                {
                    cPosition p=testbed->positionAtMouse();
                    if(p.cell!=-1 && !mesh->testPointCollision(agent_shape,context,p))
                    {
                        if(!agent)
                            agent=mesh->placeAgent(agent_shape,p);
                        else
                            agent->moveTo(p);
                        delete path;
                        path = 0;
                    }
                }
                break;
            case '5':
            // place or move obstruction 1
                {
                    cPosition p=testbed->positionAtMouse();
                    if(p.cell!=-1 && (agent==0 || !agent->testCollisionDirectlyAgainstPlacedShape(obstruction_shape1,p)))
                    {
                    // place the agent if not yet placed, otherwise move to the requested position
                        if(!obstruction_agent1)
                        {
                            obstruction_agent1=mesh->placeAgent(obstruction_shape1,p);
                            context->addAgent(obstruction_agent1);
                        }
                        else
                            obstruction_agent1->moveTo(p);
                        delete path;
                        path = 0;
                    }
                }
                break;
            case '6':
            // place or move obstruction 2
                {
                    cPosition p=testbed->positionAtMouse();
                    if(p.cell!=-1 && (agent==0 || !agent->testCollisionDirectlyAgainstPlacedShape(obstruction_shape2,p)))
                    {
                    // place the agent if not yet placed, otherwise move to the requested position
                        if(!obstruction_agent2)
                        {
                            obstruction_agent2=mesh->placeAgent(obstruction_shape2,p);
                            context->addAgent(obstruction_agent2);
                        }
                        else
                            obstruction_agent2->moveTo(p);
                        delete path;
                        path = 0;
                    }
                }
                break;
            }
        }
    }
}

