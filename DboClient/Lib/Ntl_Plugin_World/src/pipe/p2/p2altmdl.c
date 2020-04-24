/*
 * Pipeline execution functionailty for custom pipelines
 *
 * Copyright (c) Criterion Software Limited
 */
/****************************************************************************
 *                                                                          *
 *  Module  :   p2altmdl.c                                                  *
 *                                                                          *
 *  Purpose :   Pipeline II pipe-execution functionality                    *
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
#include "p2heap.h"
#include "p2define.h"
#include "p2macros.h"

#include "p2altmdl.h"

/****************************************************************************
 Local Defines
 */

#define SPEWx

#ifdef SPEW

#include <assert.h>

#if (defined(_MSC_VER))
#include <windows.h>
#include <crtdbg.h>            /* Pick up _ASSERTE macro */

#define P2ASSERTE(_condition) _ASSERTE(_condition)
#endif /* (defined(_MSC_VER)) */

#if (!defined(P2ASSERTE))
#define P2ASSERTE(_condition)  /* No-Op */
#endif /* (!defined(P2ASSERTE)) */

#undef RWAPIFUNCTION
#define RWAPIFUNCTION(str)   INFORM((str))
#undef RWFUNCTION
#define RWFUNCTION(str)      INFORM((str))
#undef RWASSERT
#define RWASSERT(assertion)                     \
    MACRO_START                                 \
    {                                           \
        P2ASSERTE(assertion);                   \
        assert(assertion);                      \
    }                                           \
    MACRO_STOP

#undef RWRETURN
#define RWRETURN(value)      return(value)
#undef RWRETURNVOID
#define RWRETURNVOID()       return

#endif /* SPEW */

#if (!defined(INFORM))
#define INFORM(args)           /* No op */
#endif /* (!defined(INFORM)) */


/****************************************************************************
 Globals (across program)
 */

RxExecutionContext  _rxExecCtxGlobal;
/* single global execution context */


/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Functions

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**********************************************************************
 * rxPacketCrossPipelineAlignClusters()
 */
static void
rxPacketCrossPipelineAlignClusters(RxPacket * packet,
                                   RxPipeline * toPipeline,
                                   RxPacket * oldPacket)
{
    RwUInt32            i, j;

    RWFUNCTION(RWSTRING("rxPacketCrossPipelineAlignClusters"));

    /* expecting to use an alignment cache here [with same rather
     * funky cache mechanism as the old PL2PipelineGetForeignSlotMap()
     * stuff (see old revisions of the file in MKS)], which will
     * allow me to iterate over _packet_ clusters pulling
     * corresponding cluster from _oldPacket_ */

    for (i = 0; i < toPipeline->numInputRequirements; i++)
    {
        RxPipelineRequiresCluster *reqdCluster =
            &toPipeline->inputRequirements[i];

        for (j = 0; j < oldPacket->numClusters; j++)
        {
            /* match...? then break */
            if ((oldPacket->clusters[j].clusterRef != NULL) &&
                (oldPacket->clusters[j].clusterRef->clusterRef
                   == reqdCluster->clusterDef))
            {
                break;
            }
        }

        /* so... what was the result of all that, then? */
        if (!(j == oldPacket->numClusters /* tried all clusters */ ||
            oldPacket->clusters[j].data ==
              NULL /* found corresponding cluster but empty */ ))
        {
            /* match */
            /* copy... */
            packet->clusters[reqdCluster->slotIndex] =
                oldPacket->clusters[j];

            /* ... and update clusterRef */
            packet->clusters[reqdCluster->slotIndex].clusterRef =
                toPipeline->nodes[0].slotClusterRefs[reqdCluster->slotIndex];

            /* ... and prevent _rxPacketDestroy from freeing
             * associated memory! */
            oldPacket->clusters[j].data = NULL;
        }
#if (defined(RWDEBUG))
        else
        {                      /* no match - did it matter? */

            RwBool PacketBetweenPipelines_required_cluster_present =
                (reqdCluster->rqdOrOpt != rxCLREQ_REQUIRED);

            RWASSERT(PacketBetweenPipelines_required_cluster_present);

        }
#endif /* (defined(RWDEBUG))*/

    }

    RWRETURNVOID();
}

/**********************************************************************
 * _rxEmbeddedPacketBetweenPipelines()
 */
void
_rxEmbeddedPacketBetweenPipelines(RxPipeline * fromPipeline,
                                 RxPipeline * toPipeline)
{
    RWFUNCTION(RWSTRING("_rxEmbeddedPacketBetweenPipelines"));

    /* Bear in mind that RxPacketDispatch() will clean up
     * in toPipeline, but not in from */

    /* Don't assume there's an INUSE/PENDING packet */
    if (fromPipeline->embeddedPacketState > rxPKST_UNUSED)
    {
        RxPacket *oldPacket = fromPipeline->embeddedPacket;
        RxPacket *newPacket = toPipeline->embeddedPacket;

        newPacket = toPipeline->embeddedPacket;

        rxPacketCrossPipelineAlignClusters(newPacket, toPipeline, oldPacket);

        _rxPacketDestroy(oldPacket);

        newPacket->slotClusterRefs = toPipeline->nodes[0].slotClusterRefs;

        newPacket->inputToClusterSlot =
            toPipeline->nodes[0].inputToClusterSlot;
        newPacket->slotsContinue = toPipeline->nodes[0].slotsContinue;
        toPipeline->embeddedPacketState = rxPKST_PENDING;
    }

    RWRETURNVOID();
}

/**********************************************************************
 * _rxEmbeddedPacketBetweenNodes()
 */
RxPipelineNode *
_rxEmbeddedPacketBetweenNodes(RxPipeline * pipeline,
                             RxPipelineNode * nodeFrom,
                             RwUInt32 whichOutput)
{
    RxPipelineNode *result;

    RWFUNCTION(RWSTRING("_rxEmbeddedPacketBetweenNodes"));

    EMBEDDEDPACKETBETWEENNODESMACRO(result, pipeline, nodeFrom, whichOutput);

    RWRETURN(result);
}

/**********************************************************************
 * _rxPacketCreate()
 */
#ifdef RWDEBUG
RxPacket *
_rxPacketCreate(RxPipeline * forPipeline)
{
    RWFUNCTION(RWSTRING("_rxPacketCreate"));

    /* We shouldn't be creating a packet if another exists */
    RWASSERT(forPipeline->embeddedPacketState <=
             rxPKST_UNUSED /* UNUSED or PACKETLESS */ );

    /* this packet has been comprehensively nulled
     * ('cept for numClusters) in theprocess of destruction */

    /* we rely entirely on LockWrite to set up
     * pipelineClusterRefs and clusterAttributes */

    RWRETURN(_rxPacketCreateMacro(forPipeline));
}
#endif /* RWDEBUG */

/**********************************************************************
 * _rxPacketDestroy()
 */
void
_rxPacketDestroy(RxPacket * packet)
{
    RxPipeline *pipeline;
    RwUInt32    n;
    RxCluster  *cl;

    RWFUNCTION(RWSTRING("_rxPacketDestroy"));

    RWASSERT(NULL != packet);

    pipeline = packet->pipeline;
    RWASSERT(NULL != pipeline);
    RWASSERT(packet == pipeline->embeddedPacket);

    RWASSERT(pipeline->embeddedPacketState == rxPKST_INUSE);
    pipeline->embeddedPacketState = rxPKST_UNUSED;

    /* zero non-empty clusters (& destroy data)
     * to ready packet for reuse */
    n = packet->numClusters;
    cl = &packet->clusters[0];

    do
    {
        CLUSTERTERMINATE(cl);
    }
    while (cl++, --n);

    packet->flags = (RwUInt16) 0U;

    RWRETURNVOID();
}

