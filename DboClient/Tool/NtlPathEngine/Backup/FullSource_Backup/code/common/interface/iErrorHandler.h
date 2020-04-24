//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef I_ERRORHANDLER_HAS_BEEN_INCLUDED
#define I_ERRORHANDLER_HAS_BEEN_INCLUDED
class iErrorHandler
{
public:
    virtual ~iErrorHandler() {};
    enum eAction
    {
        CONTINUE,
        ABORT,
        BREAK,
        CONTINUE_IGNORE_IN_FUTURE,
    };
    virtual eAction handle(const char *type, const char *description, const char *const* attributes)=0;
};
#endif
