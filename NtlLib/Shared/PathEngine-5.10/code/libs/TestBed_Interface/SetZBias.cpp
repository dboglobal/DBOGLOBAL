//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "cTestBed.h"
#include "interface/Singletons_TestBed.h"
#include "libs/TestBed_Application/interface/iGraphicsInterface.h"

void SetZBias(float value)
{
    cTestBed& testBed = nSingletons::testBed();
    iGraphicsInterface* gip = testBed.getGIP();
    gip->setZBias(value);
}
