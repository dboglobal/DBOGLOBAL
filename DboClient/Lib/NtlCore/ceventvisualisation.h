/*****************************************************************************
*
* File :     CEventVisualization.h
*
* Abstract : Debugging system for visualizing events.
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

#ifdef RWS_EVENTVISUALIZATION

#ifndef __CEVENTVISUALIZATION_H__
#define __CEVENTVISUALIZATION_H__

namespace RWS
{
   class CRegisteredMsgs;
   class EventData;
   class CMsg;

   /**
   *
   *  \ingroup CEventVisualization
   *
   *  \page CEventVisualizationOverview Overview
   *
   *  Base class providing a generic interface for event Visualization. The event Visualization
   *  system needs to be able to determine the 3D position of any event handler. Event handlers 
   *  can be used for much more than handling the events of 3D objects, therefore they may or may
   *  not have a 3D position. It is the responsibility of the event handler to provide a CEventVisualization
   *  class in order to obtain this position, this is done in the constructor of the \ref CEventHandler
   *  class.
   *
   *  \note This class is only available if RWS_EVENTVISUALIZATION is defined.
   */

   /**
   *
   *  \ingroup CEventVisualization
   *
   *  Declaration of CEventVisualization object.
   *
   */

   class CEventVisualization
   {
      public:

         virtual ~CEventVisualization(void) {}

         /**
         *  
         *  This virtual function is called by the event handler system in order
         *  to obtain the world co-ordinates of an event handler for use when visualizing events
         *
         *  The default for an event handler is to not have a position and so returns 0.
         *
         *  \return A pointer to a RenderWare Gfx RwV3d object, containing the world co-ordinates used when
         *  visualizing an event sent to or from an event handler, otherwise 0.
         *  false.
         *
         */
         virtual RwV3d *GetWorldPos(void) {return 0;}

         static void EventSend(CRegisteredMsgs &rCRegisteredMsgs);

         static void EventSent(CRegisteredMsgs &rCRegisteredMsgs);

         static void EventReceived(EventData &pEventData);

         static void EventUnRegistered(CRegisteredMsgs &rCRegisteredMsgs);

      private:

         static RwBool m_bVisualizingEvent;  /**< This is used to prevent the event visualization event
                                                visualizing themselves.*/
   };
}

#endif//__CEVENTVISUALIZATION_H__
#endif//RWS_EVENTVISUALIZATION