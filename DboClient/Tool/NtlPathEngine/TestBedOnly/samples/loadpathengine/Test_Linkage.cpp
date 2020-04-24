
#include "test_linkage.h"
#include <string.h>
#include <stdlib.h>
#include "i_pathengine.h"

const char*
GetAttributeValue(const char *const* attributes, const char *name, iErrorHandler *error_handler)
{
    while(1)
    {
        if(!attributes || *attributes==0)
            return 0;
        if(strcmp(name,attributes[0])==0)
            return attributes[1];
        attributes+=2;
    }
}
bool CheckAttributeValue(iPathEngine *pathengine, iErrorHandler *error_handler, const char *name, long required_value)
{
    const char *value;
    long value_long;
    
    value = GetAttributeValue(pathengine->getVersionAttributes(), name,error_handler);
    if(!value)
    {
        error_handler->handle("NonFatal", "Required version attribute was not found.",0);
        return false;
    }
    value_long=strtol(value,0,10);
    if(value_long != required_value)
    {
        error_handler->handle("NonFatal", "The value for a version attribute does not agree with direct query.",0);
        return false;
    }
    return true;
}

class cSimpleTerrainCallBack : public iTerrainCallBack
{
public:
    float getHeightAt(long x, long y)
    {
        return 52.f;
    }
    float getHeightAtWithPrecision(long x, long y, float precisionX, float precisionY)
    {
        return 52.f;
    }
};

class cSimpleQueryCallBack : public iQueryCallBack
{
    long _count;

public:

    cSimpleQueryCallBack()
    {
        _count = 1;
    }

// iQueryCallBack

    long desiredCallBackFrequency()
    {
        return 1;
    }
    bool abort()
    {
        --_count;
        return _count == 0;
    }
};

bool Test_Linkage(iPathEngine *pathengine, iErrorHandler *error_handler)
{
    if(!CheckAttributeValue(pathengine,error_handler,"interface_minor_version",pathengine->getInterfaceMinorVersion()))
        return false;
    if(!CheckAttributeValue(pathengine,error_handler,"interface_major_version",pathengine->getInterfaceMajorVersion()))
        return false;

    pathengine->setErrorHandler(error_handler);
    pathengine->deleteAllObjects();

    iShape *shape;
    {
        long coordinate_data[]=
        {
            -10,-10,
            -10,10,
            10,10,
            10,-10,
        };
        shape=pathengine->newShape(sizeof(coordinate_data)/sizeof(*coordinate_data)/2,coordinate_data);
        if(shape==0)
        {
            error_handler->handle("NonFatal", "newShape() returned 0.",0);
            return false;
        }
    }

    iMesh *mesh;
    {
        const char mesh_string[]=
"<mesh>"
	"<mesh3D>"
		"<verts>"
			"<vert x=\"-100\" y=\"100\" z=\"51\"/>"
			"<vert x=\"100\" y=\"100\" z=\"51\"/>"
			"<vert x=\"-100\" y=\"-100\" z=\"51\"/>"
			"<vert x=\"100\" y=\"-100\" z=\"45\"/>"
		"</verts>"
		"<tris>"
			"<tri sectionID=\"0\" surfaceType=\"1\" edge0StartVert=\"0\" edge1StartVert=\"1\" edge2StartVert=\"2\"/>"
			"<tri edge0StartVert=\"1\" edge1StartVert=\"3\" edge2StartVert=\"2\" edge2Connection=\"0\" edge0StartZ=\"50\" edge2StartZ=\"35\"/>"
		"</tris>"
	"</mesh3D>"
"</mesh>";
        mesh=pathengine->loadMeshFromBuffer("xml", mesh_string, strlen(mesh_string), 0);
        if(mesh==0)
        {
            error_handler->handle("NonFatal", "loadMeshFromBuffer() returned 0.",0);
            return false;
        }
    }

    pathengine->setPathPoolParameters(2,2);

    if(shape->size() != 4)
    {
        error_handler->handle("NonFatal", "shape->size() does not agree.",0);
        return false;
    }
    {
        long x = 0;
        long y = 0;
        shape->vertex(3, x, y);
        if(x != 10 || y != -10)
        {
            error_handler->handle("NonFatal", "shape->vertex() does not agree.",0);
            return false;
        }
    }

    if(mesh->getNumberOf3DFaces() != 2)
    {
        error_handler->handle("NonFatal", "Bad result from cMesh::getNumberOf3DFaces().",0);
        return false;
    }
    {
        long baseVertexIndex = 0;
        if(mesh->get3DFaceConnection(0, baseVertexIndex) == -1)
        {
            baseVertexIndex++;
        }
        if(mesh->get3DFaceConnection(0, baseVertexIndex) == -1)
        {
            baseVertexIndex++;
        }

        if(mesh->get3DFaceConnection(0, baseVertexIndex) != 1)
        {
            error_handler->handle("NonFatal", "Bad result from cMesh::get3DFaceConnection().",0);
            return false;
        }
        if(mesh->get3DFaceConnection(0, (baseVertexIndex + 1) & 3) != -1)
        {
            error_handler->handle("NonFatal", "Bad result from cMesh::get3DFaceConnection().",0);
            return false;
        }

        long x, y;
        float z;
        mesh->get3DFaceVertex(0, 1, x, y, z);
        if(z != 51.f
           &&
           (x != -100 && y != 100)
           &&
           (x != 100 && y != 100)
           &&
           (x != -100 && y != -100)
           )
        {
            error_handler->handle("NonFatal", "Bad result from cMesh::get3DFaceVertex().",0);
            return false;
        }
        mesh->get3DFaceVertex(1, 2, x, y, z);
        if(z != 50.f
           &&
           (x != 100 && y != 100)
           &&
           (x != 100 && y != -100)
           &&
           (x != -100 && y != -100)
           )
        {
            error_handler->handle("NonFatal", "Bad result from cMesh::get3DFaceVertex().",0);
            return false;
        }
    }

    cPosition position;
    {
        long point[]={61,60,60};
        position=mesh->positionFor3DPoint(point);
        if(position.cell==-1 || position.x != 61 || position.y != 60)
        {
            error_handler->handle("NonFatal", "Incorrect position from positionFor3DPoint().",0);
            return false;
        }
        float pointf[]={61.0f,60.0f,60.0f};
        if(mesh->positionFor3DPointF(pointf) != position)
        {
            error_handler->handle("NonFatal", "positionFor3DPoint() does not agree with positionFor3DPointF() (coords=<60,60,60>).",0);
            return false;
        }
    }

    if(mesh->getSurfaceType(position) != 0)
    {
        error_handler->handle("NonFatal", "getSurfaceType() is incorrect.",0);
        return false;
    }
    if(mesh->get3DFaceAtPosition(position) != 1)
    {
        error_handler->handle("NonFatal", "get3DFaceIndex() is incorrect.",0);
        return false;
    }
    {
        float normalX, normalY, normalZ;
        long faceIndex = mesh->get3DFaceAtPosition(position);
        mesh->get3DFaceNormal(faceIndex, normalX, normalY, normalZ);
        if(normalX < -0.050f
         || normalX > -0.049f
         || normalY < -0.025f
         || normalY > -0.024f
         || normalZ < 0.998f
         || normalZ > 0.999f)
        {
            error_handler->handle("NonFatal", "getGroundNormal() is incorrect.",0);
            return false;
        }
    }

    cPosition position2;
    {
        long point[]={-95,0,60};
        position2=mesh->positionFor3DPoint(point);
    }

    mesh->addEndPoint(position);
    mesh->addEndPoint(position2);
    mesh->addOffMeshConnection(0, 1, 1000);

    if(mesh->heightAtPosition(position2) != 51)
    {
        error_handler->handle("NonFatal", "heightAtPosition() is incorrect (horizontal face).",0);
        return false;
    }

    cSimpleTerrainCallBack callBack;
    mesh->setTerrainCallBack(0, &callBack);
    if(mesh->heightAtPosition(position2) != 52)
    {
        error_handler->handle("NonFatal", "heightAtPosition() is incorrect (terrain section).",0);
        return false;
    }

    if(mesh->getSurfaceType(position2) != 1)
    {
        error_handler->handle("NonFatal", "getSurfaceType() is incorrect.",0);
        return false;
    }
    if(mesh->get3DFaceAtPosition(position2) != 0)
    {
        error_handler->handle("NonFatal", "get3DFaceIndex() is incorrect.",0);
        return false;
    }

// generate all states for the preprocess and check that helper functions agree
    if(mesh->shapeCanCollide(shape) || mesh->shapeCanPathfind(shape))
    {
        error_handler->handle("NonFatal", "preprocess helpers incorrect.",0);
        return false;
    }
    mesh->generateCollisionPreprocessFor(shape, 0);
    if(!mesh->shapeCanCollide(shape) || mesh->shapeCanPathfind(shape))
    {
        error_handler->handle("NonFatal", "preprocess helpers incorrect.",0);
        return false;
    }
    {
        const char *const* attributes = mesh->getCollisionPreprocessAttributes(shape);
        if(attributes[0] != 0)
        {
            error_handler->handle("NonFatal", "getCollisionPreprocessAttributes() result incorrect.", 0);
            return false;
        }
    }
    mesh->releaseAllPreprocessFor(shape);
    if(mesh->shapeCanCollide(shape) || mesh->shapeCanPathfind(shape))
    {
        error_handler->handle("NonFatal", "preprocess helpers incorrect.",0);
        return false;
    }
    {
        const char* attributes[3];
        attributes[0] = "split_with_circumference_below";
        attributes[1] = "1200";
        attributes[2] = 0;
        mesh->generateCollisionPreprocessFor(shape, attributes);
    }
    mesh->generatePathfindPreprocessFor(shape, 0);
    if(!mesh->shapeCanCollide(shape) || !mesh->shapeCanPathfind(shape))
    {
        error_handler->handle("NonFatal", "preprocess helpers incorrect.",0);
        return false;
    }
    {
        const char *const* attributes = mesh->getCollisionPreprocessAttributes(shape);
        if(strcmp(attributes[0], "split_with_circumference_below") != 0
         || strcmp(attributes[1], "1200") != 0
         || attributes[2] != 0)
        {
            error_handler->handle("NonFatal", "getCollisionPreprocessAttributes() result incorrect.", 0);
            return false;
        }
    }

    if(mesh->getCellForEndOfLine(position, position2.x, position2.y) != position2.cell)
    {
        error_handler->handle("NonFatal", "getCellForEndOfLine() is incorrect.",0);
        return false;
    }

    iAgent *agent=mesh->placeAgent(shape, position);
    if(agent==0)
    {
        error_handler->handle("NonFatal", "placeAgent() returned 0.",0);
        return false;
    }
    {
        void* userdata=agent->getUserData();
        if(userdata != 0)
        {
            error_handler->handle("NonFatal", "Agent userdata is not initially 0.",0);
            return false;
        }
        agent->setUserData((void*)1);
        userdata=agent->getUserData();
        if(userdata != (void*)1)
        {
            error_handler->handle("NonFatal", "iAgent::getUserData() does not agree with value passed in to iAgent::setUserData().",0);
            return false;
        }
    }

    iCollisionContext *context=mesh->newContext();
    if(context==0)
    {
        error_handler->handle("NonFatal", "newContext() returned 0.",0);
        return false;
    }
    context->updateCollisionPreprocessFor(shape);
    context->updatePathfindingPreprocessFor(shape);

    if(mesh->testPointCollision(shape, context, position))
    {
        error_handler->handle("NonFatal", "testPointCollision() is incorrect.",0);
        return false;
    }
    if(!mesh->testPointCollision(shape, context, position2))
    {
        error_handler->handle("NonFatal", "testPointCollision() is incorrect.",0);
        return false;
    }
    if(!mesh->testLineCollision(shape, context, position, position2))
    {
        error_handler->handle("NonFatal", "testLineCollision() is incorrect.",0);
        return false;
    }

    cPosition position3;
    {
        long point[]={-90,-90,60};
        position3=mesh->positionFor3DPoint(point);
    }
    if(mesh->testLineCollision(shape, context, position, position3))
    {
        error_handler->handle("NonFatal", "testLineCollision() is incorrect.",0);
        return false;
    }

    {
        long cell;
    // if there is no collision then cell is set for point at end of line
        if(mesh->testLineCollision_XY(shape,context,position,position3.x,position3.y,cell))
        {
            error_handler->handle("NonFatal", "testLineCollision_XY() returnvalue is incorrect.",0);
            return false;
        }
        if(cell != position3.cell)
        {
            error_handler->handle("NonFatal", "testLineCollision_XY() cell is incorrect.",0);
            return false;
        }
    }

    {
        long cell;
        iCollisionInfo *collisioninfo=mesh->firstCollision(shape,context,position,position2.x,position2.y,cell);
        if(collisioninfo==0)
        {
            error_handler->handle("NonFatal", "firstCollision() returnvalue is incorrect.",0);
            return false;
        }
        long coords[4];
        collisioninfo->getCollidingLine(coords);
        if(coords[0] != -90 || coords[2] != -90 || coords[1]<=coords[3])
        {
            error_handler->handle("NonFatal", "getCollidingLine() line coords are incorrect.",0);
            return false;
        }
        collisioninfo->destroy();
    }

    {
        cPosition result;
        result=mesh->findClosestUnobstructedPosition(shape, context, position2, 10);
        if(result.cell == -1 || result.x != -90 || result.y != 0)
        {
            error_handler->handle("NonFatal", "iMesh::findClosestUnobstructedPosition() failed.",0);
            return false;
        }
        result=mesh->findClosestUnobstructedPosition(shape, context, position2, 4);
        if(result.cell != -1)
        {
            error_handler->handle("NonFatal", "iMesh::findClosestUnobstructedPosition() failed.",0);
            return false;
        }
        result=agent->findClosestUnobstructedPosition(context, 10);
        if(result.cell == -1 || result.x != 61 || result.y != 60)
        {
            error_handler->handle("NonFatal", "iAgent::findClosestUnobstructedPosition() failed.",0);
            return false;
        }
        result=mesh->findClosestUnobstructedPosition(shape, 0, position2, 10);
        if(result.cell == -1 || result.x != -90 || result.y != 0)
        {
            error_handler->handle("NonFatal", "iMesh::findClosestUnobstructedPosition() with null context failed.",0);
            return false;
        }
    }

// test mesh->findShortestPath() and path interface at the same time
    {
        iPath* path = mesh->findShortestPath(shape, context, position, position3);
        if(path == 0)
        {
            error_handler->handle("NonFatal", "findShortestPath() returned 0.",0);
            return false;
        }
        if(path->size() != 2)
        {
            error_handler->handle("NonFatal", "size() incorrect.",0);
            return false;
        }
        if(path->position(0) != position || path->position(1) != position3)
        {
            error_handler->handle("NonFatal", "path positions are incorrect.",0);
            return false;
        }
        {
            const cPosition *array=path->getPositionArray();
            if(array[0] != position || array[1] != position3)
            {
                error_handler->handle("NonFatal", "path positions are incorrect.",0);
                return false;
            }
        }
        if(path->getLength() != 212)
        {
            error_handler->handle("NonFatal", "path length is incorrect.",0);
            return false;
        }
        path->destroy();
    }
    
    {
        cSimpleQueryCallBack callBack;
        iPath* path = mesh->findShortestPath_WithQueryCallBack(shape, context, position, position3, &callBack);
        if(path == 0)
        {
            error_handler->handle("NonFatal", "findShortestPath() returned 0.",0);
            return false;
        }
        if(path->size() != 1)
        {
            error_handler->handle("NonFatal", "size() incorrect.",0);
            return false;
        }
        if(path->position(0) != position)
        {
            error_handler->handle("NonFatal", "path positions are incorrect.",0);
            return false;
        }
        path->destroy();
    }

    {
        iPath* path=mesh->findPathAway(shape,context,position,position2,170);
        if(path==0)
        {
            error_handler->handle("NonFatal", "mesh->findPathAway() returned 0.",0);
            return false;
        }
        if(path->size() != 2)
        {
            error_handler->handle("NonFatal", "size() incorrect.",0);
            return false;
        }
        if(path->position(0) != position
            ||
            path->position(1).x != 75
            ||
            path->position(1).y != 60
            )
        {
            error_handler->handle("NonFatal", "path positions are incorrect.",0);
            return false;
        }
        path->destroy();
    }

    
    context->addAgent(agent); // agent should ignore itself

    if(context->getNumberOfAgents() != 1)
    {
        error_handler->handle("NonFatal", "context->getNumberOfAgents() incorrect.",0);
        return false;        
    }
    if(context->getAgent(0) != agent)
    {
        error_handler->handle("NonFatal", "context->getAgent() incorrect.",0);
        return false;        
    }

    {
    // check collision info for agent collision
        long cell;
        iCollisionInfo *collisioninfo=mesh->firstCollision(shape,context,position3,position.x,position.y,cell);
        if(collisioninfo==0)
        {
            error_handler->handle("NonFatal", "firstCollision() returnvalue is incorrect.",0);
            return false;
        }
        if(collisioninfo->getCollidingAgent() != agent)
        {
            error_handler->handle("NonFatal", "getCollidingAgent() incorrect.",0);
            return false;
        }
        collisioninfo->destroy();
    }

    {
        iAgent *agent2=mesh->placeAgent(shape, position);
        if(agent2==0)
        {
            error_handler->handle("NonFatal", "placeAgent() returned 0.",0);
            return false;
        }
        context->addAgent(agent2);
        if(context->getNumberOfAgents() != 2)
        {
            error_handler->handle("NonFatal", "context->getNumberOfAgents() incorrect.",0);
            return false;        
        }
        // agents can be in any order
        if(context->getAgent(0)==agent)
        {
            if(context->getAgent(1) != agent2)
            {
                error_handler->handle("NonFatal", "context->getAgent() incorrect.",0);
                return false;        
            }
        }
        else
        {
            if(context->getAgent(0) != agent2)
            {
                error_handler->handle("NonFatal", "context->getAgent() incorrect.",0);
                return false;        
            }
            if(context->getAgent(1) != agent)
            {
                error_handler->handle("NonFatal", "context->getAgent() incorrect.",0);
                return false;        
            }
        }

        delete agent2; // agent should automatically remove itself from the context
        if(context->getNumberOfAgents() != 1)
        {
            error_handler->handle("NonFatal", "context->getNumberOfAgents() incorrect.",0);
            return false;        
        }
    }

    if(agent->testCollisionAt(context,position))
    {
        error_handler->handle("NonFatal", "agent->testCollisionAt() incorrect.",0);
        return false;        
    }
    if(!agent->testCollisionAt(context,position2))
    {
        error_handler->handle("NonFatal", "agent->testCollisionAt() incorrect.",0);
        return false;        
    }

    if(agent->testCollisionTo(context,position))
    {
        error_handler->handle("NonFatal", "agent->testCollisionTo() incorrect.",0);
        return false;        
    }
    if(!agent->testCollisionTo(context,position2))
    {
        error_handler->handle("NonFatal", "agent->testCollisionTo() incorrect.",0);
        return false;        
    }

    
    {
        long cell;
        if(agent->testCollisionTo_XY(context,position3.x,position3.y,cell))
        {
            error_handler->handle("NonFatal", "agent->testCollisionTo_XY() returnvalue incorrect.",0);
            return false;        
        }
        if(cell != position3.cell)
        {
            error_handler->handle("NonFatal", "agent->testCollisionTo_XY() cell incorrect.",0);
            return false;        
        }
    }
    {
        long cell;
        iCollisionInfo *collisioninfo=agent->firstCollisionTo(context,position2.x,position2.y,cell);
        if(!collisioninfo)
        {
            error_handler->handle("NonFatal", "agent->firstCollisionTo() returnvalue incorrect.",0);
            return false;        
        }
        long coords[4];
        collisioninfo->getCollidingLine(coords);
        if(coords[0] != -90 || coords[2] != -90 || coords[1]<=coords[3])
        {
            error_handler->handle("NonFatal", "getCollidingLine() line coords are incorrect.",0);
            return false;
        }
        collisioninfo->destroy();
    }

    {
        iPath* path=agent->findPathAway(context, position2, 170);
        if(path==0)
        {
            error_handler->handle("NonFatal", "agent->findPathAway() returned 0.",0);
            return false;
        }
        if(path->size() != 2)
        {
            error_handler->handle("NonFatal", "size() incorrect.",0);
            return false;
        }
        if(path->position(0) != position
            ||
            path->position(1).x != 75
            ||
            path->position(1).y != 60
            )
        {
            error_handler->handle("NonFatal", "path positions are incorrect.",0);
            return false;
        }
        path->destroy();
    }



    {
        cPosition positionsBuffer[3];
        long connectionIndicesBuffer[] = {-1, 0};
        positionsBuffer[0] = position3;
        positionsBuffer[1] = position;
        positionsBuffer[2] = position2;
        iPath* path;
        path = mesh->constructPath(positionsBuffer, connectionIndicesBuffer, 3);
        if(path->size() != 3
            ||
            path->position(0) != position3
            ||
            path->position(1) != position
            ||
            path->position(2) != position2
            ||
            path->connectionIndex(0) != -1
            ||
            path->connectionIndex(1) != 0
            )
        {
            error_handler->handle("NonFatal", "iMesh::constructPath(): Bad path constructed.", 0);
            return false;
        }
        path->destroy();
        path = mesh->constructPath_Reversed(positionsBuffer, connectionIndicesBuffer, 3);
        if(path->size() != 3
            ||
            path->position(0) != position2
            ||
            path->position(1) != position
            ||
            path->position(2) != position3
            ||
            path->connectionIndex(0) != 0
            ||
            path->connectionIndex(1) != -1
            )
        {
            error_handler->handle("NonFatal", "iMesh::constructPath(): Bad path constructed.", 0);
            return false;
        }
        path->destroy();
        path = mesh->constructPath(positionsBuffer, 0, 2);
        if(path->size() != 2
            ||
            path->position(0) != position3
            ||
            path->position(1) != position
            ||
            path->connectionIndex(0) != -1
            )
        {
            error_handler->handle("NonFatal", "iMesh::constructPath(): Bad path constructed.", 0);
            return false;
        }
        path->destroy();
    }

    agent->moveTo(position3);
    if(agent->getPosition() != position3)
    {
        error_handler->handle("NonFatal", "agent->moveTo() or agent->getPosition() failed.",0);
        return false;
    }

    {
        iPath* path=agent->findShortestPathTo(context,position);
        if(path==0)
        {
            error_handler->handle("NonFatal", "agent->findShortestPathTo() returned 0.",0);
            return false;
        }
        if(path->size() != 2)
        {
            error_handler->handle("NonFatal", "size() incorrect.",0);
            return false;
        }
        if(path->position(0) != position3 || path->position(1) != position)
        {
            error_handler->handle("NonFatal", "path positions are incorrect.",0);
            return false;
        }

        {
            iCollisionInfo *collisioninfo=agent->advanceAlongPath(path,1000.0f,context);
            if(collisioninfo)
            {
                error_handler->handle("NonFatal", "agent->advanceAlongPath() returnvalue incorrect.",0);
                return false;
            }
            if(path->size() != 1)
            {
                error_handler->handle("NonFatal", "agent->advanceAlongPath() effected path incorrect.",0);
                return false;
            }
        }
        path->destroy();
    }

    if(!agent->testCollisionDirectlyAgainstPlacedShape(shape,position))
    {
        error_handler->handle("NonFatal", "agent->testCollisionDirectlyAgainstPlacedShape() failed.",0);
        return false;
    }

    if(agent->getMesh() != mesh || agent->getShape() != shape)
    {
        error_handler->handle("NonFatal", "agent helper function failed.",0);
        return false;
    }

    if(!context->includes(agent))
    {
        error_handler->handle("NonFatal", "context->includes() failed.",0);
        return false;
    }
    context->removeAgent(agent);
    if(context->includes(agent))
    {
        error_handler->handle("NonFatal", "context->includes() failed.",0);
        return false;
    }

    context->destroy();

    agent->destroy();
    mesh->destroy();
    shape->destroy();

    return true;
}
