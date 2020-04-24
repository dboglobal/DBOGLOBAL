//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef SINGLETONS_TESTBED_INCLUDED
#define SINGLETONS_TESTBED_INCLUDED

#include "libs/PathEngine_Interface/interface/Singletons.h"

class osinterface;
class cTestBed;
class iTestBed;

namespace nSingletons
{

void init_TestBed(osinterface* os);
void shutDown_TestBed();
bool isInitialised_TestBed();

cTestBed& testBed();
iTestBed& testBedI();

}

#endif
