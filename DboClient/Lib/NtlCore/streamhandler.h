/*****************************************************************************
*
* File :     StreamHandler.h
*
*****************************************************************************
*
* This file is a product of Criterion Software Ltd.
*
* This file is provided as is with no warranties of any kind and is
* provided without any obligation on Criterion Software Ltd. or
* Canon Inc. to assist in its use or modification.
*
* Criterion Software Ltd. will not, under any
* circumstances, be liable for any lost revenue or other damages arising
* from the use of this file.
*
* Copyright (c) 2000 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/

#ifndef _STREAMHANDLER_H_
#define _STREAMHANDLER_H_

#ifdef _MSC_VER
#pragma warning (disable: 4786)
#endif

#include <rwcore.h>
#include <map>

namespace RWS
{
   typedef void (*StreamChunkHandler)(RwChunkHeaderInfo &rChunkHeader, RwStream *pStream);
   
   /**
   *
   *  \ingroup Stream
   *
   *  \page StreamOverview Overview
   *
   *  The RenderWare Studio Workspace sends commands and data to the Framework
   *  via a RwStream. A stream handler reads the chunk id of the next chunk from the stream and 
   *  acts on them accordingly by calling a registered chunk handler for that chunk id. If no 
   *  chunk handler has been registered then the chunk is skipped.
   *
   */


   /**
   *
   *  \ingroup Stream
   *
   *  This class is used to process RenderWare streams, by maintaining a list of chunk handlers each
   *  of which is used to process a particular chunk type.
   *
   */
   class CStreamHandler
   {
      public:

         static void ProcessStream(RwStream *pStream);

         static void RegisterChunkHandler(RwUInt32 ChunkType, StreamChunkHandler ChunkHandler);

         static void UnRegisterChunkHandler(RwUInt32 ChunkType);

         static CStreamHandler *Instance(void);

         static void RegisterStreamChunkHandlers(void);

         static void UnRegisterStreamChunkHandlers(void);

      protected:      
         
         static bool RwStreamEOF(RwStream *pStream);

      private:

         typedef std::map< RwUInt32, StreamChunkHandler > StreamChunkHandlerMap;

         StreamChunkHandlerMap m_StreamChunkHandlerMap;

         RwChunkHeaderInfo m_RwChunkHeaderInfo;     /**< Chunk header of currently processed chunk */
   };
}

#endif