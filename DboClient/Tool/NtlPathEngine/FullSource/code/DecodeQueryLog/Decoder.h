//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef DECODE_QUERY_LOG__DECODER_INCLUDED
#define DECODE_QUERY_LOG__DECODER_INCLUDED

#include "common/STL/string.h"

class iXMLOutputStream;

class cDecoder
{
    const char* _bufferPosition;
    tUnsigned32 _remainingSize;
    bool _overran;
    bool _methodEntry;
    iXMLOutputStream& _os;

    tSigned32 decodeSigned(tSigned32 tokenSize);
    tUnsigned32 decodeUnsigned(tSigned32 tokenSize);
    bool decodeBool();
    float decodeFloat();
    std::string decodeString();

    void decodeTimeStamp();

    void decodeMethodInfo(tSigned32& interfaceIndex, tSigned32& methodIndex, bool& methodEntry);

public:

    cDecoder(
        const char* buffer, tUnsigned32 bufferLength, iXMLOutputStream& os,
        tSigned32& interfaceIndex, tSigned32& methodIndex, bool& methodEntry,
        tUnsigned32& recordSize
        );

    bool overran() const;
    bool empty() const;

    void writeInterfaceName(const char* name);
    void writeMethodName(const char* name);

    void decode_bool(const char* name);
    void decode_tSigned32(const char* name);
    void decode_tUnsigned32(const char* name);
    void decode_float(const char* name);
    void decode_const_char_star(const char* name);
    void decode_const_char__starconst_star(const char* name);
    void decode_const_tSigned32_star(const char* name);
    void decode_const_float_star(const char* name);
    void decode_void_star(const char* name);
    void decode_cPosition(const char* name);
    void decode_cHorizontalRange(const char* name);
    void decodeAPIObject(const char* name);
    void decodeCallBack(const char* name);
    void decodeClassCallBack(const char* name);
    void decodeCollidingLine();
    void decodeCollisionShape();
    void decodeDataBuffer();
    void decodePathBuffers();
    void decodeSourceContent();
    void decodeAgentVector();
    void decodeContentChunkInstanceVector();
    void decodeLongVector();

    void writeRecordEnd();
};

#endif
