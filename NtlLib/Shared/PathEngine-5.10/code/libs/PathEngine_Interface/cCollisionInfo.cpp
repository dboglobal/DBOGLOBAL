//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/cCollisionInfo.h"
#include "Pool.h"
#include "interface/cMesh.h"
#include "interface/Singletons.h"
#include "interface/cPathEngine.h"
#include "iAPIObjectWrapper.h"

cCollisionInfo::cCollisionInfo()
{
    _wrapper = 0; // if logging is on then this will always be reset after init
    _poolIndex = -1;
    _nodeForThis.setInvalid();
    _mesh = 0;
}

cCollisionInfo::~cCollisionInfo()
{
    assertD(_poolIndex == -1); // assert not held by pool
    assertD(!_nodeForThis.valid());
}

void
cCollisionInfo::init(cMesh* mesh, const tLine& collidingLine, iAgent* collidingAgent, cList_NoSize<cCollisionInfo*>::iterator node)
{
    _mesh = mesh;
    _collidingLine = collidingLine;
    _collidingAgent = collidingAgent;
    _nodeForThis = node;
}

void
cCollisionInfo::init(cMesh* mesh, iAgent* collidingAgent, cList_NoSize<cCollisionInfo*>::iterator node)
{
    _mesh = mesh;
    _collidingLine = tLine(tPoint(0,0,0), tPoint(0,0,0));
    _collidingAgent = collidingAgent;
    _nodeForThis = node;
}

// iCollisionInfo

void
cCollisionInfo::destroy()
{
    if(_wrapper) {_wrapper->destroy_APIObjectWrapper();}
    _wrapper = 0;
    _mesh->detachCollisionInfo(_nodeForThis);
    _nodeForThis.setInvalid();
    nSingletons::pathEngine().refCollisionInfoPool().release(this);
}

void
cCollisionInfo::getCollidingLine(tSigned32* coords) const
{
    coords[0]=_collidingLine.start().getX();
    coords[1]=_collidingLine.start().getY();
    coords[2]=_collidingLine.end().getX();
    coords[3]=_collidingLine.end().getY();
}
iAgent*
cCollisionInfo::getCollidingAgent() const
{
    return _collidingAgent;
}
