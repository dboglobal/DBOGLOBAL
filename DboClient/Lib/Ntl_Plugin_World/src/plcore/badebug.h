/***************************************************************************
 *                                                                         *
 * Module  : badebug.h                                                     *
 *                                                                         *
 * Purpose : Debug handling                                                *
 *                                                                         *
 **************************************************************************/

#ifndef RWDEBUG_H
#define RWDEBUG_H

/****************************************************************************
 Includes
 */

#if (defined(RWDEBUG) && defined(RWVERBOSE))
#if (defined(_MSC_VER))
#if (_MSC_VER>=1000)
/* Pick up _ASSERTE macro */
#ifdef _XBOX
#include <xtl.h>
#endif /* _XBOX */
#if (defined(RWMEMDEBUG) && !defined(_CRTDBG_MAP_ALLOC))
#define _CRTDBG_MAP_ALLOC
#endif /* defined(RWMEMDEBUG) && !defined(_CRTDBG_MAP_ALLOC)) */
#include <crtdbg.h>
#undef  RWASSERTE
#define RWASSERTE(_condition) _ASSERTE(_condition)
#endif /* (_MSC_VER>=1000) */
#endif /* (defined(_MSC_VER)) */
#endif /* (defined(RWDEBUG) && defined(RWVERBOSE)) */

#include "baerr.h"
#include "batype.h"

#if (!defined(RWASSERTE))
#define RWASSERTE(_condition)  /* No-Op */
#endif /* (!defined(RWASSERTE)) */

#if (!defined(RWPENTER))
#define RWPENTER(_func)        /* No-Op */
#endif /* (!defined(RWPENTER)) */

#if (!defined(RWPEXIT))
#define RWPEXIT(_func)         /* No-Op */
#endif /* (!defined(RWPEXIT)) */

/****************************************************************************
 Defines
 */

#ifdef RWDEBUG

#if (!(defined(RWDEBUGSTACKDEPTH)))
#define RWDEBUGSTACKDEPTH (RWSRCGLOBAL(debugStackDepth))
#endif /* (!(defined(RWDEBUGSTACKDEPTH))) */

/* Message macros */

#ifdef RWTRACE

/* Note RWTRACE should only be defined for internal builds. It should
 * also only be used rarely. It will cause the generation of Trace
 * messages for all functions. Not just those directly called from
 * the application
 */

#define RWAPIFUNCTION(function)                                         \
static const RwChar __dbFunctionName[] = function;                      \
const RwInt32 startstackdepth = RWDEBUGSTACKDEPTH++;                    \
RWPENTER(__dbFunctionName);                                             \
if (RWSRCGLOBAL(debugTrace))                                            \
{                                                                       \
    RwDebugSendMessage(rwDEBUGTRACE,                                    \
                       __dbFunctionName,                                \
                       _rwdbsprintf("Enter %s [Depth %d]",              \
                                    (startstackdepth)?"SPI":"API",      \
                                    (int)startstackdepth));             \
}

#define RWFUNCTION(function) RWAPIFUNCTION(function)

#define RWRETURN(result)                                                \
MACRO_START                                                             \
{                                                                       \
    RwInt32 _validateStackDepth = --RWDEBUGSTACKDEPTH;                  \
    if (_validateStackDepth != startstackdepth)                         \
    {                                                                   \
        RwDebugSendMessage(rwDEBUGERROR,                                \
                             __dbFunctionName,                          \
                             _rwdberrcommon(E_RW_DEBUGSTACK));          \
        RWDEBUGSTACKDEPTH = startstackdepth;                            \
    }                                                                   \
    if (RWSRCGLOBAL(debugTrace))                                        \
    {                                                                   \
        RwDebugSendMessage(rwDEBUGTRACE,                                \
                             __dbFunctionName, RWSTRING("Exit"));       \
    }                                                                   \
    RWASSERTE(_validateStackDepth == startstackdepth);                  \
    RWPEXIT(__dbFunctionName);                                          \
    return (result);                                                    \
}                                                                       \
MACRO_STOP

#define RWRETURNVOID()                                                  \
MACRO_START                                                             \
{                                                                       \
    RwInt32 _validateStackDepth = --RWDEBUGSTACKDEPTH;                  \
    if ( _validateStackDepth != startstackdepth)                        \
    {                                                                   \
        RwDebugSendMessage(rwDEBUGERROR,                                \
                             __dbFunctionName,                          \
                             _rwdberrcommon (E_RW_DEBUGSTACK));         \
        RWDEBUGSTACKDEPTH = startstackdepth;                            \
    }                                                                   \
    if (RWSRCGLOBAL(debugTrace))                                        \
    {                                                                   \
        RwDebugSendMessage(rwDEBUGTRACE,                                \
                             __dbFunctionName, RWSTRING("Exit"));       \
    }                                                                   \
    RWASSERTE(_validateStackDepth == startstackdepth);                  \
    RWPEXIT(__dbFunctionName);                                          \
    return;                                                             \
}                                                                       \
MACRO_STOP

#else /* RWTRACE */

/* Defining RWSTACKDEPTHCHECKING in a non-RWTRACE build will enable stack
   depth checking on entry and exit of API and non API functions. It also
   allows a trace of the entry and exit from the RenderWare Graphics
   library.
   This functionality is disabled by default because it is not thread-safe
 */
#ifdef RWSTACKDEPTHCHECKING

#define RWSTACKDEPTHCHECKONENTRY()                                      \
    const RwInt32 startstackdepth = RWDEBUGSTACKDEPTH++;                \
    if (RWSRCGLOBAL(debugTrace) && !startstackdepth)                    \
    {                                                                   \
        RwDebugSendMessage(rwDEBUGTRACE,                                \
                             __dbFunctionName, RWSTRING("Enter"));      \
    }

#define RWSTACKDEPTHCHECKONRETURN()                                     \
MACRO_START                                                             \
{                                                                       \
    RwInt32 _validateStackDepth = --RWDEBUGSTACKDEPTH;                  \
    if (_validateStackDepth != startstackdepth)                         \
    {                                                                   \
        RwDebugSendMessage(rwDEBUGERROR,                                \
                             __dbFunctionName,                          \
                             _rwdberrcommon(E_RW_DEBUGSTACK));          \
        RWDEBUGSTACKDEPTH = startstackdepth;                            \
    }                                                                   \
    if (RWSRCGLOBAL(debugTrace) && (!startstackdepth))                  \
    {                                                                   \
        RwDebugSendMessage(rwDEBUGTRACE,                                \
                             __dbFunctionName, RWSTRING("Exit"));       \
    }                                                                   \
    RWASSERTE(_validateStackDepth == startstackdepth);                  \
}                                                                       \
MACRO_STOP

#else /* RWSTACKDEPTHCHECKING */

#define RWSTACKDEPTHCHECKONENTRY()
#define RWSTACKDEPTHCHECKONRETURN()

#endif /* RWSTACKDEPTHCHECKING */


#define RWAPIFUNCTION(function)                                         \
static const RwChar __dbFunctionName[] = function;                      \
do { } while (__dbFunctionName != __dbFunctionName);                    \
RWSTACKDEPTHCHECKONENTRY();                                             \
RWPENTER(__dbFunctionName);

#define RWFUNCTION(function) RWAPIFUNCTION(function)

#define RWRETURN(result)                                                \
MACRO_START                                                             \
{                                                                       \
    RWSTACKDEPTHCHECKONRETURN();                                        \
    RWPEXIT(__dbFunctionName);                                          \
    return (result);                                                    \
}                                                                       \
MACRO_STOP

#define RWRETURNVOID()                                                  \
MACRO_START                                                             \
{                                                                       \
    RWSTACKDEPTHCHECKONRETURN();                                        \
    RWPEXIT(__dbFunctionName);                                          \
    return;                                                             \
}                                                                       \
MACRO_STOP

#endif /* RWTRACE */

#define RWERROR(ecode)                          \
MACRO_START                                     \
{                                               \
    RwError _rwErrorCode;                       \
                                                \
    _rwErrorCode.pluginID = rwID_COREPLUGIN;    \
    _rwErrorCode.errorCode = _rwerror ecode;    \
                                                \
    RwErrorSet(&_rwErrorCode);                  \
                                                \
    if (_rwErrorCode.errorCode & 0x80000000)    \
    {                                           \
        RwDebugSendMessage(rwDEBUGERROR,        \
                         __dbFunctionName,      \
                         _rwdberrcommon ecode); \
    }                                           \
    else                                        \
    {                                           \
        RwDebugSendMessage(rwDEBUGERROR,        \
                         __dbFunctionName,      \
                         _rwdb_errcore ecode);  \
    }                                           \
}                                               \
MACRO_STOP

#define RWMESSAGE(args)                                         \
MACRO_START                                                     \
{                                                               \
    RwDebugSendMessage(rwDEBUGMESSAGE,                          \
                         __dbFunctionName, _rwdbsprintf args);  \
}                                                               \
MACRO_STOP

#define RWASSERT(condition)                             \
MACRO_START                                             \
{                                                       \
    if (!(condition))                                   \
    {                                                   \
        RwDebugSendMessage(rwDEBUGASSERT,               \
                           __dbFunctionName,            \
                           RWSTRING(#condition));       \
    }                                                   \
    RWASSERTE(condition);                               \
}                                                       \
MACRO_STOP

#define RWASSERTM(condition, messageArgs)               \
MACRO_START                                             \
{                                                       \
    if (!(condition))                                   \
    {                                                   \
        RwDebugSendMessage(rwDEBUGASSERT,               \
                           __dbFunctionName,            \
                           RWSTRING(#condition));       \
        RwDebugSendMessage(rwDEBUGMESSAGE,              \
                           __dbFunctionName,            \
                           _rwdbsprintf messageArgs);   \
    }                                                   \
    RWASSERTE(condition);                               \
}                                                       \
MACRO_STOP

#else /* RWDEBUG */

#define RWRETURN(value) return(value)
#define RWRETURNVOID() return
#define RWERROR(errorcode)                              \
MACRO_START                                             \
{                                                       \
    RwError _rwErrorCode;                               \
                                                        \
    _rwErrorCode.pluginID = rwID_COREPLUGIN;            \
    _rwErrorCode.errorCode = _rwerror errorcode;        \
                                                        \
    RwErrorSet(&_rwErrorCode);                          \
}                                                       \
MACRO_STOP

#define RWFUNCTION(name)       /* No op */
#define RWAPIFUNCTION(name)    /* No op */
#define RWASSERT(condition)    /* No op */
#define RWASSERTM(condition, messageArgs)    /* No op */
#define RWMESSAGE(args)        /* No op */

#endif /* RWDEBUG */

#ifdef RWSTACKDEPTHCHECKING
#define RWVALIDATEDEBUGSTACKDEPTH() \
    RWASSERT(1 == (RWDEBUGSTACKDEPTH - startstackdepth))
#else /* RWSTACKDEPTHCHECKING */
#define RWVALIDATEDEBUGSTACKDEPTH()
#endif /* RWSTACKDEPTHCHECKING */

/* RWPUBLIC */

/****************************************************************************
 Global Types
 */

/**
 * \ingroup rwdebug
 * RwDebugType 
 * This type represents the different types of debug and 
 * trace messages that can be sent to the currently installed debug handler 
 * (see API function \ref RwDebugSendMessage)*/
enum RwDebugType
{
    rwNADEBUGTYPE = 0,          /**<Invalid */
    rwDEBUGASSERT,              /**<Send an assert message */
    rwDEBUGERROR,               /**<Send an error message */
    rwDEBUGMESSAGE,             /**<Send an informational message */
    rwDEBUGTRACE,               /**<Send a trace message */
    rwDEBUGTYPEFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RwDebugType RwDebugType;

/**
 * \ingroup rwdebug
 * \ref RwDebugHandler
 * This type represents the
 * function called from \ref RwDebugSendMessage for sending a message to the
 * RenderWare debug stream.
 *
 * \param  type   Type of debug message (assert, error, etc.).
 *
 * \param  string   Pointer to a string containing the error
 * message.
 *
 * \see RwDebugSetHandler
 */
typedef void        (*RwDebugHandler) (RwDebugType type,

                                       const RwChar * string);

#ifdef RWDEBUG

#define RwDebugSendMessage(type, funcName, message)     \
        _rwDebugSendMessage(type,                       \
                            RWSTRING(__FILE__),         \
                            __LINE__,                   \
                            funcName,                   \
                            message)

/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern              "C"
{
#endif                          /* __cplusplus */

/* Setting the debug message handler */
extern RwDebugHandler RwDebugSetHandler(RwDebugHandler handler);
extern void         RwDebugSetTraceState(RwBool state);

extern void         _rwDebugSendMessage(RwDebugType type,
                                        const RwChar * file,
                                        const RwInt32 line,
                                        const RwChar * funcName,
                                        const RwChar * message);

/* Sending a message */
extern RwChar      *_rwdberrcommon(RwInt32 code, ...);

#if (!defined(DOXYGEN))
/* Doxy doesn't appear to like the __RWFORMAT__ attribute */
extern RwChar      *_rwdbsprintf(const RwChar * format,
                                 ...) __RWFORMAT__(printf, 1, 2);
#endif /* (!defined(DOXYGEN)) */
/* RWPUBLICEND */

/* Opening and closing */
extern RwBool       _rwDebugOpen(void);
extern RwBool       _rwDebugClose(void);

/* Setting Stream handlers at start */
extern RwBool       _rwDebugStart(void);

/* RWPUBLIC */

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#else /* RWDEBUG */

#define RwDebugSetHandler(handler)
#define RwDebugSetTraceState(state)
#define RwDebugSendMessage(type, funcName, message)

#if (!defined(RWREGSETDEBUGTRACE))
#define RWREGSETDEBUGTRACE(_name) /* No op */
#endif /* (!defined(RWREGSETDEBUGTRACE)) */

#endif /* RWDEBUG */

/* RWPUBLICEND */

#if (defined(RWDEBUG) && defined(RWVERBOSE))
#define RWNOTIFY(_func, _message)                                       \
    _rwDebugSendMessage(rwDEBUGTRACE, RWSTRING(__FILE__),  __LINE__,    \
                        RWSTRING(_func), RWSTRING(_message))
#endif /* (defined(RWDEBUG) && defined(RWVERBOSE)) */

#if (!defined(RWNOTIFY))
#define RWNOTIFY(_func, _message) /* No op */
#endif /* (!defined(RWNOTIFY)) */

#endif /* _RWDEBUG_H */

