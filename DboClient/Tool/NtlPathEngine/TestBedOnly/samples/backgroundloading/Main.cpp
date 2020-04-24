
#include "EntryPoint.h"
#include "Error.h"
#include <vector>
#include <string>
#include <sstream>
#include <windows.h>

iMesh*
GenerateRandomMesh(iPathEngine* pathEngine, iTestBed* testBed, const char* meshName, const std::vector<iShape*>& baseObstacleShapes)
{
    char* meshBuffer;
    unsigned long meshBufferSize;
    {
        std::string meshPath = "..\\resource\\meshes\\";
        meshPath += meshName;
        meshPath += ".tok";
        meshBuffer = testBed->loadBinary(meshPath.c_str(), meshBufferSize);
    }
    iMesh* mesh = pathEngine->loadMeshFromBuffer("tok", meshBuffer, meshBufferSize, 0);
    testBed->freeBuffer(meshBuffer);
    iCollisionContext* context = mesh->newContext();
    long i;
    for(i = 0; i < static_cast<long>(baseObstacleShapes.size()); i++)
    {
        cPosition randomPosition;
        do
        {
            randomPosition = mesh->generateRandomPosition();
        }
        while(randomPosition.cell == -1);
        iAgent* agent = mesh->placeAgent(baseObstacleShapes[i], randomPosition);
        context->addAgent(agent);
    }
    mesh->burnContextIntoMesh(context);
    return mesh;
}

class cMeshLoadingQueue
{
    long _meshIterations;
    CRITICAL_SECTION* _queueMutexes;
    iMesh** _queueMeshes;
    long _nextIndex;

    CRITICAL_SECTION _numberLoadedMutex;
    long _numberLoaded;

public:

    // initialisation to be performed before foreground thread creation
    cMeshLoadingQueue(long meshIterations);

    // called by background thread to load meshes and place on queue
    void loadMeshesAndPlaceOnQueue(
            iPathEngine* pathEngine, iTestBed* testBed,
            const char* meshName, const std::vector<iShape*>& baseObstacleShapes,
            iShape* agentShape, const char** collisionPreprocessAttributes
            );

    // called by a single foreground thread to obtain meshes from the queue
    // returns zero when queue is empty
    // the calling code takes responsibility for deleting the mesh
    iMesh* getNext();

    // called by the foreground thread just for printing in the testbed
    long getNumberLoaded();
    long getQueuePosition()
    {
        return _nextIndex;
    }

    // destruction after foreground thread is completed
    ~cMeshLoadingQueue();
};

cMeshLoadingQueue::cMeshLoadingQueue(long meshIterations)
{
    _meshIterations = meshIterations;
    _queueMutexes = new CRITICAL_SECTION[_meshIterations];
    _queueMeshes = new iMesh*[_meshIterations];
    long i;
    for(i = 0; i < _meshIterations; ++i)
    {
        InitializeCriticalSection(_queueMutexes + i);
        EnterCriticalSection(_queueMutexes + i);
    }
    _nextIndex = 0;
    InitializeCriticalSection(&_numberLoadedMutex);
    _numberLoaded = 0;
}

void
cMeshLoadingQueue::loadMeshesAndPlaceOnQueue(
            iPathEngine* pathEngine, iTestBed* testBed,
            const char* meshName, const std::vector<iShape*>& baseObstacleShapes,
            iShape* agentShape, const char** collisionPreprocessAttributes
            )
{
    long i;
    for(i = 0; i < _meshIterations; ++i)
    {
        _queueMeshes[i] = GenerateRandomMesh(pathEngine, testBed, meshName, baseObstacleShapes);
        _queueMeshes[i]->generateCollisionPreprocessFor(agentShape, collisionPreprocessAttributes);
        _queueMeshes[i]->generatePathfindPreprocessFor(agentShape, 0);
        LeaveCriticalSection(_queueMutexes + i);

        EnterCriticalSection(&_numberLoadedMutex);
        _numberLoaded++;
        LeaveCriticalSection(&_numberLoadedMutex);
    }
}

iMesh*
cMeshLoadingQueue::getNext()
{
    if(_nextIndex == _meshIterations)
    {
        return 0;
    }
    // the critical section is locked here as a way of waiting for the next queue item to be ready
    EnterCriticalSection(_queueMutexes + _nextIndex);
    LeaveCriticalSection(_queueMutexes + _nextIndex);
    return _queueMeshes[_nextIndex++];
}

long
cMeshLoadingQueue::getNumberLoaded()
{
    long result;
    EnterCriticalSection(&_numberLoadedMutex);
    result = _numberLoaded;
    LeaveCriticalSection(&_numberLoadedMutex);
    return result;
}

cMeshLoadingQueue::~cMeshLoadingQueue()
{
    assertD(_nextIndex == _meshIterations);
    long i;
    for(i = 0; i < _meshIterations; ++i)
    {
        DeleteCriticalSection(_queueMutexes + i);
    }
    delete [] _queueMutexes;
    delete [] _queueMeshes;
    DeleteCriticalSection(&_numberLoadedMutex);
}


class cGlobals
{
public:

    cGlobals(
        iTestBed* testBed,
        cMeshLoadingQueue& queue,
        iShape* agentShape,
        const std::vector<iShape*>& dynamicObstacleShapes,
        long dynamicObstacleIterations,
        long pathIterations) :
     _testBed(testBed),
     _queue(queue),
     _agentShape(agentShape),
     _dynamicObstacleShapes(dynamicObstacleShapes),
     _dynamicObstacleIterations(dynamicObstacleIterations),
     _pathIterations(pathIterations)
    {
    }

    iTestBed* _testBed;
    cMeshLoadingQueue& _queue;
    iShape* _agentShape;
    const std::vector<iShape*>& _dynamicObstacleShapes;
    long _dynamicObstacleIterations;
    long _pathIterations;
};

void DrawContext(iTestBed* testbed, iCollisionContext* context)
{
    long i;
    for(i = 0; i < context->getNumberOfAgents(); i++)
    {
        iAgent* agent = context->getAgent(i);
        testbed->drawAgent(agent, 40);
    }
}

void
DoPathIterations(cGlobals globals, iMesh* mesh, iCollisionContext* context)
{
    long pathIterations = globals._pathIterations;
    do
    {
        globals._testBed->setColourRGB(0.0f,0.0f,0.85f);
        globals._testBed->drawMesh();
        globals._testBed->setColour("blue");
        globals._testBed->drawMeshEdges();
        globals._testBed->setColour("red");
        globals._testBed->drawBurntInObstacles();
        globals._testBed->setColour("orange");
        DrawContext(globals._testBed, context);

        {
            std::ostringstream os;
            os << "meshes dispatched = " << globals._queue.getQueuePosition();
            globals._testBed->printTextLine(10, os.str().c_str());
        }
        {
            std::ostringstream os;
            os << "meshes loaded = " << globals._queue.getNumberLoaded();
            globals._testBed->printTextLine(10, os.str().c_str());
        }

        long i;
        for(i = 0; i < 10; i++)
        {
            if(!pathIterations)
            {
                break;
            }
            cPosition start;
            do
            {
                start = mesh->generateRandomPosition();
            }
            while(start.cell == -1 || mesh->testPointCollision(globals._agentShape, context, start));

            cPosition goal;
            do
            {
                goal = mesh->generateRandomPosition();
            }
            while(goal.cell == -1);

            iPath* path = mesh->findShortestPath(globals._agentShape, context, start, goal);
            globals._testBed->setColour("green");
            globals._testBed->drawPath(path);
            delete path;

            pathIterations--;
        }

        globals._testBed->update();
    }
    while(pathIterations);
}

iCollisionContext*
InitContext(iMesh* mesh, const std::vector<iShape*>& shapes, std::vector<iAgent*>& agents)
{
    iCollisionContext* result = mesh->newContext();
    cPosition randomPosition;
    do
    {
        randomPosition = mesh->generateRandomPosition();
    }
    while(randomPosition.cell == -1);
    size_t i;
    for(i = 0; i < shapes.size(); i++)
    {
        iAgent* placed = mesh->placeAgent(shapes[i], randomPosition);
        result->addAgent(placed);
        agents[i] = placed;
    }
    return result;
}
void
DeleteContextAndAgents(iCollisionContext* context)
{
    while(context->getNumberOfAgents())
    {
        iAgent* agent = context->getAgent(0);
        delete agent;
    }
    delete context;
}

void
DoDynamicObstacleIterations(cGlobals& globals, iMesh* mesh)
{
    long dynamicObstacles = globals._dynamicObstacleShapes.size();
    std::vector<iAgent*> agents(dynamicObstacles);
    iCollisionContext* context = InitContext(mesh, globals._dynamicObstacleShapes, agents);
    long i;
    for(i = 0; i < globals._dynamicObstacleIterations; i++)
    {
        long j;
        for(j = 0; j < dynamicObstacles; j++)
        {
            cPosition randomPosition;
            do
            {
                randomPosition = mesh->generateRandomPosition();
            }
            while(randomPosition.cell == -1);
            agents[j]->moveTo(randomPosition);
        }
        DoPathIterations(globals, mesh, context);
    }
    DeleteContextAndAgents(context);
}

// thread proc for the foreground pathfinding thread
DWORD WINAPI
PopMeshesAndRunQueries(LPVOID globalsVP)
{
    cGlobals& globals = *static_cast<cGlobals*>(globalsVP);

    bool isFirst = true;
    iMesh* mesh;
    while(mesh = globals._queue.getNext())
    {
        globals._testBed->setMesh(mesh);
        if(isFirst)
        {
            globals._testBed->zoomExtents();
            isFirst = false;
        }
        DoDynamicObstacleIterations(globals, mesh);
        delete mesh;
    }

    return 0;
}

void
demo(iPathEngine* pathEngine, iTestBed* testBed)
{
    iShape* shape_Square20;
    {
        long array[]=
        {
            -20, 20,
            20, 20,
            20, -20,
            -20, -20,
        };
        shape_Square20 = pathEngine->newShape(sizeof(array)/sizeof(*array)/2, array);
    }
    iShape* shape_Octagon8_20;
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
        shape_Octagon8_20 = pathEngine->newShape(sizeof(array)/sizeof(*array)/2,array);
    }
    iShape* shape_Swarmer;
    {
        long array[]=
        {
            0,16,
            14,-8,
            -14,-8,
        };
        shape_Swarmer = pathEngine->newShape(sizeof(array)/sizeof(*array)/2,array);
    }
    iShape* shape_Square60;
    {
        long array[]=
        {
            -60,60,
            60,60,
            60,-60,
            -60,-60,
        };
        shape_Square60 = pathEngine->newShape(sizeof(array)/sizeof(*array)/2,array);
    }

    const char* attributes[3];
    attributes[0] = "split_with_circumference_below";
    attributes[1] = "2000";
    attributes[2] = 0;

    std::vector<iShape*> baseObstacles;
    baseObstacles.push_back(shape_Square60);
    baseObstacles.push_back(shape_Square60);
    baseObstacles.push_back(shape_Square60);
    baseObstacles.push_back(shape_Square60);
    baseObstacles.push_back(shape_Square60);
    baseObstacles.push_back(shape_Swarmer);
    baseObstacles.push_back(shape_Swarmer);
    baseObstacles.push_back(shape_Swarmer);
    baseObstacles.push_back(shape_Swarmer);
    baseObstacles.push_back(shape_Swarmer);
    baseObstacles.push_back(shape_Swarmer);
    baseObstacles.push_back(shape_Square20);
    baseObstacles.push_back(shape_Square20);
    baseObstacles.push_back(shape_Octagon8_20);
    baseObstacles.push_back(shape_Octagon8_20);
    baseObstacles.push_back(shape_Octagon8_20);
    baseObstacles.push_back(shape_Octagon8_20);
    std::vector<iShape*> dynamicObstacles;
    dynamicObstacles.push_back(shape_Square60);
    dynamicObstacles.push_back(shape_Swarmer);
    dynamicObstacles.push_back(shape_Octagon8_20);

    cMeshLoadingQueue queue(100);

    iShape* agentShape = shape_Swarmer;
    cGlobals globals(testBed, queue, agentShape, dynamicObstacles,
                    5, // dynamic obstacle iterations
                    50 // path iterations
                    );

    HANDLE foregroundThread = CreateThread(0, 0, PopMeshesAndRunQueries, static_cast<LPVOID>(&globals), 0, 0);

    queue.loadMeshesAndPlaceOnQueue(pathEngine, testBed, "mesh1", baseObstacles, shape_Swarmer, attributes);

    DWORD result = WaitForSingleObject(foregroundThread, INFINITE);
    assertD(result == WAIT_OBJECT_0);
}
