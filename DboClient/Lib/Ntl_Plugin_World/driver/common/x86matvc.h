/***************************************************************************
 *                                                                         *
 * Purpose : Matrix-vector multiplication                                  *
 *                                                                         *
 **************************************************************************/

#ifndef X86MATVC_H
#define X86MATVC_H

/****************************************************************************
 Includes
 */

#include "bamatrix.h"
#include "bavector.h"

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

extern RwV3d* RWASMCALL _rwIntelx86AsmV3dTransformPoints( RwV3d *pointsOut,
                                                          const RwV3d *pointsIn,
                                                          RwInt32 numPoints,
                                                          const RwMatrix *matrix );

extern RwV3d* RWASMCALL _rwIntelx86AsmV3dTransformVectors( RwV3d *pointsOut,
                                                           const RwV3d *pointsIn,
                                                           RwInt32 numPoints,
                                                           const RwMatrix *matrix );

extern RwV3d* RWASMCALL _rwIntelx86AsmV3dTransformPoint( RwV3d *pointOut,
                                                         const RwV3d *pointIn,
                                                         const RwMatrix *matrix );

extern RwV3d* RWASMCALL _rwIntelx86AsmV3dTransformVector( RwV3d *pointOut,
                                                          const RwV3d *pointIn,
                                                          const RwMatrix *matrix );

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#endif /* X86MATVC_H */
