
/***************************************************************************
 *                                                                         *
 * Module  : babinmtx.h                                                    *
 *                                                                         *
 * Purpose : Binary matrix handling functions.                          *
 *                                                                         *
 **************************************************************************/

#ifndef RWBINMTX_H
#define RWBINMTX_H

/****************************************************************************
 Includes
 */

#include "bamatrix.h"
#include "bastream.h"

/****************************************************************************
 Defines
 */

/* RWPUBLIC */

/****************************************************************************
 Global types
 */

/* Matrix stream format */
typedef struct rwStreamMatrix RwMatrixChunkInfo;
typedef struct rwStreamMatrix rwStreamMatrix;
#if (!defined(DOXYGEN))
struct rwStreamMatrix
{
    RwV3d               right;
    RwV3d               up;
    RwV3d               at;
    RwV3d               pos;
    RwInt32             type;
};
#endif /* (!defined(DOXYGEN)) */

/* RWPUBLICEND */

/****************************************************************************
 Global Variables 
 */

/* RWPUBLIC */

/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern              "C"
{
#endif                          /* __cplusplus */

/* Matrix binary format */
extern RwUInt32     RwMatrixStreamGetSize(const RwMatrix * matrix);
extern RwMatrix    *RwMatrixStreamRead(RwStream * stream,
                                       RwMatrix * matrix);
extern const RwMatrix *RwMatrixStreamWrite(const RwMatrix * matrix,
                                           RwStream * stream);
extern RwMatrixChunkInfo *RwMatrixChunkInfoRead(RwStream * stream,
                                                RwMatrixChunkInfo *
                                                matrixChunkInfo,
                                                RwInt32 * bytesRead);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

/* RWPUBLICEND */

#endif                          /* RWBINMTX_H */
