/***************************************************************************
 *                                                                         *
 * Module  : rwgrp.h                                                       *
 *                                                                         *
 * Purpose : Binary file group handling                                    *
 *                                                                         *
 **************************************************************************/

#ifndef RWBRP_H
#define RWGRP_H

/****************************************************************************
 Includes
 */

#include "batypes.h"
#include "bastream.h"
#include "bamemory.h"

/* RWPUBLIC */

/****************************************************************************
 Defines
 */
#define rwCHUNKGROUPMAXNAME          32

#define rwCHUNKGROUPVERSION          0x01

/****************************************************************************
 Global Types
 */

typedef struct RwChunkGroup RwChunkGroup;

#ifndef RWADOXYGENEXTERNAL
/**
 * \ingroup rwchunkgroup
 * \struct RwChunkGroup
 * Holds the RenderWare stream group data.
 */
#endif /* RWADOXYGENEXTERNAL */
struct RwChunkGroup
{
    RwChar      name[rwCHUNKGROUPMAXNAME + 1];  /**< Name identifier for the group. */
};


/* RWPUBLICEND */

typedef struct rwChunkGroupGlobals rwChunkGroupGlobals;
struct rwChunkGroupGlobals
{
    RwFreeList         *groupFList;
};

/****************************************************************************
 Global Variables
 */

/* RWPUBLIC */
/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */


/* RWPUBLICEND */

/*
 * Internal functions.
 */

extern void  *
_rwChunkGroupOpen(void *instance,
             RwInt32 __RWUNUSED__ offset ,
             RwInt32 __RWUNUSED__ size );

extern void  *
_rwChunkGroupClose(void *instance,
             RwInt32 __RWUNUSED__ offset ,
             RwInt32 __RWUNUSED__ size );

/* RWPUBLIC */

/*
 * API Functions.
 */
extern void
RwChunkGroupSetFreeListCreateParams( RwInt32 blockSize, RwInt32 numBlocksToPrealloc );

extern RwChunkGroup *
RwChunkGroupCreate( void );

extern RwBool
RwChunkGroupDestroy( RwChunkGroup *grp );

extern RwChunkGroup *
RwChunkGroupBeginStreamRead( RwStream *stream );

extern RwChunkGroup *
RwChunkGroupEndStreamRead( RwChunkGroup *grp,  RwStream *stream );

extern const RwChunkGroup *
RwChunkGroupBeginStreamWrite(const RwChunkGroup *grp, RwStream *stream );

extern const RwChunkGroup *
RwChunkGroupEndStreamWrite(const RwChunkGroup *grp, RwStream *stream);

extern RwUInt32
RwChunkGroupStreamGetSize(const RwChunkGroup *grp);

extern RwChunkGroup *
RwChunkGroupSetName( RwChunkGroup *grp, const RwChar * name);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

/* RWPUBLICEND */

#endif /* RWGRP_H */

