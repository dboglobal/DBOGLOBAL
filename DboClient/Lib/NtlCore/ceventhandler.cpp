/*****************************************************************************
*
* File :     CEventHandler.cpp
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


//////////////////////////////////////////////////////////////////
//
// System Includes
//
#include <string.h>

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "ceventhandler.h"
#include "debugmacros.h"
#include "idfromstring.h"
#include "NtlDebug.h"

namespace 
{
   /*
   *
   *   Compares two strings.
   *
   *   param uiId1 Unique Id generated from ps1
   *   param uiId2 Unique Id generated from ps2
   *   param ps1 Pointer to a string
   *   param ps2 Pointer to a string
   *
   *   return TRUE if strings are identical both in length and content otherwise
   *   returns FALSE.
   *
   */
   static RwBool StrIdentical(const RwUInt32 uiId1, RwUInt32 uiId2, const char *ps1,  const char *ps2)
   {
      RWS_FUNCTION("NULL::StrIdentical");

      // Optimization Check Almost Unique Id's
      //
      if (uiId1 != uiId2) RWS_RETURN(FALSE); 

      while (*ps1 != 0 && *ps2 != 0)
      {
         if (*ps1 != *ps2) RWS_RETURN(FALSE);
      
         ps1++;
         ps2++;
      }
   
      if (*ps1 != *ps2) RWS_RETURN(FALSE); // Check 0 terminate
   
      RWS_RETURN(TRUE);
   }
}

namespace RWS
{
   CEventId iMsgDeleteEventHandler; // Used by event system to delete all linked event handlers.
   CEventId iMsgDeleteEntity;       // CEventHandlers registered for deletion \see Delete are
                                    // linked to this event.
   CEventId iMsgOnDeleteEntity;     // As CEventHandlers are being deleted this event is sent

#if defined RWS_EVENTVISUALIZATION
   CEventId iMsgEventReceived;      // Event has been received
   CEventId iMsgEventSend;          // Event about to be sent
   CEventId iMsgEventSent;          // Event has been sent
   CEventId iMsgEventUnRegistered;  // Event about to be unregistered.
#endif

   // Head of List, pointer to event/message headers
   // 
   //
   static CRegisteredMsgs pRMsgs;


   /**
   *
   *  Constructs a CEventId object.
   *
   */
   CEventId::CEventId(void) : pEventId(0), m_linked_count(0), m_registered_count(0)
   {
      RWS_FUNCTION("RWS::CEventId::CEventId");

      RWS_RETURNVOID();
   };


   /**
   *
   *  Destructor for CEventId object. 
   *
   *  \remarks Performs various error checks before freeing the CEventId object.
   *
   *  \li If the CEventId is still linked \ref CEventHandler::LinkMsg then an assert is generated.
   *  \li Similarly if the CEventId is still registered \ref CEventHandler::RegisterMsg an assert
   *      is generated.
   *
   *  \note If NDEBUG is defined, this function is removed.
   *
   */
#ifdef NDEBUG
   CEventId::~CEventId(void) {}
#else
   CEventId::~CEventId(void)
   {
      RWS_FUNCTION("RWS::CEventId::~CEventId");

      RWS_ASSERT( (m_linked_count == 0),
         "CEventId::~CEventId " << RWS_VALIDSTRING(pEventId->p_msgname) << " not unlinked" );

      RWS_ASSERT( (m_registered_count == 0),
         "CEventId::~CEventId " << RWS_VALIDSTRING(pEventId->p_msgname) << " not unregistered" );

      RWS_ASSERT( (pEventId == 0),
         "CEventId::~CEventId " << RWS_VALIDSTRING(pEventId->p_msgname)
         << " not unregistered (This should never happen)." );

      RWS_RETURNVOID();
   }
#endif


   /**
   *
   *  Increments the reference counter \ref m_registered_count of the number
   *  of times a CEventId has been registered.
   *
   *  \remarks Used internally by \ref CEventHandler::RegisterMsg..
   *
   */
   void CEventId::Inc_registered_count(void)
   {
      RWS_FUNCTION("RWS::CEventId::Inc_registered_count");

      pEventId->Inc_registered_count();

      m_registered_count++;
  
      RWS_ASSERT((pEventId != 0), "CEventId::Inc_registered_count pEventId==0, this should never happen.");

      RWS_RETURNVOID();
   }


   /**
   *
   *  Decrements the reference counter \ref m_registered_count of the number
   *  of times a CEventId has been registered.
   *
   *  \remarks Used internally by \ref CEventHandler::UnRegisterMsg.
   *
   */
   void CEventId::Dec_registered_count(void)
   {
      RWS_FUNCTION("RWS::CEventId::Dec_registered_count");

      pEventId->Dec_registered_count();

      m_registered_count--;

      if (m_registered_count == 0)
      {
         pEventId = 0;
      }

      RWS_ASSERT(m_registered_count >= 0,
         "CEventId::Dec_registered_count CEventId"
         " unregistered more times than it has been registered.");

      RWS_RETURNVOID();
   }

   /**
   *
   *  Increment the reference counter \ref m_linked_count number of times a CEventId
   *  has been linked.
   *
   *  \remarks Used internally by \ref CEventHandler::LinkMsg.
   *
   */
   void CEventId::Inc_linked_count(void)
   {
      RWS_FUNCTION("RWS::CEventId::Inc_linked_count");

      m_linked_count++;

      RWS_RETURNVOID();
   }

   /**
   *
   *  Decrement the reference counter \ref m_linked_count number of times a CEventId
   *  has been linked.
   *
   *  \remarks Used internally by \ref CEventHandler::UnLinkMsg.
   *
   */

   void CEventId::Dec_linked_count(void)
   {
      RWS_FUNCTION("RWS::CEventId::Dec_linked_count");

      m_linked_count--;

      RWS_ASSERT(m_linked_count >= 0, "CEventId::Dec_linked_count m_linked_count < 0 for ( "
            << GetName() << ")");

      RWS_RETURNVOID();
   }

   /**
   *
   *  Assignment operator, copies contents of CEventId specified by rhs.
   *
   *  \param rhs A reference to a CEventId.
   *
   *  \code
         
         CEventId a,b,c;
         
         a = b = RegisterMsg(c,...);

         UnRegisterMsg(a);
         UnRegisterMsg(b);
         UnRegisterMsg(c);

      \endcode
   *
   *  Note, the assignment operator also registers the event, i.e. the above
   *  example is the same as the code below.
   *
   *  \code
      
        CEventId a,b,c;

        RegisterMsg(a,...);
        RegisterMsg(b,...);
        RegisterMsg(c,...);

        UnRegisterMsg(a);
        UnRegisterMsg(b);
        UnRegisterMsg(c);

      \endcode
   *
   */

   CEventId &CEventId::operator = (const CEventId &rhs)
   {
      RWS_FUNCTION("RWS::CEventId::operator=");

      *this = rhs.pEventId;

      if (*this != ERROR_NOT_A_VALID_MESSAGE_ID)
      {
         Inc_registered_count(); // Increment the reference counts.
      }

      RWS_RETURN(*this);
   }

  /**
   *
   *  Assignment operator, Copies a CEventId from a pointer to a CRegisteredMsgs,
   *  if the left hand operator hasn't been completely unlinked and unregistered 
   *  then this operation will generate an assert.
   *
   *  \note CEventId is a smart pointer to a CRegisteredMsgs, it is therefore valid 
   *  to assign a CRegisteredMsgs* to a CEventId.
   *
   *  \param rhs A pointer to a CRegisteredMsgs.
   *
   *  \code

         CEventId a,b,c

         CRegisteredMsgs *pCRegisteredMsgs;

         a = b = c = pCRegisteredMsgs;

       \endcode
   *
   */

   CEventId &CEventId::operator = (const CRegisteredMsgs *rhs)
   {
      RWS_FUNCTION("RWS::CEventId::operator=");

      if (pEventId != rhs)
      {
         RWS_ASSERT((m_linked_count == 0),
            "CEventId::CEventId &operator= m_linked_count != 0,"
            "\nAssigning an CEventId to another where the first hasn't been completely "
            "unlinked would result in memory fragmentation. Link count "
            <<Get_linked_count()<<" Event Name "<<pEventId->p_format);

         RWS_ASSERT((m_registered_count == 0),
            "CEventId::CEventId &operator= m_registered_count != 0,"
            "\nAssigning an CEventId to another where the first hasn't been completely "
            "unregistered would result in memory fragmentation. Registered count "
            <<Get_registered_count()<<" Event Name "<<pEventId->p_format);
         
         // I want this operator to show that it doesn't change the contents of rhs, but
         // the CEventId can change the contents of rhs through other operations, so I'm
         // using a const cast.
         //
         pEventId = const_cast<CRegisteredMsgs*>(rhs);
      }

      RWS_RETURN(*this);
   }

  /**
   *
   *  Open the event handler system, this function should be called before using the event
   *  handler system.
   *
   *  \remarks During the initialization of the event handler system several useful system
   *  messages are registered by calling \ref RegisterMsg, namely \ref iMsgDeleteEventHandler,
   *  \ref iMsgDeleteEntity and \ref iMsgOnDeleteEntity
   *
   *  \ref iMsgDeleteEventHandler A CEventHandler can link to this message by calling
   *       CEventHandler::RegisterForAutoDelete(). At set points in a game, all registered
   *       event handlers can be destroyed by calling
   *       CEventHandler::DeleteEventHandlers (iMsgDeleteEventHandler).
   * 
   *  \ref iMsgDeleteEntity when the method CEventHandler::Delete is called the event handler
   *  is linked to the iMsgDeleteEntity event. At the end of each logic update 
   *  DeleteEventHandlers(iMsgDeleteEntity) is called which safely deletes each of these event handlers.
   *
   *  \ref iMsgOnDeleteEntity when an event handler is deleted, the destructor 
   *  CEventHandler::~CEventHandler generates a iMsgOnDeleteEntity event, CEventHandlers can link
   *  to this event if they need to update when an event handler is deleted.
   *
   *  \see RegisterMsg, DeleteEventHandlers, Delete, LinkMsg, UnLinkMsg, iMsgDeleteEventHandler,
   *  iMsgDeleteEntity, iMsgOnDeleteEntity
   *
   *  \param linkBlockSize Initial and increment block size for memory management of message links.
   *                         If zero, uses default size.
   *
   *  \param regBlockSize Initial and increment block size for memory management of registered messages.
   *                        If zero, uses default size.
   *
   */
   RwUInt32 CEventHandler::refCountEventHandlersCreated = 0;

   void CEventHandler::Open(RwUInt32 linkBlockSize, RwUInt32 regBlockSize)
   {
      RWS_FUNCTION("RWS::CEventHandler::Open");

      RWS_ASSERT(refCountEventHandlersCreated == 0,
         "Open has been called after CEventHandlers have been instanced.");

      // Initialize block memory allocation for associated classes.
      
      CLinkedMsg::InitMemHandler(linkBlockSize);
      CRegisteredMsgs::InitMemHandler(regBlockSize);
      
      RegisterMsg (iMsgDeleteEventHandler, iMsgDeleteEventHandlerStr,0);
      RegisterMsg (iMsgDeleteEntity, iMsgDeleteEntityStr,0);
      RegisterMsg (iMsgOnDeleteEntity, iMsgOnDeleteEntityStr,"CEventHandler*");

#if defined RWS_EVENTVISUALIZATION
      RegisterMsg (iMsgEventSend, iMsgEventSendStr, "CRegisteredMsgs*");
      RegisterMsg (iMsgEventSent, iMsgEventSentStr, "CRegisteredMsgs*");
      RegisterMsg (iMsgEventReceived, iMsgEventReceivedStr, "EventData*");
      RegisterMsg (iMsgEventUnRegistered, iMsgEventUnRegisteredStr, "CRegisteredMsgs*");
#endif

      RWS_RETURNVOID();
   }

  /**
   *
   *  Close the event system, this should be called when the event handling system is no longer needed.
   *
   *  \remarks Calls \ref UnRegisterMsg for each of the system events \ref iMsgDeleteEventHandler,
   *  \ref iMsgDeleteEntity and \ref iMsgOnDeleteEntity
   *
   *  \see UnRegisterMsg, iMsgDeleteEventHandler, iMsgDeleteEntity, iMsgOnDeleteEntity
   *
   */
   void CEventHandler::Close(void)
   {
      RWS_FUNCTION("RWS::CEventHandler::Close");

      RWS_ASSERT(refCountEventHandlersCreated == 0,
         "Close has been called before all instances of CEventHandler have been deleted.");

      UnRegisterMsg ( iMsgDeleteEventHandler ); 
      UnRegisterMsg ( iMsgDeleteEntity );
      UnRegisterMsg ( iMsgOnDeleteEntity );

#if defined RWS_EVENTVISUALIZATION
      UnRegisterMsg (iMsgEventSend );
      UnRegisterMsg (iMsgEventSent );
      UnRegisterMsg (iMsgEventReceived );
      UnRegisterMsg (iMsgEventUnRegistered );
#endif

      // Close block memory allocation for associated classes.
      // CRegisteredMsgs::CloseMemHandler should be called before CLinkedMsg::CloseMemHandler
      // in order to assist trapping of unlinked and unregistered events.
      
      CRegisteredMsgs::CloseMemHandler();
      CLinkedMsg::CloseMemHandler();
      
      RWS_RETURNVOID();
   }

 /**
   *
   *  Register an event/message with the event handler system.
   *
   *  \param Id A reference to an CEventId which will be filled in by this function.
   *
   *  \param p_msgname A pointer to a string which names the event/message.
   *
   *  \param p_formatstring A pointer to a string which specifies the format of the string.
   *
   *  Events are registered using an arbitrary string and an optional format string. If the format
   *  string is specified then it must match the format string of a previously registered
   *  event with the same name otherwise an exception will be generated,
   *
   *  \code 

      Example 1

      RegisterMsg(Id1,"MyEvent","RwInt32");
      RegisterMsg(Id2,"MyEvent","float");

      Example 2

      RegisterMsg(Id1,"MyEvent","RwInt32");
      RegisterMsg(Id2,"MyEvent","RwInt32");

      Example 3

      RegisterMsg(Id1,"MyEventA","RwInt32");
      RegisterMsg(Id2,"MyEventB","RwReal");

      \endcode 
   * 
   *  Example 1, will generate an exception error at run-time because the same event name "MyEvent"
   *  has been used twice but with a different format string specifically "RwInt32" and "RwReal"
   *
   *  Example 2, will not produce an error, as although the same event name is registered twice "MyEvent"
   *  the format string is the same "RwInt32". 
   *
   *  Example 3, is also ok as it defines two independent events specifically "MyEventA" and "MyEventB"
   *
   *  It is possible to specify that a client registering an event is only interested in the event
   *  and not the data associated specified by the format string. To do this the client passes 0
   *  as the format string, this has an implied meaning of don't care.
   *
   *  \code 

      Example 4

         RegisterMsg(Id1,"MyEvent","RwInt32");
         RegisterMsg(Id2,"MyEvent",0);

      Example 5

         RegisterMsg(Id2,"MyEvent",0);
         RegisterMsg(Id1,"MyEvent","RwInt32");
  
      \endcode
   *
   *  Example 4, shows the same event "MyEvent" being registered twice, in the first case (Id1) the format
   *  string is specified as "RwInt32". In the second case (Id2) "MyEvent" is registered again but the format
   *  string is set to 0 indicating that any data will either not be sent or will be ignored when received
   *  depending on how the event is used.
   *
   *  Example 5, is the same as Example 4 and shows that the order that events are registered is unimportant.
   *
   *  \note It is the responsibility of the client that registered an event to unregister
   *  \ref CEventHandler::UnRegisterMsg the same event, the
   *  event handler maintains a reference count for each CEventId for the number of times it is registered
   *  if the CEventId is destroyed without being unregistered the same number of times an assert will be
   *  generated. Allowing an event handler to register the same CEventId multiple times allows the event
   *  handler to share the CEventId with other event handlers, saving memory.
   *
   */

   void CEventHandler::RegisterMsg(
                          CEventId &Id,
                          const char *p_msgname,
                          const char *p_formatstring)
   {
      RWS_FUNCTION("RWS::CEventHandler::RegisterMsg");

      RWS_ASSERT(p_msgname, "CEventHandler::RegisterMsg p_msgname = 0");

      // p_char == NULL or strlen(p_char)<1 i.e. p_char == ""
      // not an event is return ERROR_NOT_A_VALID_MESSAGE_ID
      //
      if (!p_msgname || p_msgname[0] == 0)
      {
         // Can't register
         //
         Id = ERROR_NOT_A_VALID_MESSAGE_ID;

         RWS_RETURNVOID();
      }

      RwUInt32 ui_msgname = GenerateUIntIdFromString(p_msgname);
      RwUInt32 ui_format = GenerateUIntIdFromString(p_formatstring);

      // Set That this CEventId hasn't been registered
      //
      if (Id.Get_registered_count()>0)
      {
         // Registered with a different name ?
         //
         if (!StrIdentical (Id->ui_msgname, ui_msgname, Id->p_msgname, p_msgname))
         {
            RWS_ASSERT(FALSE,
               "CEventHandler::RegisterMsg CEventId Already registered with a different event "
               "name, previous name "<<Id->p_msgname<<" new name "<<p_msgname);
         }
         else
         {
            // Registered with the same name ?
            //
            if (Id->p_format != 0 && p_formatstring == 0)
            {
               // Behavior registering doesn't care about format
            }
            else if (Id->p_format == 0 && p_formatstring != 0)
            {
               // Previously registered with no format string, redefined using this format string
               // from then on it must match.
               //
               CRegisteredMsgs *pRMsg = Id;

			   // 형석
               // pRMsg->p_format = static_cast<char *>(::RWS_OP_NEW( strlen (p_formatstring) + 1));
			   RwInt32 iLen = (RwInt32)strlen (p_formatstring) + 1;
			   pRMsg->p_format = static_cast<char *>(NTL_NEW char[iLen]);
               
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
               strcpy_s (pRMsg->p_format, iLen, p_formatstring);
#else
			   strcpy (pRMsg->p_format, p_formatstring);
#endif
            }
            else if (Id->p_format != 0)
            {
               if (StrIdentical(Id->ui_format, ui_format, Id->p_format, p_formatstring))
               {
                  // Previously registered with same format...Just need to bump up the registered
                  // count and return
                  //

                  // Mark that this CEventId has been Registered
                  //
                  Id.Inc_registered_count();

                  // Early exit point, the same EventId has been registered multiple times.
                  //
                  RWS_RETURNVOID();
               }
               else
               {
                  RWS_ASSERT(FALSE,
                     "CEventHandler::RegisterMsg CEventId Already registered with a different "
                     "format string, previous format "<<Id->p_format<<" new format "<<p_formatstring);
               }
            }
         }
      }

      // Check if this message has already been registered
      //   
      CRegisteredMsgs *pRMsg = pRMsgs.p_next;
      
      while (pRMsg)
      {
         // Check if the name matches
         //
         if (pRMsg->p_msgname != 0)
         {
            RwBool samestring = StrIdentical (pRMsg->ui_msgname, ui_msgname, pRMsg->p_msgname, p_msgname);
      
            if (samestring) 
            {
               if (pRMsg->p_format != 0 && p_formatstring == 0)
               {
                  // Behavior registering doesn't care
               }
               else if (pRMsg->p_format == 0 && p_formatstring != 0)
               {
                  // Previously registered with no format string, redefine using this format string
                  // from then on it must match.
                  //
				  // 형석
                  // pRMsg->p_format = static_cast<char *>(::RWS_OP_NEW( strlen (p_formatstring) + 1));
				  RwInt32 iLen = (RwInt32)strlen (p_formatstring) + 1;
				  pRMsg->p_format = static_cast<char *>(NTL_NEW char[iLen]);
      
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
                  strcpy_s (pRMsg->p_format, iLen, p_formatstring);
#else
				  strcpy (pRMsg->p_format, p_formatstring);
#endif

                  pRMsg->ui_format = ui_format;
               }
               else if (pRMsg->p_format != 0
                  && !StrIdentical(pRMsg->ui_format, ui_format, pRMsg->p_format, p_formatstring))
               {
                  RWS_ASSERTFAIL( "RegisterMsg: " << p_msgname << " Message Format Does Not Match "
                     << pRMsg->p_format << " != " << p_formatstring );
               }

               // Message Already Registered
               //
               Id = pRMsg;

               // Mark that this CEventId has been Registered
               //
               Id.Inc_registered_count();

               RWS_RETURNVOID();
            }
         }
         pRMsg = pRMsg->p_next;
      }

      // Message Not Registered, create a new event/message handler and register this message,
      // if pRMsg an exception will be raised...this could be handled with a try/catch but 
      // I won't do that as it would have a performance hit.
      //
      pRMsg = NTL_NEW CRegisteredMsgs;     // Do NOT use RWS_NEW here as CRegisteredMsg has own operator new.

      // Link to head of the list
      //
      CRegisteredMsgs *p_next = pRMsgs.p_next;

      // this -> oldfirst
      //
      pRMsg->p_next = pRMsgs.p_next;
      
      // this <- oldfirst 
      //
      if (p_next) p_next->p_prev = pRMsg;

      // head <- this
      //
      pRMsg->p_prev = &pRMsgs;

      // head -> this
      //
      pRMsgs.p_next = pRMsg;

      // operator new some memory to store a copy of this string, this is important
      // as the address of the string which has been passed maybe on the stack
      //
      // Note: "ABC" returns 3 0 terminate requires "ABC" + 0
      //
	  // 형석
      // pRMsg->p_msgname = static_cast<char *>(::RWS_OP_NEW( strlen (p_msgname) + 1));
	  RwInt32 iLen = (RwInt32)strlen (p_msgname) + 1;
	  pRMsg->p_msgname = static_cast<char *>(NTL_NEW char [iLen]);
   
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
      strcpy_s (pRMsg->p_msgname, iLen, p_msgname);
#else
	  strcpy (pRMsg->p_msgname, p_msgname);
#endif
   
      if (p_formatstring)
      {
		  // 형석
          // pRMsg->p_format = static_cast<char *>(::RWS_OP_NEW( strlen (p_formatstring) + 1));
		  RwInt32 iLen = (RwInt32)strlen (p_formatstring) + 1;
		  pRMsg->p_format = static_cast<char *>(NTL_NEW char[strlen (p_formatstring) + 1]);
      
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
         strcpy_s (pRMsg->p_format, iLen, p_formatstring);
#else
		  strcpy (pRMsg->p_format, p_formatstring);
#endif
      }
      else
      {
         pRMsg->p_format = 0;
      }

      pRMsg->ui_format = ui_format;
      pRMsg->ui_msgname = ui_msgname;

      // New Message Registered
      //
      Id = pRMsg;

      // Mark that this CEventId has been Registered, also bump up the ref count of the registered event
      //
      Id.Inc_registered_count();

      RWS_RETURNVOID();
   }

  /**
   *
   *  Unregister an event, \note If the event hasn't been previously registered it is ignored.
   *
   *  \param Id A reference to the CEventId which specifies the event to unregister.
   *
   */

   void CEventHandler::UnRegisterMsg(CEventId &Id)
   {
      RWS_FUNCTION("RWS::CEventHandler::UnRegisterMsg");

      if (Id != ERROR_NOT_A_VALID_MESSAGE_ID)
      {
         CRegisteredMsgs *pRMsg = Id;

         // Check if this CEventId has been registered, if not return as can't unregister
         //
         if (Id.Get_registered_count() == 0)
         {
            // This may be a copy of a registered msg, set Id = ERROR_NOT_A_VALID_MESSAGE_ID;
            // to show that it has been unregistered. i.e.
            //
            // CEventId Msg;
            //
            // Msg = iMsgRunningTick
            //
            // UnRegisterMsg(Msg);  // Msg.pEventId = (iMsgRunningTick)
            //                   // m_linked_count = 0
            //                   // m_registered_count =0
            //
            // result
            //
            // Msg.pEventId = 0
            //
            Id = ERROR_NOT_A_VALID_MESSAGE_ID;

            RWS_RETURNVOID();
         }

         // Set that this event Id has been unregistered, also decrement the ref count of the
         // registered event
         //
         Id.Dec_registered_count();

         if (pRMsg->iCountRegistered<0)
         {
            RWS_ASSERT(FALSE, "CEventHandler::UnRegisterMsg pRMsg->ref count<0");
         }

         if (pRMsg->iCountRegistered == 0)
         {
            // Check if this event handler is no longer in use...
            //
            CLinkedMsg *pErm = pRMsg->Erm.p_next;
   
            if (pErm)
            {
               RWS_ASSERTFAIL( "CEventHandler::UnRegisterMsg " << pRMsg->p_msgname
                  << " Is still linked to CEventHandlers cannot unregister" );

               RWS_RETURNVOID();
            }

#ifdef RWS_EVENTVISUALIZATION
            CEventVisualization::EventUnRegistered(*pRMsg);
#endif

            // Free the memory associated with the name string
            //
			RWS_ARRAY_DELETE(pRMsg->p_msgname);
			// 형석
			// operator delete (pRMsg->p_msgname);
            pRMsg->p_msgname = 0;
   
            // Free the memory associated with the format string
            // 형석
            // operator delete (pRMsg->p_format);
			RWS_ARRAY_DELETE(pRMsg->p_format);
            pRMsg->p_format = 0;
   

            // Unlink and remove the pRMsg from the system
            //
            CRegisteredMsgs *p_next = pRMsg->p_next;
            CRegisteredMsgs *p_prev = pRMsg->p_prev; 

            if (p_prev) p_prev->p_next = p_next;
            if (p_next) p_next->p_prev = p_prev;

            Id = ERROR_NOT_A_VALID_MESSAGE_ID;  // CEventId has been unregistered

			NTL_DELETE( pRMsg );
         }
      }

      RWS_RETURNVOID();
   }

  /**
   *
   *  Deconstruct a \ref RWS::CEventHandler object, an event \ref iMsgOnDeleteEntity is
   *  sent informing any event handlers \ref RWS::CEventHandler that (this) event handler
   *  has been removed from the system.
   *
   */

   CEventHandler::~CEventHandler(void)
   {
      RWS_FUNCTION("RWS::CEventHandler::~CEventHandler");

      CMsg Msg(iMsgOnDeleteEntity, static_cast<CEventHandler*>(this));

      SendMsg(Msg);

#ifndef NDEBUG
      refCountEventHandlersCreated--;
#endif

      //Unlink from auto-delete list.
      //
      UnLinkMsg(iMsgDeleteEventHandler);

      //Unlink from delete
      // 
      UnLinkMsg(iMsgDeleteEntity);

#ifdef _DEBUG
      // Need to check that there are no messages linked to this CEventHandler
      // if there are then assert as unable to proceed...
      //
      CRegisteredMsgs *pRMsg = pRMsgs.p_next;
   
      while (pRMsg)
      {
         // Skip first CLinkedMsg as its the head of the list
         //
         CLinkedMsg *pErm = pRMsg->Erm.p_next;
      
         while (pErm != 0)
         {
            CLinkedMsg *p_next_Erm = pErm->p_next;
         
            // Found message registered to this CEventHandler
            //
            if (pErm->pCEventHandler == this )
            {
               RWS_ASSERTFAIL("CEventHandler::~CEventHandler not unlinked from ["<<pRMsg->p_msgname<<"]");
            }
         
            pErm = p_next_Erm;
         }

         pRMsg = pRMsg->p_next;
      }
#endif

      RWS_RETURNVOID();
   }

  /**
   *
   *  Links the specified event to (this) event handler. Linking to an event informs the event handler
   *  system that (this) event handler should be called when \ref SendMsg is called with the specified event
   *  via the event handlers \ref HandleEvent function.
   *
   *  \param Id The id of the event/message.
   *
   *  \param p_formatstring A string which define the format of the event/message, this field can
   *                        be used to check that the format of the event/message matches the registered
   *                        format of the same event/message.
   *
   *  \param priority       The priority of the link, 0 lowest 0xffff highest the
   *                        priority determines the order in which the CEventHandler::HandleEvents
   *                        function is called, if no value is specified 0x8000 is used. 
   *
   *  \see Examples         LinkPriority for example of using priority parameter to specify the calling
   *                        order in response to an event.
   *
   *  The following code example demonstrates several different ways of using LinkMsg;
   *
   *  \code 

      CMyClass::CMyFunc(void)
      {
         CEventId Id;

         // Register an event/message called MyEvent where pData will be of type RwUInt32
         //
         RegisterMsg(Id,"MyEvent","RwUInt32"); 

         // Link to event/message "MyEvent", default priority (0x8000), format is unspecified therefore
         // LinkMsg assumes that the client will not make use of the pData field of the CMsg
         //
         LinkMsg(Id);   
                        

         // Link to event/message "MyEvent", default priority (0x8000), format is specified and must
         // match the previously registered format string for the event MyEvent
         //
         LinkMsg(Id,"RwUInt32"); 
                                 
         // As above, except that will fail as format differs from Registered format string.
         //
         LinkMsg(Id,"RwReal");   

         // Link to event/message "MyEvent", highest priority (0xffff), format is specified and must
         // match the previously registered format string for the event MyEvent
         //
         LinkMsg(Id,"RwUint32",0xffff);   
                                          

         UnLinkMsg(Id);
         UnLinkMsg(Id);
         UnLinkMsg(Id);
         UnLinkMsg(Id);

         UnRegisterMsg(id);
      }

      \endcode
   *
   *
   *
   *  \note If the same event handler is linked to the same event more than once,
   *  the event will only be received once each time it is triggered. 
   *
   *  It is the responsibility of the client to UnLinkMsg from any events the correct
   *  number of time otherwise an assert will occur when the deconstructor for CEventId
   *  is called.
   *
   *  \see UnLinkMsg, RegisterMsg, UnRegisterMsg
   *
   */
   void CEventHandler::LinkMsg(CEventId &Id,
                               const char *p_formatstring,
                               RwUInt16 priority)
   {
      RWS_FUNCTION("RWS::CEventHandler::LinkMsg");

      if (Id != ERROR_NOT_A_VALID_MESSAGE_ID)
      {
         LinkMsgToEventHandler(this, Id, p_formatstring, priority);
      }

      RWS_RETURNVOID();
   }

  /**
   *
   *  Unlink (this) event handler from the specified event.
   *
   *  \note If the event handler hasn't been linked to this event it is ignored.
   *
   *  \param Id reference to a \ref CEventId which specifies the event.
   *
   *  \see LinkMsg, RegisterMsg, UnRegisterMsg
   *
   */

   void CEventHandler::UnLinkMsg(CEventId &Id)
   {
      RWS_FUNCTION("RWS::CEventHandler::UnLinkMsg");

      if (Id == ERROR_NOT_A_VALID_MESSAGE_ID)
      {
	        RWS_RETURNVOID();
      }

      if (Id.Get_linked_count() == 0)
      {
         RWS_RETURNVOID();
      }

      CRegisteredMsgs *pRMsg = Id;

      if (pRMsg->p_msgname != 0)
      {
         CLinkedMsg *pErm = &pRMsg->Erm;
   
         while (pErm != 0)
         {
            CLinkedMsg *p_next_Erm = pErm->p_next;
      
            // Found message registered to this CEventHandler
            //
            if (pErm->pCEventHandler == this )
            {
               Id.Dec_linked_count();

               pErm->iCountLinked -= 1;

               if (pErm->iCountLinked < 0)
               {
                  RWS_ASSERT(FALSE, "CEventHandler::UnLinkMsg pErm->ref count<0");
               }

               if (pErm->iCountLinked == 0)
               {
                  // Check if need to setup pending unlink event
                  //
                  if (pRMsg->HandlingEvent)
                  {
                     pErm->pCEventHandler = 0;

                     pRMsg->PendingUnlink = TRUE;
                  }
                  else
                  {
                     CLinkedMsg *p_prev_Erm = pErm->p_prev;
                     
                     // p_prev_Erm <- pErm -> p_next_Erm Linked List remove entry
                     //
                     if (p_prev_Erm) p_prev_Erm->p_next = p_next_Erm;
                     if (p_next_Erm) p_next_Erm->p_prev = p_prev_Erm;
                     
					 NTL_DELETE( pErm );

                     RWS_RETURNVOID();
                  }
               }
               break;
            }
            pErm = p_next_Erm;
         }
      }

      RWS_RETURNVOID();
   }

  /**
   *
   *  Deletes each of the event handlers \ref CEventHandler linked to the specified event \ref CMsg,
   *  uses delete operator which under certain circumstances may be unsafe see \ref Delete method for
   *  explanation of when it is unsafe to use DeleteEventHandlers.
   *
   *  \param p_Msg A pointer to the CMsg class specifying the event.
   *
   *  \see DeleteEventHandlers(const CEventId &Id), Delete
   *
   */

   void CEventHandler::DeleteEventHandlers(
                              const CMsg *p_Msg)
   {
      RWS_FUNCTION("RWS::CEventHandler::DeleteEventHandlers");

      RWS_ASSERT(p_Msg, "CEventHandler::DeleteEventHandlers p_Msg = 0");

      RWS_ASSERT(p_Msg->Id->HandlingEvent == FALSE,
         "ERROR::CEventHandler::DeleteEventHandlers cannot be called from within "
         "an EventHandler, use RegisterToDeleteEventHandlers");

      CRegisteredMsgs *pRMsg = p_Msg->Id;
   
      if (pRMsg != ERROR_NOT_A_VALID_MESSAGE_ID)
      {
         CLinkedMsg *pErm = pRMsg->Erm.p_next; // First Entity Registered Message
   
         while (pErm != 0)
         {
			 NTL_DELETE( pErm->pCEventHandler );

            // Always take from the front of the queue - it's possible that
            // one event handler will delete others.
            pErm = pRMsg->Erm.p_next;
         }
      }

      RWS_RETURNVOID();
   }


  /**
   *
   *  Deletes each of the event handlers linked to the specified event, uses delete operator
   *
   *  \param Id A reference to a CEventId specifying the event.
   *
   *  \see DeleteEventHandlers(const CMsg *p_Msg), Delete
   *
   *  \warning see Delete method for explanation of when it is unsafe to use DeleteEventHandlers.
   *
   */
   void CEventHandler::DeleteEventHandlers(const CEventId &Id)
   {
      RWS_FUNCTION("RWS::CEventHandler::DeleteEventHandlers");

      if (Id != ERROR_NOT_A_VALID_MESSAGE_ID)
      {
         CMsg iMsg(Id);
   
         // Send message
         //
         DeleteEventHandlers (&iMsg);
      }

      RWS_RETURNVOID();
   }


  /**
   *
   *  Register each event handler linked to this event for deletion, uses \ref Delete method.
   *
   *  \param p_Msg A pointer to CMsg class specifying the event.
   *
   *  \see RegisterToDeleteEventHandlers(const CEventId &Id), Delete
   *
   *  \warning see Delete method for explanation of when it is unsafe to use DeleteEventHandlers.
   *
   */

   void CEventHandler::RegisterToDeleteEventHandlers(
                              const CMsg *p_Msg)
   {
      RWS_FUNCTION("RWS::CEventHandler::RegisterToDeleteEventHandlers");

      RWS_ASSERT(p_Msg, "CEventHandler::DeleteEventHandlers p_Msg = 0");

      RWS_ASSERT(p_Msg->Id->HandlingEvent == FALSE,
         "ERROR::CEventHandler::DeleteEventHandlers cannot be called from within"
         "an EventHandler, use RegisterToDeleteEventHandlers");

      CRegisteredMsgs *pRMsg = p_Msg->Id;
   
      if (pRMsg != ERROR_NOT_A_VALID_MESSAGE_ID)
      {
         CLinkedMsg *pErm = pRMsg->Erm.p_next; // First Entity Registered Message
   
         while (pErm != 0)
         {
            pErm->pCEventHandler->Delete();

            // Always take from the front of the queue - it's possible that

            // one event handler will delete others.
            pErm = pRMsg->Erm.p_next;
         }
      }

      RWS_RETURNVOID();
   }

  /**
   *
   *  Register each event handler linked to this event for deletion, uses \ref Delete method.
   *
   *  \param Id A reference to a CEventId specifying the event.
   *
   *  \see RegisterToDeleteEventHandlers(const CMsg *p_Msg), Delete
   *
   *
   */

   void CEventHandler::RegisterToDeleteEventHandlers(
                                const CEventId &Id)
   {
      RWS_FUNCTION("RWS::RegisterToDeleteEventHandlers");

      if (Id != ERROR_NOT_A_VALID_MESSAGE_ID)
      {
         CMsg iMsg(Id);

         RegisterToDeleteEventHandlers (&iMsg);
      }

      RWS_RETURNVOID();
   }

  /**
   *
   *  This function is provided as a safe way to delete an event handler, It is sometimes
   *  possible to use (delete this;) but it is not advised as this will immediately free
   *  the instance of the event handler which may result in unsafe behavior.
   *
   *  \param pCEventHandler A pointer to the CEventHandler.
   *
   *  Consider the following example
   *
   *  \code

      void CMyClass::HandleEvent(HandleEvents(CMsg &pMsg))
      {
         if (pMsg.Id == Event1)
         {
            SendMsg(Event2);  // calls CMyClass::HandleEvent where pMsg.Id == Event2
            .
            .
            .
               Code returns here after Event2 is processed, which may be illegal as
               "delete this;" has already been called to free (this) instance of the class.
            .
            .
            .
         }
         else if (pMsg.Id == Event2)
         {
            delete this.
            .
            .
            .
            returns to point following SendMsg
         }
      }

      \endcode
   *
   *  The safe way to do this is to use the Delete method
   *  \code

      void CMyClass::HandleEvent(HandleEvents(CMsg &pMsg))
      {
         if (pMsg.Id == Event1)
         {
            SendMsg(Event2);  // calls CMyClass::HandleEvent where pMsg.Id == Event2
            .
            .
            .
               Code returns here after Event2 is processed, but (this) instance of the
               class is not deleted until the end of the current game frame by calling
               CEventHandler::DeleteEventHandlers(iMsgDeleteEntity);
            .
            .
            .
         }
         else if (pMsg.Id == Event2)
         {
            Delete()
            .
            .
            .
            returns to point following SendMsg
         }
      }

      \endcode
   *
   *
   *  \note When a CEventHandler is deleted a message iMsgOnDeleteEntity is sent to each of the event
   *  handlers linked to the iMsgOnDeleteEntity event, the pData field of the CMsg class contains a
   *  pointer to the event handler that has been deleted.
   *
   *  This can be useful if another event handler maintains a pointer to the first, as it can maintain
   *  when the pointer becomes invalid. It is the responsibility of the client to link, and unlink from
   *  the iMsgOnDeleteEntity if this service is required.
   *
   */

   void CEventHandler::Delete(CEventHandler *pCEventHandler)
   {
      RWS_FUNCTION("RWS::CEventHandler::DeleteEntity");

      pCEventHandler->m_Active = FALSE;

      LinkMsgToEventHandler(pCEventHandler, iMsgDeleteEntity, 0);

      RWS_RETURNVOID();
   }

  /**
   *
   *  Links the specified event \ref CEventid to the specified event handler \ref CEventHandler. 
   *
   *  \param pCEventHandler A pointer to event handler.
   *
   *  \param Id The id of the event.
   *
   *  \param p_formatstring A string which define the format of the event/message, this field can
   *                        be used to check that the format of the event/message matches the registered
   *                        format of the same event/message.
   *
   *  \param priority       The priority of the link, 0 lowest 0xffff highest the
   *                        priority determines the order in which the CEventHandler::HandleEvents
   *                        function is called, if no value is specified 0x8000 is used. 
   *
   *  \see LinkMsg
   *
   */

   RwBool CEventHandler::LinkMsgToEventHandler(
                        CEventHandler *pCEventHandler,
                        CEventId &Id,                 
                        const char *p_formatstring,   
                                                      
                                                      
                        RwUInt16 priority)      
   {
      RWS_PRE(pCEventHandler);

      RWS_FUNCTION("RWS::CEventHandler::LinkMsgToEventHandler" );

      if (Id != ERROR_NOT_A_VALID_MESSAGE_ID)
      {
         CRegisteredMsgs *pRMsg = Id;

         if (Id.Get_linked_count()>0)
         {
         }

         Id.Inc_linked_count();

         // Check if this event handler has already been linked to this event,
         // if it has increment the reference count and return.
         //
         CLinkedMsg *pErm = Id->Erm.p_next;   

         while (pErm != 0)
         {
            if (pErm->pCEventHandler == pCEventHandler) 
            {
               RWS_ASSERT( ( priority == pErm->m_priority ),
                  "CEventHandler::LinkMsgToEventHandler has been called to link to an event,"
                  " that this event handler is already linked to but with a different priority."
                  " Prev Priority "
                  << RWS_DEC(priority) << " New Priority " << RWS_DEC(pErm->m_priority) );

               // Increment reference count
               //
               pErm->iCountLinked += 1;

               RWS_RETURN( TRUE );
            }
            pErm = pErm->p_next;
         }

         // Do we care what the format of the message is or are we only using it as a signal
         //
         if (p_formatstring)
         {
            // Yep we care...
            //
            if (pRMsg->p_format == 0)
            {
               // No format string was specified when the event was registered, i.e. its just a signal
               // so the receiver will always receive NULL as the parameter
            }
            else
            {
               RwUInt32 ui_format = GenerateUIntIdFromString(p_formatstring);

               // Check that the format strings match
               //
               if (!StrIdentical(pRMsg->ui_format, ui_format, pRMsg->p_format, p_formatstring))
               {
                  RWS_ASSERTFAIL( "CEventHandler::LinkMsgToEntity Message Format for event "
                     << pRMsg->p_msgname << " does not match "
                     << pRMsg->p_format << "!=" << p_formatstring );
                  
                  RWS_RETURN(FALSE);
               }
            }
         }
         
         // Allocate the CEventHandlerRegisteredMsg which links the event to the event
         // handler and add it onto the event list
         // Do NOT use RWS_NEW here as CEventHandlerRegisteredMsg has own operator new.
         //
         CLinkedMsg *pNewErm = NTL_NEW CLinkedMsg(pCEventHandler, priority);
         
         if (!pNewErm)
         {
            RWS_ASSERT(pNewErm, "CEventHandler::LinkMsgToEventHandler Unable to allocate CLinkedMsg struct");

            RWS_RETURN(FALSE);
         }
 
         // Insertion Sort...
         //

         CLinkedMsg *pheadErm = const_cast<CLinkedMsg*>(&Id->Erm);  // Head of list
         CLinkedMsg *ptailErm = Id->Erm.p_next;  // Tail of list

         while (ptailErm)
         {
            if ( pheadErm->m_priority >= priority
            &&  ptailErm->m_priority <= priority )
            {
               // Insert into list
               //
               pheadErm->p_next = pNewErm;
               pNewErm->p_prev = pheadErm;

               ptailErm->p_prev = pNewErm;
               pNewErm->p_next = ptailErm;

               RWS_RETURN(TRUE);
            }

            pheadErm = ptailErm;
            ptailErm = ptailErm->p_next;
         }

         pheadErm->p_next = pNewErm;
         pNewErm->p_prev = pheadErm;
      }

      RWS_RETURN(TRUE);
   }

  /**
   *
   *  IsLinked Determine if (this) event handler \ref CEventHandler is linked to the specified
   *  event \ref CEventid.
   *
   *  \return TRUE is event handler is linked to the specified event, otherwise FALSE.
   *
   *  \param Id A reference to a CEventId specifying the event.
   *
   */

   RwBool CEventHandler::IsLinked(CEventId &Id)
   {
      RWS_FUNCTION("RWS::CEventHandler::IsLinked");

      if (Id == ERROR_NOT_A_VALID_MESSAGE_ID)
      {
         RWS_RETURN(FALSE);
      }

      // Check if this event handler has already been linked to this event,
      //
      CLinkedMsg *pErm = Id->Erm.p_next;   

      while (pErm != 0)
      {
         if (pErm->pCEventHandler == this) 
         {
            RWS_RETURN(TRUE);
         }
         pErm = pErm->p_next;
      }

      RWS_RETURN(FALSE);
   }

  /**
   *
   * \ref Determines the priority with which the event has been linked to the CEventHandler.
   *
   * \param Id A reference to a CEventId specifying the event.
   *
   * \return Returns -1 if the CEventId is invalid i.e. not registered, -2 if the CEventId
   *          isn't linked to this CEventHandler, otherwise returns the priority 0 to 0xffff.
   *
   */
   RwInt32 CEventHandler::GetLinkedPriority(CEventId &Id)
   {
      RWS_FUNCTION("RWS::CEventHandler::IsLinked");

      if (Id == ERROR_NOT_A_VALID_MESSAGE_ID)
      {
         RWS_RETURN(-1);
      }

      // Check if this event handler has already been linked to this event,
      //
      CLinkedMsg *pErm = Id->Erm.p_next;   

      while (pErm != 0)
      {
         if (pErm->pCEventHandler == this) 
         {
            RWS_RETURN(pErm->m_priority);
         }
         pErm = pErm->p_next;
      }

      RWS_RETURN(-2);
   }

  /**
   *
   *  [Example code] Lists all the registered events, and the links to event handlers
   *  for each of those events.
   *
   *  \note Only available if RWS_EVENTVISUALIZATION is defined. Uses RWS_TRACE macro.
   *
   */

   /*
   void CEventHandler::DebugEventHandler(void)
   {
      RWS_FUNCTION("RWS::CEventHandler::DebugEventHandler");

      CRegisteredMsgs *pRMsg = pRMsgs.p_next;

      while (pRMsg)
      {
         RWS_TRACEif (pRMsg->p_msgname, "pRMsg->p_msgname " << pRMsg->p_msgname);
         RWS_TRACEif (pRMsg->p_format, "pRMsg->p_format " << pRMsg->p_format);

         CLinkedMsg *pErm = pRMsg->Erm.p_next;

         while (pErm)
         {
            RWS_TRACE("\t\tLinked To " << RWS_HEX(pErm->pCEventHandler)
                        << " (" << RWS_DEC(pErm->iCountLinked) << ")");

            pErm = pErm->p_next;
         }

         pRMsg=pRMsg->p_next;
      }

      RWS_RETURNVOID();
   }
   */

  /**
   *
   *  Re-registers an event, this is a function that is used to simplify the code of a commonly required
   *  task regarding events, ReplaceRegisteredMsg will unregister the event and then registers it with
   *  the new event specified by the name of the event.
   *
   *  \param Id A reference to a CEventid
   *
   *  \param p_msgname A pointer to a string which names the event/message.
   *
   *  \param p_formatstring A pointer to a string which specifies the format of the string.
   *
   *  Implementation;
   *
   *  \code

      void CEventHandler::ReplaceRegisteredMsg(CEventId &Id,const char *p_char,const char *p_formatstring)
      {
         UnRegisterMsg(Id);
         RegisterMsg(Id,p_char,p_formatstring);
      }
      
      \endcode
   *
   */

   void CEventHandler::ReplaceRegisteredMsg(CEventId &Id,
      const char *p_msgname,
      const char *p_formatstring)
   {
      RWS_FUNCTION("RWS::CEventHandler::ReplaceRegisteredMsg");

      UnRegisterMsg(Id);
      RegisterMsg(Id, p_msgname, p_formatstring);

      RWS_RETURNVOID();
   }

  /**
   *
   *  Re-registers an event, and re-link to that event.
   *
   *  \param Id A reference to a CEventid
   *
   *  \param p_msgname A pointer to a string which names the event/message.
   *
   *  \param p_formatstring A pointer to a string which specifies the format of the string.
   *
   *  Implementation;
   *
   *  \code

      void CEventHandler::ReplaceLinkedMsg(CEventId &Id,const char *p_char,const char *p_formatstring)
      {
         UnLinkMsg(Id);
         UnRegisterMsg(Id);
         RegisterMsg(Id,p_char,p_formatstring);
         LinkMsg(Id,p_formatstring);
      }
      
      \endcode
   *
   */

   void CEventHandler::ReplaceLinkedMsg(CEventId &Id,
      const char *p_msgname,
      const char *p_formatstring)
   {
      RWS_FUNCTION("RWS::CEventHandler::ReplaceLinkedMsg");

      UnLinkMsg(Id);
      UnRegisterMsg(Id);
      RegisterMsg(Id, p_msgname, p_formatstring);
      LinkMsg(Id, p_formatstring);

      RWS_RETURNVOID();
   }

   /**
   *
   * Static version of ReplaceLinkedMsg - use by direct-set message handling functions.
   *
   * \param rEventHand A reference to the CEventHandler or derived class (ie behaviour's class).
   *
   * \param Id A reference to a CEventid
   *
   * \param p_msgname A pointer to a string which names the event/message.
   *
   * \param p_formatstring A pointer to a string which specifies the format of the string.
   *
   */

   void CEventHandler::ReplaceLinkedMsg(CEventHandler &rEventHand, CEventId &Id, const char *p_msgname,
      const char *p_formatstring, RwUInt16 priority)
   {
      RWS_FUNCTION("RWS::CEventHandler::ReplaceLinkedMsg (static)");

      rEventHand.UnLinkMsg(Id);
      UnRegisterMsg(Id);
      RegisterMsg(Id, p_msgname, p_formatstring);
      rEventHand.LinkMsg(Id, p_formatstring, priority);

      RWS_RETURNVOID();
   }

   /**
   *
   * Static function which changes the link priority on a message. For use by 'direct-set' static
   * functions.
   *
   * \param rEventHand A reference to the CEventHandler or derived class (ie behaviour's class).
   *
   * \param Id A reference to a CEventid
   *
   * \param p_formatstring A pointer to a string which specifies the format of the string.
   *
   * \param priority The new linkage priority, default is 'standard' setting.
   *
   */

   void CEventHandler::SetLinkPriority(CEventHandler &rEventHand, CEventId &Id, 
      const char *p_formatString, RwUInt16 priority)
   {
      RWS_FUNCTION("RWS::CEventHandler::SetLinkPriority");

      rEventHand.UnLinkMsg(Id);
      rEventHand.LinkMsg(Id, p_formatString, priority);

      RWS_RETURNVOID();
   }

 /**
   *
   *  Low level send an event, skips test for validating the event, i.e.
   *  use SendMsg to perform the validity test inline. SendMsg calls _SendMsg
   *  if the test passes.
   *
   *  For each of the event handlers linked to the specified event call the virtual
   *  method HandleEvent.
   *
   *  \param p_Msg A reference to a CMsg class specifying the event.
   *
   */
   RwBool _SendMsg(
                   CMsg &p_Msg)
   {
      RWS_FUNCTION("RWS::CEventHandler::_SendMsg");

#ifdef RWS_EVENTVISUALIZATION      
      CEventVisualization::EventSend(*p_Msg.Id);
#endif

      // Get a pointer to the first entity registered message
      //
      CLinkedMsg *pErm = p_Msg.Id->Erm.p_next; 
   
      if (!pErm) 
      {
#ifdef RWS_EVENTVISUALIZATION      
         CEventVisualization::EventSent(*p_Msg.Id);
#endif
         RWS_RETURN( FALSE );
      }

      // This is the event we are currently processing, this is important in order to process
      // unlinking from this event.
      //
      p_Msg.Id->HandlingEvent = TRUE;

      do
      {
         // Translate the message
         //
         // pErm-pCEventHandler may be 0 here, so need to check before attempting the function call.
         //
         // E.g. If there are two behaviors linked to this message :
         //    p_Msg
         //      - Behavior1
         //        - Behavior2
         //
         // Then if Behavior2 is unlinked within Behaviors HandleEvents() 
         // its pErm->pCEventHandler will be set to 0, pending it being properly unlinked
         // after this while loop.
         //
         if (pErm->pCEventHandler && pErm->pCEventHandler->m_Active)
         {
            pErm->pCEventHandler->HandleEvents(p_Msg);
         }

#ifdef RWS_EVENTVISUALIZATION
         EventData   _EventData( 0, // Src
                                 pErm->pCEventHandler,  // Dst
                                 p_Msg.Id); // The event
         
         CEventVisualization::EventReceived(_EventData);
#endif
         
         pErm = pErm->p_next;  
      } while (pErm != 0);

      p_Msg.Id->HandlingEvent = FALSE;

      if (p_Msg.Id->PendingUnlink)
      {
         CEventHandler::ProcessPendingUnlinks(p_Msg.Id->Erm.p_next);

         p_Msg.Id->PendingUnlink = FALSE;
      }

#ifdef RWS_EVENTVISUALIZATION      
      CEventVisualization::EventSent(*p_Msg.Id);
#endif

      RWS_RETURN( TRUE );
   }

 /**
   *
   *  Removes any pending unlinked event handlers from the specified event, this 
   *  function is used to cleanup the list of (linked) event handlers where an event handler has unlinked
   *  from an event while the list is being traversed.
   *
   *  \param pErm A pointer to a CLinkedMsg this is the head of a list, it is used to
   *  specify an event and maintains a linked list of event handlers (linked) to that event.
   *
   *  \note This function is used by SendMsg.
   *  
   */

   void CEventHandler::ProcessPendingUnlinks(CLinkedMsg *pErm)
   {
      RWS_FUNCTION("RWS::CEventHandler::ProcessPendingUnlinks");

      while (pErm != 0)
      {
         CLinkedMsg *p_next_Erm = pErm->p_next;
   
         // Found pending unlink
         //
         if (pErm->pCEventHandler == 0 )
         {
              CLinkedMsg *p_prev_Erm = pErm->p_prev;

              // p_prev_Erm <- pErm -> p_next_Erm Linked List remove entry
              //
              if (p_prev_Erm) p_prev_Erm->p_next = p_next_Erm;
              if (p_next_Erm) p_next_Erm->p_prev = p_prev_Erm;

			  NTL_DELETE( pErm );
         }
         pErm = p_next_Erm;
      }

      RWS_RETURNVOID();
   }

#ifdef RWS_EVENTVISUALIZATION

 /**
   *
   *  Low level send an event, skips test for valid message, i.e.
   *  use SendMsg to perform the validity test inline, this then calls _SendMsg
   *  if the test passes.
   *
   *  For each of the event handlers linked to the specified event call the virtual
   *  method HandleEvent.
   *
   *  \param p_Msg A reference to a CMsg class specifying the event.
   *
   *  \note Only available if RWS_EVENTVISUALIZATION is defined enabling event Visualization.
   */

   RwBool CEventHandler::_SendMsg(CMsg &p_Msg)
   {
      RWS_FUNCTION("RWS::CEventHandler::_SendMsg");

      CEventVisualization::EventSend(*p_Msg.Id);

      // this->m_pCEventVisualization

      // Get a pointer to the first entity registered message
      //
      CLinkedMsg *pErm = p_Msg.Id->Erm.p_next; 
   
      if (!pErm) 
      {
         CEventVisualization::EventSent(*p_Msg.Id);

         RWS_RETURN( FALSE );
      }

      p_Msg.Id->HandlingEvent = TRUE;

      do
      {
         EventData   _EventData(this, // Src
                                pErm->pCEventHandler,  // Dst
                                p_Msg.Id); // The event

         CEventVisualization::EventReceived(_EventData);
 
         // Translate the message
         //
         // pErm-pCEventHandler may be 0 here, so need to check before attempting the function call.
         //
         // E.g. If there are two behaviors linked to this message :
         //    p_Msg
         //      - Behavior1
         //        - Behavior2
         //
         // Then if Behavior2 is unlinked within Behaviors HandleEvents() 
         // its pErm->pCEventHandler will be set to 0, pending it being properly unlinked
         // after this while loop.
         //
         if (pErm->pCEventHandler && pErm->pCEventHandler->m_Active)
         {
            pErm->pCEventHandler->HandleEvents(p_Msg);
         }

         pErm = pErm->p_next;
      } while (pErm != 0);

      p_Msg.Id->HandlingEvent = FALSE;

      if (p_Msg.Id->PendingUnlink)
      {
         ProcessPendingUnlinks(p_Msg.Id->Erm.p_next);

         p_Msg.Id->PendingUnlink = FALSE;
      }

      CEventVisualization::EventSent(*p_Msg.Id);

      RWS_RETURN( TRUE );
   }
#endif

 /**
   *
   *  Call the HandleEvent function of the specified event handler.
   *
   *  \param p_Msg A reference to a CMsg class specifying the event.
   *
   *  \param pCEventHandler A pointer to the event handler that will receive the event.
   *
   */

   void SendMsgToEventHandler(
                          CMsg &p_Msg,                   
                          CEventHandler *pCEventHandler) 
   {
      RWS_FUNCTION("RWS::CEventHandler::SendMsgToEventHandler");

      if (pCEventHandler && p_Msg.Id != ERROR_NOT_A_VALID_MESSAGE_ID)
      {
#ifdef RWS_EVENTVISUALIZATION      
         CEventVisualization::EventSend(*p_Msg.Id);
#endif

         if (pCEventHandler->m_Active)
         {
            p_Msg.Id->HandlingEvent = TRUE;

#ifdef RWS_EVENTVISUALIZATION
            EventData   _EventData(0, // Src
                                   pCEventHandler,  // Dst
                                   p_Msg.Id); // The event

            CEventVisualization::EventReceived(_EventData);
#endif
            // Translate the message
            //
            pCEventHandler->HandleEvents(p_Msg);

            p_Msg.Id->HandlingEvent = FALSE;

            if (p_Msg.Id->PendingUnlink)
            {
               CEventHandler::ProcessPendingUnlinks(p_Msg.Id->Erm.p_next);

               p_Msg.Id->PendingUnlink = FALSE;
            }
         }
#ifdef RWS_EVENTVISUALIZATION      
         CEventVisualization::EventSent(*p_Msg.Id);
#endif
      }
      RWS_RETURNVOID();
   }

 /**
   *
   *  SendMsgToEventHandler Call the HandleEvent function of the specified event handler.
   *
   *  \param p_Msg A reference to a CMsg class specifying the event.
   *
   *  \param pCEventHandler A pointer to the event handler that will receive the event.
   *
   */

   void SendMsgToEventHandler(
                            const CEventId &Id,
                            CEventHandler *pCEventHandler)
   {
      RWS_FUNCTION("RWS::CEventHandler::SendMsgToEventHandler");

      if (pCEventHandler)
      {
         CMsg tMsg(Id);
         SendMsgToEventHandler(tMsg, pCEventHandler);
      }

      RWS_RETURNVOID();
   }

 /**
   *
   *  SendMsgToEventHandler Call the HandleEvent function of the specified event handler.
   *
   *  \param p_Msg reference to a CMsg class specifying the event.
   *
   *  \param pCEventHandler pointer to the event handler.
   *
   */

   void CEventHandler::SendMsgToEventHandler(
                          CMsg &p_Msg,
                          CEventHandler *pCEventHandler)
   {
      RWS_FUNCTION("RWS::CEventHandler::SendMsgToEventHandler");

      if (pCEventHandler && p_Msg.Id != ERROR_NOT_A_VALID_MESSAGE_ID)
      {
#ifdef RWS_EVENTVISUALIZATION      
         CEventVisualization::EventSend(*p_Msg.Id);
#endif
         if (pCEventHandler->m_Active)
         {
            p_Msg.Id->HandlingEvent = TRUE;

#ifdef RWS_EVENTVISUALIZATION
            EventData   _EventData(this, // Src
                                   pCEventHandler,  // Dst
                                   p_Msg.Id); // The event

            CEventVisualization::EventReceived(_EventData);
#endif
            // Translate the message
            //
            pCEventHandler->HandleEvents(p_Msg);

            p_Msg.Id->HandlingEvent = FALSE;

            if (p_Msg.Id->PendingUnlink)
            {
               CEventHandler::ProcessPendingUnlinks(p_Msg.Id->Erm.p_next);

               p_Msg.Id->PendingUnlink = FALSE;
            }
         }
#ifdef RWS_EVENTVISUALIZATION      
         CEventVisualization::EventSent(*p_Msg.Id);
#endif
      }
      RWS_RETURNVOID();
   }

 /**
   *
   *  SendMsgToEventHandler Call the HandleEvent function of the specified event handler.
   *
   *  \param Id A reference to a CEventId class specifying the event.
   *
   *  \param pCEventHandler A pointer to the event handler.
   *
   */

   void CEventHandler::SendMsgToEventHandler(
                            const CEventId &Id,
                            CEventHandler *pCEventHandler)
   {
      RWS_FUNCTION("RWS::CEventHandler::SendMsgToEventHandler");

      if (pCEventHandler)
      {
         CMsg tMsg(Id);
         SendMsgToEventHandler(tMsg, pCEventHandler);
      }

      RWS_RETURNVOID();
   }

 /**********************************************
   *
   * CLinkedMsg class functions.
   *
   ********************************************/

   RwFreeList *CLinkedMsg::pFreeList = 0;
   const RwUInt32 CEVENT_HANDLER_REGISTERED_MSG_DEFAULT_BLOCK_SIZE = 20;

 /**
   *
   * \ref CLinkedMsg InitMemHandler static function to initialize the memory handling system.
   *
   * \param blockSize The initial and increment block size for memory allocation. Zero = use default.
   *
   */

   void CLinkedMsg::InitMemHandler(RwUInt32 blockSize)
   {
      RWS_FUNCTION("RWS::CLinkedMsg::InitMemHandler");

      if (!blockSize)
      {
         blockSize = CEVENT_HANDLER_REGISTERED_MSG_DEFAULT_BLOCK_SIZE;
      }

      pFreeList = RwFreeListCreate(sizeof(CLinkedMsg), blockSize, 16, rwMEMHINTDUR_GLOBAL);
      RWS_ASSERT(pFreeList, "failed to allocate free list");

      RWS_RETURNVOID();
   }

 /**
   *
   * \ref CLinkedMsg PurgeMem static function to purge unused memory within system
   *      (possible large overhead).
   *
   */

   void CLinkedMsg::PurgeMem(void)
   {
      RWS_FUNCTION("RWS::CLinkedMsg::PurgeMem");

      if (pFreeList)
      {
         RwFreeListPurge(pFreeList);
      }

      RWS_RETURNVOID();
   }

#ifndef NDEBUG
   void CLinkedMsg::CheckFreeListEmptyCallBack(void *pMem, void *pData) 
   {
      RWS_FUNCTION("RWS::CLinkedMsg::CheckFreeListEmptyCallBack");
      
      // At this point we only have a CLinkedMsg, from this we can determine the behavior
      // linked to the message, pEHRMsg->pCEventHandler, but not the name of the event. This
      // should never happen.
      //
      RWS_ASSERTFAIL("An event hasn't been UnLinked before calling CLinkedMsg::CloseMemHandler");

      RWS_RETURNVOID();
   }
#endif

 /**
   *
   * \ref CLinkedMsg CloseMemHandler static function to close the memory handling system.
   *
   */

   void CLinkedMsg::CloseMemHandler(void)
   {
      RWS_FUNCTION("RWS::CLinkedMsg::CloseMemHandler");

      if (pFreeList)
      {
#ifndef NDEBUG
         RwFreeListForAllUsed  ( pFreeList, CheckFreeListEmptyCallBack, 0);
#endif
         RwFreeListDestroy(pFreeList);
         pFreeList = 0;
      }

      RWS_RETURNVOID();
   }

 /**
   *
   * new - allocates new objects using the RenderWare free lists. Can only allocate base type objects.
   *
   */

   void *CLinkedMsg::operator new(size_t size)
   {
      RWS_FUNCTION("RWS::CLinkedMsg::operator new");

      // NOTE: Cannot allow classes to derive from this class as it uses an RwFreeList for the memory
      //       handling. This requires objects of a FIXED size, therefore derived classes of a different
      //       size would cause memory overruns, etc.

      RWS_ASSERT(size == sizeof(CLinkedMsg), "Cannot derive from class, no support due to RwFreeList");
      RWS_ASSERT(pFreeList, "System not initialized, need to call 'InitMemHandler'");

      // Get a new block from the RwFreeList

      RWS_RETURN(RwFreeListAlloc(pFreeList, rwMEMHINTDUR_GLOBAL));
   }

   void *CLinkedMsg::operator new[](size_t size)
   {
      // NOTE: array allocation is not allowed for this type. Should not be needed. Using an array would
      //       preclude the use of RwFreeList and therefore slow the system.

      RWS_FUNCTION("RWS::CLinkedMsg::operator new[]");
      RWS_ASSERT(0, "Cannot allocate arrays for this type");
      // Note cannot return 0 as ProDG for gamecube generates a warning
      // this function is basically used to prevent operator new[] from being used.
      //
      RWS_RETURN(reinterpret_cast<void*>(-1));
   }

 /**
   *
   * delete - deletes objects using the RenderWare free lists.
   *
   */

   void CLinkedMsg::operator delete(void *pObj)
   {
      // new stops non-standard sized allocs, so should be safe to just free here.

      if (pObj)
      {
         RwFreeListFree(pFreeList, pObj);
      }
   }

   void CLinkedMsg::operator delete[](void *pObj)
   {
      // NOTE: array allocation is not allowed for this type. Should not be needed. Using an array would
      //       preclude the use of RwFreeList and therefore slow the system.

      RWS_FUNCTION("RWS::CLinkedMsg::operator delete[]");
      RWS_ASSERT(0, "Cannot deallocate arrays for this type");
      RWS_RETURNVOID();
   }

 /***********************************
   *
   * CRegisteredMsgs class functions.
   *
   **********************************/

   RwFreeList *CRegisteredMsgs::pFreeList = 0;
   const RwUInt32 CREGISTERED_MSGS_DEFAULT_BLOCK_SIZE = 10;

 /**
   *
   * \ref CRegisteredMsgs InitMemHandler static function to initialize the memory handling system.
   *
   * \param blockSize This is the initial and increment block size for memory allocation. Zero = default.
   */

   void CRegisteredMsgs::InitMemHandler(RwUInt32 blockSize)
   {
      RWS_FUNCTION("RWS::CRegisteredMsgs::InitMemHandler");

      if (!blockSize)
      {
         blockSize = CREGISTERED_MSGS_DEFAULT_BLOCK_SIZE;
      }

      pFreeList = RwFreeListCreate(sizeof(CRegisteredMsgs), blockSize, 16, rwMEMHINTDUR_GLOBAL);
      RWS_ASSERT(pFreeList, "failed to allocate free list");

      RWS_RETURNVOID();
   }

 /**
   *
   * \ref CLinkedMsg PurgeMem static function to purge unused memory within system (large overhead).
   *
   */

   void CRegisteredMsgs::PurgeMem(void)
   {
      if (pFreeList)
      {
         RwFreeListPurge(pFreeList);
      }
   }

#ifndef NDEBUG
   void CRegisteredMsgs::CheckFreeListEmptyCallBack(void *pMem, void *pData) 
   {
      RWS_FUNCTION("RWS::CRegisteredMsgs::CheckFreeListEmptyCallBack");

      CRegisteredMsgs *pRMsg = reinterpret_cast<CRegisteredMsgs*>(pMem);

      RWS_ASSERTFAIL("The following event hasn't been UnRegistered before calling"
         " CRegisteredMsgs::CloseMemHandler [" << RWS_VALIDSTRING(pRMsg->p_msgname) << "]");

      RWS_RETURNVOID();
   }
#endif

 /**
   *
   * \ref CRegisteredMsgs CloseMemHandler static function to close the memory handling system.
   *
   */

   void CRegisteredMsgs::CloseMemHandler(void)
   {
      RWS_FUNCTION("RWS::CRegisteredMsgs::CloseMemHandler");

      if (pFreeList)
      {
#ifndef NDEBUG
         RwFreeListForAllUsed  ( pFreeList, CheckFreeListEmptyCallBack, 0);
#endif

         RwFreeListDestroy(pFreeList);
         pFreeList = 0;
      }

      RWS_RETURNVOID();
   }


 /**
   *
   * new - allocates new objects using the RenderWare free lists. Can only allocate base type objects.
   *
   */

   void *CRegisteredMsgs::operator new(size_t size)
   {
      RWS_FUNCTION("RWS::CRegisteredMsgs::operator new");

      // NOTE: Cannot allow classes to derive from this class as it uses an RwFreeList for the memory
      //       handling. This requires objects of a FIXED size, therefore derived classes of a different
      //       size would cause memory overruns, etc.

      RWS_ASSERT(size == sizeof(CRegisteredMsgs), "Cannot derive from class, no support due to RwFreeList");
      RWS_ASSERT(pFreeList, "System not initialized, need to call 'InitMemHandler'");

      // Get a new block from the RwFreeList

      RWS_RETURN(RwFreeListAlloc(pFreeList, rwMEMHINTDUR_GLOBAL));
   }

   void *CRegisteredMsgs::operator new[](size_t size)
   {
      // NOTE: array allocation is not allowed for this type. Should not be needed. Using an array would
      //       preclude the use of RwFreeList and therefore slow the system.

      RWS_FUNCTION("RWS::CRegisteredMsgs::operator new[]");
      RWS_ASSERT(0, "Cannot allocate arrays for this type");
      // Note cannot return 0 as ProDG for gamecube generates a warning
      // this function is basically used to prevent operator new[] from being used.
      //
      RWS_RETURN(reinterpret_cast<void*>(-1));
   }

 /**
   *
   * delete - deletes objects using the RenderWare free lists.
   *
   */

   void CRegisteredMsgs::operator delete(void *pObj)
   {
      RWS_FUNCTION("RWS::CRegisteredMsgs::operator delete");

      // new stops non-standard sized allocs, so should be safe to just free here.

      if (pObj)
      {
         RwFreeListFree(pFreeList, pObj);
      }

      RWS_RETURNVOID();
   }

   void CRegisteredMsgs::operator delete[](void *pObj)
   {
      // NOTE: array allocation is not allowed for this type. Should not be needed. Using an array would
      //       preclude the use of RwFreeList and therefore slow the system.

      RWS_FUNCTION("RWS::CRegisteredMsgs::operator delete[]");
      RWS_ASSERTFAIL("Cannot deallocate arrays for this type");
      RWS_RETURNVOID();
   }

}//namespace RWS


namespace RWS
{
   namespace RpAtomicToEventHandler
   {
      /**
      *
      *  \ingroup RpAtomicToEventHandlerPlugin
      *
      *  \page RpAtomicToEventHandlerPluginOverview Overview
      *
      *  RpAtomicToEventHandlerPlugin is a RenderWare Graphics plug-in that
      *  adds an extra data field to a RpAtomic to allow an event handler
      *  (a behavior that inherits from CEventHandler) to be associated
      *  with it.
      *
      *  For example, if a collision is detected between a RpAtomic and
      *  a RpWorld, the behavior associated with the RpAtomic can be found
      *  via RWS::RpAtomicToEventHandler::GetEventHandler and a message 
      *  sent directly to it to react to the collision.
      *
      */
      namespace
      {
         /**
         *  \ingroup RpAtomicToEventHandlerPlugin
         *
         *  The extra data to be attached and managed by the plug-in
         */
         struct PluginData
         {
            RWS::CEventHandler* m_pEventHandler;
         };

         /**
         *  \ingroup RpAtomicToEventHandlerPlugin
         *
         *  Offset of extra data within RpAtomic
         */
         RwUInt32 iPlugin_Offset = 0;

         /**
         *  \ingroup RpAtomicToEventHandlerPlugin
         *
         *  Retrieves Atomic Plug-in data.
         *
         *  \param atomic A reference to an RpAtomic object from which to extract the plug-in data.
         *
         *  \return A pointer to the RpAtomicToEventHandlerPluginData data.
         */
         inline PluginData* GetPluginData( RpAtomic& atomic )
         {
            RWS_FUNCTION("RWS::RpAtomicToEventHandler::NULL::GetPluginData");

            RWS_PRE(iPlugin_Offset);

            const RwUInt32 baseAddr = reinterpret_cast<RwUInt32>(&atomic);
            const RwUInt32 pluginAddr = baseAddr + iPlugin_Offset;

            RWS_RETURN(reinterpret_cast<PluginData*>(pluginAddr));
         }

         /**
         *  \ingroup RpAtomicToEventHandlerPlugin
         *
         *  Retrieves Atomic Plug-in data, const version.
         *
         *  \param atomic A reference to an RpAtomic object from which to extract the plug-in data.
         *
         *  \return A pointer to the RpAtomicToEventHandlerPluginData data.
         */
         inline const PluginData *GetPluginData( const RpAtomic& atomic )
         {
            RWS_FUNCTION("RWS::RpAtomicToEventHandler::NULL::GetPluginData");

            RWS_PRE(iPlugin_Offset);

            const RwUInt32 baseAddr = reinterpret_cast<RwUInt32>(&atomic);
            const RwUInt32 pluginAddr = baseAddr + iPlugin_Offset;

            RWS_RETURN(reinterpret_cast<const PluginData*>(pluginAddr));
         }

         /**
         *  \ingroup RpAtomicToEventHandlerPlugin
         *
         *  Constructor for RpAtomicToEventHandlerPlugin data.
         *
         *  \param pObject A pointer to the object to be constructed, the RpAtomic.
         *  \param offsetInObject A byte offset from object pointer to plug-in data.
         *  \param sizeInObject The size of the plug-in data.
         *
         *  \return Returns pointer to the constructed object.
         */
         void* Plugin_Constructor(void* pObject, RwInt32 offsetInObject, RwInt32 sizeInObject)
         {
            RWS_FUNCTION("RWS::RpAtomicToEventHandler::NULL::Plugin_Constructor");

            RWS_PRE(pObject);

            PluginData& pluginData = *GetPluginData(*reinterpret_cast<RpAtomic*>(pObject));
            pluginData.m_pEventHandler = 0;

            RWS_RETURN(pObject);
         }

         /**
         *  \ingroup RpAtomicToEventHandlerPlugin
         *
         *  Copy operator for RpAtomicToEventHandlerPlugin.
         *
         *  \param pDstObject A void pointer to the destination object, the source RpAtomic.
         *  \param pSrcObject A void pointer to the destination object, the destination RpAtomic.
         *  \param offset The byte offset from object pointers to plug-in data.
         *  \param size The size of plug-in data to copy.
         */
         void* Plugin_Copy(void* pDstObject, const void* pSrcObject, RwInt32 offset, RwInt32 size)
         {
            RWS_FUNCTION("RWS::RpAtomicToEventHandler::NULL::Plugin_Copy");

            RWS_PRE(pDstObject);
            RWS_PRE(pSrcObject);

            PluginData& srcData = *GetPluginData(
               *reinterpret_cast<RpAtomic*>(const_cast<void*>(pSrcObject)));

            PluginData& dstData = *GetPluginData(*reinterpret_cast<RpAtomic*>(pDstObject));

            dstData = srcData;

            RWS_RETURN(pDstObject);
         }

      } // namespace <anonymous>

      /**
      *  \ingroup RpAtomicToEventHandlerPlugin
      *
      *  Unique ID of the RpAtomicToEventHandlerPlugin used by RenderWare to identify the plug-in
      *
      */
      const RwUInt32 PluginID = 0xFF;

      /**
      *  \ingroup RpAtomicToEventHandlerPlugin
      *
      *  Attaches the CEventHandler-RpAtomic association plug-in - this must 
      *  happen AFTER the RenderWare engine is
      *  initialized but BEFORE it is opened. Returns FALSE if the plug-in fails to
      *  attach.
      *
      *  \return TRUE if successful otherwise FALSE.
      */
      RwBool PluginAttach(void)
      {
         RWS_FUNCTION("RWS::RpAtomicToEventHandler::PluginAttach");

         iPlugin_Offset = RpAtomicRegisterPlugin(
            sizeof(PluginData),
            MAKECHUNKID(rwVENDORID_CRITERIONRM, PluginID),
            Plugin_Constructor,  
            0, // NOT USED
            Plugin_Copy
            );

         RWS_POST(iPlugin_Offset);

         RWS_RETURN (0 != iPlugin_Offset);
      }

      /**
      *  \ingroup RpAtomicToEventHandlerPlugin
      *
      *  Sets the event handler to be associated with the atomic.
      *
      *  \param atomic A reference to an RpAtomic object.
      *  \param pEventHandler A pointer to a CEventHandler object.
      */
      void SetEventHandler( RpAtomic& atomic, CEventHandler* pEventHandler )
      {
         RWS_FUNCTION("RWS::RpAtomicToEventHandler::SetEventHandler");

         RWS_PRE( iPlugin_Offset );

         GetPluginData(atomic)->m_pEventHandler = pEventHandler;

         RWS_RETURNVOID();
      }

      /**
      *  \ingroup RpAtomicToEventHandlerPlugin
      *
      *  Retrieves the event handler associated with the atomic
      *
      *  \param atomic A reference to an RpAtomic object.
      *
      *  \return A pointer to the CEventHandler that manages this atomic, otherwise 0.
      */
      CEventHandler* GetEventHandler( const RpAtomic& atomic )
      {
         RWS_FUNCTION("RWS::RpAtomicToEventHandler::GetEventHandler");

         RWS_PRE( iPlugin_Offset );
      
         RWS_RETURN(GetPluginData(atomic)->m_pEventHandler);
      }
   } // namespace RpAtomicToEventHandler
}// namespace RWS
