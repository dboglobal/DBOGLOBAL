//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef ATTRIBUTES_BUILDER_INCLUDED
#define ATTRIBUTES_BUILDER_INCLUDED

#include "common/interface/Assert.h"
#include "common/STL/string.h"
#include "common/STL/vector.h"
#include "common/STL/list.h"
#include "common/STL/sstream.h"
#include "common/StringFormatting.h"

class cAttributesBuilder
{
    mutable std::vector<const char*> _attributes;
    std::vector<char> _buffer;
    tSigned32 _bufferPos;
    std::list<std::string> _strings;

public:

    cAttributesBuilder() :
      _buffer(100)
    {
        _bufferPos = 0;
    }

    bool empty() const
    {
        return _attributes.empty();
    }

    // the strings are not copied
    void addPointers(const std::vector<std::string>& v)
    {
        tSigned32 i = 0;
        while(i < SizeL(v))
        {
            assertD(i + 1 < SizeL(v));
            _attributes.push_back(v[i].c_str());
            _attributes.push_back(v[i + 1].c_str());
            i += 2;
        }
    }

    void clear()
    {
        _attributes.clear();
        _strings.clear();
        _bufferPos = 0;
    }
    void add(const char* name, const char* value)
    {
        _attributes.push_back(name);
        _attributes.push_back(value);
    }
    void addCopies(const char* name, const char* value)
    {
        _strings.push_back(name);
        _attributes.push_back(_strings.back().c_str());
        _strings.push_back(value);
        _attributes.push_back(_strings.back().c_str());
    }
    void add(const char *const* attributes)
    {
        while(attributes && *attributes)
        {
            add(attributes[0], attributes[1]);
            attributes += 2;
        }
    }
    void addCopies(const char *const* attributes)
    {
        while(attributes && *attributes)
        {
            addCopies(attributes[0], attributes[1]);
            attributes += 2;
        }
    }
    void add_CopyString(const char* name, const std::string& value)
    {
        _attributes.push_back(name);
        _strings.push_back(value);
        _attributes.push_back(_strings.back().c_str());
    }
    template <class T>
    void add(const char* name, const T& value)
    {
        _attributes.push_back(name);
        std::ostringstream oss;
        oss << value;
        _strings.push_back(oss.str());
        _attributes.push_back(_strings.back().c_str());
    }

    void add(const char *name, tSigned32 value)
    {
        _attributes.push_back(name);
        if(_bufferPos + nStringFormatting::bufferSize_Long() >= SizeL(_buffer))
        {
            std::ostringstream oss;
            oss << value;
            _strings.push_back(oss.str());
            _attributes.push_back(_strings.back().c_str());
        }
        else
        {
            char* formatted = nStringFormatting::format_Long(value, &_buffer[_bufferPos]);
            _bufferPos += nStringFormatting::bufferSize_Long();
            _attributes.push_back(formatted);
        }
    }
    
    void finalise()
    {
        _attributes.push_back(0);
    }
    const char *const* get() const
    {
        if(_attributes.size() == 0)
        {
            return 0;
        }
        if(_attributes.back() != 0)
        {
            _attributes.push_back(0);
        }
        return &_attributes.front();
    }
};

#endif
