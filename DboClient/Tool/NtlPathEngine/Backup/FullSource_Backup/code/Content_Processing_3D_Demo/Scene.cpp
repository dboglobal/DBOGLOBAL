//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "Scene.h"
#include "MeshObject.h"
#include "SolidObject.h"
#include "libs/TestBed_Interface/interface/SetZBias.h"
#include "libs/TestBed_Interface/interface/ZoomExtents.h"
#include "libs/TestBed_Interface/interface/Singletons_TestBed.h"
#include "libs/TestBed_Interface/interface/ProgressBar.h"
#include "libs/Content_Processing_3D/interface/BuildMeshFrom3DContent.h"
#include "libs/Geometry/interface/tPoint3.h"
#include "common/interface/iXMLOutputStream.h"
#include "common/STL_Helpers.h"
#include "i_testbed.h"

void
cScene::decomposeCurrentlySelected(bool& isMeshObject, tSigned32& vectorIndex) const
{
    assertD(_currentlySelected != -1);
    vectorIndex = _currentlySelected;
    isMeshObject = true;
    if(vectorIndex >= SizeL(_meshObjects))
    {
        vectorIndex -= SizeL(_meshObjects);
        assertD(vectorIndex < SizeL(_solidObjects));
        isMeshObject = false;
    }
}

bSceneObject&
cScene::currentlySelectedSceneObject()
{
    bool isMeshObject;
    tSigned32 vectorIndex;
    decomposeCurrentlySelected(isMeshObject, vectorIndex);
    if(isMeshObject)
    {
        return *_meshObjects[vectorIndex];
    }
    return *_solidObjects[vectorIndex];
}

void
cScene::zoomExtentsSelected()
{
    bool isMeshObject;
    tSigned32 vectorIndex;
    decomposeCurrentlySelected(isMeshObject, vectorIndex);
    std::vector<tPoint3> points;
    if(isMeshObject)
    {
        _meshObjects[vectorIndex]->addAllPoints(points);
    }
    else
    {
        _solidObjects[vectorIndex]->addAllPoints(points);
    }
    ZoomExtents(points);
}

cScene::cScene()
{
    _currentlySelected = -1;
}
cScene::~cScene()
{
    tSigned32 i;
    for(i = 0; i != SizeL(_meshObjects); ++i)
    {
        delete _meshObjects[i];
    }
    for(i = 0; i != SizeL(_solidObjects); ++i)
    {
        delete _solidObjects[i];
    }
}

bool
cScene::empty() const
{
    return _meshObjects.empty() && _solidObjects.empty();
}

void
cScene::initSelection()
{
    assertR(!empty());
    _currentlySelected = 0;
}

void
cScene::updateWithEditing()
{
    assertD(_currentlySelected != -1);
    iTestBed& testBed = nSingletons::testBedI();
    {
        bSceneObject& currentSceneObject = currentlySelectedSceneObject();
        tSigned32 dx, dy;
        testBed.getMouseScreenDelta(dx, dy);
        if(testBed.getKeyState("_LSHIFT") && testBed.getKeyState("_LMOUSE") && testBed.getKeyState("_MMOUSE"))
        {
            currentSceneObject.addRotation(static_cast<float>(dx) * 0.01f);
        }
        else
        if(testBed.getKeyState("_LSHIFT") && testBed.getKeyState("_LMOUSE"))
        {
            currentSceneObject.addTranslation(dx * 40, dy * 40, 0, 100000);
        }
        else
        if(testBed.getKeyState("_LSHIFT") && testBed.getKeyState("_MMOUSE"))
        {
            currentSceneObject.addTranslation(0, 0, dy * 40, 100000);
        }
        else
        if(testBed.getKeyState("_RSHIFT") && testBed.getKeyState("_LMOUSE"))
        {
            currentSceneObject.addScale(static_cast<float>(dy) * 0.01f, 0.1f, 10.f);
        }
    }
}

void
cScene::handleKeyPress(const char* keyPressed)
{
    switch(keyPressed[1])
    {
    case '_':
        {
            if(strcmp("PAGEUP", keyPressed + 2) == 0)
            {
                ++_currentlySelected;
                if(_currentlySelected == SizeL(_meshObjects) + SizeL(_solidObjects))
                {
                    _currentlySelected = 0;
                }
            }
            else if(strcmp("PAGEDOWN", keyPressed + 2) == 0)
            {
                --_currentlySelected;
                if(_currentlySelected < 0)
                {
                    _currentlySelected = SizeL(_meshObjects) + SizeL(_solidObjects) - 1;
                }
            }
            else if(strcmp("HOME", keyPressed + 2) == 0)
            {
                zoomExtentsSelected();
            }
            else if(strcmp("END", keyPressed + 2) == 0)
            {
                zoomExtentsAll();
            }
            break;
        }
    }
}

void
cScene::render(bool highlightSelected)
{
    iTestBed& testBed = nSingletons::testBedI();
    bool isMeshObject;
    tSigned32 vectorIndex;
    decomposeCurrentlySelected(isMeshObject, vectorIndex);
    tSigned32 i;
    SetZBias(0.0f);
    testBed.setColourRGB(0.0f,0.0f,0.85f);
    for(i = 0; i != SizeL(_meshObjects); ++i)
    {
        if(!highlightSelected || !isMeshObject || i != vectorIndex)
        {
            _meshObjects[i]->renderFaces();
        }
    }
    for(i = 0; i != SizeL(_solidObjects); ++i)
    {
        if(!highlightSelected || isMeshObject || i != vectorIndex)
        {
            _solidObjects[i]->renderFaces();
        }
    }
    if(highlightSelected)
    {
        testBed.setColourRGB(0.25f,0.25f,1.f);
        if(isMeshObject)
        {
            _meshObjects[vectorIndex]->renderFaces();
        }
        else
        {
            _solidObjects[vectorIndex]->renderFaces();
        }
    }

    SetZBias(-0.5f);
    testBed.setColour("blue");
    for(i = 0; i != SizeL(_meshObjects); ++i)
    {
        if(!highlightSelected || !isMeshObject || i != vectorIndex)
        {
            _meshObjects[i]->renderEdges();
        }
    }
    for(i = 0; i != SizeL(_solidObjects); ++i)
    {
        if(!highlightSelected || isMeshObject || i != vectorIndex)
        {
            _solidObjects[i]->renderEdges();
        }
    }
    if(highlightSelected)
    {
        testBed.setColour("white");
        if(isMeshObject)
        {
            _meshObjects[vectorIndex]->renderEdges();
        }
        else
        {
            _solidObjects[vectorIndex]->renderEdges();
        }
    }
}

void
cScene::save(iXMLOutputStream& os) const
{
    tSigned32 i;
    os.openElement("placement3D");
    os.openElement("meshObjects");
    for(i = 0; i != SizeL(_meshObjects); ++i)
    {
        os.openElement("meshObject");
        _meshObjects[i]->addAttributes(os);
        os.closeElement("meshObject");
    }
    os.closeElement("meshObjects");
    os.openElement("solidObjects");
    for(i = 0; i != SizeL(_solidObjects); ++i)
    {
        os.openElement("solidObject");
        _solidObjects[i]->addAttributes(os);
        os.closeElement("solidObject");
    }
    os.closeElement("solidObjects");
    os.closeElement("placement3D");
}

void
cScene::zoomExtentsAll()
{
    std::vector<tPoint3> points;
    tSigned32 i;
    for(i = 0; i != SizeL(_meshObjects); ++i)
    {
        _meshObjects[i]->addAllPoints(points);
    }
    for(i = 0; i != SizeL(_solidObjects); ++i)
    {
        _solidObjects[i]->addAllPoints(points);
    }
    if(!points.empty())
    {
        ZoomExtents(points);
    }
}

iMesh*
cScene::buildGround(tSigned32 extrudeHeight, tSigned32 maxStepHeight, tSigned32 maxSlope)
{
    std::vector<const iFaceVertexMesh*> meshInstances;
    cSolidObjects solidObjects;
    tSigned32 i;
    for(i = 0; i != SizeL(_meshObjects); ++i)
    {
        meshInstances.push_back(_meshObjects[i]->getCachedInstance());
    }
    for(i = 0; i != SizeL(_solidObjects); ++i)
    {
        _solidObjects[i]->addTransformedOperands(solidObjects);
    }
    cGIPProgressCallBack progressCallBack;
    return BuildMeshFrom3DContent(
        VectorBuffer(meshInstances), SizeL(meshInstances),
        &solidObjects,
        extrudeHeight,
        maxStepHeight,
        maxSlope,
        0, // no options
        &progressCallBack
        );
}

// interface for iXMLHandler

iXMLHandler*
cScene::element(const char* el, const char *const* attr)
{
    if(strcmp(el, "meshObject") == 0)
    {
        _meshObjects.push_back(new cMeshObject(attr));
        return 0;
    }
    if(strcmp(el, "solidObject") == 0)
    {
        _solidObjects.push_back(new cSolidObject(attr));
        return 0;
    }
    return this;
}

