//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "i_pathengine.h"
#include "common/STL/vector.h"

// the data is put to the clipboard when the object is deleted

class cClipboardOutputStream : public iOutputStream
{
    std::vector<char> buffer;
public:

    ~cClipboardOutputStream();

    void put(const char *data, tUnsigned32 data_size);
};
