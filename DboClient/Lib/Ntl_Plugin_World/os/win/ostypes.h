/***************************************************************************
 *                                                                         *
 * Module  : ostypes.h                                                     *
 *                                                                         *
 * Purpose : Types                                                         *
 *                                                                         *
 **************************************************************************/

/* RWPUBLIC */
#ifndef WIN_OSTYPES_H
#define WIN_OSTYPES_H
/* RWPUBLICEND */

/****************************************************************************
 Includes
 */

/****************************************************************************
 Library version number
 */

/* RWPUBLIC */

#define rwLITTLEENDIAN         /* This is a little endian machine */

typedef long RwFixed;
typedef int  RwInt32;
typedef unsigned int RwUInt32;
typedef short RwInt16;
typedef unsigned short RwUInt16;
typedef unsigned char RwUInt8;
typedef signed char RwInt8;

#ifdef RWUNICODE
typedef wchar_t RwChar;
#else /* RWUNICODE */
typedef char RwChar;
#endif /* RWUNICODE */
typedef float RwReal;
typedef RwInt32 RwBool;

#if defined(_MSC_VER)
typedef __int64 RwInt64;
typedef unsigned __int64 RwUInt64;
#define RWZERO64 ((RwUInt64)0)
#elif defined(__GNUC__)
typedef long long RwInt64;
typedef unsigned long long RwUInt64;
#define RWZERO64 ((RwUInt64)0) 
#else /* defined(_MSC_VER) || defined(__GNUC__) */

typedef struct _RwUInt64 RwUInt64;
typedef struct _RwInt64 RwInt64;

/* We'll do it with structures (can't do maths on these, but OK for allocation): */
#if (!defined(DOXYGEN))
#ifdef rwBIGENDIAN
struct _RwUInt64
{
    RwUInt32 top;
    RwUInt32 bottom;
};

struct _RwInt64
{
    RwInt32 top;
    RwUInt32 bottom;
};

#else /* rwBIGENDIAN */
#ifdef rwLITTLEENDIAN
struct _RwUInt64
{
    RwUInt32 bottom;
    RwUInt32 top;
};

struct _RwInt64
{
    RwUInt32 bottom;
    RwInt32 top;
};

#else /* rwLITTLEENDIAN */
#error "ENDIAN-ness undefined!"
#endif /* rwLITTLEENDIAN */
#endif /* rwBIGENDIAN */
#endif /* (!defined(DOXYGEN)) */

#define RWZERO64 { (RwUInt32)0, (RwUInt32)0 }
#endif /* defined(_MSC_VER) || defined(__GNUC__) */

typedef struct _RwUInt128 RwUInt128;
typedef struct _RwInt128 RwInt128;

/* We'll do it with structures
 * (can't do maths on these, but OK for allocation): */
#if (!defined(DOXYGEN))
#ifdef rwBIGENDIAN 
struct _RwUInt128
{
    RwUInt64 top;
    RwUInt64 bottom;
};

struct _RwInt128
{
    RwInt64 top;
    RwUInt64 bottom;
};

#else /* rwBIGENDIAN */
#ifdef rwLITTLEENDIAN
struct _RwUInt128
{
    RwUInt64 bottom;
    RwUInt64 top;
};

struct _RwInt128
{
    RwUInt64 bottom;
    RwInt64 top;
};

#else /* rwLITTLEENDIAN */
#error "ENDIAN-ness undefined!"
#endif /* rwLITTLEENDIAN */
#endif /* rwBIGENDIAN */
#endif /* (!defined(DOXYGEN)) */

#define RWZERO128 { RWZERO64, RWZERO64 }

/* Limits of types */
#define RwInt32MAXVAL       0x7FFFFFFF
#define RwInt32MINVAL       0x80000000
#define RwUInt32MAXVAL      0xFFFFFFFF
#define RwUInt32MINVAL      0x00000000
#define RwRealMAXVAL        (RwReal)(3.40282347e+38)
#define RwRealMINVAL        (RwReal)(1.17549435e-38)
#define RwInt16MAXVAL       0x7FFF
#define RwInt16MINVAL       0x8000
#define RwUInt16MAXVAL      0xFFFF
#define RwUInt16MINVAL      0x0000

/* Structure alignment */
#define RWALIGN(type, x)   type /* nothing */
#define rwMATRIXALIGNMENT sizeof(RwUInt32)
#define rwFRAMEALIGNMENT sizeof(RwUInt32)
#define rwV4DALIGNMENT sizeof(RwUInt32)

#if (!defined(rwMALLOCALIGNMENT))
#define rwMALLOCALIGNMENT sizeof(RwUInt32)
#endif /* (!defined(rwMALLOCALIGNMENT) */

#if (defined(_MSC_VER))

#if (defined(RWVERBOSE))
#include <tchar.h>
#pragma comment (lib , "advapi32.LIB") /* Registry functions */

/*
 * registry code
 */

#if (defined(RpWinRegGetDWordValue))
#undef RpWinRegGetDWordValue
#endif /* (defined(RpWinRegGetDWordValue)) */

#define RpWinRegGetDWordValue(_result, _hKey, _name, _val)              \
MACRO_START                                                             \
{                                                                       \
    DWORD               _size;                                          \
    DWORD               _type;                                          \
    LONG                _status;                                        \
                                                                        \
    _status =                                                           \
        RegQueryValueEx((_hKey), (_name), 0, &_type, NULL, &_size);     \
    (_result) = ((ERROR_SUCCESS == _status) && (REG_DWORD == _type));   \
                                                                        \
    if ((_result))                                                      \
    {                                                                   \
        _status =                                                       \
            RegQueryValueEx((_hKey), (_name), 0, &_type,                \
                            (BYTE *) (_val), &_size);                   \
        (_result) = (ERROR_SUCCESS == _status);                         \
    }                                                                   \
}                                                                       \
MACRO_STOP

#if (defined(RpWinRegGetBinaryValue))
#undef RpWinRegGetBinaryValue
#endif /* (defined(RpWinRegGetBinaryValue)) */

#define RpWinRegGetBinaryValue(_result, _hKey, _name, _val)             \
MACRO_START                                                             \
{                                                                       \
    DWORD               _size;                                          \
    DWORD               _type;                                          \
    LONG                _status;                                        \
                                                                        \
    _status =                                                           \
        RegQueryValueEx((_hKey), (_name), 0, &_type, NULL, &_size);     \
    (_result) =                                                         \
        ((ERROR_SUCCESS == _status) &&                                  \
         (REG_BINARY == _type) && (0 < _size));                         \
                                                                        \
    if ((_result))                                                      \
    {                                                                   \
        *(_val) = RwMalloc(sizeof(BYTE) * _size, rwMEMHINTDUR_EVENT);   \
        (_result) = (NULL != *(_val));                                  \
                                                                        \
        if ((_result))                                                  \
        {                                                               \
                                                                        \
            _status =                                                   \
                RegQueryValueEx((_hKey),                                \
                                (_name), 0, &_type,                     \
                                (BYTE *) * (_val), &_size);             \
            (_result =) (ERROR_SUCCESS == _status);                     \
                                                                        \
            if (!(_result))                                             \
            {                                                           \
                RwFree(*(_val));                                        \
                *(_val) = NULL;                                         \
            }                                                           \
                                                                        \
        }                                                               \
                                                                        \
    }                                                                   \
}                                                                       \
MACRO_STOP

#if (defined(RpWinRegGetStringValue))
#undef RpWinRegGetStringValue
#endif /* (defined(RpWinRegGetStringValue)) */

#define RpWinRegGetStringValue(_result, _hKey, _name, _val)             \
MACRO_START                                                             \
{                                                                       \
    DWORD               _size;                                          \
    DWORD               _type;                                          \
    LONG                _status;                                        \
                                                                        \
    _status =                                                           \
        RegQueryValueEx((_hKey), (_name), 0, &_type, NULL, &_size);     \
    (_result) =                                                         \
        ((ERROR_SUCCESS == _status) &&                                  \
         (REG_SZ == _type) && (0 < _size));                             \
                                                                        \
    if ((_result))                                                      \
    {                                                                   \
                                                                        \
        *(_val) = RwMalloc(sizeof(TCHAR) * _size, rwMEMHINTDUR_EVENT);  \
        (_result) = (NULL != *(_val));                                  \
                                                                        \
        if ((_result))                                                  \
        {                                                               \
            _status =                                                   \
                RegQueryValueEx((_hKey), (_name), 0, &_type,            \
                                (BYTE *) * (_val), &_size);             \
            (_result) = (ERROR_SUCCESS == _status);                     \
                                                                        \
            if (!(_result))                                             \
            {                                                           \
                RwFree(*(_val));                                        \
                *(_val) = NULL;                                         \
            }                                                           \
        }                                                               \
    }                                                                   \
}                                                                       \
MACRO_STOP

/* ------------------------------------------------------------------- */

#define RpWinRegCloseKey(hKey)                  \
MACRO_START                                     \
{                                               \
    RegCloseKey(hKey);                          \
}                                               \
MACRO_STOP

/* ------------------------------------------------------------------- */

#define RpWinRegOpenMachineKey(result)                          \
MACRO_START                                                     \
{                                                               \
    static const TCHAR  RenderWareKey[] =                       \
        "Software\\Criterion\\RenderWare";                      \
    DWORD               disposition;                            \
    LONG                status =                                \
        RegCreateKeyEx(HKEY_LOCAL_MACHINE, RenderWareKey, 0,    \
                       REG_NONE, REG_OPTION_NON_VOLATILE,       \
                       KEY_READ | KEY_WRITE,                    \
                       NULL, &result, &disposition);            \
                                                                \
    if (status != ERROR_SUCCESS)                                \
    {                                                           \
        result = NULL;                                          \
    }                                                           \
}                                                               \
MACRO_STOP

/* ------------------------------------------------------------------- */

#if (defined(RWGETWINREGDWORD))
#undef RWGETWINREGDWORD
#endif /* (defined(RWGETWINREGDWORD)) */

#define RWGETWINREGDWORD(result, match)                 \
MACRO_START                                             \
{                                                       \
    HKEY                hKey;                           \
                                                        \
    RpWinRegOpenMachineKey(hKey);                       \
    if (hKey)                                           \
    {                                                   \
        RwBool              success;                    \
                                                        \
        RpWinRegGetDWordValue(success, hKey, match,     \
                               &result);                \
                                                        \
        RpWinRegCloseKey(hKey);                         \
    }                                                   \
}                                                       \
MACRO_STOP

#if (defined(RWGETWINREGBINARY))
#undef RWGETWINREGBINARY
#endif /* (defined(RWGETWINREGBINARY)) */

#define RWGETWINREGBINARY(result, match)                \
MACRO_START                                             \
{                                                       \
    HKEY                hKey;                           \
                                                        \
    result = NULL;                                      \
    RpWinRegOpenMachineKey(hKey);                       \
    if (hKey)                                           \
    {                                                   \
        RwBool              success;                    \
                                                        \
        RpWinRegGetBinaryValue(success, hKey, match,    \
                                &result, NULL);         \
                                                        \
        if (!success)                                   \
            result = NULL;                              \
                                                        \
        RpWinRegCloseKey(hKey);                         \
    }                                                   \
}                                                       \
MACRO_STOP

#if (defined(RWGETWINREGSTRING))
#undef RWGETWINREGSTRING
#endif /* (defined(RWGETWINREGSTRING)) */

#define RWGETWINREGSTRING(result, match)                \
MACRO_START                                             \
{                                                       \
    HKEY                hKey;                           \
                                                        \
    result = NULL;                                      \
    RpWinRegOpenMachineKey(hKey);                       \
    if (hKey)                                           \
    {                                                   \
        RwBool              success;                    \
                                                        \
        RpWinRegGetStringValue(success, hKey, match,    \
                                &result);               \
                                                        \
        if (!success)                                   \
            result = NULL;                              \
                                                        \
        RpWinRegCloseKey(hKey);                         \
    }                                                   \
}                                                       \
MACRO_STOP

#if (defined(_DEBUG))

#if (defined(RWREGSETBREAKALLOC))
#undef RWREGSETBREAKALLOC
#endif /* (defined(RWREGSETBREAKALLOC)) */

#define RWREGSETBREAKALLOC(_name)                               \
MACRO_START                                                     \
{                                                               \
    char _message[256];                                         \
    long _lBreakAlloc = -1;                                     \
                                                                \
    RWGETWINREGDWORD(_lBreakAlloc, _name);                      \
                                                                \
    RWCRTSETBREAKALLOC(_lBreakAlloc);                           \
                                                                \
    _snprintf(_message, sizeof(_message),                       \
              "%s(%d): RWCRTSETBREAKALLOC(%ld)\n",              \
              __FILE__, __LINE__,                               \
              _lBreakAlloc);                                    \
    OutputDebugString(_message);                                \
                                                                \
}                                                               \
MACRO_STOP

#if (defined(RWREGSETDEBUGTRACE))
#undef RWREGSETDEBUGTRACE
#endif /* (defined(RWREGSETDEBUGTRACE)) */

#define RWREGSETDEBUGTRACE(_name)                     \
MACRO_START                                           \
{                                                     \
    char _message[256];                               \
    long _lDebugtrace = 0;                            \
                                                      \
    RWGETWINREGDWORD(_lDebugtrace, _name);            \
                                                      \
    RwDebugSetTraceState(_lDebugtrace);               \
                                                      \
    _snprintf(_message, sizeof(_message),             \
              "%s(%d): RwDebugSetTraceState(%ld)\n",  \
              __FILE__, __LINE__,                     \
              _lDebugtrace);                          \
    OutputDebugString(_message);                      \
                                                      \
}                                                     \
MACRO_STOP

#if (defined(_CRTDBG_FLAGS))
#undef _CRTDBG_FLAGS
#endif /* (defined(_CRTDBG_FLAGS)) */

#define _CRTDBG_FLAGS                                                           \
( _CRTDBG_ALLOC_MEM_DF || /* Turn on the debug heap allocations                 \
                           * and use the memory block identifiers.              \
                           * This is the only flag that's on by default. */     \
  _CRTDBG_CHECK_ALWAYS_DF || /* Check and validate all memory                   \
                              * on each allocation and deallocation request.    \
                              * Setting this flag on is what catches the        \
                              * under and overwrites                            \
                              * so it is very important to                      \
                              * get it turned on. */                            \
  _CRTDBG_CHECK_CRT_DF || /* Include _CRT_BLOCK memory allocations              \
                           * in all leak detection and state differences. */    \
  _CRTDBG_DELAY_FREE_MEM_DF || /* Instead of truly freeing memory,              \
                                * keep the block allocated and                  \
                                * in the internal heap list.                    \
                                * The blocks are filled with the value0xDD      \
                                * so you know the memory is freed when          \
                                * looking at it in the debugger.                \
                                * By also not freeing the memory,               \
                                * this can help provide stress                  \
                                * conditions for the program. */                \
  _CRTDBG_LEAK_CHECK_DF) /* Do memory leak checking at                          \
                          * the end of the program. */

#endif /* (defined(_DEBUG)) */
#endif /* (defined(RWVERBOSE)) */

#include <math.h>
/*
 * Keep true calls to these functions since
 * some x86 runtime libraries do not support _CIpow() etc
 */
#pragma function( acos, asin, cosh, fmod, pow, sinh , tanh ) 

#if (!defined(RWINT32FROMFLOAT))

static __inline RwInt32
int32fromreal(RwReal x)
{
    RwInt16 savemode;
    RwInt16 workmode;
    RwInt32 res;
    
    _asm
    {
        fnstcw    savemode      ; get fpu mode
        fld dword ptr[x]        ; load rwreal x  
  
        mov       ax,savemode   ; put fpu mode in register
        or        ah,0ch        ; or-in truncate mode
  
        mov       workmode,ax   ; make ready to set fpu mode
        fldcw     workmode      ; set fpu to truncate mode
        fistp     dword ptr[res]; store the rwint32eger result 
        fldcw     savemode      ; restore fpu mode
    }

    return res;
}
#define RwInt32FromRealMacro(x) int32fromreal(x)

#endif /* (!defined(RWINT32FROMFLOAT)) */

#if (!defined(NOASM))
static __inline RwUInt32 
RwFastRealToUInt32Inline(RwReal x)
{
    RwUInt32 res;

    __asm FLD DWord Ptr[x];
    __asm FISTP DWord Ptr[res];
    
    return(res);
}

#define RwFastRealToUInt32 RwFastRealToUInt32Inline

#endif /* (defined(NOASM)) */

#endif /* (defined(_MSC_VER)) */

#endif /* WIN_OSTYPES_H */

/* RWPUBLICEND */
