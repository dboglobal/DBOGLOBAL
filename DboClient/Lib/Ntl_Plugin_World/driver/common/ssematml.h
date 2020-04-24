/***************************************************************************
 *                                                                         *
 * Purpose : SSE Matrix Multiply                                           *
 *                                                                         *
 **************************************************************************/

#ifndef SSEMATML_H
#define SSEMATML_H

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

extern void
_rwIntelSSEMatrixMultiply( RwMatrix *dstMat,
                           const RwMatrix *matA,
                           const RwMatrix *matB );

extern void
_rwIntelSSEAlignedMatrixMultiply( RwMatrix *dstMat,
                                  const RwMatrix *matA,
                                  const RwMatrix *matB );

extern RwV3d *
_rwIntelSSEV3dTransformPoints( RwV3d          *pointsOut,
                               const RwV3d    *pointsIn,  
                               RwInt32        numPoints,  
                               const RwMatrix *matrix );

extern RwV3d *
_rwIntelSSEV3dTransformPointsAlignedMatrix( RwV3d          *pointsOut,
                                            const RwV3d    *pointsIn,  
                                            RwInt32        numPoints,  
                                            const RwMatrix *matrix );

extern RwV3d *
_rwIntelSSEV3dTransformVectors( RwV3d          *vectorsOut,
                                const RwV3d    *vectorsIn,  
                                RwInt32        numVectors,  
                                const RwMatrix *matrix );

extern RwV3d *
_rwIntelSSEV3dTransformVectorsAlignedMatrix( RwV3d          *vectorsOut,
                                             const RwV3d    *vectorsIn,  
                                             RwInt32        numVectors,  
                                             const RwMatrix *matrix );


extern RwV3d *
_rwIntelSSEV3dTransformPoint( RwV3d          *pointOut,
                              const RwV3d    *pointIn,  
                              const RwMatrix *matrix );

extern RwV3d *
_rwIntelSSEV3dTransformPointAlignedMatrix( RwV3d          *pointOut,
                                           const RwV3d    *pointIn,  
                                           const RwMatrix *matrix );

extern RwV3d *
_rwIntelSSEV3dTransformVector( RwV3d          *vectorOut,
                               const RwV3d    *vectorIn,  
                               const RwMatrix *matrix );

extern RwV3d *
_rwIntelSSEV3dTransformVectorAlignedMatrix( RwV3d          *vectorOut,
                                            const RwV3d    *vectorIn,  
                                            const RwMatrix *matrix );

/* Pentium 4 specific versions */
extern RwV3d *
_rwIntelSSEP4V3dTransformPoints(RwV3d          *pointsOut,
                                const RwV3d    *pointsIn,  
                                RwInt32        numPoints,  
                                const RwMatrix *matrix );

extern RwV3d *
_rwIntelSSEP4V3dTransformVectors(RwV3d          *vectorsOut,
                                 const RwV3d    *vectorsIn,  
                                 RwInt32        numVectors,  
                                 const RwMatrix *matrix );

extern RwV3d *
_rwIntelSSEP4V3dTransformPoint(RwV3d          *pointOut,
                               const RwV3d    *pointIn,  
                               const RwMatrix *matrix );


extern RwV3d *
_rwIntelSSEP4V3dTransformVector(RwV3d          *vectorOut,
                                const RwV3d    *vectorIn,  
                                const RwMatrix *matrix );

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#endif /* SSEMATML_H */
