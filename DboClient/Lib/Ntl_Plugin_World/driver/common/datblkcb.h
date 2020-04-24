/***************************************************************************
 *                                                                         *
 * Module  : datblkcb.h                                                    *
 *                                                                         *
 * Purpose : Data block callbacks                                          *
 *                                                                         *
 **************************************************************************/

#ifndef DATBLKCB_H
#define DATBLKCB_H

/****************************************************************************
 Includes
 */

#include "batypes.h"

/****************************************************************************
 Defines
 */

/****************************************************************************
 Global Types
 */

enum RwRecallDirection
{
    rwDIRNADIR = 0,
    rwDIRFIFO,
    rwDIRLIFO,
    rwRECALLDIRECTIONFORCEENUMSIZEINT =  RWFORCEENUMSIZEINT
};
typedef enum RwRecallDirection RwRecallDirection;

typedef void (*RwDataBlockCallBack)(void *data);

typedef struct RwDataBlockMgr RwDataBlockMgr;
struct RwDataBlockMgr
{
    /* This is the global pool */
    RwUInt8        *dataBlocks;            /* Pool of memory allocated for driver data */
    RwUInt32        dataBlocksSize;         /* Amount of memory allocated */
    RwUInt32        dataBlocksLevel;        /* Amount of memory used */

    RwUInt32       *dataBlockOffsets;      /* Array of offsets into memory pool */
    RwUInt32        dataBlockOffsetSize;    /* Number of entries in the array */
    RwUInt32        numDataBlockEntries;    /* Num of polygon data entries */

    RwUInt32        sampleNum;             /* Sample number */
    RwUInt32        peakLevel;             /* Peak level in the last <n> flushes */
};

/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */
extern RwBool 
_rwDataBlockMgrDestroy(RwDataBlockMgr *blockMgr);

extern RwDataBlockMgr *
_rwDataBlockMgrCreate(void);

extern RwDataBlockMgr *
_rwDataBlockMgrEmptyBlocks(RwDataBlockMgr *blockMgr);

extern RwDataBlockMgr *
_rwDataBlockMgrRecallBlocks(RwDataBlockMgr *blockMgr, 
                            RwRecallDirection direction);

extern void *
_rwDataBlockMgrCreateBlock(RwDataBlockMgr *blockMgr, 
                           RwUInt32 nSize,
                           RwDataBlockCallBack callBack);

#define rwDataBlockMgrDestroy(blockMgr) \
   _rwDataBlockMgrDestroy(blockMgr)

#define rwDataBlockMgrCreate() \
   _rwDataBlockMgrCreate()

#define rwDataBlockMgrEmptyBlocks(blockMgr) \
   _rwDataBlockMgrEmptyBlocks(blockMgr)

#define rwDataBlockMgrRecallBlocks(blockMgr,direction) \
   _rwDataBlockMgrRecallBlocks(blockMgr,direction);

#define rwDataBlockMgrCreateBlock(blockMgr, nSize, callBack) \
   _rwDataBlockMgrCreateBlock(blockMgr, nSize, callBack)

#define rwDataBlockEmpty(blockMgr) ((blockMgr)->dataBlocksLevel == 0)



#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#endif /* DATBLKCB_H */
