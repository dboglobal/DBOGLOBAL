/*****************************************************************************
*
* File :     Profile.cpp
*
* Abstract : Real time profiling
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
* Copyright (c) 2001 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/

//#include "framework/toolkits/debugtools/debugtools.h"
#include "precomp_ntlcore.h"
#include "debugmacros.h"
#include "memoryhandler.h"

#include "profile.h"
#include <new>

#ifdef RWS_FUNCTIONPROFILE

namespace RWS
{
   namespace FunctionProfile
   {

      RwUInt32   (*g_pTimeFunc)(void) = 0;

      ////////////////////////////////////////////////////////////////////////////////////////////////
      //
      // CProfileNode
      //
      ////////////////////////////////////////////////////////////////////////////////////////////////

      /**
      * 
      * Constructor for a profiler node. A new node is created for each unique function called
      * containing an RWS_FUNCTION macro.
      *
      * \param pName Pointer to a static string which is the name of this profile node.
      * \param pParent Parent pointer.
      *           
      * \note
      * The name is assumed to be a static pointer, only the pointer is stored and compared for
      * efficiency reasons.
      *
      */
      CProfileNode::CProfileNode( RwChar * pName, CProfileNode * pParent ) :
         m_pName( pName ),
         m_pFileName(0),
         m_LineNumber(0),
         m_TotalCalls( 0 ),
         m_TotalTime( 0 ),
         m_StartTime( 0 ),
         m_RecursionCounter( 0 ),
         m_AccumulatedCalls(0),
         m_AccumulatedTime(0.0f),
         m_AccumulatedChildTimes(0.0f),
         m_NumAccumulatedResults(0),
         m_pParent( pParent ),
         m_pChild( 0 ),
         m_pSibling( 0 )
      {
         Reset();
      }

      /**
      * 
      * Search through sub-nodes for a specific node. If the node can't be found create a new node and put
      * it in the profiler tree.  
      *
      * \param pName Static string pointer to the name of the node we are searching for.
      *
      * \return Returns a pointer to a node in the profile tree.
      * 
      * \note
      * All profile names are assumed to be static strings so this function uses pointer compares 
      * to find the named node.
      *
      */
      CProfileNode * CProfileNode::GetSubNode( const RwChar * pName )
      {
         // Try to find this sub-node.
         CProfileNode * pChild = m_pChild;
         
         while ( pChild )
         {
            if ( pChild->m_pName == pName ) 
            {
               return pChild;
            }
            
            pChild = pChild->m_pSibling;
         }         

         // If the number of currently allocated nodes is less than the total amount of space in
         // the memory block create a new Profiler node using 'placement new'.
         if (CProfileManager::m_NumNodesAllocated < CProfileManager::m_NumNodesInBuffer)
         {
            CProfileNode * pNode = 
                        NTL_NEW (CProfileManager::m_NodeBuffer) CProfileNode( const_cast<RwChar*>(pName), this );

            CProfileManager::m_NodeBuffer += sizeof(CProfileNode);
            CProfileManager::m_NumNodesAllocated++;
         
            pNode->m_pSibling = m_pChild;
            m_pChild = pNode;
            return pNode;
         }
         else
         {
            RWS_ASSERTFAIL("You have run out of allocated profiler nodes. You can increase the amount of"
                           " nodes in startup.cpp.  See CProfileManager::OpenProfiler(10000);. The number"
                           " passed into OpenProfiler tells the profiler how many profile nodes to"
                           " allocate.");
         }

         return 0;
      }
      
      /**
      *
      * Reset all profile times for this node.  This function doesn't destroy the node, it only resets 
      * the recorded total times and number of calls.
      *
      */
      void  CProfileNode::Reset( void )
      {
         m_TotalCalls = 0;
         m_TotalTime = 0.0f;
         
         if ( m_pChild ) 
         {
            m_pChild->Reset();
         }
         
         if ( m_pSibling ) 
         {
            m_pSibling->Reset();
         }
      }

      /**
      *
      * When a function encounters an RWS_FUNCTION macro this function will get called and log the starting
      * time of that function.
      *
      * \param pFilename The filename of the source file where the profiled function comes from.
      * \param Linenumber The line number of the function in the source code.
      *
      */
      void  CProfileNode::Call( const char *pFilename, const int Linenumber )
      {
         m_TotalCalls++;
         
         if (m_RecursionCounter++ == 0) 
         {
            m_StartTime = g_pTimeFunc();

         }

         m_pFileName = const_cast<RwChar*>(pFilename);
         m_LineNumber = Linenumber;
      }
      
      /**
      *
      * When a function that is being profiled, returns, this function will get called to calculate the
      * amount of time spent in that function.
      *
      * \return Return FALSE if a timer function has not been registered with the profiler.
      *
      */
      RwBool   CProfileNode::Return( void )
      {
         if (g_pTimeFunc)
         {
            if ( --m_RecursionCounter == 0 && m_TotalCalls != 0 ) 
            { 
               RwUInt32 Time = g_pTimeFunc();

               Time -= m_StartTime;
               m_TotalTime += static_cast<RwReal>(Time) / 1000.0f;
            }       
         
            return ( m_RecursionCounter == 0 );
         }
         else
            return FALSE;
      }
    
      ////////////////////////////////////////////////////////////////////////////////////////////////
      //
      // CProfileIterator
      //
      ////////////////////////////////////////////////////////////////////////////////////////////////
  
      /**
      *
      * Constructor for a profiler tree iterator. This constructor sets a parent node and stores a pointer 
      * to that parents child.
      *
      * \param pStart A pointer to a profiler node.  This node will get set as the parent for the iterator.
      *
      */
      CProfileIterator::CProfileIterator( CProfileNode * pStart )
      {
         m_pCurrentParent = pStart;
         m_pCurrentChild = m_pCurrentParent->GetChild();
      }
           
      /**
      *
      * In the workspace profiler control, you can view a functions profiler data.  You can also double
      * click on one of these profile functions and it's children will be displayed. This function
      * allows you to enter a profiler node and access its children.
      *
      * \param Index Number of the child to be made a parent.
      *
      * \see EnterParent
      *
      */
      void  CProfileIterator::EnterChild( RwUInt32  Index )
      {        
         m_pCurrentChild = m_pCurrentParent->GetChild();

         while ( (m_pCurrentChild != 0) && (Index != 0) ) 
         {
            Index--;
            m_pCurrentChild = m_pCurrentChild->GetSibling();
         }
         
         if ( m_pCurrentChild != 0 ) 
         {
            m_pCurrentParent = m_pCurrentChild;
            m_pCurrentChild = m_pCurrentParent->GetChild();
         }

         _SendProfileNewChildFlag();
      }
      
      /**
      *
      * Make the current nodes parent the new node. When a new node is entered a message has to be sent 
      * to the workspace to let it know new data is on its way.
      *
      * \see EnterChild, 
      *
      */
      void  CProfileIterator::EnterParent( void )
      {
         if ( m_pCurrentParent->GetParent() != 0 ) 
         {
            m_pCurrentParent = m_pCurrentParent->GetParent();
         }

         m_pCurrentChild = m_pCurrentParent->GetChild();
         
         _SendProfileNewChildFlag();
      }

      ////////////////////////////////////////////////////////////////////////////////////////////////
      //
      // CProfileManager
      //
      ////////////////////////////////////////////////////////////////////////////////////////////////

      char            * CProfileManager::m_NodeBuffer = 0; 
      char            * CProfileManager::m_NodeBufferOriginalPtr = 0;
      CProfileNode      CProfileManager::m_Root( "Root", 0 );
      CProfileNode    * CProfileManager::m_pCurrentNode = &CProfileManager::m_Root;
      RwUInt32          CProfileManager::m_FrameCounter = 0;
      RwUInt32          CProfileManager::m_ResetTime = 0;
      CProfileIterator* CProfileManager::pIttr = 0; 
      RwUInt32          CProfileManager::m_NumNodesInBuffer = 0;
      RwUInt32          CProfileManager::m_NumNodesAllocated = 0;

      /**
      *
      * This function allocates the memory buffer used to store the profile node classes.
      *
      * \param NumNodes The number of nodes which will be catered for in the memory buffer allocation. 
      *                 If the number of nodes that need to be created exceeds the size of this buffer,  
      *                 the profiler node class will assert telling you to increase this value.
      *
      * \see CloseProfiler
      *
      */
      void CProfileManager::OpenProfiler (RwUInt32 NumNodes, RwUInt32 (*pTimeFunc)(void))
      {
		 RwInt32 iSize = (RwInt32)sizeof(CProfileNode) * NumNodes;
         m_NodeBuffer = NTL_NEW char[iSize];          

         RWS_ASSERT (m_NodeBuffer, "Memory for the profiler nodes was not allocated.");

         m_NodeBufferOriginalPtr = m_NodeBuffer;
         m_NumNodesInBuffer = NumNodes;

         g_pTimeFunc = pTimeFunc;

         if (pIttr == 0)
         {
            pIttr = GetIterator();
         }

         m_ResetTime = g_pTimeFunc();

         RWS_ASSERT (g_pTimeFunc, "No 'get time' function provided.");
      }

      /**
      *
      * Free up the memory used for the profiler node memory buffer.
      *
      * \see OpenProfiler
      *
      */
      void CProfileManager::CloseProfiler ()
      {
         RWS_ARRAY_DELETE(m_NodeBufferOriginalPtr);

         g_pTimeFunc = 0;
         
         if (pIttr)
         {         
            ReleaseIterator(pIttr);
         }
      }

      /**
      *
      * Begin a named profile. Steps one level deeper into the tree, if a child already exists with
      * the specified name then it accumulates the profiling; otherwise a new child node is added to
      * the profile tree.
      *
      * \param pName      Name of function being profiled.
      * \param pFilename  Name of the file which this function belongs.
      * \param Linenumber Line number were this function appears in the source file.
      *    
      * \note The string used is assumed to be a static string; pointer compares are used throughout  
      *       the profiling code for efficiency.
      *
      * \see StopProfiler
      *
      */         
      void  CProfileManager::StartProfile(   const RwChar * pName,
                                             const char *pFilename,
                                             const int Linenumber )
      {
         // If the timer functions have been registered we can start taking reading.
         if (g_pTimeFunc)
         {
            if (pName != m_pCurrentNode->GetName()) 
            {
               m_pCurrentNode = m_pCurrentNode->GetSubNode( pName );
            } 
      
            m_pCurrentNode->Call(pFilename, Linenumber);
         }

      }

      /**
      *
      * Stop timing and record the results.
      *
      * \see StartProfiler
      *
      */
      void  CProfileManager::StopProfile( void )
      {
         // Return will indicate whether we should back up to our parent (we may
         // be profiling a recursive function).
         if (m_pCurrentNode->Return()) 
         {
            m_pCurrentNode = m_pCurrentNode->GetParent();
         }
      }

      /**
      *
      * Reset the contents of the profiling system.
      *
      * This resets everything except for the tree structure.  All of the timing data is reset. 
      *
      * \see GetTimeSinceReset
      * \see GetFrameCountSinceReset
      *
      */
      void  CProfileManager::Reset( void )
      { 
         m_Root.Reset(); 
         m_FrameCounter = 0;

         m_ResetTime = g_pTimeFunc();
      }

      /**
      *
      * Increment the frame counter.
      *
      */
      void CProfileManager::IncrementFrameCounter( void )
      {
         m_FrameCounter++;
      }

      /**
      *
      * Returns the elapsed time since last reset.
      *
      * \return Returns the elapsed time since last reset.
      *
      * \see GetFrameCountSinceReset
      * \see Reset
      *
      */
      RwReal CProfileManager::GetTimeSinceReset( void )
      {
         RwUInt32 time = g_pTimeFunc();

         time -= m_ResetTime;

         return static_cast<RwReal>(time) / MILLISECONDS_TO_SECONDS;
      }

      /**
      *
      * Returns the frame count since the last reset.
      *
      * \return Returns the frame count since the last reset.
      *
      * \see GetTimeSinceReset
      * \see Reset
      *
      */
      RwUInt32 CProfileManager::GetFrameCountSinceReset( void )    
      { 
         return m_FrameCounter; 
      }

      /**
      *
      * Get a pointer to an iterator starting at the root node.
      *
      * \return Pointer to an iterator.
      *
      * \see ReleaseIterator
      *
      */
      CProfileIterator * CProfileManager::GetIterator( void ) 
      { 
         return NTL_NEW CProfileIterator( &m_Root ); 
      }

      /**
      *
      * Release an iterator.
      *
      * \param pIterator A pointer to an iterator.
      *
      * \see GetIterator
      *
      */
      void CProfileManager::ReleaseIterator( CProfileIterator * pIterator ) 
      {  
		NTL_DELETE( pIterator );
      }

      /**
      *
      * Broadcast the current child nodes to the workspace.  
      *
      * \see BufferProfileTimes
      * \see SendAccumulatedData
      *
      */
      void CProfileManager::SendProfileDataToWorkspace( void )
      {
         static RwUInt32 LastTime = g_pTimeFunc();

         // Delta time before frame rate is recalculated.
         //
         static const RwUInt32 DeltaTimeLimit = BROADCASTRATE;
      
         RwUInt32 ThisTime = g_pTimeFunc();
         RwUInt32 DeltaTime = ThisTime - LastTime;
     
         AccumulateProfileTimes();

         // We'll send out the profile data every n seconds.
         if (DeltaTime > DeltaTimeLimit)
         {
            SendAccumulatedData();

            LastTime = ThisTime;
         }
                 
         IncrementFrameCounter();
      }

      /**
      *
      * Calculates the total time for a nodes children. This is useful as you can subtract
      * this time from the total time of it's parent and see how much time was spent in the
      * parent only.
      *
      * \return Returns the time spent in the current child child.
      *
      */
      RwReal CProfileManager::TotalChildTimes( void )
      {
         RwReal Child = 0;

         // Get child of current child
         CProfileNode *pTempnode = pIttr->m_pCurrentChild->GetChild();
         if (pTempnode)
         {
            Child = pTempnode->GetTotalTime();
         }

         // Iterate through siblings of this child and total up times.
         while (!pTempnode == 0)
         {
            pTempnode = pTempnode->GetSibling();
            
            if (pTempnode)
            {
              Child = Child + pTempnode->GetTotalTime();
            }
         }

         return Child;
      }

      /**
      *
      * Accumulate the profiler times until they are ready to be sent to the workspace.
      *
      * \see SendProfileDataToWorkspace
      * \see SendAccumulatedData
      *
      */
      void CProfileManager::AccumulateProfileTimes( void )
      {
         // Get access to the first child of the current parent.
         pIttr->First();
         
         // Iterate through the siblings of the current child.
         while (!pIttr->IsDone())
         {       
            RwUInt32 AccumulatedCalls = pIttr->m_pCurrentChild->GetNumAccumulatedCalls();
            AccumulatedCalls += pIttr->GetCurrentTotalCalls();
            pIttr->m_pCurrentChild->SetNumAccumulatedCalls(AccumulatedCalls);

            RwReal AccumulatedTime = pIttr->m_pCurrentChild->GetNumAccumulatedTime();
            AccumulatedTime += pIttr->GetCurrentTotalTime();
            pIttr->m_pCurrentChild->SetNumAccumulatedTime(AccumulatedTime);

            RwReal AccumulatedChildTime = pIttr->m_pCurrentChild->GetNumAccumulatedChildTime();
            AccumulatedChildTime += TotalChildTimes();
            pIttr->m_pCurrentChild->SetNumAccumulatedChildTime(AccumulatedChildTime);

            RwUInt32 AccumulatedReuslts = pIttr->m_pCurrentChild->GetNumAccumulatedResults();
            AccumulatedReuslts++;
            pIttr->m_pCurrentChild->SetNumAccumulatedResults( AccumulatedReuslts );

            pIttr->Next();
         }
      }

      /**
      *
      * Send the accumulated profiler data to the workspace.
      *
      * \param ThisTime. Time elapsed since last packet was sent across the network.
      *
      * \see SendProfileDataToWorkspace
      * \see AccumulateProfileTimes
      *
      */
      void CProfileManager::SendAccumulatedData( void )
      {
         RwUInt32 AccumulateCalls = 0;
         RwReal   AccumulateTime = 0.0f;
         RwReal   AccumulateChild = 0.0f;
         RwUInt32 Count = 0;

         // Get access to the first child of the current parent.
         pIttr->First();

         // Iterate through the siblings of the current child.
         while (!pIttr->IsDone())
         {           
            if (pIttr->m_pCurrentChild->GetNumAccumulatedResults() > 0)
            {
               AccumulateCalls = pIttr->m_pCurrentChild->GetNumAccumulatedCalls() / 
                                 pIttr->m_pCurrentChild->GetNumAccumulatedResults();

               AccumulateTime  = pIttr->m_pCurrentChild->GetNumAccumulatedTime() / 
                                 static_cast<RwReal>(pIttr->m_pCurrentChild->GetNumAccumulatedResults());

               AccumulateChild = pIttr->m_pCurrentChild->GetNumAccumulatedChildTime() / 
                                 static_cast<RwReal>(pIttr->m_pCurrentChild->GetNumAccumulatedResults());
            }
 
            _SendProfileDataBuffer(Count,
                                   pIttr->m_pCurrentChild->GetChild() ?1:0,
                                   GetFrameCountSinceReset(),
                                   GetTimeSinceReset(),
                                   pIttr->GetCurrentParentName(),
                                   pIttr->GetCurrentName(), 
                                   AccumulateCalls,
                                   AccumulateTime,
                                   pIttr->GetCurrentFileName(),
                                   pIttr->GetLineNumber(),
                                   AccumulateChild);

            pIttr->m_pCurrentChild->SetNumAccumulatedCalls(0);
            pIttr->m_pCurrentChild->SetNumAccumulatedTime(0.0f);
            pIttr->m_pCurrentChild->SetNumAccumulatedChildTime(0.0f); 
            pIttr->m_pCurrentChild->SetNumAccumulatedResults(0); 
            
            Count++;

            pIttr->Next();
         }
      }

   } // Profile
} // RWS

#endif // RWS_FUNCTIONPROFILE


