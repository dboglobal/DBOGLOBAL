/*
 * Handling binary matrix representations.
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 */

/****************************************************************************
 Includes
 */

#include <stdlib.h>
#include <string.h>

#include "batypes.h"
#include "batype.h"
#include "balibtyp.h"
#include "badebug.h"
#include "bamemory.h"

#include "bamatrix.h"

#include "babinary.h"
#include "batkreg.h"
#include "batkbin.h"
#include "bavector.h"

#include "babinmtx.h"

/****************************************************************************
 Local Types
 */

/****************************************************************************
 Local (Static) Prototypes
 */

/****************************************************************************
 Local Defines
 */

#ifndef _ORTHONORMAL_TOL
#define _ORTHONORMAL_TOL ((RwReal)0.01)
#endif /* _ORTHONORMAL_TOL */

#ifndef _IDENTITY_TOL
#define _IDENTITY_TOL ((RwReal)0.01)
#endif /* _IDENTITY_TOL */

/****************************************************************************
 Globals (across program)
 */

/****************************************************************************
 Local (static) Globals
 */

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Matrix Binary Format Functions

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwmatrix
 * \ref RwMatrixStreamGetSize determines the size in bytes of the binary
 * representation of the given matrix. This is used in the binary chunk
 * header to indicate the size of the chunk.
 *
 * \param  matrix   A pointer to a matrix for the size that is required
 *
 * \return Returns the chunk size of the matrix in bytes or zero if
 * there an error
 *
 * \see RwMatrixStreamRead
 * \see RwMatrixStreamWrite
 */
RwUInt32
RwMatrixStreamGetSize( const RwMatrix * matrix __RWUNUSEDRELEASE__ )
{
    const RwUInt32 result = sizeof(rwStreamMatrix) + rwCHUNKHEADERSIZE;
    RWAPIFUNCTION(RWSTRING("RwMatrixStreamGetSize"));
    RWASSERT(matrix);

    RWRETURN(result);
}

/**
 * \ingroup rwmatrix
 * \ref RwMatrixStreamWrite writes the specified matrix to a binary
 * stream.
 * \note The stream will have been opened prior to this function
 * call.
 *
 * \param matrix  A pointer to the matrix to be written
 * \param stream  A pointer to the stream
 *
 * \return Returns a pointer to the matrix if successful or NULL if there is
 * an error
 *
 * \see RwMatrixStreamRead
 * \see RwMatrixStreamGetSize
 * \see RwStreamOpen
 * \see RwStreamClose
 *
 */
const RwMatrix *
RwMatrixStreamWrite(const RwMatrix *matrix, RwStream *stream)
{
    RwBool      status;
    rwStreamMatrix   mat;
    RwStream *check;
    RwMatrix source;

    RWAPIFUNCTION(RWSTRING("RwMatrixStreamWrite"));
    RWASSERT(matrix);
    RWASSERT(stream);

    rwMatrixInitialize(&source, 0);

    if (rwMATRIXTYPEORTHONORMAL ==
        rwMatrixTestFlags(matrix, rwMATRIXTYPEMASK) )
    {
        RwMatrixOrthoNormalize(&source, matrix);
    }
    else
    {
        RwMatrixCopy(&source, matrix);
    }

    check = RwStreamWriteChunkHeader(stream,
                                     rwID_MATRIX,
                                     RwMatrixStreamGetSize(&source));

    status =  ( NULL !=  check);
    if (!status)
    {
        RWERROR((E_RW_WRITE));
        RWRETURN((const RwMatrix *)NULL);
    }

    check = RwStreamWriteChunkHeader(stream, rwID_STRUCT, sizeof(mat));

    status = ( NULL != check );
    if (!status)
    {
        RWERROR((E_RW_WRITE));
        RWRETURN((const RwMatrix *)NULL);
    }

    /* Fill it */
    mat.type = rwMatrixTestFlags(&source, rwMATRIXTYPEMASK);

    RwV3dAssign(&mat.right, &source.right);
    RwV3dAssign(&mat.up, &source.up);
    RwV3dAssign(&mat.at, &source.at);
    RwV3dAssign(&mat.pos, &source.pos);

    /* Convert it */
    (void)RwMemRealToFloat32(&mat.right, sizeof(mat.right));
    (void)RwMemRealToFloat32(&mat.up, sizeof(mat.up));
    (void)RwMemRealToFloat32(&mat.at, sizeof(mat.at));
    (void)RwMemRealToFloat32(&mat.pos, sizeof(mat.pos));
    (void)RwMemLittleEndian32(&mat, sizeof(mat));

    /* Write it */
    status = ( NULL !=  RwStreamWrite(stream, &mat, sizeof(mat)));
    if (!status)
    {
        RWERROR((E_RW_WRITE));
        matrix = (const RwMatrix *)NULL;
    }

    RWRETURN(matrix);
}

/**
 * \ingroup rwmatrix
 * \ref RwMatrixChunkInfoRead extracts Chunk Info data from a RenderWare
 * stream. The data is converted from its original format and inserted into
 * an RwMatrixChunkInfo structure. A pointer to this structure is returned
 * on success.
 *
 * \param stream  A pointer to the RenderWare stream object
 * \param matrixChunkInfo  A pointer to the matrixChunkInfo structure
 * to be filled by the data
 * \param bytesRead  A pointer to an \ref RwInt32 that will hold the number
 * of bytes read
 *
 * \return Returns a pointer to the filled structure if successful.
 * Returns NULL if an error occurred.
 *
 * \see RwMatrixStreamRead
 * \see RwMatrixStreamWrite
 * \see RwStreamOpen
 * \see RwStreamClose
 * \see RwStreamFindChunk
 */

RwMatrixChunkInfo *
RwMatrixChunkInfoRead(RwStream *stream,
                      RwMatrixChunkInfo *matrixChunkInfo,
                      RwInt32 *bytesRead)
{
    RwBool              status;
    RwUInt32            size;
    RwUInt32            readSize;

    RWAPIFUNCTION(RWSTRING("RwMatrixChunkInfoRead"));
    RWASSERT(stream);
    RWASSERT(matrixChunkInfo);

    status = RwStreamFindChunk(stream, (RwUInt32)rwID_STRUCT,
                               &size, (RwUInt32 *)NULL);

    if (!status)
    {
        RWERROR((E_RW_READ));
        RWRETURN((rwStreamMatrix *)NULL);
    }

    RWASSERT(size < sizeof(RwMatrixChunkInfo));
    readSize = sizeof(RwMatrixChunkInfo);
    memset(matrixChunkInfo, 0, readSize);

    status = ( readSize ==
               RwStreamRead(stream, matrixChunkInfo, readSize) );

    if (!status)
    {
        RWERROR((E_RW_READ));
        RWRETURN((rwStreamMatrix *)NULL);
    }

    *bytesRead = size + (sizeof(RwInt32) * 3);
    /* move on to known place */
    RwStreamSkip(stream, size - readSize);

    (void)RwMemNative32(matrixChunkInfo, sizeof(RwMatrixChunkInfo));
    (void)RwMemFloat32ToReal(&matrixChunkInfo->right,
                       sizeof(matrixChunkInfo->right));
    (void)RwMemFloat32ToReal(&matrixChunkInfo->up, sizeof(matrixChunkInfo->up));
    (void)RwMemFloat32ToReal(&matrixChunkInfo->at, sizeof(matrixChunkInfo->at));
    (void)RwMemFloat32ToReal(&matrixChunkInfo->pos, sizeof(matrixChunkInfo->pos));

    RWRETURN(matrixChunkInfo);
}

/**
 * \ingroup rwmatrix
 * \ref RwMatrixStreamRead reads a matrix from a binary stream. If the
 * matrix argument is NULL then a new matrix is created, otherwise the
 * specified one is over-written.
 * \note Prior to this function call a
 * binary matrix chunk must be found in the stream using the
 * \ref RwStreamFindChunk API function.
 *
 * The sequence to locate and read a matrix from a binary stream connected
 * to a disk file is as follows:
 *
 * \code
    RwStream *stream;
    RwMatrix *matrix;

    stream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, "mybinary.xxx");
    if( stream )
    {
        if( RwStreamFindChunk(stream, rwID_MATRIX, NULL, NULL) )
        {
            matrix = RwMatrixStreamRead(stream, NULL);
        }

        RwStreamClose(stream, NULL);
    }
    \endcode
 *
 * The second parameter is useful if you have a matrix allocated on the stack
 * so you could also do:
 *
 * \code
    RwStream *stream;
    RwMatrix matrix;

    stream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, "mybinary.xxx");
    if( stream )
    {
        if( RwStreamFindChunk(stream, rwID_MATRIX, NULL, NULL) )
        {
            RwMatrixStreamRead(stream, &matrix);
        }

        RwStreamClose(stream, NULL);
    }
    \endcode
 *
 * \param stream  A pointer to the stream from which the matrix will be read
 * \param matrix  A pointer to a matrix which will receive the data from the stream.
 *       If one is not provided by the application (NULL is passed),
 *       then one will be created
 *
 * \return Returns pointer to the matrix if successful or NULL if there
 *        is an error.
 *
 * \see RwMatrixStreamWrite
 * \see RwMatrixStreamGetSize
 * \see RwStreamOpen
 * \see RwStreamClose
 * \see RwStreamFindChunk
 *
 */
RwMatrix *
RwMatrixStreamRead(RwStream *stream, RwMatrix *matrix)
{
    RwBool              status;
    RwUInt32            size;
    RwUInt32            version;

    RWAPIFUNCTION(RWSTRING("RwMatrixStreamRead"));
    RWASSERT(stream);

    status = RwStreamFindChunk(stream, rwID_STRUCT, &size, &version);

    if (status)
    {
        if ((version >= rwLIBRARYBASEVERSION) &&
            (version <= rwLIBRARYCURRENTVERSION))
        {
            rwStreamMatrix           mat;
            RwUInt32                 bytes;

            RWASSERT(size <= sizeof(mat));

            memset(&mat, 0, sizeof(mat));

            bytes = RwStreamRead(stream, &mat, size) ;

            status = ( size == bytes );

            if (status)
            {
                (void)RwMemNative32(&mat, sizeof(mat));

                (void)RwMemFloat32ToReal(&mat.right, sizeof(mat.right));
                (void)RwMemFloat32ToReal(&mat.up, sizeof(mat.up));
                (void)RwMemFloat32ToReal(&mat.at, sizeof(mat.at));
                (void)RwMemFloat32ToReal(&mat.pos, sizeof(mat.pos));

                if (!matrix)
                {
                    /* need to create it */
                    matrix = RwMatrixCreate();
                }

                if (matrix)
                {
                    /* Copy over the type and flags */
                    const RwUInt32 flags = ( mat.type &
                                             (RwInt32) rwMATRIXTYPEMASK);

                    rwMatrixInitialize(matrix, 0);

                    *RwMatrixGetRight(matrix) = mat.right;
                    *RwMatrixGetUp(matrix) = mat.up;
                    *RwMatrixGetAt(matrix) = mat.at;
                    *RwMatrixGetPos(matrix) = mat.pos;

                    /* Copy the matrix type */
                    rwMatrixSetFlags(matrix, flags);

                    if ( rwMATRIXTYPEORTHONORMAL ==
                         (rwMATRIXTYPEORTHONORMAL & flags) )
                    {
                        RwMatrixOrthoNormalize(matrix, matrix);
                    }

                    /* Check flags */
                    RWASSERT( ( (rwMATRIXTYPEORTHONORMAL & flags) !=
                                (rwMATRIXTYPEORTHONORMAL) ) ||
                              rwMatrixIsOrthonormal(matrix,
                                                    _ORTHONORMAL_TOL));

                    RWASSERT( ( (rwMATRIXINTERNALIDENTITY & flags) !=
                                (rwMATRIXINTERNALIDENTITY) ) ||
                              rwMatrixIsIdentity(matrix, _IDENTITY_TOL) );
                }
            }
            else
            {
                RWERROR((E_RW_READ));
                RWRETURN((RwMatrix *)NULL);
            }
        }
        else
        {
            RWERROR((E_RW_BADVERSION));
            matrix = (RwMatrix *)NULL;
        }
    }
    else
    {
        RWERROR((E_RW_READ));
        matrix = ((RwMatrix *)NULL);
    }

    RWRETURN(matrix);
}
