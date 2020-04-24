//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef CONTENT_PROCESSING_3D_DEMO__SCENE_INCLUDED
#define CONTENT_PROCESSING_3D_DEMO__SCENE_INCLUDED

#include "libs/ParseXML/interface/iXMLHandler.h"
#include "common/STL/vector.h"

class bSceneObject;
class cMeshObject;
class cSolidObject;
class iXMLOutputStream;
class iMesh;

class cScene : public iXMLHandler
{
    std::vector<cMeshObject*> _meshObjects;
    std::vector<cSolidObject*> _solidObjects;
    tSigned32 _currentlySelected;

    void decomposeCurrentlySelected(bool& isMeshObject, tSigned32& vectorIndex) const;
    bSceneObject& currentlySelectedSceneObject();
    void zoomExtentsSelected();

public:

    cScene();
    ~cScene();

    bool empty() const;
    void initSelection();

    void updateWithEditing();
    void handleKeyPress(const char* keyPressed);
    void render(bool highlightSelected);

    void save(iXMLOutputStream& os) const;

    void zoomExtentsAll();

    iMesh* buildGround(tSigned32 extrudeHeight, tSigned32 maxStepHeight, tSigned32 maxSlope);

// interface for iXMLHandler

    iXMLHandler* element(const char* el, const char *const* attr);
};

#endif
