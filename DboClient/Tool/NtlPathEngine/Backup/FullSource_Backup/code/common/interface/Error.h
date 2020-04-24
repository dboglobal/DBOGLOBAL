//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef ERROR_INCLUDED
#define ERROR_INCLUDED

void Error(const char* error_type, const char* error_string, const char *const* attributes=0);
void PathEngine_DebugBreak();
void PathEngine_Abort();

class iErrorHandler;
void SetGlobalErrorHandler(iErrorHandler*);
iErrorHandler* GetGlobalErrorHandler();

#endif
