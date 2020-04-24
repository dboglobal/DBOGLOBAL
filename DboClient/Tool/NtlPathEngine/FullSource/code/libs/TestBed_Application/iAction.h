//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

class cActionContext
{
public:

    bool _blendingOn;
    bool _renderBlended;

    cActionContext()
    {
        _blendingOn = false;
        _renderBlended = false;
    }
};

class iAction
{
public:

    virtual ~iAction() {}
    virtual void perform(cActionContext& context) = 0;
    virtual bool isRenderGeometry()
    {
        return false;
    }
};
