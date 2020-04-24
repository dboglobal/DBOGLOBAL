/*****************************************************************************
*
* File :     CEventVisualization.cpp
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
#include "precomp_ntlcore.h"
#include <rwcore.h>
#include <rpworld.h>
// Çü¼®
// #include "memoryhandler.h"

#ifdef RWS_EVENTVISUALIZATION

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "CEventVisualisation.h"
#include "ceventhandler.h"
#include "debugmacros.h"

namespace RWS
{
   RwBool CEventVisualization::m_bVisualizingEvent = FALSE;

   /**
   *
   *  If RWS_EVENTVISUALIZATION is defined EventReceived is called just before an event handler receives an event.
   *
   *  \param rEventData Reference to an EventData class containing the source, destination and type of the event.
   *
   *  \note Order of events for visualization send, received, sent.
   */
   void CEventVisualization::EventReceived(EventData &rEventData)
   {
      RWS_FUNCTION("RWS::CEventVisualization::EventReceived");

      if (!m_bVisualizingEvent)
      {
         m_bVisualizingEvent = TRUE;

         CMsg Msg(iMsgEventReceived, &rEventData);

         SendMsg(Msg);

         m_bVisualizingEvent = FALSE;
      }

      RWS_RETURNVOID();
   }

   /**
   *
   *  if RWS_EVENTVISUALIZATION is defined EventSend is called just once for each event just before it is sent, as
   *  events are received EventReceived is called.
   *
   *  \param rCRegisteredMsgs Reference to a CRegisteredMsgs class.
   *
   *  \note Order of events for visualization send, received, sent.
   */
   void CEventVisualization::EventSend(CRegisteredMsgs &rCRegisteredMsgs)
   {
      RWS_FUNCTION("RWS::CEventVisualization::EventSend");

      if (!m_bVisualizingEvent)
      {
         m_bVisualizingEvent = TRUE;

         CMsg Msg(iMsgEventSend, &rCRegisteredMsgs);

         SendMsg(Msg);

         m_bVisualizingEvent = FALSE;
      }

      RWS_RETURNVOID();
   }

   /**
   *
   *  if RWS_EVENTVISUALIZATION is defined EventSent is called just once for each event that has been sent.
   *
   *  \param rCRegisteredMsgs Reference to a CRegisteredMsgs class.
   *
   *  \note Order of events for visualization send, received, sent.
   */
   void CEventVisualization::EventSent(CRegisteredMsgs &rCRegisteredMsgs)
   {
      RWS_FUNCTION("RWS::CEventVisualization::EventSent");

      if (!m_bVisualizingEvent)
      {
         m_bVisualizingEvent = TRUE;

         CMsg Msg(iMsgEventSent, &rCRegisteredMsgs);

         SendMsg(Msg);

         m_bVisualizingEvent = FALSE;
      }

      RWS_RETURNVOID();
   }
   /**
   *
   *  If RWS_EVENTVISUALIZATION is defined EventUnRegistered is called just before an event is finally
   *  unregistered. An event is only finally unregistered when its reference count is zero.
   *  
   *  \param rCRegisteredMsgs Reference to a CRegisteredMsgs class.
   *
   */
   void CEventVisualization::EventUnRegistered(CRegisteredMsgs &rCRegisteredMsgs)
   {
      RWS_FUNCTION("RWS::CEventVisualization::EventUnRegistered");

      if (!m_bVisualizingEvent)
      {
         m_bVisualizingEvent = TRUE;

         CMsg Msg(iMsgEventUnRegistered, &rCRegisteredMsgs);

         SendMsg(Msg);

         m_bVisualizingEvent = FALSE;
      }

      RWS_RETURNVOID();
   }
}//namespace RWS

#endif
