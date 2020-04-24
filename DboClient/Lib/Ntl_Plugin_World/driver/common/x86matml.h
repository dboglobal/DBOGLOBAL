/***************************************************************************
 *                                                                         *
 * Purpose : Transform                                                     *
 *                                                                         *
 **************************************************************************/

#ifndef X86MATML_H
#define X86MATML_H

/****************************************************************************
 Includes
 */

#include "bamatrix.h"

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

extern void RWASMCALL _rwX86AsmMatMul(RwMatrix *dstMat, const RwMatrix *matA,
                                                        const RwMatrix *matB);

extern void RWASMCALL _rwIntelx86AsmMatrixMultiply(RwMatrix *dstMat, const RwMatrix *matA,
                                                                     const RwMatrix *matB);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#endif /* X86MATML_H */
