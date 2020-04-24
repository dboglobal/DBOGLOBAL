/***************************************************************************
 *                                                                         *
 * Module  : barwasmm.h                                                    *
 *                                                                         *
 * Purpose : Renderware maths assembler                                    *
 *                                                                         *
 **************************************************************************/

#ifndef RWRWASMM_H
#define RWRWASMM_H

/****************************************************************************
 Includes
 */

/****************************************************************************
 Defines
 */

/****************************************************************************
 Global Types
 */

/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

extern RWASMAPI(RwFixed)
_rwFRwDiv(RwFixed nA,RwFixed nB);
extern RWASMAPI(RwFixed)
_rwFRwMul(RwFixed nA,RwFixed nB);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#endif /* RWRWASMM_H */



