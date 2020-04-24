/*
 * Defining nodes in custom pipelines
 *
 * Copyright (c) Criterion Software Limited
 */

/****************************************************************************
 *                                                                          *
 *  Module  :   definepipe.c                                                *
 *                                                                          *
 *  Purpose :   Yes, of course it has                                       *
 *                                                                          *
 ****************************************************************************/

/****************************************************************************
 Includes
 */

#include <float.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "batypes.h"
#include "balibtyp.h"
#include "badebug.h"
#include "bamemory.h"

#include "p2core.h"
#include "p2define.h"
#include "p2dep.h"

#include "bapipe.h"

#if defined (__MWERKS__)
#if (defined(RWVERBOSE))
#pragma message (__FILE__ "/" _SKY_EXPAND(__LINE__) ": __MWERKS__ == " _SKY_EXPAND(__MWERKS__))
#endif /* (defined (__MWERKS__)) */
#if (__option (global_optimizer))
#pragma always_inline off
#endif /* (__option (global_optimizer)) */
#endif /*  defined (__MWERKS__) */

/****************************************************************************
 Local Types
 */

typedef struct P2MemoryLimits P2MemoryLimits;
struct P2MemoryLimits
{
    RwUInt8 *StalacTiteBase;
    RwUInt8 *StalacMiteBase;
};

typedef struct tagTopSortData TopSortData;
struct tagTopSortData
{
    RxPipeline         *pipeline;
    RwUInt32            nodesArraySlot;
};

/****************************************************************************
 Local Defines
 */

/* by their NodeRefs shall ye know them */
#define ISNODELIVE(_node) ( NULL != ( (_node)->nodeDef) )

#define ALIGN(n) ( ((n) + 3U) & ~3U )

/* NOTE: we're very careful with parentheses here because
 *       the precendence relationships dictate that:
 * "a ? b : c = d" => "(a ? b : c) = d"
 *       because people will want to assign to/from the
 *       result of the "?:" operator. Also note that:
 * "a ? b = c : d" => "a ? (b = c) : d"
 *        because "b = c" is INSIDE the "?:" operator and
 *        so is correctly turned into a sub-experession */
/* For fixup of [non-NULL] pointers: */
#define _PTRINC(_ptr, _type, _numbytes)                      \
    (((_type *)NULL == (_ptr))?                              \
     ((_type *)NULL):                                        \
     ((_ptr) = (_type *)(((RwUInt8 *)_ptr) + (_numbytes))) )

#define CalcTopSortDataMemSize()                                            \
     /* An array of topSortData structs, one per node */                    \
    ( sizeof(RxPipelineNodeTopSortData)*RXPIPELINEGLOBAL(maxNodesPerPipe) )

/* The memory required for the three conservatively-large arrays allocated
 * in RxPipelineLock() (pipeline nodes, node output indices and topSortData): */
#define CalcNodeOutputsTopSortDataUnCompactedMemSize()                     \
     /* The pipeline nodes array */                                         \
    (sizeof(RxPipelineNode           )*RXPIPELINEGLOBAL(maxNodesPerPipe) +  \
     /* The outputs arrays for each node (max. possible size) */            \
     sizeof(RwUInt32)*RXNODEMAXOUTPUTS*RXPIPELINEGLOBAL(maxNodesPerPipe) +  \
     /* An array of topSortData structs, one per node */                    \
     CalcTopSortDataMemSize()                                              )

/* Temporary memory required by dep-chasing [_PropagateDependenciesAndKillDeadPaths,
 * _ForAllNodeReqsAddOutputClustersAndBuildContinuityBitfields, _TraceClusterScopes
 * and _AssignClusterSlots], a conservative over-estimate: */
#define CalcTempDepChaseMemSize(_pipeParam, _numClustersParam)              \
    /* One rxReq per node */                                                \
    ( _pipeParam->numNodes*sizeof(rxReq) +                                  \
    /* Per rxReq we have an array of numUniqueClusters RwReqEntrys */       \
      _pipeParam->numNodes*_numClustersParam*sizeof(RwReqEntry) +           \
    /* One scope trace per reqentry per node */                             \
      _pipeParam->numNodes*_numClustersParam*sizeof(RwScopeTrace) )         \

#ifdef RWDEBUG
/* Space for trample-detecting magic value and its alignment */
#define tramplingMagicValueSpace (2*sizeof(RwUInt32) - 1)
#else /* RWDEBUG */
#define tramplingMagicValueSpace (0)
#endif /* RWDEBUG */


/****************************************************************************
 Local Variables
 */

/* See RxLockedPipeUnlock,
 * used during unlock for memory allocation - NOT threadsafe! */
static P2MemoryLimits gMemoryLimits = { (RwUInt8 *) NULL, (RwUInt8 *) NULL };


/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Functions

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/* See RxLockedPipeUnlock, used during unlock for memory allocation */
void *
StalacTiteAlloc(RwUInt32 size)
{
    RWFUNCTION(RWSTRING("StalacTiteAlloc"));

    /* Let's keep everything aligned to at least 4 bytes */
    RWASSERT((size & 3) == 0);
    size = (size + 3) & ~3;

    gMemoryLimits.StalacTiteBase -= size;

    RWASSERT(gMemoryLimits.StalacTiteBase >= gMemoryLimits.StalacMiteBase);
    if (gMemoryLimits.StalacTiteBase < gMemoryLimits.StalacMiteBase)
    {
        gMemoryLimits.StalacTiteBase += size;
        RWERROR((E_RW_NOMEM, size));
        RWRETURN(NULL);
    }

    RWRETURN(gMemoryLimits.StalacTiteBase);
}

/* See RxLockedPipeUnlock, used during unlock for memory allocation */
void *
StalacMiteAlloc(RwUInt32 size)
{
    RWFUNCTION(RWSTRING("StalacMiteAlloc"));

    /* Let's keep everything aligned to at least 4 bytes */
    RWASSERT((size & 3) == 0);
    size = (size + 3) & ~3;

    gMemoryLimits.StalacMiteBase += size;

    RWASSERT(gMemoryLimits.StalacMiteBase <= gMemoryLimits.StalacTiteBase);
    if (gMemoryLimits.StalacMiteBase > gMemoryLimits.StalacTiteBase)
    {
        gMemoryLimits.StalacMiteBase -= size;
        RWERROR((E_RW_NOMEM, size));
        RWRETURN(NULL);
    }

    RWRETURN(gMemoryLimits.StalacMiteBase - size);
}

/*****************************************************************************
 PipelineCalcNumUniqueClusters
 */
RwUInt32
PipelineCalcNumUniqueClusters(RxPipeline *pipeline)
{
    RxClusterDefinition *lastAddress, *newAddress;
    RwUInt32             numUniqueClusters;
    RwUInt32             i, j;

    /* To determine the max. possible amount of memory required during
     * dependency chasing, we count the number of unique clusters in the
     * pipeline - the worst case is that every node requires them all. */

    RWFUNCTION(RWSTRING("PipelineCalcNumUniqueClusters"));

    RWASSERT(NULL != pipeline);

    /* Basically, we don't want to allocate any memory  */
    /* so we count up the unique clusters by traversing */
    /* them in memory address order.                    */
    numUniqueClusters = 0;
    newAddress = (RxClusterDefinition *)NULL;
    while (1)
    {
        lastAddress = newAddress;
        newAddress  = (RxClusterDefinition *)0xFFFFFFFF;

        for (i = 0;i < pipeline->numNodes;i++)
        {
            RxNodeDefinition *nodeDef = pipeline->nodes[i].nodeDef;
            for (j = 0;j < nodeDef->io.numClustersOfInterest;j++)
            {
                RxClusterDefinition *address;
                address = nodeDef->io.clustersOfInterest[j].clusterDef;
                if ((address > lastAddress) &&
                    (address < newAddress ) )
                {
                    newAddress = address;
                }
            }
        }

        /* This will correctly deal with pipes with */
        /* no nodes and/or no clusters of interest. */
        if (newAddress == (RxClusterDefinition *)0xFFFFFFFF) break;
        numUniqueClusters++;
    }

    RWRETURN(numUniqueClusters);
}

/*****************************************************************************
 ReallocAndFixupSuperBlock
 */
static RwBool
ReallocAndFixupSuperBlock(RxPipeline *pipeline, RwUInt32 newSize)
{
    void    *oldBlock, *newBlock;
    RwUInt32 i;

    /* Reallocing the superblock isn't completely trivial, since it
     * contains internal cross-references :o/ */
    RWFUNCTION(RWSTRING("ReallocAndFixupSuperBlock"));

    /* NOTE:
     * People decide whether to do this realloc before calling this function.
     * Sometimes they want to grow only, sometimes they want to shrink.
     * So we just do as we're told here. */
    oldBlock = pipeline->superBlock;
    newBlock = RwRealloc(oldBlock, newSize,
        rwMEMHINTDUR_GLOBAL | rwMEMHINTFLAG_RESIZABLE | rwID_PIPEMODULE);
    if (NULL != newBlock)
    {
        const RwUInt32 numNodes = pipeline->numNodes;
        RwInt32 diff;

        diff = ((RwUInt8 *)newBlock) - ((RwUInt8 *)oldBlock);

        /* Fix up per-pipeline pointers */
        pipeline->superBlock = newBlock;
        pipeline->superBlockSize = newSize;
        pipeline->nodes = (RxPipelineNode *)pipeline->superBlock;
        _PTRINC(pipeline->embeddedPacket, RxPacket, diff);
        _PTRINC(pipeline->inputRequirements,
                RxPipelineRequiresCluster, diff);

        /* Fix up per-pipelinenode pointers */
        for (i = 0; i < numNodes; i++)
        {
            _PTRINC(pipeline->nodes[i].outputs, RwUInt32, diff);
            _PTRINC(pipeline->nodes[i].slotClusterRefs,
                    RxPipelineCluster *, diff);
            _PTRINC(pipeline->nodes[i].slotsContinue, RwUInt32, diff);
            _PTRINC(pipeline->nodes[i].privateData, void, diff);
            _PTRINC(pipeline->nodes[i].inputToClusterSlot, RwUInt32, diff);
            _PTRINC(pipeline->nodes[i].topSortData,
                    RxPipelineNodeTopSortData, diff);
        }
    }
    else
    {
        RWERROR((E_RW_NOMEM, newSize));
        RWRETURN(FALSE);
    }

    RWRETURN(TRUE);
}

/*****************************************************************************
 LockPipelineExpandData
 */
static RwBool
LockPipelineExpandData(RxPipeline *dstPipe, RxPipeline *srcPipe)
{
    RxPipelineNodeTopSortData *topSortArray;
    RwUInt32   *outputs;
    RwInt32     i;

    RWFUNCTION(RWSTRING("LockPipelineExpandData"));

    /* RxPipelineLock() expands the pipelineNodes and outputs arrays
     * to conservative sizes and creates a topSortData array too.
     * This is also used by RxPipelineClone, to copy the relevant
     * bits of an unlocked pipeline into a locked clone of it. */

    if (dstPipe != srcPipe)
    {
        /* We're cloning an unlocked pipe's nodes into a locked pipe. */
        /* First make sure the locked pipe's been set up right        */
        RWASSERT(FALSE != dstPipe->locked);
        RWASSERT(NULL  != dstPipe->superBlock);
        RWASSERT(dstPipe->superBlockSize >=
                 CalcNodeOutputsTopSortDataUnCompactedMemSize());
        RWASSERT(dstPipe->nodes == dstPipe->superBlock);
        RWASSERT(0 == dstPipe->numNodes);
        RWASSERT(FALSE == srcPipe->locked);

        /* Then copy the relevant bits of the pipeline nodes across */
        for (i = srcPipe->numNodes - 1;i >= 0;i--)
        {
            memcpy(&(dstPipe->nodes[i]),
                   &(srcPipe->nodes[i]),
                   sizeof(RxPipelineNode));

            /* There are some things that a locked   */
            /* pipe just doesn't need to know about! */
            dstPipe->nodes[i].slotClusterRefs = (RxPipelineCluster **)NULL;
            dstPipe->nodes[i].slotsContinue = (RwUInt32 *)NULL;
            dstPipe->nodes[i].privateData = NULL;
            dstPipe->nodes[i].inputToClusterSlot = (RwUInt32 *)NULL;

            /* DO copy initializationData though, m'bebbeh.
             * it's the heart and soul of cloning, man.
             * NOTE: if there are internal references within the
             *      initData, this'll screw it up... cloning sucks. */
            if (0 < dstPipe->nodes[i].initializationDataSize)
            {
                dstPipe->nodes[i].initializationData =
                    RwMalloc(dstPipe->nodes[i].initializationDataSize,
                         rwMEMHINTDUR_EVENT | rwID_PIPEMODULE);
                if (NULL == dstPipe->nodes[i].initializationData)
                {
                    RWERROR((E_RW_NOMEM,
                        dstPipe->nodes[i].initializationDataSize));
                    RWRETURN(FALSE);
                }
                memcpy(dstPipe->nodes[i].initializationData,
                       srcPipe->nodes[i].initializationData,
                       dstPipe->nodes[i].initializationDataSize);
            }
        }
        /* _NodeCreate() incs numNodes, but we copied nodes directly, so: */
        dstPipe->numNodes = srcPipe->numNodes;
    }

    outputs = (RwUInt32 *)
        &(dstPipe->nodes[RXPIPELINEGLOBAL(maxNodesPerPipe)]);
    /* outputs get packed to follow on from the pipeline nodes  */
    /* array at the end of unlock, so if we work from the end   */
    /* backwards we'll avoid overwriting as we go.              */
    for (i = srcPipe->numNodes - 1;i >= 0;i--)
    {
        /* We copy more than we need to, it matters not.    */
        /* [i.e. the extra entries will never be read from] */
        dstPipe->nodes[i].outputs = &(outputs[i*RXNODEMAXOUTPUTS]);
        if (NULL != srcPipe->nodes[i].outputs)
        {
            memcpy(dstPipe->nodes[i].outputs,
                   srcPipe->nodes[i].outputs,
                   RXNODEMAXOUTPUTS*sizeof(RwUInt32));
        }
        else
        {
            RWASSERT(0 == dstPipe->nodes[i].numOutputs);
        }
    }

    /* Create topSortData for the existing nodes */
    topSortArray = (RxPipelineNodeTopSortData *)
        &(outputs[RXNODEMAXOUTPUTS*RXPIPELINEGLOBAL(maxNodesPerPipe)]);
    for (i = 0;(RwUInt32)i < srcPipe->numNodes;i++)
    {
        topSortArray[i].numIns = 0;
        topSortArray[i].numInsVisited = 0;
        topSortArray[i].req = (rxReq *)NULL;
        dstPipe->nodes[i].topSortData = &(topSortArray[i]);
    }

    RWRETURN(TRUE);
}

/*****************************************************************************
 CalcNodesOutputsCompactedMemSize
 */
static RwUInt32
CalcNodesOutputsCompactedMemSize(RxPipeline *pipeline)
{
    RwUInt32 size, i;

    /* Calc the memory required for the pipeline nodes array and the output
     * indices of its nodes, once compacted in RxLockedPipeUnlock(): */

    RWFUNCTION(RWSTRING("CalcNodesOutputsCompactedMemSize"));

    size = pipeline->numNodes*sizeof(RxPipelineNode);
    for (i = 0;i < pipeline->numNodes;i++)
    {
        size += pipeline->nodes[0].numOutputs*sizeof(RwUInt32);
    }

    RWRETURN(size);
}

/*****************************************************************************
 CalcUnlockPersistentMemSize
 */
static RwUInt32
CalcUnlockPersistentMemSize(RxPipeline *pipeline, RwUInt32 numClusters)
{
    RwUInt32        blockSize = 0;
    RxPipelineNode *node;
    RwUInt32        i;

    /* Calc the amount of memory required by _ForAllNodesWriteClusterAllocations
     * for all memory which will persist after unlock (doesn't count the
     * compacted pipeline nodes and node output indices arrays): */

    RWFUNCTION(RWSTRING("CalcUnlockPersistentMemSize"));


    /* The RxPipelineClusters (n separate allocs but they end up in an */
    /* array anyway - see _MyEnumPipelineClustersCallBack())           */
    blockSize += numClusters*sizeof(RxPipelineCluster);
    /* The input requirements of the *pipeline* */
    blockSize += numClusters*sizeof(RxPipelineRequiresCluster);
    /* An array of pointers to the appropriate RxPipelineCluster */
    /* for each cluster (used by the packet)                     */
    blockSize += pipeline->numNodes*numClusters*sizeof(RxPipelineCluster *);
    /* A bit-field per cluster for which outputs it continues through   */
    /* [the first entry is the AND of all subsequent ones and is used  */
    /*  to optimise EMBEDDEDPACKETBETWEENNODESMACRO() - if a given bit */
    /*  (output) is '1' then ALL clusters persist through that output] */
    blockSize += pipeline->numNodes*(numClusters + 1)*sizeof(RwUInt32);

    /* Sum up private data size and the lengths */
    /* of the inputToClusterSlot arrays         */
    for (i = 0;i < pipeline->numNodes;i++)
    {
        node = &(pipeline->nodes[i]);
        if (node->nodeDef->pipelineNodePrivateDataSize != 0)
        {
            blockSize += tramplingMagicValueSpace +
                         node->nodeDef->pipelineNodePrivateDataSize;
        }
        blockSize += sizeof(RwUInt32)*node->nodeDef->io.numClustersOfInterest;
    }

    /* The 'embedded' packet and its cluster array */
    blockSize += sizeof(RxPacket) + sizeof(RxCluster)*(numClusters - 1);

    /* C'est fait */
    RWRETURN(blockSize);
}

/*****************************************************************************
 _NodeClone

 called from: RxPipelineNodeRequestCluster() and RxPipelineNodeReplaceCluster()
 This clones a node's noderef (for subsequent modification) and sets the node
 to point at the clone if successful.
*/
static RxNodeDefinition *
_NodeClone(RxPipelineNode *node, RxClusterDefinition *cluster2add)
{
    /* optional param.; clone()
     * function doubles as clone&growclusterlist() */
    /*
     * damn, but a node is a complex beast
     * (coz of all the variable size components)
     */

    RxNodeDefinition *result = (RxNodeDefinition *)NULL;
    RxNodeDefinition *source;
    RwUInt32          i, size, destnumcli;
    RwUInt8          *block;

    RWFUNCTION(RWSTRING("_NodeClone"));

    RWASSERT(node);
    source = node->nodeDef;
    RWASSERT(source);

    destnumcli = ((cluster2add == NULL) ?
                  source->io.numClustersOfInterest :
                  source->io.numClustersOfInterest + 1);

    size = (ALIGN(sizeof(RxNodeDefinition)) +
            ALIGN(strlen(source->name) + 1) +
            ALIGN(destnumcli * sizeof(RxClusterRef)));

    size += (ALIGN(destnumcli * sizeof(RxClusterValidityReq)) +
             ALIGN(source->io.numOutputs * sizeof(RxOutputSpec)));

    for (i = 0; i < source->io.numOutputs; i++)
    {
        RxOutputSpec       *outputspec = &source->io.outputs[i];

        size += ALIGN(strlen(outputspec->name) + 1);
        size += ALIGN(destnumcli * sizeof(RxClusterValid));
    }

    block = (RwUInt8 *) RwMalloc(size,
        rwMEMHINTDUR_EVENT | rwID_PIPEMODULE);
    /* Make sure it's aligned enough */
    RWASSERT(0 == (((RwUInt32)block) & 3));
    if (block != NULL)
    {
        /* node */
        result = (RxNodeDefinition *)block;
        block += ALIGN(sizeof(RxNodeDefinition));

        /* name */
        result->name = (char *) block;
        block += ALIGN(strlen(source->name) + 1);
        strcpy((char *) (result->name), (const char *) (source->name));

        /* nodemethods */
        result->nodeMethods = source->nodeMethods;

        /* numclustersofinterest */
        result->io.numClustersOfInterest = destnumcli;

        /* clustersofinterest */
        result->io.clustersOfInterest = (RxClusterRef *) block;
        block += ALIGN(destnumcli * sizeof(RxClusterRef));

        for (i = 0; i < source->io.numClustersOfInterest; i++)
        {
            /* forcepresent */
            result->io.clustersOfInterest[i].forcePresent =
                source->io.clustersOfInterest[i].forcePresent;

            /* clusterdef */
            result->io.clustersOfInterest[i].clusterDef =
                source->io.clustersOfInterest[i].clusterDef;

            /* reserved */
            result->io.clustersOfInterest[i].reserved =
                source->io.clustersOfInterest[i].reserved;
        }

        if (cluster2add != NULL)
        {
            /* forcepresent - we're being called by the app to add this
             * cluster, but we give them more debugging feedback (if no
             * node ends up using the cluster) if we *don't* set
             * forcepresent (it fails later on cos the cluster
             * disappears) and if they really need FORCEPRESENT then
             * they can do it themselves */
            result->io.clustersOfInterest[i].forcePresent =
                (RxClusterForcePresent)FALSE;

            /* clusterdef */
            result->io.clustersOfInterest[i].clusterDef = cluster2add;

            /* reserved */
            result->io.clustersOfInterest[i].reserved = 0;
        }

        /* inputrequirements */
        result->io.inputRequirements = (RxClusterValidityReq *) block;
        block += ALIGN(destnumcli * sizeof(RxClusterValidityReq));
        memcpy(result->io.inputRequirements,
               source->io.inputRequirements,
               source->io.numClustersOfInterest *
               sizeof(RxClusterValidityReq));

        if (cluster2add != NULL)
        {
            result->io.inputRequirements[destnumcli - 1] =
                rxCLREQ_DONTWANT;
        }

        /* numoutputs */
        result->io.numOutputs = source->io.numOutputs;

        /* outputs */
        result->io.outputs = (RxOutputSpec *) block;
        block += ALIGN(source->io.numOutputs * sizeof(RxOutputSpec));

        for (i = 0; i < source->io.numOutputs; i++)
        {
            /* name */
            result->io.outputs[i].name = (char *) block;
            block += ALIGN(strlen(source->io.outputs[i].name) + 1);
            strcpy((char *) result->io.outputs[i].name,
                   (const char *) (source->io.outputs[i].name));

            /* outputclusters */
            result->io.outputs[i].outputClusters =
                (RxClusterValid *) block;
            block += ALIGN(destnumcli * sizeof(RxClusterValid));
            memcpy(result->io.outputs[i].outputClusters,
                   source->io.outputs[i].outputClusters,
                   source->io.numClustersOfInterest *
                   sizeof(RxClusterValid));

            if (cluster2add != NULL)
            {
                result->io.outputs[i].outputClusters[destnumcli - 1] =
                    source->io.outputs[i].allOtherClusters;
            }

            /* allotherclusters */
            result->io.outputs[i].allOtherClusters =
                source->io.outputs[i].allOtherClusters;
        }

        /* pipelinenodeprivatedatasize */
        result->pipelineNodePrivateDataSize =
            source->pipelineNodePrivateDataSize;

        /* editable */
        result->editable = (RxNodeDefEditable)TRUE;

        /* While a pipeline is locked, its nodes do not count as referencing
         * node defs, so we initialise the new node def to have no references
         * and we leave the old node def alone
         * (it's count will have been decremented by RxPipelineLock() anyway),
         * unless it's editable.*/
        result->InputPipesCnt = 0;

        /* In this case kill the source nodeDef if editable, cos it must have
         * been created during this pipeline-editing session and you cannot
         * (currently, and this mustn't change until the whole reference-counting
         * scheme sorts itself out):
         * (a) clone a locked pipeline or
         * (b) reference an editable node def during editing without cloning
         *    a pipe (i.e you can't reference it in two places within the
         *    same pipe - if you do, you're doing it behind our backs, which
         *    is illegal)
         * (c) add/replace a node's cluster in an unlocked pipe
         */
        RWASSERT(0 <= source->InputPipesCnt);
        if ((source->InputPipesCnt == 0) && (source->editable))
        {
            RwFree(source);
        }
        node->nodeDef = result;
    }
    else
    {
        RWERROR((E_RW_NOMEM, size));
    }

    RWRETURN(result);
}

/****************************************************************************
 IoSpecSearchForCluster()
 */
static              RwUInt32
IoSpecSearchForCluster(RxIoSpec * iospec,
                       RxClusterDefinition * clusterDef)
{
    RwUInt32            result = (RwUInt32)-1;
    RwUInt32            n;

    RWFUNCTION(RWSTRING("IoSpecSearchForCluster"));

    for (n = 0; n < iospec->numClustersOfInterest; n++)
    {
        if (iospec->clustersOfInterest[n].clusterDef == clusterDef)
        {                      /* success */
            result = n;
            break;
        }
    }

    RWRETURN(result);
}

/*****************************************************************************
 _RwNodeCreate
 - called from RxLockedPipeAddFragment(), RxLockedPipeReplaceNode()
*/
static              RwBool
_NodeCreate(RxPipeline * pipeline,
            RxPipelineNode * node, RxNodeDefinition * nodespec)
{
    RxPipelineNodeTopSortData *topSortData;
    RwUInt32           *outputs;
    RwBool              result = TRUE;
    RwUInt32            n;

    RWFUNCTION(RWSTRING("_NodeCreate"));
    RWASSERT(NULL != pipeline);
    RWASSERT(FALSE != pipeline->locked);
    RWASSERT(NULL != node);
    RWASSERT(NULL != nodespec);

    n = nodespec->io.numOutputs;

    memset(node, 0x00U, sizeof(RxPipelineNode));

    if (n > RXNODEMAXOUTPUTS)
    {
        RWERROR((E_RX_NODETOOMANYOUTPUTS));
        result = FALSE;
    }

    if (nodespec->io.numClustersOfInterest > RXNODEMAXCLUSTERSOFINTEREST)
    {
        RWERROR((E_RX_NODETOOMANYCLUSTERSOFINTEREST));
        result = FALSE;
    }

    if (RXPIPELINEGLOBAL(maxNodesPerPipe) !=
        (RwUInt32) _rxPipelineMaxNodes)
    {
        RwDebugSendMessage(rwDEBUGMESSAGE,
                           RWSTRING("_NodeCreate"),
                           RWSTRING("_rxPipelineMaxNodes should only be changed BEFORE initializing RenderWare!"));
    }

    if (n >= RXPIPELINEGLOBAL(maxNodesPerPipe))
    {
        RWERROR((E_RX_PIPELINETOOMANYNODES));
        result = FALSE;
    }

    if (FALSE != result)
    {
        /* Find the first free empty slot in our conservatively-
         * large outputs array [see RxPipelineLock()] */
        outputs = (RwUInt32 *)
            & (pipeline->nodes[RXPIPELINEGLOBAL(maxNodesPerPipe)]);
        outputs += pipeline->numNodes * RXNODEMAXOUTPUTS;

        /* Make sure we're where we expect to be */
        RWASSERT(node == &(pipeline->nodes[pipeline->numNodes]));

        node->outputs = outputs;
        node->numOutputs = n;

        for (n = 0; n < node->numOutputs; n++)
        {
            *outputs = (RwUInt32) - 1;
            outputs++;
        }

        /* Find the first free empty slot in our conservatively-
         * large topSortData array [see RxPipelineLock()] */
        outputs = (RwUInt32 *)
            & (pipeline->nodes[RXPIPELINEGLOBAL(maxNodesPerPipe)]);
        topSortData = (RxPipelineNodeTopSortData *)
            & (outputs[RXPIPELINEGLOBAL(maxNodesPerPipe) * RXNODEMAXOUTPUTS]);
        topSortData += pipeline->numNodes;

        topSortData->numIns = 0;
        topSortData->numInsVisited = 0;
        topSortData->req = (rxReq *) NULL;
        node->topSortData = topSortData;

        node->initializationData = NULL;
        node->initializationDataSize = 0;

        node->nodeDef = nodespec; /* ISNODELIVE(node)? YES! */

        /* Only this and _NodeCreate() should ever modify numNodes
         * (okay, so RxPipelineClone() might do as well) */
        pipeline->numNodes++;
    }

    RWRETURN(result);
}

/*****************************************************************************
 PipelineTallyInputs
 - called from RxLockedPipeUnlock()
*/
static void
PipelineTallyInputs(RxPipeline * pipeline)
{
    RxPipelineNode     *nodes;
    RwUInt32            i;

    RWFUNCTION(RWSTRING("PipelineTallyInputs"));

    nodes = &pipeline->nodes[0];
    for (i = 0; i < pipeline->numNodes; i++)
    {
        if (ISNODELIVE(nodes))
        {
            /* clear this down prior to _TopSort() */
            nodes->topSortData->numInsVisited = 0;

            /* cleared for below */
            nodes->topSortData->numIns = 0;
        }
        nodes++;
    }

    nodes = &pipeline->nodes[0];
    for (i = 0; i < pipeline->numNodes; i++)
    {
        if (ISNODELIVE(nodes))
        {
            if (nodes->numOutputs != 0)
            {
                int                 j = nodes->numOutputs;
                RwUInt32           *outputs = &nodes->outputs[0];

                do
                {              /* loop over node outputs */
                    if (*(RwInt32 *) outputs != -1)
                    {
                        pipeline->nodes[*outputs].topSortData->numIns++;
                    }
                }
                while (outputs++, --j);
            }
        }
        nodes++;
    }

    RWRETURNVOID();
}

/*****************************************************************************
 PipelineTopSort
 - called from RxLockedPipeUnlock() (& from self!)
*/
static void                /* RECURSIVE */
PipelineTopSort(TopSortData *data, RwUInt32 nodeIndex)
{
    RxPipelineNode *curNode;
    RwUInt32 i = data->nodesArraySlot;
    RwUInt32 j = nodeIndex;

    RWFUNCTION(RWSTRING("PipelineTopSort"));

    /* Move this node to its final destination in the post-sort array.
     * To do this, we swap two nodes, and their outputs arrays and
     * topSortData, in the conservatively large, uniform arrays
     * created and described in RxPipelineLock() */
    if (i != j)
    {
        RwUInt32                   tmpOutput;
        RwUInt32                  *outputsI,     *outputsJ;
        RxPipelineNodeTopSortData  tempTopSortData;
        RxPipelineNodeTopSortData *topSortDataI, *topSortDataJ;
        RxPipelineNode             tmpNode;
        RwUInt32                   k, l;

        /* Exchange outputs arrays */
        outputsI = data->pipeline->nodes[i].outputs;
        outputsJ = data->pipeline->nodes[j].outputs;
        for (k = 0;k < RXNODEMAXOUTPUTS;k++)
        {
            tmpOutput   = outputsI[k];
            outputsI[k] = outputsJ[k];
            outputsJ[k] = tmpOutput;
        }
        /* Fix up outputs array pointers */
        data->pipeline->nodes[i].outputs = outputsJ;
        data->pipeline->nodes[j].outputs = outputsI;


        /* Exchange topSortData for this node */
        topSortDataI = data->pipeline->nodes[i].topSortData;
        topSortDataJ = data->pipeline->nodes[j].topSortData;
        tempTopSortData = *topSortDataI;
       *topSortDataI    = *topSortDataJ;
       *topSortDataJ    =  tempTopSortData;
        /* Fix up topSortData pointers */
        data->pipeline->nodes[i].topSortData = topSortDataJ;
        data->pipeline->nodes[j].topSortData = topSortDataI;


        /* Exchange node data */
        tmpNode = data->pipeline->nodes[i];
        data->pipeline->nodes[i] = data->pipeline->nodes[j];
        data->pipeline->nodes[j] = tmpNode;
        /* Fix up output indices */
        for (k = 0;k < data->pipeline->numNodes;k++)
        {
            RxPipelineNode *node = &(data->pipeline->nodes[k]);
            for (l = 0;l < node->numOutputs;l++)
            {
                if (node->outputs[l] == i)
                {
                    node->outputs[l] = j;
                }
                else
                if (node->outputs[l] == j)
                {
                    node->outputs[l] = i;
                }
            }
        }
    }

    curNode = &(data->pipeline->nodes[data->nodesArraySlot]);
    data->nodesArraySlot++;

    if (curNode->numOutputs != 0)
    {
        for (i = 0;i < curNode->numOutputs;i++)
        {
            RwUInt32 outIndex = curNode->outputs[i];

            if (outIndex != (RwUInt32)-1)
            {
                RxPipelineNode *outNode = &(data->pipeline->nodes[outIndex]);

                /* If all of this output node's inputs have been processed
                 * (including the current node, right now) then traverse
                 * down, that node can be next in the post-sort array */
                outNode->topSortData->numInsVisited++;
                if (outNode->topSortData->numIns ==
                    outNode->topSortData->numInsVisited)
                {
                    PipelineTopSort(data, outIndex);
                }
            }
        }
    }

    RWRETURNVOID();
}

/*****************************************************************************
 PipelineNode2Index
 - called from RxLockedPipeDeleteNode(), RxLockedPipeSetEntryPoint(), RxLockedPipeAddPath()
*/
static RwUInt32
PipelineNode2Index(RxPipeline *pipeline, RxPipelineNode *node)
{
    RwUInt32 nodeIndex = node - pipeline->nodes;

    RWFUNCTION(RWSTRING("PipelineNode2Index"));

    /* sanity check: does node fall within this pipeline's Nodes array ? */
    RWASSERT(&(pipeline->nodes[nodeIndex]) == node);
    RWASSERT(nodeIndex < pipeline->numNodes);

    if ((&(pipeline->nodes[nodeIndex]) == node) &&
        (nodeIndex < pipeline->numNodes))
    {
        RWRETURN(nodeIndex);
    }

    RWRETURN((RwUInt32)-1);
}

/*****************************************************************************/
static RxPipeline *
PipelineUnlockTopSort(RxPipeline *pipeline)
{
    TopSortData data;
    RwUInt32    i;

    RWFUNCTION(RWSTRING("PipelineUnlockTopSort"));
    RWASSERT(pipeline != NULL);

    data.pipeline = pipeline;
    data.nodesArraySlot = 0;

    /* Add up inputs for each node by enumerating outputs in all nodes */
    PipelineTallyInputs(pipeline);

    /* Does the entry-point has zero in-degrees? */
    if (pipeline->nodes[pipeline->entryPoint].topSortData->numIns != 0)
    {
        /* entry point is invalid;
         * fail (pipeline remains Lock()ed) */
        RWERROR((E_RX_INVALIDENTRYPOINT));
        RWRETURN((RxPipeline *)NULL);
    }

    /* Do all nodes form one connected graph? */
    for (i = 0; i < pipeline->numNodes; i++)
    {
        if ((i != pipeline->entryPoint) &&
            (pipeline->nodes[i].topSortData->numIns == 0))
        {
            /* the pipeline is fragmented;
             * fail (pipeline remains Lock()ed) */
            RWERROR((E_RX_FRAGMENTEDPIPELINE));
            RWRETURN((RxPipeline *)NULL);
        }
    }

    /* Sort nodes so that dependencies always go in the same direction
     * (we put the user-defined entrypoint at the start - I don't think
     * this can ever be different to the one we'd calculate though.
     * If people want to enter at the 2nd node in a linear chain,
     * topsort will just fail, won't it ?).
     * NOTE: this updates cross-references as it sorts, so even if we
     *      fail it "preserves the integrity of the graph, boyo" */
    PipelineTopSort(&data, pipeline->entryPoint);

    /* cycle-free graph? */
    for (i = 0; i < pipeline->numNodes; i++)
    {
        /* If a cycle is present, not all the nodes will be visited
         * along all their inputs during the PipelineTopSort */
        if (pipeline->nodes[i].topSortData->numIns !=
            pipeline->nodes[i].topSortData->numInsVisited)
        {
            /* pipeline had cycles;
             * fail (pipeline remains Lock()ed, partially sorted) */
            RWERROR((E_RX_CYCLICPIPELINE));
            RWRETURN((RxPipeline *)NULL);
        }
    }

    /* Paranoia (above error-checking should have caught this) */
    RWASSERT(pipeline->numNodes == data.nodesArraySlot);

    pipeline->entryPoint = 0;

    RWRETURN(pipeline);
}

/**
 * \ingroup rwcoregeneric
 * \ref RxPipelineNodeCloneDefinition
 * creates a private copy of the \ref RxNodeDefinition
 * of a given an \ref RxPipelineNode, so that the
 * definition can be modified. It optionally adds a new cluster_of_interest.
 *
 * The same cluster may not appear twice in a node's clusters_of_interest
 * array, so it is an error to add a cluster that is already present.
 *
 * This function is generally used by nodes to assist in the provision of
 * construction-time "customization" APIs.
 *
 * \param node  A pointer to the source pipeline node
 * \param cluster2add  An optional pointer to
 * a cluster definition to add
 * \return A pointer to the clone node definition
 *
 * \see RxPipelineNodeFindInput
 * \see RxPipelineNodeFindOutputByIndex
 * \see RxPipelineNodeFindOutputByName
 * \see RxPipelineNodeRequestCluster
 * \see RxPipelineNodeReplaceCluster
 * \see RxLockedPipeAddFragment
 * \see RxLockedPipeAddPath
 * \see RxPipelineClone
 * \see RxPipelineInsertDebugNode
 * \see RxLockedPipeDeleteNode
 * \see RxLockedPipeDeletePath
 * \see RxPipelineFindNodeByIndex
 * \see RxPipelineFindNodeByName
 * \see RxLockedPipeGetEntryPoint
 * \see RxPipelineLock
 * \see RxLockedPipeReplaceNode
 * \see RxLockedPipeSetEntryPoint
 * \see RxLockedPipeUnlock
 */
RxNodeDefinition   *
RxPipelineNodeCloneDefinition(RxPipelineNode * node,
                              RxClusterDefinition * cluster2add)
{
    /* API-grade wrapper for internal function _NodeClone() */

    RxNodeDefinition   *result = (RxNodeDefinition *)NULL;

    RWAPIFUNCTION(RWSTRING("RxPipelineNodeCloneDefinition"));

    RWASSERT(RxPipelineInstanced);
    RWASSERT(node != NULL);

    if (node != NULL)
    {
        if (cluster2add != NULL)
        {
            RwUInt32            clusterindex;
            RwBool              clusteralreadypresent;

            clusterindex = IoSpecSearchForCluster(&node->nodeDef->io,
                                                  cluster2add);

            clusteralreadypresent = (clusterindex != ((RwUInt32)-1));

            RWASSERT(!clusteralreadypresent);

            if (clusteralreadypresent)
            {
                goto Exit;
            }
        }

        result = _NodeClone(node, cluster2add);
    }

  Exit:
    RWRETURN(result);
}

/**
 * \ingroup rwcoregeneric
 * \ref RxPipelineNodeFindOutputByName
 * searches through the node's outputs,
 * as defined in its struct RxNodeDefinition, returning a handle to the
 * requested output - requested with a string in the \ref RxNodeDefinition .
 * This handle can be used with \ref RxLockedPipeAddPath, etc.
 *
 * \param node  A pointer to a pipeline node
 * \param outputname  A pointer to a string identifying an output of the node
 *
 * \return An \ref RxNodeOutput handle for the output on success,
 * or NULL if there is an error.
 *
 * \see RxPipelineNodeCloneDefinition
 * \see RxPipelineNodeFindInput
 * \see RxPipelineNodeFindOutputByIndex
 * \see RxPipelineNodeReplaceCluster
 * \see RxPipelineNodeRequestCluster
 * \see RxLockedPipeAddFragment
 * \see RxLockedPipeAddPath
 * \see RxPipelineClone
 * \see RxPipelineInsertDebugNode
 * \see RxLockedPipeDeleteNode
 * \see RxLockedPipeDeletePath
 * \see RxPipelineFindNodeByIndex
 * \see RxPipelineFindNodeByName
 * \see RxLockedPipeGetEntryPoint
 * \see RxPipelineLock
 * \see RxLockedPipeReplaceNode
 * \see RxLockedPipeSetEntryPoint
 * \see RxLockedPipeUnlock
 */
RwUInt32           *
RxPipelineNodeFindOutputByName(RxPipelineNode * node,
                               const RwChar *outputname)
{
    RWAPIFUNCTION(RWSTRING("RxPipelineNodeFindOutputByName"));

    RWASSERT(RxPipelineInstanced);
    RWASSERT(node != NULL);
    RWASSERT(ISNODELIVE(node));
    RWASSERT(outputname != NULL);

    if (node != NULL && ISNODELIVE(node) && outputname != NULL)
    {
        int                 n;
        int                 numouts = node->nodeDef->io.numOutputs;
        RxOutputSpec       *out = node->nodeDef->io.outputs;

        for (n = 0, out = node->nodeDef->io.outputs;
             n < numouts; n++, out++)
        {
            if (rwstrcmp(out->name, outputname) == 0)
            {
                RWRETURN(&node->outputs[n]);
            }
        }
    }

    RWRETURN((RwUInt32 *) NULL);
}

/**
 * \ingroup rwcoregeneric
 * \ref RxPipelineNodeFindOutputByIndex
 * searches through the node's
 * outputs, as defined in its \ref RxNodeDefinition, returning a handle
 * to the requested output (requested with an index into the node's array
 * of outputs, in the order defined in the node's \ref RxNodeDefinition).
 * This handle can be used with \ref RxLockedPipeAddPath, etc.
 *
 * \param node  A pointer to a pipeline node
 * \param outputindex  The index of the output to retrieve
 *
 * \return An \ref RxNodeOutput handle for the output on success, or
 * NULL if there is an error
 *
 * \see RxPipelineNodeCloneDefinition
 * \see RxPipelineNodeFindInput
 * \see RxPipelineNodeFindOutputByName
 * \see RxPipelineNodeReplaceCluster
 * \see RxPipelineNodeRequestCluster
 * \see RxLockedPipeAddFragment
 * \see RxLockedPipeAddPath
 * \see RxPipelineClone
 * \see RxPipelineInsertDebugNode
 * \see RxLockedPipeDeleteNode
 * \see RxLockedPipeDeletePath
 * \see RxPipelineFindNodeByIndex
 * \see RxPipelineFindNodeByName
 * \see RxLockedPipeGetEntryPoint
 * \see RxPipelineLock
 * \see RxLockedPipeReplaceNode
 * \see RxLockedPipeSetEntryPoint
 * \see RxLockedPipeUnlock
 */
RwUInt32           *
RxPipelineNodeFindOutputByIndex(RxPipelineNode * node,
                                RwUInt32 outputindex)
{
    RWAPIFUNCTION(RWSTRING("RxPipelineNodeFindOutputByIndex"));

    RWASSERT(RxPipelineInstanced);
    RWASSERT(NULL != node);
    RWASSERT(ISNODELIVE(node));
    RWASSERT(outputindex < node->numOutputs);

    if ((NULL != node) && ISNODELIVE(node) &&
        (outputindex < node->numOutputs))
    {
        RWRETURN(&node->outputs[outputindex]);
    }

    RWRETURN((RwUInt32 *) NULL);
}

/**
 * \ingroup rwcoregeneric
 * \ref RxPipelineNodeFindInput
 * returns a handle to the node's input,
 * suitable for use with \ref RxLockedPipeAddPath, etc.
 *
 * \param node  A pointer to a pipeline node
 *
 * \return An \ref RxNodeInput handle for the node's input on success,
 * or NULL if there is an error.
 *
 * \see RxPipelineNodeCloneDefinition
 * \see RxPipelineNodeFindOutputByIndex
 * \see RxPipelineNodeFindOutputByName
 * \see RxPipelineNodeReplaceCluster
 * \see RxPipelineNodeRequestCluster
 * \see RxLockedPipeAddFragment
 * \see RxLockedPipeAddPath
 * \see RxPipelineClone
 * \see RxPipelineInsertDebugNode
 * \see RxLockedPipeDeleteNode
 * \see RxLockedPipeDeletePath
 * \see RxPipelineFindNodeByIndex
 * \see RxPipelineFindNodeByName
 * \see RxLockedPipeGetEntryPoint
 * \see RxPipelineLock
 * \see RxLockedPipeReplaceNode
 * \see RxLockedPipeSetEntryPoint
 * \see RxLockedPipeUnlock
 */
RxPipelineNode     *
RxPipelineNodeFindInput(RxPipelineNode * node)
{
    RWAPIFUNCTION(RWSTRING("RxPipelineNodeFindInput"));

    RWASSERT(RxPipelineInstanced);
    RWASSERT(node != NULL);
    RWASSERT(ISNODELIVE(node));

    if (node != NULL && ISNODELIVE(node))
    {
        RWRETURN(node);
    }

    RWRETURN((RxPipelineNode *) NULL);
}

/**
 * \ingroup rwcoregeneric
 * \ref RxPipelineNodeRequestCluster
 * causes a node to request a cluster.
 *
 * When pipelines are unlocked, a requirements/data flow analysis is
 * performed within the pipeline, which is intended to validate the
 * pipeline and eliminate redundant effort.
 *
 * This analysis can not, of itself, factor in the requirements of any
 * pipelines to which this pipeline might dispatch (such dispatches are
 * dependent on the execution-time behavior of node body methods).
 *
 * For this reason, it is important that terminal nodes which dispatch to
 * other pipelines require (in their \ref RxNodeDefinition's ) the clusters
 * that the destination pipelines will require. Without this, it might be
 * determined that certain clusters are no longer required, and they may
 * be terminated before packets reach the dispatching node.
 *
 * \ref RxPipelineNodeRequestCluster causes the node to request a cluster.
 * A copy of the node's \ref RxNodeDefinition is made, and this is edited,
 * so other RxPipelineNodes instanced from the same RxNodeDefinition are
 * not impacted. The cluster is always requested as rxCLREQ_OPTIONAL
 * because it is more flexible than rxCLREQ_REQUIRED. If this function
 * is used and the cluster is not present, rxCLREQ_REQUIRED would cause
 * dependency chasing to fail, whereas with rxCLREQ_OPTIONAL no harm is
 * done.
 *
 * This function must be called prior to unlocking the containing pipeline.
 *
 * \param pipeline  A pointer to the pipeline to act upon
 * \param node  A pointer to the node to request the specified cluster
 * \param clusterDef  A pointer to the cluster definition of the cluster to request
 *
 * \return A pointer to the input pipeline on success, or NULL if there is an error
 * The function fails if the node already requests the cluster
 *
 * \see RxPipelineNodeCloneDefinition
 * \see RxPipelineNodeFindInput
 * \see RxPipelineNodeFindOutputByIndex
 * \see RxPipelineNodeFindOutputByName
 * \see RxPipelineNodeReplaceCluster
 * \see RxLockedPipeAddFragment
 * \see RxLockedPipeAddPath
 * \see RxPipelineClone
 * \see RxPipelineInsertDebugNode
 * \see RxLockedPipeDeleteNode
 * \see RxLockedPipeDeletePath
 * \see RxPipelineFindNodeByIndex
 * \see RxPipelineFindNodeByName
 * \see RxLockedPipeGetEntryPoint
 * \see RxPipelineLock
 * \see RxLockedPipeReplaceNode
 * \see RxLockedPipeSetEntryPoint
 * \see RxLockedPipeUnlock
 */
RxPipeline *
RxPipelineNodeRequestCluster(RxPipeline          *pipeline,
                             RxPipelineNode      *node,
                             RxClusterDefinition *clusterDef)
{
    RWAPIFUNCTION(RWSTRING("RxPipelineNodeRequestCluster"));

    RWASSERT(RxPipelineInstanced);
    RWASSERT(NULL  != pipeline);
    RWASSERT(FALSE != pipeline->locked);
    RWASSERT(NULL  != node);
    RWASSERT(ISNODELIVE(node));
    RWASSERT(NULL != clusterDef);

    if ((NULL  != pipeline)         &&
        (FALSE != pipeline->locked) &&
        (NULL  != node)             &&
         ISNODELIVE(node)           &&
        (NULL != clusterDef))
    {
        RxIoSpec         *iospec = &node->nodeDef->io;
        RxNodeDefinition *nodespec;
        RwUInt32          i;

        /* loop over node's clusters of interest */
        for (i = 0; i < iospec->numClustersOfInterest; i++)
        {
            if (iospec->clustersOfInterest[i].clusterDef == clusterDef)
            {
                RWRETURN((RxPipeline *)NULL); /* fail if node already handles cluster */
            }
        }

        /* Clone and replace the old nodeDef */
        nodespec = _NodeClone(node, clusterDef);

        if (NULL != nodespec) /* created clone with new cluster? */
        {
            /* fix up a [couple of] fields in cloned nodespec */
            nodespec->io.inputRequirements[
                nodespec->io.numClustersOfInterest - 1] = rxCLREQ_OPTIONAL;

            RWRETURN(pipeline);
        }
    }

    RWRETURN((RxPipeline *)NULL);
}

/**
 * \ingroup rwcoregeneric
 * \ref RxPipelineNodeReplaceCluster
 * replaces a cluster in a
 * pipeline node during pipeline definition.
 *
 * This can be used in multipass pipelines, which use the same
 * node several times and want to pass in a different cluster to each
 * instance (a different set of RxUVs or a different RxMeshStateVector,
 * for instance).
 *
 * This function can also be used when the node designer has no means of
 * knowing what a cluster will be called (e.g. a node which simply copies
 * the contents of 'a cluster' into memory somewhere - it reads stride at
 * run-time and can potentially work on any cluster); a place-holder
 * cluster (&clusterPlaceHolder) is then used, which is invariably
 * replaced during pipeline definition.
 *
 * This function must be called prior to unlocking the containing pipeline.
 *
 * \param pipeline  A pointer to the target pipeline
 * \param node  A pointer to the target node
 * \param oldClusterDef  A pointer to the cluster definition of the cluster to be removed
 * \param newClusterDef  A pointer to the cluster definition of the cluster to be added
 *
 * \return A pointer to the input pipeline on success, or NULL if there is an error
 *
 * \see RxPipelineNodeCloneDefinition
 * \see RxPipelineNodeFindInput
 * \see RxPipelineNodeFindOutputByIndex
 * \see RxPipelineNodeFindOutputByName
 * \see RxPipelineNodeRequestCluster
 * \see RxLockedPipeAddFragment
 * \see RxLockedPipeAddPath
 * \see RxPipelineClone
 * \see RxPipelineInsertDebugNode
 * \see RxLockedPipeDeleteNode
 * \see RxLockedPipeDeletePath
 * \see RxPipelineFindNodeByIndex
 * \see RxPipelineFindNodeByName
 * \see RxLockedPipeGetEntryPoint
 * \see RxPipelineLock
 * \see RxLockedPipeReplaceNode
 * \see RxLockedPipeSetEntryPoint
 * \see RxLockedPipeUnlock
 */
RxPipeline *
RxPipelineNodeReplaceCluster(RxPipeline          *pipeline,
                             RxPipelineNode      *node,
                             RxClusterDefinition *oldClusterDef,
                             RxClusterDefinition *newClusterDef)
{
    RWAPIFUNCTION(RWSTRING("RxPipelineNodeReplaceCluster"));

    RWASSERT(RxPipelineInstanced);
    RWASSERT(NULL  != pipeline);
    RWASSERT(FALSE != pipeline->locked);
    RWASSERT(NULL  != node);
    RWASSERT(ISNODELIVE(node));
    RWASSERT(NULL != oldClusterDef);
    RWASSERT(NULL != newClusterDef);

    if ((NULL  != pipeline)         &&
        (FALSE != pipeline->locked) &&
        (NULL  != node)             &&
        ISNODELIVE(node)            &&
        (NULL != oldClusterDef)     &&
        (NULL != newClusterDef))
    {
        RxIoSpec *iospec = &node->nodeDef->io;
        RwUInt32  i;

        /* loop over node's clusters of interest */
        for (i = 0; i < iospec->numClustersOfInterest; i++)
        {
            if (iospec->clustersOfInterest[i].clusterDef ==
                oldClusterDef)
            {
                break;
            }
        }

        if (i != iospec->numClustersOfInterest) /* a match ? */
        {
            RxNodeDefinition *nodespec;

            /* Clone and replace the old nodeDef */
            nodespec = _NodeClone(node, (RxClusterDefinition *)NULL);

            if (nodespec != NULL) /* cloned okay ? */
            {
                /* fix up one more field. */
                nodespec->io.clustersOfInterest[i].clusterDef = newClusterDef;

                RWRETURN(pipeline);
            }
        }
    }

    RWRETURN((RxPipeline *)NULL);
}

/**
 * \ingroup rwcoregeneric
 * \ref RxPipelineNodeCreateInitData
 * creates space for initialization
 * data for a pipeline node.
 *
 * Initialization data can be supplied before a pipeline is unlocked, so
 * that the pipeline node initialization function has some data to work
 * with when it is called at unlock time.
 *
 * This function creates space for initialization data in a node and
 * returns a pointer to that space so that the application can fill it.
 * It creates the space locally so that it can safely be freed when the
 * node is destroyed. If this function is called subsequently, any prior
 * data will be freed.
 *
 * Initialization data was introduced to support \ref RxPipelineClone.
 * It is used to 'remember' the effects of pipeline node setup
 * API functions, such that a pipeline node's \ref RxPipelineNodeInitFn
 * may use it to perform automatic initialization of the node in a clone
 * pipeline.
 *
 * This function should be called before the containing pipeline is unlocked.
 *
 * \param node  A pointer to a pipeline node
 * \param size  An \ref RwUInt32 equal to the size (size > 0) of the initialization data
 *
 * \return a pointer to the space for the initialization data on success,
 * NULL otherwise.
 *
 * \see RxPipelineNodeGetInitData
 * \see RxPipelineNodeCloneDefinition
 * \see RxPipelineNodeFindInput
 * \see RxPipelineNodeFindOutputByIndex
 * \see RxPipelineNodeFindOutputByName
 * \see RxPipelineNodeRequestCluster
 * \see RxLockedPipeAddFragment
 * \see RxLockedPipeAddPath
 * \see RxPipelineClone
 * \see RxPipelineInsertDebugNode
 * \see RxLockedPipeDeleteNode
 * \see RxLockedPipeDeletePath
 * \see RxPipelineFindNodeByIndex
 * \see RxPipelineFindNodeByName
 * \see RxLockedPipeGetEntryPoint
 * \see RxPipelineLock
 * \see RxLockedPipeReplaceNode
 * \see RxLockedPipeSetEntryPoint
 * \see RxLockedPipeUnlock
 */
void *
RxPipelineNodeCreateInitData(RxPipelineNode *node, RwUInt32 size)
{
    RWAPIFUNCTION(RWSTRING("RxPipelineNodeCreateInitData"));

    RWASSERT(RxPipelineInstanced);
    RWASSERT(node != NULL);
    RWASSERT(size > 0);
    /* Should be called before unlock, there's no point otherwise */
    RWASSERT(node->topSortData != NULL);

    if (node->initializationData != NULL)
    {
        RwFree(node->initializationData);
        node->initializationData = NULL;
        node->initializationDataSize = 0;
    }

    node->initializationData = RwMalloc(size,
        rwMEMHINTDUR_EVENT | rwID_PIPEMODULE);
    if (node->initializationData == NULL)
    {
        RWERROR((E_RW_NOMEM, size));
        RWRETURN(NULL);
    }
    node->initializationDataSize = size;

    RWRETURN(node->initializationData);
}

/**
 * \ingroup rwcoregeneric
 * \ref RxPipelineNodeGetInitData
 * gets the initialization data pointer
 * of a pipeline node.
 *
 * Initialization data may be supplied before a pipeline is unlocked, so
 * that the pipeline node initialization function has some data to work
 * with when it is called.
 *
 * When the pipeline node is destroyed (this occurs when its containing
 * pipeline is destroyed or the node removed from it) or the data is
 * replaced (this occurs whenever \ref RxPipelineNodeCreateInitData is called),
 * the data is freed.
 *
 * \param node  A pointer to a pipeline node
 *
 * \return A pointer to the initialization data on success, or NULL if there is an error.
 *
 * \see RxPipelineNodeCreateInitData
 * \see RxPipelineNodeCloneDefinition
 * \see RxPipelineNodeFindInput
 * \see RxPipelineNodeFindOutputByIndex
 * \see RxPipelineNodeFindOutputByName
 * \see RxPipelineNodeRequestCluster
 * \see RxLockedPipeAddFragment
 * \see RxLockedPipeAddPath
 * \see RxPipelineClone
 * \see RxPipelineInsertDebugNode
 * \see RxLockedPipeDeleteNode
 * \see RxLockedPipeDeletePath
 * \see RxPipelineFindNodeByIndex
 * \see RxPipelineFindNodeByName
 * \see RxLockedPipeGetEntryPoint
 * \see RxPipelineLock
 * \see RxLockedPipeReplaceNode
 * \see RxLockedPipeSetEntryPoint
 * \see RxLockedPipeUnlock
 */
void               *
RxPipelineNodeGetInitData(RxPipelineNode * node)
{
    RWAPIFUNCTION(RWSTRING("RxPipelineNodeGetInitData"));

    RWASSERT(RxPipelineInstanced);
    RWASSERT(node != NULL);

    RWRETURN(node->initializationData);
}

/**
 * \ingroup rwcoregeneric
 * \ref RxLockedPipeUnlock
 * unlocks a pipeline (switches, in effect,
 * pipeline state from EDITABLE to EXECUTABLE).
 *
 * Pipeline unlocking is computationally intensive (see
 * \ref RxPipelineExecute), and can fail if it is determined that the
 * requirements of all nodes cannot be met by the nodes feeding them.
 *
 * \param pipeline  A pointer to a locked pipeline.
 *
 * \return A pointer to the input pipeline on success, or NULL if there is an error
 *
 * \see RxPipelineNodeCloneDefinition
 * \see RxPipelineNodeFindInput
 * \see RxPipelineNodeFindOutputByIndex
 * \see RxPipelineNodeFindOutputByName
 * \see RxPipelineNodeReplaceCluster
 * \see RxPipelineNodeRequestCluster
 * \see RxLockedPipeAddFragment
 * \see RxLockedPipeAddPath
 * \see RxPipelineClone
 * \see RxPipelineInsertDebugNode
 * \see RxLockedPipeDeleteNode
 * \see RxLockedPipeDeletePath
 * \see RxPipelineFindNodeByIndex
 * \see RxPipelineFindNodeByName
 * \see RxLockedPipeGetEntryPoint
 * \see RxPipelineLock
 * \see RxLockedPipeReplaceNode
 * \see RxLockedPipeSetEntryPoint
 */
RxPipeline         *
RxLockedPipeUnlock(RxPipeline * pipeline)
{
    RWAPIFUNCTION(RWSTRING("RxLockedPipeUnlock"));

    /* MEMORY ALLOCATION DURING RXPIPELINE CONSTRUCTION:
     *
     * The RxPipeline structure is allocated in a block of its own.
     * It's actually allocated from a freelist (one block of which
     * should be able to hold all default and common plugin pipes).
     *
     * During the first RxPipelineLock(), a secondary block is
     * allocated to contain three arrays - RxPipelineNodes,
     * RwUInt32 'outputs' (a list of output indexes per node)
     * and 'topSortData', a RxPipelineNodeTopSortData per node.
     * These three arrays are allocated to be as big as they
     * will ever need to be. That's RXPIPELINEGLOBAL(maxNodesPerPipe)
     * (initialised at startup from _rxPipelineMaxNodes) for the
     * nodes and topsort arrays and RXNODEMAXOUTPUTS outputs per
     * node in the outputs array.
     *
     * These arrays are filled by calls to RxLockedPipeAddFragment.
     * RxLockedPipe[Delete|Replace]Node modify them too (keeping the
     * array fully compacted).
     *
     * At the beginning of RxLockedPipeUnlock, the pipelinenodes
     * and outputs arrays are packed down to the start of the memory
     * block. The block is then realloced to contain as much space
     * as will be required during unlock and the topsortdata array
     * is then packed to the end of this memory block. Temporary
     * data is allocated from the top of the block down (the
     * 'stalactite' stack, starts at the bottom of topsortdata) and
     * persistent data (i.e that which will persist after unlock
     * completes) will be allocated from the bottom of the block up
     * (the 'stalacmite' stack, from the top of the outputs array).
     *
     * At the end of unlock, topsortdata is discarded along with other
     * temporary data (e.g RwReqEntrys) and topsortdata pointers in
     * the pipeline nodes are set to NULL. The persistent data is then
     * 'shrinkwrapped' by reallocing down to the top of the stalacmite
     * stack.
     *
     * If RxPipelineLock is called again, the pipelinenodes and outputs
     * arrays are moved and expanded up to their conservatively large
     * sizes again and the topsortdata array is recreated. The pointers
     * from nodes to outputs and topsortdata are fixed up. Realloc is
     * used to grow the memory block if necessary (again, w/ fixup).
     *
     *
     * RxPipelineDestroy() merely has 2 memory blocks to free, the
     * pipeline itself and the auxiliary block mentioned above. It
     * must as before call node term callbacks and free any
     * initializationData that's been allocated.
     *
     * RxPipelineClone() has to create a locked pipe and copy the
     * pipelinenodes and outputs arrays into its memory block, recreate
     * the topsortdata array, fix up pointers to outputs and topsortdata,
     * copy initializationData (and fix up pointers to that) and then
     * unlock the new pipe (we don't actually expect pipelineclone to
     * work, you know. It'll probably get removed in later revisions. I'd
     * only expect it to reappear once/if we make pipe creation more
     * automated and data-driven). */


    RWASSERT(RxPipelineInstanced);
    RWASSERT(NULL != pipeline);
    RWASSERT(FALSE != pipeline->locked);

    if ((NULL  != pipeline) &&
        (FALSE != pipeline->locked))
    {
        if (pipeline->numNodes != 0)
        {
            RxPipelineNodeTopSortData *newTopSortData;
            RxPipelineNodeTopSortData *topSortData;
            RwUInt32  numUniqueClusters;
            RwUInt32  unlockStartBlockSize;
            RwUInt32  unlockEndBlockSize;
            RwUInt32  topSortBlockSize;
            RwUInt32  depChaseBlockSize;
            RwUInt32  totalOutputs = 0;
            RwUInt32 *newOutputs;
            RwUInt32 *outputs;
            RwBool    error = FALSE;
            RwUInt32  doneNodes = 0;
            RwInt32   i;


            RWASSERT(pipeline->entryPoint < pipeline->numNodes);
            RWASSERT(ISNODELIVE(&pipeline->nodes[pipeline->entryPoint]));

            if (!( (pipeline->entryPoint < pipeline->numNodes) &&
                   (ISNODELIVE(&pipeline->nodes[pipeline->entryPoint])) )  )
            {
                /* Entry-point is invalid; fail (pipeline remains Lock()ed) */
                RWERROR((E_RX_INVALIDENTRYPOINT));
                RWRETURN((RxPipeline *)NULL);
            }


            /* Sets up numUniqueClusters, which is used in
             * subsequent macros and dep-chase functions */
            numUniqueClusters = PipelineCalcNumUniqueClusters(pipeline);


            /* We need to calculate a conservative value for the amount
             * of memory we'll need during unlock() and allocate it.
             * Here's the sequence of memory usage and freeing/packing:
             *
             *  - lock, allocate a conservatively-sized block
             *  o 3 arrays [nodes, outputs, topsortdata] (mite)
             *  - edit the pipe (no further allocation)
             *  - start unlock, allocate the big block
             *  o 3 arrays [nodes, outputs, topsortdata] (mite)
             *  - do topsort
             *  - move topSortData to top of mem (tite), fixup ptrs
             *  - compact nodes+outputs arrays, fixup ptrs
             *  - set up mite ptr (should be NULL before this) to just after outputs
             *  o compacted 2 arrays [nodes, outputs] (mite) + further persistent mem (mite) +
             *    depchase temp mem (tite) + topsortdata (tite)
             *  - do depchase (including _ForAllNodesWriteClusterAllocations)
             *  - shrinkwrap to (compacted 2 arrays + further persistent mem)
             */

            /* First task is PipelineUnlockTopSort() which needs this much: */
            topSortBlockSize = CalcNodeOutputsTopSortDataUnCompactedMemSize();

            /* Second task is _rxChaseDependencies() (including
             * _ForAllNodesWriteClusterAllocations()), which requires: */
            depChaseBlockSize  = CalcTopSortDataMemSize();
            depChaseBlockSize += CalcNodesOutputsCompactedMemSize(pipeline);
            depChaseBlockSize += CalcTempDepChaseMemSize(
                                     pipeline, numUniqueClusters);
            depChaseBlockSize += CalcUnlockPersistentMemSize(
                                     pipeline, numUniqueClusters);


            /* Allocate the largest required block */
            unlockStartBlockSize = topSortBlockSize;
            if (depChaseBlockSize > unlockStartBlockSize)
            {
                unlockStartBlockSize = depChaseBlockSize;
            }

            /* Grow superBlock if necessary */
            if (unlockStartBlockSize > pipeline->superBlockSize)
            {
                if (FALSE == ReallocAndFixupSuperBlock(
                                 pipeline, unlockStartBlockSize))
                {
                    RWRETURN((RxPipeline *)NULL);
                }
            }

            /* Just for PipelineUnlockTopSort(), we start the stalacTite array after the
             * topSortData array. This is so that we can keep the three arrays uniform and
             * full-sized until after PipelineUnlockTopSort() (makes its shuffling simpler) */
            gMemoryLimits.StalacTiteBase = ((RwUInt8 *)pipeline->superBlock) + unlockStartBlockSize;
            /* Mite ptr NULL for now, it should not be used. If it was going to be used,
             * it'd be: (RwUInt8 *)&(topSortData[RXPIPELINEGLOBAL(maxNodesPerPipe)]) */
            gMemoryLimits.StalacMiteBase = (RwUInt8 *) NULL;

            pipeline = PipelineUnlockTopSort(pipeline);
            if (NULL == pipeline)
            {
                /* The three arrays haven't moved (stuff might have been
                 * shuffled but that's okay), so if we return now we're in
                 * the locked state, as desired. */
                RWRETURN((RxPipeline *)NULL);
            }


            /* Now we compact the pipeline nodes and outputs arrays to the bottom
             * of superBlock and move the topSortData array to the top of mem,
             * fixing up pointers as we go. NOTE: PipelineUnlockTopSort() should
             * have shuffled all three arrays to remain in the same order as
             * each other. */

            outputs = (RwUInt32 *)&(pipeline->nodes[RXPIPELINEGLOBAL(maxNodesPerPipe)]);
            topSortData = (RxPipelineNodeTopSortData *)&
                (outputs[RXNODEMAXOUTPUTS*RXPIPELINEGLOBAL(maxNodesPerPipe)]);
            topSortData += pipeline->numNodes - 1;
            newTopSortData = (RxPipelineNodeTopSortData *)
                (((RwUInt8 *)pipeline->superBlock) + unlockStartBlockSize);
            newTopSortData--;
            RWASSERT(newTopSortData > topSortData);
            /* Move the topSortData in reverse order to avoid overwriting */
            for (i = (pipeline->numNodes - 1);i >= 0;i--)
            {
                memcpy(newTopSortData,
                       topSortData,
                       sizeof(RxPipelineNodeTopSortData));
                pipeline->nodes[i].topSortData = newTopSortData;

                topSortData--;
                newTopSortData--;
            }

            /* Pack the outputs array after the used pipeline nodes */
            newOutputs = (RwUInt32 *)&(pipeline->nodes[pipeline->numNodes]);
            for (i = 0;(RwUInt32)i < pipeline->numNodes;i++)
            {
                if (0 == pipeline->nodes[i].numOutputs)
                {
                    pipeline->nodes[i].outputs = (RwUInt32 *)NULL;
                }
                else
                {
                    memcpy(newOutputs,
                           outputs,
                           pipeline->nodes[i].numOutputs*sizeof(RwUInt32));
                    pipeline->nodes[i].outputs = newOutputs;
                }

                outputs      += RXNODEMAXOUTPUTS;
                newOutputs   += pipeline->nodes[i].numOutputs;
                totalOutputs += pipeline->nodes[i].numOutputs;
            }

            /* For dep-chasing and persistent data setup, the stalacTite
             * pointer starts at the bottom of the topSortData array and
             * moves down from there. The stalacMite pointer starts above
             * the packed outputs array. */
            gMemoryLimits.StalacMiteBase  = (RwUInt8 *)&(outputs[totalOutputs]);
            gMemoryLimits.StalacTiteBase  = (RwUInt8 *)topSortData;

            if (_rxChaseDependencies(pipeline) != 0)
            {
                /* dep. chasing failed;
                 * fail (pipeline remains Lock()ed, sorted) */
                RWRETURN((RxPipeline *)NULL);
            }

            /* Now we're done, 'shrinkwrap' our persistent allocations
             * (the stalacMite stack) and fix up internal pointers
             * where necessary */
            unlockEndBlockSize = gMemoryLimits.StalacMiteBase -
                                 (RwUInt8 *)pipeline->superBlock;
            if (FALSE == ReallocAndFixupSuperBlock(
                             pipeline, unlockEndBlockSize))
            {
                goto fail;
            }

            /* We've effectively 'free' topSortData with the above
             * realloc, so set those pipelinenode pointers to NULL: */
            for (i = 0;(RwUInt32)i < pipeline->numNodes;i++)
            {
                pipeline->nodes[i].topSortData = (RxPipelineNodeTopSortData *)NULL;
            }

            RXCHECKFORUSERTRAMPLING(pipeline);


            /* init & config method calls flow from bottom to
             * top (i.e. consumer -> producer) */
            for (i = (pipeline->numNodes - 1);i >= 0;i--)
            {
                RxNodeDefinition *nodeDef =   pipeline->nodes[i].nodeDef;
                RxPipelineNode   *node    = &(pipeline->nodes[i]);

                /* nodeinit (called first time node
                 * is used in an application) */
                if (nodeDef->InputPipesCnt++ == 0)
                {
                    if (NULL != nodeDef->nodeMethods.nodeInit)
                    {
                        if (nodeDef->nodeMethods.nodeInit(nodeDef) == FALSE)
                        {
                            /* Early-out but unroll nodeInits with nodeTerms */
                            doneNodes = (pipeline->numNodes - 1) - i;
                            error = TRUE;
                            break;
                        }
                    }
                }

                /* pipelinenodeinit */
                if (nodeDef->nodeMethods.pipelineNodeInit != NULL)
                {
                    if (nodeDef->nodeMethods.pipelineNodeInit(node) == FALSE)
                    {
                        /* Early-out but unroll nodeInits with nodeTerms */
                        nodeDef->InputPipesCnt--;
                        if (0 == nodeDef->InputPipesCnt)
                        {
                            if (NULL != nodeDef->nodeMethods.nodeTerm)
                            {
                                nodeDef->nodeMethods.nodeTerm(nodeDef);
                            }
                        }
                        doneNodes = (pipeline->numNodes - 1) - i;
                        error = TRUE;
                        break;
                    }
                }
            }

            if (FALSE == error)
            {
                for (i = (pipeline->numNodes - 1);i >= 0;i--)
                {
                    RxNodeDefinition *nodeDef =   pipeline->nodes[i].nodeDef;
                    RxPipelineNode   *node    = &(pipeline->nodes[i]);

                    /* pipelinenodeconfig */
                    if (nodeDef->nodeMethods.pipelineNodeConfig != NULL)
                    {
                        if (nodeDef->nodeMethods.pipelineNodeConfig(
                                node, pipeline) == FALSE)
                        {
                            /* Early-out but unroll nodeInits with nodeTerms */
                            doneNodes = pipeline->numNodes;
                            error = TRUE;
                            break;
                        }
                    }
                }
            }

            if (FALSE != error)
            {
                /* On error, undo completed node initializations in reverse order */
                for (i = (pipeline->numNodes - 1) - (doneNodes - 1);
                     (RwUInt32)i < pipeline->numNodes;
                     i++)
                {
                    RxNodeDefinition *nodeDef =   pipeline->nodes[i].nodeDef;
                    RxPipelineNode   *node    = &(pipeline->nodes[i]);

                    /* pipelinenodeterm undoes pipelinenodeinit */
                    if (nodeDef->nodeMethods.pipelineNodeTerm != NULL)
                    {
                        nodeDef->nodeMethods.pipelineNodeTerm(node);
                    }

                    /* nodeterm undoes nodeinit */
                    nodeDef->InputPipesCnt--;
                    if (0 == nodeDef->InputPipesCnt)
                    {
                        if (NULL != nodeDef->nodeMethods.nodeTerm)
                        {
                            nodeDef->nodeMethods.nodeTerm(nodeDef);
                        }
                    }
                }

                /* Revert to the locked state and exit */
                goto fail;
            }

            RXCHECKFORUSERTRAMPLING(pipeline);
        }

        pipeline->locked = FALSE;

        RWRETURN(pipeline);    /* succeed */
    }

    if (pipeline == NULL)
    {
        RWERROR((E_RW_NULLP));
    }
    else
    {
        RWERROR((E_RX_UNLOCKEDPIPE));
    }

    RWRETURN((RxPipeline *)NULL); /* No pipeline or not locked; fail */

  fail:

    /* We can't merely free superBlock here, we need to return to the
     * locked pipe's state. so we use the code from RxPipelineLock()
     * which expands the nodes and outputs and creates the topsortdata */
    LockPipelineExpandData(pipeline, pipeline);

    RWRETURN((RxPipeline *)NULL);
}

/**
 * \ingroup rwcoregeneric
 * \ref RxPipelineLock
 * locks a pipeline (switches, in effect,
 * pipeline state from EXECUTABLE to EDITABLE).
 *
 * \param pipeline  A pointer to an unlocked pipeline.
 *
 * \return A pointer to the input pipeline on success, or NULL if there is an error
 *
 * \see RxPipelineNodeCloneDefinition
 * \see RxPipelineNodeFindInput
 * \see RxPipelineNodeFindOutputByIndex
 * \see RxPipelineNodeFindOutputByName
 * \see RxPipelineNodeReplaceCluster
 * \see RxPipelineNodeRequestCluster
 * \see RxLockedPipeAddFragment
 * \see RxLockedPipeAddPath
 * \see RxPipelineClone
 * \see RxPipelineInsertDebugNode
 * \see RxLockedPipeDeleteNode
 * \see RxLockedPipeDeletePath
 * \see RxPipelineFindNodeByIndex
 * \see RxPipelineFindNodeByName
 * \see RxLockedPipeGetEntryPoint
 * \see RxLockedPipeReplaceNode
 * \see RxLockedPipeSetEntryPoint
 * \see RxLockedPipeUnlock
 */
RxPipeline         *
RxPipelineLock(RxPipeline * pipeline)
{
    RWAPIFUNCTION(RWSTRING("RxPipelineLock"));

    RWASSERT(RxPipelineInstanced);
    RWASSERT(pipeline != NULL);

    if (pipeline->locked == FALSE)
    {
        RwUInt32 lockedBlockSize;

        /* We allocate enough memory for a pipeline with the maximum
         * number of nodes and the maximum number of outputs per node.
         * This keeps allocation during pipe editing down to zero. */
        lockedBlockSize = CalcNodeOutputsTopSortDataUnCompactedMemSize();

        /* Resize or alloc superBlock */
        if (pipeline->nodes != NULL)
        {
            RWASSERT(NULL != pipeline->superBlock);
            RWASSERT(pipeline->nodes == pipeline->superBlock);

            /* Realloc to the necessary size and fixup pointers if the block moves */
            if (lockedBlockSize > pipeline->superBlockSize)
            {
                if (FALSE == ReallocAndFixupSuperBlock(
                                 pipeline, lockedBlockSize))
                {
                    RWRETURN((RxPipeline *)NULL);
                }
            }

            /* We need to move existing outputs arrays to the right
             * position in our conservative array and fixup pipeline
             * node pointers to their entries. */
            if (FALSE == LockPipelineExpandData(pipeline, pipeline))
            {
                RWRETURN((RxPipeline *)NULL);
            }
        }
        else
        {
            RWASSERT(NULL == pipeline->superBlock);
            RWASSERT(0    == pipeline->numNodes);

            pipeline->superBlock = RwMalloc(lockedBlockSize,
                rwMEMHINTDUR_EVENT | rwMEMHINTFLAG_RESIZABLE | rwID_PIPEMODULE);
            if (NULL == pipeline->superBlock)
            {
                RWERROR((E_RW_NOMEM, lockedBlockSize));
                RWRETURN((RxPipeline *)NULL);
            }
            pipeline->superBlockSize = lockedBlockSize;

            /* pipelineNodes heads up the array */
            pipeline->nodes = (RxPipelineNode *)pipeline->superBlock;
        }

        pipeline->locked = TRUE;
        if (pipeline->nodes != NULL)
        {
            RwUInt32            n;

            for (n = 0; n < pipeline->numNodes; n++)
            {
                const RxNodeMethods *const nodeMethods =
                    &pipeline->nodes[n].nodeDef->nodeMethods;

                /* pipelinenodeterm */
                if (nodeMethods->pipelineNodeTerm != NULL)
                {
                    nodeMethods->pipelineNodeTerm(&pipeline->nodes[n]);
                }

                /* nodeterm (called when no more UNLOCKED pipeline nodes
                 * reference this node def) */
                if (--(pipeline->nodes[n].nodeDef->InputPipesCnt) == 0)
                {
                    if (nodeMethods->nodeTerm != NULL)
                    {
                        nodeMethods->nodeTerm(pipeline->nodes[n].nodeDef);
                    }
                }

                /* honour the promise in the field description (qv) */
                pipeline->nodes[n].slotClusterRefs = (RxPipelineCluster **)NULL;
            }
        }
    }

    RWRETURN(pipeline);
}

/**
 * \ingroup rwcoregeneric
 * \ref RxPipelineClone
 * deep-copies an unlocked pipeline; the
 * resultant copy may then form the basis for subsequent edits.
 *
 * Cloning a pipeline is non-trivial. This is because the nodes
 * within a pipeline that was created in external code will have
 * been set up by unknown API calls. Initialization data (see
 * \ref RxPipelineNodeCreateInitData) is used to 'remember' the
 * effects of pipeline node setup API functions, such that a
 * pipeline node's \ref RxPipelineNodeInitFn may use it to
 * perform automatic initialization of the node in a clone
 * pipeline.
 *
 * The use of this function is no longer recommended. It may be
 * removed from the library in subsequent versions.
 *
 * \param pipeline  A pointer to the unlocked pipeline to clone
 *
 * \return A pointer to the clone on success, otherwise NULL.
 *
 * \see RxPipelineNodeCloneDefinition
 * \see RxPipelineNodeFindInput
 * \see RxPipelineNodeFindOutputByIndex
 * \see RxPipelineNodeFindOutputByName
 * \see RxPipelineNodeReplaceCluster
 * \see RxPipelineNodeRequestCluster
 * \see RxLockedPipeAddFragment
 * \see RxLockedPipeAddPath
 * \see RxPipelineInsertDebugNode
 * \see RxLockedPipeDeleteNode
 * \see RxLockedPipeDeletePath
 * \see RxPipelineFindNodeByIndex
 * \see RxPipelineFindNodeByName
 * \see RxLockedPipeGetEntryPoint
 * \see RxPipelineLock
 * \see RxLockedPipeReplaceNode
 * \see RxLockedPipeSetEntryPoint
 * \see RxLockedPipeUnlock
 */
RxPipeline         *
RxPipelineClone(RxPipeline * pipeline)
{
    RWAPIFUNCTION(RWSTRING("RxPipelineClone"));

    /* "deep" copy a pipeline */

    RWASSERT(RxPipelineInstanced);
    RWASSERT((pipeline != NULL) && (pipeline->locked == FALSE));

    if ((pipeline != NULL) && (pipeline->locked == FALSE))
    {
        RxPipeline *clonePipe;
        RxPipeline *result;

        clonePipe = RxPipelineCreate();

        if (clonePipe != NULL)
        {
            /* Guess we'd better copy these.. they can be overridden */
            clonePipe->pluginId   = pipeline->pluginId;
            clonePipe->pluginData = pipeline->pluginData;

            if (pipeline->numNodes == 0)
            {
                RWRETURN(clonePipe);
            }

            result = RxPipelineLock(clonePipe);

            if (NULL != result)
            {
                if (FALSE != LockPipelineExpandData(clonePipe, pipeline))
                {
                    if (NULL != RxLockedPipeUnlock(clonePipe))
                    {
                        RWRETURN(clonePipe);
                    }
                }
            }

            _rxPipelineDestroy(clonePipe);
        }
    }

    RWRETURN((RxPipeline *) NULL);
}

/**
 * \ingroup rwcoregeneric
 * \ref RxPipelineFindNodeByName
 *  finds the first PipelineNode
 * in a given pipeline which has a given name specified in its
 * \ref RxNodeDefinition.
 * If a non-NULL reference to a \ref RxPipelineNode
 * within the pipeline is passed in, the search starts immediately after
 * that pipeline node. If a non-NULL reference to a \ref RwInt32 index is
 * passed in, it will be filled, on success, with the index of the
 * pipeline node. On failure it will be filled with -1.
 *
 * Pipeline node pointers are not valid across an
 * \ref RxLockedPipeUnlock, as this function causes the nodes to be
 * re-ordered.
 *
 * \param pipeline  A pointer to a pipeline to search
 * \param name  A pointer to a string to search with
 * \param start  An optional pointer to a pipeline node after which to start the search
 * \param nodeIndex  An optional reference to an index in which to put the pipeline node's index
 *
 * \return A pointer to the found pipeline node on success, or NULL if there is an error
 *
 * \see RxPipelineNodeCloneDefinition
 * \see RxPipelineNodeFindInput
 * \see RxPipelineNodeFindOutputByIndex
 * \see RxPipelineNodeFindOutputByName
 * \see RxPipelineNodeReplaceCluster
 * \see RxPipelineNodeRequestCluster
 * \see RxLockedPipeAddFragment
 * \see RxLockedPipeAddPath
 * \see RxPipelineClone
 * \see RxPipelineInsertDebugNode
 * \see RxLockedPipeDeleteNode
 * \see RxLockedPipeDeletePath
 * \see RxPipelineFindNodeByIndex
 * \see RxLockedPipeGetEntryPoint
 * \see RxPipelineLock
 * \see RxLockedPipeReplaceNode
 * \see RxLockedPipeSetEntryPoint
 * \see RxLockedPipeUnlock
 */
RxPipelineNode     *
RxPipelineFindNodeByName(RxPipeline * pipeline,
                         const RwChar * name,
                         RxPipelineNode * start,
                         RwInt32 * nodeIndex)
{
    RwBool              check;

    RWAPIFUNCTION(RWSTRING("RxPipelineFindNodeByName"));

    RWASSERT(RxPipelineInstanced);
    RWASSERT(pipeline != NULL);
    RWASSERT(name != NULL);

    check = (pipeline != NULL) && (name != NULL)
        && (pipeline->numNodes);

    if (check != 0)
    {
        RxPipelineNode     *node = &pipeline->nodes[0];
        RwInt32             n = pipeline->numNodes;

        /* Search from just after a given pipelinenode -
         * allows finding all the nodes within
         * a given pipeline with the specified nodedefinition */
        if (start)
        {
            while ((node != start) && (n > 0))
            {
                node++;
                n--;
            }
            node++;
            n--;
        }

        /* loop over pipeline nodes */
        while (n > 0)
        {
            if (ISNODELIVE(node))
            {
                if (!rwstrcmp(node->nodeDef->name, name))
                {
                    if (nodeIndex != NULL)
                        *nodeIndex = pipeline->numNodes - n;
                    RWRETURN(node);
                }
            }
            node++;
            n--;
        }
    }

    /* Nope, didn't see it missus */
    if (nodeIndex != NULL)
        *nodeIndex = -1;
    RWRETURN((RxPipelineNode *)NULL);
}

/**
 * \ingroup rwcoregeneric
 * \ref RxPipelineFindNodeByIndex
 *  returns the node in a pipeline at
 * index "nodeindex". Indices increment from zero with the first fragment
 * through each subsequent pipeline node added.
 *
 * Neither pipeline node indices nor pipeline node pointers are valid
 * across an \ref RxLockedPipeUnlock, as this function causes the pipeline
 * nodes to be re-ordered.
 *
 * \param pipeline  A pointer to the target pipeline
 * \param nodeindex  The index of the pipeline node
 *
 * \return A pointer to the pipeline node on success, NULL otherwise.
 *
 * \see RxPipelineNodeCloneDefinition
 * \see RxPipelineNodeFindInput
 * \see RxPipelineNodeFindOutputByIndex
 * \see RxPipelineNodeFindOutputByName
 * \see RxPipelineNodeReplaceCluster
 * \see RxPipelineNodeRequestCluster
 * \see RxLockedPipeAddFragment
 * \see RxLockedPipeAddPath
 * \see RxPipelineClone
 * \see RxPipelineInsertDebugNode
 * \see RxLockedPipeDeleteNode
 * \see RxLockedPipeDeletePath
 * \see RxPipelineFindNodeByName
 * \see RxLockedPipeGetEntryPoint
 * \see RxPipelineLock
 * \see RxLockedPipeReplaceNode
 * \see RxLockedPipeSetEntryPoint
 * \see RxLockedPipeUnlock
 */
RxPipelineNode     *
RxPipelineFindNodeByIndex(RxPipeline * pipeline, RwUInt32 nodeindex)
{
    RWAPIFUNCTION(RWSTRING("RxPipelineFindNodeByIndex"));

    RWASSERT(RxPipelineInstanced);
    RWASSERT(pipeline != NULL);
    RWASSERT(nodeindex < pipeline->numNodes);

    RWRETURN((pipeline != NULL
              && nodeindex <
              pipeline->numNodes) ? &pipeline->
             nodes[nodeindex] : (RxPipelineNode *) NULL);
}

/**
 * \ingroup rwcoregeneric
 * \ref RxLockedPipeAddFragment
 * adds a fragment to a locked pipeline.
 *
 * A fragment is a chain of nodes, interconnected on their first (default)
 * outputs.
 *
 * Constructing a pipeline is typically a process of adding a number of
 * fragments (describing linear sections of the pipeline graph), and then
 * adding additional paths between fragments using \ref RxLockedPipeAddPath.
 *
 * \note Each node definition is limited to have RXNODEMAXCLUSTERSOFINTEREST
 *       clusters of interest.
 * \note Each node definition is limited to have RXNODEMAXOUTPUTS outputs.
 * \note Each pipeline is limited to have by default a maximum of
 *       RXPIPELINEDEFAULTMAXNODES nodes. This value may be overridden by
 *       changing _rxPipelineMaxNodes BEFORE RenderWare is initialized.
 *
 * This function should be called before the containing pipeline is unlocked.
 *
 * \param pipeline  A pointer to a locked pipeline
 * \param firstIndex  An optional pointer to a \ref RwUInt32 to receive the
 *  index of the first added node
 * \param nodeDef0  A pointer to an array of one or more RxNodeDefinitions.
 *  The array is of variable length and is NULL terminated
 *
 * \return A pointer to the locked pipeline or NULL if it fails
 *
 * \see RxPipelineNodeCloneDefinition
 * \see RxPipelineNodeFindInput
 * \see RxPipelineNodeFindOutputByIndex
 * \see RxPipelineNodeFindOutputByName
 * \see RxPipelineNodeReplaceCluster
 * \see RxPipelineNodeRequestCluster
 * \see RxLockedPipeAddPath
 * \see RxPipelineClone
 * \see RxPipelineInsertDebugNode
 * \see RxLockedPipeDeleteNode
 * \see RxLockedPipeDeletePath
 * \see RxPipelineFindNodeByIndex
 * \see RxPipelineFindNodeByName
 * \see RxLockedPipeGetEntryPoint
 * \see RxPipelineLock
 * \see RxLockedPipeReplaceNode
 * \see RxLockedPipeSetEntryPoint
 * \see RxLockedPipeUnlock
 */
RxPipeline *
RxLockedPipeAddFragment(RxPipeline       *pipeline,
                        RwUInt32         *firstIndex,
                        RxNodeDefinition *nodeDef0,
                        ...)
{
    RWAPIFUNCTION(RWSTRING("RxLockedPipeAddFragment"));

    RWASSERT(RxPipelineInstanced);
    RWASSERT(pipeline != NULL);
    RWASSERT(pipeline->locked);

    if (pipeline != NULL && pipeline->locked != FALSE)
    {
        va_list             va;
        RxNodeDefinition   *nodeDef;
        RwUInt32            fragLength = 0;

        va_start(va, nodeDef0);
        for (nodeDef = nodeDef0; nodeDef != NULL;
             nodeDef = va_arg(va, RxNodeDefinition *))
        {
            fragLength++;
        }
        va_end(va);

        if (fragLength != 0)
        {
            RwUInt32        oldnumnodes, n;
            RxPipelineNode *prevnode = (RxPipelineNode *)NULL;

            RWASSERT((pipeline->numNodes + fragLength) <= RXPIPELINEGLOBAL(maxNodesPerPipe));
            if ((pipeline->numNodes + fragLength) > RXPIPELINEGLOBAL(maxNodesPerPipe))
            {
                RWERROR((E_RX_PIPELINETOOMANYNODES));
                RWRETURN((RxPipeline *)NULL);
            }

            oldnumnodes = pipeline->numNodes;

            n = 0;
            va_start(va, nodeDef0);
            for (nodeDef = nodeDef0; nodeDef != NULL;
                 nodeDef = va_arg(va, RxNodeDefinition *))
            {
                RxPipelineNode *node = &pipeline->nodes[oldnumnodes + n];

                if (!_NodeCreate(pipeline, node, nodeDef))
                {
                    break;
                }
                if (prevnode != NULL)
                {
                    /* connect to previous node */
                    if (!RxLockedPipeAddPath
                        (pipeline,
                         RxPipelineNodeFindOutputByIndex(prevnode, 0),
                         RxPipelineNodeFindInput(node)))
                    {
                        PipelineNodeDestroy(node, pipeline);

                        break;
                    }
                }

                prevnode = node;

                n++;
            }
            va_end(va);

            if (n == fragLength)
            {
                if (firstIndex != 0)
                {
                    *firstIndex = oldnumnodes;
                }
                RWRETURN(pipeline); /* success */
            }
            else
            {
                RwDebugSendMessage(rwDEBUGMESSAGE,
                                   "RxLockedPipeAddFragment",
                                   "Failed to create and link all nodes in new fragment");

                /* fragment construction did not complete: unwind */

                while (n--)
                {
                    PipelineNodeDestroy(
                        &(pipeline->nodes[(oldnumnodes - 1) + (n + 1)]), pipeline);
                }
            }
        }
        else
        {
            RwDebugSendMessage(rwDEBUGMESSAGE,
                               "RxLockedPipeAddFragment",
                               "No nodes specified to add!");
        }
    }
    else
    {
        if (pipeline == NULL)
        {
            RWERROR((E_RW_NULLP));
        }
        else
        {
            RWERROR((E_RX_UNLOCKEDPIPE));
        }
    }

    RWRETURN((RxPipeline *)NULL);            /* failure */
}

/**
 * \ingroup rwcoregeneric
 * \ref RxLockedPipeReplaceNode replaces an existing pipeline node
 * with a new pipeline node instanced from a specified node definition.
 * Paths into and out of the node are retained to the greatest extent
 * possible.
 *
 * Pipeline nodes can be located within pipelines using
 * \ref RxPipelineFindNodeByName or \ref RxPipelineFindNodeByIndex.
 *
 * \param pipeline  A pointer to a locked pipeline
 * \param node      A pointer to the pipeline node to be replaced
 * \param nodeDef   A pointer to the node definition of the pipeline node to be added
 *
 * \return A pointer to the pipeline added.
 *
 * \see RxPipelineNodeCloneDefinition
 * \see RxPipelineNodeFindInput
 * \see RxPipelineNodeFindOutputByIndex
 * \see RxPipelineNodeFindOutputByName
 * \see RxPipelineNodeReplaceCluster
 * \see RxPipelineNodeRequestCluster
 * \see RxLockedPipeAddFragment
 * \see RxLockedPipeAddPath
 * \see RxPipelineClone
 * \see RxPipelineInsertDebugNode
 * \see RxLockedPipeDeleteNode
 * \see RxLockedPipeDeletePath
 * \see RxPipelineFindNodeByIndex
 * \see RxPipelineFindNodeByName
 * \see RxLockedPipeGetEntryPoint
 * \see RxPipelineLock
 * \see RxLockedPipeSetEntryPoint
 * \see RxLockedPipeUnlock
 */
RxPipeline *
RxLockedPipeReplaceNode(RxPipeline       *pipeline,
                        RxPipelineNode   *node,
                        RxNodeDefinition *nodeDef)
{
    RWAPIFUNCTION(RWSTRING("RxLockedPipeReplaceNode"));

    RWASSERT(RxPipelineInstanced);
    RWASSERT(NULL  != pipeline);
    RWASSERT(FALSE != pipeline->locked);
    RWASSERT(NULL  != node);
    RWASSERT(ISNODELIVE(node));
    RWASSERT(NULL  != nodeDef);

    if ((NULL  != pipeline)         &&
        (FALSE != pipeline->locked) &&
        (NULL  != node)             &&
        ISNODELIVE(node)            &&
        (NULL  != nodeDef))
    {
        RwUInt32        oldIndex, newIndex, commonOuts;
        RxPipelineNode *newNode;
        RwUInt32        i, j;

        /* Construct a new node */
        newNode = &(pipeline->nodes[pipeline->numNodes]);
        if (!_NodeCreate(pipeline, newNode, nodeDef))
        {
            RWRETURN((RxPipeline *)NULL);
        }

        /* Copy the output assignments from the predecessor
         * (if the new node has fewer outputs, only copy that
         * many, if it has more then set the extra ones to
         * '-1', i.e unassigned) */
        commonOuts = (newNode->numOutputs < node->numOutputs) ?
                     (newNode->numOutputs) :
                     (node->numOutputs);
        if (commonOuts > 0)
        {
            memcpy(newNode->outputs,
                   node->outputs,
                   commonOuts*sizeof(RwUInt32));
        }
        if (commonOuts < newNode->numOutputs)
        {
            memset(&(node->outputs[commonOuts]),
                   -1,
                   (newNode->numOutputs - commonOuts)*sizeof(RwUInt32));
        }

        /* Fix up all existing nodes that used to
         * point to 'node' to point at 'newNode' */
        newIndex = newNode - &(pipeline->nodes[0]);
        oldIndex = node    - &(pipeline->nodes[0]);
        for (i = 0;i < pipeline->numNodes;i++)
        {
            for (j = 0;j < pipeline->nodes[i].numOutputs;j++)
            {
                if (pipeline->nodes[i].outputs[j] == oldIndex)
                {
                    /* A node should never output to itself */
                    RWASSERT(i != newIndex);
                    pipeline->nodes[i].outputs[j] = newIndex;
                }
            }
        }

        /* Delete the old node (compacts the node, outputs and topsortdata
         * arrays whilst maintaining correct output indices) */
        PipelineNodeDestroy(node, pipeline);

        RWRETURN(pipeline);
    }

    RWRETURN((RxPipeline *)NULL);
}

/**
 * \ingroup rwcoregeneric
 * \ref RxLockedPipeDeleteNode
 * deletes the specified node from a locked pipeline.
 *  Paths into and out of the node are deleted.
 *
 * \param pipeline  A pointer to a locked pipeline
 * \param node  A pointer to the pipeline node to be deleted
 *
 * \return A pointer to the input pipeline on success, or NULL if there is an error
 *
 * \see RxPipelineNodeCloneDefinition
 * \see RxPipelineNodeFindInput
 * \see RxPipelineNodeFindOutputByIndex
 * \see RxPipelineNodeFindOutputByName
 * \see RxPipelineNodeReplaceCluster
 * \see RxPipelineNodeRequestCluster
 * \see RxLockedPipeAddFragment
 * \see RxLockedPipeAddPath
 * \see RxPipelineClone
 * \see RxPipelineInsertDebugNode
 * \see RxLockedPipeDeletePath
 * \see RxPipelineFindNodeByIndex
 * \see RxPipelineFindNodeByName
 * \see RxLockedPipeGetEntryPoint
 * \see RxPipelineLock
 * \see RxLockedPipeReplaceNode
 * \see RxLockedPipeSetEntryPoint
 * \see RxLockedPipeUnlock
 */
RxPipeline *
RxLockedPipeDeleteNode(RxPipeline *pipeline, RxPipelineNode *node)
{
    RwBool deletable;

    RWAPIFUNCTION(RWSTRING("RxLockedPipeDeleteNode"));

    RWASSERT(RxPipelineInstanced);
    RWASSERT(pipeline != NULL);
    RWASSERT(pipeline->locked);
    RWASSERT(node != NULL);
    RWASSERT(ISNODELIVE(node));

    deletable = ((pipeline != NULL) &&
                 (pipeline->locked != FALSE) &&
                 (node != NULL) &&
                 ISNODELIVE(node));

    if (deletable)
    {
        /* This sets all output indices pointing at the dead node
         * to '-1' (and compacts the node, outputs and topsortdata
         * arrays whilst maintaining correct output indices) */
        PipelineNodeDestroy(node, pipeline);

        RWRETURN(pipeline);
    }

    RWRETURN((RxPipeline *)NULL);
}

/**
 * \ingroup rwcoregeneric
 * \ref RxLockedPipeSetEntryPoint
 * sets a locked pipeline's entry point -
 * that is, the index of node that forms the head of the pipeline
 * graph, where pipeline processing will commence. After
 * \ref RxLockedPipeUnlock, the specified node will be the first node in the
 * flattened node array.
 *
 * \note You should not normally need to call this function. Each
 * pipeline has one entry point and dependency-chasing should identify
 * it unambiguously. This allows the decision to be overridden.
 *
 * \param pipeline  A pointer to a locked pipeline
 * \param in  A pointer to the pipeline node to set as the pipeline's entry point
 *
 * \return A pointer to the input pipeline on success, or NULL if there is an error
 *
 * \see RxPipelineNodeCloneDefinition
 * \see RxPipelineNodeFindInput
 * \see RxPipelineNodeFindOutputByIndex
 * \see RxPipelineNodeFindOutputByName
 * \see RxPipelineNodeReplaceCluster
 * \see RxPipelineNodeRequestCluster
 * \see RxLockedPipeAddFragment
 * \see RxLockedPipeAddPath
 * \see RxPipelineClone
 * \see RxPipelineInsertDebugNode
 * \see RxLockedPipeDeleteNode
 * \see RxLockedPipeDeletePath
 * \see RxPipelineFindNodeByIndex
 * \see RxPipelineFindNodeByName
 * \see RxLockedPipeGetEntryPoint
 * \see RxPipelineLock
 * \see RxLockedPipeReplaceNode
 * \see RxLockedPipeUnlock
 */
RxPipeline         *
RxLockedPipeSetEntryPoint(RxPipeline * pipeline, RxPipelineNode * in)
{
    RWAPIFUNCTION(RWSTRING("RxLockedPipeSetEntryPoint"));

    RWASSERT(RxPipelineInstanced);
    RWASSERT(pipeline != NULL);
    RWASSERT(pipeline->locked);
    RWASSERT(in != NULL);
    RWASSERT(ISNODELIVE(in));

    if (pipeline != NULL && pipeline->locked && in != NULL
        && ISNODELIVE(in))
    {
        const RwUInt32 nodeIndex = PipelineNode2Index(pipeline, in);

        if (nodeIndex != (RwUInt32)-1)
        {
            pipeline->entryPoint = nodeIndex;

            RWRETURN(pipeline);
        }
    }

    RWRETURN((RxPipeline *)NULL);
}

/**
 * \ingroup rwcoregeneric
 * \ref RxLockedPipeGetEntryPoint
 * gets the locked pipeline's entry
 * point - that is, the pipeline node that forms the head of the pipeline
 * graph, where pipeline processing will commence. After
 * \ref RxLockedPipeUnlock, the specified node will be the first node in the
 * flattened node array.
 *
 * \param pipeline  A pointer to a locked pipeline
 *
 * \return A pointer to the entry-point pipeline node on success,
 * NULL otherwise.
 *
 * \see RxPipelineNodeCloneDefinition
 * \see RxPipelineNodeFindInput
 * \see RxPipelineNodeFindOutputByIndex
 * \see RxPipelineNodeFindOutputByName
 * \see RxPipelineNodeReplaceCluster
 * \see RxPipelineNodeRequestCluster
 * \see RxLockedPipeAddFragment
 * \see RxLockedPipeAddPath
 * \see RxPipelineClone
 * \see RxPipelineInsertDebugNode
 * \see RxLockedPipeDeleteNode
 * \see RxLockedPipeDeletePath
 * \see RxPipelineFindNodeByIndex
 * \see RxPipelineFindNodeByName
 * \see RxPipelineLock
 * \see RxLockedPipeReplaceNode
 * \see RxLockedPipeSetEntryPoint
 * \see RxLockedPipeUnlock
 */
RxPipelineNode *
RxLockedPipeGetEntryPoint(RxPipeline *pipeline)
{
    RxPipelineNode *entryNode;

    RWAPIFUNCTION(RWSTRING("RxLockedPipeGetEntryPoint"));

    RWASSERT(RxPipelineInstanced);
    RWASSERT(NULL  != pipeline);
    RWASSERT(FALSE != pipeline->locked);

    entryNode = (RxPipelineNode *)NULL;

    if ((NULL  != pipeline) && (FALSE != pipeline->locked))
    {
        if (pipeline->entryPoint != (RwUInt32)-1)
        {
            entryNode = RxPipelineFindNodeByIndex(
                            pipeline, pipeline->entryPoint);
        }
    }

    RWRETURN(entryNode);
}

/**
 * \ingroup rwcoregeneric
 * \ref RxLockedPipeAddPath
 * adds a path in the specified locked pipeline,
 * between the given \ref RxNodeOutput and the given \ref RxNodeInput.
 *
 * \param pipeline  A pointer to a locked pipeline
 * \param out A reference to a pipeline node output
 * \param in A reference to a pipeline node input
 *
 * \return A pointer to the input pipeline on success, or NULL if it fails
 *
 * \see RxPipelineNodeCloneDefinition
 * \see RxPipelineNodeFindInput
 * \see RxPipelineNodeFindOutputByIndex
 * \see RxPipelineNodeFindOutputByName
 * \see RxPipelineNodeReplaceCluster
 * \see RxPipelineNodeRequestCluster
 * \see RxLockedPipeAddFragment
 * \see RxPipelineClone
 * \see RxPipelineInsertDebugNode
 * \see RxLockedPipeDeleteNode
 * \see RxLockedPipeDeletePath
 * \see RxPipelineFindNodeByIndex
 * \see RxPipelineFindNodeByName
 * \see RxLockedPipeGetEntryPoint
 * \see RxPipelineLock
 * \see RxLockedPipeReplaceNode
 * \see RxLockedPipeSetEntryPoint
 * \see RxLockedPipeUnlock
 */
RxPipeline         *
RxLockedPipeAddPath(RxPipeline *pipeline, RwUInt32 *out, /* A reference to a pipeline node output */
                    RxPipelineNode *in) /* A reference to a pipeline node input */
{
    RWAPIFUNCTION(RWSTRING("RxLockedPipeAddPath"));

    RWASSERT(RxPipelineInstanced);
    RWASSERT(NULL != pipeline);
    RWASSERT(FALSE != pipeline->locked);
    RWASSERT(NULL != out);
    RWASSERT((RwUInt32)-1 == *out);
    RWASSERT(NULL != in);
    RWASSERT(ISNODELIVE(in));

    if ((NULL != pipeline) && (FALSE != pipeline->locked) &&
        (NULL != out)      && ((RwUInt32)-1 == *out)      &&
        (NULL != in)       && ISNODELIVE(in))
    {
        const RwUInt32 nodeIndex = PipelineNode2Index(pipeline, in);

        if (nodeIndex != (RwUInt32)-1)
        {
            *out = nodeIndex;

            RWRETURN(pipeline);
        }
    }

    if (!ISNODELIVE(in))
    {
        RWMESSAGE(("Node has been deleted from pipeline"));
    }
    if (*out != (RwUInt32)-1)
    {
        RWMESSAGE(("Path already in use, use RxLockedPipeDeletePath()"));
    }

    RWRETURN((RxPipeline *)NULL);
}

/**
 * \ingroup rwcoregeneric
 * \ref RxLockedPipeDeletePath
 * deletes a path in the specified
 * locked pipeline, between the given \ref RxNodeOutput and the given
 * \ref RxNodeInput.
 *
 * \param pipeline  A pointer to a locked pipeline
 * \param out A reference to a pipeline node output
 * \param in A reference to a pipeline node input
 *
 * \return A pointer to the pipeline on success, NULL otherwise.
 *
 * \see RxPipelineNodeCloneDefinition
 * \see RxPipelineNodeFindInput
 * \see RxPipelineNodeFindOutputByIndex
 * \see RxPipelineNodeFindOutputByName
 * \see RxPipelineNodeReplaceCluster
 * \see RxPipelineNodeRequestCluster
 * \see RxLockedPipeAddFragment
 * \see RxLockedPipeAddPath
 * \see RxPipelineClone
 * \see RxPipelineInsertDebugNode
 * \see RxLockedPipeDeleteNode
 * \see RxPipelineFindNodeByIndex
 * \see RxPipelineFindNodeByName
 * \see RxLockedPipeGetEntryPoint
 * \see RxPipelineLock
 * \see RxLockedPipeReplaceNode
 * \see RxLockedPipeSetEntryPoint
 * \see RxLockedPipeUnlock
 */
RxPipeline *
RxLockedPipeDeletePath(RxPipeline     *pipeline,
                       RwUInt32       *out, /* A reference to a pipeline node output */
                       RxPipelineNode *in)  /* A reference to a pipeline node input */
{
    RWAPIFUNCTION(RWSTRING("RxLockedPipeDeletePath"));

    RWASSERT(RxPipelineInstanced);
    RWASSERT(NULL  != pipeline);
    RWASSERT(FALSE != pipeline->locked);
    RWASSERT(NULL  != out);
    RWASSERT(NULL  != in);
    RWASSERT(&(pipeline->nodes[*out]) == in);

    if ((NULL != pipeline) && (FALSE != pipeline->locked) &&
        (NULL != out)      && (NULL  != in)               &&
        (&(pipeline->nodes[*out]) == in))
    {
        *out = (RwUInt32)-1;

        RWRETURN(pipeline);
    }

    RWRETURN((RxPipeline *)NULL);
}

/**
 * \ingroup rwcoregeneric
 * \ref RxPipelineInsertDebugNode
 * inserts a 'debug' pipeline node
 * inbetween two specified pipeline nodes.
 *
 * For all clusters active in either of the two specified pipeline
 * nodes, the added pipeline node will have rxCLREQ_REQUIRED specified
 * for them on input and rxCLVALID_VALID on output.
 *
 * Debug nodes are constructed to monitor traffic by "packet sniffing".
 *
 * \param pipeline  A pointer to an unlocked pipeline
 * \param before  A pointer to a pipeline node to feed into the debug node
 * \param after  A pointer to a pipeline node for the debug node to feed into
 * \param debugNodeDef  A pointer to the node definition for the debug node
 *
 * \return Input pipeline on success, or NULL if there is an error
 *
 * \see RxPipelineNodeCloneDefinition
 * \see RxPipelineNodeFindInput
 * \see RxPipelineNodeFindOutputByIndex
 * \see RxPipelineNodeFindOutputByName
 * \see RxPipelineNodeReplaceCluster
 * \see RxPipelineNodeRequestCluster
 * \see RxLockedPipeAddFragment
 * \see RxLockedPipeAddPath
 * \see RxPipelineClone
 * \see RxLockedPipeDeleteNode
 * \see RxLockedPipeDeletePath
 * \see RxPipelineFindNodeByIndex
 * \see RxPipelineFindNodeByName
 * \see RxLockedPipeGetEntryPoint
 * \see RxPipelineLock
 * \see RxLockedPipeReplaceNode
 * \see RxLockedPipeSetEntryPoint
 * \see RxLockedPipeUnlock
 */
RxPipeline         *
RxPipelineInsertDebugNode(RxPipeline * pipeline,
                          RxPipelineNode * before,
                          RxPipelineNode * after,
                          RxNodeDefinition * debugNodeDef)
{
    RWAPIFUNCTION(RWSTRING("RxPipelineInsertDebugNode"));

    RWASSERT(RxPipelineInstanced);
    RWASSERT(pipeline);
    RWASSERT(before || after);
    RWASSERT(debugNodeDef);
    if ((pipeline != NULL) && ((before != NULL) || (after != NULL))
        && (debugNodeDef != NULL))
    {
        RxLockedPipe       *lpipe = (RxPipeline *)NULL;
        RxPipelineNode     *debugNode = (RxPipelineNode *)NULL;
        RxClusterDefinition **clusterRefs = (RxClusterDefinition **)NULL;
        RwUInt32            i, j, numClustersToRequest;

        /* after is the 'outputNum'th output of before :) */
        RwUInt32 outputNum = (RwUInt32)-1;

        /* index of after in the pipeline's pipelinenode array */
        RwUInt32 afterNum  = (RwUInt32)-1;

        /* index of the debug node after it's been added */
        RwUInt32 debugNum  = (RwUInt32)-1;

        if (before == NULL)
        {
            if (after != (&pipeline->nodes[0]))
            {
                RwDebugSendMessage(rwDEBUGMESSAGE, "Debug node insert",
                                   "before node NULL, but after node not the start of the pipeline");
                RWRETURN(FALSE);
            }
        }
        else
        {
            /* Ensure before is a member of this pipeline */
            if (PipelineNode2Index(pipeline, before) == (RwUInt32)-1)
            {
                RwDebugSendMessage(rwDEBUGMESSAGE, "Debug node insert",
                                   "before node not a member of specified pipeline!");
                RWRETURN(FALSE);
            }
        }

        if (after == NULL)
        {
            if (before->numOutputs != 0)
            {
                RwDebugSendMessage(rwDEBUGMESSAGE, "Debug node insert",
                                   "after node NULL, but before node not a terminal node");
                RWRETURN(FALSE);
            }
        }
        else
        {
            /* Find after's position in pipeline's array of pipeline
             * nodes and cache it in afterNum
             */
            afterNum = PipelineNode2Index(pipeline, after);
            if (afterNum == (RwUInt32)-1)
            {
                RwDebugSendMessage(rwDEBUGMESSAGE, "Debug node insert",
                                   "after node not a member of specified pipeline!");
                RWRETURN(FALSE);
            }
        }

        if (before && after)
        {
            /* Find which output of before outputs to after and
             * cache it in outputNum */
            for (i = 0; i < before->numOutputs; i++)
            {
                if (before->outputs[i] == afterNum)
                {
                    outputNum = i;
                    i = before->numOutputs;
                }
            }
            if (outputNum == (RwUInt32)-1)
            {
                RwDebugSendMessage(rwDEBUGMESSAGE, "Debug node insert",
                                   "after node not an output of before node");
                RWRETURN(FALSE);
            }
        }

        /* Find out which clusters are active during before and after
         * and make sure the debug Node requests them (do this now cos
         * this info disappears when you lock - it's still valid until
         * you change the pipeline. Luckily for us we don't care about
         * the *order* of the clusters in their slots) */
        if ((clusterRefs = (RxClusterDefinition **)
             RwMalloc(2 * pipeline->packetNumClusterSlots *
                      sizeof(RxClusterDefinition *),
                      rwMEMHINTDUR_FUNCTION | rwID_PIPEMODULE)) == NULL)
        {
            RWERROR((E_RW_NOMEM, (2 * pipeline->packetNumClusterSlots *
                                  sizeof(RxClusterDefinition *))));
            RWRETURN(FALSE);
        }
        numClustersToRequest = 0;

        if (before)
        {
            for (i = 0; i < pipeline->packetNumClusterSlots; i++)
            {
                if (before->slotClusterRefs[i])
                {
                    RxClusterDefinition *newClusterRef =
                        before->slotClusterRefs[i]->clusterRef;

                    for (j = 0; j < numClustersToRequest; j++)
                    {
                        if (clusterRefs[j] == newClusterRef)
                            j = numClustersToRequest + 1;
                    }
                    if (j == numClustersToRequest)
                    {
                        clusterRefs[j] = newClusterRef;
                        numClustersToRequest++;
                    }
                }
            }
        }
        if (after)
        {
            for (i = 0; i < pipeline->packetNumClusterSlots; i++)
            {
                if (after->slotClusterRefs[i])
                {
                    RxClusterDefinition *newClusterRef =
                        after->slotClusterRefs[i]->clusterRef;

                    for (j = 0; j < numClustersToRequest; j++)
                    {
                        if (clusterRefs[j] == newClusterRef)
                            j = numClustersToRequest + 1;
                    }
                    if (j == numClustersToRequest)
                    {
                        clusterRefs[j] = newClusterRef;
                        numClustersToRequest++;
                    }
                }
            }
        }

        lpipe = RxPipelineLock(pipeline);
        RWASSERT(lpipe != NULL);
        if (lpipe != NULL)
        {
            /* Add the debug node to the pipeline */
            lpipe =
                RxLockedPipeAddFragment(lpipe, &debugNum, debugNodeDef,
                                        NULL);
            debugNode = &(lpipe->nodes[debugNum]);

            /* Request all the clusters active
             * during, before and after for the debug node */
            for (i = 0; i < numClustersToRequest; i++)
            {
                RxPipelineNodeRequestCluster(pipeline, debugNode,
                                             clusterRefs[i]);
            }

            if (before)
            {
                /* Disconnect before and after from one another */
                RxLockedPipeDeletePath(lpipe,
                                       RxPipelineNodeFindOutputByIndex
                                       (before, outputNum), after);
                /* Connect before to the debug node */
                RxLockedPipeAddPath(lpipe,
                                    RxPipelineNodeFindOutputByIndex
                                    (before, outputNum), debugNode);
            }
            if (after)
            {
                /* Connect the debug node to after */
                RxLockedPipeAddPath(lpipe,
                                    RxPipelineNodeFindOutputByIndex
                                    (debugNode, 0), after);
            }

            pipeline = RxLockedPipeUnlock(lpipe);

            if (!pipeline)
            {
                _rxPipelineDestroy(lpipe);
            }

            RwFree(clusterRefs);

            RWRETURN(pipeline);
        }
    }

    if (pipeline == NULL)
    {
        RwDebugSendMessage(rwDEBUGMESSAGE, "Debug node insert",
                           "pipeline pointer NULL");
    }
    if (debugNodeDef == NULL)
    {
        RwDebugSendMessage(rwDEBUGMESSAGE, "Debug node insert",
                           "debug node definition pointer NULL");
    }
    if ((before == NULL) && (after == NULL))
    {
        RwDebugSendMessage(rwDEBUGMESSAGE, "Debug node insert",
                           "both before and after node pointers NULL");
    }
    RWRETURN((RxPipeline *)NULL);
}

#if (0)

/*****************************************************************************
 DefinePipeExercise
*/

#include "p2stdcls.h"


static void
DefinePipeExercise(void)
{
    RWFUNCTION(RWSTRING("DefinePipeExercise"));

    static RxNodeDefinition Trevor = {
        "Trevor.csl",           /* Name */
        {(NodeBodyFn) NULL, (NodeInitFn) NULL, (NodeTermFn) NULL,
         (PipelineNodeInitFn) NULL, (PipelineNodeTermFn) NULL}, /* nodemethods */
        {(RwUInt32) 0, (RxClusterRef *) NULL,
         (RxClusterValidityReq *) NULL, (RwUInt32) 1,
         (RxOutputSpec *) NULL}, /* Io */
        (RwUInt32) 0,          /* PipelineNodePrivateDataSize */
        rxNODEDEFCONST,        /* Editable */
        (RwInt32) 0            /* inpipes */
    };
    static RxNodeDefinition James = {
        "James.csl",            /* Name */
        {(NodeBodyFn) NULL, (NodeInitFn) NULL, (NodeTermFn) NULL,
         (PipelineNodeInitFn) NULL, (PipelineNodeTermFn) NULL}, /* nodemethods */
        {(RwUInt32) 0, (RxClusterRef *) NULL,
         (RxClusterValidityReq *) NULL, (RwUInt32) 1,
         (RxOutputSpec *) NULL}, /* Io */
        (RwUInt32) 0,          /* PipelineNodePrivateDataSize */
        rxNODEDEFCONST,        /* Editable */
        (RwInt32) 0            /* inpipes */
    };
    static RxNodeDefinition Horace = {
        "Horace.csl",           /* Name */
        {(NodeBodyFn) NULL, (NodeInitFn) NULL, (NodeTermFn) NULL,
         (PipelineNodeInitFn) NULL, (PipelineNodeTermFn) NULL}, /* nodemethods */
        {(RwUInt32) 0, (RxClusterRef *) NULL,
         (RxClusterValidityReq *) NULL, (RwUInt32) 0,
         (RxOutputSpec *) NULL}, /* Io */
        (RwUInt32) 0,          /* PipelineNodePrivateDataSize */
        rxNODEDEFCONST,        /* Editable */
        (RwInt32) 0            /* inpipes */
    };
    static RxNodeDefinition Ahab = {
        "Ahab.csl",             /* Name */
        {(NodeBodyFn) NULL, (NodeInitFn) NULL, (NodeTermFn) NULL,
         (PipelineNodeInitFn) NULL, (PipelineNodeTermFn) NULL}, /* nodemethods */
        {(RwUInt32) 0, (RxClusterRef *) NULL,
         (RxClusterValidityReq *) NULL, (RwUInt32) 0,
         (RxOutputSpec *) NULL}, /* Io */
        (RwUInt32) 0,          /* PipelineNodePrivateDataSize */
        rxNODEDEFCONST,        /* Editable */
        (RwInt32) 0            /* inpipes */
    };
    RxPipeline         *pipeline;
    RxLockedPipe       *lckp;
    RxPipelineNode     *trev, *jim;

    RxPipelineNode /* RxNodeDefinition */  * horahab;

    pipeline = RxPipelineCreate();

    lckp = RxPipelineLock(pipeline);

    /* NB/ return value from AddFragment() is a NodeRef & can be used in
     * same manner as in prototype */
    RxLockedPipeAddFragment(lckp, NULL, &Trevor, &James, &Horace, NULL);

    horahab = RxPipelineFindNodeByName(lckp, Horace.name, NULL, NULL);
    RxLockedPipeReplaceNode(lckp, horahab, &Ahab); /* Ahab replaces Horace */

    trev = RxPipelineFindNodeByName(lckp, Trevor.name, NULL, NULL);
    jim = RxPipelineFindNodeByName(lckp, James.name, NULL, NULL);

    RxLockedPipeDeletePath(lckp,
                           RxPipelineNodeFindOutputByIndex(trev, 0),
                           RxPipelineNodeFindInput(jim)); /* delete Trevor -> James */
    RxLockedPipeAddPath(lckp, RxPipelineNodeFindOutputByIndex(trev, 0),
                        RxPipelineNodeFindInput(horahab)); /* connect Trevor -> Ahab */
    RxLockedPipeDeletePath(lckp,
                           RxPipelineNodeFindOutputByIndex(jim, 0),
                           RxPipelineNodeFindInput(horahab)); /* delete James -> Ahab */
    RxLockedPipeAddPath(lckp, RxPipelineNodeFindOutputByIndex(jim, 0),
                        RxPipelineNodeFindInput(trev)); /* connect James -> Trevor */

    /* as Iestyn has pointed out, entry pt. can be determined by top. sort,
     * but having user explicitly set is good for clarity & sanity checking */
    RxLockedPipeSetEntryPoint(lckp, RxPipelineNodeFindInput(jim)); /* now pipeline reads James -> Trevor -> Ahab */

    RxLockedPipeDeleteNode(lckp, trev); /* now pipeline reads James    Ahab */
    RxLockedPipeAddPath(lckp, RxPipelineNodeFindOutputByIndex(jim, 0),
                        RxPipelineNodeFindInput(horahab));

    RxLockedPipeUnlock(lckp);

    _rxPipelineDestroy(pipeline);

    RWRETURNVOID();
}

#endif /* (0) */

/*****************************************************************************/

#if defined (__MWERKS__)
#if (defined(RWVERBOSE))
#pragma message (__FILE__ "/" _SKY_EXPAND(__LINE__) ": __MWERKS__ == " _SKY_EXPAND(__MWERKS__))
#endif /* (defined (__MWERKS__)) */
#if (__option (global_optimizer))
#pragma always_inline on
#endif /* (__option (global_optimizer)) */
#endif /*  defined (__MWERKS__) */

