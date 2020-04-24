//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "common/Containers/BitVector.h"
#include <vector>
#include <string>

class iFaceVertexMesh;

class cMeshesForSelectionSet
{
    std::vector<TriObject*> _triObjects;
    cBitVector _mustBeDeleted;
    std::vector<const char*> _names;
    std::vector<iFaceVertexMesh*> _wrappers;

    void
    add_Recursive(Interface* ip,
                  const char* prefix, tSigned32 prefixLength,
                  bool useMaterialIDs, bool setTerrainLayer, bool processConnections, float scale, bool negateY,
                  INode* node);

public:

    // if prefix is non-zero then searches the whole scene for objects
    // otherwise uses current selection set
    cMeshesForSelectionSet(Interface* ip, const char* prefix, bool useMaterialIDs, bool setTerrainLayer, bool processConnections, float scale, bool negateY);
    ~cMeshesForSelectionSet();

    bool wasConverted(tSigned32 index) const;

    const std::vector<iFaceVertexMesh*>& refMeshes() const
    {
        return _wrappers;
    }
    const char* name(tSigned32 index) const
    {
        return _names[index];
    }
};

