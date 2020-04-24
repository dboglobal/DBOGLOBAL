/*
 * Error code handling
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 */

/**
 * \ingroup rwerror
 * \page rwerroroverview RwError Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h
 * \li \b Libraries: rwcore.lib
 *
 * \subsection erroroverview Overview
 *
 * This object provides a generic error reporting mechanism.
 *
 * If a function fails for any reason, it uses \ref RwErrorSet to set the error
 * code to a predetermined value (usually a constant defined in a header file),
 * so that the calling function can determine what the problem is using
 * \ref RwErrorGet.
 *
*/




/****************************************************************************
 Includes
 */

#include <stdlib.h>
#include <string.h>

#include "batypes.h"

#include "balibtyp.h"

/* Error handling */

#include "baerr.h"
#include "badebug.h"


/****************************************************************************
 Local Types
 */

/****************************************************************************
 Local (Static) Prototypes
 */

/****************************************************************************
 Local Defines
 */

#define RWERRORGLOBAL(var) \
    (RWPLUGINOFFSET(rwErrorGlobals, RwEngineInstance, \
    errorModule.globalsOffset)->var)

/****************************************************************************
 Globals (across program)
 */

/****************************************************************************
 Local (static) Globals
 */

static RwModuleInfo errorModule;

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Error handling

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 _rwErrorOpen

 On entry   : object, offset, size
 On exit    : object pointer on success
 */

void *
_rwErrorOpen(void *object, RwInt32 offset, RwInt32 __RWUNUSED__ size )
{
    RWFUNCTION(RWSTRING("_rwErrorOpen"));

    /* Cache the globals offset */
    errorModule.globalsOffset = offset;

    /* One more module instance */
    errorModule.numInstances++;

    /* Initialise the variables */
    RWERRORGLOBAL(errorCode).pluginID = 0;
    RWERRORGLOBAL(errorCode).errorCode = (RwInt32) E_RW_NOERROR;

    /* All done */
    RWRETURN(object);
}

/****************************************************************************
 _rwErrorclose

 On entry   : object, offset, size
 On exit    : object pointer on success
 */

void *
_rwErrorClose(void *object,
              RwInt32 __RWUNUSED__ offset ,
              RwInt32 __RWUNUSED__ size)
{
    RWFUNCTION(RWSTRING("_rwErrorClose"));

    /* One less module instance */
    errorModule.numInstances--;

    RWRETURN(object);
}

/**
 * \ingroup rwerror
 * \ref RwErrorSet is used to set the error code. The error code
 * identifies both the numerical value of the error and the plugin that
 * issued it. The error code is reset to E_RW_NOERROR once it has been
 * read.
 *
 * This function is available in both debug and release versions of the
 * RenderWare library. In a debug library an error message is also
 * written to the debug stream when an error is raised.
 *
 * \param code  A pointer to an \ref RwError value that will be used to set
 *              the error code.
 *
 * \return A pointer to the error if successful.
 *
 * \see RwErrorGet
 * \see RwDebugSetHandler
 * \see RwDebugSendMessage
 * \see RwDebugSetTraceState
 */
/**************************************************************************/
RwError *
RwErrorSet(RwError *code)
{
    RWAPIFUNCTION(RWSTRING("RwErrorSet"));
    RWASSERT(code);

    if ((RWERRORGLOBAL(errorCode).pluginID == 0x0) &&
        (RWERRORGLOBAL(errorCode).errorCode == (RwInt32) E_RW_NOERROR))
    {
        if (code->errorCode & 0x80000000)
        {
            /* common error */
            RWERRORGLOBAL(errorCode).pluginID = 0x0;
        }
        else
        {
            /* plugin error */
            RWERRORGLOBAL(errorCode).pluginID = code->pluginID;
        }
        /* No current error status */
        RWERRORGLOBAL(errorCode).errorCode = code->errorCode;
    }
    RWRETURN(code);
}

/**
 * \ingroup rwerror
 * \ref RwErrorGet is used to retrieve the error code for the
 * last reported error. The error code identifies both the numerical value
 * of the error and the plugin that issued it. The error code is reset to
 * E_RW_NOERROR once it has been read.
 *
 * This function is available in both debug and release versions of the
 * RenderWare library. In a debug library an error message is also
 * written to the debug stream when an error is raised.
 *
 * \param code  A pointer to an \ref RwError value that will receive the error
 *              code.
 *
 * \return Returns a pointer to an RwError code if successful.
 *
 * \see RwDebugSetHandler
 * \see RwDebugSendMessage
 * \see RwDebugSetTraceState
 *
 */
RwError *
RwErrorGet(RwError *code)
{
    RWAPIFUNCTION(RWSTRING("RwErrorGet"));
    RWASSERT(code);

    *code = RWERRORGLOBAL(errorCode);
    RWERRORGLOBAL(errorCode).pluginID = 0x0;
    RWERRORGLOBAL(errorCode).errorCode = (RwInt32) E_RW_NOERROR;

    RWRETURN(code);
}

RwInt32
_rwerror(RwInt32 code, ...)
{
    va_list             ap;

#if (0)
    RWFUNCTION(RWSTRING("_rwerror"));
#endif /* (0) */

    va_start(ap, code);
    va_end(ap);
    return (code);
}

