//**********************************************************************
//
// Copyright (c) 2004-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "ConnectionsLoader.h"
#include "libs/PathEngine_Core/interface/OffMeshConnections.h"
#include "libs/Mesh2D/interface/Mesh2DPersistence.h"
#include "common/interface/Attributes.h"

iXMLHandler*
cConnectionsLoader::element(const char* el, const char *const* attributes)
{
    if(getDepth() == 0)
    {
        if(strcmp(el, "endPoints") == 0)
        {
            _loadingEndPoints = true;
            return this;
        }
        if(strcmp(el, "connections") == 0)
        {
            _loadingEndPoints = false;
            return this;
        }
        return 0;
    }
    if(_loadingEndPoints && strcmp(el, "endPoint") == 0)
    {
        const char* value = GetValueForAttribute(attributes, "position");
        cInternalPosition position = PositionFromString(_mesh, value);
        _loadInto.addEndPoint(position);
        return 0;
    }
    if(!_loadingEndPoints && strcmp(el, "connection") == 0)
    {
        tSigned32 from, to, penalty;
        from = ValueAsLong(attributes, "from");
        to = ValueAsLong(attributes, "to");
        penalty = ValueAsLong(attributes, "penalty");
        _loadInto.addConnection(from, to, penalty);
        return 0;
    }
    return 0;
}
