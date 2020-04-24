/*****************************************************************************
*
* File :     eventhandlerstrfunc.cpp
*
* Abstract : Implements chunk handlers provided by this module.
*
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
#include "precomp_ntlcore.h"
#include <rwcore.h>
#include <rpworld.h>
#include "memoryhandler.h"
#include "debugmacros.h"
#include "streamhandler.h"
#include "strfunc.h"
#include "ceventhandler.h"
#include "NtlDebug.h"

namespace RWS
{
   namespace
   {
     /*
      *
      *  Process a strfunc_SendTestEvent chunk.
      *
      *  \param rChunkHeader A reference to a RenderWare Gfx RwChunkHeaderInfo object.
      *
      *  \param pStream A pointer to the RenderWare Gfx Stream object.
      *
      */
      void SendTestEvent (RwChunkHeaderInfo &rChunkHeader, RwStream *pStream)
      {
         RWS_FUNCTION("RWS::NULL::SendTestEvent");
      
		 // 형석
         // RwChar *buffer = static_cast<char*>(::RWS_OP_NEW(rChunkHeader.length) );
		 RwChar *buffer = static_cast<char*>(NTL_NEW char[rChunkHeader.length]);
      
         RwStreamRead(pStream, buffer, rChunkHeader.length);
      
         // Create event/message to fire into system.
         //
         CEventId testevent;
      
         // All event/messages should be capable of taking NULL as a parameter, so that 
         // they can always be used as an event rather than to transfer data.
         //
         CEventHandler::RegisterMsg( testevent, reinterpret_cast<RwChar*>(buffer), 0);   
      
         SendMsg(testevent);
      
         CEventHandler::UnRegisterMsg(testevent); 
      
		 RWS_ARRAY_DELETE(buffer);
		 // 형석
         //operator delete (buffer);
      
         RWS_RETURNVOID();
      }
   }

   /**
   *
   *  Register the chunk handlers implemented by CEventHandler, \see strfunc_func \see CStreamHandler.
   *
   *  CEventHandler implements the following chunk handler \ref strfunc_SendTestEvent.
   *
   */
   void CEventHandler::RegisterStreamChunkHandlers(void)
   {
      RWS_FUNCTION("RWS::CEventHandler::RegisterStreamChunkHandlers");

      CStreamHandler::RegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM,
         strfunc_SendTestEvent), SendTestEvent);

      RWS_RETURNVOID();
   }

   /**
   *
   *  Unregister the chunk handlers implemented by CEventHandler, \see strfunc_func \see CStreamHandler.
   *
   */
   void CEventHandler::UnRegisterStreamChunkHandlers(void)
   {
      RWS_FUNCTION("RWS::CEventHandler::UnRegisterStreamChunkHandlers");

      CStreamHandler::UnRegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_SendTestEvent));

      RWS_RETURNVOID();
   }
}
