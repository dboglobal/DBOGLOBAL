/*****************************************************************************
*
* File :     CEventHandler.h
*
* Abstract :
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

/**
*
*  \ingroup CEventHandler
*
*  \page CEventHandlerOverview Overview
*
*  A common approach to games development is the decomposition of the game into a collection
*  of inter-communicating entities or processes. This decomposition is typically reflected
*  in the way that the code is structured, and in how the games designer combines these
*  entities in order to create the game.
*
*  There are many methods that a programmer can use to implement the communication between
*  these game components. The specific method chosen is typically left to the individual
*  programming team or programmer and frequently differs between teams, programmers and
*  software development companies. This means that developers may find themselves unable
*  to share or re-use components between projects, and are forced to repeatedly re-write
*  components every time they begin a new project.
*
*  As part of RenderWare Studio, the console framework provides a formalized method for performing
*  communication between entities, thereby enabling greater re-use and sharing of game
*  components between teams and projects.
*  In addition to this RenderWare Studio provides genre specific modules providing behaviors common
*  to certain game styles, for example action, driving, sports or real-time strategy.
*  This will allow the games developer to concentrate on the areas that make their game
*  unique rather than having to re-invent the proverbial wheel for every game.
*
*  The event handler forms one of the core components of the console framework from which
*  a finished game can be developed. All new behaviors inherit from the CEventHandler
*  class, which provides the base methods for sending, receiving and processing events.
*
*/

/**
*
*  \ingroup CEventHandler
*
*  \page CEventHandlerKeyFeatures Key Features
*
*  The event handler component provides the following key features:
*
*  \li An efficient, formalized method for behaviors to send and receive events.
*
*  \li A well defined order for behaviors to process events.
*
*  \li The ability to create and destroy events at run-time; the RenderWare Studio
*  Editor enables the game designer to name events and specify its senders and recipients.
*
*  \li A many to many relationship for communication between behaviors, i.e. there
*  can be many recipients of a particular event and similarly many senders.
*
*  The following classes, CEventHandler, CEventId, CMsg provide the interface to the event
*  handler system, with CLinkedMsg, and CRegisteredMsg being used internally.
*
*/

/**
*  \ingroup CEventHandler
*  \page CEventHandlerEvents System Events
*  \li iMsgDeleteEventHandler
*  Event handlers can link to this event to allow themselves to be deleted
*  when CEventHandler::DeleteEventHandlers (iMsgDeleteEventHandler) is called.
*  \li iMsgOnDeleteEntity
*  This event is sent when an event handler is deleted,
*  some event handlers may keep pointers to other event handlers, this event
*  can be used to tell them that the entity they are tracking has been 
*  deleted.
*  \li iMsgDeleteEntity
*  This message is sent at the end of each game frame to delete entities
*  this is a safe method of deleting an entity. Deleting an entity from
*  within an entity could result in undesirable results due to the code
*  returning to the same entity via a return
*
*  i.e. This is unsafe as after processing MyEventB it returns to the code handling
*  the MyEventA.
*
*  \verbatim

     void HandleEvents(CMsg &pMsg)
     {
      if(m_p_Msg->Id == MyEventA) 
      {
         SendMsg(MyEventB);
         . 
         .
         . (this) has been deleted so following code may crash if any reference to 
         . to class is made.
      }
      else if(m_p_Msg->Id == MyEventB)
      {
         delete this;
      }
   }

   \endverbatim
*
*  The safe way to do this is...
*
*  \verbatim

   void HandleEvents(CMsg &pMsg)
   {
      if(m_p_Msg->Id == MyEventA) 
      {
         SendMsg(MyEventB);
         . 
         .
         . (this) will not be deleted until the end of the current game frame
         . by calling CEventHandler::DeleteEventHandlers(iMsgDeleteEntity);
      }
      else if(m_p_Msg->Id == MyEventB)
      {
         Delete();
      }
   }

   \endverbatim
*
*
*/


/**
*
*  \ingroup CEventHandler
*
*  \page CEventHandlerProblems Known Problems
*
*  1) If an event handler contains a CEventId which references a global event i.e. iMsgRunningTick
*  then it is possible for the lifetime management of the event to become corrupt generating an assert.
*  "CEventId::~CEventId [EventName] not unlinked" for example...
*
*  \verbatim 

    CMyClass::CMyClass(const CAttributePacket& attr)
    {
        LinkMsg(iMsgRunningTick);   
        UnLinkMsg(iMsgRunningTick); // Not linked to iMsgRunningTick

        RegisterMsg(m_Event,iMsgRunningTickStr, "RwUInt32"); // i.e. m_Event == iMsgRunningTick

        LinkMsg(m_Event);   
    }
    
    CMyClass::~CMyClass(void)
    {
        UnLinkMsg(iMsgRunningTick); // Thinks CMyClass is linked to iMsgRunningTick due to LinkMsg(m_Event); 
                                    //because m_Event == iMsgRunningTick

        UnLinkMsg(m_Event);

        UnRegisterMsg(m_Event); // Will generate an assert as m_Event linked ref-count will equal 1
    }

    \endverbatim 
*
*   The solution is to unlink from the global CEventId iMsgRunningTick last i.e.
*
*   \verbatim 

    CMyClass::~CMyClass(void)
    {
        UnLinkMsg(m_Event); // if m_Event was linked, CMyClass will unlink from iMsgRunningTick

        UnLinkMsg(iMsgRunningTick); // if CMyClass is linked to iMsgRunningTick will unlink

        UnRegisterMsg(m_Event);
    }

    \endverbatim 
*
*   This has not been fixed internally within the event handler system as it would generate
*   a considerable memory/efficiency hit.
*
*/

#ifndef __CEVENTHANDLER_H__
#define __CEVENTHANDLER_H__

#ifdef DOXYGEN

/**
*
*  \ingroup Forms
* 
*  Define an event, this macro can be used to expose events defined within the 
*  console source code to the RenderWare Studio Workspace.
*
*  \li string is a unique string used to register the event
*
*  \li type is the second parameter passed to Register event and specifies the 
*      type of the message
*
*  \li desc text description of the event
*
*  Example 
*
*  \verbatim

   RWS_DEFINE_EVENT(iMsgDoRender ,0,"Sent each frame to begin rendering.")
   
   \endverbatim
*
*/
#define RWS_DEFINE_EVENT(name, type, desc)

#else

#define RWS_DEFINE_EVENT(name, type, desc) const char * const name##Str = #name; extern CEventId name

#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//

#include <rwcore.h>
#include <rpworld.h>
#include "ceventvisualisation.h"

struct RpAtomic;  // forward reference to the RpAtomic struct, for the RpCEventHandlerAtomicPlugin functions.

namespace RWS
{
   /**
   *
   *  \ingroup CEventHandler
   *
   *  This class is used by the event handler system to maintain a list of event handlers.
   *
   */

   class CLinkedMsg
   {
      public:

         /**
         *
         * \ref CLinkedMsg default constructor of the CLinkedMsg class.
         *
         */

         CLinkedMsg(void) : 
            p_next(0),
            p_prev(0),
            pCEventHandler(0),
            iCountLinked(1),
            m_priority(0xffff)
         {
         }

         /**
         *
         * \ref CLinkedMsg This is an alternative constructor of the CLinkedMsg class, enabling the client
         * to specify the CEventHandler and the priority of the link.
         *
         * \param tpCEventHandler A pointer to the CEventHandler to be linked to the event.
         *
         * \param tpriority The priority of the link, determines the calling of HandleEvents when an
         * event is sent.
         *
         */

         CLinkedMsg(
                  class CEventHandler *tpCEventHandler,
                  RwUInt16 tpriority) :
            p_next(0),
            p_prev(0),
            pCEventHandler(tpCEventHandler),
            iCountLinked(1),
            m_priority(tpriority)
         {
         }

         CLinkedMsg *p_next;                       /**<  Pointer to next registered message, CLinkedMsg  */
         CLinkedMsg *p_prev;                       /**<  Pointer to prev registered message, CLinkedMsg  */

         class CEventHandler *pCEventHandler;      /**<  Pointer to an event handler which will
                                                         respond to this event/message   */
      
         RwInt32 iCountLinked;                     /**<  Count of the number of times a
                                                         \ref CEventHandler has been linked
                                                         to this event  */

         RwUInt16 m_priority;                      /**<  Priority of the link maintained by this CLinkedMsg
                                                         0xffff highest 0x0000 lowest, the priority of the
                                                         CLinkedMsg determines the order that event handlers
                                                         \ref CEventHandler are called.*/


         void *operator new(size_t size);
         void *operator new[](size_t size);
         void operator delete(void *pObj);
         void operator delete[](void *pObj);

         static void InitMemHandler(RwUInt32);
         static void PurgeMem(void);
         static void CloseMemHandler(void);

      private:

         static RwFreeList *pFreeList;

#ifndef NDEBUG
         static void CheckFreeListEmptyCallBack(void *pMem, void *pData);
#endif
   } ;

   /**
   *
   *  \ingroup CEventHandler
   *
   *  This class is used internally by the event handler system to maintain a link between a
   *  registered event and a list of event handlers which have linked to that event. 
   *
   */

   class CRegisteredMsgs
   {
   public:

      /**
      *
      * \ref CRegisteredMsgs Default constructor of the RegisteredMsgs class.
      *
      */

      CRegisteredMsgs(void) : 
         p_msgname(0),
         p_format(0),
         p_next(0),
         p_prev(0),
         iCountRegistered(0),
         PendingUnlink(false),
         HandlingEvent(false)
      {
      }

      CLinkedMsg  Erm;       /**< Start of the list of CLinkedMsg's   */

      RwChar *p_msgname;                        /**< Pointer to a string description of this event.   */
      RwChar *p_format;                         /**< Pointer to the string description of
                                                     the format of this event.  */

      RwUInt32 ui_msgname;                      /**< Unique Id, generated from p_msgname */
      RwUInt32 ui_format;                       /**< Unique Id, generated from p_format */

      CRegisteredMsgs *p_next;                  /**< Pointer to the next CRegisteredMsgs  */
      CRegisteredMsgs *p_prev;                  /**< Pointer to the previous CRegisteredMsgs */

      RwInt32 iCountRegistered;                 /**< A reference count, the number of times this
                                                   event has been registered.   */

      void Inc_registered_count(void) {iCountRegistered++;}
      void Dec_registered_count(void) {iCountRegistered--;}

      RwBool PendingUnlink;                    /**< Flag indicating that this event has pending unlink's.  */
      RwBool HandlingEvent;                    /**< Flag indicating that this event is 
                                                    currently being processed */

      void *operator new(size_t size);
      void *operator new[](size_t size);
      void operator delete(void *pObj);
      void operator delete[](void *pObj);

      static void InitMemHandler(RwUInt32);
      static void PurgeMem(void);
      static void CloseMemHandler(void);

   private:

      static RwFreeList *pFreeList;

#ifndef NDEBUG
      static void CheckFreeListEmptyCallBack(void *pMem, void *pData);
#endif
   } ;

   /**
   *
   *  \ingroup CEventHandler
   *
   *  This class is used to store the clients reference to an event. 
   *  The CEventId class provides useful debugging tools, alerting the 
   *  client if they fail to unlink or unregister an event correctly.
   *
   */

   class CEventId
   {
      public:
         CEventId(void);

         ~CEventId(void);

         CEventId &operator= (const CEventId &rhs);   
         
         CEventId &operator= (const CRegisteredMsgs *rhs);  
         
         /**   
         *
         *  Typecast operator, 
         *
         *  \verbatim

               CEventId Id;
            
               CRegisteredMsgs *p_gfRMsg = Id;
         
            \endverbatim
         *
         */
         
         operator CRegisteredMsgs*() const
         {
            return const_cast<CRegisteredMsgs*>(pEventId);
         }

         /**
         *
         *  Equality operator, returns true if specified event is the same
         *
         *  \verbatim

            CEventId E1,E2,E3;

            RegisterMsg(E1,"MyEvent",0);

            RegisterMsg(E2,"MyEvent",0);

            RegisterMsg(E3,"OtherEvent",0);
            
            if ( E1 == E2 )
            {
               .
               .
                  Runs as E1 does equal E2
               .
               .
            }

            if ( E1 == E3)
            {
               .
               .
                  Never runs as E1 does not equal E3
               .
               .
            }

            \endverbatim
         *
         */
         
         inline RwBool operator==(const CEventId &rhs) const
         {
            return (pEventId == rhs.pEventId);
         }

         /**
         *
         *  Inequality operator, returns false if specified event is the same
         *
         *  \verbatim

            CEventId E1,E2,E3;

            RegisterMsg(E1,"MyEvent",0);

            RegisterMsg(E2,"MyEvent",0);

            RegisterMsg(E3,"OtherEvent",0);
            
            if ( E1 != E2 )
            {
               .
               .
                  Never runs as E1 does equal E2
               .
               .
            }

            if ( E1 != E3)
            {
               .
               .
                  Runs as E1 does not equal E3
               .
               .
            }

            \endverbatim
         *
         */

         inline RwBool operator!=(const CEventId &rhs) const
         {
            return (pEventId != rhs.pEventId);
         }

         /**
         *
         *  Pointer operator 
         *
         */

         const CRegisteredMsgs *operator->() const {return pEventId;}

         void Inc_registered_count(void);
         void Dec_registered_count(void);

         /**   \return The number of times (this) CEventId has been registered,
               \ref CEventHandler::RegisterMsg  */

         RwInt32 Get_registered_count(void) const {return m_registered_count;}

         void Inc_linked_count(void);
         void Dec_linked_count(void);

         /**   \return  The number of times (this) CEventId has been linked,
               \ref CEventHandler::LinkMsg    */

         RwInt32 Get_linked_count(void) const {return m_linked_count;}

         /**   \return The pointer to the CRegisteredMsgs specified by (this) CEventid */

         const CRegisteredMsgs *Get_pEventId(void) const {return pEventId;}

        /**
         *
         * \return A pointer to a string containing the name of this event.
         */
         const char *GetName(void) {
            const CRegisteredMsgs *pRMsg = this->Get_pEventId();
            if(!pRMsg) return 0;
            return pRMsg->p_msgname;
         };

         /**
         *
         * \return A pointer to a string containing the format of this event.
         */
         const char *GetFormat(void) {
            const CRegisteredMsgs *pRMsg = this->Get_pEventId();
            if(!pRMsg) return 0;
            return pRMsg->p_format;
         };

      private:

         CRegisteredMsgs *pEventId;                   /**< A pointer to the registered event/message.*/

         short m_linked_count;                        /**<  A reference counter, the number of times this
                                                            CEventId has been linked. A CEventId can be
                                                            linked many times to one or more event 
                                                            handlers this counter is used to check that 
                                                            CEventHandler::UnLink has been called the 
                                                            correct number of times.*/

         short m_registered_count;                    /**< A reference counter, the number of times this
                                                           <c CEventId> has been registered. A CEventId
                                                           can be registered many times to one or more
                                                           event handlers, <c CEventHandler> this counter
                                                           is used to check that the correct number of
                                                           <mf CEventHandler::UnRegister> operations have
                                                           been performed for this event.*/

         CEventId(const CEventId& rhs);               /**< Disable the copy constructor.  */
   };

   const CEventId ERROR_NOT_A_VALID_MESSAGE_ID;

   /**
   *
   *  \ingroup CEventHandler
   *
   *  This class is used to define an event.
   *
   *  \note An event may be used to transfer data in which case it is referred to as a message. The event
   *  handler treats both events and messages in the same way.
   *
   *  The following example illustrates several different methods of generating an event/message and also
   *  how SendMsg can be used to send the events to other event handlers.
   *
   *  \verbatim

         void CMyClass::MyFunc(void)
         {
            CEventId Id;

            RegisterMsg (Id, "MyEvent", "RwUInt32");

            // -------------------------------------------

            CMsg Msg1;         // CMsg.Id = Undefined , CMsg.pData = Undefined

            Msg1.Id = Id;
            Msg1.pData = 0;

            SendMsg (Msg1);

            // -------------------------------------------

            CMsg Msg2(Id);     // CMsg.Id = Id , CMsg.pData = 0

            SendMsg (Msg2);

            // -------------------------------------------

            CMsg Msg2(Id,50);  // CMsg.Id = Id , CMsg.pData = 50

            SendMsg (Msg3);

            UnRegister (Id);
         }

      \endverbatim
   */

   class CMsg
   {
   public:

      /**
      *
      *  Default constructor for the CMsg class.
      *
      */

      CMsg(void) {}

      /**
      *
      *  Alternate constructor for the \ref CMsg class enabling a
      *  client to specify the event \ref CEventId.
      *
      *  \param tId A reference to a \ref CEventId specifying the event, copied into the \ref Id field.
      *
      *  \note pData field is cleared to 0
      *
      */

      CMsg(
            const CEventId &tId
         ) : 
         Id(tId),
         pData(0)
      {}

      /**
      *
      *  Alternate constructor for the \ref CMsg class
      *  allowing the client to specify both the event \ref CEventId and the associated data.
      *
      *  \param tId A reference to a \ref CEventId specifying the event, copied into the Id field.
      *
      *  \param tpData The data associated with this message, copied into \ref pData
      *
      */

      CMsg(
          const CEventId &tId,
          void *tpData
          ) :     
         Id(tId),
         pData(tpData)
      {}

      CRegisteredMsgs *Id;     /**< The Id of the event. */

      void *pData;             /**< Pointer to message data 0 if no data specified. */
   };

  /**
   *  \ingroup SystemEvents
   *  \page EventSystemEvents Event System Events
   *  \li iMsgDeleteEventHandler
   *  This event is used to keep a list of CEventHandlers that have registered
   *  themselves to be automatically deleted. All of the event handlers linked to
   *  this event can be deleted by calling CEventHandler::DeleteEventHandlers (iMsgDeleteEventHandler).
   *
   *  \see CEventHandler::UnRegisterForAutoDelete
   *  \see CEventHandler::RegisterForAutoDelete
   *  
   */
   RWS_DEFINE_EVENT( iMsgDeleteEventHandler,
                     0,
                     "This event is used to keep a list of CEventHandlers that have registered"
                     "themselves to be automatically deleted.");

  /**
   *  \ingroup SystemEvents
   *  \page EventSystemEvents Event System Events
   *  \li iMsgDeleteEntity
   *  When \ref CEventHandler::Delete is called the CEventHandler is linked to iMsgDeleteEntity this allows
   *  the event system to maintain a list of deleted event handlers that can be destroyed at a safe point in
   *  the programs execution, typically a point in the main loop where no events are being processed.
   */
   RWS_DEFINE_EVENT( iMsgDeleteEntity,
                     0,
                     "See documentation for CEventHandler::Delete().");

  /**
   *  \ingroup SystemEvents
   *  \page EventSystemEvents Event System Events
   *  \li iMsgOnDeleteEntity
   *  When a CEventHandler is destroyed this event is broadcast allowing other CEventHandlers to detect that
   *  the specified CEventHandler is about to be destroyed. This is useful is CEventHandlers maintain
   *  pointers to other CEventHandlers.
   */
   RWS_DEFINE_EVENT( iMsgOnDeleteEntity,
                     "CEventHandler*",
                     "See documentation for CEventHandler::~CEventHandler.");

#if defined RWS_EVENTVISUALIZATION
   /**
   *
   *  This class is used to encapsulate the data associated with an event and pass it to the
   *  event visualization system.
   *
   */
   class EventData
   {
      public:

         EventData(CEventHandler *_pSrc, CEventHandler *_pDst, CRegisteredMsgs *_pRegisteredEvent) :
            m_pSrc(_pSrc),
            m_pDst(_pDst),
            m_pRegisteredEvent(_pRegisteredEvent)
         {};

         ~EventData() {};

         CEventHandler *m_pSrc; /**< Pointer to the source of the event, if the event doesn't
                                     have an event handler as its source this will be 0.*/

         CEventHandler *m_pDst; /**< The recipient of the event.*/

         CRegisteredMsgs *m_pRegisteredEvent; /**< The registered event.*/
   };

  /**
   *  \ingroup SystemEvents
   *  \page EventSystemEvents Event Visualization System Events
   *  \li iMsgEventSend
   *
   *  Sent by the event system when an event is fired, i.e. in response to SendMsg
   *  and SendMsgToEventHandler.
   */
   RWS_DEFINE_EVENT( iMsgEventSend,
                     "CRegisteredMsgs*",
                     "Sent by the event system when an event is fired, i.e. in response to SendMsg"
                     " and SendMsgToEventHandler.");
   /**
   *  \ingroup SystemEvents
   *  \page EventSystemEvents Event Visualization System Events
   *  \li iMsgEventReceived
   *
   *  Sent by the event system for each recipient of an event when an event is fired,
   *  i.e. in response to SendMsg and SendMsgToEventHandler.
   */
   RWS_DEFINE_EVENT( iMsgEventReceived,
                     "EventData*",
                     "Sent by the event system for each recipient of an event when an event is fired,"
                     " i.e. in response to SendMsg and SendMsgToEventHandler.");

   /**
   *  \ingroup SystemEvents
   *  \page EventSystemEvents Event Visualization System Events
   *  \li iMsgEventSent
   *
   *  Sent by the event system when an event has been fired, i.e. in response to SendMsg
   *  and SendMsgToEventHandler.
   */
   RWS_DEFINE_EVENT( iMsgEventSent,
                     "CRegisteredMsgs*",
                     "Sent by the event system when an event has been fired, i.e. in response to SendMsg"
                     " and SendMsgToEventHandler.");

   /**
   *  \ingroup SystemEvents
   *  \page EventSystemEvents Event Visualization System Events
   *  \li iMsgEventUnRegistered
   *
   *  Sent by the event system when a registered message is completely unregistered.
   */
   RWS_DEFINE_EVENT( iMsgEventUnRegistered,
                     "CRegisteredMsgs*",
                     "Sent by the event system when a registered message is completely unregistered.");

#endif

   extern RwBool _SendMsg(CMsg &p_Msg);

   /**
   *
   *  \ref SendMsg Send the specified event \ref CMsg to each of the event handlers /CEventHandler
   *  linked to the specified event, this is a performance critical function and is therefore partially 
   *  implemented inline.
   *
   *  \note Defining RWS_EVENTVISUALIZATION modifies this function to enable event Visualization.
   *
   */

   __inline RwBool SendMsg(CMsg &p_Msg)
   {
      if (p_Msg.Id != ERROR_NOT_A_VALID_MESSAGE_ID && p_Msg.Id->Erm.p_next)
      {
         return _SendMsg(p_Msg);
      }
      else
      {
         return false;
      }
   }

   /**
   *
   *  \ref SendMsg Send the specified event \ref CEventId to each of the event handlers /CEventHandler
   *  linked to the specified event, this is a performance critical function and is therefore partially 
   *  implemented inline.
   *
   *  \note Defining RWS_EVENTVISUALIZATION modifies this function to enable event Visualization.
   *
   */

   __inline RwBool SendMsg(const CEventId &Id)
   {
      if (Id != ERROR_NOT_A_VALID_MESSAGE_ID && Id->Erm.p_next)
      {
         CMsg iMsg(Id,0);

         return _SendMsg (iMsg);
      }
      else
      {
         return false;
      }
   }

   extern void SendMsgToEventHandler(CMsg &p_Msg, CEventHandler *pCEventHandler);
   extern void SendMsgToEventHandler(const CEventId &Id, CEventHandler *pCEventHandler);

   /**
   *
   *  \ingroup CEventHandler
   *
   *  This is the abstract base class which defines the interface to the event handling
   *  system. A client wishing to make use of the event handling system should publicly
   *  derive from this class as shown below:
   *
   *  \verbatim

      #include "ceventhandler.h"
   
      class MyClass : public CEventHandler
      {
      }
   
      \endverbatim
   *
   */

   class CEventHandler
   {
   private:

	  // Prevent EventHandlers being created before event sys had been opened
      //
      static RwUInt32 refCountEventHandlersCreated;

   public:

      /**
      *
      *  Construct an instance of the CEventHandler class. Clients of CEventHandler should not call the
      *  constructor directly but by using the \a InitCEventHandler macro, this is because the parameters
      *  to the constructor change when \a RWS_EVENTVISUALIZATION is defined, the \a InitCEventHandler takes
      *  care of this.
      *
      *  \param pCEventVisualization A pointer to the CEventVisualization class which defines an interface
      *  which if supported provides the 3D position of the event handler which is used by the event
      *  Visualization system.
      *
      *  \note Event Visualization is only enabled when RWS_EVENTVISUALIZATION is defined.
      *
      *  Example showing the difference between using CEventHandler, and InitCEventHandler
      *
      *  \verbatim

         class DisplayDebugTools : public CEventHandler
         {
            public:

              virtual void HandleEvents(CMsg &pMsg);

              DisplayDebugTools(void) : InitCEventHandler(m_pEntity) {}

              // using the constructor directly would require RWS_EVENTVISUALIZATION to be handled i.e.
              //
#ifdef RWS_EVENTVISUALIZATION
              DisplayDebugTools(void) : CEventHandler(m_pEntity) {}
#else
              DisplayDebugTools(void) : CEventHandler() {}
#endif

              ~DisplayDebugTools(void);

            protected:

               C3DRwEntityPtr m_pEntity;  // Note: C3DRwEntity is derived from CEventVisualization
         };
      
         \endverbatim
      *
      *  \note If the event handler doesn't provide the CEventVisualization interface initialize the
      *  event handler with 0 as the parameter, i.e. InitCEventHandler(0).
      */

#ifdef RWS_EVENTVISUALIZATION
      CEventHandler(CEventVisualization *pCEventVisualization) : 
         m_pCEventVisualization(pCEventVisualization), m_Active(true)
      {
#ifndef NDEBUG
         refCountEventHandlersCreated++;
#endif
      };

      /**
      *
      */

      #define InitCEventHandler(a) CEventHandler(a)

#else
      CEventHandler() : m_Active(true)
      {
#ifndef NDEBUG
         refCountEventHandlersCreated++;
#endif
      };

      /**
      *
      */

      #define InitCEventHandler(a) CEventHandler()
#endif

      virtual ~CEventHandler(void);
      
      static void Open(RwUInt32 linkBlockSize = 0, RwUInt32 regBlockSize = 0);
      static void Close(void);

      static void RegisterStreamChunkHandlers(void);

      static void UnRegisterStreamChunkHandlers(void);

      /**
      *
      * Purge - purges unused memory within the event system. Call can be slow.
      *
      */

      static void Purge(void)
      {
         // Purges sub-objects which use 'RwFreeList'.
         
         CLinkedMsg::PurgeMem();
         CRegisteredMsgs::PurgeMem();
      }


      /**
      *
      *  Translate and process messages for this event handler \ref CEventHandler, this is how event
      *  handlers receive an event,
      *
      *  \param pMsg A reference to an event \ref CMsg, this is the event that caused HandleEvents
      *  to be called.
      *
      *  \note calling BaseClass::HandleEvents will pass on the event to the baseclass, this can
      *  be used to extend the behavior of previously define event handler.
      *
      */

      virtual void HandleEvents(CMsg &pMsg) = 0;

      static void UnRegisterMsg(CEventId &Id);

      static void RegisterMsg(CEventId &Id,const char *p_msgname,const char *p_formatstring);

      static void ReplaceRegisteredMsg(CEventId &Id,const char *p_msgname,const char *p_formatstring);

      void ReplaceLinkedMsg(CEventId &Id,const char *p_msgname,const char *p_formatstring);
      
      static void ReplaceLinkedMsg(CEventHandler &rEventHand, CEventId &Id,const char *p_msgname,
         const char *p_formatstring, RwUInt16 priority = 0x8000);

      static void SetLinkPriority(CEventHandler &rEventHand, CEventId &Id, 
         const char *p_formatString, RwUInt16 priority = 0x8000);

      static void DeleteEventHandlers(const CMsg *p_Msg);

      static void DeleteEventHandlers(const CEventId &Id);

      static void RegisterToDeleteEventHandlers(const CMsg *p_Msg);

      static void RegisterToDeleteEventHandlers(const CEventId &Id);

      static RwBool LinkMsgToEventHandler(
                     CEventHandler *pCEventHandler,
                     CEventId &Id,
                     const char *p_formatstring = 0,
                     RwUInt16 priority  = 0x8000);
  
      void LinkMsg(CEventId &Id,const char *p_formatstring = 0,RwUInt16 priority  = 0x8000);

      RwBool IsLinked(CEventId &Id);

      RwInt32 GetLinkedPriority(CEventId &Id);

      static void Delete(CEventHandler *pCEventHandler);

      void Delete(void) {Delete(this);}

      void UnLinkMsg(CEventId &Id);

      /**
      *
      *  \ref UnRegisterForAutoDelete Removes the CEventHandler from the list of
      *       CEventHandlers to be deleted when 
      *       CEventHandler::DeleteEventHandlers() is called.
      *
      */

      void UnRegisterForAutoDelete() { UnLinkMsg(iMsgDeleteEventHandler); }

      /**
      *
      *  \ref RegisterToAutoDelete 
      *
      *       Registers a CEventHandler with the auto delete event 
      *       \ref iMsgDeleteEventHandler. All of the event handlers
      *       registered with this event can be destroyed at the end of a level
      *       when CEventHandler::DeleteEventHandlers (iMsgDeleteEventHandler)
      *       is called.
      *
      */

      void RegisterForAutoDelete() { LinkMsg(iMsgDeleteEventHandler); }

      static void ProcessPendingUnlinks(CLinkedMsg *pErm);

      void SendMsgToEventHandler(CMsg &p_Msg,CEventHandler *pCEventHandler);

      void SendMsgToEventHandler(const CEventId &Id,CEventHandler *pCEventHandler);

   private:

      CEventHandler(const CEventHandler& rhs);               // Disable copy constructor 

      CEventHandler &operator = (const CEventHandler& rhs);  // Disable copy operator,

      // Following section is used to visualize events.
      //
#ifdef RWS_EVENTVISUALIZATION

   public:

      CEventVisualization *Get_m_pCEventVisualization(void) {return m_pCEventVisualization;}

      void Set_m_pCEventVisualization(CEventVisualization *pCEventVisualization)
         {m_pCEventVisualization = pCEventVisualization;}


   protected:

      /**
      *
      *  \ref m_pCEventVisualization Pointer to an event Visualization class, provides methods
      *  for extracting the 3D position or the event handler.
      *
      *  \see CEventHandler constructor
      *
      *  \note m_pCEventVisualization is only available if RWS_EVENTVISUALIZATION is defined
      */

      CEventVisualization *m_pCEventVisualization;

   protected:

      RwBool _SendMsg(CMsg &p_Msg);

      /**
      *
      *  Broadcast event plus data.
      *
      *  \param p_Msg A reference to a CMsg class containing the Id of the event and any additional data.
      *
      *  \return true if any event handlers are linked to the event, otherwise false.
      *
      */
      RwBool SendMsg(CMsg &p_Msg) 
      {
         if (p_Msg.Id != ERROR_NOT_A_VALID_MESSAGE_ID && p_Msg.Id->Erm.p_next)
         {
            return _SendMsg(p_Msg);
         }
         else
         {
            return false;
         }
      }

      /**
      *
      *  Broadcast event. The data field of the sent message is set to 0.
      *
      *  \param Id A reference to a CEventId class containing the Id of the event.
      *
      *  \return true if any event handlers are linked to the event, otherwise false.
      *
      */
      RwBool SendMsg(const CEventId &Id)
      {
         if (Id != ERROR_NOT_A_VALID_MESSAGE_ID && Id->Erm.p_next)
         {
            CMsg iMsg(Id,0);

            return _SendMsg (iMsg);
         }
         else
         {
            return false;
         }
      }
#endif

   public:

      RwBool m_Active; /**< Used to block a CEventHandler from receiving any further
                            events after Delete has been called. */

   protected:
   } ;


   namespace RpAtomicToEventHandler
   {
      /**
      *
      * Plugin for associating a CEventHandler with an RpAtomic
      *
      */
      RwBool PluginAttach();

      void SetEventHandler( RpAtomic& atomic, CEventHandler* pEventHandler );

      CEventHandler* GetEventHandler( const RpAtomic& atomic );
      
   }
}

// Macro for easy use of event Send (by agebrak 2007.10.29)
#define SEND_MSG(event, data)   \
    RWS::CMsg msg; \
    msg.Id = event;\
    msg.pData = reinterpret_cast<void*>(data);\
    _SendMsg(msg);

#endif
