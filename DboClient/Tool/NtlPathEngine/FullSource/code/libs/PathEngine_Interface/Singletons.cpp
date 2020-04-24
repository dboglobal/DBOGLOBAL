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
#include "common/interface/Assert.h"
#include "interface/Singletons.h"
#include "interface/cPathEngine.h"

namespace nSingletons
{

static cPathEngine* _pathEngine = 0;

void
init_StandAlone()
{
    assertD(_pathEngine == 0);
    _pathEngine = new cPathEngine;
    _pathEngine->stage2Initialisation();
}
void
shutDown_StandAlone()
{
    assertD(_pathEngine != 0);
    delete _pathEngine;
    _pathEngine = 0;
}
bool isInitialised_StandAlone()
{
    return _pathEngine != 0;
}

cPathEngine& pathEngine()
{
    assertD(_pathEngine);
    return *_pathEngine;
}
iPathEngine& pathEngineI()
{
    assertD(_pathEngine);
    return *_pathEngine;
}

}
