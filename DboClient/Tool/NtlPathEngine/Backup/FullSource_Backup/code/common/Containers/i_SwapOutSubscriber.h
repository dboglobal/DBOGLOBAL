//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef I_SWAP_OUT_SUBSCRIBER
#define I_SWAP_OUT_SUBSCRIBER

class iSwapOutSubscriber
{
public:
    virtual ~iSwapOutSubscriber() {}
    virtual void swapOut(tSigned32 index) = 0;
    virtual void pushBack() = 0;
};

#endif
