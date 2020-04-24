 /***************************************************************************
 *                                                                         *
 * Module  : babinary.h                                                    *
 *                                                                         *
 * Purpose : Binary file format handling                                   *
 *                                                                         *
 **************************************************************************/

#ifndef RWBINARY_H
#define RWBINARY_H

/****************************************************************************
 Includes
 */

#include "batypes.h"
#include "bastream.h"

/* RWPUBLIC */
/****************************************************************************
 Defines
 */
#ifndef rwCHUNKHEADERSIZE
#define rwCHUNKHEADERSIZE (sizeof(RwUInt32)*3)
#endif /* rwCHUNKHEADERSIZE */

#if !(defined(RWDEBUG) || defined(RWSUPPRESSINLINE) || defined(rwBIGENDIAN))
/* Binary Portability Functions/Macros */
#define RwMemLittleEndian16(_m, _s) (_m)
#define RwMemLittleEndian32(_m, _s) (_m)
#define RwMemNative16(_m, _s)       (_m)
#define RwMemNative32(_m, _s)       (_m)
#endif /* !(defined(RWDEBUG) || defined(RWSUPPRESSINLINE) || defined(rwBIGENDIAN)) */

#if (!defined(RWFLOAT32FROMREAL))
#define RwMemRealToFloat32(_m, _s)  (_m)
#endif /* (!defined(RWFLOAT32FROMREAL)) */

#if (!defined(RWREALFROMFLOAT32))
#define RwMemFloat32ToReal(_m, _s)  (_m)
#endif /* (!defined(RWREALFROMFLOAT32)) */

/* Compatibility macro */
#define RwStreamWriteInt(_stream, _ints, _numBytes) \
        RwStreamWriteInt32(_stream, _ints, _numBytes)

#define RwStreamReadInt(_stream, _ints, _numBytes) \
        RwStreamReadInt32(_stream, _ints, _numBytes)

#define RwMemLittleEndian(_mem, _size) \
        RwMemLittleEndian32(_mem, _size)

#define RwMemNative(_mem, _size) \
        RwMemNative32(_mem, _size)

/****************************************************************************
 Global Types
 */

typedef struct RwChunkHeaderInfo RwChunkHeaderInfo;
#ifndef RWADOXYGENEXTERNAL
/**
 * \ingroup rwstream
 * \struct RwChunkHeaderInfo
 * Holds data for a chunk header read from a
 * stream with \ref RwStreamReadChunkHeaderInfo. */
#endif /* RWADOXYGENEXTERNAL */
struct RwChunkHeaderInfo
{
    RwUInt32 type;      /**< chunk ID - see \ref RwStreamFindChunk */
    RwUInt32 length;    /**< length of the chunk data in bytes */
    RwUInt32 version;   /**< version of the chunk data.
                         *   See \ref RwEngineGetVersion. */
    RwUInt32 buildNum;  /**< build number of the RenderWare libraries
                         *   previously used to stream out the data */
    RwBool isComplex;   /**< Internal Use */
/* RWPUBLICEND */
        /* WAS: "TRUE if the chunk contains sub-chunks."
         * NOTE: The "isComplex" field is only ever used by
         * the stream viewer app. Do NOT add Doxygen comments for it.
         * It should not be in RenderWare at all and ought to be deleted.
        */
/* RWPUBLIC */
};

/* RWPUBLICEND */

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

/* Chunk header stuff */
extern RwBool RwStreamFindChunk(RwStream *stream, RwUInt32 type,
                                RwUInt32 *lengthOut, RwUInt32 *versionOut);

#define RwStreamWriteChunkHeader(stream, type, size) \
    _rwStreamWriteVersionedChunkHeader(         \
        stream, type, size, rwLIBRARYCURRENTVERSION, RWBUILDNUMBER)

extern RwStream *_rwStreamWriteVersionedChunkHeader(RwStream *stream,
                                                   RwInt32 type,
                                                   RwInt32 size,
                                                   RwUInt32 version,
                                                   RwUInt32 buildNum);

extern RwStream *RwStreamWriteReal(RwStream *stream, const RwReal *reals,
                                   RwUInt32 numBytes);
extern RwStream *RwStreamWriteInt32(RwStream *stream, const RwInt32 *ints,
                                    RwUInt32 numBytes);
extern RwStream *RwStreamWriteInt16(RwStream *stream, const RwInt16 *ints,
                                    RwUInt32 numBytes);

extern RwStream *RwStreamReadReal(RwStream *stream, RwReal *reals,
                                  RwUInt32 numBytes);
extern RwStream *RwStreamReadInt32(RwStream *stream, RwInt32 *ints,
                                   RwUInt32 numBytes);
extern RwStream *RwStreamReadInt16(RwStream *stream, RwInt16 *ints,
                                   RwUInt32 numBytes);

/* Binary Portability Functions */
#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE) || defined(rwBIGENDIAN))
extern void *RwMemLittleEndian16(void *mem, RwUInt32 size);
extern void *RwMemLittleEndian32(void *mem, RwUInt32 size);
extern void *RwMemNative16(void *mem, RwUInt32 size);
extern void *RwMemNative32(void *mem, RwUInt32 size);
#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE) || defined(rwBIGENDIAN)) */

#if (defined(RWFLOAT32FROMREAL))
extern void *RwMemRealToFloat32(void *mem, RwUInt32 size);
#endif /* (defined(RWFLOAT32FROMREAL)) */

#if (defined(RWREALFROMFLOAT32))
extern void *RwMemFloat32ToReal(void *mem, RwUInt32 size);
#else
#endif /* (defined(RWREALFROMFLOAT32)) */

extern RwStream *
RwStreamReadChunkHeaderInfo(RwStream *stream, RwChunkHeaderInfo *chunkHeaderInfo);

/* RWPUBLICEND */

extern RwBool _rwStreamReadChunkHeader(RwStream *stream, RwUInt32 *type,
                                       RwUInt32 *length, RwUInt32 *versionOut,
                                       RwUInt32 *buildNumOut);

/* RWPUBLIC */
#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

/* RWPUBLICEND */

#endif /* RWBINARY_H */

