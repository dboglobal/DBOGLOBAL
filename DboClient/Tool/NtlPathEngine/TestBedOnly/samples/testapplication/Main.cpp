
#include "i_pathengine.h"
#include "i_testbed.h"
#include "MoveAgent.h"
#include "WaitForFrame.h"
#include <sstream>
#include <vector>
#include <math.h>

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
    bool exitflag=false;
    
// create shapes
    iShape *agent_shape;
    std::vector<iShape*> obstruction_shapes;
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
        obstruction_shapes.push_back(pathengine->newShape(sizeof(array)/sizeof(*array)/2,array));
    }
    {
        long array[]=
        {
            10,35,
            20,-20,
            -10,-30,
            -15,32,
        };
        obstruction_shapes.push_back(pathengine->newShape(sizeof(array)/sizeof(*array)/2,array));
    }
    {
        long array[]=
        {
            15,60,
            50,0,
            10,-40,
            -40,-40,
            -40,-5,
        };
        obstruction_shapes.push_back(pathengine->newShape(sizeof(array)/sizeof(*array)/2,array));
    }
    {
        long array[]=
        {
            15,20,
            18,-5,
            7,-20,
            -18,-18,
            -16,16,
        };
        obstruction_shapes.push_back(pathengine->newShape(sizeof(array)/sizeof(*array)/2,array));
    }
    obstruction_shapes.push_back(0);

    iMesh* mesh;
    {
        char *buffer;
        unsigned long size;
        buffer = testbed->loadBinary("..\\resource\\meshes\\mesh1.xml", size);
        mesh = pathengine->loadMeshFromBuffer("xml", buffer, size, 0);
        testbed->freeBuffer(buffer);
    }

    testbed->setMesh(mesh); // set this mesh as the current mesh for rendering
    testbed->zoomExtents(); // place camera to see all of newly loaded mesh

// generate preprocess for the agent shape
    mesh->generateCollisionPreprocessFor(agent_shape, 0);
    mesh->generatePathfindPreprocessFor(agent_shape, 0);

// iAgent objects are used to instantiate shapes on a mesh - either as true agents or as obstructions

    // values are initialised to 0 to indicate that nothing has yet been placed
    iAgent* agent = 0;
    float agentPrecisionX = 0.f, agentPrecisionY = 0.f;
    std::vector<iAgent*> obstruction_agents;
    size_t i;
    for(i = 0; i < obstruction_shapes.size() - 1; i++)
    {
        obstruction_agents.push_back(0);
    }

// iCollisionContext objects encapsulate a state of collision on a mesh - essentially this is a collection of agents
    iCollisionContext* context = mesh->newContext();

// target position for pathfinding
    cPosition agent_target;
// path to target
    iPath* path = 0;

// direction agent is currently facing
    float agent_heading=0;

    bool drawExpansion = false;
    bool paused = false;

    while(!exitflag)
    {
    // draw mesh
        testbed->setColourRGB(0.0f,0.0f,0.85f);
        testbed->drawMesh();
        testbed->setColour("blue");
        testbed->drawMeshEdges();

        if(drawExpansion)
        {
            testbed->setColour("orange");
            testbed->drawMeshExpansion(agent_shape);
            testbed->drawSmallConvexExpansions(agent_shape);
        }

    // determine closest unobstructed point to agent target if necessary,
    // draw target and closest unobstructed point as appropriate
        cPosition agent_target_closest_unobstructed_point;
        if(agent_target.cell!=-1)
        {
            if(mesh->testPointCollision(agent_shape,context,agent_target))
            {
                testbed->setColour("red");
                testbed->drawPosition(agent_target,20);
                agent_target_closest_unobstructed_point=mesh->findClosestUnobstructedPosition(
                                    agent_shape,
                                    context,
                                    agent_target,
                                    20);
                testbed->setColour("orange");
                if(agent_target_closest_unobstructed_point.cell!=-1)
                    testbed->drawPosition(agent_target_closest_unobstructed_point,15);
            }
            else
            {
                testbed->setColour("orange");
                testbed->drawPosition(agent_target,20);
                agent_target_closest_unobstructed_point=agent_target;
            }
        }

    // draw agent if it has been placed
        if(agent)
        {
            testbed->setColour("white");
            testbed->drawAgentWithPrecision(agent, 70, agentPrecisionX, agentPrecisionY);
            testbed->setColour("orange");
            testbed->drawAgentHeadingWithPrecision(agent, 30, 70,agent_heading, agentPrecisionX, agentPrecisionY);

        // pathfind to target if it has been placed
            if(agent_target_closest_unobstructed_point.cell!=-1 && path==0)
            {
                path = agent->findShortestPathTo(context,agent_target_closest_unobstructed_point);
            }
        }

    // draw path
        testbed->setColour("green");
        testbed->drawPath(path);

    // draw any obstacles which have been placed
        size_t i;
        for(i = 0; i < obstruction_agents.size(); i++)
        {
            if(obstruction_agents[i])
            {
                if(obstruction_agents[i]->getTraverseCost() == -1.f)
                {
                    testbed->setColour("red");
                    testbed->drawAgent(obstruction_agents[i], 20);
                }
                else
                {
                    testbed->setColour("brown");
                    testbed->drawAgent(obstruction_agents[i], 4);
                }
                if(drawExpansion)
                {
                    testbed->setColour("orange");
                    testbed->drawAgentExpansion(obstruction_agents[i], agent_shape);
                }
            }
        }

    // tell the testbed to render this frame
        testbed->update();

        WaitForFrame();

        if(agent)
        {
            if(path && !paused)
            {
                if(path->size()>=2)
                {
                // set heading for the current path section
                    cPosition next_target=path->position(1);
                    cPosition current=agent->getPosition();
                    long dx,dy;
                    dx=next_target.x-current.x;
                    dy=next_target.y-current.y;
                    agent_heading = static_cast<float>(atan2(static_cast<double>(dx),static_cast<double>(dy)));
                }
                agent->advanceAlongPathWithPrecision(path, 11.f, context, agentPrecisionX, agentPrecisionY);
            }
            else
            {
                TurnAgentUnderMouseControl(testbed,agent_heading);
                MoveAgentUnderKeyControl(testbed, pathengine,
                                        agent, agent_heading, context, 7.f,
                                        agentPrecisionX, agentPrecisionY);
            }
        }

    // receive and process messages for all keys pressed since last frame
        const char *keypressed;
        while(keypressed=testbed->receiveKeyMessage())
        {
            if(keypressed[0]!='d') // is it a key down message?
                continue;

            if(keypressed[1]>='6' && keypressed[1]<='9')
            {
            // move obstruction shape
                long index = keypressed[1]-'6';
                if(static_cast<size_t>(index) < obstruction_agents.size())
                {
                // get the position on the mesh under the mouse
                // p.cell is set to -1 if the mouse is not over the mesh
                    cPosition p=testbed->positionAtMouse();
                    if(    p.cell!=-1 &&
                        (agent==0 || !agent->testCollisionDirectlyAgainstPlacedShape(obstruction_shapes[index],p))
                        )
                    {
                    // place the obstruction if not yet placed, otherwise move to the requested position
                        if(!obstruction_agents[index])
                        {
                            obstruction_agents[index]=mesh->placeAgent(obstruction_shapes[index],p);
                            context->addAgent(obstruction_agents[index]);
                        }
                        else
                        {
                            obstruction_agents[index]->moveTo(p);
                        }                        
                    // holding the left shift key makes the obstacle a 'soft obstacle'
                        if(testbed->getKeyState("_LSHIFT"))
                        {
                            // note that the cost is in addition to base movement cost
                            // so movement through the obstacle will cost twice as much as movement around
                            obstruction_agents[index]->setTraverseCost(1.f);
                        }
                        else
                        {
                            obstruction_agents[index]->setTraverseCost(-1.f);
                        }
                    // the current path may need to be regenerated
                        delete path;
                        path = 0;
                    }
                }
                continue;
            }

            switch(keypressed[1])
            {
            case '_':
                {
                    if(!strcmpi("ESCAPE",keypressed+2))
                        exitflag=true;
                    break;
                }
            case '4':
                {
                // move or place the agent
                    cPosition p=testbed->positionAtMouse();
                    if(p.cell!=-1 && !mesh->testPointCollision(agent_shape,context,p))
                    {
                        if(!agent)
                            agent=mesh->placeAgent(agent_shape,p);
                        else
                            agent->moveTo(p);
                        agent_target.cell = -1;
                        delete path;
                        path = 0;
                    }
                }
                break;
            case '5':
                {
                // set a target for pathfinding
                    cPosition p=testbed->positionAtMouse();
                    if(p.cell!=-1)
                        agent_target=p;
                    delete path;
                    path = 0;
                }
                break;
            case 'W':
            case 'A':
            case 'S':
            case 'D':
            // a direct control key has been pressed
            // delete path
            // control will revert to user control
                agent_target.cell = -1;
                delete path;
                path = 0;
                break;
            case 'E':
                drawExpansion = !drawExpansion;
                break;
            case 'P':
                paused = !paused;
                break;
            }
        }
    }
}
