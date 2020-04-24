
#include "entrypoint.h"

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

    while(!testbed->getKeyState("_ESCAPE"))
    {
        testbed->setColourRGB(0.0f,0.0f,0.85f);
        testbed->drawMesh();
        testbed->setColour("blue");
        testbed->drawMeshEdges();
        testbed->update();
    }
}

