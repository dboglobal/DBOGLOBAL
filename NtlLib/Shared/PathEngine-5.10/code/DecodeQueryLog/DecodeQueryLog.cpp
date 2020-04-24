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
#include "libs/ParseXML/interface/StandardXMLOutputStream.h"
#include "platform_common/FileOutputStream.h"
#include "platform_common/LoadSaveBinary.h"
#include "platform_common/DirectErrorHandling.h"
#include "common/interface/Error.h"
#include "common/STL/sstream.h"

int
main(int argc, char* argv[])
{
    std::vector<char> buffer;
    LoadBinary("queryLog.bin", buffer);
    if(buffer.empty())
    {
        Error("NonFatal", "Binary query log ('queryLog.bin') not found in current directory, or empty. Aborting.");
        return 1;
    }
    cFileOutputStream os("queryLog.xml");
    cStandardXMLOutputStream xos(os);
    xos.openElement("queryLog");
    const char* recordStart = &buffer.front();
    tUnsigned32 bufferRemaining = static_cast<tUnsigned32>(buffer.size());
    while(bufferRemaining)
    {
        bool decodedOK = DecodeRecord(recordStart, bufferRemaining, xos);
        if(!decodedOK)
        {
            xos.openElement("decodingFailedForPreviousEntry");
        }
    }
    xos.closeElement("queryLog");
	return 0;
}
