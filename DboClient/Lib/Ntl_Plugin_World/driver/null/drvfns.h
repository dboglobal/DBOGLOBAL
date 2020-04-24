/***************************************************************************
 *                                                                         *
 * Module  : drvfns.h (null)                                               *
 *                                                                         *
 * Purpose : Driver functionality                                          *
 *                                                                         *
 **************************************************************************/

#ifndef DRVFNS_H
#define DRVFNS_H

#include "batypes.h"

/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

/* Called from RwEngineInit to give the driver a chance to register plugins */
extern RwBool _rwDeviceRegisterPlugin(void);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

/* RWPUBLICEND */

#endif /* DRVFNS_H */
