//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef LOG_INCLUDED
#define LOG_INCLUDED

#ifdef LOGGING_ON

void Log(const char* string);
void Log(tUnsigned32 value);

#else

inline void
Log(const char* string)
{
}
inline void
Log(tUnsigned32 value)
{
}

#endif

#endif // include guard

