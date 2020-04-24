//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"

#include "Max.h"

#include "MeshesForSelectionSet.h"
#include "MaxMeshWrapper.h"

// Return a pointer to a TriObject given an Object
// or return 0 if the Object cannot be converted to a TriObject
TriObject* GetTriObjectFromObject(Object* object, bool& mustBeDeleted)
{
    mustBeDeleted = false;
    if(object->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
    { 
        TriObject* tri = (TriObject*) object->ConvertToType(0, Class_ID(TRIOBJ_CLASS_ID, 0));
        if(object != tri)
        {
            mustBeDeleted = true;
        }
        return tri;
    }
    return 0;
}

void
cMeshesForSelectionSet::add_Recursive(Interface* ip,
                                      const char* prefix, tSigned32 prefixLength,
                                      bool useMaterialIDs, bool setTerrainLayer, bool processConnections, float scale, bool negateY,
                                      INode* node)
{
    const char* name = node->GetName();
    if(strncmp(name, prefix, prefixLength) == 0)
    {
        TimeValue time = ip->GetTime();
        Object* object = node->EvalWorldState(time).obj;
        bool mustBeDeleted;
        TriObject* triObject = GetTriObjectFromObject(object, mustBeDeleted);
        if(!triObject)
        {
            char buffer[2000];
            sprintf(buffer,"Object '%s' cannot be converted to a tri object, skipped.", node->GetName());
            //... TODO sort out modality of message boxes
            MessageBox(0, buffer, "Ground Export - Error", MB_OK);
        }
        else
        {
            _triObjects.push_back(triObject);
            _mustBeDeleted.push_back(mustBeDeleted);
            _names.push_back(name);
            const Mesh &mesh = _triObjects.back()->GetMesh();
            _wrappers.push_back(new cMaxMeshWrapper(mesh, node->GetObjectTM(time), useMaterialIDs, setTerrainLayer, processConnections, scale, negateY));
        }
    }
    tSigned32 i;
    for(i = 0; i < node->NumberOfChildren(); ++i)
    {
        add_Recursive(ip,
                    prefix, prefixLength,
                    useMaterialIDs, setTerrainLayer, processConnections, scale, negateY,
                    node->GetChildNode(i));
    }
}

cMeshesForSelectionSet::cMeshesForSelectionSet(Interface* ip, const char* prefix, bool useMaterialIDs, bool setTerrainLayer, bool processConnections, float scale, bool negateY)
{
    if(prefix)
    {
        tSigned32 prefixLength = static_cast<tSigned32>(strlen(prefix));
        INode* root = ip->GetRootNode();
        add_Recursive(ip,
                    prefix, prefixLength,
                    useMaterialIDs, setTerrainLayer, processConnections, scale, negateY,
                    root);
    }
    else
    {
        bool reportLooksLikeShape = true;
        char buffer[2000];

        int numberSelected = ip->GetSelNodeCount();
        TimeValue time = ip->GetTime();

        tSigned32 i;
        for(i = 0; i < numberSelected; i++)
        {
            INode* node = ip->GetSelNode(i);                        
            Object* object = node->EvalWorldState(time).obj;
            bool mustBeDeleted;
            TriObject* triObject = GetTriObjectFromObject(object, mustBeDeleted);

            if(!triObject)
            {
                sprintf(buffer,"Object '%s' cannot be converted to a tri object, skipped.", node->GetName());

                //... TODO sort out modality of message boxes
                MessageBox(0, buffer, "Ground Export - Error", MB_OK);
                continue;
            }

            const char* name = node->GetName();
            if(strncmp(name, "shape_", 6) == 0
             || strncmp(name, "baseObstacle_", 13) == 0
             || strncmp(name, "obstacle_", 9) == 0
             )
            {
                if(reportLooksLikeShape)
                {
                    sprintf(buffer, "Object '%s' has the prefix for an anchored shape,"
                        " but is included in the selection set,"
                        " and will be exported as ground."
                        " (Reported once per exported.)"
                        , name);
                    //... TODO sort out modality of message boxes
                    MessageBox(0, buffer, "Ground Export - Warning", MB_OK);
                    reportLooksLikeShape = false;
                }
            }

            _triObjects.push_back(triObject);
            _mustBeDeleted.push_back(mustBeDeleted);
            _names.push_back(name);
            const Mesh &mesh = _triObjects.back()->GetMesh();
            _wrappers.push_back(new cMaxMeshWrapper(mesh, node->GetObjectTM(time), useMaterialIDs, setTerrainLayer, processConnections, scale, negateY));
        }
    }
}

cMeshesForSelectionSet::~cMeshesForSelectionSet()
{
    tSigned32 i;
    for(i = 0; i < SizeL(_triObjects); i++)
    {
        delete _wrappers[i];
        if(_mustBeDeleted[i])
        {
            _triObjects[i]->DeleteMe();
        }
    }
}

bool
cMeshesForSelectionSet::wasConverted(tSigned32 index) const
{
    return _mustBeDeleted[index];
}

