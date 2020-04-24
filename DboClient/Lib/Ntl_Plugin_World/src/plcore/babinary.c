/****************************************************************************
 *
 * Memory handling functions.
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 *
 */

/****************************************************************************
 Includes
 */

#include <stdlib.h>
#include <string.h>

#include "batypes.h"
#include "bastream.h"
#include "balibtyp.h"
#include "batype.h"
#include "badebug.h"
#include "bamemory.h"
#include "babinary.h"

/****************************************************************************
 Local Types
 */

/* types to be written off to a stream */

typedef struct __rwMark _rwMark;

struct __rwMark
{
        RwUInt32            type;
        RwUInt32            length;
        RwUInt32            libraryID;
};

/* See rwversion.h */
#define VersionIsValidMacro(_version)   \
    (((_version) > 0x30000U) && ((((_version) - 0x30000U) & ~0x3ff3f) == 0))


/****************************************************************************
 Local (Static) Prototypes
 */

/****************************************************************************
 Local Defines
 */

#if (defined(rwLITTLEENDIAN) && !defined(RWDEBUG))
#if (!(defined(__RWUNUSEDRELEASELITTLEENDIAN__)))
#define __RWUNUSEDRELEASELITTLEENDIAN__ __RWUNUSED__
#endif /* (!(defined(__RWUNUSEDRELEASELITTLEENDIAN__))) */
#endif /* (defined(rwLITTLEENDIAN) && !defined(RWDEBUG)) */

#if (!(defined(__RWUNUSEDRELEASELITTLEENDIAN__)))
#define __RWUNUSEDRELEASELITTLEENDIAN__ /* No op */
#endif /* (!(defined(__RWUNUSEDRELEASELITTLEENDIAN__))) */

#ifndef RWLONGSWAP
#define RWLONGSWAP(x, y)                                        \
   (*(RwUInt32 *)(x)) = (((*(RwUInt32 *)(y))>>24) |             \
                         (((*(RwUInt32 *)(y))>>8)&0xff00UL) |   \
                         (((*(RwUInt32 *)(y))<<8)&0xff0000UL) | \
                         ((*(RwUInt32 *)(y)<<24)))
#endif /* LONG_SWAP */

#define RWSHORTSWAP(x, y)                           \
    (*(RwUInt16 *)(x)) = (((*(RwUInt16 *)(y))>>8) | \
                          ((*(RwUInt16 *)(y))<<8) )


#ifdef rwLITTLEENDIAN

#define RWLITTLEENDIANLONG(x, y)    (*(RwUInt32 *)x) = (*(RwUInt32 *)y)
#define RWNATIVELONG(x, y)          (*(RwUInt32 *)x) = (*(RwUInt32 *)y)
#define RWLITTLEENDIANSHORT(x, y)    (*(RwUInt16 *)x) = (*(RwUInt16 *)y)
#define RWNATIVESHORT(x, y)          (*(RwUInt16 *)x) = (*(RwUInt16 *)y)

#else /* rwLITTLEENDIAN */

#define RWLITTLEENDIANLONG(x, y)    RWLONGSWAP(x, y)
#define RWNATIVELONG(x, y)          RWLONGSWAP(x, y)
#define RWLITTLEENDIANSHORT(x, y)    RWSHORTSWAP(x, y)
#define RWNATIVESHORT(x, y)          RWSHORTSWAP(x, y)

#endif /* rwLITTLEENDIAN */

/****************************************************************************
 Globals (across program)
 */

/****************************************************************************
 Local Globals
 */

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Static functions

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

static              RwBool
ChunkIsComplex(RwChunkHeaderInfo * chunkHeaderInfo)
{
    RwBool              result = FALSE;

    RWFUNCTION(RWSTRING("ChunkIsComplex"));
    RWASSERT(chunkHeaderInfo);

    switch (chunkHeaderInfo->type)
    {

        case rwID_STRUCT:
            {
                result = (FALSE);
                break;
            }

        case rwID_STRING:
            {
                result = (FALSE);
                break;
            }

        case rwID_EXTENSION:
            {
                result = (FALSE);
                break;
            }

        case rwID_CAMERA:
            {
                result = (TRUE);
                break;
            }

        case rwID_TEXTURE:
            {
                result = (TRUE);
                break;
            }

        case rwID_MATERIAL:
            {
                result = (TRUE);
                break;
            }

        case rwID_MATLIST:
            {
                result = (TRUE);
                break;
            }

        case rwID_ATOMICSECT:
            {
                result = (TRUE);
                break;
            }

        case rwID_PLANESECT:
            {
                result = (TRUE);
                break;
            }

        case rwID_WORLD:
            {
                result = (TRUE);
                break;
            }

        case rwID_MATRIX:
            {
                result = (FALSE);
                break;
            }

        case rwID_FRAMELIST:
            {
                result = (TRUE);
                break;
            }

        case rwID_GEOMETRY:
            {
                result = (TRUE);
                break;
            }

        case rwID_CLUMP:
            {
                result = (TRUE);
                break;
            }

        case rwID_LIGHT:
            {
                result = (TRUE);
                break;
            }

        case rwID_UNICODESTRING:
            {
                result = (FALSE);
                break;
            }

        case rwID_ATOMIC:
            {
                result = (TRUE);
                break;
            }

        case rwID_GEOMETRYLIST:
            {
                result = (TRUE);
                break;
            }

        default:
            {
                result = (FALSE);
                break;
            }
    }

    RWRETURN(result);
}

/****************************************************************************
 _rwStreamReadChunkHeader

 On entry   : stream
            : stream chunk id (OUT)
            : stream chunk length (OUT)
            : stream chunk version (OUT)
            : stream chunk lib build number (OUT)
 On exit    : TRUE if found, else FALSE
 */

RwBool
_rwStreamReadChunkHeader(RwStream * stream, RwUInt32 * type,
                         RwUInt32 * length, RwUInt32 * version,
                         RwUInt32 * buildNum)
{
    RwBool              status;
    _rwMark             mark;
     RwChunkHeaderInfo  chunkHdrInfo;

    RWFUNCTION(RWSTRING("_rwStreamReadChunkHeader"));
    RWASSERT(stream);

    /* Read it */
    status = ( RwStreamRead(stream, &mark, sizeof(mark)) == sizeof(mark) );

    if (!status)
    {
        /* Ooops, failed somewhere */
        RWERROR((E_RW_READ));
        RWRETURN(FALSE);
    }
    else
    {
        /* Convert it */
        (void)RwMemNative32(&mark, sizeof(mark));

        /* Use it */
        chunkHdrInfo.type = mark.type;
        chunkHdrInfo.length = mark.length;

        /* Check for old library ID */
        if (!(mark.libraryID & 0xffff0000))
        {
            /* Just contains old-style version number */
            chunkHdrInfo.version = mark.libraryID << 8;
            chunkHdrInfo.buildNum = 0;
        }
        else
        {
            /* Unpack new library ID */
            chunkHdrInfo.version =
                RWLIBRARYIDUNPACKVERSION(mark.libraryID);
            chunkHdrInfo.buildNum =
                RWLIBRARYIDUNPACKBUILDNUM(mark.libraryID);
        }

        chunkHdrInfo.isComplex = ChunkIsComplex(&chunkHdrInfo);

        if (type)
        {
            *type = chunkHdrInfo.type;
        }

        if (length)
        {
            *length = chunkHdrInfo.length;
        }

        if (buildNum)
        {
            *buildNum = chunkHdrInfo.buildNum;
        }

        if (version)
        {
            *version = chunkHdrInfo.version;
        }

        RWRETURN(TRUE);
    }

}

RwStream           *
_rwStreamWriteVersionedChunkHeader(RwStream * stream, RwInt32 type,
                                   RwInt32 size, RwUInt32 version,
                                   RwUInt32 buildNum)
{
    _rwMark             mark;
    RwStream           *streamRet;

    /* We fake the function name - usually called through a #define */
    RWFUNCTION(RWSTRING("_rwStreamWriteVersionedChunkHeader"));
    RWASSERT(stream);
    RWASSERT(VersionIsValidMacro(version));
    RWASSERT((buildNum & ~0xffff) == 0);

    /* This function replaces RwStreamWriteChunkHeader(),
     * at end of file */
    /* Fill it */
    mark.type = type;
    mark.length = size;
    mark.libraryID = RWLIBRARYIDPACK(version, buildNum);

    /* Convert it */
    (void)RwMemLittleEndian32(&mark, sizeof(mark));

    /* Write it */
    streamRet = RwStreamWrite(stream, &mark, sizeof(mark));

    RWRETURN(streamRet);
}

/**
 * \ingroup rwstream
 * \ref RwStreamFindChunk is used to scan through the specified stream
 * looking for the first occurrence of a chunk of the given type. This function
 * can be used in conjunction with the appropriate stream read function
 * to load objects and data from RenderWare binary files.
 *
 * This function will produce warning messages if old binary files are streamed
 * in. Depending on the binary file's version stamp, it may be still be read
 * successfully or fail. It is recommended that all old binary files be
 * updated to newer versions, by re-exporting, if the warning messages appears.
 *
 * \param stream  Pointer to the stream to scan through.
 * \param type  A RwUInt32 value equal to the chunk's identifier listed in
 * \ref RwCorePluginID:
 *
 * \param lengthOut  Pointer to a RwUInt32 value that will receive the length in bytes
 * of the chunk data if it is found. Set to NULL if not required.
 * \param versionOut  Pointer to a RwUInt32 value that will receive the version
 * of the chunk data if it is found. Set to NULL if not required. See
 * \ref RwEngineGetVersion.
 *
 * \return Returns TRUE if the chunk was found or FALSE if not or if there
 * is an error.
 *
 * \see RwCameraStreamRead
 * \see RpClumpStreamRead
 * \see RpGeometryStreamRead
 * \see RpLightStreamRead
 * \see RpMaterialStreamRead
 * \see RwMatrixStreamRead
 * \see RwTextureStreamRead
 * \see RpWorldStreamRead
 * \see RwStreamWriteChunkHeader
 * \see RwCorePluginID
 */
RwBool
RwStreamFindChunk(RwStream * stream, RwUInt32 type, RwUInt32 * lengthOut,
                  RwUInt32 * versionOut)
{
    RwUInt32            readType;
    RwUInt32            readLength;
    RwUInt32            readVersion;

    RWAPIFUNCTION(RWSTRING("RwStreamFindChunk"));
    RWASSERT(stream);

    while (_rwStreamReadChunkHeader(stream, &readType,
                                   &readLength, &readVersion,
                                   (RwUInt32 *) NULL))
	{
        if (readType == type)
		{
            /* Reject invalid versions */
            if (readVersion < rwLIBRARYBASEVERSION) // 217088
            {
                RWERROR((E_RW_BADVERSION));
                RWMESSAGE((RWSTRING("Old version 0x%x < rwLIBRARYBASEVERSION 0x%x, "
                                    "and should be converted with an older library"),
                                    readVersion, rwLIBRARYBASEVERSION));
                RWRETURN(FALSE);
            }
            else if (readVersion > rwLIBRARYCURRENTVERSION)
            {
                RWERROR((E_RW_BADVERSION));
                RWMESSAGE((RWSTRING("Version 0x%x is newer than rwLIBRARYCURRENTVERSION"
                                    " 0x%x"),
                                    readVersion, rwLIBRARYCURRENTVERSION));
                RWRETURN(FALSE);
            }
            else if (readVersion < rwLIBRARYNOWARNVERSION) // 221184
            {
                /* Warn the user but still read it. */
                RWMESSAGE((RWSTRING("Old version data (0x%0x) encountered,"
                                    " content may be sub-optimal and may require"
                                    " conversion during streaming. Please see the"
                                    " UpdatingYourRWGAssets whitepaper for details"),
                           (unsigned int)readVersion));
            }

            /* Found it, return the values */
            if (lengthOut)
            {
                *lengthOut = readLength;
            }
            if (versionOut)
            {
                *versionOut = readVersion;
            }

            RWRETURN(TRUE);
        }

        if (!RwStreamSkip(stream, readLength))
        {
            RWRETURN(FALSE);
        }
    }

    /* Oh, well, the chunk hasn't been found */
    RWRETURN(FALSE);
}

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE) || defined(rwBIGENDIAN))

/**
 * \ingroup rwmem
 * \ref RwMemLittleEndian32 converts a specified chunk of memory to a
 * little endian format suitable for writing out to a binary file.  The binary
 * format specifies little endian as a standard to permit compatibility across
 * different endian-ness machines.
 *
 * \param mem  Pointer to the memory to convert
 * \param size  Amount of memory to prepare (multiple of 4 bytes).
 *
 * \return Returns the address of the value if its format has been converted
 * successfully, or NULL on failure.
 *
 * \see RwMemNative32
 * \see RwMemFloat32ToReal
 * \see RwMemRealToFloat32
 * \see RwStreamReadInt32
 * \see RwStreamWriteInt32
 * \see RwStreamWriteReal
 * \see RwStreamReadReal
 * \see RwStreamWrite
 * \see RwStreamRead
 *
 */
void               *
RwMemLittleEndian32(void *mem,
                    RwUInt32 __RWUNUSEDRELEASELITTLEENDIAN__ size)
{

    RWAPIFUNCTION(RWSTRING("RwMemLittleEndian32"));
    RWASSERT(mem);
    RWASSERT((size & 0x00000003L) == 0);

#ifdef rwBIGENDIAN
    {
        RwUInt32           *memInt = (RwUInt32 *) mem;

        for (size >>= 2 /* Divide by 4 */ ; size > 0; size--)
        {
            RWLITTLEENDIANLONG(memInt, memInt);
            memInt++;
        }
    }

#endif /* rwBIGENDIAN */

    RWRETURN(mem);
}

/**
 * \ingroup rwmem
 * \ref RwMemLittleEndian16 converts a specified chunk of memory to a
 * little endian format suitable for writing out to a binary file.  The binary
 * format specifies little endian as a standard to permit compatibility across
 * different endian-ness machines.
 *
 * \param mem  Pointer to the memory to convert
 * \param size  Amount of memory to prepare (multiple of 2 bytes).
 *
 * \return Returns the address of the value if its format has been converted
 * successfully, or NULL on failure.
 *
 * \see RwMemNative16
 * \see RwMemFloat32ToReal
 * \see RwMemRealToFloat32
 * \see RwStreamReadInt16
 * \see RwStreamWriteInt16
 * \see RwStreamWriteReal
 * \see RwStreamReadReal
 * \see RwStreamWrite
 * \see RwStreamRead
 *
 */
void               *
RwMemLittleEndian16(void *mem,
                    RwUInt32 __RWUNUSEDRELEASELITTLEENDIAN__ size )
{

    RWAPIFUNCTION(RWSTRING("RwMemLittleEndian16"));
    RWASSERT(mem);
    RWASSERT((size & 1) == 0);

#ifdef rwBIGENDIAN
    {
        RwUInt16           *memInt = (RwUInt16 *) mem;

        for (size >>= 1 /* Divide by 2 */ ; size > 0; size--)
        {
            RWLITTLEENDIANSHORT(memInt, memInt);
            memInt++;
        }
    }

#endif /* rwLITTLEENDIAN */

    RWRETURN(mem);
}

/**
 * \ingroup rwmem
 * \ref RwMemNative32 is used to convert the specified chunk of memory from a little
 * endian format to native format after reading the chunk from a binary
 * file.
 * The binary format specifies little endian as a standard to permit
 * compatibility across different endian- ness machines.
 *
 * \param mem  A pointer to the memory.
 * \param size  An \ref RwUInt32 value equal to the size in bytes of the
 * memory (multiple of 4).
 *
 * \return pointer to the converted memory (converted in situ)
 *        if successful or NULL if there is an error.
 *
 * \see RwMemLittleEndian32
 * \see RwMemFloat32ToReal
 * \see RwMemRealToFloat32
 * \see RwStreamReadInt32
 * \see RwStreamWriteInt32
 * \see RwStreamWriteReal
 * \see RwStreamReadReal
 * \see RwStreamWrite
 * \see RwStreamRead
 */
void               *
RwMemNative32(void *mem,
              RwUInt32  __RWUNUSEDRELEASELITTLEENDIAN__ size)
{

    RWAPIFUNCTION(RWSTRING("RwMemNative32"));
    RWASSERT(mem);
    RWASSERT((size & 0x00000003L) == 0);

#ifdef rwBIGENDIAN
    {
        RwUInt32           *memInt = (RwUInt32 *) mem;

        for (size >>= 2 /* Divide by 4 */ ; size > 0; size--)
        {
            RWNATIVELONG(memInt, memInt);
            memInt++;
        }
    }

#endif

    RWRETURN(mem);
}

/**
 * \ingroup rwmem
 * \ref RwMemNative16
 * is used to convert the specified chunk of memory from a little
 * endian format to native format after reading the chunk from a binary
 * file.
 * The binary format specifies little endian as a standard to permit
 * compatibility across different endian- ness machines.
 *
 * \param mem  A pointer to the memory.
 * \param size  An \ref RwUInt32 value equal to the size in bytes of the
 * memory (multiple of 2).
 *
 * \return Returns the address of the value it its format has been converted
 * successfully, or NULL if there is an error.
 *
 * \see RwMemLittleEndian16
 * \see RwMemFloat32ToReal
 * \see RwMemRealToFloat32
 * \see RwStreamReadInt16
 * \see RwStreamWriteInt16
 * \see RwStreamWriteReal
 * \see RwStreamReadReal
 * \see RwStreamWrite
 * \see RwStreamRead
 */
void               *
RwMemNative16(void *mem,
              RwUInt32 __RWUNUSEDRELEASELITTLEENDIAN__ size )
{

    RWAPIFUNCTION(RWSTRING("RwMemNative16"));
    RWASSERT(mem);
    RWASSERT((size & 1L) == 0);

#ifdef rwBIGENDIAN
    {
        RwUInt16           *memInt = (RwUInt16 *) mem;

        for (size >>= 1 /* Divide by 2 */ ; size > 0; size--)
        {
            RWNATIVESHORT(memInt, memInt);
            memInt++;
        }
    }

#endif /* rwBIGENDIAN */

    RWRETURN(mem);
}

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE) || defined(rwBIGENDIAN)) */

#if (defined(RWFLOAT32FROMREAL) || defined(DOXYGEN))
/**
 * \ingroup rwmem
 * \ref RwMemRealToFloat32 converts an array of reals to an array
 * of 32 bit floats.  This allows portability between machines which represent
 * floating point numbers in different ways.
 *
 * \param mem  A pointer to an array of 32-bit floats.
 * \param size  An \ref RwUInt32 value equal to the size in bytes of the array of floats
 *       (should be a multiple of 4).
 *
 * \return Returns the address of the value it its format has been converted
 * successfully, or NULL if there is an error.
 *
 * \see RwMemFloat32ToReal
 * \see RwMemLittleEndian32
 * \see RwMemNative32
 * \see RwStreamReadInt32
 * \see RwStreamWriteInt32
 * \see RwStreamWriteReal
 * \see RwStreamReadReal
 * \see RwStreamWrite
 * \see RwStreamRead
 *
 */
void               *
RwMemRealToFloat32(void *mem, RwUInt32 size)
{
    RwUInt32           *memInt = (RwUInt32 *) mem;

    RWAPIFUNCTION(RWSTRING("RwMemRealToFloat32"));
    RWASSERT(mem);

    for ( size >>= 2 /* Divide by 4 */ ; size > 0; size--)
    {
        RWFLOAT32FROMREAL(memInt, memInt);
        memInt++;
    }

    RWRETURN(mem);
}
#endif /* (defined(RWFLOAT32FROMREAL) || defined(DOXYGEN)) */

#if (defined(RWREALFROMFLOAT32) || defined(DOXYGEN))
/**
 * \ingroup rwmem
 * \ref RwMemFloat32ToReal is used to convert an array of 32-bit floats
 * to an array of reals. This allows portability between machines that
 * represent floating point numbers in different ways.
 *
 * \param mem  Pointer to the memory to convert
 * \param size  Amount of memory to prepare (should be a multiple of 4 bytes).
 *
 * \return Returns the address of the value if its format has been converted
 * successfully, or NULL on failure.
 *
 * \see RwMemRealToFloat32
 * \see RwMemLittleEndian32
 * \see RwMemNative32
 * \see RwStreamRead
 * \see RwStreamReadInt32
 * \see RwStreamReadReal
 * \see RwStreamWrite
 * \see RwStreamWriteInt32
 * \see RwStreamWriteReal
 *
 */
void               *
RwMemFloat32ToReal(void *mem, RwUInt32 size)
{
    RwUInt32           *memInt = (RwUInt32 *) mem;

    RWAPIFUNCTION(RWSTRING("RwMemFloat32ToReal"));
    RWASSERT(mem);

    for (size >>= 2 /* Divide by 4 */ ; size > 0; size--)
    {
        RWREALFROMFLOAT32(memInt, memInt);
        memInt++;
    }

    RWRETURN(mem);
}
#endif /* (defined(RWREALFROMFLOAT32) || defined(DOXYGEN)) */

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Binary file format

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Simplifying functions

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwstream
 * \ref RwStreamWriteReal is used to write the given array of RwReal
 * data values to the specified stream. For compatibility across multiple
 * platforms, the data is automatically converted to little-endian format
 * if necessary. This process is reversed upon reading the stream data so
 * no platform-specific conversion work is needed.
 *
 * \param stream  Pointer to the stream to write to.
 * \param reals  Pointer to array of RwReal data values.
 * \param numBytes  A RwInt32 value equal to the size of the array in bytes. (Use the
 * sizeof operator to ensure portability.)
 *
 * \return Returns pointer to the stream if successful or NULL if there
 * is an error.
 *
 * \see RwStreamReadReal
 * \see RwStreamWriteInt32
 * \see RwStreamReadInt32
 * \see RwStreamWrite
 * \see RwStreamRead
 *
 */
RwStream *
RwStreamWriteReal(RwStream *stream, const RwReal *reals, RwUInt32 numBytes)
{
    RwUInt8 convertBuffer[256];

    RWAPIFUNCTION(RWSTRING("RwStreamWriteReal"));
    RWASSERT(stream);
    RWASSERT(reals);

    while (numBytes)
    {
        RwUInt32 bytesToWrite = (numBytes >= 256) ? 256 : numBytes;

        memcpy(convertBuffer, reals, bytesToWrite);
        (void)RwMemRealToFloat32(convertBuffer, bytesToWrite);
        (void)RwMemLittleEndian32(convertBuffer, bytesToWrite);

        if (!RwStreamWrite(stream, convertBuffer, bytesToWrite))
        {
            RWRETURN((RwStream *)NULL);
        }

        numBytes -= bytesToWrite;
        reals = (const RwReal *) (((const RwUInt8 *) reals) + bytesToWrite);
    }

    RWRETURN(stream);
}

/**
 * \ingroup rwstream
 * \ref RwStreamWriteInt32 is used to write the given array of RwInt32
 * data values to the specified stream. For compatibility across multiple
 * platforms, the data is automatically converted to little-endian format
 * if necessary. This process is reversed upon reading the stream data so
 * no platform-specific conversion is needed.
 *
 * \param stream  Pointer to the stream to write to.
 * \param ints  Pointer to array of RwInt32 data values.
 * \param numBytes  A RwInt32 value equal to the size of the array in bytes. (Use the
 * sizeof operator to ensure portability.)
 *
 * \return Returns pointer to the stream if successful or NULL if there
 * is an error.
 *
 * \see RwStreamReadInt32
 * \see RwStreamWriteReal
 * \see RwStreamReadReal
 * \see RwStreamWrite
 * \see RwStreamRead
 *
 */
RwStream *
RwStreamWriteInt32(RwStream *stream, const RwInt32 *ints, RwUInt32 numBytes)
{
    RWAPIFUNCTION(RWSTRING("RwStreamWriteInt32"));
    RWASSERT(stream);
    RWASSERT(ints);
    RWASSERT((numBytes & 0x00000003) == 0);

#ifdef rwLITTLEENDIAN
    RwStreamWrite(stream, ints, numBytes);
#else /* rwLITTLEENDIAN */
    while (numBytes)
    {
        RwUInt8     convertBuffer[256];
        RwUInt32    bytesToWrite = (numBytes >= 256) ? 256 : numBytes;

        memcpy(convertBuffer, ints, bytesToWrite);
        (void)RwMemLittleEndian32(convertBuffer, bytesToWrite);

        if (!RwStreamWrite(stream, convertBuffer, bytesToWrite))
        {
            RWRETURN(NULL);
        }

        numBytes -= bytesToWrite;
        ints = (const RwInt32 *) (((const RwUInt8 *) ints) + bytesToWrite);
    }
#endif /* rwLITTLEENDIAN */

    RWRETURN(stream);
}

/**
 * \ingroup rwstream
 * \ref RwStreamWriteInt16 is used to write the given array of RwInt16
 * data values to the specified stream. For compatibility across multiple
 * platforms, the data is automatically converted to little-endian format
 * if necessary. This process is reversed upon reading the stream data so
 * no platform-specific conversion is needed.
 *
 * \param stream  Pointer to the stream to write to.
 * \param ints  Pointer to array of RwInt16 data values.
 * \param numBytes  A RwInt32 value equal to the size of the array in bytes. (Use the
 * sizeof operator to ensure portability.)
 *
 * \return Returns pointer to the stream if successful or NULL if there
 * is an error.
 *
 * \see RwStreamReadInt16
 * \see RwStreamWriteReal
 * \see RwStreamReadReal
 * \see RwStreamWrite
 * \see RwStreamRead
 *
 */
RwStream *
RwStreamWriteInt16(RwStream *stream, const RwInt16 *ints, RwUInt32 numBytes)
{

    RWAPIFUNCTION(RWSTRING("RwStreamWriteInt16"));
    RWASSERT(stream);
    RWASSERT(ints);
    RWASSERT((numBytes&1L) == 0);

#ifdef rwLITTLEENDIAN
    RwStreamWrite(stream, ints, numBytes);
#else /* rwLITTLEENDIAN */
    while (numBytes)
    {
        RwUInt8     convertBuffer[256];
        RwUInt32    bytesToWrite = (numBytes >= 256) ? 256 : numBytes;

        memcpy(convertBuffer, ints, bytesToWrite);
        (void)RwMemLittleEndian16(convertBuffer, bytesToWrite);

        if (!RwStreamWrite(stream, convertBuffer, bytesToWrite))
        {
            RWRETURN(NULL);
        }

        numBytes -= bytesToWrite;
        ints = (const RwInt16 *) (((const RwUInt8 *) ints) + bytesToWrite);
    }
#endif /* rwLITTLEENDIAN */

    RWRETURN(stream);
}

/**
 * \ingroup rwstream
 * \ref RwStreamReadInt16 is used to read an array of RwInt16
 * data values from the specified stream. The data is read from the stream
 * in and converted automatically to little-endian or big-endian
 * representation, as appropriate for the platform.
 * (RenderWare Streams use little-endian format internally.)
 *
 * \param stream  Pointer to the stream to read from.
 * \param ints  Pointer to an array of RwInt16 values that will receive the
 * data from the stream.
 * \param numBytes  A RwInt32 value equal to the size of the array in bytes. (Use the
 * sizeof operator to ensure portability.)
 *
 * \return Returns pointer to the stream if successful or NULL if there
 * is an error.
 *
 * \see RwStreamWriteInt16
 * \see RwStreamWriteReal
 * \see RwStreamReadReal
 * \see RwStreamWrite
 * \see RwStreamRead
 *
 */
RwStream *
RwStreamReadInt16(RwStream *stream, RwInt16 *ints, RwUInt32 numBytes)
{
    RWAPIFUNCTION(RWSTRING("RwStreamReadInt16"));
    RWASSERT(stream);
    RWASSERT(ints);
    RWASSERT((numBytes & 1L) == 0);

    if (!RwStreamRead(stream, ints, numBytes))
    {
        RWERROR((E_RW_READ));
        RWRETURN((RwStream *)NULL);
    }

    (void)RwMemNative16(ints, numBytes);

    RWRETURN(stream);
}

/**
 * \ingroup rwstream
 * \ref RwStreamReadReal is used to read an array of RwReal
 * data values from the specified stream. The data is read from the stream
 * and converted automatically to little-endian or big-endian format as
 * appropriate for the platform.
 * (RenderWare Streams use little-endian format internally.)
 *
 * \param stream  Pointer to the stream to read from.
 * \param reals  Pointer to an array of RwReal values that will receive the
 * data from the stream.
 * \param numBytes  A RwInt32 value equal to the size of the array in bytes. (Use the
 * sizeof operator to ensure portability.)
 *
 * \return Returns pointer to the stream if successful or NULL if there
 * is an error.
 *
 * \see RwStreamWriteReal
 * \see RwStreamWriteInt32
 * \see RwStreamReadInt32
 * \see RwStreamWrite
 * \see RwStreamRead
 *
 */
RwStream *
RwStreamReadReal(RwStream *stream, RwReal *reals, RwUInt32 numBytes)
{
    RWAPIFUNCTION(RWSTRING("RwStreamReadReal"));
    RWASSERT(stream);
    RWASSERT(reals);

    /* We can pull this straight into the input buffer - it's not const */
    if (!RwStreamRead(stream, reals, numBytes))
    {
        RWERROR((E_RW_READ));
        RWRETURN((RwStream *)NULL);
    }

    (void)RwMemNative32(reals, numBytes);
    (void)RwMemFloat32ToReal(reals, numBytes);

    RWRETURN(stream);
}

/**
 * \ingroup rwstream
 * \ref RwStreamReadInt32 is used to read an array of RwInt32
 * data values from the specified stream. The data is read from the stream
 * in and converted automatically to little-endian or big-endian
 * representation, as appropriate for the platform.
 * (RenderWare Streams use little-endian format internally.)
 *
 * \param stream  Pointer to the stream to read from.
 * \param ints  Pointer to an array of RwInt32 values that will receive the
 * data from the stream.
 * \param numBytes  A RwInt32 value equal to the size of the array in bytes. (Use the
 * sizeof operator to ensure portability.)
 *
 * \return Returns pointer to the stream if successful or NULL if there
 * is an error.
 *
 * \see RwStreamWriteInt32
 * \see RwStreamWriteReal
 * \see RwStreamReadReal
 * \see RwStreamWrite
 * \see RwStreamRead
 *
 */
RwStream *
RwStreamReadInt32(RwStream *stream, RwInt32 *ints, RwUInt32 numBytes)
{
    RWAPIFUNCTION(RWSTRING("RwStreamReadInt32"));
    RWASSERT(stream);
    RWASSERT(ints);

    /* We can pull this straight into the input buffer - it's not const */
    if (!RwStreamRead(stream, ints, numBytes))
    {
        RWERROR((E_RW_READ));
        RWRETURN((RwStream *)NULL);
    }

    (void)RwMemNative32(ints, numBytes);

    RWRETURN(stream);
}

/**
 * \ingroup rwstream
 * \ref RwStreamReadChunkHeaderInfo reads the Chunk Header information and
 * stores it in the structure provided.
 *
 * \param stream  Pointer to stream to extract information from
 * \param chunkHeaderInfo  Pointer to storage for chunkHeaderInfo datatype.
 *
 * \return Pointer to the stream if successful; NULL if an error occurred
 *
 * \see RwStreamOpen
 * \see RwStreamRead
 */
RwStream           *
RwStreamReadChunkHeaderInfo(RwStream * stream,
                            RwChunkHeaderInfo * chunkHeaderInfo)
{
    RwUInt32    readType, readLength, readVersion, readBuildNum;

    RWAPIFUNCTION(RWSTRING("RwStreamReadChunkHeaderInfo"));
    RWASSERT(stream);
    RWASSERT(chunkHeaderInfo);

    if (!_rwStreamReadChunkHeader(stream, &readType, &readLength, &readVersion,
                &readBuildNum))
    {
        RWRETURN((RwStream *)NULL);
    }

    chunkHeaderInfo->type = readType;
    chunkHeaderInfo->length = readLength;
    chunkHeaderInfo->version = readVersion;
    chunkHeaderInfo->buildNum = readBuildNum;
    chunkHeaderInfo->isComplex = ChunkIsComplex(chunkHeaderInfo);

    RWRETURN(stream);
}

#if (defined(DOXYGEN))
/*
 * This function is now a #define that uses
 * _rwStreamWriteVersionedChunkHeader()
 */

/**
 * \ingroup rwstream
 * \ref RwStreamWriteChunkHeader is used to write a chunk header to the
 * specified stream. This function would typically be used by an application
 * wishing to write application specific data to a file.
 *
 * The chunk identifier is a 32 bit value which identifies the type of data
 * to be written in a universally unique way. The three highest order bytes
 * (24 most significant bits) should be the developer's ID.  The developer's ID
 * can be any three digit ID that does not clash with RwPluginVendor in rwplcore.h. 
 * The low-order byte (8 least significant bits) is used for each custom 
 * chunk type, and is allocated by the individual developer. 
 *
 * RwStreamWriteChunkHeader is currently implemented as a macro.
 *
 * \param stream  Pointer to the stream to write chunk header to.
 * \param type  A RwInt32 value equal to the chunk's identifier cast to an RwInt32 type.
 * \param size  A RwInt32 value equal to the size of the chunk data.
 *
 * \return Returns pointer to the stream if successful or NULL if there
 * is an error.
 *
 * \see RwStreamFindChunk
 * \see RwStreamWriteReal
 * \see RwStreamWriteInt32
 * \see RwStreamReadInt32
 * \see RwStreamWrite
 * \see RwStreamRead
 * \see RwCorePluginID
 *
 */
RwStream           *
RwStreamWriteChunkHeader(RwStream * stream, RwInt32 type, RwInt32 size)
{
    RwStream           *result = NULL;

    RWAPIFUNCTION(RWSTRING("RwStreamWriteChunkHeader"));

    RWRETURN(result);
}
#endif /* (defined(DOXYGEN)) */

