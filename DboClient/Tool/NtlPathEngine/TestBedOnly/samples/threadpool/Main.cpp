
#include "LoadPathEngine.h"
#include "i_pathEngine.h"
#include "LoadBinary.h"
#include "ErrorHandler.h"
#include "Error.h"
#include <sstream>
#include <vector>
#include <windows.h>
#include <time.h>
#include <iostream>

class cGlobals
{
    iMesh* _mesh;
    const std::vector<iShape*>& _agentShapes;
    std::vector<iAgent*> _obstacleAgents;
    iCollisionContext* _context;
    long _newAgentFrequency;
    long _moveObstacleFrequency;

    CRITICAL_SECTION _communicationMutex;
    long _iterationCount;
    bool _measurementCompleted;

    void signalIterationCompleted()
    {
        EnterCriticalSection(&_communicationMutex);
        _iterationCount++;
        LeaveCriticalSection(&_communicationMutex);
    }

    bool testMeasurementCompleted()
    {
        EnterCriticalSection(&_communicationMutex);
        bool result = _measurementCompleted;
        LeaveCriticalSection(&_communicationMutex);
        return result;
    }

    cPosition getRandomUnobstructedPosition(iShape* shape) // unobstructed against the base mesh
    {
        cPosition result;
        do
        {
            result = _mesh->generateRandomPosition();
        }
        while(result.cell == -1 || _mesh->testPointCollision(shape, 0, result));
        return result;
    }

    iAgent* placeRandomAgent()
    {
        long i = (rand() % _agentShapes.size());        
        return _mesh->placeAgent(_agentShapes[i], getRandomUnobstructedPosition(_agentShapes[i]));
    }

    cPosition getRandomUnobstructedPosition(iAgent* agent) // unobstructed against the base mesh
    {
        return getRandomUnobstructedPosition(agent->getShape());
    }

    iAgent* getRandomObstacle()
    {
        long i = (rand() % _obstacleAgents.size());        
        return _obstacleAgents[i];
    }

    cPosition getRandomPosition()
    {
        cPosition result;
        do
        {
            result = _mesh->generateRandomPosition();
        }
        while(result.cell == -1);
        return result;
    }

public:

    cGlobals(
            iMesh* mesh,
            const std::vector<iShape*>& agentShapes,
            const std::vector<iShape*>& obstacleShapes,
            long newAgentFrequency,
            long moveObstacleFrequency
            ) :
      _mesh(mesh),
      _agentShapes(agentShapes),
      _newAgentFrequency(newAgentFrequency),
      _moveObstacleFrequency(moveObstacleFrequency)
    {
        _context = _mesh->newContext();
        size_t i;
        for(i = 0; i < obstacleShapes.size(); ++i)
        {
            _obstacleAgents.push_back(_mesh->placeAgent(obstacleShapes[i], getRandomPosition()));
            _context->addAgent(_obstacleAgents.back());
        }
        InitializeCriticalSection(&_communicationMutex);
        _iterationCount = 0;
        _measurementCompleted = false;
    }
    ~cGlobals()
    {
        DeleteCriticalSection(&_communicationMutex);
        delete _context;
        size_t i;
        for(i = 0; i < _obstacleAgents.size(); ++i)
        {
            delete _obstacleAgents[i];
        }
    }

  // this is run by each worker thread
    void runIterations();

  // called by the main thread for measuring throughput
    long getIterationCount()
    {
        EnterCriticalSection(&_communicationMutex);
        long result = _iterationCount;
        LeaveCriticalSection(&_communicationMutex);
        return result;
    }
  // called by the main thread to signal when the measurement has been completed
    void signalMeasurementCompleted()
    {
        EnterCriticalSection(&_communicationMutex);
        _measurementCompleted = true;
        LeaveCriticalSection(&_communicationMutex);
    }
  // called by the main thread to clear the measurement complete signal
  // so that the globals object can be reused
    void clearMeasurementCompleteSignal()
    {
        EnterCriticalSection(&_communicationMutex);
        _measurementCompleted = false;
        LeaveCriticalSection(&_communicationMutex);
    }
};

void
cGlobals::runIterations()
{
    long count = 0;
    iAgent* agent = placeRandomAgent();
    while(!testMeasurementCompleted())
    {
        iPath* path = agent->findShortestPathTo(_context, getRandomUnobstructedPosition(agent));
        if(path)
        {
            agent->moveTo(path->position(path->size() - 1));
            delete path;
        }
        count++;
        if((count % _newAgentFrequency) == 0)
        {
            delete agent;
            agent = placeRandomAgent();
        }
        if((count % _moveObstacleFrequency) == 0)
        {
            iAgent* obstacleAgent = getRandomObstacle();
            obstacleAgent->moveTo(getRandomPosition());
        }
        signalIterationCompleted();
    }
    delete agent;
}

// thread proc for the worker threads
DWORD WINAPI
RunIterations(LPVOID globalsVP)
{
    cGlobals& globals = *static_cast<cGlobals*>(globalsVP);
    globals.runIterations();

    return 0;
}


static void
RunThreadPoolAndMeasureThroughput(long numberOfThreads, cGlobals& globals)
{
    assertD(numberOfThreads > 0);
    std::vector<HANDLE> threadHandles(numberOfThreads);

    std::cout << "starting ";
    if(numberOfThreads == 1)
    {
        std::cout << "1 thread\n";
    }
    else
    {
        std::cout << numberOfThreads << " threads\n";
    }

    long i;
    for(i = 0; i < numberOfThreads; ++i)
    {
        threadHandles[i] = CreateThread(0, 0, RunIterations, static_cast<LPVOID>(&globals), 0, 0);
    }

    Sleep(2000); // warm up time (milliseconds)
    std::cout << "warmed up, starting timer\n";

    long startIterationCount = globals.getIterationCount();
    clock_t startTime = clock();

    for(i = 0; i != 30; ++i)
    {
        std::cout << '.';
        Sleep(500);
    }
    std::cout << '\n';

    long endIterationCount = globals.getIterationCount();
    clock_t endTime = clock();

    globals.signalMeasurementCompleted();

    DWORD result = WaitForMultipleObjects(numberOfThreads, &threadHandles.front(), true, INFINITE);
    assertD(result >= WAIT_OBJECT_0 && result <= WAIT_OBJECT_0 + (numberOfThreads - 1));

    globals.clearMeasurementCompleteSignal();

    double duration = static_cast<double>(endTime - startTime) / CLOCKS_PER_SEC;
    double throughput = (endIterationCount - startIterationCount) / duration;

    std::cout << "throughput = " << throughput << " iterations per second\n";
}

int
main(int argC, char** argV)
{
    iPathEngine* pathEngine = LoadPathEngine("PathEngine", &cErrorHandler::refInstance());
    if(!pathEngine)
    {
        return 0;
    }

// check if interface for dll loaded is compatible with the headers used for compilation
    if(pathEngine->getInterfaceMajorVersion() != PATHENGINE_INTERFACE_MAJOR_VERSION
        ||
        pathEngine->getInterfaceMinorVersion() < PATHENGINE_INTERFACE_MINOR_VERSION)
    {
        cErrorHandler::refInstance().handle("Fatal", "LoadPathEngine: pathengine version is incompatible with headers used for compilation.",0);
        return 0;
    }

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

    std::vector<iShape*> agentShapes;
    agentShapes.push_back(shape_Swarmer);
    agentShapes.push_back(shape_Square20);

    std::vector<iShape*> obstacleShapes;
    obstacleShapes.push_back(shape_Square60);
    obstacleShapes.push_back(shape_Swarmer);
    obstacleShapes.push_back(shape_Octagon8_20);

    iMesh* mesh;

    {
        std::vector<char> buffer;
        std::string fullName = "..\\resource\\meshes\\mesh1.tok";
        LoadBinary(fullName.c_str(), buffer);
        mesh = pathEngine->loadMeshFromBuffer("tok", &buffer.front(), buffer.size(), 0);
    }

    {
        const char* attributes[3];
        attributes[0] = "split_with_circumference_below";
        attributes[1] = "2000";
        attributes[2] = 0;
        size_t i;
        for(i = 0; i < agentShapes.size(); ++i)
        {
            if(mesh->shapeCanPathfind(agentShapes[i]))
            {
                continue;
            }
            mesh->generateCollisionPreprocessFor(agentShapes[i], attributes);
            mesh->generatePathfindPreprocessFor(agentShapes[i], 0);
        }
        mesh->preprocessGenerationCompleted();
    }

    std::ostringstream results;

    cGlobals globals(mesh, agentShapes, obstacleShapes,
                10, // new agent frequency
                8 // move obstacle frequency
                );

    RunThreadPoolAndMeasureThroughput(1, globals);
    RunThreadPoolAndMeasureThroughput(5, globals);
    RunThreadPoolAndMeasureThroughput(40, globals);

    std::cout << "completed";
    {
        char c;
        std::cin.get(c);
    }

    return 0;
}
