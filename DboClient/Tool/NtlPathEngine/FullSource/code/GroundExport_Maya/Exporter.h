//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef EXPORTER_INCLUDED
#define EXPORTER_INCLUDED

#include <maya/MPxFileTranslator.h>

class cExporter : public MPxFileTranslator
{
    cExporter()
    {
    }

public:

	static void* creator();
    bool haveWriteMethod() const;
    bool haveReadMethod() const;
    MStatus writer(const MFileObject& file, const MString& optionsString, MPxFileTranslator::FileAccessMode mode);
    MStatus reader(const MFileObject& file, const MString& optionsString, MPxFileTranslator::FileAccessMode mode);
};

#endif
