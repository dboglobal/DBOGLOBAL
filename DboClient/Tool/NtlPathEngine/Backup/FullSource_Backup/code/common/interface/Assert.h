//**********************************************************************
//
// Copyright (c) 2002-2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef ASSERT_INCLUDED
#define ASSERT_INCLUDED

bool HandleAssertion(const char*, tSigned32, const char*);
bool& FullAssertsOn();
bool ReportInterfaceCheckFailure(const char*, tSigned32, const char*);

#ifdef PATHENGINE_FULL_RELEASE
#define INTERFACE_ASSERT(expr) do{}while(0)
#define INTERFACE_CHECK(expr,comment) do{}while(0)
#define assertR(expr) do{}while(0)
#else
#define INTERFACE_ASSERT(expr) do{static bool on = true; if(on && !(expr)) on = ReportInterfaceCheckFailure(__FILE__,__LINE__,#expr);}while(0)
#define INTERFACE_CHECK(expr,comment) do{static bool on = true; if(on && !(expr)) on = ReportInterfaceCheckFailure(__FILE__,__LINE__,comment);}while(0)
#define assertR(expr) do{static bool on = true;if(on && !(expr)) on = HandleAssertion(__FILE__,__LINE__,#expr);}while(0)
#endif

#ifdef ASSERTIONS_ON
#define assertF(expr) do{if(FullAssertsOn() && !(expr)) HandleAssertion(__FILE__,__LINE__,#expr);}while(0)
#define assertD(expr) assertR(expr)
#define invalid() do{static bool on=true; if(on) on = HandleAssertion(__FILE__,__LINE__,"<invalid>");}while(0)
#else
#define assertF(expr) do{}while(0)
#define assertD(expr) do{}while(0)
#define invalid() do{}while(0)
#endif

#define assertC(expr) do{typedef char tCompileTimeAssert[(expr)?1:-1];assertD(expr);}while(0)

#endif
