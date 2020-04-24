//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "LoadOperands.h"
#include "OperandsLoader.h"
#include "libs/ParseXML/interface/ParseXML.h"
#include "platform_common/LoadSaveBinary.h"

void
LoadOperands(const char* type, std::vector<std::vector<tSigned32> >& result)
{
    assertR(result.empty());
    std::string fileName = "..\\resource\\solidObjects\\";
    fileName.append(type);
    fileName.append(".xml");
    std::vector<char> buffer;
    LoadBinary(fileName.c_str(), buffer);
    if(buffer.empty())
    {
        Error("Fatal", "Failed to load operands for a solid object.");
        return;
    }
    cOperandsLoader loader;
    bool parsedOK = ParseXML(&buffer[0], static_cast<tUnsigned32>(buffer.size()), loader);
    if(!parsedOK)
    {
        Error("Fatal", "Failed to parse solid object file.");
        return;
    }
    result.swap(loader._operands);
    if(result.empty())
    {
        Error("Fatal", "Loaded solid object has no operands.");
        return;
    }
}
