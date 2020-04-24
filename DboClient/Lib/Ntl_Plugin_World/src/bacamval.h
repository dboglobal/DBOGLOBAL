/***************************************************************************
 * Module  : bacamval.h                                                    *
 *                                                                         *
 * Purpose : Camera functions                                              *
 *                                                                         *
 **************************************************************************/

#ifndef RWCAMVAL_H
#define RWCAMVAL_H

/****************************************************************************
 Includes
 */

#include "bacamera.h"

/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern              "C"
{
#endif                          /* __cplusplus */

/* Open closing */
extern void         _rwCameraValOpen(void);
extern void         _rwCameraValClose(void);

/* Render */
extern void         _rwCameraValRender(RwCamera * camera);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#endif                          /* RWCAMVAL_H */
