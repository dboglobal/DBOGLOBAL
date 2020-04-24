//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef SINGLETONS_INCLUDED
#define SINGLETONS_INCLUDED

class cPathEngine;
class iPathEngine;

namespace nSingletons
{

void init_StandAlone();
void shutDown_StandAlone();
bool isInitialised_StandAlone();

cPathEngine& pathEngine();
iPathEngine& pathEngineI();

}

#endif
