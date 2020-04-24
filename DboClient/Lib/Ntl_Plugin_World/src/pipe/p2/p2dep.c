/*
 * Inter-node dependencies in custom pipelines
 *
 * Copyright (c) Criterion Software Limited
 */

/****************************************************************************
 *                                                                          *
 *  Module  :   dep.c                                                       *
 *                                                                          *
 *  Purpose :   See definepipe.c                                            *
 *                                                                          *
 ****************************************************************************/

/****************************************************************************
 Includes
 */

/* #include <assert.h> */
#include <float.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "batypes.h"
#include "balibtyp.h"
#include "badebug.h"
#include "bamemory.h"

#include "bapipe.h"

#include "p2resort.h"
#include "p2core.h"
#include "p2stdcls.h"
#include "p2define.h"
#include "p2dep.h"

/****************************************************************************
 Local Defines
 */

#define _PTRINC(_ptr, _type, _numbytes) \
   (_ptr = (_type *)(((RwUInt8 *)_ptr) + (_numbytes)) )

/* leave undefined to disable logging */
/* #define LOGFILE "depchase.log" */

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Functions

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

#define _ReqNumEntries(_req)   ( (_req)->numused )
#define _ReqGetEntry(_req, _n) ( ( ((RwUInt32) (_n)) < (_req)->numused ) ? \
                               &(_req)->reqentries[(_n)] :                 \
                               (RwReqEntry *) NULL )

static rxReq *
_ReqCreate(RxPipelineNode *node, RwUInt32 numClusters)
{
    rxReq *req;

    RWFUNCTION(RWSTRING("_ReqCreate"));

    req = (rxReq *)StalacTiteAlloc(sizeof(rxReq));
    if (req != NULL)
    {
        /* Now we Titealloc this to a known conservative size */
        req->reqentries = (RwReqEntry *)
            StalacTiteAlloc(numClusters*sizeof(RwReqEntry));
        if (NULL != req->reqentries)
        {
            req->numalloced = numClusters;
            req->numused = 0;
            req->node = node;
            req->unavailslots = 0U;

            RWRETURN(req);
        }
    }

    RWRETURN((rxReq *) NULL);
}

static RwReqEntry *
_ReqSearch4Cluster(rxReq *req, RxClusterDefinition *clusterDef)
{
    RWFUNCTION(RWSTRING("_ReqSearch4Cluster"));

    if (req->numused != 0)
    {
        RwReqEntry *reqentry = &(req->reqentries[0]);
        RwUInt32    n        =   req->numused;

        for (n = 0;n < req->numused;n++)
        {
            if (reqentry->clusterDef == clusterDef)
            {
                RWRETURN(reqentry);
            }
            reqentry++;
        }
    }

    RWRETURN((RwReqEntry *) NULL);
}

/* classic xxxAddEntry behaviour starts about 1/2-way down; preceding code
   checks whether a given cluster is already present, and if so
   upgrades ceOPTIONAL to ceREQUIRED if appropriate */

static RwReqEntry *
_ReqAddEntry(rxReq *req,
             RxClusterDefinition *clusterDef,
             RxClusterValidityReq required,
             RwUInt32 inputs,
             RxPipelineNode *originatingNode)
{
    RwReqEntry *reqentry;

    RWFUNCTION(RWSTRING("_ReqAddEntry"));

    reqentry = _ReqSearch4Cluster(req, clusterDef);

    if (reqentry != NULL)
    {
        if (required == rxCLREQ_REQUIRED) /* upgrade possible 'ceOPTIONAL's to 'ceREQUIRED' */
        {
            reqentry->required = rxCLREQ_REQUIRED;
            reqentry->originatingNode = originatingNode;
        }

        RWRETURN(reqentry);
    }

    /* numalloced is now just used to check we haven't overflowed
     * (should NOT be, EVER) the array we alloc'd in _ReqCreate() */
    RWASSERT(req->numused < req->numalloced);

    reqentry = &req->reqentries[req->numused];
    req->numused++;

    reqentry->clusterDef = clusterDef;
    reqentry->required = required;
    reqentry->inputs = inputs;
    reqentry->next = (RwReqEntry *)NULL;
    reqentry->req = req;
    reqentry->st = (RwScopeTrace *)NULL;
    reqentry->outbf = 0U;
    reqentry->assignedslot = (RwUInt32) - 1;
    reqentry->originatingNode = originatingNode;

    RWRETURN(reqentry);
}

static void
_ReqDeleteEntry(rxReq * req, RwReqEntry * reqentry)
{
    RWFUNCTION(RWSTRING("_ReqDeleteEntry"));

    /* copy last entry into gap */
    if (reqentry != &req->reqentries[req->numused - 1])
    {
        *reqentry = req->reqentries[req->numused - 1];
    }

    req->numused--;

    RWRETURNVOID();
}

static              RwUInt32
_IoSpecSearch4Cluster(RxIoSpec * iospec,
                      RxClusterDefinition * clusterDef)
{
    RwUInt32            i;

    RWFUNCTION(RWSTRING("_IoSpecSearch4Cluster"));

    for (i = 0; i < iospec->numClustersOfInterest; i++)
    {
        if (iospec->clustersOfInterest[i].clusterDef == clusterDef)
        {
            RWRETURN(i);
        }
    }

    RWRETURN((RwUInt32) ~ 0);
}

/* changed action: to achieve correct behaviour, we only ripple down to channel
   below if cluster dies */

static void
_PropDownElimPath(RxPipeline * pipeline, RxPipelineNode * node,
                  RxClusterDefinition * clusterDef)
{
    RwUInt32    i;
    RxIoSpec   *iospec = &node->nodeDef->io;
    RwReqEntry *reqentry = _ReqSearch4Cluster(node->topSortData->req, clusterDef);

    RWFUNCTION(RWSTRING("_PropDownElimPath"));

    if (reqentry == NULL)
    {
        RWRETURNVOID();
    }

    if (--reqentry->inputs != 0)
    {
        RWRETURNVOID();
    }

    _ReqDeleteEntry(node->topSortData->req, reqentry);

    for (i = 0; i < node->numOutputs; i++)
    {
        if (node->outputs[i] != (RwUInt32) ~ 0)
        {
            RxOutputSpec *outspec = &node->nodeDef->io.outputs[i];
            RwUInt32      n =
                _IoSpecSearch4Cluster(iospec, clusterDef);

            RxClusterValid presinout =
                (n == (RwUInt32) - 1) ?
                outspec->allOtherClusters : outspec->outputClusters[n];

            if (presinout == rxCLVALID_NOCHANGE)
            {
                _PropDownElimPath(pipeline,
                                  &pipeline->nodes[node->outputs[i]],
                                  clusterDef);
            }
        }
    }

    RWRETURNVOID();
}

static RwScopeTrace *
_ScopeTraceCreate(RwScopeTrace **headref)
{
    RwScopeTrace *scopetrace;

    RWFUNCTION(RWSTRING("_ScopeTraceCreate"));

    scopetrace = (RwScopeTrace *)StalacTiteAlloc(sizeof(RwScopeTrace));
    if (scopetrace != NULL)
    {
        scopetrace->continuation = (RwScopeTrace *)NULL;
        scopetrace->head = (RwReqEntry *)NULL;
        scopetrace->parent = (RwScopeTrace *)NULL;

        scopetrace->next = *headref;
        *headref = scopetrace;

        RWRETURN(scopetrace);
    }

    RWRETURN((RwScopeTrace *) NULL);
}

static void
_ScopeTraceAddEntry(RwScopeTrace * scopetrace, RwReqEntry * reqentry)
{
    RWFUNCTION(RWSTRING("_ScopeTraceAddEntry"));

    reqentry->next = scopetrace->head;
    scopetrace->head = reqentry;

    RWRETURNVOID();
}

static void
_ScopeTraceMerge(RwScopeTrace ** headref, RwScopeTrace * p,
                 RwScopeTrace * q)
{
    RwScopeTrace       *proot = p, *qroot = q;

    RWFUNCTION(RWSTRING("_ScopeTraceMerge"));

    /* merge p + previous p merges with q + previous q merges */

    while (proot->parent != NULL)
    {
        proot = proot->parent;
    }
    while (qroot->parent != NULL)
    {
        qroot = qroot->parent;
    }

    if (proot != qroot)
    {
        RwScopeTrace       *prootfinalpage = proot;

        while (prootfinalpage->continuation != NULL)
        {
            prootfinalpage = prootfinalpage->continuation;
        }

        prootfinalpage->continuation = qroot;

        /* qroot is now a child scope trace, so... */

        /* ... setup its parent pointer, so we can work back in the event of subsequent merges */

        qroot->parent = proot;

        /* ... find & nix its entry in the master list */

        while (*headref != qroot)
        {
            headref = &(*headref)->next;
        }

        *headref = (*headref)->next;
    }

    RWRETURNVOID();
}

#if (defined(LOGFILE))

static void
_ReqDump(FILE * fpLog, rxReq * req)
{
    RWFUNCTION(RWSTRING("_ReqDump"));

    if (req->numused != 0)
    {
        const char         *cluspresstrings[] = { "DONTWANT",
                                                  "REQUIRED",
                                                  "OPTIONAL"
        };
        RwUInt32            i = req->numused;
        RwReqEntry         *reqentry = &req->reqentries[0];

        do
        {
            fprintf(fpLog, "%-8.8s %s 0x%08X\n",
                    reqentry->clusterDef->name,
                    cluspresstrings[reqentry->required],
                    (unsigned int) reqentry->outbf);

            if (--i != 0)
            {
                fprintf(fpLog, "          ");
            }
        }
        while (reqentry++, i);
    }
    else
    {
        fprintf(fpLog, "\n");
    }

    RWRETURNVOID();
}

static void
_ReqsDump(RxPipeline * pipeline)
{
    FILE               *fpLog;

    RWFUNCTION(RWSTRING("_ReqsDump"));

    fpLog = fopen(LOGFILE, "at");

    if (pipeline->numNodes != 0)
    {
        RwUInt32            i = pipeline->numNodes;
        RxPipelineNode     *node = &pipeline->nodes[0];

        do
        {
            fprintf(fpLog, "%8.8s: ", node->nodeRef->name);
            _ReqDump(fpLog, node->topSortData->req);
        }
        while (node++, --i);
    }

    fclose(fpLog);

    RWRETURNVOID();
}

#define REQSDUMP(pipeline)  _ReqsDump(pipeline)

static void
_AllocsDump(RxPipeline * pipeline)
{
    FILE               *fpLog = fopen(LOGFILE, "at");

    RWFUNCTION(RWSTRING("_AllocsDump"));

    if (pipeline->numNodes != 0)
    {
        RwUInt32            i = pipeline->numNodes, j;
        RxPipelineNode     *node = &pipeline->nodes[0];

        do
        {
            fprintf(fpLog, "%8.8s:", node->nodeRef->name);

            for (j = 0; j < pipeline->packetNumClusterSlots; j++)
            {
                RxPipelineCluster  *clusterRef = node->slotClusterRefs[j];

                if (clusterRef != NULL)
                {
                    fprintf(fpLog, " %-8.8s",
                            clusterRef->clusterRef->name);
                }
                else
                {
                    fprintf(fpLog, " --------");
                }
            }

            for (j = 0; j < node->nodeRef->io.numClustersOfInterest;
                 j++)
            {
                RwUInt32            n = node->inputToClusterSlot[j];

                fprintf(fpLog, (n != (RwUInt32) ~ 0) ? " %02X" : " --",
                        n);
            }

            fprintf(fpLog, "\n");
        }
        while (node++, --i);
    }

    fclose(fpLog);

    RWRETURNVOID();
}

#define ALLOCSDUMP(pipeline) _AllocsDump(pipeline)

static void
_ScopeTracesDump(RwScopeTrace * head)
{
    RwScopeTrace       *st;
    FILE               *fpLog;

    RWFUNCTION(RWSTRING("_ScopeTracesDump"));

    st = head;
    fpLog = fopen(LOGFILE, "at");

    while (st != NULL)         /* outer loop moves through master list */
    {
        RwScopeTrace       *st2 = st;

        fprintf(fpLog, "%02X:", (unsigned int) st2->head->assignedslot);

        do
        {                      /* inner loop moves through continuations (created by merging) */
            RwReqEntry         *reqentry = st2->head;

            while (reqentry != NULL)
            {
                fprintf(fpLog, " %8.8s/%-8.8s",
                        reqentry->req->node->nodeRef->name,
                        reqentry->clusterDef->name);

                reqentry = reqentry->next;
            }

            st2 = st2->continuation;
        }
        while (st2 != NULL);

        st = st->next;

        fprintf(fpLog, "\n");
    }

    fclose(fpLog);

    RWRETURNVOID();
}

#define SCOPETRACESDUMP(head) _ScopeTracesDump(head)

#endif /* (defined(LOGFILE)) */

#if (!defined(REQSDUMP))
#define REQSDUMP(pipeline)     /* No op */
#endif /* (!defined(REQSDUMP)) */

#if (!defined(ALLOCSDUMP))
#define ALLOCSDUMP(pipeline)   /* No op */
#endif /* (!defined(ALLOCSDUMP)) */

#if (!defined(SCOPETRACESDUMP))
#define SCOPETRACESDUMP(head)  /* No op */
#endif /* (!defined(SCOPETRACESDUMP)) */

/*****************************************************************************/

#define E_RX_DEP_SUCCESS              0

static RwUInt32
_PropagateDependenciesAndKillDeadPaths(RxPipeline * pipeline)
{
    RwUInt32        numUniqueClusters;
    RwUInt32        i, j, k;
    RxPipelineNode *node;

    RWFUNCTION(RWSTRING("_PropagateDependenciesAndKillDeadPaths"));

    i = pipeline->numNodes;
    node = &pipeline->nodes[pipeline->numNodes - 1];

    /* loop over pipeline nodes (bottom -> top) */
    do
    {
        RxIoSpec *iospec = &node->nodeDef->io;

        /* ensure no ClusterDef is referenced twice
         * within a node and no ClusterDefs are NULL */
        for (j = 0;j < node->nodeDef->io.numClustersOfInterest;j++)
        {
            RxClusterDefinition *cluster =
                node->nodeDef->io.clustersOfInterest[j].clusterDef;
            if (cluster == NULL)
            {
                RWERROR((E_RX_DEP_NULLCLUSTERDEF, node->nodeDef->name, j));
                RWRETURN(E_RX_DEP_NULLCLUSTERDEF);
            }

            /* lovely O(n^2) loop, yum yum */
            for (k = j + 1;k < node->nodeDef->io.numClustersOfInterest;k++)
            {
                RxClusterDefinition *cluster2 =
                    node->nodeDef->io.clustersOfInterest[k].clusterDef;
                if (cluster == cluster2)
                {
                    RWERROR((E_RX_DEP_DUPLICATECLUSTERDEFS,
                             node->nodeDef->name, cluster2->name));
                    RWRETURN(E_RX_DEP_DUPLICATECLUSTERDEFS);
                }
            }
        }

        numUniqueClusters = PipelineCalcNumUniqueClusters(pipeline);
        if ((node->topSortData->req = _ReqCreate(node, numUniqueClusters)) == NULL)
        {
            RWERROR((E_RX_DEP_OUTOFMEMORY));
            RWRETURN(E_RX_DEP_OUTOFMEMORY);
        }

        /* inherited requirements ************************************************************** */
        for (j = 0; j < node->numOutputs; j++) /* loop over node outputs */
        {
            if (node->outputs[j] != (RwUInt32) -1)
            {
                RxPipelineNode *outnode = &pipeline->nodes[node->outputs[j]];
                RxOutputSpec   *outspec = &node->nodeDef->io.outputs[j];

                /* examine requirements of output node against output specification */
                RWASSERT(outnode->topSortData->req != NULL);
                if      (outnode->topSortData->req == NULL)
                {
                    RWMESSAGE(("Topological sorting produced incorrect results - the pipeline probably contains a cycle, but that should have been detected (RW bug!)"));
                }
                for (k = 0; k < _ReqNumEntries(outnode->topSortData->req); k++)
                {
                    RwReqEntry *reqentry = _ReqGetEntry(outnode->topSortData->req, k);

                    /* output node flags cluster as either required or optional */

                    /* search specification of driving output to find whether */
                    /* cluster is NOCHANGE, VALID, INVALID */

                    RwUInt32 n = _IoSpecSearch4Cluster(
                                     iospec, reqentry->clusterDef);
                    RxClusterValid  presinout = (n == (RwUInt32)-1) ?
                                                outspec->allOtherClusters :
                                                outspec->outputClusters[n];

                    /* if cluster is NOCHANGE add to inherited requirements */

                    if (presinout == rxCLVALID_NOCHANGE)
                    {
                        if (!_ReqAddEntry(node->topSortData->req,
                                          reqentry->clusterDef,
                                          reqentry->required,
                                          node->topSortData->numIns,
                                          reqentry->originatingNode))
                        {
                            RWERROR((E_RX_DEP_OUTOFMEMORY));
                            RWRETURN(E_RX_DEP_OUTOFMEMORY);
                        }
                    }
                    else
                    {
                        if (reqentry->required == rxCLREQ_REQUIRED)
                        {
                            if (presinout != rxCLVALID_VALID)
                            {
                                RWERROR((E_RX_DEP_DEPENDENCIESMISMATCH,
                                         reqentry->clusterDef->name,
                                         reqentry->originatingNode->nodeDef->name,
                                         node->nodeDef->name,
                                         j,
                                         outspec->name));
                                RWRETURN(E_RX_DEP_DEPENDENCIESMISMATCH);
                            }
                        }
                        else
                        {      /* ceOPTIONAL */
                            if (presinout == rxCLVALID_INVALID)
                            {
                                /* we've just learnt that a cluster that outnode (or its descendants) had flagged
                                 * as optionally required can not be present (via this path, anyway)
                                 *
                                 * start at outnode & traverse through NOCHANGE outputs,
                                 * decrementing req->inputs for this clusterid; if req->inputs
                                 * hits zero then this cluster has been eliminated from all
                                 * paths to the node */

                                _PropDownElimPath(pipeline, outnode,
                                                  reqentry->clusterDef);
                            }
                        }
                    }
                }
            }
        }

        /* our requirements ******************************************************************** */
        for (j = 0; j < iospec->numClustersOfInterest; j++)
        {
            if (iospec->inputRequirements[j] != rxCLREQ_DONTWANT)
            {
                if (!_ReqAddEntry(node->topSortData->req,
                                  iospec->
                                  clustersOfInterest[j].clusterDef,
                                  iospec->inputRequirements[j],
                                  node->topSortData->numIns, node))
                {
                    RWERROR((E_RX_DEP_OUTOFMEMORY));
                    RWRETURN(E_RX_DEP_OUTOFMEMORY);
                }
            }
        }
    }
    while (node--, --i);

    RWRETURN(E_RX_DEP_SUCCESS);
}

/*****************************************************************************/
static              RwUInt32
_ForAllNodeReqsAddOutputClustersAndBuildContinuityBitfields(RxPipeline
                                                            * pipeline)
{

    RwUInt32            i, j, k;
    RxPipelineNode     *node;
    RWFUNCTION(RWSTRING("_ForAllNodeReqsAddOutputClustersAndBuildContinuityBitfields"));

    i = pipeline->numNodes;
    node = &pipeline->nodes[0];

    do
    {
        RxIoSpec   *iospec = &node->nodeDef->io;

        /* suppose cluster is required by node below & we pass-thru (NOCHANGE), but
         * FORCEPRESENT - path is eliminated coz valid cluster cannot reach us, but
         * then we add here; if outputs below can use, will be passed on but should
         * really be destroyed at end because will be invalid on input & therefore
         * invalid on output
         */

        for (j = 0; j < iospec->numClustersOfInterest; j++)
        {
            if (iospec->clustersOfInterest[j].forcePresent)
            {
                if (!_ReqAddEntry(node->topSortData->req,
                                  iospec->clustersOfInterest[j].clusterDef,
                                  rxCLREQ_DONTWANT, 1, node))
                {
                    RWERROR((E_RX_DEP_OUTOFMEMORY));
                    RWRETURN(E_RX_DEP_OUTOFMEMORY);
                }
            }
        }

        for (j = 0; j < node->numOutputs; j++) /* loop over node outputs */
        {
            if (node->outputs[j] != (RwUInt32) ~ 0)
            {
                RxPipelineNode     *outnode =
                    &pipeline->nodes[node->outputs[j]];
                RxOutputSpec       *outspec =
                    &node->nodeDef->io.outputs[j];

                for (k = 0; k < _ReqNumEntries(outnode->topSortData->req); k++)
                {
                    RwReqEntry    *reqentry =
                        _ReqGetEntry(outnode->topSortData->req, k);
                    RwUInt32       n =
                        _IoSpecSearch4Cluster(iospec, reqentry->clusterDef);
                    RxClusterValid presinout =
                        (n == (RwUInt32) - 1) ?
                        outspec->allOtherClusters :
                        outspec->outputClusters[n];

                    if (presinout != rxCLVALID_INVALID)
                    {
                        RwReqEntry         *drivingreqe;

                        if (presinout != rxCLVALID_NOCHANGE)
                        {
                            if ((drivingreqe =
                                 _ReqAddEntry(node->topSortData->req,
                                              reqentry->clusterDef,
                                              rxCLREQ_DONTWANT,
                                              1, node)) == NULL)
                            {
                                RWERROR((E_RX_DEP_OUTOFMEMORY));
                                RWRETURN(E_RX_DEP_OUTOFMEMORY);
                            }
                        }
                        else
                        {
                            drivingreqe =
                                _ReqSearch4Cluster(node->topSortData->req,
                                                   reqentry->clusterDef);
                        }

                        if (drivingreqe != NULL)
                        {
                            drivingreqe->outbf |= (1U << j); /* flag that this output wants (REQ / OPT) this cluster */
                        }
                    }
                }
            }
        }
    }
    while (node++, --i);

    RWRETURN(E_RX_DEP_SUCCESS);
}

/*****************************************************************************/
static              RwUInt32
_TraceClusterScopes(RxPipeline * pipeline, RwScopeTrace ** stheadref)
{

    RwUInt32            i, j, k;
    RxPipelineNode     *node;

    RWFUNCTION(RWSTRING("_TraceClusterScopes"));

    /* top -> bottom */

    i = pipeline->numNodes;
    node = &pipeline->nodes[0];

    do
    {
        /* go through own reqs - for all NULL create a scope trace */

        for (k = 0; k < _ReqNumEntries(node->topSortData->req); k++)
        {
            RwReqEntry *reqentry = _ReqGetEntry(node->topSortData->req, k);

            if (reqentry->st == NULL)
            {
                if ((reqentry->st = _ScopeTraceCreate(stheadref)) ==
                    NULL)
                {
                    RWERROR((E_RX_DEP_OUTOFMEMORY));
                    RWRETURN(E_RX_DEP_OUTOFMEMORY);
                }

                _ScopeTraceAddEntry(reqentry->st, reqentry);
            }
        }

        /* go through descendants reqs & for all non-DONTWANT, copy
         * relevant scope trace (if already non-NULL, merge)
         */

        for (j = 0; j < node->numOutputs; j++) /* loop over node outputs */
        {
            if (node->outputs[j] != (RwUInt32) ~ 0)
            {
                RxPipelineNode     *outnode =
                    &pipeline->nodes[node->outputs[j]];

                for (k = 0; k < _ReqNumEntries(outnode->topSortData->req); k++)
                {
                    RwReqEntry *reqentry =
                        _ReqGetEntry(outnode->topSortData->req, k);

                    /* hmmm... if can't service (this output can't drive or passes through & predecessors
                     * can't drive) then there is no continuity of scope across this output - don't
                     * currently reflect this */

                    if (reqentry->required != rxCLREQ_DONTWANT)
                    {
                        /* non-DONTWANT cluster in outnode: we won't necessarily service it
                         * (might be optional & other input nodes do) */

                        /* as commented above, need to demonstrate continuity of
                         * scope: therefore, this node must either service (VALID)
                         * or pass-thru (NOCHANGE) w/ associated req->input != 0 */

                        RwReqEntry *drivingreqe =
                            _ReqSearch4Cluster(node->topSortData->req,
                                               reqentry->clusterDef);

                        if (drivingreqe != NULL
                            && (drivingreqe->outbf & (1U << j)) != 0U)
                        {
                            if (reqentry->st == NULL)
                            {
                                _ScopeTraceAddEntry(drivingreqe->st,
                                                    reqentry);

                                reqentry->st = drivingreqe->st;
                            }
                            else
                            {
                                _ScopeTraceMerge(stheadref,
                                                 reqentry->st,
                                                 drivingreqe->st);
                            }
                        }
                    }
                }
            }
        }
    }
    while (node++, --i);

    RWRETURN(E_RX_DEP_SUCCESS);
}

/*****************************************************************************/
static              RwUInt32
_AssignClusterSlots(RxPipeline * pipeline, RwScopeTrace ** stheadref)
{

    RwUInt32            numslotsused = 0U;
    RwScopeTrace       *st = *stheadref;

    RWFUNCTION(RWSTRING("_AssignClusterSlots"));

    /* now iterate through scope traces, assigning cluster slots */

    while (st != NULL)         /* outer loop moves through master list */
    {
        RwScopeTrace       *st2;
        RwUInt32            unavailslots = 0U;
        RwUInt32            assignedslot;

        st2 = st;
        do
        {                      /* inner loop moves through continuations (created by merging) */
            RwReqEntry         *reqentry = st2->head;

            while (reqentry != NULL)
            {
                unavailslots |= reqentry->req->unavailslots; /* this bit-field ends up with a 1 for any slot
                                                              * used somewhere
                                                              */

                reqentry = reqentry->next;
            }

            st2 = st2->continuation;
        }
        while (st2 != NULL);

        assignedslot = 0;
        while (unavailslots & 1) /* once we exhaust all slots, will always pick slot 32! */
        {
            unavailslots >>= 1;
            assignedslot++;
        }

        if (assignedslot >= numslotsused)
        {
            numslotsused = assignedslot + 1;
        }

        /* & again to update all req->unavailslots w/ chosen slot */

        st2 = st;
        do
        {                      /* inner loop moves through continuations (created by merging) */
            RwReqEntry         *reqentry = st2->head;

            while (reqentry != NULL)
            {
                reqentry->assignedslot = assignedslot;
                reqentry->req->unavailslots |= 1U << assignedslot;

                reqentry = reqentry->next;
            }

            st2 = st2->continuation;
        }
        while (st2 != NULL);

        st = st->next;
    }

    pipeline->packetNumClusterSlots = numslotsused;

    RWRETURN(E_RX_DEP_SUCCESS);
}

/*****************************************************************************/

typedef void (*EnumPipelineClustersCallBack)
    (RxClusterDefinition *clusterdef,
     RwUInt32             pipelineclusterindex,
     void                *privatedata);

static RwUInt32
_EnumPipelineClusters(RwScopeTrace *head,
                      EnumPipelineClustersCallBack fp,
                      void *callbackprivatedata)
{
    RwUInt32      numpipelineclusters = 0;
    RwScopeTrace *st;

    RWFUNCTION(RWSTRING("_EnumPipelineClusters"));

    for (st = head; st != NULL; st = st->next)
    {
        RwScopeTrace       *stdedupe;

        /* run over all scope traces prior to this one, checking if any are
         * tracing the same cluster */

        for (stdedupe = head; stdedupe != st; stdedupe = stdedupe->next)
        {
            if (stdedupe->head->clusterDef == st->head->clusterDef)
            {
                break;
            }
        }

        if (stdedupe == st)
        {
            /* dedupe has come up empty, i.e. haven't encountered this cluster previously... */

            if (fp != NULL)
            {
                fp(st->head->clusterDef, numpipelineclusters,
                   callbackprivatedata);
            }

            numpipelineclusters++;
        }
    }

    RWRETURN(numpipelineclusters);
}

/*****************************************************************************/
static              RwUInt32
_CountHeadNodeRqdsAndOpts(RxPipeline * pipeline)
{

    RxPipelineNode     *node = &pipeline->nodes[0];
    RwUInt32            i, count = 0;

    RWFUNCTION(RWSTRING("_CountHeadNodeRqdsAndOpts"));

    for (i = 0; i < _ReqNumEntries(node->topSortData->req); i++)
    {
        RwReqEntry *reqentry = _ReqGetEntry(node->topSortData->req, i);

        if (reqentry->required == rxCLREQ_REQUIRED ||
            reqentry->required == rxCLREQ_OPTIONAL    )
        {
            count++;
        }
    }

    RWRETURN(count);
}

/*****************************************************************************/
static void
_WriteHeadNodeRqdsAndOpts2PipelineRequirements(RxPipeline *pipeline)
{
    RxPipelineNode *node = &pipeline->nodes[0];
    RwUInt32        i, count = 0;

    RWFUNCTION(RWSTRING("_WriteHeadNodeRqdsAndOpts2PipelineRequirements"));

    for (i = 0; i < _ReqNumEntries(node->topSortData->req); i++)
    {
        RwReqEntry *reqentry = _ReqGetEntry(node->topSortData->req, i);

        if ((reqentry->required == rxCLREQ_REQUIRED) ||
            (reqentry->required == rxCLREQ_OPTIONAL)    )
        {
            RxPipelineRequiresCluster *reqcl =
                &(pipeline->inputRequirements[count++]);

            reqcl->clusterDef = reqentry->clusterDef;
            reqcl->rqdOrOpt   = reqentry->required;
            reqcl->slotIndex  = reqentry->assignedslot;
        }
    }

    pipeline->numInputRequirements = count;

    /* Sort pipeline input requirements by clusterDef address so that
     * nodes can be passed between pipelines as quickly as possible. */
    _rx_rxRadixExchangeSort(pipeline->inputRequirements,
                      pipeline->numInputRequirements,
                      sizeof(RxPipelineRequiresCluster),
                      offsetof(RxPipelineRequiresCluster, clusterDef),
                      0x00000000U, /* keyLo */
                      0xFFFFFFFFU /* keyHi */ );

    RWRETURNVOID();
}

/*****************************************************************************/
static void
_MyEnumPipelineClustersCallBack(RxClusterDefinition *clusterdef,
                                RwUInt32 __RWUNUSED__ pipelineclusterindex,
                                void *data)
{
    RxPipelineCluster **array = (RxPipelineCluster **)data;
    RxPipelineCluster  *pipelineCluster;

    /* MiteAlloc and initialize an RxPipelineCluster */

    RWFUNCTION(RWSTRING("_MyEnumPipelineClustersCallBack"));

    /* This is like PipelineCalcNumUniqueClusters() in p2define.c,
     * except it uses scopetraces to find all the unique clusterDefs
     * (so it should eliminate unused clusters).
     * NOTE: these seem to be alloc'd separately but since this
     * callback is called n times in a row by _EnumPipelineClusters
     * they end up in a contiguous array anyway. We cache the address of
     * the first pipelinecluster (the start of the array) in *array. */

    pipelineCluster = (RxPipelineCluster *)
        StalacMiteAlloc(sizeof(RxPipelineCluster));
    RWASSERT(NULL != pipelineCluster);
    /* Cache the address of the first one we alloc
     * (they'll actually form a contiguous array) */
    if (NULL == *array) *array = pipelineCluster;

    pipelineCluster->clusterRef = clusterdef;
    pipelineCluster->creationAttributes = clusterdef->defaultAttributes;
    /* but this may well be modified when node config() methods are called;
     * otherwise we'd just pick up via pipelineCluster->clusterRef */

    RWASSERT(sizeof(RxPipelineCluster) % sizeof(RwUInt32) == 0);
    /* or we're hosed... */

    RWRETURNVOID();
}

/*****************************************************************************/
static RwUInt32
_ForAllNodesWriteClusterAllocations(RxPipeline   *pipeline,
                                    RwScopeTrace *stHead)
{
    RwUInt32            numPipelineRequiresClusters;
    RwUInt32            numPipelineClusters;
    RxPipelineCluster  *pipelineClusters;
    RxPipelineNode     *node;
    RwUInt8            *titeBase, *miteBase;
    RwUInt32            slotsContinueAND;
    RwUInt32            i, j, k;

    RWFUNCTION(RWSTRING("_ForAllNodesWriteClusterAllocations"));

    /* NB/ all the following means that we can't delete or add individual.
     * nodes, so we throw all this stuff away at Lock() - stash a pointer
     * to pBlock in pipeline, for this and destruction of Unlock()ed
     * pipelines */

    numPipelineRequiresClusters = _CountHeadNodeRqdsAndOpts(pipeline);
    numPipelineClusters = _EnumPipelineClusters(
        stHead, (EnumPipelineClustersCallBack)NULL, NULL);

    /* Clear all unused memory (i.e between the stalacMite and
     * stalacTite pointers) to zero to start with. This initialises
     * the private data to zero... */
    titeBase = (RwUInt8 *)StalacTiteAlloc(0);
    RWASSERT(NULL != titeBase);
    miteBase = (RwUInt8 *)StalacMiteAlloc(0);
    RWASSERT(NULL != miteBase);
    i = titeBase - miteBase;
    RWASSERT((RwInt32)i >= 0);
    memset(miteBase, 0, i);

    pipeline->embeddedPacket = (RxPacket *)
        StalacMiteAlloc(
            sizeof(RxPacket) +
            /* Minus one because the first element of the clusters
             * array is embedded in teh end of the RxPacket struct. */
            (pipeline->packetNumClusterSlots - 1)*sizeof(RxCluster));

    pipeline->embeddedPacket->numClusters =
        (RwUInt16)pipeline->packetNumClusterSlots;
    pipeline->embeddedPacket->pipeline = pipeline;
    pipeline->embeddedPacketState = rxPKST_PACKETLESS;

    /* Allocate RxPipelineClusters (they end up in a contiguous
     * array even though they're allocated separately - we'll
     * get the start of the array in pipelineClusters) */
    pipelineClusters = (RxPipelineCluster *) NULL;
    _EnumPipelineClusters(stHead,
                          _MyEnumPipelineClustersCallBack,
                          &pipelineClusters);

    for (i = 0;i < pipeline->numNodes;i++)
    {
        node = &(pipeline->nodes[i]);

        /* Pointers to the appropriate pipelinecluster for each cluster */
        if (0 < pipeline->packetNumClusterSlots)
        {
            node->slotClusterRefs = (RxPipelineCluster **)StalacMiteAlloc(
                pipeline->packetNumClusterSlots*sizeof(RxPipelineCluster *));
            RWASSERT(NULL != node->slotClusterRefs);
        }

        /* A bit-field per cluster for which outputs it continues through */
        node->slotsContinue = (RwUInt32 *)StalacMiteAlloc(
            (1 + pipeline->packetNumClusterSlots)*sizeof(RwUInt32));

        /* Which clusterslot for each cluster of interest? */
        if (0 < node->nodeDef->io.numClustersOfInterest)
        {
            node->inputToClusterSlot = (RwUInt32 *)StalacMiteAlloc(
                node->nodeDef->io.numClustersOfInterest*sizeof(RwUInt32));
            RWASSERT(NULL != node->inputToClusterSlot);
        }

        /* Allocate space for the private data of this node (if any is required) */
        if (node->nodeDef->pipelineNodePrivateDataSize != 0)
        {
            /* Everything else is 4-byte aligned, private data should be too */
            j = (node->nodeDef->pipelineNodePrivateDataSize + 3) & ~3;
            node->privateData = (void *)StalacMiteAlloc(j);
            RWASSERT(NULL != node->privateData);
#ifdef RWDEBUG
            {
                RwUInt32 *magic;
                /* Write "magic" value into first 32-bit aligned address above
                 * private data space, so we can check for trampling */
                magic = (RwUInt32 *)StalacMiteAlloc(sizeof(RwUInt32));
                RWASSERT(NULL != magic);
               *magic = RXNODEPRIVATESPACEMAGIC;
            }
#endif /* RWDEBUG */
        }

        /* Run through all reqentrys - if have a slot allocation, update slotclusterids */
        slotsContinueAND = ~0;
        for (j = 0; j < _ReqNumEntries(node->topSortData->req); j++)
        {
            RxPipelineCluster *pipelineCluster = (RxPipelineCluster *)NULL;
            RwReqEntry        *reqEntry;

            reqEntry = _ReqGetEntry(node->topSortData->req, j);

            /* Brute force search through pipelineclusters to
             * effect clusterDef-to-pipelineCluster mapping */
            for (k = 0;k < numPipelineClusters;k++)
            {
                if (pipelineClusters[k].clusterRef == reqEntry->clusterDef)
                {
                    pipelineCluster = &(pipelineClusters[k]);
                    break;
                }
            }

            RWASSERT(pipelineCluster != NULL); /* bad. logic failure. cluster is used in pipeline,
                                                * but no pipelineCluster record has been created */

            node->slotClusterRefs[reqEntry->assignedslot] =
                pipelineCluster;
            node->slotsContinue[1 + reqEntry->assignedslot] =
                reqEntry->outbf;

            slotsContinueAND &= reqEntry->outbf;
        }

        /* Used to optimise EMBEDDEDPACKETBETWEENNODESMACRO() - if it's '1'
         * for a given bit (output) then ALL clusters persist through it. */
        node->slotsContinue[0] = slotsContinueAND;

        for (j = 0; j < node->nodeDef->io.numClustersOfInterest; j++)
        {
            /* clustersofinterest not present (i.e an optional cluster is missing
             * or something's wrong) need to be labelled absent with '-1' */
            node->inputToClusterSlot[j] = (RwUInt32)-1;
            for (k = 0; k < pipeline->packetNumClusterSlots; k++)
            {
                if ((node->slotClusterRefs[k] != NULL) &&
                    (node->slotClusterRefs[k]->clusterRef ==
                     node->nodeDef->io.clustersOfInterest[j].clusterDef))
                {
                    node->inputToClusterSlot[j] = k;
                    break;
                }
            }
        }
    }

    /* An array of requirements for the *pipeline* */
    if (0 < numPipelineRequiresClusters)
    {
        pipeline->inputRequirements = (RxPipelineRequiresCluster *)
            StalacMiteAlloc(sizeof(RxPipelineRequiresCluster)*
                            numPipelineRequiresClusters);
        RWASSERT(NULL != pipeline->inputRequirements);
    }
    _WriteHeadNodeRqdsAndOpts2PipelineRequirements(pipeline);

    RWRETURN(E_RX_DEP_SUCCESS);
}

/*****************************************************************************
 * _rxChaseDependencies
 */
RwUInt32
_rxChaseDependencies(RxPipeline * pipeline)
{
    RwScopeTrace       *sthead = (RwScopeTrace *)NULL;
    RwUInt32            result;

    RWFUNCTION(RWSTRING("_rxChaseDependencies"));

    /*****/
    /* 1 */
    /*****/
    result = _PropagateDependenciesAndKillDeadPaths(pipeline);

    if (result != E_RX_DEP_SUCCESS)
    {
        goto Exit;
    }

    /* Every node's 'req' now contains clusters directly required by that node &
     * clusters required for pass-thru to descendants. The complete cluster
     * assignment during node execution is made up of this PLUS clusters we may
     * create (VALID) that descendants require [LESS any clusters
     * that downward traversal reveals cannot be present !! NO !! now handled in
     * 1st pass, _PropDownElimPath()]
     */

    /* finally, must execute cluster assignment scheme:
     * when we included our own requirements or inherited pass-thrus, they
     * will have been required or optional
     * when we add our outputs VALID required by descendants, add these as DONTWANT (?)
     * assignment --- traverse down tree
     *
     * for all unassigned clusters in node, determine scope (traverse thru all
     * outputs & form list) - traversal stops at nodes that don't require
     * this cluster in input over scope list, form (by anding bit-field?)
     * table of possible cluster slots pick lowest 1 then down scope list,
     * flagging cluster slot as used & flagging cluster as assigned
     * continue thru clusters & down tree
     */

    /* (1) assign clusters as required */

    /* (2) create map (node's "clustersofinterest" to assigned clusters) */

    /* (3) create clusterdestroyondispatch info. */

    /* (4) downward prop.: eliminate from requirements those clusters that can't be present
     *     (assume any reqd. cluster can be present at top) - caution! some nodes
     *     have multiple inputs, so despite the elimination of a cluster from one path to
     *     the node, it may arrive via another path
     */

    /* next pass: add to reqs as DONTWANT clusters VALID in our output specs. that outnode can use [REQ/OPT] */

    /* next pass(es): repeatedly run through tree extracting scope traces (pick first unassigned req. &
     * track until DONTWANT or simply not present), & assign these a cluster
     * --- so pick an unassigned cluster near top, and follow all occurrences of this through graph
     *     some may be independent
     */

    /* helpful comment: pass the 1st has built up the requirements proper of each node (immediate & inherited);
     * also optional requirements have been eliminated from every stage at which they demonstrably cannot be met.
     * (it follows that if any node->req contains an OPT req, there is a possibility it will be occupied).
     * now we add to this structure clusters that will be generated by a node to service the requirements of the node
     * below
     */

    /* consider merging nodes. not sure reduction of reqentry->inputs is adequate */

    /*****/
    /* 2 */
    /*****/
    result =
        _ForAllNodeReqsAddOutputClustersAndBuildContinuityBitfields(pipeline);

    if (result != E_RX_DEP_SUCCESS)
    {
        goto Exit;
    }

    REQSDUMP(pipeline);

    /*****/
    /* 3 */
    /*****/
    result = _TraceClusterScopes(pipeline, &sthead);

    if (result != E_RX_DEP_SUCCESS)
    {
        goto Exit;
    }

    /*****/
    /* 4 */
    /*****/
    result = _AssignClusterSlots(pipeline, &sthead);

    if (result != E_RX_DEP_SUCCESS)
    {
        goto Exit;
    }

    SCOPETRACESDUMP(sthead);

    /*****/
    /* 5 */
    /*****/
    result = _ForAllNodesWriteClusterAllocations(pipeline, sthead);

    if (result != E_RX_DEP_SUCCESS)
    {
        goto Exit;
    }

    ALLOCSDUMP(pipeline);

 Exit:

    RWRETURN(result);
}

#if (0)

static              RwBool
_nodecode(RxPipelineNodeInstance * self,
          const RxPipelineNodeParam *__RWUNUSED__ params)
{
    RWFUNCTION(RWSTRING("_nodecode"));

    /* No packets, just execute all the nodes */
    RxPacketDispatch(NULL, 0, self);

    RWRETURN(TRUE);
}

void
_rxDepChaseExercise(void)
{

    static RxClusterDefinition clA = { "A.csl" };
    static RxClusterDefinition clB = { "B.csl" };
    static RxClusterDefinition clC = { "C.csl" };
    static RxClusterDefinition clD = { "D.csl" };
    static RxClusterDefinition clE = { "E.csl" };
    static RxClusterDefinition clF = { "F.csl" };
    static RxClusterRef N1clofinterest[] =
    { {(RxClusterDefinition *) & clA, rxCLALLOWABSENT},
      {(RxClusterDefinition *) & clC, rxCLALLOWABSENT}
    };
    static RxClusterValidityReq N1inputreqs[] =
    { rxCLREQ_REQUIRED, rxCLREQ_DONTWANT };
    static RxClusterValid N1outcl1[] =
    { rxCLVALID_VALID, rxCLVALID_VALID };
    static RxClusterValid N1outcl2[] =
    { rxCLVALID_VALID, rxCLVALID_VALID };
    static RxOutputSpec N1outputs[] =
    { {"N1OUT1", N1outcl1, rxCLVALID_NOCHANGE},
      {"N1OUT2", N1outcl2, rxCLVALID_NOCHANGE}
    };
    static RxNodeDefinition N1 =
    { "1.csl",
      {(RxNodeBodyFn) _nodecode, (RxNodeInitFn) NULL,
       (RxNodeTermFn) NULL, (RxPipelineNodeInitFn) NULL,
       (RxPipelineNodeTermFn) NULL},
      {2, N1clofinterest, N1inputreqs, 2, N1outputs}, (RwUInt32) 0,
      rxNODEDEFCONST, (RwInt32) 0
    };
    static RxClusterRef N2clofinterest[] =
    { {(RxClusterDefinition *) & clA, rxCLALLOWABSENT},
      {(RxClusterDefinition *) & clC, rxCLALLOWABSENT},
      {(RxClusterDefinition *) & clF, rxCLALLOWABSENT}
    };
    static RxClusterValidityReq N2inputreqs[] =
    { rxCLREQ_REQUIRED, rxCLREQ_REQUIRED, rxCLREQ_OPTIONAL };
    static RxClusterValid N2outcl1[] =
    { rxCLVALID_VALID, rxCLVALID_VALID, rxCLVALID_INVALID };
    static RxClusterValid N2outcl2[] =
    { rxCLVALID_VALID, rxCLVALID_INVALID, rxCLVALID_INVALID };
    static RxOutputSpec N2outputs[] =
    { {"N2OUT1", N2outcl1, rxCLVALID_INVALID},
      {"N2OUT2", N2outcl2, rxCLVALID_INVALID}
    };
    static RxNodeDefinition N2 =
    { "2.csl",
      {(RxNodeBodyFn) _nodecode, (RxNodeInitFn) NULL,
       (RxNodeTermFn) NULL, (RxPipelineNodeInitFn) NULL,
       (RxPipelineNodeTermFn) NULL},
      {3, N2clofinterest, N2inputreqs, 2, N2outputs}, (RwUInt32) 0,
      rxNODEDEFCONST, (RwInt32) 0
    };
    static RxClusterRef N3clofinterest[] =
    { {(RxClusterDefinition *) & clA, rxCLALLOWABSENT},
      {(RxClusterDefinition *) & clC, rxCLALLOWABSENT},
      {(RxClusterDefinition *) & clD, rxCLALLOWABSENT}
    };
    static RxClusterValidityReq N3inputreqs[] =
    { rxCLREQ_REQUIRED, rxCLREQ_REQUIRED, rxCLREQ_DONTWANT };
    static RxClusterValid N3outcl1[] =
    { rxCLVALID_VALID, rxCLVALID_VALID, rxCLVALID_VALID };
    static RxOutputSpec N3outputs[] =
    { {"N3OUT1", N3outcl1, rxCLVALID_NOCHANGE} };
    static RxNodeDefinition N3 =
    { "3.csl",
      {(RxNodeBodyFn) _nodecode, (RxNodeInitFn) NULL,
       (RxNodeTermFn) NULL, (RxPipelineNodeInitFn) NULL,
       (RxPipelineNodeTermFn) NULL},
      {3, N3clofinterest, N3inputreqs, 1, N3outputs}, (RwUInt32) 0,
      rxNODEDEFCONST, (RwInt32) 0
    };
    static RxClusterRef N4clofinterest[] =
    { {(RxClusterDefinition *) & clB, rxCLALLOWABSENT},
      {(RxClusterDefinition *) & clD, rxCLALLOWABSENT}
    };
    static RxClusterValidityReq N4inputreqs[] =
    { rxCLREQ_OPTIONAL, rxCLREQ_REQUIRED };
    static RxNodeDefinition N4 =
    { "4.csl",
      {(RxNodeBodyFn) _nodecode, (RxNodeInitFn) NULL,
       (RxNodeTermFn) NULL, (RxPipelineNodeInitFn) NULL,
       (RxPipelineNodeTermFn) NULL},
      {2, N4clofinterest, N4inputreqs, 0, NULL}, (RwUInt32) 0,
      rxNODEDEFCONST, (RwInt32) 0
    };
    static RxClusterRef N5clofinterest[] =
    { {(RxClusterDefinition *) & clA, rxCLALLOWABSENT},
      {(RxClusterDefinition *) & clB, rxCLALLOWABSENT},
      {(RxClusterDefinition *) & clD, rxCLALLOWABSENT}
    };
    static RxClusterValidityReq N5inputreqs[] =
    { rxCLREQ_REQUIRED, rxCLREQ_OPTIONAL, rxCLREQ_DONTWANT };
    static RxClusterValid N5outcl1[] =
    { rxCLVALID_VALID, rxCLVALID_NOCHANGE, rxCLVALID_VALID };
    static RxOutputSpec N5outputs[] =
    { {"N5OUT1", N5outcl1, rxCLVALID_NOCHANGE} };
    static RxNodeDefinition N5 =
    { "5.csl",
      {(RxNodeBodyFn) _nodecode, (RxNodeInitFn) NULL,
       (RxNodeTermFn) NULL, (RxPipelineNodeInitFn) NULL,
       (RxPipelineNodeTermFn) NULL},
      {3, N5clofinterest, N5inputreqs, 1, N5outputs}, (RwUInt32) 0,
      rxNODEDEFCONST, (RwInt32) 0
    };
    static RxClusterRef N6clofinterest[] =
    { {(RxClusterDefinition *) & clA, rxCLALLOWABSENT},
      {(RxClusterDefinition *) & clC, rxCLALLOWABSENT},
      {(RxClusterDefinition *) & clE, rxCLALLOWABSENT}
    };
    static RxClusterValidityReq N6inputreqs[] =
    { rxCLREQ_REQUIRED, rxCLREQ_REQUIRED, rxCLREQ_DONTWANT };
    static RxClusterValid N6outcl1[] =
    { rxCLVALID_VALID, rxCLVALID_INVALID, rxCLVALID_VALID };
    static RxOutputSpec N6outputs[] =
    { {"N6OUT1", N6outcl1, rxCLVALID_NOCHANGE} };
    static RxNodeDefinition N6 =
    { "6.csl",
      {(RxNodeBodyFn) _nodecode, (RxNodeInitFn) NULL,
       (RxNodeTermFn) NULL, (RxPipelineNodeInitFn) NULL,
       (RxPipelineNodeTermFn) NULL},
      {3, N6clofinterest, N6inputreqs, 1, N6outputs}, (RwUInt32) 0,
      rxNODEDEFCONST, (RwInt32) 0
    };
    static RxClusterRef N7clofinterest[] =
    { {(RxClusterDefinition *) & clA, rxCLALLOWABSENT},
      {(RxClusterDefinition *) & clB, rxCLALLOWABSENT},
      {(RxClusterDefinition *) & clD, rxCLALLOWABSENT},
      {(RxClusterDefinition *) & clE, rxCLALLOWABSENT}
    };
    static RxClusterValidityReq N7inputreqs[] =
    { rxCLREQ_REQUIRED, rxCLREQ_DONTWANT, rxCLREQ_DONTWANT,
      rxCLREQ_OPTIONAL
    };
    static RxClusterValid N7outcl1[] =
    { rxCLVALID_INVALID, rxCLVALID_VALID, rxCLVALID_VALID,
      rxCLVALID_INVALID
    };
    static RxOutputSpec N7outputs[] =
    { {"N7OUT1", N7outcl1, rxCLVALID_NOCHANGE} };
    static RxNodeDefinition N7 =
    { "7.csl",
      {(RxNodeBodyFn) _nodecode, (RxNodeInitFn) NULL,
       (RxNodeTermFn) NULL, (RxPipelineNodeInitFn) NULL,
       (RxPipelineNodeTermFn) NULL},
      {4, N7clofinterest, N7inputreqs, 1, N7outputs}, (RwUInt32) 0,
      rxNODEDEFCONST, (RwInt32) 0
    };

    RWFUNCTION(RWSTRING("_rxDepChaseExercise"));

    {
        RxPipeline         *pipeline;
        RxLockedPipe       *lckp;
        RxPipelineNode     *frag1234, *frag5, *frag67;

        pipeline = RxPipelineCreate();

        lckp = RxPipelineLock(pipeline);

        RxLockedPipeAddFragment(lckp, NULL, &N1, &N2, &N3, &N4, NULL);
        RxLockedPipeAddFragment(lckp, NULL, &N5, NULL);
        RxLockedPipeAddFragment(lckp, NULL, &N6, &N7, NULL);

        /* hmm... because of realloc()s: */
        frag1234 = RxPipelineFindNodeByName(lckp, N1.name, NULL, NULL);
        frag5 = RxPipelineFindNodeByName(lckp, N5.name, NULL, NULL);
        frag67 = RxPipelineFindNodeByName(lckp, N6.name, NULL, NULL);

        RxLockedPipeAddPath(lckp,
                            RxPipelineNodeFindOutputByIndex(&frag1234
                                                            [0], 1),
                            RxPipelineNodeFindInput(&frag67[0])); /* wire 1#2 -> 6 */
        RxLockedPipeAddPath(lckp,
                            RxPipelineNodeFindOutputByIndex(&frag67[1],
                                                            0),
                            RxPipelineNodeFindInput(&frag1234[3])); /* wire 7#1 -> 4 */

        RxLockedPipeAddPath(lckp,
                            RxPipelineNodeFindOutputByIndex(&frag1234
                                                            [1], 1),
                            RxPipelineNodeFindInput(&frag5[0])); /* wire 2#2 -> 5 */
        RxLockedPipeAddPath(lckp,
                            RxPipelineNodeFindOutputByIndex(&frag5[0],
                                                            0),
                            RxPipelineNodeFindInput(&frag1234[3])); /* wire 5#1 -> 4 */

        RxLockedPipeUnlock(lckp);

        RxPipelineExecute(pipeline, NULL, TRUE);

        _rxPipelineDestroy(pipeline);
    }

    RWRETURNVOID();
}

#endif /* (0) */

/* *INDENT-ON* */

