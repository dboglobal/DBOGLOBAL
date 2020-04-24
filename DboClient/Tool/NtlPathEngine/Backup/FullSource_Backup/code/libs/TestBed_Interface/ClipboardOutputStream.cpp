//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "ClipboardOutputStream.h"
#include "common/interface/Error.h"
#include <windows.h>

cClipboardOutputStream::~cClipboardOutputStream()
{
    buffer.push_back(0);

    BOOL b = OpenClipboard(NULL);
    if (!b)
    {
        Error("NonFatal", "Failed to send to clipboard: Could not open clipboard.");
        return;
    }
    EmptyClipboard();

    HGLOBAL clipMem = GlobalAlloc(GMEM_MOVEABLE, buffer.size());
    if(0 == clipMem)
    {
        Error("NonFatal", "Failed to send to clipboard: Could not get memory.");
        return;
    }
    char* pClipMem = reinterpret_cast<char*>(GlobalLock(clipMem));

    strcpy(pClipMem, &buffer.front());
    GlobalUnlock(clipMem);

    SetClipboardData (CF_TEXT, clipMem);
    CloseClipboard ();
}

void
cClipboardOutputStream::put(const char *data, tUnsigned32 data_size)
{
    size_t before = buffer.size();
    buffer.resize(before + data_size);
    memcpy(&buffer[before], data, data_size);
}
