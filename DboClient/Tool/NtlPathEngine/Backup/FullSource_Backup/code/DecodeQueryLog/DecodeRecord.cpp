//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "DecodeRecord.h"
#include "Decoder.h"
#include "MethodSpecificDecoding.h"
#include "common/interface/Error.h"

bool
DecodeRecord(const char*& recordStart, tUnsigned32& bufferRemaining, iXMLOutputStream& os)
{
    tSigned32 interfaceIndex, methodIndex;
    bool methodEntry;
    tUnsigned32 recordSize;
    cDecoder decoder(recordStart, bufferRemaining, os, interfaceIndex, methodIndex, methodEntry, recordSize);
    if(decoder.overran())
    {
        decoder.writeRecordEnd();
        Error("NonFatal", "Buffer ran out when decoding entry header.");
        return false;
    }
    MethodSpecificDecoding(decoder, interfaceIndex, methodIndex, methodEntry);
    decoder.writeRecordEnd();
    if(decoder.overran())
    {
        Error("NonFatal", "Buffer ran out during method specific decoding.");
        return false;
    }
    if(!decoder.empty())
    {
        Error("NonFatal", "Amount decoded is less than record size.");
        return false;
    }
    recordStart += recordSize;
    bufferRemaining -= recordSize;
    return true;
}
