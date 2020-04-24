/*****************************************************************************
*
* File :     StreamHandler.cpp
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
//#define RWS_CALLSTACKENABLE
//#define RWS_TRACEENABLE
#include "precomp_ntlcore.h"
#include <rwcore.h>
#include <rpworld.h>
#include "memoryhandler.h"


//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include <rtfsyst.h>

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "streamhandler.h"
#include "strfunc.h"
#include "debugmacros.h"
#include "NtlDebug.h"

#ifdef _DEBUG
namespace
{
   typedef struct IdToTextTable_tag
   {
      RwInt32 Id;
      RwChar *pName;
   } IdToTextTable;

   IdToTextTable strfunc_func_IdToTextTable[]=
   {
      {MAKECHUNKID (rwVENDORID_CRITERIONRM, RWS::strfunc_VersionNumber), "strfunc_VersionNumber"},
      {MAKECHUNKID (rwVENDORID_CRITERIONRM, RWS::strfunc_Reset), "strfunc_Reset"},
      {MAKECHUNKID (rwVENDORID_CRITERIONRM, RWS::strfunc_Reserved1), "strfunc_Reserved1"},
      {MAKECHUNKID (rwVENDORID_CRITERIONRM, RWS::strfunc_Reserved2), "strfunc_Reserved2"},
      {MAKECHUNKID (rwVENDORID_CRITERIONRM, RWS::strfunc_SetDirectorsCameraMatrix), "strfunc_SetDirectorsCameraMatrix"},
      {MAKECHUNKID (rwVENDORID_CRITERIONRM, RWS::strfunc_CreateEntity), "strfunc_CreateEntity"},
      {MAKECHUNKID (rwVENDORID_CRITERIONRM, RWS::strfunc_UpdateEntityAttributes), "strfunc_UpdateEntityAttributes"},
      {MAKECHUNKID (rwVENDORID_CRITERIONRM, RWS::strfunc_SetFrozenMode), "strfunc_SetFrozenMode"},
      {MAKECHUNKID (rwVENDORID_CRITERIONRM, RWS::strfunc_SetRunningMode), "strfunc_SetRunningMode"},
      {MAKECHUNKID (rwVENDORID_CRITERIONRM, RWS::strfunc_EnableDirectorsCamera), "strfunc_EnableDirectorsCamera"},
      {MAKECHUNKID (rwVENDORID_CRITERIONRM, RWS::strfunc_DisableDirectorsCamera), "strfunc_DisableDirectorsCamera"},
      {MAKECHUNKID (rwVENDORID_CRITERIONRM, RWS::strfunc_TextComment), "strfunc_TextComment"},
      {MAKECHUNKID (rwVENDORID_CRITERIONRM, RWS::strfunc_StartSystem), "strfunc_StartSystem"},
      {MAKECHUNKID (rwVENDORID_CRITERIONRM, RWS::strfunc_StopSystem), "strfunc_StopSystem"},
      {MAKECHUNKID (rwVENDORID_CRITERIONRM, RWS::strfunc_DeleteEntity), "strfunc_DeleteEntity"},
      {MAKECHUNKID (rwVENDORID_CRITERIONRM, RWS::strfunc_DeleteAllEntities), "strfunc_DeleteAllEntities"},
      {MAKECHUNKID (rwVENDORID_CRITERIONRM, RWS::strfunc_UnLoadAsset), "strfunc_UnLoadAsset"},
      {MAKECHUNKID (rwVENDORID_CRITERIONRM, RWS::strfunc_Shutdown), "strfunc_Shutdown"},
      {MAKECHUNKID (rwVENDORID_CRITERIONRM, RWS::strfunc_CloseConnection), "strfunc_CloseConnection"},
      {MAKECHUNKID (rwVENDORID_CRITERIONRM, RWS::strfunc_SendTestEvent), "strfunc_SendTestEvent"},
      {MAKECHUNKID (rwVENDORID_CRITERIONRM, RWS::strfunc_Reserved3), "strfunc_Reserved3"},
      {MAKECHUNKID (rwVENDORID_CRITERIONRM, RWS::strfunc_Reserved3b), "strfunc_Reserved3b"},
      {MAKECHUNKID (rwVENDORID_CRITERIONRM, RWS::strfunc_LoadAsset), "strfunc_LoadAsset"},
      {MAKECHUNKID (rwVENDORID_CRITERIONRM, RWS::strfunc_LoadEmbeddedAsset), "strfunc_LoadEmbeddedAsset"},
      {MAKECHUNKID (rwVENDORID_CRITERIONRM, RWS::strfunc_Reserved4), "strfunc_Reserved4"},
      {MAKECHUNKID (rwVENDORID_CRITERIONRM, RWS::strfunc_GetEntityMatrix), "strfunc_GetEntityMatrix"},
      {MAKECHUNKID (rwVENDORID_CRITERIONRM, RWS::strfunc_CustomData), "strfunc_CustomData"},
      {MAKECHUNKID (rwVENDORID_CRITERIONRM, RWS::strfunc_FunctionProfiler), "strfunc_FunctionProfiler"},
      {MAKECHUNKID (rwVENDORID_CRITERIONRM, RWS::strfunc_ResetEntity), "strfunc_ResetEntity"},
      {MAKECHUNKID (rwVENDORID_CRITERIONRM, RWS::strFunc_PlacementNew), "strFunc_PlacementNew"},
      {MAKECHUNKID (rwVENDORID_CRITERIONRM, RWS::strfunc_Initialize), "strfunc_Initialize"},
      {MAKECHUNKID (rwVENDORID_CRITERIONRM, RWS::strfunc_UpdateAsset), "strfunc_UpdateAsset"},
      {MAKECHUNKID (rwVENDORID_CRITERIONRM, RWS::strfunc_DynamicSequence), "strfunc_DynamicSequence"}
   } ;

   /*
   *  Helper function for converting a chunk Id into a meaningful name.
   */
   const RwChar *IdToText(RwInt32 Id)
   {
      RWS_FUNCTION("NULL::IdToText");

      for(RwUInt32 i = 0; i < sizeof(strfunc_func_IdToTextTable) / sizeof(IdToTextTable); i++)
      {
         if (Id == strfunc_func_IdToTextTable[i].Id)
         {
            RWS_RETURN(strfunc_func_IdToTextTable[i].pName);
         }
      }
      RWS_RETURN("Unknown Id");
   }
}
#endif

namespace RWS
{
   /**
   *
   *  Used to return the single instance of CStreamHandler.
   *
   */
   CStreamHandler *CStreamHandler::Instance(void)
   {
      RWS_FUNCTION("RWS::CStreamHandler::Instance");
      
      static CStreamHandler _CStreamHandler;
      
      RWS_RETURN(&_CStreamHandler);
   }
   
   /**
   *
   *  Register a function responsible for handling a specific chunk id. Chunk Id's are used to identify
   *  regions of a RenderWare stream.
   *
   *  \param ChunkType The identity of the chunk.
   *  \param ChunkHandler The chunk handler function.
   *
   */
   void CStreamHandler::RegisterChunkHandler(RwUInt32 ChunkType, StreamChunkHandler ChunkHandler)
   {
      RWS_FUNCTION("RWS::CStreamHandler::RegisterChunkHandler");
      
      CStreamHandler *pCStreamHandler = Instance();
    
      // Check if we already have a chunk handler for this chunk type registered
      //
      const StreamChunkHandlerMap::iterator it = pCStreamHandler->m_StreamChunkHandlerMap.find(ChunkType);
      
      if (it != pCStreamHandler->m_StreamChunkHandlerMap.end())
      {
         RWS_ASSERTFAIL("Unable to register chunk handler, a chunk handler for the specified chunk id has already"
            " been registered. Chunk type = " << ChunkType);
      }
      else
      {
         pCStreamHandler->m_StreamChunkHandlerMap.insert(StreamChunkHandlerMap::value_type(ChunkType, ChunkHandler));
      }
      
      RWS_RETURNVOID();
   }
   
   /**
   *
   *  Unregister a chunk handler function \see RegisterChunkHandler
   *
   */
   void CStreamHandler::UnRegisterChunkHandler(RwUInt32 ChunkType)
   {
      RWS_FUNCTION("RWS::CStreamHandler::UnRegisterChunkHandler");
      
      CStreamHandler *pCStreamHandler = Instance();
      
      pCStreamHandler->m_StreamChunkHandlerMap.erase(ChunkType);
      
      RWS_RETURNVOID();
   }
   
   /**
   *
   *  \ref CStreamHandler::RwStreamEOF, extends RenderWare Streams to provide an EOF test.
   *
   *  \param _pStream Pointer to a RwStream object.
   *
   *  \return Returns true if stream is at the end, otherwise false.
   *
   */
   bool CStreamHandler::RwStreamEOF(RwStream *_pStream)
   {
      RWS_FUNCTION("RWS::CStreamHandler::RwStreamEOF");
      
      RWS_PRE(_pStream);
      
      switch (_pStream->type)
      {
      case rwSTREAMFILENAME:
      case rwSTREAMFILE:
         {
            void *fp = _pStream->Type.file.fpFile;
            if (RwFeof(fp)) RWS_RETURN(true);
         }
         break;
      case rwSTREAMMEMORY:
         {
            RwStreamMemory     *smpMem = &_pStream->Type.memory;
            
            if (smpMem->nSize == smpMem->position)
            {
               RWS_RETURN(true);
            }
         }
         break;
      case rwSTREAMCUSTOM:
         {
            // The custom _pStream is the network _pStream, because there is no overload for eof
            // we have modified the network _pStream so that reading into a buffer of 0 or a size of
            // 0 bytes returns the length of the _pStream remaining.
            //
            if (_pStream->Type.custom.sfnread(_pStream->Type.custom.data, 0, 0) == 0)
            {
               RWS_RETURN(true);
            }
         }
         break;
      default:
         {
            RWS_ASSERTFAIL("Unknown Stream Type.");
         }
      }
      
      RWS_RETURN(false);
   }

   /**
   *
   *  ref CStreamHandler::ProcessStream, Entry point for receiving command packets,
   *  determine type of command and calls respective function.
   *
   *  \param pStream A pointer to a RenderWare stream.
   *
   */
   void CStreamHandler::ProcessStream(RwStream* pStream)
   {
      RWS_FUNCTION("RWS::CStreamHandler::ProcessStream");
      
      RWS_PRE(pStream);
      
      // For each chunk
      //
      while (!RwStreamEOF(pStream))
      {
         RwStreamReadChunkHeaderInfo(pStream, &Instance()->m_RwChunkHeaderInfo);
         
         // Check if we have a chunk handler registered
         //
         const StreamChunkHandlerMap::iterator it =
            Instance()->m_StreamChunkHandlerMap.find(Instance()->m_RwChunkHeaderInfo.type);
         
         // Yes then call the handler
         //
         if (it != Instance()->m_StreamChunkHandlerMap.end())
         {
            StreamChunkHandler pFunc = it->second;
            
            // Process the chunk
            //
            pFunc(Instance()->m_RwChunkHeaderInfo, pStream);
         }
         else
         {
            // No then don't process this chunk, skip it.
            //
            RwStreamSkip(pStream, Instance()->m_RwChunkHeaderInfo.length);
         }
      }
      RWS_RETURNVOID();
   }

   namespace
   {
      /**
      *
      *  Validate RenderWare Studio Stream version number.
      *
      */
      void VersionNumber (RwChunkHeaderInfo &rChunkHeader, RwStream *pStream) 
      {
         RWS_FUNCTION("RWS::CDefaultStreamHandler::VersionNumber");
      
		 // 형석
         RwChar *buffer = static_cast<char*>(NTL_NEW char[rChunkHeader.length]);
      
         RwStreamRead(pStream, buffer, rChunkHeader.length);
      
         RWS_ASSERT((*reinterpret_cast<RwUInt32*>(buffer)) == 0x0100, "Incompatible RenderWare Studio Stream, version number ( "
               << (*reinterpret_cast<RwUInt32*>(buffer)) << " )");

		 // 형석
		 RWS_ARRAY_DELETE(buffer);
   
         RWS_RETURNVOID();
      }
   }

  /**
   *
   *  Register the chunk handlers implemented by CStreamHandler:, \see strfunc_func \see CStreamHandler.
   *
   *  CStreamHandler: implements the following chunk handlers, \ref strfunc_VersionNumber.
   *
   */
   void CStreamHandler::RegisterStreamChunkHandlers(void)
   {
      CStreamHandler::RegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM,
         strfunc_VersionNumber), VersionNumber);
   }
   
   /**
   *
   *  Unregister the chunk handlers implemented by CResourceManager, \see strfunc_func \see CStreamHandler.
   *
   */
   void CStreamHandler::UnRegisterStreamChunkHandlers(void)
   {
      CStreamHandler::UnRegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_VersionNumber));
   }
}

// rwVENDORID_CRITERIONRM
