/**
 *
 * Macros for execution of custom pipelines 
 *
 * Copyright (c) Criterion Software Limited
 */

/****************************************************************************
 *                                                                          *
 *  Module  :   p2altmdl.h                                                  *
 *                                                                          *
 *  Purpose :   See p2altmdl.c                                              *
 *                                                                          *
 ****************************************************************************/

#ifndef P2ALTMDL_H
#define P2ALTMDL_H

#include "p2core.h"


/*************************************************************
 * Local Defines
 */

#if (defined(DEBUGGINGPOWERPIPE) && defined(RWDEBUG))

#define INFORM(args)                                                        \
MACRO_START                                                                 \
{                                                                           \
    const RwChar *str = _rwdbsprintf args ;                                 \
    RwDebugSendMessage(rwDEBUGMESSAGE, "", str);                            \
    fprintf(stdout, "%s\n", str);                                           \
    fflush(stdout);                                                         \
}                                                                           \
MACRO_STOP

#endif /* (defined(DEBUGGINGPOWERPIPE) && defined(RWDEBUG)) */

#if (!defined(INFORM))
#define INFORM(args) /* No op*/
#endif /* (!defined(INFORM)) */

#define CLUSTERZERO(cl)                                                     \
MACRO_START                                                                 \
{                                                                           \
    RWASSERT( offsetof(RxCluster, data) == sizeof(RwUInt32) );              \
    /* flags, stride */     *(RwUInt32 *) &(cl)->flags = 0U;                \
    /* data */              (cl)->data = NULL;                              \
    /* currentData */                                                       \
    /* numAlloced */        (cl)->numAlloced = 0U;                          \
    /* numUsed */           (cl)->numUsed    = 0U;                          \
    /* clusterRef */        (cl)->clusterRef = (RxPipelineCluster *)NULL;   \
    /* attributes */                                                        \
}                                                                           \
MACRO_STOP

#define CLUSTERTERMINATE(cl)                                                \
MACRO_START                                                                 \
{                                                                           \
    if ( (cl)->clusterRef != (RxPipelineCluster *)NULL )                    \
    {                                                                       \
        if ( (cl)->data != NULL &&                                          \
             !((cl)->flags & rxCLFLAGS_EXTERNAL) )                          \
        {                                                                   \
            RxHeapFree(_rxHeapGlobal, (cl)->data);                          \
        }                                                                   \
                                                                            \
        CLUSTERZERO(cl);                                                    \
    }                                                                       \
}                                                                           \
MACRO_STOP

#define EMBEDDEDPACKETBETWEENNODESMACRO(result, pipeline, nodeFrom, Index)  \
MACRO_START                                                                 \
{                                                                           \
    RwUInt32             outputIndex = nodeFrom->outputs[Index];            \
                                                                            \
    if (outputIndex != (RwUInt32) - 1)                                      \
    {                                                                       \
        RxPacket           *packet;                                         \
        RxPipelineNode     *nodeTo;                                         \
        RwUInt32           outputMask;                                      \
                                                                            \
        nodeTo = &pipeline->nodes[outputIndex];                             \
                                                                            \
        /* Don't assume there's an INUSE/PENDING packet */                  \
        if (pipeline->embeddedPacketState > rxPKST_UNUSED)                  \
        {                                                                   \
            /* terminate clusters,                             */           \
            /* load new inputToClusterSlot & slotsContinue map */           \
            outputMask = 1U << Index;                                       \
            packet = pipeline->embeddedPacket;                              \
            /* The first entry in slotsContinue is the AND of the rest of */\
            /* the entries. If, for a given bit, this is '1', then ALL    */\
            /* clusters continue for the output represented by that bit   */\
            if (!(packet->slotsContinue[0] & outputMask))                   \
            {                                                               \
                RwUInt32            n = packet->numClusters;                \
                                                                            \
                do                                                          \
                {                                                           \
                    if (!(packet->slotsContinue[n] & outputMask))           \
                    {                                                       \
                        CLUSTERTERMINATE(&packet->clusters[n - 1]);         \
                    }                                                       \
                }                                                           \
                while (--n);                                                \
            }                                                               \
                                                                            \
            packet->inputToClusterSlot = nodeTo->inputToClusterSlot;        \
            packet->slotsContinue = nodeTo->slotsContinue;                  \
            packet->slotClusterRefs = nodeTo->slotClusterRefs;              \
                                                                            \
            pipeline->embeddedPacketState = rxPKST_PENDING;                 \
        }                                                                   \
                                                                            \
        result = nodeTo;                                                    \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        result = (RxPipelineNode *)NULL;                                    \
    }                                                                       \
}                                                                           \
MACRO_STOP

#define _rxExecuteNodeMacro(_pipeline, _node, _params)                      \
MACRO_START                                                                 \
{                                                                           \
    RwUInt32                exitCode;                                       \
    const RxNodeDefinition *nodeDef;                                        \
    /* Once a node has requested termination, */                            \
    /* no further executes will be performed  */                            \
    nodeDef = (_node)->nodeDef;                                             \
    RWASSERT(NULL != nodeDef);                                              \
                                                                            \
    INFORM(("executing %s\n", nodeDef->name));                              \
                                                                            \
    exitCode = nodeDef->nodeMethods.nodeBody(                               \
                   (_node), (_params));                                     \
                                                                            \
    INFORM(("returned\n"));                                                 \
                                                                            \
    /* Caution!                              */                             \
    /* don't overwrite a terminate code from */                             \
    /* a node further down the pipeline with */                             \
    /* a non-terminate one!                  */                             \
    if (FALSE == exitCode)                                                  \
    {                                                                       \
        _rxExecCtxGlobal.exitCode = exitCode;                               \
    }                                                                       \
                                                                            \
    /* node returns only in these circumstances: */                         \
    /* pipeline execution complete,              */                         \
    /* no packets generated,                     */                         \
    /* unfetched input                           */                         \
    if ( (_pipeline)->embeddedPacketState > rxPKST_UNUSED                   \
         /* !UNUSED and !PACKETLESS */ )                                    \
    {                                                                       \
        (_pipeline)->embeddedPacketState = rxPKST_INUSE;                    \
        _rxPacketDestroy((_pipeline)->embeddedPacket);                      \
    }                                                                       \
}                                                                           \
MACRO_STOP

#define _rxPacketCreateMacro(_forPipeline)                                  \
    (   (_forPipeline)->embeddedPacketState = rxPKST_INUSE,                 \
        (_forPipeline)->embeddedPacket)

/****************************************************************************
 * Global Prototypes
 */

extern RxHeap      *_rxHeapGlobal;

/* Used in p2altmdl.c */
#if (defined(RWDEBUG))

extern RxPacket *_rxPacketCreate(RxPipeline *forPipeline);

#else /* (defined(RWDEBUG)) */

#define _rxPacketCreate(_forPipeline)    _rxPacketCreateMacro(_forPipeline)

#endif /* (defined(RWDEBUG)) */

#endif /* P2ALTMDL_H */

