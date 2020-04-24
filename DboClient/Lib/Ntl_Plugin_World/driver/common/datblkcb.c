/***************************************************************************
 *                                                                         *
 * Module  : bacachd3.c                                                    *
 *                                                                         *
 * Purpose : D3D texture cache handling                                    *
 *                                                                         *
 **************************************************************************/

/****************************************************************************
 Includes
 */

#include <stdlib.h>

#include "batypes.h"
#include "balibtyp.h"
#include "barwtyp.h"

/* This file */
#include "datblkcb.h"


/****************************************************************************
 Global prototypes
 */

/****************************************************************************
 Local Defines
 */

#define rwBLOCKOFFSETGRANULARITY    5

/****************************************************************************
 Local Types
 */

/****************************************************************************
 Local (Static) Prototypes
 */

/****************************************************************************
 Globals (across program)
 */

/****************************************************************************
 Local (static) Globals
 */

/****************************************************************************
 rwDataBlockMgrCreate

 On entry   : Nothing
 On exit    : Pointer to datablock callback manager if successful, NULL on failure
 */

RwDataBlockMgr *
_rwDataBlockMgrCreate(void)
{
    RwDataBlockMgr      *blockMgr;

    RWFUNCTION(RWSTRING("_rwDataBlockMgrCreate"));

    blockMgr = (RwDataBlockMgr *)RwDriverMalloc(sizeof(RwDataBlockMgr),
                                rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT);
    if (blockMgr)
    {
        blockMgr->dataBlocks = NULL;
        blockMgr->dataBlocksSize = 0;
        blockMgr->dataBlocksLevel = 0;

        blockMgr->dataBlockOffsets = NULL;
        blockMgr->dataBlockOffsetSize = 0;
        blockMgr->numDataBlockEntries = 0;

        blockMgr->sampleNum = 0;
        blockMgr->peakLevel = 0;

        RWRETURN(blockMgr);
    }

    RWERROR((E_RW_NOMEM, sizeof(RwDataBlockMgr)));
    RWRETURN(NULL);
}

/****************************************************************************
 rwDataBlockMgrDestroy

 On entry   : Datablock callback manager
 On exit    : TRUE on success
 */

RwBool 
_rwDataBlockMgrDestroy(RwDataBlockMgr *blockMgr)
{
    RWFUNCTION(RWSTRING("_rwDataBlockMgrDestroy"));
    RWASSERT(blockMgr);

    if (blockMgr->dataBlocks)
    {
        RwDriverFree(blockMgr->dataBlocks);
        blockMgr->dataBlocks = NULL;
    }
    blockMgr->dataBlocksSize = 0;
    blockMgr->dataBlocksLevel = 0;

    if (blockMgr->dataBlockOffsets)
    {
        RwDriverFree(blockMgr->dataBlockOffsets);
        blockMgr->dataBlockOffsets = NULL;
    }
    blockMgr->dataBlockOffsetSize = 0;
    blockMgr->numDataBlockEntries = 0;

    RwDriverFree(blockMgr);

    RWRETURN(TRUE);
}

/****************************************************************************
 rwDataBlockMgrCreateBlock

 On entry   : Datablock callback manager, size, callback
 On exit    : Pointer to datablock on success, NULL on failure
 */

void * 
_rwDataBlockMgrCreateBlock(RwDataBlockMgr *blockMgr, RwUInt32 nSize,
                          RwDataBlockCallBack callBack)
{
    RwUInt32                realSizeWanted;
    RwDataBlockCallBack     *newDataBlock;

    RWFUNCTION(RWSTRING("_rwDataBlockMgrCreateBlock"));
    RWASSERT(blockMgr);
    RWASSERT(callBack);

    /* Adjust for function pointer, and round to nearest 4 byte offset */
    realSizeWanted = (nSize + sizeof(RwDataBlockCallBack) + 3) & ~3;

    /* Make sure data block memory pool is big enough */
    if ((blockMgr->dataBlocksLevel + realSizeWanted) > blockMgr->dataBlocksSize)
    {
        RwUInt8     *newDataBlocks = NULL;
        size_t      memSize = (blockMgr->dataBlocksLevel + realSizeWanted);

        /* Extend memory allocated */
        if (blockMgr->dataBlocks)
        {
            newDataBlocks = (RwUInt8 *)RwDriverRealloc(blockMgr->dataBlocks, memSize,
                   rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT | rwMEMHINTFLAG_RESIZABLE);
        }
        else
        {
            newDataBlocks = (RwUInt8 *)RwDriverMalloc(memSize,
                rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT | rwMEMHINTFLAG_RESIZABLE);
        }

        if (newDataBlocks == NULL)
        {
            RWERROR((E_RW_NOMEM, memSize));
            RWRETURN(NULL);
        }

        blockMgr->dataBlocks = newDataBlocks;
        blockMgr->dataBlocksSize = blockMgr->dataBlocksLevel + realSizeWanted;
    }

    /* Make sure offset array is big enough */
    if (blockMgr->numDataBlockEntries >= blockMgr->dataBlockOffsetSize)
    {
        RwUInt32    *newDataBlockOffsets = NULL;
        size_t      memSize = (blockMgr->dataBlockOffsetSize +
                               rwBLOCKOFFSETGRANULARITY) * sizeof(RwUInt32);

        /* Extend memory allocated */
        if (blockMgr->dataBlockOffsets)
        {
            newDataBlockOffsets = (RwUInt32 *)RwDriverRealloc(blockMgr->dataBlockOffsets, memSize,
                               rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT | rwMEMHINTFLAG_RESIZABLE);
        }
        else
        {
            newDataBlockOffsets = (RwUInt32 *)RwDriverMalloc(memSize,
                rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT | rwMEMHINTFLAG_RESIZABLE);
        }

        if (newDataBlockOffsets == NULL)
        {
            RWERROR((E_RW_NOMEM, memSize));
            RWRETURN(NULL);
        }

        blockMgr->dataBlockOffsets = newDataBlockOffsets;
        blockMgr->dataBlockOffsetSize += rwBLOCKOFFSETGRANULARITY;
    }

    /* We now have all the memory we need, so do the biz */

    blockMgr->dataBlockOffsets[blockMgr->numDataBlockEntries] = blockMgr->dataBlocksLevel;
    newDataBlock = (RwDataBlockCallBack *)(blockMgr->dataBlocks + blockMgr->dataBlocksLevel);
    *newDataBlock++ = callBack;

    blockMgr->numDataBlockEntries++;
    blockMgr->dataBlocksLevel += realSizeWanted;

    RWRETURN((void *)newDataBlock);
}

/****************************************************************************
 rwDataBlockMgrRecallBlocks

 On entry   : Datablock callback manager, direction to flush in
 On exit    : Pointer to datablock callback manager on success
 */

RwDataBlockMgr *
_rwDataBlockMgrRecallBlocks(RwDataBlockMgr *blockMgr, RwRecallDirection direction)
{
    RWFUNCTION(RWSTRING("_rwDataBlockMgrRecallBlocks"));
    RWASSERT(blockMgr);

    /* Call the callback for every data block entry */
    if (direction == rwDIRFIFO)
    {
        RwInt32     dataBlockNum = blockMgr->numDataBlockEntries;
        RwUInt32    *offsets = blockMgr->dataBlockOffsets;

        while (dataBlockNum--)
        {
            RwDataBlockCallBack     *fppFunction;

            fppFunction = (RwDataBlockCallBack *)(blockMgr->dataBlocks + *offsets);
            (*fppFunction)((void *)(fppFunction + 1));

            offsets++;
        }
    }
    else
    {
        RwInt32     dataBlockNum = blockMgr->numDataBlockEntries;
        RwUInt32    *offsets = blockMgr->dataBlockOffsets + (dataBlockNum-1);
        while (dataBlockNum--)
        {
            RwDataBlockCallBack     *fppFunction;

            fppFunction = (RwDataBlockCallBack *)(blockMgr->dataBlocks + *offsets);
            (*fppFunction)((void *)(fppFunction + 1));

            offsets--;
        }
    }

    RWRETURN(blockMgr);
}

RwDataBlockMgr *
_rwDataBlockMgrEmptyBlocks(RwDataBlockMgr *blockMgr)
{
    RWFUNCTION(RWSTRING("_rwDataBlockMgrEmptyBlocks"));

    /* We maintain a peak level over sample sizes of <n> samples.  If the
     * allocated memory is more than 1.5 times this peak, we allocate down
     * to the peak size.
     */

    if (blockMgr->dataBlocksLevel > blockMgr->peakLevel)
    {
        blockMgr->peakLevel = blockMgr->dataBlocksLevel;
    }

    blockMgr->sampleNum = (blockMgr->sampleNum + 1) & 63;

    if (blockMgr->sampleNum == 0)
    {
        /* We must have just wrapped, we can proceed to check memory allocation */
        if (blockMgr->dataBlocksSize > (blockMgr->peakLevel + (blockMgr->peakLevel >> 1)))
        {
            RwUInt8   *newDataBlocks = NULL;

            /* Data block is bigger than 1.5 times peak, so shrink it to peak level */

            if (blockMgr->peakLevel)
            {
                newDataBlocks = (RwUInt8 *)RwDriverRealloc(blockMgr->dataBlocks, blockMgr->peakLevel,
                                   rwID_DRIVERMODULE | rwMEMHINTDUR_EVENT | rwMEMHINTFLAG_RESIZABLE);
                if (newDataBlocks != NULL)
                {
                    blockMgr->dataBlocks = newDataBlocks;
                    blockMgr->dataBlocksSize = blockMgr->peakLevel;
                }
            }
            else
            {
                RwDriverFree(blockMgr->dataBlocks);
                blockMgr->dataBlocks = NULL;
                blockMgr->dataBlocksSize = blockMgr->peakLevel;
            }
        }

        /* Reset stats */
        blockMgr->peakLevel = 0;
    }

    /* Just reset all the pointers */
    blockMgr->dataBlocksLevel = 0;
    blockMgr->numDataBlockEntries = 0;

    RWRETURN(blockMgr);
}
