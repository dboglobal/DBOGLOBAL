/*
 *  RenderWare Graphics Collision Plugin
 *
 *  File:    ctdata.c
 *  Purpose: Collision split tree data management
 */

/******************************************************************************
 *  Include files
 */

#include "rwcore.h"
#include "rpworld.h"
#include "rpplugin.h"
#include "rpdbgerr.h"
#include "rpcollis.rpe"

#include "ctdata.h"

/******************************************************************************
 *  Types
 */

typedef struct
{
    RwUInt32        flags;
    RwV3d           inf;
    RwV3d           sup;
    RwUInt32        numEntries;
    RwUInt32        numSplits;

}   rpStreamCollTree;

/******************************************************************************
 *  Macros
 */

#define rpCOLL_PRE36001_BRANCHNODEID  2

#define ISALIGNED(_mem, _alignment) (!(((_alignment)-1) & (RwUInt32)(_mem)))
#define ALIGN(_mem, _alignment) \
    (((RwUInt32)(_mem) + ((_alignment)-1)) & ~((_alignment)-1))

/******************************************************************************
 *  Functions
 */


/* _rpCollTreeCreate
 *
 * numEntries   number of entries (eg triangles) in the tree (must be > 0)
 * numSplits    number of divisions that hierarchically divide the entries
 *                  (can be zero)
 * bbox         bbox enclosing the set of triangles/objects
 * flags        RpCollTreeFlags
 *
 * returns      tree, ready for initialization of RpCollSplit array and map
 */
RpCollTree *
_rpCollTreeCreate(RwUInt16  numEntries,
                  RwUInt16  numSplits,
                  RwBBox   *bbox,
                  RwUInt32  flags)
{
    RwUInt32        size;
    RpCollTree     *tree;
    RwUInt8        *mem;

    RWFUNCTION(RWSTRING("_rpCollTreeCreate"));
    RWASSERT(bbox);
    RWASSERT(numEntries > 0);

    /* Get mem size for RpCollTree, splits array, and map */
    size = sizeof(RpCollTree) ;

    if (numSplits > 0)
    {
        size += numSplits * sizeof(RpCollSplit) + rpCOLLSPLIT_ALIGNMENT - 1;
    }

    if (rpCOLLTREE_USEMAP & flags)
    {
        size += numEntries * sizeof(RwUInt16) + 1;
    }

    /* Allocate */
    mem = (RwUInt8 *) RwMalloc(size, rwID_COLLTREE | rwMEMHINTDUR_EVENT);
    if (!mem)
    {
        RWERROR((E_RW_NOMEM, size));
        RWRETURN((RpCollTree *)NULL);
    }

    /* Init RpCollTree */
    tree = (RpCollTree *) mem;
    tree->flags = flags;
    tree->numEntries = numEntries;
    tree->numSplits = numSplits;
    tree->bbox = *bbox;
    mem += sizeof(RpCollTree);

    /* Init Splits */
    if (numSplits > 0)
    {
        mem = (RwUInt8 *) ALIGN(mem, rpCOLLSPLIT_ALIGNMENT);
        tree->splits = (RpCollSplit *)mem;
        mem += numSplits * sizeof(RpCollSplit);
    }
    else
    {
        tree->splits = (RpCollSplit *)NULL;
    }

    /* Init Map */
    if (rpCOLLTREE_USEMAP & flags)
    {
        mem = (RwUInt8 *) ALIGN(mem, 2);
        tree->map = (RwUInt16 *)mem;
    }
    else
    {
        tree->map = (RwUInt16 *)NULL;
    }

    RWRETURN(tree);
}


void
_rpCollTreeDestroy(RpCollTree *tree)
{
    RWFUNCTION(RWSTRING("_rpCollTreeDestroy"));
    RWASSERT(tree);

    RwFree(tree);

    RWRETURNVOID();
}


/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 *      Streaming
 *
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */




/* _rpCollTreeStreamGetSize
 *
 * Get size in bytes of binary stream data for collision tree. This
 * does not include the rwID_COLLTREE chunk header.
 */
RwUInt32
_rpCollTreeStreamGetSize(const RpCollTree *tree)
{
    RwUInt32    size;

    RWFUNCTION(RWSTRING("_rpCollTreeStreamGetSize"));
    RWASSERT(tree);

    /* STRUCT header + rpStreamCollTree */
    size = rwCHUNKHEADERSIZE + sizeof(rpStreamCollTree);

    /* Splits */
    size += tree->numSplits * sizeof(RpCollSplit);

    /* Map */
    if (rpCOLLTREE_USEMAP & tree->flags)
    {
        size += tree->numEntries * sizeof(RwInt16);
    }

    RWRETURN(size);
}


/* _rpCollTreeStreamWrite
 *
 * Serialise collision tree.
 */
const RpCollTree *
_rpCollTreeStreamWrite(const RpCollTree *tree, RwStream *stream)
{
    RwUInt32            size;
    rpStreamCollTree    streamCollTree;

    RWFUNCTION(RWSTRING("_rpCollTreeStreamWrite"));
    RWASSERT(tree);
    RWASSERT(stream);

    /* Main chunk header */
    size = _rpCollTreeStreamGetSize(tree);
    if (!RwStreamWriteChunkHeader(stream, rwID_COLLTREE, size))
    {
        RWRETURN((const RpCollTree *)NULL);
    }

    /* Wrap structure data */
    size -= rwCHUNKHEADERSIZE;
    if (!RwStreamWriteChunkHeader(stream, rwID_STRUCT, size))
    {
        RWRETURN((const RpCollTree *)NULL);
    }

    /* rpStreamCollTree */
    streamCollTree.inf = tree->bbox.inf;
    streamCollTree.sup = tree->bbox.sup;
    streamCollTree.flags = tree->flags;
    streamCollTree.numEntries = tree->numEntries;
    streamCollTree.numSplits = tree->numSplits;

    (void)RwMemRealToFloat32(&streamCollTree.inf, sizeof(RwV3d));
    (void)RwMemRealToFloat32(&streamCollTree.sup, sizeof(RwV3d));
    (void)RwMemLittleEndian32(&streamCollTree, sizeof(rpStreamCollTree));

    if (!RwStreamWrite(stream, &streamCollTree, sizeof(rpStreamCollTree)))
    {
        RWRETURN((const RpCollTree *)NULL);
    }

    /* Splits */
    if (tree->numSplits > 0)
    {
        RwUInt32 i;

        for (i=0; i<tree->numSplits; i++)
        {
            RpCollSplit temp;

            temp = tree->splits[i];

            (void)RwMemLittleEndian16(&temp.leftSector.index, sizeof(RwUInt16));
            (void)RwMemLittleEndian16(&temp.rightSector.index, sizeof(RwUInt16));
            (void)RwMemRealToFloat32(&temp.leftSector.value, sizeof(RwReal));
            (void)RwMemLittleEndian32(&temp.leftSector.value, sizeof(RwReal));
            (void)RwMemRealToFloat32(&temp.rightSector.value, sizeof(RwReal));
            (void)RwMemLittleEndian32(&temp.rightSector.value, sizeof(RwReal));

            if (!RwStreamWrite(stream, &temp, sizeof(RpCollSplit)))
            {
                RWRETURN((const RpCollTree *)NULL);
            }
        }
    }

    /* Map */
    if (rpCOLLTREE_USEMAP & tree->flags)
    {
        if (!RwStreamWriteInt16(stream,
                (RwInt16 *)tree->map, tree->numEntries * sizeof(RwInt16)))
        {
            RWRETURN((const RpCollTree *)NULL);
        }
    }

    RWRETURN(tree);
}


RpCollTree *
_rpCollTreeStreamRead(RwStream *stream)
{
    RpCollTree         *tree;
    RwUInt32            version;
    rpStreamCollTree    streamCollTree;
    RwBBox              bbox;
    RwUInt32            size;

    RWFUNCTION(RWSTRING("_rpCollTreeStreamRead"));
    RWASSERT(stream);

    /* STRUCT header */
    if (!RwStreamFindChunk(stream, rwID_STRUCT, NULL, &version))
    {
        RWRETURN((RpCollTree *)NULL);
    }

    /* rpStreamCollTree */
    size = sizeof(rpStreamCollTree);
    if (RwStreamRead(stream, &streamCollTree, size) != size)
    {
        RWRETURN((RpCollTree *)NULL);
    }

    (void)RwMemNative32(&streamCollTree, sizeof(rpStreamCollTree));
    (void)RwMemFloat32ToReal(&streamCollTree.inf, sizeof(RwV3d));
    (void)RwMemFloat32ToReal(&streamCollTree.sup, sizeof(RwV3d));

    bbox.inf = streamCollTree.inf;
    bbox.sup = streamCollTree.sup;

    tree = _rpCollTreeCreate(
        (RwUInt16) streamCollTree.numEntries,
        (RwUInt16) streamCollTree.numSplits,
        &bbox, streamCollTree.flags);

    if (!tree)
    {
        RWRETURN((RpCollTree *)NULL);
    }

    /* Splits */
    if (tree->numSplits > 0)
    {
        RwUInt32    i;
        RpCollSplit *split;

        size = tree->numSplits * sizeof(RpCollSplit);
        if (RwStreamRead(stream, tree->splits, size) != size)
        {
            _rpCollTreeDestroy(tree);
            RWRETURN((RpCollTree *)NULL);
        }

        /* Convert to native memory (no effect on most platforms) */
        for (i=0, split=tree->splits; i<tree->numSplits; i++, split++)
        {
            (void)RwMemNative16(&split->leftSector.index, sizeof(RwUInt16));
            (void)RwMemNative16(&split->rightSector.index, sizeof(RwUInt16));
            (void)RwMemNative32(&split->leftSector.value, sizeof(RwReal));
            (void)RwMemFloat32ToReal(&split->leftSector.value, sizeof(RwReal));
            (void)RwMemNative32(&split->rightSector.value, sizeof(RwReal));
            (void)RwMemFloat32ToReal(&split->rightSector.value, sizeof(RwReal));
        }
    }

    /* Map */
    if (rpCOLLTREE_USEMAP & tree->flags)
    {
        if (!RwStreamReadInt16(stream,
                (RwInt16 *)tree->map, tree->numEntries * sizeof(RwInt16)))
        {
            _rpCollTreeDestroy(tree);
            RWRETURN((RpCollTree *)NULL);
        }
    }

    RWRETURN(tree);
}


/* _rpCollTreeStreamReadPre36001
 *
 * Read collision tree from pre 36001 data stream
 *
 * This function may fail in rare cases for incompatibility reasons (the old
 * data format supported >256 entries per leaf node). An error is raised in
 * these circumstances, and failConv is set (streaming should continue so that
 * the app has a chance of reading all data and doing a rebuild of collision).
 *
 */
RpCollTree *
_rpCollTreeStreamReadPre36001(RwStream *stream,
                              RwUInt32  numEntries,
                              RwUInt32  numSplits,
                              RwBBox   *bbox,
                              RwBool   *failConv)
{
    RpCollTree  *tree;

#if rwLIBRARYBASEVERSION >= 0x36001
#error "_rpCollTreeStreamReadPre36001 should now be removed."
#endif

    RWFUNCTION(RWSTRING("_rpCollTreeStreamReadPre36001"));
    RWASSERT(stream);
    RWASSERT((numEntries > 0) && (numEntries < 0x10000));
    RWASSERT(numSplits < 0x10000);
    RWASSERT(bbox);
    RWASSERT(failConv);

    *failConv = FALSE;

    /* Create the tree */
    tree = _rpCollTreeCreate(
        (RwUInt16)numEntries, (RwUInt16)numSplits, bbox, rpCOLLTREE_USEMAP);
    if (!tree)
    {
        RWRETURN((RpCollTree *)NULL);
    }

    /* Splits */
    if (tree->numSplits > 0)
    {
        RpCollSplit    *split;
        RwUInt32        i, numLeafNodes;

        /* Data for branches - allows for zero branches */
        split = tree->splits;
        for (i=0; i<tree->numSplits; i++, split++)
        {
            RwUInt32    types, indices, leftType, rightType;

            if (!(  RwStreamReadInt(stream,
                        (RwInt32 *) &types, sizeof(types))
                 && RwStreamReadInt(stream,
                        (RwInt32 *) &indices, sizeof(indices))
                 && RwStreamReadReal(stream,
                        &split->leftSector.value, sizeof(RwReal))
                 && RwStreamReadReal(stream,
                        &split->rightSector.value, sizeof(RwReal))))
            {
                _rpCollTreeDestroy(tree);
                RWRETURN((RpCollTree *)NULL);
            }

            split->rightSector.type = (RwUInt8) ((types >> 16) & 0xff);
            split->leftSector.type =
                (RwUInt8)(split->rightSector.type | rpCOLLSECTOR_TYPE_NEG);
            split->rightSector.index = (RwUInt16) (indices & 0xffff);
            split->leftSector.index = (RwUInt16) ((indices >> 16) & 0xffff);

            leftType = (types >> 8) & 0xff;
            if (leftType == rpCOLL_PRE36001_BRANCHNODEID)
            {
                split->leftSector.contents = rpCOLLSECTOR_CONTENTS_SPLIT;
            }
            else
            {
                split->leftSector.contents = 0;
            }

            rightType = types & 0xff;
            if (rightType == rpCOLL_PRE36001_BRANCHNODEID)
            {
                split->rightSector.contents = rpCOLLSECTOR_CONTENTS_SPLIT;
            }
            else
            {
                split->rightSector.contents = 0;
            }
        }

        /* Leaf data */
        {
            RpCollSector   start;
            RpCollSector  *curr, *stack[rpCOLLTREE_MAXDEPTH+1];
            RwUInt32       nStack;

            start.contents = rpCOLLSECTOR_CONTENTS_SPLIT;
            start.index = 0;
            curr = &start;
            nStack = 1;
            numLeafNodes = 0;
            while (nStack > 0)
            {
                if (curr->contents != rpCOLLSECTOR_CONTENTS_SPLIT)
                {
                    RwUInt16    count;
                    RwUInt32    data;

                    RWASSERT(curr->index == numLeafNodes);
                    if (!RwStreamReadInt(stream, (RwInt32 *)&data, sizeof(data)))
                    {
                        _rpCollTreeDestroy(tree);
                        RWRETURN((RpCollTree *)NULL);
                    }

                    count = (RwUInt16)(data >> 16);

                    /* We now only allow 8bits for the count, this should always
                     * be enough, but we must check.
                     */
                    if (count > rpCOLLSECTOR_CONTENTS_MAXCOUNT)
                    {
                        *failConv = TRUE;
                        _rpCollTreeDestroy(tree);

                        RWMESSAGE((RWSTRING("Unable to convert pre 3.6001 "
                            "collision data. Requires re-building via API or "
                            "re-exporting.")));

                        RWERROR((E_RP_COLLIS_STREAM_FAILURE));
                        RWRETURN((RpCollTree *)NULL);
                    }

                    curr->contents = (RwUInt8) count;
                    curr->index = (RwUInt16) (data & 0xffff);
                    numLeafNodes++;
                    curr = stack[--nStack];
                }
                else
                {
                    stack[nStack++] = &tree->splits[curr->index].rightSector;
                    curr = &tree->splits[curr->index].leftSector;
                }
            }
        }

        RWASSERT(numLeafNodes == (RwUInt32)(tree->numSplits + 1));
    }
    else
    {
        /* Single leaf node - we can skip it since data is redundant */
        if (!RwStreamSkip(stream, sizeof(RwInt32)))
        {
            _rpCollTreeDestroy(tree);
            RWRETURN((RpCollTree *)NULL);
        }
    }

    /* Map */
    if (rpCOLLTREE_USEMAP & tree->flags)
    {
        RwUInt32     i;

        for (i=0; i < tree->numEntries; i++)
        {
            RwInt32  index;

            if (!RwStreamReadInt32(stream, &index, sizeof(RwInt32)))
            {
                _rpCollTreeDestroy(tree);
                RWRETURN((RpCollTree *)NULL);
            }

            tree->map[i] = (RwUInt16) index;
        }
    }

    RWRETURN(tree);
}

/******************************************************************************
 */


#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/**
 * \ingroup rpcollis
 * \ref RpCollTreeGetEntryMap retrieves the mapping table of entries
 * (ie triangles of a world sector or geometry) in a collision tree.
 *
 * \param tree   Pointer to the collision tree.
 *
 * \return Pointer to an array of entry indices, or NULL if the collision
 *         tree has no mapping table.
 *
 * The collision tree relies on a specific order of entries where any
 * \ref RpCollSector node of the tree contains a contiguous block of the
 * entries. Where it is possible to re-order the entries after generation
 * of the collision tree, a mapping table is not required
 * (see \ref RpCollisionBuildParam).
 *
 * \see \ref rpcollisoverview
 */
RwUInt16 *
RpCollTreeGetEntryMap(RpCollTree *tree)
{
    RWAPIFUNCTION(RWSTRING("RpCollTreeGetEntryMap"));
    RWRETURN(tree->map);
}

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */
