//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "common/interface/Assert.h"
#include "interface/Singletons_TestBed.h"
#include "cTestBed.h"
#include "platform_common/PointMultiplication.h"

namespace nSingletons
{

static cTestBed* _testBed = 0;

void
init_TestBed(osinterface* os)
{
    init_StandAlone();
    assertD(_testBed == 0);
    _testBed = new cTestBed(os);
}
void
shutDown_TestBed()
{
    assertD(_testBed != 0);
    delete _testBed;
    _testBed = 0;
    shutDown_StandAlone();
}
bool isInitialised_TestBed()
{
    return _testBed != 0;
}

cTestBed& testBed()
{
    assertD(_testBed);
    return *_testBed;
}
iTestBed& testBedI()
{
    assertD(_testBed);
    return static_cast<iTestBed&>(*_testBed);
}

}
