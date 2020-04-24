/*
 *  p2macros
 */


/* AutoDuck comments for API macros */

/* slightly concerned that some compilers might not generate an object file,
   and hence confuse make, so... */

#include "batypes.h"

extern int          _rxNotAFunction17(int x);

int
_rxNotAFunction17(int x)
{
#if 0
    RWFUNCTION(RWSTRING("_rxNotAFunction17"));
#endif /* 0 */
    return x * x;
}

#if (defined(DOXYGEN))

/* brackets entire file! */

/**
 * \ingroup rwcoregeneric
 * \ref RxClusterDecCursorByStride decrements the cluster's data
 * array cursor by "stride" bytes.
 *
 * \param cluster  A pointer to a cluster
 * \param stride  An \ref RwInt32 equal to the number of bytes by which to decrement the cluster's cursor
 *
 * \return A pointer to the cursor in its new location
 *
 * \see RxClusterResetCursor
 * \see RxClusterGetCursorData
 * \see RxClusterIncCursor
 * \see RxClusterDecCursor
 * \see RxClusterIncCursorByStride
 */

void               *
RxClusterDecCursorByStride(RxCluster * cluster, RwUInt32 stride)
{
    RWAPIFUNCTION(RWSTRING("RxClusterDecCursorByStride"));

    RWRETURN(NULL);
}

/**
 * \ingroup rwcoregeneric
 * \ref RxClusterDecCursor decreases the cluster's data array cursor
 * by the array's stride.
 *
 * \param cluster  A pointer to a cluster.
 *
 * \return A pointer to the cursor in its new location.
 *
 * \see RxClusterResetCursor
 * \see RxClusterGetCursorData
 * \see RxClusterIncCursor
 * \see RxClusterIncCursorByStride
 * \see RxClusterDecCursorByStride
 */

void               *
RxClusterDecCursor(RxCluster * cluster)
{
    RWAPIFUNCTION(RWSTRING("RxClusterDecCursor"));

    RWRETURN(NULL);
}

/**
 * \ingroup rwcoregeneric
 * \ref RxClusterIncCursorByStride increments the cluster's data
 * array cursor by "stride" bytes.
 *
 * \param cluster  A pointer to a cluster
 * \param stride   An \ref RwUInt32 equal to the number of bytes by which to
 * increment the cluster's cursor
 *
 * \return A pointer to the cursor in its new location
 *
 * \see RxClusterResetCursor
 * \see RxClusterGetCursorData
 * \see RxClusterIncCursor
 * \see RxClusterDecCursor
 * \see RxClusterDecCursorByStride
 */

void               *
RxClusterIncCursorByStride(RxCluster * cluster, RwUInt32 stride)
{
    RWAPIFUNCTION(RWSTRING("RxClusterIncCursorByStride"));

    RWRETURN(NULL);
}

/**
 * \ingroup rwcoregeneric
 * \ref RxClusterIncCursor increases the cluster's data array cursor
 * by the array's stride.
 *
 * \param cluster  A pointer to a cluster.
 *
 * \return A pointer to the cursor in its new location.
 *
 * \see RxClusterResetCursor
 * \see RxClusterGetCursorData
 * \see RxClusterIncCursor
 * \see RxClusterDecCursor
 * \see RxClusterIncCursorByStride
 * \see RxClusterDecCursorByStride
 */

void               *
RxClusterIncCursor(RxCluster * cluster)
{
    RWAPIFUNCTION(RWSTRING("RxClusterIncCursor"));

    RWRETURN(NULL);
}

/**
 * \ingroup rwcoregeneric
 * \ref RxClusterResetCursor resets the cluster's data array cursor, so
 * that it points to the first element in the cluster's data array.
 *
 * A cursor reset is performed automatically by the API after
 * \ref RxClusterLockRead, \ref RxClusterLockWrite, \ref RxClusterInitializeData,
 * \ref RxClusterResizeData, \ref RxClusterSetData and
 * \ref RxClusterSetExternalData.
 *
 * \param cluster  A pointer to a cluster
 *
 * \return A pointer to the cluster's cursor in its new location.
 *
 * \see RxClusterGetCursorData
 * \see RxClusterIncCursor
 * \see RxClusterDecCursor
 * \see RxClusterIncCursorByStride
 * \see RxClusterDecCursorByStride
 * \see RxClusterLockRead
 * \see RxClusterLockWrite
 * \see RxClusterInitializeData
 * \see RxClusterResizeData
 * \see RxClusterSetData
 * \see RxClusterSetExternalData
 */

void               *
RxClusterResetCursor(RxCluster * cluster)
{
    RWAPIFUNCTION(RWSTRING("RxClusterResetCursor"));

    RWRETURN(NULL);
}

/**
 * \ingroup rwcoregeneric
 * \ref RxClusterGetCursorData is a macro that returns a reference to the
 * element of the cluster's data array pointed to by the data array cursor.
 *
 * \param cluster  A pointer to a cluster
 * \param TYPE  The type of the data
 *
 * \return A pointer to the cluster's cursor
 *
 * \see RxClusterResetCursor
 * \see RxClusterIncCursor
 * \see RxClusterDecCursor
 * \see RxClusterIncCursorByStride
 * \see RxClusterDecCursorByStride
 */

TYPE               *
RxClusterGetCursorData(RxCluster * cluster, TYPE * TYPE)
{
    RWAPIFUNCTION(RWSTRING("RxClusterGetCursorData"));

    RWRETURN(NULL);
}

/**
 * \ingroup rwcoregeneric
 * \ref RxClusterGetIndexedData returns a reference to the element
 * of the cluster's data array specified by the given index.
 *
 * \param cluster A pointer to a cluster
 * \param TYPE    The type of the data
 * \param index   An \ref RwUInt32 equal to the index of the desired element
 *
 * \return A pointer to the indexed data element
 *
 * \see RxClusterResetCursor
 * \see RxClusterGetCursorData
 * \see RxClusterIncCursor
 * \see RxClusterDecCursor
 * \see RxClusterIncCursorByStride
 * \see RxClusterDecCursorByStride
 */

TYPE               *
RxClusterGetIndexedData(RxCluster * cluster, TYPE * TYPE, RwUInt32 index)
{
    RWAPIFUNCTION(RWSTRING("RxClusterGetIndexedData"));

    RWRETURN(NULL);
}

/**
 * \ingroup rwcoregeneric
 * \ref RxClusterGetFreeIndex returns an index one beyond the last
 * used element in the cluster's data array. Additionally increments the
 * numUsed member of the cluster.
 *
 * It is the responsibility of the user to test that the array is large
 * enough to hold a new entry - i.e. that RxClusterGetFreeIndex(cl) is less
 * than the cluster and greater than numAlloced - and to
 * \ref RxClusterResizeData as necessary.
 *
 * \param cluster  A pointer to a cluster
 *
 * \return An \ref RwUInt32 equal to the index of the first free element in the cluster
 *
 * \see RxClusterResetCursor
 * \see RxClusterGetCursorData
 * \see RxClusterIncCursor
 * \see RxClusterDecCursor
 * \see RxClusterIncCursorByStride
 * \see RxClusterDecCursorByStride
 */

RwUInt32
RxClusterGetFreeIndex(RxCluster * cluster)
{
    RWAPIFUNCTION(RWSTRING("RxClusterGetFreeIndex"));

    RWRETURN(NULL);
}

/**
 * \ingroup rwcoregeneric
 * \ref RxPipelineClusterAssertAttributeSet provides a convenient
 * debugging test that can be made if one plans to manipulate a cluster's
 * attributes. See \ref RxPipelineClusterSetCreationAttributes for
 * further information.
 *
 * \param cluster  A pointer to a cluster
 * \param attributeSet  A pointer to the name string of the attribute set you wish use
 *
 * \return None
 *
 * \see RxPipelineClusterSetCreationAttributes
 */

void
RxPipelineClusterAssertAttributeSet(RxCluster * cluster,
                                    RwChar *attributeSet)
{
    RWAPIFUNCTION(RWSTRING("RxPipelineClusterAssertAttributeSet"));

    RWRETURNVOID();
}

/**
 * \ingroup rwcoregeneric
 * \ref RxPipelineNodeParamGetData gets the data pointer which
 * was passed in to \ref RxPipelineExecute for the current pipeline
 * execution.
 *
 * \param param  A pointer to the current pipeline node's params.
 *
 * \return A pointer to the current pipeline execution data.
 *
 * \see RxPipelineExecute
 * \see RxPipelineNodeParamGetHeap
 */
void               *
RxPipelineNodeParamGetData(RxPipelineNodeParam * param)
{
    RWAPIFUNCTION(RWSTRING("RxPipelineNodeParamGetData"));

    RWRETURNVOID();
}

/**
 * \ingroup rwcoregeneric
 * \ref RxPipelineNodeParamGetHeap
 * gets the \ref RxHeap in use
 * by the current pipeline execution
 *
 * \param param  A pointer to the current pipeline node's params.
 *
 * \return A pointer to the current heap.
 *
 * \see RxPipelineExecute
 * \see RxPipelineNodeParamGetData
 */
RxHeap             *
RxPipelineNodeParamGetHeap(RxPipelineNodeParam * param)
{
    RWAPIFUNCTION(RWSTRING("RxPipelineNodeParamGetHeap"));

    RWRETURNVOID();
}

#endif /* (defined(DOXYGEN)) */


