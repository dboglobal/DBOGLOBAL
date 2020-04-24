//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef ATTRIBUTES_INCLUDED
#define ATTRIBUTES_INCLUDED

#include "common/interface/Assert.h"
#include "common/interface/Error.h"
#include "common/CLib/string.h"
#include "common/CLib/stdlib.h"
#include "platform_common/FloatPersistence.h"

inline const char*
GetValueForAttribute(const char *const* attributes, const char* attribute)
{
    if(attributes && *attributes)
    {
        do
        {
            if(strcmp(attributes[0], attribute) == 0)
            {
                return attributes[1];
            }
            attributes += 2;
        }
        while(*attributes);
    }
    return 0;
}
inline const char*
GetValueForAttributeWithDefault(const char *const* attributes, const char* attribute, const char* defaultValue)
{
    const char* result = GetValueForAttribute(attributes, attribute);
    if(result == 0)
    {
        result = defaultValue;
    }
    return result;
}

inline tSigned32
ValueAsLong(const char* value)
{
    assertD(value);
    if(!*value)
    {
        Error("NonFatal", "Reading tSigned32 from attribute: attribute is empty. Returning 0.");
        return 0;
    }
    char *ptr;
    tSigned32 result = strtol(value, &ptr, 10);
    if(*ptr != 0)
    {
        Error("NonFatal", "Garbage encountered reading tSigned32 from attribute.");
    }
    return result;
}

inline tSigned32
ValueAsLong(const char *const* attributes, const char* attribute)
{
    const char* value = GetValueForAttribute(attributes, attribute);
    return ValueAsLong(value);
}

inline tSigned32
ValueAsLongWithDefault(const char *const* attributes, const char* attribute, tSigned32 defaultValue)
{
    const char* value = GetValueForAttribute(attributes, attribute);
    if(value)
    {
        return ValueAsLong(value);
    }
    return defaultValue;
}

inline tUnsigned32
ValueAsUnsignedLong(const char* value)
{
    assertD(value);
    if(!*value)
    {
        Error("NonFatal", "Reading tUnsigned32 from attribute: attribute is empty. Returning 0.");
        return 0;
    }
    char *ptr;
    tUnsigned32 result = strtoul(value, &ptr, 10);
    if(*ptr != 0)
    {
        Error("NonFatal", "Garbage encountered reading tUnsigned32 from attribute.");
    }
    return result;
}

inline tUnsigned32
ValueAsUnsignedLong(const char *const* attributes, const char* attribute)
{
    const char* value = GetValueForAttribute(attributes, attribute);
    return ValueAsUnsignedLong(value);
}

inline float
ValueAsFloat(const char* value)
{
    assertD(value);
    if(!*value)
    {
        Error("NonFatal", "Reading float from attribute: attribute is empty. Returning 0.");
        return 0;
    }
    char *ptr;
    float result = static_cast<float>(strtod(value, &ptr));
    if(*ptr != 0)
    {
        Error("NonFatal", "Garbage encountered reading float from attribute.");
    }
    return result;
}

inline float
ValueAsFloat(const char *const* attributes, const char* attribute)
{
    const char* value = GetValueForAttribute(attributes, attribute);
    return ValueAsFloat(value);
}

inline float
ValueAsPersistentFloat(const char* value)
{
    assertD(value);
    if(!*value)
    {
        Error("NonFatal", "Reading persistent float from attribute: attribute is empty. Returning 0.");
        return 0;
    }
    return FloatFromPersistent(value);
}

inline float
ValueAsPersistentFloat(const char *const* attributes, const char* attribute)
{
    const char* value = GetValueForAttribute(attributes, attribute);
    return ValueAsPersistentFloat(value);
}

inline float
ValueAsPersistentFloatWithDefault(const char *const* attributes, const char* attribute, float defaultValue)
{
    const char* value = GetValueForAttribute(attributes, attribute);
    if(value == 0)
    {
        return defaultValue;
    }
    return ValueAsPersistentFloat(value);
}

inline bool
ValueAsBool(const char* value)
{
    assertD(value);
    if(strcmp(value, "true") == 0)
    {
        return true;
    }
    if(strcmp(value, "false") != 0)
    {
        Error("NonFatal", "Unexpected value encounter reading bool from attribute. "
            "Expected 'true' or 'false'. Returning false.");
    }
    return false;
}

inline bool
ValueAsBool(const char *const* attributes, const char* attribute)
{
    const char* value = GetValueForAttribute(attributes, attribute);
    return ValueAsBool(value);
}
inline bool
ValueAsBoolWithDefault(const char *const* attributes, const char* attribute, bool defaultValue)
{
    const char* value = GetValueForAttribute(attributes, attribute);
    if(value == 0)
    {
        return defaultValue;
    }
    return ValueAsBool(value);
}

#endif
