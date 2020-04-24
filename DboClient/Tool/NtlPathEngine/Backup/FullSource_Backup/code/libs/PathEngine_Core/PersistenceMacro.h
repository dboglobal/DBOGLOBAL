//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PERSISTENCEMACRO_INCLUDED
#define PERSISTENCEMACRO_INCLUDED

class iInSerialiser;
class iOutSerialiser;
#define PERSISTENCE(classname) \
    friend void Load(classname&,iInSerialiser*); \
    friend void Save(const classname&,iOutSerialiser*);

#endif
