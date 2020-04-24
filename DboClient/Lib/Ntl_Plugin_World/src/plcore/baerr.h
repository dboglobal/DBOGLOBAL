/***************************************************************************
 *                                                                         *
 * Module  : baerr.h                                                       *
 *                                                                         *
 * Purpose : Error handling                                                *
 *                                                                         *
 **************************************************************************/

#ifndef RWERR_H
#define RWERR_H

/****************************************************************************
 Includes
 */

#include "rwdbgerr.h"

/****************************************************************************
 Defines
 */

/* RWPUBLIC */
/****************************************************************************
 Global Types
 */

typedef struct RwError RwError;
#ifndef RWADOXYGENEXTERNAL
/**
 * \ingroup rwerror
 * \struct RwError
 * This type represents a RenderWare error specified by the
 * ID of the plugin that the error was issued from (pluginID) and the error
 * code itself (errorCode) (see API function \ref RwErrorGet).
 * \param pluginID The ID of the plugin that issued the error.
 * \param errorCode A value representing the error code.
 */
#endif /* RWADOXYGENEXTERNAL */
struct RwError
{
    RwInt32     pluginID;  /**< Internal Use */
    RwInt32     errorCode; /**< Internal Use */
};

/* RWPUBLICEND */
typedef struct rwErrorGlobals rwErrorGlobals;
struct rwErrorGlobals
{
    RwError     errorCode;                      /* Error code for last error */
};

/* RWPUBLIC */
#define RWECODE(a,b) a,

/* common errors have the MSB set */

enum RwErrorCodeCommon
{
    E_RW_NOERROR = (int)0x80000000L,
#include "errcom.def"
    E_RW_LASTERROR = RWFORCEENUMSIZEINT
};
typedef enum RwErrorCodeCommon RwErrorCodeCommon;

#undef RWECODE

/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

/* RWPUBLICEND */

extern void *_rwErrorOpen(void *object, RwInt32 offset, RwInt32 size);
extern void *_rwErrorClose(void *object, RwInt32 offset, RwInt32 size);

/* RWPUBLIC */
extern RwError *RwErrorGet(RwError *code);
extern RwError *RwErrorSet(RwError *code);
extern RwInt32 _rwerror(RwInt32 code, ...);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

/* RWPUBLICEND */

#endif /* RWERR_H */

