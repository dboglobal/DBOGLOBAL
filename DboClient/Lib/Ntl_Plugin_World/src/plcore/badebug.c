/*
 * Debug handling
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 */

/**
 * \ingroup rwdebug
 * \page rwdebugoverview RwDebug  Overview
 *
 * (DEBUG BUILD ONLY.)
 *
 * \par Requirements
 * \li \b Headers: rwcore.h
 * \li \b Libraries: rwcore.lib
 *
 * \subsection debugoverview Overview
 *
 * This object represents a debug handler and stream. Error messages can be sent
 * to this object and it will pass them onto the currently registered debug handler.
 * A default handler is registered by default. 
 *
 * Developers should note that this API may degrade performance if too many messages
 * are being sent to the handler.
 *
*/

/****************************************************************************
 Includes
 */

#include <stdlib.h>
#include <string.h>

#include "batypes.h"
#include "balibtyp.h"

#include "bamemory.h"

/* Abstraction of string functionality - for unicode support */
#include "rwstring.h"

#include "osintf.h"

#include "baerr.h"
#include "badebug.h"

/****************************************************************************
 Local Types
 */

/****************************************************************************
 Local (Static) Prototypes
 */
#ifdef RWDEBUG
static void     debugWriteHeader(void);
#endif /* RWDEBUG */

/****************************************************************************
 Local Defines
 */

/****************************************************************************
 Globals (across program)
 */

/****************************************************************************
 Local (static) Globals
 */

/* The strings used in the debug error reporting are derived from the
 * .def files
 */

#define RWECODE(a,b) RWSTRING(b),
const RwChar   *rw_errcomstr[] =
{
    RWSTRING("No Error"),
#include "errcom.def"
};

#undef RWECODE
#define RWECODE(a,b) RWSTRING(#a),
const RwChar   *rw_errcomcstr[] =
{
    RWSTRING("E_RW_NOERROR"),
#include "errcom.def"
};

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Debugging layer

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

#ifdef RWDEBUG

/****************************************************************************
 _rwDebugOpen

 On entry   :
 On exit    : TRUE on success
 */

RwBool
_rwDebugOpen(void)
{
#if (0)
    RWFUNCTION(RWSTRING("_rwDebugOpen"));
#endif /* (0) */

    /* Note the DEBUG macros are deliberately not used here */
    RWSRCGLOBAL(debugStackDepth) = 0;
    RWSRCGLOBAL(debugTrace) = FALSE;

    return (TRUE);
}

/****************************************************************************
 _rwDebugClose

 On entry   :
 On exit    : TRUE on success
 */

RwBool
_rwDebugClose(void)
{
#if (0)
    RWFUNCTION(RWSTRING("_rwDebugClose"));
#endif /* (0) */

    return TRUE;

}

/**
 * \ingroup rwdebug
 * \ref RwDebugSetTraceState is used to enable or disable the reporting
 * of debug trace messages. Initially, the trace state is set to FALSE.
 *
 * This function is only meaningful when used in conjunction with custom
 * versions of the RenderWare libraries that have tracing or stack depth
 * checking enabled.
 *
 * \param state  An \ref RwBool value equal to the new message reporting state:
 *               \li TRUE - Enable debug trace message reporting.
 *               \li FALSE - Disable debug trace message reporting.
 *
 * \return None.
 *
 * \see RwDebugSendMessage
 * \see RwDebugSetHandler
 * \see RwErrorGet
 *
 */
void
RwDebugSetTraceState(RwBool state)
{
    RWAPIFUNCTION(RWSTRING("RwDebugSetTraceState"));
    RWSRCGLOBAL(debugTrace) = state;
    RWRETURNVOID();
}

/**
 * \ingroup rwdebug
 * \ref RwDebugSetHandler is used to register a new handler for the
 * debug stream. A default system error handler is initially defined.
 *
 * Note that there is no default debug handler. Your application needs to 
 * provide its own debug handler. Please refer to the skeleton for an example
 * of how to set a debug handler.
 *
 * This function is only meaningful when used in conjunction with the debug
 * version of the RenderWare libraries.
 *
 * \param handler  A pointer to the new debug message handler.
 *
 * \return Returns a pointer to the previously registered debug message handler
 *        if successful or NULL if no previous handler have been set.
 *
 * \see RwDebugSendMessage
 * \see RwDebugSetTraceState
 * \see RwErrorGet
 *
 */
RwDebugHandler
RwDebugSetHandler(RwDebugHandler handler)
{
    RwDebugHandler  old = RWSRCGLOBAL(debugFunction);

    RWAPIFUNCTION(RWSTRING("RwDebugSetHandler"));

    if (handler)
    {
        /* New handler specified */
        if (handler != old)
        {
            /* It's really a new one */
            RWSRCGLOBAL(debugFunction) = handler;
            debugWriteHeader();
        }
    }
    
    RWRETURN(old);
}

/****************************************************************************
 *
 * This is for documentation purposes only, so must be '#if 0'ed out
 *
 */

#if (defined(DOXYGEN))

/**
 * \ingroup rwdebug
 * \ref RwDebugSendMessage is used to send a message to the currently
 * installed debug handler.
 *
 * This function is only meaningful when used in conjunction with the debug
 * version of the RenderWare libraries.
 *
 * \param type  An \ref RwDebugType value equal to the message type:
 *              \li rwDEBUGASSERT  - Send an assert message.
 *              \li rwDEBUGERROR   - Send an error message.
 *              \li rwDEBUGMESSAGE - Send an informational message.
 *              \li rwDEBUGTRACE   - Send a trace message.
 * \param funcName  A pointer to a string containing the name of the function the
 *       message originated from.
 * \param message  A pointer to a string containing the text of the message.
 *
 * \return None.
 *
 * \see RwDebugSetHandler
 * \see RwDebugSetTraceState
 * \see RwErrorGet
 *
 */
void
RwDebugSendMessage(RwDebugType type,
                   const RwChar *funcName, const RwChar *message)
{
    RWAPIFUNCTION(RWSTRING("RwDebugSendMessage"));

    RWRETURNVOID(FALSE);
}

#endif /* 0 */

void
_rwDebugSendMessage(RwDebugType type,
                    const RwChar *fileName, const RwInt32 line,
                    const RwChar *funcName, const RwChar *message)
{
    const RwChar   *typeString = (const RwChar *)NULL;
    RwChar          buffer[512];

#if (0)
    RWFUNCTION(RWSTRING("_rwDebugSendMessage"));
#endif /* (0) */

    switch (type)
    {
        case rwNADEBUGTYPE:
            break;
        case rwDEBUGASSERT:
            typeString = RWSTRING("ASSERT");
            break;
        case rwDEBUGERROR:
            typeString = RWSTRING("ERROR");
            break;
        case rwDEBUGMESSAGE:
            typeString = RWSTRING("MESSAGE");
            break;
        case rwDEBUGTRACE:
            typeString = RWSTRING("TRACE");
            break;
        case rwDEBUGTYPEFORCEENUMSIZEINT:
            break;
    }

    if (!typeString)
    {
        /* not a valid message type */
        rwstrcpy(buffer, RWSTRING("Invalid Type"));
    }
    else
    {
        rwsprintf(buffer, RWSTRING("%s(%d): %s: %s: %s"), fileName,
                  line, typeString, funcName, message);
    }

    if (RWSRCGLOBAL(debugFunction))
    {
        RWSRCGLOBAL(debugFunction) (type, buffer);
    }
    return;
}

/*
 *  debugWriteHeader()
 *  Writes an introductory string to the top
 *  of the Error Stream when it's first opened.
 *  Details RenderWare implementation number, version number and time
 */

static void
debugWriteHeader(void)
{
    RwChar          buffer[256];
    RwChar          timeBuffer[256];

#if (0)
    RWFUNCTION(RWSTRING("debugWriteHeader"));
#endif /* (0) */

    if (!_rwgetcurrenttime(timeBuffer, sizeof (timeBuffer)))
    {
        /* We failed to get time, use a NUL string */
        timeBuffer[0] = '\0';
    }
    else
    {
        /* Remove newline that ctime in get current time appends */
        timeBuffer[rwstrlen(timeBuffer) - 1] = '\0';
    }

    /* Construct a header string */
    rwsprintf(buffer,
              RWSTRING("RenderWare(TM) V%d.%d.%d.%d(%s) Debugging Session: %s"),
              (RwEngineGetVersion() & 0xf0000) >> 16,
              (RwEngineGetVersion() & 0x0f000) >> 12,
              (RwEngineGetVersion() & 0x00f00) >> 8,
              (RwEngineGetVersion() & 0x000ff),
              __DATE__,
              timeBuffer);

    RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, RWSTRING(" "));
    RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);

    /* Now write a row of dashes to underline the header */
    {
        int             i;
        RwChar         *b;

        b = buffer;
        i = rwstrlen(buffer);
        while (i--)
            *b++ = '-';
    }

    RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, buffer);
    RWSRCGLOBAL(debugFunction) (rwDEBUGMESSAGE, RWSTRING(" "));
    return;
}

static RwChar   dberr[512];

RwChar         *
_rwdberrcommon(RwInt32 code,...)
{
    va_list         ap;

#if (0)
    RWFUNCTION(RWSTRING("_rwdberrcommon"));
#endif /* (0) */

    va_start(ap, code);

    code &= ~0x80000000;       /* remove MSB */
    rwstrcpy(dberr, rw_errcomcstr[code]);
    rwstrcat(dberr, RWSTRING(" : "));
    rwvsprintf(&dberr[rwstrlen(dberr)], rw_errcomstr[code], ap);
    va_end(ap);
    return dberr;
}

RwChar         *
_rwdbsprintf(const RwChar * format,...)
{
    va_list         ap;

#if (0)
    RWFUNCTION(RWSTRING("_rwdbsprintf"));
#endif /* (0) */

    va_start(ap, format);

    rwvsprintf(dberr, format, ap);
    va_end(ap);
    return dberr;
}

#endif /* RWDEBUG */

